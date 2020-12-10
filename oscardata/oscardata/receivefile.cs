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
* File restauration process
* =========================
* 1) file header is received, get filename and file ID (which is the CRC16 over the complete file contents)
* 2) is this file already existing ? yes-> 3)
* 3) this file does not exist -> 4)
* 
* 3) file with this name and ID exists is already, cancel reception, show file
* 
* 4) file dows not exist, receive the blocks
* 5) a block is missing -> find a previous block-file with this name+ID. If exists, take the block from this file
* 6) reception complete, the files is OK -> save the file, delete a block file
* 7) reception complete, the files is incomplete -> save the block file for late use
* 
* Filenames:
* ----------
* transmitter:
* the sender takes any filename, but the ID is the C&C16 over the file contents (to identify different files with the same name)
* 
* receiver:
* use the filename as transmitted for the good original file
* the blockfile's filename is the ID in Ascii-Hex representation, i.e.: ID= 0x45 0xfe ... filename is: 45FE.blk
*/

using System;
using System.Drawing;
using System.IO;

namespace oscardata
{
    class receivefile
    {
        int rxtype;
        int rxfrmnum;
        int minfo;
        int rxstat;
        int speed;
        int maxlevel;
        int dummy4;
        int dummy5;
        Byte[] rxdata = new byte[statics.PayloadLen];

        // file buffer, we have max 2^10=1024 blocks with 219 bytes each = 224.256kB
        Byte[,] blockbuf = new byte[1024, statics.PayloadLen];
        bool[] blockvalid = new bool[1024];
        int blockidx;
        Byte[] firstblock;

        bool[] lastblockvalid = new bool[1024];
        int lastblockidx;

        bool receiving = false;
        public String filename = null;
        public String StatusText = "";
        public long filesize = 0;
        public Bitmap pbmp = null;
        DateTime starttime;
        public TimeSpan runtime;
        public int rxbytes = 0;
        bool autoRXnum = false;
        String blockFilename = "";

        //  1 ... file OK
        //  0 ... continue receiving
        // -1 ... invalid header
        // -2 ... first frame invalid
        // -3 ... invalid data in cont frame
        // -4 ... invalid data in last frame
        // -5 ... file ends, but bad blocks

        public int receive(Byte []rxdp)
        {
            // read frame header
            if(!getFrameHeader(rxdp))
            {
                // invalid situation
                blockidx = 0;
                receiving = false;
                return -1;
            }

            // receive first frame of a transmission
            if (minfo == statics.FirstFrame || minfo == statics.SingleFrame)
            {
                receiving = false;
                starttime = DateTime.UtcNow;
                rxbytes = 0;
                filesize = 0;
                filename = "";
                if (!StartFileRX()) return -2;   // invalid file
            }

            if (receiving && minfo != statics.FirstFrame)
                runtime = DateTime.UtcNow - starttime;

            // receive continous frames of a transmission
            if (minfo == statics.NextFrame)
            {
                // there are more frames for this file
                if(!FileRX())
                {
                    // invalid situation
                    blockidx = 0;
                    receiving = false;
                    return -3;
                }
            }

            rxbytes += statics.PayloadLen;

            // receive last file of a transmission
            if (minfo == statics.LastFrame || minfo == statics.SingleFrame)
            {
                if (!FileRX())
                {
                    // invalid situation
                    blockidx = 0;
                    receiving = false;
                    return -4;
                }

                // the last block was received ok
                // save file if all blocks valid
                bool fileisok = SaveFile();
                if(fileisok == false)
                {
                    // file not complete, do not to unzip
                    return -5;
                }
                blockidx = 0;
                receiving = false;

                if (rxtype == statics.AsciiFile || rxtype == statics.HTMLFile || rxtype == statics.BinaryFile)
                {
                    // these file type must be unzipped
                    handleZIPfiles();
                    receiving = false;
                    return 1;
                }
            }

            if (rxtype == statics.Image)
            {
                // build bitmap from received data
                pbmp = buildBitmap();
                return 1;
            }

            return 0;
        }

        bool getFrameHeader(Byte[] rxd)
        {
            rxtype = rxd[0];
            rxfrmnum = rxd[1];
            rxfrmnum <<= 8;
            rxfrmnum += rxd[2];
            minfo = rxd[3];
            rxstat = rxd[4];
            speed = rxd[5];
            speed <<= 8;
            speed += rxd[6];
            maxlevel = rxd[7];
            dummy4 = rxd[8];
            dummy5 = rxd[9];

            if (rxfrmnum >= 1024) return false;
            if (!autoRXnum)
                blockidx = rxfrmnum;

            Array.Copy(rxd, 10, rxdata, 0, rxd.Length - 10);
            return true;
        }

        bool StartFileRX()
        {
            if (receiving) return true; // already open

            //Console.WriteLine("first block");

            // store first block
            filename = null;
            if (rxfrmnum != 0)
            {
                Console.WriteLine("blockidx auto increment");
                autoRXnum = true;    // for old compatibility, increment blockidx by ourself
            }
            blockidx = 0;
            if (rxdata.Length > statics.PayloadLen)
            {
                Console.WriteLine("wrong payload size: " + rxdata.Length + " expected:" + statics.PayloadLen);
                return false; // wrong size
            }

            // read file header
            ArraySend.rxFilename = "";
            firstblock = ArraySend.GetAndRemoveHeader(rxdata);
            if (firstblock == null)
            {
                Console.WriteLine("invalid File header");
                return false;   // cannot read header, file is corrupted, ignore
            }

            // get block filename for this file
            blockFilename = idToFilename(ArraySend.FileID);

            // init valid blocks table
            for(int i=0; i<blockvalid.Length; i++)
                blockvalid[i] = false;

            // insert previously received blocks
            readBlocks();

            // insert first block
            for (int i = 0; i < rxdata.Length; i++)
                blockbuf[blockidx, i] = rxdata[i]; // contains file info header and file data

            // mark first block as valid
            blockvalid[blockidx] = true;
            
            receiving = true;

            return true;
        }

        String idToFilename(int id)
        {
            char hexToChar(Byte h)
            {
                if (h >= 0 && h <= 9) return (char)(0x30 + h);
                return (char)(0x41 + h - 10);
            }

            Byte b0 = (Byte)((id >> 12) & 0x0f);
            Byte b1 = (Byte)((id >> 8) & 0x0f);
            Byte b2 = (Byte)((id >> 4) & 0x0f);
            Byte b3 = (Byte)(id & 0x0f);

            String s = "";
            s += hexToChar(b0);
            s += hexToChar(b1);
            s += hexToChar(b2);
            s += hexToChar(b3);
            s += ".blk";

            return statics.getHomePath("blocks", s);
        }

        public bool blockstat(int[] result)
        {
            if (blockidx == 0) return false;
            int ok = 0;
            for (int i = 0; i <= blockidx; i++)
            {
                if (blockvalid[i]) ok++;
            }
            result[0] = blockidx+1; // +1 because we start with block 0
            result[1] = ok;

            Array.Copy(blockvalid, lastblockvalid, blockvalid.Length);
            lastblockidx = blockidx;

            return true;
        }

        public void oldblockinfo(int[] result)
        {
            int ok = 0;
            for (int i = 0; i <= lastblockidx; i++)
                if (lastblockvalid[i]) ok++;
            result[0] = lastblockidx + 1; // +1 because we start with block 0
            result[1] = ok;
        }

        public String missingBlockString()
        {
            String s = "";
            bool f = true;
            for (int i = 0; i <= lastblockidx; i++)
            {
                if (!lastblockvalid[i])
                {
                    if (!f)
                        s += ", ";
                    else
                        f = false;

                    s += i.ToString();
                }
            }

            return s;
        }

        void saveBlocks()
        {
            try
            {
                using (BinaryWriter writer = new BinaryWriter(File.Open(blockFilename, FileMode.Create)))
                {
                    for (int i = 0; i <= blockidx; i++)
                    {
                        // first byte of a block: valid/invalid block
                        Byte[] blk = new byte[statics.PayloadLen + 1];
                        for (int j = 0; j < statics.PayloadLen; j++)
                            blk[j + 1] = blockbuf[i, j];

                        blk[0] = blockvalid[i] ? (Byte)1 : (Byte)0;

                        writer.Write(blk);
                    }
                    writer.Close();
                }
            }
            catch { }
        }

        void readBlocks()
        {
            Byte[] blk = new byte[statics.PayloadLen + 1];
            int idx = 0;

            try
            {
                using (BinaryReader reader = new BinaryReader(File.Open(blockFilename, FileMode.Open)))
                {
                    while (true)
                    {
                        int ret = reader.Read(blk, 0, statics.PayloadLen + 1);
                        if (ret != statics.PayloadLen + 1) break;

                        for (int j = 0; j < statics.PayloadLen; j++)
                        {
                            if (blk[0] == 1)
                            {
                                blockbuf[idx, j] = blk[j + 1];
                                blockvalid[idx] = true;
                            }
                        }
                        idx++;
                    }
                    reader.Close();
                }
            }
            catch { }
        }

        bool FileRX()
        {
            if (!receiving)
            {
                //Console.WriteLine("next/last block: not receiving, first block missing?");
                return false;
            }

            //Console.WriteLine("next/last block");

            // store next block
            if (rxdata.Length != statics.PayloadLen)
            {
                Console.WriteLine("wrong payload size: " + rxdata.Length + " expected:" + statics.PayloadLen);
                return false; // wrong size
            }

            if (autoRXnum)
            {
                blockidx++;
                Console.WriteLine("blockidx: do auto increment " + blockidx);
            }

            for (int i = 0; i < rxdata.Length; i++)
                blockbuf[blockidx, i] = rxdata[i];

            blockvalid[blockidx] = true;

            return true;
        }

        // build full path+filename from received filename
        String makeRXfilename()
        {
            String fn = "";
            // remove possible path from filename
            String fname = ArraySend.rxFilename;
            int idx = fname.IndexOfAny(new char[] { '\\', '/' });
            if (idx != -1)
            {
                try
                {
                    fname = fname.Substring(idx + 1);
                }
                catch { }
            }
            if (rxtype == statics.Image)
                fn = statics.getHomePath(statics.RXimageStorage, fname);
            else
                fn = statics.getHomePath("", fname);
            return fn;
        }

        bool SaveFile()
        {
            Console.WriteLine("save file");

            // check if all blocks ok
            for (int i = 0; i <= blockidx; i++)
            {
                if (blockvalid[i] == false)
                {
                    Console.WriteLine("save file: not all blocks ok");
                    // save block file
                    saveBlocks();
                    return false;
                }
            }

            // reception was ok, blockfile no more needed, delete it
            // file is OK, blockfile no more needed, delete it
            try { statics.FileDelete(blockFilename); } catch { }

            // make filename
            filename = makeRXfilename();

            Console.WriteLine("save at " + filename);

            try
            {
                using (BinaryWriter writer = new BinaryWriter(File.Open(filename, FileMode.Create)))
                {
                    for (int i = 0; i <= blockidx; i++)
                    {
                        if (i == 0)
                            writer.Write(firstblock);
                        else
                        {
                            Byte[] blk = new byte[statics.PayloadLen];
                            for (int j = 0; j < statics.PayloadLen; j++)
                                blk[j] = blockbuf[i, j];

                            writer.Write(blk);
                        }
                    }
                    writer.Close();
                }
            }
            catch
            {
            }

            return true;
        }

        Bitmap buildBitmap()
        {
            Bitmap bmp = null;

            using (MemoryStream memStream = new MemoryStream())
            {
                for (int i = 0; i <= blockidx; i++)
                {
                    if (i == 0)
                        memStream.Write(firstblock, 0, firstblock.Length);
                    else
                    {
                        Byte[] blk = new byte[statics.PayloadLen];
                        for (int j = 0; j < statics.PayloadLen; j++)
                            blk[j] = blockbuf[i, j];

                        memStream.Write(blk, 0, blk.Length);
                    }
                }
                try
                {
                    // first assign it to an "Image", this checks if the image is valid
                    Image img = Image.FromStream(memStream);
                    bmp = new Bitmap(img);
                }
                catch
                {
                    return null;
                }
            }

            return bmp;
        }

        void handleZIPfiles()
        {
            if (filename == null)
            {
                Console.WriteLine("handleZIPfile: no filename");
                return;
            }

            try
            {
                // filename has the received data, but maybe too long (multiple of payload length)
                // reduce for the real file length
                Byte[] fc = File.ReadAllBytes(filename);
                Byte[] fdst = new byte[ArraySend.FileSize];
                if (fc.Length < ArraySend.FileSize)
                {
                    Console.WriteLine("file not complete: got len=" + fc.Length + " expected len=" + ArraySend.FileSize);
                    return;
                }
                Array.Copy(fc, 0, fdst, 0, ArraySend.FileSize);
                File.WriteAllBytes(statics.zip_RXtempfilename, fdst);   // the received file (still zipped) is here

                // unzip received data and store result in file: unzipped_RXtempfilename
                ZipStorer zs = new ZipStorer();
                String fl = zs.unzipFile(statics.zip_RXtempfilename);
                if (fl != null)
                {
                    // save file
                    // fl is the filename of the file inside the zip file, so the originally zipped file
                    // remove path to get just the filename
                    int idx = fl.LastIndexOf('/');
                    if (idx == -1) idx = fl.LastIndexOf('\\');
                    String fdest = fl.Substring(idx + 1);
                    fdest = statics.getHomePath("", fdest);
                    // fdest is the file in the oscardata's user home directoty
                    // remove old file with same name
                    try { statics.FileDelete(fdest); } catch { }
                    // move the unzipped file to the final location
                    File.Move(fl, fdest);
                    filesize = statics.GetFileSize(fdest);
                    StatusText = "unzip OK";
                }
                else
                    StatusText = "unzip failed";

                statics.FileDelete(statics.zip_RXtempfilename);
            }
            catch
            {
                StatusText = "unzip failed";
            }
        }
    }
}
