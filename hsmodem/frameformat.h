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

/*
 * The total length of the FEC-secured part is 255, 
 * this is a requirement of the Shifra FEC routine, which
 * is the best FEC that I have seen so far, highly recommended
*/

// total "on the air" frame size
// the total length must be a multiple of 2 and 3, so QPSK and 8PSK symbols fit into full bytes
// this is the case with a total length of 258
#define HEADERLEN   3   
#define FECBLOCKLEN 255
#define UDPBLOCKLEN (HEADERLEN + FECBLOCKLEN)

/* !!! IMPORTANT for GNU RADIO !!!
 * the UDP payload size for TX MUST be exactly UDPBLOCKLEN (258 in this case) or
 * the transmitter will not align bits to symbols correctly !
 * 
 * RX payload size is not that important. But the currect size for
 * QPSK is UDPBLOCKLEN*8/2 = 1032 and for 8PSK UDPBLOCKLEN*8/3 = 688
 * so we can use 344 which are 2 blocks for 8PSK and 3 blocks for QPSK
 * */

// size of the elements inside an FECblock
// sum must be 255
#define FECLEN      32  // supported: 16,32,64,128
#define STATUSLEN   2
#define CRCLEN      2
#define PAYLOADLEN  (FECBLOCKLEN - FECLEN - CRCLEN - STATUSLEN)
#define CRCSECUREDLEN (PAYLOADLEN + STATUSLEN)
#define DATABLOCKLEN   (PAYLOADLEN + CRCLEN + STATUSLEN)


// the header is not FEC secured therefore we give some room for bit
// errors. Only 24 out of the 32 bits must be correct for
// a valid frame detection
extern uint8_t header[HEADERLEN];

typedef struct {
    // the total size of the following data must be 255 - 32 = 223 bytes
    // the FEC is calculated on FRAME with a length of 223 and returns
    // a data block with length 255.
    
    // we use a 10 bits frame counter -> 1024 values
    // so we can transmit a data block with a maximum
    // size of 255 * 1024 = 261kByte. With the maximum modem speed
    // this would be a transmission time of 5,8 minutes which
    // is more then enough for a single data block
    uint8_t counter_LSB;    // lower 8 bits of the frame counter
    
    // the status byte contains these information:
    // bit 0..3 : 4 bit (16 values) frame type information
    // bit 4 : first frame of a block if "1"
    // bit 5 : last frame of a block if "1"
    // bit 6..7 : MSB of the frame counter
    uint8_t status;
    
    // payload
    uint8_t payload[PAYLOADLEN];
    
    // CRC16
    uint8_t crc16_MSB;
    uint8_t crc16_LSB;
} FRAME;
