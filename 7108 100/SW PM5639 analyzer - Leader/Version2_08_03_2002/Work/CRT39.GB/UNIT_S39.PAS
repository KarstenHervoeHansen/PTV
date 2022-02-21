Unit Unit_S39;
{
 Utilities for sensor 5639/90 serie III and up
                      5639/92 serie I and up
                      5639/93 serie I and up
}
{$DEFINE LYSKASSE}
(*{$DEFINE RGBGEN}  d.8.05.01 :JK*)
{$DEFINE IEEEBUS}

Interface
uses Dos,Peputil, Comutil;

type
 LLType      = array[1..5] of byte;
 ValueKType  = array[1..5] of word;

const
 jk_delay                = 1000;
 TraceFlag               = 1;             {1:   cal traceable to NIST}
                                          {255: cal traceable to NPL}
 COMNo                   = _COM1;   { color sensor }
 COMGen                  = _COM2;   { PM5639M/82 generator }
 SensorSWRev             : array[1..4] of str_4 =
                           ('02.1','03.2','03.0','03.4');
 TestSWNo                = '7108 100 73261';     {12 NC for test-sw}
 TestSWRev               = '010516';
 MaxTimeBetweenRefMeas   = 30;    {minutter}
 SONY_Timeout            = 900;   {sec}      {SONY=black timeout}
 MinMonitorOnTime        = 45;    {sec}      {min tëndt inden cal (white)}

 LIGHT_BOX_FILE_NAME    = 'lbox.dat';

 EBU_XYFilename          = 'EBU_XY.DAT';     {xy-vërdier + luminans}
                                             {for EBU monitor}
 SMPTE_XYFilename        = 'SMPTE_XY.DAT';   {xy-vërdier + luminans}
                                             {for EBU monitor}
 MeasFileName            = 'NEW_MEAS._39';   {file med nyeste data}
                                             {overskrives ved ny mÜling}
 RefSensorFilename       = 'REF_XY.DAT';     {ref sensor xyY values for}
                                             {EBU & SMPTE monitors}
 MonitorTimeFileName     = 'MONITOR.DAT';
 PrinterPort             = 'LPT2';
 HDDrive                 = 'C'; {d. 9.05.01: JK: no LAN}
 PathName                = ':\MEASDATA\5639SEN\';
 Marg                    = '        ';           {v. margin i certiffile}
 NameExt                 = '.I39';
 MaxLuminans             = 150;                  {max luminans i filen XYFilename}
 xyTolRefSensor          = 0.002;                { Ò tol }
 YTolRefSensor           = 2.0;                  { Ò tol i NIT }

 D6500Ref           : array[1..2] of real = (0.1978,0.4684); {u',v'}

 NIT_fL_Factor      = 3.426259;                  {cd/m˝ til footLambert}

(* CalLightLevel is defined here and not initialized.  It is filled with
   values read from file LIGHT_BOX_FILE_NAME *)

CalLightLevel  : array[1..2] of LLType = 
((0,0,0,0,0),
 (0,0,0,0,0)); 



(* CalLightLevel      : array[1..2] of LLType =  {lysstyrke ved gain-kal}
    ((248, 213, 174, 153, 115), { d.08.03.02: JK}*)
(*  ((246, 199, 160, 150,110), { d.14.05.01: JK} *)
(*  ((255,200,160,140,100),    { /90 <=serie 4 + /92/93 <=serie 2 fra 10.3.1995 }*)
(*  (255,255,200,150,120));   { /90 serie 5 + /92/93 serie 3   fra 23.6.1999} *)
(*    (248,248,213,164,134));   { d.08.03.02 JK /90 serie 5 + /92/93 serie 3   fra 23.6.1999}*)

 CalMinValueK       : array[1..2] of ValueKType =
           ((500,200,50,8,2),    { /90 <=serie 4 + /92/93 <=serie 2 fra 10.3.1995 }
            (200,200,100,20,2)); { /90 serie 5 + /92/93 serie 3   fra 23.6.1999}

 DefaultGainFactor  : array[1..12] of byte =
                      ($FF,$FF,$00,$40,$00,$40,$00,$40,$00,$40,$00,$40);

 DefaultPreGain     : array[1..3] of real =
                      (1.0,1.0,1.0);

 PM5639IdStr        : array[1..4] of str_29  =
                      ('PHILIPS,400810979300,KU',     {5639/90 serie III,IV,V}
                       'PHILIPS,400810978930,KU',     {5639/92 serie I,II,III}
                       'PHILIPS,400810978930,KU',     {5639/916}
                       'PHILIPS,400810983120,KU');    {5639/93 serie II,III}

 VoltMax            : real = 9.0;       {display unit shutdown control}
 VoltMin            : real = 3.95;      {   -      -      -       -   }
 VoltStepSize       : real = 0.05;      {   -      -      -       -   }
 VoltStep           : array[1..5] of real
                    = (5.5,6.0,6.6,7.1,7.5); {display unit shutdown control}
 R29                : real = 1.12;      { ohm }
 ADRef              : real = 2.4;       {volt  (ved beregning af offset)}
 ADCounts           : word = 50000;     {ved beregning af offset}
 TolUpper5Va        : byte = 8;         {õvre Ò 5V analog tolerance i %}
 TolLower5Va        : byte = 8;         {nedre Ò 5V analog tolerance i %}
 TolRef             : byte = 8;         {AD-ref tolerance i %}
 ValuePos5Va        : real = 4.9;       {Nominel positiv analog spënding}
 ValueNeg5Va        : real = -4.9;      {Nominel negativ analog spënding}
 Min5VBarco         : real = 4.8;
 Max5VBarco         : real = 5.2;
 ValueRef           : real = 2.4;       {Nominel AD-ref spënding}
 CableLoose         : real = 90E-3;     {Tab i stelledning i Volt}
 CurrentMin         : real = 35;        {Min strõm i mA til 5V}
 CurrentMax         : real = 58;        {Max strõm i mA til 5V}
                                        {50 mA max for ÊP=MASKE }
                                        {58 mA max for ÊP=EPROM }
                                        {/93 bruger ca 3mA ekstra pga LED}
 DarkLimitRed       : real = 0.03;      {Max dark current}
 DarkLimitGreen     : real = 0.03;      {MX-value / fieldtime}
 DarkLimitBlue      : real = 0.03;
 GANom              : array[1..5] of Word = (16384,16384,16384,16384,16384);

 KorrNom            : array[1..5] of real = (1.08,1.00,1.03,0.88,0.96);
 KorrFactorTol      = 7;            {tolerance i procent}

 AntalMeas          = 7;            {ved mÜling af korr-faktorer,}
                                    {K-matrix og CRT-matrix}
 EEPROMwriteDelay   = 30;           {mS}
 LearnDelay         = 8;            {Sek. ved learn af K-matrix eller CRT-matrix}
 I2CIOAddr          = $300;         {I˝C-PC-interface I/O-adresse}
 InQueSize          = 500;          {COM1 port}
 OutQueSize         = 100;          {  -    - }

 PreGainMulFactor   = 1024;         {Multiplikationsfaktor for}
                                    {XFact, YFact og ZFact}
 CRTMulFactor       = 8192;         {Multiplikationsfaktor for}
                                    {CRT-matrix}
 KorrMulFactor      : longint =
                      100000;       {Multiplikationsfaktor for}
                                    {korrektionsmatrix}
 KorrAddr           = 103;          {Start-adresse i E˝PROM for}
                                    {korrektionsmatrix}
 PreGainAddr        = 97;           {Start-adresse i E˝PROM for}
                                    {for pre gain}
 CRTNameAddr        = 1016;         {Hõjeste start-adresse i E˝PROM}
                                    {for CRT-navne}
 CRTMatrixAddr      = 1004;         {Hõjeste start-adresse i E˝PROM}
                                    {for CRT-matrix}
 DefInternID        : array[1..14,1..4] of byte =    {addr 58 - 61}
                     ((1,0,0,1),            {5639/90 serie III   }
                      (1,0,0,2),            {5639/90 serie IV,V  }
                      (0,0,0,0),            {                    }
                      (0,0,0,0),            {                    }
                      (1,0,0,32),           {5639/92 serie I     }
                      (1,0,0,33),           {5639/92 serie II,III}
                      (0,0,0,0),            {                    }
                      (0,0,0,0),            {                    }
                      (1,0,0,56),           {5639/93 serie II,III}
                      (1,0,0,0),            {                    }
                      (1,0,0,0),            {                    }
                      (1,0,0,0),            {                    }
                      (1,0,0,40),           {5639/916            }
                      (1,0,0,0));           {                    }

 DefMaxNoCRTFosfor   = 30;             {Max antal CRT-fosforer}
 DefNoEEPROMPages    = 4;              {Antal sider a 256 bytes i E˝PROM}
 DefMaxWPAddr : Word = 193;            {Hõjeste adresse i E˝PROM som er}
                                       {skriveskyttet}
 RGBIOutputMode      = 1;
 RGBOutputMode       = 3;
 Barco               = 2;
 Silent              = false;
 NotSilent           = true;
 SaveCalData         = true;
 NotSaveCalData      = false;
 MonEBU              = 1;
 MonSMPTE            = 2;


type
 MeasStrType  = array[1..AntalMeas] of string[30];
 KMEType      = array[1..18] of byte;
 OffsetType   = array[0..5,0..3] of real;      {gain 0-5, channel 0-3}
 XYMatrixType = array[1..6] of real;


var
 MeasFile,
 CertifFile,
 MonitorTimeFile,
 RefSensorFile,
 XYFile,
 Logfile             : text;
 COMStatus           : word;       {status for COM port}
 St,                               {status for I2C-bus}
 IICPort64,
 IICPort66           : byte;       {status af I˝C-port addr 64 og 66}
 SWRevStr            : str_4;      {mÜlehovedets SW-revision}
 DateStr,
 KU_No               : str_6;      {mÜlehovedets KU-nummer}
 CompanyIDStr        : str_10;
 CertifFileName      : str_80;     {fil med testcertifikat}
 DirInfo             : SearchRec;

 LogFilename         : str_40;

 MonitorType         : byte;       {Kalibrering pÜ EBU eller SMPTE monitor}
                                   { se const MonEBU og MonSMPTE}
 RefMonitor          : byte;       {valgt ref monitor 1=EBU   2=SMPTE}
 RefMonitorValue     : byte;       {ref monitor value in addr 193}

 SensorVer,                        {3,4,5 for /90   1,2,3 for /92  2,3 for /93}
 SensorSerie         : Word;       {90  for /90
                                    92  for /92
                                    93  for /93
                                    916 for /916
                                    2   for Barco option}
 MinValueK      : ValueKType;     {min mÜlevërdi ved gainkalibrering}
 LightLevel     : LLType;         {aktuel lysstyrke ved gain-kal}

 IICBusError,
 TestFailed,
 RS232TestOk,
 CurrentTestOk,
 AnalogPos5VTestOk,
 AnalogNeg5VTestOk,
 ADRefTestOk,
 Sensor5VBarcoOk,
 BarcoMeasureOk,
 DarkCurrentTestOk,
 BeregnGainFactorOk,
 ReadIdStringOk,
 ReadGainFactorOk,
 ReadCRTNameOk,
 ReadKorrMatrixOk,
 ReadCRTMatrixOk,
 ReadxyPhOk,
 ReadInternIDOk,
 ReadPreGainOk,
 ReadCalDateOk,
 BaudrateShiftOk,
 ReadwriteProtectOk,
 ReadNoCRTFosforOk,
 ReadNoEEPROMPagesOk,
 ReadKalDataOk,
 LearnOffsetOk,
 MakeSensorCalibrationOk,
 LearnEBUPhosphorOk,
 LearnSMPTEPhosphorOk,
 GainSelOk,
 TotalTestOk,
 ValidKU,
 FilterOk,
 Analog5VTestValgt,
 ADRefTestValgt,
 writeProtectOk,
 TotalTestFlag,
 PrinterFlag,
 EEPROMwriteError,
 EBUSetOn,
 SMPTESetOn,
 ReadRefMonOk,
 AfslutProgram     : boolean;

 MeasStr           : MeasStrType;
 MeanValue_1,
 MeanValue_2       : real;
 KalStr            : array[1..25] of STR_120;
 InternIDRead      : array[1..4] of byte;

 Offset            : OffSetType;
 OffSetData        : array[0..5,0..5] of byte;

 KorrMatrix        : KMEType;
 KMatrwrited,
 KMatr             : array[1..9] of real;
 xyrefsens         : array[1..2,1..3] of real;  { EBU,SMPTE xyY values for ref sensor }

 CRTMatrix         : array[1..30,1..12] of byte;
 XYCRTMatrix       : XYMatrixType;
 CRTName           : array[1..30] of STR_10;
 WriteProtectAddr  : word;
 XFactRead,              { reading from EEPROM }
 YFactRead,
 ZFactRead,
 XFact,                  { writing to EEPROM }
 YFact,
 ZFact             : real;
 NoCRTFosfor,
 NoEEPROMPages     : byte;


 XR,YR,ZR,                    {mÜlte vërdier i MX-mode ved beregning af}
 XG,YG,ZG,                    {matricer.}
 XB,YB,ZB,                    {XR=Rõd kanal v. rõd skërm}
 X65,Y65,Z65,                 {YR=Grõn kanal v. rõd skërm}
                              {XG=Rõd kanal v. grõn skërm}
                              {Y65=Grõn kanal v. hvid skërm     osv}

 a,b,c,d,e,f,g,h,j,           {K-matrice}
 XpR,YpR,ZpR,                 {red       xyz-vërdier i CIE-diagram for }
 XpG,YpG,ZpG,                 {green     monitor lëses fra filen 'XYFileName'}
 XpB,YpB,ZpB,                 {blue}
 Xp65,Yp65,Zp65,              {white D6500}

 NominelLum        : real;    {luminans ved hvid D6500}
                              {lëses fra filen 'XYFilename'}



Procedure CalculateMean(MS : MeasStrType; NoMeas : byte; var R,G,B : real; var St : boolean);
Procedure writeEEPROM(Addr : WORD; Data : byte);
Procedure LampOn;            {Tast L}
Procedure LampOff;
Procedure RelayOn;           {Tast R}
Procedure RelayOff;
Procedure InitIEEEBus;
Procedure InitI2CBus;
Procedure InitPC_COMPort;
Procedure PowerOffOn;                     {Tast P}
Procedure MXModePM5639;
Procedure ToggleRelay;                    {Tast R}
Procedure ToggleLamp;
Function  ReadKalStatus : STR_20;         {F4}
Procedure ReadIdString(flag : boolean);   {F4}
Procedure writeOKToEEPROM;
Function ReadKU : string;
Function ReadSWRev : string;
Function Read12NC : string;
Function ReadTraceFlag : string;
Function ReadOutputFormat : string;
Procedure ReadWPAddr(Flag : boolean);
Procedure ReadInternID;
Procedure ReadNoEEPROMPages;
Function TestAfbrudt : boolean;
Procedure writeDefaultGainFactor;
Procedure TrueOrFalse( Bool : boolean);
Procedure ShowTestFlagStatus(Message : STR_30; Bool : boolean; XPos : byte);
Function FinalTestOk : boolean;
Procedure ShowOffsetValues(Flag : boolean);    {CtrlF6}
Procedure BeregnGainFactor;
Procedure LearnOffset;                         {F5}
Procedure ReadGainFactor;                      {F4}
Procedure RS232Test;
Procedure DarkCurrentTest;                     {F6}
Procedure EnterKU_Nummer;                      {F2}
Procedure writeIdString;                       {AltF5}
Procedure writePHILIPS_ID;                     {AltF1}
Procedure WriteCalDate;
Procedure ReadCalDate(Flag : boolean);         {F4}
Procedure WriteRefMonitor;
Procedure ReadRefMonitor(Flag : boolean);      {F4}
Procedure ReadPreGainFactor(Flag : boolean);   {F4}
Procedure writeProtectYesOrNo(Flag: boolean);  {CtrlF4}
Procedure EnterLevel;                          { Tast A }
Procedure CheckSWBaudrateShift;
Procedure  EBUWhite;
Procedure  EBURed;
Procedure  EBUGreen;
Procedure  EBUBlue;
Procedure  EBUBlack;
Procedure  SMPTEWhite;
Procedure  SMPTERed;
Procedure  SMPTEGreen;
Procedure  SMPTEBlue;
Procedure  SMPTEBlack;
Procedure  CRT625Lines;
Procedure  CRT525Lines;
Procedure  RGBGenRemote;
Procedure  RGBGenLocal;
Procedure  WPOn;                   { Ctrl F8 }
Procedure  WPOff;                  { Ctrl F9 }
Procedure InitTopWindow;
Procedure InitBottomWindow;
Procedure InitMainWindow;
Procedure SelectMonitor;
Procedure InitBottomWindowBarco;
Procedure ReadKMatrix(Flag : boolean);    {CtrlF2}
Procedure SaveMonitorOnTime;
Procedure ReadMonitorOnTime(var ElapsedSek : word);
Function GetKeycode : byte;
Function CheckMonitorOnTime : boolean;
Procedure InitPM5639_82Generator;
procedure get_light_box_data;





Implementation

uses Unit_IIC, Win, Crt;



Procedure CalculateMean(MS : MeasStrType; NoMeas : byte; var R,G,B : real; var St : boolean);
var   n,p       : byte;
      Kode      : integer;
      FT,MVal   : real;
begin
 R := 0; G := 0; B := 0;
 for n := 1 to NoMeas do
  begin
   p := Pos(',',MS[n]);                    {Red}
    if p > 0 then
    begin
     Val(Copy(MS[n],1,p-1),MVal,Kode);
     if Kode <> 0 then St := false;
     R := R + Mval;
     Delete(MS[n],1,p);
    end;
   p := Pos(',',MS[n]);                    {Green}
    if p > 0 then
    begin
     Val(Copy(MS[n],1,p-1),MVal,Kode);
     if Kode <> 0 then St := false;
     G := G + Mval;
     Delete(MS[n],1,p);
    end;
   p := Pos(',',MS[n]);                    {Blue}
    if p > 0 then
    begin                                  { output format R,G,B,I }
     Val(Copy(MS[n],1,p-1),MVal,Kode);
     if Kode <> 0 then St := false;
     B := B + Mval;
     Delete(MS[n],1,p);
    end
    else
    begin                                  { output format R,G,B }
     Val(Copy(MS[n],1,Length(MS[n])),MVal,Kode);
     if Kode <> 0 then St := false;
     B := B + Mval;
    end
  end; {for}


 if SensorVer = 916 then
  begin
   FT := 25;                                 {Fixed field time = 25 mS}
   Kode := 0;
  end
 else
   Val(MS[NoMeas],FT,Kode);                  {Field time}

 if (Kode = 0) and (FT <> 0) then
  begin
   R := R / NoMeas / Abs(FT) * 20;            {norm to field time = 20 mS}
   G := G / NoMeas / Abs(FT) * 20;
   B := B / NoMeas / Abs(FT) * 20;
   St := true;
  end
 else
  St := false;
end;



Procedure LampOff;
begin
{$ifdef LYSKASSE}  {d. 8.05.01: JK}
 IICPort64 := IICPort64 or $01;
 Trm8574(I2CIOAddr,64,IICPort64,St);
{$endif}
end;

Procedure LampOn;
begin
{$ifdef LYSKASSE}  {d. 8.05.01: JK}
 IICPort64 := IICPort64 and $FE;
 Trm8574(I2CIOAddr,64,IICPort64,St);
{$endif}
end;

Procedure RelayOn;
begin
{$ifdef LYSKASSE}   {d. 8.05.01:JK}
 IICPort64 := IICPort64 and $EF;
 Trm8574(I2CIOAddr,64,IICPort64,St);
{$endif}
end;

Procedure RelayOff;
begin
{$ifdef LYSKASSE}   {d. 8.05.01:JK}
 IICPort64 := IICPort64 or $10;
 Trm8574(I2CIOAddr,64,IICPort64,St);
{$endif}
end;

Procedure InitIEEEBus;
{Initialiserer IEEE-interface. Checker om PM2534 er i kontakt med IEEE-
 interface pÜ adresse 22. Hvis der ikke er kontakt, sëttes flaget
 AfslutProgram = true}
begin
 if not IeeeFilesOpen then
  OpenIeeeFiles;
  writeln(IeOut,'RESET');
  writeln(IeOut,'ERROR OFF');
  writeln(IeOut,'TIME OUT 2');
  writeln(IeOut,'CONFIG /B30');
  writeln(IeOut,'CONFIG /N PM2534 22');
 {$I-}
  writeln(IeOut,'CLEAR  PM2534');                       {DMM}
  writeln(IeOut,'OUTPUT  PM2534; FNC VDC');
  writeln(IeOut,'LOCAL PM2534');                        {DMM=local}
 {$I+}
  if IOResult > 0 then
   begin
    CreateWindow(20,10,4,40,' IEEE-bus error ',Green,Green,Frame);
    CursorOff;
    writeln(Bell,' No contact with PM2534 on IEEE-bus');
    write(' Program terminated  -  press any key');
    WaitForAnyKey; CloseWindow;
    CursorOn;
    AfslutProgram := true;
   end
end;


Procedure InitI2CBus;
{Initialiserer I˝C-interface. Checker om lyskasse og RGB-generator er i
 kontakt med I˝C-interface pÜ henholdsvis adresse 64 og 144. Hvis der
 ikke er kontakt, sëttes flaget AfslutProgram = true}

begin

(*{$ifdef RGBGEN}     d.8.05.01:JK*)
 InitIIC(I2CIOAddr,St);
(*{$endif}*)

{$ifdef RGBGEN}
 Trm8574(I2CIOAddr,66,$FF,St);       {Addr findes i RGB-generator}
  if (St and $08) = $08 then
   begin
    CreateWindow(12,10,4,56,' I˝C-bus error ',Green,Green,Frame);
    CursorOff; TextColor(White);
    writeln(Bell,' No contact with RGB-generator on I˝C-bus (',St,')');
    write(' Program terminated  -  press any key');
    WaitForAnyKey; CloseWindow; CursorOn;
    AfslutProgram := true;
    IICBusError := true;
   end;
{$endif}

{$IFDEF LYSKASSE}
 Trm8574(I2CIOAddr,64,$FF,St);
  if (St and $08) = $08 then
   begin
    CreateWindow(15,10,4,50,' I˝C-bus error ',Green,Green,Frame);
    CursorOff; TextColor(White);
    writeln(Bell,' No contact with light box on I˝C-bus (',St,')');
    write(' Program terminated  -  press any key');
    WaitForAnyKey; CloseWindow;
    CursorOn;
    AfslutProgram := true;
    IICBusError := true;
   end;

  if not IICBusError then
   TrmDA8591(I2CIOAddr,144,50,St);   {lysstyrke i lampe}
{$ENDIF}

end;


Procedure InitPC_COMPort;
{Initialiserer seriel-COM-port.}
begin
 COMStatus := OpenCOMPort(COMNo,InQueSize,OutQueSize);
 if (SensorVer = 916) then
  COMStatus := InitCOMPort(COMNo,_Baud9600,_Databit8,_Stopbit2,_None,_CTSNotRequired)
 else
  COMStatus := InitCOMPort(COMNo,_Baud4800,_Databit8,_Stopbit2,_None,_CTSNotRequired);

 { PM5639M/82 generator }
 COMStatus := OpenCOMPort(COMGen,50,100);
 COMStatus := InitCOMPort(COMGen,_Baud4800,_Databit8,_Stopbit2,_None,_CTSNotRequired);

end;


Procedure PowerOffOn;        {TastP}
var Svar : string;
begin
 writeln('Power OFF - ON');
 IICPort64 := IICPort64 and $DF;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(1000);
 IICPort64 := IICPort64 or $20;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 Delay(200);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'F?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,2);
 COMStatus := ClearInputQue(COMNo);
end;

Procedure ToggleRelay;       {TastR}
begin
 IICPort64 := IICPort64 XOR $10;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 if (IICPort64 and $10) = $10 then
   writeln('Relay: Probe')
  else
   writeln('Relay: Current measurement');
end;

Procedure ToggleLamp;
begin
 IICPort64 := IICPort64 XOR $01;
 Trm8574(I2CIOAddr,64,IICPort64,St);
 if (IICPort64 and $01) = $01 then
   writeln('Lamp: OFF')
  else
   writeln('Lamp: ON');
end;


Procedure MXModePM5639;
begin
 COMStatus := writeCOMPort(COMNo,'MS,MX,');
end;


Procedure writeEEPROM(Addr : WORD; Data : byte);
{Skriver en byte til adressen Addr. PM5639 skal vëre i Testmode (MX-mode)}
var s  : STR_4;
begin
 Str(Addr,s);
 COMStatus := writeCOMPort(COMNo,'MA' + s + ',');
 Delay(20);
 Str(Data,s);
 COMStatus := writeCOMPort(COMNo,'SM' + s + ',');
 Delay(EEPROMwriteDelay);
end;


Procedure writeDefaultGainFactor;
{Skriver default gainfaktorer til E˝PROM}
var n,k : byte;
begin
 writeln('Default gain factors for E˝PROM');
 MXModePM5639;
 for k := 0 TO 2 DO
  for n := 1 TO 12 DO
   writeEEPROM(k*12 + n + 1,DefaultGainFactor[n]);
end;

Procedure TrueOrFalse( Bool : boolean);
begin
 GotoXY(72,WhereY);
 if Bool then
  writeln(' OK')
 else
  writeln(' FAILED');
end;

Procedure ShowTestFlagStatus(Message : STR_30; Bool : boolean; XPos : byte);
var  OldAttr : byte;
begin
 OldAttr := TextAttr;
 write(Message);
 GotoXY(XPos,WhereY);
 if Bool then
  write(' OK')
 else
  begin
   TextColor(Red);
   write(' FAILED');
   TextAttr := OldAttr;
  end;
 GotoXY(1,WhereY+1);
end;

FUNCTION ReadKalStatus : STR_20;         {F4}
var  Svar  : string;
     k     : integer;
     v     : byte;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'MA157,RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 if COMStatus = 0 then
  begin
   Val(Svar,v,k);
   if k = 0 then
    if v <> 0 then
      ReadKalStatus := '        FACTORY'
     else
      ReadKalStatus := '           USER';
  end
  else
   ReadKalStatus := '';
end;



Procedure ReadIdString(flag : boolean);         {F4}
{flag = true     resultat vises pÜ skërm
 flag = false    resultat vises ikke pÜ skërm
 Firmanavn skrives til CompanyIDStr
 SW rev skrives til SWRevStr
 }
var  Svar  : string;
begin
 ReadIdStringOk := false;
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 CompanyIDStr := Copy(Svar,1,7);
 SWRevStr := Copy(Svar,31,4);

 (*
 'PHILIPS,400810979300,KU'    {5639/90 serie III,IV}
 'PHILIPS,400810978930,KU'    {5639/92 serie I,II}
 'PHILIPS,400810978930,KU'    {5639/916}
 'PHILIPS,400810983120,KU'    {5639/93}

 fra rettelsesdato = 990212
 'PTV    ,400810979300,KU',     {5639/90 serie III,IV,V}
 'PTV    ,400810978930,KU',     {5639/92 serie I,II,III}
 'PTV    ,400810978930,KU',     {5639/916}
 'PTV    ,400810983120,KU');    {5639/93 serie II,III}
 *)

 case SensorVer of
  90 : begin
        ReadIdStringOk := (Length(Svar) = 34) and
                          (Pos(Copy(Svar,8,15),PM5639IdStr[1]) = 8) and
                          (Svar[1] = 'P') and
                          ((SWRevStr = SensorSWRev[SensorSerie-2]) or
                           (SWRevStr = SensorSWRev[SensorSerie-3]) or
                           (SWRevStr = SensorSWRev[4]));
       end;
  92 : begin
        ReadIdStringOk := (Length(Svar) = 34) and
                          (Pos(Copy(Svar,8,15),PM5639IdStr[2]) = 8) and
                          (Svar[1] = 'P') and
                          ((SWRevStr = SensorSWRev[SensorSerie]) or
                           (SWRevStr = SensorSWRev[SensorSerie-1]) or
                           (SWRevStr = SensorSWRev[4]));
       end;

  93 : begin
        ReadIdStringOk := (Length(Svar) = 34) and
                          (Pos(Copy(Svar,8,15),PM5639IdStr[4]) = 8) and
                          (Svar[1] = 'P') and
                          ((SWRevStr = SensorSWRev[2]) or
                           (SWRevStr = SensorSWRev[4]));
       end;

  916 : begin
         if (Length(Svar) = 34) and
            (Pos(Copy(Svar,8,15),PM5639IdStr[3]) = 8) and
            (Svar[1] = 'P') and
            ((SWRevStr = SensorSWRev[2]) or      { ver 3.2 }
             (SWRevStr = SensorSWRev[3]) or      { ver 3.0 }
             (SWRevStr = SensorSWRev[4])) then   { ver 3.3 }
            ReadIdStringOk := true;
        end;
 end; { case }
 if flag then
  begin
   write('ID: ',Svar);
   TrueOrFalse(ReadIdStringOk);
     if not ReadIdStringOk then
      begin
       case SensorVer of
        90:  begin
              writeln('  Correct ID-string:     ',Copy(PM5639IdStr[1],1,23),'xxxxxx');
              writeln('  Correct sensor SW rev: ',SensorSWRev[SensorSerie-2],' eller ',SensorSWRev[4]);
             end;
        92:  begin
              writeln('  Correct ID-string:     ',Copy(PM5639IdStr[2],1,23),'xxxxxx');
              writeln('  Correct sensor SW rev: ',SensorSWRev[SensorSerie],' eller ',SensorSWRev[4]);
             end;
        93:  begin
              writeln('  Correct ID-string:     ',Copy(PM5639IdStr[4],1,23),'xxxxxx');
              writeln('  Correct sensor SW rev: ',SensorSWRev[2],' eller ',SensorSWRev[4]);
             end;
        916: begin
              writeln('  Correct ID-string:     ',Copy(PM5639IdStr[2],1,23),'xxxxxx');
              writeln('  Correct sensor SW rev: ',SensorSWRev[2],' eller ',SensorSWRev[3],' eller ',SensorSWRev[4]);
             end;
       end; {case}
      end;
    end;
end;



Procedure writeOKToEEPROM;
begin
 writeln('"OK" to E˝PROM');
 MXModePM5639;
 writeEEPROM(56,Ord('O'));
 writeEEPROM(57,Ord('K'));
end;

Procedure writenot_OKToEEPROM;
begin
 writeln('"not OK" to E˝PROM');
 MXModePM5639;
 writeEEPROM(56,Ord('-'));
 writeEEPROM(57,Ord('-'));
end;


FUNCTION ReadKU : string;
{Lëser KU-nummer fra E˝PROM}
var  Svar : string;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(40);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Delay(100);
 if COMStatus = 0 then
   ReadKU := Copy(Svar,24,6)
  else
   ReadKU := '';
end;

FUNCTION ReadSWRev : string;
{Lëser SWRev fra E˝PROM}
var  Svar : string;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Delay(100);
 ReadSWRev := Copy(Svar,31,4);
end;

Function Read12NC : string;
{Lëser 12NC-nummer fra E˝PROM}
var  Svar : string;
begin
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'I?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Delay(100);
 Read12NC := Copy(Svar,9,12);
end;



Function ReadTraceFlag : string;
{Lëser traceability status for sensoren}
var  Svar : string;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA96,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Delay(20);
 if Svar = '255' then
  ReadTraceFlag := '      NPL'
 else
  ReadTraceFlag := '      NIST';
end;



Function ReadOutputFormat : string;
{Lëser outputformat for sensoren}
var  Svar : string;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA95,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,'); Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Delay(20);
 if Svar = '255' then
  ReadOutputFormat:= '     R,G,B,I'
 else
 if Svar = '000' then
  ReadOutputFormat:= '     R,G,B'
 else
  ReadOutputFormat:= '????';
end;


Procedure  ReadWPAddr(Flag : boolean);
var Svar   : string;
    DLow,
    DHigh  : byte;
    k      : integer;
begin
 write('Write-protect address: ');
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA54,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');
 Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,DLow,k);
 ReadwriteProtectOk := k = 0;
 COMStatus := writeCOMPort(COMNo,'RM,');
 Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,DHigh,k);
{$R-}
 writeProtectAddr := DHigh * 256 + DLow;
{$R+}
 ReadwriteProtectOk := ReadwriteProtectOk and
                       (writeProtectAddr > 2) and
                       (k = 0);
 write(writeProtectAddr);
 TrueOrFalse(ReadwriteProtectOk);
end;


Procedure  ReadInternID;
var Svar   : string;
    k : integer;
begin
 Svar := '';
 write('Internal ID: ');
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA58,'); Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,InternIDRead[1],k);
 COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,InternIDRead[2],k);
 COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,InternIDRead[3],k);
 COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,InternIDRead[4],k);

 if (InternIDRead[1] = 255) and
    (InternIDRead[2] = 255) and
    (InternIDRead[3] = 255) and
    (InternIDRead[4] = 255) then
   begin
    writeln('       ',InternIDRead[1]:4,InternIDRead[2]:4,InternIDRead[3]:4,InternIDRead[4]:4);
    exit;
   end;

 ReadInternIDOk := (InternIDRead[1] = 1) and
                   (InternIDRead[2] = 0) and
                   (InternIDRead[3] = 0);
 case SensorVer of
  90  : case SensorSerie of
         3 : ReadInternIDOk := ReadInternIDOk and (DefInternID[1,4] = InternIDRead[4]);
         4 : ReadInternIDOk := ReadInternIDOk and (DefInternID[2,4] = InternIDRead[4]);
        end;

  92  : case SensorSerie of
         1 : ReadInternIDOk := ReadInternIDOk and (DefInternID[5,4] = InternIDRead[4]);
         2 : ReadInternIDOk := ReadInternIDOk and (DefInternID[6,4] = InternIDRead[4]);
        end;

  93  : ReadInternIDOk := ReadInternIDOk and (DefInternID[9,4] = InternIDRead[4]);

  916 : ReadInternIDOk := ReadInternIDOk and (DefInternID[13,4] = InternIDRead[4]);
 end;

 write('       ',InternIDRead[1]:4,InternIDRead[2]:4,InternIDRead[3]:4,InternIDRead[4]:4);
 TrueOrFalse(ReadInternIDOk);
 if not ReadInternIDOk then
  begin
   write('Correct intern ID:');
   case SensorVer of
    90  : case SensorSerie of
           3 : writeln(DefInternID[1,1]:4,DefInternID[1,2]:4,DefInternID[1,3]:4,DefInternID[1,4]:4);
           4 : writeln(DefInternID[2,1]:4,DefInternID[2,2]:4,DefInternID[2,3]:4,DefInternID[2,4]:4);
          end;
    92  : case SensorSerie of
           1 : writeln(DefInternID[5,1]:4,DefInternID[5,2]:4,DefInternID[5,3]:4,DefInternID[5,4]:4);
           2 : writeln(DefInternID[6,1]:4,DefInternID[6,2]:4,DefInternID[6,3]:4,DefInternID[6,4]:4);
          end;
    93  : writeln(DefInternID[9,1]:4,DefInternID[9,2]:4,DefInternID[9,3]:4,DefInternID[9,4]:4);
    916 : writeln(DefInternID[13,1]:4,DefInternID[13,2]:4,DefInternID[13,3]:4,DefInternID[13,4]:4);
   end; { case }
   TestFailed := true;
  end;
end;



Procedure  ReadNoEEPROMPages;
var Kode   : integer;
    Svar   : string;
begin
 Svar := '';
 write('Number of E˝PROM sides: ');
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA52,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');
 Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,NoEEPROMPages,Kode);
 if Kode = 0 then
  begin
   ReadNoEEPROMPagesOk := DefNoEEPROMPages = NoEEPROMPages;
   write(NoEEPROMPages:12);
   TrueOrFalse(ReadNoEEPROMPagesOk);
  end
 else
  begin
   writeln('Read Failure !!');
   ReadNoEEPROMPagesOk := false;
   TestFailed := true;
  end; {if}
 if not ReadNoEEPROMPagesOk then
  TestFailed := true;
end;



Function TestAfbrudt : boolean;
{ Afbryder totaltest hvis der tastes pÜ SPACE }
begin
 if KeyPressed then
  TestAfbrudt := ReadKey = SPACE
 else
  TestAfbrudt := false;
 EmptyKeyboardBuffer;
end;



FUNCTION FinalTestOk : boolean;
{Returnerer true hvis "OK" er skrevet i adresse 56 og 57
 i E˝PROM - ellers returneres false}
var Svar   : string;
    D1,D2  : STR_5;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA56,');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 D1 := Svar;
 COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 D2 := Svar;
 FinalTestOk := (D1 = '079') and (D2 = '075') and (COMStatus = 0);
end;




Procedure ShowOffsetValues(Flag : boolean);    {CtrlF6}
{viser offset-vërdierne for gain 0-5 for RGB-kanalerne (kanal #4 vises ikke).
 Resultatet vises i mV ved en AD-reference = ADRef, som svarer til ADCounts.
 Se definitioner i CONST.
 Hvis Flag = false vises intet pÜ skërmen}
var
 Instr,
 Svar        : string;
 Data,
 n,k         : byte;
 Kode,
 Os          : integer;
begin
 FillChar(Offset,SizeOf(Offset),0);
 n := 1;
 Svar[0] := #40;
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA121,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 repeat
  COMStatus := writeCOMPort(COMNo,'RM,');
  Delay(10);
  COMStatus := ReadlnCOMPort(COMNo,InStr,1);
  Delay(10);
  Val(Instr,Data,Kode);
  Svar[n] := Chr(Data);
  Inc(n);
 until (n > 48) or (COMStatus <> 0) or (Kode <> 0);
 Svar[0] := Chr(n);

 k := 0;
 if COMStatus <> 0 then
   writeln(Bell,'Timeout - no response from sensor!')
  else
 begin
  if Flag then
   writeln('            Red     Green   Blue            Red         Green      Blue');
   repeat
   if Flag then
    write('Gain ',k,':');    {k selects gain 0..5}

    for n := 0 to 2 do       {n selects R,G,B}
     begin
      OffsetData[k,n*2]   := Ord(Svar[k*2+n*12+1]);
      OffsetData[k,n*2+1] := Ord(Svar[k*2+n*12+2]);

      {$R-}
      Os := Ord(Svar[k*2+n*12+1]) + 256 * Ord(Svar[k*2+n*12+2]);
      if Os < 0 then
       Os := -1 * (32768 + Os);
      {$R+}

      OffSet[k,n] := Os * 1E3 * ADRef / ADCounts;   {til mV}
                                          {OffSet array bruges ved ST-mÜlinger}

      if Flag then
       write(OffSet[k,n]:8:2);
     end; {for}

    if Flag then
     begin
      write(' mV');
      write(OffsetData[k,0]:12);
      write(OffsetData[k,1]:4);
      write(OffsetData[k,2]:8);
      write(OffsetData[k,3]:4);
      write(OffsetData[k,4]:8);
      writeln(OffsetData[k,5]:4);
     end;

    Inc(k);                            {next gain #}
   until k > 5;                        {until gain 5}
 end; {if}
end;  {ShowOffsetValues}




Procedure BeregnGainFactor;
{der mÜles i MX-mode og MC-mode. Default gain faktorer skrives til E˝PROM.
 MÜleresultatet midles og korrektionsfaktorerne K1 - K5 beregnes.
 Derefter beregnes gainfaktorerne GA1 - GA5. Disse sammenlignes med de
 nominelle gainfaktorer, som er givet i konstanterne GANom[1] - GANom[5].
 Er tolerancerne overholdt, skrives de nye gainfaktorer til E˝PROM.
 Konstanten LightLevel skal defineres i hovedprogrammet.
}
var  Svar        : string;
     Status,
     n,k         : byte;
     KorrFactor  : array[1..3,1..5] of real;
     GainFactor  : array[1..3,1..5] of WORD;
     GA          : array[1..5] of WORD;
     MeanRed,
     MeanGreen,
     MeanBlue    : array[1..2] of real;
     St,
     SensitivityFlag,
     Overflow    : boolean;
begin
 FillChar(KorrFactor,SizeOf(KorrFactor),0);
 FillChar(GainFactor,SizeOf(GainFactor),0);
 Beep(800,200);
 writeln('Place measuring head on light box - press any key');
 WaitForAnyKey;
 writeDefaultGainFactor;
 LampOn;
 SensitivityFlag := false;

 COMStatus := writeCOMPort(COMNo,'MS,'); (* measure stop *)
 COMStatus := writeCOMPort(COMNo,'NR,'); (* return to normal mode *)
 TrmDA8591(I2CIOAddr,144,LightLevel[1],Status);
 COMStatus := writeCOMPort(COMNo,'MX,'); (* test mode: unmodified sensor outputs*)

 COMStatus := writeCOMPort(COMNo,'MC,'); (* transmit continously data *)
 writeln('R':10,'G':8,'B':8,'       Nom Ò',KorrFactorTol:2,'%',
         'R':11,'G':8,'B':8);


 COMStatus := writeCOMPort(COMNo,'FG0,'); (* JK fix gain *)
 Delay(jk_delay);
 n := 1;
 Overflow := false;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 CalculateMean(MeasStr,AntalMeas,MeanRed[1],MeanGreen[1],MeanBlue[1],St);

 if (MeanRed[1] = 0) or (MeanGreen[1] = 0) or (MeanBlue[1] = 0) then
  Overflow := true;

  FilterOk := (MeanRed[1] > MinValueK[1]) and
              (MeanGreen[1] > MinValueK[1]) and
              (MeanBlue[1] > MinValueK[1]);

  If not FilterOk then
   begin
    TestFailed := true;
    write('Too low results   ');
    writeln('R:',MeanRed[1]:7:1,'    G:',MeanGreen[1]:7:1,'    B:',MeanBlue[1]:7:1);
    writeln('Correct results :       R > ',MinValueK[1],'      G > ',MinValueK[1],'     B > ',MinValueK[1]);
    if Overflow then
     writeln('Failure, check resistors/photodiodes(BPW20)');
    COMStatus := writeCOMPort(COMNo,'MS,NR,');
    LampOff;
    Exit;
   end;

 write('K1: ');
 COMStatus := writeCOMPort(COMNo,'FG1;');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until n = AntalMeas + 1;

 CalculateMean(MeasStr,AntalMeas,MeanRed[2],MeanGreen[2],MeanBlue[2],St);

 if (MeanRed[2] = 0) or (MeanGreen[2] = 0) or (MeanBlue[2] = 0) then
  Overflow := true;

 if MeanRed[1] <> 0 then
  KorrFactor[1,1] := MeanRed[1] / MeanRed[2];
 if MeanGreen[1] <> 0 then
  KorrFactor[2,1] := MeanGreen[1] / MeanGreen[2];
 if MeanBlue[1] <> 0 then
  KorrFactor[3,1] := MeanBlue[1] / MeanBlue[2];
 write(KorrFactor[1,1]:8:3,KorrFactor[2,1]:8:3,KorrFactor[3,1]:8:3);
 write(KorrNom[1]:10:2,'        ');
 writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);

 if TestAfbrudt then     { space key pressed }
  begin
   TestFailed := true;
   writeln('Calibration cancelled');
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;

 if MeanRed[1] > 1800 then
  SensitivityFlag := true; { sërlig fõlsom sensor: lys fra lyskasse }
                           { reduceres ved gainkalibrering af K4 og K5 }

 write('K2: ');
 TrmDA8591(I2CIOAddr,144,LightLevel[2],Status);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'FG1;');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[1],MeanGreen[1],MeanBlue[1],St);

 if (MeanRed[1] = 0) or (MeanGreen[1] = 0) or (MeanBlue[1] = 0) then
  Overflow := true;

 COMStatus := writeCOMPort(COMNo,'FG2;');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[2],MeanGreen[2],MeanBlue[2],St);

 if (MeanRed[2] = 0) or (MeanGreen[2] = 0) or (MeanBlue[2] = 0) then
  Overflow := true;

 if MeanRed[2] <> 0 then
  KorrFactor[1,2] := MeanRed[1] / MeanRed[2];
 if MeanGreen[2] <> 0 then
  KorrFactor[2,2] := MeanGreen[1] / MeanGreen[2];
 if MeanBlue[2] <> 0 then
  KorrFactor[3,2] := MeanBlue[1] / MeanBlue[2];
 write(KorrFactor[1,2]:8:3,KorrFactor[2,2]:8:3,KorrFactor[3,2]:8:3);
 write(KorrNom[2]:10:2,'        ');
 writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);

 if OverFlow then
  begin
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);
   writeln('Overflow - check photodiodes/resistors');
   TestFailed := true;
   Exit;
  end;


 if TestAfbrudt then
  begin
   TestFailed := true;
   writeln('Calibration interrupted');
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;

  FilterOk := (MeanRed[1] > MinValueK[2]) and
              (MeanGreen[1] > MinValueK[2]) and
              (MeanBlue[1] > MinValueK[2]);

  If not FilterOk then
   begin
    TestFailed := true;
    write('For lave mÜlevërdier   ');
    writeln('R:',MeanRed[1]:7:1,'    G:',MeanGreen[1]:7:1,'    B:',MeanBlue[1]:7:1);
    writeln('Correcte values :       R > ',MinValueK[2],'      G > ',MinValueK[2],'     B > ',MinValueK[2]);
    COMStatus := writeCOMPort(COMNo,'MS,NR,');
    LampOff;
    Exit;
   end;





 write('K3: ');
 TrmDA8591(I2CIOAddr,144,LightLevel[3],Status);
 COMStatus := writeCOMPort(COMNo,'FG2,');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[1],MeanGreen[1],MeanBlue[1],St);

 if (MeanRed[1] = 0) or (MeanGreen[1] = 0) or (MeanBlue[1] = 0) then
  Overflow := true;

 COMStatus := writeCOMPort(COMNo,'FG3,');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[2],MeanGreen[2],MeanBlue[2],St);


 if (MeanRed[2] = 0) or (MeanGreen[2] = 0) or (MeanBlue[2] = 0) then
  Overflow := true;

 if MeanRed[2] <> 0 then
  KorrFactor[1,3] := MeanRed[1] / MeanRed[2];
 if MeanGreen[2] <> 0 then
  KorrFactor[2,3] := MeanGreen[1] / MeanGreen[2];
 if MeanBlue[2] <> 0 then
  KorrFactor[3,3] := MeanBlue[1] / MeanBlue[2];
 write(KorrFactor[1,3]:8:3,KorrFactor[2,3]:8:3,KorrFactor[3,3]:8:3);
 write(KorrNom[3]:10:2,'        ');
 writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);

 if OverFlow then
  begin
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);
   writeln('Overflow - check photodiodes/resistors');
   TestFailed := true;
   Exit;
  end;

 if TestAfbrudt then
  begin
   TestFailed := true;
   writeln('Calibration terminated');
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;
  FilterOk := (MeanRed[1] > MinValueK[3]) and
              (MeanGreen[1] > MinValueK[3]) and
              (MeanBlue[1] > MinValueK[3]);

  If not FilterOk then
   begin
    TestFailed := true;
    write('Too low results   ');
    writeln('R:',MeanRed[1]:7:1,'    G:',MeanGreen[1]:7:1,'    B:',MeanBlue[1]:7:1);
    writeln('Correcte values :       R > ',MinValueK[3],'      G > ',MinValueK[3],'     B > ',MinValueK[3]);
    COMStatus := writeCOMPort(COMNo,'MS,NR,');
    LampOff;
    Exit;
   end;



 write('K4: ');
  { SensitivityFlag: sërlig fõlsom sensor - lys fra lyskasse }
  { reduceres ved gainkalibrering af K4 og K5 }
 if SensitivityFlag then
   TrmDA8591(I2CIOAddr,144,LightLevel[4]-20,Status)
  else
   TrmDA8591(I2CIOAddr,144,LightLevel[4],Status);
 COMStatus := writeCOMPort(COMNo,'FG3,');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  if (Svar[0] <> NULL) and (Svar[1] = '-') then
   Overflow := true;
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[1],MeanGreen[1],MeanBlue[1],St);

 if (MeanRed[1] = 0) or (MeanGreen[1] = 0) or (MeanBlue[1] = 0) then
  Overflow := true;

 COMStatus := writeCOMPort(COMNo,'FG4,');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  if (Svar[0] <> NULL) and (Svar[1] = '-') then
   Overflow := true;
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[2],MeanGreen[2],MeanBlue[2],St);

 if (MeanRed[2] = 0) or (MeanGreen[2] = 0) or (MeanBlue[2] = 0) then
  Overflow := true;

 if MeanRed[2] <> 0 then
  KorrFactor[1,4] := MeanRed[1] / MeanRed[2];
 if MeanGreen[2] <> 0 then
  KorrFactor[2,4] := MeanGreen[1] / MeanGreen[2];
 if MeanBlue[2] <> 0 then
  KorrFactor[3,4] := MeanBlue[1] / MeanBlue[2];
 write(KorrFactor[1,4]:8:3,KorrFactor[2,4]:8:3,KorrFactor[3,4]:8:3);
 write(KorrNom[4]:10:2,'        ');
 writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);


 if OverFlow then
  begin
   TestFailed := true;
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);
   writeln('Overflow - kontrollÇr fotodioder/modstande');
   Exit;
  end;

 if TestAfbrudt then
  begin
   TestFailed := true;
   writeln('Calibratiion terminated');
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   TrmDA8591(I2CIOAddr,144,100,Status);
   LampOff;
   Exit;
  end;

  FilterOk := (MeanRed[1] > MinValueK[4]) and
              (MeanGreen[1] > MinValueK[4]) and
              (MeanBlue[1] > MinValueK[4]);

  If not FilterOk then
   begin
    TestFailed := true;
    write('Too low results   ');
    writeln('R:',MeanRed[1]:5:1,'    G:',MeanGreen[1]:5:1,'    B:',MeanBlue[1]:5:1);
    writeln('Correct  values :       R > ',MinValueK[4],'      G > ',MinValueK[4],'     B > ',MinValueK[4]);
    COMStatus := writeCOMPort(COMNo,'MS,NR,');
    LampOff;
    Exit;
   end;


 write('K5: ');
  { SensitivityFlag: sërlig fõlsom sensor - lys fra lyskasse }
  { reduceres ved gainkalibrering af K4 og K5 }
 if SensitivityFlag then
   TrmDA8591(I2CIOAddr,144,LightLevel[5]-10,Status)
  else
   TrmDA8591(I2CIOAddr,144,LightLevel[5],Status);
 COMStatus := writeCOMPort(COMNo,'FG4,');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  if (Svar[0] <> NULL) and (Svar[1] = '-') then
   Overflow := true;
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[1],MeanGreen[1],MeanBlue[1],St);

 if (MeanRed[1] < 5) or (MeanGreen[1] < 5) or (MeanBlue[1] < 5) then
  begin
   TrmDA8591(I2CIOAddr,144,LightLevel[5]+8,Status);
   Delay(jk_delay);
   n := 1;
   COMStatus := ClearInputQue(COMNo);
   COMStatus := ReadlnCOMPort(COMNo,Svar,1);
   repeat
    COMStatus := ReadlnCOMPort(COMNo,Svar,1);
    if (Svar[0] <> NULL) and (Svar[1] = '-') then
     Overflow := true;
    MeasStr[n] := Svar;
    Inc(n);
   until (n = AntalMeas + 1) or OverFlow;

   CalculateMean(MeasStr,AntalMeas,MeanRed[1],MeanGreen[1],MeanBlue[1],St);

  end;


 if (MeanRed[1] = 0) or (MeanGreen[1] = 0) or (MeanBlue[1] = 0) then
  Overflow := true;

 COMStatus := writeCOMPort(COMNo,'FG5,');
 Delay(jk_delay);
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  if (Svar[0] <> NULL) and (Svar[1] = '-') then
   Overflow := true;
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or OverFlow;

 CalculateMean(MeasStr,AntalMeas,MeanRed[2],MeanGreen[2],MeanBlue[2],St);

 if (MeanRed[2] = 0) or (MeanGreen[2] = 0) or (MeanBlue[2] = 0) then
  Overflow := true;

 if MeanRed[2] <> 0 then
  KorrFactor[1,5] := MeanRed[1] / MeanRed[2];
 if MeanGreen[2] <> 0 then
  KorrFactor[2,5] := MeanGreen[1] / MeanGreen[2];
 if MeanBlue[2] <> 0 then
  KorrFactor[3,5] := MeanBlue[1] / MeanBlue[2];
 write(KorrFactor[1,5]:8:3,KorrFactor[2,5]:8:3,KorrFactor[3,5]:8:3);
 write(KorrNom[5]:10:2,'        ');
 writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);

 if OverFlow then
  begin
   TestFailed := true;
   COMStatus := writeCOMPort(COMNo,'MS,NR,');
   writeln(MeanRed[1]:8:1,MeanGreen[1]:8:1,MeanBlue[1]:8:1);
   writeln('Overflow - check photodiodes/resistors');
   LampOff;
   Exit;
  end;

  FilterOk := (MeanRed[1] > MinValueK[5]) and
              (MeanGreen[1] > MinValueK[5]) and
              (MeanBlue[1] > MinValueK[5]);

  If not FilterOk then
   begin
    TestFailed := true;
    write('Too low results   ');
    writeln('R:',MeanRed[1]:5:1,'    G:',MeanGreen[1]:5:1,'    B:',MeanBlue[1]:5:1);
    writeln('Correct values  :       R > 2      G > 2     B > 2');
    COMStatus := writeCOMPort(COMNo,'MS,NR,');
    LampOff;
    Exit;
   end;


{----------------------------------- beregninger ---------------------------}

 COMStatus := writeCOMPort(COMNo,'MS,NR,');
 LampOff;

 BeregnGainFactorOk := true;
 for n := 1 TO 3 DO
  for k := 1 TO 5 DO
   if ((KorrNom[k] * (1 + KorrFactorTol/100)) < KorrFactor[n,k]) OR
      ((KorrNom[k] * (1 - KorrFactorTol/100)) > KorrFactor[n,k]) then
     BeregnGainFactorOk := false;

 if BeregnGainFactorOk then
  begin

{$R-}   {hvis KorrFactor[n,x] bliver meget stor, fÜs runtime error pga}
        {range error ved beregning af GainFactor[n,x]}
   for n := 1 TO 3 DO
    begin
     GainFactor[n,1] := Trunc(KorrFactor[n,1] * GANom[1]);
     GainFactor[n,2] := Trunc(KorrFactor[n,1] * KorrFactor[n,2] * GANom[2]);
     GainFactor[n,3] := Trunc(KorrFactor[n,1] * KorrFactor[n,2] * KorrFactor[n,3]
                            * GANom[3]);
     GainFactor[n,4] := Trunc(KorrFactor[n,1] * KorrFactor[n,2] * KorrFactor[n,3]
                            * KorrFactor[n,4] * GANom[4]);
     GainFactor[n,5] := Trunc(KorrFactor[n,1] * KorrFactor[n,2] * KorrFactor[n,3]
                            * KorrFactor[n,4] * KorrFactor[n,5] * GANom[5]);
    end;
{$R+}


    MXModePM5639;
     for n := 1 To 3 DO
      for k := 1 TO 5 DO
       begin
        writeEEPROM(n*12+k*2-10,  Lo(GainFactor[n,k]));
        writeEEPROM(n*12+k*2-10+1,Hi(GainFactor[n,k]));
       end;
   end
  else
   begin
    TestFailed := true;
    writenot_OKToEEPROM;
   end;

 write('Calculated gain factors: ');
 TrueOrFalse(BeregnGainFactorOk);
 {
 for n := 1 to 5 do
 writeln('Gain ',n,': ',GainFactor[1,n]:16,GainFactor[2,n]:8,GainFactor[3,n]:8);
  }

 write('Calculated gain factors: ');
 if BeregnGainFactorOk then
   writeln(' written to E˝PROM')
  else
   writeln(' NOT written to E˝PROM');

end;  {beregngainfactor}




Procedure LearnOffset;     {F5}
var Svar   : string;
    n      : byte;
begin
 LampOff;
 Beep(1000,200);
 LearnOffsetOk := true;
 writeln('Offset calibration:');
 if SensorVer = 93 then
   write('Place measuring head in a completely black box - press any key')
  else
   write('Place measuring head with top fastened - press any key');
 WaitForAnyKey;
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MX,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MO,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 n := WhereX;
 write(' ∞∞'); GotoXY(n+1,WhereY);
 n := 0;
 repeat
  write('≤');
  COMStatus := ReadlnCOMPort(COMNo,Svar,25);
  Inc(n);
 until (n > 1) or (COMStatus <> 0);
 writeln;
 if COMStatus <> 0 then
  begin
   COMStatus := writeCOMPort(COMNo,'NR,');
   writeln(' Timeout !');
   Exit;
  end;

 COMStatus := writeCOMPort(COMNo,'NR,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MX,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MO,');
 COMStatus := ClearInputQue(COMNo);
 n := 0;
 repeat
  write('Gain ',n,': ');
  COMStatus := ReadlnCOMPort(COMNo,Svar,25);
  if COMStatus = 0 then
   writeln(Svar)
    else
   writeln(' Timeout !');
  if svar[1] = '-' then
   begin
    writeln(Bell,'Overflow ');
    LearnOffsetOk := false;
    TestFailed := true;
    exit;
   end;
  Inc(n);
 until (n > 5) or (COMStatus <> 0);
 COMStatus := writeCOMPort(COMNo,'NR,');

 if COMStatus <> 0 then
   begin
    writenot_OKToEEPROM;
    LearnOffsetOk := false;
    TestFailed := true;
   end
  else
   begin
    ShowOffsetValues(NotSilent);
    Beep(1000,200);
    write('Are the offset values OK? Y/N ');
    if ScanCode <> TastY then
     begin
      LearnOffsetOk := false;
      TestFailed := true;
     end;
   end; {if}
 TrueOrFalse(LearnOffsetOk);
end; {LearnOffset}




Procedure ReadGainFactor;           {F4}
var  n,
     g1,g2    : byte;
     k        : integer;
     Svar     : string;
     GF       : array[1..18] of WORD;
     Err      : boolean;
begin
 writeln('Gain factors: ');
 Err := false;
 ReadGainFactorOk := false;
 n := 1;
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA2,');
 COMStatus := ClearInputQue(COMNo);
 repeat
   COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
   COMStatus := ReadlnCOMPort(COMNo,Svar,1);
   Val(Svar,g1,k);
   if k <> 0 then Err := true;
   COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
   COMStatus := ReadlnCOMPort(COMNo,Svar,1);
   Val(Svar,g2,k);
   if k <> 0 then Err := true;
  {$R-}
   GF[n] := g2 * 256 + g1;
  {$R+}
   Inc(n);
 until (n > 18) or (COMStatus <> 0);

 if (not Err) then
  ReadGainFactorOk := true;

  write(' R: ');
  for n := 1 TO 6 DO
   write(GF[n]:6);
  writeln;
  write(' G: ');
  for n := 1 TO 6 DO
   write(GF[n+6]:6);
  writeln;
  write(' B: ');
  for n := 1 TO 6 DO
   write(GF[n+12]:6);
  writeln;

 TrueOrFalse(ReadGainFactorOk);
end;

Procedure RS232Test;
{Den serielle forbindelse kontrolleres}
var Svar : string;
begin
 RS232TestOk := false;
 write('RS232 test: ');
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,',MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'F?,');
 Delay(100);
 COMStatus := writeCOMPort(COMNo,'F?,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,2);
 RS232TestOk := COMStatus = 0;
 Delay(100);
 if not RS232testOk then
  if SensorVer = 916 then
    write('(9600 baud) ')
   else
    write('(4800 baud)');

 TrueOrFalse(RS232TestOk);
 if not RS232testOk then
  TestFailed := true;
end;

Procedure DarkCurrentTest;       {F6}
{Der mÜles med overpart pÜsat, hvor mÜleresultatet fra
 RGB-kanalerne ikke mÜ vëre over DarkLimitRed/Green/Blue.
 Omgivende lys skal undgÜs!!
}
var Svar       : string;
    Status     : byte;
    Kode       : integer;
    ValueRed,
    ValueGreen,
    ValueBlue,
    FRate      : real;
begin
 Beep(1000,200);
 LampOff;
 write('Test of dark current  -  press any key');
 WaitForAnyKey; writeln;
 Delay(jk_delay);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MX,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'TM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,2);
 Val(Copy(Svar,1,6),ValueRed,Kode);
 Val(Copy(Svar,8,6),ValueGreen,Kode);
 Val(Copy(Svar,15,6),ValueBlue,Kode);
  if (Kode = 0) then
 ValueRed := Abs(ValueRed / 25);
  if (Kode = 0) then
 ValueGreen := Abs(ValueGreen / 25);
  if (Kode = 0) then
 ValueBlue := Abs(ValueBlue / 25);
 DarkCurrentTestOk := (ValueRed < DarkLimitRed) and
                      (ValueGreen < DarkLimitGreen) and
                      (ValueBlue < DarkLimitBlue) and
                      (COMStatus = 0);

 write('      R:',ValueRed:6:3,'  G:',ValueGreen:6:3,'  B:',ValueBlue:6:3);
 TrueOrFalse(DarkCurrentTestOk);
 writeln('Max   R:',DarkLimitRed:6:2,'  G:',DarkLimitGreen:6:2,'  B:',DarkLimitBlue:6:2);

 if not DarkCurrentTestOk then
   TestFailed := true;
end;


Procedure EnterKU_Nummer;        {F2}
{Indtastning af apparatets KU-nummer. Kun numeriske vërdier kan indtastes.
 Tastes der kun ENTER ëndres der ikke pÜ nummeret i E˝PROM'men.
 Derefter udlëses KU-nummeret fra E˝PROM'men.
 2. ciffer skal vëre serie nr      (Eks. 031158 for serie III)
}
var    Svar       : string;
       KUStr,
       InStr         : STR_6;
       KUNo          : LONGINT;
       kode          : integer;
       n,L           : byte;
       SensorUpdate,
       ValidKUNo     : boolean;

begin
 KUStr := ReadKU;
 EmptyKeyboardBuffer;
 {Check if KU number is 900000 or 900001, which indicates that this is
  reference sensor, which must not be calibrated  d.17.05.01: JK}
 Val(KUStr, KUNo, Kode);
 if ((KUNo = 900000) or (KUNo = 900001)) then
   begin
     {writeln('Present KU-number:   ',KUStr);}
     writeln('This is reference sensor which must not be calibrated.');
     TestFailed := true;
   end
else
 begin
 SensorUpdate := false;
 repeat
  writeln('Present KU-number:   ',KUStr);
  write('Type new KU-nummer: ');
  Readln(InStr);
  Val(InStr,KUNo,Kode);
  if KUNo < 0 then Kode := 1;

  if InStr <> '' then
   begin
     case SensorVer of
      90  :  case SensorSerie of
              3 : ValidKUNo := InStr[2] = '3';
              4 : ValidKUNo := InStr[2] = '4';
              5 : ValidKUNo := InStr[2] = '5';
             end;
      92  :  case SensorSerie of
              1 : ValidKUNo := InStr[2] = '1';
              2 : ValidKUNo := InStr[2] = '2';
              3 : ValidKUNo := InStr[2] = '3';
             end;
      93  :  case SensorSerie of
              2 : ValidKUNo := InStr[2] = '2';
              3 : ValidKUNo := InStr[2] = '3';
             end;
     end; { case }

     if not ValidKUNo then
      begin
       writeln(Bell,'Wrong digits in KU-number:');
       writeln(     '  REMEMBER: 2-nd digit = series nr');
{       writeln(     '  HUSK:   2. ciffer = serie nr');?????????????????????}
       writeln(     '          ');
       writeln(     '          At customer update the KU-number shall NOT be changed');
{       writeln(     '          Ved kundeopdatering mÜ KU-nummer IKKE ëndres');?????????}
       writeln;
       write  (     'Key  F1:  Change from series ');
{       write  (     'Tast  F1:  Opdatering fra serie ');?????????????}
       case SensorVer of
        90 : writeln(     '2/3 to 4');
        92 : writeln(     '1 to 2');
       end;
       writeln(     '      F2:  New input of KU-number');
       EmptyKeyboardBuffer;
       if scancode = F1 then
        SensorUpdate := true;
      end; { if }
   end; { if }

 until ((Kode = 0) and ValidKUNo) or (InStr = '') or SensorUpdate;

 L := Length(InStr);
 if L > 0 then
  begin
   MXModePM5639;
   Str(KUNo,InStr);
   L := Length(InStr);
   for n := 1 to 6 do
     writeEEPROM(n+84,Ord('0'));
   for n := 1 to L do
     writeEEPROM((n-L+90),Ord(InStr[n]));
   writeln('KU-nummer er nu:       ',ReadKU);
   ValidKU := true;
  end
 else
  begin
   ValidKU := true;
   InStr := ReadKU;
    for n := 1 to Length(InStr) do
     begin
      if not (InStr[n] in Tal) then
       begin
        ValidKU := false;
        TestFailed := true;
       end; {if}
     end; {for}

   writeln('KU-number is not changed!    ');
  end; {if L}
 end;
end;



Procedure writeIdString;               {AltF5}
{ID-streng skrives til E˝PROM}
Var n    : byte;
begin
 writeln('ID string for sensor ');
 MXModePM5639;
 CompanyIDStr := '';
 ReadIDString(false);
 case SensorVer of
  90 :  for n := 1 to Length(PM5639IdStr[1]) do
         writeEEPROM(n+61,Ord(PM5639IdStr[1,n]));
  92 :  for n := 1 to Length(PM5639IdStr[2]) do
         writeEEPROM(n+61,Ord(PM5639IdStr[2,n]));
  93 :  for n := 1 to Length(PM5639IdStr[4]) do
         writeEEPROM(n+61,Ord(PM5639IdStr[4,n]));
  916 : for n := 1 to Length(PM5639IdStr[3]) do
         writeEEPROM(n+61,Ord(PM5639IdStr[3,n]));
 end; { case }

 if CompanyIDStr = 'PHILIPS' then
  for n := 1 to 7 do
   writeEEPROM(n+61,Ord(CompanyIDStr[n]));
end;

Procedure writePHILIPS_ID;
var n : byte;
begin
 CompanyIDStr := 'PHILIPS';
 for n := 1 to 7 do
  writeEEPROM(n+61,Ord(CompanyIDStr[n]));
 CompanyIDStr := '';
 writeln('Company = PHILIPS');
end;

{JKJKJKJKJKJKJKJKJKJK}

Procedure WriteCalDate;
{Cal dato skrives til E˝PROM adresse 187-192
 Cal dato er indtil juni 1997 skrevet til adresse 176-181.
 Dette var en fejl, idet usertext ogsÜ ligger i dette omrÜde
}
var n    : byte;
begin
 DateStr := dato;
 writeln(DateStr);
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA187,');
 Delay(20);
 for n := 1 to 6 do
  begin
   writeEEPROM(n+186,Ord(DateStr[n]));
  end;
end;

Procedure ReadCalDate(Flag : boolean);      {F4}
{Lëser fõrst cal dato fra E˝PROM fra addr 187-192.
 Hvis dato er ugyldig lëses fra addr 176-181.
 Sensorer kalibreret fra juni 1997 har dato i addr 187-192
 Hvis Flag=true udskrives til skërmen. }
var kode    : integer;
    n,
    Value   : byte;
    Svar    : string;
    ValidDate : boolean;
begin
 if Flag then
  Write('Cal date:           ');
 ReadCalDateOk := true;
 DateStr := '';
 n := 1;
 COMStatus := WriteCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := WriteCOMPort(COMNo,'MA187,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 repeat
  COMStatus := WriteCOMPort(COMNo,'RM,');
  Delay(20);
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  Delay(20);
  Val(Svar,Value,kode);
  if (kode = 0) and (Value <> 255) then
    DateStr := DateStr + Chr(Value)
   else
    DateStr := DateStr + '?';
  Inc(n);
 until (COMStatus <> 0) or (n > 6) or (kode <> 0);
 ValidDate := (DateStr[1] in ['9','0']) and
              (DateStr[3] in ['0'..'1']) and
              (DateStr[4] in ['0'..'9']) and
              (DateStr[5] in ['0'..'3']) and
              (DateStr[6] in ['0'..'9']);

 if ValidDate = false then
  begin
   ReadCalDateOk := true;
   DateStr := '';
   n := 1;
   COMStatus := WriteCOMPort(COMNo,'MS,');
   Delay(20);
   COMStatus := WriteCOMPort(COMNo,'MA176,');
   Delay(20);
   COMStatus := ClearInputQue(COMNo);
   repeat
    COMStatus := WriteCOMPort(COMNo,'RM,');
    Delay(20);
    COMStatus := ReadlnCOMPort(COMNo,Svar,1);
    Delay(20);
    Val(Svar,Value,kode);
    if (kode = 0) and (Value <> 255) then
      DateStr := DateStr + Chr(Value)
     else
      DateStr := DateStr + '?';
    Inc(n);
   until (COMStatus <> 0) or (n > 6) or (kode <> 0);
   ValidDate := (DateStr[1] in ['9','0']) and
                (DateStr[3] in ['0'..'1']) and
                (DateStr[4] in ['0'..'9']) and
                (DateStr[5] in ['0'..'3']) and
                (DateStr[6] in ['0'..'9']);
  end; { if ValidDate }

 if Flag then
  writeln(DateStr)
end;



Procedure WriteRefMonitor;
{Flag for ref monitor skrives til E˝PROM adresse 193
 255: intet angivet, sandsynligvis EBU
   1: EBU  monitor
   2: SMPTE  --
}
begin
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 if MonitorType = MonEBU then
   writeEEPROM(193,Ord(1))
  else
   writeEEPROM(193,Ord(2));
end;



Procedure ReadRefMonitor(Flag : boolean);      {F4}
{Lëser indhold i E˝PROM fra addr 193 til variabel RefMonitorValue.
 Hvis Flag=true udskrives til skërmen. }
var kode    : integer;
    Svar    : string;
begin
 if Flag then
  Write('Ref monitor:        ');
 ReadRefMonOk := true;
 DateStr := '';
 COMStatus := WriteCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := WriteCOMPort(COMNo,'MA193,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := WriteCOMPort(COMNo,'RM,');
 Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 Val(Svar,RefMonitorValue,kode);

 if ((length(Svar) = 0) or (kode <> 0)) then
  ReadRefMonOk := false;


 if Flag then
  if RefMonitorValue in [1,255] then
    writeln(RefMonitorValue,' = EBU')
   else
  if RefMonitorValue = 2 then
    writeln(RefMonitorValue,' = SMPTE')
   else
    writeln(Bell,RefMonitorValue, ' = ????');
end; {ReadRefMonitor}



Procedure ReadPreGainFactor(Flag : boolean);      {F4}
{Lëser pre gain fra E˝PROM fra addr 97-102.
 Hvis Flag=true udskrives Pregain til skërmen. }
var Kode    : INTEGER;
    PreGain : array[1..6] OF BYTE;
    n,
    Value   : BYTE;
    Svar    : string;
    Yf      : WORD;
begin
 if Flag then
  Write('Pre gain: ');
 ReadPreGainOk := true;
 FillChar(PreGain,SizeOf(PreGain),0);
 n := 1;
 COMStatus := WriteCOMPort(COMNo,'MS,');   Delay(20);
 COMStatus := WriteCOMPort(COMNo,'MA97,'); Delay(20);
 COMStatus := ClearInputQue(COMNo);
 repeat
  COMStatus := WriteCOMPort(COMNo,'RM,');
  Delay(20);
  COMStatus := ReadlnCOMPort(COMNo,Svar,1);
  Delay(20);
  Val(Svar,Value,Kode);
  if Kode = 0 then
   PreGain[n] := Value
  else
   ReadPreGainOk := false;
  Inc(n);
 until (COMStatus <> 0) or (Kode <> 0) or (n > 6);
{$R-}
 Yf := PreGain[3] * 256 + PreGain[4];
{$R+}
 if Yf > 0 then
  begin
   XFactRead := (PreGain[1] * 256 + PreGain[2]) / Yf;
   YFactRead := Yf / PreGainMulFactor;;
   ZFactRead := (PreGain[5] * 256 + PreGain[6]) / Yf;
   XFact := XFactRead;
   YFact := YFactRead;;
   ZFact := ZFactRead;
   if Flag then
    Writeln(XFact:8:3,YFact:8:3,ZFact:8:3);
  end
 else
  begin
    if Flag then
     begin
      Write(' Fail  >>> ');
      for n := 1 TO 6 DO
       Write(PreGain[n]:4);
      Writeln;
     end;
   ReadPreGainOk := false;
   TestFailed := true;
  end;

 if NOT ReadPreGainOk then
  TestFailed := true;
end;



Procedure WriteProtectYesOrNo(Flag: boolean);    {CtrlF4}
{undersõger om skriveskyttelsen er ON eller OFF ved at prõve at ëndre data
 i adresse 1.
 Hvis Flag = false vises ingen udskrift pÜ skërmen}
var ReadByte,
    writeByte  : byte;
    k          : integer;
    Svar       : string;
begin
 if Flag then
  write('Write-protected ?   ');
 MXModePM5639;
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA1,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'RM,');
 Delay(20);
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 if COMStatus = 0 then
  begin
   Val(Svar,ReadByte,k);
   writeByte := ReadByte XOR $FF;
   writeEEPROM(1,writeByte);
   COMStatus := writeCOMPort(COMNo,'MA1,');  Delay(20);
   COMStatus := ClearInputQue(COMNo);
   COMStatus := writeCOMPort(COMNo,'RM,');   Delay(20);
   COMStatus := ReadlnCOMPort(COMNo,Svar,1);
   Val(Svar,ReadByte,k);
   writeProtectOk := ReadByte <> writeByte;
   if Flag then
    if writeProtectOk then
      writeln('YES')
     else
      writeln('NO');
  end
  else
    writeln(' Error reading measuring head!!');
end;



Procedure CheckSWBaudrateShift;
var  Svar   : string;
begin
 COMStatus := writeCOMPort(COMNo,'MS,');  Delay(20);
 BaudrateShiftOk := true;

 write('Baudrate skift til 9600  .');
 COMStatus := writeCOMPort(COMNo,'SB96,');  Delay(20);
 COMStatus := CloseCOMPort(COMNo);
 COMStatus := OpenCOMPort(COMNo,InQueSize,OutQueSize);
 COMStatus := InitCOMPort(COMNo,_Baud9600,_Databit8,_Stopbit2,_None,_CTSNotRequired);
 write('.');
 Delay(1000);
 write('. ');
 ReadIdString(false);
 if ReadIdStringOk = false then
  BaudrateShiftOk := false;

 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'MA40,'); Delay(100);
 COMStatus := writeCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 if ((COMStatus <> 0) or (Svar <> '250')) then
  BaudrateShiftOk := false;

 if BaudrateShiftOk = true then
   writeln('OK')
  else
   begin
   write('FAIL');
    if Svar <> '250' then
      writeln('   (EEPROM addr 40 = ',Svar,'    skal vëre = 250)')
     else
      writeln;
   end;

 write('Baudrate skift til 4800  .');
 COMStatus := writeCOMPort(COMNo,'SB48,');
 COMStatus := CloseCOMPort(COMNo);
 COMStatus := OpenCOMPort(COMNo,InQueSize,OutQueSize);
 COMStatus := InitCOMPort(COMNo,_Baud4800,_Databit8,_Stopbit2,_None,_CTSNotRequired);
 write('.');
 Delay(1000);
 write('. ');
 ReadIdString(false);
 if ReadIdStringOk = false then
  BaudrateShiftOk := false;

 COMStatus := ClearInputQue(COMNo);
 COMStatus := writeCOMPort(COMNo,'MA40,'); Delay(20);
 COMStatus := writeCOMPort(COMNo,'RM,');
 COMStatus := ReadlnCOMPort(COMNo,Svar,1);
 if ((COMStatus <> 0) or (Svar <> '244')) then
  BaudrateShiftOk := false;

 if BaudrateShiftOk = true then
   writeln('OK')
  else
   begin
   write('FAIL');
    if Svar <> '244' then
      writeln('   (EEPROM addr 40 = ',Svar,'    shall be = 244)')
     else
      writeln;
   end;


end;






Procedure EnterLevel;   { Tast A }
var L,S : byte;
begin
{$IFDEF LYSKASSE}


 write('Enter light level 0-255: '); Readln(L);
 TrmDA8591(I2CIOAddr,144,L,S);
{$endIF}
end;

Procedure EBUWhite;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 or $07;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 EBUSetOn := true;
end;

Procedure EBURed;
begin
{$IFDEF RGBGEN}
 IICPort66 := (IICPort66 and $F8) or $01;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 EBUSetOn := true;
end;

Procedure EBUGreen;
begin
{$IFDEF RGBGEN}
 IICPort66 := (IICPort66 and $F8) or $02;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 EBUSetOn := true;
end;

Procedure EBUBlue;
begin
{$IFDEF RGBGEN}
 IICPort66 := (IICPort66 and $F8) or $04;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 EBUSetOn := true;
end;

Procedure EBUBlack;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 and $F8;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 EBUSetOn := false;
end;
(* JK : Procedures below modified by JK d. 9.05.01 for controlling
        SMPTE Monitor directly via LPT1 *)

Procedure SMPTEWhite;
begin
{$ifdef RGBGEN}
 IICPort66 := IICPort66 or $38;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endif}
 port[$378] := $07;
 SMPTESetOn := true;
end;

Procedure SMPTERed;
begin
{$ifdef RGBGEN}
 IICPort66 := (IICPort66 and $C7) or $08;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endif}
 port[$378] := $01;
 SMPTESetOn := true;
end;

Procedure SMPTEGreen;
begin
{$IFDEF RGBGEN}
 IICPort66 := (IICPort66 and $C7) or $10;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 port[$378] := $02;
 SMPTESetOn := true;
end;

Procedure SMPTEBlue;
begin
{$IFDEF RGBGEN}
 IICPort66 := (IICPort66 and $C7) or $20;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 port[$378] := $04;
 SMPTESetOn := true;
end;

Procedure SMPTEBlack;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 and $C7;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
 port[$378] := $00;
 SMPTESetOn := false;
end;

Procedure CRT625Lines;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 or $40;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
end;

Procedure CRT525Lines;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 and $BF;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
end;

Procedure RGBGenRemote;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 and $7F;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
end;

Procedure RGBGenLocal;
begin
{$IFDEF RGBGEN}
 IICPort66 := IICPort66 or $80;
 Trm8574(I2CIOAddr,66,IICPort66,St);   Delay(20);
{$endIF}
end;

Procedure WPOn;   { Ctrl F8 }
begin
 COMStatus := writeCOMPort(COMNo,'WR0,');
 Delay(100);
 writeln('write protect = ON');
end;

Procedure WPOff;   { Ctrl F9 }
begin
 COMStatus := writeCOMPort(COMNo,'MS,WR79,WR102,WR102,');
 Delay(100);
 writeln('write protect = OFF');
end;

Procedure InitBottomWindowBarco;
begin
 Window(1,24,80,25);
 TextBackground(LightGray);
 ClrScr;
 TextColor(Red); write(' SPACE:');
 TextColor(Black); write('Power ON/OFF from jack          ');
 TextColor(Red); write(' End:');
 TextColor(Black); write('Exit');
 Window(1,2,80,23);  { main window }
 Color(Yellow,Blue);
 ClrScr;
end;



Procedure InitBottomWindow;
begin
 Window(1,24,80,25);
 TextBackground(LightGray);
 ClrScr;
{$ifDEF IEEEBUS}
 TextColor(Red); write(' F1:'); TextColor(Black);
 if SensorVer <> Barco then
   write('Current')
  else
   write('5 v');
{$else}
 write('         ');
{$endif}

 if SensorVer <> Barco then
  begin
   TextColor(Red); write(' F2:'); TextColor(Black); write('KU-nr');
  end
  else
  begin
   TextColor(Red); write(' F2:'); TextColor(Black); write('Barco measure');
  end;

 if SensorVer <> Barco then
  begin
{   TextColor(Red); write(' F3:'); TextColor(Black); write('Disp.unit');}
   TextColor(Red); write(' F4:'); TextColor(Black); write('Read E˝PROM ');
   TextColor(Red); write(' F5:'); TextColor(Black); write('Offset ');
   TextColor(Red); write(' F6:'); TextColor(Black); write('Dark Current ');
   TextColor(Red); write(' H:');  TextColor(Black); writeln('Help ');
   TextColor(Red); write(' F7:'); TextColor(Black); write('Gainfactor ');
   TextColor(Red); write(' F8:'); TextColor(Black); write('Learn Phosphor ');
   TextColor(Red); write(' F9:'); TextColor(Black); write('Calibration ');

  end;
 gotoxy(54,2);
 TextColor(Red); write(' F10:'); TextColor(Black); write('Total test');
 TextColor(Red); write('  ESC:'); TextColor(Black); write('Exit');

end;


Procedure InitMainWindow;
begin
 Window(1,2,80,23);  { main window }
 Color(Yellow,Blue);
 ClrScr;
end;


Procedure SelectMonitor;
begin
(*
 Window(1,2,80,23);  { main window }
 gotoxy(20,5); Color(Red,White);
 write(' Monitor choice for sensor calibration ');
 Color(Yellow,Blue);
 gotoxy(1,7);
 writeln('    Normal calibration shall be done on EBU-monitor.');
 writeln('    Sensors for american monitors are calibrated on SMPTE-monitor.');
 writeln('    Check the process-card for reference monitor choices.');
 writeln;
 writeln('    Choose monitor: EBU        press ENTER');
 write  ('                    SMPTE      press S    ');
 if ScanCode <> TastS then
   MonitorType := MonEBU
  else
*)
   MonitorType := MonSMPTE;

 Window(1,1,80,1);  {top window}
 Color(Black,LightGray);
 gotoxy(69,1);
 if MonitorType = MonEBU then
   write('Ref = EBU')
  else
   write('Ref = SMPTE');

 InitMainWindow;
end;


Procedure InitTopWindow;
begin
 Window(1,1,80,1);
 Color(Red,LightGray);
 ClrScr;
 write(' DK-Audio A/S ');
(* write(' ProTeleVision');*)
 write('   PM5639');
 case SensorVer of
  90    : case SensorSerie of
           3 : write('/90  CRT color sensor (series III)');
           4 : write('/90  CRT color sensor (series IV)');
           5 : write('/90  CRT color sensor (series V)');
          end;
  916   : write('/916 Ind color sensor (BrÅgge)');
  92    : case SensorSerie of
           1 : write('/92  Ind. color sensor (series I)');
           2 : write('/92  Ind. color sensor (series II)');
           3 : write('/92  Ind. color sensor (series III)');
          end;
  93    : case SensorSerie of
           2 : write('/93  Miniature color sensor (series II)');
           3 : write('/93  Miniature color sensor (series III)');
          end;
  Barco : write('/02  Barco option');
 end;
end;



Procedure ReadKMatrix(Flag : boolean);    {CtrlF2}
{Lëser K-matricen fra E˝PROM, gemmer vërdierne i en variabel men viser dem
 kun pÜ skërmen hvis Flag=true}
var  n,k     : byte;
     svar    : string;
     kode    : integer;
     value   : byte;
begin
 FillChar(KorrMatrix,SizeOf(KorrMatrix),0);
 if Flag then
  writeln('K-matrix:');
 ReadKorrMatrixOk := true;
 n := 0;
 COMStatus := writeCOMPort(COMNo,'MS,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MA103,');
 Delay(20);
 COMStatus := ClearInputQue(COMNo);
 repeat
  Inc(n);
  Delay(20);
  COMStatus := writeCOMPort(COMNo,'RM,');
  Delay(20);
  COMStatus := ReadlnCOMPort(COMNo,svar,1);
  Val(svar,value,kode);
  if (kode = 0) and (COMStatus = 0) then
    KorrMatrix[n] := value
   else
    begin
     ReadKorrMatrixOk := FALSE;
     TestFailed := true;
    end;
 until (n = 18) or (COMStatus <> 0) or (Kode <> 0);

 {$R-}
 KMatr[1] := Trunc(KorrMatrix[1] * 256 + KorrMatrix[2]);
 KMatr[2] := Trunc(KorrMatrix[3] * 256 + KorrMatrix[4]);
 KMatr[3] := Trunc(KorrMatrix[5] * 256 + KorrMatrix[6]);
 KMatr[4] := Trunc(KorrMatrix[7] * 256 + KorrMatrix[8]);
 KMatr[5] := Trunc(KorrMatrix[9] * 256 + KorrMatrix[10]);
 KMatr[6] := Trunc(KorrMatrix[11] * 256 + KorrMatrix[12]);
 KMatr[7] := Trunc(KorrMatrix[13] * 256 + KorrMatrix[14]);
 KMatr[8] := Trunc(KorrMatrix[15] * 256 + KorrMatrix[16]);
 KMatr[9] := Trunc(KorrMatrix[17] * 256 + KorrMatrix[18]);
 {$R+}

 if Flag then
  begin
    for k := 1 to n DO
     write(KorrMatrix[k]:4);
   {$R-}
    writeln;
    write(KMatr[1]:8:0);
    write(KMatr[2]:10:0);
    writeln(KMatr[3]:10:0);
    write(KMatr[4]:8:0);
    write(KMatr[5]:10:0);
    writeln(KMatr[6]:10:0);
    write(KMatr[7]:8:0);
    write(KMatr[8]:10:0);
    writeln(KMatr[9]:10:0);
   {$R+}
   if not ReadKorrMatrixOk then
    writeln('Failure reading K-matrix');
  end; {if}
end;  {ReadKorrektionsMatrix}


Procedure SaveMonitorOnTime;
var Hours,Minutes,Sec,Sec100 : word;
begin
 { gem tidspunkt for monitor = on }
 GetTime(Hours,Minutes,Sec,Sec100);
 assign(MonitorTimeFile,MonitorTimeFileName);
 rewrite(MonitorTimeFile);
 writeln(MonitorTimeFile,Hours:2,Minutes:2,Sec:2);
 close(MonitorTimeFile);
end;

Procedure ReadMonitorOnTime(var ElapsedSek : word);
var Hours1,Minutes1,Sec1,Sec100,
    Hours2,Minutes2,Sec2 : word;
    Time : str_10;
    k    : integer;
begin
 { lës tid for monitor = on }
 GetTime(Hours2,Minutes2,Sec2,Sec100);
 assign(MonitorTimeFile,MonitorTimeFileName);
 {$I-}
 reset(MonitorTimeFile);
 {$I+}
 readln(MonitorTimeFile,Time);
 val(copy(Time,1,2),Hours1,k);
 val(copy(Time,3,2),Minutes1,k);
 val(copy(Time,5,2),Sec1,k);
 ElapsedSek := ((Hours2 * 3600) + Minutes2 * 60 + Sec2) -
               ((Hours1 * 3600) + Minutes1 * 60 + Sec1);

 close(MonitorTimeFile);
end;



Function GetKeycode : byte;
begin
 EmptyKeyboardBuffer;
 GetKeyCode := Scancode;
end;


Function CheckMonitorOnTime : boolean;
{ returnerer false hvis der afbrydes med ESC }
var n,
    MonitorOnTime : word;
begin
 if EBUSetOn = true then { check om monitoren har vëret hvid i minimum tid (= MinMonitorOnTime)}
  begin
   ReadMonitorOnTime(MonitorOnTime);     { sekunder }
   if MonitorOnTime < MinMonitorOnTime then
    begin
     writeln('Monitor shall be on for minimum ',MinMonitorOnTime, ' sec. before calibration');
     write('Wait ',MinMonitorOnTime-MonitorOnTime:3,' sek.     (ESC=stop)');
      for n := 1 to MinMonitorOnTime-MonitorOnTime do
       begin
        Delay(1000);
        if keypressed then
         if  Escape = Scancode then
          begin
           gotoxy(1,whereY); clreol; writeln('Stop');
           CheckMonitorOnTime := false;
           exit;
          end;
          if (n mod 5) = 0 then
           begin
            gotoxy(6,whereY);write(MinMonitorOnTime-MonitorOnTime-n:3);
           end;
       end;
     end;
  end;

 if EBUSetOn = false then { tënd monitoren og vent i minimum tid }
  begin
   EBUWhite;
   SMPTEWhite;
     writeln('Monitor shall be on for minimum ',MinMonitorOnTime, ' sec. before calibration');
   write('Vent ',MinMonitorOnTime:3,' sek.');
   for n := 1 to MinMonitorOnTime do
    begin
     Delay(1000);
        if keypressed then
         if  Escape = Scancode then
          begin
           gotoxy(1,whereY); clreol; writeln('Stop');
           CheckMonitorOnTime := false;
           exit;
          end;
      if (n mod 5) = 0 then
       begin
        gotoxy(6,whereY);write(MinMonitorOnTime-n:3);
       end;
    end;
  end;

 gotoxy(1,whereY); clreol;
 CheckMonitorOnTime := true;

end; {CheckMonitorOnTime}


{ get_light_box_data:
  This procedure reads light box DAC values from file LIGHT_BOX_FILE_NAME
  into CalLightLevel
}  
procedure get_light_box_data;
  var
     lbfile     : text;
     lbbuffer   : str_70;
     n, k       : byte;
     code       : integer;
     
  begin
   Assign(lbfile, LIGHT_BOX_FILE_NAME);
 {$I-}
   Reset(lbfile);
 {$I+}
   if IOResult <> 0 then
     begin
     Writeln(Bell, LIGHT_BOX_FILE_NAME,' not found');
     Writeln('Program terminated - press a key');
     WaitForAnyKey;
     AfslutProgram := true;
     Halt(1);
     end;
  for k := 1 to 2 do
    begin
    for n := 1 to 5 do
      begin
      Readln(lbfile, lbbuffer);
      if lbbuffer[1] = ';' then
        begin
        n := n - 1;
        continue;
        end;
      Val(lbbuffer, CalLightLevel[k, n], code);
      if code <> 0 then
        begin
        Writeln(Bell, LIGHT_BOX_FILE_NAME,' :format error.');
        Writeln('Program terminated - press a key');
        WaitForAnyKey;
        AfslutProgram := true;
        Halt(1);
        end;
      end;
    end;
  end;


Procedure InitPM5639_82Generator;
begin
 COMStatus := writeCOMPort(COMGen,'GPATT2,');   { window pattern }
 delay(100);
 COMStatus := writeCOMPort(COMGen,'GLEVL20,');  { 100 % level}
 delay(100);
 COMStatus := writeCOMPort(COMGen,'GKEY13,');   { mode = GBR}
 delay(200);
 COMStatus := writeCOMPort(COMGen,'GKEY15,');   { sync = on}
 delay(200);
 COMStatus := writeCOMPort(COMGen,'GKEY18,');   { power down = off}
 delay(200);
 COMStatus := writeCOMPort(COMGen,'GKEY19,');   { setup = on}
 delay(200);
end;





begin

end.
