@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

c:
cd c:\cxa\examples\c

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee mk32 %1 -a -r -f intrpt.mak > intrpt.err
goto end

:build
ftee mk32 -r -f intrpt.mak > intrpt.err
goto end

:rebuild
ftee mk32 -a -r -f intrpt.mak > intrpt.err

:end
SET PATH=%TMPPATH%
