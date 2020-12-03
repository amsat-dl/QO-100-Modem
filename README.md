# QO-100-modem
The purpose of this project is to transfer data (pictures...) via a 2,7kHz SSB channel on the narrow band transponder as fast as possible.

# this is work in progress
Version 0.4 is working on:
Windows 10 (should work on Win7, not tested)
linux Desktop PC, 
Odroid SBC
Raspberry 4 (3B+)

# Prerequisites
* Windopws 10 Desktop PC ... working
* LINUX Desktop PC ... working
* Raspberry PI 4 ... working
* Raspberry PI 3B+ ... working, but not 100% error free in fullduplex mode (RX only or TX only is working)
* Odroid N2 ... working
* Odroid C2 ... working
* Odroid C4 ... working

* Raspberry: Raspian OS ist NOT working, instead Ubuntu 64bit is required

* Application Software "oscardata.exe" running on Windows, Linux, (possibly MAC-OS, not tested)

# 3rd Party Libraries
this software uses these programs:
* Shifra Reed Solomon code: https://www.schifra.com/ (GPL V.2)
* liquid-SDR: https://github.com/jgaeddert/liquid-dsp (MIT License)
* BASS Audio: https://www.un4seen.com/ (free for non-commercial use)
* fftw3: http://www.fftw.org (GPL V.2 or later)
* libcodec2 (Linux: standard lib, Windows: from freeDV)

# Download alternatives
* download from github and build from source
* download windows installer and download Odroid and Raspberry images:
https://wx.spdns.de/german/technik/sat/modem/images.htm


# building the software for Linux
* Linux
1. Download from github the file hsmodem/hsmodem.sh, ONLY ! Do not clone the repository, download ONLY this single file.
2. copy hsmodem.hs to your home directory and make it executable (chmod 755 hsmodem.sh)
3. run the file:  ./hsmodem.sh
this will make download, compile hsmodem an install all required files
the resulting project is in ./SSB_HighSpeed_Modem/hsmodemLinux

# starting the modem and application

you need to run 2 programs, the first one is "hsmodem" which runs in a termimal without GUI. This is the modem doing all modulation and demodulation staff.
The second program is the user interface "oscardata.exe".
oscardata.exe starts hsmodem automatically in the background. If you want to run hsmodem on one machine and oscardata on another then deactivate the autostart in the Setup menu in oscardata.exe

* Windows
if the software was installed with this setup file: 
https://wx.spdns.de/german/technik/sat/modem/images.htm
then you find the program under the program group "amsat" in the Window's start menu

* Linux
1. go into the folder ..../SSB_HighSpeed_Modem/hsmodemLinux
2. run the software:  mono  oscardata.exe

# tested scenarious

* QO-100 via IC-9700, IC-7300 or IC-7100 ... working
* Short Wave 6m band via IC-7300, IC-7100 ... working. In case of significant noise, use the lowest bit rate (3000 bit/s)


# usage

In the IC-9700 activate the DATA mode and the RX filter FIL1 to full range of 3.6kHz.

In oscardata.exe go to the "BER" tab. Then click START. If you change the bitrate, wait a few seconds before starting again.

The program is now sending test data frames to the default sound card. If your sound card is properly connected to the transceiver then switch the transceiver to TX and the data will be sent to QO-100.
Receive your transmission, feed it to the default soundcard. As soon as oscardata.exe detects a correct data frame it will display status messages on the screen.

(For testing purposes you can just connect Line-Out of your soundcard with Line-IN with a cable.)

To assign the soundcard to the modem I recommend to use pavucontrol. Using the TX volume set a signal level of about 20 to 24 dB over noise floor. You will need about -10dB compared to the BPSK400 beacon. The received audio volume can be adjusted with help of the spectrum display in oscardata.exe-

Now as the transmission is OK, you can go to the "Image RX/TX" tab. First, select a picture quality then load a picture and finally press SEND to send it to QO-100. When you correctly receive your own transmission the RX picture will be displayed line by line.

vy 73, DJ0ABR

