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
* codec2.c ... function to handle voice transfer via codec2
*
*/

#include "hsmodem.h"

void sendCodecToModulator(uint8_t* pdata, int len);

struct CODEC2 *pc2 = NULL;
int samplesPerPacket = 160;
int bytesPerPacket = 8;
firdecim_crcf decim48_8 = NULL;
firinterp_crcf interp8_48;
const int decfactor = 6;

void init_codec2()
{
	close_codec2();

	// create pre-decimator
	decim48_8 = firdecim_crcf_create_kaiser(decfactor, 7, 40.0f);
	firdecim_crcf_set_scale(decim48_8, 1.0f / (float)decfactor);

	// create post-interpolator
	interp8_48 = firinterp_crcf_create_kaiser(decfactor, 7, 40.0f);

	switch (speedmode)
	{
	case 0: pc2 = codec2_create(CODEC2_MODE_700C); break;
	case 1: pc2 = codec2_create(CODEC2_MODE_1600); break;
	case 2: pc2 = codec2_create(CODEC2_MODE_2400); break;
	default: pc2 = codec2_create(CODEC2_MODE_3200); break;
	}

	codec2_set_natural_or_gray(pc2, 0);
	bytesPerPacket = codec2_bits_per_frame(pc2) / 8;
	samplesPerPacket = codec2_samples_per_frame(pc2);
	printf("Codec2: BytesPerFrame:%d  SamplesPerFrame:%d\n", bytesPerPacket, samplesPerPacket);
}

void close_codec2()
{
	if (pc2 != NULL)
		codec2_destroy(pc2);
	pc2 = NULL;

	if (decim48_8 != NULL)
		firdecim_crcf_destroy(decim48_8);
	decim48_8 = NULL;

	if (interp8_48 != NULL)
		firinterp_crcf_destroy(interp8_48);
	interp8_48 = NULL;
}

// encode 160 voice samples (8kS/s) into 64 bits output
void encode_codec2(float f)
{
	static int decim = 0;
	static int16_t sbuf[500];	// this is easily more than "samplesPerPacket" in any cases
	static int fbuf_idx = 0;
	uint8_t outbuf[50];			// this is easily more than "bytesPerPacket" in any cases
	static liquid_float_complex fdec[decfactor];

	if (pc2 == NULL || decim48_8 == NULL) return;

	// this encoder is called with a sound card sample rate of 48000
	// codec2 needs 8 kS/s, so we have to decimate by 6
	fdec[decim].real = f;
	fdec[decim].imag = 0;
	if (++decim >= 6)
	{
		decim = 0;

		liquid_float_complex y;
		firdecim_crcf_execute(decim48_8, fdec, &y);

		// here we have a sample rate of 8 kS/s
		// one encoding call needs 160 samples
		sbuf[fbuf_idx] = (int16_t)(y.real * 32768);	// convert to short
		if (++fbuf_idx >= samplesPerPacket)
		{
			fbuf_idx = 0;

			// we have 160 samples in fbuf, encode them now
			codec2_encode(pc2, outbuf, sbuf);

			// outbuf is 64bit = 8byte long

			// send Codec data to modulator
			if (VoiceAudioMode == VOICEMODE_DV_FULLDUPLEX)
			{
				memmove(outbuf + 1, outbuf, bytesPerPacket);
				outbuf[0] = 0xff; // start of codec2 packet marker
				sendCodecToModulator(outbuf, bytesPerPacket+1);
			}

			if (VoiceAudioMode == VOICEMODE_CODECLOOP)
			{
				// codec loop mode: decode and play it
				int16_t spbbuf[500];
				codec2_decode(pc2, spbbuf, outbuf);

				for (int i = 0; i < samplesPerPacket; i++)
				{
					float f = (float)spbbuf[i];
					f /= 32768;
					// here we have 8kS/s, need to interpolate to 48 kS/s
					liquid_float_complex inp;
					inp.real = f;
					inp.imag = 0;
					liquid_float_complex outp[decfactor];

					firinterp_crcf_execute(interp8_48, inp, outp);

					for (int x = 0; x < decfactor; x++)
						io_ls_write_fifo(outp[x].real);
				}
			}
		}
	}
}

#define CHUNKSIZE_VOICE	40

void toCodecDecoder_codec2(uint8_t* pdata, int len)
{
	static uint8_t chunk[50];

	// go through all data bytes
	for (int vd = 0; vd < len; vd++)
	{
		// shift the data through the chunk buffer
		for (int i = 0; i < CHUNKSIZE_VOICE - 1; i++)
			chunk[i] = chunk[i + 1];
		chunk[CHUNKSIZE_VOICE - 1] = pdata[vd];

		// an Codec-2 packet has max length of max 8 Byte 
		// in the chunk size of 40 fit minimum 4 chunks
		// so lets test if 4 chunks are there, by looking for the marker
		// at distance bytesPerPacket
		int mfound = 1;
		for (int m = 0; m < 4; m++)
		{
			if (chunk[(bytesPerPacket + 1) * m] != 0xff)
			{
				mfound = 0;
				break;
			}
		}

		if (mfound)
		{
			// codec loop mode: decode and play it
			int16_t spbbuf[500];
			//showbytestring("CODEC2:", chunk + 1, 3, 3);
			codec2_decode(pc2, spbbuf, chunk+1);

			for (int i = 0; i < samplesPerPacket; i++)
			{
				float f = (float)spbbuf[i];
				f /= 32768;
				// here we have 8kS/s, need to interpolate to 48 kS/s
				liquid_float_complex inp;
				inp.real = f;
				inp.imag = 0;
				liquid_float_complex outp[decfactor];

				firinterp_crcf_execute(interp8_48, inp, outp);

				for (int x = 0; x < decfactor; x++)
					io_ls_write_fifo(outp[x].real);
			}
		}
	}
}