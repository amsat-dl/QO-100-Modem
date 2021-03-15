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
*/


#include "hsmodem.h"

void bc_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock);
void appdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock);
void startModem();

// threads will exit if set to 0
int keeprunning = 1;

// UDP I/O
int BC_sock_AppToModem = -1;
int DATA_sock_AppToModem = -1;
int DATA_sock_ExtToModem = -1;

int UdpBCport_AppToModem =      40131;  // broadcast messages from GUI
int UdpDataPort_AppToModem =    40132;  // data  messages from GUI
int UdpDataPort_ModemToApp =    40133;  // all messages to GUI
int TcpDataPort_WebSocket =     40134;  // web socket data exchange to local browser
int UdpDataPort_ExtWebdata =    40135;  // get data from ext. application to sent via modem

// op mode depending values
// default mode if not set by the app
int speedmode = 4;
int set_speedmode = 4; 
int bitsPerSymbol = 2;      // QPSK=2, 8PSK=3
int constellationSize = 4;  // QPSK=4, 8PSK=8

char localIP[] = { "127.0.0.1" };
char ownfilename[] = { "hsmodem" };
char appIP[20] = { 0 };
int fixappIP = 0;
int restart_modems = 0;
int init_voice = 0;
int trigger_resetmodem = 0;
char homepath[1000] = { 0 };

int caprate = 44100;
int txinterpolfactor = 20;
int rxPreInterpolfactor = 5;
int linespeed = 4410;

char captureDeviceName[101] = { 0 };
char playbackDeviceName[101] = { 0 };
char micDeviceName[101] = { 0 };
char lsDeviceName[101] = { 0 };

int announcement = 0;
int VoiceAudioMode = VOICEMODE_OFF;
int codec = 1;  // 0=opus, 1=codec2
int tuning = 0;
int marker = 1;

int init_voice_result = 0;

// number of audio device in libkmaudio
int io_capidx = -1;
int io_pbidx = -1;
int voice_capidx = -1;
int voice_pbidx = -1;

int sendIntro = 0;
int extData_active = 0;

char mycallsign[21];
char myqthloc[11];
char myname[21];

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

// get user home path
#ifdef _WIN32_
    strcpy(homepath, getenv("USERPROFILE"));

    char nd[1000];
    sprintf(nd, "%s/oscardata", homepath);
    if (CreateDirectory(nd, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        sprintf(nd, "%s\\oscardata\\intro", homepath);
        CreateDirectory(nd, NULL);
    }
#endif
#ifdef _LINUX_
    char* ph;

    if ((ph = getenv("HOME")) == NULL) 
    {
        ph = getpwuid(getuid())->pw_dir;
    }
    if (ph != NULL)
        strcpy(homepath, ph);
    else
        *homepath = 0;

    struct stat st = { 0 };

    char nd[1100];
    sprintf(nd, "%s/oscardata", homepath);
    if (stat(nd, &st) == -1) 
    {
        mkdir(nd, 0755);
        sprintf(nd, "%s/oscardata/intro", homepath);
        if (stat(nd, &st) == -1)
            mkdir(nd, 0755);
    }
#endif
    printf("user home path:<%s>\n", homepath);

    init_fifos();           // init fifos for PSK data and RTTY characters
    init_distributor();     // init distribution process for PSK data
    init_tune();            // init tuning tones (mixed to signal)
    kmaudio_init();         // init soundcard driver
    kmaudio_getDeviceList();// get sound devices
    init_packer();          // init PSK packer/unpacker
    initFEC();              // init FEC calculator
    ws_init();              // init Websocket

    // start udp RX to listen for broadcast search message from Application
    UdpRxInit(&BC_sock_AppToModem, UdpBCport_AppToModem, &bc_rxdata, &keeprunning);

    // start udp RX for data from application
    UdpRxInit(&DATA_sock_AppToModem, UdpDataPort_AppToModem, &appdata_rxdata, &keeprunning);

    // start udp RX to listen for data from external program
    // these data will be sent via QO100 (i.e.: to the receiver's websocket)
    UdpRxInit(&DATA_sock_ExtToModem, UdpDataPort_ExtWebdata, &ext_rxdata, &keeprunning);

    printf("QO100modem initialised and running\n");

    while (keeprunning)
    {
        int wait = 1;
        
        if (restart_modems == 1)
        {
            printf("restart modem requested\n");
            startModem();
            restart_modems = 0;
        }

        if (init_voice == 1)
        {
            initVoice();
            init_voice = 0;
        }

        //doArraySend();
        if (VoiceAudioMode == VOICEMODE_INTERNALLOOP)
        {
            // loop voice mic to LS
            float f[1100]; // 1.1 x need rate to have reserve for resampler
            int anz = kmaudio_readsamples(voice_capidx, f, 1000, micvol, 0);
            if (anz > 0)
                kmaudio_playsamples(voice_pbidx, f, anz,lsvol);
        }

        if (VoiceAudioMode == VOICEMODE_RECORD)
        {
            // loop voice mic to LS, and record into PCM file
            float f[1100];
            while (keeprunning)
            {
                int anz = kmaudio_readsamples(voice_capidx, f, 1000, micvol,0);
                if (anz > 0)
                {
                    io_saveStream(f, anz);
                    kmaudio_playsamples(voice_pbidx, f, anz,lsvol);
                }
                else
                    break;
            }
        }

        if (VoiceAudioMode == VOICEMODE_PLAYBACK)
        {
            playIntro();
            VoiceAudioMode = VOICEMODE_OFF;
        }

        if (VoiceAudioMode == VOICEMODE_CODECLOOP || VoiceAudioMode == VOICEMODE_DV_FULLDUPLEX)
        {
            // send mic to codec
            float f;
            while(kmaudio_readsamples(voice_capidx, &f, 1, micvol,0))
            //while (io_mic_read_fifo(&f))
            {
                encode(f);
            }
        }

        if (tuning != 0)
        {
            do_tuning(tuning);
            wait = 0;
        }
        
        if (speedmode == 10)
        {
            // nothing to do here
            //testall();
            //fmtest();
        }
        else
        {
#ifdef _LINUX_
            /*static uint64_t old_tm = 0;
            uint64_t tm = getms();
            if (tm >= (old_tm + 1000))
            {
                // read Audio device list every 1s
                // runtime dectection currently works under linux only
                kmaudio_getDeviceList();
                old_tm = tm;
            }*/
#endif

            // demodulate incoming audio data stream
            int dret = demodulator();
            if (dret) wait = 0;
        }
        if (wait) sleep_ms(10);
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
#define NUMSPEEDMODES 11
SPEEDRATE sr[NUMSPEEDMODES] = {
    // BPSK modes
    {48000, 40,10, 1, 1200, 800},
    {48000, 20, 5, 1, 2400, 2000},

    // QPSK modes
    {48000, 32, 8, 2, 3000, 2400}, 
    {48000, 24, 6, 2, 4000, 3200},
    {44100, 20, 5, 2, 4410, 3600},
    {48000, 20, 5, 2, 4800, 4000},

    // 8PSK modes
    {44100, 24, 6, 3, 5500, 4400},
    {48000, 24, 6, 3, 6000, 4800},
    {44100, 20, 5, 3, 6600, 5200},
    {48000, 20, 5, 3, 7200, 6000},

    // RTTY
    {48000, 0, 0, 0, 0, 0},
};

void startModem()
{
    printf("startModem. Speedmode:%d\n",set_speedmode);

    close_rtty();
    fifo_clear(PSK_GUI_TX);
    close_dsp();
    speedmode = set_speedmode;
    if (speedmode < 0 || speedmode >= NUMSPEEDMODES)
        speedmode = 4;

    bitsPerSymbol = sr[speedmode].bpsym;
    constellationSize = (1 << bitsPerSymbol); // QPSK=4, 8PSK=8

    caprate = sr[speedmode].audio;
    txinterpolfactor = sr[speedmode].tx;
    rxPreInterpolfactor = sr[speedmode].rx;
    linespeed = sr[speedmode].linespeed;
    opusbitrate = sr[speedmode].codecrate;

    _init_fft();
    if (speedmode < 10)
    {
        init_dsp();
    }
    if (speedmode == 10)
    {
        rtty_txoff = 1;
        init_rtty();
    }

    // int TX audio and modulator
    io_capidx = kmaudio_startCapture(captureDeviceName, caprate);
    if (io_capidx == -1)
    {
        printf("CAP: cannot open device: %s\n", captureDeviceName);
        return;
    }
    
    io_pbidx = kmaudio_startPlayback(playbackDeviceName, caprate);
    if (io_pbidx == -1)
    {
        printf("PB: cannot open device: %s\n", playbackDeviceName);
        return;
    }
    
    init_tune();
}

void initVoice()
{
    // init voice audio
    if (VoiceAudioMode == VOICEMODE_OFF)
    {
        float f = 0.0f;
        io_saveStream(&f, 1);    // close recording
        close_voiceproc();
        close_stream(voice_capidx);
        close_stream(voice_pbidx);
    }
    else
    {
        int srate = VOICE_SAMPRATE;

        // voice always runs with 48000 with one exception:
        // if it is used for monitoring only and the digital audio
        // stream runs with 44100, then also the monitoring voice audio
        // must runs with 44100
        if (VoiceAudioMode == VOICEMODE_LISTENAUDIOIN && caprate == 44100)
            srate = 44100;

        voice_capidx = kmaudio_startCapture(micDeviceName, srate);
        if (voice_capidx == -1)
        {
            printf("Voice CAP: cannot open device: %s\n", micDeviceName);
            return;
        }
        voice_pbidx = kmaudio_startPlayback(lsDeviceName, srate);
        if (voice_pbidx == -1)
        {
            printf("Voice PB: cannot open device: %s\n", lsDeviceName);
            return;
        }
        init_voiceproc();
    }
}

// called from UDP callback
void setSpeedmode(int spm)
{
    printf("set speedmode:%d\n", spm);

    set_speedmode = spm;
    restart_modems = 1;
    transmissions = 1000;   // announcement at next TX
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

uint8_t *getDevList(int* plen)
{
    uint8_t* txdata = io_getAudioDevicelist(plen);
    txdata[0] = 3;  // ID of this UDP message

    txdata[1] = (io_capidx != -1 && devlist[io_capidx].working) ? '1' : '0';
    txdata[2] = (io_pbidx != -1 && devlist[io_pbidx].working) ? '1' : '0';
    txdata[3] = (voice_capidx != -1 && devlist[voice_capidx].working) ? '1' : '0';
    txdata[4] = (voice_pbidx != -1 && devlist[voice_pbidx].working) ? '1' : '0';
    
    return txdata;
}

// called from UDP RX thread for Broadcast-search from App
void bc_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{

    static uint64_t lastms = 0;  // time of last received BC message
    uint64_t actms = getms();


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
        * 6 ... safe mode number
        * 7 ... send Intro
        * 8 ... rtty autosync
        * 9 ... hsmodem speed mode
        * 10 .. external data IF on/off
        * 11-19 ... unused
        * 20 .. 119 ... PB device name
        * 120 .. 219 ... CAP device name
        * 220 .. 239 ... Callsign
        * 230 .. 249 ... qthloc
        * 250 .. 269 ... Name
        */

        char rxip[20];
        strcpy(rxip, inet_ntoa(rxsock->sin_addr));

        //printf("GUI search received:%s\n",rxip);

        if (fixappIP == 0)
        {
            if (strcmp(appIP, rxip))
            {
                if (appIP[0] != 0)
                {
                    // there was an appIP already
                    // before accepting this new one, wait 3 seconds
                    int ts = (int)(actms - lastms);
                    //printf("new app IP: %s since %d, restarting modems\n", rxip,ts);
                    if (ts < 3000)
                        return;
                }
                //printf("first app IP: %s, restarting modems\n", rxip);
                restart_modems = 1;
            }

            strcpy(appIP, rxip);
            //printf("app (%s) is searching modem. Sending modem IP to the app\n",appIP);
            // App searches for the modem IP, mirror the received messages
            // so the app gets an UDP message with this local IP
            int alen;
            uint8_t* txdata = getDevList(&alen);
            sendUDP(appIP, UdpDataPort_ModemToApp, txdata, alen);
        }
        else
        {
            // appIP is fixed, answer only to this IP
            if (!strcmp(appIP, rxip))
            {
                //printf("app (%s) is searching modem. Sending modem IP to the app\n",appIP);
                // App searches for the modem IP, mirror the received messages
                // so the app gets an UDP message with this local IP
                int alen;
                uint8_t* txdata = getDevList(&alen);
                sendUDP(appIP, UdpDataPort_ModemToApp, txdata, alen);
            }
            else
                return;
        }

        memcpy(mycallsign, pdata + 220, sizeof(mycallsign));
        mycallsign[sizeof(mycallsign) - 1] = 0;

        memcpy(myqthloc, pdata + 240, sizeof(myqthloc));
        myqthloc[sizeof(myqthloc) - 1] = 0;

        memcpy(myname, pdata + 250, sizeof(myname));
        myname[sizeof(myname) - 1] = 0;

        if(pdata[9] != 255 && set_speedmode != pdata[9])
            setSpeedmode(pdata[9]);

        //printf("<%s> <%s> <%s>\n", mycallsign, myqthloc, myname);

        //printf("%d %d %d %d %d %d %d \n",pdata[1], pdata[2], pdata[3], pdata[4], pdata[5], pdata[6], pdata[7]);
        io_setAudioDevices(pdata[1], pdata[2], pdata[3], pdata[4], pdata[5], (char*)(pdata + 20), (char*)(pdata + 120));
        sendIntro = pdata[7];
        rtty_autosync = pdata[8];

        if (extData_active == 0 && pdata[10] == 1)
            printf("ext.Data activated\n");
        else if (extData_active == 1 && pdata[10] == 0)
            printf("ext.Data deactivated\n");

        extData_active = pdata[10];

        lastms = actms;
    }
}

// called by UDP RX thread for data from App
void appdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{
    // ignore data when modem is starting
    if (restart_modems == 1) return;

    uint8_t type = pdata[0];
    uint8_t minfo = pdata[1];

    //printf("from GUI: %d %d\n", pdata[0], pdata[1]);
    
    // type values: see oscardata config.cs: frame types

    if (type == 16)
    {
        // a bulletin file from the GUI
        // has to be sent to webbrowsers via websocket
        //printf("Bulletin contents:\n<%s>\n", pdata + 1);
        // the first byte (16) is used as the external type specifier

        ws_send(pdata, len);
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
        tuning = 0;
        resetModem();
        //io_clear_audio_fifos();
        io_fifo_clear(voice_pbidx);
        io_fifo_clear(voice_capidx);
        return;
    }

    if (type == 21)
    {
        // set playback volume (in % 0..100)
        io_setPBvolume(minfo);
        return;
    }

    if (type == 22)
    {
        // set capture volume (in % 0..100)
        io_setCAPvolume(minfo);
        return;
    }

    if (type == 23)
    {
        // set playback volume (in % 0..100)
        io_setLSvolume(minfo);
        return;
    }

    if (type == 24)
    {
        // set capture volume (in % 0..100)
        io_setMICvolume(minfo);
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

        //printf("LS:<%s> MIC:<%s> Mode:%d codec:%d\n", lsDeviceName, micDeviceName, VoiceAudioMode, codec);

        init_voice = 1;

        if (!strcmp(micDeviceName, captureDeviceName))
        {
            printf("capture device already in use, ignoring: %s\n", micDeviceName);
            init_voice = 0;
        }

        if (!strcmp(lsDeviceName, playbackDeviceName))
        {
            printf("playback device already in use, ignoring: %s\n", lsDeviceName);
            init_voice = 0;
        }
        return;
    }

    if (type == 26)
    {
        // GUI requests termination of this hsmodem
        printf("shut down hsmodem\n");
        closeAllandTerminate();
        return;
    }

    if (type == 27)
    {
        // send Tuning tones
        printf("Tuning mode active:%d\n",minfo);
        tuning_runtime = 0;
        tuning = minfo;
        return;
    }

    if (type == 28)
    {
        printf("marker:%d\n",minfo);
        marker = minfo;
        return;
    }

    if (speedmode == 10)
    {
        // rtty commands
        if (type == 29)
        {
            int16_t freq = pdata[1];
            freq <<= 8;
            freq += pdata[2];
            printf("set freq:%d\n", freq);

            rtty_modifyRXfreq(freq);
            return;
        }

        if (type == 30)
        {
            // rtty key pressed
            write_fifo(FIFO_RTTYTX,&minfo,1);
            return;
        }

        if (type == 31)
        {
            // rtty string
            int len = pdata[1];
            len <<= 8;
            len += pdata[2];
            len++; // the first toTX command
            //printf("hsmodem.cpp rtty_tx_write_fifo: ");
            write_fifo(FIFO_RTTYTX, pdata+3,len);

            /*for (int i = 0; i < len; i++)
            {
                //printf("%c", pdata[3 + i]);
                write_fifo(FIFO_RTTYTX, pdata[3 + i]);
            }*/
            //printf("\n");
            return;
        }

        if (type == 32)
        {
            // TX on/off, but send buffer
            rtty_txoff = minfo?0:1;
            return;
        }

        if (type == 33)
        {
            // stop TX immediately
            rtty_txoff = 1;
            fifo_clear(FIFO_RTTYTX);
        }
    }
    if (type >= 29 && type <= 32) return;

    if (speedmode == 10) return;


    // here we are with payload data to be sent via the modulator

    if (len != (PAYLOADLEN + 2))
    {
        printf("data from app: wrong length:%d (should be %d)\n", len - 2, PAYLOADLEN);
        return;
    }
    
    //if (getSending() == 1) return;   // already sending (Array sending)

    // send a payload
    if (minfo == 0 || minfo == 3)
    {
        // this is the first frame of a larger file
        sendAnnouncement();
        toGR_sendData(pdata + 2, type, minfo, 5); // repeat the first frame a couple of times
        sendStationInfo();
    }
    else if ((len - 2) < PAYLOADLEN)
    {
        // if not enough data for a full payload add Zeros 
        uint8_t payload[PAYLOADLEN];
        memset(payload, 0, PAYLOADLEN);
        memcpy(payload, pdata + 2, len - 2);
        
        if (minfo == 2) // if its the last frame, repeate a couple of times
            toGR_sendData(payload, type, minfo, 5);
        else
            toGR_sendData(payload, type, minfo, 0); // send only once
    }
    else
    {
        // normal sending: continous or last frame
        if (minfo == 2) // if its the last frame, repeate a couple of times
            toGR_sendData(pdata + 2, type, minfo, 5);
        else
            toGR_sendData(pdata + 2, type, minfo, 0);
    }
}

// pack and send PSK data
void toGR_sendData(uint8_t* data, int type, int status, int repeat)
{
    modem_sendPSKData(data, type, status, repeat, PSK_GUI_TX);
}

// pack and send PSK data
// handle repetitions and check if TX was down
// repeat: 0=do not repeat, 1=repeat if currently not sending, >1 = number of repetitions
void modem_sendPSKData(uint8_t* data, int type, int status, int repeat, int fifoID)
{
    // send the first frame normal (with a new frame counter value)
    int len = 0;
    uint8_t* txdata = Pack(data, type, status, &len, 0);
    if (txdata != NULL)
    {
        sendPSKdata(txdata, len, fifoID);
    }
    if (repeat == 0) return;

    // now check if repetitions are required
    if (bitsPerSymbol == 0 || txinterpolfactor == 0) return; // just for security, no useful function
    int repetitions = 6 * ((caprate / txinterpolfactor) * bitsPerSymbol / 8) / 258 + 1;

    if (isPlaying(io_pbidx) == 0) // if not sending, repeat frame
    {
        if (repeat == 1)
            repeat = repetitions;
        else if (repeat > 1)
        {
            // if TX was down, do at least "repetitions" repetitions
            if (repeat < repetitions) repeat = repetitions;
        }
    }

    // and the rest repeated if requested
    txdata = Pack(data, type, status, &len, 1);
    for (int i = 0; i < repeat; i++)
    {
        if (txdata != NULL) sendPSKdata(txdata, len, fifoID);
    }
}

void sendStationInfo()
{
    uint8_t payload[PAYLOADLEN];
    memcpy(payload, mycallsign, 20);
    memcpy(payload+20, myqthloc, 10);
    memcpy(payload+30, myname, 20);

    int len = 0;
    uint8_t* txdata = Pack(payload, 7, 1, &len, 1);

    //showbytestring((char *)"TX Userinfo: ", txdata, len);

    for (int i = 0; i < 2; i++)
    {
        if (txdata != NULL) sendPSKdata(txdata, len, PSK_GUI_TX);
    }
}

// called by liquid demodulator for received data
void GRdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock)
{
    static int64_t lasttime = -1;
    static uint64_t triggertime = 0;

    // raw symbols
    uint8_t* pl = unpack_data(pdata, len);
    if (pl != NULL)
    {
        // complete frame received
        //printf("type:%d\n", pl[0]);

        if (pl[0] == 8)
        {
            // external data received
            ext_modemRX(pl);
        }

        // send payload to app
        uint8_t txpl[PAYLOADLEN + 10 + 1];
        memcpy(txpl + 1, pl, PAYLOADLEN + 10);
        txpl[0] = 1;    // type 1: payload data follows
        sendUDP(appIP, UdpDataPort_ModemToApp, txpl, PAYLOADLEN + 10 + 1);

        if (VoiceAudioMode == VOICEMODE_DV_FULLDUPLEX)
        {
            // send to Codec decoder
            if (*(pl + 3) != 0) // minfo=0 ... just a filler, ignore
                toCodecDecoder(pl + 10, PAYLOADLEN);
        }
        trigger_resetmodem = 0;
        rx_in_sync = 1;
        lasttime = getms();
    }
    else
    {
        // no frame found
        // if longer ws seconds nothing found, reset liquid RX modem
        // comes here with symbol rate, i.e. 4000 S/s
        int bps = sr[speedmode].linespeed;
        // time for one frame [ms]
        int frmlen = UDPBLOCKLEN * 8;
        int tmfrm_ms = (frmlen * 1000) / bps;   // ms for one frame
        
        uint64_t acttm = getms();
        if (lasttime == -1)
        {
            lasttime = acttm;
            return;
        }
        int tdiff = (int)(acttm - lasttime); // elapsed time in ms
        int elapsed_frames = tdiff / tmfrm_ms;
        //if((tdiff%1000)==0) printf("elapsed time:%d  frames:%d\n", tdiff, elapsed_frames);

        if (trigger_resetmodem == 1)
        {
            // reset requested by FFT level detector
            trigger_resetmodem = 2;
            //printf("set triggertime\n");
            triggertime = acttm;
        }

        if ((acttm - triggertime) > 1000 && trigger_resetmodem == 2)
        {
            // reset rx 1 second after level detection
            //printf("reset RX modem, 1s since signal detection\n");
            trigger_resetmodem = 0;
            rx_in_sync = 0;
            resetModem();
            lasttime = acttm;
        }

        if (tdiff > 5000)
        {
            // in any case, only every 5s or longer
            //printf("5s elapsed\n");
            if (elapsed_frames > 2)
            {
                // reset modem if more than 2 frames have not been received
                trigger_resetmodem = 0;
                rx_in_sync = 0;
                if (speedmode < 10)
                {
                    //printf("no signal detected, reset RX modem\n");
                    resetModem();
                }
                lasttime = acttm;
            }
        }

        if (elapsed_frames > 5)
        {
            // if > 5 frames not recieved, mark "not in sync"
            rx_in_sync = 0;
        }
    }
}
