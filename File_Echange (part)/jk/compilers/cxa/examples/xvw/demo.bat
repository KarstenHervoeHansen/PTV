@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

c:
cd c:\cxa\examples\xvw

if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild

ftee mk32 %1 -a -r -f demo.mak > demo.err
goto end

:build
ftee mk32 -r -f demo.mak > demo.err
goto end

:rebuild
ftee mk32 -a -r -f demo.mak > demo.err

:end
SET PATH=%TMPPATH%
