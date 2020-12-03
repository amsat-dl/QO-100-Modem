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
* audio_wasabi.c ... functions to handle audio in/out via a soundcard uses the "BASSWASAPI" library
* wasapi is needed because we need exclusive access to the sound card which is not provided for Windows with the normal bass.lib
*/

#include "hsmodem.h"

#ifdef _WIN32_

#define WASAPI_CHANNELS 2   // wasapi works with 2 only

void cap_write_fifo(float sample);
int pb_read_fifo(float* data, int elements);
void close_wasapi();
DWORD CALLBACK PBcallback_wasapi(void* buffer, DWORD length, void* user);
DWORD CALLBACK CAPcallback_wasapi(void* buffer, DWORD length, void* user);

float minPBvol = 0;
float maxPBvol = 99;
float minCAPvol = 0;
float maxCAPvol = 99;

extern int openpbdev;
extern int opencapdev;

int init_wasapi(int pbdev, int capdev)
{
    int ret = 0;

    close_wasapi();

    // ======= init PLAYBACK device ========

    // initialize default output device
    if (!BASS_WASAPI_Init(pbdev, caprate, WASAPI_CHANNELS, BASS_WASAPI_EXCLUSIVE, 0.1f, 0, PBcallback_wasapi, NULL))
    {
        printf("Can't initialize output device: %d err:%d\n", pbdev, BASS_ErrorGetCode());
        ret = 1;
    }
    else
    {

        // read real device number since a -1 cannot be started
        int device = BASS_WASAPI_GetDevice();
        if (device == -1)
        {
            printf("BASS_WASAPI_GetDevice: %d err:%d\n", pbdev, BASS_ErrorGetCode());
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
                minPBvol = info.volmin;
                maxPBvol = info.volmax;

                // start playback
                if (!BASS_WASAPI_Start())
                {
                    printf("BASS_WASAPI_Start: %d err:%d\n", pbdev, BASS_ErrorGetCode());
                    ret = 1;
                }
                else
                    openpbdev = pbdev;
            }
        }
    }

    // ======= init CAPTURE device ========

    // initalize default recording device
    if (capdev == -1) capdev = -2;  // cap: -2 is the default device for input
    if (!BASS_WASAPI_Init(capdev, caprate, WASAPI_CHANNELS, BASS_WASAPI_EXCLUSIVE, 0.1f, 0, CAPcallback_wasapi, NULL))
    {
        printf("Can't initialize recording device: %d err:%d\n", capdev, BASS_ErrorGetCode());
        ret |= 2;
    }
    else
    {

        // read real device number since a -2 cannot be started
        int device = BASS_WASAPI_GetDevice();
        if (device == -1)
        {
            printf("BASS_WASAPI_GetDevice: %d err:%d\n", capdev, BASS_ErrorGetCode());
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
                minCAPvol = info.volmin;
                maxCAPvol = info.volmax;

                // start recording
                if (!BASS_WASAPI_Start())
                {
                    printf("BASS_WASAPI_Start: %d err:%d\n", capdev, BASS_ErrorGetCode());
                    ret |= 2;
                }
                else
                    opencapdev = capdev;
            }
        }
    }

    if (ret == 0)
        printf("WASAPI started successfully for PBdev:%d and CAPdev:%d\n", openpbdev, opencapdev);
    else
    {
        opencapdev = -1;
        openpbdev = -1;
        readAudioDevices();
    }
    if (ret == 1)
        printf("wasapi audio initialized: PBerror CapOK\n");
    if (ret == 2)
        printf("wasapi audio initialized: PBOK CapERROR\n");
    if (ret == 3)
        printf("wasapi audio initialized: PBerror CapERROR\n");

    return ret;
}

int selectPBdevice_wasapi()
{
    if (!BASS_WASAPI_SetDevice(openpbdev))
    {
        printf("BASS_WASAPI_SetDevice: %d err:%d\n", openpbdev, BASS_ErrorGetCode());
        return 0;
    }
    return 1;
}

int selectCAPdevice_wasapi()
{
    if (!BASS_WASAPI_SetDevice(opencapdev))
    {
        printf("BASS_WASAPI_SetDevice: %d err:%d\n", opencapdev, BASS_ErrorGetCode());
        return 0;
    }
    return 1;
}

void setPBvolume(int v)
{
    // the volume comes in % 0..99
    // map to min/maxPBvol
    float vf = v * (maxPBvol - minPBvol) / 100 + minPBvol;

    if (vf < minPBvol) vf = minPBvol;
    if (vf > maxPBvol) vf = maxPBvol;

    //printf("set PB volume to:%d / %f [%f..%f]\n", v, vf, minPBvol, maxPBvol);

    if(selectPBdevice_wasapi())
        if (!BASS_WASAPI_SetVolume(BASS_WASAPI_CURVE_DB, vf))
            printf("setPBvolume: %d err:%d\n", openpbdev, BASS_ErrorGetCode());
}

void setCAPvolume(int v)
{
    // non of the BASS input level functions are working in WASAPI exclusive mode
    // so we adjust the input level by software
    softwareCAPvolume = (float)v;
    softwareCAPvolume /= 50;
}

void close_wasapi()
{
    printf("close WASAPI Audio Devices\n");

    if (openpbdev != -1)
    {
        if(selectPBdevice_wasapi())
            if (!BASS_WASAPI_Free()) printf("BASS_WASAPI_Free: dev:%d err:%d\n", openpbdev, BASS_ErrorGetCode());
    }

    if (opencapdev != -1)
    {
        if(selectCAPdevice_wasapi())
            if (!BASS_WASAPI_Free()) printf("BASS_WASAPI_Free: dev:%d err:%d\n", opencapdev, BASS_ErrorGetCode());
    }
}

DWORD CALLBACK PBcallback_wasapi(void* buffer, DWORD length, void* user)
{
    float* fbuffer = (float*)buffer;

    // requested number of stereo samples: length/sizeof(float)
    // requested real number of samples
    int req_samples = length / sizeof(float) / WASAPI_CHANNELS;
    // prepare a buffer to store the mono samples from the fifo
    float* fdata = (float*)malloc(sizeof(float) * req_samples);
    // read mono samples from fifo
    int ret = pb_read_fifo(fdata, req_samples);
    if (ret == 0)
    {
        // fifo empty, send 00
        memset(fdata, 0, sizeof(float) * req_samples);
    }
    // copy the mono samples into the stereo output buffer
    int didx = 0;
    for (int i = 0; i < req_samples; i++)
    {
        fbuffer[didx++] = fdata[i];
        fbuffer[didx++] = fdata[i];
    }

    free(fdata);
    return length;
}

DWORD CALLBACK CAPcallback_wasapi(void* buffer, DWORD length, void* user)
{
    //printf("CAP callback, len:%d\n",length);
    //measure_speed_bps(length/sizeof(float)/ WASAPI_CHANNELS);

    float* fbuffer = (float*)buffer;
    //showbytestringf((char*)"rx: ", fbuffer, 10);
    //printf("%10.6f\n", fbuffer[0]);
    for (unsigned int i = 0; i < (length / sizeof(float)); i += WASAPI_CHANNELS)
    {
        //nullChecker(fbuffer[i],fbuffer, length / sizeof(float));
        cap_write_fifo(fbuffer[i]);
    }

    return TRUE; // continue recording
}

#endif // _WIN32_
