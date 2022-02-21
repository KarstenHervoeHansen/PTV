OPEN "bbdatag" FOR OUTPUT AS #1
pi = 3.14159265359#

fsamp = 27000000#

ped = 100
amp = 680
trise = 2.3E-07

korr = pi / 2 / ATN(.8 / SQR(1 - .8 * .8))
test = SIN(korr)
PRINT , korr, test

fsubc = 4433618.75#
tbstart = .0000056
tbslut = .000008-.00000015
tsampstart = .0000052
tburise = .0000004#
buamp = 340

bukorr = pi / 2 / ATN(.8 / SQR(1 - .8 * .8)) * 1
bustig = tburise / 2 * bukorr

FOR fasein = 0 TO 2047 STEP 1

 tstart = 1 / 2.7E+07 / 2048 * fasein + 1 / 2.7E+07 * 7
 bufase = 2 * pi * fasein / 2048
 fi = 2 * pi * fasein / 2048#
 FOR n = 0 TO 95
  t = n / fsamp + tsampstart
  IF t < tbstart - bustig THEN
   burst = 680
  ELSE
   IF t < tbstart + bustig THEN
    a = buamp * (SIN((t - tbstart) / bustig * pi / 2) / 2 + 1 / 2)
    burst = a * SIN(2 * pi * t * fsubc + fi) + 680
   ELSE
    IF t < tbslut - bustig THEN
     burst = buamp * SIN(2 * pi * t * fsubc + fi) + 680
    ELSE
     IF t < tbslut + bustig THEN
      a = buamp * (SIN((-t + tbslut) / bustig * pi / 2) / 2 + 1 / 2)
      burst = a * SIN(2 * pi * t * fsubc + fi) + 680
     ELSE
      burst = 680
     END IF
    END IF
   END IF
  END IF
  IF burst > 1023 THEN burst = 1023
  PRINT #1, 1023-INT(burst + .5)
 NEXT n

 FOR b = 0 TO 14
  t = 1 / 2.7E+07 * b
  arg = (t - tstart) / trise * 2 / korr * pi / 2
  IF arg < -pi / 2 THEN
   sync = amp
  END IF
  IF arg > pi / 2 THEN
   sync = 0
  END IF
  IF arg > -pi / 2 AND arg < pi / 2 THEN
   sync = -amp * SIN(arg) / 2 + amp / 2
  END IF
  PRINT #1, 1023-INT(sync + .5)
 NEXT b
 FOR b = 15 TO 23
  sync = 0
  PRINT #1, 1023-INT(sync + .5)
 NEXT b

 FOR b = 24 TO 38
  t = 1 / 2.7E+07 * (b - 24)
  arg = (t - tstart) / trise * 2 / korr * pi / 2
  IF arg < -pi / 2 THEN
   sync = 0
  END IF
  IF arg > pi / 2 THEN
   sync = amp
  END IF
  IF arg > -pi / 2 AND arg < pi / 2 THEN
   sync = amp * SIN(arg) / 2 + amp / 2
  END IF
  PRINT #1, 1023-INT(sync + .5)
 NEXT b

 FOR b = 39 TO 47
  sync = amp
  PRINT #1, 1023-INT(sync + .5)
 NEXT b

 FOR b = 48 TO 62
  t = 1 / 2.7E+07 * (b - 48)
  arg = (t - tstart) / trise * 2 / korr * pi / 2
  IF arg < -pi / 2 THEN
   sync = amp
  END IF
  IF arg > pi / 2 THEN
   sync = amp + ped
  END IF
  IF arg > -pi / 2 AND arg < pi / 2 THEN
   sync = ped * SIN(arg) / 2 + amp + ped / 2
  END IF
  PRINT #1, 1023-INT(sync + .5)
 NEXT b
 FOR b = 63 TO 71
  sync = amp + ped
  PRINT #1, 1023-INT(sync + .5)
 NEXT b

 FOR b = 72 TO 86
  t = 1 / 2.7E+07 * (b - 72)
  arg = (t - tstart) / trise * 2 / korr * pi / 2
  IF arg < -pi / 2 THEN
   sync = amp + ped
  END IF
  IF arg > pi / 2 THEN
   sync = amp
  END IF
  IF arg > -pi / 2 AND arg < pi / 2 THEN
   sync = -ped * SIN(arg) / 2 + amp + ped / 2
  END IF
  PRINT #1, 1023-INT(sync + .5)
 NEXT b
 FOR b = 87 TO 95
  sync = amp
  PRINT #1, 1023-INT(sync + .5)
 NEXT b

NEXT fasein
CLOSE all

