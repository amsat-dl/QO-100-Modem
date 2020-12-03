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

uint8_t scr[400] = {    
130 ,   239 ,   223 ,   19  ,   146 ,   254 ,   12  ,   86  ,   106 ,   68  ,
77  ,   213 ,   243 ,   216 ,   102 ,   227 ,   108 ,   113 ,   229 ,   89  ,
26  ,   64  ,   138 ,   216 ,   225 ,   121 ,   194 ,   137 ,   152 ,   64  ,
51  ,   175 ,   68  ,   200 ,   37  ,   104 ,   247 ,   68  ,   193 ,   50  ,
19  ,   14  ,   196 ,   81  ,   4   ,   236 ,   191 ,   249 ,   83  ,   25  ,
161 ,   171 ,   167 ,   29  ,   33  ,   139 ,   7   ,   152 ,   230 ,   144 ,
125 ,   206 ,   34  ,   236 ,   112 ,   78  ,   219 ,   34  ,   181 ,   161 ,
7   ,   45  ,   198 ,   235 ,   62  ,   115 ,   194 ,   100 ,   209 ,   95  ,
186 ,   161 ,   53  ,   10  ,   110 ,   246 ,   122 ,   246 ,   207 ,   194 ,
178 ,   63  ,   232 ,   93  ,   158 ,   234 ,   231 ,   73  ,   214 ,   64,
130 ,   239 ,   223 ,   19  ,   146 ,   254 ,   12  ,   86  ,   106 ,   68  ,
77  ,   213 ,   243 ,   216 ,   102 ,   227 ,   108 ,   113 ,   229 ,   89  ,
26  ,   64  ,   138 ,   216 ,   225 ,   121 ,   194 ,   137 ,   152 ,   64  ,
51  ,   175 ,   68  ,   200 ,   37  ,   104 ,   247 ,   68  ,   193 ,   50  ,
19  ,   14  ,   196 ,   81  ,   4   ,   236 ,   191 ,   249 ,   83  ,   25  ,
161 ,   171 ,   167 ,   29  ,   33  ,   139 ,   7   ,   152 ,   230 ,   144 ,
125 ,   206 ,   34  ,   236 ,   112 ,   78  ,   219 ,   34  ,   181 ,   161 ,
7   ,   45  ,   198 ,   235 ,   62  ,   115 ,   194 ,   100 ,   209 ,   95  ,
186 ,   161 ,   53  ,   10  ,   110 ,   246 ,   122 ,   246 ,   207 ,   194 ,
178 ,   63  ,   232 ,   93  ,   158 ,   234 ,   231 ,   73  ,   214 ,   64,
130 ,   239 ,   223 ,   19  ,   146 ,   254 ,   12  ,   86  ,   106 ,   68  ,
77  ,   213 ,   243 ,   216 ,   102 ,   227 ,   108 ,   113 ,   229 ,   89  ,
26  ,   64  ,   138 ,   216 ,   225 ,   121 ,   194 ,   137 ,   152 ,   64  ,
51  ,   175 ,   68  ,   200 ,   37  ,   104 ,   247 ,   68  ,   193 ,   50  ,
19  ,   14  ,   196 ,   81  ,   4   ,   236 ,   191 ,   249 ,   83  ,   25  ,
161 ,   171 ,   167 ,   29  ,   33  ,   139 ,   7   ,   152 ,   230 ,   144 ,
125 ,   206 ,   34  ,   236 ,   112 ,   78  ,   219 ,   34  ,   181 ,   161 ,
7   ,   45  ,   198 ,   235 ,   62  ,   115 ,   194 ,   100 ,   209 ,   95  ,
186 ,   161 ,   53  ,   10  ,   110 ,   246 ,   122 ,   246 ,   207 ,   194 ,
178 ,   63  ,   232 ,   93  ,   158 ,   234 ,   231 ,   73  ,   214 ,   64,
130 ,   239 ,   223 ,   19  ,   146 ,   254 ,   12  ,   86  ,   106 ,   68  ,
77  ,   213 ,   243 ,   216 ,   102 ,   227 ,   108 ,   113 ,   229 ,   89  ,
26  ,   64  ,   138 ,   216 ,   225 ,   121 ,   194 ,   137 ,   152 ,   64  ,
51  ,   175 ,   68  ,   200 ,   37  ,   104 ,   247 ,   68  ,   193 ,   50  ,
19  ,   14  ,   196 ,   81  ,   4   ,   236 ,   191 ,   249 ,   83  ,   25  ,
161 ,   171 ,   167 ,   29  ,   33  ,   139 ,   7   ,   152 ,   230 ,   144 ,
125 ,   206 ,   34  ,   236 ,   112 ,   78  ,   219 ,   34  ,   181 ,   161 ,
7   ,   45  ,   198 ,   235 ,   62  ,   115 ,   194 ,   100 ,   209 ,   95  ,
186 ,   161 ,   53  ,   10  ,   110 ,   246 ,   122 ,   246 ,   207 ,   194 ,
178 ,   63  ,   232 ,   93  ,   158 ,   234 ,   231 ,   73  ,   214 ,   64
};

uint8_t rx_scrbuf[400];

void TX_Scramble(uint8_t *data, int len)
{
    if (len > 400) return;
    
    for(int i=0; i<len; i++)
        data[i] ^= scr[i];
}

uint8_t *RX_Scramble(uint8_t *data, int len)
{
    if (len > 400) return data;
    
    memcpy(rx_scrbuf,data,len);
    
    for(int i=0; i<len; i++)
        rx_scrbuf[i] ^= scr[i];

    return rx_scrbuf;

}
