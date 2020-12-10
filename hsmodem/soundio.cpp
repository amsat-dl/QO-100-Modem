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
/*#ifdef _WIN32_
        // only use raw (exclusive) devices
        if (device->is_raw == false) return 0;
#endif*/

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
            exit(1);
        }
        if (!frame_count)
            break;

        for (int frame = 0; frame < frame_count; frame += 1)
        {
            for (int ch = 0; ch < instream->layout.channel_count; ch += 1)
            {
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

        //measure_speed_bps(frame_count);

        if ((err = soundio_instream_end_read(instream)))
        {
            fprintf(stderr, "end read error: %s", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}
/*
void read_callback(struct SoundIoInStream* instream, int frame_count_min, int frame_count_max) 
{
    int err;
    //printf("cap: %d  %d\n", frame_count_min, frame_count_max);
    // bytes_per_frame == 4 because we use float
    // instream->bytes_per_sample/bytes_per_frame = 1 (mono) or 2 (stereo)
    int chans = instream->layout.channel_count;

    struct SoundIoChannelArea* areas;
    // samples are in areas.ptr
    int frames_left = frame_count_max; // take all
    while (1)
    {
        int frame_count = frames_left;
        if ((err = soundio_instream_begin_read(instream, &areas, &frame_count)))
        {
            fprintf(stderr, "begin read error: %s", soundio_strerror(err));
            exit(1);
        }
        if (!frame_count)
            break;

        // do something with the data in *area.ptr
        // take a float every chans*4 Bytes
        int16_t* samples = (int16_t*)(areas[0].ptr);
        int pos = 0;
        for (int i = 0; i < frame_count; i++)
        {
            float f = samples[pos];
            f /= 32768;
            f *= softwareCAPvolume;
            io_cap_write_fifo(f);
            pos += chans;
        }

        //measure_speed_bps(frame_count);

        if ((err = soundio_instream_end_read(instream))) {
            fprintf(stderr, "end read error: %s", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}
*/
void overflow_callback(struct SoundIoInStream* instream) 
{
    static int count = 0;
    printf("overflow %d\n", ++count);
}

#define MAXCAPCHUNKLEN  50000

static void write_callback(struct SoundIoOutStream* outstream, int frame_count_min, int frame_count_max)
{
    struct SoundIoChannelArea* areas;
    int err;
    int frames_left = frame_count_max;

    while (1)
    {
        int frame_count = frames_left;
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count)))
        {
            fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;

        float f[MAXCAPCHUNKLEN];
        int fiforet = io_pb_read_fifo_num(f, frame_count);
        if (fiforet == 0)
        {
            // elements not available, fill with zeroes
            //printf("not enough data, send zeroes\n");
            memset(f, 0, sizeof(float) * frame_count);
        }

        const struct SoundIoChannelLayout* layout = &outstream->layout;

        for (int frame = 0; frame < frame_count; frame++)
        {
            for (int channel = 0; channel < layout->channel_count; channel++)
            {
                write_sample_s16ne(areas[channel].ptr, f[frame]);
                areas[channel].ptr += areas[channel].step;
            }
        }

        if ((err = soundio_outstream_end_write(outstream))) {
            if (err == SoundIoErrorUnderflow)
                return;
            fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}
/*
static void write_callback(struct SoundIoOutStream* outstream, int frame_count_min, int frame_count_max) 
{
    int chans = outstream->layout.channel_count;
    struct SoundIoChannelArea* areas;
    int err;
    int frames_left = min_int(frame_count_max,MAXCAPCHUNKLEN);// frame_count_max;
    //printf("\nmin: %d max:%d\n", frame_count_min, frame_count_max);

    // we have to write frame_count_max, not less, or we get an underrun
    // this has to be written in chunks requested by soundio_outstream_begin_write
    float f[MAXCAPCHUNKLEN];
    while (1)
    {
        int frame_count = frames_left;
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            printf("unrecoverable soundio_outstream_begin_write error: %s\n", soundio_strerror(err));
            exit(1);
        }
        if (!frame_count) break; // will normally never happen
        //printf("chunk: %d\n", frame_count);

        // soundio_outstream_begin_write requested to write frame_count elements
        int fiforet = io_pb_read_fifo_num(f, frame_count);
        if (fiforet == 0)
        {
            // elements not available, fill with zeroes
            //printf("not enough data, send zeroes\n");
            memset(f, 0, sizeof(float) * frame_count);
        }

        // apply volume
        for (int i = 0; i < frame_count; i++)
            f[i] *= softwarePBvolume;
        
        // put data into soundio buffer
        for (int frame = 0; frame < frame_count; frame++) 
        {
            for (int ch = 0; ch < chans; ch++) 
            {
                int16_t* s = (int16_t*)areas[ch].ptr;
                *s = (int16_t)(f[frame] * 32768.0);
                areas[ch].ptr += areas[ch].step;
            }
        }

        // and finalize this chunk
        if ((err = soundio_outstream_end_write(outstream))) {
            if (err == SoundIoErrorUnderflow)
                return;
            printf("unrecoverable soundio_outstream_end_write error: %s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}
*/
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
    printf("requested: <%s> <%s>\ncapture rate:%d\n",pbname,capname,caprate);

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
    printf("selected CAP device:\nname:%s\nid  :%s\n", capname, capdevid);

    soundio_flush_events(soundio);

    for (int i = 0; i < soundio_input_device_count(soundio); i++)
    {
        io_cap_device = NULL;
        struct SoundIoDevice* device = soundio_get_input_device(soundio, i);
        if (strcmp(device->id, capdevid) == 0 
#ifdef _WIN32_
            && device->is_raw == true
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

    instream->format = SoundIoFormatS16NE;
    instream->sample_rate = caprate;
    instream->software_latency = 0.0;
    instream->read_callback = read_callback;
    instream->overflow_callback = overflow_callback;
    instream->userdata = NULL;

    if ((err = soundio_instream_open(instream))) {
        printf("unable to open input stream: %s", soundio_strerror(err));
        return 0;
    }

    if ((err = soundio_instream_start(instream))) {
        fprintf(stderr, "unable to start input device: %s", soundio_strerror(err));
        return 0;
    }
    init_audio_result |= 2;
    // the CAP callback is running now

    // define the playback device
    printf("selected PB device:\nname:%s\nid  :%s\n", pbname, pbdevid);

    for (int i = 0; i < soundio_output_device_count(soundio); i++)
    {
        io_pb_device = NULL;
        struct SoundIoDevice* device = soundio_get_output_device(soundio, i);
        if (strcmp(device->id, pbdevid) == 0 
#ifdef _WIN32_
            && device->is_raw == true
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

    outstream->format = SoundIoFormatS16NE;
    outstream->sample_rate = caprate;
    outstream->software_latency = 0.0;
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

    printf("==== Audio init finished: %d ====\n", init_audio_result);
    
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

