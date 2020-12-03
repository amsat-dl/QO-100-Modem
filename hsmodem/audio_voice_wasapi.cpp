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
* audio.c ... very similar to audio.c but is used for Microphone / Loudspeaker
*
*/

#include "hsmodem.h"

void cap_write_fifo(float sample);
int pb_read_fifo(float* data, int elements);
void close_wasapi_voice();
DWORD CALLBACK PBcallback_wasapi_voice(void* buffer, DWORD length, void* user);
DWORD CALLBACK CAPcallback_wasapi_voice(void* buffer, DWORD length, void* user);
void cap_write_fifo_voice(float sample);
int pb_read_fifo_voice(float* data, int elements);

#define WASAPI_CHANNELS_VOICE 2

float minPBvol_voice = 0;
float maxPBvol_voice = 99;
float minCAPvol_voice = 0;
float maxCAPvol_voice = 99;

extern int openpbdev_voice;
extern int opencapdev_voice;

extern float softwareCAPvolume_voice;

int mic_channel_num = 2;

#ifdef _WIN32_

int init_wasapi_voice(int pbdev, int capdev)
{
    int ret = 0;

	close_wasapi_voice();

    if (VoiceAudioMode == VOICEMODE_OFF) return 0;    // Voice off

    // ======= init PLAYBACK device ========

// initialize default output device
    if (!BASS_WASAPI_Init(pbdev, VOICE_SAMPRATE, WASAPI_CHANNELS_VOICE, BASS_WASAPI_EXCLUSIVE, 0.1f, 0, PBcallback_wasapi_voice, NULL))
    {
        printf("Can't initialize wasapi voice output device: %d err:%d\n", pbdev, BASS_ErrorGetCode());
        ret = 1;
    }
    else
    {

        // read real device number since a -1 cannot be started 
        int device = BASS_WASAPI_GetDevice();
        if (device == -1)
        {
            printf("BASS_WASAPI_GetDevice_voice: %d err:%d\n", pbdev, BASS_ErrorGetCode());
            ret = 1;
        }
        else
        {
            pbdev = device;

            // read the possible volume settings
            BASS_WASAPI_INFO info;
            if (!BASS_WASAPI_GetInfo(&info))
            {
                printf("BASS_WASAPI_GetInfo: %d err:%d\n", pbdev, BASS_ErrorGetCode());
                ret = 1;
            }
            else
            {
                minPBvol_voice = info.volmin;
                maxPBvol_voice = info.volmax;

                // start playback
                if (!BASS_WASAPI_Start())
                {
                    printf("BASS_WASAPI_Start voice: %d err:%d\n", pbdev, BASS_ErrorGetCode());
                    ret = 1;
                }
                else
                    openpbdev_voice = pbdev;
            }
        }
    }

    // ======= init CAPTURE device ========

    // initalize default recording device
    if (capdev == -1) capdev = -2;  // cap: -2 is the default device for input

    BOOL micret = false;
    mic_channel_num = 2;
    micret = BASS_WASAPI_Init(capdev, VOICE_SAMPRATE, mic_channel_num, BASS_WASAPI_EXCLUSIVE, 0.1f, 0, CAPcallback_wasapi_voice, NULL);
    if (!micret)
    {
        micret = BASS_WASAPI_Init(capdev, VOICE_SAMPRATE, 1, BASS_WASAPI_EXCLUSIVE, 0.1f, 0, CAPcallback_wasapi_voice, NULL);
        if (!micret)
        {
            printf("Can't initialize wasapi voice recording device: %d err:%d\n", capdev, BASS_ErrorGetCode());
            ret |= 2;
        }
        else
        {
            mic_channel_num = 1;
        }
    }

    if(micret)
    {
        printf("mic opened with %d channels\n", mic_channel_num);
        // read real device number since a -2 cannot be started
        int device = BASS_WASAPI_GetDevice();
        if (device == -1)
        {
            printf("BASS_WASAPI_GetDevice: voice: %d err:%d\n", capdev, BASS_ErrorGetCode());
            ret |= 2;
        }
        else
        {
            capdev = device;

            // read the possible volume settings
            BASS_WASAPI_INFO info;
            if (!BASS_WASAPI_GetInfo(&info))
            {
                printf("BASS_WASAPI_GetInfo: %d err:%d\n", pbdev, BASS_ErrorGetCode());
                ret |= 2;
            }
            else
            {
                minCAPvol_voice = info.volmin;
                maxCAPvol_voice = info.volmax;

                // start recording
                if (!BASS_WASAPI_Start())
                {
                    printf("BASS_WASAPI_Start voice: %d err:%d\n", capdev, BASS_ErrorGetCode());
                    ret |= 2;
                }
                else
                    opencapdev_voice = capdev;
            }
        }
    }

    if (ret == 0)
        printf("wasapi voice started successfully for PBdev:%d and CAPdev:%d\n", openpbdev_voice, opencapdev_voice);
    else
    {
        opencapdev_voice = -1;
        openpbdev_voice = -1;
        readAudioDevices();
    }
    if (ret == 1)
        printf("wasapi voice initialized: PBerror CapOK\n");
    if (ret == 2)
        printf("wasapi voice initialized: PBOK CapERROR\n");
    if (ret == 3)
        printf("wasapi voice initialized: PBerror CapERROR\n");

    return ret;
}

int selectPBdevice_wasapi_voice()
{
    if (!BASS_WASAPI_SetDevice(openpbdev_voice))
    {
        printf("BASS_WASAPI_SetDevice VOICE: %d err:%d\n", openpbdev_voice, BASS_ErrorGetCode());
        return 0;
    }
    return 1;
}

int selectCAPdevice_wasapi_voice()
{
    if (!BASS_WASAPI_SetDevice(opencapdev_voice))
    {
        printf("BASS_WASAPI_SetDevice VOICE: %d err:%d\n", opencapdev_voice, BASS_ErrorGetCode());
        return 0;
    }
    return 1;
}

void close_wasapi_voice()
{
    printf("close WASAPI Voice Devices\n");

    if (openpbdev_voice != -1)
    {
        if(selectPBdevice_wasapi_voice())
            if (!BASS_WASAPI_Free()) printf("BASS_WASAPI_Free voice: dev:%d err:%d\n", openpbdev_voice, BASS_ErrorGetCode());
    }

    if (opencapdev_voice != -1)
    {
        if(selectCAPdevice_wasapi_voice())
            if (!BASS_WASAPI_Free()) printf("BASS_WASAPI_Free voice: dev:%d err:%d\n", opencapdev_voice, BASS_ErrorGetCode());
    }
    printf("closed WASAPI Voice Devices\n");
}

void setLSvolume(int v)
{
    // the volume comes in % 0..99
    // map to min/maxPBvol
    float vf = v * (maxPBvol_voice - minPBvol_voice) / 100 + minPBvol_voice;

    if (vf < minPBvol_voice) vf = minPBvol_voice;
    if (vf > maxPBvol_voice) vf = maxPBvol_voice;

    //printf("set PB volume to:%d / %f [%f..%f]\n", v, vf, minPBvol_voice, maxPBvol_voice);

    if(selectPBdevice_wasapi_voice())
        if (!BASS_WASAPI_SetVolume(BASS_WASAPI_CURVE_DB, vf))
            printf("setPBvolume: %d err:%d\n", openpbdev_voice, BASS_ErrorGetCode());
}

void setMICvolume(int v)
{
    // non of the BASS input level functions are working in WASAPI exclusive mode
    // so we adjust the input level by software
    softwareCAPvolume_voice = (float)v;
    softwareCAPvolume_voice /= 50;
}

DWORD CALLBACK PBcallback_wasapi_voice(void* buffer, DWORD length, void* user)
{
    float* fbuffer = (float*)buffer;
    int ret = pb_read_fifo_voice(fbuffer, length / sizeof(float));
    if (ret == 0)
    {
        // fifo empty, send 00
        memset(buffer, 0, length);
    }
    return length;
}

DWORD CALLBACK CAPcallback_wasapi_voice(void* buffer, DWORD length, void* user)
{
    float* fbuffer = (float*)buffer;
    for (unsigned int i = 0; i < (length / sizeof(float)); i += mic_channel_num)
    {
        cap_write_fifo_voice(fbuffer[i]);
    }

    return TRUE; // continue recording
}

#endif
