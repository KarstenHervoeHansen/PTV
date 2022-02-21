{ Programmet bruges til at genrere en datafil med silence til D1black-
  generatoren. Der genereres 192 samples a 3 bytes til 4 kanaler. Der
  genereres data til gruppe 1. Z.bittet s‘ttes i 1. sample. Prommen hedder
  D1blcsnd.DAT.
  961106 NHa.
}
PROGRAM  D1SILENC;

USES     CRT;

CONST    Bufsize=2304;  {192*3*4 BYTES}
         Bufsize1=4608;

TYPE     Segtype=ARRAY [1..Bufsize] OF LONGINT;
         Promtype=ARRAY [1..Bufsize1] OF BYTE;
         Checksum=ARRAY [1..24] OF BYTE;

VAR      I,Ii,Ip                                  :WORD;
         P,Cbit                                   :BYTE;
         Numread, Numreadx                        :WORD;
         Block,Ampl,Paritet                       :LONGINT;
         P1                                       :^Segtype;
         Prom                                     :^Promtype;
         Check_array                              :^Checksum;
         F1                                       :FILE;
         F2                                       :TEXT;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN
             ASSIGN (F1,'D1BLCSND.DAT');
             REWRITE (F1,1);
             ASSIGN (F2,'D1BLCSND.TXT');
             REWRITE (F2);
    END;

{ *********************************************************** }

PROCEDURE Init_checksum;
    VAR
        Ic     :INTEGER  ;
        Q      : array[1..8] of byte;
        N, X8  : byte;

    BEGIN

      FillChar(Q,SizeOf(Q),1);

      FOR N := 0 TO 183 DO
        BEGIN
          X8:=((Check_array^[((N MOD 192) DIV 8)+1]
               SHR (N MOD 8)) AND $1) XOR Q[8];
          Q[8] := Q[7];
          Q[7] := Q[6];
          Q[6] := Q[5];
          Q[5] := Q[4] XOR X8;
          Q[4] := Q[3] XOR X8;
          Q[3] := Q[2] XOR X8;
          Q[2] := Q[1];
          Q[1] := X8;
        WRITELN('X8 = ',X8);
        END;

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

        WRITELN('CRC = ',Check_array^[24]);

    END;

{ *********************************************************** }

{ Her gemmes 1 Prom'fil }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F1,Block);
          BLOCKWRITE(F1, Prom^, 2*Numreadx);
    END;

{ *********************************************************** }

    BEGIN    NEW (P1);
             NEW (Prom);
             NEW (Check_array);

             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[1]:=$1;  {1 bit professional use }
             Check_array^[2]:=$0;  {Checksum til D1-black}
             Check_array^[5]:=$0;
             Check_array^[23]:=$00;
             {Checksum skal udregnes p† 23 pladser i Check_array}

             Init_checksum;
             Block:=0;
             Numreadx:=2304;
             Fileopen;
             FOR I:=0 TO 191 DO {192 SAMPLES}
               BEGIN
                 FOR Ii:=0 TO 3 DO {4 KANALER}
                   BEGIN
                     Ampl:=0;

                     P1^[12*I+3*Ii+1]:=(Ampl SHL 3) AND $1FF+((Ii SHL 1)
                                        AND $6);
                     IF I MOD 192 = 0 THEN
                       P1^[12*I+3*Ii+1]:=P1^[12*I+3*Ii+1] +1; {Z-bit }

                     IF P1^[12*I+3*Ii+1] < 256 THEN
                       P1^[12*I+3*Ii+1]:=P1^[12*I+3*Ii+1]+512; {S‘t MSB}


                     P1^[12*I+3*Ii+2]:=(Ampl SHR 6) AND $1FF;
                     IF P1^[12*I+3*Ii+2] < 256 THEN
                       P1^[12*I+3*Ii+2]:=P1^[12*I+3*Ii+2]+512; {S‘t MSB}

                     P1^[12*I+3*Ii+3]:=(Ampl SHR 15) AND $1F;

                     {Her s‘ttes V, U og C-bit (U og V-bit er dog altid 0}
                     Cbit:=(Check_array^[((I MOD 192) DIV 8)+1]
                            SHR (I MOD 8)) AND $1;
                     IF (Cbit > 0) THEN
                       P1^[12*I+3*Ii+3]:=P1^[12*I+3*Ii+3]+128
                      ELSE
                       P1^[12*I+3*Ii+3]:=P1^[12*I+3*Ii+3] AND $37F;


                     {Her skal Paritets-bittet udregnes og, m†ske, s‘ttes}
                     Paritet:=(P1^[12*I+3*Ii+3] AND $FF) SHL 18 +
                              (P1^[12*I+3*Ii+2] AND $1FF) SHL 9 +
                              (P1^[12*I+3*Ii+1] AND $1FF);

                     P:=0;
                     FOR Ip:= 0 TO 25 DO
                       BEGIN
                         IF ((Paritet SHR Ip) AND $1) = 1 THEN
                          P:= NOT (P);
                       END;

                     IF (P > 0) THEN
                       P1^[12*I+3*Ii+3]:=P1^[12*I+3*Ii+3]+256
                      ELSE
                       P1^[12*I+3*Ii+3]:=P1^[12*I+3*Ii+3] AND $2FF;

                     IF P1^[12*I+3*Ii+3] < 256 THEN
                       P1^[12*I+3*Ii+3]:=P1^[12*I+3*Ii+3]+512; {S‘t MSB}

                   END;

                END;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}

             FOR I:=1 TO 2304 DO
               BEGIN
                Prom^[I]:=P1^[I] AND $FF;
                Prom^[I+2304]:=(P1^[I] SHR 8) AND $3;
               END;

             Filesave(Block);
             WRITELN(F2,  'Silence D1 BLACK','   ',Block);
             Block := 2*Numreadx;
             WRITELN(F2,  '1kHz D1 BLACK   ','   ',Block);
             CLOSE (F2);
    END.




