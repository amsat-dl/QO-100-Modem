/*
 * 9/2020 (c) DJ0ABR, Kurt Moraw
 * License: GPL 3.0
 * 
 * udp.cs
 * ------
 * 
 * Creates a new thread which handles all incoming and outgoing UDP traffic.
 * Communication to other threads is done via a thread-safe pipe.
 * The UDP transmitter handles the correct datarate according to the modem speed.
*/

using System;
using System.Collections;
using System.Drawing;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace oscardata
{
    public static class Udp
    {
        // this thread handles udp RX
        static Thread udprx_thread;
        static Thread udptx_thread;

        // Pipes for data transferred via UDP ports
        static UdpQueue uq_rx = new UdpQueue();
        static UdpQueue uq_tx = new UdpQueue();
        static UdpQueue uq_ctrl = new UdpQueue();
        static UdpQueue uq_fft = new UdpQueue();
        static UdpQueue uq_iq = new UdpQueue();
        static UdpQueue uq_rtty_rx = new UdpQueue();

        public static int searchtimeout = 0;
        static String last_audiodevstring = "";

        // Constructor
        // called when Udp is created by the main program
        public static void InitUdp()
        {
            // create thread for UDP RX
            udprx_thread = new Thread(new ThreadStart(Udprxloop));
            udprx_thread.Name = "Thread: oscardata UDP-RX";
            udprx_thread.Start();

            // create thread for UDP TX
            udptx_thread = new Thread(new ThreadStart(Udptxloop));
            udptx_thread.Name = "Thread: oscardata UDP-TX";
            udptx_thread.Start();
        }

        public static void Close()
        {
            try
            {
                udprx_thread.Abort();
                udptx_thread.Abort();
            }
            catch { }
        }

        // Udp RX Loop runs in its own thread
        
        static void Udprxloop()
        {
            int extIPcnt = 0;

            // define UDP port
            UdpClient udpc = new UdpClient(statics.UdpRXport);
            udpc.Client.ReceiveTimeout = 100;

            while (statics.running)
            {
                try
                {
                    // receive data from UDP port
                    IPEndPoint RemoteEndpoint = new IPEndPoint(IPAddress.Any, 0);
                    Byte[] rxarr = udpc.Receive(ref RemoteEndpoint);
                    if (rxarr != null)
                    {
                        // Data received:
                        // RemoteEndpoint.Address ... IP address of the sender
                        // RemoteEndpoint.Port ... port
                        // b[0] ... Type of data
                        // b+1 ... Byte array containing the data
                        int rxtype = rxarr[0];
                        Byte[] b = new byte[rxarr.Length - 1];
                        Array.Copy(rxarr, 1, b, 0, b.Length);

                        // payload
                        if (rxtype == statics.udp_payload)
                        {
                            //Console.WriteLine("payload");
                            uq_rx.Add(b);
                        }

                        // Broadcast response
                        if (rxtype == statics.udp_bc)
                        {
                            String ModIP = RemoteEndpoint.Address.ToString();
                            if (ModIP != statics.MyIP)
                            {
                                // this is not the local IP
                                // wait for 3 Receptions before accepting it
                                if (extIPcnt < 3)
                                {
                                    //Console.WriteLine("myIP:"+statics.MyIP+" modem is ext IP:"+ModIP+", waiting:" + extIPcnt);
                                    if (extIPcnt < 4) extIPcnt++;
                                    if (extIPcnt < 3)
                                        continue;
                                }
                                //Console.WriteLine("modem is ext IP, accepted");
                            }
                            else
                            {
                                //Console.WriteLine("modem is local IP");
                                extIPcnt = 0;
                            }

                            statics.ModemIP = ModIP;
                            searchtimeout = 0;
                            // message b contains audio devices and init status

                            statics.initAudioStatus = (b[0] == '1') ? 2 : 0;
                            statics.initAudioStatus |= (b[1] == '1') ? 1 : 0;
                            statics.initVoiceStatus = (b[2] == '1') ? 2 : 0;
                            statics.initVoiceStatus |= (b[3] == '1') ? 1 : 0;

                            //String s = statics.ByteArrayToString(b,4);
                            String s = statics.ByteArrayToStringUtf8(b, 4);
                            String[] sa1 = s.Split(new char[] { '^' });
                            statics.AudioPBdevs = sa1[0].Split(new char[] { '~' });
                            statics.AudioCAPdevs = sa1[1].Split(new char[] { '~' }); 

                            // has the device list changed ?
                            if(s != last_audiodevstring)
                            {
                                statics.GotAudioDevices = 1;
                                last_audiodevstring = s;
                            }

                            if(statics.GotAudioDevices == 0)
                                statics.GotAudioDevices = 1;
                        }

                        // FFT data
                        if (rxtype == statics.udp_fft)
                        {
                            int idx = 0;
                            statics.PBfifousage = b[idx++];
                            statics.CAPfifousage = b[idx++];
                            statics.RXlevelDetected = b[idx++];
                            statics.RXinSync = b[idx++];
                            statics.maxRXlevel = b[idx++];
                            statics.maxTXlevel = b[idx++];
                            statics.tune_frequency = b[idx++];
                            statics.tune_frequency <<= 8;
                            statics.tune_frequency += b[idx++];
                            //Console.WriteLine("f:" + statics.tune_frequency);
                            Byte[] b1 = new byte[b.Length - idx];
                            Array.Copy(b, idx, b1, 0, b1.Length);
                            drawFftBitmap(b1);
                        }

                        // IQ data
                        if (rxtype == statics.udp_iq)
                        {
                            Int16[] re = new Int16[b.Length / 2];
                            Int16[] im = new Int16[b.Length / 2];
                            int idx = 0;

                            for (int i = 0; i < b.Length; i+=4)
                            {
                                re[idx] = b[i+0];
                                re[idx] <<= 8;
                                re[idx] += b[i+1];

                                im[idx] = b[i+2];
                                im[idx] <<= 8;
                                im[idx] += b[i+3];
                                idx++;
                            }
                            drawBitmap(re, im);
                        }

                        if (rxtype == statics.udp_rtty_rx)
                        {
                            uq_rtty_rx.Add(b);
                        }
                    }
                }
                catch { }
            }
        }

        static int panelw = 75, panelh = 75;
        static Bitmap bm;
        const int maxsum = 5000;
        static Int16[] resum = new Int16[maxsum];
        static Int16[] imsum = new Int16[maxsum];
        static int sumidx = 0;
        static SolidBrush bgcol = new SolidBrush(Color.Silver);//FromArgb(255, (byte) 0x40, (byte) 0x00, (byte) 0x00));

        static double scaleiq(int v)
        {
            double f = v;
            f /= 15000.0;
            // f goes from -1 to +1
            // scale it to the graphics
            const int sz = 45;
            f += 1;
            f /= 2;
            f *= sz;
            f += (panelw-sz)/2;
            return f;
        }

        static void drawBitmap(Int16[] re, Int16[] im)
        {
            // collect IQ data
            for (int i = 0; i < re.Length; i++)
            {
                if (sumidx < maxsum)
                {
                    resum[sumidx] = re[i];
                    imsum[sumidx] = im[i];
                    sumidx++;
                }
            }

            // check if there is space in bitmap fifo
            // if the GUI does not process the bitmaps fast enough, just cancel it
            if (uq_iq.Count() > 2)
                return;

            // bitmap for drawing the complete picture
            bm = new Bitmap(panelw, panelh);

            using (Graphics gr = Graphics.FromImage(bm))
            {
                // background
                gr.FillRectangle(bgcol, 0,0, panelw, panelh);
                // oscilloscope screen
                gr.DrawImage(new Bitmap(Properties.Resources.screen), 2, 1);
                /*
                // screws at the 4 corners
                Bitmap screw = new Bitmap(Properties.Resources.schraube);
                gr.DrawImage(screw, 2, 2);
                gr.DrawImage(screw, panelw - 2-screw.Width, 2);
                gr.DrawImage(screw, 2, panelh - 2 - screw.Height);
                gr.DrawImage(screw, panelw - 2 - screw.Width, panelh - 2 - screw.Height);
                */
                // draw constellation points
                for (int i = 0; i < sumidx; i++)
                {
                    if (resum[i] == 0 || imsum[i] == 0) continue;
                    double dist = Math.Sqrt((resum[i] * resum[i]) + (imsum[i] * imsum[i]));
                    if (dist > 22000) continue; // do not draw outside scope

                    double x = scaleiq(resum[i]);
                    double y = scaleiq(imsum[i]);

                    double et = 1.6;
                    x -= et;
                    y -= et;
                    double w = et * 2;
                    double h = et * 2;
                    gr.FillEllipse(Brushes.Yellow, (int)x, (int)y, (int)w, (int)h);
                }
            }

            uq_iq.Add(bm);
            sumidx = 0;
        }

        static int fftw = 410, ffth = 72;
        static Bitmap bmskala = new Bitmap(fftw,ffth);
        static bool bmf = false;
        static Font fnt = new Font("Verdana", 9.0f);
        static Font smallfnt = new Font("Verdana", 7.0f);
        static Pen penyl = new Pen(Brushes.Yellow, 1);
        static Pen pengn = new Pen(Brushes.LightGreen, 3);

        static void drawFftBitmap(Byte[] b1)
        {
            int yl = ffth - 20;
            int yh = 20;

            if (!bmf)
            {
                // pre-draw background
                bmf = true;
                Pen pen = new Pen(Brushes.Navy, 1);
                Pen pensolid = new Pen(Brushes.Navy, 1);
                pen.DashPattern = new float[] { 1.0F, 2.0F, 1.0F, 2.0F };
                Pen penred = new Pen(Brushes.Red, 1);
                Pen penred2 = new Pen(Brushes.Red, 2);

                using (Graphics gr = Graphics.FromImage(bmskala))
                {
                    gr.FillRectangle(bgcol, 0, 0, fftw, ffth);
                    gr.DrawImage(new Bitmap(Properties.Resources.osci), 0, 0);

                    for (int x = 10; x <= 390; x += 10)
                        gr.DrawLine(pen, x, yl, x, yh);

                    gr.DrawLine(penred2, 11, yl, 10, yh);
                    gr.DrawLine(penred, 150, yl, 150, yh);
                    gr.DrawLine(pensolid, 20, yl, 20, yh);
                    gr.DrawLine(pensolid, 280, yl, 280, yh);
                    gr.DrawLine(pensolid, 360, yl, 360, yh);

                    gr.DrawRectangle(penred, 15, yh, 270, yl-yh);

                    gr.DrawString("200", smallfnt, Brushes.Black, 8, yl);
                    gr.DrawString("1500", smallfnt, Brushes.Black, 135, yl);
                    gr.DrawString("2800", smallfnt, Brushes.Black, 265, yl);
                    gr.DrawString("3600", smallfnt, Brushes.Black, 345, yl);

                    gr.DrawString(statics.langstr[8], fnt, Brushes.Black, 100, 0);
                }

                bmskala.MakeTransparent(Color.White);
            }

            // check if there is space in bitmap fifo
            // if the GUI does not process the bitmaps fast enough, just cancel it
            if (uq_fft.Count() > 2)
                return;

            // bitmap for drawing the complete picture
            bm = new Bitmap(442, 76);

            int rshift = 14;
            using (Graphics gr = Graphics.FromImage(bm))
            {
                // background
                gr.FillRectangle(bgcol, 0, 0, bm.Width, bm.Height);
                // scala
                gr.DrawImage(bmskala,16,2);

                if(statics.real_datarate == 45)
                {
                    // RTTY Markers
                    int low = (statics.tune_frequency - 170 / 2)/10;
                    int high = (statics.tune_frequency + 170 / 2)/10;
                    gr.DrawLine(pengn, low + rshift, yl, low + rshift, yh + 3);
                    gr.DrawLine(pengn, high + rshift, yl, high + rshift, yh + 3);
                }
                /*
                // screws at the 4 corners
                Bitmap screw = new Bitmap(Properties.Resources.schraube);
                gr.DrawImage(screw, 2, 2);
                gr.DrawImage(screw, 442 - 2 - screw.Width, 2);
                gr.DrawImage(screw, 2, 76 - 2 - screw.Height);
                gr.DrawImage(screw, 442 - 2 - screw.Width, 76 - 2 - screw.Height);
                */
                // spectrum
                int lastus = -1;
                // values
                for (int i = 0; i < b1.Length-1; i+=2)
                {
                    int us = b1[i];
                    us <<= 8;
                    us += b1[i + 1];
                    double fus = 0;
                    if (us > 0)
                        fus = 35 * Math.Log10((double)us / 10);

                    us = (int)(fus - 5.0);
                    if(lastus != -1 && i>0)
                        gr.DrawLine(penyl, i/2+ rshift, 76-lastus, i/2+1+ rshift, 76-us); // 15 istead of 16 to get it in exact position
                    lastus = us;
                }
            }
            uq_fft.Add(bm);
        }

        // Udp TX Loop runs in its own thread
        static void Udptxloop()
        {
            DateTime dt = DateTime.UtcNow;
            UdpClient udpc = new UdpClient();

            while (statics.running)
            {
                bool wait = true;
                if(uq_ctrl.Count() > 0)
                {
                    // Control Message: send immediately
                    Byte[] b = uq_ctrl.Getarr();
                    udpc.Send(b, b.Length, statics.ModemIP, statics.UdpTXport);
                    wait = false;
                }

                if(statics.PBfifousage < 3)
                {
                    // we need to send more payload data
                    // but never send faster than 1000 Byte/s
                    // because statics.PBfifousage may be updated too slow
                    //DateTime dtact = DateTime.UtcNow;
                    //TimeSpan ts = dtact - dt;
                    //if (ts.TotalMilliseconds > statics.UdpBlocklen)
                    {
                        if (uq_tx.Count() > 0)
                        {
                            Byte[] b = uq_tx.Getarr();
                            udpc.Send(b, b.Length, statics.ModemIP, statics.UdpTXport);
                            wait = false;
                            //dt = dtact;
                        }
                    }
                }
                if (wait) Thread.Sleep(1);
            }
        }

        public static void UdpBCsend(Byte[] b, String ip, int port)
        {
            UdpClient udpc = new UdpClient();
            udpc.EnableBroadcast = true;
            udpc.Send(b, b.Length, ip, port);
        }

        // send a Byte array via UDP
        // this function can be called from anywhere in the program
        // it transfers the data to the udp-tx thread via a thread-safe pipe
        public static void UdpSendData(Byte[] b)
        {
            uq_tx.Add(b);
        }

        public static void UdpSendCtrl(Byte[] b)
        {
            uq_ctrl.Add(b);
        }

        public static int GetBufferCount()
        {
            return uq_tx.Count();
        }

        public static int GetBufferCountCtrl()
        {
            return uq_ctrl.Count();
        }

        public static Byte[] UdpReceive()
        {
            if (uq_rx.Count() == 0) return null;

            return uq_rx.Getarr();
        }

        public static qpskitem UdpGetIQ()
        {
            if (uq_iq.Count() == 0) return null;

            return uq_iq.GetQPSKitem();
        }

        public static Bitmap UdpBitmap()
        {
            if (uq_iq.Count() == 0) return null;

            return uq_iq.GetBitmap();
        }

        public static Bitmap UdpFftBitmap()
        {
            if (uq_fft.Count() == 0) return null;

            return uq_fft.GetBitmap();
        }

        public static bool IQavail()
        {
            if (uq_iq.Count() == 0) return false;
            return true;
        }

        public static Byte[] getRTTYrx()
        {
            if (uq_rtty_rx.Count() == 0) return null;
            return uq_rtty_rx.Getarr();
        }
    }

    // this class is a thread safe queue wich is used
    // to exchange data with the UDP RX/TX threads
    public class UdpQueue
    {
        Queue myQ = new Queue();

        public void Add(Byte [] b)
        {
            lock (myQ.SyncRoot)
            {
                myQ.Enqueue(b);
            }
        }

        public void Add(qpskitem b)
        {
            lock (myQ.SyncRoot)
            {
                myQ.Enqueue(b);
            }
        }

        public void Add(Bitmap bm)
        {
            lock (myQ.SyncRoot)
            {
                myQ.Enqueue(bm);
            }
        }

        public Bitmap GetBitmap()
        {
            Bitmap b;

            lock (myQ.SyncRoot)
            {
                b = (Bitmap)myQ.Dequeue();
            }
            return b;
        }

        public Byte[] Getarr()
        {
            Byte[] b;

            lock (myQ.SyncRoot)
            {
                b = (Byte[])myQ.Dequeue();
            }
            return b;
        }

        public qpskitem GetQPSKitem()
        {
            qpskitem b;

            lock (myQ.SyncRoot)
            {
                b = (qpskitem)myQ.Dequeue();
            }
            return b;
        }

        public qpskitem GetItem()
        {
            qpskitem b;

            lock (myQ.SyncRoot)
            {
                b = (qpskitem)myQ.Dequeue();
            }
            return b;
        }

        public int Count()
        {
            int result;

            lock (myQ.SyncRoot)
            {
                result = myQ.Count;
            }
            return result;
        }

        public void Clear()
        {
            lock (myQ.SyncRoot)
            {
                myQ.Clear();
            }
        }
    }

    public class qpskitem
    {
        public int re;
        public int im;
    }
}
