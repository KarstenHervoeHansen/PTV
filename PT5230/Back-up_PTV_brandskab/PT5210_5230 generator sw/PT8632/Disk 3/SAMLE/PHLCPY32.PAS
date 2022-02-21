{ Programmet bruges til PT8632.
  Programmet bruges til flytte data fra PHIL1, PHIL2 og PHIL3 og samtidigt
  inds‘tte EAV og SAV data i signalet. Data flyttes fra center i 503 til
  center i 361. Programmet kan, bortset fra filnavne, bruges til alle
  billeder over 4 fields. Data ender i channel1.dat, channel2.dat og
  channel3.dat. Endvidere flyttes data til 16x9 billedet i 625 liniers
  system til Channel#.DAT. Data flyttes fra filerne phl16x9#.dat.
  NB!
  I f›rste omgang er 16x9 billedet lagt i stedet for billedet uden antipal.
  980115 NHa.
}
PROGRAM  PHLCP32;

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
             ASSIGN (F4,'D:\PT5230\PT8633\PHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8633\PHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8633\PHIL3.DAT');
             RESET (F6,2);
    END;


{ *********************************************************** }

PROCEDURE Fileopen2;
    BEGIN    CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'D:\PT5230\PT8633\PHIL1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8633\PHIL2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8633\PHIL3AUP.DAT');
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

             Fileopen;

             Numreadx:=1024;
             FOR Block:=150 TO 229 DO  {Blanker ikke brugte linier}
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
             Blocknr:=1958;
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
             Blocknr:=1998;
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
             Blocknr:=2038;
             Moffset:=0;
             Linie_dobler:=0;
             REPEAT                {tekstfelt til Philips 4x3 uden antipal}
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

         {625 linier, 4x3 tekstfelt uden dato-felt, med antipal}

             ASSIGN (F7,'D:\PT5230\PT8633\PHIL1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'D:\PT5230\PT8633\PHIL2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'D:\PT5230\PT8633\PHIL3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=2078;
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

             ASSIGN (F7,'D:\PT5230\PT8633\PHIL1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'D:\PT5230\PT8633\PHIL2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'D:\PT5230\PT8633\PHIL3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3AUP.DAT');
             RESET (F6,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=2158;
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

         {625 linier, 4x3 PLUGE-felt UDEN pluge og med antipal}
             ASSIGN (F4,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F6,2);

             Block:=154;
             Block2L:=0;
             Blocknr:=2198;
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
             Blocknr:=2240;
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
             Blocknr:=2282;
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

         {625 linier, 4x3 1o trin grey scale uden antipal}

             ASSIGN (F7,'D:\PT5230\PT8632\TEXT1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'D:\PT5230\PT8632\TEXT2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'D:\PT5230\PT8632\TEXT3.DAT');
             RESET (F9,2);

             ASSIGN (F4,'D:\PT5230\PT8633\PHIL1AUP.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8633\PHIL2AUP.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8633\PHIL3AUP.DAT');
             RESET (F6,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=2324;
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

             ASSIGN (F4,'D:\PT5230\PT8633\PHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'D:\PT5230\PT8633\PHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'D:\PT5230\PT8633\PHIL3.DAT');
             RESET (F6,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=2366;
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



             CLOSE (F1);
             CLOSE (F2);
             CLOSE (F3);
             writeln ('Slut ');
    END.



