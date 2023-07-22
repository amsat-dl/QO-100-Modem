rd /s /q package
mkdir package
copy oscardata\oscardata\bin\Debug\* package
copy hsmodem\SharedLibs\windows\* package
mkdir package\audio
copy hsmodem\audio\* package\audio
popd
popd