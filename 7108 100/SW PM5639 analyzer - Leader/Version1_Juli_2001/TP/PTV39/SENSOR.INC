(*
 SENSOR.INC:
 Denne fil indeholder rutiner som er f‘lles for PESA/VISTEK color probes
 og PM5639/90serie II color sensor.
 Se desuden filen TPTEST.INC

 SENSOR.INC indeholder f›lgende procedurer/functioner:
 InitIEEEBus
 InitI2CBus
 InitPrinter;
 TestModePM5639;
 LEDOn;
 LEDOff;
 FreqHigh;
 FreqLow;
 LampOn;
 LampOff;
 RelayOn;
 RelayOff;
 PowerOffOn;
 RGBGenRemote;
 RGBGenLocal;
 ToggleRelay;
 ToggleLED;
 ToggleFreq;
 WriteEEPROM;
 ReadFieldRate;          {F5}
 TrueOrFalse;
 ReadKalStatus;          {F4}
 ReadIdString;           {F4}
 WriteOKToEEPROM;        {AltF1}
 Writenot_OKToEEPROM;    {AltF2}
 ReadSWRev;              {F4}
 ReadKU_Nummer;          {F4}
 Read12NC_Nummer;
 ReadEEPROMWriteProtect; {F4}
 ReadNoEEPROMPages;
 TestAfbrudt : BOOLEAN;
 WriteDefaultGainFactor; {AltF6}
 FieldRateTest;          {F5}
 ShowOffsetValues;       {CtrlF6}
 LearnOffset;            {AltF4}
 BeregnGainFactor;       {F7}
 ReadGainFactor;         {F4}
 RS232Test;              {F2}
 DarkCurrentTest;        {F6}
 EnterKU_Nummer;         {F2}
 WriteIdString;          {AltF5}
 WriteProtectYesOrNo;    {CtrlF4}
*)


Procedure InitIEEEBus;
{Initialiserer IEEE-interface. Checker om PM2534 er i kontakt med IEEE-
 interface p† adresse 22. Hvis der ikke er kontakt, s‘ttes flaget
 AfslutProgram = TRUE}
begin
 if not IeeeFilesOpen then
  OpenIeeeFiles;
  Writeln(IeOut,'RESET');
  Writeln(IeOut,'ERROR OFF');
  Writeln(IeOut,'TIME OUT 2');
  Writeln(IeOut,'CONFIG /B30');
  Writeln(IeOut,'CONFIG /N PM2534 22');
 {$I-}
  Writeln(IeOut,'CLEAR  PM2534');                       {DMM}
  Writeln(IeOut,'OUTPUT  PM2534; FNC VDC');
  Writeln(IeOut,'LOCAL PM2534');                        {DMM=local}
 {$I+}
  if IOResult > 0 then
   begin
    CreateWindow(20,10,4,40,' IEEE-bus error ',Green,Green,Frame);
    CursorOff;
    Writeln(Bell,' Ingen kontakt med PM2534 p† IEEE-bus');
    Write(' Program afbrudt  -  tryk en tast');
    WaitForAnyKey; CloseWindow;
    CursorOn;
    AfslutProgram := TRUE;
   end
end;


Procedure InitI2CBus;
{Initialiserer IýC-interface. Checker om lyskasse og RGB-generator er i
 kontakt med IýC-interface p† henholdsvis adresse 64 og 146. Hvis der
 ikke er kontakt, s‘ttes flaget AfslutProgram = TRUE}

begin
 InitIIC(I2CIOAddr,St);

{$ifDEF RGBGEN}
 Trm8574(I2CIOAddr,66,$FF,St);       {Addr findes i RGB-generator}
  if (St AND $08) = $08 then
   begin
    CreateWindow(12,10,4,56,' IýC-bus error ',Green,Green,Frame);
    CursorOff; TextColor(White);
    Writeln(Bell,' Ingen kontakt med RGB-generator p† IýC-bus (',St,')');
    Write(' Program afbrudt  -  tryk en tast');
    WaitForAnyKey; CloseWindow; CursorOn;
    AfslutProgram := TRUE;
    IICBusError := TRUE;
   end;
{$endif}

{$ifDEF LYSKASSE}
 Trm8574(I2CIOAddr,64,$FF,St);
  if (St AND $08) = $08 then
   begin
    CreateWindow(15,10,4,50,' IýC-bus error ',Green,Green,Frame);
    CursorOff; TextColor(White);
    Writeln(Bell,' Ingen kontakt med lyskasse p† IýC-bus (',St,')');
    Write(' Program afbrudt  -  tryk en tast');
    WaitForAnyKey; CloseWindow;
    CursorOn;
    AfslutProgram := TRUE;
    IICBusError := TRUE;
   end;

  if not IICBusError then
   TrmDA8591(I2CIOAddr,144,50,St);   {lysstyrke i lampe}
{$endif}

end;


Procedure InitPC_COM1Port;
{Initialiserer seriel-COM1-port.}
begin
 if not COM1Open then
  COM1Status := OpenCOMPort(_COM1,InQueSize,OutQueSize);
 COM1Status := InitCOMPort(_COM1,_Baud4800,_Databit8,_Stopbit2,_None,_CTSNotRequired);
end;


Procedure InitPrinter;
{Returnerer TRUE hvis printeren er klar, og FALSE hvis den ikke er klar
 Printeren skal v‘re tilsluttet LPT1}
var  Status  : BYTE;
begin
 PrinterFlag := PrinterReady(1,Status);
end;

Procedure TestModePM5639;
begin
 COM1Status := WriteCOMPort(_COM1,'MX,');
end;

Procedure MeasModePM5639;
begin
 COM1Status := WriteCOMPort(_COM1,'MS,');
 Delay(10);
end;

Procedure LEDOn;
begin
 IICPort64 := IICPort64 AND $FD;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;
Procedure LEDOff;
begin
 IICPort64 := IICPort64 OR $02;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;
Procedure FreqHigh;
begin
 IICPort64 := IICPort64 AND $FB;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;
Procedure FreqLow;
begin
 IICPort64 := IICPort64 OR $04;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;

Procedure LampOff;
begin
 IICPort64 := IICPort64 OR $01;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;

Procedure LampOn;
begin
 IICPort64 := IICPort64 AND $FE;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;

Procedure RelayOn;
begin
 IICPort64 := IICPort64 AND $EF;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;

Procedure RelayOff;
begin
 IICPort64 := IICPort64 OR $10;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(100);
end;

Procedure PowerOffOn;        {TastP}
var Svar : STRING;
begin
 Writeln('Power OFF - ON');
 IICPort64 := IICPort64 AND $DF;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(1000);
 IICPort64 := IICPort64 OR $20;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(200);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'F?,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,2);
 Delay(10);
 COM1Status := ClearInputQue(_COM1);
end;

{$I RGB72801.INC}


Procedure ToggleRelay;       {TastR}
begin
 IICPort64 := IICPort64 XOR $10;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 if (IICPort64 AND $10) = $10 then
   Writeln('Rel‘: Probe')
  else
   Writeln('Rel‘: Str›mm†ling');
end;
Procedure ToggleLED;         {TastL}
begin
 IICPort64 := IICPort64 XOR $02;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 if (IICPort64 AND $02) = $02 then
   Writeln('LED: OFF')
  else
   Writeln('LED: ON');
end;
Procedure ToggleLamp;
begin
 IICPort64 := IICPort64 XOR $01;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 if (IICPort64 AND $01) = $01 then
   Writeln('Lampe: OFF')
  else
   Writeln('Lampe: ON');
end;
Procedure ToggleFreq;
begin
 IICPort64 := IICPort64 XOR $04;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 if (IICPort64 AND $04) = $04 then
   Writeln('Freq: 40 Hz')
  else
   Writeln('Freq: 120 Hz');
end;




Procedure WriteEEPROM(Addr : WORD; Data : BYTE);
{Skriver en byte til adressen Addr. PM5639 skal v‘re i Testmode (MX-mode)}
var s  : STR_4;
begin
 Str(Addr,s);
 COM1Status := WriteCOMPort(_COM1,'MA' + s + ',');
 Delay(10);
 Str(Data,s);
 COM1Status := WriteCOMPort(_COM1,'SM' + s + ',');
 Delay(EEPROMWriteDelay);
end;

FUNCTION ReadFieldRate : STR_10;
{Returnerer fieldrate som en streng}
var Svar : STRING;
begin
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'F?,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,3);
 Delay(10);
 ReadFieldRate := Svar;
end;



Procedure TrueOrFalse( Bool : BOOLEAN);
var  OldAttr : BYTE;
begin
 OldAttr := TextAttr;
 GotoXY(74,WhereY);
 if Bool then
  Writeln(' OK')
 else
  begin
   TextColor(Red);
   Writeln(' FAIL');
   TextAttr := OldAttr;
  end;
end;

Procedure ShowTestFlagStatus(Message : STR_30; Bool : BOOLEAN; XPos : BYTE);
var  OldAttr : BYTE;
begin
 OldAttr := TextAttr;
 Write(Message);
 GotoXY(XPos,WhereY);
 if Bool then
  Write(' OK')
 else
  begin
   TextColor(Red);
   Write(' FAIL');
   TextAttr := OldAttr;
  end;
 GotoXY(1,WhereY+1);
end;

FUNCTION ReadKalStatus : STR_10;         {F4}
var  Svar  : STRING;
     k     : INTEGER;
     v     : BYTE;
begin
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'MA157,RM,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 if COM1Status = 0 then
  begin
   Val(Svar,v,k);
   if v <> 0 then
     ReadKalStatus := 'FACTORY'
    else
     ReadKalStatus := 'USER';
  end
  else
   ReadKalStatus := '';
end;



Procedure ReadIdString;         {F4}
var  Svar  : STRING;
begin
 ReadIdStringOk := FALSE;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'I?,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 SWRevStr := Copy(Svar,31,4);
 if (Length(Svar) = 34) AND
    (Pos(Copy(Svar,1,23),IdStr[ProbeNo]) = 1) AND
    ((SWRevStr = MeasHeadSWRev[1]) OR (SWRevStr = MeasHeadSWRev[2])) then
    ReadIdStringOk := TRUE;
 Write('ID: ',Svar);
 TrueOrFalse(ReadIdStringOk);
 if not ReadIdStringOk then
  begin
   Writeln('  Korrekt m†lehoved software rev: ',MeasHeadSWRev[1]);
   Writeln('                           eller: ',MeasHeadSWRev[2]);
   Writeln('  Korrekt ID-string: ',Copy(IdStr[ProbeNo],1,23),'xxxxxx');
  end;
end;



Procedure WriteOKToEEPROM;
begin
 Writeln('"OK" til EýPROM');
 TestModePM5639;
 WriteEEPROM(56,Ord('O'));
 WriteEEPROM(57,Ord('K'));
end;

Procedure Writenot_OKToEEPROM;
begin
 Writeln('"not OK" til EýPROM');
 TestModePM5639;
 WriteEEPROM(56,Ord('-'));
 WriteEEPROM(57,Ord('-'));
end;


FUNCTION ReadKU_Nummer : STRING;
{L‘ser KU-nummer fra EýPROM}
var  Svar : STRING;
begin
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'I?,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 Delay(100);
 ReadKU_Nummer := Copy(Svar,24,6);
end;

FUNCTION ReadSWRev : STRING;
{L‘ser SWRev fra EýPROM}
var  Svar : STRING;
begin
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'I?,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 Delay(100);
 ReadSWRev := Copy(Svar,31,4);
end;

FUNCTION Read12NC_Nummer : STRING;
{L‘ser 12NC-nummer fra EýPROM}
var  Svar : STRING;
begin
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'I?,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 Delay(100);
 Read12NC_Nummer := Copy(Svar,9,12);
end;




Procedure  ReadEEPROMWriteProtect(Flag : BOOLEAN);
var Svar   : STRING;
    DLow,
    DHigh  : BYTE;
    k      : INTEGER;
begin
 Write('Skrive-beskyttelses adresse: ');
 COM1Status := WriteCOMPort(_COM1,'MA54,'); Delay(5);
 Delay(10);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(5);
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,DLow,k);
 ReadWriteProtectOk := k = 0;
 COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(5);
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
 Val(Svar,DHigh,k);
{$R-}
 WriteProtectAddr := DHigh * 256 + DLow;
{$R+}
 ReadWriteProtectOk := ReadWriteProtectOk AND
                       (WriteProtectAddr > 2) AND
                       (k = 0);
 Write(WriteProtectAddr);
 TrueOrFalse(ReadWriteProtectOk);
end;



Procedure  ReadNoEEPROMPages;
var Kode   : INTEGER;
    Svar   : STRING;
begin
 Svar := '';
 Write('Antal EýPROM sider: ');
 COM1Status := WriteCOMPort(_COM1,'MA52,');
 Delay(10);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 Val(Svar,NoEEPROMPages,Kode);
 if Kode = 0 then
  begin
   ReadNoEEPROMPagesOk := DefNoEEPROMPages = NoEEPROMPages;
   Write(NoEEPROMPages);
   TrueOrFalse(ReadNoEEPROMPagesOk);
  end
 else
  begin
   Writeln('L‘se fejl !!');
   ReadNoEEPROMPagesOk := FALSE;
   TestFailed := TRUE;
  end; {if}
 if not ReadNoEEPROMPagesOk then
  TestFailed := TRUE;
end;


FUNCTION TestAfbrudt : BOOLEAN;
{ Afbryder totaltest hvis der tastes p† SPACE }
begin
 if KeyPressed then
  TestAfbrudt := ReadKey = SPACE
 else
  TestAfbrudt := FALSE;
 EmptyKeyboardBuffer;
end;

Procedure WriteDefaultGainFactor;
{Skriver default gainfaktorer til EýPROM afh‘ngig af SW-revision}
var n : BYTE;
begin
 Write('Default gain faktorer til EýPROM  (SW=');
 TestModePM5639;
 SWRevStr := ReadSWRev;
 Writeln(SWRevStr,')');
 if SWRevStr = MeasHeadSWRev[1] then
    for n := 1 TO 10 DO
     WriteEEPROM(n + 1,DefaultGainFactor[1,n])
  else
    for n := 1 TO 10 DO
     WriteEEPROM(n + 1,DefaultGainFactor[2,n]);
end;

Procedure FieldRateTest;         {F5}
{Test af fieldrate. M†lehovedet er placeret p† lyskassen.
 Der kontrolleres ved 2 frekvenser: 40Hz og 120Hz.
 Frekvenserne justeres med potmeterne R25 og R26 i lyskassen.
 Lysstyrken fra lysdioden i lyskassen skal v‘re justeret med potmeter R24
 i lyskassen.
 Er f›rste tegn i svaret et "-", er fieldratedetektoren ul†st}
var Status    : BYTE;
    Rate1_Ok,
    Rate2_Ok  : BOOLEAN;
    RateStr   : STR_10;
begin
 Write('Fieldrate 40Hz:   ');
 LampOff;
 LEDOn;
 FreqLow;
 Delay(3000);
 RateStr := ReadFieldRate;
 Rate1_Ok := ((Pos('-',RateStr)) = 0) AND (Length(RateStr) > 0);
 Write(RateStr);
 TrueOrFalse(Rate1_Ok);
 Write('Fieldrate 120Hz:  ');
 FreqHigh;
 Delay(2000);
 RateStr := ReadFieldrate;
 Rate2_Ok := ((Pos('-',RateStr)) = 0) AND (Length(RateStr) > 0);
 Write(RateStr);
 TrueOrFalse(Rate2_Ok);
 LEDOff;
 FieldRateTestOk := Rate1_Ok AND Rate2_Ok;

 if not FieldRateTestOk then
  TestFailed := TRUE;
end;


FUNCTION FinalTestOk : BOOLEAN;
{Returnerer TRUE hvis "OK" er skrevet i adresse 56 og 57
 i EýPROM - ellers returneres FALSE}
var Svar   : STRING;
    D1,D2  : STR_5;
begin
 COM1Status := WriteCOMPort(_COM1,'MA56,');  Delay(5);
 Delay(10);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');    Delay(5);
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);  Delay(5);
 D1 := Svar;
 COM1Status := WriteCOMPort(_COM1,'RM,');    Delay(5);
 Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);  Delay(5);
 D2 := Svar;
 FinalTestOk := (D1 = '079') AND (D2 = '075') AND (COM1Status = 0);
end;

Procedure ShowOffsetValues(Flag : BOOLEAN);    {CtrlF6}
{viser offset-v‘rdierne for gain 0-4 for RGB-kanalerne (kanal #4 vises ikke).
 Resultatet vises i mV ved en AD-reference = ADRef, som svarer til ADCounts.
 L‘ses fra adresse 12 - 51.
 Se definitioner i CONST
 Hvis Flag = FALSE vises intet p† sk‘rmen}
var
 Instr,
 Svar        : STRING;
 Data,
 n,k         : BYTE;
 Kode,
 Os          : INTEGER;
 OffsetValue : REAL;
begin
 FillChar(Offset,SizeOf(Offset),0);
 n := 1;
 Svar[0] := #40;
 COM1Status := WriteCOMPort(_COM1,'MS,');   Delay(10);
 COM1Status := WriteCOMPort(_COM1,'MA12,'); Delay(10);
 COM1Status := ClearInputQue(_COM1);
 repeat
  COM1Status := WriteCOMPort(_COM1,'RM,');  Delay(10);
  COM1Status := ReadlnCOMPort(_COM1,InStr,1); Delay(10);
  Val(Instr,Data,Kode);
  Svar[n] := Chr(Data);
  Inc(n);
 until (n > 40) OR (COM1Status <> 0) OR (Kode <> 0);
 Svar[0] := Chr(n);

 k := 0;
 if COM1Status <> 0 then
   Writeln(Bell,'Timeout - intet svar fra m†lehoved !')
  else
 begin
 if Flag then
  Writeln('Adresse 12-51:');
  Writeln('             R›d      Gr›n     Bl†     ');
  repeat
  if Flag then
   Write('Gain ',k,': ');
   for n := 0 TO 5 DO
    begin
 {$R-}
     Os := Ord(Svar[k*8+n+1]) + 256 * Ord(Svar[k*8+n+2]);
     if Os < 0 then
      Os := -1 * (32768 + Os);
     {OffSet[k,n DIV 2] := Os;}
 {$R+}
     OffsetValue := Os * 1E3 * ADRef / ADCounts;    {til mV}
     OffSet[k,n DIV 2] := OffsetValue;
     if Flag then
      Write(OffsetValue:9:2);
     Inc(n);
    end; {for}
   if Flag then
    Writeln('   mV');
  Inc(k);                  {offset value for channel 4 not used}
  until k > 4;
 end; {if}
end;  {ShowOffsetValues}


Procedure LearnOffset;     {AltF4}
var Svar   : STRING;
    n      : BYTE;
begin
 n := 0;
 LampOff;
 LEDOff;
 Beep(1000,200);
 Writeln('Offset kalibrering:');
 Write('Placer m†lehoved med FASTSKRUET overpart p† et m›rkt underlag - tryk en tast');
 WaitForAnyKey;
 Writeln;
 LearnOffsetOk := TRUE;
 COM1Status := WriteCOMPort(_COM1,'MS,');
 Delay(10);
 COM1Status := WriteCOMPort(_COM1,'MX,');
 Delay(10);
 COM1Status := WriteCOMPort(_COM1,'SF250,');   {fix field rate to 25mS}
 Delay(10);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'MO,');
 Delay(10);
 repeat
  Write('Gain ',n,': ');
  COM1Status := ReadlnCOMPort(_COM1,Svar,25);
  if COM1Status = 0 then
   Writeln(Svar)
    else
   Writeln(' Time out !');
  Inc(n);
 until (n > 4) OR (COM1Status <> 0);
 COM1Status := WriteCOMPort(_COM1,'NR,');
 Delay(10);

 if COM1Status <> 0 then
   begin
    Writenot_OKToEEPROM;
    LearnOffsetOk := FALSE;
    TestFailed := TRUE;
   end
  else
   begin
    ShowOffsetValues(NotSilent);
    Beep(1000,200);
    Write('Er offsetv‘rdierne OK ?   J/N ');
    if ScanCode <> TastJ then
     begin
      LearnOffsetOk := FALSE;
      TestFailed := TRUE;
     end;
   end; {if}
 TrueOrFalse(LearnOffsetOk);
end; {LearnOffset}




Procedure BeregnGainFactor;
{der m†les i MX-mode og MC-mode. Default gain faktorer skrives til EýPROM.
 M†leresultatet midles og korrektionsfaktorerne K1 - K4 beregnes.
 Derefter beregnes gainfaktorerne GA1 - GA4. Disse sammenlignes med de
 nominelle gainfaktorer, som er givet i konstanterne GA1Nom - GA4Nom.
 Er tolerancerne overholdt, skrives de nye gainfaktorer til EýPROM.
 Konstanten LightLevel skal defineres i hovedprogrammet.
 Gainfaktorerne beregnes forskelligt afh‘ngigt af SWRev:
 Rev 5.7:      Nominel 65535 16384 4096 1024 256
 Rev 5.8 >=:   Nominel 65535 16384 16384 16384 16384
               (m†lehoved korrigerer selv i omr†de 2, 3 og 4)    }

var  Svar        : STRING;
     Status,
     n           : BYTE;
     GA1,GA2,
     GA3,GA4     : WORD;
     KorrFactor  : ARRAY[1..4] OF REAL;
     GainFactor  : ARRAY[1..4] OF WORD;
     MeanRed,
     MeanGreen,
     MeanBlue    : REAL;
     Overflow    : BOOLEAN;
begin
 MeanValue_1 := 0;
 MeanValue_2 := 0;
 FillChar(KorrFactor,SizeOf(KorrFactor),0);
 LampOff;
 LEDOn;
 FreqHigh;
 Write('Fieldrate 120Hz: ');
 Delay(1800);
 Svar := ReadFieldRate;
 Writeln(Svar);

 LampOn;
 LEDOff;
 COM1Status := WriteCOMPort(_COM1,'MS,');
 Delay(10);
 WriteDefaultGainFactor;
 TrmDA8591(I2CIOAddr,144,LightLevel[1],Status);
 COM1Status := WriteCOMPort(_COM1,'MX,');
 Delay(10);
 COM1Status := WriteCOMPort(_COM1,'MC,');
 Delay(10);
 COM1Status := WriteCOMPort(_COM1,'FG1,');
 Delay(4000);
 n := 1;
 Overflow := FALSE;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  if (Svar[0] <> NULL) AND (Svar[1] = '-') then
   Overflow := TRUE;
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

  MeanValue_1 := UdregnMeanValueGreen;
  MeanGreen := MeanValue_1;
  MeanRed := UdregnMeanValueRed;
  MeanBlue := UdregnMeanValueBlue;
  FilterOk := (MeanRed > 10) AND (MeanGreen > 10) AND (MeanBlue > 10)
              AND not Overflow;

  Writeln('R:',MeanRed:6:0,'  G:',MeanValue_1:6:0,'  B:',MeanBlue:6:0);

  If not FilterOk then
   begin
    TestFailed := TRUE;
    Write('Korrekte v‘rdier:  R,G,B > 10');
    TrueOrFalse(not TestFailed);
    if Overflow then
     Writeln('Overflow');
     COM1Status := WriteCOMPort(_COM1,'MS,NR,');
    Delay(10);
    LampOff;
    Exit;
   end;


 Write('Korrektionsfaktor K1: ');
 COM1Status := WriteCOMPort(_COM1,'FG0;');
 Delay(3000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_2 := UdregnMeanValueGreen;
 if MeanValue_1 <> 0 then
  KorrFactor[1] := MeanValue_2 / MeanValue_1;
 Writeln(KorrFactor[1]:5:3,'     ',MeanValue_1:7:1,'  ',MeanValue_2:7:1);

 if TestAfbrudt then
  begin
   Writeln('Kalibrering afbrudt');
   COM1Status := WriteCOMPort(_COM1,'MS,NR,');
   Delay(10);
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;


 Write('Korrektionsfaktor K2: ');
 TrmDA8591(I2CIOAddr,144,LightLevel[2],Status);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'FG1;');
 Delay(4000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_1 := UdregnMeanValueGreen;

 COM1Status := WriteCOMPort(_COM1,'FG2;');
 Delay(3000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_2 := UdregnMeanValueGreen;
 if MeanValue_2 <> 0 then
  KorrFactor[2] := MeanValue_1 / MeanValue_2;
 Writeln(KorrFactor[2]:5:3,'     ',MeanValue_1:7:1,'  ',MeanValue_2:7:1);


 if TestAfbrudt then
  begin
   Writeln('Kalibrering afbrudt');
   COM1Status := WriteCOMPort(_COM1,'MS,NR,');
   Delay(10);
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;


 Write('Korrektionsfaktor K3: ');
 TrmDA8591(I2CIOAddr,144,LightLevel[3],Status);
 COM1Status := WriteCOMPort(_COM1,'FG2,');
 Delay(4000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_1 := UdregnMeanValueGreen;

 COM1Status := WriteCOMPort(_COM1,'FG3,');
 Delay(3000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_2 := UdregnMeanValueGreen;
 if MeanValue_2 <> 0 then
  KorrFactor[3] := MeanValue_1 / MeanValue_2;
 Writeln(KorrFactor[3]:5:3,'     ',MeanValue_1:7:1,'  ',MeanValue_2:7:1);

 if TestAfbrudt then
  begin
   Writeln('Kalibrering afbrudt');
   COM1Status := WriteCOMPort(_COM1,'MS,NR,');
   Delay(10);
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;



 Write('Korrektionsfaktor K4: ');
 TrmDA8591(I2CIOAddr,144,LightLevel[4],Status);
 COM1Status := WriteCOMPort(_COM1,'FG3,');
 Delay(4000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_1 := UdregnMeanValueGreen;

 COM1Status := WriteCOMPort(_COM1,'FG4,');
 Delay(4000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 MeanValue_2 := UdregnMeanValueGreen;

 if MeanValue_2 <> 0 then
  KorrFactor[4] := MeanValue_1 / MeanValue_2;
 Writeln(KorrFactor[4]:5:3,'     ',MeanValue_1:7:1,'  ',MeanValue_2:7:1);

 COM1Status := WriteCOMPort(_COM1,'MS,');   Delay(10);
 COM1Status := WriteCOMPort(_COM1,'NR,');   Delay(10);
 LampOff;

 SWRevStr := ReadSWRev;
 if SWRevStr = MeasHeadSWRev[1] then
  begin
    GA1 := GA1Nom57;        {bruges kun ved SW-revsion = 5.7}
    GA2 := GA2Nom57;
    GA3 := GA3Nom57;
    GA4 := GA4Nom57;
  end
   else
  begin
    GA1 := GA1Nom;          {bruges ved SW-revsion >= 5.8 }
    GA2 := GA2Nom;
    GA3 := GA3Nom;
    GA4 := GA4Nom;
  end;

 {$R-}   {hvis KorrFactor[x] bliver meget stor, f†s runtime error pga}
         {range error ved beregning af GainFactor[x]}
 GainFactor[1] := Trunc(KorrFactor[1] * GA1);
 GainFactor[2] := Trunc(KorrFactor[1] * KorrFactor[2] * GA2);
 GainFactor[3] := Trunc(KorrFactor[1] * KorrFactor[2] * KorrFactor[3]
                        * GA3);
 GainFactor[4] := Trunc(KorrFactor[1] * KorrFactor[2] * KorrFactor[3]
                        * KorrFactor[4] * GA4);
 {$R+}
 BeregnGainFactorOk :=
             ((GA1 * (1 + (GainFactorTol / 100))) > GainFactor[1]) AND
             ((GA1 * (1 - (GainFactorTol / 100))) < GainFactor[1]) AND
             ((GA2 * (1 + (GainFactorTol / 100))) > GainFactor[2]) AND
             ((GA2 * (1 - (GainFactorTol / 100))) < GainFactor[2]) AND
             ((GA3 * (1 + (GainFactorTol / 100))) > GainFactor[3]) AND
             ((GA3 * (1 - (GainFactorTol / 100))) < GainFactor[3]) AND
             ((GA4 * (1 + (GainFactorTol / 100))) > GainFactor[4]) AND
             ((GA4 * (1 - (GainFactorTol / 100))) < GainFactor[4]);



 if BeregnGainFactorOk then
   begin
    TestModePM5639;
    WriteEEPROM(4,Lo(GainFactor[1]));
    WriteEEPROM(5,Hi(GainFactor[1]));
    WriteEEPROM(6,Lo(GainFactor[2]));
    WriteEEPROM(7,Hi(GainFactor[2]));
    WriteEEPROM(8,Lo(GainFactor[3]));
    WriteEEPROM(9,Hi(GainFactor[3]));
    WriteEEPROM(10,Lo(GainFactor[4]));
    WriteEEPROM(11,Hi(GainFactor[4]));
   end
  else
   begin
    TestFailed := TRUE;
    Writenot_OKToEEPROM;
   end;


 Write('Beregnede gainfaktorer: ',GainFactor[1],'  ',GainFactor[2],'  ');
 Write(GainFactor[3],'  ',GainFactor[4]);
 TrueOrFalse(BeregnGainFactorOk);
 Write('Nominelle gainfaktorer: ',GA1,'  ',GA2,'  ');
 Writeln(GA3,'  ',GA4,'   (tol: ñ ',GainFactorTol,'%)');

 Write('Beregnede gainfaktorer ');
 if BeregnGainFactorOk then
   Writeln(' skrevet til EýPROM')
  else
   Writeln(' IKKE skrevet til EýPROM');
end;



Procedure ReadGainFactor;           {F4}
var  n,
     g1,g2    : BYTE;
     k        : INTEGER;
     Svar     : STRING;
     GainVal  : STR_40;
     GF       : ARRAY[1..5] OF WORD;
     Err      : BOOLEAN;
begin
 Err := FALSE;
 ReadGainFactorOk := FALSE;
 GainVal := '';
 n := 1;
 COM1Status := WriteCOMPort(_COM1,'MA2,');   Delay(10);
 COM1Status := ClearInputQue(_COM1);
 repeat
   COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
   COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
   GainVal := GainVal + Svar + ',';
   Val(Svar,g1,k);
   if k <> 0 then Err := TRUE;
   COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
   COM1Status := ReadlnCOMPort(_COM1,Svar,1); Delay(5);
   Val(Svar,g2,k);
   if k <> 0 then Err := TRUE;
  {$R-}
   GF[n] := g2 * 256 + g1;
  {$R+}
   GainVal := GainVal + Svar + ',';
   Inc(n);
 until (n > 5) OR (COM1Status <> 0);

 if (Length(GainVal) = 40) AND (not Err) then
  begin
   Delete(GainVal,40,1);
   ReadGainFactorOk := TRUE;
  end;
 Writeln('Gain faktorer: ',GainVal);
 Write('               ',GF[1]:6,GF[2]:6,GF[3]:6,GF[4]:6,GF[5]:6);
 TrueOrFalse(ReadGainFactorOk);
end;

Procedure RS232Test;
{Den serielle forbindelse kontrolleres}
var Svar : STRING;
begin
 RS232TestOk := FALSE;
 Write('RS232 test: ');
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'F?,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'F?,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,2);
 RS232TestOk := COM1Status = 0;
 Delay(100);
 TrueOrFalse(RS232TestOk);
 if not RS232testOk then
  TestFailed := TRUE;
end;

Procedure DarkCurrentTest;       {F6}
{Der m†les p† lyskassen med overpart p†sat, hvor m†leresultatet fra
 RGB-kanalerne ikke m† v‘re overskredet.
 Omgivende lys skal undg†s
 Tolerancer er givet efter keyword CONST}
var Svar       : STRING;
    Status     : BYTE;
    Kode       : INTEGER;
    ValueRed,
    ValueGreen,
    ValueBlue,
    FRate      : REAL;
begin
 Beep(1000,200);
 LampOff;
 LEDOff;
 Writeln('M›rkestr›m: ');
 Writeln('Monter overpart   -   tryk en tast');
 WaitForAnyKey;
 Delay(3000);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'MX,');   Delay(10);
 COM1Status := WriteCOMPort(_COM1,'F?,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,2);
 if Svar[1] = '-' then
  Delete(Svar,1,1);
 Val(Svar,FRate,Kode);
 COM1Status := WriteCOMPort(_COM1,'TM,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,2);
 Val(Copy(Svar,1,6),ValueRed,Kode);
 Val(Copy(Svar,8,6),ValueGreen,Kode);
 Val(Copy(Svar,15,6),ValueBlue,Kode);
  if (Kode = 0) AND (FRate > 0) then
 ValueRed := Abs(ValueRed / FRate);
  if (Kode = 0) AND (FRate > 0) then
 ValueGreen := Abs(ValueGreen / FRate);
  if (Kode = 0) AND (FRate > 0) then
 ValueBlue := Abs(ValueBlue / FRate);
 DarkCurrentTestOk := (ValueRed < DarkLimitRed) AND
                      (ValueGreen < DarkLimitGreen) AND
                      (ValueBlue < DarkLimitBlue) AND
                      (COM1Status = 0) AND
                      (Svar[1] <> '-');
 Write('      R:',ValueRed:6:3,'  G:',ValueGreen:6:3,'  B:',ValueBlue:6:3);
 TrueOrFalse(DarkCurrentTestOk);
 Writeln('Max   R:',DarkLimitRed:6:2,'  G:',DarkLimitGreen:6:2,'  B:',DarkLimitBlue:6:2);

 if not DarkCurrentTestOk then
   TestFailed := TRUE;
end;

Procedure EnterKU_Nummer;        {F2}
{Indtastning af apparatets KU-nummer. Kun numeriske v‘rdier kan indtastes.
 Tastes der kun ENTER ‘ndres der ikke p† nummeret i EýPROM'men
 Tilslut udl‘ses KU-nummeret fra EýPROM'men}
var    Svar    : STRING;
       KUStr,
       InStr   : STR_6;
       KUNo    : LONGINT;
       kode    : INTEGER;
       n,L     : BYTE;

begin
 KUStr := ReadKU_Nummer;
 EmptyKeyboardBuffer;
 repeat
  Writeln('Nuv‘rende KU-nummer:   ',KUStr);
  Write('Indtast nyt KU-nummer: ');
  Readln(InStr);
  Val(InStr,KUNo,Kode);
  if KUNo < 0 then Kode := 1;
 until (Kode = 0) OR (InStr = '');

 L := Length(InStr);
 if L > 0 then
  begin
   TestModePM5639;
   Str(KUNo,InStr);
   L := Length(InStr);
   for n := 1 TO 6 DO
     WriteEEPROM(n+84,Ord('0'));
   for n := 1 TO L DO
     WriteEEPROM((n-L+90),Ord(InStr[n]));
   Writeln('KU-nummer rettet til:  ',ReadKU_Nummer);
   ValidKU := TRUE;
  end
 else
  begin
   ValidKU := TRUE;
   InStr := ReadKU_Nummer;
    for n := 1 TO Length(InStr) DO
     begin
      if not (InStr[n] IN Tal) then
       begin
        ValidKU := FALSE;
        TestFailed := TRUE;
       end; {if}
     end; {for}

   Writeln('KU-nummer ikke rettet!    ',ValidKU);
  end; {if L}
end;



Procedure WriteIdString;               {AltF5}
{Id-streng skrives til EýPROM}
Var n    : BYTE;
begin
 Writeln('Skriver ',ProbeType,' ID string');
 TestModePM5639;
 for n := 1 TO Length(IdStr[ProbeNo]) DO
  WriteEEPROM(n+61,Ord(IdStr[ProbeNo,n]));
end;

Procedure WriteProtectYesOrNo(Flag: BOOLEAN);    {CtrlF4}
{unders›ger om SP6 er ON eller OFF ved at pr›ve at ‘ndre data i adresse 1.
 SP6=ON: writeprotect ON     SP6=OFF: writeprotect OFF
 Hvis Flag = FALSE vises ingen udskrift p† sk‘rmen}
var ReadByte,
    WriteByte  : BYTE;
    Svar       : string;
begin
 if Flag then
  Write('SP6 = skrivebeskyttet ?     ');
 TestModePM5639;
 COM1Status := WriteCOMPort(_COM1,'MA1,');   Delay(20);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RN1,');   Delay(20);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 if COM1Status = 0 then
  begin
   WriteByte := Ord(Svar[1]) XOR $FF;
   WriteEEPROM(1,WriteByte);
   COM1Status := WriteCOMPort(_COM1,'MA1,');   Delay(20);
   COM1Status := ClearInputQue(_COM1);
   COM1Status := WriteCOMPort(_COM1,'RN1,');   Delay(20);
   COM1Status := ReadlnCOMPort(_COM1,Svar,1);
   ReadByte := Ord(Svar[1]);
   SP6WriteProtectOk := ReadByte <> WriteByte;
   if Flag then
    if SP6WriteProtectOk then
      Writeln('JA')
     else
      Writeln('NEJ');
  end
  else
    Writeln(' L‘sefejl fra m†lehoved!!');
end;
