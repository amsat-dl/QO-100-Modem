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

void close_a();

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
    inp.real = f/5;
    inp.imag = 0;
    firfilt_crcf_push(qfilt, inp);      // push input sample
    firfilt_crcf_execute(qfilt, &outp); // compute output

    return outp.real;
}

// destination: 1=transceiver, 2=loudspeaker, 3=both
void playAudioPCM(char* fn, int destination)
{
    int len;
    int16_t d[100];
    printf("play:%s, caprate:%d\n", fn,caprate);
    FILE* fp = fopen(fn, "rb");
    if (fp)
    {
        while ((len = fread(d, sizeof(int16_t), 100, fp)))
        {
            for (int i = 0; i < len; i++)
            {
                float f = (float)d[i];

                // local playback at 48k, no filtering, no interpolation
                if ((destination & 2) == 2)
                {
                    int to = 4000;
                    int res;
                    while ((res = io_ls_fifo_usedspace()) > 10000)
                    {
                        if (--to == 0) 
                        {
                            printf("timed out waiting for LS fifo. Res:%d\n",res);
                            fclose(fp);
                            return;
                        }
                        sleep_ms(1);
                    }
                    io_ls_write_fifo(f / 32768);
                }

                if (caprate == 44100)
                {
                    unsigned int num_written = 0;
                    liquid_float_complex in;
                    liquid_float_complex out;
                    in.real = f;
                    in.imag = 0;
                    msresamp_crcf_execute(anndecim, &in, 1, &out, &num_written);
                    if (num_written != 1) continue;
                    f = out.real;
                }

                f = lowpass(f);
                f /= 32768;

                if ((destination & 1) == 1)
                {
                    int to = 4000;
                    while (io_pb_fifo_usedspace() > 10000)
                    {
                        if (--to == 0)
                        {
                            printf("timed out waiting for PB fifo\n");
                            fclose(fp);
                            return;
                        }
                        sleep_ms(1);
                    }
                    io_pb_write_fifo(f);
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

    io_clear_voice_fifos();
    create_a();
    playAudioPCM(fn, 3);
    close_a();
}
