using System;
using System.Threading;

// Input: Byte Array
// Action: sends this byte array to the modem

namespace oscardata
{
    public static class ArraySend
    {
        static Timer TTimer;
        static Byte[] txdata;
        static int txlen;
        public static int txpos;
        static Byte txtype;
        static bool sending = false;
        public static Byte filestat = statics.noTX;
        static private readonly object busyLock = new object();
        static int timeout_period_ms = 10;
        static UInt16 fncrc;

        // start a timer which is used to send a file from txdata
        public static void ArraySendInit()
        {
            TTimer = new Timer(new TimerCallback(TimerTick), 0, 0, timeout_period_ms);
        }

        static void setSending(bool v)
        {
            lock(busyLock)
            {
                sending = v;
                if (v == false)
                    filestat = statics.LastFrame;
            }
        }

        public static bool getSending()
        {
            bool v;

            lock (busyLock)
            {
                v = sending;
            }
            return v;
        }

        /*
         * start sending a file
         * data ... contents of the file in a Byte array
         * type ... type of the file (see statics)
         * filename ... description of the file or its name (payload length max)
         */
        public static bool Send(Byte[] data, Byte type, String filename, String RealFileName)
        {
            // check if already sending
            if (getSending()) return false;

            txtype = type;
            txpos = 0;
            filestat = statics.FirstFrame;
            // add a file header and copy to txdata for transmission
            AddHeader(data,filename, RealFileName);

            // marker, we are sending
            txlen = txdata.Length;
            setSending(true);

            return true;
        }

        public static void stopSending()
        {
            setSending(false);
        }

        static void AddHeader(Byte[] data, String filename, String realname)
        {
            long filesize = data.Length;// statics.GetFileSize(filename);

            Byte[] fnarr = statics.StringToByteArray(realname);

            // CRC16 over complete file contents is the file ID
            Crc c = new Crc();
            fncrc = c.crc16_messagecalc(data, data.Length);

            // create the file header
            // 50 bytes ... Filename (or first 50 chars of the filename)
            // 2 bytes .... CRC16 od the filename, this is used as a file ID
            // 3 bytes .... size of file
            Byte[] header = new Byte[55];

            int len = fnarr.Length;
            if (len > 50) len = 50;
            Array.Copy(fnarr, header, len);
            header[50] = (Byte)((fncrc >> 8)&0xff);
            header[51] = (Byte)(fncrc&0xff);

            header[52] = (Byte)((filesize >> 16) & 0xff);
            header[53] = (Byte)((filesize >> 8) & 0xff);
            header[54] = (Byte)(filesize & 0xff);

            txdata = new Byte[data.Length + header.Length];
            Array.Copy(header, txdata, header.Length);
            Array.Copy(data, 0, txdata, header.Length, data.Length);
        }

        public static String rxFilename;
        public static int FileID;
        public static int FileSize;
        public static Byte[] GetAndRemoveHeader(Byte[] data)
        {
            try
            {
                Byte[] fnarr = new byte[50];
                Array.Copy(data, fnarr, 50);
                rxFilename = statics.ByteArrayToString(fnarr);

                FileID = data[50];
                FileID <<= 8;
                FileID += data[51];

                FileSize = data[52];
                FileSize <<= 8;
                FileSize += data[53];
                FileSize <<= 8;
                FileSize += data[54];

                Byte[] f = new byte[data.Length - 55];
                Array.Copy(data, 55, f, 0, data.Length - 55);
                return f;
            }
            catch { }
            return null;
        }

        // runs every 10 ms to send a file
        static void TimerTick(object stateInfo)
        {
            // check if we need to send something
            if (getSending() == false) return; // nothing to send

            // check the TX buffer, do not feed more data into
            // the buffer if it has already more than n entries
            if (Udp.GetBufferCount() > 3) return;

            Byte[] txarr = new byte[statics.PayloadLen];

            // check if txdata is smaller or equal one payload
            if (filestat == statics.FirstFrame)
            {
                // send the first frame
                if (txlen <= statics.PayloadLen)
                {
                    // we just need to send one frame
                    for (int i = 0; i < txdata.Length; i++)
                        txarr[i] = txdata[i];
                    txudp(txarr, txtype, statics.SingleFrame);
                    setSending(false);  // transmission complete
                }
                else
                {
                    // additional frame follows
                    // from txdata send one chunk of length statics.PayloadLen
                    // frame is repeated for preamble by hsmodem.cpp
                    Array.Copy(txdata, 0, txarr, 0, statics.PayloadLen);
                    txudp(txarr, txtype, statics.FirstFrame);
                    txpos = statics.PayloadLen;
                    filestat = statics.NextFrame;
                }
                return;
            }

            if (filestat == statics.NextFrame)
            {
                // check if this is the last frame
                int restlen = txlen - txpos;
                if(restlen <= statics.PayloadLen)
                {
                    // send as the last frame
                    Array.Copy(txdata, txpos, txarr, 0, restlen); // unused byte will be 0

                    // send the last frame a couple of times (see hsmodem.cpp)
                    txudp(txarr, txtype, statics.LastFrame);
                    setSending(false);  // transmission complete
                }
                else
                {
                    // additional frame follows
                    // from txdata send one chunk of length statics.PayloadLen
                    Array.Copy(txdata, txpos, txarr, 0, statics.PayloadLen);
                    txudp(txarr, txtype, statics.NextFrame);
                    txpos += statics.PayloadLen;
                }
                return;
            }
        }

        static void txudp(Byte[] txdata, Byte txtype, Byte filestat)
        {
            // add the txtype and filestatus at the beginning
            Byte[] darr = new byte[statics.PayloadLen + 2];
            darr[0] = txtype;
            darr[1] = filestat;
            Array.Copy(txdata, 0, darr, 2, statics.PayloadLen);
            Udp.UdpSendData(darr);
            // Console.WriteLine("TX filestat: " + filestat+ " data:" + darr[2].ToString("X2") + " " + darr[3].ToString("X2"));
        }
    }
}
