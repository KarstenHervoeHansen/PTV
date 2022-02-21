{ Programmet bruges til at inds‘tte hvid baggrund i det nedre tekstfelt
  til Teledanmark logo. Der s‘ttes hvidt ind i billede b†de med og uden
  antipal. Udgangspunkt er billeder, der er brugt til Frankrig (Thomson) og
  Australien (PT8603/904 og /905). Resultatet ligger i de oprindelige filer
  (channelx.dat).
  971208 NHa.
}
PROGRAM  TELEHVID;

USES     CRT;

CONST    Bufsize2=1024; {Liniel‘ngde i channelx.dat-filer (word)}

TYPE     Segtype=ARRAY [1..Bufsize2] OF WORD;
         Segtype2=ARRAY [1..Bufsize2] OF REAL;

         VAR

         I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Blockch                            :LONGINT;
         P1,P2,P3,P4,P5,P6                        :^Segtype;
         P7,P8,P9,Key                             :^Segtype2;
         F1,F2,F3,F4,F5,F6                        :FILE;
         Filter                                   : ARRAY[1..20] of EXTENDED;
         Nfolde                                   :INTEGER;
         Ch                                       : CHAR;

{ *********************************************************** }

PROCEDURE Key_Fold;

VAR   I,Ii          : INTEGER;
      Scala,Sum     : EXTENDED;
      Temp          : ARRAY[1..720] OF EXTENDED;

BEGIN
{ Foldning:
  Folder de to funktioner, Key^[] og Filter[]
  Resultatet afleveres i Key^[] efter en tur i Temp[] (real array)}

   Scala:=0;
   FOR Ii:=1 TO Nfolde DO             {'SKALERING TIL 1 GANGS}
          Scala:=Scala + Filter[Ii];  {FORST’RKNING EFTER FOLDNING}

   FOR I:=Nfolde+1 TO 720 DO
     BEGIN
       Sum:=0;
       FOR Ii:=1 TO Nfolde DO
         BEGIN
           Sum:=Sum + Key^[(I-Ii)]*Filter[Ii]/Scala;
         END;
       Temp[I]:=Sum;
      END;

   FOR I:=720-Nfolde DOWNTO 1 DO   {Flytter data p† plads }
     BEGIN
       Key^[I]:=Temp[I+TRUNC(Nfolde DIV 2)];
     END;
END;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'CHANNEL1.DAT');
             RESET (F1,2);
             ASSIGN (F2,'CHANNEL2.DAT');
             RESET (F2,2);
             ASSIGN (F3,'CHANNEL3.DAT');
             RESET (F3,2);
{             ASSIGN (F4,'LOGOY.DAT');
             RESET (F4,2);
             ASSIGN (F5,'LOGOU.DAT');
             RESET (F5,2);
             ASSIGN (F6,'LOGOV.DAT');
             RESET (F6,2);}
    END;

{ *********************************************************** }

{ Her hentes 1 block (linie) med data fra billed-filerne channel1/2/3.dat}

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F1,Block*Bufsize2);
          BLOCKREAD(F1, P1^, Bufsize2, Numread);
          SEEK (F2,Block*Bufsize2);
          BLOCKREAD(F2, P2^, Bufsize2, Numread);
          SEEK (F3,Block*Bufsize2);
          BLOCKREAD(F3, P3^, Bufsize2, Numread);
          Numreadx:=Numread;
   END;

{ *********************************************************** }

{ Her hentes 1 block (linie) med data fra logo-filerne logoy/u/v.dat}

PROCEDURE Logoload(Block:LONGINT; VAR Numreadx:WORD);

    BEGIN
          SEEK (F4,Block*Bufsize2);
          BLOCKREAD(F4, P4^, Bufsize2, Numread);
          SEEK (F5,Block*Bufsize2);
          BLOCKREAD(F5, P5^, Bufsize2, Numread);
          SEEK (F6,Block*Bufsize2);
          BLOCKREAD(F6, P6^, Bufsize2, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

PROCEDURE Add_White(N1,N2:INTEGER);

VAR   I          : INTEGER;

    BEGIN
      FOR I:=N1 TO N2 DO
       BEGIN
         P1^[I]:=940;
         P2^[I]:=512;
         P3^[I]:=512;
       END;
    END;

{ *********************************************************** }

PROCEDURE MakeKeybit;

VAR   I,C          : INTEGER;

    BEGIN
      C:=0;
      FOR I:=1 TO Bufsize2 DO
       BEGIN
         Key^[I]:=0;
         IF  (P7^[I]<>0) OR (P8^[I]<>0) OR (P9^[I]<>0) THEN
           BEGIN
             IF C=0 THEN
               C:=1
              ELSE
               Key^[I]:=1
           END
          ELSE
           BEGIN
             IF C=1 THEN
              BEGIN
               C:=0;
               Key^[I-1]:=0;
              END;
          END;
       END;
       Key_Fold;
    END;
{         Writeln(I,'',Key^[I]);
             WHILE NOT KEYPRESSED DO DELAY (1);
             IF I mod 20 = 0 then Ch:=READKEY;}

{ *********************************************************** }

PROCEDURE Make_Key_Filter;

VAR   I,C                   : INTEGER;
      Y,Yr,Ys,Ampl,Glampl   : REAL;

BEGIN
     Yr:=175E-9;
     Ys:=13.5E6;
     Glampl :=0;
     C:=TRUNC(1.03734*Yr*Ys);
     FOR I:=1 TO 20 DO
       Filter[I]:=0;

     FOR I:=-C TO C DO
       BEGIN
         Y:=(I)/Yr/Ys/2.07468+0.5;
         Ampl:=(Y-SIN(2*PI*Y)/(2*PI));
         Filter[1+C+I]:=Ampl-Glampl;
         Glampl:=Ampl;
       END;
       Filter[1+C+I+1]:=1-Glampl;
       WRITELN( 1+C+I+1,Filter[1+C+I+1] );
       Nfolde:=2*(I+1);    { Antal, der skal foldes med}
       Writeln('Nfolde',Nfolde)
END;

{ *********************************************************** }

PROCEDURE Makelogo;

VAR   I          : INTEGER;
      Y,U,V      : REAL;

    BEGIN
      FOR I:=1 TO Bufsize2 DO
       BEGIN
         P7^[I]:=0;
         P8^[I]:=0;
         P9^[I]:=0;
       END;

      FOR I:=1 TO 175 DO  {Max l‘ngde af resulterende logo}
       BEGIN
         Y:=P4^[9+(I-1)*4];
         P7^[290+I]:=Y-64;

         U:=P5^[9+(I-1)*4];
         V:=P6^[9+(I-1)*4];

         P8^[290+I]:=U-512;
         P9^[290+I]:=V-512;
       END;
    END;
{ *********************************************************** }

PROCEDURE Add_logo;

VAR   I          : INTEGER;
      Y,U,V      : REAL;
      Y1,U1,V1   : REAL;

    BEGIN
      FOR I:=16 TO Bufsize2-16 DO
       BEGIN
         Y1:=P1^[I];
         Y:=(Y1-64)*(1-Key^[I])+P7^[I]*Key^[I]+64;

         U1:=P2^[I];
         U:=(U1-512)*(1-Key^[I])+P8^[I]{*Key^[I]}+512;

         V1:=P3^[I];
         V:=(V1-512)*(1-Key^[I])+P9^[I]{*Key^[I]}+512;

         P1^[I]:=ROUND(Y);
         P2^[I]:=ROUND(U);
         P3^[I]:=ROUND(V);
       END;
    END;

{ *********************************************************** }

{ Her gemmes 1 block af de 3 CHANNEL#'filer }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F1,Block*Bufsize2 );
          BLOCKWRITE(F1, P1^, Numreadx);
          SEEK (F2,Block*Bufsize2 );
          BLOCKWRITE(F2, P2^, Numreadx);
          SEEK (F3,Block*Bufsize2 );
          BLOCKWRITE(F3, P3^, Numreadx);
    END;

{ *********************************************************** }

    BEGIN    NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (P4);
             NEW (P5);
             NEW (P6);
             NEW (P7);
             NEW (P8);
             NEW (P9);
             NEW (Key);

             Fileopen;

             Make_Key_Filter;   {Filter til key-bit}

             Numreadx:=1024;



             FOR Block:=565 TO 606 DO
               BEGIN      {Hvid s‘ttes ind i linier, der ikke er logo i}
                 Fileload(Block-1,Numread);
                 Add_White(250,502);
                 Filesave(Block-1);
                 Fileload(Block-1+576,Numread);
                 Add_White(250,502);
                 Filesave(Block-1+576);
                 Fileload(Block-1+1152,Numread);
                 Add_White(250,502);
                 Filesave(Block-1+1152);
               writeln ('Linie nr= ',Block);
               END;
             END.  {Midlertidigt stop i programmet TELEHVID}

               Block:=187;      {1. Linie med logo}

             FOR I:=1 TO 70 DO  {71 linier til logo}
               BEGIN
               Fileload(Block-1+I,Numread); {Philips-field 1+2}
               Add_White(290,470);
               Logoload(138+4*I,Numread);
               Makelogo;     { Udtynder og filtrerer logo }
               MakeKeybit;
               Add_logo;
               Filesave(Block-1+I);

               Fileload(Block-1+I+576,Numread); {Philips-field 1+2}
               Add_White(290,470);
               Add_logo;
               Filesave(Block-1+I+576);

               Fileload(Block-1+I+1152,Numread); {Philips uden antipal}
               Add_White(290,470);
               Add_logo;
               Filesave(Block-1+I+1152);

               writeln ('Linie nr= ',I);
               END;}
             writeln ('Slut ');
    END.



