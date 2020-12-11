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

#include "hsmodem.h"
#ifdef _WIN32_
#include "fftw_lib/fftw3.h"
#endif
#ifdef _LINUX_
#include <fftw3.h>
#endif

#define FFT_AUDIOSAMPLERATE 8000

double *din = NULL;		// input data for  fft
fftw_complex *cpout = NULL;	    // ouput data from fft
fftw_plan plan = NULL;
#define fft_rate  (FFT_AUDIOSAMPLERATE / 10)       // resolution: 10 Hz
int fftidx = 0;
int fftcnt = fft_rate/2+1;      // number of output values
uint16_t fftout[FFT_AUDIOSAMPLERATE / 10/2+1];
float f_fftout[FFT_AUDIOSAMPLERATE / 10 / 2 + 1];
int downsamp = 0;
int downphase = 0;
int rxlevel_deteced = 0;
int rx_in_sync = 0;

uint16_t *make_waterfall(float fre, int *retlen)
{
    // Downsampling:
    // needed 8000 bit/s
    // caprate 48k: downsample by 6
    // caprate 44,1k: downsample by 5,5

    if (physcaprate == 48000)
        if (++downsamp < 6) return NULL;

    // TODO: the following simple resamp results in double peeks in fft
    if (physcaprate == 44100)
    {
        if (downphase <= 1100)
        {
            if (++downsamp < 5) return NULL;
        }
        else
        {
            if (++downsamp < 6) return NULL;
        }
        if(++downphase >= 2000) downphase = 0;
    }
    downsamp = 0;

    int fftrdy = 0;
        
    // fre are the float samples
    // fill into the fft input buffer
    din[fftidx++] = fre;
        
    if(fftidx == fft_rate)
    {
        fftidx = 0;

        // the fft buffer is full, execute the FFT 
        fftw_execute(plan);
            
        for (int j = 0; j < fftcnt; j++)
        {
            // calculate absolute value (magnitute without phase)
            float fre = (float)cpout[j][0];
            float fim = (float)cpout[j][1];
            float mag = sqrt((fre * fre) + (fim * fim));

            fftout[j] = (uint16_t)mag;
            f_fftout[j] = mag;
                
            fftrdy = 1;
        }

        if (rx_in_sync == 0)
        {
            // signal detection
            // measure level at band edges
            float edgelevel = 0;
            for (int e = 0; e < 10; e++)
                edgelevel += f_fftout[e];
            edgelevel /= 10;

            for (int e = 300; e < 320; e++)
                edgelevel += f_fftout[e];
            edgelevel /= 20;

            // measure level at mid band
            float midlevel = 0;
            for (int e = 100; e < 300; e++)
                midlevel += f_fftout[e];
            midlevel /= 200;

            //calc difference in %
            int idiff = (int)((edgelevel * 100) / midlevel);
            //printf("diff:%d %%  edge:%10.6f midband:%10.6f\n", idiff,edgelevel, midlevel);

            // idiff SDR Console:
            // no signal ... > 100
            // signal < 20
            static int checks = 0;
            static int lastsig = 0;
            int sig = 0;

            // check if signal detected or not
            if (idiff > 100) sig = 0;
            if (idiff < 20) sig = 1;

            rxlevel_deteced = sig;

            // check if changed since last check
            if (sig != lastsig)
            {
                lastsig = sig;
                checks = 0;
            }
            else
            {
                if (checks <= 3) checks++;
                if (checks == 3)
                {
                    if (sig == 1)
                    {
                        printf("===>>> level detected, reset modem\n");
                        trigger_resetmodem = 1;
                    }
                }
            }
        }
    }
    
    if(fftrdy == 1)
    {
        *retlen = fftcnt;
        return fftout;
    }
    
    return NULL;
}

void init_fft()
{
	/* 
char fn[300];
    * storing to a file in the working directory may be a problem under Windows, so we do not use wisdom files
    sprintf(fn, "capture_fft_%d", fft_rate);	// wisdom file for each capture rate

	fftw_import_wisdom_from_filename(fn);*/

	din = (double *)fftw_malloc(sizeof(double) * fft_rate);
	cpout = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * fft_rate);

	plan = fftw_plan_dft_r2c_1d(fft_rate, din, cpout, FFTW_MEASURE);
	
	//fftw_export_wisdom_to_filename(fn);
}

void exit_fft()
{
    if(plan) fftw_destroy_plan(plan);
	if(din) fftw_free(din);
	if(cpout) fftw_free(cpout);
}
