@echo off
:start
cls
echo .
echo           -------------- Disk formattering -------------
echo .
echo      Tryk A     drev A
echo      Tryk B     drev B
echo .
echo      Tryk 9     ingen formattering
echo .
ask ' Tryk en tast for valg  ',abAB9
if errorlevel 5 goto end
if errorlevel 4 format b: /4
if errorlevel 3 format a: /f:720
if errorlevel 2 format b: /4
if errorlevel 1 format a: /f:720
echo .
ask 'Formattering af endnu en disk ?  J/N  ',jJnN
if errorlevel 4 goto end
if errorlevel 3 goto end
if errorlevel 2 goto start
if errorlevel 1 goto start
:end

