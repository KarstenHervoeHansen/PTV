*version 9.1 1014637802
u 74
U? 2
C? 6
R? 4
V? 5
? 3
@libraries
@analysis
.AC 1 3 0
+0 1000
+1 10
+2 10000K
.TRAN 0 0 0 0
+0 0ns
+1 1000ns
@targets
@attributes
@translators
a 0 u 13 0 0 0 hln 100 PCBOARDS=PCB
a 0 u 13 0 0 0 hln 100 PSPICE=PSPICE
a 0 u 13 0 0 0 hln 100 XILINX=XILINX
@setup
unconnectedPins 0
connectViaLabel 0
connectViaLocalLabels 0
NoStim4ExtIFPortsWarnings 1
AutoGenStim4ExtIFPorts 1
@index
pageloc 1 0 3831 
@status
n 0 103:10:19:10:19:13;1069233553 e 
s 0 103:10:19:10:19:17;1069233557 e 
c 103:10:19:10:19:28;1069233568
*page 1 0 970 720 iA
@ports
port 3 AGND 290 490 h
port 46 AGND 430 320 h
port 47 AGND 430 230 h
port 43 AGND 270 300 h
@parts
part 44 VDC 390 230 v
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 a 0:13 0 0 0 hln 100 PKGREF=V2
a 1 ap 9 0 24 7 hcn 100 REFDES=V2
a 1 u 13 0 -11 18 hcn 100 DC=5V
part 45 VDC 430 320 d
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 a 0:13 0 0 0 hln 100 PKGREF=V3
a 1 ap 9 0 24 7 hcn 100 REFDES=V3
a 1 u 13 0 -11 18 hcn 100 DC=5V
part 55 VAC 270 260 h
a 0 sp 0 0 0 50 hln 100 PART=VAC
a 0 a 0:13 0 0 0 hln 100 PKGREF=V4
a 1 ap 9 0 20 10 hcn 100 REFDES=V4
a 0 u 0 0 0 20 hcn 100 ACPHASE=0
a 0 u 13 0 -9 23 hcn 100 ACMAG=300mV
part 6 r 360 380 h
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R1
a 0 u 13 0 15 25 hln 100 VALUE=22k
a 0 ap 9 0 0 30 hln 100 REFDES=R1
part 7 r 290 430 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R2
a 0 ap 9 0 30 0 hln 100 REFDES=R2
a 0 u 13 0 15 0 hln 100 VALUE=22k
part 2 LM324 350 260 h
a 0 sp 11 0 14 70 hcn 100 PART=LM324
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=DIP14
a 0 s 0:13 0 0 0 hln 100 GATE=A
a 0 a 0:13 0 0 0 hln 100 PKGREF=U1
a 0 ap 9 0 26 33 hcn 100 REFDES=U1A
part 8 r 290 490 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R3
a 0 ap 9 0 15 0 hln 100 REFDES=R3
a 0 u 13 0 15 40 hln 100 VALUE=1
part 59 C 370 350 h
a 0 sp 0 0 0 10 hlb 100 PART=C
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=CK05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=C3
a 0 ap 9 0 15 0 hln 100 REFDES=C3
a 0 u 13 0 15 25 hln 100 VALUE=8.2p
part 1 titleblk 970 720 h
a 1 s 13 0 350 10 hcn 100 PAGESIZE=A
a 1 s 13 0 180 60 hcn 100 PAGETITLE=
a 1 s 13 0 340 95 hrn 100 PAGECOUNT=1
a 1 s 13 0 300 95 hrn 100 PAGENO=1
part 52 nodeMarker 450 280 h
a 0 s 0 0 0 0 hln 100 PROBEVAR=
a 0 s 0 0 0 0 hln 100 PROBEVAR=
a 0 a 0 0 4 22 hlb 100 LABEL=1
part 53 nodeMarker 300 260 h
a 0 s 0 0 0 0 hln 100 PROBEVAR=
a 0 s 0 0 0 0 hln 100 PROBEVAR=
a 0 a 0 0 4 22 hlb 100 LABEL=2
@conn
w 49
a 0 up 0:33 0 0 0 hln 100 V=
s 390 250 390 230 48
a 0 up 33 0 392 240 hlt 100 V=
w 51
a 0 up 0:33 0 0 0 hln 100 V=
s 390 310 390 320 50
a 0 up 33 0 392 315 hlt 100 V=
w 24
a 0 up 0:33 0 0 0 hln 100 V=
s 290 450 290 430 27
a 0 up 33 0 292 440 hlt 100 V=
w 32
a 0 up 0:33 0 0 0 hln 100 V=
a 0 sr 0 0 0 0 hln 100 LABEL=out
s 450 280 430 280 35
a 0 sr 3 0 440 278 hcn 100 LABEL=out
s 400 380 450 380 31
s 450 380 450 350 33
a 0 up 33 0 452 315 hlt 100 V=
s 450 350 450 280 67
s 400 350 450 350 65
w 20
a 0 up 0:33 0 0 0 hln 100 V=
s 290 390 290 380 19
s 320 300 320 350 11
a 0 up 33 0 322 325 hlt 100 V=
s 360 380 320 380 16
s 290 380 320 380 21
s 320 350 320 380 62
s 320 350 370 350 63
s 350 300 320 300 71
w 41
a 0 up 0:33 0 0 0 hln 100 V=
a 0 sr 0 0 0 0 hln 100 LABEL=in
s 300 260 270 260 54
a 0 sr 3 0 285 258 hcn 100 LABEL=in
s 350 260 300 260 69
a 0 up 33 0 325 259 hct 100 V=
@junction
j 290 490
+ s 3
+ p 8 1
j 320 380
+ w 20
+ w 20
j 290 390
+ p 7 2
+ w 20
j 350 300
+ p 2 -
+ w 20
j 360 380
+ p 6 1
+ w 20
j 290 430
+ p 7 1
+ w 24
j 290 450
+ p 8 2
+ w 24
j 400 380
+ p 6 2
+ w 32
j 430 280
+ p 2 OUT
+ w 32
j 350 260
+ p 2 +
+ w 41
j 430 320
+ s 46
+ p 45 +
j 430 230
+ s 47
+ p 44 -
j 390 250
+ p 2 V+
+ w 49
j 390 230
+ p 44 +
+ w 49
j 390 320
+ p 45 -
+ w 51
j 390 310
+ p 2 V-
+ w 51
j 450 280
+ p 52 pin1
+ w 32
j 300 260
+ p 53 pin1
+ w 41
j 270 300
+ p 55 -
+ s 43
j 270 260
+ p 55 +
+ w 41
j 320 350
+ w 20
+ w 20
j 370 350
+ p 59 1
+ w 20
j 400 350
+ p 59 2
+ w 32
j 450 350
+ w 32
+ w 32
@attributes
a 0 s 0:13 0 0 0 hln 100 PAGETITLE=
a 0 s 0:13 0 0 0 hln 100 PAGENO=1
a 0 s 0:13 0 0 0 hln 100 PAGESIZE=A
a 0 s 0:13 0 0 0 hln 100 PAGECOUNT=1
@graphics
