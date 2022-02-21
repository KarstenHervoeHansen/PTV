{ Programmet bruges til at flytte data fra TELEDENA.BMP til logoy.dat,
  logou.dat og logov.dat. Samtidigt konverteres fra 1 bit pixel-formatat
  til YUV-formatet, der er brugt i PT8603. Data placeres, s† de direkte
  fylder 512 samples pr. linie. Efter dette program skal data filtreres
  og udtyndes, f›r de kan klippes ind i philips-billedet, der ligger i
  channel1.dat, channel2.dat og channel3.dat.
  971205 NHa.
}
PROGRAM  TELECOPY;

USES     CRT;

CONST    Bufsize=400;  {Liniel‘ngde i .BMP-fil. 400 bytes, 1 bit pr. pixel}
         Bufsize2=512; {Liniel‘ngde i logox.dat-filer (word)}

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
    BEGIN    ASSIGN (F1,'LOGOY.DAT');
             REWRITE (F1,2);
             ASSIGN (F2,'LOGOU.DAT');
             REWRITE (F2,2);
             ASSIGN (F3,'LOGOV.DAT');
             REWRITE (F3,2);
             ASSIGN (F4,'TELEDENB.BMP');
             RESET (F4,1);
    END;

{ *********************************************************** }

{ Her hentes 1 block (linie) med data fra logo-filen .BMP}

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F4,(1079-Block)*Bufsize+62);
          BLOCKREAD(F4, Tvs^, Bufsize, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

PROCEDURE Rgbyuv;

VAR   R,G,B,Y,U,V,X        : REAL;
      Ii                   :INTEGER;

    BEGIN
      Ii:=0;
      I:=0;
      WHILE I < 168 DO   {124 samples pr. linie ved 27 MHz}
       BEGIN
           X:=Tvs^[7+Ii] AND $80;
           I:=I+1;
         IF X > 0 THEN
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
         P1^[Ii+10]:=ROUND(Y*876/700+64);
         P2^[Ii+10]:=ROUND(U*448/350+512);
         P3^[Ii+10]:=ROUND(V*448/350+512);
         Ii:=Ii+1;
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

             Numreadx:=512;

             FOR I:=1 TO Numreadx DO
               BEGIN          {Hele linien s‘ttes til hvidt YUV-signal}
                 P1^[I] := 940;
                 P2^[I] := 512;
                 P3^[I] := 512;
               END;

             Block:=140;
             Block_gem:=0;
             REPEAT
               Fileload(Block,Numread);
               Rgbyuv;
               Filesave(Block_gem);
               INC (Block_gem);
               Block:=Block+20;
               writeln ('BLOCK= ',Block);
             UNTIL (Block_gem = 44);

             writeln ('Slut ');
    END.



