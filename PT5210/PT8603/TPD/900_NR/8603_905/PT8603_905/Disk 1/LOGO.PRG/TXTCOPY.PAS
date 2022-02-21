{ Programmet bruges til at flytte data fra TDKTXTC.BMP til txty.dat,
  txtu.dat og txtv.dat. Samtidigt konverteres fra 1 bit pixel-formatat
  til YUV-formatet, der er brugt i PT8603. Data placeres, s† de direkte
  fylder 512 samples pr. linie. Efter dette program skal data filtreres
  og udtyndes, f›r de kan klippes ind i philips-billedet, der ligger i
  channel1.dat, channel2.dat og channel3.dat.
  971212 NHa.
}
PROGRAM  TXTCOPY;

USES     CRT;

CONST    Bufsize=2160; {Liniel‘ngde i .BMP-fil. 720 bytes, 3 bytes pr. pixel}
         Bufsize2=1024; {Liniel‘ngde i txtx.dat-filer (word)}
         Bufsizel=720;

TYPE     Segtype2=ARRAY [1..Bufsize2] OF WORD;
         Segtype1=ARRAY [1..Bufsize] OF BYTE;

         VAR

         I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Block_gem                          :LONGINT;
         Tvs                                      :^Segtype1;
         P1,P2,P3                                 :^Segtype2;
         F1,F2,F3,F4                              :FILE;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'TXTY.DAT');
             REWRITE (F1,2);
             ASSIGN (F2,'TXTU.DAT');
             REWRITE (F2,2);
             ASSIGN (F3,'TXTV.DAT');
             REWRITE (F3,2);
             ASSIGN (F4,'TDKTXTC.BMP');
             RESET (F4,1);
    END;

{ *********************************************************** }

{ Her hentes 1 block (linie) med data fra logo-filen .BMP}

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F4,(575-Block)*Bufsize+56+1);
          BLOCKREAD(F4, Tvs^, Bufsize, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

PROCEDURE Rgbyuv;

VAR   R,G,B,Y,U,V        : REAL;
      X                  : BYTE;
       ky:char;
    BEGIN
      FOR I:=1 TO 720 DO  {720 samples pr. linie 1 nibble pr. sample}
       BEGIN
         B:=(Tvs^[3*I]);
         IF B = 0 THEN
           BEGIN
            Y:= 700;
            U:=0;
            V:=0;
           END
          ELSE
           BEGIN
            Y:= 59.90;
            U:= 262.5;
            V:= -42.698;
          END;
         P1^[I]:=ROUND(Y*876/700+64);
         P2^[I]:=ROUND(U*448/350+512);
         P3^[I]:=ROUND(V*448/350+512);
        END;
       END;

{ *********************************************************** }

{ Her gemmes 1 block af de 3 CHANNEL#'filer }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F1,Block_gem*Bufsize2 );
          BLOCKWRITE(F1, P1^, Numreadx);
          SEEK (F2,Block_gem*Bufsize2 );
          BLOCKWRITE(F2, P2^, Numreadx);
          SEEK (F3,Block_gem*Bufsize2 );
          BLOCKWRITE(F3, P3^, Numreadx);
    END;

{ *********************************************************** }

    BEGIN    NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (Tvs);

             Fileopen;

             Numreadx:=1024;

             FOR I:=1 TO Numreadx DO
               BEGIN          {Hele linien s‘ttes til hvidt YUV-signal}
                 P1^[I] := 940;
                 P2^[I] := 512;
                 P3^[I] := 512;
               END;

             Block:=138;
             Block_gem:=0;

             REPEAT
               Fileload(Block,Numread);
               Rgbyuv;
               Numreadx:=1024;
               Filesave(Block_gem);
               INC (Block_gem);
               Block:=Block+2;
               IF Block_gem = 33 THEN Block:=Block+1;
               writeln ('BLOCK= ',Block);
             UNTIL (Block_gem = 66);

             writeln ('Slut ');
    END.



