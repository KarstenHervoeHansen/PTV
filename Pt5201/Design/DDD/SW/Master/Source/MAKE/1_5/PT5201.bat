@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

u:
cd u:\pt5201\design\ddd\sw\master\source\make\1_5

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee mk32 %1 -a -f pt5201_.mak > PT5201.err
goto end

:build
ftee mk32 -f pt5201_.mak > PT5201.err
goto end

:rebuild
ftee mk32 -a -f pt5201_.mak > PT5201.err

:end
SET PATH=%TMPPATH%
