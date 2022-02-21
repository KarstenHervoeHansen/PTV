'  Udvidet 980209 med linieadressen. Denne er ëndret tl at tage hensyn til den
'  mÜde linier adresseres pÜ i digital video generatorer. (Adressen õges med
'  0 1 eller 2 afhëngig af hvilken type linien oprindeligt er.)

'  PROGRAM NAME IS "kinacopa"! 900422
   CLS
   OPTION BASE 1
   DEFLNG I,N,X
   DEFSTR H
   DEFDBL A,B,Y,C,D,P,S,J,K,L,M
   DIM Title$(160),Title2$(160),Titles%(161,2),Titles2%(161,2)
   DIM Tout$(15),NmaxArray(2)
   DIM Linstart!(2000,8),Til!(2000,8)
   H="0123456789ABCDEF"
   COLOR 14
   Filename$="NONAME"
   Hstorename="C:\P5643\CHANNEL"
   Nmax=0
   NmaxTo=0
'
'  ***********************************************************************
'
'  HOVEDPROGRAM
   DO
   PRINT : CLS
   I=1
   PRINT "SKAL DER:    1)HENTES NY INPUT FIL ? "
   PRINT TAB(14) "2)HENTES NYT SLUT-BIBLIOTEK?"
   PRINT TAB(14) "3)FLYTTES DATA?"
   PRINT TAB(14) "4)GEMMES NYT LINIEBIBLIOTEK?"
   PRINT TAB(14) "5)STATUS?"
   PRINT TAB(14) "6)STOP?"
   WHILE INSTAT=0
   WEND
   I=VAL(INKEY$)
   IF I>5 THEN EXIT LOOP
   ON I GOSUB Taperead,Tapereadslut,Edit,Gemdata,Status
   LOOP
   END
'
'  *******************************************************
'
  Status:
   PRINT "ANTAL LINIER I GAMMELT BILLIOTEK ",Nmax
   PRINT "ANTAL INDTASTNINGER I GAMMELT",Linstart!(2000-(Nmax+1)\8,_
                                                     (Nmax+1)MOD 8+1)
   PRINT "ANTAL LINIER I NYT BIBLIOTEK ",NmaxTo
   PRINT "ANTAL INDTASTNINGER I NYT",Til!(2000-(NmaxTo+1)\8,(NmaxTo+1)MOD 8+1)
   WHILE INSTAT=0
   WEND
   Hi=INKEY$
   RETURN
'
'  *******************************************************
'
  Tapereadslut:
'   INDLíSERUTINE
   NmaxTo=0
   ERASE Titles2%
   ERASE Til!
   ERASE Title2$
   INPUT "ANGIV NAVN Pè FIL MED NUVíRENDE PROMDATA ? ",Hn
   Filename$=Hn
  DiskIndslut:
   ON ERROR GOTO DiskErrorslut
   OPEN Hn+".DAT" FOR INPUT AS #1
   CLOSE #1
   CALL MatreadS2(Til!(),2000,8,Hn)
   CALL MatreadI2(Titles2%(),161,2,Hn+"S")
   Istart=3
   FOR I=0 TO 22
     FOR Ii=Istart TO 8
       IF Til!(2000-I,Ii) > 0 THEN
         NmaxTo=NmaxTo+1
        ELSE
         EXIT FOR
       END IF
     NEXT Ii
     Istart=1
   NEXT I
   FOR I=2 TO NmaxTo+1
     IF (Titles2%(I,1)<>0) THEN
       Npointer=Titles%(I,1)
     END IF
     IF (Titles2%(I,2)<>0) THEN
       Ncpointer=Titles2%(I,2)
     END IF
   NEXT I
'   PRINT I,Npointer,Ncpointer
'   WHILE INSTAT=0
'   WEND
   Hi=INKEY$
   CALL MatreadStr(Title2$(),160,Hn+"T")   ' FIND TITLER Pè LINIER
   RETURN
'  Liniedata hentes fõrst, nÜr de skal bruges.
'
  DiskErrorslut:
  BEEP
  PRINT "KAN IKKE FINDE ùNSKET BIBLIOTEK:",Hn
  PRINT "SKAL VI PRùVE IGEN (Y/N) ?"
  WHILE INSTAT=0
  WEND
  Hi=UCASE$(INKEY$)
  IF Hi="Y" THEN
    RESUME DiskIndslut
   ELSE
    RESUME Tapereadslut
  END IF
'
'
'   ********************************************************************
'
  Taperead:  '
'   INDLíSERUTINE
   GOSUB NulstilVariable                 ' NULSTIL ALLE VARIABLE
   INPUT "ANGIV NAVN Pè FIL MED NUVíRENDE PROMDATA ? ",Hn
  DiskInd:
   ON ERROR GOTO DiskError
   OPEN Hn+".DAT" FOR INPUT AS #1
   CLOSE #1
   CALL MatreadS2(Linstart!(),2000,8,Hn)
   CALL MatreadI2(Titles%(),161,2,Hn+"S")
   Istart=3
   FOR I=0 TO 22
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
        LINE INPUT #2, Array$(I)            ' L[SER OGS] KOMMA'ER
        Array$(I)=MID$(Array$(I),2,LEN(Array$(I))-2)
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
  SUB MatprintBetaI2 (Array#(2),Size1&,Size2&,Hname) ' SUBRUTINE DER GEMMER
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
      IF LEN (Hinput) < Number THEN ExitSub
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
      ELSE
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
'  *******************************************************
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
   CALL MatprintS2 (Til!(),2000,8,Hn)    ' HERI GEMMES DE INDTASTEDE DATA
   CALL MatprintI2 (Titles2%(),161,2,Hn+"S")
   CALL MatprintStr (Title2$(),160,Hn+"T")
   RETURN
'  Linier bliver gemt samtidig med, at de bliver lavet.
'
'  *************************************************************
'
  Edit:     ' EDITERING I LINIER
   ON ERROR GOTO EditError
   INPUT "DER SKAL KOPIERES LINIER FRA TIL ",N1,N2
   IF N2<N1 THEN
     N2=N1
   END IF
   IF N1=0 THEN
     RETURN
   END IF
   FOR Ilin=1 TO N2-N1+1
     NmaxTo=NmaxTo+1
     I1=Linstart!(2000-(N1+Ilin-1)\8,(N1+Ilin-1)MOD 8+1)+1
     I2=Linstart!(2000-(N1+Ilin)\8,(N1+Ilin)MOD 8+1)
     Ito1=Til!(2000-NmaxTo\8,NmaxTo MOD 8+1)+1
     FOR Iindtast=I1 TO I2
       FOR Ii=1 TO 8
         Til!(Ito1,Ii)=Linstart!(Iindtast,Ii)
       NEXT Ii
     Ito1=Ito1+1
     NEXT Iindtast
'     PRINT N1+Ilin-1,Title$(N1+Ilin-1)
     Title2$(NmaxTo)=Title$(N1+Ilin-1)
     Til!(2000-(NmaxTo+1)\8,(NmaxTo+1) MOD 8+1)=Ito1-1
     IF NmaxTo=1 THEN
       Titles2%(NmaxTo,1)=1
       Titles2%(NmaxTo,2)=1
     END IF

    Ntype=Til!(Ito1-1,4)
    SELECT CASE LEFT$(Title2$(NmaxTo),1)
      CASE "$"
        Titles2%(NmaxTo+1,1)=Titles2%(NmaxTo,1)+1+Ntype
        Titles2%(NmaxTo+1,2)=Titles2%(NmaxTo,2)
      CASE "ú"
        Titles2%(NmaxTo+1,1)=Titles2%(NmaxTo,1)
        Titles2%(NmaxTo+1,2)=Titles2%(NmaxTo,2)+1+Ntype
      CASE ELSE
        Titles2%(NmaxTo+1,1)=Titles2%(NmaxTo,1)+1+Ntype
        Titles2%(NmaxTo+1,2)=Titles2%(NmaxTo,2)+1+Ntype
    END SELECT
   NEXT Ilin
   GOTO Edit
  EditError:
  PRINT "ERROR NUMMER";ERR
  RESUME Edit
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
  Printervalg:   ' SUBRUTINE DER VíLGER PRINTER
   PRINT "ùNSKES PAPIRUDSSKRIFT AF DATA  Y/N ?"
   WHILE INSTAT=0
   WEND
   Hi=UCASE$(INKEY$)
   IF Hi = "Y" THEN
     OPEN "LPT1:" FOR OUTPUT AS #1
     ON ERROR GOTO PrinterError
     PRINT #1," "; : PRINT #1," ";
     PRINT #1,CHR$(27)+"N"+CHR$(5);CHR$(27)+"6";
    ELSE
     OPEN "SCRN:" FOR OUTPUT AS #1
     END IF
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
