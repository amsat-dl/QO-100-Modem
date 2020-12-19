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
* fifo_voice.c ... thread safe buffer for DV audio
*
*
*/

#include "hsmodem.h"

#ifdef _WIN32_
CRITICAL_SECTION io_mic_crit_sec;
CRITICAL_SECTION io_ls_crit_sec;
#define IO_MIC_LOCK	EnterCriticalSection(&io_mic_crit_sec)
#define IO_LS_LOCK	EnterCriticalSection(&io_ls_crit_sec)
void IO_MIC_UNLOCK()
{
    if (&io_mic_crit_sec != NULL)
        LeaveCriticalSection(&io_mic_crit_sec);
}
void IO_LS_UNLOCK()
{
    if (&io_ls_crit_sec != NULL)
        LeaveCriticalSection(&io_ls_crit_sec);
}
#endif

#ifdef _LINUX_
pthread_mutex_t io_mic_crit_sec;
pthread_mutex_t io_ls_crit_sec;
#define IO_MIC_LOCK	pthread_mutex_lock(&io_mic_crit_sec)
void IO_MIC_UNLOCK() { pthread_mutex_unlock(&io_mic_crit_sec); }
#define IO_LS_LOCK	pthread_mutex_lock(&io_ls_crit_sec)
void IO_LS_UNLOCK() { pthread_mutex_unlock(&io_ls_crit_sec); }
#endif

#define VOICE_PLAYBACK_BUFLEN (500000)
#define VOICE_CAPTURE_BUFLEN  (10000) //48000)// * 10) // space for 10 seconds of samples

int io_mic_wridx = 0;
int io_mic_rdidx = 0;
float io_mic_buffer[VOICE_CAPTURE_BUFLEN];

int io_ls_wridx = 0;
int io_ls_rdidx = 0;
float io_ls_buffer[VOICE_PLAYBACK_BUFLEN];

void io_voice_init_pipes()
{
#ifdef _WIN32_
    if (&io_mic_crit_sec != NULL) DeleteCriticalSection(&io_mic_crit_sec);
    InitializeCriticalSection(&io_mic_crit_sec);

    if (&io_ls_crit_sec != NULL) DeleteCriticalSection(&io_ls_crit_sec);
    InitializeCriticalSection(&io_ls_crit_sec);

    io_clear_audio_fifos();
#endif
}

// write one sample into the fifo
// overwrite old data if the fifo is full
void io_mic_write_fifo(float sample)
{
    IO_MIC_LOCK;

    if (((io_mic_wridx + 1) % VOICE_CAPTURE_BUFLEN) == io_mic_rdidx)
    {
        //printf("mic fifo full\n");
        IO_MIC_UNLOCK();
        return;
    }

    io_mic_buffer[io_mic_wridx] = sample;
    if (++io_mic_wridx >= VOICE_CAPTURE_BUFLEN) io_mic_wridx = 0;
    IO_MIC_UNLOCK();
}

int io_mic_read_fifo(float* data)
{
    IO_MIC_LOCK;

    if (io_mic_rdidx == io_mic_wridx)
    {
        // Fifo empty, no data available
        IO_MIC_UNLOCK();
        return 0;
    }

    *data = io_mic_buffer[io_mic_rdidx];
    if (++io_mic_rdidx >= VOICE_CAPTURE_BUFLEN) io_mic_rdidx = 0;
    IO_MIC_UNLOCK();

    return 1;
}


void io_mic_write_fifo_clear()
{
    io_mic_wridx = io_mic_rdidx = 0;
}

int io_mic_fifo_freespace()
{
    int freebuf = 0;

    IO_MIC_LOCK;

    int elemInFifo = (io_mic_wridx + VOICE_CAPTURE_BUFLEN - io_mic_rdidx) % VOICE_CAPTURE_BUFLEN;
    freebuf = VOICE_CAPTURE_BUFLEN - elemInFifo;

    IO_MIC_UNLOCK();

    return freebuf;
}

int io_mic_fifo_usedPercent()
{
    int fs = io_mic_fifo_freespace();
    int used = VOICE_CAPTURE_BUFLEN - fs;
    used = (used * 100) / VOICE_CAPTURE_BUFLEN;
    return used;
}

void io_ls_write_fifo_clear()
{
    io_ls_wridx = io_ls_rdidx = 0;
}

int io_ls_fifo_freespace(int nolock)
{
    int freebuf = 0;

    if (nolock == 0) IO_LS_LOCK;

    int elemInFifo = (io_ls_wridx + VOICE_PLAYBACK_BUFLEN - io_ls_rdidx) % VOICE_PLAYBACK_BUFLEN;
    freebuf = VOICE_PLAYBACK_BUFLEN - elemInFifo;

    if (nolock == 0) IO_LS_UNLOCK();

    //printf("fifolen:%d check: pbw:%d pbr:%d freebuf:%d\n",VOICE_PLAYBACK_BUFLEN,io_ls_wridx,io_ls_rdidx,freebuf);

    return freebuf;
}

int io_ls_fifo_usedspace()
{
    int anz = io_ls_fifo_freespace(0);
    return VOICE_PLAYBACK_BUFLEN - anz;
}

void io_ls_write_fifo(float sample)
{
    IO_LS_LOCK;

    // check if there is free space in fifo
    if (io_ls_fifo_freespace(1) == 0)
    {
        IO_LS_UNLOCK();
        printf("************* LS fifo full\n");
        return;
    }

    io_ls_buffer[io_ls_wridx] = sample;
    if (++io_ls_wridx >= VOICE_PLAYBACK_BUFLEN) io_ls_wridx = 0;
    IO_LS_UNLOCK();
}

// read num elements
// if num elems not avail, return 0
int io_ls_read_fifo_num(float* data, int num)
{
    IO_LS_LOCK;

    int elemInFifo = (io_ls_wridx + VOICE_PLAYBACK_BUFLEN - io_ls_rdidx) % VOICE_PLAYBACK_BUFLEN;

    if (elemInFifo < num)
    {
        // Fifo empty, no data available
        //printf("only %d elements available\n", elemInFifo);
        IO_LS_UNLOCK();
        return 0;
    }

    for (int i = 0; i < num; i++)
    {
        *data++ = io_ls_buffer[io_ls_rdidx];
        if (++io_ls_rdidx >= VOICE_PLAYBACK_BUFLEN) io_ls_rdidx = 0;
    }
    IO_LS_UNLOCK();

    return 1;
}

void io_clear_voice_fifos()
{
    io_ls_write_fifo_clear();
    io_mic_write_fifo_clear();
}
