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
* fifo.c ... thread safe buffer for audio I/O
*
*
*/

#include "hsmodem.h"

void rtty_init_pipes();

#ifdef _WIN32_
CRITICAL_SECTION io_cap_crit_sec;
CRITICAL_SECTION io_pb_crit_sec;
#define IO_CAP_LOCK	EnterCriticalSection(&io_cap_crit_sec)
#define IO_PB_LOCK	    EnterCriticalSection(&io_pb_crit_sec)
void IO_CAP_UNLOCK()
{
    if (&io_cap_crit_sec != NULL)
        LeaveCriticalSection(&io_cap_crit_sec);
}
void IO_PB_UNLOCK()
{
    if (&io_pb_crit_sec != NULL)
        LeaveCriticalSection(&io_pb_crit_sec);
}
#endif

#ifdef _LINUX_
pthread_mutex_t io_cap_crit_sec;
pthread_mutex_t io_pb_crit_sec;
#define IO_CAP_LOCK	pthread_mutex_lock(&io_cap_crit_sec)
void IO_CAP_UNLOCK() { pthread_mutex_unlock(&io_cap_crit_sec); }
#define IO_PB_LOCK	pthread_mutex_lock(&io_pb_crit_sec)
void IO_PB_UNLOCK() { pthread_mutex_unlock(&io_pb_crit_sec); }
#endif

#define AUDIO_PLAYBACK_BUFLEN (48000 * 15) // space for 10 seconds of samples
#define AUDIO_CAPTURE_BUFLEN  (10000) //48000)// * 10) // space for 10 seconds of samples

int io_cap_wridx = 0;
int io_cap_rdidx = 0;
float io_cap_buffer[AUDIO_CAPTURE_BUFLEN];

int io_pb_wridx = 0;
int io_pb_rdidx = 0;
float io_pb_buffer[AUDIO_PLAYBACK_BUFLEN];

void io_init_pipes()
{
#ifdef _WIN32_
    if (&io_cap_crit_sec != NULL) DeleteCriticalSection(&io_cap_crit_sec);
    InitializeCriticalSection(&io_cap_crit_sec);

    if (&io_pb_crit_sec != NULL) DeleteCriticalSection(&io_pb_crit_sec);
    InitializeCriticalSection(&io_pb_crit_sec);

    io_clear_audio_fifos();
#endif

    io_voice_init_pipes();
    rtty_init_pipes();
}

// write one sample into the fifo
// overwrite old data if the fifo is full
void io_cap_write_fifo(float sample)
{
    if (((io_cap_wridx + 1) % AUDIO_CAPTURE_BUFLEN) == io_cap_rdidx)
    {
        //printf("cap fifo full\n");
        return;
    }

    IO_CAP_LOCK;
    io_cap_buffer[io_cap_wridx] = sample;
    if (++io_cap_wridx >= AUDIO_CAPTURE_BUFLEN) io_cap_wridx = 0;
    IO_CAP_UNLOCK();
}

int io_cap_read_fifo(float* data)
{
    IO_CAP_LOCK;

    if (io_cap_rdidx == io_cap_wridx)
    {
        // Fifo empty, no data available
        IO_CAP_UNLOCK();
        return 0;
    }

    *data = io_cap_buffer[io_cap_rdidx];
    if (++io_cap_rdidx >= AUDIO_CAPTURE_BUFLEN) io_cap_rdidx = 0;
    IO_CAP_UNLOCK();

    return 1;
}


void io_cap_write_fifo_clear()
{
    io_cap_wridx = io_cap_rdidx = 0;
}

int io_cap_fifo_freespace()
{
    int freebuf = 0;

    IO_CAP_LOCK;

    int elemInFifo = (io_cap_wridx + AUDIO_CAPTURE_BUFLEN - io_cap_rdidx) % AUDIO_CAPTURE_BUFLEN;
    freebuf = AUDIO_CAPTURE_BUFLEN - elemInFifo;

    IO_CAP_UNLOCK();

    return freebuf;
}

int io_cap_fifo_usedPercent()
{
    int fs = io_cap_fifo_freespace();
    int used = AUDIO_CAPTURE_BUFLEN - fs;
    used = (used * 100) / AUDIO_CAPTURE_BUFLEN;
    return used;
}

void io_pb_write_fifo(float sample)
{
    IO_PB_LOCK;

    // check if there is free space in fifo
    if (io_pb_fifo_freespace(1) == 0)
    {
        IO_PB_UNLOCK();
        printf("************* pb fifo full\n");
        return;
    }

    io_pb_buffer[io_pb_wridx] = sample;
    if (++io_pb_wridx >= AUDIO_PLAYBACK_BUFLEN) io_pb_wridx = 0;
    IO_PB_UNLOCK();
    //printf("write: pbw:%d pbr:%d\n",io_pb_wridx,io_pb_rdidx);
}

void io_pb_write_fifo_clear()
{
    io_pb_wridx = io_pb_rdidx = 0;
}

int io_pb_fifo_usedBlocks()
{
    int fs = io_pb_fifo_freespace(0);
    int used = AUDIO_PLAYBACK_BUFLEN - fs;
    used /= (txinterpolfactor * UDPBLOCKLEN * 8 / bitsPerSymbol);
    return used;
}

int io_pb_fifo_freespace(int nolock)
{
    int freebuf = 0;

    if (nolock == 0) IO_PB_LOCK;

    int elemInFifo = (io_pb_wridx + AUDIO_PLAYBACK_BUFLEN - io_pb_rdidx) % AUDIO_PLAYBACK_BUFLEN;
    freebuf = AUDIO_PLAYBACK_BUFLEN - elemInFifo;

    if (nolock == 0) IO_PB_UNLOCK();

    //printf("fifolen:%d check: pbw:%d pbr:%d freebuf:%d\n",AUDIO_PLAYBACK_BUFLEN,io_pb_wridx,io_pb_rdidx,freebuf);

    return freebuf;
}

int io_pb_fifo_usedspace()
{
    IO_PB_LOCK;
    int elemInFifo = (io_pb_wridx + AUDIO_PLAYBACK_BUFLEN - io_pb_rdidx) % AUDIO_PLAYBACK_BUFLEN;
    IO_PB_UNLOCK();

    return elemInFifo;
    /*
    int anz = io_pb_fifo_freespace(0);
    return AUDIO_PLAYBACK_BUFLEN - anz;*/
}

// read num elements
// if num elems not avail, return all what fifo has stored
int io_pb_read_fifo_num(float* data, int num)
{
    IO_PB_LOCK;

    int elemInFifo = (io_pb_wridx + AUDIO_PLAYBACK_BUFLEN - io_pb_rdidx) % AUDIO_PLAYBACK_BUFLEN;

    if (elemInFifo == 0)
    {
        // Fifo empty, no data available
        //printf("only %d elements available\n", elemInFifo);
        IO_PB_UNLOCK();
        return 0;
    }

    if (num > elemInFifo)
        num = elemInFifo;

    for (int i = 0; i < num; i++)
    {
        *data++ = io_pb_buffer[io_pb_rdidx];
        if (++io_pb_rdidx >= AUDIO_PLAYBACK_BUFLEN) io_pb_rdidx = 0;
    }
    IO_PB_UNLOCK();
    
    return num;
}

void io_clear_audio_fifos()
{
    io_pb_write_fifo_clear();
    io_cap_write_fifo_clear();
}

// ================== RTTY FIFO ===================

void clear_rtty_fifos();

#ifdef _WIN32_
CRITICAL_SECTION rtty_tx_crit_sec;
CRITICAL_SECTION rtty_rx_crit_sec;
#define RTTY_TX_LOCK	EnterCriticalSection(&rtty_tx_crit_sec)
#define RTTY_RX_LOCK	    EnterCriticalSection(&rtty_rx_crit_sec)
void RTTY_TX_UNLOCK()
{
    if (&rtty_tx_crit_sec != NULL)
        LeaveCriticalSection(&rtty_tx_crit_sec);
}
void RTTY_RX_UNLOCK()
{
    if (&rtty_rx_crit_sec != NULL)
        LeaveCriticalSection(&rtty_rx_crit_sec);
}
#endif

#ifdef _LINUX_
pthread_mutex_t rtty_tx_crit_sec;
pthread_mutex_t rtty_rx_crit_sec;
#define RTTY_TX_LOCK	pthread_mutex_lock(&rtty_tx_crit_sec)
void RTTY_TX_UNLOCK() { pthread_mutex_unlock(&rtty_tx_crit_sec); }
#define RTTY_RX_LOCK	pthread_mutex_lock(&rtty_rx_crit_sec)
void RTTY_RX_UNLOCK() { pthread_mutex_unlock(&rtty_rx_crit_sec); }
#endif

void rtty_init_pipes()
{
#ifdef _WIN32_
    if (&rtty_tx_crit_sec != NULL) DeleteCriticalSection(&rtty_tx_crit_sec);
    InitializeCriticalSection(&rtty_tx_crit_sec);

    if (&rtty_rx_crit_sec != NULL) DeleteCriticalSection(&rtty_rx_crit_sec);
    InitializeCriticalSection(&rtty_rx_crit_sec);

#endif

    clear_rtty_fifos();
}

#define RTTY_FIFOLEN    200

int rtty_tx_wridx = 0;
int rtty_tx_rdidx = 0;
char rtty_tx_buffer[RTTY_FIFOLEN];

int rtty_rx_wridx = 0;
int rtty_rx_rdidx = 0;
char rtty_rx_buffer[RTTY_FIFOLEN];

// TX char from GUI to RTTY TX thread

void clear_rtty_fifos()
{
    rtty_tx_wridx = rtty_tx_rdidx = 0;
    rtty_rx_wridx = rtty_rx_rdidx = 0;
}

int rtty_tx_fifo_freespace()
{
    int elemInFifo = (rtty_tx_wridx + RTTY_FIFOLEN - rtty_tx_rdidx) % RTTY_FIFOLEN;
    return RTTY_FIFOLEN - elemInFifo;
}

void clear_rtty_txfifo()
{
    RTTY_TX_LOCK;
    rtty_tx_wridx = rtty_tx_rdidx = 0;
    RTTY_TX_UNLOCK();
}

void rtty_tx_write_fifo(char c)
{
    RTTY_TX_LOCK;

    // check if there is free space in fifo
    if (rtty_tx_fifo_freespace() == 0)
    {
        RTTY_TX_UNLOCK();
        return;
    }

    rtty_tx_buffer[rtty_tx_wridx] = c;
    if (++rtty_tx_wridx >= RTTY_FIFOLEN) rtty_tx_wridx = 0;
    RTTY_TX_UNLOCK();
}

int rtty_tx_read_fifo(char *pc)
{
    RTTY_TX_LOCK;

    if (rtty_tx_rdidx == rtty_tx_wridx)
    {
        // Fifo empty, no data available
        RTTY_TX_UNLOCK();
        return 0;
    }

    *pc = rtty_tx_buffer[rtty_tx_rdidx];
    if (++rtty_tx_rdidx >= RTTY_FIFOLEN) rtty_tx_rdidx = 0;
    RTTY_TX_UNLOCK();

    return 1;
}

int rtty_rx_fifo_freespace()
{
    int elemInFifo = (rtty_rx_wridx + RTTY_FIFOLEN - rtty_rx_rdidx) % RTTY_FIFOLEN;
    return RTTY_FIFOLEN - elemInFifo;
}

void rtty_rx_write_fifo(char c)
{
    RTTY_RX_LOCK;

    // check if there is free space in fifo
    if (rtty_rx_fifo_freespace() == 0)
    {
        RTTY_RX_UNLOCK();
        return;
    }

    rtty_rx_buffer[rtty_rx_wridx] = c;
    if (++rtty_rx_wridx >= RTTY_FIFOLEN) rtty_rx_wridx = 0;
    RTTY_RX_UNLOCK();
}

int rtty_rx_read_fifo(char* pc)
{
    RTTY_RX_LOCK;

    if (rtty_rx_rdidx == rtty_rx_wridx)
    {
        // Fifo empty, no data available
        RTTY_RX_UNLOCK();
        return 0;
    }

    *pc = rtty_rx_buffer[rtty_rx_rdidx];
    if (++rtty_rx_rdidx >= RTTY_FIFOLEN) rtty_rx_rdidx = 0;
    RTTY_RX_UNLOCK();

    return 1;
}
