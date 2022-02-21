{ Programmet bruges til at inds‘tte Teledanmark tekst i et Philips-billede.
  Der forventes b†de billede med og uden antpal-felter. Philips-billedet
  ligger i channel1.dat, channel2.dat og channel3.dat. Logo ligger i txty.dat,
  txtu.dat og txtv.dat. Resultatet ligger i de oprindelige filer
  (channelx.dat).
  971216 NHa.
}
PROGRAM  TDKLOGO;

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
         P7,P8,P9,Key,Tempo                       :^Segtype2;
         F1,F2,F3,F4,F5,F6                        :FILE;
         Filter,Filter_logo                       : ARRAY[1..32] OF REAL;
         Nfolde,Nfolde2                           :INTEGER;
         Ch                                       : CHAR;

{ *********************************************************** }

PROCEDURE Key_Fold;

VAR   I,Ii          : INTEGER;
      Scala,Sum     : REAL;
      Temp          : ARRAY[1..720] OF REAL;

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

PROCEDURE Logo_fold;

VAR   I,Ii          : INTEGER;
      Scala,Sum     : REAL;
      Temp         : ARRAY[1..720] OF REAL;

BEGIN
{ Foldning:
  Folder de to funktioner, Tempo^[] og Filter_logo[]
  Resultatet afleveres i Tempo^[] efter en tur i Temp[] (real array)}

   Scala:=0;
   FOR Ii:=1 TO Nfolde2 DO                  {'SKALERING TIL 1 GANGS}
          Scala:=Scala + Filter_logo[Ii];   {FORST’RKNING EFTER FOLDNING}

   FOR I:=Nfolde2+1 TO 720 DO
     BEGIN
       Sum:=0;
       FOR Ii:=1 TO Nfolde2 DO
         BEGIN
           Sum:=Sum + Tempo^[(I-Ii)]*Filter_logo[Ii]/Scala;
         END;
       Temp[I]:=Sum;
      END;

   FOR I:=720-Nfolde2 DOWNTO 1 DO   {Flytter data p† plads }
     BEGIN
       Tempo^[I]:=Temp[I+TRUNC(Nfolde2 DIV 2)];
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
             ASSIGN (F4,'TXTY.DAT');
             RESET (F4,2);
             ASSIGN (F5,'TXTU.DAT');
             RESET (F5,2);
             ASSIGN (F6,'TXTV.DAT');
             RESET (F6,2);
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

PROCEDURE Add_Black(N1,N2:INTEGER);

VAR   I          : INTEGER;

    BEGIN
      FOR I:=N1 TO N2 DO
       BEGIN
         P1^[I]:=64;
         P2^[I]:=512;
         P3^[I]:=512;
       END;
    END;

{ *********************************************************** }

PROCEDURE MakeKeybit;

VAR   I,C          : INTEGER;
      Y,U,V        : REAL;

    BEGIN
      C:=0;
      FOR I:=1 TO Bufsize2 DO
        Key^[I]:=0;

      FOR I:=1 TO Bufsize2 DO
        Tempo^[I]:=0;

      FOR I:=1 TO 720 DO    {Logo i Y-kanalen}
        BEGIN
         Y:=P4^[I];
         U:=P5^[I];
         V:=P6^[I];
         Y:=Y-64;
         U:=U-512;
         V:=V-512;
         IF  (Y<>0) OR (U<>0) OR (V<>0) THEN
           BEGIN
             IF C<>3 THEN
               C:=C+1
              ELSE
               Key^[I]:=1
           END
          ELSE
           BEGIN
             IF C<>0 THEN
              BEGIN
               C:=C-1;
               Key^[I-3]:=0;
               Key^[I-4]:=0;
              END;
          END;
        END;

       Key_Fold;

      FOR I:=1 TO 175 DO  {Max l‘ngde af resulterende logo}
       BEGIN
         Tempo^[290+I+1]:=Key^[10+(I-1)*4];
       END;

      FOR I:=1 TO Bufsize2 DO
        Key^[I]:=0;

      FOR I:=1 TO Bufsize2 DO
        Key^[I]:=Tempo^[I];

    END;

{ *********************************************************** }

{         Writeln(I,'',Key^[I]);
             WHILE NOT KEYPRESSED DO DELAY (1);
             IF I mod 20 = 0 then Ch:=READKEY;}

{ *********************************************************** }

PROCEDURE Make_Key_Filter;

VAR   I,C                   : INTEGER;
      Y,Yr,Ys,Ampl,Glampl   : REAL;

BEGIN
     Yr:=125E-9;
     Ys:=54E6;
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
       Writeln('Nfolde',Nfolde);

END;

{ *********************************************************** }

PROCEDURE Make_Logo_Filter;

VAR   I,C                   : INTEGER;
      Y,Yr,Ys,Ampl,Glampl,A   : REAL;
      Ky :CHAR;

BEGIN
     Yr:=200E-9;
     Ys:=27E6;
     Glampl :=0;
     C:=TRUNC(1.03734*Yr*Ys);
     FOR I:=1 TO 32 DO
       Filter_logo[I]:=0;

     A:=-C;
     FOR I:=-C TO C DO
       BEGIN
         Y:=(A)/Yr/Ys/2.07468+0.5;
         Ampl:=(Y-SIN(2*PI*Y)/(2*PI));
         Filter_logo[1+C+I]:=Ampl-Glampl;
         Glampl:=Ampl;
         A:=A+1;
       writeln('I,Ampl',' ',1+C+I,' ',Filter_logo[1+C+I]);
       END;
       Filter_logo[1+C+I+1]:=1-Glampl;
       WRITELN( 1+C+I+1,Filter_logo[1+C+I+1] );
       Nfolde2:=2*(I+1);    { Antal, der skal foldes med}
       Writeln('Nfolde2  ',Nfolde2,'  ',I);
       WHILE NOT KEYPRESSED DO DELAY (1);
             Ky:=READKEY;
END;

{ *********************************************************** }

PROCEDURE Makelogo;

VAR   I,Ii       : WORD;
      Y,U,V      : REAL;

    BEGIN
      FOR I:=1 TO Bufsize2 DO
       BEGIN
         P7^[I]:=0;
         P8^[I]:=0;
         P9^[I]:=0;
         Tempo^[I]:=0;
       END;

      FOR I:=1 TO 720 DO    {Logo i Y-kanalen}
        BEGIN
          Tempo^[I]:=P4^[I];
          Tempo^[I]:=Tempo^[I]-940;
        END;

      Logo_fold;

      FOR I:=1 TO 720 DO
       BEGIN
        Y:=(Tempo^[I])+940;
        Y:=INT(Y+0.1);
        P4^[I]:=ROUND(Y);
       END;

      FOR I:=1 TO 720 DO    {Logo i U-kanalen}
        BEGIN
          Tempo^[I]:=P5^[I];
          Tempo^[I]:=Tempo^[I]-512;
        END;

      Logo_fold;

      FOR I:=1 TO 720 DO
        P5^[I]:=ROUND(Tempo^[I]+512);

      FOR I:=1 TO 720 DO    {Logo i V-kanalen}
        BEGIN
          Tempo^[I]:=P6^[I];
          Tempo^[I]:=Tempo^[I]-512;
        END;


      Logo_fold;

      FOR I:=1 TO 720 DO
        P6^[I]:=ROUND(Tempo^[I]+512);

    END;
{ *********************************************************** }

PROCEDURE Add_logo;

VAR   I,Ii       : word;
      Y,U,V      : REAL;
      Y1,U1,V1   : REAL;

    BEGIN
      I:=45;
      Ii:=1;
      WHILE Ii < 221 DO
       BEGIN
{         Y1:=P1^[I];
         Y:=(Y1-64)*(1-Key^[I])+P7^[I]*Key^[I]+64;

         U1:=P2^[I];
         U:=(U1-512)*(1-Key^[I])+P8^[I]+512;

         V1:=P3^[I];
         V:=(V1-512)*(1-Key^[I])+P9^[I]+512;}

         P1^[Ii+270]:=P4^[I]{ROUND(Y)};
         P2^[Ii+270]:=P5^[I]{ROUND(U)};
         P3^[Ii+270]:=P6^[I]{ROUND(V)};
        I:=45+round(2*Ii);
        INC(Ii);
       END;
       writeln('I er :',I);
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
             NEW (Tempo);

             Fileopen;

             Make_Key_Filter;   {Filter til key-bit}
             Make_Logo_Filter;  {Filter til logo}

             Numreadx:=1024;

             FOR Block:=5 TO 6 DO
               BEGIN      {Sort s‘ttes ind i linie, der ikke bruges }
                 Fileload(Block-1,Numread);
                 Add_Black(24,733);
                 Filesave(Block-1);
               END;

             FOR Block:=187 TO 228 DO  {228}
               BEGIN      {Hvid s‘ttes ind i linier, der ikke er logo i}
                 Fileload(Block-1,Numread);
                 Add_White(290,460);
                 Filesave(Block-1);
                 Fileload(Block-1+576,Numread);
                 Add_White(290,460);
                 Filesave(Block-1+576);
                 Fileload(Block-1+1152,Numread);
                 Add_White(290,460);
                 Filesave(Block-1+1152);
               END;

             FOR Block:=229 TO 270 DO {270}
               BEGIN      {Reflektionspulse fjernes}
                 Fileload(Block-1,Numread);
                 Add_White(272,288);
                 Filesave(Block-1);
                 Fileload(Block-1+576,Numread);
                 Add_White(272,288);
                 Filesave(Block-1+576);
                 Fileload(Block-1+1152,Numread);
                 Add_White(272,288);
                 Filesave(Block-1+1152);
               END;
             Block:=201;      {1. Linie med tekst}
             Numread:=1024;

             FOR I:=1 TO 66 DO  {73 linier til tekst}
               BEGIN
               Fileload(Block-1+I,Numread); {Philips-field 1+2}
               Logoload(I,Numread);
{               MakeKeybit;}
               Makelogo;     { Filtrerer logo }
               Add_logo;
               Filesave(Block-1+I);

               Fileload(Block-1+I+576,Numread); {Philips-field 1+2}
               Add_logo;
               Filesave(Block-1+I+576);

               Fileload(Block-1+I+1152,Numread); {Philips uden antipal}
               Add_logo;
               Filesave(Block-1+I+1152);

               writeln ('Linie nr= ',I);
               END;
             writeln ('Slut ');
    END.



