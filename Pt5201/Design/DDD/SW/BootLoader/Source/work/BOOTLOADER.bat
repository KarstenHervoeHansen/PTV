@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

u:
cd u:\pt5201\design\ddd\sw\bootloader\source\work

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee mk32 %1 -a -r -f BOOTLOADER.mak > BOOTLOADER.err
goto end

:build
ftee mk32 -r -f BOOTLOADER.mak > BOOTLOADER.err
goto end

:rebuild
ftee mk32 -a -r -f BOOTLOADER.mak > BOOTLOADER.err

:end
SET PATH=%TMPPATH%
