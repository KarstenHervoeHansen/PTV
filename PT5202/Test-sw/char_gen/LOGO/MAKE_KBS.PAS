{ Programmet bruges til at inds‘tte logo i et Philips-billede til PT 8633.
  Der forventes b†de billede med og uden antpal-felter. Philips-billedet
  ligger i channel1.dat, channel2.dat og channel3.dat. Logo ligger i
  mbc1.bmp. Resultatet ligger i de oprindelige filer (channelx.dat).
  991028 NHa.

PROGRAM  PX33_903;
}
{  Mofified by JK: logo file name is kbs.bmb.  Program name is make_kbs.pas
}

PROGRAM MAKE_KBS;

USES     CRT;

CONST    Bufsize=872;  {Liniel‘ngde i .BMP-fil. 290 samples … 3 bytes}
         Bufsize2=1024; {Liniel‘ngde i channelx.dat-filer (word)}

TYPE     Segtype=ARRAY [1..Bufsize2] OF WORD;
         Segtype1=ARRAY [1..Bufsize] OF BYTE;
         Segtype2=ARRAY [1..Bufsize2] OF REAL;

         VAR

         I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Blockch                            :LONGINT;
         P1,P2,P3,P4,P5,P6                        :^Segtype;
         P7,P8,P9,Key,Tempo                       :^Segtype2;
         Tvs                                      :^Segtype1;
         F1,F2,F3,F4                              :FILE;
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

   FOR I:=1 TO 720 DO
     Temp[I]:=0;

   Scala:=0;
   FOR Ii:=1 TO Nfolde2 DO             {'SKALERING TIL 1 GANGS}
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
             ASSIGN (F4,'KBS.BMP');
             RESET (F4,1);
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
{ Her hentes 1 block (linie) med data fra logo-filen .BMP}

PROCEDURE Logoload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F4,(55-Block)*Bufsize+170+1);
          BLOCKREAD(F4, Tvs^, Bufsize, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

PROCEDURE Add_White(N1,N2,N3:INTEGER);

VAR   I          : INTEGER;

    BEGIN
      FOR I:=N1 TO N2 DO
       BEGIN
         P4^[I]:=N3;
         P5^[I]:=512;
         P6^[I]:=512;
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
     Yr:=150E-9;
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

PROCEDURE Make_Logo_Filter;

VAR   I,C                   : INTEGER;
      Y,Yr,Ys,Ampl,Glampl   : REAL;

BEGIN
     Yr:=150E-9;
     Ys:=13.5E6;
     Glampl :=0;
     C:=TRUNC(1.03734*Yr*Ys);
     FOR I:=1 TO 20 DO
       Filter_logo[I]:=0;

     FOR I:=-C TO C DO
       BEGIN
         Y:=(I)/Yr/Ys/2.07468+0.5;
         Ampl:=(Y-SIN(2*PI*Y)/(2*PI));
         Filter_logo[1+C+I]:=Ampl-Glampl;
         Glampl:=Ampl;
       END;
       Filter_logo[1+C+I+1]:=1-Glampl;
       WRITELN( 1+C+I+1,Filter_logo[1+C+I+1] );
       Nfolde2:=2*(I+1);    { Antal, der skal foldes med}
       Writeln('Nfolde2  ',Nfolde2,'  ',I)
END;

{ *********************************************************** }

PROCEDURE Makelogo(N3:INTEGER);

VAR   I          : INTEGER;
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
          Tempo^[I]:=Tempo^[I]-N3;
        END;

      Logo_fold;

      FOR I:=1 TO 720 DO
        P4^[I]:=ROUND(Tempo^[I]+N3);

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

      FOR I:=1 TO 175 DO  {Max l‘ngde af resulterende logo}
       BEGIN
         Y:=P4^[10+(I-1)*4];
         P7^[290+I]:=Y;

         U:=P5^[10+(I-1)*4];
         V:=P6^[10+(I-1)*4];

         P8^[290+I]:=U;
         P9^[290+I]:=V;
       END;
    END;
{ *********************************************************** }

PROCEDURE Klip_logo(N1,N2,N3:INTEGER);

VAR   Ikl          : INTEGER;

    BEGIN
      FOR Ikl:=N1 TO N2 DO
       BEGIN
         P1^[Ikl]:=P4^[Ikl+N3];
         P2^[Ikl]:=P5^[Ikl+N3];
         P3^[Ikl]:=P6^[Ikl+N3];
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

PROCEDURE Rgbyuv;

VAR   R,G,B,Y,U,V          : REAL;
      Irgb                 : INTEGER;

    BEGIN
      FOR Irgb:=1 TO 720 DO
       BEGIN
         B:=Tvs^[3*(Irgb-1)+1];
         G:=Tvs^[3*(Irgb-1)+2];
         R:=Tvs^[3*(Irgb-1)+3];
         IF R+G+B > 745 THEN {Filter til Hvid}
          BEGIN
           R:=255;
           B:=255;
           G:=255;
          END;
         IF R+G+B < 10 THEN {Filter til sort}
          BEGIN
           R:=0;
           B:=0;
           G:=0;
          END;
{         Writeln(Irgb,'',R,G,B);
             WHILE NOT KEYPRESSED DO DELAY (1);
             IF Irgb mod 20 = 0 then Ch:=READKEY;}
         Y:= (0.299*R + 0.587*G + 0.114*B);
         U:=(B-Y)*0.493 *1.1447 ;{kun SDI;}
         V:=(R-Y)*0.877 *0.8133 ;{kun SDI;}
         P4^[Irgb]:=ROUND(Y*876/255+64);
         P5^[Irgb]:=ROUND(U*448/128+512);
         P6^[Irgb]:=ROUND(V*448/128+512);
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
             NEW (Tempo);
             NEW (Tvs);

             FOR I:=1 TO Bufsize2 DO
              BEGIN
                P4^[I]:=0;
                P5^[I]:=0;
                P6^[I]:=0;
                P7^[I]:=0;
                P8^[I]:=0;
                P9^[I]:=0;
              END;

             Fileopen;

{             Make_Key_Filter;}   {Filter til key-bit}
             Make_Logo_Filter;  {Filter til logo}

             Numreadx:=1024;

               Block:=5167;      {1. Linie med logo 230 start i billede}

             FOR I:=1 TO 36 DO  {36 linier til logo i NTSC}
               BEGIN
               Fileload(Block-1+I,Numread); {Philips-field 1+2}
               Logoload(I+8,Numread);
               Rgbyuv;
{               Add_White(1,249,832);}
{               Add_White(473,720,832);}
               {MakeKeybit;}
               Makelogo(64);     { Udtynder og filtrerer logo }
               {Add_logo;}
{               IF I > 10 THEN}
                 Klip_logo(288,475,-280)
     {           ELSE
                 Klip_logo(280,443);};
               Filesave(Block-1+I);

               writeln ('Linie nr= ',I);
             END;

             writeln ('Slut ');
    END.



