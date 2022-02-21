@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

c:
cd c:\work\5220\sync

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee mk32 %1 -a -r -f sync.mak > sync.err
goto end

:build
ftee mk32 -r -f sync.mak > sync.err
goto end

:rebuild
ftee mk32 -a -r -f sync.mak > sync.err

:end
SET PATH=%TMPPATH%
