/*
 * main_helper
 * ===========
 * by DJ0ABR
 * 
 * functions useful for every main() program
 * 
 * */

#include "hsmodem.h"

#ifdef _LINUX_
// check if it is already running
int isRunning(char *prgname)
{
    int num = 0;
    char s[256];
    sprintf(s,"ps -e | grep %s",prgname);
    
    FILE *fp = popen(s,"r");
    if(fp)
    {
        // gets the output of the system command
        while (fgets(s, sizeof(s)-1, fp) != NULL) 
        {
            if(strstr(s,prgname) && !strstr(s,"grep"))
            {
                if(++num == 2)
                {
                    printf("%s is already running, do not start twice !",prgname);
                    pclose(fp);
                    return 1;
                }
            }
        }
        pclose(fp);
    }
    return 0;
}


// signal handler
void sighandler(int signum)
{
    printf("program stopped by signal\n");
    exit_fft();
    keeprunning = 0;
    close(BC_sock_AppToModem);
}

void install_signal_handler()
{
    
    // signal handler, mainly used if the user presses Ctrl-C
    struct sigaction sigact;
    sigact.sa_handler = sighandler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGINT, &sigact, NULL);
	sigaction(SIGTERM, &sigact, NULL);
	sigaction(SIGQUIT, &sigact, NULL);
    sigaction(SIGABRT, &sigact, NULL); // assert() error
    
    //sigaction(SIGSEGV, &sigact, NULL);
    
    // switch off signal 13 (broken pipe)
    // instead handle the return value of the write or send function
    signal(SIGPIPE, SIG_IGN);
}
#endif // _LINUX_

void closeAllandTerminate()
{
    // terminate all Threads
    keeprunning = 0;
    // close audio
    io_close_audio();
    io_close_voice();
    // close fft
    exit_fft();
    // close codec2 and opus
    close_codec2();
    close_voiceproc();
    // close liquid-SDR
    close_dsp();
    // close network sockets
#ifdef _LINUX_
    close(BC_sock_AppToModem);
#endif
#ifdef _WIN32_
    closesocket(BC_sock_AppToModem);
#endif
#ifdef _LINUX_
    close(DATA_sock_AppToModem);
#endif
#ifdef _WIN32_
    closesocket(DATA_sock_AppToModem);
#endif

    exit(0);
}

void showbytestring(char *title, uint8_t *data, int totallen, int anz)
{
    printf("%s. Len %d: ",title, totallen);
    for(int i=0; i<anz; i++)
        printf("%02X ",data[i]);
    printf("\n");
}

void showbytestring16(char *title, uint16_t *data, int anz)
{
    printf("%s. Len %d: ",title,anz);
    for(int i=0; i<anz; i++)
        printf("%04X ",data[i]);
    printf("\n");
}

void showbytestringf(char* title, float* data, int totallen, int anz)
{
    printf("%s. Len %d: ", title, totallen);
    for (int i = 0; i < anz; i++)
        printf("%7.4f ", data[i]);
    printf("\n");
}

#ifdef _LINUX_
void sleep_ms(int ms)
{
    usleep(ms * 1000);
}
#endif
#ifdef _WIN32_
void sleep_ms(int ms)
{
    Sleep(ms);
}
#endif
