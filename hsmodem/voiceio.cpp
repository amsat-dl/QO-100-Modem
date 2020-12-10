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
* voiceio.c ... interface to libsoundio für DV audio
*
*/

#include "hsmodem.h"

struct SoundIo* voice_soundio = NULL;
struct SoundIoDevice* io_ls_device = NULL;
struct SoundIoDevice* io_mic_device = NULL;
struct SoundIoOutStream* outlsstream = NULL;
struct SoundIoInStream* inmicstream = NULL;

void read_voicecallback(struct SoundIoInStream* instream, int frame_count_min, int frame_count_max)
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
                    io_mic_write_fifo(f);
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

void overflow_voicecallback(struct SoundIoInStream* instream)
{
    static int count = 0;
    printf("voice overflow %d\n", ++count);
}

void write_sample_s16ne(char* ptr, double sample) {
    int16_t* buf = (int16_t*)ptr;
    double range = (double)INT16_MAX - (double)INT16_MIN;
    double val = sample * range / 2.0;
    *buf = (int16_t)val;
}

#define MAXCAPCHUNKLEN  50000

static void write_voicecallback(struct SoundIoOutStream* outstream, int frame_count_min, int frame_count_max) 
{
    struct SoundIoChannelArea* areas;
    int err;
    int frames_left = frame_count_max;

    while(1) 
    {
        int frame_count = frames_left;
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) 
        {
            fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;

        // soundio_outstream_begin_write requested to write frame_count elements
        float f[MAXCAPCHUNKLEN];
        int fiforet = io_ls_read_fifo_num(f, frame_count);
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
static void write_voicecallback(struct SoundIoOutStream* outstream, int frame_count_min, int frame_count_max)
{
    float f[MAXCAPCHUNKLEN];

    int chans = outstream->layout.channel_count;
    struct SoundIoChannelArea* areas;
    int err;
    int frames_left = min_int(frame_count_max, MAXCAPCHUNKLEN);// frame_count_max;
    printf("min: %d max:%d\n", frame_count_min, frame_count_max);

    // we have to write frame_count_max, not less, or we get an underrun
    // this has to be written in chunks requested by soundio_outstream_begin_write
    
    while (1)
    {
        int frame_count = frames_left;
        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            printf("unrecoverable soundio_outstream_begin_write error: %s\n", soundio_strerror(err));
            exit(1);
        }
        printf("chunk: %d\n", frame_count);
        if (!frame_count) break; // will normally never happen

        // soundio_outstream_begin_write requested to write frame_count elements
        int fiforet = io_ls_read_fifo_num(f, frame_count);
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

        for (int frame = 0; frame < frame_count; frame += 1) 
        {
            for (int channel = 0; channel < chans; channel += 1)
            {
                write_sample_float32ne(areas[channel].ptr, f[frame]);
                areas[channel].ptr += areas[channel].step;
            }
        }

        // and finalize this chunk
        if ((err = soundio_outstream_end_write(outstream))) {
            if (err == SoundIoErrorUnderflow)
            {
                printf("underflow x\n");
                return;
            }
            printf("unrecoverable soundio_outstream_end_write error: %s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }
}
*/

void underflow_voicecallback(struct SoundIoOutStream* outstream)
{
    static int count = 0;
    printf("voice underflow %d\n", count++);
}


int io_init_voice(char* lsname, char* micname)
{
    int err;
    init_voice_result = 0;

    printf("\n ==== IO INIT VOICE devices ====\n");
    printf("requested: <%s> <%s>\ncapture rate:%d\n", lsname, micname, VOICE_SAMPRATE);

    io_close_voice();

    // prepare and connect to libsoundio
    voice_soundio = soundio_create();
    if (!voice_soundio) {
        printf("vsoundio_create: out of memory\n");
        return 0;
    }
#ifdef _WIN32_
    if ((err = soundio_connect_backend(voice_soundio, SoundIoBackendWasapi))) {
        printf("vsoundio_connect: %s\n", soundio_strerror(err));
        return 0;
    }
#endif
#ifdef _LINUX_
    if ((err = soundio_connect(voice_soundio))) {
        printf("vsoundio_connect: %s\n", soundio_strerror(err));
        return 0;
    }
#endif

    if (lsname == NULL || micname == NULL || strlen(lsname) < 3 || strlen(micname) < 3)  // no devices defined yet
    {
        printf("no devices specified\n");
        return 0;
    }

    char* lsdevid = getDevID(lsname, 1);
    if (lsdevid == NULL) return 0;

    char* micdevid = getDevID(micname, 0);
    if (micdevid == NULL) return 0;

    soundio_flush_events(voice_soundio);

    printf("================ %d\n", VoiceAudioMode);
    if (VoiceAudioMode != VOICEMODE_LISTENAUDIOIN)
    {

        // define the capture device
        printf("selected CAP device:\nname:%s\nid  :%s\n", micname, micdevid);

        for (int i = 0; i < soundio_input_device_count(voice_soundio); i++)
        {
            io_mic_device = NULL;
            struct SoundIoDevice* device = soundio_get_input_device(voice_soundio, i);
            if (strcmp(device->id, micdevid) == 0
#ifdef _WIN32_
                && device->is_raw == true
#endif
                )
            {
                io_mic_device = device;
                break;
            }
            soundio_device_unref(device);
        }

        if (!io_mic_device)
        {
            printf("Invalid device id: %s\n", micdevid);
            sleep_ms(2000);
            return 0;
        }

        if (io_mic_device->probe_error)
        {
            printf("Unable to probe voice device: %s\n", soundio_strerror(io_mic_device->probe_error));
            sleep_ms(2000);
            return 0;
        }

        printf("cap raw: %s\n", io_mic_device->is_raw ? "raw" : "---");

        // create capture callback
        inmicstream = soundio_instream_create(io_mic_device);
        if (!inmicstream) {
            printf("out of memory\n");
            return 0;
        }

        inmicstream->format = SoundIoFormatS16NE;
        inmicstream->sample_rate = VOICE_SAMPRATE;
        inmicstream->software_latency = 0.0;
        inmicstream->read_callback = read_voicecallback;
        inmicstream->overflow_callback = overflow_voicecallback;
        inmicstream->userdata = NULL;

        if ((err = soundio_instream_open(inmicstream))) {
            printf("unable to open voice input stream: %s", soundio_strerror(err));
            return 0;
        }

        if ((err = soundio_instream_start(inmicstream))) {
            fprintf(stderr, "unable to start voice input device: %s", soundio_strerror(err));
            return 0;
        }
        init_voice_result |= 2;
        // the CAP callback is running now
    }

    // define the playback device
    printf("selected PB device:\nname:%s\nid  :%s\n", lsname, lsdevid);
    for (int i = 0; i < soundio_output_device_count(voice_soundio); i++)
    {
        io_ls_device = NULL;
        struct SoundIoDevice* device = soundio_get_output_device(voice_soundio, i);
        if (strcmp(device->id, lsdevid) == 0
#ifdef _WIN32_
            && device->is_raw == true
#endif
            )
        {
            io_ls_device = device;
            break;
        }
        soundio_device_unref(device);
    }
    if (!io_ls_device)
    {
        printf("Invalid device id: %s\n", lsdevid);
        return 0;
    }

    if (io_ls_device->probe_error)
    {
        printf("Unable to probe device: %s\n", soundio_strerror(io_ls_device->probe_error));
        return 0;
    }

    printf("pb raw: %s\n", io_ls_device->is_raw ? "raw" : "---");

    // create playback callback
    outlsstream = soundio_outstream_create(io_ls_device);
    if (!outlsstream) {
        printf("soundio_outstream_create: out of memory\n");
        return 0;
    }

    outlsstream->format = SoundIoFormatS16NE;
    outlsstream->sample_rate = VOICE_SAMPRATE;
    outlsstream->software_latency = 0.0;
    outlsstream->write_callback = write_voicecallback;
    outlsstream->underflow_callback = underflow_voicecallback;
    outlsstream->userdata = NULL;

    if ((err = soundio_outstream_open(outlsstream))) {
        printf("unable to open voice output stream: %s", soundio_strerror(err));
        return 0;
    }

    if ((err = soundio_outstream_start(outlsstream))) {
        fprintf(stderr, "unable to start voice output device: %s", soundio_strerror(err));
        return 0;
    }
    init_voice_result |= 1;

    printf("==== Voice init finished: %d ====\n", init_audio_result);

    return init_voice_result;
}

void io_close_voice()
{
    printf("close Voice\n");
    if (inmicstream) soundio_instream_destroy(inmicstream);
    inmicstream = NULL;

    if (outlsstream) soundio_outstream_destroy(outlsstream);
    outlsstream = NULL;

    if (io_ls_device) soundio_device_unref(io_ls_device);
    io_ls_device = NULL;

    if (io_mic_device) soundio_device_unref(io_mic_device);
    io_mic_device = NULL;

    if (voice_soundio) soundio_destroy(voice_soundio);
    voice_soundio = NULL;

}
