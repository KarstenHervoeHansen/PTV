@ECHO OFF

PROMPT $P$G

PATH C:\SYSTEM;C:\NORTON;C:\DATA\PRINTER;C:\BATCH;C:\ARJ;;C:\FASTBACK

C:\SYSTEM\MODE CON CODEPAGE PREPARE=((865) C:\SYSTEM\EGA.CPI)
C:\SYSTEM\KEYB DK,865,C:\SYSTEM\KEYBOARD.SYS
DOSKEY /INSERT

SET FASTBACK=C:\FASTBACK

:select
cls
cd c:\data
echo                       Vlg produkt:
echo .
echo                       Tast 1     PM5639/20      version 3.05
echo                       Tast 2     PM8550
echo                       Tast 3     PM5639/20 908
echo                       Tast 4     PM5639/01      version 3.05
echo .
echo                       Tast P     Print label
echo .
echo                       Tast S     Shutdown / park HD
echo .
echo                       Tast V     VirusCheck version 6.55 (940414)
echo.
echo                       Tast N     Norton Commander Ver 3.0
echo.
echo                       Tast F     Backup af harddisk m. FastBackup
echo.
ask "                      Tast 9     Til DOS  ",1234pP9SvVnNfF
if errorlevel 14 goto fastbackup
if errorlevel 13 goto fastbackup
if errorlevel 12 goto nortoncom
if errorlevel 11 goto nortoncom
if errorlevel 10 goto viruscheck
if errorlevel 9 goto viruscheck
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

