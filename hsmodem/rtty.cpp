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
* 
*  RTTY:
*  =====
*  bit/symbol: 1
*  BW: 170 Hz 
*  carrier: 1500 Hz
*  speed: 45.45 bits/s
*  samples/symbol: 33
*/

#include "hsmodem.h"

#define rtty_CENTERFREQUENCY  1500            

fskmod rtty_mod = NULL;
fskdem rtty_dem = NULL;

unsigned int rtty_m = 1;	// bits/symbol
const unsigned int rtty_k = 33;	// samples/symbol (periods of the 1500 Hz Carrier in one symbol)
float rtty_bandwith = (170.0f/2)/ (float)rtty_CENTERFREQUENCY;	// 170 Hz spacing normalized to 1500 Hz

firinterp_crcf rtty_TX_interpolator = NULL;
unsigned int rtty_k_SampPerSymb = 20;         // 44100 / (4410/2)
unsigned int rtty_m_filterDelay_Symbols = 15;         // not too short for good filter
float        rtty_beta_excessBW = 0.2f;      // filter excess bandwidth factor
float        rtty_tau_FracSymbOffset = -0.2f;      // fractional symbol offset

nco_crcf rtty_upnco = NULL;
nco_crcf rtty_dnnco = NULL;

firdecim_crcf rtty_decim = NULL;
unsigned int rtty_m_predec = 8;  // filter delay
float rtty_As_predec = 40.0f;    // stop-band att 

void close_rtty();

void init_rtty()
{
	close_rtty();

	rtty_mod = fskmod_create(rtty_m, rtty_k, rtty_bandwith);
    rtty_dem = fskdem_create(rtty_m, rtty_k, rtty_bandwith);

    // TX: Interpolator Filter
    rtty_k_SampPerSymb = caprate / rtty_CENTERFREQUENCY;

    // compute delay
    while (rtty_tau_FracSymbOffset < 0) rtty_tau_FracSymbOffset += 1.0f;  // ensure positive tau
    float g = rtty_k_SampPerSymb * rtty_tau_FracSymbOffset;                 // number of samples offset
    int ds = (int)floorf(g);               // additional symbol delay
    float dt = (g - (float)ds);     // fractional sample offset
    // force dt to be in [0.5,0.5]
    if (dt > 0.5f)
    {
        dt -= 1.0f;
        ds++;
    }

    // calculate filter coeffs
    unsigned int h_len_NumFilterCoeefs = 2 * rtty_k_SampPerSymb * rtty_m_filterDelay_Symbols + 1;
    float h[4000];
    if (h_len_NumFilterCoeefs >= 4000)
    {
        printf("rtty h in h_len_NumFilterCoeefs too small, need %d\n", h_len_NumFilterCoeefs);
        return;
    }
    liquid_firdes_prototype(LIQUID_FIRFILT_RRC,
        rtty_k_SampPerSymb,
        rtty_m_filterDelay_Symbols,
        rtty_beta_excessBW,
        dt,
        h);
    // create the filter
    rtty_TX_interpolator = firinterp_crcf_create(rtty_k_SampPerSymb, h, h_len_NumFilterCoeefs);

    // create NCO for upmixing to 1500 Hz
    float rtty_RADIANS_PER_SAMPLE = ((2.0f * (float)M_PI * (float)rtty_CENTERFREQUENCY) / (float)caprate);
    rtty_upnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(rtty_upnco, 0.0f);
    nco_crcf_set_frequency(rtty_upnco, rtty_RADIANS_PER_SAMPLE);

    rtty_dnnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(rtty_dnnco, 0.0f);
    nco_crcf_set_frequency(rtty_dnnco, rtty_RADIANS_PER_SAMPLE);

    rtty_decim = firdecim_crcf_create_kaiser(rtty_k_SampPerSymb, rtty_m_predec, rtty_As_predec);
    firdecim_crcf_set_scale(rtty_decim, 1.0f / (float)rtty_k_SampPerSymb);
}

void close_rtty()
{
	if (rtty_mod != NULL) fskmod_destroy(rtty_mod);
	rtty_mod = NULL;
	if (rtty_TX_interpolator != NULL) firinterp_crcf_destroy(rtty_TX_interpolator);
	rtty_TX_interpolator = NULL;
    if (rtty_upnco != NULL) nco_crcf_destroy(rtty_upnco);
    rtty_upnco = NULL;
    if (rtty_dnnco != NULL) nco_crcf_destroy(rtty_dnnco);
    rtty_dnnco = NULL;
    if (rtty_decim != NULL) firdecim_crcf_destroy(rtty_decim);
    rtty_decim = NULL;
    if (rtty_dem != NULL) fskdem_destroy(rtty_dem);
    rtty_dem = NULL;
}

char text[6] = {"ABCD\n"};
int tidx = 0;
int bitidx = 1;

void rtty_tx()
{
    if (rtty_mod == NULL)
        init_rtty();

    unsigned int sym = (text[tidx] & bitidx) ? 1 : 0;
    bitidx <<= 1;
    if (bitidx == 0x0100)
    {
        bitidx = 1;
        tidx++;
        if (tidx == 6) tidx = 0;
    }

	liquid_float_complex rtty_txbuf[33+1];

    // gets one symbol at a speed of 45.45
	fskmod_modulate(rtty_mod, sym, rtty_txbuf);
	// here we have the complex RTTY signal in baseband
    // one symbol was expanded to rtty_k periods:
    // 45.454545 * 33 = 1500 periods
    
    for (unsigned int i = 0; i < rtty_k; i++)
    {
        // resample it to the soundcard rate caprate
        // interpolate by k_SampPerSymb
        liquid_float_complex y[40];
        if (rtty_k_SampPerSymb >= 40)
        {
            printf("y in k_SampPerSymb too small, need %d\n", rtty_k_SampPerSymb);
            return;
        }

        firinterp_crcf_execute(rtty_TX_interpolator, rtty_txbuf[i], y);
        // here we have rtty_k_SampPerSymb samples in y[] in the baseband at caprate
        // speed 
        
        for (unsigned int i = 0; i < rtty_k_SampPerSymb; i++)
        {
            // move sample to 1,5kHz carrier
            nco_crcf_step(rtty_upnco);

            liquid_float_complex c;
            nco_crcf_mix_up(rtty_upnco, y[i], &c);
            float usb = c.real + c.imag;

            // speed: 48000
            // adapt speed to soundcard samplerate
            int fs;
            while (1)
            {
                fs = io_pb_fifo_freespace(0);
                // wait until there is space in fifo
                if (fs > 20000) break;
                sleep_ms(10);
            }

            io_pb_write_fifo(usb * 0.2f); // reduce volume and send to soundcard
        }
    }
}

// RTTY: sample rate HAS TO BE 48000
int rtty_rx()
{
    static liquid_float_complex ccol[500];
    static unsigned int ccol_idx = 0;

    if (rtty_dnnco == NULL) return 0;

    // get one received sample
    float f;
    int ret = io_cap_read_fifo(&f);
    if (ret == 0) return 0;

    if (VoiceAudioMode == VOICEMODE_LISTENAUDIOIN)
        io_ls_write_fifo(f);

    // input volume
    f *= softwareCAPvolume;

    //getMax(f);
    //make_FFTdata(f * 100);
    
    // downconvert 1,5kHz into baseband, still at soundcard sample rate
    nco_crcf_step(rtty_dnnco);

    liquid_float_complex in;
    in.real = f;
    in.imag = f;
    liquid_float_complex c;
    nco_crcf_mix_down(rtty_dnnco, in, &c);

    // c is the actual sample, converted to complex and shifted to baseband

    // this is the first decimator. We need to collect rtty_k_SampPerSymb number of samples
    // then call execute which will give us one decimated sample
    ccol[ccol_idx++] = c;
    if (ccol_idx < rtty_k_SampPerSymb) return 1;
    ccol_idx = 0;

    // we have rtty_k_SampPerSymb samples in ccol
    liquid_float_complex y;
    firdecim_crcf_execute(rtty_decim, ccol, &y);
    // the output of the pre decimator is exactly one sample in y
    // ready for demodulation
    // here we have 1500 samples/s
    // collect rtty_k (33) samples then demodulate, 1500/33 = 45.45
    static unsigned int cs = 0;
    static liquid_float_complex camps[rtty_k];
    camps[cs] = y;
    if (++cs < rtty_k) return 1;
    cs = 0;

    //measure_speed_bps(1);

    unsigned int sym_out = fskdem_demodulate(rtty_dem, camps);
    
    static int sym = 0;
    static int symidx = 0;
    sym |= sym_out << symidx;
    symidx++;
    if (symidx == 8)
    {
        symidx = 0;
        printf("%d: %c\n", sym, sym);
        sym = 0;
    }

    return 1;
}
