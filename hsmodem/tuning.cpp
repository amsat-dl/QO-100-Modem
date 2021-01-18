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
* tuning.c ... generate tuning tones and feed audio to TRX
*
*/

#include "hsmodem.h"

#define NUMFREQ 15
int tunefreq[NUMFREQ] = { 100,300,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900 };
nco_crcf tunenco[NUMFREQ];
uint32_t tuning_runtime = 0;

void init_tune()
{
    static int f = 1;
    if (f)
    {
        for (int i = 0; i < NUMFREQ; i++)
            tunenco[i] = NULL;
        f = 0;
    }

    printf("init tune tones to samprate: %d\n", caprate);
    for (int i = 0; i < NUMFREQ; i++)
    {
        if (tunenco[i] != NULL) nco_crcf_destroy(tunenco[i]);

        // create NCO for frequency
        float rad_per_sample = ((2.0f * (float)M_PI * (float)tunefreq[i]) / (float)caprate);
        tunenco[i] = nco_crcf_create(LIQUID_NCO);
        nco_crcf_set_phase(tunenco[i], 0.0f);
        nco_crcf_set_frequency(tunenco[i], rad_per_sample);
    }
}

// send= 1 ... send all frequencies to soundcard
// send= 2,3,4 ... send a single freq to soundcard
float do_tuning(int send)
{
    if (send < 0 || send >= NUMFREQ) return 0.0f;

    float f = 0;
    if (send == 1)
    {
        for (int i = 0; i < NUMFREQ; i++)
        {
            nco_crcf_step(tunenco[i]);
            float v = nco_crcf_sin(tunenco[i]);
            f += v;
        }
        f /= 3;
    }
    else
    {
        nco_crcf_step(tunenco[send]);
        f = nco_crcf_sin(tunenco[send]);
    }

    // adapt speed to soundcard samplerate
    int fs;
    while (keeprunning)
    {
        fs = io_fifo_usedspace(io_pbidx);
        // wait until there is space in fifo
        if (fs < 48000) break;
        if (tuning == 0) return 0.0f;
        sleep_ms(10);
    }

    f *= 0.05f;
    kmaudio_playsamples(io_pbidx, &f, 1, pbvol);

    if (++tuning_runtime >= (48000 * 5 * 60))
    {
        // 5min max time
        printf("Maximum tuning time, switch off tuning\n");
        tuning = 0;
    }
    return f;
}

float singleFrequency()
{
    int i = 0; // 100 Hz
    if (tunenco[i] == NULL) return 0.0f;
    nco_crcf_step(tunenco[i]);
    float f = nco_crcf_sin(tunenco[i]);

    return f;
}
