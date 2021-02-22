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
*/
#include "hsmodem.h"

#ifdef _LINUX_
void* threadfunction(void* param);
#endif
#ifdef _WIN32_
void threadfunction(void* param);
#endif

#define MAXUDPTHREADS 20
RXCFG rxcfg[MAXUDPTHREADS];
int rxcfg_idx = 0;

// start UDP reception
// sock ... pointer to a socket (just a pointer to an int)
// port ... own port, messages only to this port are received
// rxfunc ... pointer to a callback function, will be called for received data
// keeprunning ... pointer to an int. If it is set to 0, the function exits
void UdpRxInit(int *sock, int port, void (*rxfunc)(uint8_t *, int, struct sockaddr_in*), int *keeprunning)
{
    if(rxcfg_idx >= MAXUDPTHREADS)
    {
        printf("max number of UDP threads\n");
        exit(0);
    }
    
    rxcfg[rxcfg_idx].sock = sock;
    rxcfg[rxcfg_idx].port = port;
    rxcfg[rxcfg_idx].rxfunc = rxfunc;
    rxcfg[rxcfg_idx].keeprunning = keeprunning;
    
    // bind port
    struct sockaddr_in sin;

#ifdef _WIN32_
    WSADATA wsaData = { 0 };
    int ires = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ires != 0)
        printf("WSAStartup failed: %d\n", ires);
#endif

	*sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (*sock == -1){
		printf("Failed to create Socket\n");
		exit(0);
	}
	
	char enable = 1;
	setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
    sin.sin_addr.s_addr = INADDR_ANY;

	if (bind(*sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)) != 0)
	{
		printf("Failed to bind socket, port:%d\n",port);
#ifdef _LINUX_
        close(*sock);
#endif
#ifdef _WIN32_
        closesocket(*sock);
#endif
		exit(0);
	}

    printf("port %d sucessfully bound\n", port);
	
	// port sucessfully bound
	// create the receive thread
#ifdef _LINUX_
	pthread_t rxthread;
	pthread_create(&rxthread, NULL, threadfunction, &(rxcfg[rxcfg_idx]));
#endif
#ifdef _WIN32_
    _beginthread(threadfunction, 0, &(rxcfg[rxcfg_idx]));
#endif
    rxcfg_idx++;
}

#ifdef _LINUX_
void* threadfunction(void* param) {
    socklen_t fromlen;
    pthread_detach(pthread_self());
#endif
#ifdef _WIN32_
void threadfunction(void* param) {
        int fromlen;
#endif
    RXCFG rxcfg;
    memcpy((uint8_t *)(&rxcfg), (uint8_t *)param, sizeof(RXCFG));
	int recvlen;
    const int maxUDPpacketsize = 2048;
	char rxbuf[maxUDPpacketsize];
	struct sockaddr_in fromSock;
	fromlen = sizeof(struct sockaddr_in);
	while(*rxcfg.keeprunning)
	{
        recvlen = recvfrom(*rxcfg.sock, rxbuf, maxUDPpacketsize, 0, (struct sockaddr *)&fromSock, &fromlen);
		if (recvlen > 0)
        {
            // data received, send it to callback function
            (*rxcfg.rxfunc)((uint8_t *)rxbuf,recvlen, &fromSock);
        }
        if (recvlen < 0)
        {
#ifdef _WIN32_
            printf("recvfrom error: %d", WSAGetLastError());
#endif
        }
	}
#ifdef _LINUX_
    pthread_exit(NULL); // self terminate this thread
    return NULL;
#endif
}

// send UDP message
void sendUDP(char *destIP, int destPort, uint8_t *pdata, int len)
{
    int sockfd; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        printf("sendUDP: socket creation failed\n"); 
        exit(0); 
    } 
    memset(&servaddr, 0, sizeof(servaddr)); 
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(destPort); 
    //printf("Send to <%s><%d> Len:%d\n",destIP,destPort,len);
    servaddr.sin_addr.s_addr=inet_addr(destIP);
    sendto(sockfd, (char *)pdata, len, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
#ifdef _LINUX_
    close(sockfd);
#endif
#ifdef _WIN32_
    closesocket(sockfd);
#endif
}

