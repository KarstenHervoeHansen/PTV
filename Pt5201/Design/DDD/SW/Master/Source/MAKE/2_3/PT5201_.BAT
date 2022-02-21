@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin;c:\bc4\bin

u:
cd u:\pt5201\design\ddd\sw\master\source\make\2_3

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee make -B -r -f pt5201_.mak %1 > pt5201.err
goto end

:build
ftee make -r -f pt5201_.mak > pt5201.err
goto end

:rebuild
ftee make -B -r -f pt5201_.mak > pt5201.err

:end
SET PATH=%TMPPATH%
