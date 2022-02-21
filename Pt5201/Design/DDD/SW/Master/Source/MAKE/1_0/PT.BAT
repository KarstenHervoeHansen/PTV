@ECHO OFF
SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin

make -r -B -f pt5201_.mak %1

SET PATH=%TMPPATH%
