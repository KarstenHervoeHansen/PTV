{ Programmet bruges til at flytte data fra tdk1.dat, tdk2.dat og tdk3.dat
  til channel1, channel2 og channel3.dat. Formatet i channe# er 512 words
  pr linie. Format i tdk#.dat er 1024 word pr linie (normalt regneformat).
  Bruges til at teste bev‘geligt logo til Tele Danmark.
  971203 NHa.
}
PROGRAM  TESTTDK;

USES     CRT;

CONST    Bufsize=256;  {Liniel‘ngde i resulterende channel# filer}
         Bufsize2=1024; {Liniel‘ngde i TDK#.dat-filer (word)}

TYPE     Segtype2=ARRAY [1..Bufsize2] OF WORD;
         Segtype1=ARRAY [1..Bufsize] OF WORD;

         VAR

         I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block                                    :LONGINT;
         P4,P5,P6                                 :^Segtype2;
         P1,P2,P3                                 :^Segtype1;
         F1,F2,F3,F4,F5,F6                        :FILE;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'TDK1.DAT');
             RESET (F1,2);
             ASSIGN (F2,'TDK2.DAT');
             RESET (F2,2);
             ASSIGN (F3,'TDK3.DAT');
             RESET (F3,2);
             ASSIGN (F4,'CHANNEL1.DAT');
             REWRITE (F4,2);
             ASSIGN (F5,'CHANNEL2.DAT');
             REWRITE (F5,2);
             ASSIGN (F6,'CHANNEL3.DAT');
             REWRITE (F6,2);
    END;

{ *********************************************************** }

{ Her hentes 1 block (linie) med data fra logo-filen .BMP}

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F1,0);
          BLOCKREAD(F1, P4^, Bufsize2, Numread);
          SEEK (F2,0);
          BLOCKREAD(F2, P5^, Bufsize2, Numread);
          SEEK (F3,0);
          BLOCKREAD(F3, P6^, Bufsize2, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

PROCEDURE Rgbyuv;

VAR   I          : INTEGER;
      G          : REAL;

    BEGIN
      FOR I:=1 TO 256 DO
       BEGIN
         P1^[I]:=P4^[I+301];
         P2^[I]:=P5^[I+301];
         P3^[I]:=P6^[I+301];
       END;
      FOR I:=195 TO 247 DO
       BEGIN
         G:=P1^[I];
         G:=(G-64)*(31-Block)/31+64;
         P1^[I]:=ROUND(G);
         G:=P2^[I];
         G:=(G-512)*(31-Block)/31+512;
         P2^[I]:=ROUND(G);
         G:=P3^[I];
         G:=(G-512)*(31-Block)/31+512;
         P3^[I]:=ROUND(G);
       END;
    END;

{ *********************************************************** }

{ Her gemmes 1 block af de 3 CHANNEL#'filer }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F4,Block*Bufsize );
          BLOCKWRITE(F4, P1^, Numreadx);
          SEEK (F5,Block*Bufsize );
          BLOCKWRITE(F5, P2^, Numreadx);
          SEEK (F6,Block*Bufsize );
          BLOCKWRITE(F6, P3^, Numreadx);
    END;

{ *********************************************************** }

    BEGIN    NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (P4);
             NEW (P5);
             NEW (P6);

             Fileopen;

             Numreadx:=1024;

             FOR I:=1 TO Numreadx DO
               BEGIN          {Hele linien s‘ttes til sort YUV-signal}
                 P1^[I] := 64;
                 P2^[I] := 512;
                 P3^[I] := 512;
               END;

            Block:=0;
            Numreadx:=256;

            Fileload(Block,Numread);

             FOR Block:=0 TO 31 DO
              BEGIN
               Rgbyuv;
               Filesave(Block);
               writeln ('BLOCK= ',Block);
              END;
             writeln ('Slut ');
    END.



