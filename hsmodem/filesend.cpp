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
* filesend.cpp
* ============
* send files and send all file in a folder
* this is a separate thread waiting for a start condition.
* Then its sends all files in the specified folder.
* 
* Actions required to send a file:
* --------------------------------
* Images: 
* jpg images, already scaled to the required format
* 
* files: 
* any file, assign file type according file-extension
* zip compression (zip filename same as zipped file)
* add the HSmdem-File-Header
* 
*
*/

#include "hsmodem.h"

void sendFile(char* file, char* dir);
int zipfile(char* fullname, char* name, char* zipfilename);
int prepare_filesend(char* fullname, char* filename);
void sendNextInFolder();
int wait_sendtime(int mode);

int do_filesend = 0;	// set to 1 to activate automated file sending
// hold the complete file to be sent
uint8_t filearray[250000];
int filestatus = 0; // first frame
int filarr_len = 0;	// length of data in filearray

#ifdef _WIN32_
char AUTOSENDFOLDER[512] = {"c:\\tmp" };
#else
char AUTOSENDFOLDER[512] = {"/home/odroid/hsmodem/html/bulletin"};
#endif

int send_idx = 0;

#ifdef _LINUX_
void* filesend_function(void* param);
#endif
#ifdef _WIN32_
void filesend_function(void* param);
#endif

void startFileSend()
{
#ifdef _LINUX_
	pthread_t filesend_txthread;
	pthread_create(&filesend_txthread, NULL, filesend_function, NULL);
#endif
#ifdef _WIN32_
	_beginthread(filesend_function, 0, NULL);
#endif
}

#ifdef _LINUX_
void* filesend_function(void* param)
{
	pthread_detach(pthread_self());
#endif
#ifdef _WIN32_
	void filesend_function(void* param)
	{
#endif
		static int lastfifostat = -1;
		printf("FileSend running\n");
		while (keeprunning)
		{
			// wait for time span and until fifo is available
			
			int fifostat = fifo_usedspace(FIFO_FILESEND);
			if (fifostat > 0)
			{
				//printf("currently sending\n");
				sleep_ms(1000);
				lastfifostat = fifostat;
				continue;
			}
			if (lastfifostat > 0 && fifostat == 0)
			{
				//printf("current transmission finished, reset time counter\n");
				wait_sendtime(1);
				sleep_ms(1000);
				lastfifostat = fifostat;
				continue;
			}
			lastfifostat = fifostat;

			if (wait_sendtime(0) == 0)
			{
				//printf("wait for send-time-interval\n");
				sleep_ms(1000);
				continue;
			}

			printf("send next file in %s\n", AUTOSENDFOLDER);
			sendNextInFolder();
		}
		printf("FileSend exits\n");

#ifdef _LINUX_
		pthread_exit(NULL); // self terminate this thread
		return NULL;
#endif
	}

	// wait for filesend_delay_seconds 
	// return: 0=waiting, 1=send

	int filesend_delay_seconds = 10;

	// mode: 0=wait for time span, 1=set time to beginning
	int wait_sendtime(int mode)
	{

		static uint64_t lastSendTime = 0;
		uint64_t actTime = getms();

		static int f = 1;
		if (f || mode)
		{
			printf("reset time\n");
			lastSendTime = actTime;
			f = 0;
			return 0;
		}

		uint64_t difftime_s = (actTime - lastSendTime)/1000;
		uint64_t ts_s = (uint64_t)filesend_delay_seconds;

		//printf("difftime_s %ld ts_s %ld\n", difftime_s ,ts_s);

		if (difftime_s >= ts_s)
		{
			lastSendTime = actTime;
			return 1;
		}
		return 0;
	}

void sendNextInFolder()
{
	char files[MAXFILENUM][MAXFILESIZE];

	char* folder = AUTOSENDFOLDER;

	int fnum = GetFilesInDirectory(folder, files);
	if (fnum == 0) return;

	if (send_idx >= fnum) send_idx = 0;
	sendFile(files[send_idx++], folder);
}

void sendFile(char* file, char *dir)
{
	// check file type
	int type = 5;	// binary file (default)
	if (strstr(file, ".html") || strstr(file, ".htm"))
		type = 4;
	if (strstr(file, ".blt") || strstr(file, ".txt"))
		type = 3;
	if (strstr(file, ".jpg"))
		type = 2;

	char fullname[MAXFILESIZE * 2];
	sprintf(fullname, "%s/%s", dir, file);

	if (type != 2)
	{
		char zipfn[1024];
		temppath_name(zipfn);
		strcat(zipfn, "myzipfile.zip");
		if (zipfile(fullname, file, zipfn) == -1) return;
		strcpy(fullname,zipfn);
	}

	// fullname ... filename (including path) to be sent
	if (prepare_filesend(fullname, file) == -1) return;

	// data to send are in filearray with length filarr_len

	// check if the complete buffer fits into one frame
	if (filarr_len <= 219)
	{
		//printf("send single frame\n");
		filestatus = 3;	// single frame
		// pack and send PSK data
		// handle repetitions and check if TX was down
		// repeat: 0=do not repeat, 1=repeat if currently not sending, >1 = number of repetitions
		modem_sendPSKData(filearray, type, filestatus, 1, FIFO_FILESEND);
	}
	else
	{
		filestatus = 0; // first frame
		int frameanz = (filarr_len + 218) / 219;
		for (int frame = 0; frame < frameanz; frame++)
		{
			//printf("send frame %d\n",frame);
			if (frame == 0) filestatus = 0;						// first frame
			else if (frame == (frameanz - 1)) filestatus = 2;	// last frame
			else filestatus = 1;								// next frame
			modem_sendPSKData(filearray + frame * 219, type, filestatus, 1, FIFO_FILESEND);

			if (keeprunning == 0) return;
		}
	}
}

int zipfile(char* fullname, char *name, char* zipfilename)
{
	int ret = -1;

	// path for the temporary zip file
	//printf("compress <%s> as <%s> into <%s>\n", fullname, name, zipfilename);
	struct zip_t* zip = zip_open(zipfilename, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
	if(zip)
	{
		if(zip_entry_open(zip, name) == 0)
		{
			zip_entry_fwrite(zip, fullname);
			zip_entry_close(zip);
			ret = 0;
		}
		zip_close(zip);
	}
	
	return ret;
}

int prepare_filesend(char *fullname, char *filename)
{
	printf("send <%s> in <%s>\n", filename, fullname);

	// read the complete file into memory
	int len = 0;
	FILE* fp = fopen(fullname, "rb");
	if (fp)
	{
		// read file content, at the beginning keep header len=55 clear
		len = fread(filearray+55, 1, sizeof(filearray)-55, fp);
		fclose(fp);
	}
	if (len == 0) return -1; // nothing to send

	// add header
	// CRC16 over complete file contents is the file ID
	uint16_t fncrc = Crc16_messagecalc(CRC16FILETX, filearray + 55, len);

	// create the file header
	// 50 bytes ... Filename (or first 50 chars of the filename)
	// 2 bytes .... CRC16 od the filename, this is used as a file ID
	// 3 bytes .... size of file
	memset(filearray, 0, 50);
	int namelen = strlen(filename);
	if (namelen > 50) len = 50;
	memcpy(filearray, filename, namelen);

	filearray[50] = (fncrc >> 8) & 0xff;
	filearray[51] = fncrc & 0xff;

	filearray[52] = (len >> 16) & 0xff;
	filearray[53] = (len >> 8) & 0xff;
	filearray[54] = len & 0xff;

	len += 55;
	// filearray: data to send, len=data length in filearray
	filarr_len = len;

	return 0;
}