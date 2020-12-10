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

typedef struct _AUDIOFILES_ {
    char fn[256];
    int duration;
} AUDIOFILES;

AUDIOFILES audiofiles[12] =
{
    {"amsat", 1100},
    {"qpsk", 1100},
    {"psk8", 1100},
    {"3000", 600},
    {"4000", 600},
    {"4410", 900},
    {"4800", 900},
    {"5500", 900},
    {"6000", 600},
    {"6600", 900},
    {"7200", 900},
    {"kbps", 1000},
};

char* getAudiofn(int aidx, char* ext)
{
    static char filename[300];
    strcpy(filename, "audio/");
    strcat(filename, audiofiles[aidx].fn);
    strcat(filename, ext);
    return filename;
}

void playAudioFLAC(int aidx)
{
    int resamp = 0;
    int len;
    int16_t d[100];
    printf("play:%s, caprate:%d\n", getAudiofn(aidx, ".pcm"),caprate);
    FILE* fp = fopen(getAudiofn(aidx, ".pcm"), "rb");
    if (fp)
    {
        while ((len = fread(d, sizeof(int16_t), 100, fp)))
        {
            for (int i = 0; i < len; i++)
            {
                float f = (float)d[i];
                f /= 32768;
                io_pb_write_fifo(f);

                if (caprate == 48000)
                {
                    if (++resamp >= 9)
                    {
                        resamp = 0;
                        io_pb_write_fifo(f);
                    }
                }

                // sync with soundcard
                while (io_pb_fifo_usedspace() > 10000) sleep_ms(1);
            }
            if (len != 100) break;
        }
        fclose(fp);
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
        ann_running = 1;
        transmissions = 0;

        playAudioFLAC(0);
        if (bitsPerSymbol == 2) playAudioFLAC(1);
        else playAudioFLAC(2);
        switch (linespeed)
        {
        case 3000: playAudioFLAC(3); break;
        case 4000: playAudioFLAC(4); break;
        case 4410: playAudioFLAC(5); break;
        case 4800: playAudioFLAC(6); break;
        case 5500: playAudioFLAC(7); break;
        case 6000: playAudioFLAC(8); break;
        case 6600: playAudioFLAC(9); break;
        case 7200: playAudioFLAC(10); break;
        }
        playAudioFLAC(11);

        ann_running = 0;
    }
}
