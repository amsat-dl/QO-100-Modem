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

int mod_running = 0;

void modulator(uint8_t sym_in);
void init_demodulator();
void close_demodulator();
void init_modulator();
void close_modulator();

void init_dsp()
{
    printf("init DSP\n");
    //close_dsp();
    init_modulator();
    io_fifo_clear(io_pbidx);
    io_fifo_clear(io_capidx);
    init_demodulator();
    printf("init DSP FIN\n");
    mod_running = 1;
}

void close_dsp()
{
    printf("close DSP\n");
    mod_running = 0;
    io_fifo_clear(io_pbidx);
    io_fifo_clear(io_capidx);
    sleep_ms(200); // give system a chance to stop
    
    close_modulator();
    close_demodulator();
}


modulation_scheme getMod()
{
    if (bitsPerSymbol == 1)
        return LIQUID_MODEM_BPSK;
    if(bitsPerSymbol == 2)
        return LIQUID_MODEM_QPSK;
    if (bitsPerSymbol == 3)
        return LIQUID_MODEM_APSK8;

    return LIQUID_MODEM_QPSK;
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
    float h[4000];
    if (h_len_NumFilterCoeefs >= 4000)
    {
        printf("h in h_len_NumFilterCoeefs too small, need %d\n", h_len_NumFilterCoeefs);
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
void _sendToModulator(uint8_t *d, int len)
{
    if (upnco == NULL) return;
    if (mod == NULL) return;
    if (TX_interpolator == NULL) return;
    if (mod_running == 0) return;
    
    int symanz = len * 8 / bitsPerSymbol;
    uint8_t syms[10000]; 
    if (symanz >= 10000)
    {
        printf("syms in symanz too small\n");
        return;
    }

    if (bitsPerSymbol == 1)
        convertBytesToSyms_BPSK(d, syms, len);
    else if (bitsPerSymbol == 2)
        convertBytesToSyms_QPSK(d, syms, len);
    else if (bitsPerSymbol == 3)
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
    if (upnco == NULL) return;
    if (mod == NULL) return;
    if (TX_interpolator == NULL) return;
    if (mod_running == 0) return;

    liquid_float_complex sample;
    modem_modulate(mod, sym_in, &sample);
    
    //printf("TX ================= sample: %f + i%f\n", sample.real, sample.imag);
    
    // interpolate by k_SampPerSymb
    liquid_float_complex y[400];
    if (k_SampPerSymb >= 400)
    {
        printf("y in k_SampPerSymb too small, need %d\n", k_SampPerSymb);
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
        int to = 0;
        /*
        while(keeprunning)
        {
            fs = io_fifo_freespace(io_pbidx);
            // wait until there is space in fifo
            if(fs > 10) break;
            printf("wit:%d: %d\n", io_pbidx,io_fifo_elems_avail(io_pbidx));
            sleep_ms(10);
            if (++to >= 400) break; // give up after 4s 
        }*/

        // put max. 4 frames in fifo, to minimize latency
        // 4 frames are 1032 bytes
        while (keeprunning && mod_running)
        {
            fs = io_fifo_usedspace(io_pbidx);
            if (fs <= 20000) break;
            //printf("wit:%d: %d\n", io_pbidx, io_fifo_elems_avail(io_pbidx));
            sleep_ms(10);
            if (++to >= 400) break; // give up after 4s 
        }

        if (marker)
            usb += singleFrequency() / 4.0f;

        // reduce volume and send to soundcard
        usb *= 0.1f;
        kmaudio_playsamples(io_pbidx, &usb, 1, pbvol);
    }
}

// =========== DEMODULATOR =============================

nco_crcf dnnco = NULL;
firdecim_crcf decim = NULL;

// decimator parameters
unsigned int m_predec = 8;  // filter delay
float As_predec = 40.0f;    // stop-band att 

// symtrack parameters
int          ftype_st       = LIQUID_FIRFILT_RRC;
unsigned int k_st = 4;       // samples per symbol
unsigned int m_st           = 7;       // filter delay (symbols)
float        beta_st        = beta_excessBW;//0.30f;   // filter excess bandwidth factor
float        bandwidth_st   = 0.9f;  // loop filter bandwidth

uint8_t maxLevel = 0;   // maximum RXlevel over the last x samples in %
uint8_t maxTXLevel = 0; // maximum TXlevel over the last x samples in %

float radians_per_sample = ((2.0f * (float)M_PI * (float)FREQUENCY) / (float)caprate);
float last_radians_per_sample = 0;

SYMTRACK *km_symtrack = NULL;

void init_demodulator()
{
    printf("init RX demodulator\n");
    
    // downmixer oscillator
    radians_per_sample = ((2.0f*(float)M_PI*(float)FREQUENCY)/(float)caprate);
    
    dnnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(dnnco, 0.0f);
    nco_crcf_set_frequency(dnnco, radians_per_sample);
    
    // create pre-decimator
    decim = firdecim_crcf_create_kaiser(rxPreInterpolfactor, m_predec, As_predec);
    firdecim_crcf_set_scale(decim, 1.0f/(float)rxPreInterpolfactor);

    // create symbol tracking synchronizer
    km_symtrack = km_symtrack_cccf_create(ftype_st, k_st, m_st, beta_st, getMod());
    km_symtrack_cccf_set_bandwidth(km_symtrack, bandwidth_st);
}

void close_demodulator()
{
    if(decim != NULL) firdecim_crcf_destroy(decim);
    decim = NULL;
    if (dnnco != NULL) nco_crcf_destroy(dnnco);
    dnnco = NULL;
    if (km_symtrack != NULL) km_symtrack_cccf_destroy(km_symtrack);
    km_symtrack = NULL;
}

void resetModem()
{
    //printf("Reset Symtrack\n");
    if (dnnco == NULL) return;
    if (decim == NULL) return;
    if (km_symtrack == NULL) return;
    if (mod_running == 0) return;
    km_symtrack_cccf_reset(km_symtrack,0xff);
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
            printf("FFTdata: txpl too small !!!\n");
            return;
        }

        int bidx = 0;
        txpl[bidx++] = 4;    // type 4: FFT data follows

        int us = 0;
        if (speedmode < 10)
        {
            // Bytes in Fifo
            /*
            int bus = io_fifo_usedspace(io_pbidx);
            // Payloads in fifo
            if (bitsPerSymbol == 0 || txinterpolfactor == 0) return; // just for security, no useful function
            us = bus / (txinterpolfactor * UDPBLOCKLEN * 8 / bitsPerSymbol);
            */

            // checke PSK_GUI_TX
            us = fifo_usedspace(PSK_GUI_TX);


            //printf("bytes:%d blocks:%d\n", bus, us);
        }
        if (speedmode == 10)
        {
            // RTTY
            us = io_fifo_usedspace(io_pbidx);
            us = us * 20 / 48000;
            //printf("bytes:%d\n", us);
        }

        if (us > 255 || ann_running == 1) us = 255;
        txpl[bidx++] = us;    // usage of TX fifo

        us = io_fifo_usedspace(io_capidx);
        us /= 400;
        if (us > 255) us = 255;
        txpl[bidx++] = us;    // usage of RX fifo

        txpl[bidx++] = rxlevel_deteced; // RX level present
        txpl[bidx++] = rx_in_sync;

        txpl[bidx++] = kmaudio_maxlevel(io_capidx);
        txpl[bidx++] = kmaudio_maxlevel(io_pbidx);

        txpl[bidx++] = rtty_frequency >> 8;     // rtty qrg by autosync
        txpl[bidx++] = rtty_frequency & 0xff;

        txpl[bidx++] = rtty_txoff ? 0 : 1;    // TX on/off

        for (int i = 0; i < fftlen; i++)
        {
            txpl[bidx++] = fft[i] >> 8;
            txpl[bidx++] = fft[i] & 0xff;
        }

        sendUDP(appIP, UdpDataPort_ModemToApp, txpl, bidx);
    }
}

#define CONSTPOINTS 400
int demodulator()
{
static liquid_float_complex ccol[500];
static int ccol_idx = 0;
static int16_t const_re[CONSTPOINTS];
static int16_t const_im[CONSTPOINTS];
static int const_idx = 0;

    if (dnnco == NULL) return 0;
    if (decim == NULL) return 0;
    if (km_symtrack == NULL) return 0;
    if (mod_running == 0) return 0;

    // get available received samples
    float farr[1100];
    int ret = kmaudio_readsamples(io_capidx, farr, 1000, capvol,0);
    if(ret == 0) return 0;

    //measure_speed_bps(ret);

    for (int fanz = 0; fanz < ret; fanz++)
    {
        float f = farr[fanz];
        if (VoiceAudioMode == VOICEMODE_LISTENAUDIOIN)
        {
            kmaudio_playsamples(voice_pbidx, &f, 1,lsvol);
        }

        make_FFTdata(f * 100);

        // if user changed frequency
        if (radians_per_sample != last_radians_per_sample)
        {
            //printf("new QRG. Rad:%f\n", radians_per_sample);
            nco_crcf_set_frequency(dnnco, radians_per_sample);
            last_radians_per_sample = radians_per_sample;
        }

        // downconvert 1,5kHz into baseband, still at soundcard sample rate
        nco_crcf_step(dnnco);

        liquid_float_complex in;
        in.real = f;
        in.imag = f;
        liquid_float_complex c;
        nco_crcf_mix_down(dnnco, in, &c);

        // c is the actual sample, converted to complex and shifted to baseband

        // this is the first decimator. We need to collect rxPreInterpolfactor number of samples
        // then call execute which will give us one decimated sample
        ccol[ccol_idx++] = c;
        if (ccol_idx < rxPreInterpolfactor) continue;
        ccol_idx = 0;

        // we have rxPreInterpolfactor samples in ccol
        liquid_float_complex y;
        firdecim_crcf_execute(decim, ccol, &y);
        // the output of the pre decimator is exactly one sample in y

        unsigned int num_symbols_sync;
        liquid_float_complex syms;
        unsigned int nsym_out;   // demodulated output symbol
        km_symtrack_execute(km_symtrack, y, &syms, &num_symbols_sync, &nsym_out);

        if (num_symbols_sync > 1) printf("symtrack_cccf_execute %d output symbols ???\n", num_symbols_sync);
        if (num_symbols_sync != 0)
        {
            measure_speed_syms(1); // do NOT remove, used for speed display in GUI

            // try to unpack and extract a complete frame
            uint8_t symb = nsym_out;
            if (bitsPerSymbol == 2) symb ^= (symb >> 1);
            GRdata_rxdata(&symb, 1, NULL);

            // send the data "as is" to app for Constellation Diagram
            // collect values until an UDP frame is full
            const_re[const_idx] = (int16_t)(syms.real * 15000.0f);
            const_im[const_idx] = (int16_t)(syms.imag * 15000.0f);
            if (++const_idx >= CONSTPOINTS)
            {
                uint8_t txpl[CONSTPOINTS * sizeof(int16_t) * 2 + 1];
                int idx = 0;
                txpl[idx++] = 5;    // type 5: IQ data follows

                for (int i = 0; i < CONSTPOINTS; i++)
                {
                    txpl[idx++] = (uint8_t)(const_re[i] >> 8);
                    txpl[idx++] = (uint8_t)(const_re[i]);
                    txpl[idx++] = (uint8_t)(const_im[i] >> 8);
                    txpl[idx++] = (uint8_t)(const_im[i]);
                }

                sendUDP(appIP, UdpDataPort_ModemToApp, txpl, sizeof(txpl));

                const_idx = 0;
            }
        }
    }
            
    return 1;
}
