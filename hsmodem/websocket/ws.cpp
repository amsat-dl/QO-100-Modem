/*
* High Speed modem to transfer data in a 2,7kHz SSB channel
* =========================================================
* Author: DJ0ABR
*
*   (c) DJ0ABR
*   www.dj0abr.de

websocket server: based on the work by: Davidson Francis <davidsondfgl@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "../hsmodem.h"

/* Registered events. */
struct ws_events events;

/**
 * Gets the IP address relative to a
 * file descriptor opened by the server.
 * @param fd File descriptor target.
 * @return Pointer the ip address.
 */
char* ws_getaddress(int fd)
{
	struct sockaddr_in addr;
#ifdef WIN32
	int addr_size;
#else
    socklen_t addr_size;
#endif
    char *client;

    addr_size = sizeof(struct sockaddr_in);
    if ( getpeername(fd, (struct sockaddr *)&addr, &addr_size) < 0 )
    	return NULL;

    client = (char *)malloc(sizeof(char) * 20);
    if(client == NULL)
        return NULL;
    
    strcpy(client, inet_ntoa(addr.sin_addr));
    return (client);
}

/**
 * Creates and send a WebSocket frame
 * with some binary message.
 * @param fd  Target to be send.
 * @param msg Message to be send.
 */
int ws_sendframe_binary(int fd, unsigned char *msg, uint64_t length)
{
	unsigned char *response;  /* Response data.  */
	unsigned char frame[10];  /* Frame.          */
	uint8_t idx_first_rData;  /* Index data.     */
	int idx_response;      /* Index response. */
	int output;               /* Bytes sent.     */

	/* Binary data. */
	frame[0] = (WS_FIN | WS_FR_OP_BINARY);

	/* Split the size between octects. */
	if (length <= 125)
	{
		frame[1] = length & 0x7F;
		idx_first_rData = 2;
	}

	/* Size between 126 and 65535 bytes. */
	else if (length >= 126 && length <= 65535)
	{
		frame[1] = 126;
		frame[2] = (length >> 8) & 255;
		frame[3] = length & 255;
		idx_first_rData = 4;
	}

	/* More than 65535 bytes. */
	else
	{
		frame[1] = 127;
		frame[2] = (unsigned char) ((length >> 56) & 255);
		frame[3] = (unsigned char) ((length >> 48) & 255);
		frame[4] = (unsigned char) ((length >> 40) & 255);
		frame[5] = (unsigned char) ((length >> 32) & 255);
		frame[6] = (unsigned char) ((length >> 24) & 255);
		frame[7] = (unsigned char) ((length >> 16) & 255);
		frame[8] = (unsigned char) ((length >> 8) & 255);
		frame[9] = (unsigned char) (length & 255);
		idx_first_rData = 10;
	}

	/* Add frame bytes. */
	idx_response = 0;
	response = (unsigned char *)malloc((size_t)( sizeof(unsigned char) * (idx_first_rData + length + 1)));
    if(response != NULL)
    {
        for (int i = 0; i < idx_first_rData; i++)
        {
            response[i] = frame[i];
            idx_response++;
        }

        /* Add data bytes. */
        for (uint64_t i = 0; i < length; i++)
        {
            response[idx_response] = msg[i];
            idx_response++;
        }
		output = send(fd, (char *)response, idx_response,0);
        free(response);
        return (output);
    }
	return 0;
}


/**
 * Receives a text frame, parse and decodes it.
 * @param frame  WebSocket frame to be parsed.
 * @param length Frame length.
 * @param type   Frame type.
 */
static unsigned char* ws_receiveframe(unsigned char *frame, size_t length, int *type)
{
	unsigned char *msg;     /* Decoded message.        */
	uint8_t mask;           /* Payload is masked?      */
	uint8_t flength;        /* Raw length.             */
	uint8_t idx_first_mask; /* Index masking key.      */
	uint8_t idx_first_data; /* Index data.             */
	size_t  data_length;    /* Data length.            */
	uint8_t masks[4];       /* Masking key.            */
	int     i,j;            /* Loop indexes.           */

	msg = NULL;
	
	/* Checks the frame type and parse the frame. */
	if (frame[0] == (WS_FIN | WS_FR_OP_TXT) )
	{
		*type = WS_FR_OP_TXT;
		idx_first_mask = 2;
		mask           = frame[1];
		flength        = mask & 0x7F;

		if (flength == 126)
			idx_first_mask = 4;
		else if (flength == 127)
			idx_first_mask = 10;

		idx_first_data = idx_first_mask + 4;
		data_length = length - idx_first_data;

		masks[0] = frame[idx_first_mask+0];
		masks[1] = frame[idx_first_mask+1];
		masks[2] = frame[idx_first_mask+2];
		masks[3] = frame[idx_first_mask+3];

		msg = (unsigned char *)malloc(sizeof(unsigned char) * (data_length+1) );
        if(msg == NULL)
            return NULL;
        
		for (i = idx_first_data, j = 0; i < (int)length; i++, j++)
			msg[j] = frame[i] ^ masks[j % 4];

		msg[j] = '\0';
	}

	/* Close frame. */
	else if (frame[0] == (WS_FIN | WS_FR_OP_CLSE) )
		*type = WS_FR_OP_CLSE;
	
	/* Not supported frame yet. */
	else
		*type = frame[0] & 0x0F;

	return msg;
}

// nonblocking read with a 10ms timeout
int readsocket(int sock, unsigned char* buf, int maxlen)
{
	int n;
	// make the read unblocking
	// but check with select if something is in the receive buffer
	fd_set input;
	FD_ZERO(&input);
	FD_SET(sock, &input);
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 10000;
	n = select(sock + 1, &input, NULL, NULL, &timeout); // select will socket+1, blöd, aber ist so
	if (n <= 0)
	{
		return n; // 0=no data, <0=error
	}

	if (!FD_ISSET(sock, &input))
	{
		return -1;  // error
	}

	sleep_ms(10);  // wait a bit to give a message the chance to be rxed completely
	n = recv(sock, (char*)buf, maxlen, 0);
    return n;
}

/**
 * Establishes to connection with the client and trigger
 * events when occurs one.
 * @param vsock Client file descriptor.
 * @note This will be run on a different thread.
 */

#ifdef WIN32
void ws_establishconnection(void* vsock)
{
#else
void* ws_establishconnection(void* vsock)
{
	pthread_detach(pthread_self());
#endif

	int sock;
	size_t n;                           /* Number of bytes sent/received. */
	unsigned char frm[MESSAGE_LENGTH];  /* Frame.                         */
	unsigned char *msg;                 /* Message.                       */
	char *response;                     /* Response frame.                */
	int  handshaked;                    /* Handshake state.               */
	int  type;                          /* Frame type.                    */
	unsigned char cnt0=0,cnt1=0;

	handshaked = 0;
	sock = (int)(intptr_t)vsock;

	while (keeprunning)
	{
		n = readsocket(sock, frm, sizeof(unsigned char) * MESSAGE_LENGTH);
		/* Receives message until get some error. */
		if (n >= 0)
		{
			if (n > 0)
			{
				// data received
				/* If not handshaked yet. */
				if (!handshaked)
				{
					getHSresponse((char*)frm, &response);
					handshaked = 1;
					/*printf("Handshaked, response: \n"
						"------------------------------------\n"
						"%s"
						"------------------------------------\n"
						,response);*/
					n = send(sock, response, strlen(response),0);
					events.onopen(sock);
					free(response);
				}

				/* Decode/check type of frame. */
				msg = ws_receiveframe(frm, n, &type);

				if (msg == NULL)
					continue;

				/* Trigger events. */
				if (type == WS_FR_OP_TXT)
					events.onmessage(sock, msg);
				else if (type == WS_FR_OP_CLSE)
				{
					if (msg != NULL) free(msg);
					events.onclose(sock);
#ifdef WIN32
					return;
#else
					return vsock;
#endif
				}
				else
					printf("type :%d\n", type);

				if (msg != NULL) free(msg);
			}
			if (n == 0)
			{
				if (get_alive(sock) == 0)
				{
					events.onclose(sock);
#ifdef WIN32
					return;
#else
					return vsock;
#endif
				}
				
				// no data received, normal processing loop
				int ret = events.onwork(sock, &cnt0, &cnt1);
				if (ret == -1)
				{
					// other side closed the connection (write error)
					events.onclose(sock);
#ifdef WIN32
					return;
#else
					return vsock;
#endif
				}
				sleep_ms(10);      // do not eat up the CPU time
			}
		}
	}

	
#ifdef WIN32
	_close(sock);
	return;
#else
	close(sock);
	return vsock;
#endif
}

/**
 * Main loop for the server, runs in a thread
 * @param evs  Events structure.
 * @param port Server port.
 */
int ws_socket(struct ws_events *evs, int port)
{
	int sock;                  /* Current socket.        */
	int new_sock;              /* New opened connection. */
	struct sockaddr_in server; /* Server.                */
	struct sockaddr_in client; /* Client.                */
	int len;                   /* Length of sockaddr.    */

	if (evs == NULL || port <= 0 || port >  65535)
	{
		printf("An error has ocurred, please review your events or the desired port!\n");
		return -1;
	}

	/* Copy events. */
	memcpy(&events, evs, sizeof(struct ws_events));

#ifdef _WIN32_
	WSADATA wsaData = { 0 };
	int ires = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ires != 0)
		printf("WSAStartup failed: %d\n", ires);
#endif

	/* Create socket. */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Could not create socket\n");
		return -1;
	}

	/* Reuse previous address. */
	const char val = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) < 0)
	{
    	perror("setsockopt(SO_REUSEADDR) failed");
		return -1;
	}

	/* Prepare the sockaddr_in structure. */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
    printf("Websocket Server: listen to port:%d\n",port);
	server.sin_port = htons(port);

	/* Bind. */
	if( bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0 )
	{
		perror("Bind failed");
		return -1;
	}

	/* Listen. */
	listen(sock, MAX_CLIENTS);

	/* Wait for incoming connections. */
	printf("Waiting for incoming connections...\n");

	len = sizeof(struct sockaddr_in);
	
	/* Accept connections. */
	while (keeprunning)
	{
		//if (extData_active)
		{
			/* Accept. */
#ifdef WIN32
			new_sock = accept(sock, (struct sockaddr*)&client, &len);
			printf("new socket: %d\n", new_sock);
#else
			new_sock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&len);
#endif
			if (new_sock < 0)
			{
				perror("Error on accepting conections..");
				exit(-1);
			}

#ifdef WIN32
			printf("start Thread\n");
			_beginthread(ws_establishconnection, 0, (void*)(intptr_t)new_sock);
#else
			pthread_t client_thread;
			if (pthread_create(&client_thread, NULL, ws_establishconnection, (void*)(intptr_t)new_sock) < 0)
				perror("Could not create the client thread!");

			pthread_detach(client_thread); // automatically release all ressources as soon as the thread is done
#endif
		}
		/*else
		{
			sleep_ms(100);
		}*/
	}
	return 0;
}
