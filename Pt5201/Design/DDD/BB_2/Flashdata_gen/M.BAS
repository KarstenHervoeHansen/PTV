OPEN "bbdatam" FOR OUTPUT AS #1
pi = 3.14159265359#

fsamp = 27000000#

ped = 53.5/300*680
amp = 680*40/140*1000/300
trise = 1.4E-07
black=-680+int(amp+.5)

korr = pi / 2 / ATN(.8 / SQR(1 - .8 * .8))
test = SIN(korr)
PRINT , korr, test

fsubc = fsamp/858/2*455/2
tbstart = .0000053
tbslut = .000007822
tsampstart = .0000049
tburise = .0000004#
buamp = 340*40/140*1000/300

bukorr = pi / 2 / ATN(.8 / SQR(1 - .8 * .8)) * 1
bustig = tburise / 2 * bukorr

FOR fasein = 0 TO 2047 STEP 1

 tstart = 1 / 2.7E+07 / 2048 * fasein + 1 / 2.7E+07 * 7
 bufase = 2 * pi * fasein / 2048
 fi = 2 * pi * fasein / 2048#
 FOR n = 0 TO 95
  t = n / fsamp + tsampstart
  IF t < tbstart - bustig THEN
   burst = amp
  ELSE
   IF t < tbstart + bustig THEN
    a = buamp * (SIN((t - tbstart) / bustig * pi / 2) / 2 + 1 / 2)
    burst = a * SIN(2 * pi * t * fsubc + fi) + amp
   ELSE
    IF t < tbslut - bustig THEN
     burst = buamp * SIN(2 * pi * t * fsubc + fi) + amp
    ELSE
     IF t < tbslut + bustig THEN
      a = buamp * (SIN((-t + tbslut) / bustig * pi / 2) / 2 + 1 / 2)
      burst = a * SIN(2 * pi * t * fsubc + fi) + amp
     ELSE
      burst = amp
     END IF
    END IF
   END IF
  END IF
  IF burst > 1023 THEN burst = 1023
  PRINT #1, 1023-INT(burst + .5)+black
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
  PRINT #1, 1023-INT(sync + .5)+black
 NEXT b
 FOR b = 15 TO 23
  sync = 0
  PRINT #1, 1023-INT(sync + .5)+black
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
  PRINT #1, 1023-INT(sync + .5)+black
 NEXT b

 FOR b = 39 TO 47
  sync = amp
  PRINT #1, 1023-INT(sync + .5)+black
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
  PRINT #1, 1023-INT(sync + .5)+black
 NEXT b
 FOR b = 63 TO 71
  sync = amp + ped
  PRINT #1, 1023-INT(sync + .5)+black
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
  PRINT #1, 1023-INT(sync + .5)+black
 NEXT b
 FOR b = 87 TO 95
  sync = amp
  PRINT #1, 1023-INT(sync + .5)+black
 NEXT b

NEXT fasein
CLOSE all

