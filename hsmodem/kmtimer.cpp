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
*
* Repeatable Timer for various needs
* ==================================
*
*/

#include "hsmodem.h"

/*
usage:

void timerhandler_function(void)
{
// called every time_in_ms
}

start_timer(time_in_ms, &timerhandler_function);

*/

void(*timer_func_handler_pntr)(void);

#ifdef _LINUX_

struct itimerval timervalue;
struct sigaction new_handler, old_handler;
void timer_sig_handler(int);

void start_timer(int mSec, void(*timer_func_handler)(void))
{
	timer_func_handler_pntr = timer_func_handler;

	timervalue.it_interval.tv_sec = mSec / 1000;
	timervalue.it_interval.tv_usec = (mSec % 1000) * 1000;
	timervalue.it_value.tv_sec = mSec / 1000;
	timervalue.it_value.tv_usec = (mSec % 1000) * 1000;
	if (setitimer(ITIMER_REAL, &timervalue, NULL))
	{
		printf("start_timer() error\n");
		return;
	}

	new_handler.sa_handler = &timer_sig_handler;
	new_handler.sa_flags = SA_NOMASK;
	if (sigaction(SIGALRM, &new_handler, &old_handler))
	{
		printf("sigaction() error\n");
		return;
	}

	return;
}


void timer_sig_handler(int arg)
{
	timer_func_handler_pntr();
}


void stop_timer(void)
{
	timervalue.it_interval.tv_sec = 0;
	timervalue.it_interval.tv_usec = 0;
	timervalue.it_value.tv_sec = 0;
	timervalue.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &timervalue, NULL);

	sigaction(SIGALRM, &old_handler, NULL);
}

#endif

#ifdef WIN32

int ms;
unsigned int timethreadID = 0;
unsigned int __stdcall TimerThread(void* p);

void start_timer(int mSec, void(*timer_func_handler)(void))
{
	timer_func_handler_pntr = timer_func_handler;
	ms = mSec;

	_beginthreadex(NULL, 0, TimerThread, NULL, 0, &timethreadID);
}

unsigned int __stdcall TimerThread(void* p)
{
	HANDLE event_handle = CreateEvent(NULL, FALSE, FALSE, "mytimerhandle");
	while (keeprunning)
	{
		switch (WaitForSingleObject(event_handle, ms))
		{
		case WAIT_TIMEOUT:
			timer_func_handler_pntr();
			break;
		}
	}
	return 0;
}

#endif
