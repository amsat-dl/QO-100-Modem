# makefile for hsmodem on Linux (PC desktop and ARM)
# usage:
# make ... builds the program and copies all needed files to ../hsmodemLinux
# the shared libs are also copied to this location
# to distribute hsmodem you need the folder hsmodemLinux and its subfolders
# installation: when these files are copied to the target system,
# then (in the target system) run "sudo install". 
# This will copy the sharded libs to its final location.

CXXFLAGS = -Wall -O3 -std=c++0x -Wno-write-strings -Wno-narrowing
LDFLAGS = -lpthread -lrt -lsndfile -lasound -lm -lopus -lfftw3 -lfftw3_threads -lliquid -lcodec2 -lsoundio
OBJ = hsmodem.o constellation.o crc16.o frame_packer.o main_helper.o\
scrambler.o speed.o fec.o udp.o fft.o liquid_if.o symboltracker.o\
voiceprocessor.o codec2.o fifo.o announcement.o tuning.o rtty.o volume.o\
libkmaudio/libkmaudio_fifo.o\
libkmaudio/libkmaudio_getDevices.o\
libkmaudio/libkmaudio_getDevices_Linux.o\
libkmaudio/libkmaudio_init.o\
libkmaudio/libkmaudio_init_linux.o\
libkmaudio/libkmaudio_interface.o\
libkmaudio/libkmaudio_capture_linux.o\
libkmaudio/libkmaudio_playback_linux.o\
libkmaudio/libkmaudio_resampler.o\
websocket/ws.o\
websocket/ws_callbacks.o\
websocket/websocketserver.o\
websocket/sha1.o\
websocket/base64.o\
websocket/handshake.o\
extdata.o distrubution.o kmtimer.o

default: $(OBJ)
	mkdir -p ../hsmodemLinux
	cp ../oscardata/oscardata/bin/Release/oscardata.exe ../hsmodemLinux
	g++ $(CXXFLAGS) -o ../hsmodemLinux/hsmodem $(OBJ) $(LDFLAGS)

clean:
	rm -r *.o
	rm -r libkmaudio/*.o
	rm -r websocket/*.o
