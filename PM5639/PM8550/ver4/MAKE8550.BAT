rem @echo off
rem   bat-fil til generering af PM8550.EXE
rem   Peter Frederiksen, P-DPE, 9.2.1993
rem   rettet: 23.3.1993   Pix data og KU nummer direkte i EXE file
rem           23.4.1993   Version 1.30
rem           11.5.1993   Kopiering til drev D og E
rem           931201      Version 1.31  (‘ndringsciffer 3)
rem           931019      Version 1.40  (‘ndringsciffer 4)

cls
echo ***********  PM8550 CRT Color Sensor Calibration Software   **********
echo .
echo  Placer 3«" 720kb disk i drev D           (8709 001 14371)
echo         5¬" 360kb disk i drev E           (8709 001 12171)
echo .
echo  Tast F        Forts‘tte
echo  Tast 9        Afbryd
rem \norton\ask " Tast D        Disk formattering  ",fFdD9
if errorlevel 5 goto end
rem if errorlevel 4 call diskfor
rem if errorlevel 3 call diskfor

rem   Indtast KU-nummer, generer pixel data file med KU-nummer samt
rem   filen SERIAL.INC
:kupix
kupixel
if errorlevel 1 goto error

copy pm8550.org pm8550.exe

rem   Kopierer pixel data + serie nummer ind i EXE-file
enterexe pm8550.exe
if errorlevel 0 goto setfiletime
goto error

:setfiletime
rem  S‘tter EXE-filen's tid = SW version
FILETIME PM8550.EXE 1.40

cls
echo .
echo  Kopiering af PM8550.EXE og README til 3«" og 5¬" disk  
echo .
echo    Tast F      Forts‘t
ask "   Tast 9      Afbryd   ",fF9
if errorlevel 3 goto end

echo .
echo   Kopiering af 4 filer ... vent
COPY PM8550.EXE D: /V
COPY PM8550.EXE E: /V
COPY README D: /V
COPY README E: /V

cls
dir D:
echo .
echo   Tryk ENTER for at forts‘tte 
getkey

cls
dir E:
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
echo  Husk at afpr›ve programmerne p† 3«" og 5¬" diskene 
echo  Part Number: 4008 002 950x4

:end
echo .
echo   Programmet genstartes ved at taste   MAKE8550
echo .
echo   HUSK  N†r PC'en slukkes tast S ENTER
