/*
* High Speed modem to transfer data in a 2,7kHz SSB channel
* =========================================================
* Author: DJ0ABR
* made for: AMSAT-DL
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
*/

/*
* this is a console program
* it can be compiled under Linux: make
* and under Windows: Visual-Studio
* 
* !!! compile x86 (32bit) version !!! all supplied dlls are 32 bit !!!
* ====================================================================
* 
* 3rd party libraries:
* 1) BASS Audio from https://www.un4seen.com/
	copy bass.h and bass.lib into source directory
	Windows: copy bass.dll into executable directory
	Linux: copy libbass.so into shared-lib folder, usually /usr/local/lib
	! NOTE: for PC-Linux and ARM-Linux you need different libraries !

2) liquid-DSP
	Linux Install Script:
		this installs it from source

		sudo apt install git autoconf libsndfile-dev libasound-dev
		git clone git://github.com/jgaeddert/liquid-dsp.git
		cd liquid-dsp
		./bootstrap.sh
		./configure
		make -j 8
		sudo make install
		sudo ldconfig

		a working copy of the source code is in ../3rdParty/liquid-dsp
		to use this source simply remove the "git clone" line from above script
		it installs libliquid.so into /usr/local/lib (Ubuntu) and
		liquid.h into /usr/local/include/liquid/

	Windows:
		ready libraries are in ../3rdParty/liquid-dsp-windows
		copy liquid.h and liquid.lib into source directory
		copy liquid.dll into executable directory
*/


#include "hsmodem.h"

void toGR_sendData(uint8_t* data, int type, int status);
void bc_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock);
void appdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock);
void startModem();

// threads will exit if set to 0
int keeprunning = 1;

// UDP I/O
int BC_sock_AppToModem = -1;
int DATA_sock_AppToModem = -1;
int DATA_sock_FFT_from_GR = -1;
int DATA_sock_I_Q_from_GR = -1;

int UdpBCport_AppToModem = 40131;
int UdpDataPort_AppToModem = 40132;
int UdpDataPort_ModemToApp = 40133;

// op mode depending values
// default mode if not set by the app
int speedmode = 2;
int bitsPerSymbol = 2;      // QPSK=2, 8PSK=3
int constellationSize = 4;  // QPSK=4, 8PSK=8

char localIP[] = { "127.0.0.1" };
char ownfilename[] = { "hsmodem" };
char appIP[20] = { 0 };
int fixappIP = 0;
int restart_modems = 0;
int trigger_resetmodem = 0;

int caprate = 44100;
int txinterpolfactor = 20;
int rxPreInterpolfactor = 5;
int linespeed = 4410;

char captureDeviceName[101] = { 0 };
char playbackDeviceName[101] = { 0 };
char micDeviceName[101] = { 0 };
char lsDeviceName[101] = { 0 };

float softwareCAPvolume = 1;
float softwarePBvolume = 1;
float softwareMICvolume = 1;
float softwareLSvolume = 1;

int announcement = 0;
int VoiceAudioMode = VOICEMODE_OFF;
int codec = 1;  // 0=opus, 1=codec2

int init_audio_result = 0;
int init_voice_result = 0;

int main(int argc, char* argv[])
{
    int opt = 0;
    char* modemip = NULL;

#ifdef _LINUX_
    while ((opt = getopt(argc, argv, "m:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            // specify IP of application: hsmodem -m 192.168.0.1
            modemip = optarg;
            memset(appIP, 0, 20);
            int len = strlen(modemip);
            if (len < 16)
            {
                memcpy(appIP, modemip, len);
                fixappIP = 1;
                printf("Application IP set to: %s\n", modemip);
            }
            else
            {
                printf("invalid Application IP: %s\n", modemip);
                exit(0);
            }
            break;
        }
    }

    if (isRunning(ownfilename) == 1)
        exit(0);

    install_signal_handler();
#endif

#ifdef _WIN32_
    if (argc != 1 && argc != 3)
    {
        printf("invalid argument\n");
        exit(0);
    }
    if (argc == 3)
    {
        memset(appIP, 0, 20);
        int len = strlen(argv[2]);
        if (len < 16)
        {
            memcpy(appIP, argv[2], len);
            fixappIP = 1;
            printf("Application IP set to: %s\n", argv[2]);
        }
        else
        {
            printf("invalid Application IP: %s\n", modemip);
            exit(0);
        }
    }
#endif
    
    init_packer();
    initFEC();
    init_fft();

    // start udp RX to listen for broadcast search message from Application
    UdpRxInit(&BC_sock_AppToModem, UdpBCport_AppToModem, &bc_rxdata, &keeprunning);

    // start udp RX for data from application
    UdpRxInit(&DATA_sock_AppToModem, UdpDataPort_AppToModem, &appdata_rxdata, &keeprunning);

    printf("QO100modem initialised and running\n");

    while (keeprunning)
    {
        if (restart_modems == 1)
        {
            startModem();
            restart_modems = 0;
        }
        
        //doArraySend();
        if (VoiceAudioMode == VOICEMODE_INTERNALLOOP)
        {
            // loop voice mic to LS
            float f;
            if (io_mic_read_fifo(&f))
            {
                 io_ls_write_fifo(f);
            }
        }

        if (VoiceAudioMode == VOICEMODE_CODECLOOP || VoiceAudioMode == VOICEMODE_DV_FULLDUPLEX)
        {
            // send mic to codec
            float f;
            if (io_mic_read_fifo(&f))
            {
                encode(f);
            }
        }
        
        // demodulate incoming audio data stream
        static int old_tm = 0;
        int tm = getus();
        if (tm >= (old_tm + 1000000))
        {
            // read Audio device list every 1s
            io_readAudioDevices();
            old_tm = tm;
        }
        int dret = demodulator();
        if (dret == 0)
        {
            // no new data in fifo
            // not important how long to sleep, 10ms is fine
#ifdef _LINUX_
            sleep_ms(10);   
#endif
        }
    }
    printf("stopped: %d\n", keeprunning);

#ifdef _LINUX_
    close(BC_sock_AppToModem);
#endif
#ifdef _WIN32_
    closesocket(BC_sock_AppToModem);
#endif


    return 0;
}


typedef struct {
    int audio;
    int tx;
    int rx;
    int bpsym;
    int linespeed;
    int codecrate;
} SPEEDRATE;

// AudioRate, TX-Resampler, RX-Resampler/4, bit/symbol, Codec-Rate
SPEEDRATE sr[8] = {
    // QPSK modes
    {48000, 32, 8, 2, 3000, 2400}, 
    {48000, 24, 6, 2, 4000, 3200},
    {44100, 20, 5, 2, 4410, 3600},
    {48000, 20, 5, 2, 4800, 4000},

    // 8PSK modes
    {44100, 24, 6, 3, 5500, 4400},
    {48000, 24, 6, 3, 6000, 4800},
    {44100, 20, 5, 3, 6600, 5200},
    {48000, 20, 5, 3, 7200, 6000}
};

void startModem()
{
    bitsPerSymbol = sr[speedmode].bpsym;
    constellationSize = (1 << bitsPerSymbol); // QPSK=4, 8PSK=8

    caprate = sr[speedmode].audio;
    txinterpolfactor = sr[speedmode].tx;
    rxPreInterpolfactor = sr[speedmode].rx;
    linespeed = sr[speedmode].linespeed;
    opusbitrate = sr[speedmode].codecrate;

    // int TX audio and modulator
    close_dsp();
    init_audio_result = io_init_sound(playbackDeviceName, captureDeviceName);
    init_dsp();
}

void io_setAudioDevices(uint8_t pbvol, uint8_t capvol, uint8_t announce, uint8_t pbls, uint8_t pbmic, char *pbname, char*capname)
{
    io_setPBvolume(pbvol);
    io_setCAPvolume(capvol);
    io_setLSvolume(pbls);
    io_setMICvolume(pbmic);

    announcement = announce;

    if (strcmp(pbname, playbackDeviceName) || strcmp(captureDeviceName,capname))
    {
        restart_modems = 1;

        snprintf(playbackDeviceName, 100, "%s", pbname);
        playbackDeviceName[99] = 0;

        snprintf(captureDeviceName, 100, "%s", capname);
        captureDeviceName[99] = 0;
    }
}

// called from UDP RX thread for Broadcast-search from App
void bc_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{
    if (len > 0 && pdata[0] == 0x3c)
    {
        /* searchmodem message received
        * Format:
        * Byte :
        * 0 ... 0x3c
        * 1 ... PB volume
        * 2 ... CAP volume
        * 3 ... announcement on / off, duration
        * 4 ... DV loudspeaker volume
        * 5 ... DV mic volume
        * 6..9 ... unused
        * 10 .. 109 ... PB device name
        * 110 .. 209 ... CAP device name
        */

        //printf("%d %d %d %d %d %d %d \n",pdata[1], pdata[2], pdata[3], pdata[4], pdata[5], pdata[6], pdata[7]);
        io_setAudioDevices(pdata[1], pdata[2], pdata[3], pdata[4], pdata[5], (char *)(pdata + 10), (char *)(pdata + 110));

        char rxip[20];
        strcpy(rxip, inet_ntoa(rxsock->sin_addr));

        if (fixappIP == 0)
        {
            if (strcmp(appIP, rxip))
            {
                printf("new app IP: %s, restarting modems\n", rxip);
                restart_modems = 1;
            }
            strcpy(appIP, rxip);
            //printf("app (%s) is searching modem. Sending modem IP to the app\n",appIP);
            // App searches for the modem IP, mirror the received messages
            // so the app gets an UDP message with this local IP
            int alen;
            uint8_t* txdata = io_getAudioDevicelist(&alen);
            sendUDP(appIP, UdpDataPort_ModemToApp, txdata, alen);
        }
        else
        {
            // appIP is fixed, answer only to this IP
            if (!strcmp(appIP, rxip))
            {
                //printf("app (%s) is searching modem. Sending modem IP to the app\n",appIP);
                restart_modems = 1;
                // App searches for the modem IP, mirror the received messages
                // so the app gets an UDP message with this local IP
                int alen;
                uint8_t* txdata = io_getAudioDevicelist(&alen);
                sendUDP(appIP, UdpDataPort_ModemToApp, txdata, alen);
            }
        }
    }
}

// called by UDP RX thread for data from App
void appdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{
    uint8_t type = pdata[0];
    uint8_t minfo = pdata[1];

    // type values: see oscardata config.cs: frame types
    if (type == 16)
    {
        // Byte 1 contains the resampler ratio for TX and RX modem
        if (pdata[1] >= 8)
        {
            printf("wrong speedmode %d, ignoring\n", pdata[1]);
            return;
        }
        speedmode = pdata[1];
        printf("set speedmode to %d\n", speedmode);
        restart_modems = 1;
        transmissions = 1000;   // announcement at next TX
        return;
    }

    if (type == 17)
    {
        // auto send file
        // TODO

        // for testing only:
        // simulate sending a text file with 1kB length
        /*int testlen = 100000;
        uint8_t arr[100000];
        char c = 'A';
        for (int i = 0; i < testlen; i++)
        {
            arr[i] = c;
            if (++c > 'Z') c = 'A';
        }
        arraySend(arr, testlen, 3, (char*)"testfile.txt");*/
        return;
    }
    if (type == 18)
    {
        // auto send folder
        // TODO
    }

    if (type == 19)
    {
        // shut down this modem PC
#ifdef _LINUX_
        int r = system("sudo shutdown now");
        exit(r);
#endif
    }

    if (type == 20)
    {
        // reset liquid RX modem
        resetModem();
        return;
    }

    if (type == 21)
    {
        // set playback volume (in % 0..100)
        io_setVolume(0,minfo);
        return;
    }

    if (type == 22)
    {
        // set capture volume (in % 0..100)
        io_setVolume(1,minfo);
        return;
    }

    if (type == 23)
    {
        // set playback volume (in % 0..100)
        setVolume_voice(0, minfo);
        return;
    }

    if (type == 24)
    {
        // set capture volume (in % 0..100)
        setVolume_voice(1, minfo);
        return;
    }

    if (type == 25)
    {
        /*
        * Format:
        * 0 ... statics.SetVoiceMode (25)
        * 1 ... voicemode
        * 2 ... codec
        * 3-102 ... LS device name
        * 103-202 ... MIC device name
        */
        memcpy(lsDeviceName, pdata + 3, 100);
        lsDeviceName[99] = 0;
        memcpy(micDeviceName, pdata + 103, 100);
        micDeviceName[99] = 0;

        VoiceAudioMode = pdata[1];
        codec = pdata[2];

        printf("LS:<%s> MIC:<%s> Mode:%d codec:%d\n", lsDeviceName, micDeviceName, VoiceAudioMode, codec);

        // init voice audio
        if (VoiceAudioMode == 0)
        {
            close_voiceproc();
            io_close_voice();
        }
        else
        {
            init_voice_result = io_init_voice(lsDeviceName, micDeviceName);
            init_voiceproc();
        }
        return;
    }

    if (type == 26)
    {
        // GUI requests termination of this hsmodem
        printf("shut down hsmodem\n");
        closeAllandTerminate();
    }

    if (len != (PAYLOADLEN + 2))
    {
        printf("data from app: wrong length:%d (should be %d)\n", len - 2, PAYLOADLEN);
        return;
    }


    //if (getSending() == 1) return;   // already sending (Array sending)

    if (minfo == 0 || minfo == 3)
    {
        // this is the first frame of a larger file
        sendAnnouncement();
        // send first frame multiple times, like a preamble, to give the
        // receiver some time for synchronisation
        // caprate: samples/s. This are symbols: caprate/txinterpolfactor
        // and bits: symbols * bitsPerSymbol
        // and bytes/second: bits/8 = (caprate/txinterpolfactor) * bitsPerSymbol / 8
        // one frame has 258 bytes, so we need for 6s: 6* ((caprate/txinterpolfactor) * bitsPerSymbol / 8) /258 + 1 frames
        int numframespreamble = 6 * ((caprate / txinterpolfactor) * bitsPerSymbol / 8) / 258 + 1;
        for (int i = 0; i < numframespreamble; i++)
            toGR_sendData(pdata + 2, type, minfo);
    }
    else if ((len - 2) < PAYLOADLEN)
    {
        // if not enough data for a full payload add Zeros 
        uint8_t payload[PAYLOADLEN];
        memset(payload, 0, PAYLOADLEN);
        memcpy(payload, pdata + 2, len - 2);
        toGR_sendData(payload, type, minfo);
    }
    else
    {
        toGR_sendData(pdata + 2, type, minfo);
    }
}

void toGR_sendData(uint8_t* data, int type, int status)
{
    int len = 0;
    uint8_t* txdata = Pack(data, type, status, &len);

    //showbytestring((char *)"BERtx: ", txdata, len);

    if (txdata != NULL)
        sendToModulator(txdata, len);
}

// called by liquid demodulator for received data
void GRdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{
    static int fnd = 0;

    // raw symbols
    uint8_t* pl = unpack_data(pdata, len);
    if (pl != NULL)
    {
        if (VoiceAudioMode != VOICEMODE_DV_FULLDUPLEX)
        {
            // complete frame received
            // send payload to app
            uint8_t txpl[PAYLOADLEN + 10 + 1];
            memcpy(txpl + 1, pl, PAYLOADLEN + 10);
            txpl[0] = 1;    // type 1: payload data follows
            sendUDP(appIP, UdpDataPort_ModemToApp, txpl, PAYLOADLEN + 10 + 1);
        }
        else
        {
            // send to Codec decoder
            if (*(pl + 3) != 0) // minfo=0 ... just a filler, ignore
                toCodecDecoder(pl + 10, PAYLOADLEN);
        }
        fnd = 0;
        trigger_resetmodem = 0;
        rx_in_sync = 1;
    }
    else
    {
        // no frame found
        // if longer ws seconds nothing found, reset liquid RX modem
        // comes here with symbol rate, i.e. 4000 S/s
        int ws = 5; 
        int wt = sr[speedmode].audio / sr[speedmode].tx;
        if (++fnd >= (wt * ws) || trigger_resetmodem)
        {
            fnd = 0;
            trigger_resetmodem = 0;
            rx_in_sync = 0;
            printf("no signal detected %d, reset RX modem\n", wt);
            resetModem();
        }
        else if (fnd >= wt)
        {
            rx_in_sync = 0;
        }
    }
}
