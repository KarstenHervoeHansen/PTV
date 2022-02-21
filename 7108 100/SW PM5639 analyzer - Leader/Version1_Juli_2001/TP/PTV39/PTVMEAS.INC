(*
Include fil til brug ved m†linger med color sensor i PTV mode
*)
FUNCTION ReadColorSensor : STRING;
VAR Sv : STRING;
BEGIN
 COM1Status := ReadlnCOMPort(_COM1,Sv,2);
 IF COM1Status = 0 THEN
  ReadColorSensor := Sv
 ELSE
  ReadColorSensor := '0,0,0,0';
END;

PROCEDURE GetRGBChannel
            (VAR R,G,B,FR : REAL; VAR Overflow : BOOLEAN; InStr : STRING);
VAR   p     : BYTE;
      Kode  : INTEGER;
      s     : STR_10;
BEGIN
  Overflow := InStr[1] = '-';

  p := Pos(',',InStr);
  IF p > 0 THEN
   s := Copy(InStr,1,p-1);
  Val(s,R,Kode);
  Delete(InStr,1,p);

  p := Pos(',',InStr);
  IF p > 0 THEN
   s := Copy(InStr,1,p-1);
  Val(s,G,Kode);
  Delete(InStr,1,p);

  p := Pos(',',InStr);
  IF p > 0 THEN
   s := Copy(InStr,1,p-1);
  Val(s,B,Kode);
  Delete(InStr,1,p);

  Val(InStr,FR,Kode);
END;


PROCEDURE PTVxyMeasure;
(*
K-matrix indl‘ses kun i starten af PTVxyMeasure-proceduren.
Hvis der skiftes m†lehoved, skal proceduren forlades og kaldes igen.
Proceduren beregner x,y,u',v',Y,CIELUV.
Hvis Y er < 0.1 cd/mý, blankes felterne for m†leresultaterne.
*)
VAR KM          : ARRAY[1..9] OF REAL;
    X,Y,Z,                     {X,Y,Z integralet}
    XYZSum,
    uvDiv,
    fL,
    FR          : REAL;            {fieldrate in mS}
    CieLuv      : INTEGER;
    uKoor,
    vKoor,
    xKoor,
    yKoor,
    zKoor       : REAL;
    Svar        : STRING;
    KUNo        : STR_10;


FUNCTION MakeKmatrix : BOOLEAN;
VAR  Flag : BOOLEAN;
BEGIN
{$R-}
 KM[1] := (KorrMatrix[1] * 256 + KorrMatrix[2]) / KorrMulFactor;
 KM[2] := (KorrMatrix[3] * 256 + KorrMatrix[4]) / KorrMulFactor;
 KM[3] := (KorrMatrix[5] * 256 + KorrMatrix[6]) / KorrMulFactor;
 KM[4] := (KorrMatrix[7] * 256 + KorrMatrix[8]) / KorrMulFactor;
 KM[5] := (KorrMatrix[9] * 256 + KorrMatrix[10]) / KorrMulFactor;
 KM[6] := (KorrMatrix[11] * 256 + KorrMatrix[12]) / KorrMulFactor;
 KM[7] := (KorrMatrix[13] * 256 + KorrMatrix[14]) / KorrMulFactor;
 KM[8] := (KorrMatrix[15] * 256 + KorrMatrix[16]) / KorrMulFactor;
 KM[9] := (KorrMatrix[17] * 256 + KorrMatrix[18]) / KorrMulFactor;
 Flag := TRUE;
 IF KM[1] < 1E-4 THEN
  Flag := FALSE;
 MakeKmatrix := Flag;
{$R+}
END;

PROCEDURE CalXYZ (MStr : STR_30);
VAR R,G,B,FR : REAL;
    OFlow    : BOOLEAN;
BEGIN
 GetRGBChannel(R,G,B,FR,OFlow,MStr);
 X := R*KM[1] + G*KM[2] + B*KM[3];
 Y := R*KM[4] + G*KM[5] + B*KM[6];
 Z := R*KM[7] + G*KM[8] + B*KM[9];
END;

FUNCTION CalCieLuv(uKoor,vKoor,uRef,vRef : REAL) : INTEGER;
{Lininger for CIELUV-beregninger findes i Television Engineering Handbook,
 McGraw Hill 1986 side 2.38-2.40 (ISBN 0-07-004779-0) }
VAR CL, LSt, uSt, vSt : REAL;
BEGIN
Lst := 100;          {Lst := 116 * (Exp(ln(Y/Yref))/3) - 16 }
                     {color diff without luminance (Y/Yref = 1)}
uSt := 13*Lst*(uKoor-uRef);
vSt := 13*Lst*(vKoor-vRef);
CL := Sqrt({Sqr(Lst)} + Sqr(ust) + Sqr(vst));
CalCieLuv := Round(CL);
END;


BEGIN
 ClrScr;

 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'I?,');
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 KUNo := Copy(Svar,24,6);
 IF Length(KUNo) < 6 THEN
  BEGIN
   Writeln(Bell,'Ingen color sensor !');
   Exit;
  END;

 IF Copy(Svar,1,23) <> PM5639IdStr THEN
  BEGIN
   Writeln(Bell,'Ikke PTV color sensor >> ',Copy(Svar,1,20));
   Writeln('Korrekt color sensor  >> ',Copy(PM5639IdStr,1,20));
   Exit;
  END;

 ReadKMatrixFromEEPROM(Silent);
 IF (NOT MakeKmatrix) THEN
  BEGIN
   Writeln(Bell,'K-matrix ikke indl‘st !');
   Exit;
  END;

 CursorOff;
 GotoXY(25,2); Write('M†ling med color sensor KU',KUNo);
 GotoXY(2,21); Write('Afslut: tryk en tast');
 GotoXY(25,10); Write('x= ');
 GotoXY(25,11); Write('y= ');
 GotoXY(45,10); Write('u',#39,'= ');
 GotoXY(45,11); Write('v',#39,'= ');
 GotoXY(25,12); Write('Y=        NIT'); {Write('           fL');}
 GotoXY(25,13); Write('CIELUV=       (D6500)');
 COM1Status := WriteCOMPort(_COM1,'MX,MC,');
 COM1Status := ClearInputQue(_COM1);
 REPEAT
  REPEAT
   CalXYZ(ReadColorSensor);
   XYZSum := X + Y + Z;
    IF Y > 0.1 THEN
     BEGIN
      xKoor := X / XYZSum;
      yKoor := Y / XYZSum;
      uvDiv := -2*xKoor + 12*yKoor + 3;
      uKoor := 4*xKoor / uvDiv;
      vKoor := 9*yKoor / uvDiv;
      CieLuv := CalCieLuv(uKoor,vKoor,D6500Ref[1],D6500Ref[2]);
     { fL := Y / NIT_fL_Factor;}
      GotoXY(28,10);Write(xKoor:6:3);
      GotoXY(28,11);Write(yKoor:6:3);
      GotoXY(49,10);Write(uKoor:6:3);
      GotoXY(49,11);Write(vKoor:6:3);
      GotoXY(27,12);Write(Y:6:1);
      GotoXY(33,13); Write(CieLuv:4);
     { GotoXY(42,12);Write(fL:6:1);}
     END
    ELSE
     BEGIN
      GotoXY(28,10);Write('      ');
      GotoXY(28,11);Write('      ');
      GotoXY(49,10);Write('      ');
      GotoXY(49,11);Write('      ');
      GotoXY(27,12);Write('      ');
      GotoXY(33,13);Write('     ');
     { GotoXY(42,12);Write('    ');}
     END;
  UNTIL (COM1Status <> 0) OR Keypressed;
 COM1Status := WriteCOMPort(_COM1,'MX,MC,');
 UNTIL Keypressed;

 ClrScr;
 CursorOn;
 COM1Status := WriteCOMPort(_COM1,'MS,');
END; {PTVxyMeasure}



(*


PROCEDURE OffsetMeasure;
{
Offset-v‘rdier fra EEPROM indl‘ses kun i starten af OffsetMeasure-proceduren.
}
VAR RVal,
    GVal,
    BVal,
    FRate     : REAL;
    Svar      : STRING;
    KUNo      : STR_10;
    RangeNo   : BYTE;
    Overflow,
    StopFlag  : BOOLEAN;


PROCEDURE SetFixRange (R : BYTE);
VAR RStr : STR_10;
BEGIN
 RStr := 'FG' + IntToStr(R) + ',';
 COM1Status := WriteCOMPort(_COM1,RStr);
 GotoXY(16,10); Write(IntToStr(R));
 GotoXY(19,13); Write(Offset[R,0]:10:1,Offset[R,1]:10:1,Offset[R,2]:10:1);
END;


PROCEDURE ReadPressedKey;
BEGIN
 CASE Scancode OF
  Tast0  : RangeNo := 0;
  Tast1  : RangeNo := 1;
  Tast2  : RangeNo := 2;
  Tast3  : RangeNo := 3;
  Tast4  : RangeNo := 4;
  EndKey : StopFlag := TRUE;
 ELSE
  Beep(800,200);
 END;
 SetFixRange(RangeNo);
 EmptyKeyboardBuffer;
END;


BEGIN
 ClrScr;

 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'I?,');
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 KUNo := Copy(Svar,24,6);
 IF Length(KUNo) < 6 THEN
  BEGIN
   Writeln(Bell,'Ingen color sensor !');
   Exit;
  END;
 EmptyKeyboardBuffer;
 ShowOffsetValues(Silent);
 CursorOff;
 StopFlag := FALSE;
 RangeNo := 4;
 GotoXY(25,2); Write('ST m†ling med color sensor KU',KUNo);
 GotoXY(26,8);  Write('R›d       Gr›n      Bl†              Field rate');
 GotoXY(52,10); Write('mV');
 GotoXY(71,10); Write('mS');
 GotoXY(52,13); Write('mV');
 GotoXY(65,13); Write('25.0  mS');
 GotoXY(4,13); Write('EýPROM offset:');
 GotoXY(2,21); Write('Afslut: tryk End');
 GotoXY(30,21); Write('Gain 0 -> 4: tryk 0 -> 4');
 GotoXY(11,10); Write('Gain  :');
 COM1Status := WriteCOMPort(_COM1,'ST,MC,');
 SetFixRange(RangeNo);
 COM1Status := ClearInputQue(_COM1);
 REPEAT
  REPEAT
   GetRGBChannel(RVal,GVal,BVal,FRate,Overflow,ReadColorSensor);
   RVal :=
    (RVal * 1E3 * ADRef / ADCounts) - (Offset[RangeNo,0] * Abs(FRate) / 25); {mV}
   GVal :=
    (GVal * 1E3 * ADRef / ADCounts) - (Offset[RangeNo,1] * Abs(FRate) / 25);
   BVal :=
    (BVal * 1E3 * ADRef / ADCounts) - (Offset[RangeNo,2] * Abs(FRate) / 25);
   IF NOT Overflow THEN
    BEGIN
     GotoXY(19,10); Write(RVal:10:1,GVal:10:1,BVal:10:1);
     GotoXY(59,10); Write(FRate:10:1);
    END
    ELSE
    BEGIN
     GotoXY(19,10); Write('                               ');
     GotoXY(59,10); Write('           ');
    END;

  UNTIL (COM1Status <> 0) OR Keypressed;
 IF KeyPressed THEN
  ReadPressedKey;
 COM1Status := WriteCOMPort(_COM1,'ST,MC,');
 UNTIL StopFlag;

 ClrScr;
 CursorOn;
 COM1Status := WriteCOMPort(_COM1,'NR,');  {return to normal run}
 COM1Status := WriteCOMPort(_COM1,'MS,');
END;

*)