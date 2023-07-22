del m0lte-amsat-dl-hsmodem.zip
rd /s /q tmp
mkdir tmp
copy oscardata\oscardata\bin\Debug\* tmp
copy hsmodem\SharedLibs\windows\* tmp
mkdir tmp\audio
copy hsmodem\audio\* tmp\audio
pushd tmp
"C:\Program Files\7-Zip\7z" a ..\m0lte-amsat-dl-hsmodem.zip *
popd
rd /s /q tmp