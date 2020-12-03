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

void init_pipes_voice();
BOOL CALLBACK RecordingCallback_voice(HRECORD handle, const void* buffer, DWORD length, void* user);
DWORD CALLBACK WriteStream_voice(HSTREAM handle, float* buffer, DWORD length, void* user);
//void CALLBACK EncodeProc(HENCODE handle, DWORD channel, const void* buffer, DWORD length, void* user);
void cap_write_fifo_voice(float sample);
int pb_read_fifo_voice(float* data, int elements);
void setLSvolume(int v);
void setMICvolume(int v);

extern AUDIODEVS audioPBdevs[100];
extern AUDIODEVS audioCAPdevs[100];
extern int pbanz;
extern int capanz;


HRECORD rchan_voice = 0;		// recording channel
BASS_INFO info_voice;
HSTREAM stream_voice = 0;

int openpbdev_voice = -1;
int opencapdev_voice = -1;
int caprate_voice = VOICE_SAMPRATE;

int initialLSvol = -1;
int initialMICvol = -1;

float softwareCAPvolume_voice = 1;

// pbdev, capdev: -1=default device
int init_audio_voice(int setpbdev, int setcapdev)
{
    static int f = 1;
    int ret = 0;

    if (f == 1)
    {
        // do only once after program start
        f = 0;
        init_pipes_voice();
    }

    // translate requested device numbers to bass device numbers
    if (setpbdev < 0 || setpbdev >= pbanz) setpbdev = 0;
    if (setcapdev < 0 || setcapdev >= capanz) setcapdev = 0;

    int pbdev = -1;
    if (setpbdev >= 0 && setpbdev < pbanz) pbdev = audioPBdevs[setpbdev].bassdev;
    int capdev = -2;
    if (setcapdev >= 0 && setcapdev < capanz) capdev = audioCAPdevs[setcapdev].bassdev;

    printf("voice: init audio_voice, caprate:%d\n", caprate_voice);
    printf("voice: requested LS   device: %d bassno:%d name:%s\n", setpbdev, pbdev, audioPBdevs[setpbdev].name);
    printf("voice: requested MIC device: %d bassno:%d name:%s\n", setcapdev, capdev, audioCAPdevs[setcapdev].name);

#ifdef _WIN32_
    // use WASAPI for Windows to get exclusive access to sound card
    return init_wasapi_voice(pbdev, capdev);
#endif

#ifdef _LINUX_
    close_audio_voice();

    if (VoiceAudioMode == 0) return 0;    // Voice off

    // ===== PLAYBACK ======

    // initialize default output device
    if (!BASS_Init(pbdev, caprate_voice, 0, NULL, NULL))
    {
        printf("voice: Can't initialize output device: %d err:%d\n", pbdev, BASS_ErrorGetCode());
        close_audio_voice();
        ret = 1;
    }
    else
    {

        // read real device number
        int device = BASS_GetDevice();
        if (device == -1)
        {
            printf("voice: BASS_GetDevice: %d err:%d\n", pbdev, BASS_ErrorGetCode());
            close_audio_voice();
            ret = 1;
        }
        else
        {
            pbdev = device;
            openpbdev_voice = pbdev;
            printf("voice: real BASS PB Device No: %d\n", pbdev);

            // set play callback
            BASS_GetInfo(&info_voice);
            stream_voice = BASS_StreamCreate(info_voice.freq, 2, BASS_SAMPLE_FLOAT, (STREAMPROC*)WriteStream_voice, 0); // sample: 32 bit float
            BASS_ChannelSetAttribute(stream_voice, BASS_ATTRIB_BUFFER, 0); // no buffering for minimum latency
            BASS_ChannelPlay(stream_voice, FALSE); // start it

            setLSvolume(initialLSvol);
        }
    }

    // ===== CAPTURE ====

    // initalize default recording device
    if (!BASS_RecordInit(capdev))
    {
        printf("voice: Can't initialize recording device: %d err:%d\n", capdev, BASS_ErrorGetCode());
        close_audio_voice();
        ret |= 2;
    }
    else
    {

        // read real device number
        int device = BASS_RecordGetDevice();
        if (device == -1)
        {
            printf("voice: BASS_GetDevice: %d err:%d\n", capdev, BASS_ErrorGetCode());
            close_audio_voice();
            ret |= 2;
        }
        else
        {
            capdev = device;
            printf("voice: real BASS CAP Device No: %d\n", capdev);

            // set capture callback
            rchan_voice = BASS_RecordStart(caprate_voice, 2, BASS_SAMPLE_FLOAT, RecordingCallback_voice, 0);
            if (!rchan_voice) {
                printf("voice: Can't start capturing: %d\n", BASS_ErrorGetCode());
                close_audio_voice();
                ret |= 2;
            }
            else
            {
                opencapdev_voice = capdev;
                setMICvolume(initialMICvol);
            }
        }
    }

    if (ret == 0)
        printf("voice started successfully for PBdev:%d and CAPdev:%d\n", openpbdev_voice, opencapdev_voice);
    else
    {
        opencapdev_voice = -1;
        openpbdev_voice = -1;
        readAudioDevices();
    }
    if (ret == 1)
        printf("voice initialized: PBerror CapOK\n");
    if (ret == 2)
        printf("voice initialized: PBOK CapERROR\n");
    if (ret == 3)
        printf("voice initialized: PBerror CapERROR\n");

#endif

    return ret;
}

#ifdef _LINUX_
void selectPBdevice_voice()
{
    if (!BASS_SetDevice(openpbdev_voice))
        printf("BASS_SetDevice: %d err:%d\n", openpbdev_voice, BASS_ErrorGetCode());
}

void selectCAPdevice_voice()
{
    if (!BASS_SetDevice(opencapdev_voice))
        printf("BASS_SetDevice: %d err:%d\n", opencapdev_voice, BASS_ErrorGetCode());
}

void close_audio_voice()
{
    printf("voice: close Audio Devices\n");
    if (stream_voice > 0)
        BASS_StreamFree(stream_voice);

    if (openpbdev_voice != -1)
    {
        selectPBdevice_voice();
        int r = BASS_Free();
        if (!r) printf("voice: Bass_Free error: %d\n", BASS_ErrorGetCode());
    }
    
    if (rchan_voice > 0)
        BASS_ChannelStop(rchan_voice);

    if (opencapdev_voice != -1)
    {
        selectCAPdevice_voice();
        int rr = BASS_RecordFree();
        if (!rr) printf("voice: Bass_RecordFree error: %d\n", BASS_ErrorGetCode());
    }

    openpbdev_voice = -1;
    opencapdev_voice = -1;
    rchan_voice = 0;
    stream_voice = 0;
}

void setLSvolume(int v)
{
    if (v < 0 || v>100) return;

    // the volume comes in % 0..99
    // map to 0..1
    float vf = v;
    vf /= 100;

    //printf("set PB volume to:%d / %f [0..1]\n", v, vf );

    selectPBdevice_voice();
    if (!BASS_SetVolume(vf))
        printf("setPBvolume: %d err:%d\n", openpbdev_voice, BASS_ErrorGetCode());
}

void setMICvolume(int v)
{
    if (v < 0 || v>100) return;

    // the volume comes in % 0..99
    // map to min/maxPBvol
    float vf = v;
    vf /= 100;

    //printf("set CAP volume to:%d / %f [0..1]\n", v, vf);

    selectCAPdevice_voice();
    if (!BASS_RecordSetInput(-1, BASS_INPUT_ON, vf))
    {
        printf("setCAPvolume: %d err:%d, using software level\n", opencapdev_voice, BASS_ErrorGetCode());
        softwareCAPvolume_voice = ((float)v / 100);
    }
}


// capture callback
BOOL CALLBACK RecordingCallback_voice(HRECORD handle, const void* buffer, DWORD length, void* user)
{
    //printf("captured %ld samples, channels:%d\n",length/sizeof(float),2);
    //measure_speed(length/sizeof(float));

    float* fbuffer = (float*)buffer;
    //showbytestringf((char*)"cap:", fbuffer, 10);
    //printf("w:%ld ",length/sizeof(float));
    for (unsigned int i = 0; i < (length / sizeof(float)); i += 2)
    {
        //printf("%f\n",fbuffer[i]);
        cap_write_fifo_voice(fbuffer[i]);
    }

    return TRUE; // continue recording
}

// play callback
// length: bytes. float=4byte, 2channels, so it requests samples*8
DWORD CALLBACK WriteStream_voice(HSTREAM handle, float* buffer, DWORD length, void* user)
{
    //printf("requested %ld samples\n", length / sizeof(float));
    int ret = pb_read_fifo_voice(buffer, length / sizeof(float));
    if (ret == 0)
    {
        // fifo empty, send 00
        memset(buffer, 0, length);
    }
    return length;
}

#endif

// set volume 
void setVolume_voice(int pbcap, int v)
{
    if (pbcap == 0) setLSvolume(v);
    else setMICvolume(v);
}

// ================= resampling and other tasks for voice audio =================

// samples come from the data-audio capture with a speed of caprate
// resample (if required) to VOICE_SAMPRATE, which is the voice-audio rate
void toVoice(float sample)
{
    if (caprate == VOICE_SAMPRATE)
    {
        // resampling not required, just put in LS fifo
        pb_write_fifo_voice(sample);
    }
    else
    {
        pb_write_fifo_voice(sample);
        // samprate of incoming signal is 44100, voice needs 48000
        // we have 44100 samples/s, so we ar missing 3900 S/s.
        // if we insert an additional sample every 11 samples
        // this results in a rate of 48109 S/s
        static int cnt = 0;
        if (++cnt >= 11)
        {
            cnt = 0;
            pb_write_fifo_voice(sample);
        }
    }
}

// ================= VOICE FIFOs ===================

#ifdef _WIN32_
CRITICAL_SECTION cap_crit_sec_voice;
CRITICAL_SECTION pb_crit_sec_voice;
#define CAP_LOCK_VOICE	EnterCriticalSection(&cap_crit_sec_voice)
#define PB_LOCK_VOICE	    EnterCriticalSection(&pb_crit_sec_voice)
void CAP_UNLOCK_VOICE()
{
    if (&cap_crit_sec_voice != NULL)
        LeaveCriticalSection(&cap_crit_sec_voice);
}
void PB_UNLOCK_VOICE()
{
    if (&pb_crit_sec_voice != NULL)
        LeaveCriticalSection(&pb_crit_sec_voice);
}
#endif

#ifdef _LINUX_
pthread_mutex_t cap_crit_sec_voice;
pthread_mutex_t pb_crit_sec_voice;
#define CAP_LOCK_VOICE	pthread_mutex_lock(&cap_crit_sec_voice)
void CAP_UNLOCK_VOICE() { pthread_mutex_unlock(&cap_crit_sec_voice); }
#define PB_LOCK_VOICE	pthread_mutex_lock(&pb_crit_sec_voice)
void PB_UNLOCK_VOICE() { pthread_mutex_unlock(&pb_crit_sec_voice); }
#endif

#define AUDIO_PLAYBACK_BUFLEN_VOICE (48000) 
#define AUDIO_CAPTURE_BUFLEN_VOICE  (48000) 

int cap_wridx_voice = 0;
int cap_rdidx_voice = 0;
float cap_buffer_voice[AUDIO_CAPTURE_BUFLEN_VOICE];

int pb_wridx_voice = 0;
int pb_rdidx_voice = 0;
float pb_buffer_voice[AUDIO_PLAYBACK_BUFLEN_VOICE];

void init_pipes_voice()
{
#ifdef _WIN32_
    if (&cap_crit_sec_voice != NULL) DeleteCriticalSection(&cap_crit_sec_voice);
    InitializeCriticalSection(&cap_crit_sec_voice);

    if (&pb_crit_sec_voice != NULL) DeleteCriticalSection(&pb_crit_sec_voice);
    InitializeCriticalSection(&pb_crit_sec_voice);
#endif
}

// write one sample into the fifo
// overwrite old data if the fifo is full
void cap_write_fifo_voice(float sample)
{
    if (((cap_wridx_voice + 1) % AUDIO_CAPTURE_BUFLEN_VOICE) == cap_rdidx_voice)
    {
        //printf("cap_voice fifo full\n");
        CAP_UNLOCK_VOICE();
        return;
    }

    CAP_LOCK_VOICE;
    cap_buffer_voice[cap_wridx_voice] = sample;
    if (++cap_wridx_voice >= AUDIO_CAPTURE_BUFLEN_VOICE) cap_wridx_voice = 0;
    CAP_UNLOCK_VOICE();
}

int cap_read_fifo_voice(float* data)
{
    CAP_LOCK_VOICE;

    if (cap_rdidx_voice == cap_wridx_voice)
    {
        // Fifo empty, no data available
        CAP_UNLOCK_VOICE();
        return 0;
    }

    *data = cap_buffer_voice[cap_rdidx_voice];
    if (++cap_rdidx_voice >= AUDIO_CAPTURE_BUFLEN_VOICE) cap_rdidx_voice = 0;
    CAP_UNLOCK_VOICE();

    return 1;
}

void cap_write_fifo_clear_voice()
{
    cap_wridx_voice = cap_rdidx_voice = 0;
}


void pb_write_fifo_clear_voice()
{
    pb_wridx_voice = pb_rdidx_voice = 0;
}

int pb_fifo_freespace_voice(int nolock)
{
    int freebuf = 0;

    if (nolock == 0) PB_LOCK_VOICE;

    int elemInFifo = (pb_wridx_voice + AUDIO_PLAYBACK_BUFLEN_VOICE - pb_rdidx_voice) % AUDIO_PLAYBACK_BUFLEN_VOICE;
    freebuf = AUDIO_PLAYBACK_BUFLEN_VOICE - elemInFifo;

    if (nolock == 0) PB_UNLOCK_VOICE();

    return freebuf;
}

void pb_write_fifo_voice(float sample)
{
    PB_LOCK_VOICE;

    // check if there is free space in fifo
    if (pb_fifo_freespace_voice(1) == 0)
    {
        PB_UNLOCK_VOICE();
        //printf("************* pb fifo_voice full\n");
        return;
    }

    pb_buffer_voice[pb_wridx_voice] = sample;
    if (++pb_wridx_voice >= AUDIO_PLAYBACK_BUFLEN_VOICE) pb_wridx_voice = 0;
    PB_UNLOCK_VOICE();
}

int pb_fifo_usedspace_voice()
{
    int anz = pb_fifo_freespace_voice(0);
    return AUDIO_PLAYBACK_BUFLEN_VOICE - anz;
}

// read elements floats from fifo or return 0 if not enough floats are available
int pb_read_fifo_voice(float* data, int elements)
{
    //printf("pb read fifo_voice: %d\n",elements);
    PB_LOCK_VOICE;

    int e = AUDIO_PLAYBACK_BUFLEN_VOICE - pb_fifo_freespace_voice(1);
    if (e < elements)
    {
        // Fifo empty, no data available
        PB_UNLOCK_VOICE();
        //printf("pb fifo empty, need:%d have:%d size:%d\n",elements,e,AUDIO_PLAYBACK_BUFLEN);
        return 0;
    }

    for (int i = 0; i < elements; i+=2)
    {
        // channel1 and the same for channel 2
        data[i] = pb_buffer_voice[pb_rdidx_voice];
        data[i+1] = pb_buffer_voice[pb_rdidx_voice];
        if (++pb_rdidx_voice >= AUDIO_PLAYBACK_BUFLEN_VOICE) pb_rdidx_voice = 0;
    }
    //printf("read %d floats\n",elements);

    PB_UNLOCK_VOICE();
    return 1;
}

void clear_voice_fifos()
{
    pb_write_fifo_clear_voice();
    cap_write_fifo_clear_voice();
}