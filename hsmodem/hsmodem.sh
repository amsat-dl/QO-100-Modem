#!/bin/bash
echo
echo
echo
echo
echo "this script installs HSmodem on your Linux system"
echo "================================================="
echo
echo "1. Downloads the sources from github"
echo "2. compiles the sources for your local machine and architecture"
echo "3. copy the shared libraries to it's final location"
echo
echo "Install required packages and libraries. Press ENTER ..."
read
sudo apt-get update
sudo apt-get -y install libopus-dev libfftw3-dev libcodec2-dev libsndfile-dev libasound-dev mono-complete pavucontrol git g++ build-essential libsoundio-dev
echo
echo "Download hsmodem sources from github. Press ENTER ... "
read
git clone https://github.com/dj0abr/SSB_HighSpeed_Modem
echo
echo "Compile the sources and build the executable program. Press ENTER ..."
read
pwd
cd SSB_HighSpeed_Modem/hsmodem
pwd
make -j 4
echo
echo "copy the shared libraries to it's final location. Press ENTER ..."
read
echo copy oscardata.exe
pwd
cp ../oscardata/oscardata/bin/Release/oscardata.exe ../hsmodemLinux
# now find the shared library directory
# this is the directory where libopus.so is installed
LIBNAME=$(find /usr -name libopus.so | head -1)
LIBFOLDER=$(dirname $LIBNAME)
ARCHITECTURE=$(uname -m)
echo copy shared libraries for $ARCHITECTURE to $LIBFOLDER
echo
if [ ${ARCHITECTURE} == 'x86_64' ]; 
then
sudo cp ./SharedLibs/$ARCHITECTURE/*.so $LIBFOLDER
echo "finished. Your project is in folder: ./SSB_HighSpeed_Modem/hsmodemLinux"
echo "you can copy this folder to any location"
echo "to run hsmodem:  mono  oscardata.exe"
elif [ ${ARCHITECTURE} == 'aarch64' ]; 
then
sudo cp ./SharedLibs/$ARCHITECTURE/*.so $LIBFOLDER
echo "finished. Your project is in folder: ./SSB_HighSpeed_Modem/hsmodemLinux"
echo "you can copy this folder to any location"
echo "to run hsmodem:  mono  oscardata.exe"
else
    echo ================================================================
    echo error: no shared libs for $ARCHITECTURE . Install them manually.
    echo ================================================================
fi
