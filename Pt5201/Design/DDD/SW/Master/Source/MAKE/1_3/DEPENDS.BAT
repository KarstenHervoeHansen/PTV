@ECHO OFF
SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin


ftee make -p -n -r -f pt5201_.mak > Depends.txt

SET PATH=%TMPPATH%
