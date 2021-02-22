/*
* Audio Library for Linux and Windows
* ===================================
* Author: DJ0ABR
*
* Author: Kurt Moraw, Ham radio: DJ0ABR, github: dj0abr
* License: GPL-3
*
* compilation:
* Windows ... Visual Studio
* Linux ... make
*
* Documentation see: libkmaudio.h
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
* fifo.cpp ... thread safe FIFOs
*
*/

#include "hsmodem.h"

#define NUM_OF_FIFOS 20

#ifdef WIN32
CRITICAL_SECTION fifo_crit_sec[NUM_OF_FIFOS];
#define LOCKFIFO(pn)	EnterCriticalSection(&(fifo_crit_sec[pn]))
void UNLOCKFIFO(int pn)
{
    if (&(fifo_crit_sec[pn]) != NULL)
        LeaveCriticalSection(&(fifo_crit_sec[pn]));
}
#else
pthread_mutex_t fifo_crit_sec[NUM_OF_FIFOS];
#define LOCKFIFO(pn)	pthread_mutex_lock(&(fifo_crit_sec[pn]))
#define UNLOCKFIFO(pn)	pthread_mutex_unlock(&(fifo_crit_sec[pn]))
#endif

#define FIFOBUFLEN 100      // number of fifo buffers
#define FIFOELEMENTLEN 300  // length of one fifo element

int wridx[NUM_OF_FIFOS];
int rdidx[NUM_OF_FIFOS];
int8_t buffer[NUM_OF_FIFOS][FIFOBUFLEN][FIFOELEMENTLEN];

void init_fifos()
{
    // init pipes only once
    static int f = 1;
    if (f)
    {
        f = 0;
        for (int i = 0; i < NUM_OF_FIFOS; i++)
        {
#ifdef WIN32
            if (&(fifo_crit_sec[i]) != NULL) DeleteCriticalSection(&(fifo_crit_sec[i]));
            InitializeCriticalSection(&(fifo_crit_sec[i]));
#else
            if (&(fifo_crit_sec[i]) != NULL) pthread_mutex_destroy(&(fifo_crit_sec[i]));
            pthread_mutex_init(&(fifo_crit_sec[i]), NULL);
#endif
        }
    }

    for (int i = 0; i < NUM_OF_FIFOS; i++)
        fifo_clear(i);
}

// write into the fifo
// ignore data if the fifo is full
void write_fifo(int pipenum, uint8_t *pdata, int len)
{
    if (pipenum < 0 || pipenum >= NUM_OF_FIFOS) return;

    LOCKFIFO(pipenum);
    if (((wridx[pipenum] + 1) % FIFOBUFLEN) == rdidx[pipenum])
    {
        //printf("cannot WRITE fifo %d full\n",pipenum);
        UNLOCKFIFO(pipenum);
        return;
    }

    // as the first 2 bytes store the length, MSB first
    buffer[pipenum][wridx[pipenum]][0] = len >> 8;
    buffer[pipenum][wridx[pipenum]][1] = len & 0xff;

    // followed by the data
    memcpy(buffer[pipenum][wridx[pipenum]] + 2, pdata, len);
    if (++wridx[pipenum] >= FIFOBUFLEN) wridx[pipenum] = 0;

    UNLOCKFIFO(pipenum);
}

// read from the fifo
// return: number of bytes read
int read_fifo(int pipenum, uint8_t* pdata, int maxlen)
{
    if (pipenum < 0 || pipenum >= NUM_OF_FIFOS)
    {
        printf("read_fifo: wrong pipenum:%d (%d ..%d)\n", pipenum, 0, NUM_OF_FIFOS-1);
        return 0;
    }

    LOCKFIFO(pipenum);

    if (rdidx[pipenum] == wridx[pipenum])
    {
        // Fifo empty, no data available
        //printf("read: no data\n");
        UNLOCKFIFO(pipenum);
        return 0;
    }

    // read length
    int len = buffer[pipenum][rdidx[pipenum]][0];
    len <<= 8;
    len += buffer[pipenum][rdidx[pipenum]][1];

    if (len > maxlen)
    {
        printf("read_fifo: %d, pdata too small. Need:%d has:%d\n", pipenum, len, maxlen);
        return 0; // pdata too small
    }

    // read data
    memcpy(pdata, buffer[pipenum][rdidx[pipenum]] + 2, len);
    if (++rdidx[pipenum] >= FIFOBUFLEN) rdidx[pipenum] = 0;
    UNLOCKFIFO(pipenum);

    return len;
}

void fifo_clear(int pipenum)
{
    if (pipenum < 0 || pipenum >= NUM_OF_FIFOS) return;

    wridx[pipenum] = rdidx[pipenum] = 0;
}

int fifo_freespace(int pipenum)
{
    if (pipenum < 0 || pipenum >= NUM_OF_FIFOS) return 0;

    int freebuf = 0;

    LOCKFIFO(pipenum);

    int elemInFifo = (wridx[pipenum] + FIFOBUFLEN - rdidx[pipenum]) % FIFOBUFLEN;
    freebuf = FIFOBUFLEN - elemInFifo;

    UNLOCKFIFO(pipenum);
    return freebuf;
}

int fifo_dataavail(int pipenum)
{
    LOCKFIFO(pipenum);

    if (rdidx[pipenum] == wridx[pipenum])
    {
        // Fifo empty, no data available
        UNLOCKFIFO(pipenum);
        return 0;
    }
    UNLOCKFIFO(pipenum);
    return 1;
}

int fifo_usedspace(int pipenum)
{
    int us = FIFOBUFLEN - fifo_freespace(pipenum);
    //printf("fifo:%d used space:%d\n", pipenum, us);
    return us;
}

int fifo_usedpercent(int pipenum)
{
    int used = FIFOBUFLEN - fifo_freespace(pipenum);
    int percent = (used * 100) / FIFOBUFLEN;
    return percent;
}
