(*
Include file: UTIL92.inC
Procedure ReadNoCRTFosfor
Procedure ReadRefSensorxyYValues;
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

Procedure ReadNoCRTFosfor( Flag : boolean);
var Kode   : integer;
    Svar   : string;
begin
 if Flag then
  write('Number of CRT-fosfor layers: ');
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



Procedure ReadRefSensorxyYValues;
{ Read xyY values for ref sensor for EBU & SMPTE monitors.
  Data gemmes i xyrefsens[1..2,1..3]}
var Svar       : str_70;
    Kode       : integer;
    s          : str_1;
    xyStr,
    LumStr     : str_10;
    n,k        : byte;
    xyError    : boolean;
begin
 Assign(RefSensorFile,RefSensorFilename);
 {$I-}
 Reset(RefSensorFile);
 {$I+}
 if IOResult <> 0 then
  begin
   Writeln(Bell,'File "',RefSensorFilename,'" not found');
   Writeln('Program terminated - press a key');
   WaitForAnyKey;
   AfslutProgram := true;
   Exit;
  end;

 xyError := false;
 for k := 1 to 2 do
  begin
      n := 1;
      repeat
       Readln(RefSensorFile,Svar);
      until Eof(RefSensorFile) or (Length(Svar) > 5);
      xyStr := '';

      repeat
       s := Copy(Svar,n,1);
       Inc(n);
      until (s[1] in Tal) or (s[1] = '.') or (s[1] = '');

      repeat
       if (s[1] in Tal) or (s[1] = '.') then
        xyStr := Concat(xyStr,s);
       s := Copy(Svar,n,1);
       Inc(n);
      until not ((s[1] in Tal) or (s[1] = '.')) or (s = '');

      Val(xyStr,xyrefsens[k,1],Kode);  { x  for ref sensor (EBU-monitor)}
      if Kode <> 0 then
       xyError := true;

      xyStr := '';
      repeat
       s := Copy(Svar,n,1);
       Inc(n);
      until (s[1] in Tal) or (s[1] = '.') or (s[1] = '');

      repeat
       if (s[1] in Tal) or (s[1] = '.') then
        xyStr := Concat(xyStr,s);
       s := Copy(Svar,n,1);
       Inc(n);
      until not ((s[1] in Tal) or (s[1] = '.')) or (s = '');

      Val(xyStr,xyrefsens[k,2],Kode);  { y for ref sensor (EBU-monitor)}
      if Kode <> 0 then
       xyError := true;

     Readln(RefSensorFile,Svar);    { Y value = luminance (EBU-monitor)}
     LumStr := '';
     repeat
      s := Copy(Svar,n,1);
      Inc(n);
     until (s[1] in Tal) or (s[1] = '.') or (s[1] = '');
     repeat
      if (s[1] in Tal) or (s[1] = '.') then
       LumStr := Concat(LumStr,s);
      s := Copy(Svar,n,1);
      Inc(n);
     until NOT ((s[1] in Tal) or (s[1] = '.')) or (s = '');
     Val(LumStr,xyrefsens[k,3],Kode);
     if Kode <> 0 then
      xyError := true;
  end; {for}
 Close(RefSensorFile);

if xyError then
  begin
   Writeln(Bell,'Data failure in file "',RefSensorFilename,'"');
   Writeln('Program terminated - press a key');
   WaitForAnyKey;
   AfslutProgram := true;
   Exit;
  end;

end; { ReadRefSensorxyYValues }





Procedure ReadxyValuesFromFile(XYFilename : string);
var Svar       : str_70;
    Kode       : integer;
    s          : str_1;
    xyStr,
    LumStr     : str_10;
    xyVal      : array[1..4,1..2] of real; { xyY values for ref ref monitor }
    n,k        : byte;
    xyError    : boolean;
begin
 FillChar(xyVal,SizeOf(xyVal),0);
 Assign(XYFile,XYFilename);
 {$I-}
 Reset(XYFile);
 {$I+}
 if IOResult <> 0 then
  begin
   Writeln(Bell,XYFilename,' not found');
   Writeln('Program terminated - press a key');
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
  until Eof(XYFile) or (Length(Svar) > 5);
  xyStr := '';

  repeat
   s := Copy(Svar,n,1);
   Inc(n);
  until (s[1] in Tal) or (s[1] = '.') or (s[1] = '');

  repeat
   if (s[1] in Tal) or (s[1] = '.') then
    xyStr := Concat(xyStr,s);
   s := Copy(Svar,n,1);
   Inc(n);
  until not ((s[1] in Tal) or (s[1] = '.')) or (s = '');

  Val(xyStr,xyVal[k,1],Kode);
  if Kode <> 0 then
   xyError := true;

  xyStr := '';
  repeat
   s := Copy(Svar,n,1);
   Inc(n);
  until (s[1] in Tal) or (s[1] = '.') or (s[1] = '');
  repeat
   if (s[1] in Tal) or (s[1] = '.') then
    xyStr := Concat(xyStr,s);
   s := Copy(Svar,n,1);
   Inc(n);
  until not ((s[1] in Tal) or (s[1] = '.')) or (s = '');

  Val(xyStr,xyVal[k,2],Kode);
  if Kode <> 0 then
   xyError := true;

  Inc(k);
 until Eof(XYFile) or (k > 4) or xyError;

 Readln(XYFile,Svar);    { Y value = luminance }
 LumStr := '';
 repeat
  s := Copy(Svar,n,1);
  Inc(n);
 until (s[1] in Tal) or (s[1] = '.') or (s[1] = '');
 repeat
  if (s[1] in Tal) or (s[1] = '.') then
   LumStr := Concat(LumStr,s);
  s := Copy(Svar,n,1);
  Inc(n);
 until NOT ((s[1] in Tal) or (s[1] = '.')) or (s = '');
 Val(LumStr,NominelLum,Kode);

 Close(XYFile);

  if (Kode <> 0) or (NominelLum < 0) or (NominelLum > MaxLuminans) then
   begin
    Writeln('Fail in luminance value: ',NominelLum:6:1,' cd/mý');
    xyError := true;
   end;

 {overf›r xy koordinater til globale variabler }
  XpR := xyVal[1,1];   YpR := xyVal[1,2];   ZpR := 1 - XpR - YpR;
  XpG := xyVal[2,1];   YpG := xyVal[2,2];   ZpG := 1 - XpG - YpG;
  XpB := xyVal[3,1];   YpB := xyVal[3,2];   ZpB := 1 - XpB - YpB;
 Xp65 := xyVal[4,1];  Yp65 := xyVal[4,2];  Zp65 := 1 - Xp65 - Yp65;

 for k := 1 to 4 do
  if (xyVal[k,1] + xyVal[k,2]) > 1.00 then
   begin
    xyError := true;
    Writeln('Fail in xy values: ',xyVal[k,1]:10:4,xyVal[k,2]:10:4);
   end;

 if xyError then
  begin
   AfslutProgram := true;
   Writeln('Check data in file: ',XYFilename);
   Writeln('Program terminated - press a key');
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
 MakeSensorCalibrationOk := false;
 LearnOffsetOk := false;
 LearnEBUPhosphorOk := false;
 LearnSMPTEPhosphorOk := false;
 FilterOk := false;
 GainSelOk := false;
 Analog5VTestValgt := false;
 ADRefTestValgt := false;
 TotalTestOk := false;
end;

Procedure InitVar(MonType : byte);
begin
 ClearTestFlag;
 IICBusError := false;
 TotalTestFlag := false;
 AfslutProgram := false;
 IICPort64 := $FF;
 IICPort66 := $FF;
 FillChar(MeasStr,SizeOf(MeasStr),0);
 ReadRefSensorxyYValues;
 if MonType = MonEBU then
   ReadxyValuesFromFile(EBU_XYFilename)
  else
   ReadxyValuesFromFile(SMPTE_XYFilename);
end;



Procedure WriteCRTNamesToEEPROM;  {AltF8}
var n : byte;
begin
 Writeln('CRT-names written to EýPROM');
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
 for n := 2 to 29 do
  WriteEEPROM(1016 - (n * 20),Ord(NULL));   {CRT#3 -> #30}
end;


Procedure ReadCRTNames(Flag : boolean);     {CtrlF3}
{L‘ser CRT-navne fra EýPROM, gemmer navnene i CertifData.Name,
 og viser dem p† sk‘rmen}
var n,
    k       : byte;       {antal tegn i hvert navn}
    Kode    : integer;
    Value   : byte;
    Addr    : word;
    AddrStr : STR_5;
    Svar    : string;
    NULLFound : boolean;
begin
 if Flag then
  Writeln('CRT-names for KU',ReadKU);
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
   until (COMStatus <> 0) or (Svar = '000') or (Kode <> 0) or (k > 8);

 if Flag then
  if (n mod 2) = 1 then
    begin
      Write(' #',n:2,': ',CRTName[n])
    end
   else
    begin
     GotoXY(40,WhereY);
      Write(' #',n:2,': ',CRTName[n]);
    end;

  if (NOT NULLFound) or (COMStatus <> 0) then
   begin
    ReadCRTNameOk := false;
    TestFailed := true;
    LastAttr := TextAttr; TextColor(White);
    if Flag then
     Write('  NULL not found');
    TextAttr := LastAttr;
   end; {if}
  k := 1;
  Inc(n);
  Dec(Addr,20);
  NULLFound := false;
  if ((n mod 2) = 1) and Flag then
   Writeln;
 until (Addr <= (CRTNameAddr - (20 * DefMaxNoCRTFosfor))) or (COMStatus <> 0);
end;





Procedure InitPtvEEPROM;     {AltF3}
begin
 Writeln('Data to EýPROM:');
 ReadInternID;
 if (not ReadInternIDOk ) and
   ((InternIDRead[1] <> 255) or
    (InternIDRead[2] <> 255) or
    (InternIDRead[3] <> 255) or
    (InternIDRead[4] <> 255)) then
  begin
   TestFailed := true;
   write(bell);
   writeln('Internal ID is not correct.  Check that correct test program is being used for');
   writeln('the color sensor.  If the print plate is to be used in other color sensor types,');
   writeln('the intern ID can be changed by COM5639.EXE program. Addresses 58-61 shall all be');
   writeln('changed to 255 as follows:  MA58,SM255,SM255,SM255,SM255,');
   writeln('Remember: write-protection must be OFF (done with Ctrl-F9)');
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
        4,5: begin
              WriteEEPROM(58,DefInternID[2,1]);    { /90  serie IV + V}
              WriteEEPROM(59,DefInternID[2,2]);
              WriteEEPROM(60,DefInternID[2,3]);
              WriteEEPROM(61,DefInternID[2,4]);
             end;
          end; { case }
         WriteEEPROM(95,255);                      {output format flag}
        end;
  92  : begin
         case SensorSerie of
          1: begin
              WriteEEPROM(58,DefInternID[5,1]);    { /92 serie I }
              WriteEEPROM(59,DefInternID[5,2]);
              WriteEEPROM(60,DefInternID[5,3]);
              WriteEEPROM(61,DefInternID[5,4]);
             end;
        2,3: begin
              WriteEEPROM(58,DefInternID[6,1]);    { /92 serie II + III }
              WriteEEPROM(59,DefInternID[6,2]);
              WriteEEPROM(60,DefInternID[6,3]);
              WriteEEPROM(61,DefInternID[6,4]);
             end;
         end; { case }
         WriteEEPROM(95,255);                 {output format flag}
        end;
  93 : begin
        WriteEEPROM(58,DefInternID[9,1]);    {default intern kode for /93}
        WriteEEPROM(59,DefInternID[9,2]);
        WriteEEPROM(60,DefInternID[9,3]);
        WriteEEPROM(61,DefInternID[9,4]);
        WriteEEPROM(95,255);                 {output format flag}
       end;
  916 : begin
        WriteEEPROM(58,DefInternID[13,1]);    {default intern kode for /916}
        WriteEEPROM(59,DefInternID[13,2]);
        WriteEEPROM(60,DefInternID[13,3]);
        WriteEEPROM(61,DefInternID[13,4]);
        WriteEEPROM(95,0);                   {output format flag}
        end;
 end; {case SensorVer of}

 WriteEEPROM(96,TraceFlag);                  {cal traceability flag}
 WriteEEPROM(157,255);                       {kal status = FACTORY --> PM8550}
 Writeln(' Number of EýPROM sides: ',DefNoEEPROMPages);
 Writeln(' Number of CRT-fosfor layers: ',DefMaxNoCRTFosfor);
 Writeln(' Highiest write-protection address: ',DefMaxWPAddr);
 Writeln(' Calibration status = FACTORY ');
 Writeln(' Calibration trackability = NIST ');

  case SensorVer of
   90,92,93 : writeln(' Output format = R,G,B,I');
   916      : writeln(' Output format = R,G,B');
  end;

 Write(' Internal ID: ',DefInternID[1,1]:4,DefInternID[1,2]:4,DefInternID[1,3]:4);
 case SensorVer of
  90  : case SensorSerie of
         3 : writeln(DefInternID[1,4]:4);
       4,5 : writeln(DefInternID[2,4]:4);
        end;
  92  : case SensorSerie of
         1: writeln(DefInternID[5,4]:4);
       2,3: writeln(DefInternID[6,4]:4);
        end;
  93  : writeln(DefInternID[9,4]:4);
  916 : writeln(DefInternID[13,4]:4);
 end;

 {WriteNOT_OKToEEPROM;}
 PowerOffOn;
end;


Procedure ShowHelpWindow;         {Tast H}
begin
 CreateWindow(2,2,22,78,' Help  ',Black+16*Cyan,Black+16*Cyan,Frame);
 Color(Black,Cyan);
 ClrScr;
 CursorOff;
 Writeln(' Test software nr: ',TestSWNo,'   Rev: ',TestSWRev);
 writeln;
 Writeln(' Alt-F1    : Write company name = PHILIPS');
 Writeln(' Alt-F3    : Initialise  EýPROM               A : Light intens. in box');
 Writeln(' Alt-F5    : Write default ID string          C : Clear PC-screen');
 Writeln(' Alt-F6    : Write default gain factor        G : RGB measurement (XY)');
 Writeln(' Alt-F7    : Write default pre-gain factor    H : Help');
 Writeln(' Alt-F8    : Write default CRT-name           L : Lamp ON/OFF');
 Writeln(' Alt-F9    : Read cal-data and save on disc  ');
 Writeln(' Alt-F10   : Print cal-data on laserprinter   M : xyY-measurement');
 Writeln(' Ctrl-F1   : Read CRT-matrix                  ');
 Writeln(' Ctrl-F2   : Read K-matrix                    O : ST-measurement');
 Writeln(' Ctrl-F3   : Read CRT-navn                    P : Power OFF - ON');
 Writeln(' Ctrl-F4   : Sensor write-protected ?         ');
 Writeln(' Ctrl-F5   : Show actual xy-koor and lum      R : Reference measurements');
 Writeln(' Ctrl-F6   : Read offset values in EýPROM     ');
 Writeln(' Ctrl-F8   : Set writeprotect = ON            Y : Change relay');
 Writeln(' Ctrl-F9   : Set writeprotect = OFF           W : SONY monitor=white');
 if SensorVer = 93 then
  Writeln(' Ctrl-F10  : Baudrate change test (4800/9600)');
 GotoXY(45,20);
 Write('Exit help . . . press a key');
 EmptyKeyboardBuffer;
 WaitForAnyKey;
 CursorOn;
 CloseWindow;
end;



Procedure ShowActualXYValues(XYFileName : string);   {CtrlF5}
begin
 Writeln('Data from file: "',XYFileName,'":');
 Writeln('  Red:   x= ',XpR:5:3,'   y= ',YpR:5:3);
 Writeln('  Green: x= ',XpG:5:3,'   y= ', YpG:5:3);
 Writeln('  Blue:  x= ',XpB:5:3,'   y= ', YpB:5:3);
 Writeln('  White: x= ',Xp65:5:3,'  y= ', Yp65:5:3);
 Writeln('  Luminance: ',NominelLum:5:1,' cd/mý');
end;

Procedure WriteDefaultPreGain;  {AltF7}
begin
 Write('Default pre-gain factors written to EýPROM');
 Writeln(' address ',PreGainAddr,' - ',PreGainAddr + 5);
 MXModePM5639;
 WriteEEPROM(PreGainAddr    ,Hi(Trunc((DefaultPreGain[1]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 1,Lo(Trunc((DefaultPreGain[1]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 2,Hi(Trunc((DefaultPreGain[2]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 3,Lo(Trunc((DefaultPreGain[2]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 4,Hi(Trunc((DefaultPreGain[3]*PreGainMulFactor))));
 WriteEEPROM(PreGainAddr + 5,Lo(Trunc((DefaultPreGain[3]*PreGainMulFactor))));
 {WriteNOT_OKToEEPROM;}
end;

Procedure DAVoltage(Volt : REAL);
var Data : byte;
begin
{$ifdef RGBGEN}
 Data := Trunc((Volt - VoltMin) / ((VoltMax - VoltMin) / 255));
 TrmDA8591(I2CIOAddr,146,Data,St);
{$endif}
end;

(*
Procedure ShutdownControl;  {F3}
const L = 15;
var Marker,
    n,No,
    Sc       : byte;
    VoltOut  : real;

Procedure SetVoltage(Volt : REAL);
var  data : byte;
begin
 data := Trunc((Volt - VoltMin) / ((VoltMax - VoltMin) / 255));
{$ifdef RGBGEN}
 TrmDA8591(I2CIOAddr,146,data,St);
{$endif}
 GotoXY(Marker,L); Write(chr(220));
 Marker := 6 + (data div 4);
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
 Writeln(' Connect battery terminals in display unit to control voltage');
 Writeln;
 Writeln(' Arrow ',Chr(30),' ',Chr(31),':  Continous adjustment up/down');
 Writeln(' Arrow ',Chr(17),' ',Chr(16),':  Step adjustment up/down');
 GotoXY(5,L+1); Write(VoltMin:3:1,' V');
 GotoXY(68,L+1); Write(VoltMax:3:1,' V');
 GotoXY(2,20); Write(' Exit - Press End');
 GotoXY(6,L);
 for n := 1 to 64 do
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
*)


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
     if (k = 0) and (v in[3,4,5]) then
       SensorSerie := v
      else
       SensorSerie := 4;

    if SensorVer = 92 then
     if (k = 0) and (v in[1,2,3]) then
       SensorSerie := v
      else
       SensorSerie := 2;

    if SensorVer = 93 then
     if (k = 0) and (v in[2,3]) then
       SensorSerie := v
      else
       SensorSerie := 3;
  end;
end;


Procedure TestRelayRGB;
CONST  D = 500;
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

