(*
Include file: UTIL92.INC
Procedure ReadNoCRTFosfor
Procedure ReadxyValuesFromFile
Procedure ClearTestFlag
Procedure InitVar
Procedure InitScreen
Procedure WriteCRTNamesToEEPROM   {AltF8}
Procedure ReadCRTNames            {CtrlF3}
Procedure InitPtvEEPROM           {AltF3}
Procedure ShowTotalTest
Procedure ShowHelpWindow          {Tast H}
Procedure ShowActualXYValues      {CtrlF5}
Procedure WriteDefaultPreGain     {AltF7}
Procedure DAVoltage
Procedure ShutdownControl         {F3}
Procedure ShowTestStatus
Procedure SelectSensor
Procedure TestRelayRGB
*)

Procedure ReadNoCRTFosfor( Flag : BOOLEAN);
var Kode   : INTEGER;
    Svar   : STRING;
begin
 if Flag then
  write('Antal CRT-fosforer: ');
 COMStatus := writeCOMPort(COMNo,'MA53,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,NoCRTFosfor,Kode);
 if Kode = 0 then
  begin
   ReadNoCRTFosforOk := DefMaxNoCRTFosfor = NoCRTFosfor;
   if Flag then
    begin
     write(NoCRTFosfor:12);
     TrueOrFalse(ReadNoCRTFosforOk);
    end;
  end
 else
  begin
   if Flag then
    writeln('L‘se fejl!');
   ReadNoCRTFosforOk := FALSE;
   TestFailed := TRUE;
  end; {if}
 if not ReadNoCRTFosforOk then
  TestFailed := TRUE;
end; {ReadNoCRTFosfor}


Procedure ReadxyValuesFromFile;
var Svar       : STR_70;
    Kode       : INTEGER;
    s          : STR_1;
    xyStr,
    LumStr     : STR_10;
    xyVal      : array[1..4,1..2] OF REAL;
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
   AfslutProgram := true;
   Exit;
  end;

  xyError := false;
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
   xyError := true;

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
   xyError := true;

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
    xyError := true;
   end;


  XpR := xyVal[1,1];   YpR := xyVal[1,2];   ZpR := 1 - XpR - YpR;
  XpG := xyVal[2,1];   YpG := xyVal[2,2];   ZpG := 1 - XpG - YpG;
  XpB := xyVal[3,1];   YpB := xyVal[3,2];   ZpB := 1 - XpB - YpB;
 Xp65 := xyVal[4,1];  Yp65 := xyVal[4,2];  Zp65 := 1 - Xp65 - Yp65;

 for k := 1 TO 4 DO
  if (xyVal[k,1] + xyVal[k,2]) > 1.00 then
   begin
    xyError := true;
    Writeln('Fejl i xy-v‘rdier: ',xyVal[k,1]:10:4,xyVal[k,2]:10:4);
   end;

 if xyError then
  begin
   AfslutProgram := true;
   Writeln('Kontroller data i filen: ',XYFilename);
   Writeln('Program afbrudt  -  tryk en tast');
   WaitForAnyKey;
  end;
end;  {ReadxyValuesFromFile}



Procedure ClearTestFlag;
{Initialiserer variable mellem hver totaltest}
begin
 FillChar(MeasStr,SizeOf(MeasStr),0);
 TestFailed := false;
 RS232TestOk := false;
{$ifdef IEEEBUS}
 CurrentTestOk := false;
 AnalogPos5VTestOk := false;
 AnalogNeg5VTestOk := false;
 ADRefTestOk := false;
{$else}
 CurrentTestOk := true;
 AnalogPos5VTestOk := true;
 AnalogNeg5VTestOk := true;
 ADRefTestOk := true;
{$endif}
 DarkCurrentTestOk := false;
 BeregnGainFactorOk := false;
 ReadIdStringOk := false;
 ReadGainFactorOk := false;
 ReadCRTNameOk := false;
 ReadKorrMatrixOk := false;
 ReadCRTMatrixOk := false;
 ReadInternIDOk := false;
 ReadPreGainOk := false;
 ReadWriteProtectOk := false;
 ReadNoCRTFosforOk := false;
 ReadNoEEPROMPagesOk := false;
 LearnKMatrixOk := false;
 LearnOffsetOk := false;
 LearnEBUOk := false;
 LearnSMPTE_COk := false;
 FilterOk := false;
 GainSelOk := false;
 Analog5VTestValgt := false;
 ADRefTestValgt := false;
 TotalTestOk := false;
end;

Procedure InitVar;
begin
 ClearTestFlag;
 IICBusError := false;
 TotalTestFlag := false;
 AfslutProgram := false;
 IICPort64 := $FF;
 IICPort66 := $FF;
 FillChar(MeasStr,SizeOf(MeasStr),0);
 ReadxyValuesFromFile;
end;



Procedure WriteCRTNamesToEEPROM;  {AltF8}
var n : BYTE;
begin
 Writeln('CRT-navne skrevet til EýPROM');
 MXModePM5639;
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
  WriteEEPROM(1016 - (n * 20),Ord(NULL));   {CRT#3 -> #30}
end;


Procedure ReadCRTNames(Flag : BOOLEAN);     {CtrlF3}
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
  Writeln('CRT-navne for KU',ReadKU);
 ReadCRTNameOk := true;
 FillChar(CRTName,SizeOf(CrtName),0);
 NULLFound := false;
 k := 1;
 n := 1;
 Addr := CRTNameAddr;
 repeat
  Str(Addr,AddrStr);
  COMStatus := WriteCOMPort(COMNo,'MA'+ AddrStr + ','); Delay(10);
  COMStatus := ClearInputQue(COMNo);
   repeat
    COMStatus := WriteCOMPort(COMNo,'RM,'); Delay(10);
    COMStatus := ReadlnCOMPort(COMNo,Svar,1);
    Val(Svar,Value,Kode);
    if Value <> 0 then
     begin
      CRTName[n] := CRTName[n] + Chr(Value);
     end
    else
     NULLFound := true;
    Inc(k);
   until (COMStatus <> 0) OR (Svar = '000') OR (Kode <> 0) OR (k > 8);

 if Flag then
  if (n MOD 2) = 1 then
    begin
      Write(' #',n:2,': ',CRTName[n])
    end
   else
    begin
     GotoXY(40,WhereY);
      Write(' #',n:2,': ',CRTName[n]);
    end;

  if (NOT NULLFound) OR (COMStatus <> 0) then
   begin
    ReadCRTNameOk := false;
    TestFailed := true;
    LastAttr := TextAttr; TextColor(White);
    if Flag then
     Write('  NULL ikke fundet');
    TextAttr := LastAttr;
   end; {if}
  k := 1;
  Inc(n);
  Dec(Addr,20);
  NULLFound := false;
  if ((n MOD 2) = 1) AND Flag then
   Writeln;
 until (Addr <= (CRTNameAddr - (20 * DefMaxNoCRTFosfor))) OR (COMStatus <> 0);
end;





Procedure InitPtvEEPROM;     {AltF3}
begin
 Writeln('Data til EýPROM:');
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
   writeln('Husk: skrivebeskyttelsen skal v‘re OFF (g›res med Ctrl-F9)');
   exit;
  end;

 MXModePM5639;
 WriteEEPROM(52,DefNoEEPROMPages);
 WriteEEPROM(53,DefMaxNoCRTFosfor);
 WriteEEPROM(54,Lo(DefMaxWPAddr));
 WriteEEPROM(55,Hi(DefMaxWPAddr));
 case SensorVer of
  90  : begin
         case SensorSerie of
          3: begin
              WriteEEPROM(58,DefInternID[1,1]);    { /90 serie III}
              WriteEEPROM(59,DefInternID[1,2]);
              WriteEEPROM(60,DefInternID[1,3]);
              WriteEEPROM(61,DefInternID[1,4]);
             end;
          4: begin
              WriteEEPROM(58,DefInternID[2,1]);    { /90  serie IV}
              WriteEEPROM(59,DefInternID[2,2]);
              WriteEEPROM(60,DefInternID[2,3]);
              WriteEEPROM(61,DefInternID[2,4]);
             end;
          end; { case }
        WriteEEPROM(95,255);                 {output format flag}
       end;
  916 : begin
        WriteEEPROM(58,DefInternID[9,1]);    {default intern kode for /916}
        WriteEEPROM(59,DefInternID[9,2]);
        WriteEEPROM(60,DefInternID[9,3]);
        WriteEEPROM(61,DefInternID[9,4]);
        WriteEEPROM(95,0);                   {output format flag}
       end;
  93 : begin
        WriteEEPROM(58,DefInternID[8,1]);    {default intern kode for /93}
        WriteEEPROM(59,DefInternID[8,2]);
        WriteEEPROM(60,DefInternID[8,3]);
        WriteEEPROM(61,DefInternID[8,4]);
        WriteEEPROM(95,255);                 {output format flag}
       end;
  92  : begin
         case SensorSerie of
          1: begin
              WriteEEPROM(58,DefInternID[5,1]);    { /92 serie I }
              WriteEEPROM(59,DefInternID[5,2]);
              WriteEEPROM(60,DefInternID[5,3]);
              WriteEEPROM(61,DefInternID[5,4]);
             end;
          2: begin
              WriteEEPROM(58,DefInternID[6,1]);    { /92 serie II }
              WriteEEPROM(59,DefInternID[6,2]);
              WriteEEPROM(60,DefInternID[6,3]);
              WriteEEPROM(61,DefInternID[6,4]);
             end;
         end; { case }
        WriteEEPROM(95,255);                 {output format flag}
       end;
 end; {case}

 WriteEEPROM(96,TraceFlag);                  {cal traceability flag}
 WriteEEPROM(157,255);                       {kal status = FACTORY --> PM8550}
 Writeln(' Antal EýPROM sider: ',DefNoEEPROMPages);
 Writeln(' Antal CRT-fosforer: ',DefMaxNoCRTFosfor);
 Writeln(' H›jeste skrivebeskyttelse adresse: ',DefMaxWPAddr);
 Writeln(' Kal status = FACTORY ');
 Writeln(' Kal sporbar = NIST ');

  case SensorVer of
   90,92,93 : writeln(' Output format = R,G,B,I');
   916      : writeln(' Output format = R,G,B');
  end;

 Write(' Intern ID: ',DefInternID[1,1]:4,DefInternID[1,2]:4,DefInternID[1,3]:4);
 case SensorVer of
  90  : case SensorSerie of
         3 : writeln(DefInternID[1,4]:4);
         4 : writeln(DefInternID[2,4]:4);
        end;
  92  : case SensorSerie of
         1: writeln(DefInternID[5,4]:4);
         2: writeln(DefInternID[6,4]:4);
        end;
  93  : writeln(DefInternID[8,4]:4);
  916 : writeln(DefInternID[9,4]:4);
 end;

 WriteNOT_OKToEEPROM;
 PowerOffOn;
end;


Procedure ShowHelpWindow;         {Tast H}
begin
 CreateWindow(2,2,22,78,' Help ',Black+16*Cyan,Black+16*Cyan,Frame);
 Color(Black,Cyan);
 ClrScr;
 CursorOff;
 Writeln(' Test software no: ',TestSWNo,'   Rev: ',TestSWRev);
 Writeln('                                              A : Lysstyrke i lyskasse');
 Writeln('                                              ');
 Writeln(' Alt-F3    : Initialiser EýPROM               C : Slet PC-sk‘rm');
 Writeln(' Alt-F5    : Skriv default ID string          E : Learn XY CRT EBU');
 Writeln(' Alt-F6    : Skriv default gain factor        F : L‘s XY CRT matrix');
 Writeln(' Alt-F7    : Skriv default pre-gain factor    G : RGB m†ling (XY)');
 Writeln(' Alt-F8    : Skriv default CRT-navn           H : Hj‘lp');
 Writeln(' Alt-F9    : L‘s cal-data og gem p† LAN       L : T‘nd/sluk lampe');
 Writeln(' Alt-F10   : Print cal-data p† laserprinter   M : xy-m†ling');
 Writeln(' Ctrl-F1   : L‘s CRT-matrix                   ');
 Writeln(' Ctrl-F2   : L‘s K-matrix                     O : ST-m†ling');
 Writeln(' Ctrl-F3   : L‘s CRT-navn                     P : Power OFF - ON');
 Writeln(' Ctrl-F4   : Sensor skrivebeskyttet ?         R : Reference m†linger');
 Writeln(' Ctrl-F5   : Vis aktuel xy-koor og lum        Y : Skift rel‘');
 Writeln(' Ctrl-F6   : L‘s offset v‘rdier i EýPROM      W : SONY monitor=hvid');
 Writeln(' Ctrl-F8   : Set writeprotect = ON');
 Writeln(' Ctrl-F9   : Set writeprotect = OFF');
 if SensorVer = 93 then
  Writeln(' Ctrl-F10  : Baudrate skift test (4800/9600)');
 GotoXY(45,20);
 Write('Afslut hj‘lp . . . tryk en tast');
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
 MXModePM5639;
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
{$ifdef RGBGEN}
 Data := Trunc((Volt - VoltMin) / ((VoltMax - VoltMin) / 255));
 TrmDA8591(I2CIOAddr,146,Data,St);
{$endif}
end;


Procedure ShutdownControl;  {F3}
const L = 15;
var Marker,
    n,No,
    Sc       : byte;
    VoltOut  : real;

Procedure SetVoltage(Volt : REAL);
var  data : BYTE;
begin
 data := Trunc((Volt - VoltMin) / ((VoltMax - VoltMin) / 255));
{$ifdef RGBGEN}
 TrmDA8591(I2CIOAddr,146,data,St);
{$endif}
 GotoXY(Marker,L); Write(chr(220));
 Marker := 6 + (data DIV 4);
 GotoXY(Marker,L); Write(chr(219));
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
  case Sc of
   Up   : begin
           if VoltOut < (VoltMax - 1E-3)  then
            begin
             VoltOut := VoltOut + VoltStepSize;
             SetVoltage(VoltOut);
            end
            else
            Beep(1000,200);
          end;
   Down : begin
           if VoltOut > (VoltMin + 1E-3) then
            begin
             VoltOut := VoltOut - VoltStepSize;
             SetVoltage(VoltOut);
            end
            else
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
            else
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
            else
            Beep(1000,200);
          end;
{$ifdef RGBGEN}
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
   else
  if VoltOut < VoltStep[1]
   then No := 0;

  until Sc = EndKey;
 SetVoltage(7.1);
 Delay(500);
 CloseWindow;
 CursorOn;
end;



Procedure SelectSensor;
var k  : integer;
    v  : word;
begin
 SensorVer := 90;    { default }
 SensorSerie := 4;   { default }

 if ParamCount > 0 then
  begin
   Val(ParamStr(1),v,k);
    if k = 0 then
     SensorVer := v
    else
     SensorVer := 90;    { default }

   Val(ParamStr(2),v,k);
    if SensorVer = 90 then
     if (k = 0) and (v in[3,4]) then
       SensorSerie := v
      else
       SensorSerie := 4;

    if SensorVer = 92 then
     if (k = 0) and (v in[1,2]) then
       SensorSerie := v
      else
       SensorSerie := 2;

    if SensorVer = 93 then
       SensorSerie := 2;
  end;

 SensorVer := 93;    { default for /94 sensor}
 SensorSerie := 2;   { default }


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
