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

// functions for non-differential QPSK
// depending on the phase shift rotate a data blocks constellation

//uint8_t headerbytes[HEADERLEN] = {0x53, 0xe1, 0xa6};
// corresponds to these QPSK symbols:
// bits: 01010011 11100001 10100110
// syms:  1 1 0 3  3 2 0 1  2 2 1 2

uint8_t rxbytebuf[UDPBLOCKLEN+100]; // +100 ... reserve, just to be sure

uint8_t *convertQPSKSymToBytes(uint8_t *rxsymbols)
{
    int sidx = 0;
    for(int i=0; i<UDPBLOCKLEN; i++)
    {
        rxbytebuf[i] =  rxsymbols[sidx++] << (bitsPerSymbol*3);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol*2);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol*1);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol*0);
    }
    return rxbytebuf;
}

void convertBytesToSyms_QPSK(uint8_t *bytes, uint8_t *syms, int bytenum)
{
    unsigned int symidx = 0;
    for(int i=0; i<bytenum; i++)
    {
        syms[symidx++] = (bytes[i] >> 6) & 3;
        syms[symidx++] = (bytes[i] >> 4) & 3;
        syms[symidx++] = (bytes[i] >> 2) & 3;
        syms[symidx++] = (bytes[i] >> 0) & 3;
    }
}

void rotateQPSKsyms(uint8_t *src, uint8_t *dst, int len)
{
    for(int i=0; i<len; i++)
    {
        dst[i] = src[i] + 4;
        dst[i]++;
        dst[i] %= 4;
    }
}

uint8_t QPSK_backbuf[UDPBLOCKLEN*8/2];

uint8_t *rotateBackQPSK(uint8_t *buf, int len, int rotations)
{
    memcpy(QPSK_backbuf,buf,len);
    
    for(unsigned int i=0; i<(unsigned int)len; i++)
    {
        for(int r=0; r<rotations; r++)
        {
            QPSK_backbuf[i] += 4;
            QPSK_backbuf[i]--;
            QPSK_backbuf[i] %= 4;
        }
    }
    
    return QPSK_backbuf;
}

// works ONLY if number of bytes is a multiple of 3 !!!
void convertBytesToSyms_8PSK(uint8_t *bytes, uint8_t *syms, int bytenum)
{
    unsigned int symidx = 0;
    for(int i=0; i<bytenum; i+=3)
    {
        // convert next 3 bytes to 8 syms
        syms[symidx++] = (bytes[0+i] >> 5) & 7;
        syms[symidx++] = (bytes[0+i] >> 2) & 7;
        syms[symidx++] = ((bytes[0+i] & 3) << 1) | ((bytes[1+i] >> 7) & 1);
        syms[symidx++] = (bytes[1+i] >> 4) & 7;
        syms[symidx++] = (bytes[1+i] >> 1) & 7;
        syms[symidx++] = ((bytes[1+i] & 1) << 2) | ((bytes[2+i] >> 6) & 3);
        syms[symidx++] = (bytes[2+i] >> 3) & 7;
        syms[symidx++] = bytes[2+i] & 7;
    }
}

void rotate8PSKsyms(uint8_t *src, uint8_t *dst, int len)
{
    for(int i=0; i<len; i++)
    {
        dst[i] = src[i] + 8;
        dst[i]++;
        dst[i] %= 8;
    }
}

void rotate8APSKsyms(uint8_t *src, uint8_t *dst, int len)
{
    for(int i=0; i<len; i++)
    {
        if(src[i] == 0) dst[i] = 0;
        else if(src[i] == 1) dst[i] = 4;
        else if(src[i] == 2) dst[i] = 3;
        else if(src[i] == 3) dst[i] = 1;
        else if(src[i] == 4) dst[i] = 5;
        else if(src[i] == 5) dst[i] = 7;
        else if(src[i] == 6) dst[i] = 2;
        else if(src[i] == 7) dst[i] = 6;
    }
}


uint8_t _8PSK_backbuf[UDPBLOCKLEN*8/3];

uint8_t *rotateBack8PSK(uint8_t *buf, int len, int rotations)
{
    memcpy(_8PSK_backbuf,buf,len);
    
    for(int i=0; i<len; i++)
    {
        for(int r=0; r<rotations; r++)
        {
            _8PSK_backbuf[i] += 8;
            _8PSK_backbuf[i]--;
            _8PSK_backbuf[i] %= 8;
        }
    }
    return _8PSK_backbuf;
}

uint8_t *rotateBack8APSK(uint8_t *buf, int len, int rotations)
{
    memcpy(_8PSK_backbuf,buf,len);
    
    for(int i=0; i<len; i++)
    {
        for(int r=0; r<rotations; r++)
        {
            if(_8PSK_backbuf[i] == 0) _8PSK_backbuf[i] = 0;
            else if(_8PSK_backbuf[i] == 4) _8PSK_backbuf[i] = 1;
            else if(_8PSK_backbuf[i] == 3) _8PSK_backbuf[i] = 2;
            else if(_8PSK_backbuf[i] == 1) _8PSK_backbuf[i] = 3;
            else if(_8PSK_backbuf[i] == 5) _8PSK_backbuf[i] = 4;
            else if(_8PSK_backbuf[i] == 7) _8PSK_backbuf[i] = 5;
            else if(_8PSK_backbuf[i] == 2) _8PSK_backbuf[i] = 6;
            else if(_8PSK_backbuf[i] == 6) _8PSK_backbuf[i] = 7;
        }
    }
    return _8PSK_backbuf;
}

uint8_t *convert8PSKSymToBytes(uint8_t *rxsymbols, int len)
{
    int sidx = 0;
    // works ONLY if total frame length is a multiple of 3 !
    for(int i=0; i<len; i+=3)
    {
        rxbytebuf[i]   =  rxsymbols[sidx++] << 5;
        rxbytebuf[i]   |= rxsymbols[sidx++] << 2;
        rxbytebuf[i]   |= rxsymbols[sidx] >> 1;
        rxbytebuf[i+1] = rxsymbols[sidx++] << 7;
        rxbytebuf[i+1] |= rxsymbols[sidx++] << 4;
        rxbytebuf[i+1] |= rxsymbols[sidx++] << 1;
        rxbytebuf[i+1] |= rxsymbols[sidx] >> 2;
        rxbytebuf[i+2] = rxsymbols[sidx++] << 6;
        rxbytebuf[i+2] |= rxsymbols[sidx++] << 3;
        rxbytebuf[i+2] |= rxsymbols[sidx++];
    }
    return rxbytebuf;
}

void shiftleft(uint8_t *data, int shiftnum, int len)
{
    for(int j=0; j<shiftnum; j++)
    {
        int b1=0,b2=0;
        for(int i=len-1; i>=0; i--)
        {
            b1 = (data[i] & 0x80)>>7;
            data[i] <<= 1;
            data[i] |= b2;
            b2 = b1;
        }
    }
}

void rotateBPSKsyms(uint8_t* src, uint8_t* dst, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (src[i] == 0) dst[i] = 1;
        else dst[i] = 0;
    }
}

uint8_t BPSK_backbuf[UDPBLOCKLEN * 8 / 1];
uint8_t* rotateBackBPSK(uint8_t* buf, int len, int rotations)
{
    memcpy(BPSK_backbuf, buf, len);

    if (rotations == 1)
    {
        for (unsigned int i = 0; i < (unsigned int)len; i++)
        {
            if (BPSK_backbuf[i] == 1) BPSK_backbuf[i] = 0;
            else BPSK_backbuf[i] = 1;
        }
    }

    return BPSK_backbuf;
}

uint8_t* convertBPSKSymToBytes(uint8_t* rxsymbols)
{
    int sidx = 0;
    for (int i = 0; i < UDPBLOCKLEN; i++)
    {
        rxbytebuf[i]  = rxsymbols[sidx++] << (bitsPerSymbol * 7);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 6);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 5);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 4);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 3);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 2);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 1);
        rxbytebuf[i] |= rxsymbols[sidx++] << (bitsPerSymbol * 0);
    }
    return rxbytebuf;
}

void convertBytesToSyms_BPSK(uint8_t* bytes, uint8_t* syms, int bytenum)
{
    unsigned int symidx = 0;
    for (int i = 0; i < bytenum; i++)
    {
        // 1 sym per 1 bit:
        // convert next byte to 8 syms
        syms[symidx++] = (bytes[i] >> 7) & 1;
        syms[symidx++] = (bytes[i] >> 6) & 1;
        syms[symidx++] = (bytes[i] >> 5) & 1;
        syms[symidx++] = (bytes[i] >> 4) & 1;
        syms[symidx++] = (bytes[i] >> 3) & 1;
        syms[symidx++] = (bytes[i] >> 2) & 1;
        syms[symidx++] = (bytes[i] >> 1) & 1;
        syms[symidx++] = bytes[i] & 1;
    }
}

