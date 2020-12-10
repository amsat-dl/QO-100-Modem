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
* liquid_if.c ... functions using liquid-dsp
* 
* liquid-dsp must be previously installed by running ./liquid-dsp-install (under linux)
* 
*/

#include "hsmodem.h"

void modulator(uint8_t sym_in);
void init_demodulator();
void close_demodulator();
void init_modulator();
void close_modulator();

void init_dsp()
{
    init_modulator();
    io_pb_write_fifo_clear();
    init_demodulator();
}

void close_dsp()
{
    close_modulator();
    close_demodulator();
}

modulation_scheme getMod()
{
    if(bitsPerSymbol == 2)
        return LIQUID_MODEM_QPSK;
    else
    {
        return LIQUID_MODEM_APSK8;
    }
}

// =========== MODULATOR ==================================================

// modem objects
modem mod = NULL;

// NCOs for mixing baseband <-> 1500 Hz
#define FREQUENCY           1500            
int      type        = LIQUID_NCO;                  // nco type
nco_crcf upnco = NULL;

// TX-Interpolator Filter Parameters
// 44100 input rate for 2205 Sym/s = 20
// change for other rates
firinterp_crcf TX_interpolator = NULL;
unsigned int k_SampPerSymb =            20;         // 44100 / (4410/2)
unsigned int m_filterDelay_Symbols =    15;         // not too short for good filter
float        beta_excessBW  =           0.2f;      // filter excess bandwidth factor
float        tau_FracSymbOffset   =     -0.2f;      // fractional symbol offset

void init_modulator()
{
    close_dsp();
    printf("init TX modulator\n");
    
    k_SampPerSymb = txinterpolfactor;
    
    // create modulator
    mod = modem_create(getMod());
    
    // create NCO for upmixing to 1500 Hz
    float RADIANS_PER_SAMPLE   = ((2.0f*(float)M_PI*(float)FREQUENCY)/(float)caprate);

    upnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(upnco, 0.0f);
    nco_crcf_set_frequency(upnco, RADIANS_PER_SAMPLE);
    
    // TX: Interpolator Filter
    // compute delay
    while (tau_FracSymbOffset < 0) tau_FracSymbOffset += 1.0f;  // ensure positive tau
    float g = k_SampPerSymb*tau_FracSymbOffset;                 // number of samples offset
    int ds=(int)floorf(g);               // additional symbol delay
    float dt = (g - (float)ds);     // fractional sample offset
    // force dt to be in [0.5,0.5]
    if (dt > 0.5f) 
    {                
        dt -= 1.0f;
        ds++;
    }
    
    // calculate filter coeffs
    unsigned int h_len_NumFilterCoeefs = 2 * k_SampPerSymb * m_filterDelay_Symbols + 1;
    float h[1000];
    if (h_len_NumFilterCoeefs >= 1000)
    {
        printf("h in h_len_NumFilterCoeefs too small\n");
        return;
    }
    liquid_firdes_prototype(    LIQUID_FIRFILT_RRC,
                                k_SampPerSymb,
                                m_filterDelay_Symbols,
                                beta_excessBW,
                                dt,
                                h);
    // create the filter
    TX_interpolator = firinterp_crcf_create(k_SampPerSymb,h,h_len_NumFilterCoeefs);
    
    printf("DSP created\n");
    return;
}

void close_modulator()
{
    if(mod != NULL) modem_destroy(mod);
    if(upnco != NULL) nco_crcf_destroy(upnco);
    if(TX_interpolator != NULL) firinterp_crcf_destroy(TX_interpolator);
    mod = NULL;
    upnco = NULL;
    TX_interpolator = NULL;
}

// d ... symbols to send
// len ... number of symbols in d
void sendToModulator(uint8_t *d, int len)
{
    if(upnco == NULL) return;
    
    int symanz = len * 8 / bitsPerSymbol;
    uint8_t syms[10000]; 
    if (symanz >= 10000)
    {
        printf("syms in symanz too small\n");
        return;
    }
    if(bitsPerSymbol == 2)
        convertBytesToSyms_QPSK(d, syms, len);
    else
        convertBytesToSyms_8PSK(d, syms, len);

    for(int i=0; i<symanz; i++)
    {
        // remove gray code
        // this adds gray code, liquid adds it again which removes it
        if(bitsPerSymbol == 2) syms[i] ^= (syms[i]>>1);
        
        modulator(syms[i]);
    }
}

// call for every symbol
// modulates, filters and upmixes symbols and send it to soundcard
void modulator(uint8_t sym_in)
{
    liquid_float_complex sample;
    modem_modulate(mod, sym_in, &sample);
    
    //printf("TX ================= sample: %f + i%f\n", sample.real, sample.imag);
    
    // interpolate by k_SampPerSymb
    liquid_float_complex y[100];
    if (k_SampPerSymb >= 100)
    {
        printf("y in k_SampPerSymb too small\n");
        return;
    }

    firinterp_crcf_execute(TX_interpolator, sample, y);

    for(unsigned int i=0; i<k_SampPerSymb; i++)
    {
        // move sample to 1,5kHz carrier
        nco_crcf_step(upnco);
        
        liquid_float_complex c;
        nco_crcf_mix_up(upnco,y[i],&c);
        float usb = c.real + c.imag;
    
        // adapt speed to soundcard samplerate
        int fs;
        while(1)
        {
            fs = io_pb_fifo_freespace(0);
            // wait until there is space in fifo
            if(fs) break;
            sleep_ms(10);
        }
        
        io_pb_write_fifo(usb * 0.2f); // reduce volume and send to soundcard
    }
}

// =========== DEMODULATOR =============================

nco_crcf dnnco = NULL;
symtrack_cccf symtrack = NULL;
firdecim_crcf decim = NULL;

// decimator parameters
unsigned int m_predec = 8;  // filter delay
float As_predec = 40.0f;    // stop-band att 

// symtrack parameters
int          ftype_st       = LIQUID_FIRFILT_RRC;
unsigned int k_st           = 4;       // samples per symbol
unsigned int m_st           = 7;       // filter delay (symbols)
float        beta_st        = beta_excessBW;//0.30f;   // filter excess bandwidth factor
float        bandwidth_st   = 0.9f;  // loop filter bandwidth

uint8_t maxLevel = 0; // maximum level over the last x samples in %

void init_demodulator()
{
    printf("init RX demodulator\n");
    
    // downmixer oscillator
    float RADIANS_PER_SAMPLE   = ((2.0f*(float)M_PI*(float)FREQUENCY)/(float)caprate);
    
    dnnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(dnnco, 0.0f);
    nco_crcf_set_frequency(dnnco, RADIANS_PER_SAMPLE);
    
    // create pre-decimator
    decim = firdecim_crcf_create_kaiser(rxPreInterpolfactor, m_predec, As_predec);
    firdecim_crcf_set_scale(decim, 1.0f/(float)rxPreInterpolfactor);
    
    // create symbol tracking synchronizer
    //k_st = txinterpolfactor;
    //symtrack = km_symtrack_cccf_create(ftype_st,k_st,m_st,beta_st,getMod());
    km_symtrack_cccf_create(ftype_st, k_st, m_st, beta_st, getMod());
    //symtrack_cccf_set_bandwidth(symtrack,bandwidth_st);
    km_symtrack_cccf_set_bandwidth(bandwidth_st);
}

void close_demodulator()
{
    if(decim != NULL) firdecim_crcf_destroy(decim);
    symtrack = NULL;
    decim = NULL;
}

void resetModem()
{
    //printf("Reset Symtrack\n");
    km_symtrack_cccf_reset(0xff);
}

// called for Audio-Samples (FFT)
void make_FFTdata(float f)
{
    // send IQ data to FFT for waterfall calculation
    int fftlen = 0;
    uint16_t* fft = make_waterfall(f, &fftlen);
    if (fft != NULL)
    {
        uint8_t txpl[10000];
        if (fftlen > (10000 * 2 + 1))
        {
            printf("GRdata_FFTdata: txpl too small !!!\n");
            return;
        }

        int bidx = 0;
        txpl[bidx++] = 4;    // type 4: FFT data follows

        int us = io_pb_fifo_usedBlocks();
        if (us > 255 || ann_running == 1) us = 255;
        txpl[bidx++] = us;    // usage of TX fifo

        us = io_cap_fifo_usedPercent();
        if (us > 255) us = 255;
        txpl[bidx++] = us;    // usage of TX fifo

        txpl[bidx++] = rxlevel_deteced; // RX level present
        txpl[bidx++] = rx_in_sync;

        for (int i = 0; i < fftlen; i++)
        {
            txpl[bidx++] = fft[i] >> 8;
            txpl[bidx++] = fft[i] & 0xff;
        }
        sendUDP(appIP, UdpDataPort_ModemToApp, txpl, bidx);
    }
}

#define MCHECK 1000
void getMax(float fv)
{
    static float farr[MCHECK];
    static int idx = 0;
    static int f = 1;

    if (f)
    {
        f = 0;
        for (int i = 0; i < MCHECK; i++)
            farr[i] = 1;
    }

    farr[idx] = fv;
    idx++;
    if (idx == MCHECK)
    {
        idx = 0;
        float max = 0;
        for (int i = 0; i < MCHECK; i++)
        {
            if (farr[i] > max) max = farr[i];
        }
        maxLevel = (uint8_t)(max*100);
        //printf("max: %10.6f\n", max);
    }
}

int demodulator()
{
static liquid_float_complex ccol[100];
static int ccol_idx = 0;
    
    if(dnnco == NULL) return 0;
    
    // get one received sample
    float f;
    int ret = io_cap_read_fifo(&f);
    if(ret == 0) return 0;
       
    if (VoiceAudioMode == VOICEMODE_LISTENAUDIOIN)
    {
        io_ls_write_fifo(f);
    }

    // input volume
    f *= softwareCAPvolume;

    getMax(f);

    make_FFTdata(f*60);
    
    // downconvert into baseband
    // still at soundcard sample rate
    nco_crcf_step(dnnco);
    
    liquid_float_complex in;
    in.real = f;
    in.imag = f;
    liquid_float_complex c;
    nco_crcf_mix_down(dnnco,in,&c);
    
    // c is the actual sample, converted to complex and shifted to baseband
    
    // this is the first decimator. We need to collect rxPreInterpolfactor number of samples
    // then call execute which will give us one decimated sample
    ccol[ccol_idx++] = c;
    if(ccol_idx < rxPreInterpolfactor) return 1;
    ccol_idx = 0;
    
    // we have rxPreInterpolfactor samples in ccol
    //printf("sc:%10.6f dn:%10.6f j%10.6f  ", f, c.real, c.imag);
    liquid_float_complex y;
    firdecim_crcf_execute(decim, ccol, &y);

    unsigned int num_symbols_sync;
    liquid_float_complex syms;
    //symtrack_cccf_execute(symtrack, y, &syms, &num_symbols_sync);
    unsigned int nsym_out;   // output symbol
    km_symtrack_execute(y, &syms, &num_symbols_sync,&nsym_out);
    
    if(num_symbols_sync > 1) printf("symtrack_cccf_execute %d output symbols ???\n",num_symbols_sync);
    if(num_symbols_sync != 0)
    {
        unsigned int sym_out;   // output symbol
        sym_out = nsym_out;

        measure_speed_syms(1);
        
        // try to extract a complete frame
        uint8_t symb = sym_out;
        if(bitsPerSymbol == 2) symb ^= (symb>>1);
        GRdata_rxdata(&symb, 1, NULL);
            
        // send the data "as is" to app for Constellation Diagram
        // we have about 2000 S/s, but this many points would make the GUI slow
        // so we send only every x
        static int ev = 0;
        if (++ev >= 10)
        {
            ev = 0;
            int32_t re = (int32_t)(syms.real * 16777216.0);
            int32_t im = (int32_t)(syms.imag * 16777216.0);
            uint8_t txpl[13];
            int idx = 0;
            txpl[idx++] = 5;    // type 5: IQ data follows
            uint32_t sy = 0x3e8;
            txpl[idx++] = sy >> 24;
            txpl[idx++] = sy >> 16;
            txpl[idx++] = sy >> 8;
            txpl[idx++] = sy;
            txpl[idx++] = re >> 24;
            txpl[idx++] = re >> 16;
            txpl[idx++] = re >> 8;
            txpl[idx++] = re;
            txpl[idx++] = im >> 24;
            txpl[idx++] = im >> 16;
            txpl[idx++] = im >> 8;
            txpl[idx++] = im;
            sendUDP(appIP, UdpDataPort_ModemToApp, txpl, 13);
        }
    }
            
    return 1;
}
