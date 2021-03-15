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
* extdata.c ... handle external data coming via udp 40135
* 
* Data format (Packet Length: 219 Byte, fits into one HSmodem payload)
* ====================================================================
* Byte 0 ... Data Type			
* Byte 1 ... Length				
* Byte 2-218 .. data (217 Bytes)
* 
* Data Type:
* types 0-31 ... reserved for HSmodem's internal use
* type 32-255 .. available for public use. Registration recommended to avoid identical use by different apps
* already defined by HSmodem:
* type 0 ... payload contains DX-cluster messages as ASCII text
* type 1 ... NB spectrum data
* 
* Length:
* length of the data field, maximum: 217 
*
*/

#include "hsmodem.h"

void makeSpecData(uint8_t* pdata, int len);
void handleNBSpecData(uint8_t *pdata, int len);
void makeWBSpecData(uint8_t* pdata, int len);
void handleWBSpecData(uint8_t* pdata, int len);

uint32_t extDataID = 0x7743fa9f;

// message received on UDP port 40135
void ext_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{
    if (extData_active == 0) return;

    uint32_t id = pdata[0];
    id <<= 8;
    id += pdata[1];
    id <<= 8;
    id += pdata[2];
    id <<= 8;
    id += pdata[3];

    //printf("incoming data on 40135, ID: %d, pdata[4]:%d\n", id,pdata[4]);

    if (id != extDataID)
    {
        printf("incoming data on 40135, wrong ID: %d\n", id);
        return;
    }

    if (pdata[4] == 0)
    {
        // DX cluster message
        printf("DX cluster message received: <%s>\n", pdata + 5);
        // pdata MUST have size: PAYLOADLEN
        if (len != PAYLOADLEN)
        {
            printf("ext_rxdata wrong size:%d, need:%d, ignoring\n", len, PAYLOADLEN);
            return;
        }

        // 8 ... ExternalData
        // 3 ... SingleFrame
        // 1 ... repeat frame if TX currently down
        modem_sendPSKData(pdata + 4, 8, 3, 1, EXT_TX);
    }

    else if (pdata[4] == 1)
    {
        // NB spectrum data
        makeSpecData(pdata + 4 + 1, len - 1 - 4);
    }

    else if (pdata[4] == 2)
    {
        // CW Skimmer data

        // generate a full payload, padded with zeros
        uint8_t payload[PAYLOADLEN];
        memset(payload, 0, PAYLOADLEN);
        if (len > PAYLOADLEN) len = PAYLOADLEN; // just for security, will usually never happen
        memcpy(payload, pdata + 4, len-4);

        printf("external CW Skimmer message ID: %d msglen:%d message<%s>\n", pdata[4], len, payload);

        // 8 ... ExternalData
        // 3 ... SingleFrame
        // 1 ... repeat frame if TX currently down
        modem_sendPSKData(payload, 8, 3, 1, EXT_TX);
    }

    else if (pdata[4] == 3)
    {
        // WB spectrum data
        makeWBSpecData(pdata + 4 + 1, len - 1 - 4);
    }

    else
    {
        printf("external message: %d msglen: %d unknown\n", pdata[0], len);
    }
}

// message received by modem
// length of pdata is PAYLOADLEN+10 (see frame_packer.c: getPayload())
void ext_modemRX(uint8_t* pdata)
{
	static uint8_t lastpl[PAYLOADLEN];
    static uint8_t lastcwpl[PAYLOADLEN];

	uint8_t* payload = pdata + 10;
    // the first byte is the external-type specifier
    if (payload[0] == 0)
    {
        if (memcmp(payload, lastpl, PAYLOADLEN))
        {
            // new frame received
            memcpy(lastpl, payload, PAYLOADLEN);

            // DX-cluster message
            // send to websocket
            ws_send(payload, PAYLOADLEN);
        }
    }

    if (payload[0] == 1)
    {
        handleNBSpecData(payload, PAYLOADLEN);
    }

    if (payload[0] == 2)
    {
        if (memcmp(payload, lastcwpl, PAYLOADLEN))
        {
            // new frame received
            memcpy(lastcwpl, payload, PAYLOADLEN);

            // CW skimmer message
            // send to websocket
            ws_send(payload, PAYLOADLEN);
        }
    }

    if (payload[0] == 3)
    {
        handleWBSpecData(payload, PAYLOADLEN);
    }

    // type=16 is also in use, see hsmodem (Bulletin)
}

/*
* pdata: array of 550 16-bit values
* starting at 10489.475 with a resolution of 1 kHz
* up to 10490.025 which is a range of 550kHz (550 16bit values)
* left beacon 10489.500 is at index 25 (24..26)
*/
void makeSpecData(uint8_t *pdata, int len)
{
    // check if TX fifo has data already
    int us = fifo_usedspace(EXT_SPECNB);
    if (us > 1) return; // ignore data

    const int mlen = 550;
    if (len > mlen) len = mlen;

    // convert into 16 bit values
    uint16_t sval[mlen];
    for (int i = 0; i < mlen; i++)
    {
        sval[i] = pdata[2 * i];
        sval[i] <<= 8;
        sval[i] += pdata[2 * i + 1];
    }

    // measure value of left beacon
    int vmax = 0;
    for (int i = 24; i <= 26; i++)
        if (sval[i] > vmax) vmax = sval[i];
    if (vmax < 1) vmax = 1; // avoid divide by zero error

    //printf("Beaconlevel: %d\n",vmax);

    // normalize to beaconlevel, which is 100%
    // and 100% is 6 bit maximum, which is 63
    // also reduce length by 2, resulting in 275 values
    const int vlen = mlen / 2;
    uint8_t snormval[vlen];
    int idx = 0;
    for (int i = 0; i < mlen; i+=2)
    {
        if (idx >= vlen)
        {
            printf("vlen too small\n");
            break; // just for security, will never happen
        }
        snormval[idx] = (uint8_t)((63 * sval[i]) / vmax);
        uint8_t v = (uint8_t)((63 * sval[i+1]) / vmax);
        if (v > snormval[idx]) snormval[idx] = v;
        idx++;
    }

    // here we have 275 values with a resolution of  2kHz
    // each value is 6 bit long
    // so we have 275 * 6 = 1650 bit, which is 207 byte, 
    // and fits into the extData payload of 217 byte

    //showbytestring("TX:",snormval,30,30);

    // store in average buffer
    static uint16_t avgbuf[vlen];
    static int avganz = 0;
    for (int i = 0; i < idx; i++)
        //avgbuf[i] += snormval[i];
        if(avgbuf[i] < snormval[i]) avgbuf[i] = snormval[i];
    avganz++;

    /*
    // check if TX fifo has data already
    int us = fifo_usedspace(EXT_SPECNB);
    if (us > 1) return;

    
    // check if audio playback fifo is filled already
    us = io_fifo_usedspace(io_pbidx);
    if (us > 48000) return; // max 1s latency
    */

    // build average
    //for (int i = 0; i < idx; i++)
      //  avgbuf[i] /= avganz;
    avganz = 0;

    // snormval has 6-bit values, each in one byte
    // convert it to a bitstream
    uint8_t bitstream[PAYLOADLEN];    // the result will be shorter
    int sbyte = 0, sbit = 0;
    int dbyte = 1, dbit = 0;            // dbyte=1 because bitstream[0] is the message ID
    memset(bitstream, 0, sizeof(bitstream));
    while (1)
    {
        // read actual bit
        uint8_t bit = avgbuf[sbyte] & (1 << sbit);
        if (bit) bit = 1;
        // write into bitstream
        bitstream[dbyte] |= (bit << dbit);
        // move source to next position
        if (++sbit >= 6)
        {
            sbit = 0;
            sbyte++;
            if (sbyte == idx) break;    // finished
        }
        // move destination to next position
        if (++dbit >= 8)
        {
            dbit = 0;
            dbyte++;
        }
        if (dbyte >= PAYLOADLEN)
        {
            printf("dbyte wrong:%d max is %d\n", dbyte, PAYLOADLEN);
            break;
        }
    }
    memset(avgbuf, 0, vlen * sizeof(uint16_t));
    // data in: bitstream, length of data: dbyte

    // send to modem
    // 8 ... ExternalData
    // 3 ... SingleFrame
    // 1 ... repeat frame if TX currently down

    bitstream[0] = 1;   // message ID for spectrum data
    modem_sendPSKData(bitstream, 8, 3, 1, EXT_SPECNB);
}

void handleNBSpecData(uint8_t *pdata, int len)
{
    // extract into original data
    uint8_t odata[1000];
    int sby = 1, sbi = 0;
    int dby = 1, dbi = 0;
    memset(odata, 0, sizeof(odata));
    while (1)
    {
        // read actual bit
        uint8_t bit = pdata[sby] & (1 << sbi);
        if (bit) bit = 1;
        // write into orig data
        odata[dby] |= (bit << dbi);
        // move source to next position
        if (++sbi >= 8)
        {
            sbi = 0;
            sby++;
            if (sby >= len) break;    // finished
        }
        // move destination to next position
        if (++dbi >= 6)
        {
            dbi = 0;
            dby++;
        }
    }

    //showbytestring("RX:", odata, 30, 30);

    // send to websocket
    odata[0] = 1;
    ws_send(odata, dby);
}

/*
* Spectrum data format as received by the browser:
* ------------------------------------------------
* Byte 0 ... fixed to 0
* Byte 1 ... length MSB
* Byte 2 ... length LSB
* Byte 3 ... =1 identifies the message as NB spectrum
* Byte 4-278 ... spectrum data (275 values)
* 
* spectrum data:
* --------------
* 10489.525 - 10490.025 = 550kHz Resolution 2 kHz: 275 values
* each value has 6 bit. 0x3f is the maximum
*/

// WB Transponder 

/*
* pdata: array of 266 16-bit values
* starting at 10491.500 with a resolution of 30 kHz
* up to 10499.500 which is a range of 8000kHz (266 16bit values)
* left beacon 10491.500 is at index 0 (0..16)
*/
void makeWBSpecData(uint8_t* pdata, int len)
{
    // check if TX fifo has data already
    int us = fifo_usedspace(EXT_SPECWB);
    if (us > 1) return; // ignore data

    const int mlen = 266;
    if (len > mlen) len = mlen;

    // convert into 16 bit values
    uint16_t sval[mlen];
    for (int i = 0; i < mlen; i++)
    {
        sval[i] = pdata[2 * i];
        sval[i] <<= 8;
        sval[i] += pdata[2 * i + 1];
    }

    // measure value of left beacon
    int vmax = 0;
    for (int i = 0; i <= 16; i++)
        if (sval[i] > vmax) vmax = sval[i];
    if (vmax < 1) vmax = 1; // avoid divide by zero error

    //printf("Beaconlevel: %d\n",vmax);
    //showbytestring16("gultiti:", sval, 40);

    // substract 0-level then
    // normalize to beaconlevel, which is 100%
    // and 100% is 6 bit maximum, which is 63
    uint16_t WBnullLevel = 350;
    uint8_t snormval[mlen];
    int idx = 0;
    vmax -= WBnullLevel;
    if (vmax < 0) vmax = 0;
    for (int i = 0; i < mlen; i++)
    {
        int nv = sval[i] - WBnullLevel;
        if (nv < 0) nv = 0;
        uint8_t va = (uint8_t)((63 * nv) / vmax);
        if (va > 63) va = 63;
        snormval[idx] = va;
        idx++;
    }

    // here we have 266 values with a resolution of  30kHz
    // each value is 6 bit long
    // so we have 266 * 6 = 1596 bit, which is 199 byte, 
    // and fits into the extData payload of 217 byte

    //showbytestring("TX:",snormval,30,30);

    // store in average buffer
    static uint16_t avgbuf[mlen];
    static int avganz = 0;
    for (int i = 0; i < idx; i++)
        //avgbuf[i] += snormval[i];
        if (avgbuf[i] < snormval[i]) avgbuf[i] = snormval[i];
    avganz++;
    /*
    // check if TX fifo has data already
    int us = fifo_usedspace(EXT_SPECWB);
    if (us > 1) return;

    // check if audio playback fifo is filled already
    us = io_fifo_usedspace(io_pbidx);
    if (us > 48000) return; // max 1s latency
    */
    // build average
    //for (int i = 0; i < idx; i++)
      //  avgbuf[i] /= avganz;
    avganz = 0;

    // snormval has 6-bit values, each in one byte
    // convert it to a bitstream
    uint8_t bitstream[PAYLOADLEN];    // the result will be shorter
    int sbyte = 0, sbit = 0;
    int dbyte = 1, dbit = 0;            // dbyte=1 because bitstream[0] is the message ID
    memset(bitstream, 0, sizeof(bitstream));
    while (1)
    {
        // read actual bit
        uint8_t bit = avgbuf[sbyte] & (1 << sbit);
        if (bit) bit = 1;
        // write into bitstream
        bitstream[dbyte] |= (bit << dbit);
        // move source to next position
        if (++sbit >= 6)
        {
            sbit = 0;
            sbyte++;
            if (sbyte == idx) break;    // finished
        }
        // move destination to next position
        if (++dbit >= 8)
        {
            dbit = 0;
            dbyte++;
        }
        if (dbyte >= PAYLOADLEN)
        {
            printf("dbyte wrong:%d max is %d\n", dbyte, PAYLOADLEN);
            break;
        }
    }
    memset(avgbuf, 0, mlen * sizeof(uint16_t));
    // data in: bitstream, length of data: dbyte

    // send to modem
    // 8 ... ExternalData
    // 3 ... SingleFrame
    // 1 ... repeat frame if TX currently down

    bitstream[0] = 3;   // message ID for WB spectrum data
    modem_sendPSKData(bitstream, 8, 3, 1, EXT_SPECWB);
}

// WB data received via HF
void handleWBSpecData(uint8_t* pdata, int len)
{
    // extract into original data
    uint8_t odata[1000];
    int sby = 1, sbi = 0;
    int dby = 1, dbi = 0;
    memset(odata, 0, sizeof(odata));
    while (1)
    {
        // read actual bit
        uint8_t bit = pdata[sby] & (1 << sbi);
        if (bit) bit = 1;
        // write into orig data
        odata[dby] |= (bit << dbi);
        // move source to next position
        if (++sbi >= 8)
        {
            sbi = 0;
            sby++;
            if (sby >= len) break;    // finished
        }
        // move destination to next position
        if (++dbi >= 6)
        {
            dbi = 0;
            dby++;
        }
    }

    //showbytestring("RX:", odata, 30, 30);

    // send to websocket
    odata[0] = 3;
    ws_send(odata, dby);
}
