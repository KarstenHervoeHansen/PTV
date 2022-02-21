@ECHO OFF

SET TMPPATH=%PATH%
SET PATH=%PATH%;c:\cxa\bin;c:\bc4\bin

rem d:
rem cd d:\project\pt5201\make\work_ken

ftee make -r -f pt5201_.mak > pt5201.err

SET PATH=%TMPPATH%
