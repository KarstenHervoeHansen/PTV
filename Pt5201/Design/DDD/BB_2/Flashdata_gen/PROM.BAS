OPEN "bbdatam" FOR INPUT AS #1
OPEN "bbdata1.bin" FOR OUTPUT AS #2
OPEN "bbdata2.bin" FOR OUTPUT AS #3
WHILE NOT EOF(1)
 INPUT #1, i0
 INPUT #1, i1
 INPUT #1, i2
 by1 = (i0 MOD 64) * 4 + (i0 \ 64) \ 4
 by2 = ((i0 \ 64) MOD 4) * 64 + (i1 MOD 64)
 by3 = ((i1 \ 64) * 4) + ((i2 MOD 64) \ 16)
 by4 = ((i2 ) MOD 16) * 16 + (i2 \ 64)
 PRINT #2, CHR$(by2); CHR$(by1);
 PRINT #3, CHR$(by4); CHR$(by3);
WEND
close 1
OPEN "bbdatag" FOR INPUT AS #1
WHILE NOT EOF(1)
 INPUT #1, i0
 INPUT #1, i1
 INPUT #1, i2
 by1 = (i0 MOD 64) * 4 + (i0 \ 64) \ 4
 by2 = ((i0 \ 64) MOD 4) * 64 + (i1 MOD 64)
 by3 = ((i1 \ 64) * 4) + ((i2 MOD 64) \ 16)
 by4 = ((i2 ) MOD 16) * 16 + (i2 \ 64)
 PRINT #2, CHR$(by2); CHR$(by1);
 PRINT #3, CHR$(by4); CHR$(by3);
WEND
CLOSE


