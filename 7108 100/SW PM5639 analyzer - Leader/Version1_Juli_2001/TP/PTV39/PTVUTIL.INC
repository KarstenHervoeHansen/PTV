

Procedure ReadxyValuesFromFile;
var Svar       : STR_70;
    Kode       : INTEGER;
    s          : STR_1;
    xyStr,
    LumStr     : STR_10;
    xyVal      : ARRAY[1..4,1..2] OF REAL;
    n,k        : BYTE;
    xyError    : BOOLEAN;
begin
 FillChar(xyVal,SizeOf(xyVal),0);
 Assign(XYFile,XYFilename);
 {$I-}
 Reset(XYFile);
 {$I+}
 if IOResult <> 0 then
  begin
   Writeln(Bell,XYFilename,' ikke fundet');
   Writeln('Program afbrudt  -  tryk en tast');
   WaitForAnyKey;
   AfslutProgram := TRUE;
   Exit;
  end;

  xyError := FALSE;
  n := 1;
  k := 1;
 repeat
  repeat
   Readln(XYFile,Svar);
  until Eof(XYFile) OR (Length(Svar) > 5);
  xyStr := '';
  repeat
   s := Copy(Svar,n,1);
   Inc(n);
  until (s[1] IN Tal) OR (s[1] = '.') OR (s[1] = '');
  repeat
   if (s[1] IN Tal) OR (s[1] = '.') then
    xyStr := Concat(xyStr,s);
   s := Copy(Svar,n,1);
   Inc(n);
  until NOT ((s[1] IN Tal) OR (s[1] = '.')) OR (s = '');

  Val(xyStr,xyVal[k,1],Kode);
  if Kode <> 0 then
   xyError := TRUE;

  xyStr := '';
  repeat
   s := Copy(Svar,n,1);
   Inc(n);
  until (s[1] IN Tal) OR (s[1] = '.') OR (s[1] = '');
  repeat
   if (s[1] IN Tal) OR (s[1] = '.') then
    xyStr := Concat(xyStr,s);
   s := Copy(Svar,n,1);
   Inc(n);
  until NOT ((s[1] IN Tal) OR (s[1] = '.')) OR (s = '');

  Val(xyStr,xyVal[k,2],Kode);
  if Kode <> 0 then
   xyError := TRUE;

  Inc(k);
 until Eof(XYFile) OR (k > 4) OR xyError;

 Readln(XYFile,Svar);
 LumStr := '';
 repeat
  s := Copy(Svar,n,1);
  Inc(n);
 until (s[1] IN Tal) OR (s[1] = '.') OR (s[1] = '');
 repeat
  if (s[1] IN Tal) OR (s[1] = '.') then
   LumStr := Concat(LumStr,s);
  s := Copy(Svar,n,1);
  Inc(n);
 until NOT ((s[1] IN Tal) OR (s[1] = '.')) OR (s = '');

 Close(XYFile);

 Val(LumStr,NominelLum,Kode);
  if (Kode <> 0) OR (NominelLum < 0) OR (NominelLum > MaxLuminans) then
   begin
    Writeln('Fejl i luminans-v‘rdi: ',NominelLum:6:1,' cd/mý');
    xyError := TRUE;
   end;


  XpR := xyVal[1,1];   YpR := xyVal[1,2];   ZpR := 1 - XpR - YpR;
  XpG := xyVal[2,1];   YpG := xyVal[2,2];   ZpG := 1 - XpG - YpG;
  XpB := xyVal[3,1];   YpB := xyVal[3,2];   ZpB := 1 - XpB - YpB;
 Xp65 := xyVal[4,1];  Yp65 := xyVal[4,2];  Zp65 := 1 - Xp65 - Yp65;

 for k := 1 TO 4 DO
  if (xyVal[k,1] + xyVal[k,2]) > 1.00 then
   begin
    xyError := TRUE;
    Writeln('Fejl i xy-v‘rdier: ',xyVal[k,1]:10:4,xyVal[k,2]:10:4);
   end;

 if xyError then
  begin
   AfslutProgram := TRUE;
   Writeln('Kontroller data i filen: ',XYFilename);
   Writeln('Program afbrudt  -  tryk en tast');
   WaitForAnyKey;
  end;
end;  {ReadxyValuesFromFile}



Procedure ClearTestFlag;
{Initialiserer variable mellem hver totaltest}
begin
 FillChar(MeasStr,SizeOf(MeasStr),0);
 TestFailed := FALSE;
 RS232TestOk := FALSE;
{$ifDEF IEEEBUS}
 CurrentTestOk := FALSE;
 AnalogPos5VTestOk := FALSE;
 AnalogNeg5VTestOk := FALSE;
 ADRefTestOk := FALSE;
{$ELSE}
 CurrentTestOk := TRUE;
 AnalogPos5VTestOk := TRUE;
 AnalogNeg5VTestOk := TRUE;
 ADRefTestOk := TRUE;
{$endif}
 EEPROMTestOk := TRUE;
 FieldRateTestOk := FALSE;
 DarkCurrentTestOk := FALSE;
 BeregnGainFactorOk := FALSE;
 ReadIdStringOk := FALSE;
 ReadGainFactorOk := FALSE;
 ReadCRTNameOk := FALSE;
 ReadKorrMatrixOk := FALSE;
 ReadCRTMatrixOk := FALSE;
 ReadInternIDOk := FALSE;
 ReadPreGainOk := FALSE;
 ReadWriteProtectOk := FALSE;
 ReadNoCRTFosforOk := FALSE;
 ReadNoEEPROMPagesOk := FALSE;
 LearnKMatrixOk := FALSE;
 LearnOffsetOk := FALSE;
 LearnEBUOk := FALSE;
 LearnSMPTE_COk := FALSE;
 FilterOk := FALSE;
 Analog5VTestValgt := FALSE;
 ADRefTestValgt := FALSE;
 EEPROMTestValgt := FALSE;
 TotalTestOk := FALSE;
end;

Procedure InitVar;
begin
 ClearTestFlag;
 IICBusError := FALSE;
 TotalTestFlag := FALSE;
 AfslutProgram := FALSE;
 EEPROMPass := 1;
 IICPort64 := $FF;
 IICPort66 := $FF;
 FillChar(MeasStr,SizeOf(MeasStr),0);
 ReadxyValuesFromFile;
end;



Procedure InitScreen;
begin
 Window(1,24,80,25);
 TextBackground(LightGray);
 ClrScr;
{$ifDEF IEEEBUS}
 TextColor(Red); Write(' F1:'); TextColor(Black); Write('Str›m');
{$ELSE}
 Write('         ');
{$endif}
 TextColor(Red); Write(' F2:'); TextColor(Black); Write('KU-nr');
 TextColor(Red); Write(' F3:'); TextColor(Black); Write('Battery');
 TextColor(Red); Write(' F4:'); TextColor(Black); Write('L‘s EýPROM');
 TextColor(Red); Write(' F5:'); TextColor(Black); Write('Fieldrate');
 TextColor(Red); Write(' F6:'); TextColor(Black); Write('M›rkestr›m');
 TextColor(Red); Write('  H:'); TextColor(Black); Writeln('Hj‘lp');
 TextColor(Red); Write(' F7:'); TextColor(Black); Write('Gainfaktor');
 TextColor(Red); Write(' F8:'); TextColor(Black); Write('Learn SMPTE C');
 TextColor(Red); Write(' F9:'); TextColor(Black); Write('Learn K-matrix,EBU');
 TextColor(Red); Write(' F10:'); TextColor(Black); Write('Totaltest');
 TextColor(Red); Write('  ESC:'); TextColor(Black); Write('Afslut');
 Window(1,1,80,1);
 Color(Red,LightGray);
 ClrScr;
 Write(' Philips TV Test Equipment A/S     ');
 Write(' Test og kalibrering PM5639/90 serie II');

 Window(1,2,80,23);
 Color(Yellow,Blue);
 ClrScr;
end;


FUNCTION UdregnMeanValueRed : REAL;
var   n,p       : BYTE;
      Kode      : INTEGER;
      MeasValue : REAL;
      MVal      : REAL;
      s         : STR_10;
begin
 MVal := 0;
 for n := 1 TO AntalMeas DO
  begin
   p := Pos(',',MeasStr[n]);
   if p > 0 then
     s := Copy(MeasStr[n],1,p-1);
   Val(s,MeasValue,Kode);
   MVal := MVal + MeasValue;
  end;
 MVal := MVal / AntalMeas;
 UdregnMeanValueRed := MVal;
end;


FUNCTION UdregnMeanValueGreen : REAL;
var   n,p,k     : BYTE;
      Kode      : INTEGER;
      MeasValue : REAL;
      MVal      : REAL;
      s         : STR_1;
      s1        : STR_10;
begin
 MVal := 0;
 for n := 1 TO AntalMeas DO
  begin
   s1 := '';
   p := Pos(',',MeasStr[n]);
   if p > 0 then
     repeat
      Inc(p);
      s := Copy(MeasStr[n],p,1);
       if s[1] IN ['.','0'..'9'] then
        s1 := s1 + s;
     until s = ',';
   Val(s1,MeasValue,Kode);
   MVal := MVal + MeasValue;
  end;
 MVal := MVal / AntalMeas;
 UdregnMeanValueGreen := MVal;
end;

FUNCTION UdregnMeanValueBlue : REAL;
var   n,p       : BYTE;
      Kode      : INTEGER;
      MeasValue : REAL;
      MVal      : REAL;
begin
 MVal := 0;
 for n := 1 TO AntalMeas DO
  begin
   p := Pos(',',MeasStr[n]);
    if p > 0 then
    Delete(MeasStr[n],1,p);
   p := Pos(',',MeasStr[n]);
    if p > 0 then
    Delete(MeasStr[n],1,p);
   p := Pos(',',MeasStr[n]);
    if p > 0 then
    Delete(MeasStr[n],p,6);
   Val(MeasStr[n],MeasValue,Kode);
   MVal := MVal + MeasValue;
  end;
 MVal := MVal / AntalMeas;
 UdregnMeanValueBlue := MVal;
end;




{$I SENSOR.INC}

Procedure  ReadInternID;
var Svar   : string;
    k      : integer;
begin
 Svar := '';
 Write('Intern ID: ');
 COM1Status := WriteCOMPort(_COM1,'MS,');   Delay(5);
 COM1Status := WriteCOMPort(_COM1,'MA58,'); Delay(5);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(5);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,InternIDRead[1],k);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(5);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,InternIDRead[2],k);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(5);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,InternIDRead[3],k);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(5);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,InternIDRead[4],k);
   ReadInternIDOk := (DefInternID[1] = InternIDRead[1]) AND
                     (DefInternID[2] = InternIDRead[2]) AND
                     (DefInternID[3] = InternIDRead[3]) AND
                     (DefInternID[4] = InternIDRead[4]);
 if ReadInternIDOk then
  Write(InternIDRead[1]:4,InternIDRead[2]:4,InternIDRead[3]:4,InternIDRead[4]:4);
 TrueOrFalse(ReadInternIDOk);
 if NOT ReadInternIDOk then
  TestFailed := TRUE;
end;



Procedure WriteCRTNamesToEEPROM;  {AltF8}
var n : BYTE;
begin
 Writeln('CRT-navne skrevet til EýPROM');
 TestModePM5639;
 WriteEEPROM(1016,Ord('E'));     {CRT navn # 1}
 WriteEEPROM(1017,Ord('B'));
 WriteEEPROM(1018,Ord('U'));
 WriteEEPROM(1019,Ord(NULL));
 WriteEEPROM(996,Ord('S'));      {CRT navn # 2}
 WriteEEPROM(997,Ord('M'));
 WriteEEPROM(998,Ord('P'));
 WriteEEPROM(999,Ord('T'));
 WriteEEPROM(1000,Ord('E'));
 WriteEEPROM(1001,Ord(' '));
 WriteEEPROM(1002,Ord('C'));
 WriteEEPROM(1003,Ord(NULL));
 for n := 2 TO 29 DO
  WriteEEPROM(1016 - (n * 20),Ord(NULL));   {CRT#3 > #30}
end;


Procedure ReadCRTNamesFromEEPROM(Flag : BOOLEAN);     {CtrlF3}
{L‘ser CRT-navne fra EýPROM, gemmer navnene i CertifData.Name,
 og viser dem p† sk‘rmen}
var n,
    k       : BYTE;       {antal tegn i hvert navn}
    Kode    : INTEGER;
    Value   : BYTE;
    Addr    : WORD;
    AddrStr : STR_5;
    Svar    : STRING;
    NULLFound : BOOLEAN;
begin
 if Flag then
  Writeln('CRT-navne: ');
 ReadCRTNameOk := TRUE;
 FillChar(CRTName,SizeOf(CrtName),0);
 NULLFound := FALSE;
 k := 1;
 n := 1;
 Addr := CRTNameAddr;
 repeat
  Str(Addr,AddrStr);
  COM1Status := WriteCOMPort(_COM1,'MA'+ AddrStr + ',');
  delay(20);
  COM1Status := ClearInputQue(_COM1);
   repeat
    COM1Status := WriteCOMPort(_COM1,'RM,');delay(20);
    COM1Status := ReadlnCOMPort(_COM1,Svar,1);
    Val(Svar,Value,Kode);
    if Value <> 0 then
     begin
      CRTName[n] := CRTName[n] + Chr(Value);
     end
    ELSE
     NULLFound := TRUE;
    Inc(k);
   until (COM1Status <> 0) OR (Svar = '000') OR (Kode <> 0) OR (k > 8);

 if Flag then
  if (n MOD 2) = 1 then
    begin
      Write(' #',n:2,': ',CRTName[n])
    end
   ELSE
    begin
     GotoXY(40,WhereY);
      Write(' #',n:2,': ',CRTName[n]);
    end;

  if (NOT NULLFound) OR (COM1Status <> 0) then
   begin
    ReadCRTNameOk := FALSE;
    TestFailed := TRUE;
    LastAttr := TextAttr; TextColor(White);
    if Flag then
     Write('  NULL ikke fundet');
    TextAttr := LastAttr;
   end; {if}
  k := 1;
  Inc(n);
  Dec(Addr,20);
  NULLFound := FALSE;
  if ((n MOD 2) = 1) AND Flag then
   Writeln;
 until (Addr <= (CRTNameAddr - (20 * DefMaxNoCRTFosfor))) OR (COM1Status <> 0);
end;

Procedure ReadPreGainFactor;      {F4}
{L‘ser pre gain fra EýPROM fra addr 97-102}
var Kode    : INTEGER;
    PreGain : ARRAY[1..6] OF BYTE;
    n,
    Value   : BYTE;
    Svar    : STRING;
    Yf      : WORD;
begin
 Write('Pre gain :');
 ReadPreGainOk := TRUE;
 FillChar(PreGain,SizeOf(PreGain),0);
 n := 1;
 COM1Status := WriteCOMPort(_COM1,'MA97,'); Delay(5);
 COM1Status := ClearInputQue(_COM1);
 repeat
  COM1Status := WriteCOMPort(_COM1,'RM,'); Delay(5);
  COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
  Val(Svar,Value,Kode);
  if Kode = 0 then
   PreGain[n] := Value
  ELSE
   ReadPreGainOk := FALSE;
  Inc(n);
 until (COM1Status <> 0) OR (Kode <> 0) OR (n > 6);
{$R-}
 Yf := PreGain[3] * 256 + PreGain[4];
{$R+}
 if Yf > 0 then
  begin
   XFact := (PreGain[1] * 256 + PreGain[2]) / Yf;
   YFact := Yf / PreGainMulFactor;;
   ZFact := (PreGain[5] * 256 + PreGain[6]) / Yf;
   Write('  XFact:',XFact:6:3,'   YFact:',YFact:6:3);
   Writeln('   ZFact:',ZFact:6:3);
  end
 ELSE
  begin
   Write(' Error !  >>> ');
   for n := 1 TO 6 DO
    Write(PreGain[n],'  ');
   Writeln;
   ReadPreGainOk := FALSE;
   TestFailed := TRUE;
  end;

 if NOT ReadPreGainOk then
  TestFailed := TRUE;
end;

FUNCTION ReadTraceFlag : STRING;
{L‘ser traceability status for sensoren}
var  Svar : STRING;
begin
 COM1Status := WriteCOMPort(_COM1,'MS,');    Delay(5);
 COM1Status := WriteCOMPort(_COM1,'MA96,');  Delay(5);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');    Delay(5);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);  Delay(5);
 Delay(20);
 if Svar = '255' then
  ReadTraceFlag := 'NPL'
 ELSE
  ReadTraceFlag := 'NIST';
end;

Procedure WriteCalDate;
{Cal dato skrives til EýPROM adresse 176-181}
Var n    : byte;
begin
 DateStr := dato;
 writeln(datestr);
 COM1Status := writeCOMPort(_COM1,'MS,');
 Delay(20);
 COM1Status := writeCOMPort(_COM1,'MA176,');
 Delay(20);
 for n := 1 to 6 do
  begin
   writeEEPROM(n+175,Ord(DateStr[n]));
  end;
end;

Procedure ReadCalDate(Flag : boolean);      {F4}
{L‘ser cal dato fra EýPROM fra addr 176-181.
 Hvis Flag=true udskrives til sk‘rmen. }
var kode    : integer;
    n,
    Value   : byte;
    Svar    : string;
begin
 if Flag then
  Write('Kal dato:     ');
 ReadCalDateOk := true;
 DateStr := '';
 n := 1;
 COM1Status := WriteCOMPort(_COM1,'MS,');
 Delay(20);
 COM1Status := WriteCOMPort(_COM1,'MA176,');
 Delay(20);
 COM1Status := ClearInputQue(_COM1);
 repeat
  COM1Status := WriteCOMPort(_COM1,'RM,');
  Delay(20);
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  Delay(20);
  Val(Svar,Value,kode);
  if (kode = 0) and (value <> 255) then
    DateStr := DateStr + Chr(Value)
   else
    DateStr := DateStr + '?';
  Inc(n);
 until (COM1Status <> 0) or (n > 6) or (kode <> 0);
 if Flag then
  writeln(DateStr)
end;

Procedure InitPtvEEPROM;     {AltF3}
begin
 ReadInternID;
 if (not ReadInternIDOk ) and
   ((InternIDRead[1] <> 255) or
    (InternIDRead[2] <> 255) or
    (InternIDRead[3] <> 255) or
    (InternIDRead[4] <> 255)) then
  begin
   TestFailed := true;
   write(bell);
   writeln('Intern ID er ikke korrekt. Kontroller at der anvendes korrekt testprogram til');
   writeln('color sensoren. Hvis printet skal anvendes i en anden type color sensor, kan');
   writeln('intern ID kan rettes med programmet COM5639.EXE. Adresserne 58-61 skal da alle');
   writeln('rettes til 255 s†ledes:  MA58,SM255,SM255,SM255,SM255,');
   writeln('Husk: skrivebeskyttelsen skal v‘re OFF');
   exit;
  end;

 Writeln('Skriver PTV data til EýPROM:');
 TestModePM5639;
 WriteEEPROM(52,DefNoEEPROMPages);
 WriteEEPROM(53,DefMaxNoCRTFosfor);
 WriteEEPROM(54,$FF);
 WriteEEPROM(55,$FF);
 WriteEEPROM(58,DefInternID[1]);              {default intern kode for PTV}
 WriteEEPROM(59,DefInternID[2]);
 WriteEEPROM(60,DefInternID[3]);
 WriteEEPROM(61,DefInternID[4]);
 WriteEEPROM(157,255);                   {kal status = FACTORY}
 WriteEEPROM(96,TraceFlag);              {set trace status}
 Writeln(' Antal EýPROM sider: ',DefNoEEPROMPages);
 Writeln(' Antal CRT-fosforer: ',DefMaxNoCRTFosfor);
 Writeln(' H›jeste skrivebeskyttelse adresse: 255 255');
 Writeln(' Kal status : FACTORY');       { used in PM8550 }
 Writeln(' Trace status:', ReadTraceFlag);
 Write(' Intern ID (adr 58 - 61): ');
 Writeln(DefInternID[1]:4,DefInternID[2]:4,DefInternID[3]:4,DefInternID[4]:4);
 WriteNOT_OKToEEPROM;
end;


Procedure ShowHelpWindow;         {Tast H}
var  Sc : BYTE;
begin
 CreateWindow(2,2,22,78,' Help ',Black+16*Cyan,Black+16*Cyan,Frame);
 Color(Black,Cyan);
 ClrScr;
 CursorOff;
 Writeln(' Test software no: ',TestSWNo,'   Rev: ',TestSWRev);
 Writeln('                                                A : Lampe lysniveau');
 Writeln('                                                C : Slet PC-sk‘rm');
 Writeln(' Alt-F3    : Initialiser EýPROM til PTV');
 Writeln(' Alt-F4    : M†l offset                         E : Skift LED on/off');
 Writeln(' Alt-F5    : Skriv default ID string            F : Skift frekvens');
 Writeln(' Alt-F6    : Skriv default gain factor          H : Vis hj‘lp');
 Writeln(' Alt-F7    : Skriv default pre-gain factor      L : T‘nd/sluk lampe');
 Writeln(' Alt-F8    : Skriv default CRT-navn             M : xyY-m†ling');
 Writeln(' Alt-F9    : L‘s cal-data og gem p† LAN         O : ST-m†ling');
 Writeln(' Alt-F10   : Udskriv cal-data til laserprinter  P : Power OFF - ON');
 Writeln('  ');
 Writeln(' Ctrl-F1   : L‘s CRT-matrix                     R : Skift rel‘');
 Writeln(' Ctrl-F2   : L‘s K-matrix                       W : SONY monitor=hvid');
 Writeln(' Ctrl-F3   : L‘s CRT-navn');
 Writeln(' Ctrl-F4   : Sensor = skrivebeskyttet ?');
 Writeln(' Ctrl-F5   : Vis aktuel xy-koordinater og luminans');
 Writeln(' Ctrl-F6   : L‘s offset v‘rdier i EýPROM');
 Writeln(' Ctrl-F7   : Skriv writeprotect = ',DefMaxWriteProtectAddr);
 GotoXY(2,20);
 EmptyKeyboardBuffer;
 WaitForAnyKey;
 CursorOn;
 CloseWindow;
end;



Procedure ShowActualXYValues;   {CtrlF5}
begin
 Writeln('Data fra file: "',XYFileName,'":');
 Writeln('  R›d:  x= ',XpR:5:3,'   y= ',YpR:5:3);
 Writeln('  Gr›n: x= ',XpG:5:3,'   y= ', YpG:5:3);
 Writeln('  Bl†:  x= ',XpB:5:3,'   y= ', YpB:5:3);
 Writeln('  Hvid: x= ',Xp65:5:3,'   y= ', Yp65:5:3);
 Writeln('  Luminans: ',NominelLum:5:1,' cd/mý');
end;

Procedure WriteDefaultPreGain;  {AltF7}
begin
 Write('Default pre-gain faktorer skrevet til EýPROM');
 Writeln(' adresse ',PreGainAddr,' - ',PreGainAddr + 5);
 TestModePM5639;
 WriteEEPROM(PreGainAddr    ,Hi(Trunc((DefaultPreGain[1]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 1,Lo(Trunc((DefaultPreGain[1]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 2,Hi(Trunc((DefaultPreGain[2]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 3,Lo(Trunc((DefaultPreGain[2]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 4,Hi(Trunc((DefaultPreGain[3]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 5,Lo(Trunc((DefaultPreGain[3]*PreGainMulFactor))));
 WriteNOT_OKToEEPROM;
end;

Procedure DAVoltage(Volt : REAL);
var Data : BYTE;
begin
{$ifDEF RGBGEN}
 Data := Trunc((Volt - VoltMin) / ((VoltMax - VoltMin) / 255));
 TrmDA8591(I2CIOAddr,146,Data,St);
{$endif}
end;


Procedure ShutdownControl;  {F3}
 const L = 15;
 var Marker,
     n,No,
     Sc       : BYTE;
     VoltOut  : REAL;

 Procedure SetVoltage(Volt : REAL);
 var  Data : BYTE;
 begin
  Data := Trunc((Volt - VoltMin) / ((VoltMax - VoltMin) / 255));
{$ifDEF RGBGEN}
  TrmDA8591(I2CIOAddr,146,Data,St);
{$endif}
  GotoXY(Marker,L); Write(CHR(220));
  Marker := 6 + (Data DIV 4);
  GotoXY(Marker,L); Write(CHR(219));
  GotoXY(36,L+3); Color(Red,White);
  Write(Volt:5:2,' V'); Color(Yellow,Blue);
 end;

begin
 CreateWindow(2,2,22,78,' Display unit shutdown control ',White+16*Blue,Yellow+16*Blue,Frame);
 Color(Yellow,Blue);
 CursorOff;
 ClrScr;
 Writeln;
 Writeln(' Forbind batteriterminaler i display unit til kontrolsp‘ndingen');
 Writeln;
 Writeln(' Pil ',Chr(30),' ',Chr(31),':  Trinl›s justering op/ned');
 Writeln(' Pil ',Chr(17),' ',Chr(16),':  Stepvis justering op/ned');
 GotoXY(5,L+1); Write(VoltMin:3:1,' V');
 GotoXY(68,L+1); Write(VoltMax:3:1,' V');
 GotoXY(2,20); Write(' Afslut - tast End');
 GotoXY(6,L);
 for n := 1 TO 64 DO
  Write(CHR(220));
 GotoXY(27,L-2); Write('OFF ¿');
 GotoXY(31,L-1); Write(Chr(31));
 GotoXY(34,L-2); Write('Batt ¿');
 GotoXY(39,L-1); Write(Chr(31));
 GotoXY(42,L-2); Write('ON ¿');
 GotoXY(45,L-1); Write(Chr(31));


 No := 4;
 Marker := 10;
 VoltOut := 7.1;
 SetVoltage(VoltOut);
 repeat
  Sc := ScanCode;
  CASE Sc OF
   Up   : begin
           if VoltOut < (VoltMax - 1E-3)  then
            begin
             VoltOut := VoltOut + VoltStepSize;
             SetVoltage(VoltOut);
            end
            ELSE
            Beep(1000,200);
          end;
   Down : begin
           if VoltOut > (VoltMin + 1E-3) then
            begin
             VoltOut := VoltOut - VoltStepSize;
             SetVoltage(VoltOut);
            end
            ELSE
            Beep(1000,200);
          end;
   Right,
   PgUp : begin
           if No < 5 then
            begin
             Inc(No);
             SetVoltage(VoltStep[No]);
             VoltOut := VoltStep[No];
            end
            ELSE
            Beep(1000,200);
          end;
   Left,
   PgDn : begin
           if No > 1 then
            begin
             Dec(No);
             SetVoltage(VoltStep[No]);
             VoltOut := VoltStep[No];
            end
            ELSE
            Beep(1000,200);
          end;
{$ifDEF RGBGEN}
   AltO:  begin
           TrmDA8591(I2CIOAddr,146,255,St);   {maxv‘rdi fra D/A-converter}
          end;
   AltN:  begin
           TrmDA8591(I2CIOAddr,146,0,St);     {minv‘rdi fra D/A-converter}
          end;
{$endif}

   end; {case}

  if VoltOut > VoltStep[5]
   then No := 6
   ELSE
  if VoltOut < VoltStep[1]
   then No := 0;

  until Sc = EndKey;
 SetVoltage(7.1);
 Delay(500);
 CloseWindow;
 CursorOn;
end;


Procedure EnterLevel;   { Tast A }
var L,S : byte;
begin
{$IFDEF LYSKASSE}
 write('Enter light level 0-255: '); Readln(L);
 TrmDA8591(I2CIOAddr,144,L,S);
{$endIF}
end;


Procedure TestRelayRGB;
const  D = 500;
begin
 EBURed;  delay(D);
 EBUGreen;delay(D);
 EBUBlue; delay(D);
 EBUBlack;delay(D);
 EBUWhite; delay(D);
 SMPTERed;   delay(D);
 SMPTEGreen; delay(D);
 SMPTEBlue;  delay(D);
 SMPTEBlack; delay(D);
 SMPTEWhite; delay(D);
 CRT525Lines; delay(D);
 CRT625Lines; delay(D);
 RGBGenRemote;delay(D);
 RGBGenLocal; delay(D);
end;
