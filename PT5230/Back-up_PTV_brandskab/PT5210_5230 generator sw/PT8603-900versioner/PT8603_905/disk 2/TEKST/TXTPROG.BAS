'  PROGRAM NAME IS "TEXTPROG.BAS"! 960926
   CLS
   COLOR 14
   OPTION BASE 1
   DEFLNG I,N,X
   DEFSTR H
   DEFDBL A,B,C,D,E,F,G,P,Q,S,T,U,V,Y
   DIM Real#(1024),Imag#(1024),Filter#(100)
   DIM Array(100),Pakke#(100)
   DIM Prom1%(32767),NyProm2%(32767)
   H="0123456789ABCDEF"
   PI=3.141592654
   Yr=150E-9  ' Stigetid for tekst
   Ys=27E6
   Ylsb=1/88  '99
   Ydelay=0

'  ***********************************************************

' Filter-beregning
' Beregner filter, gemmer filter p† disk "FILTER.TXT"
'    ON ERROR GOTO ShapeError
   C=INT(1.03734*Yr*Ys)
   Fi=0     '-(Linstart!-INT(Linstart!+.5))*2*PI/(2*(C+1))
   DeltaY=0 '-(Linstart!-INT(Linstart!+.5))
   J=0       ' Gammelt niveau
   K=1       ' Nyt niveau
   Ax=C+1
   Glampl=0
   OPEN "FILTER.TXT" FOR OUTPUT AS #1
   FOR I=Ax-C+Ydelay TO Ax+C+Ydelay
     Y=(I-(Ax+Ydelay-DeltaY))/Yr/Ys/2.07468+.5
     Ampl=(K-J)*(Y-SIN(2*PI*Y)/(2*PI))+J
     Filter#(I)=Ampl-Glampl
     Glampl=Ampl
     PRINT I;Filter#(I),Ampl
     WRITE #1,I,Filter#(I),Ampl
   NEXT I
   Filter#(I)=K-Glampl
   PRINT I;Filter#(I),K
   WRITE #1,I,Filter#(I),Ampl
   Nfolde=I    ' Antal, der skal foldes med
   CLOSE

'  ***********************************************************

'  Indl‘s 1. halvdel af prom fra PM8546 tekst-generator.

   OPEN "10258311.BIN" AS #1 LEN=1   ' ET Byte-array
   FIELD #1, 1 AS Ha                 ' l‘ses ind som integer
   FOR I=1 TO 32767
     GET #1,I
      Ha1="  "
      Ha1=Ha+"0"
'      PRINT I;Ha1,CVI(Ha1)-12288
'     IF (I MOD 20)=0 THEN GOSUB Pause1

     Prom1%(I)=CVI(Ha1)-12288
   NEXT I
   CLOSE #1

'  ***********************************************************

'  Program, der l‘ser nyt tekst-bibliotek og beregner nye flanker

   Nchar=1 ' F›rste karakters adresse er 0
   OPEN "CHARFILE.DAT" FOR INPUT AS #1
   OPEN "CHARLIST.DAT" FOR OUTPUT AS #2
'   OPEN "TESTLIST.DAT" FOR OUTPUT AS #3
'   OPEN "TESTPROM.DAT" FOR OUTPUT AS #4
   PRINT #2, " 0,            (INGENTING)"
   DO
     LINE INPUT #1, Hi
     IF MID$(Hi,1,4)="SLUT" THEN EXIT LOOP

'    Her findes adresse p† karakter og antal blokke p† karakter
'    Karakteren gemmes i Char$

     K1=0 : K2=0
     FOR Ii=1 TO LEN(Hi)
       IF (MID$(Hi,Ii,1)=",") OR (Ii=LEN(Hi)) THEN
         K1=Ii
         Adresse=VAL(MID$(Hi,1,K1-1))  ' Adresse p† karakter
'         PRINT "K1=";K1;VAL(MID$(Hi,1,K1-1))
         EXIT FOR
       END IF
     NEXT Ii
     FOR Ii=K1+1 TO LEN(Hi)
       IF MID$(Hi,Ii,1)="," OR Ii=LEN(Hi) THEN
         K2=Ii
       END IF
       IF K2 >K1+1 THEN
         Nblok=VAL(MID$(Hi,K1+1,K2-K1-1)) ' Antal blokke (1, 2 eller 3)
          EXIT FOR
       END IF
     NEXT Ii
     IF LEN(Hi) > K2 THEN
       Char$=MID$(Hi,K2+1,LEN(Hi)-K2)  ' karakteren gemmes til senere
     END IF

     FOR Iy=1 TO 32 'Antal liner til karakter i PT5210-format

'    Her fyldes Pakke#() med data til en linie tekst
       ERASE Pakke#  'Sletter for evt. gamle rester
       ERASE Real#
       ERASE Array
       FOR Ii=1 TO 2*Nblok
         Byte=Prom1%(Adresse+6+Iy +(Ii-1)*64)
         B=0
         FOR Iii=1 TO 8
           J=(Byte-B) \ (256 \ (2^Iii))
           Array(Iii+8*(Ii-1))=J
           B=B+J*(256 \ (2^Iii))
         NEXT Iii
       NEXT Ii
'     Her kan en linie af en karakter foldes med filteret
'     Linien ligger i Array(1) -> Array(8*2*Nblok)

'      PRINT "ADRESSE, BLOK, CHAR";Adresse,Nblok,Char$

      GOSUB Foldning  'Her foldes, resultat afleveres i Real#()

'     Kvantisering

      FOR Ii=1 TO 8*2*Nblok
'        IF Char$="T" THEN
'          print #3, using "###.###";Iy;Ii;Array(Ii),Real#(Ii)
'        END IF
'        if Ii mod 19 =0 then gosub Pause1
        Array(Ii)=128+INT(Real#(Ii)/Ylsb+.5)
      NEXT Ii
'      IF Char$="T" THEN
'        PRINT #3,
'      END IF

'     Gemning i NyProm2%

 '     ON ERROR GOTO Errprint
      IF (Nchar+Nblok) > 127 GOTO Overflow
      FOR Ii=1 TO 8*2*Nblok STEP 2
        A=1+(8*Nchar+((Iy-1) \ 2)*1024+((Iy-1) MOD 2)*16384+(Ii-1)\2)
       NyProm2%(A)=Array(Ii)   'Real#(Ii)
'       IF Char$="A" THEN
'       print #4,A;Ii,NyProm2%(A)
'       END IF
      NEXT Ii
'      IF Char$="T" THEN
'        PRINT #4,
'      END IF
     NEXT Iy

     Nchar=Nchar+Nblok
 '    gosub Errprint

'    Gemning af Nchar, (antal) og Char$ i Filen: CHARLIST.DAT
     FOR Ii=1 TO Nblok
       PRINT #2,Nchar-Nblok-1+Ii;
     NEXT Ii
     PRINT #2, TAB(15) Char$

   LOOP
   CLOSE
'  ***********************************************************

'  Program, der gemmer ny prom

    PRINT "Gemmer ny prom"

    OPEN "TXTPROM2.BIN"  AS #1 LEN=2
    NyProm2%(32767)=0   ' Fix af, at der mangler en karakter i at hele
    NyProm2%(32767)=0   ' prommen er fuld (option base 1 !)

    FIELD #1, 2 AS Ha   ' NyProm2% er et HELTALS-ARRAY
    FOR I=1 TO  32765 STEP 2
      LSET Ha=MKI$((NyProm2%(I) MOD 256) + 256*(NyProm2%(I+1) MOD 256) )
      PUT #1,(I-1)\2 +1
    NEXT I
      LSET Ha=MKI$(0)
      PUT #1
    CLOSE #1
    PRINT "SLUT"

   END ' HER SKAL PROGRAMMET SLUTTE, RESTEN ER SUBRUTINE(R).
'  ***********************************************************

 Foldning:
'  Folder de to funktioner, Array og Filter#()
'  Resultatet afleveres i Real# (double precission array)

   Scala=0
   FOR Ii=1 TO Nfolde   'SKALERING TIL 1 GANGS "FORST’RKNING EFTER FOLDNING
     Scala=Scala + Filter#(Ii)
   NEXT Ii
'   PRINT "Scala=";Scala
'  Scala=1

'   FOR I=2*Nblok*8 TO 1 STEP -1  'Flytter data, s† der er plads til foldning
'     Array(I+4)=Array(I)
'   NEXT I

   FOR I=1 TO 2*Nblok*8+Nfolde
     Sum=0
     FOR Ii=1 TO Nfolde
       Sum=Sum + Array((I-Ii))*Filter#(Ii)/Scala
'      PRINT Filter#(Ii);Real#((I-Ii+1024)MOD 1024)
     NEXT Ii
'     PRINT "I,SUM=";I;Sum
     Real#(I)=Sum
     IF Real#(I) > 1 THEN
     PRINT Char$
         GOSUB Pause1
     END IF
   NEXT I

   FOR I=1 TO 2*Nblok*8+Nfolde\2   'Flytter data p† plads igen
     Real#(I)=Real#(I+Nfolde\2)
   NEXT I
   RETURN


'   OPEN "TEXTTST.UD" FOR OUTPUT AS #1
'   FOR I=1 TO 2*Nblok*8
'     WRITE #1,I,Real#(I)
'     PRINT "I,Real#";I,Real#(I)
'   NEXT I
'   CLOSE


'  ***********************************************************

   Errprint:
    PRINT "A,Nchar,Iy,Nblok";A,Nchar,Iy,Nblok
    return
    END

'  ***********************************************************

   Overflow:
   PRINT "FOR MANGE KARAKTER TIL PROM. FJERN NOGLE OG GENSTART PROGRAM"
   END
'  ***********************************************************

  Pause1:
   PRINT "Tryk p† en taste for at forts‘tte"
   WHILE INSTAT = 0
   WEND
   Hind=INKEY$
'   LOCATE CSRLIN-1,POS
   RETURN
