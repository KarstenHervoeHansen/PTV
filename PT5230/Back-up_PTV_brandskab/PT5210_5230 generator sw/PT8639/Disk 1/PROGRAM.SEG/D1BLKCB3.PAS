{ Programmet bruges til at splitte data til d1black-generator. Ud fra
  channel#.dat, laves en fil med f›rst 8 LSB til uP-prom, og herefter
  2 MSB, 2K data for hver blok. Der kan evt. v‘re flere linier til D1black
  generator. Data gemmes i en fil, der hedder 8639DATA.DAT.
  961010 NHa.
}
PROGRAM  D1BLKCB3;

USES     CRT;

CONST    Bufsize=1024;
         Bufsize2=2048;

TYPE     Segtype=ARRAY [1..Bufsize] OF  WORD;
         Promtype=ARRAY [1..Bufsize2] OF BYTE;

VAR      I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block                                    :LONGINT;
         Segdat1,Segdat2,Segdat3                  :^Segtype;
         P1                                       :^Promtype;
         F1,F2,F3,F4                              :FILE;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'CHANNEL1.DAT');
             RESET (F1,2);
             ASSIGN (F2,'CHANNEL2.DAT');
             RESET (F2,2);
             ASSIGN (F3,'CHANNEL3.DAT');
             RESET (F3,2);
             ASSIGN (F4, '8639DATA.DAT');
             REWRITE (F4,1);
    END;


{ *********************************************************** }

{ Her hentes 1 block med data fra segmentdata-filen }

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F1,Block*Bufsize);
          BLOCKREAD(F1, Segdat1^, Bufsize, Numread);
          SEEK (F2,Block*Bufsize);
          BLOCKREAD(F2, Segdat2^, Bufsize, Numread);
          SEEK (F3,Block*Bufsize);
          BLOCKREAD(F3, Segdat3^, Bufsize, Numread);
          Numreadx:=Numread;
   END;

{ *********************************************************** }

{ Her gemmes 1 block af  Prom'en }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F4,2*Block*Bufsize2);
          BLOCKWRITE(F4, P1^, 4*Numreadx);
    END;


{ *********************************************************** }

PROCEDURE Promsplit;

    VAR       I,Ii                      :INTEGER;

    BEGIN
         Ii:=1;
         FOR I:=1 TO Numreadx DO
          BEGIN

              P1^[Ii]:=(Segdat1^[I] ) AND $FF;
              P1^[Ii+2048]:=(Segdat1^[I] SHR 8) AND $03;
              INC(Ii);

            IF (I MOD 2) <> 0 THEN
              BEGIN
                P1^[Ii]:=(Segdat3^[I] ) AND $FF;
                P1^[Ii+2048]:=(Segdat3^[I]) SHR 8 AND $03;
                INC(Ii);
              END
             ELSE
              BEGIN
                P1^[Ii]:=(Segdat2^[I+1]) AND $FF;
                P1^[Ii+2048]:=(Segdat2^[I+1]) SHR 8 AND $03;
                INC(Ii);
              END;

          END;
          WRITELN (Numreadx)
    END;

{ *********************************************************** }

    BEGIN    NEW (Segdat1);
             NEW (Segdat2);
             NEW (Segdat3);
             NEW (P1);

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



