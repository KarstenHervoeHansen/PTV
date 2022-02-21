rem Dette program beregner fasekorrektionen p† de forskellige sampel
rem positioner for PALG ved 13.5M

DEFDBL A-Z
pi = 3.14159265359#
freq = 4433618.75#
fsamp = 13500000#
OPEN "faseposg.s" FOR OUTPUT AS #1
FOR X = 0 TO 863
 fase = X * freq / fsamp
 WHILE fase >= 1
  fase = fase - 1
 WEND
rem fase1 = INT(fase * 256)
rem fase2 = fase * 256 - fase1
rem fase2 = INT(fase2 * 256)
rem PRINT #1, fase1 , fase2
if i=16 then
 i=0
 PRINT #1, int(fase*1024*64+.5);","
else
 i=i+1
 PRINT #1, int(fase*1024*64+.5);",";
end if
NEXT X
CLOSE 1

