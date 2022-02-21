@ECHO OFF

PROMPT $P$G

PATH C:\SYSTEM;C:\NORTON;C:\DATA\PRINTER

C:\SYSTEM\MODE CON CODEPAGE PREPARE=((865) C:\SYSTEM\EGA.CPI)
C:\SYSTEM\KEYB DK,865,C:\SYSTEM\KEYBOARD.SYS
DOSKEY /INSERT

:select
cls
cd c:\data
echo                       V‘lg produkt:
echo .
echo                       Tast 1     PM5639/20      version 3.00
echo                       Tast 2     PM8550
echo                       Tast 3     PM5639/20 908
echo                       Tast 4     PM5639/01      version 3.00
echo .
echo                       Tast P     Print label
echo .
echo                       Tast S     Shutdown / park HD
echo .
ask "                      Tast 9     Til DOS  ",1234pP9S
if errorlevel 8 goto shutdown
if errorlevel 7 goto slut
if errorlevel 6 goto printlabel
if errorlevel 5 goto printlabel
if errorlevel 4 goto pm3901
if errorlevel 3 goto pm3920_908
if errorlevel 2 goto pm8550
if errorlevel 1 goto pm3920


:pm3920
cd \data\5639_20
make3920


:pm8550
cd \data\8550
make8550

:printlabel
prtlabel
goto select

:pm3920_908
cd \data\5639_20.908
make3920

:pm3901
cd \data\5639_01
make3901

:shutdown
shutdown

:slut
