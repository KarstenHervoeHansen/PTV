@echo off
cls
rem   bat-fil til generering af PM39_20.EXE og kopiering af
rem   moduler i C, Pascal og Basic.
rem
rem   Peter Frederiksen, P-DPE, 5.4.1993
rem   rettet: 930504  C og Pascal filer til f›rste 360k disk
rem                   hoved program til anden 360k disk
rem           930504  Version 1.01  -  basic ER inkluderet
rem   rettet: 930511  Kopiering til diskformat:   A = 1.44Mb   B = 1.2Mb
rem   rettet: 930706  SW version 1.25
rem   rettet: 930922  SW version 2.00 (KEn)
rem   rettet: 930927  SW version 2.05 (KEn)
rem   rettet: 931201  SW version 3.00 (KEn)
rem
cls
echo ***********  PM5639/20 Industrial CRT Color Sensor Software   **********
echo .
echo   Placer  3«" HD 1.44Mb disk i drev A        (8709 001 14471)
echo           5¬" HD 1.2Mb disk i drev B         (8709 001 12371)
echo .
echo   Tast F        Forts‘tte
echo   Tast 9        Afbryd
\norton\ask "  Tast D        Disk formattering  ",fFdD9
if errorlevel 5 goto end
if errorlevel 4 call diskfor
if errorlevel 3 call diskfor

rem     Indtast KU-nummer, generer pixel data file med KU-nummer samt
rem     filen SERIAL.INC
:kupix
kupixel
if errorlevel 1 goto error

copy pm39_20.org pm39_20.exe

rem   Kopierer pixel data file ind i EXE-file
ent3920 pm39_20.exe
if errorlevel 0 goto setfiletime
goto error

:setfiletime
rem  S‘tter EXE-filen's tid = SW version
FILETIME PM39_20.EXE 3.00

cls
echo .
echo  Kopiering af filer til 3«" og 5¬" disk  
echo .
echo    Tast F      Forts‘t
ask "   Tast 9      Afbryd   ",fF9
if errorlevel 3 goto end

:kopiering
cls
echo .
echo   Kopiering af 14 filer til drev A ... vent
md A:\borlandc
md A:\pascal
md A:\basic

COPY PM39_20.EXE  A: /V
COPY PM5639.HLP   A: /V
COPY PM5639.NDX   A: /V
COPY FILELIST.DOC A: /V

COPY borlandc\DEMO.EXE  A:\borlandc /V
COPY borlandc\DEMO.C    A:\borlandc /V
COPY borlandc\PM39IO.C  A:\borlandc /V
COPY borlandc\PM39IO.H  A:\borlandc /V
COPY borlandc\DEMO.PRJ  A:\borlandc /V

COPY pascal\DEMO.EXE    A:\pascal /V
COPY pascal\DEMO.PAS    A:\pascal /V
COPY pascal\PM39IO.PAS  A:\pascal /V

COPY basic\DEMO.EXE A:\basic /V
COPY basic\DEMO.BAS A:\basic /V

cls
echo .
echo   Kopiering af 14 filer til drev B ... vent
md B:\borlandc
md B:\pascal
md B:\basic

COPY PM39_20.EXE  B: /V
COPY PM5639.HLP   B: /V
COPY PM5639.NDX   B: /V
COPY FILELIST.DOC B: /V

COPY borlandc\DEMO.EXE  B:\borlandc /V
COPY borlandc\DEMO.C    B:\borlandc /V
COPY borlandc\PM39IO.C  B:\borlandc /V
COPY borlandc\PM39IO.H  B:\borlandc /V
COPY borlandc\DEMO.PRJ  B:\borlandc /V

COPY pascal\DEMO.EXE    B:\pascal /V
COPY pascal\DEMO.PAS    B:\pascal /V
COPY pascal\PM39IO.PAS  B:\pascal /V

COPY basic\DEMO.EXE B:\basic /V
COPY basic\DEMO.BAS B:\basic /V





cls
echo   Fil oversigt for drev A
dir A:
echo .
echo   Tryk ENTER for at forts‘tte
getkey

cls
echo   Fil oversigt for drev B
dir B:
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
echo    Husk at afpr›ve programmerne p† 3«" og 5¬" diskene
echo    Part Number: 4008 002 95043  3«"
echo      -     -    4008 002 95053  5¬"
echo .
echo    Tast A:         tryk ENTER
echo    Tast PM39_20    tryk ENTER
echo .
echo    Tast B:         tryk ENTER
echo    Tast PM39_20    tryk ENTER
echo .
:end
echo .
echo     Programmet genstartes ved at taste    MAKE3920  ENTER
echo .
echo     HUSK:  Inden PC'en slukkes tast S  ENTER
