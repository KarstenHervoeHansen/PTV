@echo off
:start
cls
echo .
echo           -------------- Disk formattering -------------
echo .
echo      Tryk A     drev A          (8709 001 14471)
echo .
echo      Tryk 9     Ingen formattering
echo .
ask " Tryk en tast for valg  ",aA9
if errorlevel 3 goto end
if errorlevel 2 format A:
if errorlevel 1 format A:
echo .
ask 'Formattering af endnu en disk ?  J/N  ',jJnN
if errorlevel 4 goto end
if errorlevel 3 goto end
if errorlevel 2 goto start
if errorlevel 1 goto start
:end

