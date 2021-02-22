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
* distribution.cpp ... handles priorities for hsmodem TX
*/

#include "hsmodem.h"

#ifdef _LINUX_
void* dist_function(void* param);
#endif
#ifdef _WIN32_
    void dist_function(void* param);
#endif


void init_distributor()
{
#ifdef _LINUX_
    pthread_t dist_txthread;
    pthread_create(&dist_txthread, NULL, dist_function, NULL);
#endif
#ifdef _WIN32_
    _beginthread(dist_function, 0, NULL);
#endif
}

// TX thread
#ifdef _LINUX_
void* dist_function(void* param)
{
    pthread_detach(pthread_self());
#endif
#ifdef _WIN32_
void dist_function(void* param)
{
#endif

    uint8_t rxdata[500];
    int circ = 0;
    
    printf("Distributor running\n");
    while (keeprunning)
    {
        if (ann_running == 0)
        {
            // give all data sources the same priority
            if (++circ >= 5) circ = 0;

            int len = 0;
            switch (circ)
            {
            case 0: 
                len = read_fifo(PSK_GUI_TX, rxdata, sizeof(rxdata));
                break;
            case 1: 
                len = read_fifo(EXT_TX, rxdata, sizeof(rxdata));
                break;
            case 2: 
                len = read_fifo(EXT_SPECNB, rxdata, sizeof(rxdata));
                break;
            case 3: 
                len = read_fifo(EXT_SPECWB, rxdata, sizeof(rxdata));
                break;
            case 4: sleep_ms(10); //prevent process from eating 100% CPU time
                break;
            }

            if (len > 0) _sendToModulator(rxdata, len);
        }
    }
    printf("Distributor exits\n");

#ifdef _LINUX_
    pthread_exit(NULL); // self terminate this thread
    return NULL;
#endif
}


/*
* data.. to be sent to _sendToModulator
* put it into the PSK_GUI_TX fifo
*/
void sendPSKdata(uint8_t* data, int len, int fifoID)
{
    write_fifo(fifoID, data, len);
    //printf("into fifo:%d has now:%d of 100 elements (each 300 bytes long)\n", fifoID, fifo_usedspace(fifoID));

    // wait until sent
    while (keeprunning)
    {
        int us = fifo_usedspace(fifoID);
        if (us <= 2) break;
        sleep_ms(10);
    }
}
