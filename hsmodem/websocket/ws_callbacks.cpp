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

extern int useCAT;
int connections = 0;

// a new browser connected
void onopen(int fd)
{
	char *cli;
	cli = ws_getaddress(fd);
    if(cli != NULL)
    {
        insert_socket(fd,cli);
        printf("Connection opened, client: %d | addr: %s\n", fd, cli);
        connections++;
        printf("%d users logged in\n",connections);
        free(cli);
    }
}

// a browser disconnected
void onclose(int fd)
{
    remove_socket(fd);
#ifdef WIN32
    _close(fd);
#else
    close(fd);
#endif
    printf("Connection closed, client: %d\n", fd);
    connections--;
    printf("%d users logged in\n",connections);
}

// if avaiable, send data to a browser
int onwork(int fd, unsigned char *cnt0, unsigned char *cnt1)
{
int ret = 0;

    for(int i=0; i<MAX_CLIENTS; i++)
    {
        if(actsock[i].socket == fd)
        {
            // send all available data in one frame
            // (sending multiple frames resulted in data loss)
            int len;
            unsigned char *p = ws_build_txframe(i,&len);
            if(p != NULL)
            {
                ret = ws_sendframe_binary(fd, p, len);
                free(p);
                actsock[i].send = 0;
            }
            return ret;
        }
    }
    return 0;
}


// received a Websocket Message from a browser
void onmessage(int fd, unsigned char *msg)
{
USERMSG tx_usermsg;

	char *cli = ws_getaddress(fd);
    if(cli != NULL)
    {
        tx_usermsg.client = get_socket_idx(fd);
        tx_usermsg.command = 0;

        //printf("Browser messages: %s, from: %s/%d\n", msg, cli, fd);
        actsock[tx_usermsg.client].alive = 20;

        free(cli);
    }
}
