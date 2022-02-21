@echo off
rem sum.exe konverterer fra res.hex til ud.hex i 512K prom og l‘gger fra
rem  i adresse  DFFC: version, pakket f.eks 1.2 som Hex 12
rem		DFFD: till‘gstal 2 (normalt 0)
rem		DFFE: till‘gstal 1 (for at g›re low byte i checksum = 0)
rem		DFFF: high byte af checksum

oh51 res
call sum res.hex res.sum 512 FFFB 1.0
echo Tast mig, n†r du er parat til at br‘nde mig af!!!!!
reply
hexobj  res.sum res.bin i 1
