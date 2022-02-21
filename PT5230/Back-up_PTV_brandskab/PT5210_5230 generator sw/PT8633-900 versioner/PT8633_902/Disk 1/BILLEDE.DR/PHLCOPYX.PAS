{ Programmet bruges til flytte data fra PHIL1, PHIL2 og PHIL3 og samtidigt
  inds‘tte EAV og SAV data i signalet. Data flyttes fra center i 503 til
  center i 361. Programmet kan, bortset fra filnavne, bruges til alle
  billeder over 4 fields. Data ender i channel1.dat, channel2.dat og
  channel3.dat. Endvidere flyttes data til 16x9 billedet i 625 liniers
  system til Channel#.DAT. Data flyttes fra filerne phl16x9#.dat.
  NB!
  I f›rste omgang er 16x9 billedet lagt i stedet for billedet uden antipal.
  980115 NHa.
}
PROGRAM  PHLCOPYX;

USES     CRT;

CONST    Bufsize=16384;
         Bufsize2=1024;

TYPE     Segtype=ARRAY [1..Bufsize] OF WORD;

VAR      I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Blocknr,Block2L                    :LONGINT;
         P1,P2,P3,P4,P5,P6                        :^Segtype;
         F1,F2,F3,F4,F5,F6,F7,F8,F9               :FILE;
         Linie_dobler,Moffset                     :INTEGER;
{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'CHANNEL1.DAT');
             RESET (F1,2);
             ASSIGN (F2,'CHANNEL2.DAT');
             RESET (F2,2);
             ASSIGN (F3,'CHANNEL3.DAT');
             RESET (F3,2);
             ASSIGN (F4,'PHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PHIL3.DAT');
             RESET (F6,2);
    END;


{ *********************************************************** }

PROCEDURE Fileopen2;
    BEGIN    CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'PHIL1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PHIL2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PHIL3AUP.DAT');
             RESET (F6,2);
    END;


{ *********************************************************** }

PROCEDURE Fileopen3;
    BEGIN    CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'FUBK1a.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FUBK2a.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FUBK3a.DAT');
             RESET (F6,2);
    END;


{ *********************************************************** }

{ Her hentes 1 block med data fra segmentdata-filen }

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F4,Block*Bufsize+Moffset);
          BLOCKREAD(F4, P1^, Bufsize, Numread);
          SEEK (F5,Block*Bufsize+Moffset);
          BLOCKREAD(F5, P2^, Bufsize, Numread);
          SEEK (F6,Block*Bufsize+Moffset);
          BLOCKREAD(F6, P3^, Bufsize, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

{ Her hentes 1 block, 2 linier, med data fra billeddata-filen }

PROCEDURE Fileload2L(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F4,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F4, P1^, 2*Bufsize2, Numread);
          SEEK (F5,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F5, P2^, 2*Bufsize2, Numread);
          SEEK (F6,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F6, P3^, 2*Bufsize2, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

{ Her hentes 1 block, 2 linier, med data fra billeddata-filen }

PROCEDURE FileloadGrey(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F7,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F7, P4^, 2*Bufsize2, Numread);
          SEEK (F8,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F8, P5^, 2*Bufsize2, Numread);
          SEEK (F9,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F9, P6^, 2*Bufsize2, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

{ Her gemmes 1024 samples i de 3 CHANNEL#'filer }

PROCEDURE Filesave_blank(Block:LONGINT);
    BEGIN
          SEEK (F1,Block*Bufsize2);
          BLOCKWRITE(F1, P1^, Numreadx);
          SEEK (F2,Block*Bufsize2);
          BLOCKWRITE(F2, P2^, Numreadx);
          SEEK (F3,Block*Bufsize2);
          BLOCKWRITE(F3, P3^, Numreadx);
    END;


{ *********************************************************** }

{ Her gemmes 1 block af de 3 CHANNEL#'filer }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F1,Blocknr*Bufsize2 + Block*Bufsize);
          BLOCKWRITE(F1, P1^, Numreadx);
          SEEK (F2,Blocknr*Bufsize2 + Block*Bufsize);
          BLOCKWRITE(F2, P2^, Numreadx);
          SEEK (F3,Blocknr*Bufsize2 + Block*Bufsize);
          BLOCKWRITE(F3, P3^, Numreadx);
    END;

{ *********************************************************** }

{ Her gemmes 1 block, 2 linier (field 1 & 2), af de 3 CHANNEL#'filer }

PROCEDURE Filesave2L(Lnr:LONGINT);
    BEGIN
          SEEK (F1,Blocknr*Bufsize2 + 2*Lnr*Bufsize2);
          BLOCKWRITE(F1, P1^, Numreadx);
          SEEK (F2,Blocknr*Bufsize2 + 2*Lnr*Bufsize2);
          BLOCKWRITE(F2, P2^, Numreadx);
          SEEK (F3,Blocknr*Bufsize2 + 2*Lnr*Bufsize2);
          BLOCKWRITE(F3, P3^, Numreadx);
    END;

{ *********************************************************** }

PROCEDURE Eavsav;

    VAR       I                                  :INTEGER;

    BEGIN
       FOR I:=1 TO Numreadx-126 DO
         BEGIN
           P1^[I]:=P1^[I+126];
           P2^[I]:=P2^[I+126];
           P3^[I]:=P3^[I+126];
         END;

       FOR I:=Numreadx-125 TO Numreadx DO
         BEGIN
           P1^[I]:=64;
           P2^[I]:=512;
           P3^[I]:=512;
         END;

       FOR I:=0 TO 15 DO
         BEGIN
           P1^[15+1024*I]:=0;
           P1^[737+1024*I]:=0;

           P2^[ 15+1024*I]:=1023;
           P2^[737+1024*I]:=1023;
           P2^[ 16+1024*I]:=1023;
           P2^[738+1024*I]:=1023;

           P3^[ 15+1024*I]:=0;
           P3^[737+1024*I]:=0;
           P3^[ 16+1024*I]:=0;
           P3^[738+1024*I]:=0;
         END;

       FOR I:=0 TO 7 DO
         BEGIN
           P1^[  16+2048*I]:=512;{SAV  Field1}
           P1^[ 738+2048*I]:=628;{EAV  Field1}
           P1^[1040+2048*I]:=796;{SAV  Field2}
           P1^[1762+2048*I]:=872;{EAV  Field2}
         END;

    IF Linie_dobler=1 THEN
     BEGIN
      IF Block = 0 THEN  { Halve linie i linie 23 overskrives med linie 24}
       BEGIN             { Field 1}
         FOR I:=1 TO 1024 DO
          BEGIN
           P1^[I]:=P1^[I+2048];
           P2^[I]:=P2^[I+2048];
           P3^[I]:=P3^[I+2048];
          END;
      END;
     END;

    IF Block = 35 THEN {Halve linie i linie 623 overskrives med linie 622}
     BEGIN             { Field 1}
       WRITELN('Block');
       FOR I:=15360 TO 16384 DO
        BEGIN
         P1^[I]:=P1^[I-2048];
         P2^[I]:=P2^[I-2048];
         P3^[I]:=P3^[I-2048];
        END;
     END;

   IF Linie_dobler = 1 THEN
    BEGIN
     IF Block = 36 THEN { Halve linie i linie 23 overskrives med linie 24}
       BEGIN             { Field 3}
         FOR I:=1 TO 1024 DO
          BEGIN
           P1^[I]:=P1^[I+2048];
           P2^[I]:=P2^[I+2048];
           P3^[I]:=P3^[I+2048];
         END;
     END;
   END;

   IF Linie_dobler = 1 THEN
    BEGIN
      IF Block = 71 THEN {Halve linie i linie 623 overskrives med linie 622}
        BEGIN             { Field 3}
         FOR I:=15360 TO 16384 DO
          BEGIN
           P1^[I]:=P1^[I-2048];
           P2^[I]:=P2^[I-2048];
           P3^[I]:=P3^[I-2048];
          END;
       END;
    END;
   END;
{ *********************************************************** }

PROCEDURE Mdouble;

    VAR       I                                  :INTEGER;

    BEGIN

      { Field 1}
      FOR I:=1 TO 1024 DO
        BEGIN
         P1^[I+5120]:=P1^[I+3072];
         P2^[I+5120]:=P2^[I+3072];
         P3^[I+5120]:=P3^[I+3072];
         P1^[I+4096]:=P1^[I+2048];
         P2^[I+4096]:=P2^[I+2048];
         P3^[I+4096]:=P3^[I+2048];
       END;
      write('Mdouble');
   END;
{ *********************************************************** }

PROCEDURE CopyGrey(N1,N2:INTEGER);

    VAR       I                                 :INTEGER;

    BEGIN

      { Field 1}
      FOR I:=N1 TO N2 DO
        BEGIN
         P1^[I]:=P4^[I];
         P1^[I+1024]:=P4^[I+1024];
         P2^[I]:=P5^[I];
         P2^[I+1024]:=P5^[I+1024];
         P3^[I]:=P6^[I];
         P3^[I+1024]:=P6^[I+1024];
       END;
      write('CopyGrey',N1,N2);
   END;
{ *********************************************************** }

PROCEDURE Composite_YUV; { Fjerner setup fra NTSC composite og scalerer
                           til YUV-format Y=700 mV og U&V=ñ250 mV}

    VAR       I                             :INTEGER;
              Y,U,V                         :REAL;
    BEGIN

      FOR I:=1 TO 16384 DO
        BEGIN
         Y:=P1^[I];
         IF P1^[I] < 115 THEN
           Y:=((Y)-64)*876/827
          ELSE
           Y:=((Y-67)-64)*876/827;
         U:=P2^[I];
         U:=(U-512)*336/317;
         V:=P3^[I];
         V:=(V-512)*336/319;
         P1^[I]:=ROUND(Y+64);
         P2^[I]:=ROUND(U+512);
         P3^[I]:=ROUND(V+512);
       END;
   END;
{ *********************************************************** }

PROCEDURE Mswap; { Bytter om p† 2 linier til 525 liniers signal
                   til blokke, der inds‘ttes i billedet}

    VAR       I                             :INTEGER;

    BEGIN

      FOR I:=2048 DOWNTO 1 DO
        BEGIN
         P1^[I+1024]:=P1^[I];
         P2^[I+1024]:=P2^[I];
         P3^[I+1024]:=P3^[I];
       END;

      FOR I:=1 TO 1024 DO
        BEGIN
         P1^[I]:=P1^[I+2048];
         P2^[I]:=P2^[I+2048];
         P3^[I]:=P3^[I+2048];
       END;
   END;
{ *********************************************************** }

    VAR      N,Ni  :INTEGER;
    LABEL Nyelinier,Nyelinier1;

    BEGIN    NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (P4);
             NEW (P5);
             NEW (P6);
             Moffset:=0;

             FOR I:=1 TO 1024 DO
               BEGIN
                 P1^[I]:=64;
                 P2^[I]:=512;
                 P3^[I]:=512;
               END;
         GOTO Nyelinier;

             Fileopen;


             Numreadx:=1024;
             FOR Block:=204 TO 229 DO  {Blanker ikke brugte linier}
               Filesave_blank(Block);
             Numreadx:=0;

             Block:=0;
             Blocknr:=230;
             Linie_dobler:=1;
             REPEAT                    {Philips 4x3 med antipal}
               Fileload(Block,Numreadx);
               Eavsav;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

           Fileopen2;

             Block:=0;
             Blocknr:=1382;
             REPEAT                    {Philips 4x3 uden antipal}
               Fileload(Block,Numreadx);
               Eavsav;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

           Fileopen3;

             Block:=0;
             Blocknr:=1958;
             REPEAT                    {FuBK 4x3 uden antipal}
               Fileload(Block,Numreadx);
               Eavsav;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'FUBK1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FUBK2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FUBK3.DAT');
             RESET (F6,2);

             Block:=203;
             Block2L:=0;
             Blocknr:=2534;
             Linie_dobler:=0;
             REPEAT                    {Antipal til FuBK 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 40); {40 par af field 1 + 2}

             Block:=491;
             Block2L:=0;
             Blocknr:=2614;
             Linie_dobler:=0;
             REPEAT                    {Antipal til FuBK 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 40); {40 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'FB16X91A.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FB16X92A.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FB16X93A.DAT');
             RESET (F6,2);

             Block:=0;
             Blocknr:=2694;
             Linie_dobler:=1;
             REPEAT                    {FuBK 16x9 uden antipal}
               Fileload(Block,Numreadx);
               Eavsav;
               FOR N:=0 TO 15 DO
                BEGIN
                FOR Ni:=739 TO 910 DO
                 BEGIN
                  P1^[Ni+N*1024]:=64;
                  P2^[Ni+N*1024]:=512;
                  P3^[Ni+N*1024]:=512;
                 END;
               END;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'FBK16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FBK16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FBK16X93.DAT');
             RESET (F6,2);

             Block:=203;
             Block2L:=0;
             Blocknr:=3270;
             Linie_dobler:=0;
             REPEAT                    {Antipal til FuBK 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               FOR N:=0 TO 15 DO
                BEGIN
                FOR Ni:=739 TO 910 DO
                 BEGIN
                  P1^[Ni+N*1024]:=64;
                  P2^[Ni+N*1024]:=512;
                  P3^[Ni+N*1024]:=512;
                 END;
               END;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 40); {40 par af field 1 + 2}

             Block:=491;
             Block2L:=0;
             Blocknr:=3350;
             Linie_dobler:=0;
             REPEAT                    {Antipal til FuBK 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 40); {40 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'PHL16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PHL16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PHL16X93.DAT');
             RESET (F6,2);

             Block:=0;
             Blocknr:=3430;
             Linie_dobler:=1;
             REPEAT                    {Philips 16x9 uden antipal}
               Fileload(Block,Numreadx);
               Eavsav;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'P16X91AP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'P16X92AP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'P16X93AP.DAT');
             RESET (F6,2);

             Block:=101;
             Block2L:=0;
             Blocknr:=4006;
             Linie_dobler:=0;
             REPEAT                 {Antipal til Philips 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 32); {32 par af field 1 + 2}

             Block:=389;
             Block2L:=0;
             Blocknr:=4070;
             Linie_dobler:=0;
             REPEAT                 {Antipal til Philips 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 32); {32 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'P16X91AP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'P16X92AP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'P16X93AP.DAT');
             RESET (F6,2);

             Block:=14;
             Block2L:=0;
             Blocknr:=4134;
             Linie_dobler:=0;
             REPEAT                     {Uden hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2, top}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 72); {72 par af field 1 + 2, top}

             Block:=217;
             Block2L:=0;
             Blocknr:=4278;
             Linie_dobler:=0;
             REPEAT                 {Uden hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2, bund}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 56); {72 par af field 3 + 4}

             Block:=196;
             Block2L:=0;
             Blocknr:=4390;
             Linie_dobler:=0;       {Gr†-trappe 5 trin}
             REPEAT                 {Uden hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2, bund}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 21); {72 par af field 3 + 4}

             ASSIGN (F7,'G16X9TE1.DAT');   {Grey scale 10 step uden hj›rnec}
             RESET (F7,2);
             ASSIGN (F8,'G16X9TE2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'G16X9TE3.DAT');
             RESET (F9,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=4432;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Greyscale til Philips 16X9-billede}
                                        {uden hj›rnecirkler}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(367,637);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

             ASSIGN (F4,'G16X9TE1.DAT');   {Grey scale 10 step med hj›rnec}
             RESET (F4,2);
             ASSIGN (F5,'G16X9TE2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'G16X9TE3.DAT');
             RESET (F6,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=4474;
             Linie_dobler:=0;       {Gr†-trappe 10 trin, 625 linier}
             REPEAT                 {Med hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2, bund}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 21); {21 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'G16X9TE1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'G16X9TE2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'G16X9TE3.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=4516;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                {tekstfelt til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=4556;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                {tekstfelt til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             Block:=422;
             Block2L:=0;
             Blocknr:=4596;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                {tekstfelt til Philips 4x3 med antipal}

               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 20); {2 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3AUP.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=4636;
             Moffset:=0;
             Linie_dobler:=0;
             REPEAT                {tekstfelt til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {20+1 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F7,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'PHIL1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PHIL2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PHIL3AUP.DAT');
             RESET (F6,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=4678;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT              {10 trin Greyscale til Philips 4X3-billede}
                                 {uden antipal}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(308,683);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F7,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'PHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PHIL3.DAT');
             RESET (F6,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=4720;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Greyscale til Philips 4X3-billede}
                                        {med antipal}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(308,683);
               Eavsav;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(308,683);
               Eavsav;
               Filesave2L(Block2L+21);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'M4X31.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M4X32.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M4X33.DAT');
             RESET (F6,2);

             Block:=0;
             Block2L:=0;
             Blocknr:=4804;
             Linie_dobler:=0;
             Moffset:=1024; {Bruges til at synkronisere field 1 og 2, samt
                            generere en ekstra linie til digitalt signal.}
             REPEAT                 {Linie 20 NTSC Philips 4x3}
               Fileload2L(Block,Numreadx);  {field 2 + 1}
{               Composite_YUV; fjerner setup, n†r det ikke er gjort i data}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 1); {1 par af field 1 + 2}

             Block:=0;
             Blocknr:=4806;
             Linie_dobler:=0;
             Moffset:=1024; {Bruges til at synkronisere field 1 og 2, samt
                            generere en ekstra linie til digitalt signal.}
             REPEAT                    {NTSC Philips 4x3}
               Fileload(Block,Numreadx);
{               Composite_YUV; fjerner setup, n†r det ikke er gjort i data}
               Eavsav;
               IF Block=30 THEN Mdouble;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'M16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X93.DAT');
             RESET (F6,2);

             FOR I:=1 TO 1024 DO
               BEGIN
                 P1^[I]:=64;
                 P2^[I]:=512;
                 P2^[I]:=512;
               END;

             Numreadx:=1024;
             Block:=5291;  {Fjerner 1 linie, som ikke bruges i NTSC}
             Filesave_blank(Block);

             Block:=0;
             Block2L:=0;
             Blocknr:=5292;
             Linie_dobler:=0;
             Moffset:=1024; {Bruges til at synkronisere field 1 og 2, samt
                            generere en ekstra linie til digitalt signal.}
             REPEAT                 {Linie 20 NTSC Philips 4x3}
               Fileload2L(Block,Numreadx);  {field 2 + 1}
{               Composite_YUV; fjerner setup, n†r det ikke er gjort i data}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 1); {1 par af field 1 + 2}

             Block:=0;
             Blocknr:=5294;
             Linie_dobler:=0;
             REPEAT                    {NTSC Philips 16X9 }
               Fileload(Block,Numreadx);
{               Composite_YUV; fjerner setup, n†r det ikke er gjort i data}
               Eavsav;
               IF Block=30 THEN Mdouble;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             FOR I:=1 TO 1024 DO
               BEGIN
                 P1^[I]:=64;
                 P2^[I]:=512;
                 P2^[I]:=512;
               END;

             Numreadx:=1024;
             Block:=5779;  {Fjerner 1 linie, som ikke bruges i NTSC}
             Filesave_blank(Block);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'M4X3GRY1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M4X3GRY2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M4x3GRY3.DAT');
             RESET (F6,2);

             Block:=114;
             Block2L:=0;
             Blocknr:=5780;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {Tekstfelt til Philips 4X3-billede}
                               {i 525 linier's version}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 16); {16 par af field 1 + 2}

             Block:=164;
             Block2L:=0;
             Blocknr:=5812;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {10trin Greyscale til Philips 4X3-billede}
                               {i 525 linier's version}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 17); {17 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'M16X9GT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X9GT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X9GT3.DAT');
             RESET (F6,2);

             Block:=114;
             Block2L:=0;
             Blocknr:=5846;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {Tekstfelt til Philips 16X9-billede}
                               {i 525 linier's version}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 16); {16 par af field 1 + 2}

             Block:=17;
             Block2L:=0;
             Blocknr:=5878;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {Uden hj›rnecirkler til Philips 16X9-billede}
                               {i 525 linier's version, top}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 57); {57 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {525 linier, 16x9 uden hj›rnecirkler og 5 trins grey scale}
             ASSIGN (F7,'M16X91.DAT');
             RESET (F7,2);
             ASSIGN (F8,'M16X92.DAT');
             RESET (F8,2);
             ASSIGN (F9,'M16X93.DAT');
             RESET (F9,2);

             ASSIGN (F4,'M16X9GT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X9GT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X9GT3.DAT');
             RESET (F6,2);

             Block:=164;
             Block2L:=0;
             Blocknr:=5992;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {5 trins Greyscale til Philips 16X9-billede}
                               {i 525 linier's version uden hj›rnecirkler}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               CopyGrey(373,632);
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 17); {17 par af field 1 + 2}

             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {525 linier, 16x9 uden hj›rnecirkler og 10 trins grey scale}
             Block:=164;
             Block2L:=0;
             Blocknr:=6026;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {10 trins Greyscale til Philips 16X9-billede}
                               {i 525 linier's version uden hj›rnecirkler}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 17); {17 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {525 linier, 16x9 med hj›rnecirkler og 10 trins grey scale}
             ASSIGN (F4,'M16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X93.DAT');
             RESET (F6,2);

             ASSIGN (F7,'M16X9GT1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'M16X9GT2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'M16X9GT3.DAT');
             RESET (F9,2);

             Block:=164;
             Block2L:=0;
             Blocknr:=6060;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            {10 trins Greyscale til Philips 16X9-billede}
                               {i 525 linier's version med hj›rnecirkler}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               CopyGrey(370,637);
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 17); {17 par af field 1 + 2}

             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {525 linier, 16x9 uden hj›rnecirkler bundcirkler}
             ASSIGN (F4,'M16X9GT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X9GT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X9GT3.DAT');
             RESET (F6,2);

             Block:=181;
             Block2L:=0;
             Blocknr:=6094;
             Linie_dobler:=0;
             Moffset:=0 {1024};
             REPEAT            { 525 linier's version uden hj›rnecirkler}
                               { bundfelt }
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 45); {45 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {625 linier, 4x3 PLUGE-felt UDEN pluge og med antipal}
             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F6,2);

             Block:=154;
             Block2L:=0;
             Blocknr:=6184;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                {PLUGE felt til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=442;
             Block2L:=0;
             Blocknr:=6226;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                {plugefelt til Philips 4x3 med antipal}

               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 21); {21 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {625 linier, 4x3 PLUGE-felt UDEN pluge og UDEN antipal}

             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3AUP.DAT');
             RESET (F6,2);

             Block:=154;
             Block2L:=0;
             Blocknr:=6268;
             Moffset:=0;
             Linie_dobler:=0;
             REPEAT                {plugefelt til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {625 linier, 16x9 PLUGE-felt UDEN pluge}

             ASSIGN (F4,'G16X9TE1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'G16X9TE2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'G16X9TE3.DAT');
             RESET (F6,2);

             Block:=154;
             Block2L:=0;
             Blocknr:=6310;
             Moffset:=0;
             Linie_dobler:=0;
             REPEAT                {plugefelt til Philips 16x9 uden pluge}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {525 linier, 4x3 PLUGE-felt UDEN pluge}

             ASSIGN (F4,'M4X3GRY1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M4X3GRY2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M4X3GRY3.DAT');
             RESET (F6,2);

             Block:=130;
             Block2L:=0;
             Blocknr:=6352;
             Moffset:=0;
             Linie_dobler:=0;
             REPEAT                {plugefelt til Philips 4x3, 525 linier}
                                   {uden pluge}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 17); {17 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {525 linier, 16x9 PLUGE-felt UDEN pluge}

             ASSIGN (F4,'M16X9GT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X9GT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X9GT3.DAT');
             RESET (F6,2);

             Block:=130;
             Block2L:=0;
             Blocknr:=6386;
             Moffset:=0;
             Linie_dobler:=0;
             REPEAT                {plugefelt til Philips 16x9, 525 linier}
                                   {uden pluge}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 17); {17 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {625 linier, 4x3 tekstfelt uden dato-felt, med antipal}

             ASSIGN (F7,'PHIL1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'PHIL2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'PHIL3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=6420;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tekstlinie uden datofelt til}
                                        {Philips 4x3 med antipal}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(284,454);
               Eavsav;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(284,454);
               Eavsav;
               Filesave2L(Block2L+20);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {625 linier, 4x3 tekstfelt uden dato-felt, uden antipal}

             ASSIGN (F7,'PHIL1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'PHIL2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'PHIL3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3AUP.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=6500;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tekstlinie uden datofelt til}
                                        {Philips 4x3 uden antipal}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(284,454);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {625 linier, 16x9 tekstfelt uden dato-felt}

             ASSIGN (F7,'PHL16X91.DAT');
             RESET (F7,2);
             ASSIGN (F8,'PHL16X92.DAT');
             RESET (F8,2);
             ASSIGN (F9,'PHL16X93.DAT');
             RESET (F9,2);

             ASSIGN (F4,'G16X9TE1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'G16X9TE2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'G16X9TE3.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=6540;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tekstlinie uden datofelt til}
                                        {Philips 16x9}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(337,484);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {525 linier, 4x3 tekstfelt uden dato-felt}

             ASSIGN (F7,'M4X31.DAT');
             RESET (F7,2);
             ASSIGN (F8,'M4X32.DAT');
             RESET (F8,2);
             ASSIGN (F9,'M4X33.DAT');
             RESET (F9,2);

             ASSIGN (F4,'M4X3GRY1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M4X3GRY2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M4X3GRY3.DAT');
             RESET (F6,2);

             Block:=114;
             Block2L:=0;
             Blocknr:=6580;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tekstlinie uden datofelt til}
                                        {Philips 4x3}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(291,479);
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 16); {16 par af field 1 + 2}


             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {525 linier, 16x9 tekstfelt uden dato-felt}

             ASSIGN (F7,'M16X91.DAT');
             RESET (F7,2);
             ASSIGN (F8,'M16X92.DAT');
             RESET (F8,2);
             ASSIGN (F9,'M16X93.DAT');
             RESET (F9,2);

             ASSIGN (F4,'M16X9GT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'M16X9GT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'M16X9GT3.DAT');
             RESET (F6,2);

             Block:=114;
             Block2L:=0;
             Blocknr:=6612;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tekstlinie uden datofelt til}
                                        {Philips 16x9}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(342,488);
               Mswap;
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 16); {16 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {625 linier FuBK-billede 4x3 tid og dato-felt}

             ASSIGN (F4,'FUBK1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FUBK2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FUBK3.DAT');
             RESET (F6,2);

             Block:=244;
             Block2L:=0;
             Blocknr:=6644;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tid og datofelt til}
                                        {FuBK 4x3}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 19); {19 par af field 1 + 2}


             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {625 linier FuBK-billede 16x9 tid og dato-felt}

             ASSIGN (F4,'FBK16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FBK16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FBK16X93.DAT');
             RESET (F6,2);

             Block:=244;
             Block2L:=0;
             Blocknr:=6682;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tid og datofelt til}
                                        {FuBK 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 19); {19 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {625 linier FuBK-billede 4x3 tid uden dato-felt}

             ASSIGN (F7,'FUBK1A.DAT');
             RESET (F7,2);
             ASSIGN (F8,'FUBK2A.DAT');
             RESET (F8,2);
             ASSIGN (F9,'FUBK3A.DAT');
             RESET (F9,2);

             ASSIGN (F4,'FUBK1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FUBK2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FUBK3.DAT');
             RESET (F6,2);

             Block:=244;
             Block2L:=0;
             Blocknr:=6720;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tid uden datofelt til}
                                        {FuBK 4x3}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(197,364);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 19); {19 par af field 1 + 2}


             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);

         {625 linier FuBK-billede 16x9 tid uden dato-felt}

             ASSIGN (F7,'FB16X91A.DAT');
             RESET (F7,2);
             ASSIGN (F8,'FB16X92A.DAT');
             RESET (F8,2);
             ASSIGN (F9,'FB16X93A.DAT');
             RESET (F9,2);

             ASSIGN (F4,'FBK16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FBK16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FBK16X93.DAT');
             RESET (F6,2);

             Block:=244;
             Block2L:=0;
             Blocknr:=6758;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Tid uden datofelt til}
                                        {FuBK 16x9}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(240,416);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 19); {19 par af field 1 + 2}

        Nyelinier:
             ASSIGN (F1,'CHANNEL1.DAT');
             RESET (F1,2);
             ASSIGN (F2,'CHANNEL2.DAT');
             RESET (F2,2);
             ASSIGN (F3,'CHANNEL3.DAT');
             RESET (F3,2);
             {Nye linier til nedre tekstfelt til specilt DR klik}

             ASSIGN (F4,'PHILDRK1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PHILDRK2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PHILDRK3.DAT');
             RESET (F6,2);

             Block:=217;
             Block2L:=0;
             Blocknr:=6796;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Nyt nedre tekstflet til}
                                        {DR. Philips 4x3}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

         {42 linier til DR klik, linier til philips 4x3}

             ASSIGN (F7,'PHILDRK1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'PHILDRK2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'PHILDRK3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'D:\PT5230\PT8633\PHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8633\PHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8633\PHIL3.DAT');
             RESET (F6,2);

             Block:=217;
             Block2L:=0;
             Blocknr:=6838;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Nedre tekstfelt til}
                                        {Philips 4x3, field 1 + 2}
               FileloadGrey(Block,Numreadx);
               Fileload2L(Block,Numreadx);
               CopyGrey(340,664);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=217;
             Block2L:=0;
             Blocknr:=6880;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Nedre tekstfelt til}
                                        {Philips 4x3, field 3 + 4}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(340,664);
               Eavsav;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

{             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);}
             CLOSE (F1);
             CLOSE (F2);
             CLOSE (F3);
             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             writeln ('Slut ');
    END.
