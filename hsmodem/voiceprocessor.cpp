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
* voiceprocessor.c ... function to handle voice transfer
*
*/

#include "hsmodem.h"

int opusbitrate = 0;

void sendCodecToModulator(uint8_t* pdata, int len);

OpusEncoder* opusenc = (OpusEncoder*)NULL;
OpusDecoder* opusdec = (OpusDecoder*)NULL;

void init_voiceproc()
{
    if (codec == 1)
    {
        init_codec2();
        return;
    }

    if (opusbitrate == 0)
    {
        printf("Codec bitrate not set\n");
        return;
    }

	close_voiceproc();
    if (VoiceAudioMode == VOICEMODE_CODECLOOP || VoiceAudioMode == VOICEMODE_DV_FULLDUPLEX || VoiceAudioMode == VOICEMODE_DV_RXONLY)
    {
        int err;
        opusenc = opus_encoder_create(VOICE_SAMPRATE, 1, OPUS_APPLICATION_VOIP, &err);
        if (opusenc == (OpusEncoder*)NULL || err != OPUS_OK)
        {
            printf("opus_encoder_create failed\n");
            return;
        }
        opus_encoder_ctl(opusenc, OPUS_SET_BITRATE(opusbitrate));
        printf("set opus rate: %d\n", opusbitrate);
        opus_encoder_ctl(opusenc, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
        opus_encoder_ctl(opusenc, OPUS_SET_VBR(0)); // hard-CBR
        opus_encoder_ctl(opusenc, OPUS_SET_MAX_BANDWIDTH(OPUS_BANDWIDTH_NARROWBAND));

        opusdec = opus_decoder_create(VOICE_SAMPRATE, 1 , &err);
        if (opusdec == (OpusDecoder*)NULL || err != OPUS_OK)
        {
            printf("opus_encoder_create failed\n");
            return;
        }
    }
}

void close_voiceproc()
{
    if(opusenc != (OpusEncoder*)NULL)
        opus_encoder_destroy(opusenc);
    opusenc = (OpusEncoder*)NULL;

    if (opusdec != (OpusDecoder*)NULL)
        opus_decoder_destroy(opusdec);
    opusdec = (OpusDecoder*)NULL;
}

#define ENC_FRAMESIZE 2880
void encode(float f)
{
    if (codec == 1)
    {
        encode_codec2(f);
        return;
    }

    if (opusenc == (OpusEncoder*)NULL) return;

    static float farr[ENC_FRAMESIZE];
    static int farridx = 0;
    uint8_t opusdata[10000];

    // collect samples until we have ENC_FRAMESIZE
    farr[farridx] = f;
    if (++farridx == ENC_FRAMESIZE)
    {
        opus_int32 ret = opus_encode_float(opusenc, farr, farridx, opusdata, opusbitrate);
        if (ret < 0)
        {
            printf("opus_encode_float error: %d", ret);
        }

        // length of an OPUS packet:
        // duration[ms] * OutSampRate[kbps] / 8
        // line speed 7200: sample rate=6, PacketSize = (ENC_FRAMESIZE*1000/48000) * 6 / 8 = (ENC_FRAMESIZE/48)*6/8 = 45

        //measure_speed_bps(ret*8);
        //showbytestring("ENC:", opusdata, ret, ret);

        // send Codec data to modulator
        if (VoiceAudioMode == VOICEMODE_DV_FULLDUPLEX)
        {
            memmove(opusdata + 1, opusdata, ret);
            opusdata[0] = 0xff; // start of opus packet marker
            sendCodecToModulator(opusdata, ret+1);
        }

        // decode and send samples to loadspeaker
        if (VoiceAudioMode == VOICEMODE_CODECLOOP)
        {
            static float fresult[ENC_FRAMESIZE];
            int r = opus_decode_float(opusdec, opusdata, ret, fresult, ENC_FRAMESIZE, 0);
            if (r < 0)
            {
                printf("opus_decode_float error: %d", r);
            }

            for (int i = 0; i < r; i++)
               io_ls_write_fifo(fresult[i]);
        }

        farridx = 0;
    }
}

// void toGR_sendData(uint8_t* data, int type, int status)
// this function expects "data" with length: payloadlen
// so we have to collect samples until one payload is filled
// this may take too long, in this case send the frame immediately
// the first byte in the payload is the marker 0xff
// type=6 ... voice payload
// minfo=1 ... voice data available
// minfo=0 ... filler payload, just ignore

void sendCodecToModulator(uint8_t *pdata, int len)
{
    static uint8_t payload[PAYLOADLEN];
    static int vdidx = 0;

    // fill the new voice data "pdata" into the buffer "voicedata"
    for (int i = 0; i < len; i++)
    {
        if (vdidx >= PAYLOADLEN) printf("vdidx too high: %d", vdidx);
        payload[vdidx++] = pdata[i];

        // if the voicedata buffer is full, send it to the modem
        if (vdidx == PAYLOADLEN)
        {
            vdidx = 0;
            toGR_sendData(payload, 6, 1 ,0);   // 6 ... voice data, 1 ... valid voice data
        }

        while (keeprunning)
        {
            // we have to check if the TX fifo has enough data. In case of an underrun the Q(8A)PSK signal will be distorted
            int us = io_pb_fifo_usedspace();
            if (us < 20000)
            {
                //printf("tx filler\n");
                // not enough samples in the TX buffer
                // send a dummy frame, a frame with 0 voice data
                uint8_t dummy[PAYLOADLEN];
                memset(dummy, 0, PAYLOADLEN);
                toGR_sendData(dummy, 6, 0, 0);
            }
            else
                break;
        }
    }
}

// we get the received data stream with length: PAYLOADLEN
// find opus packets marked with 0xff and a length of opusPacketSize
// send send these chunks to the codec_decoder
#define CHUNKSIZE 200

void toCodecDecoder(uint8_t *pdata, int len)
{
    if (codec == 1)
    {
        toCodecDecoder_codec2(pdata, len);
        return;
    }

    static uint8_t chunk[CHUNKSIZE];
    int opusPacketSize = ((ENC_FRAMESIZE / 48) * (opusbitrate/100)) / 80;

    if (opusPacketSize > 45)
    {
        printf("wrong opusPacketSize: %d\n", opusPacketSize);
        return;
    }

    // go through all data bytes
    for (int vd = 0; vd < len; vd++)
    {
        // shift the data through the chunk buffer
        for (int i = 0; i < CHUNKSIZE - 1; i++)
            chunk[i] = chunk[i + 1];
        chunk[CHUNKSIZE - 1] = pdata[vd];

        // an OPUS packet has max length of 45 Byte. 
        // in the chunk size of 200 fit minimum 4 chunks
        // so lets test if 4 chunks are there, by looking for the marker
        // at distance opusPacketSize
        int mfound = 1;
        for (int m = 0; m < 4; m++)
        {
            if (chunk[(opusPacketSize+1) * m] != 0xff)
            {
                mfound = 0;
                break;
            }
        }

        if (mfound)
        {
            //showbytestring("OPUS:", chunk + 1, opusPacketSize, opusPacketSize);

            static float fresult[ENC_FRAMESIZE];
            int r = opus_decode_float(opusdec, chunk + 1, opusPacketSize, fresult, ENC_FRAMESIZE, 0);
            if (r < 0)
            {
                printf("opus_decode_float error: %d\n", r);
            }
            else
            {
                //measure_speed_bps(r);
                for (int j = 0; j < r; j++)
                   io_ls_write_fifo(fresult[j]);
            }
        }
    }
}
