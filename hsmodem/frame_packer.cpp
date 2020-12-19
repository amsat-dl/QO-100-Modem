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

void Insert(uint8_t bit);
uint8_t* getPayload(uint8_t* rxb);

uint8_t rxbuffer[UDPBLOCKLEN*8/1+100]; // 3...bits per symbol QPSK, enough space also for QPSK and 8PSK, +100 ... reserve, just to be sure
uint8_t rx_status = 0;

int framecounter = 0;
int lastframenum = 0;
int getPayload_error = 0;

// header for TX, 
uint8_t TXheaderbytes[HEADERLEN] = {0x53, 0xe1, 0xa6};
// corresponds to these QPSK symbols:
// bits: 01010011 11100001 10100110
// BPSK:
// syms: 01010011 11100001 10100110
// QPSK:
// syms:  1 1 0 3  3 2 0 1  2 2 1 2
// 8PSK:
// syms:   2  4   7  6  0   6  4  6

// BPSK
// each header has 24 symbols
// we have 2 constellations 
uint8_t BPSK_headertab[2][HEADERLEN * 8 / 1];

// QPSK
// each header has 12 symbols
// we have 4 constellations 
uint8_t QPSK_headertab[4][HEADERLEN * 8 / 2];

// 8PSK
// each header has 8 symbols
// we have 8 constellations 
uint8_t _8PSK_headertab[8][HEADERLEN * 8 / 3];

/*
8CONST: . Len 8: 02 04 07 06 00 06 04 06 
8CONST: . Len 8: 03 05 06 02 00 02 05 02 
8CONST: . Len 8: 01 07 02 03 00 03 07 03 
8CONST: . Len 8: 04 06 03 01 00 01 06 01 
8CONST: . Len 8: 05 02 01 04 00 04 02 04 
8CONST: . Len 8: 07 03 04 05 00 05 03 05 
8CONST: . Len 8: 06 01 05 07 00 07 01 07 
8CONST: . Len 8: 02 04 07 06 00 06 04 06 
 */

// init header tables
void init_packer()
{
    // create the BPSK symbol table for the HEADER
    // in all possible rotations
    convertBytesToSyms_BPSK(TXheaderbytes, BPSK_headertab[0], 3);
    for (int i = 1; i < 2; i++)
        rotateBPSKsyms(BPSK_headertab[i - 1], BPSK_headertab[i], 24);

    // create the QPSK symbol table for the HEADER
    // in all possible rotations
    convertBytesToSyms_QPSK(TXheaderbytes, QPSK_headertab[0], 3);
    for(int i=1; i<4; i++)
        rotateQPSKsyms(QPSK_headertab[i-1], QPSK_headertab[i], 12);
    
    // create the 8PSK symbol table for the HEADER
    // in all possible rotations
    convertBytesToSyms_8PSK(TXheaderbytes, _8PSK_headertab[0], 3);
    for(int i=1; i<8; i++)
        rotate8APSKsyms(_8PSK_headertab[i-1], _8PSK_headertab[i], 8);
    
    /*for(int i=0; i<8; i++)
        showbytestring((char*)"8CONST: ",_8PSK_headertab[i],8);*/
}

// packs a payload into an udp data block
// the payload has a size of PAYLOADLEN
// type ... inserted in the "frame type information" field
// status ... specifies first/last frame of a data stream
// repeat ... 0=normal frame, 1=repeated frame (do not increment frame counter)
uint8_t *Pack(uint8_t *payload, int type, int status, int *plen, int repeat) 
{
    FRAME frame;    // raw frame without fec 
    
    // polulate the raw frame
    
    // make the frame counter
    if(repeat == 0 || type == 1) // 1=BER test
        framecounter++;

    if (status == 0)
        framecounter = 0;   // start of file

    // insert frame counter and status bits
    frame.counter_LSB = framecounter & 0xff;
    int framecnt_MSB = (framecounter >> 8) & 0x03;  // Bit 8+9 of framecounter
    frame.status = framecnt_MSB << 6;
    frame.status += ((status & 0x03)<<4);
    frame.status += (type & 0x0f);

    //printf("type:%d stat:%d frmnum:%d fifo:%d\n", type, status, framecounter, io_pb_fifo_freespace(0));
    
    // insert the payload
    memcpy(frame.payload, payload, PAYLOADLEN);
    
    // calculate and insert the CRC16
    uint16_t crc16 = Crc16_messagecalc(CRC16TX,(uint8_t *)(&frame), CRCSECUREDLEN);
    frame.crc16_MSB = (uint8_t)(crc16 >> 8);
    frame.crc16_LSB = (uint8_t)(crc16 & 0xff);
    
    // make the final arry for transmission
    static uint8_t txblock[UDPBLOCKLEN];
    
    // calculate the fec and insert into txblock (leave space for the header)
    GetFEC((uint8_t *)(&frame), DATABLOCKLEN, txblock+HEADERLEN);
    
    // scramble
    TX_Scramble(txblock+HEADERLEN, FECBLOCKLEN); // scramble all data

    // insert the header
    memcpy(txblock,TXheaderbytes,HEADERLEN);
    
    *plen = UDPBLOCKLEN;
    return txblock;
}

#ifdef _WIN32_
#define MAXHEADERRS  5
#endif

#ifdef _LINUX_
#define MAXHEADERRS  2  // takes less CPU time, important for Rasberry PI
#endif

/*
 * Header erros will not cause any data errors because the CRC will filter out
 * false header detects,
 * but it will cause higher CPU load due to excessive execution of FEC and CRC
 */

int seekHeadersyms(int symnum)
{
    int ret = -1;
    int errs = 0;

    int exp_hdr = (UDPBLOCKLEN * 8) / bitsPerSymbol;    // we expect a new header at this symbol number
    symnum %= exp_hdr;

    int maxerr = MAXHEADERRS;

    if (constellationSize == 2)
    {
        // BPSK
        for (int tab = 0; tab < 2; tab++)
        {
            errs = 0;
            for (int i = 0; i < HEADERLEN * 8 / 1; i++)
            {
                if (rxbuffer[i] != BPSK_headertab[tab][i])
                    errs++;
            }
            if (errs <= maxerr)
            {
                ret = tab;
                break;
            }
        }
    }
    else if(constellationSize == 4)
    {
        // QPSK
        for(int tab=0; tab<4; tab++)
        {
            errs = 0;
            for(int i=0; i<HEADERLEN*8/2; i++)
            {
                if(rxbuffer[i] != QPSK_headertab[tab][i])
                    errs++;
            }
            if (errs <= maxerr)
            {
                ret = tab;
                break;
            }
        }
    }
    else
    {
        // 8PSK
        for(int tab=0; tab<8; tab++)
        {
            errs = 0;
            for(int i=0; i<HEADERLEN*8/3; i++)
            {
                if(rxbuffer[i] != _8PSK_headertab[tab][i])
                    errs++;
            }
            if(errs <= maxerr) 
            {
                ret = tab; 
                break;
            }
        }
    }

    if (ret != -1)
    {
        //printf("header detected at symbol:%d, headererrors:%d rotation:%d\n", symnum,errs,ret);
        return ret;
    }

    //if (symnum == 0) printf("header expected at symbol:%d but not found\n", symnum);
    
    return -1;
}

// unpacks a received data frame
// pdata, len ... symbols received from the modem
// returns ... payload, one full frame
uint8_t *unpack_data(uint8_t *rxd, int len)
{
    int framerdy = 0;
    static uint8_t payload[PAYLOADLEN+10];
    static int symnum = 0;
    
    rx_status = 0;
    // shift all received symbols through rxbuffer
    // until a header is detected
    for(int sym=0; sym<len; sym++)
    {
        // shift rxbuffer right by one symbol (=byte)
        // frmlen ... number of symbols
        int frmlen = UDPBLOCKLEN*8/bitsPerSymbol;
        memmove(rxbuffer,rxbuffer+1,frmlen-1);
        // insert new symbol at the top
        rxbuffer[frmlen-1] = rxd[sym];
        symnum++;
        
        
        
        int rotations = seekHeadersyms(symnum);
        if(rotations != -1) 
        {
            //showbytestring((char*)"rx: ", rxbuffer, 30,30);
            // rxbuffer contains all symbols of the received frame
            // convert to bytes
            uint8_t *rxbytes = NULL;
            if (constellationSize == 2)
            {
                uint8_t* backbuf = rotateBackBPSK(rxbuffer, frmlen, rotations);
                rxbytes = convertBPSKSymToBytes(backbuf);
            }
            else if(constellationSize == 4)
            {
                uint8_t *backbuf = rotateBackQPSK(rxbuffer,frmlen,rotations);
                rxbytes = convertQPSKSymToBytes(backbuf);
            }
            else if (constellationSize == 8)
            {
                uint8_t *backbuf = rotateBack8APSK(rxbuffer,frmlen,rotations);
                rxbytes = convert8PSKSymToBytes(backbuf,UDPBLOCKLEN);
            }
            
            if(rxbytes == NULL) return NULL;
            
            // check if we found a data block
            uint8_t *pl = getPayload(rxbytes);
            if (pl != NULL)
            {
                memcpy(payload,pl, PAYLOADLEN+10);
                framerdy = 1;
                //if(symnum != 688) printf("Header found, rotation: %d at symbol no.: %d result: OK\n", rotations, symnum);
                symnum = 0;
            }
            else
            {
                //if((symnum % ((UDPBLOCKLEN * 8) / bitsPerSymbol)) == 0) printf("Header found, rotation: %d at symbol no.: %d result: %d\n", rotations, symnum, getPayload_error);
            }
        }
    }
    if(framerdy)
        return payload;
    
    return NULL;
}

// inserts bit per bit into the rxbuffer
// rxbuffer has the size of a complete frame
// so if the header is detected, rxbuffer contains the frame
void Insert(uint8_t bit)
{
    if (bit != 0) bit = 1;
    
    // bitshift right
    for (int i = 0; i < UDPBLOCKLEN; i++)
    {
        rxbuffer[i] <<= 1;
        if (i == (UDPBLOCKLEN - 1)) break;
        uint8_t ov = (uint8_t)(rxbuffer[i + 1] & 0x80);
        if (ov != 0) rxbuffer[i] |= 1;
        else rxbuffer[i] &= 0xfe;
    }

    // insert new bit
    rxbuffer[UDPBLOCKLEN - 1] &= 0xfe;
    rxbuffer[UDPBLOCKLEN - 1] |= (uint8_t)bit;
}


uint8_t *getPayload(uint8_t *rxb)
{
    //showbytestring((char *)"orig: ",rxb+HEADERLEN,30);    
    // unscramble
    uint8_t *rxarray = RX_Scramble(rxb+HEADERLEN, FECBLOCKLEN);
            
    // calculate the FEC over the received data
    // and fill a frame structure
    FRAME frame;
    
    int ret = cfec_Reconstruct(rxarray,(uint8_t *)(&frame));
    if(ret == 0) 
    {
        //printf("fec ERROR\n");
        getPayload_error = 1;
        return NULL; // fec impossible
    }
    //printf("fec ok\n");
    
    // check the CRC
    uint16_t crc = Crc16_messagecalc(CRC16RX,(uint8_t *)(&frame), CRCSECUREDLEN);
    uint16_t rxcrc = frame.crc16_MSB;
    rxcrc <<= 8;
    rxcrc += frame.crc16_LSB;
    if (crc != rxcrc) 
    {
        //printf("crc ERROR\n");
        getPayload_error = 2;
        return NULL;    // no data found
    }
    //printf("crc OK\n");
    
    // frame counter verification: lost frame detection
    int framenumrx = (frame.status & 0xc0)>>6;  // frame counter MSB
    framenumrx <<= 8;
    framenumrx += frame.counter_LSB;        // frame counter LSB 
    
    if (lastframenum != framenumrx) rx_status |= 4;
    lastframenum = framenumrx;
    if (++lastframenum >= 1024) lastframenum = 0; // 1024 = 2^10 (10 bit frame number)
    
    // extract information and build the string for the application
    // we have 10 Management Byte then the payload follows
    static uint8_t payload[PAYLOADLEN+10];
    payload[0] = frame.status & 0x0f;       // frame type
    payload[1] = (frame.status & 0xc0)>>6;  // frame counter MSB
    payload[2] = frame.counter_LSB;         // frame counter LSB 
    payload[3] = (frame.status & 0x30)>>4;  // first/last frame marker
    payload[4] = rx_status;                 // frame lost information
    payload[5] = speed >> 8;                // measured line speed
    payload[6] = speed;
    payload[7] = 0;                         // free for later use
    payload[8] = 0;                         
    payload[9] = 0;
    
    //printf("Frame no.: %d, type:%d, minfo:%d\n",framenumrx,payload[0],payload[3]);
    
    memcpy(payload+10,frame.payload,PAYLOADLEN);
    
    return payload;
}
