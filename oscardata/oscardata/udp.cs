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
                            statics.ModemIP = RemoteEndpoint.Address.ToString();
                            searchtimeout = 0;
                            // message b contains audio devices and init status
                            statics.initAudioStatus = b[0];
                            statics.initVoiceStatus = b[1];

                            //String s = statics.ByteArrayToString(b,2);
                            String s = statics.ByteArrayToStringUtf8(b, 2);
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
                            statics.PBfifousage = b[0];
                            statics.CAPfifousage = b[1];
                            statics.RXlevelDetected = b[2];
                            statics.RXinSync = b[3];
                            statics.maxRXlevel = b[4];
                            statics.maxTXlevel = b[5];
                            Byte[] b1 = new byte[b.Length - 4];
                            Array.Copy(b, 4, b1, 0, b1.Length);
                            uq_fft.Add(b1);
                        }

                        // IQ data
                        if (rxtype == statics.udp_iq)
                        {
                            //Console.WriteLine("IQ");
                            for (int i = 0; i < b.Length; i++)
                            {
                                // insert new byte in lastb
                                for (int sh = 12 - 1; sh > 0; sh--)
                                    lastb[sh] = lastb[sh - 1];
                                lastb[0] = b[i];

                                // test if aligned
                                Int32 re = 0, im = 0;
                                if (lastb[0] == 0xe8 && lastb[1] == 3 && lastb[2] == 0 && lastb[3] == 0)
                                {
                                    // we are aligned
                                    re = lastb[7];
                                    re <<= 8;
                                    re += lastb[6];
                                    re <<= 8;
                                    re += lastb[5];
                                    re <<= 8;
                                    re += lastb[4];

                                    im = lastb[11];
                                    im <<= 8;
                                    im += lastb[10];
                                    im <<= 8;
                                    im += lastb[9];
                                    im <<= 8;
                                    im += lastb[8];
                                }

                                drawBitmap(re, im);
                            }
                        }
                    }
                }
                catch { }
            }
        }

        static int panelw = 75, panelh = 75;
        static int maxdrawanz = 160;
        static int drawanz = 0;
        static Bitmap bm;
        static void drawBitmap(Int32 re, Int32 im)
        {
            if (re == 0 && im == 0) return;
            if (++drawanz >= maxdrawanz && uq_iq.Count() <= 1)
            {
                drawanz = 0;
                uq_iq.Add(bm);
                bm = new Bitmap(75, 75);
            }
            
            using (Graphics gr = Graphics.FromImage(bm))
            {
                // re and im are in the range of +/- 2^24 (16777216)
                // scale it to +/- 128
                double fre = re;
                double fim = im;

                fre = fre * panelw / 2 / 16777216.0;
                fim = fim * panelh / 2 / 16777216.0;

                // scale it to the picture
                int x = panelw / 2 + (int)fre;
                int y = panelh / 2 + (int)fim;

                int et = 2;
                gr.FillEllipse(Brushes.Blue, x - et, y - et, et * 2, et * 2);
            }
        }

        static AutoResetEvent autoEvent = new AutoResetEvent(false);

        // Udp TX Loop runs in its own thread
        static void Udptxloop()
        {
            UdpClient udpc = new UdpClient();

            // calculate cycle time for the requested data rate
            // time in ms for one bit:  1000/statics.datarate

            //int actdatarate = statics.getDatarate();
            //int wait_datarate = (int)(((double)statics.UdpBlocklen * 8.0 * 1000.0 / (double)(statics.getDatarate())));
            //Timer TTimer = new Timer(new TimerCallback(TXTickTimer), autoEvent, 0, wait_datarate);

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

                if(statics.PBfifousage < 2)
                {
                    // we need to send more payload data
                    if (uq_tx.Count() > 0)
                    {
                        Byte[] b = uq_tx.Getarr();
                        udpc.Send(b, b.Length, statics.ModemIP, statics.UdpTXport);
                        wait = false;
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

        static void TXTickTimer(object stateInfo)
        {
            autoEvent = (AutoResetEvent)stateInfo;

            autoEvent.Set();
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

        public static UInt16[] UdpGetFFT()
        {
            if (uq_fft.Count() == 0) return null;

            Byte[] d = uq_fft.Getarr();
            UInt16[] varr = new UInt16[d.Length / 2];
            int j = 0;
            for (int i = 0; i < d.Length; i += 2)
            {
                if ((i + 1) >= d.Length) break;
                UInt16 us = d[i];
                us <<= 8;
                us += d[i + 1];
                if (j >= (varr.Length)) break;
                varr[j++] = us;
            }
            return varr;
        }

        static Byte[] lastb = new Byte[12];
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

        public static bool IQavail()
        {
            if (uq_iq.Count() == 0) return false;
            return true;
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
