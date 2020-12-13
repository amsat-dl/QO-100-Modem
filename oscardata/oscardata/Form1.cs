/*
* High Speed modem to transfer data in a 2,7kHz SSB channel
* =========================================================
* Author: DJ0ABR
*
*   (c) DJ0ABR
*   www.dj0abr.de
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
* 
*/

using System;
using System.Windows.Forms;
using System.Drawing;
using System.Text;
using System.IO;
using System.Drawing.Drawing2D;
using System.Diagnostics;
using System.Threading;
using oscardata.Properties;
using System.Reflection;

namespace oscardata
{
    public partial class Form1 : Form
    {
        Imagehandler ih = new Imagehandler();
        int txcommand = 0;      // commands what to send
        Byte frameinfo = (Byte)statics.FirstFrame;
        String TXfilename;
        int rxbytecounter = 0;
        String old_tsip = "";
        bool modemrunning = false;
        receivefile recfile = new receivefile();
        int last_initAudioStatus;
        int last_initVoiceStatus;

        public Form1()
        {
            // init GUI
            InitializeComponent();

            // needed for ARM mono, which cannot load a picbox directly from file
            var bmp = new Bitmap(Resources.hintergrundxcf);
            pictureBox_rximage.BackgroundImage = bmp;

            bmp = new Bitmap(Resources.hintergrundxcf);
            pictureBox_tximage.BackgroundImage = bmp;

            // if this program was started from another loacation
            // set the working directory to the path of the .exe file
            // so it can find hsmodem(.exe)
            try
            {
                String s = Assembly.GetExecutingAssembly().Location;
                s = Path.GetDirectoryName(s);
                Directory.SetCurrentDirectory(s);
                Console.WriteLine("working path: " + s);
            }
            catch (Exception e)
            {
                Console.WriteLine("cannot set working path: " + e.ToString());
            }

            // test OS type
            OperatingSystem osversion = System.Environment.OSVersion;
            statics.OSversion = osversion.Platform.ToString();
            if (osversion.VersionString.Contains("indow"))
                statics.ostype = 0; // Win$
            else
                statics.ostype = 1; // Linux

            statics.CreateAllDirs();

            // set temp paths
            statics.zip_TXtempfilename = statics.addTmpPath(statics.zip_TXtempfilename);
            statics.zip_RXtempfilename = statics.addTmpPath(statics.zip_RXtempfilename);
            statics.jpg_tempfilename = statics.addTmpPath(statics.jpg_tempfilename);

            load_Setup();
            cb_language_SelectedIndexChanged(null, null);

            if (cb_autostart.Checked)
            {
                // start hsmodem (.exe)
                modemrunning = statics.StartHSmodem();
            }

            checkBox_small_CheckedChanged(null, null);

            // init speed
            comboBox1_SelectedIndexChanged(null, null);

            // create Udp Communication ports and init UDP system
            Udp.InitUdp();
            ArraySend.ArraySendInit();

            // enable processing
            timer_udpTX.Enabled = true;
            timer_udprx.Enabled = true;
            timer_searchmodem.Enabled = true;
        }

        // TX timer
        int loopdelay = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            // BER testdata
            if (txcommand == statics.BERtest)
            {
                if (Udp.GetBufferCount() < 2)
                {
                    Byte[] txdata = new byte[statics.PayloadLen + 2];

                    txdata[0] = (Byte)statics.BERtest; // BER Test Marker
                    txdata[1] = frameinfo;

                    Byte tb = (Byte)'A';
                    for (int i = 2; i < txdata.Length; i++)
                    {
                        txdata[i] = tb;
                        tb++;
                        if (tb == 'z') tb = (Byte)'A';
                    }

                    // and transmit it
                    Udp.UdpSendData(txdata);

                    frameinfo = (Byte)statics.NextFrame;
                }
            }

            if (ArraySend.getSending())
            {
                button_loadimage.Enabled = false;
                button_sendimage.Enabled = false;
            }
            else
            {
                button_loadimage.Enabled = true;
                if (TXimagefilename != "")
                    button_sendimage.Enabled = true;
                else
                    button_sendimage.Enabled = false;
            }

            if (TXfoldername == "" || lastFullName == "")
                cb_loop.Enabled = false;
            else
                cb_loop.Enabled = true;

            ShowTXstatus();

            if (txcommand == statics.Image)
            {
                // if "loop" is selected send the next image in folder
                if (cb_loop.Checked)
                {
                    // check if we are ready with any transmission
                    if (ArraySend.getSending() == false)
                    {
                        // this timer runs with 10ms
                        // after an image was finished, wait before starting the new one
                        // this helps cleaning any buffer
                        int spacetime = 20000;  // ms
                        label_nextimage.Text = langstr[0] + ((spacetime / timer_udpTX.Interval - loopdelay) / 10).ToString() + " s";
                        if (++loopdelay > (spacetime / timer_udpTX.Interval))
                        {
                            // start sending a new picture
                            startNextImage();
                        }
                    }
                    else
                    {
                        loopdelay = 0;
                        label_nextimage.Text = langstr[1];
                    }
                }
                else
                    label_nextimage.Text = "";
            }
            else
                label_nextimage.Text = "";

            if (ts_ip.Text.Contains("?") || ts_ip.Text.Contains("1.2.3.4") || old_tsip != statics.ModemIP)
            {
                if (statics.ModemIP == "1.2.3.4")
                {
                    ts_ip.Text = "Modem-IP: ?";
                    ts_ip.ForeColor = Color.Red;
                }
                else
                {
                    ts_ip.Text = "Modem-IP: " + statics.ModemIP;
                    ts_ip.ForeColor = Color.Black;
                    old_tsip = statics.ModemIP;
                    comboBox1_SelectedIndexChanged(null, null); // send speed to modem
                }
            }

            if (statics.GotAudioDevices == 1)
            {
                statics.GotAudioDevices = 2;
                // populate combo boxes
                cb_audioPB.BeginUpdate();
                cb_audioPB.Items.Clear();
                cb_loudspeaker.BeginUpdate();
                cb_loudspeaker.Items.Clear();
                foreach (String s in statics.AudioPBdevs)
                {
                    if (s.Length > 1)
                    {
                        cb_audioPB.Items.Add(s);
                        cb_loudspeaker.Items.Add(s);
                    }
                }
                cb_loudspeaker.EndUpdate();
                cb_audioPB.EndUpdate();
                // check if displayed text is available in the item list
                findDevice(cb_loudspeaker);
                findDevice(cb_audioPB);

                cb_audioCAP.BeginUpdate();
                cb_audioCAP.Items.Clear();
                cb_mic.BeginUpdate();
                cb_mic.Items.Clear();
                foreach (String s in statics.AudioCAPdevs)
                {
                    if (s.Length > 1)
                    {
                        cb_audioCAP.Items.Add(s);
                        cb_mic.Items.Add(s);
                    }
                }
                cb_mic.EndUpdate();
                cb_audioCAP.EndUpdate();
                findDevice(cb_mic);
                findDevice(cb_audioCAP);
            }

            if (setPBvolume >= 0)
            {
                Byte[] txdata = new byte[2];
                txdata[0] = (Byte)statics.SetPBvolume;
                txdata[1] = (Byte)setPBvolume;
                Udp.UdpSendCtrl(txdata);
                setPBvolume = -1;
            }

            if (setCAPvolume != -1)
            {
                Byte[] txdata = new byte[2];
                txdata[0] = (Byte)statics.SetCAPvolume;
                txdata[1] = (Byte)setCAPvolume;
                Udp.UdpSendCtrl(txdata);
                setCAPvolume = -1;
            }

            if (setLSvolume >= 0)
            {
                Byte[] txdata = new byte[2];
                txdata[0] = (Byte)statics.SetLSvolume;
                txdata[1] = (Byte)setLSvolume;
                Udp.UdpSendCtrl(txdata);
                setLSvolume = -1;
            }

            if (setMICvolume != -1)
            {
                Byte[] txdata = new byte[2];
                txdata[0] = (Byte)statics.SetMICvolume;
                txdata[1] = (Byte)setMICvolume;
                Udp.UdpSendCtrl(txdata);
                setMICvolume = -1;
            }

            if (last_initAudioStatus != statics.initAudioStatus)
            {
                if ((statics.initAudioStatus & 1) == 1)
                    pb_audioPBstatus.BackgroundImage = Properties.Resources.ok;
                else
                    pb_audioPBstatus.BackgroundImage = Properties.Resources.fail;

                if ((statics.initAudioStatus & 2) == 2)
                    pb_audioCAPstatus.BackgroundImage = Properties.Resources.ok;
                else
                    pb_audioCAPstatus.BackgroundImage = Properties.Resources.fail;

                last_initAudioStatus = statics.initAudioStatus;
            }

            if (last_initVoiceStatus != statics.initVoiceStatus)
            {
                if ((statics.initVoiceStatus & 1) == 1)
                    pb_voicePBstatus.BackgroundImage = Properties.Resources.ok;
                else
                    pb_voicePBstatus.BackgroundImage = Properties.Resources.fail;

                if ((statics.initVoiceStatus & 2) == 2)
                    pb_voiceCAPstatus.BackgroundImage = Properties.Resources.ok;
                else
                    pb_voiceCAPstatus.BackgroundImage = Properties.Resources.fail;

                last_initVoiceStatus = statics.initVoiceStatus;
            }
        }

        // correct entries in the Audio Device Comboboxes if Devices have changed
        void findDevice(ComboBox cb)
        {
            int pos = -1;

            if (cb.Text.Length >= 4)
            {
                // Device Name starts at Index 3 in the string
                String dn = cb.Text.Substring(3);
                int anz = cb.Items.Count;
                for (int i = 0; i < anz; i++)
                {
                    String name = cb.Items[i].ToString();
                    name = name.Substring(3);
                    if (dn == name)
                    {
                        pos = i;
                        break;
                    }
                }
            }

            if (pos == -1)
            {
                // not available, reset to first item which usually is Default
                if (cb.Items.Count == 0)
                    cb.Text = "no sound devices available";
                else
                    cb.Text = cb.Items[0].ToString();
            }
            else
                cb.Text = cb.Items[pos].ToString();
        }


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            save_Setup();

            if (cb_autostart.Checked)
            {
                killLocalModem();
            }
            // exit the threads
            statics.running = false;
            Udp.Close();
        }

        void killLocalModem()
        {
            // tell hsmodem to terminate itself
            Byte[] txdata = new byte[1];
            txdata[0] = (Byte)statics.terminate;
            Udp.UdpSendCtrl(txdata);

            Thread.Sleep(250);

            // if that did not work, kill it directly
            if (statics.ostype == 0)
            {
                int to = 0;
                while (statics.isProcRunning("hsmodem.exe"))
                {
                    Thread.Sleep(250);
                    // tell hsmodem to terminate itself
                    Udp.UdpSendCtrl(txdata);
                    if (++to >= 10) break;  // give up after 2,5s
                }

                if (to >= 10)
                    statics.killall("hsmodem.exe");
            }
            else
            {
                Thread.Sleep(250);
                statics.killall("hsmodem");
            }
        }

        int lasttype = -1;
        void showType(int rxtype)
        {
            if(rxtype != lasttype)
            {
                if (rxtype == statics.Image) toolStrip_Type.Text = "RX: IMAGE";
                if (rxtype == statics.AsciiFile) toolStrip_Type.Text = "RX: TEXT";
                if (rxtype == statics.HTMLFile) toolStrip_Type.Text = "RX: HTML/Web";
                if (rxtype == statics.BinaryFile) toolStrip_Type.Text = "RX: BINARY";
                if (rxtype == statics.BERtest) toolStrip_Type.Text = "RX: BER TEST";
                if (rxtype == statics.Audio) toolStrip_Type.Text = "RX: DV-Audio";
                if (rxtype == -1) toolStrip_Type.Text = "no RX";
                lasttype = rxtype;
            }
        }

        // RX timer
        int rxstat = 0;
        int speed;
        private void timer_udprx_Tick(object sender, EventArgs e)
        {
            while (true)
            {
                Byte[] rxd = Udp.UdpReceive();
                if (rxd == null) break;

                // these status information are added by the unpack routine
                int rxtype = rxd[0];
                int rxfrmnum = rxd[1];
                rxfrmnum <<= 8;
                rxfrmnum += rxd[2];
                int minfo = rxd[3];
                rxstat = rxd[4];
                speed = rxd[5];
                speed <<= 8;
                speed += rxd[6];
                int dummy3 = rxd[7];
                int dummy4 = rxd[8];
                int dummy5 = rxd[9];

                rxbytecounter = statics.UdpBlocklen * rxfrmnum;

                Byte[] rxdata = new byte[rxd.Length - 10];
                Array.Copy(rxd, 10, rxdata, 0, rxd.Length - 10);

                showType(rxtype);

                //Console.WriteLine("minfo:" + minfo + " data:" + rxdata[0].ToString("X2") + " " + rxdata[1].ToString("X2"));

                // ========= receive file ==========
                // handle file receive
                if (rxtype == statics.Image)
                {
                    if (recfile.receive(rxd) == 1)
                    {
                        if (recfile.filename != null && recfile.filename.Length > 0 && minfo != statics.FirstFrame)
                        {
                            // reception complete, show stored file
                            Console.WriteLine("load " + recfile.filename);
                            pictureBox_rximage.BackgroundImage = Image.FromFile(recfile.filename);
                            pictureBox_rximage.Invalidate();
                        }
                        if (recfile.pbmp != null)
                        {
                            // in case we can display portions of an image return this portion
                            try
                            {
                                pictureBox_rximage.BackgroundImage = recfile.pbmp;
                            }
                            catch { }
                        }
                    }
                }

                if (rxtype == statics.AsciiFile)
                {
                    int fret = recfile.receive(rxd);
                    if (fret == 1)
                    {
                        // ASCII file received, show in window
                        String serg = File.ReadAllText(recfile.filename);
                        printText(rtb_RXfile, serg);
                    }
                    if (fret == -5)
                        printBadBlocks();
                }

                if (rxtype == statics.HTMLFile)
                {
                    int fret = recfile.receive(rxd);
                    if (fret == 1)
                    {
                        // HTML file received, show in window
                        String serg = File.ReadAllText(recfile.filename);
                        printText(rtb_RXfile, serg);
                        // and show in browser
                        OpenUrl(recfile.filename);
                    }
                    if (fret == -5)
                        printBadBlocks();
                }

                if (rxtype == statics.BinaryFile)
                {
                    int fret = recfile.receive(rxd);
                    if (fret == 1)
                    {
                        // Binary file received, show statistics in window
                        try
                        {
                            printText(rtb_RXfile, langstr[2]);
                            printText(rtb_RXfile, "--------------------\r\n\r\n");
                            printText(rtb_RXfile, langstr[3] + ((int)recfile.runtime.TotalSeconds).ToString() + " seconds" + "\r\n\r\n");
                            printText(rtb_RXfile, langstr[4] + ((int)(recfile.filesize * 8 / recfile.runtime.TotalSeconds)).ToString() + " bit/s" + "\r\n\r\n");
                            printText(rtb_RXfile, langstr[5] + recfile.filesize + " byte\r\n\r\n");
                            printText(rtb_RXfile, langstr[6] + recfile.filename + "\r\n\r\n");
                            if (recfile.filename.Length <= 1)
                            {
                                printText(rtb_RXfile, langstr[7]);
                            }
                        }
                        catch 
                        {
                            printText(rtb_RXfile, "RX error\r\n");
                        }
                    }
                    if (fret == -5)
                        printBadBlocks();
                }

                // ===== BER Test ================================================
                if (rxtype == statics.BERtest)
                {
                    BERcheck(rxdata, rxfrmnum,minfo);
                }

                ShowStatus(rxtype, minfo);
            }
        }

        void printBadBlocks()
        {
            rtb_RXfile.Text = "";
            printText(rtb_RXfile, langstr[2]);
            printText(rtb_RXfile, "--------------------\r\n\r\n");
            printText(rtb_RXfile, langstr[31] + "\r\n\r\n");

            int[] d = new int[2];
            recfile.oldblockinfo(d);
            int failed = d[0] - d[1];

            String s = "\n" + langstr[25] +
                "-------------------\n" +
                "total : " + (d[0]+1) + "\n" +
                langstr[26] + (d[1]+1) + "\n" +
                langstr[27] + failed + "\n";
            printText(rtb_RXfile, s + "\r\n\r\n");
            printText(rtb_RXfile, langstr[32] + ": " + recfile.missingBlockString());
        }

        private void OpenUrl(string url)
        {
            try
            {
                Process.Start(url);
            }
            catch
            {
                // hack because of this: https://github.com/dotnet/corefx/issues/10361
                if (statics.ostype == 0)
                {
                    url = url.Replace("&", "^&");
                    Process.Start(new ProcessStartInfo("cmd", $"/c start {url}") { CreateNoWindow = true });
                }
                else
                {
                    Process.Start("xdg-open", url);
                }
            }
        }

        private void timer_qpsk_Tick(object sender, EventArgs e)
        {
            if(Udp.IQavail())
                panel_constel.Invalidate();

            panel_txspectrum.Invalidate();

            // show RX and TX Buffer Usage
            if (statics.PBfifousage < progressBar_fifo.Minimum)         progressBar_fifo.Value = progressBar_fifo.Minimum;
            else if (statics.PBfifousage >= progressBar_fifo.Maximum)   progressBar_fifo.Value = progressBar_fifo.Maximum-1;
            else                                                        progressBar_fifo.Value = statics.PBfifousage;

            if (statics.CAPfifousage < progressBar_capfifo.Minimum) progressBar_capfifo.Value = progressBar_capfifo.Minimum;
            else if (statics.CAPfifousage >= progressBar_capfifo.Maximum) progressBar_capfifo.Value = progressBar_capfifo.Maximum - 1;
            else progressBar_capfifo.Value = statics.CAPfifousage;
            if (statics.CAPfifousage > 50) progressBar_capfifo.ForeColor = Color.Red; else progressBar_capfifo.ForeColor = Color.Green;

            // Show RX Status LEDs
            if (statics.RXlevelDetected == 1)
            {
                pb_rxsignal.BackgroundImage = Resources.greenmarker;
            }
            else
            {
                pb_rxsignal.BackgroundImage = Resources.redmarker;
                showType(-1);
            }
            if (statics.RXinSync == 1 && statics.RXlevelDetected == 1) pb_rxsync.BackgroundImage = Resources.greenmarker; else pb_rxsync.BackgroundImage = Resources.redmarker;

            // update rx,tx level progress bar
            int factor = 1;
            int addf = 80;
            double vl100 = (100 / addf) + Math.Log10(100 + factor);
            double vl = ((double)statics.maxRXlevel / addf) + Math.Log10((double)statics.maxRXlevel + factor);
            vl = vl * 100 / vl100;
            if (vl > 99) vl = 99;
            if (vl < 1) vl = 1;
            vu_cap.Value = (int)vl;
            if (vl < 20 || vl > 85)
                vu_cap.ForeColor = Color.Red;
            else
                vu_cap.ForeColor = Color.Yellow;

            addf = 80;
            vl = ((double)statics.maxTXlevel / addf) + Math.Log10((double)statics.maxTXlevel + factor);
            vl100 = (100 / addf) + Math.Log10(100 + factor);
            vl = vl * 100 / vl100;
            if (vl > 99) vl = 99;
            if (vl < 1) vl = 1;
            vu_pb.Value = (int)vl;
            if(vl <20 || vl > 85)
                vu_pb.ForeColor = Color.Red;
            else
                vu_pb.ForeColor = Color.Yellow;
        }

        private void panel_constel_Paint(object sender, PaintEventArgs e)
        {
            Bitmap bm = Udp.UdpBitmap();
            if (bm != null)
            {
                Pen pen = new Pen(Brushes.LightGray);
                e.Graphics.DrawEllipse(pen, 0, 0, panel_constel.Size.Width - 1, panel_constel.Size.Height - 1);
                e.Graphics.DrawLine(pen, panel_constel.Size.Width / 2, 0, panel_constel.Size.Width / 2, panel_constel.Size.Height);
                e.Graphics.DrawLine(pen, 0, panel_constel.Size.Height / 2, panel_constel.Size.Width, panel_constel.Size.Height / 2);

                e.Graphics.DrawImage(bm, 0, 0);
                bm.Dispose();
            }
        }

        static Brush brred = new SolidBrush(Color.FromArgb(255, (byte)255, (byte)220, (byte)220));
        static Brush brgreen = new SolidBrush(Color.FromArgb(255, (byte)240, (byte)255, (byte)240));
        static Brush brgray = new SolidBrush(Color.FromArgb(255, (byte)220, (byte)220, (byte)220));
        static Pen pen = new Pen(Brushes.Black);
        static Pen penblue = new Pen(Brushes.Blue, 1);
        static Pen pengrey = new Pen(brgray, 1);
        Font fnt = new Font("Verdana", 8.0f);
        Font smallfnt = new Font("Verdana", 6.0f);

        private void panel_txspectrum_Paint(object sender, PaintEventArgs e)
        {
            int miny = 200;
            int maxy = 2800;

            // horizontal level markers
            Point ps = GetFFTPos(0, 0);
            Point pe = GetFFTPos(maxxval, maxyval);
            int pw = pe.X - ps.X;
            int ph = ps.Y - pe.Y;
            e.Graphics.FillRectangle(brred, ps.X, pe.Y, pw, ph);

            ps = GetFFTPos(miny/10, 700);
            pe = GetFFTPos(maxy/10, 2300);
            pw = pe.X - ps.X;
            ph = ps.Y - pe.Y;
            e.Graphics.FillRectangle(brgreen, ps.X, pe.Y, pw, ph);

            // Coordinates
            e.Graphics.DrawLine(pen, GetFFTPos(0, 0), GetFFTPos(maxxval, 0));
            e.Graphics.DrawLine(pen, GetFFTPos(0, 0), GetFFTPos(0, maxyval));

            // vertical frequency markers for 2.7kHz
            for (int i = miny; i <= maxy; i+=100)
            {
                e.Graphics.DrawLine(pengrey, GetFFTPos(i / 10, 0), GetFFTPos(i / 10, maxyval));
            }

            // Title
            e.Graphics.DrawString(langstr[8], fnt, Brushes.Black, GetFFTPos(110, 3000));
            e.Graphics.DrawString(miny.ToString() + " Hz", smallfnt, Brushes.Black, GetFFTPos(5, 2800));
            e.Graphics.DrawString("1500 Hz", smallfnt, Brushes.Black, GetFFTPos(138, 680));
            e.Graphics.DrawString(maxy.ToString() + " Hz", smallfnt, Brushes.Black, GetFFTPos(270, 2800));

            e.Graphics.DrawString(langstr[9], smallfnt, Brushes.Black, GetFFTPos(290, 1000));
            e.Graphics.DrawString("max", smallfnt, Brushes.Black, GetFFTPos(290, 2450));

            while (true)
            {
                UInt16[] da = Udp.UdpGetFFT();
                if (da == null) break;
                if (da.Length < maxxval) return;
                Fftmean(da);
            }

            // da are the FFT data
            // from 0 Hz to 4410 Hz with a resolution of 10 Hz
            // so we get 441 values
            // there may be 442, just ignore the last one
            GraphicsPath gp = new GraphicsPath();

            // calculate mean value and calc mean value over all values
            UInt16[] su = new UInt16[maxxval+1];
            for (int i = 0; i < maxxval; i++)
            {
                su[i] = 0;
                for(int j=0; j< meansize; j++)
                    su[i] += dam[j, i];
                su[i] /= (UInt16)meansize;
            }

            // scale and X-mean
            int lastu = 0;
            for (int i = 0; i < maxxval; i++)
            {
                UInt16 u = 0;
                if (i >= 2 && i < maxxval - 2)
                    u = (UInt16)((su[i - 2] + su[i - 1] + su[i] + su[i + 1] + su[i + 2]) / 5);
                else
                    u = su[i];

                if (i < 3) u = 0;

                u *= 10;
                gp.AddLine(GetFFTPos(i, lastu), GetFFTPos(i + 1, u));
                lastu = u;
            }

            e.Graphics.DrawPath(penblue, gp);
        }

        private UInt16[,] dam = new UInt16[meansize, maxxval];

        private void Fftmean(UInt16[] v)
        {
            for (int sh = meansize - 1; sh > 0; sh--)
                for (int i = 0; i < maxxval; i++)
                    dam[sh, i] = dam[sh - 1, i];

            for (int i = 0; i < maxxval; i++)
                dam[0, i] = v[i];
        }

        readonly static int meansize = 6;//0;
        readonly static int maxxval = (statics.real_datarate / 10) * 6 / 10;
        readonly int maxyval = 3000;

        private Point GetFFTPos(int x, int y)
        {
            int leftMargin = 2;
            int rightMargin = 2;
            int topMargin = 2;
            int bottomMargin =2;

            int xsize = panel_txspectrum.Size.Width;
            int newx = (x * (xsize - leftMargin - rightMargin)) / maxxval;
            newx += leftMargin;

            int ysize = panel_txspectrum.Size.Height;
            int newy = (y * (ysize - topMargin - bottomMargin)) / maxyval;
            newy += bottomMargin;
            newy = ysize - newy;

            Point p = new Point(newx, newy);
            return p;
        }

        void printText(RichTextBox rtb, String s)
        {
            AppendTextOnce(rtb, new Font("Courier New", (float)8), Color.Blue, Color.White, s);
        }

        void AppendTextOnce(RichTextBox rtb, Font selfont, Color color, Color bcolor, string text)
        {
            try
            {
                if (text.Contains("\n"))
                {
                    char[] ca = new char[] { '\n', '\r' };

                    text = text.Trim(ca);
                    text += "\n";
                }

                // max. xxx Zeilen, wenn mehr dann lösche älteste
                if (rtb.Lines.Length > 200)
                {
                    rtb.SelectionStart = 0;
                    rtb.SelectionLength = rtb.Text.IndexOf("\n", 0) + 1;
                    rtb.SelectedText = "";
                }

                int start = rtb.TextLength;
                rtb.AppendText(text);
                int end = rtb.TextLength;

                // Textbox may transform chars, so (end-start) != text.Length
                rtb.Select(start, end - start);
                rtb.SelectionColor = color;
                rtb.SelectionFont = selfont;
                rtb.SelectionBackColor = bcolor;
                rtb.Select(end, 0);

                rtb.ScrollToCaret();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        String TXimagefilename = "";
        String TXRealFilename = "";
        long TXRealFileSize = 0;
        String TXfoldername = "";
        String lastFullName = "";

        // prepare an image file for transmission
        void prepareImage(String fullfn)
        {
            if (statics.checkImage(fullfn) == false) return;

            // all images are converted to jpg, make the new filename
            TXfoldername = statics.purePath(fullfn);
            TXRealFilename = statics.pureFilename(fullfn);
            TXRealFilename = statics.AddReplaceFileExtension(TXRealFilename,"jpg");
            lastFullName = fullfn;

            // random filename for picturebox control (picturebox cannot reload image from actual filename)
            try {
                //statics.FileDelete(TXimagefilename); 
                // delete also older unused files
                foreach (string f in Directory.EnumerateFiles(statics.addTmpPath(""), "tempTX*.jpg"))
                    statics.FileDelete(f);
            } catch { }
            Random randNum = new Random();
            TXimagefilename = statics.addTmpPath("tempTX" + randNum.Next(0, 65000).ToString() + ".jpg");

            // get the quality selected by the user
            String qual = comboBox_quality.Text;
            long max_size = 22500;
            if (qual.Contains("30s")) max_size = 12000;
            if (qual.Contains("2min")) max_size = 45000;
            if (qual.Contains("4min")) max_size = 90000;

            // resize image and save it according the quality settings
            Image img = new Bitmap(fullfn);
            String cs = tb_callsign.Text;
            if (cb_stampcall.Checked == false) cs = "";
            String inf = tb_info.Text;
            if (cb_stampinfo.Checked == false) inf = "";
            if (!checkBox_big.Checked)
            {
                img = ih.ResizeImage(img, 320, 240, cs, inf);
                // set quality by reducing the file size and save under default name
                ih.SaveJpgAtFileSize(img, TXimagefilename, max_size / 2);
            }
            else
            {
                img = ih.ResizeImage(img, 640, 480, cs, inf);
                // set quality by reducing the file size and save under default name
                ih.SaveJpgAtFileSize(img, TXimagefilename, max_size);
            }

            //pictureBox_tximage.Load(TXimagefilename); this does not work under ARM mono
            pictureBox_tximage.BackgroundImage = Image.FromFile(TXimagefilename);
            TXRealFileSize = statics.GetFileSize(TXimagefilename);
            ShowTXstatus();
            txcommand = statics.Image;
        }

        void ShowTXstatus()
        {
            if(txcommand == statics.Image)
                label_tximage.Text = langstr[10] + TXRealFilename + langstr[11] + (ArraySend.txpos / 1000).ToString() + langstr[30] + (TXRealFileSize / 1000).ToString() + " kB";
            else
                label_txfile.Text = langstr[12] + TXRealFilename + langstr[11] + (ArraySend.txpos / 1000).ToString() + langstr[30] + (TXRealFileSize / 1000).ToString() + " kB";
        }

        // in loop mode only: send the next picture in current image folder
        void startNextImage()
        {
            if (TXfoldername == "" || lastFullName == "") return;

            // read all file from folder
            String[] files = Directory.GetFiles(TXfoldername);
            Array.Sort(files);
            int i;
            bool found = false;
            for(i=0; i<files.Length; i++)
            {
                // look for the last transmitted file
                if(files[i] == lastFullName)
                {
                    // choose the next file
                    if (++i == files.Length) i = 0;
                    // check if the file is a valid image
                    try
                    {
                        Image dummy = new Bitmap(files[i]);
                        dummy.Dispose();
                        found = true;
                        break;
                    }
                    catch
                    {
                        lastFullName = files[i];
                    }
                }
            }
            if (!found) return;
            prepareImage(files[i]);
            button_sendimage_Click(null, null);
        }

        private void button_loadimage_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = langstr[13];
            if (open.ShowDialog() == DialogResult.OK)
            {
                prepareImage(open.FileName);
            }
        }

        private void button_sendimage_Click(object sender, EventArgs e)
        {
            txcommand = statics.Image;
            rxbytecounter = 0;
            pictureBox_rximage.Image = null;
            Byte[] imgarr = File.ReadAllBytes(TXimagefilename); // compressed temp file name
            ArraySend.Send(imgarr, statics.Image, TXimagefilename, TXRealFilename);    // compress temp file name and real file name
        }

        private void button_startBERtest_Click(object sender, EventArgs e)
        {
            rtb.Text = "";
            missBlocks = 0;
            frameinfo = (Byte)statics.FirstFrame;
            txcommand = statics.BERtest;
        }

        private void button_stopBERtest_Click(object sender, EventArgs e)
        {
            txcommand = statics.noTX;
        }

        int rxframecounter = 0;
        int lastfrmnum = 0;
        int missBlocks = 0;
        private void BERcheck(Byte[] rxdata, int frmnum, int minfo)
        {
            if (minfo == statics.FirstFrame)
                rxframecounter = 0;

            if (lastfrmnum == frmnum) return;
            lastfrmnum = frmnum;

            rxframecounter++;

            missBlocks += (frmnum - rxframecounter);
            if (missBlocks < 0) missBlocks = 0;
            String line = "RX: " + frmnum.ToString().PadLeft(6, ' ') + " "; // + rxframecounter + " " + missBlocks + " ";
            rxframecounter = frmnum;

            // print payload (must be printable chars)
            line += Encoding.UTF8.GetString(rxdata).Substring(0, 50) + " ...";

            // show RX status for this frame
            if (rxstat == 4)
                line += langstr[14];
            else
                line += langstr[29];

            int bits = rxframecounter * 258 * 8;
            int bytes = rxframecounter * 258;
            String sbit = "b";
            String sbyt = "B";

            if (bits > 1000)
            {
                bits /= 1000;
                sbit = "kbit";
            }
            if (bits > 1000)
            {
                bits /= 1000;
                sbit = "Mbit";
            }

            if (bytes > 1000)
            {
                bytes /= 1000;
                sbyt = "kByte";
            }
            if (bytes > 1000)
            {
                bytes /= 1000;
                sbyt = "MByte";
            }

            line += " " + bits.ToString() + " " + sbit + "  " + bytes.ToString() + " " + sbyt;

            line += "\r\n";
            printText(rtb,line);
        }

        int[] blockres = new int[2];
        private void ShowStatus(int rxtype, int minfo)
        {
            if (minfo == statics.FirstFrame)
                rxbytecounter = 0;

            // calculate speed
            int fsz = (int)recfile.filesize;
            if (fsz == 0) fsz = ArraySend.FileSize; // during reception we do not have the final size, use the transmitted size
            // fsz = real or zipped file size, whatever available
            // transmitted size in % of zipped file
            int txsize = 0;
            if (ArraySend.FileSize > 0)
                txsize = (recfile.rxbytes * 100) / ArraySend.FileSize;
            // transmitted size of real filesize
            int txreal = (fsz * txsize) / 100;
            // speed
            int speed_bps = 0;
            if(recfile.runtime.TotalSeconds > 0)
                speed_bps = (int)(((double)txreal * 8.0) / recfile.runtime.TotalSeconds);

            // show RX status on top of the RX windows
            String s = "RX: ";

            recfile.blockstat(blockres);
            int missingBlocks = blockres[0] - blockres[1];

            if (ArraySend.rxFilename != null && ArraySend.rxFilename.Length > 0)
            {
                s += ArraySend.rxFilename + " ";
                s += recfile.rxbytes / 1000 + langstr[30] + ArraySend.FileSize / 1000 + " kB ";
                s += Math.Truncate(recfile.runtime.TotalSeconds) + " s, ";
                s += blockres[1] + langstr[30] + blockres[0] + langstr[15];
            }
            else
                s += langstr[16];

            if (rxtype == statics.Image)
                label_rximage.Text = s;

            if (rxtype == statics.AsciiFile || rxtype == statics.HTMLFile || rxtype == statics.BinaryFile)
                label_rxfile.Text = s;

            // show speed in status line at the left side
            toolStripStatusLabel.Text = langstr[17] + speed.ToString() + " bps";

            if (missBlocks < 0) missBlocks = 0;
            if (missingBlocks < 0) missingBlocks = 0;

            // show RX status in the status line
            if (rxtype == statics.BERtest)
                RXstatus.Text = "RX: " + rxbytecounter + langstr[18] + missBlocks;
            else
            {
                if(fsz > 0)
                    RXstatus.Text = "RX: " + fsz + langstr[18] + missingBlocks;
                else
                    RXstatus.Text = "RX: " + rxbytecounter + langstr[18] + missingBlocks;
            }

            if(speed_bps > 0)
                RXstatus.Text += langstr[19] + speed_bps + " bps";
        }

        private void button_cancelimg_Click(object sender, EventArgs e)
        {
            //txcommand = statics.noTX;    // finished
            label_rximage.ForeColor = Color.Black;
            pictureBox_rximage.Image = null;
            ArraySend.stopSending();
        }

        private void checkBox_small_CheckedChanged(object sender, EventArgs e)
        {
            // scale all elements
            // this is required if a scaled screen resolution is used für large 4k monitor, important under mono
            // since mono fails in automatic scaling if the screen resolution is different from 1:1
            label_rximage.Location = new Point(6, 7);
            label_rximage.Location = new Point(650, 7);
            pictureBox_tximage.Size = new Size(640,480);
            pictureBox_rximage.Size = new Size(640,480);
            int yPicBoxes = label_rximage.Location.Y + label_rximage.Size.Height + 3;
            pictureBox_tximage.Location = new Point(1, yPicBoxes);
            pictureBox_rximage.Location = new Point(642, yPicBoxes);

            int gb_yloc = pictureBox_tximage.Location.Y + pictureBox_tximage.Size.Height + 3;
            groupBox1.Location = new Point(3, gb_yloc);

            tabControl1.Size = new Size(pictureBox_tximage.Size.Width + pictureBox_rximage.Size.Width + 10, 
                                        label_rximage.Size.Height + 3 + pictureBox_rximage.Size.Height + 3 + groupBox1.Size.Height + 3 + 20);

            int rxpan_yloc = tabControl1.Location.Y + tabControl1.Size.Height + 3;
            panel_constel.Location = new Point(11, rxpan_yloc);
            panel_constel.Size = new Size(75,75);

            panel_txspectrum.Location = new Point(92, rxpan_yloc);
            panel_txspectrum.Size = new Size(441,75);

            rtb.Size = new Size(tabControl1.Size.Width - 30, tabControl1.Size.Height - button_startBERtest.Location.Y - button_startBERtest.Size.Height - 44);

            this.Size = new Size(tabControl1.Size.Width + 23, rxpan_yloc + panel_constel.Size.Height + statusStrip1.Size.Height + 42);

            int xf = bt_file_ascii.Location.X + bt_file_ascii.Size.Width + 20;
            int yf = bt_file_ascii.Location.Y;
            rtb_TXfile.Location = new Point(xf, yf);

            int mw = tabControl1.Size.Width - bt_file_ascii.Size.Width - 80;
            mw /= 2;
            int mh = tabControl1.Size.Height - bt_file_ascii.Size.Height - 50;
            rtb_TXfile.Size = new Size(mw, mh);

            xf += mw + 5;
            rtb_RXfile.Location = new Point(xf, yf);
            rtb_RXfile.Size = new Size(mw, mh);

            int ly = rtb_TXfile.Location.Y / 4;
            label_txfile.Location = new Point(rtb_TXfile.Location.X, ly);
            label_rxfile.Location = new Point(rtb_RXfile.Location.X, ly);

            label_speed.Location = new Point(panel_txspectrum.Location.X + panel_txspectrum.Size.Width + 15,panel_txspectrum.Location.Y+10);
            cb_speed.Location = new Point(label_speed.Location.X + label_speed.Size.Width + 10, label_speed.Location.Y-5);

            int y = 26;
            label_fifo.Location = new Point(label_speed.Location.X, label_speed.Location.Y + y);
            progressBar_fifo.Location = new Point(cb_speed.Location.X, cb_speed.Location.Y + y+5);
            progressBar_fifo.Size = new Size(progressBar_fifo.Width, 18);

            y = 20;
            label_capfifo.Location = new Point(label_fifo.Location.X, label_fifo.Location.Y + y);
            progressBar_capfifo.Location = new Point(progressBar_fifo.Location.X, progressBar_fifo.Location.Y + y);
            progressBar_capfifo.Size = new Size(progressBar_capfifo.Width, 18);

            lb_rxsignal.Location = new Point(progressBar_capfifo.Location.X + progressBar_capfifo.Size.Width + 15, label_capfifo.Location.Y);
            pb_rxsignal.Location = new Point(lb_rxsignal.Location.X + lb_rxsignal.Size.Width + 2, label_capfifo.Location.Y-5);

            lb_rxsync.Location = new Point(pb_rxsignal.Location.X + pb_rxsignal.Size.Width + 15, label_capfifo.Location.Y);
            pb_rxsync.Location = new Point(lb_rxsync.Location.X + lb_rxsync.Size.Width + 2, label_capfifo.Location.Y-5);
        }

        public String GetMyBroadcastIP()
        {
            String ip = "255.255.255.255";
            String[] myips = statics.getOwnIPs();
            //Console.WriteLine("BClen: " + myips.Length.ToString());
            // nur wenn der PC eine IP hat
            // hat er mehr, dann wissen wir nicht in welchem Netz wird broadcasten sollen, nehmen also die 255.255.255.255
            if (myips.Length == 1)
            {
                int idx = myips[0].LastIndexOf('.');
                if (idx >= 0)
                {
                    ip = myips[0].Substring(0, idx);
                    ip += ".255";
                    //Console.WriteLine("BCip: " + ip);
                }
            }
            return ip;
        }
                
        Byte getLSaudioDevice()
        {
            String s = cb_loudspeaker.Text;
            Byte x = (Byte)cb_loudspeaker.Items.IndexOf(s);
            Console.WriteLine("LS:" + s + " " + x);
            //if (s.ToUpper() == "DEFAULT") x = 255;
            return x;
        }

        Byte getMICaudioDevice()
        {
            String s = cb_mic.Text;
            Byte x = (Byte)cb_mic.Items.IndexOf(s);
            Console.WriteLine("MIC:" + s + " " + x);
            //if (s.ToUpper() == "DEFAULT") x = 255;
            return x;
        }

        /*
         * search for the modem IP:
         * send a search message via UDP to port UdpBCport
         * if a modem receives this message, it returns with an
         * UDP message to UdpBCport containing a String with it's IP address
         * this message also contains the selected Audio Devices
         * 
         * Format:
         * Byte:
         * 0 ... 0x3c
         * 1 ... PB volume
         * 2 ... CAP volume
         * 3 ... announcement on/off, duration
         * 4 ... DV loudspeaker volume
         * 5 ... DV mic volume
         * 6 ... safe mode
         * 7..9 ... unused
         * 10 .. 109 ... PB device name
         * 110 .. 209 ... CAP device name
         * 
        */

        private void search_modem()
        {
            Byte safemode = 0; //number of frame repeats
            if (cb_safemode.Text.Contains("medium")) safemode = 2;
            else if (cb_safemode.Text.Contains("high")) safemode = 4;

            Byte[] txb = new byte[210];
            txb[0] = 0x3c;  // ID of this message
            txb[1] = (Byte)tb_PBvol.Value;
            txb[2] = (Byte)tb_CAPvol.Value;
            txb[3] = (Byte)cb_announcement.Items.IndexOf(cb_announcement.Text);
            txb[4] = (Byte)tb_loadspeaker.Value;
            txb[5] = (Byte)tb_mic.Value;
            txb[6] = safemode;
            txb[7] = (Byte)0;   // unused
            txb[8] = (Byte)0;   // unused
            txb[9] = (Byte)0;   // unused

            Byte[] bpb = statics.StringToByteArrayUtf8(cb_audioPB.Text);
            Byte[] bcap = statics.StringToByteArrayUtf8(cb_audioCAP.Text);
            //Byte[] bpb = statics.StringToByteArray(cb_audioPB.Text);
            //Byte[] bcap = statics.StringToByteArray(cb_audioCAP.Text);

            for (int i=0; i<100; i++)
            {
                if (i >= bpb.Length)
                    txb[i + 10] = 0;
                else
                    txb[i+10] = bpb[i];

                if (i >= bcap.Length)
                    txb[i + 110] = 0;
                else
                    txb[i + 110] = bcap[i];
            }

            Udp.UdpBCsend(txb, GetMyBroadcastIP(), statics.UdpBCport_AppToModem);

            Udp.searchtimeout++;
            if (Udp.searchtimeout >= 3)
                statics.ModemIP = "1.2.3.4";
        }

        private void bt_file_send_Click(object sender, EventArgs e)
        {
            rtb_RXfile.Text = "";
            statics.FileDelete(statics.zip_RXtempfilename);

            Byte[] textarr = File.ReadAllBytes(statics.zip_TXtempfilename);
            ArraySend.Send(textarr, (Byte)txcommand, TXfilename, TXRealFilename);
        }

        private void bt_file_ascii_Click(object sender, EventArgs e)
        {
            bt_sendFile("Text Files(*.txt*; *.*)|*.txt; *.*", statics.AsciiFile);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            bt_sendFile("HTML Files(*.html; *.htm; *.*)|*.html; *.htm; *.*", statics.HTMLFile);
        }

        private void bt_sendBinaryFile_Click(object sender, EventArgs e)
        {
            bt_sendFile("All Files(*.*)|*.*", statics.BinaryFile);
        }

        private void bt_sendFile(String filter, int cmd)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.Filter = filter;
            if (open.ShowDialog() == DialogResult.OK)
            {
                txcommand = cmd;
                TXfilename = open.FileName;
                TXRealFilename = open.SafeFileName;
                if (txcommand == statics.BinaryFile)
                    rtb_TXfile.Text = langstr[20] + TXfilename + langstr[21];
                else
                    rtb_TXfile.Text = File.ReadAllText(TXfilename);

                // compress file
                ZipStorer zs = new ZipStorer();
                zs.zipFile(statics.zip_TXtempfilename, open.SafeFileName, open.FileName);

                TXRealFileSize = statics.GetFileSize(statics.zip_TXtempfilename);
                ShowTXstatus();
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cb_speed.Text.Contains("3000")) statics.real_datarate = 3000;
            if (cb_speed.Text.Contains("4000")) statics.real_datarate = 4000;
            if (cb_speed.Text.Contains("4410")) statics.real_datarate = 4410;
            if (cb_speed.Text.Contains("4800")) statics.real_datarate = 4800;
            if (cb_speed.Text.Contains("5500")) statics.real_datarate = 5500;
            if (cb_speed.Text.Contains("6000")) statics.real_datarate = 6000;
            if (cb_speed.Text.Contains("6600")) statics.real_datarate = 6600;
            if (cb_speed.Text.Contains("7200")) statics.real_datarate = 7200;

            Byte[] txdata = new byte[statics.PayloadLen + 2];
            int idx = cb_speed.SelectedIndex;
            txdata[0] = (Byte)statics.ResamplingRate; // BER Test Marker
            txdata[1] = (Byte)idx;

            // and send info to modem
            Udp.UdpSendCtrl(txdata);

            txcommand = statics.noTX;
            // stop any ongoing transmission
            button_cancelimg_Click(null, null);
        }
        
        private void timer_searchmodem_Tick(object sender, EventArgs e)
        {
            search_modem();
        }

        private void bt_rximages_Click(object sender, EventArgs e)
        {
            if (statics.ostype == 0)
            {
                try
                {
                    String s = "file://" + statics.getHomePath(statics.RXimageStorage, "");
                    Process.Start(s);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
            else
            {
                try
                {
                    Process.Start("xdg-open", statics.getHomePath(statics.RXimageStorage, ""));
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }

        }

        private void bt_openrxfile_Click(object sender, EventArgs e)
        {
            if (statics.ostype == 0)
            {
                try
                {
                    String s = "file://" + statics.getHomePath("", "");
                    Process.Start(s);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
            else
            {
                try
                {
                    Process.Start("xdg-open", statics.getHomePath("", ""));
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }

        private String ReadString(StreamReader sr)
        {
            try
            {
                String s = sr.ReadLine();
                if (s != null)
                {
                    return s;
                }
            }
            catch { }
            return " ";
        }

        private int ReadInt(StreamReader sr)
        {
            int v;

            try
            {
                String s = sr.ReadLine();
                if (s != null)
                {
                    v = Convert.ToInt32(s);
                    return v;
                }
            }
            catch { }
            return 0;
        }

        void load_Setup()
        {
            try
            {
                using (StreamReader sr = new StreamReader(statics.getHomePath("", "od.cfg")))
                {
                    tb_callsign.Text = ReadString(sr);
                    cb_speed.Text = ReadString(sr);
                    String s = ReadString(sr);
                    cb_stampcall.Checked = (s == "1");
                    s = ReadString(sr);
                    // free for other usage 
                    cb_audioPB.Text = ReadString(sr);
                    cb_audioCAP.Text = ReadString(sr);
                    tb_PBvol.Value = ReadInt(sr);
                    tb_CAPvol.Value = ReadInt(sr);
                    s = ReadString(sr);
                    cb_autostart.Checked = (s == "1");
                    try { cb_announcement.Text = ReadString(sr); } catch { }
                    s = ReadString(sr);
                    try { cb_stampinfo.Checked = (s == "1"); } catch { }
                    try { tb_info.Text = ReadString(sr); } catch { }
                    try { cb_loudspeaker.Text = ReadString(sr); } catch { }
                    try { cb_mic.Text = ReadString(sr);  } catch { }
                    try { tb_loadspeaker.Value = ReadInt(sr); } catch { }
                    try { tb_mic.Value = ReadInt(sr); } catch { }
                    try
                    {
                        s = ReadString(sr);
                        rb_opus.Checked = (s == "1");
                        rb_codec2.Checked = (s != "1");
                    }
                    catch { }
                    try { cb_language.Text = ReadString(sr); } catch { }
                }
            }
            catch
            {
                tb_callsign.Text = "";
                cb_autostart.Checked = true;
            }

            if (cb_audioPB.Text.Length <= 1) cb_audioPB.Text = "Default";
            if (cb_audioCAP.Text.Length <= 1) cb_audioCAP.Text = "Default";
        }

        void save_Setup()
        {
            try
            {
                using (StreamWriter sw = new StreamWriter(statics.getHomePath("", "od.cfg")))
                {
                    sw.WriteLine(tb_callsign.Text);
                    sw.WriteLine(cb_speed.Text);
                    sw.WriteLine(cb_stampcall.Checked?"1":"0");
                    sw.WriteLine("0"); // free for other use
                    sw.WriteLine(cb_audioPB.Text);
                    sw.WriteLine(cb_audioCAP.Text);
                    sw.WriteLine(tb_PBvol.Value.ToString());
                    sw.WriteLine(tb_CAPvol.Value.ToString());
                    sw.WriteLine(cb_autostart.Checked ? "1" : "0");
                    sw.WriteLine(cb_announcement.Text);
                    sw.WriteLine(cb_stampinfo.Checked ? "1" : "0");
                    sw.WriteLine(tb_info.Text);
                    sw.WriteLine(cb_loudspeaker.Text);
                    sw.WriteLine(cb_mic.Text);
                    sw.WriteLine(tb_loadspeaker.Value.ToString());
                    sw.WriteLine(tb_mic.Value.ToString());
                    sw.WriteLine(rb_opus.Checked ? "1" : "0");
                    sw.WriteLine(cb_language.Text);
                }
            }
            catch { }
        }

        private void bt_shutdown_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show(langstr[23], langstr[22], MessageBoxButtons.YesNo);
            if (dr == DialogResult.Yes)
            {
                Byte[] txdata = new byte[1];
                txdata[0] = (Byte)statics.Modem_shutdown;
                Udp.UdpSendCtrl(txdata);

                MessageBox.Show(langstr[24], langstr[22], MessageBoxButtons.OK);
            }
        }

        /// <summary>
        // TEST ONLY: tell modem to send a file
        private void button1_Click(object sender, EventArgs e)
        {
            Byte[] txdata = new byte[1];
            txdata[0] = (Byte)statics.AutosendFile;

            // and transmit it
            Udp.UdpSendCtrl(txdata);
        }

        private void bt_resetmodem_Click(object sender, EventArgs e)
        {
            Byte[] txdata = new byte[1];
            txdata[0] = (Byte)statics.ResetModem;

            // and transmit it
            Udp.UdpSendCtrl(txdata);
        }

        int setPBvolume = -1;
        int setCAPvolume = -1;

        private void tb_PBvol_Scroll(object sender, EventArgs e)
        {
            setPBvolume = tb_PBvol.Value;
        }

        private void tb_CAPvol_Scroll(object sender, EventArgs e)
        {
            setCAPvolume = tb_CAPvol.Value;
        }

        private void bt_blockinfo_Click(object sender, EventArgs e)
        {
            String s;
            int[] d = new int[2];
            recfile.oldblockinfo(d);
            int failed = d[0] - d[1];

            s = langstr[25] +
                "---------------\n" +
                "total : " + d[0] + "\n" +
                langstr[26] + d[1] + "\n" +
                langstr[27] + failed + "\n";
            if(failed > 1)
            {
                s += langstr[28];
            }

            Form2_showtext sf = new Form2_showtext("Block Info",s);
            sf.ShowDialog();
        }

        void setVoiceAudio()
        {
            /*
             * Format:
             * 0 ... statics.SetVoiceMode (25)
             * 1 ... voicemode
             * 2 ... codec
             * 3-102 ... LS device name
             * 103-202 ... MIC device name
             */
            Byte[] txdata = new byte[203];
            txdata[0] = (Byte)statics.SetVoiceMode;
            Byte opmode = 0;
            // values see: hsmodem.h _VOICEMODES_
            if (cb_switchtoLS.Checked) opmode = 1;
            if (cb_voiceloop.Checked) opmode = 2;
            if (cb_codecloop.Checked) opmode = 3;
            if (cb_digitalVoice.Checked) opmode = 4;
            if (cb_digitalVoiceRXonly.Checked) opmode = 5;
            if(opmode == 0) pb_voice.BackgroundImage = null;
            txdata[1] = opmode;
            Byte codec;
            if (rb_opus.Checked) codec = 0;
            else codec = 1;
            txdata[2] = codec;

            Byte[] bpb = statics.StringToByteArrayUtf8(cb_loudspeaker.Text);
            Byte[] bcap = statics.StringToByteArrayUtf8(cb_mic.Text);
            //Byte[] bpb = statics.StringToByteArray(cb_loudspeaker.Text);
            //Byte[] bcap = statics.StringToByteArray(cb_mic.Text);

            for (int i = 0; i < 100; i++)
            {
                if (i >= bpb.Length)
                    txdata[i + 3] = 0;
                else
                    txdata[i + 3] = bpb[i];

                if (i >= bcap.Length)
                    txdata[i + 103] = 0;
                else
                    txdata[i + 103] = bcap[i];
            }

            Udp.UdpSendCtrl(txdata);

            if(opmode > 0)
            {
                rb_opus.Enabled = false;
                rb_codec2.Enabled = false;
            }
            else
            {
                rb_opus.Enabled = true;
                rb_codec2.Enabled = true;
            }
        }

        private void cb_switchtoLS_CheckedChanged(object sender, EventArgs e)
        {
            if(cb_switchtoLS.Checked)
            {
                cb_voiceloop.Checked = false;
                cb_codecloop.Checked = false;
                cb_digitalVoice.Checked = false;
                cb_digitalVoiceRXonly.Checked = false;
                pb_voice.BackgroundImage = Properties.Resources.cdc_digital;
            }
            setVoiceAudio();
        }

        private void cb_voiceloop_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_voiceloop.Checked)
            {
                cb_switchtoLS.Checked = false;
                cb_codecloop.Checked = false;
                cb_digitalVoice.Checked = false;
                cb_digitalVoiceRXonly.Checked = false;
                pb_voice.BackgroundImage = Properties.Resources.cdc_intloop;
            }
            setVoiceAudio();
        }

        private void cb_codecloop_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_codecloop.Checked)
            {
                cb_switchtoLS.Checked = false;
                cb_voiceloop.Checked = false;
                cb_digitalVoice.Checked = false;
                cb_digitalVoiceRXonly.Checked = false;
                pb_voice.BackgroundImage = Properties.Resources.cdc_codecloop;
            }
            setVoiceAudio();
        }

        private void cb_digitalVoice_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_digitalVoice.Checked)
            {
                cb_switchtoLS.Checked = false;
                cb_voiceloop.Checked = false;
                cb_codecloop.Checked = false;
                cb_digitalVoiceRXonly.Checked = false;
                pb_voice.BackgroundImage = Properties.Resources.cdc_dv;
            }
            setVoiceAudio();
        }

        private void cb_digitalVoiceRXonly_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_digitalVoiceRXonly.Checked)
            {
                cb_switchtoLS.Checked = false;
                cb_voiceloop.Checked = false;
                cb_codecloop.Checked = false;
                cb_digitalVoice.Checked = false;
                pb_voice.BackgroundImage = Properties.Resources.cdc_dvrx;
            }
            setVoiceAudio();
        }

        int setLSvolume = -1;
        int setMICvolume = -1;

        private void tb_loadspeaker_Scroll(object sender, EventArgs e)
        {
            setLSvolume = tb_loadspeaker.Value;
        }

        private void tb_mic_Scroll(object sender, EventArgs e)
        {
            setMICvolume = tb_mic.Value;
        }

        int language = 0;
        void set_language(int lang)
        {
            language = lang;

            if (language == 0)
            {
                langstr = langstr_en;

                tabPage_image.Text = "Image";
                label_nextimage.Text = "next image in ...";
                cb_loop.Text = "loop (send all images in folder)";
                bt_rximages.Text = "   RX Images";
                button_loadimage.Text = "    Load Image";
                label2.Text = "Quality:";
                button_cancelimg.Text = "   Cancel";
                button_sendimage.Text = "Send";
                label_rximage.Text = "RX image";
                label_tximage.Text = "TX image";
                tabPage_file.Text = "File";
                button2.Text = "Cancel";
                bt_openrxfile.Text = "Open RX file folder";
                label_rxfile.Text = "RX File";
                label_txfile.Text = "TX File";
                bt_file_send.Text = "SEND";
                bt_sendBinaryFile.Text = "Load Binary File";
                bt_file_html.Text = "Load HTML File";
                bt_file_ascii.Text = "   Load ASCII Text File";
                tabPage_audio.Text = "Voice Audio";
                groupBox7.Text = "Codec Selection";
                rb_codec2.Text = "CODEC-2 parametric audio codec. For QPSK. Audio rate: 3200 bps";
                rb_opus.Text = "OPUS rate adaptive codec. For 8APSK. Audio rate: 84% of data rate";
                groupBox6.Text = "Voice Audio Operating Mode";
                cb_digitalVoiceRXonly.Text = "Digital Voice RX:        Receiver ---> Codec ---> Loudspeaker";
                cb_digitalVoice.Text = "Digital Voice RX+TX: Microphone ---> Codec ---> Transmitter   |   Receiver ---> Codec ---> Loudspeaker";
                cb_codecloop.Text = "Codec Loop:  Microphone ---> Codec ---> Loudspeaker";
                cb_switchtoLS.Text = "Digital Monitor: Receiver ---> Loudspeaker";
                groupBox5.Text = "Loadspeaker / Microphone / Headset";
                label8.Text = "Volume:";
                label10.Text = "Loadspeaker/Headphone:";
                label11.Text = "Microphone:";
                groupBox4.Text = "Maintenance";
                cb_autostart.Text = "AUTO start/stop HSmodem";
                bt_shutdown.Text = "Shutdown Modem-SBC";
                tb_shutdown.Text = "before switching off the \r\nmodem SBC click here to \r\navoid defective SD-cards.\r\n";
                bt_resetmodem.Text = "Reset RX Modem";
                textBox3.Text = "only uncheck if modem runs on a separate PC";
                groupBox3.Text = "Transceiver Audio";
                label6.Text = "Volume:";
                label5.Text = "Volume:";
                label3.Text = "Audio Playback Device:";
                label4.Text = "Audio Record Device:";
                groupBox2.Text = "Personal Settings";
                cb_stampinfo.Text = "Insert Info into picture";
                textBox5.Text = "every";
                textBox4.Text = "transmissions";
                textBox1.Text = "send announcement before pic/file transfer";
                label1.Text = "Callsign:";
                cb_stampcall.Text = "Insert Callsign into picture";
                tabPage_about.Text = "About";
                label_speed.Text = "Speed [bit/s]:";
                label_fifo.Text = "TX Buffer:";
                bt_blockinfo.Text = "Block Info";
                label_capfifo.Text = "RX Buffer:";
                lb_rxsignal.Text = "RX Signal:";
                lb_rxsync.Text = "RX Sync:";

            }

            if (language == 1)
            {
                langstr = langstr_de;

                tabPage_image.Text = "Bilder";
                tabPage_file.Text = "Datei";
                tabPage_audio.Text = "Sprache";
                tabPage_about.Text = "Info";
                label2.Text = "Qualität";
                button_loadimage.Text = "Lade Bild";
                button_sendimage.Text = "Senden";
                button_cancelimg.Text = "    Abbruch";
                bt_rximages.Text = "RX Bilder";
                cb_loop.Text = "Endlosschleife: Alle Bilder im Verzeichnis senden";
                label_nextimage.Text = "nächstes Bild in ...";
                label_tximage.Text = "TX Bild";
                label_rximage.Text = "RX Bild";
                label_speed.Text = "Bitrate [bit/s]";
                label_fifo.Text = "TX Puffer";
                label_capfifo.Text = "RX Puffer";
                bt_file_ascii.Text = "  Lade ASCII Textdatei";
                bt_file_html.Text = "Lade HTML Datei";
                bt_sendBinaryFile.Text = "Lade Binärdatei";
                bt_file_send.Text = "SENDEN";
                button2.Text = "Abbrechen";
                bt_openrxfile.Text = "Öffne RX Dateien";
                groupBox5.Text = "Lautsprecher / Mikrofon / Kopfhörer";
                label10.Text = "Lautsprecher/Kopfhörer";
                label11.Text = "Mikrofon";
                label9.Text = "Lautst.:";
                label8.Text = "Lautst.:";
                groupBox6.Text = "Sprache / Audio Betriebsart";
                cb_switchtoLS.Text = "Digital Monitor: Empfänger -> Lautsprecher";
                cb_voiceloop.Text = "intere Schleife: Mikrofon -> Lautsprecher";
                cb_codecloop.Text = "Codec Schleife: Mikrofon -> Codec -> Lautsprecher";
                cb_digitalVoiceRXonly.Text = "DV Empfang: Empfänger -> Codec -> Lautsprecher";
                cb_digitalVoice.Text = "DV Transceiver: Mikrofon -> Codec -> Sender | Empfänger -> Codec -> Lautsprecher";
                groupBox7.Text = "Codec Auswahl";
                rb_opus.Text = "OPUS adaptiver Codec. Für 8APSK. Audio-Datenrate 84% der Bitrate";
                rb_codec2.Text = "CODEC-2 parametrischer Audiocodec. Für QPSK. Audiorate: 3200 bit/s";
                groupBox2.Text = "Persönliche Einstellungen";
                label1.Text = "Rufzeichen";
                cb_stampcall.Text = "Füge Rufzeichen ins Bild ein";
                cb_stampinfo.Text = "Füge Infotext ins Bild ein";
                textBox1.Text = "sende Ansagetext vor Daten- oder Bildsendung";
                textBox5.Text = "alle";
                textBox4.Text = "Aussendungen";
                label3.Text = "Audio Wiedergabe";
                label4.Text = "Audio Eingang";
                label5.Text = "Lautst.:";
                label6.Text = "Lautst.:";
                groupBox4.Text = "Wartung";
                textBox3.Text = "Ausschalten wenn hsmodem auf separatem PC läuft";
                tb_shutdown.Text = "Vor dem Ausschalten eines SBC diesen hier herunterfahren";
            }
        }

        private void cb_language_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(cb_language.Text.Contains("Deutsch"))
                set_language(1);
            else
                set_language(0);
        }

        String[] langstr;

        String[] langstr_en = new String[]{
            "next image in ",           //0
            "transmitting",
            "binary file received\r\n", //2
            "transmission time : ",
            "transmission speed: ",     //4
            "file size         : ",
            "file name         : ",     //6
            "file status       : not complete, retransmit\r\n\r\n",
            "Tuning Window",    //8
            "min Level",
            "TX image: ",   // 10
            ". Sent: ",
            "TX file: ",    //12
            "Image Files(*.jpg; *.jpeg; *.png; *.gif; *.bmp)|*.jpg; *.jpeg; *.png; *.gif; *.bmp",
            " frame lost",      //14
            " blocks OK",
            "wait for RX",     //16 
            "Line Speed: ",
            " Byte. Missing blocks: ",  //18
            " Net Speed:",
            "Binary file ",     //20
            " loaded",
            "Shut Down Modem",  //22
            "Do you want to shut down the Modem-Computer ?",
            "Please wait abt. 1 minute before powering OFF the modem",  //24
            "Received Blocks\n",
            "good  : ",     //26
            "failed: ",
            "\nfile incomplete, ask for retransmission",    //28
            " sequence OK",
            " of ", //30
            "Bad blocks, retransmission required",
            "Bad blocks",   //32
        };

        String[] langstr_de = new String[]{
            "nächstes Bild in",           //0
            "sende",
            "Binärdatei empfangen\r\n", //2
            "Sendezeit         : ",
            "Übertragungsgeschw: ",     //4
            "Dateigröße        : ",
            "Dateiname         : ",     //6
            "Dateistatus       : nicht komplett          \r\n\r\n",
            "Tuning Fenster",    //8
            "min Pegel",
            "TX Bild: ",   // 10
            ".Gesendet: ",
            "TX Datei: ",    //12
            "Bilddateien(*.jpg; *.jpeg; *.png; *.gif; *.bmp)|*.jpg; *.jpeg; *.png; *.gif; *.bmp",
            " Frame verloren",      //14
            " Blöcken OK",
            "warte auf RX",     //16 
            "Datenrate: ",
            " Byte. Fehlende Blöcke: ",  //18
            " Netto-Rate:",
            "Binärdatei ",     //20
            " geladen",
            "Modem herunterfahren",  //22
            "Wollen sie den Modem Computer herunterfahren?",
            "Bitte warten sie 1 Minuten vor dem Ausschalten des Modem Computers",  //24
            "empfangene Blöcke\n",
            "gut   : ",     //26
            "defekt: ",
            "\nDatei nicht komplett, bitte neu Senden ",    //28
            " Sequenz OK",
            " von ", //30
            "defekte Blöcke, Datei muss nochmal empfangen werden",
            "defekte Blöcke",   //32
        };

        private void cb_autostart_CheckedChanged(object sender, EventArgs e)
        {
            if (cb_autostart.Checked == false)
            {
                if (modemrunning)
                {
                    statics.StartHSmodem(false);
                    Console.WriteLine("Kill Modem");
                    modemrunning = false;
                }
            }
            else
            {
                if (modemrunning == false)
                {
                    modemrunning = statics.StartHSmodem(true);
                    Console.WriteLine("Start Modem");
                }
            }
        }
    }

}
