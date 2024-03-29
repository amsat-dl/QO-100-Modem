#pragma once

#ifdef _WIN32
#define _WIN32_
	// ignore senseless warnings invented by M$ to confuse developers
    // I love LINUX :-) which works 100000x better than Windows
	#pragma warning( disable : 4091 )
	#pragma warning( disable : 4003 )
    #undef AMS_TESTMODE
#else
#define _LINUX_
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>

#ifdef _WIN32_
#include "Winsock2.h"
#include "io.h"
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <Shlobj.h>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "libliquid.lib")
#pragma comment(lib, "libsoundio.lib")
#pragma comment(lib, "fftw_lib/libfftw3-3.lib")
#pragma comment(lib, "opus.lib")
#pragma comment(lib, "libcodec2.lib")
#endif

#ifdef _LINUX_
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/file.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <math.h>
#include <dirent.h>
#include <ifaddrs.h>
#include <pwd.h>
#endif

#include "opus.h"
#include "liquid.h"
#include "frameformat.h"
#include "fec.h"
#include "udp.h"
#include "symboltracker.h"
#include "codec2.h"
#include "libkmaudio/soundio.h"
#include "baudot.h"
#include "fifo.h"
#include "libkmaudio/libkmaudio.h"
#include "websocket/websocketserver.h"
#include "zip/zip.h"

#define jpg_tempfilename "rxdata.jpg"

#define CRC16TX 0
#define CRC16RX 1
#define CRC16FILETX 2
#define CRC16FILERX 3

// definitions for audio
#define MAXDEVSTRLEN    5000
#define CHANNELS 1      // no of channels used

// voice audio sampling rate
#define VOICE_SAMPRATE  48000   // do NOT change, OPUS works with 48k only
#define AUDIO_SAMPRATE  48000   // do NOT change, WASAPI VACs work with 48k only

enum _VOICEMODES_ {
    VOICEMODE_OFF,
    VOICEMODE_LISTENAUDIOIN,
    VOICEMODE_INTERNALLOOP,
    VOICEMODE_CODECLOOP,
    VOICEMODE_DV_FULLDUPLEX,
    VOICEMODE_DV_RXONLY,
    VOICEMODE_RECORD,
    VOICEMODE_PLAYBACK
};

// file send definitions
#define MAXFILENUM	500
#define MAXFILESIZE	256

extern struct ws_events events;

void init_packer();
uint8_t* Pack(uint8_t* payload, int type, int status, int* plen, int repeat);
uint8_t* unpack_data(uint8_t* rxd, int len);

void convertBytesToSyms_QPSK(uint8_t* bytes, uint8_t* syms, int bytenum);
void convertBytesToSyms_8PSK(uint8_t* bytes, uint8_t* syms, int bytenum);

uint8_t* convertQPSKSymToBytes(uint8_t* rxsymbols);
uint8_t* convert8PSKSymToBytes(uint8_t* rxsymbols, int len);

uint8_t* rotateBackBPSK(uint8_t* buf, int len, int rotations);
uint8_t* convertBPSKSymToBytes(uint8_t* rxsymbols);
void convertBytesToSyms_BPSK(uint8_t* bytes, uint8_t* syms, int bytenum);
void rotateBPSKsyms(uint8_t* src, uint8_t* dst, int len);

void rotateQPSKsyms(uint8_t* src, uint8_t* dst, int len);
void rotate8PSKsyms(uint8_t* src, uint8_t* dst, int len);
void rotate8APSKsyms(uint8_t* src, uint8_t* dst, int len);

uint8_t* rotateBackQPSK(uint8_t* buf, int len, int rotations);
uint8_t* rotateBack8PSK(uint8_t* buf, int len, int rotations);
uint8_t* rotateBack8APSK(uint8_t* buf, int len, int rotations);

void TX_Scramble(uint8_t* data, int len);
uint8_t* RX_Scramble(uint8_t* data, int len);
uint16_t Crc16_messagecalc(int rxtx, uint8_t* data, int len);

void showbytestring(char* title, uint8_t* data, int totallen, int anz);
void measure_speed_syms(int len);
void measure_speed_bps(int len);

void initFEC();
void GetFEC(uint8_t* txblock, int len, uint8_t* destArray);
int cfec_Reconstruct(uint8_t* darr, uint8_t* destination);

void io_setPBvolume(int v);
void io_setCAPvolume(int v);
void io_setLSvolume(int v);
void io_setMICvolume(int v);

void sendAnnouncement();

void sleep_ms(int ms);
uint64_t getms();
void GRdata_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock);
void toGR_sendData(uint8_t* data, int type, int status, int repeat);

void modulator(uint8_t sym_in);

void init_dsp();
int demodulator();
void _sendToModulator(uint8_t* d, int len);
void resetModem();
void close_dsp();
void _init_fft();
void _exit_fft();
void showbytestringf(char* title, float* data, int totallen, int anz);
uint16_t* make_waterfall(float fre, int* retlen);
void sendPSKdata(uint8_t* data, int len, int fifoID);
void modem_sendPSKData(uint8_t* data, int type, int status, int repeat, int fifoID);

void toCodecDecoder(uint8_t* pdata, int len);

void init_voiceproc();
void encode(float f);

void init_codec2();
void encode_codec2(float f);
void toCodecDecoder_codec2(uint8_t* pdata, int len);

void closeAllandTerminate();
void close_voiceproc();
void close_codec2();


SYMTRACK* km_symtrack_cccf_create(  int _ftype,
                                    unsigned int _k,
                                    unsigned int _m,
                                    float        _beta,
                                    int          _ms);
void km_symtrack_cccf_reset(SYMTRACK*, int mode);
void km_symtrack_cccf_set_bandwidth(SYMTRACK* , float _bw);
void km_symtrack_execute(SYMTRACK* ,liquid_float_complex _x, liquid_float_complex* _y, unsigned int* _ny, unsigned int* psym_out);
void km_symtrack_cccf_destroy(SYMTRACK*);

void io_saveStream(float *fa, int anz);
void playIntro();
float do_tuning(int send);
void init_tune();
float singleFrequency();
void singleFrequencyComplex(float* vi, float* vq);
void showbytestring16(char* title, uint16_t* data, int anz);
void init_rtty();
void make_FFTdata(float f);
void close_rtty();
void close_a();
void rtty_modifyRXfreq(int);
void showbitstring(char* title, uint8_t* data, int totallen, int anz);
void fmtest();
void initVoice();
void sendStationInfo();
void ext_rxdata(uint8_t* pdata, int len, struct sockaddr_in* rxsock);
void init_distributor();
void ext_modemRX(uint8_t* pdata);
int fifo_dataavail(int pipenum);
void showbytestring32(char* title, uint32_t* data, int anz);
void start_timer(int mSec, void(*timer_func_handler)(void));
int GetFilesInDirectory(char* folder, char files[MAXFILENUM][MAXFILESIZE]);
void temppath_name(char* pn);
void startFileSend();

#ifdef AMS_TESTMODE
void ams_BCsimulation();
void ams_testTX(liquid_float_complex sample);
int getAMSfifostat();
#endif

extern int speedmode;
extern int bitsPerSymbol;
extern int constellationSize;
extern int speed;
extern int keeprunning;
extern int caprate;
extern int BC_sock_AppToModem;
extern int DATA_sock_AppToModem;
extern int UdpDataPort_ModemToApp;
extern int txinterpolfactor;
extern int rxPreInterpolfactor;
extern char appIP[20];
extern int announcement;
extern int ann_running;
extern int transmissions;
extern int linespeed;
extern uint8_t maxLevel;
extern uint8_t maxTXLevel;
extern int VoiceAudioMode;
extern int opusbitrate;
extern int init_voice_result;
extern int initialLSvol;
extern int initialMICvol;
extern int codec;
extern int trigger_resetmodem;
extern int rxlevel_deteced;
extern int rx_in_sync;
extern int restart_modems;
extern char homepath[];
extern int sendIntro;
extern int tuning;
extern uint32_t tuning_runtime;
extern int marker;
extern int rtty_txoff;
extern int rtty_txidx;
extern int rtty_frequency;
extern int rtty_autosync;
extern int io_capidx;
extern int io_pbidx;
extern int voice_capidx;
extern int voice_pbidx;
extern float pbvol;
extern float capvol;
extern float lsvol;
extern float micvol;
extern int extData_active;
extern char AUTOSENDFOLDER[512];
extern int filesend_delay_seconds;

#ifdef _LINUX_
int isRunning(char* prgname);
void install_signal_handler();
char* ownIP();
#endif
