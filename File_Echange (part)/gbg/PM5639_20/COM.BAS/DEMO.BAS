5  'DEMONSTRATION WRITTEN IN QUICKBASIC 4.5

10 COMPORT$ = "COM1"              'Serial Communication port COM1
20 COMNO = 1                      '            -"-
25 COMBAUD = 4800                 'Baudrate for COM1

30 'COMPORT$ = "COM2"             'Serial Communication port COM2
40 'COMNO = 2                     '            -"-
45 'COMBAUD = 4800                'Baudrate for COM2

50 MAXTIMEOUT = 3                  'Timeout value for 3 seconds.

60 DIM WHR(5)                      'Array for CIE coordinates, (xy, uv & u'v')
70 DIM IDINFO$(4)                  'Array for Identity Information from the
											  'Color Sensor
80 DIM IDPOS(2)                    'Temporary Array.
90 DIM ERRORTXT$(8)

100 'Error messages:

110 ERRORTXT$(0) = "No error"
115 ERRORTXT$(1) = "Serial COM port not supported"
120 ERRORTXT$(2) = "Timeout receiving data from serial COM port"
130 ERRORTXT$(3) = "Integration time out of range"
140 ERRORTXT$(4) = "White reference values NOT calculated"
150 ERRORTXT$(5) = "Overload"
160 ERRORTXT$(6) = "Lowlight"
170 ERRORTXT$(7) = "Wrong sensor"
175 ERRORTXT$(8) = "Wrong baudrate"


180 ON ERROR GOTO 2000              'Error handler routine


190 '
200 ' ******** Main program ********
210 '

220 COLOR 7, 1, 1
230 CLS

240 COLOR 0, 7, 7
250 LOCATE 1, 1
260 PRINT "              ProTeleVision Technologies A/S, DK-2605 Broendby                  "

270 COLOR 14, 1, 1
280 LOCATE 2, 3
290 PRINT "            PM5639 Industrial Color Sensor Demonstration Program                    "

300 COLOR 15, 1, 1
310 LOCATE 5, 2
320 PRINT "      Shows the measurements from the PM5639 Industrial Color Sensor "

330 LOCATE 7, 2
340 PRINT "                         in CIE 1931 XYZ coordinates:"

350 LOCATE 13, 2
360 PRINT "                         in CIE 1931 xyY coordinates:"

370 LOCATE 19, 9
380 PRINT "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄSTATUSÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿"

390 LOCATE 20, 9
400 PRINT "³                                                              ³"

410 LOCATE 21, 9
420 PRINT "³                                                              ³"

430 LOCATE 22, 9
440 PRINT "³                                                              ³"

450 LOCATE 23, 9
460 PRINT "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"

470 COLOR 14, 1, 1
480 LOCATE 9, 22
490 PRINT "       X         Y         Z"

500 LOCATE 15, 22
510 PRINT "       x         y         Y"

520 LOCATE 24, 9
530 PRINT "                      PRESS  <ESC>  TO EXIT";

540 COLOR 15, 1, 1

550 LOCATE 21, 21
560 PRINT "Initializing Color Sensor. Please Wait!"

570 GOSUB 1000                   'Initialize the serial COM port

575 IF COMERROR = 0 THEN

580 INTEGRATIONTIME = 25
590 GOSUB 1200                   'Set integration time in the Color Sensor
											'(a value of 25 gives app. 10 measurements/s)
610 IF COMERROR = 0 THEN

620 GOSUB 1300                   'Get integration time from the Color Sensor
630 IF COMERROR = 0 THEN

640 GOSUB 1400                   'Get ID from the Color Sensor
650 IF COMERROR = 0 THEN

660 GOSUB 1100                   'Start measuring in the Color Sensor

670 DO

680 GOSUB 1800:                  'Receive LATEST measurement

690 IF COMERROR = 0 THEN
700 IF DATAREADY = 1 THEN

710 LOCATE 21, 12
720 PRINT "                                                         "

730 DATAREADY = 0

740 LOCATE 10, 22
750 PRINT USING " ######.## ######.## ######.##"; X; Y; Z

760 LOCATE 16, 22
770 PRINT USING " #####.### #####.### ######.##"; WHR(0); WHR(1); Y

780 END IF
790 ELSE

800 LOCATE 10, 22
810 PRINT "                              "

820 LOCATE 16, 22
830 PRINT "                              "

840 GOSUB 2200
850 END IF

860 LOOP WHILE INKEY$ <> CHR$(27)

870 END IF
880 END IF
885 END IF
890 END IF

900 IF COMERROR <> 0 THEN
910 GOSUB 2200

915 STARTTIMER = TIMER           'Make a 2 seconds delay
917 TIMEOUT = 0

919 WHILE (TIMEOUT < 2)
922 TIMEOUT = TIMER - STARTTIMER
924 WEND

930 END IF

940 LOCATE 21, 12
950 PRINT "                                                         "

960 LOCATE 21, 23
970 PRINT "Beginning to shut down. Please Wait!"

972 STARTTIMER = TIMER           'Make a 2 seconds delay
974 TIMEOUT = 0

976 WHILE (TIMEOUT < 2)
980 TIMEOUT = TIMER - STARTTIMER
985 WEND

990 GOSUB 1150:                  'Stop measuring in the Color Sensor
991 CLOSE #COMNO                 'Close Serial COM port.

992 COLOR 7, 0, 0
993 CLS
994 END



'Subroutines:
' 1000: Initialize serial COM port.

' 1050: Wait for Serial Data and Timeout.

' 1100: Start measuring in the Color Sensor.
' 1150: Stop measuring in the Color Sensor.

' 1200: Set integration time in the Color Sensor.
' 1300: Get integration time from the Color Sensor.

' 1400: Get ID from the Color Sensor.

' 1800: Receive measurement from serial COM port.

' 2000: Error handler routine.
' 2200: Color Sensor error handling.


'Variables used:
'INDATA$          : Data received from serial COM port.
'INDATATMP$       : Temporary data received from serial COM port.
'SENSORTYPE       : Identity for the Color Sensor.
'INTEGRATIONTIME  : Integration time in the Color Sensor.
'DATAREADY        : Data received from the Color Sensor.
'MAXTIMEOUT       : Timeout constant (2seconds).
'STARTTIME        : Used to make timeout
'COMPORT          : The serial COM port.
'COMNO            : Number of serial COM port.
'COMERROR         : Variable to hold a communication error.
'WHR(5)           : Array for CIE coordinates
'IDINFO$(4)       : Identity of the Color Sensor.
'IDPOS(2)         : Temporary array.
'ERRORTXT$(7)     : Communication error text.
'X, Y, Z          : CIE 1931 XYZ-values


1000 '
1005 '******** Initialize the serial COM port ********
1010 '
											'Open serial COM port: COMPORT as COMNO:
											'Baudrate: 4800 or 9600 baud
											'Databit: 8, Stopbit: 2, Parity: NONE
1015 IF (COMBAUD <> 4800) AND (COMBAUD <> 9600) THEN
1016 COMERROR = 8
1017 ELSE
1018 IF (COMBAUD = 4800) THEN
1020 OPEN COMPORT$ + ":4800,N,8,2,BIN,CS,DS" FOR RANDOM AS COMNO
1022 ELSE
1025 OPEN COMPORT$ + ":9600,N,8,2,BIN,CS,DS" FOR RANDOM AS COMNO
1035 END IF
1036 END IF
1037 PRINT #COMNO, "XY;";        'Force the sensor to output data in XYZ mode
1040 RETURN


1050 '
1055 '******** Wait for Serial Data or Timeout ********
1060 '

1065 STARTTIME = TIMER
1070 TIMEOUT = 0                 'Establish timeout for MAXTIMEOUT sec.

											'Loop until data received from the serial
											'COM port, COMNO, or timeout occurred.
1075 WHILE ((LOC(COMNO) = 0) AND (TIMEOUT < MAXTIMEOUT))
1080 TIMEOUT = TIMER - STARTTIME
1085 WEND

1090 RETURN


1100 '
1105 '******** Start measuring in the Color Sensor ********
1110 '

1115 PRINT #COMNO, "MC;";        'Transmit start command to Color Sensor.
											'connected to serial COM port: COMNO.
1120 RETURN


1150 '
1155 '******** Stop measuring in the Color Sensor ********
1160 '

1165 PRINT #COMNO, "MS;";        'Transmit stop command to Color Sensor.
											'connected to serial COM port COMNO.
1170 RETURN


1200 '
1205 '******** Set integration time in the Color Sensor
1210 '
											'The allowed range for the integration timer
											'is: 25 <= INTEGRATIONTIME <=250

1215 IF ((INTEGRATIONTIME >= 25) AND (INTEGRATIONTIME <= 250)) THEN
									  
											'Transmit integration time to the
											'COLOR Sensor
1220 PRINT #COMNO, "SI" + STR$(INTEGRATIONTIME) + ";";

1225 COMERROR = 0                'No error.
1230 ELSE
1235 COMERROR = 3                'Error: Integration time out of range.
1240 END IF

1245 RETURN


1300 '
1305 '******** Get integration time from the Color Sensor
1310 '

1315 PRINT #COMNO, "F?;";        'Transmit "Send integration time" command to
											'the Color Sensor
1320 GOSUB 1050                  'Wait for data or timeout

1325 IF (TIMEOUT < MAXTIMEOUT) THEN

1330 LINE INPUT #COMNO, INDATA$:
										
1335 INTEGRATIONTIME = VAL(INDATA$):
1340 COMERROR = 0                'No error
1345 ELSE
1350 COMERROR = 2                'Timeout receiving data from serial COM port
1355 END IF

1360 RETURN


1400 '
1405 '******** Get ID from the Color Sensor ********
1410 '

1415 PRINT #COMNO, "MS;";        'Stop measuring in the Color Sensor

1420 CLOSE #COMNO                'Flush serial input buffer by closing and
1425 GOSUB 1000                  'opening the serial COM port.

1430 PRINT #COMNO, "I?;";        'Get ID from the Color Sensor

1435 GOSUB 1050                  'Wait for serial Data

1440 IF (TIMEOUT < MAXTIMEOUT) THEN

1445 LINE INPUT #COMNO, INDATA$

1450 IDPOS(0) = INSTR(1, INDATA$, ",")
1455 IDPOS(1) = INSTR(IDPOS(0) + 1, INDATA$, ",")
1460 IDPOS(2) = INSTR(IDPOS(1) + 1, INDATA$, ",")

1465 IDINFO$(0) = LEFT$(INDATA$, IDPOS(0) - 1)
1470 IDINFO$(1) = MID$(INDATA$, IDPOS(0) + 1, IDPOS(1) - IDPOS(0) - 1)
1475 IDINFO$(2) = MID$(INDATA$, IDPOS(1) + 1, IDPOS(2) - IDPOS(1) - 1)
1480 IDINFO$(3) = RIGHT$(INDATA$, LEN(INDATA$) - IDPOS(2) - 1)

1485 PRINT #COMNO, "MA61;";
1490 PRINT #COMNO, "RM;";        'Get info about SensorType

1495 GOSUB 1050                  'Wait for serial Data

1500 IF (TIMEOUT < MAXTIMEOUT) THEN

1505 LINE INPUT #COMNO, INDATA$
1510 SENSORTYPE = VAL(INDATA$):  'Sensortype MUST be > 31 AND < 48

1515 IF ((SENSORTYPE <> 0) AND (SENSORTYPE <> 16)) THEN

1520 COMERROR = 0                'No Error
1525 ELSE
1530 COMERROR = 7                'Wrong Sensor
1535 END IF
1545 ELSE
1550 COMERROR = 2                'Timeout receiving data from serial COM port
1555 END IF
1560 ELSE
1565 COMERROR = 2                'Timeout receiving data from serial COM port
1670 END IF
1675 RETURN


1800 '
1805 '******** Read latest measurement from the Color Sensor ********
1810 '

1820 GOSUB 1050                  'Wait for serial Data

1825 IF (TIMEOUT < MAXTIMEOUT) THEN

1830 LINE INPUT #COMNO, INDATATMP$
1835 INDATA$ = INDATATMP$

1840 WHILE ((NOT EOF(COMNO)) AND (LOF(COMNO) > 20)):
														'Locate latest measurement
1845 INDATA$ = INDATATMP$                 'Keep a copy of the measurement
1850 LINE INPUT #COMNO, INDATATMP$        'Empty input-buffer
1855 WEND

1860 IF (LEN(INDATA$) = 20) THEN
1865 INDATATMP$ = INDATA$
1870 END IF

1875 IF (LEN(INDATATMP$) = 20) THEN

1880 DATAREADY = 1

1885 X = VAL(LEFT$(INDATA$, 6))
1890 Y = VAL(MID$(INDATA$, 8, 6))
1895 Z = VAL(RIGHT$(INDATA$, 6))

1896 IF (X > -.5) THEN

1898 IF (X > .01) AND (Y > .01) AND (Z > .01) THEN

1900 SUMXYZ = X + Y + Z
1905 DIVISOR = X + 15 * Y + 3 * Z

1910 IF ((SUMXYZ > .01) AND (DIVISOR > .01)) THEN

1915 WHR(0) = X / SUMXYZ
1920 WHR(1) = Y / SUMXYZ

1925 WHR(2) = (4 * X) / DIVISOR
1930 WHR(3) = (9 * Y) / DIVISOR
	  
1935 WHR(4) = WHR(2)
1940 WHR(5) = (6 * Y) / DIVISOR

1945 COMERROR = 0                'No error
 
1950 ELSE
1955 COMERROR = 4                'White reference values NOT calculated
1960 END IF

1962 ELSE
1964 COMERROR = 6                'Lowlight
1965 END IF

1966 ELSE
1967 COMERROR = 5                'Overload
1968 END IF

1970 END IF
1980 ELSE
1985 COMERROR = 2                'Timeout receiving data from serial COM port
1990 END IF

1995 RETURN


2000 '
2005 '******** Error Handler *******
2010 '

2015 IF ERR = 69 THEN            'Communications buffer overflow
2025 CLOSE #COMNO                'Flush input buffer by closing and opening
2030 GOSUB 1000                  'the serial COM port.
2035 END IF

2090 IF ERR = 68 THEN            'Device unavailable
2092 COMERROR = 1
2093 GOSUB 2200

2094 STARTTIMER = TIMER           'Make a 2 seconds delay
2095 TIMEOUT = 0

2096 WHILE (TIMEOUT < 2)
2097 TIMEOUT = TIMER - STARTTIMER
2098 WEND

2099 END

2100 END IF

2120 RESUME


2200 '
2205 '******** Color Sensor Error handler
2210 '


2215 LOCATE 21, 12
2220 PRINT "                                                         "

2225 LOCATE 21, 12
2230 PRINT "Error No." + STR$(COMERROR) + ": " + ERRORTXT$(COMERROR)

2240 RETURN


