@echo off
rem   bat-fil til generering af DEMO3982.EXE
rem   Kim Engedahl, P-STU, 20.6.1994
rem           940620      Version 1.00

cls
echo ***********  DEMO PC MONITOR SETUP Software   **********
echo .
echo  Placer 3«" 720kb disk i drev D           (8709 001 14371)
echo .
echo  Tast F        Forts‘tte
echo  Tast 9        Afbryd
\norton\ask " Tast D        Disk formattering  ",fFdD9
if errorlevel 5 goto end
if errorlevel 4 call diskfor
if errorlevel 3 call diskfor

rem   Indtast KU-nummer, generer pixel data file med KU-nummer samt
rem   filen SERIAL.INC
:kupix
kupixel
if errorlevel 1 goto error

copy demo3982.org demo3982.exe

rem   Kopierer pixel data file ind i EXE-file
ent3982 demo3982.exe
if errorlevel 0 goto setfiletime
goto error

:setfiletime
rem  S‘tter DOC og EXE-filen's tid = SW version
FILETIME demo3982.EXE 1.00

cls
echo .
echo  Kopiering af DEMO3982.EXE til 3«" disk  
echo .
echo    Tast F      Forts‘t
ask "   Tast 9      Afbryd   ",fF9
if errorlevel 3 goto end

echo .
echo   Kopiering af 1 fil ... vent
COPY DEMO3982.EXE D: /V

cls
dir D:
echo .
echo   Tryk ENTER for at forts‘tte 
getkey

goto ok

:error
echo .
echo FEJL ved generering af EXE file 
echo .
goto end

:ok
echo .
echo .
echo  Husk at afpr›ve programmet p† 3«" disken 
echo  Part Number: 7108 100 73921

:end
echo .
echo   Programmet genstartes ved at taste   MAKE3982
echo .
echo   HUSK  N†r PC'en slukkes tast S ENTER
