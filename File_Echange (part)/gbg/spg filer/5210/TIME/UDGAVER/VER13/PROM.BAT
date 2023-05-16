@echo off
rem sum.exe konverterer fra res.hex til ud.hex i 512K prom og l‘gger fra
rem  i adresse  DFFC: version, pakket f.eks 1.2 som Hex 12
rem		DFFD: till‘gstal 2 (normalt 0)
rem		DFFE: till‘gstal 1 (for at g›re low byte i checksum = 0)
rem		DFFF: high byte af checksum

ohs51 time PURGE
rem call sum time.hex ud.hex 512 FFFC 1.0
echo.
rem echo   HAR DU SKIFTET TIL 8 MHz???
echo.
rem pause
rem hexobj ud.hex prom.bin i
hexobj time.hex prom.bin i
rem del ud.hex
rem eprom4 < promproc
