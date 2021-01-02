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

uint16_t* mean(uint16_t* f);

#define FFT_AUDIOSAMPLERATE 800

double *din = NULL;		// input data for  fft
fftw_complex *cpout = NULL;	    // ouput data from fft
fftw_plan plan = NULL;
int fftidx = 0;
int fftcount; // number of output values
uint16_t fftout[FFT_AUDIOSAMPLERATE / 2 + 1];
float f_fftout[FFT_AUDIOSAMPLERATE / 2 + 1];
int downsamp = 0;
int downphase = 0;
int rxlevel_deteced = 0;
int rx_in_sync = 0;
msresamp_crcf fftdecim = NULL;

float doublePeak(float *f_fftout, int e)
{
    // measure level at rtty freq
    // -100..-70 and +70..+100
    float v = 0;
    v += f_fftout[e - 7];
    v += f_fftout[e - 8];
    v += f_fftout[e - 9];
    v += f_fftout[e - 10];

    v += f_fftout[e + 7];
    v += f_fftout[e + 8];
    v += f_fftout[e + 9];
    v += f_fftout[e + 10];

    return v;
}

uint16_t *make_waterfall(float fre, int *retlen)
{
    int fftrdy = 0;

    // data come with 44.1k or 48k sample rate
    // downsample to 800 to get 0-4k in 10 Hz steps
    unsigned int num_written = 0;
    liquid_float_complex in;
    liquid_float_complex out;
    in.real = fre;
    in.imag = 0;
    msresamp_crcf_execute(fftdecim, &in, 1, &out, &num_written);
    if (num_written != 1) return NULL;
    fre = out.real;
        
    // fre are the float samples
    // fill into the fft input buffer
    din[fftidx++] = fre;
        
    if(fftidx == FFT_AUDIOSAMPLERATE)
    {
        fftidx = 0;

        // the fft buffer is full, execute the FFT 
        fftw_execute(plan);
            
        for (int j = 0; j < fftcount; j++)
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
            if (speedmode == 10)
            {
                // RTTY
                int mid = (rtty_frequency - 170 / 2) / 10;
                int lowlow = mid - 5;
                int lowhigh = mid + 5;
                mid = (rtty_frequency + 170 / 2) / 10;
                int highlow = mid - 5;
                int highhigh = mid + 5;
                for (int e = lowlow; e < lowhigh; e++)
                    midlevel += f_fftout[e];
                for (int e = highlow; e < highhigh; e++)
                    midlevel += f_fftout[e];
                midlevel /= ((lowhigh-lowlow) + (highhigh-highlow));
            }
            else
            {
                for (int e = 100; e < 200; e++)
                    midlevel += f_fftout[e];
                midlevel /= 100;
            }

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
            if (idiff < 50) sig = 1;

            rxlevel_deteced = sig;

            if (speedmode == 10 && rtty_autosync == 1)
            {
                // find an RTTY signal
                // from 200 to 2800 Hz look for the beste double peak
                float dp = 0;
                int dpidx = 0;

                for (int e = 20; e < 280; e++)
                {
                    float d = doublePeak(f_fftout, e);
                    if (d > dp)
                    {
                        dp = d;
                        dpidx = e;
                    }
                }

                //printf("Signal at: %d Hz\n", (int)(dpidx * 10));

                // accept if we get 3 equal values after each other
                const static int simi = 3;
                static int simiarr[simi];
                static int simiidx = 0;
                simiarr[simiidx] = (int)(dpidx * 10);
                if (++simiidx >= simi) simiidx = 0;

                int cp0 = simiarr[0];
                for (int i = 1; i < simi; i++)
                    if (simiarr[i] < (cp0-10) || simiarr[i] > (cp0 + 10)) cp0 = 0;

                if (cp0 > 0)
                {
                    // mid value of last "arl" frequencies
                    const static int arl = 10;
                    static int fra[arl];
                    static int fraidx = 0;
                    fra[fraidx] = cp0;
                    if (++fraidx >= arl) fraidx = 0;
                    int fm = 0;
                    for (int i = 0; i < arl; i++)
                        fm += fra[i];
                    fm /= arl;

                    static int lastfm = 0;
                    if (fm == lastfm)
                    {
                        rtty_modifyRXfreq(fm);
                    }
                    lastfm = fm;
                }
            }

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
        *retlen = fftcount;
        return mean(fftout);
    }
    
    return NULL;
}

// smooth fft output
const int smoothX = 2; // must be an even number !
const int smoothY = 10;
int yidx = 0;

uint16_t* mean(uint16_t* f)
{
    
    static uint16_t fa[FFT_AUDIOSAMPLERATE / 2 + 1];

    if (tuning)
        return f;
    
    // first smooth X values
    for (int x = 0; x < smoothX / 2; x++)
        fa[x] = f[x];

    for (int x = smoothX / 2; x < fftcount - smoothX / 2; x++)
    {
        fa[x] = 0;
        for (int i = -smoothX/2; i < smoothX/2; i++)
            fa[x] += f[x+i];
        fa[x] /= smoothX;
    }

    for (int x = fftcount - smoothX / 2; x < fftcount; x++)
        fa[x] = f[x];
    
    // smooth Y values
    
    static uint16_t yarr[smoothY][FFT_AUDIOSAMPLERATE / 2 + 1];
    for (int i = 0; i < fftcount; i++)
        yarr[yidx][i] = fa[i];
    if (++yidx >= smoothY) yidx = 0;

    memset(fa, 0, FFT_AUDIOSAMPLERATE / 2 + 1);
    for (int i = 0; i < fftcount; i++)
    {
        for (int j = 0; j < smoothY; j++)
            fa[i] += yarr[j][i];
        fa[i] /= smoothY;
    }

    return fa;
}

void _init_fft()
{
    fftcount = FFT_AUDIOSAMPLERATE / 2 + 1;     // number of output samples
    // the FFT outputs 400 values from 0 to 4kHz with a resolution of 10 Hz

    _exit_fft();
	din = (double *)fftw_malloc(sizeof(double) * FFT_AUDIOSAMPLERATE);
	cpout = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * fftcount);

	plan = fftw_plan_dft_r2c_1d(FFT_AUDIOSAMPLERATE, din, cpout, FFTW_MEASURE);

    // create arbitrary pre decimator
    // decimate 44.1k or 48k down to 8000Hz
    // the FFT rate is 800, but we feed it with 8000 Samplerate
    // this results in a new fft every 100ms with a resolution of 10 Hz
    float ratio = 10.0f * (float)FFT_AUDIOSAMPLERATE / (float)physRXcaprate;
    fftdecim = msresamp_crcf_create(ratio, 40.0f);

}

void _exit_fft()
{
    if(plan) fftw_destroy_plan(plan);
	if(din) fftw_free(din);
	if(cpout) fftw_free(cpout);
    plan = NULL;
    din = NULL;
    cpout = NULL;

    if (fftdecim) msresamp_crcf_destroy(fftdecim);
    fftdecim = NULL;
}
