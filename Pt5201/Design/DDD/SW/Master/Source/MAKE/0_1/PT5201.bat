@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

d:
cd d:\project\pt5201\master\source\make\work_ken

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
