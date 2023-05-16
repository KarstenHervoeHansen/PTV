@echo off
rem sum.exe konverterer fra res.hex til ud.hex i 512K prom og l‘gger fra
rem  i adresse  DFFC: version, pakket f.eks 1.2 som Hex 12
rem		DFFD: till‘gstal 2 (normalt 0)
rem		DFFE: till‘gstal 1 (for at g›re low byte i checksum = 0)
rem		DFFF: high byte af checksum

ohs51 res PURGE
rem call sum master.hex ud.hex 512 DFFC 1.0
rem copy master.hex ud.hex
rem erase master.hex
echo.
echo  HAR DU HUSKET AT SKIFTE TIL 8 MHz ???
echo.
pause
hexobj res.hex ud.b i
eprom4 < promproc
