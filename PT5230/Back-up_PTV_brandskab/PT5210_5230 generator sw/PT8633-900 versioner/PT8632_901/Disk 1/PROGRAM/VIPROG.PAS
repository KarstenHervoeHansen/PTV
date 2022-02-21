{Programmt brugest til at generere 8 linie til video index.
 Der l‘ses fra linie 1 og to i CHANNEL#.DAT filerne for at f†
 de rigtige EAV og SAV sekvenser. Herefter skrives video index
 i de rigtige chromasamples og linien gemmes p† linie 223 og frem 
 til 230. (Linie 231 er 1. linie i billeddata og testlinier ligger 
 fra 1 til ca 200 idag).
 981211
}
PROGRAM  VIPROG;

USES     CRT;

CONST    Bufsize=1024;

TYPE     Segtype=ARRAY [1..Bufsize] OF WORD;
         Checksum=ARRAY [1..24] OF BYTE;

VAR      I,Antal, Numread1, Numread2, Numread3    :WORD;
         Name                                     :STRING;
         Numread, Numreadx                        :WORD;
         Block,Blocknr,Block2L                    :LONGINT;
         P1,P2,P3                                 :^Segtype;
         F1,F2,F3                                 :FILE;
         Check_array                              :^Checksum;
         Nindex,Firstsample                       :INTEGER;
         Bit                                      :Byte;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN    ASSIGN (F1,'CHANNEL1.DAT');
             RESET (F1,2);
             ASSIGN (F2,'CHANNEL2.DAT');
             RESET (F2,2);
             ASSIGN (F3,'CHANNEL3.DAT');
             RESET (F3,2);
    END;


{ *********************************************************** }

{ Her hentes 1 linie med data fra segmentdata-filen }

PROCEDURE Fileload(Block:LONGINT; VAR Numreadx:WORD);

VAR       I                :INTEGER;

    BEGIN
          SEEK (F1,Block*Bufsize);
          BLOCKREAD(F1, P1^, Bufsize, Numread);
          SEEK (F2,Block*Bufsize);
          BLOCKREAD(F2, P2^, Bufsize, Numread);
          SEEK (F3,Block*Bufsize);
          BLOCKREAD(F3, P3^, Bufsize, Numread);
          Numreadx:=Numread;
   END;
{ *********************************************************** }

{ Her gemmes 1024 samples i de 3 CHANNEL#'filer }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F1,Block*Bufsize);
          BLOCKWRITE(F1, P1^, Numreadx);
          SEEK (F2,Block*Bufsize);
          BLOCKWRITE(F2, P2^, Numreadx);
          SEEK (F3,Block*Bufsize);
          BLOCKWRITE(F3, P3^, Numreadx);
    END;

{ *********************************************************** }

PROCEDURE Init_checksum;
    VAR
        Ic     :INTEGER  ;
        Q      : array[1..8] of byte;
        N, X8  : byte;

    BEGIN

      FillChar(Q,SizeOf(Q),1);

      FOR N := 0 TO 8*Nindex-1 DO
        BEGIN
          X8:=((Check_array^[((N MOD (8*Nindex)) DIV 8)+1]
               SHR (N MOD 8)) AND $1) XOR Q[8];
          Q[8] := Q[7];
          Q[7] := Q[6];
          Q[6] := Q[5];
          Q[5] := Q[4] XOR X8;
          Q[4] := Q[3] XOR X8;
          Q[3] := Q[2] XOR X8;
          Q[2] := Q[1];
          Q[1] := X8;
{        WRITELN('X8 = ',X8);}
        END;

        Check_array^[24]:=0;
        IF Q[1] = 1 THEN
         Check_array^[24]:= Check_array^[24] + 128;
        IF Q[2] = 1 THEN
         Check_array^[24] := Check_array^[24] +64;
        IF Q[3] = 1 THEN
         Check_array^[24] := Check_array^[24] +32;
        IF Q[4] = 1 THEN
         Check_array^[24] := Check_array^[24] +16;
        IF Q[5] = 1 THEN
         Check_array^[24] := Check_array^[24] +8;
        IF Q[6] = 1 THEN
         Check_array^[24] := Check_array^[24] +4;
        IF Q[7] = 1 THEN
         Check_array^[24] := Check_array^[24] +2;
        IF Q[8] = 1 THEN
         Check_array^[24] := Check_array^[24] +1;

        WRITELN('CRC1 = ',Check_array^[24]);

    END;
{ *********************************************************** }
PROCEDURE Promdata; {Gemmer data til video index i chromaprommer}
    BEGIN
        I:=0;
         WHILE I < 8*(Nindex+1) DO
         BEGIN
          Bit:=(Check_array^[(I DIV 8)+1] SHR (I MOD 8)) AND $1;
          write (' ',Bit);
          P2^[Firstsample + I] := 512 + 4*Bit;
          I:=I+1;
          Bit:=(Check_array^[((I-1) DIV 8)+1] SHR (I MOD 8)) AND $1;
          P3^[Firstsample + (I-1)] := 512 + 4*Bit;
          I:=I+1;
          write (' ',Bit);
         END;
    END;
{ *********************************************************** }

  {Program:}
    VAR     Ilevel:           INTEGER;
            Ifyld:            LONGINT;

    BEGIN    NEW (P1);
             NEW (P2);
             NEW (P3);
             NEW (Check_array);

             Fileopen;

             Numreadx:=0;

{4:3 625 linier}
{Field 1}    Block:=1;  {CLASS 1.1}
             Fileload(Block,Numreadx);

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=10;
             Check_array^[2]:=2;
             Check_array^[3]:=2;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
{             Write (Check_array^[4]);}
             Firstsample:=17;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=49;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=81;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.1}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=113;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=145;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=177;
             Promdata;

             Block:=222; {1. linie field 1}
             Filesave(Block);

{Field 2}    Block:=0;  {CLASS 1.1}
             Fileload(Block,Numreadx);

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=10;
             Check_array^[2]:=2;
             Check_array^[3]:=2;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=17;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=49;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=81;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.1}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=113;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=145;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=177;
             Promdata;

             Block:=223;
             Filesave(Block);

{16:9 625 linier}
{Field 1}    Block:=1;  {CLASS 1.1}
             Fileload(Block,Numreadx);

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=22;
             Check_array^[2]:=2;
             Check_array^[3]:=2;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=17;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=49;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=81;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.1}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=113;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=145;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=177;
             Promdata;

             Block:=224; {1. linie field 1}
             Filesave(Block);

{Field 2}    Block:=0;  {CLASS 1.1}
             Fileload(Block,Numreadx);

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=22;
             Check_array^[2]:=2;
             Check_array^[3]:=2;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=17;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=49;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 1.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=81;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.1}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=113;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.2}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=145;
             Promdata;

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}
             Check_array^[1]:=0; {CLASS 2.3}
             Check_array^[2]:=0;
             Check_array^[3]:=0;
             Nindex:=3;
             Init_checksum;
             Check_array^[4]:=Check_array^[24];;
             Firstsample:=177;
             Promdata;
             writeln (Check_array^[4]);

             Block:=225;
             Filesave(Block);

            END.
