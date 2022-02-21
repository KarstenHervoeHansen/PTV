REM Dette program beregner fase korrektionen i forhold til 4 gange subc
REM og 13.5 og PALG

OPEN "FASEKORG.TAB" FOR OUTPUT AS #1
OPTION BASE 0
DIM ka&(1023)
pi = 3.14159265359#
freq = 4433618.75#
fsamp = 13500000#
REM fsamp=1135#*15625#
FOR inputfase = 0 TO 360 STEP .1
 s1 = SIN(inputfase / 360 * 2 * pi)
 s2 = SIN(inputfase / 360 * 2 * pi + 1 * 2 * pi * freq / fsamp)
 s3 = SIN(inputfase / 360 * 2 * pi + 2 * 2 * pi * freq / fsamp)
 s4 = SIN(inputfase / 360 * 2 * pi + 3 * 2 * pi * freq / fsamp)
 x = s1 - s3
 y = s2 - s4
 cal2fase = ATN((COS(2 * pi * freq / fsamp) - x / y * COS(2 * 2 * pi * freq / fsamp)) / (SIN(2 * pi * freq / fsamp) - x / y * SIN(2 * 2 * pi * freq / fsamp)))
 cal2fase = cal2fase * 180 / pi
 IF -x < 0 AND -y > 0 AND cal2fase < 0 THEN
  cal2fase = cal2fase + 180
 END IF
 IF (-x > 0 AND cal2fase > 0) OR (-x > 0 AND -y > 0) THEN
  cal2fase = cal2fase + 180
 END IF

 IF x < 0 THEN
  IF y > 0 THEN
   cal1fase = 180 / pi * ATN(-x / y) - 270
  ELSE
   cal1fase = 180 / pi * ATN(-y / -x) - 180
  END IF
 ELSE
  IF x = 0 THEN
   IF y > 0 THEN
    cal1fase = -270
   ELSE
    cal1fase = -90
   END IF
  ELSE
   IF y < 0 THEN
    cal1fase = 180 / pi * ATN(x / -y) - 90
   ELSE
    cal1fase = 180 / pi * ATN(y / x)
   END IF
  END IF
 END IF
REM  cal1fase = (-270 - cal1fase) de 90 grader
 WHILE cal1fase < 0
  cal1fase = cal1fase + 360
 WEND
 WHILE cal1fase > 360
  cal1fase = cal1fase - 360
 WEND
 fasekorr = 90 - (cal1fase + cal2fase)
 WHILE fasekorr < 0
  fasekorr = fasekorr + 360
 WEND
 WHILE fasekorr > 360
   fasekorr = fasekorr - 360
 WEND
 outfase = fasekorr + cal1fase
 WHILE outfase < 0
  outfase = outfase + 360
 WEND
 WHILE outfase > 360
   outfase = outfase - 360
 WEND

 ka&(INT(cal1fase / 360 * 1024 + .5) MOD 1024) = INT(outfase / 360 * 1024 * 64 + .5)
REM PRINT #1, USING "####.####"; inputfase; cal1fase ; cal2fase; fasekorr;outfase
NEXT inputfase
 FOR i = 0 TO 1023
 PRINT #1, ka&(i);",";
 if (i mod 16 )= 0 then print #1,
 NEXT i
CLOSE #1

