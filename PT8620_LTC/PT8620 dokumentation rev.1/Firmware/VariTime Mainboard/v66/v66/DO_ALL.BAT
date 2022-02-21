@echo off
echo Deleting old files
del *.obj
del pt5230.err
echo Compiling
make
echo Waiting 5 sec
PING 127.0.0.1 -n 6 > nul
echo Deleting bin file
del pt5300.bin
echo Creating new bin with correct checksum
h2b pt5300.hex
xcopy pt5300.bin pt5300binold.bin
dksum pt5300.bin
pause