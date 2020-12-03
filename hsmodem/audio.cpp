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
* audio.c ... functions to handle audio in/out via a soundcard
*             uses the "BASS" library
* 
* captures samples from the sound card.
* Samples are 32-bit floats in a range of -1 to +1
* get these samples from the thread safe fifo: cap_read_fifo(&floatvariable)
* 
* plays samples to the sound card
* Samples are 32-bit floats in a range of -1 to +1
* play the samples by calling the thread save function: pb_write_fifo(floatsample)
* 
*/

#include "hsmodem.h"

BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user);
DWORD CALLBACK WriteStream(HSTREAM handle, float *buffer, DWORD length, void *user);
int pb_read_fifo(float *data, int elements);
void cap_write_fifo(float sample);
int pb_fifo_freespace(int nolock);
void init_pipes();

HRECORD rchan = 0;		// recording channel
BASS_INFO info;
HSTREAM stream = 0;	

int openpbdev = -1;
int opencapdev = -1;

float softwareCAPvolume = 0.5;

void showDeviceInfo(BASS_DEVICEINFO info)
{
    printf("Name:%s driver:%s flags:%d:", info.name, info.driver,info.flags);
    if (info.flags & BASS_DEVICE_ENABLED)	printf("%s\n","BASS_DEVICE_ENABLED	");
    if (info.flags & BASS_DEVICE_DEFAULT)	printf("%s\n","BASS_DEVICE_DEFAULT	");
    if (info.flags & BASS_DEVICE_INIT)	printf("%s\n","BASS_DEVICE_INIT	");
    if (info.flags & BASS_DEVICE_LOOPBACK)	printf("%s\n","BASS_DEVICE_LOOPBACK	");
    if (info.flags & BASS_DEVICE_TYPE_DIGITAL)	printf("%s\n","BASS_DEVICE_TYPE_DIGITAL	");
    if (info.flags & BASS_DEVICE_TYPE_DISPLAYPORT)	printf("%s\n","BASS_DEVICE_TYPE_DISPLAYPORT	");
    if (info.flags & BASS_DEVICE_TYPE_HANDSET)	printf("%s\n","BASS_DEVICE_TYPE_HANDSET	");
    if (info.flags & BASS_DEVICE_TYPE_HDMI)	printf("%s\n","BASS_DEVICE_TYPE_HDMI	");
    if (info.flags & BASS_DEVICE_TYPE_HEADPHONES)	printf("%s\n","BASS_DEVICE_TYPE_HEADPHONES	");
    if (info.flags & BASS_DEVICE_TYPE_HEADSET)	printf("%s\n","BASS_DEVICE_TYPE_HEADSET	");
    if (info.flags & BASS_DEVICE_TYPE_LINE)	printf("%s\n","BASS_DEVICE_TYPE_LINE	");
    if (info.flags & BASS_DEVICE_TYPE_MICROPHONE)	printf("%s\n","BASS_DEVICE_TYPE_MICROPHONE	");
    if (info.flags & BASS_DEVICE_TYPE_NETWORK)	printf("%s\n","BASS_DEVICE_TYPE_NETWORK	");
    if (info.flags & BASS_DEVICE_TYPE_SPDIF)	printf("%s\n","BASS_DEVICE_TYPE_SPDIF	");
    if (info.flags & BASS_DEVICE_TYPE_SPEAKERS)	printf("%s\n","BASS_DEVICE_TYPE_SPEAKERS	");

}

void showDeviceInfoWasabi(BASS_WASAPI_DEVICEINFO info, int devnum)
{
    printf("%d: Name:%s defperiod:%f flags:%d minperiod:%f mixchans:%d mixfreq:%d type:%d ", devnum, info.name, info.defperiod, info.flags, info.minperiod,info.mixchans, info.mixfreq,info.type);
    if (info.flags & BASS_DEVICE_ENABLED)	printf("%s\n", "BASS_DEVICE_ENABLED	");
    if (info.flags & BASS_DEVICE_DEFAULT)	printf("%s\n", "BASS_DEVICE_DEFAULT	");
    if (info.flags & BASS_DEVICE_INIT)	printf("%s\n", "BASS_DEVICE_INIT	");
    if (info.flags & BASS_DEVICE_LOOPBACK)	printf("%s\n", "BASS_DEVICE_LOOPBACK	");
    if (info.flags & BASS_DEVICE_TYPE_DIGITAL)	printf("%s\n", "BASS_DEVICE_TYPE_DIGITAL	");
    if (info.flags & BASS_DEVICE_TYPE_DISPLAYPORT)	printf("%s\n", "BASS_DEVICE_TYPE_DISPLAYPORT	");
    if (info.flags & BASS_DEVICE_TYPE_HANDSET)	printf("%s\n", "BASS_DEVICE_TYPE_HANDSET	");
    if (info.flags & BASS_DEVICE_TYPE_HDMI)	printf("%s\n", "BASS_DEVICE_TYPE_HDMI	");
    if (info.flags & BASS_DEVICE_TYPE_HEADPHONES)	printf("%s\n", "BASS_DEVICE_TYPE_HEADPHONES	");
    if (info.flags & BASS_DEVICE_TYPE_HEADSET)	printf("%s\n", "BASS_DEVICE_TYPE_HEADSET	");
    if (info.flags & BASS_DEVICE_TYPE_LINE)	printf("%s\n", "BASS_DEVICE_TYPE_LINE	");
    if (info.flags & BASS_DEVICE_TYPE_MICROPHONE)	printf("%s\n", "BASS_DEVICE_TYPE_MICROPHONE	");
    if (info.flags & BASS_DEVICE_TYPE_NETWORK)	printf("%s\n", "BASS_DEVICE_TYPE_NETWORK	");
    if (info.flags & BASS_DEVICE_TYPE_SPDIF)	printf("%s\n", "BASS_DEVICE_TYPE_SPDIF	");
    if (info.flags & BASS_DEVICE_TYPE_SPEAKERS)	printf("%s\n", "BASS_DEVICE_TYPE_SPEAKERS	");
    printf("\n");
}

uint8_t devstring[MAXDEVSTRLEN +100];
char PBdevs[100][256]; // stores the device names, just for diagnosis, has no real fuction
char CAPdevs[100][256];

// index is enumerated number, 0=default
AUDIODEVS audioPBdevs[100];
AUDIODEVS audioCAPdevs[100];
int pbanz = 0, capanz = 0;

// populate audio device list
void readAudioDevs()
{
    int a;
    pbanz = 0;
    capanz = 0;

    // enter default device manually
    audioPBdevs[pbanz].bassdev = -1;
    strcpy(audioPBdevs[pbanz].name, "Default");
    pbanz++;

    audioCAPdevs[capanz].bassdev = -1;
    strcpy(audioCAPdevs[capanz].name, "Default");
    capanz++;

#ifdef _LINUX_
    BASS_DEVICEINFO info;
    for (a = 1; BASS_GetDeviceInfo(a, &info); a++)
    {
        //showDeviceInfo(info);
        if (info.flags & BASS_DEVICE_ENABLED && !(info.flags & BASS_DEVICE_LOOPBACK))
        {
            if (!strstr(info.name, "efault"))
            {
                audioPBdevs[pbanz].bassdev = a;
                strncpy(audioPBdevs[pbanz].name, info.name, 255);
                audioPBdevs[pbanz].name[255] = 0;
                strncpy(audioPBdevs[pbanz].id, info.driver, 255);
                pbanz++;
            }
        }
    }

    for (a = 1; BASS_RecordGetDeviceInfo(a, &info); a++)
    {
        //showDeviceInfo(info);
        if (info.flags & BASS_DEVICE_ENABLED && !(info.flags & BASS_DEVICE_LOOPBACK))
        {
            if (!strstr(info.name, "efault"))
            {
                audioCAPdevs[capanz].bassdev = a;
                strncpy(audioCAPdevs[capanz].name, info.name, 255);
                audioCAPdevs[capanz].name[255] = 0;
                strncpy(audioCAPdevs[capanz].id, info.driver, 255);
                capanz++;
            }
        }
    }
#endif

#ifdef _WIN32_
    BASS_WASAPI_DEVICEINFO info;
    for (a = 0; BASS_WASAPI_GetDeviceInfo(a, &info); a++)
    {
        //showDeviceInfoWasabi(info,a);
        if (!(info.flags & BASS_DEVICE_INPUT) && (info.flags & BASS_DEVICE_ENABLED) && !(info.flags & BASS_DEVICE_LOOPBACK))
        {
            if (!strstr(info.name, "efault"))
            {
                audioPBdevs[pbanz].bassdev = a;
                strncpy(audioPBdevs[pbanz].name, info.name, 255);
                audioPBdevs[pbanz].name[255] = 0;
                strncpy(audioPBdevs[pbanz].id, info.id, 255);
                pbanz++;
            }
        }

        if ((info.flags & BASS_DEVICE_INPUT) && (info.flags & BASS_DEVICE_ENABLED) && !(info.flags & BASS_DEVICE_LOOPBACK))
        {
            if (!strstr(info.name, "efault"))
            {
                audioCAPdevs[capanz].bassdev = a;
                strncpy(audioCAPdevs[capanz].name, info.name, 255);
                audioCAPdevs[capanz].name[255] = 0;
                strncpy(audioCAPdevs[capanz].id, info.id, 255);
                capanz++;
            }
        }
    }
#endif
}

void printAudioDevs()
{
    printf("PB devices:\n");
    for (int i = 0; i < pbanz; i++)
        printf("idx:%d ID:%s bass:%d name:%s\n", i, audioPBdevs[i].id, audioPBdevs[i].bassdev, audioPBdevs[i].name);

    printf("CAP devices:\n");
    for (int i = 0; i < capanz; i++)
        printf("idx:%d ID:%s bass:%d name:%s\n", i, audioCAPdevs[i].id, audioCAPdevs[i].bassdev, audioCAPdevs[i].name);
}

// build string of audio device name, to be sent to application as response to Broadcast search
// starting with PB devices, sperarator ^, capture devices
// separator between devices: ~
void buildUdpAudioList()
{
    memset(devstring, 0, sizeof(devstring));
    devstring[0] = ' ';     // placeholder for ID for this UDP message
    devstring[1] = '0' + init_audio_result;
    devstring[2] = '0' + init_voice_result;

    //printf("init_voice_result:%d\n", devstring[2]);


    // playback devices
    for (int i = 0; i < pbanz; i++)
    {
        sprintf((char*)devstring + strlen((char*)devstring), "%d: ", i);
        strcat((char*)devstring, audioPBdevs[i].name);
        strcat((char*)devstring, "~");    // audio device separator
    }

    strcat((char*)(devstring + 1), "^");    // PB, CAP separator

    // capture devices
    for (int i = 0; i < capanz; i++)
    {
        sprintf((char*)devstring + strlen((char*)devstring), "%d: ", i);
        strcat((char*)devstring, audioCAPdevs[i].name);
        strcat((char*)devstring, "~");    // audio device separator
    }

    devstring[0] = 3;   // ID for this UDP message
}

uint8_t* getAudioDevicelist(int *len)
{
    // update Status
    devstring[1] = '0' + init_audio_result;
    devstring[2] = '0' + init_voice_result;

    *len = strlen((char*)(devstring+1))+1;
    return devstring;
}

// read audio device list at program start, or if something went wrong
void readAudioDevices()
{
    readAudioDevs();
    //printAudioDevs();
    buildUdpAudioList();
    return;
}

// ret: 0=ok, -1=system error, 1=pb error 2=cap error 3=pb+cap error
int init_audio(int setpbdev, int setcapdev)
{
static int f = 1;
int ret = 0;

    if (f == 1)
    {
        // do only once after program start
        f = 0;
        readAudioDevices();
        init_pipes();
    }

    // translate requested device numbers to bass device numbers
    if (setpbdev < 0 || setpbdev >= pbanz) setpbdev = 0;
    if (setcapdev < 0 || setcapdev >= capanz) setcapdev = 0;

    int pbdev = -1;
    if (setpbdev >=0 && setpbdev < pbanz) pbdev = audioPBdevs[setpbdev].bassdev;
    int capdev = -2;
    if (setcapdev >= 0 && setcapdev < capanz) capdev = audioCAPdevs[setcapdev].bassdev;

    printf("init audio, caprate:%d\n",caprate);
    printf("requested PB  device: %d bassno:%d name:%s\n", setpbdev, pbdev, audioPBdevs[setpbdev].name);
    printf("requested CAP device: %d bassno:%d name:%s\n", setcapdev, capdev, audioCAPdevs[setcapdev].name);

    // check the correct BASS was loaded
    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
    {
        printf("An incorrect version of BASS was loaded\n");
        return 3;
    }

#ifdef _WIN32_
    // use WASAPI for Windows to get exclusive access to sound card
    return init_wasapi(pbdev, capdev);
#endif

#ifdef _LINUX_
    close_audio();

    // ===== PLAYBACK ======

    // initialize default output device
    if (!BASS_Init(pbdev, caprate, 0, NULL, NULL))
    {
        printf("Can't initialize output device: %d err:%d\n", pbdev, BASS_ErrorGetCode());
        ret = 1;
    }
    else
    {

        // read real device number
        int device = BASS_GetDevice();
        if (device == -1)
        {
            printf("BASS_GetDevice: %d err:%d\n", pbdev, BASS_ErrorGetCode());
            ret = 1;
        }
        else
        {
            pbdev = device;
            openpbdev = pbdev;
            printf("real BASS PB Device No: %d\n", pbdev);

            // set play callback
            BASS_GetInfo(&info);
            stream = BASS_StreamCreate(info.freq, CHANNELS, BASS_SAMPLE_FLOAT, (STREAMPROC*)WriteStream, 0); // sample: 32 bit float
            BASS_ChannelSetAttribute(stream, BASS_ATTRIB_BUFFER, 0); // no buffering for minimum latency
            BASS_ChannelPlay(stream, FALSE); // start it
        }
    }

    // ===== CAPTURE ====

    // initalize default recording device
    if (!BASS_RecordInit(capdev))
    {
        printf("Can't initialize recording device: %d err:%d\n", capdev, BASS_ErrorGetCode());
        ret |= 2;
    }
    else
    {

        // read real device number
        int device = BASS_RecordGetDevice();
        if (device == -1)
        {
            printf("BASS_GetDevice: %d err:%d\n", capdev, BASS_ErrorGetCode());
            ret |= 2;
        }
        else
        {
            capdev = device;
            printf("real BASS CAP Device No: %d\n", capdev);

            // set capture callback
            if (rchan) BASS_ChannelStop(rchan);
            rchan = BASS_RecordStart(caprate, CHANNELS, BASS_SAMPLE_FLOAT, RecordingCallback, 0);
            if (!rchan) 
            {
                printf("Can't start capturing: %d\n", BASS_ErrorGetCode());
                ret |= 2;
            }
            else
                opencapdev = capdev;
        }
    }

    if(ret == 0)
        printf("audio started successfully for PBdev:%d and CAPdev:%d\n", openpbdev, opencapdev);
    else
    {
        opencapdev = -1;
        openpbdev = -1;
        readAudioDevices();
    }
    if (ret == 1)
        printf("audio initialized: PBerror CapOK\n");
    if (ret == 2)
        printf("audio initialized: PBOK CapERROR\n");
    if (ret == 3)
        printf("audio initialized: PBerror CapERROR\n");

    return ret;
#endif
}

#ifdef _LINUX_
int selectPBdevice()
{
    if (!BASS_SetDevice(openpbdev))
    {
        printf("BASS_SetDevice: %d err:%d\n", openpbdev, BASS_ErrorGetCode());
    }
    else
        return 1;

    return 0;
}

int selectCAPdevice()
{
    if (!BASS_SetDevice(opencapdev))
    {
        //printf("BASS_SetDevice: %d err:%d\n", opencapdev, BASS_ErrorGetCode());
    }
    else
        return 1;

    return 0;
}

void close_audio()
{
    if(stream != 0)
    {
        printf("close Audio Devices\n");
        selectCAPdevice();
        BASS_ChannelStop(rchan);
        int rr = BASS_RecordFree();
        if (!rr) printf("Bass_RecordFree error: %d\n", BASS_ErrorGetCode());

        selectPBdevice();
        BASS_StreamFree(stream);
        int r = BASS_Free();
        if(!r) printf("Bass_Free error: %d\n", BASS_ErrorGetCode());
        stream = 0;
    }
}

void setPBvolume(int v)
{
    // the volume comes in % 0..99
    // map to 0..1
    float vf = v;
    vf /= 100;

    //printf("set PB volume to:%d / %f [0..1]\n", v, vf );

    if(selectPBdevice())
        if (!BASS_SetVolume(vf))
            printf("setPBvolume: %d err:%d\n", openpbdev, BASS_ErrorGetCode());
}

void setCAPvolume(int v)
{
    // the volume comes in % 0..99
    // map to min/maxPBvol
    float vf = v;
    vf /= 100;

    //printf("set CAP volume to:%d / %f [0..1]\n", v, vf);

    if (selectCAPdevice())
    {
        if (!BASS_RecordSetInput(-1, BASS_INPUT_ON, vf))
            printf("setCAPvolume: %d err:%d\n", opencapdev, BASS_ErrorGetCode());
        else
            softwareCAPvolume = 1;
    }
    else
    {
        softwareCAPvolume = (float)v;
        softwareCAPvolume /= 50;
    }
}

// capture callback
BOOL CALLBACK RecordingCallback(HRECORD handle, const void *buffer, DWORD length, void *user)
{
    //printf("captured %ld samples, channels:%d\n",length/sizeof(float),CHANNELS);
    //measure_speed(length/sizeof(float));
    
    float *fbuffer = (float *)buffer;
    //showbytestringf((char*)"cap:", fbuffer, 10);
    //printf("w:%ld ",length/sizeof(float));
    for(unsigned int i=0; i<(length/sizeof(float)); i+=CHANNELS)
    {
        //printf("%f\n",fbuffer[i]);
        cap_write_fifo(fbuffer[i]);
    }

    return TRUE; // continue recording
}

// play callback
// length: bytes. float=4byte, 2channels, so it requests samples*8
DWORD CALLBACK WriteStream(HSTREAM handle, float *buffer, DWORD length, void *user)
{
    //printf("requested %ld samples\n", length / sizeof(float));
    int ret = pb_read_fifo(buffer, length / sizeof(float));
    if(ret == 0)
    {
        // fifo empty, send 00
        memset(buffer,0,length);
    }
	return length;
}

#endif

// set volume 
void setVolume(int pbcap, int v)
{
    if (pbcap == 0) setPBvolume(v);
    else setCAPvolume(v);
}


// ================ thread safe fifo for audio callback routines ===============

#ifdef _WIN32_
CRITICAL_SECTION cap_crit_sec;
CRITICAL_SECTION pb_crit_sec;
#define CAP_LOCK	EnterCriticalSection(&cap_crit_sec)
#define PB_LOCK	    EnterCriticalSection(&pb_crit_sec)
void CAP_UNLOCK()
{
    if (&cap_crit_sec != NULL)
        LeaveCriticalSection(&cap_crit_sec);
}
void PB_UNLOCK()
{
    if (&pb_crit_sec != NULL)
        LeaveCriticalSection(&pb_crit_sec);
}
#endif

#ifdef _LINUX_
pthread_mutex_t cap_crit_sec;
pthread_mutex_t pb_crit_sec;
#define CAP_LOCK	pthread_mutex_lock(&cap_crit_sec)
void CAP_UNLOCK() { pthread_mutex_unlock(&cap_crit_sec); }
#define PB_LOCK	pthread_mutex_lock(&pb_crit_sec)
void PB_UNLOCK() { pthread_mutex_unlock(&pb_crit_sec); }
#endif

#define AUDIO_PLAYBACK_BUFLEN (48000 * 10) // space for 10 seconds of samples
#define AUDIO_CAPTURE_BUFLEN  24000        // space for 0.5s

int cap_wridx=0;
int cap_rdidx=0;
float cap_buffer[AUDIO_CAPTURE_BUFLEN];

int pb_wridx=0;
int pb_rdidx=0;
float pb_buffer[AUDIO_PLAYBACK_BUFLEN];

void init_pipes()
{
#ifdef _WIN32_
    if (&cap_crit_sec != NULL) DeleteCriticalSection(&cap_crit_sec);
    InitializeCriticalSection(&cap_crit_sec);

    if (&pb_crit_sec != NULL) DeleteCriticalSection(&pb_crit_sec);
    InitializeCriticalSection(&pb_crit_sec);
#endif
}

// write one sample into the fifo
// overwrite old data if the fifo is full
void cap_write_fifo(float sample)
{
    if (((cap_wridx + 1) % AUDIO_CAPTURE_BUFLEN) == cap_rdidx)
    {
        printf("cap fifo full\n");
    }

	CAP_LOCK;
    cap_buffer[cap_wridx] = sample;
    if(++cap_wridx >= AUDIO_CAPTURE_BUFLEN) cap_wridx = 0;
	CAP_UNLOCK();

    // if monitoring is activated then write it also to the voice fifo
    if (VoiceAudioMode == VOICEMODE_LISTENAUDIOIN)
        toVoice(sample);
}

int cap_read_fifo(float *data)
{
	CAP_LOCK;

	if (cap_rdidx == cap_wridx)
	{
		// Fifo empty, no data available
		CAP_UNLOCK();
		return 0;
	}
	
	*data = cap_buffer[cap_rdidx];
    if(++cap_rdidx >= AUDIO_CAPTURE_BUFLEN) cap_rdidx = 0;
    CAP_UNLOCK();

	return 1;
}

void cap_write_fifo_clear()
{
   cap_wridx = cap_rdidx = 0;
}

int cap_fifo_freespace()
{
    int freebuf = 0;

    CAP_LOCK;

    int elemInFifo = (cap_wridx + AUDIO_CAPTURE_BUFLEN - cap_rdidx) % AUDIO_CAPTURE_BUFLEN;
    freebuf = AUDIO_CAPTURE_BUFLEN - elemInFifo;

    CAP_UNLOCK();

    return freebuf;
}

int cap_fifo_usedPercent()
{
    int fs = cap_fifo_freespace();
    int used = AUDIO_CAPTURE_BUFLEN - fs;
    used = (used * 100) / AUDIO_CAPTURE_BUFLEN;
    return used;
}


void pb_write_fifo(float sample)
{
	PB_LOCK;
    
    // check if there is free space in fifo
    if(pb_fifo_freespace(1) == 0)
    {
        PB_UNLOCK();
        printf("************* pb fifo full\n");
        return;
    }
    
    pb_buffer[pb_wridx] = sample;
    if(++pb_wridx >= AUDIO_PLAYBACK_BUFLEN) pb_wridx = 0;
	PB_UNLOCK();
    //printf("write: pbw:%d pbr:%d\n",pb_wridx,pb_rdidx);
}

void pb_write_fifo_clear()
{
    pb_wridx = pb_rdidx = 0;
}

int pb_fifo_usedBlocks()
{
    int fs = pb_fifo_freespace(0);
    int used = AUDIO_PLAYBACK_BUFLEN - fs;
    used /= (txinterpolfactor * UDPBLOCKLEN * 8 / bitsPerSymbol);
    return used;
}

int pb_fifo_freespace(int nolock)
{
int freebuf = 0;

    if(nolock == 0) PB_LOCK;
    
    int elemInFifo = (pb_wridx + AUDIO_PLAYBACK_BUFLEN - pb_rdidx) % AUDIO_PLAYBACK_BUFLEN;
    freebuf = AUDIO_PLAYBACK_BUFLEN - elemInFifo;
    
	if(nolock == 0) PB_UNLOCK();
    
    //printf("fifolen:%d check: pbw:%d pbr:%d freebuf:%d\n",AUDIO_PLAYBACK_BUFLEN,pb_wridx,pb_rdidx,freebuf);
    
    return freebuf;
}

int pb_fifo_usedspace()
{
    int anz = pb_fifo_freespace(0);
    return AUDIO_PLAYBACK_BUFLEN - anz;
}

// read elements floats from fifo or return 0 if not enough floats are available
int pb_read_fifo(float *data, int elements)
{
    //printf("pb read fifo: %d\n",elements);
    PB_LOCK;
    
    int e = AUDIO_PLAYBACK_BUFLEN - pb_fifo_freespace(1);
    if(e < elements)
	{
		// Fifo empty, no data available
		PB_UNLOCK();
        //printf("pb fifo empty: TX underrun\n");
        //printf("pb fifo empty, need:%d have:%d size:%d\n",elements,e,AUDIO_PLAYBACK_BUFLEN);
		return 0;
	}
	
    for(int i=0; i<elements; i++)
    {
        data[i] = pb_buffer[pb_rdidx];
        if(++pb_rdidx >= AUDIO_PLAYBACK_BUFLEN) pb_rdidx = 0;
    }
    //printf("read %d floats\n",elements);
    
    PB_UNLOCK();
    return 1;
}

void clear_audio_fifos()
{
    pb_write_fifo_clear();
    cap_write_fifo_clear();
}

// ================ Play PCM Audio File ===========================

typedef struct _AUDIOFILES_ {
    char fn[256];
    int duration;
} AUDIOFILES;

AUDIOFILES audiofiles[12] =
{
    {"amsat", 1100},
    {"qpsk", 1100},
    {"psk8", 1100},
    {"3000", 600},
    {"4000", 600},
    {"4410", 900},
    {"4800", 900},
    {"5500", 900},
    {"6000", 600},
    {"6600", 900},
    {"7200", 900},
    {"kbps", 1000},
};

char* getAudiofn(int aidx, char *ext)
{
    static char filename[300];
    strcpy(filename, "audio/");
    strcat(filename, audiofiles[aidx].fn);
    strcat(filename, ext);
    return filename;
}

void playAudioFLAC(int aidx)
{
    int resamp = 0;
    int len;
    int16_t d[100];
    printf("play:%s\n", getAudiofn(aidx, ".pcm"));
    FILE *fp = fopen(getAudiofn(aidx,".pcm"), "rb");
    if (fp)
    {
        while ((len = fread(d, sizeof(int16_t), 100, fp)))
        {
            for (int i = 0; i < len; i++)
            {
                float f = (float)d[i];
                f /= 32768;
                pb_write_fifo(f);

                if (caprate == 48000)
                {
                    if (++resamp >= 9)
                    {
                        resamp = 0;
                        pb_write_fifo(f);
                    }
                }

                // sync with soundcard
                while (pb_fifo_usedspace() > 10000) sleep_ms(1);
            }
            if (len != 100) break;
        }
        fclose(fp);
    }
    else
        printf("audio file not found\n");
}

int ann_running = 0;
int transmissions = 10000;

void sendAnnouncement()
{
    if (announcement == 0) return;

    if (++transmissions >= announcement)
    {
        ann_running = 1;
        transmissions = 0;

        playAudioFLAC(0);
        if(bitsPerSymbol == 2) playAudioFLAC(1);
        else playAudioFLAC(2);
        switch (linespeed)
        {
            case 3000: playAudioFLAC(3); break;
            case 4000: playAudioFLAC(4); break;
            case 4410: playAudioFLAC(5); break;
            case 4800: playAudioFLAC(6); break;
            case 5500: playAudioFLAC(7); break;
            case 6000: playAudioFLAC(8); break;
            case 6600: playAudioFLAC(9); break;
            case 7200: playAudioFLAC(10); break;
        }
        playAudioFLAC(11);
        
        ann_running = 0;
    }
}

