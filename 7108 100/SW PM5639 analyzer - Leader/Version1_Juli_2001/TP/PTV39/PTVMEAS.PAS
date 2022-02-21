UNIT PtvMeas;
(*
Unit fil til brug ved xy-m†linger og ST-m†linger med PTV color sensor.
Rettelser:
930111   Y m†linger korrigeret for Fieldtime
930304   RGB m†linger p† grundlag af XYZ          TastG
*)

INTERFACE
TYPE KMEType    = ARRAY[1..18] OF BYTE;
     OffsetType = ARRAY[0..5,0..3] OF REAL;      {gain 0-5, channel 0-3}
     XYMatrixType = ARRAY[1..6] OF REAL;


PROCEDURE PTVxyMeasure(KMElement : KMEType; KMul : LONGINT;
          D6500Ref1,D6500Ref2 : REAL; NIT_fL_Factor : REAL);

PROCEDURE STMeasure(Offset : OffsetType; ADRef : REAL; ADCounts : WORD;
                    FixRangeMax : BYTE);

PROCEDURE RGBMeasure(XYCrtMatrix : XYMatrixType);


IMPLEMENTATION
USES Crt, ComUtil, PepUtil;

CONST  COMNo : BYTE = _COM1;
       YPos         = 10;
       MXMode       = 1;
       XYMode       = 2;

VAR COMStatus     : BYTE;
    KM            : ARRAY[1..9] OF REAL;
    KUNo          : STR_10;

FUNCTION ReadColorSensor : STRING;
VAR Sv : STRING;
BEGIN
 COMStatus := ReadlnCOMPort(COMNo,Sv,2);
  GotoXY(6,19); ClrEol; Write(Sv);
 IF COMStatus = 0 THEN
  ReadColorSensor := Sv
 ELSE
  ReadColorSensor := '0,0,0,0';
END;

PROCEDURE GetRGBChannel
            (VAR R,G,B,FR : REAL; VAR Overflow : BOOLEAN; InStr : STRING;
             Mode : BYTE);   {1=MX  2=XY}
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

  IF Mode = 1 THEN
   BEGIN
    p := Pos(',',InStr);
    IF p > 0 THEN
     s := Copy(InStr,1,p-1);
    Val(s,B,Kode);
    Delete(InStr,1,p);

    Val(InStr,FR,Kode);
   END
   ELSE
   BEGIN
    Val(InStr,B,Kode);
    FR := 0;
   END
END;

FUNCTION MakeKmatrix(KMElement : KMEType; KMul : LONGINT) : BOOLEAN;
VAR  Flag : BOOLEAN;
     n    : BYTE;
BEGIN
{$R-}
 FOR n := 1 TO 9 DO
  KM[n] := (KMElement[n*2-1] * 256 + KMElement[n*2]) / KMul;
{$R+}

 Flag := TRUE;
 FOR n := 1 TO 9 DO
  IF Abs(KM[n]) > 0.2 THEN      {K-matrix multipliceres med 1E5}
   Flag := FALSE;
 MakeKmatrix := Flag;
END;

PROCEDURE PTVxyMeasure(KMElement : KMEType; KMul : LONGINT;
          D6500Ref1,D6500Ref2 : REAL; NIT_fL_Factor : REAL);
(*
K-matrix indl‘ses kun i starten af PTVxyMeasure-proceduren.
Hvis der skiftes m†lehoved, skal proceduren forlades og kaldes igen.
Proceduren beregner x,y,u',v',Y,X,Y,Z og CIELUV.
Hvis Y er < 0.05 cd/mý, blankes felterne for m†leresultaterne.
*)
VAR
    CieLuv        : INTEGER;
    X,Y,Z,                       {X,Y,Z integralet}
    XYZSum,
    uvDiv,
    fL,
    FR,                          {fieldrate in mS}
    uKoor, vKoor,
    xKoor, yKoor,
    zKoor         : REAL;
    Svar          : STRING;
    Overflow      : BOOLEAN;


PROCEDURE CalculateXYZ (MStr : STR_30);
VAR R,G,B,FR : REAL;
    OFlow    : BOOLEAN;
BEGIN
 GetRGBChannel(R,G,B,FR,OFlow,MStr,MXMode);
 FR := Abs(FR);
 IF FR <> 0 THEN
  BEGIN
   X := (R*KM[1] + G*KM[2] + B*KM[3]) / FR * 20;
   Y := (R*KM[4] + G*KM[5] + B*KM[6]) / FR * 20;
   Z := (R*KM[7] + G*KM[8] + B*KM[9]) / FR * 20;
  END
  ELSE
  BEGIN
   X := 0; Y := 0; Z := 0;
  END;
 Overflow := OFlow;
END;



FUNCTION CalculateCieLuv(uKoor,vKoor,uRef,vRef : REAL) : INTEGER;
{Lininger for CIELUV-beregninger findes i Television Engineering Handbook,
 McGraw Hill 1986 side 2.38-2.40 (ISBN 0-07-004779-0) }
VAR CL, LSt, uSt, vSt : REAL;
BEGIN
Lst := 100;          {Lst := 116 * (Exp(ln(Y/Yref))/3) - 16 }
                     {color diff without luminance (Y/Yref = 1)}
uSt := 13*Lst*(uKoor-uRef);
vSt := 13*Lst*(vKoor-vRef);
CL := Sqrt({Sqr(Lst)} + Sqr(ust) + Sqr(vst));
CalculateCieLuv := Round(CL);
END;


BEGIN
 ClrScr;
 COMStatus := WriteCOMPort(COMNo,'MS,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 KUNo := Copy(Svar,24,6);
 IF Length(KUNo) < 6 THEN
  BEGIN
   Writeln(Bell,'Ingen color sensor !');
   Exit;
  END;

 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'MA58,');
 COMStatus := WriteCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 IF Svar <> '001' THEN
  BEGIN
   Writeln(Bell,'Ikke PTV color sensor !');
   Exit;
  END;

 IF (NOT MakeKmatrix(KMElement,KMul)) THEN
  BEGIN
   Writeln(Bell,'K-matrix ikke indl‘st !');
   Writeln('Matrix elementer > 20000');
   Exit;
  END;

 CursorOff;
 GotoXY(25,2); Write('M†ling med color sensor KU',KUNo);
 GotoXY(2,21); Write('Afslut: tryk en tast');
 GotoXY(25,YPos+1); Write('x= ');
 GotoXY(25,YPos+2); Write('y= ');
{ GotoXY(45,YPos+1); Write('u',#39,'= ');}
{ GotoXY(45,YPos+2); Write('v',#39,'= ');}
 GotoXY(25,YPos+3); Write('Y=         cd/mý');
{ GotoXY(25,YPos+4); Write('CIELUV=       (D6500)');}
 GotoXY(25,YPos+5); Write('XZ=');
 GotoXY(2,19); Write('MX:');
 COMStatus := WriteCOMPort(COMNo,'MX,');
 COMStatus := WriteCOMPort(COMNo,'MC,');
 COMStatus := ClearInputQue(COMNo);
 REPEAT
  REPEAT
   CalculateXYZ(ReadColorSensor);
   XYZSum := X + Y + Z;
    IF Y > 0.05 THEN
     BEGIN
      xKoor := X / XYZSum;
      yKoor := Y / XYZSum;
      uvDiv := -2*xKoor + 12*yKoor + 3;
      uKoor := 4*xKoor / uvDiv;
      vKoor := 9*yKoor / uvDiv;
{      CieLuv := CalculateCieLuv(uKoor,vKoor,D6500Ref1,D6500Ref2);}
      GotoXY(28,YPos+1);Write(xKoor:6:3);
      GotoXY(28,YPos+2);Write(yKoor:6:3);
{      GotoXY(49,YPos+1);Write(uKoor:6:3);}
{      GotoXY(49,YPos+2);Write(vKoor:6:3);}
      GotoXY(27,YPos+3);Write(Y:7:2);
{      GotoXY(33,YPos+4); Write(CieLuv:4);}
      GotoXY(29,YPos+5); Write(X:7:2,Z:7:2);
     END
    ELSE
     BEGIN
      GotoXY(28,YPos+1); Write('      ');
      GotoXY(28,YPos+2); Write('      ');
{      GotoXY(49,YPos+1); Write('      ');}
{      GotoXY(49,YPos+2); Write('      ');}
      GotoXY(27,YPos+3);
       IF  Overflow THEN
        Write('   ^^  ')
         ELSE
        Write('       ');
{      GotoXY(33,YPos+4);Write('     ');}  {cieluv}
      GotoXY(29,YPos+5);Write('               ');
      GotoXY(6,19);
      IF COMStatus <> 0 THEN
       Write('Timeout');
     END;
  UNTIL (COMStatus <> 0) OR Keypressed;
 COMStatus := WriteCOMPort(COMNo,'MX,MC,');
 UNTIL Keypressed;

 ClrScr;
 CursorOn;
 COMStatus := WriteCOMPort(COMNo,'MS,');
END; {PTVxyMeasure}




PROCEDURE STMeasure(Offset : OffsetType; ADRef : REAL; ADCounts : WORD;
                    FixRangeMax : BYTE);
{
Offset-v‘rdier fra EEPROM indl‘ses kun i starten af STMeasure-proceduren.
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
 COMStatus := WriteCOMPort(COMNo,RStr);
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
  Tast5  : RangeNo := 5;
  EndKey : StopFlag := TRUE;
 ELSE
  Beep(800,200);
 END;
 SetFixRange(RangeNo);
 EmptyKeyboardBuffer;
END;


BEGIN
 ClrScr;
 COMStatus := WriteCOMPort(COMNo,'MS,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 KUNo := Copy(Svar,24,6);
 IF Length(KUNo) < 6 THEN
  BEGIN
   Writeln(Bell,'Ingen color sensor !');
   Exit;
  END;
 EmptyKeyboardBuffer;
 CursorOff;
 StopFlag := FALSE;
 RangeNo := 0;
 GotoXY(25,2); Write('ST m†ling med color sensor KU',KUNo);
 GotoXY(26,8);  Write('R›d       Gr›n      Bl†              Field rate');
 GotoXY(52,10); Write('mV');
 GotoXY(71,10); Write('mS');
 GotoXY(52,13); Write('mV');
 GotoXY(65,13); Write('25.0  mS');
 GotoXY(4,13); Write('EýPROM offset:');
 GotoXY(2,21); Write('Afslut: tryk End');
 GotoXY(30,21); Write('Gain 0 -> ',FixRangeMax,': Tryk 0 -> ',FixRangeMax);
 GotoXY(11,10); Write('Gain  :');
 COMStatus := WriteCOMPort(COMNo,'MC');
 COMStatus := WriteCOMPort(COMNo,'ST,');
 SetFixRange(RangeNo);
 COMStatus := ClearInputQue(COMNo);
 REPEAT
  REPEAT
   GetRGBChannel(RVal,GVal,BVal,FRate,Overflow,ReadColorSensor,MXMode);
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

  UNTIL (COMStatus <> 0) OR Keypressed;
 IF KeyPressed THEN
  ReadPressedKey;
 COMStatus := WriteCOMPort(COMNo,'ST,MC,');
 UNTIL StopFlag;

 ClrScr;
 CursorOn;
 COMStatus := WriteCOMPort(COMNo,'NR,');  {return to normal run}
 COMStatus := WriteCOMPort(COMNo,'MS,');
END;



PROCEDURE RGBMeasure(XYCrtMatrix : XYMatrixType);   {TastG}
VAR RBar,
    GBar,
    BBar     : REAL;
    OverFlow : BOOLEAN;
    Svar     : STRING;

PROCEDURE CalculateRGB (MStr : STR_30);
VAR R,G,B,FR : REAL;
    OFlow    : BOOLEAN;
BEGIN
 GetRGBChannel(R,G,B,FR,OFlow,MStr,XYMode);
{ GotoXY(1,17); ClrEol;  Write(R:7:2,G:7:2,B:7:2);}
{ R:=46.3; G:=68.6;  B:=92.5;}
 RBar := (R*1 + G*XYCrtMatrix[1] + B*XYCrtMatrix[2]);
 GBar := (R*XYCrtMatrix[3] + G*1 + B*XYCrtMatrix[4]);
 BBar := (R*XYCrtMatrix[5] + G*XYCrtMatrix[6] + B*1);
 Overflow := OFlow;
END;



BEGIN
 ClrScr;
 COMStatus := WriteCOMPort(COMNo,'MS,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 KUNo := Copy(Svar,24,6);
 IF Length(KUNo) < 6 THEN
  BEGIN
   Writeln(Bell,'Ingen color sensor !');
   Exit;
  END;

 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'MA58,');
 COMStatus := WriteCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 IF Svar <> '001' THEN
  BEGIN
   Writeln(Bell,'Ikke PTV color sensor !');
   Exit;
  END;

 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'MA61,');
 COMStatus := WriteCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 IF Svar <> '032' THEN
  BEGIN
   Writeln(Bell,'Ikke Industri color sensor !');
   Exit;
  END;

 CursorOff;
 GotoXY(25,2); Write('M†ling med color sensor KU',KUNo);
 GotoXY(2,21); Write('Afslut: tryk en tast');
 GotoXY(35,YPos-1); Write('R= ');
 GotoXY(35,YPos+1); Write('G= ');
 GotoXY(35,YPos+3); Write('B= ');
 COMStatus := WriteCOMPort(COMNo,'MS,');
 COMStatus := WriteCOMPort(COMNo,'XY,');
 COMStatus := WriteCOMPort(COMNo,'MC,');
 COMStatus := ClearInputQue(COMNo);
 REPEAT
  REPEAT
   CalculateRGB(ReadColorSensor);
   GotoXY(38,YPos-1);Write(RBar:6:2);
   GotoXY(38,YPos+1);Write(GBar:6:2);
   GotoXY(38,YPos+3);Write(BBar:6:2);
  UNTIL (COMStatus <> 0) OR Keypressed;
 COMStatus := WriteCOMPort(COMNo,'XY,MC,');
 UNTIL Keypressed;

 COMStatus := WriteCOMPort(COMNo,'MS,MX');
 ClrScr;
 CursorOn;

END;




BEGIN
{evt. initialisering}
END.
