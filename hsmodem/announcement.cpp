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
* announcement.c ... sends the *.pcm files to the playback device
*
*
*/

#include "hsmodem.h"

const int h_len = 57;
float h[h_len];
firfilt_crcf qfilt = NULL;
float ratio;
msresamp_crcf anndecim = NULL;

void create_a()
{
    close_a();

    // audio filter to reduce Audio BW for the 2k7 SSB channel
    float fc = 0.055f; // cut off freq. normalized to 48000 (= 1.0)
    liquid_firdes_kaiser(h_len, fc, 60.0f, 0.0f, h);
    qfilt = firfilt_crcf_create(h, h_len);

    // create arbitrary pre decimator
    // if Audio SR is 48000 but caprate is 44100
    ratio = (float)((float)caprate / 48000.0);
#ifdef _WIN32_
    /*
    * only Windows needs this special resampling ratio
    * maybe it has to do with the VAC, I don't know why we have
    * to resample twice 48k -> 44.1k
    * maybe this will not work on another PC
    */
    /*
    if(caprate == 44100)
        ratio = (float)(40526.0f / 48000.0);
    else 
        ratio = (float)(44100.0f / 48000.0);
    */
    if (caprate == 48000)
        ratio = (float)(48000.0f / 44100.0);
    else
        ratio = 1.0f;
#endif
    anndecim = msresamp_crcf_create(ratio, 40.0f);
}

void close_a()
{
    if(qfilt) firfilt_crcf_destroy(qfilt);
    qfilt = NULL;
    if (anndecim) msresamp_crcf_destroy(anndecim);
    anndecim = NULL;
}

float lowpass(float f)
{
    if (qfilt == NULL)
    {
        printf("low pass filter not initialized\n");
        return f;
    }
    liquid_float_complex inp, outp;
    inp.real = f;
    inp.imag = 0;
    firfilt_crcf_push(qfilt, inp);      // push input sample
    firfilt_crcf_execute(qfilt, &outp); // compute output

    return outp.real;
}

int measureLevel(char* fn)
{
    int max = 0;
    FILE* fp = fopen(fn, "rb");
    if (fp)
    {
        int len = 0;
        int16_t v;
        while ((len = fread(&v, sizeof(int16_t), 1, fp)))
        {
            if (v > max) max = v;
        }
        fclose(fp);
    }
    return max;
}

// destination: 1=transceiver, 2=loudspeaker, 3=both
void playAudioPCM(char* fn, int destination)
{
    int len;
    int16_t d[100];
    printf("play:%s, caprate:%d\n", fn,caprate);
    int max = measureLevel(fn);
    float ampl = 32767.0f / (float)max;
    //printf("max:%d ampl:%f\n", max, ampl);
    FILE* fp = fopen(fn, "rb");
    const float ann_volume = 0.1f;  // volume reduction for announcement
    if (fp)
    {
        while ((len = fread(d, sizeof(int16_t), 100, fp)))
        {
            for (int i = 0; i < len; i++)
            {
                float f = (float)d[i];  // 16-bit values
                f /= 32768;             // float values 0..1
                f *= ampl;              // normalize volume
                f *= ann_volume;        // reduce volume

                // local playback at 48k, no filtering, no interpolation
                if ((destination & 2) == 2)
                {
                    int to = 4000;
                    int res;
                    while ((res = io_fifo_usedspace(voice_pbidx)) > 10000)
                    {
                        if (--to == 0) 
                        {
                            printf("timed out waiting for LS fifo. Res:%d\n",res);
                            fclose(fp);
                            return;
                        }
                        sleep_ms(1);
                    }
                    kmaudio_playsamples(voice_pbidx, &f, 1, lsvol);
                }

                // resample if required (PCM files are always 48000)
                unsigned int num_written = 1;
                liquid_float_complex out[10];
                out[0].real = f; // value, if no resampling
#ifdef _LINUX_
                if (caprate == 44100)
#endif
                {
                    liquid_float_complex in;
                    in.real = f;
                    in.imag = 0;
                    msresamp_crcf_execute(anndecim, &in, 1, out, &num_written);
                }
                for (unsigned int n = 0; n < num_written; n++)
                {
                    f = out[n].real;
                    f = lowpass(f);

                    if ((destination & 1) == 1)
                    {
                        int to = 4000;
                        while (io_fifo_usedspace(io_pbidx) > 10000)
                        {
                            if (--to == 0)
                            {
                                printf("timed out waiting for PB fifo\n");
                                fclose(fp);
                                return;
                            }
                            sleep_ms(1);
                        }
                        kmaudio_playsamples(io_pbidx, &f, 1, pbvol);
                    }
                }

            }
            if (len != 100) break;
        }
        fclose(fp);
        printf("finished playing audio file\n");
    }
    else
        printf("audio file not found\n");
}

int ann_running = 0;
int transmissions = 10000;

void sendAnnouncement()
{
    if (announcement == 0) return;

    if (++transmissions >= announcement)
    {
        create_a();

        ann_running = 1;
        transmissions = 0;

        if (sendIntro == 1)
        {
            char fn[500];
            snprintf(fn, 499, "%s/oscardata/intro/intro.pcm", homepath);
            fn[499] = 0;
            playAudioPCM(fn, 1);
        }

        playAudioPCM("audio/amsat.pcm",1);
        if (bitsPerSymbol == 1) playAudioPCM("audio/bpsk.pcm",1);
        else if (bitsPerSymbol == 2) playAudioPCM("audio/qpsk.pcm",1);
        else playAudioPCM("audio/psk8.pcm",1);

        char s[100];
        sprintf(s,"audio/%d.pcm", linespeed);
        playAudioPCM(s,1);

        playAudioPCM("audio/kbps.pcm",1);

        ann_running = 0;

        close_a();
    }
}

void playIntro()
{
    char fn[500];
    snprintf(fn, 499, "%s/oscardata/intro/intro.pcm", homepath);
    fn[499] = 0;

    io_fifo_clear(voice_pbidx);
    io_fifo_clear(voice_capidx);
    create_a();
    playAudioPCM(fn, 3);
    close_a();
}
