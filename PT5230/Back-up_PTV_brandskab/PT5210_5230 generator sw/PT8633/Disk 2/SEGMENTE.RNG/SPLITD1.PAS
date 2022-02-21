{ Programmet bruges til at splitte data, der er segmenteret med SEGMENT.PAS,
  ud p† 3 stk 8 bit's prommer. Der l‘ses fra filen SEGDAT.DAT, som er et
  resultat fra segmenteringsprogrammet. Data gemmes i 3 nye filer, der hedder
  P1d1tst, P2d1tst og P3d1tst.
  960808 NHa.
}
PROGRAM  SPLITD1;

USES     CRT;

CONST    Bufsize=8000;

TYPE     Segtype=ARRAY [1..Bufsize] OF LONGINT;
         Promtype=ARRAY [1..Bufsize] OF BYTE;

VAR      I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block                                    :LONGINT;
         Segdat                                   :^Segtype;
         P1,P2,P3                                 :^Promtype;
         F1,F2,F3,F4                              :FILE;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'SEGDAT.DAT');
             RESET (F1,4);
             ASSIGN (F2,'P1d1tst.DAT');
             REWRITE (F2,1);
             ASSIGN (F3,'P2d1tst.DAT');
             REWRITE (F3,1);
             ASSIGN (F4, 'P3d1tst.DAT');
             REWRITE (F4,1);
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
    END;


{ *********************************************************** }

PROCEDURE Promsplit;

    VAR       I                                  :INTEGER;

    BEGIN

         FOR I:=1 TO Numreadx DO
          BEGIN
                P1^[I]:=(Segdat^[I] SHR 2) AND $FF;

             IF (I+1) MOD 2 =0 THEN {lige sample, U-data f›rst}

               BEGIN
                 P2^[I]:=((Segdat^[I] AND $03)) +
                         ((Segdat^[I] SHR 8) AND $0C);

                 P3^[I]:=((Segdat^[I] SHR 12) AND $FF);
               END
                ELSE
               BEGIN
                 P2^[I]:=((Segdat^[I] AND $03)) +
                         ((Segdat^[I-1] SHR 18) AND $0C);

                 P3^[I]:=((Segdat^[I-1] SHR 22) AND $FF);
             END;
           END;
    END;

{ *********************************************************** }

    BEGIN    NEW (Segdat);
             NEW (P1);
             NEW (P2);
             NEW (P3);

             Block:=0;
             Fileopen;
             REPEAT
               Fileload(Block,Numreadx);
               Promsplit;
               Filesave(Block);
               INC (Block);
               writeln ('BLOCK= ',Block);
             UNTIL (Numreadx < Bufsize);
    END.



