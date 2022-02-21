@ECHO OFF

PROMPT $P$G

PATH C:\SYSTEM;C:\NORTON;C:\DATA\PRINTER;C:\BATCH;C:\ARJ;C:\FASTBACK

C:\SYSTEM\MODE CON CODEPAGE PREPARE=((865) C:\SYSTEM\EGA.CPI)
C:\SYSTEM\KEYB DK,865,C:\SYSTEM\KEYBOARD.SYS
DOSKEY /INSERT

SET FASTBACK=C:\FASTBACK

:select
cls
cd c:\data
echo                       V‘lg produkt:
echo .
echo                       Tast 1     PM5639/20              version 3.10
echo                       Tast 2     PM8550                 version 1.40
echo                       Tast 3     PM8550/901             version 1.00 
echo                       Tast 4     PM5639/20 908
echo                       Tast 5     PM5639/01              version 3.10
echo                       Tast 6     DEMO PC MONITOR SETUP  version 1.00
echo                       Tast 7     PM5639/914             version 1.00
echo .
echo                       Tast P     Print label
echo .
echo                       Tast S     Shutdown / park HD
echo .
echo                       Tast V     VirusCheck version 7.06 (950104)
echo.
echo                       Tast N     Norton Commander Ver 3.0
echo.
echo                       Tast F     Backup af harddisk m. FastBackup
echo.
ask "                      Tast 9     Til DOS  ",1234567pP9SvVnNfF
if errorlevel 17 goto fastbackup
if errorlevel 16 goto fastbackup
if errorlevel 15 goto nortoncom
if errorlevel 14 goto nortoncom
if errorlevel 13 goto viruscheck
if errorlevel 12 goto viruscheck
if errorlevel 11 goto shutdown
if errorlevel 10 goto slut
if errorlevel 9 goto printlabel
if errorlevel 8 goto printlabel
if errorlevel 7 goto pm39_914
if errorlevel 6 goto demo3982
if errorlevel 5 goto pm3901
if errorlevel 4 goto pm39_908
if errorlevel 3 goto pm8550_901
if errorlevel 2 goto pm8550
if errorlevel 1 goto pm3920


:pm3920
cd \data\5639_20
make3920


:pm8550
cd \data\8550
make8550

:pm8550_901
cd \data\8550_901
make8550


:printlabel
prtlabel
goto select

:pm39_908
cd \data\5639_20.908
make3920

:pm3901
cd \data\5639_01
make3901

:demo3982
cd \data\demo3982
make3982

:pm39_914
cd \data\5639.914
mk39_914

:shutdown
shutdown
goto slut

:viruscheck
cd \toolkit
call anti
goto select

:fastbackup
call fb @c:\fastback\keys.fb
goto select

:nortoncom
NC


:slut

