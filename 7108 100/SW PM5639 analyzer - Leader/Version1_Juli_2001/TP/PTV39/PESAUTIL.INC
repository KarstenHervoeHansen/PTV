


PROCEDURE InitVar;
BEGIN
 IICBusError := FALSE;
 TotalTestFlag := FALSE;
 AfslutProgram := FALSE;
 EEPROMPass := 1;
 IICPort64 := $FF;
 IICPort66 := $FF;
 FillChar(MeasStr,SizeOf(MeasStr),0);
 FillChar(KorrFactor,SizeOf(KorrFactor),0);
END;

PROCEDURE ClearTestFlag;
{Initialiserer variable mellem hver totaltest}
BEGIN
 FillChar(MeasStr,SizeOf(MeasStr),0);
 FillChar(KorrFactor,SizeOf(KorrFactor),0);
 TestFailed := FALSE;
 RS232TestOk := FALSE;
{$IFDEF IEEEBUS}
 CurrentTestOk := FALSE;
 AnalogPos5VTestOk := FALSE;
 AnalogNeg5VTestOk := FALSE;
 ADRefTestOk := FALSE;
{$ELSE}
 CurrentTestOk := TRUE;
 AnalogPos5VTestOk := TRUE;
 AnalogNeg5VTestOk := TRUE;
 ADRefTestOk := TRUE;
{$ENDIF}
 EEPROMTestOk := FALSE;
 LearnOffsetOk := FALSE;
 FieldRateTestOk := FALSE;
 DarkCurrentTestOk := FALSE;
 BeregnGainFactorOk := FALSE;
 ReadIdStringOk := FALSE;
 ReadStatusRequestOk := FALSE;
 ReadRefColorOk := FALSE;
 ReadCRTValueOk := FALSE;
 ReadGainFactorOk := FALSE;
 ReadPreGainOk := FALSE;
 LearnRGBOk := FALSE;
 MeasureLCOk := FALSE;
 FilterOk := FALSE;
 Analog5VTestValgt := FALSE;
 ADRefTestValgt := FALSE;
 EEPROMTestValgt := FALSE;
 TotalTestOk := FALSE;
END;




PROCEDURE InitScreen;
BEGIN
Window(1,24,80,25);
Textbackground(White);
ClrScr;
{$IFDEF IEEEBUS}
 TextColor(Red); Write(' F1:'); TextColor(Black); Write('Str›m');
{$ELSE}
 Write('         ');
{$ENDIF}
 TextColor(Red); Write(' F2:'); TextColor(Black); Write('KU-nr');
 Write('          ');
 TextColor(Red); Write(' F4:'); TextColor(Black); Write('L‘s EýPROM');
 TextColor(Red); Write(' F5:'); TextColor(Black); Write('Fieldrate');
 TextColor(Red); Write(' F6:'); TextColor(Black); Write('M›rkestr›m');
 TextColor(Red); Write('   H:'); TextColor(Black); Writeln('Hj‘lp');
 TextColor(Red); Write(' F7:'); TextColor(Black); Write('Gainfaktor');
 TextColor(Red); Write('             '); TextColor(Black); Write('');
 TextColor(Red); Write('   F9:'); TextColor(Black); Write('Learn_RGB');
 TextColor(Red); Write('  F10:'); TextColor(Black); Write('Totaltest');
 TextColor(Red); Write('            ESC:'); TextColor(Black); Write('Afslut');
 Window(1,1,80,1);
 TextColor(Red);
 ClrScr;
 Write(' Philips TV Test Equipment A/S');
 Write('   Test og kalibrering color probe ',ProbeType);
 Window(1,2,80,23);
 Color(Yellow,Blue);
 ClrScr;
END;



FUNCTION UdregnMeanValueRed : REAL;
VAR   n,p       : BYTE;
      Kode      : INTEGER;
      MeasValue : REAL;
      MVal      : REAL;
      s         : STR_10;
BEGIN
 MVal := 0;
 FOR n := 1 TO AntalMeas DO
  BEGIN
   p := Pos(',',MeasStr[n]);
   IF p > 0 THEN
     s := Copy(MeasStr[n],1,p-1);
   Val(s,MeasValue,Kode);
   MVal := MVal + MeasValue;
  END;
 MVal := MVal / AntalMeas;
 UdregnMeanValueRed := MVal;
END;


FUNCTION UdregnMeanValueGreen : REAL;
VAR   n,p,k     : BYTE;
      Kode      : INTEGER;
      MeasValue : REAL;
      MVal      : REAL;
      s         : STR_1;
      s1        : STR_10;
BEGIN
 MVal := 0;
 FOR n := 1 TO AntalMeas DO
  BEGIN
   s1 := '';
   p := Pos(',',MeasStr[n]);
   IF p > 0 THEN
     REPEAT
      Inc(p);
      s := Copy(MeasStr[n],p,1);
       IF s[1] IN ['.','0'..'9'] THEN
        s1 := s1 + s;
     UNTIL s = ',';
   Val(s1,MeasValue,Kode);
   MVal := MVal + MeasValue;
  END;
 MVal := MVal / AntalMeas;
 UdregnMeanValueGreen := MVal;
END;

FUNCTION UdregnMeanValueBlue : REAL;
VAR   n,p       : BYTE;
      Kode      : INTEGER;
      MeasValue : REAL;
      MVal      : REAL;
BEGIN
 MVal := 0;
 FOR n := 1 TO AntalMeas DO
  BEGIN
   p := Pos(',',MeasStr[n]);
    IF p > 0 THEN
    Delete(MeasStr[n],1,p);
   p := Pos(',',MeasStr[n]);
    IF p > 0 THEN
    Delete(MeasStr[n],1,p);
   Val(MeasStr[n],MeasValue,Kode);
   MVal := MVal + MeasValue;
  END;
 MVal := MVal / AntalMeas;
 UdregnMeanValueBlue := MVal;
END;

{$I SENSOR.INC}

PROCEDURE InitEEPROM;
BEGIN
 Writeln('Skriver ',ProbeType,' data til EýPROM');
 TestModePM5639;
 WriteEEPROM(52,DefNoEEPROMPages);             {antal sider i EýPROM}
 WriteEEPROM(54,255);                            {skrivebeskyttelse}
 WriteEEPROM(55,255);

 IF ProbeType = 'PESA' THEN
  BEGIN
   WriteEEPROM(58,DefInternID[1,1]);
   WriteEEPROM(59,DefInternID[1,2]);
   WriteEEPROM(60,DefInternID[1,3]);
   WriteEEPROM(61,DefInternID[1,4]);
  END ELSE
 IF ProbeType = 'VISTEK' THEN
  BEGIN
   WriteEEPROM(58,DefInternID[2,1]);
   WriteEEPROM(59,DefInternID[2,2]);
   WriteEEPROM(60,DefInternID[2,3]);
   WriteEEPROM(61,DefInternID[2,4]);
  END;
 WriteEEPROM(97,DefPreGain[1]);      {Pre gain R}
 WriteEEPROM(98,DefPreGain[2]);      {Pre gain G}
 WriteEEPROM(99,DefPreGain[3]);      {Pre gain B}
 MeasModePM5639;
END;

PROCEDURE ReadPreGainFactor;
{L‘ser PreGain faktorer fra EýPROM addr 97-99}
VAR  Svar : STRING;
     n    : BYTE;
     Kode : INTEGER;
BEGIN
 Write('Pre gain factor   ');
 COM1Status := WriteCOMPort(_COM1,'MS,MA97,');   Delay(10);
 COM1Status := ClearInputQue(_COM1);
 ReadPreGainOk := TRUE;
 FOR n:= 1 TO 3 DO
  BEGIN
   Svar := '';
   COM1Status := WriteCOMPort(_COM1,'RM,');    Delay(10);
   COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
   Val(Svar,PreGain[n],Kode);
   IF (COM1Status <> 0) OR (Kode <> 0) OR (PreGain[n] <> DefPreGain[n])THEN
    BEGIN
     ReadPreGainOk := FALSE;
     TestFailed := TRUE;
    END;
  END;
 Write('R:',PreGain[1]:4,' G:',PreGain[2]:4,' B:',PreGain[3]:4);
 TrueOrFalse(ReadPreGainOk);
END;




PROCEDURE ReadStatusRequest;
VAR  Svar  : STRING;
BEGIN
 ReadStatusRequestOk := TRUE;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'S?,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 IF (Length(Svar) <> 11) THEN
  BEGIN
   ReadStatusRequestOk := FALSE;
   TestFailed := TRUE;
  END;
 Write('Read status request: ',Svar);
 TrueOrFalse(ReadStatusRequestOk);

END;

PROCEDURE ReadRefColor;
VAR  Svar  : STRING;
BEGIN
 ReadRefColorOk := TRUE;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'R?,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 IF (Length(Svar) <= 20) THEN
  BEGIN
   ReadRefColorOk := FALSE;
   TestFailed := TRUE;
  END;
 Write('Read Reference color: ',Svar);
 TrueOrFalse(ReadRefColorOk);
END;

PROCEDURE ReadCRTValue;
VAR  Svar  : STRING;
BEGIN
 ReadCRTValueOk := TRUE;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'C?,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 IF (Length(Svar) <= 20) THEN
  BEGIN
   ReadCRTValueOk := FALSE;
   TestFailed := TRUE;
  END;
 Write('Read CRT value: ',Svar);
 TrueOrFalse(ReadCRTValueOk);
END;

PROCEDURE  ReadInternID;
VAR Svar   : STRING;
    IId    : ARRAY[1..4] OF BYTE;
    k : INTEGER;
BEGIN
 Svar := '';
 Write('Intern ID:  ');
 COM1Status := WriteCOMPort(_COM1,'MS,');   Delay(10);
 COM1Status := WriteCOMPort(_COM1,'MA58,'); Delay(5);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,IId[1],k);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,IId[2],k);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,IId[3],k);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,IId[4],k);
   ReadInternIDOk := (DefInternID[ProbeNo,1] = IId[1]) AND
                     (DefInternID[ProbeNo,2] = IId[2]) AND
                     (DefInternID[ProbeNo,3] = IId[3]) AND
                     (DefInternID[ProbeNo,4] = IId[4]);
 Write(IId[1]:4,IId[2]:4,IId[3]:4,IId[4]:4);
 TrueOrFalse(ReadInternIDOk);
 IF NOT ReadInternIDOk THEN
  BEGIN
  Writeln(' Forventet: ',DefInternID[ProbeNo,1]:4,DefInternID[ProbeNo,2]:4,
        DefInternID[ProbeNo,3]:4,DefInternID[ProbeNo,4]:4);
  TestFailed := TRUE;
  END;
END;


PROCEDURE ShowTestStatus;
CONST OkFailPos = 23;
BEGIN
 CreateWindow(48,1,24,30,' Test status ',Black+16*LightGray,Black+16*LightGray,Frame);
 Color(Black,LightGray);
 CursorOff;
 ClrScr;
{$R-}
 ShowTestFlagStatus('RS-232 test',RS232TestOk,OkFailPos);
 ShowTestFlagStatus('Str›mforbrug',CurrentTestOk,OkFailPos);
  IF Analog5VTestValgt THEN
  BEGIN
   ShowTestFlagStatus('ñ 5 V test',AnalogPos5VTestOk AND AnalogNeg5VTestOk,OkFailPos);
   ShowTestFlagStatus('A/D-ref test',ADRefTestOk,OkFailPos);
  END;
 ShowTestFlagStatus('Offset',LearnOffsetOk,OkFailPos);
 ShowTestFlagStatus('Fieldrate',FieldRateTestOk,OkFailPos);
 ShowTestFlagStatus('M›rkestr›m',DarkCurrentTestOk,OkFailPos);
 ShowTestFlagStatus('Beregn gainfaktorer',BeregnGainFactorOk,OkFailPos);
 ShowTestFlagStatus('Filtercheck',FilterOk,OkFailPos);
 ShowTestFlagStatus('KU-nummer',ValidKU,OkFailPos);
 ShowTestFlagStatus('Status request',ReadStatusRequestOk,OkFailPos);
 ShowTestFlagStatus('Ref color',ReadRefColorOk,OkFailPos);
 ShowTestFlagStatus('CRT value',ReadCRTValueOk,OkFailPos);
 ShowTestFlagStatus('ID',ReadIDStringOk,OkFailPos);
 ShowTestFlagStatus('Intern ID',ReadInternIDOk,OkFailPos);
 ShowTestFlagStatus('Pre gain',ReadPreGainOk,OkFailPos);
 ShowTestFlagStatus('Skriv. beskyt adr',ReadWriteProtectOk,OkFailPos);
 ShowTestFlagStatus('Antal EýPROM sider',ReadNoEEPROMPagesOk,OkFailPos);
 ShowTestFlagStatus('Learn color',MeasureLCOk,OkFailPos);
 ShowTestFlagStatus('Learn RGB',LearnRGBOk,OkFailPos);
 ShowTestFlagStatus('Totaltest',TotalTestOk,OkFailPos);
{$R+}
 WaitForAnyKey;
 CloseWindow;
 CursorOn;
END;
