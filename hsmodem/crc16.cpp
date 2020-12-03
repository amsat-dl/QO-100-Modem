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

// since we use a static crc register we need TWO separated registers
// for RX and TX to get it thread safe, no.2 is for file ID generation

uint16_t reg16[3] = {0xffff,0xffff};        // shift register

uint16_t Crc16_bytecalc(int rxtx, uint8_t byt)
{
    uint16_t polynom = 0x8408;      // generator polynom

    for (int i = 0; i < 8; ++i)
    {
        if ((reg16[rxtx] & 1) != (byt & 1))
            reg16[rxtx] = (uint16_t)((reg16[rxtx] >> 1) ^ polynom);
        else
            reg16[rxtx] >>= 1;
        byt >>= 1;
    }
    return reg16[rxtx];
}

uint16_t Crc16_messagecalc(int rxtx, uint8_t *data,int len)
{
    reg16[rxtx] = 0xffff;
    for (int i = 0; i < len; i++)
        reg16[rxtx] = Crc16_bytecalc(rxtx,data[i]);
    return reg16[rxtx];
}

// =================================================================

uint32_t reg32[2] = {0xffffffff,0xffffffff};         // Shiftregister
 
void crc32_bytecalc(int rxtx, unsigned char byte)
{
int i;
uint32_t polynom = 0xEDB88320;        // Generatorpolynom

    for (i=0; i<8; ++i)
    {
        if ((reg32[rxtx]&1) != (byte&1))
             reg32[rxtx] = (reg32[rxtx]>>1)^polynom; 
        else 
             reg32[rxtx] >>= 1;
        byte >>= 1;
    }
}

uint32_t crc32_messagecalc(int rxtx, unsigned char *data, int len)
{
int i;

    reg32[rxtx] = 0xffffffff;
    for(i=0; i<len; i++) {
        crc32_bytecalc(rxtx,data[i]);
    }
    return reg32[rxtx] ^ 0xffffffff;
}
