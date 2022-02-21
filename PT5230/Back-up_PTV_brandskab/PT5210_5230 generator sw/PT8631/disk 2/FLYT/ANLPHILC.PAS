{ Programmet bruges til flytte data fra APHIL1, APHIL2 og APHIL3 og samtidigt
  inds‘tte halve linier og sync i signalet. Data flyttes fra center i 503 til
  center i 501. Sync, burst mv stammer fra linie 21 i channel1, 2 ,3.
  Programmet kan, bortset fra filnavne, bruges til alle billeder over
  4 fields. Data ender i channel1.dat, channel2.dat og channel3.dat
  970922 NHa.
  Rettet 980416 til at addere ur/dato-felter og 10 trins gr†trappe, samt at
  fjerne PLUGE-feltet.
  980901 tilf›jet 16x9 billeder + indf›rt goto, s† eksisterende billeder ikke
  overskrives.
}
PROGRAM  ANLPHILC;

USES     CRT;

CONST    Bufsize=16384;
         Bufsize2=1024;

TYPE     Segtype=ARRAY [1..Bufsize] OF WORD;

VAR      I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Blocknr,Block2L                   :LONGINT;
         P1,P2,P3,P4,P5,P6,P7,P8,P9               :^Segtype;
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
             ASSIGN (F4,'APHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'APHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'APHIL3.DAT');
             RESET (F6,2);
    END;


{ *********************************************************** }

PROCEDURE Fileopen2;
    BEGIN    CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'APHIL1A.DAT');
             RESET (F4,2);
             ASSIGN (F5,'APHIL2A.DAT');
             RESET (F5,2);
             ASSIGN (F6,'APHIL3A.DAT');
             RESET (F6,2);
    END;


{ *********************************************************** }

{ Her hentes 1 block med data fra segmentdata-filen }

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F4,Block*Bufsize);
          BLOCKREAD(F4, P1^, Bufsize, Numread);
          SEEK (F5,Block*Bufsize);
          BLOCKREAD(F5, P2^, Bufsize, Numread);
          SEEK (F6,Block*Bufsize);
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
          BLOCKREAD(F7, P7^, 2*Bufsize2, Numread);
          SEEK (F8,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F8, P8^, 2*Bufsize2, Numread);
          SEEK (F9,2*Block*Bufsize2+Moffset);
          BLOCKREAD(F9, P9^, 2*Bufsize2, Numread);
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

{ Her hentes 1024 samples i de 3 CHANNEL#'filer, til sync/burst reference }

PROCEDURE RefFileload(Block:LONGINT);
    BEGIN
          SEEK (F1,Block*Bufsize2);
          BLOCKREAD(F1, P4^, Numreadx);
          SEEK (F2,Block*Bufsize2);
          BLOCKREAD(F2, P5^, Numreadx);
          SEEK (F3,Block*Bufsize2);
          BLOCKREAD(F3, P6^, Numreadx);
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

PROCEDURE Syncref;

    VAR       I,Ii                               :INTEGER;

    BEGIN
    IF Block = 0 THEN  { Halve linie i linie 23 flyttes til sample 865}
     BEGIN             { til 928}
       FOR I:=865 TO 928 DO
        BEGIN
         P1^[I]:=P1^[I-318];
         P2^[I]:=P2^[I-318];
         P3^[I]:=P3^[I-318];
        END;
     END;

    IF Block = 35 THEN {Halve linie i linie 623 forsynes med udlignings-}
     BEGIN             { pulse}
       WRITELN('Block');
       FOR I:=16337 TO 16384 DO
        BEGIN
         P1^[I]:=P4^[I-15360];
         P2^[I]:=P5^[I-15360];
         P3^[I]:=P6^[I-15360];

         P1^[I-2048]:=P4^[I-15360];
         P2^[I-2048]:=P5^[I-15360];
         P3^[I-2048]:=P6^[I-15360];

        END;
       FOR I:=16289 TO 16336 DO
        BEGIN
         P1^[I]:=P1^[I-542];
         P2^[I]:=P2^[I-542];
         P3^[I]:=P3^[I-542];

         P1^[I-2048]:=P1^[I-542-2048];
         P2^[I-2048]:=P2^[I-542-2048];
         P3^[I-2048]:=P3^[I-542-2048];

        END;
     END;

    IF Block = 36 THEN  { Halve linie i linie 23 flyttes til sample 865}
     BEGIN              { til 928 field 3}
       FOR I:=865 TO 928 DO
        BEGIN
         P1^[I]:=P1^[I-318];
         P2^[I]:=P2^[I-318];
         P3^[I]:=P3^[I-318];
        END;
     END;

    IF Block = 71 THEN {Halve linie i linie 623 forsynes med udlignings-}
     BEGIN             { pulse field 4}
       WRITELN('Block');
       FOR I:=16337 TO 16384 DO
        BEGIN
         P1^[I]:=P4^[I-15360];
         P2^[I]:=P5^[I-15360];
         P3^[I]:=P6^[I-15360];

         P1^[I-2048]:=P4^[I-15360];
         P2^[I-2048]:=P5^[I-15360];
         P3^[I-2048]:=P6^[I-15360];

        END;
       FOR I:=16289 TO 16336 DO
        BEGIN
         P1^[I]:=P1^[I-542];
         P2^[I]:=P2^[I-542];
         P3^[I]:=P3^[I-542];

         P1^[I-2048]:=P1^[I-542-2048];
         P2^[I-2048]:=P2^[I-542-2048];
         P3^[I-2048]:=P3^[I-542-2048];

        END;
     END;

       FOR I:=0 TO 15 DO   {Data flyttes 30 til pladser tidligere}
        BEGIN
         FOR Ii:=1 TO 834 DO
           BEGIN
             P1^[Ii+1024*I]:=P1^[Ii+1024*I+34];
             P2^[Ii+1024*I]:=P2^[Ii+1024*I+34];
             P3^[Ii+1024*I]:=P3^[Ii+1024*I+34];
           END;
         FOR Ii:=835 TO 864 DO
           BEGIN
             P1^[Ii+1024*I]:=3072;
             P2^[Ii+1024*I]:=512;
             P3^[Ii+1024*I]:=512;
           END;
        END;

       FOR I:=0 TO 15 DO  {Her adderes sync og burst fra referencelinie}
        BEGIN
         FOR Ii:=1 TO 96 DO
           BEGIN
             P1^[Ii+1024*I]:=P4^[Ii];
             P2^[Ii+1024*I]:=P5^[Ii];
             P3^[Ii+1024*I]:=P6^[Ii];
           END;
         FOR Ii:=833 TO 864 DO
           BEGIN
             P1^[Ii+1024*I]:=P4^[Ii];
             P2^[Ii+1024*I]:=P5^[Ii];
             P3^[Ii+1024*I]:=P6^[Ii];
           END;
        END;
   END;
{ *********************************************************** }

PROCEDURE CopyGrey(N1,N2:INTEGER);

    VAR       I                                 :INTEGER;

    BEGIN

      { Field 1}
      FOR I:=N1 TO N2 DO
        BEGIN
         P1^[I]:=P7^[I];
         P1^[I+1024]:=P7^[I+1024];
         P2^[I]:=P8^[I];
         P2^[I+1024]:=P8^[I+1024];
         P3^[I]:=P9^[I];
         P3^[I+1024]:=P9^[I+1024];
       END;
      write('CopyGrey',N1,N2);
   END;
{ *********************************************************** }
           LABEL Nyelinier,Nyelinier1;

    BEGIN    NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (P4);
             NEW (P5);
             NEW (P6);
             NEW (P7);
             NEW (P8);
             NEW (P9);


             FOR I:=1 TO 1024 DO  {Laver blanking til linier.}
               BEGIN
                 P1^[I]:=3072;
                 P2^[I]:=512;
                 P3^[I]:=512;
               END;

             Fileopen;

             Numreadx:=1024;

             Linie_dobler:=0;
             Moffset:=0;

     GOTO Nyelinier1;
             FOR Block:=96 TO 129 DO
               Filesave_blank(Block);

             FOR Block:=1 TO 7 DO
              BEGIN
               CASE Block OF
               1: Blocknr:=81;{Moduleret rampe, NTSC}
               2: Blocknr:=83;{Multipulse, NTSC}
               3: Blocknr:=67;{Luminance rampe, NTSC}
               4: Blocknr:=93;{250 kHz, NTSC}
               5: Blocknr:=78;{10 trin grey scale, NTSC}
               6: Blocknr:=79;{Line sweep, NTSC}
               7: Blocknr:=88;{Pulse & bar, NTSC}
               END;

               RefFileload(Blocknr);
             FOR I:=94 TO 832 DO  {Laver blanking til linier.}
               BEGIN
                 P4^[I]:=3072;
                 P5^[I]:=512;
                 P6^[I]:=512;
               END;
             FOR I:=871 TO 977 DO  {Laver blanking til linier.}
               BEGIN
                 P4^[I]:=3072;
                 P5^[I]:=512;
                 P6^[I]:=512;
               END;
             FOR I:=1 TO 1024 DO  {Flytter til array, der gemmes i.}
               BEGIN
                 P1^[I]:=P4^[I];
                 P2^[I]:=P5^[I];
                 P3^[I]:=P6^[I];
               END;
               Filesave_blank(Blocknr);
             END;

{end.}
     Nyelinier1:
             Blocknr:=130;

             Block:=21;  {Linienr. til reference}
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             RefFileload(Block);

GOTO Nyelinier;
             Numreadx:=0;
             Block:=0;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             Fileopen2;

             Block:=0;
             Blocknr:=1282;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'MODPHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'MODPHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'MODPHIL3.DAT');
             RESET (F6,2);

             Block:=154;
             Block2L:=0;
             Blocknr:=1858;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {PLUGE-felt Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=196;
             Block2L:=0;
             Blocknr:=1900;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Gr†trappe til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=134;
             Block2L:=0;
             Blocknr:=1942;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur/dato-felt til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {21 par af field 1 + 2}

             ASSIGN (F7,'APHIL1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'APHIL2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'APHIL3.DAT');
             RESET (F9,2);

             Block:=134;
             Block2L:=0;
             Blocknr:=1982;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur-felt til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(280,460);
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {21 par af field 1 + 2}

             Block:=134;
             Block2L:=0;
             Blocknr:=2022;{1982}
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Dato-felt til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(560,764);
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             CLOSE (F7);
             CLOSE (F8);
             CLOSE (F9);
             ASSIGN (F4,'APHIL1.DAT');
             RESET (F4,2);
             ASSIGN (F5,'APHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'APHIL3.DAT');
             RESET (F6,2);
             ASSIGN (F7,'MODPHIL1.DAT');
             RESET (F7,2);
             ASSIGN (F8,'MODPHIL2.DAT');
             RESET (F8,2);
             ASSIGN (F9,'MODPHIL3.DAT');
             RESET (F9,2);

             Block:=154;
             Block2L:=0;
             Blocknr:=2062;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {PLUGE-felt til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(480,528);
               Syncref;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(480,528);
               Syncref;
               Filesave2L(Block2L+21);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 3 + 4}

             Block:=196;
             Block2L:=0;
             Blocknr:=2146;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT          {10 trin gr†trappe til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(322,693);
               Syncref;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(322,693);
               Syncref;
               Filesave2L(Block2L+21);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 3 + 4}

             Block:=134;
             Block2L:=0;
             Blocknr:=2230;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur og dato-felt til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(280,460);
               CopyGrey(560,764);
               Syncref;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(280,460);
               CopyGrey(560,764);
               Syncref;
               Filesave2L(Block2L+20);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 3 + 4}

             Block:=134;
             Block2L:=0;
             Blocknr:=2310;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur-felt til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(560,764);
               Syncref;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(560,764);
               Syncref;
               Filesave2L(Block2L+20);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 3 + 4}


             Block:=134;
             Block2L:=0;
             Blocknr:=2390;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Dato-felt til Philips 4x3 med antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(270,480);
               Syncref;
               Filesave2L(Block2L);
               Fileload2L(Block+288,Numreadx);  {field 3 + 4}
               CopyGrey(280,460);
               Syncref;
               Filesave2L(Block2L+20);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 3 + 4}

               writeln ('BLOCK= ',Block);

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

     Nyelinier:
             ASSIGN (F4,'PH16X9Y.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PH16X9U.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PH16X9V.DAT');
             RESET (F6,2);

             Block:=0;
             Moffset:=0;
             Linie_dobler:=0;
             Blocknr:=2470;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);{576 linier til Philips 16x9 billede}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'PG16X9Y.DAT');
             RESET (F4,2);
             ASSIGN (F5,'PG16X9U.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PG16X9V.DAT');
             RESET (F6,2);

             Block:=101;
             Block2L:=0;
             Blocknr:=3046;
             Linie_dobler:=0;
             REPEAT                 {Antipal til Philips 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 32); {32 par af field 1 + 2}

             Block:=389;{389 til field 3 + 4}
             Block2L:=0;
             Blocknr:=3110;
             Linie_dobler:=0;
             REPEAT                 {Antipal til Philips 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 32); {32 par af field 3 + 4}

             Block:=14;
             Block2L:=0;
             Blocknr:=3174;
             Linie_dobler:=0;           {625 linier, top} 
             REPEAT                     {Uden hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2, top}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 72); {72 par af field 1 + 2, top}

             Block:=217;
             Block2L:=0;
             Blocknr:=3318;
             Linie_dobler:=0;       {field 1 + 2, bund}
             REPEAT                 {Uden hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 56); {72 par af field 3 + 4}

             ASSIGN (F7,'PH16X9Y.DAT');   {Grey scale 10 step uden hj›rnec}
             RESET (F7,2);                 {hentes fra standard 16x9}  
             ASSIGN (F8,'PH16X9U.DAT');
             RESET (F8,2);
             ASSIGN (F9,'PH16X9V.DAT');
             RESET (F9,2);

             Block:=196;
             Block2L:=0;
             Blocknr:=3430;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT              {5 trin Greyscale til Philips 16X9-billede}
                                 {uden hj›rnecirkler}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(367,637);
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=196;
             Block2L:=0;
             Blocknr:=3472;
             Linie_dobler:=0;       {Gr†-trappe 10 trin, 625 linier.}
             REPEAT                 {Uden hj›rnecirkler til Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2, bund}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=196;
             Block2L:=0;
             Blocknr:=3514;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT                     {Greyscale til Philips 16X9-billede}
                                        {med hj›rnecirkler}
               FileloadGrey(Block,Numreadx);  {field 1 + 2}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               CopyGrey(185,261);  {1. cirkel}
               CopyGrey(735,823);  {2. cirkel}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=134;
             Block2L:=0;
             Blocknr:=3556;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur/dato-felt til Philips 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             Block:=134;
             Block2L:=0;
             Blocknr:=3596;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur-felt til Philips 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(340,490);
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             Block:=154;
             Block2L:=0;
             Blocknr:=3636;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT              {Uden PLUGE-felt Philips 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
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
             ASSIGN (F4,'GCIR4X31.DAT');
             RESET (F4,2);
             ASSIGN (F5,'GCIR4X32.DAT');
             RESET (F5,2);
             ASSIGN (F6,'GCIR4X33.DAT');
             RESET (F6,2);

             Block:=0;
             Moffset:=0;
             Linie_dobler:=0;
             Blocknr:=3678;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);{576 linier til cirkel 4x3 billede}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'CIR16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'CIR16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'CIR16X93.DAT');
             RESET (F6,2);

             Block:=0;
             Moffset:=0;
             Linie_dobler:=0;
             Blocknr:=4254;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);{576 linier til cirkel 4x3 billede}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'CIR16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'CIR16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'CIR16X93.DAT');
             RESET (F6,2);

             Block:=0;
             Moffset:=0;
             Linie_dobler:=0;
             Blocknr:=4254;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);{576 linier til cirkel 16x9 billede}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'FUBK4X31.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FUBK4X32.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FUBK4X33.DAT');
             RESET (F6,2);

             Block:=0;
             Moffset:=0;
             Linie_dobler:=0;
             Blocknr:=4830;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);{576 linier til FuBK 4x3 billede}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'AFBK4X31.DAT');
             RESET (F4,2);
             ASSIGN (F5,'AFBK4X32.DAT');
             RESET (F5,2);
             ASSIGN (F6,'AFBK4X33.DAT');
             RESET (F6,2);

             Block:=203;
             Block2L:=0;
             Blocknr:=5406;
             Linie_dobler:=0;
             REPEAT                 {Antipal til FuBK 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 40); {40 par af field 1 + 2}

             Block:=491;{491 til field 3 + 4}
             Block2L:=0;
             Blocknr:=5486;
             Linie_dobler:=0;
             REPEAT                 {Antipal til FuBK 4x3 uden antipal}
               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 40); {40 par af field 3 + 4}

             ASSIGN (F7,'FUBK4X31.DAT');
             RESET (F7,2);
             ASSIGN (F8,'FUBK4X32.DAT');
             RESET (F8,2);
             ASSIGN (F9,'FUBK4X33.DAT');
             RESET (F9,2);

             Block:=244;
             Block2L:=0;
             Blocknr:=5566;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur-felt til FuBK 4x3}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(209,350);  {Fjerner dato-felt}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 19); {19 par af field 1 + 2}

             Block:=244;{491 til field 1 + 2 + 3 + 4}
             Block2L:=0;
             Blocknr:=5604;
             Linie_dobler:=0;
             REPEAT                 {Ur + dato til FuBK 4x3}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 19); {19 par af field 3 + 4}

     {Nyelinier:}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);
             ASSIGN (F4,'FBK16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'FBK16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FBK16X93.DAT');
             RESET (F6,2);

             Block:=0;
             Moffset:=0;
             Linie_dobler:=0;
             Blocknr:=5642;
             REPEAT
               Fileload(Block,Numreadx);
               Syncref;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);{576 linier til FuBK 16x9 billede}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'AFU16X91.DAT');
             RESET (F4,2);
             ASSIGN (F5,'AFU16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'AFU16X93.DAT');
             RESET (F6,2);

             Block:=203;
             Block2L:=0;
             Blocknr:=6218;
             Linie_dobler:=0;
             REPEAT                 {Antipal til FuBK 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 40); {40 par af field 1 + 2}

             Block:=491;{491 til field 3 + 4}
             Block2L:=0;
             Blocknr:=6298;
             Linie_dobler:=0;
             REPEAT                 {Antipal til FuBK 16x9 uden antipal}
               Fileload2L(Block,Numreadx);  {field 3 + 4}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 40); {40 par af field 3 + 4}

             ASSIGN (F7,'FBK16X91.DAT');
             RESET (F7,2);
             ASSIGN (F8,'FBK16X92.DAT');
             RESET (F8,2);
             ASSIGN (F9,'FBK16X93.DAT');
             RESET (F9,2);

             Block:=244;
             Block2L:=0;
             Blocknr:=6378;
             Linie_dobler:=0;
             Moffset:=0;
             REPEAT             {Ur-felt til FuBK 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               FileloadGrey(Block,Numreadx);
               CopyGrey(250,400);  {Fjerner dato-felt}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 19); {19 par af field 1 + 2}

             Block:=244;{491 til field 1 + 2 + 3 + 4}
             Block2L:=0;
             Blocknr:=6416;
             Linie_dobler:=0;
             REPEAT                 {Ur + dato til FuBK 16x9}
               Fileload2L(Block,Numreadx);  {field 1 + 2}
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block);
             UNTIL (Block2L = 19); {19 par af field 3 + 4}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'APHIL1A.DAT');  {Nedre tekstfelt til Philips 4x3}
             RESET (F4,2);               {uden antipal}
             ASSIGN (F5,'APHIL2A.DAT');
             RESET (F5,2);
             ASSIGN (F6,'APHIL3A.DAT');
             RESET (F6,2);

             Block:=217;
             Block2L:=0;
             Blocknr:=6454;
             Linie_dobler:=0;
             REPEAT                 {Nedre tekstfelt til Philips 4x3}
               Fileload2L(Block,Numreadx); {uden antipal, field 1 + 2}
               FOR I:=403 TO 414 DO
                 BEGIN
                   P1^[I]:=P1^[403];
                   P1^[I+1024]:=P1^[403+1024];
                   P2^[I]:=P2^[403];
                   P2^[I+1024]:=P2^[403+1024];
                   P3^[I]:=P3^[403];
                   P3^[I+1024]:=P3^[403+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'APHIL1.DAT');   {Nedre tekstfelt til Philips 4x3}
             RESET (F4,2);               {med antipal}
             ASSIGN (F5,'APHIL2.DAT');
             RESET (F5,2);
             ASSIGN (F6,'APHIL3.DAT');
             RESET (F6,2);

             Block:=217;
             Block2L:=0;
             Blocknr:=6496;
             Linie_dobler:=0;
             REPEAT                 {Nedre tekstfelt til Philips 4x3}
               Fileload2L(Block,Numreadx); {uden antipal, field 1 + 2}
               FOR I:=403 TO 414 DO
                 BEGIN
                   P1^[I]:=P1^[403];
                   P1^[I+1024]:=P1^[403+1024];
                   P2^[I]:=P2^[403];
                   P2^[I+1024]:=P2^[403+1024];
                   P3^[I]:=P3^[403];
                   P3^[I+1024]:=P3^[403+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             Block:=505;
             Block2L:=0;
             Blocknr:=6538;
             Linie_dobler:=0;
             REPEAT                 {Nedre tekstfelt til Philips 4x3}
               Fileload2L(Block,Numreadx); {med antipal, field 3 + 4}
               FOR I:=403 TO 414 DO
                 BEGIN
                   P1^[I]:=P1^[403];
                   P1^[I+1024]:=P1^[403+1024];
                   P2^[I]:=P2^[403];
                   P2^[I+1024]:=P2^[403+1024];
                   P3^[I]:=P3^[403];
                   P3^[I+1024]:=P3^[403+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 21); {21 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'PG16X9Y.DAT');  {Nedre tekstfelt til Philips 16x9}
             RESET (F4,2);               {med hj›rnecirkler}
             ASSIGN (F5,'PG16X9U.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PG16X9V.DAT');
             RESET (F6,2);

             Block:=217;
             Block2L:=0;
             Blocknr:=6580;
             Linie_dobler:=0;
             REPEAT                        {Nedre tekstfelt til Philips 16x9}
               Fileload2L(Block,Numreadx); {med hj›rnecirkler}
               IF Block < 239 THEN
               FOR I:=424 TO 434 DO
                 BEGIN
                   P1^[I]:=P1^[424];
                   P1^[I+1024]:=P1^[424+1024];
                   P2^[I]:=P2^[424];
                   P2^[I+1024]:=P2^[424+1024];
                   P3^[I]:=P3^[424];
                   P3^[I+1024]:=P3^[424+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 56); {56 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'PH16X9Y.DAT');  {Nedre tekstfelt til Philips 16x9}
             RESET (F4,2);               {uden hj›rnecirkler}
             ASSIGN (F5,'PH16X9U.DAT');
             RESET (F5,2);
             ASSIGN (F6,'PH16X9V.DAT');
             RESET (F6,2);

             Block:=217;
             Block2L:=0;
             Blocknr:=6692;
             Linie_dobler:=0;
             REPEAT                        {Nedre tekstfelt til Philips 16x9}
               Fileload2L(Block,Numreadx); {uden hj›rnecirkler}
               IF Block <239 THEN
               FOR I:=424 TO 434 DO
                 BEGIN
                   P1^[I]:=P1^[424];
                   P1^[I+1024]:=P1^[424+1024];
                   P2^[I]:=P2^[424];
                   P2^[I+1024]:=P2^[424+1024];
                   P3^[I]:=P3^[424];
                   P3^[I+1024]:=P3^[424+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 56); {56 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'FUBK4X31.DAT');  {Tekstfelt til FuBK 4x3}
             RESET (F4,2);
             ASSIGN (F5,'FUBK4X32.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FUBK4X33.DAT');
             RESET (F6,2);

             Block:=145;
             Block2L:=0;
             Blocknr:=6804;
             Linie_dobler:=0;
             REPEAT                        {Tekstfelt til FuBK 4x3}
               Fileload2L(Block,Numreadx);
               FOR I:=494 TO 506 DO
                 BEGIN
                   P1^[I]:=P1^[494];
                   P1^[I+1024]:=P1^[494+1024];
                   P2^[I]:=P2^[494];
                   P2^[I+1024]:=P2^[494+1024];
                   P3^[I]:=P3^[494];
                   P3^[I+1024]:=P3^[494+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             CLOSE (F4);
             CLOSE (F5);
             CLOSE (F6);

             ASSIGN (F4,'FBK16X91.DAT');  {Tekstfelt til FuBK 16x9}
             RESET (F4,2);
             ASSIGN (F5,'FBK16X92.DAT');
             RESET (F5,2);
             ASSIGN (F6,'FBK16X93.DAT');
             RESET (F6,2);

             Block:=145;
             Block2L:=0;
             Blocknr:=6844;
             Linie_dobler:=0;
             REPEAT                        {Tekstfelt til FuBK 16x9}
               Fileload2L(Block,Numreadx);
               FOR I:=494 TO 506 DO
                 BEGIN
                   P1^[I]:=P1^[494];
                   P1^[I+1024]:=P1^[494+1024];
                   P2^[I]:=P2^[494];
                   P2^[I+1024]:=P2^[494+1024];
                   P3^[I]:=P3^[494];
                   P3^[I+1024]:=P3^[494+1024];
                 END;
               Syncref;
               Filesave2L(Block2L);
               INC (Block);
               INC (Block2L);
               writeln ('BLOCK= ',Block,' ',Block2L,' ',Numreadx);
             UNTIL (Block2L = 20); {20 par af field 1 + 2}

             writeln ('Slut ');
    END.



