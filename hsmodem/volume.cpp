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
* volume.cpp ... volume setting for captue and playback
*
*/

#include "hsmodem.h"

float pbvol = 1.0f;
float capvol = 1.0f;
float lsvol = 1.0f;
float micvol = 1.0f;

void io_setPBvolume(int v)
{
	pbvol = (float)v * 2.0f / 100.0f;
}

void io_setCAPvolume(int v)
{
	capvol = (float)v * 2.0f / 100.0f;
}

void io_setLSvolume(int v)
{
	lsvol = (float)v * 2.0f / 100.0f;
}

void io_setMICvolume(int v)
{
	micvol = (float)v * 2.0f / 100.0f;
}
