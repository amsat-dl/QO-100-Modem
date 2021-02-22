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

int evalSymbols(uint8_t sym);
void baudot_encoder(char c, uint8_t bd[2], int* pnum);
uint8_t getBaudot(char c, int letters);
char baudot_decoder(char c);
void sendRttyToGUI(uint8_t b);
void send_baudot(char c);

#define rtty_CENTERFREQUENCY  1500            

float rtty_RADIANS_PER_SAMPLE = 0;
float rtty_RX_RADIANS_PER_SAMPLE = 0;

nco_crcf rtty_upnco = NULL;
nco_crcf rtty_dnnco = NULL;

// RX RTTY Filter
firfilt_crcf rtty_q;
unsigned int flt_h_len = 65;  // filter length
// we filter at 48k samp rate, half BW=170/2, so lets filter at 80 Hz
float flt_fc = 80.0f/48000.0f;          // cutoff frequency
float flt_As = 60.0f;         // stop-band attenuation

int rtty_txoff = 1; // 1=off, 0=on , >1...downcount for off
int synced = 0;
int run_rtty_threads = 0;

int rtty_frequency = rtty_CENTERFREQUENCY;

unsigned int m = 1;     // number of bits/symbol
#define k             264     // filter samples/symbol
float        SNRdB = 0.0f;   // signal-to-noise ratio [dB]
float        bandwidth = 0.001894f;    // frequency spacing
unsigned int nfft = 1200;    // FFT size for compute spectrum

liquid_float_complex buf_tx[k];    // transmit buffer
liquid_float_complex buf_rx[k];    // transmit buffer
unsigned int sym_out;
float        nstd;

fskmod modi = NULL;
fskdem dem = NULL;

int rtty_autosync = 0;

void rtty_modifyRXfreq(int f_Hz)
{
    //printf("set:%d Hz\n", f_Hz);
    rtty_frequency = f_Hz;
    rtty_RX_RADIANS_PER_SAMPLE = ((2.0f * (float)M_PI * (float)f_Hz) / (float)caprate);
}


void sendRttyToGUI(uint8_t b)
{
    uint8_t txpl[7];
    txpl[0] = 6;            // RTTY RX Byte follows
    txpl[1] = b;            // RXed byte
    txpl[2] = 0;
    txpl[3] = synced;            
    txpl[4] = 0;            // unused
    txpl[5] = 0;
    txpl[6] = 0;
    sendUDP(appIP, UdpDataPort_ModemToApp, txpl, sizeof(txpl));
}

/*
* space=lower tone (Bitvalue=1)
* mark=higher tone (Bitvalue=0)
* 
* start (space)
* bit 4
* bit 3
* bit 2
* bit 1
* bit 0
* stop (mark)
* stop (mark)
* 
* send space if nothing to transmit
*/
BAUDOTTAB baudot[] = {
    {0b00010111,'Q','1',},
    {0b00010011,'W','2',},
    {0b00000001,'E','3',},
    {0b00001010,'R','4',},
    {0b00010000,'T','5',},
    {0b00010101,'Y','6',},
    {0b00000111,'U','7',},
    {0b00000110,'I','8',},
    {0b00011000,'O','9',},
    {0b00010110,'P','0',},
    {0b00000011,'A','-',},
    {0b00000101,'S','\'',},
    {0b00001001,'D','|',},
    {0b00001101,'F','|',},
    {0b00011010,'G','|',},
    {0b00010100,'H','|',},
    {0b00001011,'J','|',},
    {0b00001111,'K','(',},
    {0b00010010,'L',')',},
    {0b00010001,'Z','+',},
    {0b00011101,'X','/',},
    {0b00001110,'C',':',},
    {0b00011110,'V','=',},
    {0b00011001,'B','?',},
    {0b00001100,'N',',',},
    {0b00011100,'M','.',},
    {0b00001000,'\r','\r',},
    {0b00000010,'\n','\n',},
    {0b00000100,' ',' ',},
    {0b00011011,'@','@',},	// switch to numbers
    {0b00011111,'§','§',},	// switch to letters (if already letter, then backspace)
    {0xff,' ',' '}			// end of table
};

int isletters = 1; 

void baudot_encoder(char c, uint8_t bd[2], int* pnum)
{
    int anz = 0;
    int letters = 0;

    //printf("ascii:%c letter:%d (%d)\n", c, letters,isletters);

    if (c == '#')
    {
        rtty_txoff = 0;
        *pnum = 0;
        return;
    }

    if (c == '~')
    {
        rtty_txoff = 10;
        *pnum = 0;
        return;
    }

    if (c == 0x08)
    {
        // backspace does not exist in RTTY
        return;
    }

    if (c == ' ')
    {
        bd[anz++] = 4;
        *pnum = anz;
        return;
    }

    if (c == '\n')
    {
        bd[anz++] = getBaudot('\n', letters);
        bd[anz++] = getBaudot('\r', letters);
        *pnum = anz;
        return;
    }

    if (c >= 'A' && c <= 'Z') letters = 1;

    if (letters == 1 && isletters == 0)
    {
        bd[anz++] = getBaudot('§', letters);
        isletters = 1;
    }

    if (letters == 0 && isletters == 1)
    {
        bd[anz++] = getBaudot('@', letters);
        isletters = 0;
    }

bd[anz++] = getBaudot(c, letters);

*pnum = anz;
}

uint8_t getBaudot(char c, int letters)
{
    uint8_t res = 0;
    int idx = 0;

    while (baudot[idx].baudot != 0xff)
    {
        if (letters == 1 && c == baudot[idx].letter)
        {
            res = baudot[idx].baudot;
            break;
        }

        if (letters == 0 && c == baudot[idx].number)
        {
            res = baudot[idx].baudot;
            break;
        }

        idx++;
    }
    return res;
}

char baudot_decoder(char c)
{
    static int letter = 1;

    if (c == 0x1b)
    {
        letter = 0;
        return 0;
    }

    if (c == 0x1f)
    {
        letter = 1;
        return 0;
    }

    int idx = 0;
    while (baudot[idx].baudot != 0xff)
    {
        if (baudot[idx].baudot == c)
        {
            if (letter == 1)
                return baudot[idx].letter;
            else
                return baudot[idx].number;
        }
        idx++;
    }
    return 0;
}


/*
// ======================================================================================
// Testfunctions: sends 010101.. in rtty speed and shows RX
// 1. disable rtty_init()
// 2. call testall() from hsmodem if speedmode==10


void ttest();
int rtest();
void itest();


void testall()
{
    static int f = 1;
    if (f)
    {
        itest();
        f = 0;
    }

    while (keeprunning)
    {
        ttest();
        while(keeprunning && rtest());
        sleep_ms(1);
    }
}

void itest()
{
    M = 1 << m;
    nstd = powf(10.0f, -SNRdB / 20.0f);

    // create modulator/demodulator pair
    modi = fskmod_create(m, k, bandwidth);
    dem = fskdem_create(m, k, bandwidth);
    fskdem_print(dem);

    // create NCO for up-mixing to 1500 Hz
    rtty_RADIANS_PER_SAMPLE = ((2.0f * (float)M_PI * (float)rtty_CENTERFREQUENCY) / (float)caprate);
    rtty_upnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(rtty_upnco, 0.0f);
    nco_crcf_set_frequency(rtty_upnco, rtty_RADIANS_PER_SAMPLE);

    // create NCO for down-mixing from 1500 Hz
    float rtty_RX_RADIANS_PER_SAMPLE = 2.0f * (float)M_PI * (float)rtty_CENTERFREQUENCY / (float)caprate;
    rtty_dnnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(rtty_dnnco, 0.0f);
    nco_crcf_set_frequency(rtty_dnnco, rtty_RX_RADIANS_PER_SAMPLE);

    // modulate, demodulate, count errors
    unsigned int num_symbol_errors = 0;
}

void ttest()
{
    int i, j;
    static unsigned int sym_in=0;

    int fs = io_pb_fifo_freespace(0);
    if (fs < 20000) return;

    static int scnt = 0;
    if (++scnt == 8)
    {
        scnt = 0;
        sym_in = 1 - sym_in;
    }
    //unsigned int sym_in = rand() % M;

    //measure_speed_bps(1);

    //printf("modulate\n");
    fskmod_modulate(modi, sym_in, &(buf_tx[0]));

    // move sample to 1,5kHz carrier
    for (j = 0; j < k; j++)
    {
        nco_crcf_step(rtty_upnco);
        nco_crcf_mix_up(rtty_upnco, buf_tx[j], &(buf_tx1500[j]));

        usbf = (buf_tx1500[j]).real + (buf_tx1500[j]).imag;

        //measure_speed_bps(1);

        io_pb_write_fifo(usbf * 0.2f); // reduce volume and send to soundcard
    }
}

int rtest()
{
    static int bridx = 0;

    int j;
    float f;
    int ret = io_cap_read_fifo(&f);
    if (ret == 0) return 0;

    // noise
    //printf("%f\n", f);
    //f = f + nstd * randnf() * M_SQRT1_2;

    (buf_rx1500[bridx]).real = f;
    (buf_rx1500[bridx]).imag = f;
    nco_crcf_step(rtty_dnnco);
    nco_crcf_mix_down(rtty_dnnco, buf_rx1500[bridx], &(buf_rx[bridx]));
    bridx++;

    if (bridx == k)
    {
        bridx = 0;

        sym_out = fskdem_demodulate(dem, buf_rx);

        static int nlixd = 0;

        //measure_speed_bps(1);
        
        printf("%01X ", sym_out);
        if (++nlixd == 16)
        {
            nlixd = 0;
            printf("\n");
        }
    }


    return 1;
}
*/
// ==========================================================================

#ifdef _LINUX_
void* rtty_tx_function(void* param);
void* rtty_rx_function(void* param);
#endif

#ifdef _WIN32_
void rtty_tx_function(void* param);
void rtty_rx_function(void* param);
#endif

void init_rtty()
{
    //printf("wegen FM test, kein Init RTTY\n");
    //return;

    fifo_clear(FIFO_RTTYTX);

    close_rtty();

    printf("Init RTTY\n");
    nstd = powf(10.0f, -SNRdB / 20.0f); // SNR Simulation referenced to -1..+1

    // create modulator/demodulator pair
    modi = fskmod_create(m, k, bandwidth);
    dem = fskdem_create(m, k, bandwidth);

    // create NCO for up-mixing to 1500 Hz
    rtty_RADIANS_PER_SAMPLE = ((2.0f * (float)M_PI * (float)rtty_CENTERFREQUENCY) / (float)caprate);
    rtty_upnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(rtty_upnco, 0.0f);
    nco_crcf_set_frequency(rtty_upnco, rtty_RADIANS_PER_SAMPLE);

    // create NCO for down-mixing from 1500 Hz
    rtty_RX_RADIANS_PER_SAMPLE = 2.0f * (float)M_PI * (float)rtty_CENTERFREQUENCY / (float)caprate;
    rtty_dnnco = nco_crcf_create(LIQUID_NCO);
    nco_crcf_set_phase(rtty_dnnco, 0.0f);
    nco_crcf_set_frequency(rtty_dnnco, rtty_RX_RADIANS_PER_SAMPLE);

    // RTTY RX Filter
    rtty_q = firfilt_crcf_create_kaiser(flt_h_len, flt_fc, flt_As, 0.0f);
    firfilt_crcf_set_scale(rtty_q, 2.0f * flt_fc);
    
    // create the rtty threads
    static int f = 1;
    if (f)
    {
        f = 0;

#ifdef _LINUX_
        pthread_t rtty_txthread;
        pthread_create(&rtty_txthread, NULL, rtty_tx_function, NULL);
        pthread_t rtty_rxthread;
        pthread_create(&rtty_rxthread, NULL, rtty_rx_function, NULL);
#endif
#ifdef _WIN32_
        _beginthread(rtty_tx_function, 0, NULL);
        _beginthread(rtty_rx_function, 0, NULL);
#endif
    }

    run_rtty_threads = 1;
}

void close_rtty()
{
    printf("Close RTTY\n");

    run_rtty_threads = 0;
    sleep_ms(100);

    if (modi != NULL) fskmod_destroy(modi);
    modi = NULL;

    if (dem != NULL) fskdem_destroy(dem);
    dem = NULL;

    if (rtty_upnco != NULL) nco_crcf_destroy(rtty_upnco);
    rtty_upnco = NULL;

    if (rtty_dnnco != NULL) nco_crcf_destroy(rtty_dnnco);
    rtty_dnnco = NULL;

    if (rtty_q != NULL) firfilt_crcf_destroy(rtty_q);
    rtty_q = NULL;
}

// RTTY TX thread
#ifdef _LINUX_
void* rtty_tx_function(void* param)
{
    pthread_detach(pthread_self());
#endif
#ifdef _WIN32_
void rtty_tx_function(void* param)
{
#endif

    uint8_t bd[2];
    int anz = 0;

    printf("TX thread\n");
    while (keeprunning)
    {
        while (run_rtty_threads == 0)
        {
            sleep_ms(100);
            if (keeprunning == 0)
            {
#ifdef _LINUX_
                pthread_exit(NULL); // self terminate this thread
                return NULL;
#endif
#ifdef _WIN32_
                return;
#endif
            }
        }

        uint8_t pcsend[200];
        int rlen = read_fifo(FIFO_RTTYTX, pcsend, 200);

        if(rlen > 0)
        {
            //printf("from fifo:%d <%s>\n", rlen, pcsend);
            for (int ilen = 0; ilen < rlen; ilen++)
            {
                baudot_encoder(pcsend[ilen], bd, &anz);
                for (int il = 0; il < anz; il++)
                {
                    send_baudot(bd[il]);
                    //printf("send: %d -> %02X\n", pcsend[ilen], bd[il]);
                }
            }
        }
        else
        {
            if (rtty_txoff == 1)
            {
                sleep_ms(10);
                continue;
            }

            if (rtty_txoff > 1) rtty_txoff--;
            send_baudot(0); // idle
        }
    }
#ifdef _LINUX_
    pthread_exit(NULL); // self terminate this thread
    return NULL;
#endif
}

// send one baudot byte
void send_baudot(char c)
{
    // c is the baudot code, fill into final byte cs
    uint8_t cs = 0;
    cs |= ((c & 1) ? 0x40 : 0);
    cs |= ((c & 2) ? 0x20 : 0);
    cs |= ((c & 4) ? 0x10 : 0);
    cs |= ((c & 8) ? 0x08 : 0);
    cs |= ((c & 16) ? 0x04 : 0);
    cs &= ~0x80;    // Start bit to 1
    cs |= 3;        // 2 stop bits 

    // send cs bit per bit
    for (int bitidx = 7; bitidx >= 0; bitidx--)
    {
        if (run_rtty_threads == 0) break;

        //measure_speed_bps(1);

        unsigned int sym_in = (cs & (1 << bitidx)) ? 1 : 0;

        for (int twice = 0; twice < 4; twice++)
        {
            if (bitidx == 0 && twice == 2) break; //last bit only once

            fskmod_modulate(modi, sym_in, &(buf_tx[0]));

            // move sample to 1,5kHz carrier
            for (int j = 0; j < k; j++)
            {
                nco_crcf_step(rtty_upnco);
                liquid_float_complex outb;
                nco_crcf_mix_up(rtty_upnco, buf_tx[j], &outb);

                float usbf = outb.real + outb.imag;

                // adapt to audio sample rate
                int fs;
                while (keeprunning && run_rtty_threads)
                {
                    fs = io_fifo_usedspace(io_pbidx);
                    //printf("%d\n", fs);
                    // attention: if this number is too low, the audio write callback will not process it
                    if (fs < 24000) break;
                    sleep_ms(10);
                }

                usbf *= 0.015f; // make RTTY signal smaller then PSK
                kmaudio_playsamples(io_pbidx, &usbf, 1, pbvol);
            }
        }
    }
}

// RTTY RX thread
#ifdef _LINUX_
void* rtty_rx_function(void* param)
{
pthread_detach(pthread_self());
#endif
#ifdef _WIN32_
void rtty_rx_function(void* param)
{
#endif

    while (keeprunning)
    {
        while (run_rtty_threads == 0)
        {
            sleep_ms(100);
            if (keeprunning == 0)
            {
#ifdef _LINUX_
                pthread_exit(NULL); // self terminate this thread
                return NULL;
#endif
#ifdef _WIN32_
                return;
#endif
            }
        }

        static int bridx = 0;
        // get available received samples
        float farr[1100];
        int ret = kmaudio_readsamples(io_capidx, farr, 1000, capvol,0);
        if (ret == 0) 
        {
            sleep_ms(10);
            continue;
        }

        for (int fanz = 0; fanz < ret; fanz++)
        {
            float f = farr[fanz];
            if (VoiceAudioMode == VOICEMODE_LISTENAUDIOIN)
                kmaudio_playsamples(voice_pbidx, &f, 1,lsvol);

            make_FFTdata(f * 25);
            
            static float last_rs = 0;
            if (rtty_RX_RADIANS_PER_SAMPLE != last_rs)
            {
                // tuning freq was changed, set NCO
                nco_crcf_set_frequency(rtty_dnnco, rtty_RX_RADIANS_PER_SAMPLE);
                last_rs = rtty_RX_RADIANS_PER_SAMPLE;
            }

            liquid_float_complex rx1500;
            rx1500.real = f;
            rx1500.imag = f;
            nco_crcf_step(rtty_dnnco);
            liquid_float_complex dc_out;
            nco_crcf_mix_down(rtty_dnnco, rx1500, &dc_out);

            // sharp filter
            firfilt_crcf_push(rtty_q, dc_out);              // push input sample
            firfilt_crcf_execute(rtty_q, &(buf_rx[bridx])); // compute output
            
            bridx++;

            if (bridx == k)
            {
                bridx = 0;

                sym_out = fskdem_demodulate(dem, buf_rx);

                int db = evalSymbols(sym_out);
                if (db != -1)
                {
                    char lt = baudot_decoder((uint8_t)db);
                    //printf("rxbyte:%02X decoded:%02X\n", db, lt);
                    if (lt > 0)
                        sendRttyToGUI(lt);
                }
            }
        }
    }

#ifdef _LINUX_
    pthread_exit(NULL); // self terminate this thread
    return NULL;
#endif
}


// get the bit level, 
// offs ... offset from beginning of the symbol buffer in bits,
// so offset 0 ist the beginning
// and offset 1 is the "overs" bit, i.e.: symbol[4]
// this eliminates the need of thinking in oversampled bits

const int maxsym = 8;   // stop-start-1-2-3-4-5-stop
const int overs = 4;    // symbols per bit
uint8_t symbuf[maxsym * overs];
int bitcnt = 0;

uint8_t getBit(int offs)
{
    // we have overs symbols per bit
    // the first or maybe last symbol may be wrong, so we don't use it
    // looks like that ignoring the first sample is the best solution
    // lets evaluate the value of three symbols
    int pos = offs * overs;
    uint8_t h = 0, l = 0;
    for (int i = 0+1; i < overs - 1+1; i++)
    {
        if (symbuf[pos + i]) h++;
        else l++;
    }

    return (h > l) ? 1 : 0;
}

uint8_t getDatebyte()
{
    uint8_t db = 0;

    db |= getBit(2) ? 0x01 : 0;
    db |= getBit(3) ? 0x02 : 0;
    db |= getBit(4) ? 0x04 : 0;
    db |= getBit(5) ? 0x08 : 0;
    db |= getBit(6) ? 0x10 : 0;

    return db;
}

// check if there is a complete frame in the symbol buffer
int findStart()
{
    if ((synced == 0 || bitcnt > overs * 6) && symbuf[3] == 1 && symbuf[4] == 0)
    {
        if (getBit(0) == 1 && getBit(1) == 0 && getBit(7) == 1)
        {
            //printf("possible Frame Detection at index:%d\n", bitcnt);
            bitcnt = 0;
            synced = 1;
            return getDatebyte();
        }
        else
            synced = 0;
    }
    bitcnt++;

    return -1;
}

int evalSymbols(uint8_t sym)
{
    // feed smbol in buffer
    memmove(symbuf, symbuf + 1, maxsym * overs - 1);
    symbuf[maxsym * overs - 1] = sym;

    //showbitstring("rx: ", symbuf, sizeof(symbuf), sizeof(symbuf));

    int db = findStart();
    if (db != -1)
    {
        //printf("Data_ %02X\n", db);
    }
    return db;
}
