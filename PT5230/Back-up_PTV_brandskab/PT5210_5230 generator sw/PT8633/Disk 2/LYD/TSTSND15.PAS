{ Programmet bruges til at genrere en datafil med lyd til D1test-signal
  generatoren. Der genereres 5*(192 samples a 3 bytes) til 4 kanaler. Der
  genereres data til gruppe 1. Z.bittet s‘ttes i 1. sample. Prommen hedder
  TSTSND15.DAT.
  980618 NHa.
}
PROGRAM  TSTSND15;

USES     CRT;

CONST    Bufsize=2304;  {192*3*4 BYTES}
         Bufsize1=4608;

TYPE     Segtype=ARRAY [1..5*Bufsize] OF LONGINT;
         Promtype=ARRAY [1..5*Bufsize1] OF BYTE;
         Checksum=ARRAY [1..24] OF BYTE;

VAR      I,Ii,Ip                                  :WORD;
         P,Cbit                                   :BYTE;
         Numread, Numreadx                        :WORD;
         Block,Ampl,Ampl1,Ampl2,Paritet           :LONGINT;
         Freq1,Freq2,Amplitude                    :REAL;
         AntalSamples                             :INTEGER;
         P1                                       :^Segtype;
         Prom                                     :^Promtype;
         Check_array                              :^Checksum;
         F1                                       :FILE;
         F2                                       :TEXT;

{ *********************************************************** }

PROCEDURE Fileopen;
    BEGIN
             ASSIGN (F1,'TSTSND15.DAT');
             REWRITE (F1,1);
             ASSIGN (F2,'TSTSND15.TXT');
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
{        WRITELN('X8 = ',X8);}
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

        WRITELN('CRC1 = ',Check_array^[24]);

    END;

{ *********************************************************** }

{ Her gemmes 1 Prom'fil }

PROCEDURE Filesave(Block:LONGINT);
    BEGIN
          SEEK (F1,Block);
          BLOCKWRITE(F1, Prom^, 2*Numreadx);
    END;

{ *********************************************************** }

PROCEDURE Makesin;
    BEGIN
      FOR I:=0 TO AntalSamples DO {AntalSamples= SAMPLES}
        BEGIN
          Ampl1:=ROUND(Amplitude*SIN(2*PI*Freq1/48E3*I));
          Ampl2:=ROUND(Amplitude*SIN(2*PI*Freq2/48E3*I));

          FOR Ii:=0 TO 3 DO {4 KANALER}
            BEGIN
              IF (Ii MOD 2 = 0) THEN
                 Ampl:=Ampl1
                ELSE
                 Ampl:=Ampl2;
              P1^[12*I+3*Ii+1]:=(Ampl SHL 3) AND $1FF+((Ii SHL 1) AND $6);
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
    END;

{ *********************************************************** }

PROCEDURE Splitsinus;
    BEGIN
      FOR I:=1 TO Numreadx DO
        BEGIN
          Prom^[I]:=P1^[I] AND $FF;
          Prom^[I+Numreadx]:=(P1^[I] SHR 8) AND $3;
        END;
    END;
{ *********************************************************** }


    VAR     Ilevel:           INTEGER;
            Ifyld:            LONGINT;

    BEGIN    NEW (P1);
             NEW (Prom);
             NEW (Check_array);

             Fileopen;
             WRITELN (F2,'Oversigt over lyddelene til SDI-test signal generator');
             WRITELN (F2,'');

             {Her fyldes en prom p† 4 Mbit (524288 pladser) med $FF}
             FOR I:= 1 TO 16384 DO
               Prom^[I]:=$FF;
             Numreadx:=8192;
             FOR Ifyld := 1 TO 32 DO
               BEGIN
                 Block := (Ifyld-1)*16384;
                 Filesave(Block);
               END;

             Block:=51712;

          {Her starter silence i stereo, med kun et C-bit sat}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$02;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=2304;
             AntalSamples:=192-1;
             Freq1:=1000;  {Hz i kanal 1 & 3.}
             Freq2:=1000;  {Hz i kanal 2 & 4.}
             Amplitude:=0;
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  'Silence stero:         Start:   ',Block,
                       '   Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 1. silence}

          {Her begynder 2. frekvenspakke, silence i MONO}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$04;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=2304;
             AntalSamples:=192-1;
             Freq1:=1000;  {Hz i kanal 1 & 3.}
             Freq2:=1000;  {Hz i kanal 2 & 4.}
             Amplitude:=0;
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  'Silence MONO:          Start:   ',Block,
                       '   Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 2. silence}

          {Her begynder 3. frekvenspakke, silence i DUAL SOUND}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$08;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=2304;
             AntalSamples:=192-1;
             Freq1:=1000;  {Hz i kanal 1 & 3.}
             Freq2:=1000;  {Hz i kanal 2 & 4.}
             Amplitude:=0;
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  'Silence DUAL SUOND:    Start:   ',Block,
                       '   Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 3.pakke silence, dual sound}

       FOR Ilevel:=1 TO 7 DO

         BEGIN
             Block := Ilevel*65536;
             WRITELN(F2,  '');
             CASE Ilevel OF
               1: BEGIN
                    Amplitude:=$7FFFF;   {0dB}
                    WRITELN(F2,  'Frekvenser med amplitude 0 dB');
                   END;
               2: BEGIN
                    Amplitude:=0.354813389*$7FFFF;   {-9 dB}
                    WRITELN(F2,  'Frekvenser med amplitude -9 dB');
                   END;
               3: BEGIN
                    Amplitude:=0.251188643*$7FFFF;   {-12 dB}
                    WRITELN(F2,  'Frekvenser med amplitude -12 dB');
                   END;
               4: BEGIN
                    Amplitude:=0.177827841*$7FFFF;   {-15 dB}
                    WRITELN(F2,  'Frekvenser med amplitude -15 dB');
                   END;
               5: BEGIN
                    Amplitude:=0.158489319*$7FFFF;   {-16 dB}
                    WRITELN(F2,  'Frekvenser med amplitude -16 dB');
                   END;
               6: BEGIN
                    Amplitude:=0.125892541*$7FFFF;   {-18 dB}
                    WRITELN(F2,  'Frekvenser med amplitude -18 dB');
                   END;
               7: BEGIN
                    Amplitude:=0.1*$7FFFF;   {-20 dB}
                    WRITELN(F2,  'Frekvenser med amplitude -20 dB');
                   END;
             END;
             WRITELN(F2,  '');

          {Her begynder 4. frekvenspakke, 800 Hz 0 dB i STEREO}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$02;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=11520;
             AntalSamples:=960-1;
             Freq1:=800;  {Hz i kanal 1 & 3.}
             Freq2:=800;  {Hz i kanal 2 & 4.}
             {Amplitude:=$7FFFF;}
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  '800 Hz STEREO:         Start:   ',Block,
                       '  Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 4. lydpakke 800 Hz}

          {Her begynder 5. frekvenspakke, 1 kHz 0 dB i STEREO}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$02;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=2304;
             AntalSamples:=192-1;
             Freq1:=1000;  {Hz i kanal 1 & 3.}
             Freq2:=1000;  {Hz i kanal 2 & 4.}
             {Amplitude:=$7FFFF;}
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  '1 kHz STEREO:          Start:   ',Block,
                       '  Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 5. lydpakke 1kHz}

          {Her begynder 6. frekvenspakke, 1 kHz 0 dB i MONO}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$04;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=2304;
             AntalSamples:=192-1;
             Freq1:=1000;  {Hz i kanal 1 & 3.}
             Freq2:=1000;  {Hz i kanal 2 & 4.}
             {Amplitude:=$7FFFF;}
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  '1 kHz MONO:            Start:   ',Block,
                       '  Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 6. lydpakke 1kHz}


          {Her begynder 7. frekvenspakke, 1kHz + 400 Hz 0 dB i DUAL SOUND}
             FOR I:=1 TO 24 DO
               Check_array^[I]:=0; {Array nulstilles}

             Check_array^[ 1]:=$01;  {1 bit professional use }
             Check_array^[ 2]:=$08;  {Checksum til D1-black}
             Check_array^[23]:=$00;

             Init_checksum;{Checksum udregnes p† 23 pladser i Check_array}
             Numreadx:=11520;
             AntalSamples:=960-1;
             Freq1:=1000;  {Hz i kanal 1 & 3.}
             Freq2:=400;  {Hz i kanal 2 & 4.}
             {Amplitude:=$7FFFF;}
             Makesin;

             {Nu splittes prom op i f›rst 8 LSB, dern‘st 2 MSB.}
             Splitsinus;
             Filesave(Block);
             WRITELN(F2,  '1 kHz + 400 Hz DUAL S: Start:   ',Block,
                       '  Antal= ',Numreadx);
             Block := Block + 2*Numreadx;
          {Her slutter 7. lydpakke 1kHz + 400 Hz DUAL SOUND}

         END;

             CLOSE (F2);
    END.




