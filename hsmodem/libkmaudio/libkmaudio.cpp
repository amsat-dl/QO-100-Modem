/*
* Audio Library for Linux and Windows
* ===================================
* Author: DJ0ABR
*
* Author: Kurt Moraw, Ham radio: DJ0ABR, github: dj0abr
* License: GPL-3
*
* compilation:
* Windows ... Visual Studio
* Linux ... make
* 
* Documentation see: libkmaudio.h
* Usage Example: see main() in this file
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
* libkmaudio.cpp ... main() for test purposes only
* usually this library is linked to another program, in this case
* comment-out #define LIBTEST
*
*/

#include "libkmaudio.h"

int kmaudio_getDeviceList_test();

int keeprunning = 1;

/*
* main()
* for testing purposes only
* for library generation comment out: LIBTEST
*/


#define LIBTEST

#ifdef LIBTEST
int main()
{
    // initialize sound system
    // must be called once after program start
    // if called during program run, this will reset the sound system, so better don't do it
	kmaudio_init();
	
    // read list of devices
    // call as often as needed
    // if a user pluggs-in an USB device on the fly then the running stream may
    // be redirected by the OS. In this case closing/opening the stream
    // may be required.
	kmaudio_getDeviceList();
    
    // start capture and/or playback streams
    // Parameter: the device name and the sample rate (44100 or 48000 are supported)
    // these function return the fifo-index, which is used to access the data in the
    // coresponding fifo

	int capidx = kmaudio_startCapture((char *)"Line 2 (Virtual Audio Cable)", 48000);
    int pbidx = kmaudio_startPlayback((char *)"Line 2 (Virtual Audio Cable)", 48000);
    // int ucapidx = kmaudio_startCapture((char*)"Mikrofon (2- USB Advanced Audio Device)", 48000);
    // int upbidx = kmaudio_startPlayback((char*)"Lautsprecher (2- USB Advanced Audio Device)", 48000);
    //int capidx = kmaudio_startCapture((char*)"USB Audio CODEC: - (hw:2,0)", 48000);
    // int capidx = kmaudio_startCapture((char*)"Mikrofon (1080P Webcam)", 48000);
    // int pbidx = kmaudio_startPlayback((char*)"Lautsprecher (2- High Definition Audio Device)", 48000);
    //int pbidx = kmaudio_startPlayback((char*)"USB Audio CODEC: - (hw:2,0)", 48000);

    //int capidx = kmaudio_startCapture((char*)"PCM2902 Audio Codec Analog Stereo", 48000);
    //int pbidx = kmaudio_startPlayback((char*)"PCM2902 Audio Codec Analog Stereo", 48000);
    //int ucapidx = kmaudio_startCapture((char*)"USB Advanced Audio Device Analog Stereo", 48000);
    //int upbidx = kmaudio_startPlayback((char*)"USB Advanced Audio Device Analog Stereo", 48000);

    if (1)
    {

        int16_t f[1100]; // 1.1 x need rate to have reserve for resampler
        int16_t fv = -32768;
        /*for (int i = 0; i < 48000; i++)
        {
            kmaudio_playsamples(pbidx, &fv, 1, 1);
            if (++fv >= 32768) fv = -32768;
        }*/
        while (1)
        {
            int av = io_fifo_elems_avail(pbidx);
            if (av < 10000)
            {
                //printf("av: %d\n", av);
                for (int i = 0; i < 10000; i++)
                {
                    kmaudio_playsamples(pbidx, &fv, 1, 1);
                    if (++fv >= 32768) fv = -32768;
                    //if (fv >= -1010 && fv <= -1000) fv = 1000;  // avoid 0, for dropout testing
                }
                av = io_fifo_elems_avail(pbidx);
                //printf("ava: %d\n", av);
            }
            int anz = kmaudio_readsamples(capidx, f, 20, 1, 0);
            if (anz > 0)
            {
                /*char s[200];
                s[0] = 0;
                for(int i=0; i<anz; i++)
                    sprintf(s+strlen(s),"%d ", f[i]);
                printf("%s\n",s);*/
                static int16_t lf=-32768;
                for (int i = 0; i < anz; i++)
                {
                    if (f[i] != lf)
                    {
                        printf("error: %d %d\n",f[i], lf);
                        lf = f[i];
                    }
                    lf++;
                }
            }
        }

        /*
        while (1)
        {
            // make a loop: record from Mic and play to Speaker

            int done = 0;
            // read samples from the capture fifo
            int anz = kmaudio_readsamples(capidx, f, 1000, 1.0f, 0);
            if (anz > 0)
            {
                // if samples are available, send them to playback fifo
                //printf("write %d samples from %d to %d\n", anz, capidx, pbidx);
                kmaudio_playsamples(pbidx, f, anz,1.0f);
                done = 1;
            }
            int uanz = kmaudio_readsamples(ucapidx, f, 1000, 1.0f, 0);
            if (uanz > 0)
            {
                // if samples are available, send them to playback fifo
                //printf("write %d samples from %d to %d\n", anz, capidx, pbidx);
                kmaudio_playsamples(upbidx, f, uanz, 1.0f);
                done = 1;
            }
            if (done == 0)
            {
                // if no samples are available make a short break
                // this is important to prevent excessive CPU usage
                sleep_ms(10);
            }
        }*/
    }
    else
        printf("device not available. Ending program\n");

    // free resources. This will never happen in this example
    // but should be done in the final program
	kmaudio_close();
	return 0;
}
#endif // LIBTEST
