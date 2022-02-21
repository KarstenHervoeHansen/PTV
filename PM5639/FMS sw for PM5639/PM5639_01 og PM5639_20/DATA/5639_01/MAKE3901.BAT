@echo off
cls
rem   bat-fil til generering af PM39_01.EXE og kopiering af
rem   moduler i C, Pascal og Basic.
rem
rem   Opr. PF, P-DPE
rem   Rev. af: KEn, P-DPE, 931130
cls
echo ***********  PM5639/01 CRT Color Sensor Software, PC   **********
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
\data\5639_20\kupixel
if errorlevel 1 goto error

copy pm39_01.org pm39_01.exe

rem   Kopierer pixel data file ind i EXE-file
\data\5639_20\ent3920 pm39_01.exe
if errorlevel 0 goto setfiletime
goto error

:setfiletime
rem  S‘tter EXE-filen's tid = SW version
FILETIME PM39_01.EXE 3.00

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

COPY PM39_01.EXE  A: /V
COPY \data\5639_20\PM5639.HLP   A: /V
COPY \data\5639_20\PM5639.NDX   A: /V
COPY FILELIST.DOC A: /V

COPY \data\5639_20\borlandc\DEMO.EXE  A:\borlandc /V
COPY \data\5639_20\borlandc\DEMO.C    A:\borlandc /V
COPY \data\5639_20\borlandc\PM39IO.C  A:\borlandc /V
COPY \data\5639_20\borlandc\PM39IO.H  A:\borlandc /V
COPY \data\5639_20\borlandc\DEMO.PRJ  A:\borlandc /V

COPY \data\5639_20\pascal\DEMO.EXE    A:\pascal /V
COPY \data\5639_20\pascal\DEMO.PAS    A:\pascal /V
COPY \data\5639_20\pascal\PM39IO.PAS  A:\pascal /V

COPY \data\5639_20\basic\DEMO.EXE A:\basic /V
COPY \data\5639_20\basic\DEMO.BAS A:\basic /V

cls
echo .
echo   Kopiering af 14 filer til drev B ... vent
md B:\borlandc
md B:\pascal
md B:\basic

COPY PM39_01.EXE  B: /V
COPY \data\5639_20\PM5639.HLP   B: /V
COPY \data\5639_20\PM5639.NDX   B: /V
COPY FILELIST.DOC B: /V

COPY \data\5639_20\borlandc\DEMO.EXE  B:\borlandc /V
COPY \data\5639_20\borlandc\DEMO.C    B:\borlandc /V
COPY \data\5639_20\borlandc\PM39IO.C  B:\borlandc /V
COPY \data\5639_20\borlandc\PM39IO.H  B:\borlandc /V
COPY \data\5639_20\borlandc\DEMO.PRJ  B:\borlandc /V

COPY \data\5639_20\pascal\DEMO.EXE    B:\pascal /V
COPY \data\5639_20\pascal\DEMO.PAS    B:\pascal /V
COPY \data\5639_20\pascal\PM39IO.PAS  B:\pascal /V

COPY \data\5639_20\basic\DEMO.EXE B:\basic /V
COPY \data\5639_20\basic\DEMO.BAS B:\basic /V





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
echo    Part Number: 4008 002 95061  3«"
echo      -     -    4008 002 95071  5¬"
echo .
echo    Tast A:         tryk ENTER
echo    Tast PM39_01    tryk ENTER
echo .
echo    Tast B:         tryk ENTER
echo    Tast PM39_01    tryk ENTER
echo .
:end
echo .
echo     Programmet genstartes ved at taste    MAKE3901  ENTER
echo .
echo     HUSK:  Inden PC'en slukkes tast S  ENTER
