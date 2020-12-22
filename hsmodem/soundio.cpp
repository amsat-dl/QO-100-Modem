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
* soundio.c ... interface to libsoundio
*
*/

#include "hsmodem.h"

void io_cap_write_fifo(float sample);

#define MAXAUDIODEVICES 50

struct SoundIo* soundio = NULL;
struct SoundIoDevice* io_pb_device = NULL;
struct SoundIoDevice* io_cap_device = NULL;
struct SoundIoInStream* instream = NULL;
struct SoundIoOutStream* outstream = NULL;

float latenz = 0.1f;


typedef struct _AUDIODEV_ {
    int     in_out = 0;         // 0=in, 1=out
    char    name[1000] = { 0 };
    char    id[1000] = { 0 };
    int     minsamprate = 44100;
    int     maxsamprate = 48000;
    int     stereo_mono = 2;    // 1=mono, 2=stereo
} AUDIODEV;

AUDIODEV audiodev[MAXAUDIODEVICES];
int audiodevidx = 0;

bool pbrawdev = true;
bool caprawdev = true;

void print_devs()
{
    printf("\n ==== AUDIO devices ====\n");
    for (int i = 0; i < audiodevidx; i++)
    {
        if(i>0) printf(" -----------------\n");
        printf("Name: %s\n", audiodev[i].name);
        printf("ID  : %s\n", audiodev[i].id);
        printf("I/O : %s\n", (audiodev[i].in_out == 0) ? "record":"playback");
        printf("Chan: %s\n", (audiodev[i].stereo_mono == 2) ? "stereo" : "mono");
        printf("minR: %d\n", audiodev[i].minsamprate);
        printf("maxR: %d\n", audiodev[i].maxsamprate);
    }
    printf("\n =======================\n");
}

static void get_channel_layout(const struct SoundIoChannelLayout* layout)
{
    if (layout->name) 
    {
        if (strstr(layout->name, "ereo"))
            audiodev[audiodevidx].stereo_mono = 2;
        if (strstr(layout->name, "ono"))
            audiodev[audiodevidx].stereo_mono = 1;
    }
}

int print_device(struct SoundIoDevice* device) 
{
    if (!device->probe_error)
    {
        // ignore if exists
        for (int i = 0; i < audiodevidx; i++)
            if (!strcmp(device->id, audiodev[i].id)) return 0;

        if (strstr(device->name, "onitor")) return 0;

        strncpy(audiodev[audiodevidx].id, device->id, 999);
        audiodev[audiodevidx].id[999] = 0;
        strncpy(audiodev[audiodevidx].name, device->name, 999);
        audiodev[audiodevidx].name[999] = 0;

        for (int i = 0; i < device->layout_count; i++)
            get_channel_layout(&device->layouts[i]);

        for (int i = 0; i < device->sample_rate_count; i++) 
        {
            struct SoundIoSampleRateRange* range = &device->sample_rates[i];
            if (range->min < audiodev[audiodevidx].minsamprate)
                audiodev[audiodevidx].minsamprate = range->min;

            if (range->max > audiodev[audiodevidx].maxsamprate)
                audiodev[audiodevidx].maxsamprate = range->max;
        }
        if (audiodev[audiodevidx].minsamprate > 44100)
            return 0;
        if (audiodev[audiodevidx].maxsamprate < 48000)
            return 0;
        return 1;
    }
    return 0;
}

static int scan_devices(struct SoundIo* soundio) 
{
    audiodevidx = 0;
    for (int i = 0; i < soundio_input_device_count(soundio); i++) 
    {
        
        struct SoundIoDevice* device = soundio_get_input_device(soundio, i);
        if (print_device(device) == 1)
        {
            audiodev[audiodevidx].in_out = 0;
            audiodevidx++;
        }
        soundio_device_unref(device);
    }

    for (int i = 0; i < soundio_output_device_count(soundio); i++) 
    {
        struct SoundIoDevice* device = soundio_get_output_device(soundio, i);
        if (print_device(device) == 1)
        {
            audiodev[audiodevidx].in_out = 1;
            audiodevidx++;
        }
        soundio_device_unref(device);
    }
    return 0;
}

// build string of audio device name, to be sent to application as response to Broadcast search
// starting with PB devices, sperarator ^, capture devices
// separator between devices: ~
uint8_t io_devstring[MAXDEVSTRLEN + 100];

void io_buildUdpAudioList()
{
    memset(io_devstring, 0, sizeof(io_devstring));
    io_devstring[0] = ' ';     // placeholder for ID for this UDP message
    io_devstring[1] = '0' + init_audio_result;
    io_devstring[2] = '0' + init_voice_result;

    // playback devices
    for (int i = 0; i < audiodevidx; i++)
    {
        if (audiodev[i].in_out == 1)
        {
            strcat((char*)io_devstring, audiodev[i].name);
            strcat((char*)io_devstring, "~");    // audio device separator
        }
    }

    strcat((char*)(io_devstring + 1), "^");    // PB, CAP separator

    // capture devices
    for (int i = 0; i < audiodevidx; i++)
    {
        if (audiodev[i].in_out == 0)
        {
            strcat((char*)io_devstring, audiodev[i].name);
            strcat((char*)io_devstring, "~");    // audio device separator
        }
    }

    io_devstring[0] = 3;   // ID for this UDP message
}

uint8_t* io_getAudioDevicelist(int* len)
{
    // update Status
    io_devstring[1] = '0' + init_audio_result;
    io_devstring[2] = '0' + init_voice_result;

    *len = strlen((char*)(io_devstring + 1)) + 1;
    return io_devstring;
}

void io_readAudioDevices()
{
    if (soundio == NULL) return;
    // to get actual data
    soundio_flush_events(soundio);

    scan_devices(soundio); // read devices
    io_buildUdpAudioList();

    //print_devs();
}

char* getDevID(char* devname, int io)
{
    for (int i = 0; i < audiodevidx; i++)
    {
        if (!strcmp(devname, audiodev[i].name) && io == audiodev[i].in_out)
        {
            return audiodev[i].id;
        }
    }
    return NULL;
}

int min_int(int a, int b) 
{
    return (a < b) ? a : b;
}

void read_callback(struct SoundIoInStream* instream, int frame_count_min, int frame_count_max)
{
    int err;
    if (instream == NULL) return;
    //printf("cap: %d  %d\n", frame_count_min, frame_count_max);
    //int chans = instream->layout.channel_count;

    struct SoundIoChannelArea* areas;
    // samples are in areas.ptr
    int frames_left = frame_count_max; // take all
    while (1)
    {
        int frame_count = frames_left;
        if ((err = soundio_instream_begin_read(instream, &areas, &frame_count)))
        {
            fprintf(stderr, "begin read error: %s", soundio_strerror(err));
            restart_modems = 1;
            return;
        }
        if (!frame_count)
            break;

        for (int frame = 0; frame < frame_count; frame += 1)
        {
            for (int ch = 0; ch < instream->layout.channel_count; ch += 1)
            {
                if (caprawdev == false)
                {
                    // shared device
                    float frxdata;
                    memcpy(&frxdata, areas[ch].ptr, instream->bytes_per_sample);
                    areas[ch].ptr += areas[ch].step;
                    if (ch == 0)
                    {
                        float f = frxdata;
                        f *= softwareCAPvolume;
                        io_cap_write_fifo(f);
                    }
                }
                else
                {
                    // raw device
                    // shared device
                    int16_t rxdata;
                    memcpy(&rxdata, areas[ch].ptr, instream->bytes_per_sample);
                    areas[ch].ptr += areas[ch].step;
                    if (ch == 0)
                    {
                        float f = rxdata;
                        f /= 32768;
                        f *= softwareCAPvolume;
                        io_cap_write_fifo(f);
                    }
                }
            }
        }
        //printf("%d into fifo\n", frame_count);
        // needs to sleep or it will not work correctly on Windows, no idea why
        sleep_ms(1);

        //measure_speed_bps(frame_count);

        if ((err = soundio_instream_end_read(instream)))
        {
            fprintf(stderr, "end read error: %s", soundio_strerror(err));
            restart_modems = 1;
            return;
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}

void overflow_callback(struct SoundIoInStream* instream) 
{
    static int count = 0;
    printf("overflow %d\n", ++count);
}

#define MTXCHECK 48000
void getTXMax(float fv)
{
    static float farr[MTXCHECK];
    static int idx = 0;
    static int f = 1;

    if (f)
    {
        f = 0;
        for (int i = 0; i < MTXCHECK; i++)
            farr[i] = 1;
    }

    farr[idx] = fv;
    idx++;
    if (idx == MTXCHECK)
    {
        idx = 0;
        float max = 0;
        for (int i = 0; i < MTXCHECK; i++)
        {
            if (farr[i] > max) max = farr[i];
        }
        maxTXLevel = (uint8_t)(max * 100);
        //printf("TX max: %10.6f\n", max);
    }
}

// #define SINEWAVETEST

#ifdef SINEWAVETEST
static const double PI = 3.14159265358979323846264338328;
static double seconds_offset = 0.0;
#endif

static void write_callback(struct SoundIoOutStream* outstream, int frame_count_min, int frame_count_max)
{
    //printf("pb: %d  %d\n", frame_count_min, frame_count_max);
#ifdef SINEWAVETEST
    double float_sample_rate = outstream->sample_rate;
    double seconds_per_frame = 1.0 / float_sample_rate;
    double pitch = 440.0;
    double radians_per_second = pitch * 2.0 * PI;
#endif
    struct SoundIoChannelArea* areas;
    int err;

    int frames_left = 4800;
    if (frame_count_max < frames_left)
        frames_left = frame_count_max;

    for (;;) {
        int frame_count = frames_left;
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            fprintf(stderr, "write_callback unrecoverable soundio_outstream_begin_write error: %s\n", soundio_strerror(err));
            restart_modems = 1;
            return;
        }

        if (!frame_count)
            break;

        //printf("ck: %d\n", frame_count);

        float* f = (float*)malloc(frame_count * sizeof(float));
        int fiforet = io_pb_read_fifo_num(f, frame_count);
        if (fiforet == 0)
        {
            // elements not available, fill with zeroes
            memset(f, 0, sizeof(float) * frame_count);
        }

        const struct SoundIoChannelLayout* layout = &outstream->layout;

        for (int frame = 0; frame < frame_count; frame += 1) 
        {
#ifdef SINEWAVETEST
            double sample = sin((seconds_offset + frame * seconds_per_frame) * radians_per_second);
#endif
            for (int channel = 0; channel < layout->channel_count; channel += 1) 
            {
                float ftx = f[frame] * softwarePBvolume;
                getTXMax(ftx);
                if (pbrawdev == false)
                {
#ifdef SINEWAVETEST
                    write_sample_float32ne(areas[channel].ptr, sample); // sine wave test tone
#endif
                    write_sample_float32ne(areas[channel].ptr, ftx);
                }
                else
                    write_sample_s16ne(areas[channel].ptr, ftx);
                areas[channel].ptr += areas[channel].step;
            }
        }
#ifdef SINEWAVETEST
        seconds_offset = fmod(seconds_offset + seconds_per_frame * frame_count, 1.0);
#endif

        free(f);

        if ((err = soundio_outstream_end_write(outstream))) {
            if (err == SoundIoErrorUnderflow)
                return;
            fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
            restart_modems = 1;
            return;
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}

void underflow_callback(struct SoundIoOutStream* outstream) 
{
    static int count = 0;
    printf("underflow %d\n", count++);
}


int io_init_sound(char *pbname, char *capname)
{
    int err;
    init_audio_result = 0;

    printf("\n ==== IO INIT AUDIO devices ====\n");
    printf("requested\nTX:<%s>\nRX:<%s>\ncapture rate:%d\n\n",pbname,capname,caprate);

    io_close_audio();

    // prepare and connect to libsoundio
    soundio = soundio_create();
    if (!soundio) {
        printf("soundio_create: out of memory\n");
        return 0;
    }
#ifdef _WIN32_
    if ((err=soundio_connect_backend(soundio, SoundIoBackendWasapi))) {
        printf("soundio_connect: %s\n", soundio_strerror(err));
        return 0;
    }
#endif
#ifdef _LINUX_
    if ((err = soundio_connect(soundio))) {
        printf("soundio_connect: %s\n", soundio_strerror(err));
        return 0;
    }
#endif

    io_readAudioDevices();
    io_init_pipes();

    if (pbname == NULL || capname == NULL || strlen(pbname) < 3 || strlen(capname) < 3)  // no devices defined yet
    {
        printf("no devices specified\n");
        return 0;
    }

    char* pbdevid = getDevID(pbname,1);
    if (pbdevid == NULL) return 0;

    char* capdevid = getDevID(capname,0);
    if (capdevid == NULL) return 0;

    // define the capture device
    soundio_flush_events(soundio);

    // under Windows we usually use raw devices. This does not work with 
    // virtual sound cards due to problems in libsoundio
    // for VACs we use shared devices, otherwise raw
    pbrawdev = true;
    if (strstr(pbname, "irtual") || strstr(pbname, "VAC"))
        pbrawdev = false;

    caprawdev = true;
    if (strstr(capname, "irtual") || strstr(capname, "VAC"))
        caprawdev = false;

    for (int i = 0; i < soundio_input_device_count(soundio); i++)
    {
        io_cap_device = NULL;
        struct SoundIoDevice* device = soundio_get_input_device(soundio, i);
        if (strcmp(device->id, capdevid) == 0 
#ifdef _WIN32_
            && device->is_raw == caprawdev
#endif
            )
        {
            io_cap_device = device;
            break;
        }
        soundio_device_unref(device);
    }
    if (!io_cap_device)
    {
        printf("Invalid device id: %s\n", capdevid);
        return 0;
    }

    if (io_cap_device->probe_error)
    {
        printf("Unable to probe device: %s\n", soundio_strerror(io_cap_device->probe_error));
        return 0;
    }

    // create capture callback
    instream = soundio_instream_create(io_cap_device);
    if (!instream) {
        printf("out of memory\n");
        return 0;
    }

    // raw devices: 16bit LE, shared devices float
    if (caprawdev)
    {
        instream->format = SoundIoFormatS16NE;
        instream->sample_rate = caprate;
        physRXcaprate = caprate;
    }
    else 
    {
        // a VAC needs these settings or it will not work with 44100
        instream->format = SoundIoFormatFloat32NE;
        instream->sample_rate = AUDIO_SAMPRATE;
        physRXcaprate = AUDIO_SAMPRATE;
    }
    instream->software_latency = latenz;
    instream->read_callback = read_callback;
    instream->overflow_callback = overflow_callback;
    instream->userdata = NULL;

    if ((err = soundio_instream_open(instream))) {
        printf("unable to open input stream: %d: %s", err, soundio_strerror(err));
        return 0;
    }

    if ((err = soundio_instream_start(instream))) {
        fprintf(stderr, "unable to start input device: %s", soundio_strerror(err));
        return 0;
    }
    init_audio_result |= 2;

    printf("selected CAPTURE device:\nname:%s\nid  :%s\n", capname, capdevid);
    printf("physical capture rate:%d, logical capture rate:%d\n", physRXcaprate, caprate);
    printf("format: %s\n\n", soundio_format_string(instream->format));

    // the CAP callback is running now
    
    // define the playback device
    for (int i = 0; i < soundio_output_device_count(soundio); i++)
    {
        io_pb_device = NULL;
        struct SoundIoDevice* device = soundio_get_output_device(soundio, i);
        if (strcmp(device->id, pbdevid) == 0 
#ifdef _WIN32_
            && device->is_raw == pbrawdev
#endif
            )
        {
            io_pb_device = device;
            break;
        }
        soundio_device_unref(device);
    }
    if (!io_pb_device)
    {
        printf("Invalid device id: %s\n", pbdevid);
        return 0;
    }

    if (io_pb_device->probe_error)
    {
        printf("Unable to probe device: %s\n", soundio_strerror(io_pb_device->probe_error));
        return 0;
    }
    
    // create playback callback
    outstream = soundio_outstream_create(io_pb_device);
    if (!outstream) {
        printf("soundio_outstream_create: out of memory\n");
        return 0;
    }

    // raw devices: 16bit LE, shared devices float
    if (pbrawdev)
        outstream->format = SoundIoFormatS16NE;
    else
        outstream->format = SoundIoFormatFloat32NE;

    outstream->sample_rate = caprate;
    outstream->software_latency = latenz;
    outstream->write_callback = write_callback;
    outstream->underflow_callback = underflow_callback;
    outstream->userdata = NULL;

    if ((err = soundio_outstream_open(outstream))) {
        printf("unable to open output stream: %s", soundio_strerror(err));
        return 0;
    }

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start output device: %s", soundio_strerror(err));
        return 0;
    }
    init_audio_result |= 1;

    printf("selected PLAYBACK device:\nname:%s\nid  :%s\n", pbname, pbdevid);
    printf("physical capture rate:%d, logical capture rate:%d\n", caprate, caprate);
    printf("format: %s\n\n", soundio_format_string(outstream->format));
    
    init_tune();

	return init_audio_result;
}

void io_close_audio()
{
    printf("close Audio\n");
    if(instream) soundio_instream_destroy(instream);
    instream = NULL;

    if (outstream) soundio_outstream_destroy(outstream);
    outstream = NULL;

    if(io_pb_device) soundio_device_unref(io_pb_device);
    io_pb_device = NULL;

    if (io_cap_device) soundio_device_unref(io_cap_device);
    io_cap_device = NULL;

    if (soundio) soundio_destroy(soundio);
    soundio = NULL;
}

void io_setPBvolume(int v)
{
    // the volume comes in % 0..99
    softwarePBvolume = ((float)v) / 50.0f;
}

void io_setCAPvolume(int v)
{
    // the volume comes in % 0..99
    softwareCAPvolume = ((float)v) / 50.0f;
}

// set volume 
void io_setVolume(int pbcap, int v)
{
    if (pbcap == 0) io_setPBvolume(v);
    else io_setCAPvolume(v);
}

void io_setLSvolume(int v)
{
    // the volume comes in % 0..99
    softwareLSvolume = ((float)v) / 50.0f;
}

void io_setMICvolume(int v)
{
    // the volume comes in % 0..99
    softwareMICvolume = ((float)v) / 50.0f;
}

void setVolume_voice(int pbcap, int v)
{

}

