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
    _exit_fft();
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
    kmaudio_close();
    // close fft
    _exit_fft();
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

void showbitstring(char* title, uint8_t* data, int totallen, int anz)
{
    printf("%s. len %d: ", title, totallen);
    for (int i = 0; i < anz; i++)
        printf("%01X ", data[i]);
    printf("\n");
}

void showbytestring(char *title, uint8_t *data, int totallen, int anz)
{
    printf("%s. len % 4d: ",title, totallen);
    for(int i=0; i<anz; i++)
        printf("%02X ",data[i]);
    printf("\n");
}

void showbytestring16(char *title, uint16_t *data, int anz)
{
    printf("%s. len %d: ",title,anz);
    for(int i=0; i<anz; i++)
        printf("%04X ",data[i]);
    printf("\n");
}

void showbytestring32(char* title, uint32_t* data, int anz)
{
    printf("%s. len %d: ", title, anz);
    for (int i = 0; i < anz; i++)
        printf("%08X ", data[i]);
    printf("\n");
}

void showbytestringf(char* title, float* data, int totallen, int anz)
{
    printf("%s. len %d: ", title, totallen);
    for (int i = 0; i < anz; i++)
        printf("%7.4f ", data[i]);
    printf("\n");
}

#ifdef _LINUX_
void sleep_ms(int ms)
{
    for(int i=0; i<1000; i++)
        usleep(ms);
}
#endif
#ifdef _WIN32_
void sleep_ms(int ms)
{
    Sleep(ms);
}
#endif

/* Returns a list of files in a directory (except the ones that begin with a dot) */

int GetFilesInDirectory(char* folder, char files[MAXFILENUM][MAXFILESIZE])
{
#ifdef _WIN32_
    WIN32_FIND_DATA ffd;
    char dir[512];
    int fnum = 0;

    sprintf(dir, "%s/*", folder);
    printf("List files in:<%s>\n", dir);
    HANDLE hFind = FindFirstFile(dir, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("folder not found\n");
        return 0;
    }

    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            // ignore files > 200k
            if (ffd.nFileSizeHigh > 0 || ffd.nFileSizeLow > 200000) continue;

            if (strlen(ffd.cFileName) < (size_t)(MAXFILESIZE - 1))
            {
                //printf("%s %lld\n", ffd.cFileName, filesize.QuadPart);
                strcpy(files[fnum], ffd.cFileName);
                fnum++;
            }
            if (fnum >= MAXFILENUM) break;
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    return fnum;

#else
    DIR* dir;
    struct dirent* f;
    int fnum = 0;

    dir = opendir(folder);
    if (dir)
    {
        while ((f = readdir(dir)))
        {
            if (!f->d_name || f->d_name[0] == '.' || f->d_type != DT_REG)
                continue; // Skip everything that starts with a dot

            // ignore files > 200k
            struct stat st;
            char ffn[512];
            snprintf(ffn, 511, "%s/%s", folder, f->d_name);
            stat(ffn, &st);
            //printf("File: <%s> size:<%ld>\n", ffn, st.st_size);
            if (st.st_size < 200000)
            {
                strcpy(files[fnum], f->d_name);
                fnum++;
            }
        }
        closedir(dir);
    }
    return fnum;
#endif
}

// gets a writeable temporary path
void temppath_name(char* pn)
{
    // Windows: user home
    // Linux /tmp
    strcpy(pn,"/tmp/");
#ifdef _WIN32_
    snprintf(pn, 256, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
    strcat(pn, "\\");
#endif
}

#ifdef _LINUX_
// get own IP adress
char* ownIP()
{
    static char ip[20] = { 0 };

    struct ifaddrs* ifaddr, * ifa;
    int s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            if (strncmp(host, "127", 3) != 0)
            {
                strcpy(ip, host);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return ip;
}
#endif
