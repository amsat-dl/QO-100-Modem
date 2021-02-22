/*
* High Speed modem to transfer data in a 2,7kHz SSB channel
* =========================================================
* Author: DJ0ABR
*
*   (c) DJ0ABR
*   www.dj0abr.de
* 
* websocket server: based on the work by: Davidson Francis <davidsondfgl@gmail.com>
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
* websocketserver.c ... sends data to a web browser
* 
* if a web browser is logged into this WebSocketServer then
* we send the data to this browser.
* Its the job of the browser to make anything with these data.
* 
* This WebSocketServer is a multi threaded implementation and
* opens a new thread for each client
* 
* ! THIS implementation of a WebSocketServer DOES NOT require a Webserver (like Apache)
* because it handles all Websocket tasks by itself !
* 
* usage:
* ======
* 
* ws_init() ... call once after program start to initialize the websocket server
* 
* ws_send(unsigned char *pdata, int len) ...
* send pdata (max. lenght MESSAGE_LENGTH) to all connected clients. 
* this function is thread safe. It stores the data in a buffer.
* This buffer is sent to the clients in the websocket-thread in the background.
* 
* 
*/

#include "../hsmodem.h"

void init_ws_locks();
void ws_alive();

#ifdef _WIN32_
void wsproc(void* param);
#else
void* wsproc(void* param);
#endif

#define MAXIPLEN 16

WS_SOCK actsock[MAX_CLIENTS];
char clilist[MAX_CLIENTS][MAXIPLEN];

/*void test_websocket()
{
    char* msg = "ABCD1234";

    static int t = 0;
    if (++t > 100)
    {
        t = 0;
        printf("send ws: %s\n", msg);
        ws_send((unsigned char *)msg, strlen(msg));
    }
}*/

// initialise the WebSocketServer
// run the WebSocketServer in a new thread
void ws_init()
{
    printf("starting websocket server\n");
    init_ws_locks();
    for(int i=0; i<MAX_CLIENTS; i++)
    {
        actsock[i].socket = -1;
        actsock[i].send = 0;
    }

    start_timer(1000, ws_alive);

#ifdef WIN32
    _beginthread(wsproc, 0, NULL);
#else
    pthread_t ws_pid;
    int ret = pthread_create(&ws_pid,NULL,wsproc, NULL);
    if(ret)
        printf("websocket: process NOT started !!!\n\r");
#endif
}

// Websocket thread
#ifdef WIN32
void wsproc(void* param)
{
#else
    void* wsproc(void* param)
{
    pthread_detach(pthread_self());
#endif

    printf("websocket server running\n");

    struct ws_events evs;
	evs.onopen    = &onopen;
	evs.onclose   = &onclose;
	evs.onmessage = &onmessage;
    evs.onwork    = &onwork;
	ws_socket(&evs, TcpDataPort_WebSocket);
    
#ifndef WIN32
    pthread_exit(NULL); // self terminate this thread
    return NULL;
#endif
}

#ifdef WIN32
CRITICAL_SECTION ws_crit_sec;
#define WS_LOCK	EnterCriticalSection(&ws_crit_sec)
void WS_UNLOCK()
{
    if (&ws_crit_sec != NULL)
        LeaveCriticalSection(&ws_crit_sec);
}
#else
pthread_mutex_t ws_crit_sec;
#define WS_LOCK	pthread_mutex_lock(&ws_crit_sec)
void WS_UNLOCK() { pthread_mutex_unlock(&ws_crit_sec); }
#endif

void init_ws_locks()
{
    // init pipes only once
    static int f = 1;
    if (f)
    {
        f = 0;
#ifdef WIN32
        InitializeCriticalSection(&ws_crit_sec);
#else
        pthread_mutex_init(&ws_crit_sec, NULL);
#endif
    }
}

/*
* save the data in an array and set a flag.
* the transmission to the web browser is done by the threads
* handling all logged-in web browsers.
*
* this function is thread safe by a LOCK
*/

// insert the new message into the buffer of each active client
void ws_send(unsigned char *pdata, int len)
{
    WS_LOCK;
    for(int i=0; i<MAX_CLIENTS; i++)
    {
        if(actsock[i].socket != -1)
        {
            // insert data
            if(actsock[i].send == 0)
            {
                if ((unsigned int)len < sizeof(actsock[i].msg))
                    memcpy(actsock[i].msg, pdata, len);
                else
                    memcpy(actsock[i].msg, "ws check data size", 18);
                actsock[i].msglen = len;
                actsock[i].send = 1;
            }
        }
    }
    
    WS_UNLOCK();
}

unsigned char *ws_build_txframe(int i, int *plength)
{
    WS_LOCK;

    // calculate total length
    // add 3 for each element for the first byte which is the element type followed by the length
    int geslen = 0;
    if(actsock[i].send == 1) geslen += (actsock[i].msglen + 3);
    
    if(geslen < 10) 
    {
        WS_UNLOCK();
        return NULL;
    }
    
    // assign TX buffer
    unsigned char *txdata = (unsigned char*)malloc(geslen);
    if(txdata == NULL)
    {
        WS_UNLOCK();
        return NULL;
    }
    
    // copy data into TX buffer and set the type byte
    int idx = 0;
    if(actsock[i].send == 1) 
    {
        txdata[idx++] = 0;    // type information
        txdata[idx++] = actsock[i].msglen >> 8;
        txdata[idx++] = actsock[i].msglen & 0xff;
        memcpy(txdata+idx,actsock[i].msg,actsock[i].msglen);
        idx += actsock[i].msglen;
    }

    *plength = idx;
    
    WS_UNLOCK();
    return txdata;
}

// insert a socket into the socket-list
void insert_socket(int fd, char *cli)
{
    WS_LOCK;

    for(int i=0; i<MAX_CLIENTS; i++)
    {
        if(actsock[i].socket == -1)
        {
            actsock[i].socket = fd;
            actsock[i].send = 0;
            actsock[i].alive = 10;
            strncpy(clilist[i],cli,MAXIPLEN);
            clilist[i][MAXIPLEN-1] = 0;
            printf("accepted client %d %d\n",i,fd);
            
            WS_UNLOCK();
            return;
        }
    }
    WS_UNLOCK();
    printf("all sockets in use !!!\n");
}

// remove a socket from the socket-list
void remove_socket(int fd)
{
    WS_LOCK;
    for(int i=0; i<MAX_CLIENTS; i++)
    {
        if(actsock[i].socket == fd)
        {
            printf("remove client %d %d\n", i, fd);
/*
!!!  DO NOT close it here, this is the wrong thread !!!
            close(fd);
*/
            actsock[i].socket = -1;
            clilist[i][0] = 0;
        }
    }
            
    WS_UNLOCK();
}

int get_socket_idx(int fd)
{
    WS_LOCK;
    for(int i=0; i<MAX_CLIENTS; i++)
    {
        if(actsock[i].socket == fd)
        {
            WS_UNLOCK();
            return i;
        }
    }
    WS_UNLOCK();
    return -1;
}

// get IP corresponding to a socket
char *getSocketIP(int fd)
{
    WS_LOCK;
    for(int i=0; i<MAX_CLIENTS; i++)
    {
        if(actsock[i].socket == fd)
        {
            WS_UNLOCK();
            return clilist[i];
        }
    }
    WS_UNLOCK();
    return NULL;
}

// called by timer every 1s
void ws_alive()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (actsock[i].socket != -1 && actsock[i].alive > 0)
        {
            //printf("%d %d\n", i, actsock[i].alive);
            actsock[i].alive--;
            if (actsock[i].alive == 0)
            {
                // remove inactive client
                remove_socket(actsock[i].socket);
            }
        }
    }
}

// remove a socket from the socket-list
int get_alive(int fd)
{
    int a = 0;
    WS_LOCK;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (actsock[i].socket == fd)
        {
            a = actsock[i].alive;
            WS_UNLOCK();
            return a;
        }
    }
    WS_UNLOCK();
    return 0;
}
