del m0lte-amsat-dl-hsmodem.zip
rd /s /q package
mkdir package
copy oscardata\oscardata\bin\Debug\* package
copy hsmodem\SharedLibs\windows\* package
mkdir package\audio
copy hsmodem\audio\* package\audio
pushd package
"C:\Program Files\7-Zip\7z" a ..\m0lte-amsat-dl-hsmodem.zip *
popd