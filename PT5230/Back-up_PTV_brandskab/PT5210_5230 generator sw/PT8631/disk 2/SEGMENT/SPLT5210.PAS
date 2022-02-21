{ Programmet bruges til at splitte data, der er segmenteret med SEGMENTH.PAS,
  ud p† 4 8 bit's prommer. Der l‘ses fra filen SEGDAT.DAT, som er et resultat
  fra segmenteringsprogrammet. Data gemmes i 4 nye filer, der hedder P1, P2,
  P3 og P4. Data bruges til analig colorbar-generator.
  960808 NHa.
}
PROGRAM  SPLT5210;

{USES     CRT;}
USES Crt,DOS,Unit_Sup,Unit_Mem,Unit_Str;

CONST    Bufsize=4000;

TYPE     Segtype=ARRAY [1..Bufsize] OF LONGINT;
         Promtype=ARRAY [1..Bufsize] OF BYTE;

VAR      I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Dummy                              :LONGINT;
         Segdat                                   :^Segtype;
         P1,P2,P3,P4                              :^Promtype;
         F1,F2,F3,F4,F5                           :FILE;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'SEGDAT.DAT');
             RESET (F1,4);
             ASSIGN (F2,'P1.BIN');
             REWRITE (F2,1);
             ASSIGN (F3,'P2.BIN');
             REWRITE (F3,1);
             ASSIGN (F4, 'P3.BIN');
             REWRITE (F4,1);
             ASSIGN (F5, 'P4.BIN');
             REWRITE (F5,1);
    END;


{ *********************************************************** }

{ Her hentes 1 block med data fra segmentdata-filen }

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F1,Block*Bufsize);
          BLOCKREAD(F1, Segdat^, Bufsize, Numread);
          Numreadx:=Numread;
   END;

{ *********************************************************** }

{ Her gemmes 1 block af de 4 Prom'filer }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F2,Block*Bufsize);
          BLOCKWRITE(F2, P1^, Numreadx);
          SEEK (F3,Block*Bufsize);
          BLOCKWRITE(F3, P2^, Numreadx);
          SEEK (F4,Block*Bufsize);
          BLOCKWRITE(F4, P3^, Numreadx);
          SEEK (F5,Block*Bufsize);
          BLOCKWRITE(F5, P4^, Numreadx);
    END;


{ *********************************************************** }

PROCEDURE Promsplit;

    VAR       I                                  :INTEGER;

    BEGIN

         FOR I:=1 TO Numreadx DO
            BEGIN
                P1^[I]:=(Segdat^[I] SHR 4) AND $FF;

                P2^[I]:=((Segdat^[I] AND $0F) SHL 4) +
                        ((Segdat^[I] SHR 28) AND $0F);

                P3^[I]:=((Segdat^[I] SHR 20) AND $03) +
                        ((Segdat^[I] SHR 20) AND $FC);

                P4^[I]:=((Segdat^[I] SHR 12) AND $FF);
           END;
    END;

{ *********************************************************** }

    BEGIN    NEW (Segdat);
             NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (P4);

             Block:=0;
             Fileopen;
             REPEAT
               Fileload(Block,Numreadx);
               Promsplit;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);
             DISPOSE (Segdat);
    END.



