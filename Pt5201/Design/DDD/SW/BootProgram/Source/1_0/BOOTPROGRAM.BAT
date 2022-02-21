@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

u:
cd u:\pt5201\design\ddd\sw\bootprogram\source\work

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee mk32 %1 -a -r -f BOOTPROGRAM.mak > BOOTPROGRAM.err
goto end

:build
ftee mk32 -r -f BOOTPROGRAM.mak > BOOTPROGRAM.err
goto end

:rebuild
ftee mk32 -a -r -f BOOTPROGRAM.mak > BOOTPROGRAM.err

:end
SET PATH=%TMPPATH%
