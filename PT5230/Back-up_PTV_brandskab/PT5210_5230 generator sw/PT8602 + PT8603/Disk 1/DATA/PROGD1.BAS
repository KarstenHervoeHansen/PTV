'  PROGRAM NAME IS "prg5210a.BAS"! 960808
'  SmÜ rettelser til kinaga4.bas (960202)
'  FFT-rutiner springes over. Halve linier er flyttet til de sidste
'  160 pladser i prommen.
   CLS
   OPTION BASE 1
   DEFLNG I,N,X
   DEFSTR H
   DEFDBL A,B,Y,C,D,E,F,G,P,Q,S,T,U,V,J,K,L,M
   DIM Title$(160),HpromOutput(1024),Titles%(161,2)
   DIM Tout$(15),NmaxArray(2)
'   DIM Real#(512),Imag#(512)
   DIM Filter#(1024)
   DIM Linstart!(2000,8),Pakke#(1024,3),P1#(1024)
'  DIM Prom1%(4096),Prom2%(4096),Ry%(2048),By%(2048)
'   $INCLUDE"FFTIFT.INC"
   H="0123456789ABCDEF"
'  HER HENTES FILTER KARAKTERISTIK FOR LUMINANCE GENERATORENS LAVPASFILTER
   PI=3.141592653589793
'   CALL Matread (Filter#(),1024,"X43QC")
'   GOSUB Sinccor
   Mode=0  'Mode=0 er enkelt-linie : Mode=1 er bÜde field1 og field2 EAV/SAV
   COLOR 14
   Filename$="NONAME"
   Hstorename="CHANNEL"
   Path$=""
   Nmax=0
   Cr=275E-9
   Yr=2.35E-7                               ' LUMINANCE RISE-TIME
   Cs=13.5E6                                ' MHz CHROMA SAMPLINGFREKVENS
   Ys=13.5E6                                ' MHz LUMINANCE SAMPLINGFREKVENS
   Ylsb=700/876                             ' OPL0SNINGEN I LUMINANCE DEL
   Clsb=350/448                                ' OPL0SNINGEN I CHOMADEL
   Start=0' i PT5210 (160 i QC-version)     ' LINIESTART I PROMMER
   Aktivlinie=864                           ' SAMPLES Pè AKTIV LINIE
   Slut=1024                                ' SLUTADRESSE I PROMMER
   Cdelay=0                                 ' DELAY TIL CHROMA-PROMMER
   Ydelay=0                                 ' DELAY TIL Y-PROMMER
   Ybibl=0
   Cbibl=0
   Tout$(1)="KONSTANT NIVEAU"
   Tout$(2)="SINUS"
   Tout$(3)="RAMPE"
   Tout$(4)="20 T"
   Tout$(5)="2T"
   Tout$(6)="2.25 T"
   Tout$(7)="MULTIPULS"
   Tout$(8)="SIN X/X"
   Tout$(9)="BOWTIE"
   Tout$(10)="NOISE CORING"
   Tout$(11)="N*T-PULS"
   Tout$(12)="LINIE-SWEEP"
   Tout$(13)="TELETEKST"
'
'  ***********************************************************************
'
'  HOVEDPROGRAM
   DO
   PRINT : CLS
   I=1
   PRINT "SKAL DER:    1)LAVES NYE LINIER ? "
   PRINT TAB(14) "2)UDSKRIVES LINIE/PROMTABEL?"
   PRINT TAB(14) "3)LAVES PROM'ER?"
   PRINT TAB(14) "4)GEMMES NYT LINIEBIBLIOTEK?"
   PRINT TAB(14) "5)EDITERES I LINIEBIBLIOTEKET?"
   PRINT TAB(14) "6)HURTIG EDITERES I BIBLIOTEK?"
   PRINT TAB(14) "7)STATUS/Skift Mode?"
   PRINT TAB(14) "8)STOP?"
   WHILE INSTAT=0
   WEND
   I=VAL(INKEY$)
   Editl=0     ' Editl er 1 i edit-mode
   FastEditl=0
   IF I=8 THEN EXIT LOOP
   ON I GOSUB Ny,Udskrift,Prommer,Gemdata,Edit,FastEdit,Status
   LOOP
   END
'
'  *******************************************************
'
  Status:
   PRINT "ANTAL LINIER",Nmax
   PRINT "ANTAL INDTASTNINGER",Linstart!(2000-(Nmax+1)\8,(Nmax+1)MOD 8+1)
   PRINT "Mode er: ";Mode
   PRINT "SKAL DER SKIFTES Mode? (Y/N)";
   WHILE INSTAT=0
   WEND
   Hi=UCASE$(INKEY$)
   PRINT Hi
   IF Hi="Y" THEN
     Mode=(Mode+1) MOD 2
   END IF
   LOCATE CSRLIN-2,1
   PRINT "Mode er: ";Mode
   LOCATE CSRLIN+1,1

   WHILE INSTAT=0
   WEND
   Hi=INKEY$
   RETURN
'
'  *******************************************************
'
  Udskrift : 'UDSKRIVNING AF INPUTDATA,LINIEOVERSIGT OG PROMMER
   DO
     CLS
     PRINT "0NSKES UDSKRIFT AF:  LINIEOVERSIGT (1) ?"
     PRINT TAB(22) "INDTASTEDE DATA (2)?"
     PRINT TAB(22) "PROM'ER (3)?"
     PRINT TAB(22) "SLUT (4)?"
     PRINT "VíLG NUMMER  ";U
     WHILE INSTAT=0
     WEND
     U=VAL(INKEY$)
     IF U=4 THEN EXIT LOOP
     IF (U>4) OR (U<1) THEN EXIT LOOP
     ON U GOSUB Oversigt,Indtastededata,Promudskrift
   LOOP
   RETURN
'
'
  Oversigt: '
   GOSUB Printervalg
   IF Hi<>"Y" THEN
     CLS
    ELSE
   END IF
   PRINT #1, Filename$;":";
   PRINT #1, TAB(20),"LINIE-OVERSIGT";
   PRINT #1, TAB(50) DATE$;"    ";TIME$
   PRINT #1,
   PRINT #1, TAB(38),"STARTADRESSE"
   PRINT #1, "     NUMMER        TITTEL                     LUMINANCE   CHROMA"
   PRINT #1,
   SELECT CASE Titles%(1,1)
     CASE 1
       Xi=1
       Xii=0
     CASE ELSE
       Xi=Titles%(1,1)
       Xii=1
   END SELECT
   FOR X=1 TO Nmax+1
     N=Titles%(X,1)-1
     CALL Hex(Hadr,N,H$)
     N=Titles%(X,2)+(Titles%(X,2) \ 1024)*1024-1
     CALL Hex(Hadrc,N,H$)
     IF X=Nmax+1 THEN EXIT FOR
     IF Xii < 1 THEN
       PRINT #1, TAB(4) Xi TAB(10) Title$(X) TAB(50) Hadr TAB(59) Hadrc
      ELSE
       PRINT #1, TAB(4) Xi "("X")" TAB(16) Title$(X) TAB(50) Hadr _
                 TAB(59) Hadrc
     END IF
     IF (X MOD 19)=0 AND (Hi<>"Y") THEN GOSUB Pause1
     Xi=Xi+1
   NEXT X
   PRINT #1,   TAB(4) Xi TAB(18) " "  TAB(50) LEFT$(Hadr,5) TAB(59)_
   LEFT$(Hadrc,5)
   IF Hi<>"Y" THEN GOSUB Pause
   PRINT #1, : PRINT #1,
   GOSUB Resetprinter
   RETURN
'
  Pause1:
   PRINT "TRYK Pè EN TASTE FOR AT FORTSíTTE "
   WHILE INSTAT=0
   WEND
   Hadr=INKEY$
   LOCATE CSRLIN-1,POS
   RETURN
'
'  **************************************************************
'
  Indtastededata:        '  UDSKRIFT AF  INDTASTEDE DATA
   N=1
   N1=Nmax
   INPUT "HVILKE(T) BILLEDE(R) SKAL UDKRIVES FRA-TIL",N,N1
   IF N1 < N THEN
     N1=N
   END IF
   IF N1 > Nmax THEN
     N1=Nmax
   END IF
   IF N > Nmax THEN
     N=Nmax
   END IF
   GOSUB Printervalg
   PRINT #1, Filename$;":";
   PRINT #1, TAB(20) "INDTASTNING";
   PRINT #1, TAB(50) DATE$;"    ";TIME$
   PRINT #1, : PRINT #1,
   PRINT #1, "                 VARIGHED Y-AMPITUDE     U       V "
   PRINT #1,
   FOR Linnr=N TO N1
     PRINT #1, Linnr;": ";Title$(Linnr);
     Z%=Linstart!(2000-Linnr \ 8,Linnr MOD 8+1)+1
     FOR I=Z% TO 2000-40
       X=1
       IF Linstart!(I,1)=0 THEN EXIT FOR
       IF (Linstart!(I,5) < 4) OR (Linstart!(I,5) > 8) THEN 620
       PRINT #1, TAB(12) "CENTER";
     620:
'       IF Linstart!(I,5) =13 THEN
'         PRINT #1, Tout$(Linstart!(I,5));
'       END IF
       PRINT #1, TAB(20)  USING "###.###"; Linstart!(I,1);
       FOR X=2 TO 4
         PRINT #1, TAB(8+10*X) USING "####.###";  Linstart!(I,X);
       NEXT X
       IF Linstart!(I,5) > 1 THEN
         PRINT #1, TAB(58) Tout$(Linstart!(I,5));    ' Skriv typen af liniedel
       END IF
       SELECT CASE Linstart!(I,5)
         CASE 1
           PRINT #1, TAB(58) USING"Yr=#### ns Cr=#### ns";Linstart!(I,7)/1E-9;_
           Linstart!(I,8)/1E-9
           GOTO Nextline
         CASE 11
           PRINT #1, TAB(69) INT(Linstart!(I,7)/1E-9+.5);"ns";
         CASE 2,7,8,10,12
           PRINT #1, TAB(64+2*(Linstart!(I,5)\3)) Linstart!(I,6)/1E6;"MHz";
         CASE 3,4,5,6
           GOTO Nextline
         CASE ELSE
       END SELECT
      Nextline:
     NEXT I
     PRINT #1, : PRINT #1,
   NEXT Linnr
   PRINT #1, "SLUT Pè INDTASTNING"
   PRINT #1, : PRINT #1, : PRINT #1,
   GOSUB Resetprinter
   IF Hi<>"Y" THEN
     GOSUB Pause
    ELSE
   END IF
   RETURN
   PRINT "INGEN DATA INDTASTET"
   PRINT : PRINT : PRINT
   RETURN
'
  Promudskrift:
   PRINT "UDSKRIFT AF CHANNEL 1 (1), CHANNEL 2 (2) ELLER CHANNEL 3 (3)?"
   WHILE INSTAT=0
   WEND
   Iu=VAL(INKEY$)
   IF (Iu<0) OR (Iu>3) THEN RETURN
'  LUMINANCE OUT
   PRINT "UDSKRIFT AF HELE KANALEN (1)? ELLER DE ENKELTE PROM'ER ( 1-> 5 (2)?"
   WHILE INSTAT=0
   WEND
   Ii=VAL(INKEY$)
   N=1
   N1=Nmax
   INPUT "UDSKRIFT AF LINIENUMMER ? (FRA-TIL)",N,N1
   PRINT
   ON Ii GOTO 1034,1255
  1034:
   GOSUB Printervalg      'PRINTER-VALG
   CLS
   FOR X=N TO N1
     PRINT #1,X;": ";Title$(X); TAB(22) " UDSKRIFT AF KANAL ";Iu;_
           TAB(50) DATE$;"    "; TIME$
     PRINT #1,
     CALL MatreadRandomI2 (Pakke#(),X,Iu,Hstorename+MID$(H,Iu+1,1))
    Ix=0           ' TíLLER TIL PROMADRESSE
     FOR I=1 TO 4
       LOCATE 3,1
       FOR Ii=0 TO 19
         CALL Hex(Hadr,Ix+(X-1)*1024,H)
         PRINT #1, LEFT$(Hadr,5);": ";
         FOR Iii=0 TO 15
           Ix=Ix+1
           CALL Hex(Had,INT(Pakke#(Ix,Iu)+.5),H)
           PRINT #1, MID$(Had,3,3);" ";
         NEXT Iii
         PRINT #1,
         IF Ix=1024 THEN EXIT FOR
       NEXT Ii
       IF Ix=1024 THEN EXIT FOR
       IF Hi="N" THEN GOSUB Pause
     NEXT I
     IF Hi="N" THEN
       A=CSRLIN
       FOR I=A TO A+15
         PRINT SPACE$(80);
       NEXT I
       LOCATE 24,1
       WHILE INSTAT=0
       WEND
       Hadr=INKEY$
     ELSE
     END IF
     CLS
   NEXT X
   GOSUB Resetprinter
   RETURN
'
1255:
   DO
     PRINT "UDSKRIVNING AF PROMNUMMER  ? (SLUT=6)"
     WHILE INSTAT=0
     WEND
     Ii=VAL(INKEY$)
     IF Ii<1 OR Ii>5 THEN EXIT LOOP
     GOSUB 1284
   LOOP
   RETURN   ' TIL HOVEDPROGRAM
1284:
   GOSUB Printervalg
   GOSUB Resetprinter
   RETURN
'
  Pause:
   PRINT "TRYK Pè EN TASTE FOR AT FORTSíTTE "
   WHILE INSTAT=0
   WEND
   Hadr=INKEY$
  RETURN
'
'   ********************************************************************
'
  Taperead:  '
'   INDLíSERUTINE
   GOSUB NulstilVariable                 ' NULSTIL ALLE VARIABLE
   INPUT "ANGIV NAVN Pè FIL MED NUVíRENDE PROMDATA ? ",Hn
   Filename$=Hn
  DiskInd:
   ON ERROR GOTO DiskError
   OPEN Hn+".DAT" FOR INPUT AS #1
   CLOSE #1
   CALL MatreadS2(Linstart!(),2000,8,Hn)
   CALL MatreadI2(Titles%(),161,2,Hn+"S")
   Istart=3
   FOR I=0 TO 30
     FOR Ii=Istart TO 8
       IF Linstart!(2000-I,Ii) > 0 THEN
         Nmax=Nmax+1
        ELSE
         EXIT FOR
       END IF
     NEXT Ii
     Istart=1
   NEXT I
   FOR I=2 TO Nmax+1
     IF (Titles%(I,1)<>0) THEN
       Npointer=Titles%(I,1)
      ELSE
     END IF
     IF (Titles%(I,2)<>0) THEN
       Ncpointer=Titles%(I,2)
      ELSE
     END IF
   NEXT I
'   PRINT I,Npointer,Ncpointer
'   WHILE INSTAT=0
'   WEND
   Hi=INKEY$
   CALL MatreadStr(Title$(),160,Hn+"T")   ' FIND TITLER Pè LINIER
   RETURN
'  Liniedata hentes fõrst, nÜr de skal bruges.
'
  DiskError:
  BEEP
  PRINT "KAN IKKE FINDE ùNSKET BIBLIOTEK:",Hn
  PRINT "SKAL VI PRùVE IGEN (Y/N) ?"
  WHILE INSTAT=0
  WEND
  Hi=UCASE$(INKEY$)
  IF Hi="Y" THEN
    RESUME DiskInd
   ELSE
    RESUME Taperead
  END IF
'
' *********************************************************
'
  SUB MatreadI1 (Array%(1),Size%,Hname)     ' SUBRUTINE DER LíSER ET
       OPEN Hname+".DAT" FOR INPUT AS #1    ' EN DIMENSIONELT HELTALS-
      FOR I=1 TO Size%                      ' ARRAY
        INPUT #1, Array%(I)
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB MatreadI2 (Array%(2),Size1%,Size2%,Hname)  ' SUBRUTINE DER LíSER ET
      OPEN Hname+".DAT" FOR INPUT AS #1          ' TO DIMENSIONELT HELTALS-
      FOR I=1 TO Size1%                          ' ARRAY
        FOR Ii=1 TO Size2%
          INPUT #1, Array%(I,Ii)
        NEXT Ii
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB Matread (Array#(1),Size%,Hname)     ' SUBRUTINE DER LíSER ET
      OPEN Hname+".DAT" FOR INPUT AS #1   ' EN DIMENSIONELT DOUBLE-
      Ii=0
      FOR I=1 TO Size%/2                  ' PRECISION-ARRAY
        Ii=Ii+1
        INPUT #1, Array#(Ii)
        INPUT #1, Array#(Ii+Size%/2)
      NEXT I
      CLOSE #1
      Array#(Size%/2+1)=SQR(Array#(Size%/2)^2+Array#(Size%)^2)
      Array#(1)=Array#(1)/2
  END SUB
'
'  ********************************************************
'
  SUB MatreadStr (Array$(1),Size%,Hname)    ' SUBRUTINE DER LíSER ET
      OPEN Hname+".DAT" FOR INPUT AS #2     ' EN DIMENSIONELT STRING-
      FOR I=1 TO Size%                      ' ARRAY
        INPUT #2, Array$(I)
      NEXT I
      CLOSE #2
  END SUB
'
'  ********************************************************
'
  SUB MatreadS2 (Array!(2),Size1%,Size2%,Hname)  ' SUBRUTINE DER LíSER ET
      OPEN Hname+".DAT" FOR INPUT AS #1          ' TO DIMENSIONELT SINGLE-
      FOR I=1 TO Size1%                          ' PRECISION-ARRAY
        FOR Ii=1 TO Size2%
          INPUT #1, Array!(I,Ii)
        NEXT Ii
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB MatprintI1 (Array%(1),Size%,Hname)    ' SUBRUTINE DER GEMMER ET EN-
      OPEN Hname+".DAT" FOR OUTPUT AS #1    ' DIMENSIONELT HELTALSARRAY
      FOR I=1 TO Size%
        WRITE #1,Array%(I)
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB MatprintI2 (Array%(2),Size1%,Size2%,Hname)  ' SUBRUTINE DER LíSER ET
      OPEN Hname+".DAT" FOR OUTPUT AS #1          ' TO DIMENSIONELT HELTALS-
      FOR I=1 TO Size1%                          ' ARRAY
        FOR Ii=1 TO Size2%-1
          PRINT #1, Array%(I,Ii);",";
        NEXT Ii
        PRINT #1, Array%(I,Size2%)
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB MatreadRandomI2 (Array#(2),Size&,Promnr&,Hname) ' SUBRUTINE DER L[SER
      OPEN Hname+".DAT"  AS #3 LEN=2                  ' EN LINIE FRA PROMDATA
      FIELD #3, 2 AS Ha                               ' NUMMER ER Size&
      FOR I=1 TO 1024
        GET #3,(Size&-1)*1024+I
        Array(I,Promnr&)=CVI(Ha)
      NEXT I
      CLOSE #3
      END SUB
'
'  ********************************************************
'
  SUB MatprintRandomI2 (Array#(2),Size1&,Size2&,Hname) ' SUBRUTINE DER GEMMER
      IF Size1& <> 0 THEN
        OPEN Hname+"1.DAT"  AS #1 LEN=2                ' ET TO DIMENSIONELT
        FIELD #1, 2 AS Ha                              ' HELTALS-ARRAY
        FOR I=1 TO 1024
          LSET Ha=MKI$(Array#(I,1))
          PUT #1,(Size1&-1)*1024+I
        NEXT I
        CLOSE #1
       ELSE
      END IF
      IF Size2& <> 0 THEN
        OPEN Hname+"2.DAT"  AS #1 LEN=2                ' HER GEMMES KANAL 2
        FIELD #1, 2 AS Ha
        FOR I=1 TO 1024
          LSET Ha=MKI$(Array#(I,2))
          PUT #1,(Size2&-1)*1024+I
        NEXT I
        CLOSE #1
        OPEN Hname+"3.DAT"  AS #1 LEN=2                ' HER GEMMES KANAL 3
        FIELD #1, 2 AS Ha
        FOR I=1 TO 1024
          LSET Ha=MKI$(Array#(I,3))
          PUT #1,(Size2&-1)*1024+I
        NEXT I
        CLOSE #1
       ELSE
      END IF
  END SUB
'
'  ********************************************************
'
  SUB Matprint (Array#(1),Size%,Hname)    ' SUBRUTINE DER GEMMER ET
      OPEN Hname+".DAT" FOR OUTPUT AS #1  ' EN DIMENSIONELT DOUBLE-
      FOR I=1 TO Size%                    ' PRECISION-ARRAY
        WRITE #1, Array#(I)
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
   SUB MatprintStr (Array$(1),Size%,Hname)   ' SUBRUTINE DER GEMMER ET
      OPEN Hname+".DAT" FOR OUTPUT AS #1     ' EN DIMENSIONELT STRING-
      FOR I=1 TO Size%                       ' ARRAY
        WRITE #1, Array$(I)
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB MatprintS2 (Array!(2),Size1%,Size2%,Hname)  ' SUBRUTINE DER GEMMER ET
      OPEN Hname+".DAT" FOR OUTPUT AS #1          ' TO DIMENSIONELT SINGLE-
      FOR I=1 TO Size1%                           ' PRECISION-ARRAY
        FOR Ii=1 TO Size2%-1
          PRINT #1, Array!(I,Ii);",";
        NEXT Ii
        PRINT #1,Array!(I,Size2%)
      NEXT I
      CLOSE #1
  END SUB
'
'  ********************************************************
'
  SUB InputHandler(Number,A,B,C,D,D1,Hinput)
      D1=0
      LINE INPUT Hinput
      IF LEN (Hinput) < 1 THEN ExitSub  'Number
      D1=1
      IF Number < 5 THEN
      Ii=1
      FOR I=1 TO Number
        Ha=" "
        WHILE MID$(Hinput,Ii,1) <> ","
         Ha=Ha+MID$(Hinput,Ii,1)
         Ii=Ii+1
         IF Ii > LEN(Hinput)+1 THEN EXIT LOOP
        WEND
        Ii=Ii+1
        SELECT CASE I
         CASE 1 : A=VAL(Ha)
         CASE 2 : B=VAL(Ha)
         CASE 3 : C=VAL(Ha)
         CASE 4 : D=VAL(Ha)
        END SELECT
      NEXT I
      END IF
  Exitsub:
   END SUB
'
'  ********************************************************
'
   LineCopy:
    PRINT "DER SKAL KOPIERES FRA LININUMMER ?"
    INPUT N1
    StartCopy=Linstart!(2000-(N1) \ 8,(N1) MOD 8+1) +1
    SlutCopy=Linstart!(2000-(N1+1) \ 8,(N1+1) MOD 8+1)
    X=Linstart!(2000-(Nmax+1) \ 8,(Nmax+1) MOD 8+1)+1
    IF X+(SlutCopy-StartCopy) >2000-30 THEN RETURN
    FOR I=StartCopy TO SlutCopy
      FOR Ii=1 TO 8
        Linstart!(X,Ii)=Linstart!(I,Ii)
      NEXT Ii
      X=X+1
    NEXT I
    Title$(Nmax+1)=Title$(N1)
    Linstart!(2000-(Nmax+2) \ 8,(Nmax+2) MOD 8+1)=X-1
    Nmax=Nmax+1
    SELECT CASE LEFT$(Title$(N1),1)
      CASE "$"
        Titles%(Nmax+1,1)=Titles%(Nmax,1)+1
        Titles%(Nmax+1,2)=Titles%(Nmax,2)
      CASE "ú"
        Titles%(Nmax+1,1)=Titles%(Nmax,1)
        Titles%(Nmax+1,2)=Titles%(Nmax,2)+1
      CASE ELSE
        Titles%(Nmax+1,1)=Titles%(Nmax,1)+1
        Titles%(Nmax+1,2)=Titles%(Nmax,2)+1
    END SELECT
    PRINT "LINIEN ER KOPIERET OG SKAL NU EDITERS FOR AT KOMME I PROMMER"
    RETURN
'
'  ********************************************************
'
 NulstilVariable : '  Subrutine der nulstiller alle variable og arrays
   Nmax=0
   Npointer=0  :  Ncpointer=0
   ERASE Titles%
   ERASE Linstart!
   ERASE Title$
   Ybibl=0
   Cbibl=0
   RETURN
'
'  **************************************************************
'
'  INDLíSERUTINE
  Ny:
   CLS
   I=1
   PRINT  "SKAL DER:    FORTSíTTES (1)?"
   PRINT TAB(14) "BRUGES ET GAMMELT BIBLIOTEK (2) ?"
   PRINT TAB(14) "LAVES ET NYT (3) ?"
   PRINT TAB(14) "KOPIERES FRA GAMMEL LINIE (4)?"
   PRINT TAB(14) "RETUR (5)?"
   PRINT
   PRINT "FORTSíTTES? "
   WHILE INSTAT=0
   WEND
   I=VAL(INKEY$)
   IF I<1 OR I>5 THEN
     I=1
   ELSE
   END IF
   ERASE Real#
   ERASE Imag#
   IF I=5 THEN RETURN
   IF I=2 THEN GOTO Taperead
   IF I=3 THEN GOSUB NulstilVariable     ' Nulstil alle variable
   IF I=4 THEN GOTO LineCopy
  2110:
' INDLíS VIA TASTATUR
   Nmax=Nmax+1
   Linnr=Linstart!(2000-Nmax \ 8,Nmax MOD 8+1)
   X=Linnr
   Ax=0                                  ' SUMMERET VARIGHED
   ERASE Pakke#
   INPUT "INDLíS TITTEL Pè NY LINIE? ", Title$(Nmax)
  2145:
'  INDLíSNING VED BRUG AF FíRDIGE KURVEFORMER
   IF Editl=1 THEN Lstop                   ' I edit-mode returner til Edit
   Linstart!(X+1,5)=1
   IF Ax >= Aktivlinie THEN 2373
   PRINT Title$(Nmax),Ax
   PRINT "VíLG MELLEM:  ";
   FOR I=1 TO 13
     PRINT TAB(15),I;")";" ";Tout$(I)
   NEXT I
   PRINT Tout$(Linstart!(X+1,5))
   INPUT I
   IF I=0 THEN
     I=1
   END IF
   Linstart!(X+1,5)=I
   IF Linstart!(X+1,5)>3 THEN 2235
  2200:
   PRINT "STIGETID FOR KANAL 1 ";Yr;"OG FOR KANAL 2 & 3";Cr;
   CALL InputHandler (2,A,B,C,D,D1,Hi)
   IF D1 > 0 THEN
    Yr=A
    Cr=B
   ELSE
    END IF
   IF Yr > 1.0E-5 THEN 2200      ' Retur ved for stor lum. stigetid
   Linstart!(X+1,7)=Yr           ' Her gemmes luminance stigetid
   Linstart!(X+1,8)=Cr           ' Her gemmes chroma-stigetid
  2225:
   Yr=Linstart!(X+1,7)           ' Under edit hentes ny luminance stigetid her
   Cr=Linstart!(X+1,8)           ' Under edit hentes ny chromastigetid her
  2235:
   X=X+1
   ON Linstart!(X,5) GOTO 2245,Sinus,Sinus,Tyvet,Tot,Tret,Tyvet,Sincx,Sinus,_
   Noisecoring,Ntpulser,Lsweep,Teletekst
  2245:
   IF Editl=1 THEN Fyld
   PRINT "NY VARIGHED , NY Y-AMPL. (mV), NY FARVE-AMPL. B-Y,R-Y (mV)";
   INPUT Linstart!(X,1),Linstart!(X,2),Linstart!(X,3),Linstart!(X,4)
   PRINT USING "####  ####.###  +###.##  +###.##";_
   Linstart!(X,1),Linstart!(X,2),Linstart!(X,3),Linstart!(X,4)
 Fyld:
  Pak:
   FOR I=Start+Ax+1 TO Start+Ax+Linstart!(X,1)
     Pakke#(I+Ydelay,1)=(Linstart!(X,2))
     Pakke#(I+Cdelay,2)=(Linstart!(X,3))
     Pakke#(I+Cdelay,3)=(Linstart!(X,4))
     IF I > Start+Aktivlinie-1 THEN Form
   NEXT I
   SELECT CASE Linstart!(X,5)
    CASE 1
      GOTO Form
    CASE 2,3,10
      GOTO  Lstop
    CASE ELSE
      GOTO Lstop
   END SELECT
  Form:
   IF X=1 THEN
     Linst!=0
    ELSE
     Linst!=Linstart!(X-1,1)
   END IF
   CALL Shape (Pakke#(),Ax+Start,Linst!,Yr,Ys,Cr,Cs,Ydelay,Cdelay,PI)
   Ax=Ax+INT(Linstart!(X,1)+.5)
   IF Editl=1 THEN Lstop
   IF (X<2000-30) AND (Ax<Aktivlinie) THEN 2145
  2373:
   Linstart!(2000-(Nmax+1) \ 8,(Nmax+1) MOD 8+1)=X+1
   Linstart!(X+1,6)=Sr
   Linstart!(X+1,7)=Yr
   Linstart!(X+1,8)=Cr

   Linstart!(X+1,4)=Mode

  2385:
   IF FastEditl=0 THEN
    PRINT "STIGETID FOR UDLIGNINGSPULS OG HALVE LINIER( CH1,CH2&3 ) ?"Sr,Yr,Cr
    CALL InputHandler (3,A,B,C,D,D1,Hi)
    IF D1 > 0 THEN
     Linstart!(X+1,6)=A
     Linstart!(X+1,7)=B
     Linstart!(X+1,8)=C    '      Input SYNC og VIDEO rise-time
    END IF
   END IF
   Sr=Linstart!(X+1,6)
   Yr=Linstart!(X+1,7)
   Cr=Linstart!(X+1,8)     '      Input SYNC og VIDEO rise-time

   Mode= Linstart!(X+1,4)

   PRINT "LAVER PROMINDHOLD"
'   ON ERROR GOTO 0
'   FOR Ifft=1 TO 3
'     GOSUB FourierPakning
'     PRINT "FFT";Ifft
'     Lpower=9               ' DROUND(LOG(2048)/LOG(2),8)-1
'     CALL Fftandift (1,512,Lpower,1,Real#(),Imag#())    ' FFT-TRANSFORM
'     GOSUB Div               ' Korriger med filter + sinckorrektion
'    CALL Fftandift (-1,512,Lpower,2,Real#(),Imag#())   ' IFT-TRANSFOM
'     GOSUB Repak
'     GOSUB Rotate
'   NEXT Ifft
'   GOSUB Ompak                ' Pak med sync- og udligningspulser
'   GOSUB Totfilter
   CALL Kvant(Pakke#(),Ylsb,Slut)  ' Kvantiserer kanal 1
   CALL Ckvant(Pakke#(),Clsb,Slut) ' kvantiserer kanal 2 og 3
   IF Mode = 1 THEN GOSUB Field1  ' EAV/SAV TIL FIELD1
   GOSUB Rotate     'ROTERER 16 PLADSER TIL HùJRE PGA. SEGMENTERING
   IF Editl=1 THEN Lstop      ' Hvis linien editeres, returner til edit-mode
   Ncmax=Nmax
   IF Nmax=1 THEN
     Titles%(Nmax,1)=Nmax
     Titles%(Nmax,2)=Nmax
     Npointer=1
     Ncpointer=1
   END IF
   SELECT CASE LEFT$(Title$(Nmax),1)
     CASE "$"   '  Ydel AF 2-WIRE SIGNAL
      Titles%(Nmax+1,1)=Npointer+1
      Titles%(Nmax+1,2)=Ncpointer
      Inmax&=Titles%(Nmax,1)
      Incmax&=0
      CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&,Hstorename)
      Npointer=Npointer+1
      IF Mode = 1 THEN
        GOSUB Field2
        CALL MatprintRandomI2( Pakke#(),Inmax&+1,Incmax&,Hstorename)
        Titles%(Nmax+1,1)=Npointer+1
        Npointer=Npointer+1
      END IF
     CASE "ú"   '  Chromadel AF 2-WIRE SIGNAL
      Titles%(Nmax+1,2)=Ncpointer+1
      Titles%(Nmax+1,1)=Npointer
      Inmax&=0
      Incmax&=Titles%(Nmax,2)
      Ncpointer=Ncpointer+1
      IF LEFT$(Title$(Nmax),2)="úú" THEN
        CALL MatprintBetaI2( Pakke#(),Inmax&,Incmax&,Hstorename)
       ELSE
        CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&,Hstorename)
        IF Mode = 1 THEN
          GOSUB Field2
          CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&+1,Hstorename)
          Titles%(Nmax+1,2)=Ncpointer+1
          Ncpointer=Ncpointer+1
        END IF
      END IF
     CASE ELSE
      Titles%(Nmax+1,1)=Npointer+1
      Titles%(Nmax+1,2)=Ncpointer+1
      Inmax&=Titles%(Nmax,1)
      Incmax&=Titles%(Nmax,2)
      CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&,Hstorename)
      Npointer=Npointer+1
      Ncpointer=Ncpointer+1
      IF Mode = 1 THEN
        GOSUB Field2
        CALL MatprintRandomI2( Pakke#(),Inmax&+1,Incmax&+1,Hstorename)
        Titles%(Nmax+1,1)=Npointer+1
        Titles%(Nmax+1,2)=Ncpointer+1
        Npointer=Npointer+1
        Ncpointer=Ncpointer+1
      END IF
    END SELECT
   'Gem promdata pÜ disk
  2510:
   PRINT "SKAL DER LAVES FLERE LINIER JA/NEJ (2/1)"
   WHILE INSTAT=0
   WEND
   I=VAL(INKEY$)
   IF (I<1) OR (I>2) THEN 2510
   ON I GOTO Lstop,2110
  Lstop:
   RETURN
'  *******************************************************
'
  Rotate:

   FOR Iy=1 TO 3
     FOR I=1 TO 1024
       P1#(I)=Pakke#(I,Iy)
     NEXT I
     FOR I=17 TO 864
       Pakke#(I,Iy)=P1#(I-16)
     NEXT I
     FOR I=1 TO 16
       Pakke#(I,Iy)=P1#(848+I)
     NEXT I
   NEXT Iy

   RETURN
'  *******************************************************
'
  Totfilter:
    Istart=Linstart!(2000-(Nmax) \ 8,(Nmax) MOD 8+1)+1
    Islut=Linstart!(2000-(Nmax+1) \ 8,(Nmax+1) MOD 8+1)
    FOR I=Istart TO Islut
     IF Linstart!(I,5)=5 THEN
       IF Linstart!(I,1)-Linstart!(I-1,1)>12 OR Linstart!(I-1,5)<>5 THEN
         Ii=Linstart!(I,1)+Start+Ydelay+1
         Pakke#(Ii-12,1)=Pakke#(Ii-15,1)
         Pakke#(Ii-13,1)=Pakke#(Ii-15,1)
         Pakke#(Ii-14,1)=Pakke#(Ii-15,1)
         Ii=Linstart!(I,1)+Start+Cdelay+1
         FOR N=2 TO 3
           Pakke#(Ii-12,N)=Pakke#(Ii-15,N)
           Pakke#(Ii-13,N)=Pakke#(Ii-15,N)
           Pakke#(Ii-14,N)=Pakke#(Ii-15,N)
         NEXT N
       END IF
     END IF
    NEXT I
    RETURN
'
'   ******************************************************
'
  Sinus:                       '  Til generering af multiburst m.v.
  IF Editl=1 THEN 2560
   PRINT "VARIGHED ,Ampl (mVpp),U (mV), V (mV)";
   CALL InputHandler (4,A,B,C,D,D1,Hi)
   IF D1 > 0 THEN
    Linstart!(X,1)=A
    Linstart!(X,2)=B
    Linstart!(X,3)=C
    Linstart!(X,4)=D
   END IF
   PRINT USING "####  #######.##  Hz  +###.##  +###.##  +###.##";_
   Linstart!(X,1),Linstart!(X,6),Linstart!(X,2),Linstart!(X,3),Linstart!(X,4)
  2560:
   IF Linstart!(X,5)=3 THEN Rampe
   IF FastEditl=0 THEN
     PRINT "FREKVENS AF SINUS ? ";
     CALL InputHandler (1,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
      Linstart!(X,6)=A
     END IF
   END IF
'   IF Linstart!(X,6)<.4E6 THEN 2560
   PRINT Linstart!(X,6);" Hz"
   J=Linstart!(X,2)  : Linstart!(X,2)=J/2+Linstart!(X-1,2)
   J2=Linstart!(X,3) : Linstart!(X,3)=J2/2+Linstart!(X-1,3)
   J3=Linstart!(X,4)  : Linstart!(X,4)=J3/2+Linstart!(X-1,4)
   B=INT(.84697747*Yr*Ys)          ' Samples pga. Shape
   Linstart!(X,1)=Linstart!(X,1)-2*B
   Pakke#(Ax+Start+B+Ydelay,1)=Linstart!(X-1,2)
   Pakke#(Ax+Start+B+Ydelay,2)=Linstart!(X-1,3)
   Pakke#(Ax+Start+B+Ydelay,3)=Linstart!(X-1,4)
   Ax=Ax+B
   GOSUB Fyld                      ' Her laves indhyldningskurven til sinus
   Pakke#(Ax+Start+Linstart!(X,1)+Ydelay,1)=Pakke#(Ax+Start-B+Ydelay,1)
                                   '  Symmetrisk sinus
   Pakke#(Ax+Start+Linstart!(X,1)+Ydelay,2)=Pakke#(Ax+Start-B+Ydelay,2)
'                                     Symmetrisk sinus
   Pakke#(Ax+Start+Linstart!(X,1)+Ydelay,3)=Pakke#(Ax+Start-B+Ydelay,3)
'                                     Symmetrisk sinus
   CALL Shape(Pakke#(),Ax+Start,0,Yr,Ys,Cr,Cs,Ydelay,4*Cdelay,PI)
'       For-flanke
   Ax=Ax+Linstart!(X,1)-1           ' 50% pkt for bagflanke
   CALL Shape(Pakke#(),Ax+Start,0,Yr,Ys,Cr,Cs,Ydelay,4*Cdelay,PI)
'       Bag-flanke
   Linstart!(X,2)=J : Linstart!(X,3)=J2 : Linstart!(X,4)=J3
   Linstart!(X,1)=Linstart!(X,1)+2*B
   Ax=Ax+B
  2650:
   L=Linstart!(X,1)       ' Varighed af sinus
   J=Linstart!(X,6)       ' Frekvens af den õnskede sinus
   K=Linstart!(X-1,2)     ' Nulniveau kanal 1
   K2=Linstart!(X-1,3)    ' Nulniveau kanal 2
   K3=Linstart!(X-1,4)    ' Nulniveau kanal 3
   Ii=0 : B=0 : C=0
   IF Linstart!(X,5)=9 THEN 2765
   IF (J >= 1.4E6) OR (J < .5E6) THEN 2765
   B=INT(1.5E6/J+1)
   Xi=1+(B+1) \ 3
   FOR I=Ax+Start-L+Ydelay TO Ax+Start+Ydelay+B-L-1
     Ii=Ii+1
     C=.5*Linstart!(X,2)*(1-COS(2*PI*Ii*J/Ys))
     C2=.5*Linstart!(X,3)*(1-COS(2*PI*Ii*J/Ys))
     C3=.5*Linstart!(X,4)*(1-COS(2*PI*Ii*J/Ys))
     Pakke#(I,1)=K+C : Pakke#(I,2)=K2+C2 : Pakke#(I,3)=K3+C3
     Pakke#(I+L-2*Ii+2-Xi+B,1)=K-C
     Pakke#(I+L-2*Ii+2-Xi+B,2)=K2-C2
     Pakke#(I+L-2*Ii+2-Xi+B,3)=K3-C3
   NEXT I
   IF J < 1E6 THEN
     Ci=2      ' Startvërdi, der er valgt sÜ .5 MHz bliver symmetrisk
     Xi=Xi-2
    ELSE
     Ci=.5      ' Startvërdi, der er valgt, sÜ 1 MHz bliver symmetrisk
   END IF
   FOR I=Ax+Start-L+Ydelay+B TO Ax+Start+Ydelay-Xi
     Ci=Ci+1
     Pakke#(I,1)=K+.5*Linstart!(X,2)*SIN(2*PI*J*Ci/Ys)
     Pakke#(I,2)=K2+.5*Linstart!(X,3)*SIN(2*PI*J*Ci/Ys)
     Pakke#(I,3)=K3+.5*Linstart!(X,4)*SIN(2*PI*J*Ci/Ys)
   NEXT I
   IF J < 1E6 THEN
     Ax=Ax+B-Xi-2
    ELSE
     Ax=Ax+B-Xi+1
   END IF
   GOTO 2785
  2765:
   IF Linstart!(X,5)=9 THEN
     SELECT CASE LEFT$(Title$(Nmax),1)
       CASE "ú"         '  Chroma-del AF 2-WIRE SIGNAL
         IF Linstart!(X,6)>1.007E6 THEN
           J1=0           ' BOWTIE +COLOUR BAR
           Fi=-.326725
          ELSE
           J1=4000        ' BRUGES TIL 2-WIRE-BOWTIE
           Fi=-.26389378  ' ------DO.--------
          END IF
        CASE ELSE
         J1=2000        ' BRUGES TIL BOWTIE
         Fi=-.3141592   ' ------DO.--------  Fi=18 DEG.
     END SELECT
    ELSE
     J1=0          ' STANDARD SINUS
     Fi=0          ' ------DO.-----
   END IF
   FOR I=Ax+Start+Ydelay-L+1 TO Ax+Start+Ydelay
     Ii=Ii+1
     Pakke#(I,1)=K+(Pakke#(I,1)-K)*SIN(2*PI*J*Ii/Ys)
     Pakke#(I,2)=K2+(Pakke#(I,2)-K2)*SIN(2*PI*(J+J1)*Ii/Ys+Fi)
     Pakke#(I,3)=K3+(Pakke#(I,3)-K3)*SIN(2*PI*(J+J1)*Ii/Ys+Fi)
   NEXT I
  2785:
   Ax=Ax+1
   Pakke#(Ax+Start+Ydelay,1)=Linstart!(X-1,2)
   Pakke#(Ax+Start+Ydelay,2)=Linstart!(X-1,3)
   Pakke#(Ax+Start+Ydelay,3)=Linstart!(X-1,4)
'  Her er er element i multiburst fyldt
   IF Editl=1 THEN Lstop
   GOTO 2145              ' GÜ til nëste indlësning
'
  Rampe:
   IF FastEditl=0 THEN
   PRINT "AMPLITUDE AF OVERLEJRET SINUS (1 MHz)";Linstart!(X,6),
   CALL InputHandler (2,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
       Linstart!(X,6)=A
     END IF
   END IF
   GOSUB Fyld
   Ax=Ax+Linstart!(X,1)
   Ii=0
   J=Linstart!(X,2)-Linstart!(X-1,2)
   K=Linstart!(X,3)-Linstart!(X-1,3)
   L=Linstart!(X,4)-Linstart!(X-1,4)
   FOR I=Ax+Start-Linstart!(X,1) TO Ax+Start
     Ci=Ii/Linstart!(X,1)
     F=Linstart!(X,6)*.5*(1-COS(2*PI/Ys*1E6*Ii))
     Pakke#(I+Ydelay,1)=Linstart!(X-1,2)+J*Ci+F
     Pakke#(I+Cdelay,2)=Linstart!(X-1,3)+K*Ci+F
     Pakke#(I+Cdelay,3)=Linstart!(X-1,4)+L*Ci+F
     Ii=Ii+1
   NEXT I
   IF Editl=1 THEN Lstop
   GOTO 2145   '  Slut pÜ rampe-rutinen
'
'  *********************************************************
'
  Tyvet:                    ' Laver 20 T/10 T puls symmetrisk om Lin_start(X,1)
   IF FastEditl=0 THEN
     PRINT "CENTERPLACERING OG MAX AMPLITUDER Y,U,V (mV) ?";
     CALL InputHandler (4,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
      Linstart!(X,1)=A
      Linstart!(X,2)=B
      Linstart!(X,3)=C
      Linstart!(X,4)=D
     END IF
   END IF
   Chrampl=1
   IF Linstart!(X,5)=4 THEN 2955
    IF FastEditl=0 THEN
     PRINT  "FREKVENS AF MULTIPULS ?";
     CALL InputHandler (1,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
      Linstart!(X,6)=A
     END IF
   END IF
  2955:
   IF Linstart!(X,6)>1E6 OR Linstart!(X,5)=4 THEN
     T=1000E-9
     Ci=1
    ELSE
     Ci=.5    ' Bruges til 40 T Kun beregning af lëngde til slut
     T=2000E-9
   END IF
   IF Linstart!(X,5)=9 THEN
     T=500E-9
     Ci=.5
   END IF
   Ii=INT(Linstart!(X,1)+.5)
   K=Linstart!(X,2)
   K2=Linstart!(X,3)
   K3=Linstart!(X,4)
   L=2*Ys*2*T
   M=INT(L-1)
   IF M MOD 2=0 THEN
     M=M+1
   END IF
   Fi=.5*(PI/L+(PI/L*M-PI))           ' Symmetrisk sinus^2
   Fi=Fi+(Linstart!(X,1)-Ii)*PI/(M+1) ' Fase forskydning
   B=INT(M/2+1.5)
   FOR I=1 TO M
     Ampl=SIN(PI*I/L-Fi)^2
     J=Pakke#(Start+Ii+I-(B-1)+Ydelay,1)
     J2=Pakke#(Start+Ii+I-(B-1)+Cdelay,2)
     J3=Pakke#(Start+Ii+I-(B-1)+Cdelay,3)
     Pakke#(Start+Ii+I-(B-1)+Ydelay,1)=(K-J)*Ampl+J
     Pakke#(Start+Ii+I-(B-1)+Ydelay,2)=(K2-J2)*Ampl+J2
     Pakke#(Start+Ii+I-(B-1)+Ydelay,3)=(K3-J3)*Ampl+J3
'     PRINT I;Start+Ii+I-(B-1)+Ydelay;(K-J)*Ampl+J
'     IF I MOD 20 = 0 THEN GOSUB Pause
   NEXT I

   IF (Linstart!(X,5)=4) OR (Linstart!(X,5)=9) THEN 3025  ' 4=20*T, 9=10*T
'                                                      ellers multipuls
   K=Linstart!(X,6)              ' Frekvens af multipuls
   FOR Ix=1 TO 3
     SELECT CASE Ix
      CASE 1
       J=J
      CASE 2
       J=J2
      CASE 3
       J=J3
     END SELECT
     FOR I=1 TO M
       Ai=Pakke#(Start+Linstart!(X,1)+I-B+Ydelay,Ix)-J
       Pakke#(Start+Linstart!(X,1)+I-B+Ydelay,Ix)=_
       .5*(Ai+Ai*COS(K*2*PI*(I-B)/Ys))+J
'       PRINT I;Ai;Pakke#(Start+Linstart!(X,1)+I-B+Ydelay,Ix)
'       IF I MOD 20 = 0 THEN GOSUB Pause
     NEXT I
   NEXT Ix
  3025:
   Ax=Linstart!(X,1)+28 / Ci+(Ci / 2)*4           ' Input, der kommer efter 20T
   Ii=Start+Ax+Ydelay
   Pakke#(Ii,1)=Pakke#(Ii-1,1)
   Pakke#(Ii,2)=Pakke#(Ii-1,2)
   Pakke#(Ii,3)=Pakke#(Ii-1,3)
   GOTO 2145                                      ' GÜ til nëste indlësning
'
  Kor10T:
    SELECT CASE I
      CASE 1
        Ai=-2/12
      CASE 3
        Ai=47/63
      CASE 5
        Ai=131/143
      CASE 7
        Ai=226/235
      CASE 9
        Ai=315/319
      CASE 11
        Ai=1
      CASE 27-11
        Ai=315/319'*326/325
      CASE 27-9
        Ai=228/236*234/232*238/236
      CASE 27-7
        Ai=133/143*134/138
      CASE 27-5
        Ai=137/143*57/61
      CASE 27-3
        Ai=.86
      CASE 27-1
        Ai=-.5
      CASE ELSE
        Ai=Ai
    END SELECT
    RETURN

'  ********************************************************
'
  Tret:  ' Bruges til 225 ns puls.

  Tot:   ' LAVER 2T-PULS
   IF FastEditl=0 THEN
     PRINT "CENTERPLACERING OG MAX. AMPLITUDER KANAL 1,2 OG 3  ?";
     CALL InputHandler (4,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
      Linstart!(X,1)=A
      Linstart!(X,2)=B
      Linstart!(X,3)=C
      Linstart!(X,4)=D
     END IF
   END IF
   IF Linstart!(X,5)=5 THEN
     T=100E-9
    ELSE
     T=112.5E-9
   END IF
   Ii=INT(Linstart!(X,1)+.5)
   K=Linstart!(X,2)
   K2=Linstart!(X,3)
   K3=Linstart!(X,4)
   L=2*Ys*2*T
   M=INT(L-1)
   IF M MOD 2=0 THEN
     M=M+1
   END IF
   Fi=.5*(PI/L+(PI/L*M-PI))           ' Symmetrisk sinus^2
   Fi=Fi+(Linstart!(X,1)-Ii)*PI/(M+1) ' Fase forskydning
   B=INT(M/2+1.5)
   FOR I=1 TO M
     Ampl=SIN(PI*I/L-Fi)^2
     J=Pakke#(Start+Ii+I-(B-1)+Ydelay,1)
     J2=Pakke#(Start+Ii+I-(B-1)+Cdelay,2)
     J3=Pakke#(Start+Ii+I-(B-1)+Cdelay,3)
     Pakke#(Start+Ii+I-(B-1)+Ydelay,1)=(K-J)*Ampl+J
     Pakke#(Start+Ii+I-(B-1)+Cdelay,2)=(K2-J2)*Ampl+J2
     Pakke#(Start+Ii+I-(B-1)+Cdelay,3)=(K3-J3)*Ampl+J3
   NEXT I
   Ax=INT(Linstart!(X,1)+.5)+5
   GOTO 2145
'
'  ********************************************************
'
  Sincx:          ' Laver signal efter funktionen (sin x)/x
   IF FastEditl=0 THEN
     PRINT "CENTERPLACERING OG MAX AMPLITUDE ?",Linstart!(X,1),Linstart!(X,2),_
     Linstart!(X,3),Linstart!(X,4)
     CALL InputHandler (4,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
       Linstart!(X,1)=A
       Linstart!(X,2)=B
       Linstart!(X,3)=C
       Linstart!(X,4)=D
      ELSE
     END IF
     PRINT "BèNDBREDDE AF SIGNAL ? ",Linstart!(X,6)
     CALL InputHandler (1,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
       Linstart!(X,6)=A
      ELSE
     END IF
   END IF
   K=Linstart!(X,6)
   J=Linstart!(X,1)+Ydelay+Start
   Pakke#(J,1)=Pakke#(J,1)+Linstart!(X,2)
   Pakke#(J,2)=Pakke#(J,2)+Linstart!(X,3)
   Pakke#(J,3)=Pakke#(J,3)+Linstart!(X,4)
   Ii=2
   FOR I=J+1 TO J+82
     Pakke#(I,1)=Pakke#(I,1)+Linstart!(X,2)*SIN(PI*K*Ii/Ys)/(PI*K*Ii/Ys)
     Pakke#(I-Ii,1)=Pakke#(I,1)
     Pakke#(I,2)=Pakke#(I,2)+Linstart!(X,3)*SIN(PI*K*Ii/Ys)/(PI*K*Ii/Ys)
     Pakke#(I-Ii,2)=Pakke#(I,2)
     Pakke#(I,3)=Pakke#(I,3)+Linstart!(X,4)*SIN(PI*K*Ii/Ys)/(PI*K*Ii/Ys)
     Pakke#(I-Ii,3)=Pakke#(I,3)
     Ii=Ii+2
   NEXT I
   Ax=Linstart!(X,1)+82          ' Er denne linie n|dvendig
   GOTO 2145
'
'  ********************************************************
'
  Noisecoring:                 '
  IF Editl=0 THEN
  Nyind:
   PRINT "STIGETID FOR KANAL 1 ";Yr;"OG FOR KANAL 2 & 3";Cr;
   CALL InputHandler (2,A,B,C,D,D1,Hi)
   IF D1 > 0 THEN
     Yr=A
     Cr=B
   END IF
   IF Yr > 1.0E-5 THEN Nyind   ' Retur ved for stor lum. stigetid
   Linstart!(X,7)=Yr           ' Her gemmes luminance stigetid
   Linstart!(X,8)=Cr           ' Her gemmes chroma-stigetid
   PRINT "VARIGHED ,Ampl (mVpp),U (mV), V (mV)";
   CALL InputHandler (4,A,B,C,D,D1,Hi)
   IF D1 > 0 THEN
    Linstart!(X,1)=A
    Linstart!(X,2)=B
    Linstart!(X,3)=C
    Linstart!(X,4)=D
   ELSE
   END IF
   PRINT USING "####  #######.##  Hz  +###.##  +###.##  +###.##";_
   Linstart!(X,1),Linstart!(X,6),Linstart!(X,2),Linstart!(X,3),Linstart!(X,4)
   END IF
   IF FastEditl=0 THEN
     PRINT "FREKVENS AF SINUS ? ";Linstart!(X,6)
     CALL InputHandler (1,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
      Linstart!(X,6)=A
     ELSE
     END IF
   PRINT Linstart!(X,6);" Hz"
   END IF
   J=Linstart!(X,2)   : Linstart!(X,2)=J+Linstart!(X-1,2)
   J2=Linstart!(X,3)  : Linstart!(X,3)=J2+Linstart!(X-1,3)
   J3=Linstart!(X,4)  : Linstart!(X,4)=J3+Linstart!(X-1,4)
   B=INT(1.03734*Yr*Ys)          ' Samples pga. Shape
   Linstart!(X,1)=Linstart!(X,1)+2*B
   Ax=Ax-B
   GOSUB Fyld                      ' Her laves indhyldningskurven til sinus
   Pakke#(Ax+Start+Linstart!(X,1)+Ydelay,1)=Pakke#(Ax+Start+Ydelay,1)
                                   '  Symmetrisk sinus
   Pakke#(Ax+Start+Linstart!(X,1)+Ydelay,2)=Pakke#(Ax+Start+Ydelay,2)
'                                     Symmetrisk sinus
   Pakke#(Ax+Start+Linstart!(X,1)+Ydelay,3)=Pakke#(Ax+Start+Ydelay,3)
'                                     Symmetrisk sinus
   CALL Shape(Pakke#(),Ax+Start,0,Yr,Ys,Cr,Cs,Ydelay,4*Cdelay,PI)
'       For-flanke
   Ax=Ax+Linstart!(X,1)-1           ' 50% pkt for bagflanke
   CALL Shape(Pakke#(),Ax+Start,0,Yr,Ys,Cr,Cs,Ydelay,4*Cdelay,PI)
'       Bag-flanke
   Linstart!(X,2)=J   : Linstart!(X,3)=J2   : Linstart!(X,4)=J3
   K=Linstart!(X-1,2) : K2=Linstart!(X-1,3) : K3=Linstart!(X-1,4)
   Linstart!(X,1)=Linstart!(X,1)-2*B
   Ax=Ax+B
   L=Linstart!(X,1)
   Ii=0
   B=B*2
   FOR I= 2 TO 2*Linstart!(X,1) STEP 2
     Ii=Ii+1
     Ampl=ABS(1/(L-1)*(L+1-I)) 'SAVTAND
     J=Pakke#(Ax+Start-L+Ii-B+Ydelay,1)      'Indhyldningskurve
     J2=Pakke#(Ax+Start-L+Ii-B+4*Cdelay,2)   '   -----"-----
     J3=Pakke#(Ax+Start-L+Ii-B+4*Cdelay,3)   '   -----"-----
     Pakke#(Ax+Start-L+Ii-B+Ydelay,1)=(J-K)*Ampl+K      ' Ny amplitude
     Pakke#(Ax+Start-L+Ii-B+4*Cdelay,2)=(J2-K2)*Ampl+K2
     Pakke#(Ax+Start-L+Ii-B+4*Cdelay,3)=(J3-K3)*Ampl+K3
   NEXT I
   FOR I= 1 TO Linstart!(X,1)+2*B
     Ampl=SIN(2*PI*Linstart!(X,6)*(I-B)/Ys)   ' SINUS
     J=Pakke#(Ax+Start-L+I-2*B+Ydelay,1)      ' Indhyldningskurve
     J2=Pakke#(Ax+Start-L+I-2*B+4*Cdelay,2)   '    -----"-----
     J3=Pakke#(Ax+Start-L+I-2*B+4*Cdelay,3)   '    -----"-----
     Pakke#(Ax+Start-L+I-2*B+Ydelay,1)=(J-K)*Ampl+K      ' Ny amplitude
     Pakke#(Ax+Start-L+I-2*B+4*Cdelay,2)=(J2-K2)*Ampl+K2
     Pakke#(Ax+Start-L+I-2*B+4*Cdelay,3)=(J3-K3)*Ampl+K3
   NEXT I
   Ax=Ax+1
   Pakke#(Ax+Start+Ydelay,1)=Linstart!(X-1,2)
   Pakke#(Ax+Start+Ydelay,2)=Linstart!(X-1,3)
   Pakke#(Ax+Start+Ydelay,3)=Linstart!(X-1,4)
'  Her er er noisecoring-element fyldt, 1. sample efter element er magen til
'  til sample fõr element.
   IF Editl=1 THEN Lstop
   GOTO 2145              ' GÜ til nëste indlësning
'  Slut pÜ noisecoring rutine
'
'  ********************************************************
'
  Teletekst:                   ' Genererer teletextsignal efter euoropëisk norm
   ON Linstart!(X,5)-9 GOTO Ttext,Ntpulser,Ttext,Ttext
  Ttext:
  Ftele=6937500
   Bitnr=0  :  Bitvalue=0
   N=0  :  Nold=0
   X=X-1
  100:
   IF FastEditl=0 THEN
     PRINT "BYTE=VíRDI",Linstart!(X+1,2);
     CALL InputHandler (1,A,B,C,D,D1,Hi)
     IF D1 > 0 THEN
       Linstart!(X+1,2)=A
     END IF
   END IF
'   PRINT Ax,Linstart!(X+1,2)
   Linstart!(X+1,5)=13
   Linstart!(X+1,1)=2
   X=X+1
   B=0
   Hadr="        "   'Reserver 8 pladser til teletext
   FOR I=1 TO 8
     J=(Linstart!(X,2)-B) \ (256 \ (2^I))
     MID$(Hadr,I,1)=MID$(H,J+1,1)
     B=B+J*(256 \ (2^I))
   NEXT I
   FOR I=1 TO 8
     J=VAL(MID$(Hadr,9-I,1))
     Ampl=J-Bitvalue
     IF (SIN(PI/2/Ys*Ftele*N)^2 > .5) THEN
       Fi=PI/2
      ELSE
       Fi=0
      END IF
     C=Start+Ax+Ydelay
     FOR Ii=1 TO 2
       Pakke#(C+Ii,1)=462*(Bitvalue+Ampl*SIN(PI/Ys/2*Ftele*(N+Ii)+Fi)^2)
       Pakke#(C+Ii,2)=Pakke#(C+Ii,1)
       Pakke#(C+Ii,3)=Pakke#(C+Ii,1)
'       PRINT C+Ii;Pakke#(C+Ii,1)
     NEXT Ii
'     gosub Pause
     N=INT(1/Ftele*Bitnr*Ys+.5)
     Bitvalue=J
     Bitnr=Bitnr+1
     Ax=Ax+(N-Nold)
     IF N-Nold=0 THEN
       Ax=Ax+2
     END IF
     Nold=N
   NEXT I
   print "Ax=";Ax
  IF (Editl=1) AND (Linstart!(X+1,5)<>13) THEN 250
   Ae=Ae+1
   IF (Editl=1) AND (Linstart!(X+1,5)=13) THEN
     Hi="Y"
    ELSE
     INPUT "FLERE BYTES TIL TELETEKST Y/N",Hi
   END IF
   IF Hi="Y" THEN 100
  250:
   IF (SIN(PI/2/Ys*Ftele*N)^2 > .5) THEN
     Fi=PI/2
    ELSE
     Fi=0
   END IF
   C=Start+Ax+Ydelay
   Ampl=-Bitvalue
   FOR Ii=1 TO 2
     Pakke#(C+Ii,1)=462*(Bitvalue+Ampl*SIN(PI/Ys/2*Ftele*(N+Ii)+Fi)^2)
     Pakke#(C+Ii,2)=Pakke#(C+Ii,1)
     Pakke#(C+Ii,3)=Pakke#(C+Ii,1)
   NEXT Ii
   Ax=Ax+5
   PRINT "Ax=";Ax,Pakke#(C+Ii,1)
   GOTO 2145
   RETURN
'
'  ********************************************************
'
 Ntpulser:
   IF FastEditl=0 THEN
    PRINT "BREDDE AF n*T-PULS (ns)";Linstart!(X,7);" ?"
    CALL InputHandler (1,A,B,C,D,D1,Hi)
    IF D1 > 0 THEN
     Linstart!(X,7)=A
    END IF
    IF Linstart!(X,7) < 160E-9 THEN Ntpulser
    PRINT  "CENTERPLACERNIG AF n*T-PULS OG MAX AMPLITUDER"
    PRINT USING "### ###.### ###.### ###.###";Linstart!(X,1),_
    Linstart!(X,2),Linstart!(X,3) Linstart!(X,4)
    CALL InputHandler (4,A,B,C,D,D1,Hi)
    IF D1 > 0 THEN
     Linstart!(X,1)=A
     Linstart!(X,2)=B
     Linstart!(X,3)=C
     Linstart!(X,4)=D
    END IF
   END IF
   ON ERROR GOTO 0
   K=Linstart!(X,2) : K2=Linstart!(X,3) : K3=Linstart!(X,4) ' Amplitude af puls
   L=2*Ys*Linstart!(X,7)
   M=INT(L+1)
   Ii=INT(Linstart!(X,1)+.5)
   Fi=(Linstart!(X,1)-Ii)/M*PI
   C=1
   IF Linstart!(X,7) < 2.05E-7 AND Linstart!(X,7) >160E-9 THEN
    M=M-1
   END IF
   Fi=Fi+.5*(PI/L+(PI/L*M-PI))
   B=INT(M/2+1.5)
   Ai=1
   FOR I=1 TO M
     J=Pakke#(Start+Ii+I-(B-1)+Ydelay,1)
     J2=Pakke#(Start+Ii+I-(B-1)+Ydelay,2)
     J3=Pakke#(Start+Ii+I-(B-1)+Ydelay,3)
     Ampl=SIN(PI*I/L-Fi)^2
     G=C*(K-J)*Ampl+J
     Bl=(K2-J2)*Ampl+J2
     R=(K3-J3)*Ampl+J3
     Chrampl=1
     IF Linstart!(X,7)>820E-9 AND Linstart!(X,7)<835E-9 THEN
       GOSUB Korkina10T
       Chrampl=1.0820
     END IF
     IF Linstart!(X,7)>995E-9 AND Linstart!(X,7)<1005E-9 THEN
'       GOSUB Kor10T
       Chrampl=1.06
     END IF
     IF Chrampl > 0 THEN
       Y=.299*R+.587*G+.114*Bl
       U=Ai*Chrampl*(Bl-Y)*.493
       V=Ai*Chrampl*(R-Y)*.877
       Bl=U/.493+Y
       R=V/.877+Y
       G=(Y-.299*R-.114*Bl)/.587
'     PRINT USING "I ## Y   -###.###  U  -###.###   V   -###.###";I,Y,U,V
'     IF I MOD 20 = 0 THEN GOSUB Pause
     END IF
     Pakke#(Start+Ii+I-(B-1)+Ydelay,1)=G
     Pakke#(Start+Ii+I-(B-1)+Ydelay,2)=Bl
     Pakke#(Start+Ii+I-(B-1)+Ydelay,3)=R
   NEXT I
   Ax=INT(Linstart!(X,1)+M-B+4+.5)
   IF Editl=1 THEN Lstop
   GOTO 2145
  Korkina10T:
    SELECT CASE I
      CASE 1
        Ai=-2/12*.82
      CASE 3
        Ai=52/63*.81
      CASE 5
        Ai=135/143*.89
      CASE 7
        Ai=228/235*.96
      CASE 9
        Ai=316/319
      CASE 11
        Ai=1
      CASE M-11
        Ai=1.003
      CASE M-9
        Ai=1.004
      CASE M-7
        Ai=133/143'*134/138
      CASE M-5
        Ai=137/143*60/61*.89
      CASE M-2
        Ai=-.5*.96
      CASE ELSE
    END SELECT
    RETURN
'
'  ********************************************************
'
  Lsweep:
   ON ERROR GOTO SweepError
   IF FastEditl=0 THEN
    PRINT "LINIE-SWEEP FRA 0 Hz TIL ? Hz",Linstart!(X,6);"Hz"
    CALL InputHandler (1,A,B,C,D,D1,Hi)
    IF D1 > 0 THEN
     Linstart!(X,6)=A
    END IF
   END IF
   Yr=Linstart!(X,7)
   Cr=Linstart!(X,8)
   IF FastEditl=0 THEN
    PRINT "STIGETID FOR KANAL 1 ";Yr;"OG FOR KANAL 2 & 3";Cr;
    CALL InputHandler (2,A,B,C,D,D1,Hi)
    IF D1 > 0 THEN
     Yr=A
     Cr=B
    END IF
   END IF
    Linstart!(X,7)=Yr
    Linstart!(X,8)=Cr
   IF FastEditl=0 THEN
    PRINT "VARIGHED OG AMPLITUDER AF LINIE-SWEEP",Linstart!(X,1),_
    Linstart!(X,2),Linstart!(X,3),Linstart!(X,4)
    CALL InputHandler (4,A,B,C,D,D1,Hi)
    IF D1 > 0 THEN
     Linstart!(X,1)=A
     Linstart!(X,2)=B
     Linstart!(X,3)=C
     Linstart!(X,4)=D
    END IF
   END IF
   J=Linstart!(X,2)   : Linstart!(X,2)=J/2+Linstart!(X-1,2)
   J2=Linstart!(X,3)  : Linstart!(X,3)=J2/2+Linstart!(X-1,3)
   J3=Linstart!(X,4)  : Linstart!(X,4)=J3/2+Linstart!(X-1,4)
   B=INT(1.03734*Yr*Ys)          ' Samples pga. Shape
   Linstart!(X,1)=Linstart!(X,1)-B
'   Ax=Ax+B
   GOSUB Fyld                      ' Her laves indhyldningskurven til sinus
   I=Ax+Start+Ydelay
'   Pakke#(I,1)=Linstart!(X-1,2)      ' Nulniveau for sweep
'   Pakke#(I,2)=Linstart!(X-1,3)
'   Pakke#(I,3)=Linstart!(X-1,4)
'   CALL Shape(Pakke#(),Ax+Start,0,Yr,Ys,Cr,Cs,Ydelay,4*Cdelay,PI)
'       For-flanke
   Pakke#(I+Linstart!(X,1),1)=Linstart!(X-1,2)     '  Symmetrisk sinus
   Pakke#(I+Linstart!(X,1),2)=Linstart!(X-1,3)     '  Symmetrisk sinus
   Pakke#(I+Linstart!(X,1),3)=Linstart!(X-1,4)     '  Symmetrisk sinus
   Ax=Ax+Linstart!(X,1)-1           ' 50% pkt for bagflanke
   CALL Shape(Pakke#(),Ax+Start,0,Yr,Ys,Cr,Cs,Ydelay,4*Cdelay,PI)
'     Bag-flanke
   Linstart!(X,2)=J   : Linstart!(X,3)=J2   : Linstart!(X,4)=J3
   K=Linstart!(X-1,2) : K2=Linstart!(X-1,3) : K3=Linstart!(X-1,4)
   Linstart!(X,1)=Linstart!(X,1)+B
   Ax=Ax+B+1
   Freq=Linstart!(X,6)/2
   FOR I=1 TO Linstart!(X,1)
     Plads=Ax+Start+Ydelay+I-Linstart!(X,1)
     J=Pakke#(Plads,1)
     J2=Pakke#(Plads,2)
     J3=Pakke#(Plads,3)
     Pakke#(Plads,1)=K+(J-K)*(SIN((Freq/Linstart!(X,1)/Ys*2*PI*I^2)))
     Pakke#(Plads,2)=K2+(J2-K2)*(SIN(Freq/Linstart!(X,1)/Ys*2*PI*I^2))
     Pakke#(Plads,3)=K3+(J3-K3)*(SIN(Freq/Linstart!(X,1)/Ys*2*PI*I^2))
   NEXT I
  SweepRestart:
   Pakke#(Start+Ax+Ydelay,1)=Linstart!(X-1,2)
   Pakke#(Start+Ax+Cdelay,2)=Linstart!(X-1,3)
   Pakke#(Start+Ax+Cdelay,3)=Linstart!(X-1,4)
   IF Editl=1 THEN Lstop
   GOTO 2145    'SLUT Pè LINIE SWEEP RUTINEN
'
  SweepError:
   PRINT "ERROR Nr:";ERR
   RESUME SweepRestart
'
'   ******************************************************
'
' GEMNING AF LINIETABEL
  Gemdata:
   I=3
   PRINT "SKAL DER LAVES NY FIL (1), ELLER ER DEN GAMLE GOD NOK (2)?"
   WHILE INSTAT =0
   WEND
   I=VAL(INKEY$)
   IF (I<1) OR (I>3) THEN Gemdata
   ON I GOTO 3300,3300,3290
  3290:
   PRINT "DER ER IKKE GEMT NOGET AF BIBLIOTEKEK"
   RETURN
  3300:
   IF I=1 THEN
     PRINT "NAVN Pè GAMMEL FIL";Filename$
    ELSE
     PRINT " INDTAST NAVN Pè DEN NYE FIL (MAX 5 KARAKTERER)?";
   END IF
   CALL InputHandler (6,A,B,C,D,D1,Hi)
   IF D1 > 0 THEN
     Filename$=Hi
   ELSE
   END IF
   Hn=Filename$
   PRINT "DETTE KOMMER TIL AT TAGE NOGEN TID"
   CALL MatprintS2 (Linstart!(),2000,8,Hn)    ' HERI GEMMES DE INDTASTEDE DATA
   CALL MatprintI2 (Titles%(),161,2,Hn+"S")
   CALL MatprintStr (Title$(),160,Hn+"T")
   RETURN
'  Linier bliver gemt samtidig med, at de bliver lavet.
'
'  *************************************************************
'
  Sammenlign:
   RETURN    ' NY LINIE
'
'  *******************************************************
'
  FourierPakning:
'               FIL TIL FOURIER-PROGRAM. 2K SAMPLES. 0-1K ER PUNKT 1-3-5-7 OSV.
'               OG 1K-2K ER PUNKT 2-4-6-8 OSV.
   Xi=0
   FOR X=1 TO Slut-1 STEP 2
     Xi=Xi+1
     Real#(Xi)=Pakke#(X,1)
     Imag#(Xi)=Pakke#(X+1,1)
   NEXT X
   RETURN
'
'  *******************************************************
'
  Repak:
   Xi=0
   FOR I=1 TO Slut-1 STEP 2
     Xi=Xi+1
     Pakke#(I,1)=Real#(Xi)
     Pakke#(I+1,1)=Imag#(Xi)
   NEXT I
   RETURN
'
'  *******************************************************
'
  Sinccor:              ' KORRIGERER FILTERKARAKTERISTIKKEN MED SINC-FUNKTIONEN
   CLS
   PRINT "SINC-KORRIGERER FILTER"
   A=2*Filter#(1)
   Filter#(1)=Filter#(1)/A             ' DC-VALUE
   Filter#(1+512)=Filter#(1+512)/A   ' MAX. FREQ. VALUE
   FOR I=2 TO 448' 419
     Filter#(I)=(SIN(PI*I/1024)/(PI*I/1024))*Filter#(I)/A
     Filter#(I+512)=(SIN(PI*I/1024)/(PI*I/1024))*Filter#(I+512)/A
'     PRINT I, SQR(Filter#(I)^2+Filter#(I+512)^2)
   NEXT I
     FOR I=449 TO 512  '  420 TO 512
     B=5*I/(512-449)+(512-6*449)/(512-449)
     C=.27*B^2-.63*B+1.36
'     PRINT I,C
'     IF I MOD 20 =0 THEN GOSUB Pause
     Filter#(I)=(SIN(PI*I/1024)/(PI*I/1024))*Filter#(I)*C/A
     Filter#(I+512)=(SIN(PI*I/1024)/(PI*I/1024))*Filter#(I+512)*C/A
   NEXT I
'    PRINT I,C
'    GOSUB Pause
   Filter#(513)=SQR(Filter#(512)^2+Filter#(1024)^2)
   FOR I=38 TO 418
     Ampl=1-1/200*(1+COS(PI/190*(I-228)))
     Filter#(I)=Filter#(I)*Ampl
     Filter#(I+512)=Filter#(I+512)*Ampl
   NEXT I

   FOR I=372 TO 508  ' Placering i frekvensomrÜdet.
     Ampl=1-1/60*(1+COS(PI/68*(I-440)))
     Filter#(I)=Filter#(I)*Ampl
     Filter#(I+512)=Filter#(I+512)*Ampl
'     print I,Ampl
'     if I mod 20 = 0 then gosub Pause
   NEXT I

'  Sidste lõkke dëmper filterkorrektionens virkning
   RETURN
'
'  *******************************************************
'
  Div:   ' Regner baglëns i filter
   PRINT "KORRIGERER INPUT-DATA MED FILTERKAREKTERISTIK"
'  Real#=Real-del af õnsket kurve
'  Imag#=Imaginaer-del af õnsket kurve
'  Filter=Filterkarakteristik
   Real#(1)=Real#(1)/Filter#(1)/2
   Imag#(1)=Imag#(1)/Filter#(513)
   FOR I=2 TO 512
     A=Real#(I)
     B=Imag#(I)
     C=Filter#(I)
     D=Filter#(I+512)
     Real#(I)=(A*C+B*D)/(C^2+D^2)
     Imag#(I)=(B*C-A*D)/(C^2+D^2)
   NEXT I
   RETURN

$SEGMENT
'
'  *******************************************************
'
  FastEdit:    ' HURTIGEDITERING I LINIER
   FastEditl=1
  Edit:        ' EDITERING I LINIER
  Editl=1
   IF FastEditl=0 THEN
     PRINT "DER SKAL EDITERES  LINIE",
    ELSE
     PRINT "DER SKAL EDITERES FRA, TIL ?,
   END IF
   INPUT Ne1,Ne2
   IF Ne1 > Ne2 THEN
     Ne2=Ne1
   END IF
   SELECT CASE Ne2
     CASE > Nmax
      PRINT "LINIENUMMER ER FOR STORT, PRùV IGEN"
      GOTO Edit
     CASE 0
      RETURN
     CASE ELSE
   END SELECT
   Nmax1=Nmax
   FOR Ne=Ne1 TO Ne2
     Nmax=Ne
     ERASE Pakke#
     ERASE Real#
     ERASE Imag#
     PRINT Title$(Ne);
     IF FastEditl=0 THEN
       CALL InputHandler (6,A,B,C,D,D1,Hi)
       IF D1 > 0 THEN
         Title$(Ne)=Hi  ' Linie, der skal editeres
       END IF
     END IF
     Ax=0
     Ae=Linstart!(2000-Ne \ 8,Ne MOD 8+1)+1
     WHILE Ax<Aktivlinie
       Hi="R"
       IF Linstart!(Ae,5)>0 THEN 4235
       IF FastEditl=0 THEN
         GOSUB Inds
         Linstart!(Ae,5)=1
        ELSE
         PRINT "FEJL I LINIEDATA, BRUG ALM Edit !"
         PRINT "TRYK Pè EN TASTE FOR AT FORTSíTTE"
         WHILE INSTAT =0
         WEND
         Hi=INKEY$
         EXIT FOR
       END IF
     4235:
       IF FastEditl=1 GOTO 4330
       PRINT "Ax";Ax
       PRINT Tout$(Linstart!(Ae,5));
       SELECT CASE Linstart!(Ae,5)
        CASE 2,7,9,12
         GOTO 4245
        CASE ELSE
         GOTO 4250
       END SELECT
      4245:
       PRINT "FREKVENS";Linstart!(Ae,6)/1E6;"MHz"
      4250:
       PRINT USING "NYT INPUT ? ####.###  ####.###  +###.##  +###.##";_
       Linstart!(Ae,1),Linstart!(Ae,2),Linstart!(Ae,3),Linstart!(Ae,4)
       ON Linstart!(Ae,5) GOTO 4260,4260,4260,4265,4265,4265,4265,4265,4260,4260_
       ,4265,4260,4265
      4260:
       PRINT "LUMINANCE STIGETID";Linstart!(Ae,7);_
       "OG CHROMASTIGETID";Linstart!(Ae,8)
      4265:
       PRINT  "R=ret, S=slet, I=inds{t "
       WHILE INSTAT = 0
       WEND
       Hi=UCASE$(INKEY$)
       IF Hi="S" THEN Slet
       IF Hi="I" THEN GOSUB Inds
        PRINT Tout$(Linstart!(Ae,5));_
        "(KONSTANT NIVEAU=1,SINUS=2,RAMPE=3,20 T=4)";
        CALL InputHandler (1,A,B,C,D,D1,Hi)
       IF D1 > 0 THEN
         Linstart!(Ae,5)=A
       END IF
       ON Linstart!(Ae,5) GOTO 4300,4305,4300,4330,4330,4330,4330,4330_
       ,4305,4305,4330,4330,4330
      4300:
       Linstart!(Ae,6)=0                       ' Frekvens af sinus
      4305:
       PRINT "LUMINANCE STIGETID";Linstart!(Ae,7);"OG CHROMASTIGETID";_
       Linstart!(Ae,8);
       CALL InputHandler (2,A,B,C,D,D1,Hi)
       IF D1 > 0 THEN
         Linstart!(Ae,7)=A
         Linstart!(Ae,8)=B    '      Input Y og Chr. rise-time
       END IF
       PRINT "NY VARIGHED , NY Y-AMPL. (mV), NY FARVE-AMPL. B-Y,R-Y (mV)";
       CALL InputHandler (4,A,B,C,D,D1,Hi)
       IF D1 > 0 THEN
        Linstart!(Ae,1)=A
        Linstart!(Ae,2)=B
        Linstart!(Ae,3)=C
        Linstart!(Ae,4)=D
       END IF
       PRINT USING "\\  ####.###  ####.###  +###.##  +###.##";_
       Hi,Linstart!(Ae,1),Linstart!(Ae,2),Linstart!(Ae,3),Linstart!(Ae,4)
      4330:
       X=Ae-1          ' Ae-1 da subrutinen starter med at legge 1 til x
       GOSUB 2225     ' Lav linie-element med flanke og form (2T 20T osv.)
       Ae=Ae+1
       IF FastEditl=0 THEN
         PRINT  : PRINT
       END IF
      4350:
       WEND
       PRINT "LINIEN ER SLUT, JEG REGNER"
       J=Linstart!(2000-(Nmax1+1) \ 8,(Nmax1+1) MOD 8+1)+1
       Linstart!(J,1)=1
      4370:
'      IF Linstart!(Ae,1)<>0 THEN Slet
'      LAV Pakke UD FRA DE EDITEREDE INPUTDATA
       X=Ae-1
       Sr=Linstart!(X+1,6)
       Yr=Linstart!(X+1,7)
       Cr=Linstart!(X+1,8)
       GOSUB 2385       ' Lav bagflanke, FFT rutiner og kvantisering
'       CALL Kvant(Pakke#(),(Ylsb),Slut)  ' Kvantiserer kanal 1
'       CALL Ckvant(Pakke#(),(Clsb),Slut) ' kvantiserer kanal 2 og 3
       SELECT CASE LEFT$(Title$(Nmax),1)
        CASE "$"   '  Ydel AF 2-WIRE SIGNAL
         Inmax&=Titles%(Nmax,1)
         Incmax&=0
         CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&,Hstorename)
         IF Mode = 1 THEN
           GOSUB Field2
           CALL MatprintRandomI2( Pakke#(),Inmax&+1,Incmax&,Hstorename)
         END IF
        CASE "ú"   '  Chromadel AF 2-WIRE SIGNAL
         Inmax&=0
         Incmax&=Titles%(Nmax,2)
         IF LEFT$(Title$(Nmax),2)="úú" THEN
           CALL MatprintBetaI2( Pakke#(),Inmax&,Incmax&,Hstorename)
          ELSE
           CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&,Hstorename)
           IF Mode = 1 THEN
             GOSUB Field2
             CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&+1,Hstorename)
           END IF
        END IF
        CASE ELSE
         Inmax&=Titles%(Nmax,1)
         Incmax&=Titles%(Nmax,2)
         CALL MatprintRandomI2( Pakke#(),Inmax&,Incmax&,Hstorename)
         IF Mode = 1 THEN
           GOSUB Field2
           CALL MatprintRandomI2( Pakke#(),Inmax&+1,Incmax&+1,Hstorename)
         END IF
        END SELECT
'      Gem promdata pÜ disk
     NEXT Ne
   Nmax=Nmax1       ' Re-etabler variabel
   Editl=0          ' Nulstil flag til edit mode
   FastEditl=0
   RETURN           ' Returner fra edit-mode
'
'
  Slet:
   FOR I=Ae TO Linstart!(2000-(Nmax1+1) \ 8,(Nmax1+1) MOD 8+1)+1
     FOR Ii=1 TO 8
       Linstart!(I,Ii)=Linstart!(I+1,Ii)
     NEXT Ii
   NEXT I
   B=-1
   GOSUB Chlinnr
   IF Ax<Aktivlinie THEN GOTO 4350
   GOTO 4370         ' Slut paa Slet
'
'
  Inds:
   J=Linstart!(2000-(Nmax1+1) \ 8,(Nmax1+1) MOD 8+1)+1
   FOR I=J TO Ae STEP -1
     FOR Ii=1 TO 8
       Linstart!(I+1,Ii)=Linstart!(I,Ii)
     NEXT Ii
   NEXT I
   B=1
   Linstart!(Ae,5)=1
  Chlinnr:
   J=(Ne+1) MOD 8+1
   FOR I=2000-(Ne+1) \ 8 TO 2000-(Nmax1+1) \ 8 STEP -1
     FOR Ii=J TO 8
       IF Linstart!(I,Ii)>0 THEN
         Linstart!(I,Ii)=Linstart!(I,Ii)+B
       END IF
     NEXT Ii
     J=1
   NEXT I
   RETURN
'
'  ********************************************************
'
  Prommer:
   PRINT "HVILKEN KANAL SKAL ER LAVES PROMMER TIL ",
   WHILE INSTAT=0
   WEND
   N=VAL(INKEY$)
   PRINT N
   INPUT "DER SKAL LAVES LINIENUMMER (FRA TIL)" Nr,Nr2

  PromY:
   PRINT "HVILKEN AF DE 5 PROMMER SKAL LAVES  (A=alle 5)"
   WHILE INSTAT=0
   WEND
   Hi=UCASE$(INKEY$)
   IF Hi="A" THEN
     Iu1=1
     Iu2=5
     PRINT UCASE$(INKEY$)
    ELSE
     Iu1=VAL(INKEY$)
     Iu2=Iu1
     PRINT Iu
   END IF
   CALL MatreadI1(NmaxArray%(),2,Path$+"Nmax")  ' HENTER Nmax FOR BEGGE
   IF N=1 THEN                                     ' KANALER
    Nmax=NmaxArray%(1)
   ELSE
    Nmax=NmaxArray%(2)
   END IF
   FOR Iu =Iu1 TO Iu2
     PRINT "PROMNR: ";Iu
     LOCATE CSRLIN-1,1
     OPEN Path$+"PROM"+MID$(H,N+1,1)+MID$(H,Iu+1,1)+".HEX" AS #2 LEN=3
     FIELD #2, 3 AS Ha
     RSET Ha=CHR$(2)
     PUT #2,1
     IF Iu<5 THEN
       FOR I=Nr TO Nr2
         Iii=0
         CALL MatreadRandomI2 ( Pakke#(),I,N,Hstorename+MID$(H,N+1,1))
         FOR Ii=Iu TO 1024-(4-Iu) STEP 4
           Ioutput=Pakke#(Ii,N)\4
           CALL Hex(Houtput,Ioutput,H)
           LSET Ha=MID$(Houtput,4,2)
           PUT #2,2+Iii+(I-1)*256
           Iii=Iii+1
         NEXT Ii
       NEXT I
     ELSE
       FOR I=Nr TO Nr2
         CALL MatreadRandomI2 ( Pakke#(),I,N,Hstorename+MID$(H,N+1,1))
         Io=0
         FOR Ii=1 TO 1024 STEP 4
           Ioutput=0
           FOR Iii=0 TO 3
             X=Pakke#(Ii+Iii,N) MOD 4
             Ioutput=Ioutput + (X MOD 2)*(2^Iii) +(X \ 2)*16*(2^Iii)
           NEXT Iii
           CALL Hex(Houtput,Ioutput,H)
           LSET Ha=MID$(Houtput,4,2)
           PUT #2,2+Io+(I-1)*256
           Io=Io+1
         NEXT Ii
       NEXT I
     END IF
     LSET Ha=CHR$(3)
     PUT #2,2+Nmax*256
     LSET Ha=CHR$(10)+CHR$(26)
     PUT #2
     CLOSE #2
   NEXT Iu
   PRINT "SKAL DER LAVES FLERE PROMMER Y/N ?"
   WHILE INSTAT=0
   WEND
   Hind=UCASE$(INKEY$)
   IF Hind="Y" THEN PromY
   RETURN
'
'  *******************************************************
'
  SUB MatprintBetaI2 (Array#(2),Size1&,Size2&,Hname) ' SUBRUTINE DER GEMMER
      Hi=INKEY$
      OPEN Hname+"2.DAT"  AS #1 LEN=2                ' HER GEMMES KANAL 2
        FIELD #1, 2 AS Ha
        FOR I=1 TO 160
          LSET Ha=MKI$(Array#(I,2))
          PUT #1,(Size2&-1)*1024+I
        NEXT I
        FOR I=161 TO 988
          LSET Ha=MKI$(Array#(I,2))
          PUT #1,(Size2&-1)*1024+I+36
        NEXT I
        FOR I=989 TO 1024
          LSET Ha=MKI$(Array#(I,2))
          PUT #1,(Size2&-1)*1024+I-828
        NEXT I
        CLOSE #1
        OPEN Hname+"3.DAT"  AS #1 LEN=2                ' HER GEMMES KANAL 3
        FIELD #1, 2 AS Ha
        FOR I=1 TO 1024
          LSET Ha=MKI$(Array#(I,3))
          PUT #1,(Size2&-1)*1024+I
        NEXT I
        CLOSE #1
  END SUB
'  *******************************************************
'
   SUB Hex(Had,A&,H)                ' KONVERTER A TIL HEX OG RETURNERER I Had
       Had="     "
       ON ERROR GOTO DivErr
       MID$(Had,1)=MID$(H,INT(A& / 65536)+1,1)
       B&=A& - 65536*INT(A& / 65536)
       MID$(Had,2)=MID$(H,INT(B& / 4096)+1,1)
       C&=B& - 4096*INT(B& / 4096)
       MID$(Had,3)=MID$(H,INT(C& / 256)+1,1)
       D&=C& -256*INT(C& / 256)
       MID$(Had,4)=MID$(H,INT(D& / 16)+1,1)
       MID$(Had,5)=MID$(H,D&-16*INT(D& / 16)+1,1)
       EXIT SUB
      DivErr:
       RESUME Divrst
      Divrst:
   END SUB
'
'  *******************************************************
'
   SUB Shape( P#(2),Ax,Linstart!,Yr,Ys,Cr,Cs,Ydelay,Cdelay,PI)
       ON ERROR GOTO ShapeError
       OPTION BASE 1
       C=INT(1.03734*Yr*Ys)
       IF C=0 THEN 590           ' Skip hvis stigetid er nul
       Fi=-(Linstart!-INT(Linstart!+.5))*2*PI/(2*(C+1))
       DeltaY=-(Linstart!-INT(Linstart!+.5))
       J=P#(Ax+Ydelay,1)         ' Gammelt niveau
       K=P#(Ax+1+Ydelay,1)       ' Nyt niveau
       IF K-J=0 THEN 590         ' Skip hvis niveau'erne er ens
       FOR I=Ax-C+Ydelay TO Ax+C+Ydelay
         Y=(I-(Ax+Ydelay-DeltaY))/Yr/Ys/2.07468+.5
         P#(I,1)=(K-J)*(Y-SIN(2*PI*Y)/(2*PI))+J
'        PRINT I,P#(I,1),Y
       NEXT I
      590:
       C=INT(1.03734*Cr*Cs)
       IF C=0 THEN EXIT SUB
       Fi=-(Linstart!-INT(Linstart!+.5))*2*PI/(2*(C+1))
       DeltaY=-(Linstart!-INT(Linstart!+.5))
       J=P#(Ax+Cdelay,2)
       K=P#(Ax+1+Cdelay,2)
       L=P#(Ax+Cdelay,3)
       M=P#(Ax+1+Cdelay,3)
       FOR I=Ax-C+Cdelay TO Ax+C+Cdelay
         Y=(I-(Ax+Cdelay-DeltaY))/Cr/Cs/2.07468+.5
         P#(I,2)=(K-J)*(Y-SIN(2*PI*Y)/(2*PI))+J
         P#(I,3)=(M-L)*(Y-SIN(2*PI*Y)/(2*PI))+L
       NEXT I
      ShapeRst:
       EXIT SUB
      ShapeError:
      PRINT "Shape ERROR No:"ERR
      RESUME ShapeRst
   END SUB

' ********************************************************

  Field1:   ' Her indsëttes EAV/SAV for linien, den skal bruges til aktiv
            ' linie!
    Pakke#(721,1) = 0           	
    Pakke#(722,1) = 628     'EAV
    Pakke#(721,2) = 1023           	
    Pakke#(722,2) = 1023           	
    Pakke#(721,3) = 0           	
    Pakke#(722,3) = 0           	
    Pakke#(863,1) = 0           	
    Pakke#(864,1) = 512     'SAV
    Pakke#(863,2) = 1023           	
    Pakke#(864,2) = 1023           	
    Pakke#(863,3) = 0           	
    Pakke#(864,3) = 0           	
    RETURN
'  *******************************************************

  Field2:
    PRINT "Field2"
    Pakke#(721+16,1) = 0           	
    Pakke#(722+16,1) = 872     'EAV
    Pakke#(721+16,2) = 1023           	
    Pakke#(722+16,2) = 1023           	
    Pakke#(721+16,3) = 0           	
    Pakke#(722+16,3) = 0           	
    Pakke#(863+16-864,1) = 0           	
    Pakke#(864+16-864,1) = 796     'SAV
    Pakke#(863+16-864,2) = 1023           	
    Pakke#(864+16-864,2) = 1023           	
    Pakke#(863+16-864,3) = 0           	
    Pakke#(864+16-864,3) = 0           	
    RETURN

'  *******************************************************
'
  Printervalg:   ' SUBRUTINE DER VíLGER PRINTER
   PRINT "ùNSKES PAPIRUDSSKRIFT AF DATA  Y/N ?(F=FIL:TEKST.UD)"
   WHILE INSTAT=0
   WEND
   Hi=UCASE$(INKEY$)
    SELECT CASE Hi
     CASE "Y"
      OPEN "LPT1:" FOR OUTPUT AS #1
      ON ERROR GOTO PrinterError
      PRINT #1," "; : PRINT #1," ";
      PRINT #1,CHR$(27)+"N"+CHR$(5);CHR$(27)+"6";
     CASE "F"
       PRINT "NY FIL ELLER GAMMEL N/G";
       WHILE INSTAT=0
       WEND
       Hi=UCASE$(INKEY$)
       IF Hi="N" THEN
         OPEN "TEKST.UD" FOR OUTPUT AS #1
        ELSE
         OPEN "TEKST.UD" FOR APPEND AS #1
       END IF
       Hi="F"
     CASE ELSE
      OPEN "SCRN:" FOR OUTPUT AS #1
     END SELECT
   RETURN
'
  PrinterError:
   CLOSE #1
   BEEP
   PRINT "PRINTER ER IKKE KLAR"
   PRINT "TRYK Pè EN TASTE FOR AT FORTSíTTE"
   WHILE INSTAT=0
   WEND
   Hi=INKEY$
   RESUME Printervalg
'
  Resetprinter:
   IF Hi="Y" THEN
     PRINT #1,CHR$(12);
    ELSE
   END IF
   CLOSE #1
   RETURN
'
'  *******************************************************
'
   SUB Kvant(K#(2),A,Slut)            ' KVANTISERER K(*) TIL 10 BITS OPL0SNING
       FOR I=1 TO Slut
         K#(I,1)=(INT(K#(I,1)/A+.5)+64)
       NEXT I
    END SUB
'
'  *******************************************************
'
   SUB Ckvant(K#(2),A,Slut)       ' KVANTISER K(*,2&3) TIL 10 BIT
       FOR I=1 TO Slut
         K#(I,2)=512+INT(K#(I,2)/A+.5)
         K#(I,3)=512+INT(K#(I,3)/A+.5)
         NEXT I
   END SUB
'
'  *******************************************************
'
