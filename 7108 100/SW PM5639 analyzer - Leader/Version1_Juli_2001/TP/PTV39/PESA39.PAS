(**********************************************************************
   PESA39.PAS                              Code-nummer:  7108 100 72611
   Philips TV Test Equipment A/S   910121
   Peter Frederiksen, afd P-DPE
   Rettet 910913:  Der bruges RGB-generator 7108 100 72721
                   Rettet til nyt m†lehoved med skr† m†lekanaler
                   EýPROM-memory map rettet.
   Rettet 911118   til software 5.6
   Rettet 911212:  Til software 5.7
   Rettet 920116:  Ny RGB-generator 7108 100 72801 (kun til SONY 1410/1310)
                   Check af offset-v‘rdier
                   Til software 5.7 eller software 5.8
                   Ny beregning af gainfaktorer styret af MeasheadSWRev.
                   Visning af teststatus
                   Timeout af monitor.
   Rettet 920504:  Skrivebeskyttelse for SW-ver 5.7 og >= 5.8
   Rettet 920820:  Skrivebeskyttelsesadresserne 54 og 55 initialiseres
                   til 255 255.
   Rettet 920930:  Dark current limit ‘ndret fra 0.2 til 0.03                   til 255 255.
   Rettet 931105:  VISTEK color probe indf›rt. Se DefInternID.
   Rettet 940120:  Fejl i totaltest rettet. Variablen TotaltestOk
                   blev somme tider sat til false selv om testen var ok.


   Dette program bruges til at teste og kalibrere PESA/VISTEK color probe.
      9449 756 39001    PESA
      9449 756 39011    VISTEK

   Opstart for PESA color probe:     PESA39.EXE  PESA <enter>
               VISTEK color probe:   PESA39.EXE  VISTEK <enter>

   Programmet kommunikerer med color proben via COM1-porten. Porten er
   initialiseret til 4800 baud, 8 bit, 2 stopbit og ingen paritet.
   Dette g›res i proceduren InitPC_COM1Port.

   Testprogrammets softwarerevision skrives i konstanten 'SWRev' og vises
   i ›verste linie.

   Korrekt SW-revision for m†lehovedet er skrevet i konstanten MeasHeadSWRev.

   Funktionstasterne bruges til at styre de enkelte test og kalibre-
   ringer, i den r‘kkef›lge man ›nsker.

   Tast F10 afvikler en totaltest i en forud bestemt r‘kkef›lge.
   N†r test og kalibrering er godkendt, skrives der et "O" og et "K" i
   adresserne 56 og 57 i EýPROM'men. Inden der udf›res en EýPROM-test,
   kontrolleres adresserne 56 og 57.
   Totaltesten kan afbrydes undervejs ved at taste p† SPACE.

   Tast-H viser en hj‘lpemenu.

   Nominelle m†lev‘rdier, tolerancer, gainfaktorer, EýPROM-skrivetid,
   ID-streng m.v., er fastlagt i konstanter efter keyword CONST.

   Tastes der en ikke defineret tast skrives ??.
   Mellem hver kommando skrives *.

   Programmet afbrydes ved at taste p† ESC.

   Der kr‘ves f›lgende udstyr:
      PC med:   Seriel-port  (COM1),
                Printer-port (LPT1),
                IýC-interface 7108 100 71821,
                IOTech GP-488 interface (incl software-driver)
      Lyskasse        7108 100 72331
      RGB-generator   7108 100 72341
      PM2534 Digital multimeter p† IEEE-adresse 22
      RGB-monitor med 4 x Coax-kabel med BNC-stik
      IEEE-kabel
      COM1-kabel    (DB9   pin 2-2 3-3 5-5)
      IýC-bus kabel (DB25  pin 24-24  25-25 + GND)

   I/O-adresse p† IýC-interface er bestemt af konstanten I2CIOAddr.

   N†r programmet starter op, kontrolleres om f›lgende enheder er for-
   bundet:
      Lyskasse
      RGB-generator
      PM2534 Digital multimeter

   Kan programmet ikke f† kontakt med enhederne, afbrydes programmet.
   NB: RGB-generator og lyskasse SKAL v‘re t‘ndt, n†r programmet startes,
       ellers kan IýC interface i PC'en h‘nge. Hvis det h‘nger s† sluk/t‘nd
       for PC'en.

   Beregning og kalibrering: se pesa39.doc fil.
 *)



Program PESA39;
{$DEFINE RGBGEN}           {mulighed for at afvikle programmet uden}
{$DEFINE LYSKASSE}         {RGB-generator og lyskasse}
{$DEFINE IEEEBUS}          {eller uden PM2534 multimeter}

{$M 25000,0,300000}       {Stackst›rrelse m† ikke g›res mindre !!}
{$R+}    {Range check}
{$I+}    {I/O check}
{$S+}    {Stack check}
{$B-}    {Boolean short}

USES Crt,
     Dos,
     Printer,
     Win,
     PepUtil,
     ComUtil,
     Timer,
     Unit_IIC;

CONST
  MeasHeadSWRev        : ARRAY[1..2] OF STR_4 = ('05.7','05.8');
  TestSWNo             = '7108 100 72611';         {12 NC}
  TestSWRev            = '931105';
  SONY_Timeout         = 60;    {sec}             {SONY=black timeout}
  DefaultGainFactor    : ARRAY[1..2,1..10] OF BYTE =
                       (($FF,$FF,$00,$40,$00,$10,$00,$04,$00,$01), {<=SWRev5.7}
                       ($FF,$FF,$00,$40,$00,$40,$00,$40,$00,$40)); {>=SWRev5.8>}

  LightLevel           : ARRAY[1..4] OF BYTE =
                         (130,65,35,21);      {lysstyrke ved gain-kal}
  Marg                         = 8;           {margen i certiffile}
  CertifFileNameExtension      = '.P39';
  IdStr          : ARRAY[1..2] OF STRING[29]
                               = ('PHILIPS,400810976010,KU',
                                  'PHILIPS,400810981090,KU');
  R29            : REAL = 1.12;      { ohm }
  ADRef          : REAL = 0.9;       {volt  (ved beregning af offset)}
  ADCounts       : WORD = 50000;     {ved beregning af offset}
  TolUpper5Va    : BYTE = 8;         {›vre ñ 5V analog tolerance i %}
  TolLower5Va    : BYTE = 8;         {nedre ñ 5V analog tolerance i %}
  TolRef         : BYTE = 8;         {Tol p† AD-ref-sp‘nding}
  ValuePos5Va    : REAL = 4.9;       {Nominel positiv analog sp‘nding}
  ValueNeg5Va    : REAL = -4.9;      {Nominel negativ analog sp‘nding}
  ValueRef       : REAL = 0.88;
  CableLoose     : REAL = 52E-3;     {Tab i stelledning Volt}
  CurrentMin     : REAL = 25;        {Min str›m i mA til 5V}
  CurrentMax     : REAL = 35;        {Max str›m i mA til 5V}
  DarkLimitRed   : REAL = 0.03;      {Max dark current}
  DarkLimitGreen : REAL = 0.03;      {MX-value / fieldtime}
  DarkLimitBlue  : REAL = 0.03;
  GA1Nom57       : WORD = 16384;     {Nom gainfaktorer for SW-rev 5.7}
  GA2Nom57       : WORD = 4096;
  GA3Nom57       : WORD = 1024;
  GA4Nom57       : WORD = 256;
  GA1Nom         : WORD = 16384;     {Nom gainfaktorer for SW-rev 5.8 >}
  GA2Nom         : WORD = 16384;
  GA3Nom         : WORD = 16384;
  GA4Nom         : WORD = 16384;
  GainFactorTol  = 5;                {gainfaktor tolerance i %}
  LCNom          = 100;              {nominel v‘rdi ved LC m†ling}
  MeasureLCTol   = 5;                {tol i % ved LC m†ling}

  AntalMeas          = 7;            {ved m†ling af korrektionsfaktorer}
  EEPROMWriteDelay   = 30;           {i mS}
  I2CIOAddr          = $300;         {IýC-PC-interface I/O-adresse}
  InQueSize          = 500;          {COM1 port}
  OutQueSize         = 100;          {  -    - }

  AntalLinierFrem    = 4;            {antal linier frem ved fejludskrift}

  DefInternID        : ARRAY[1..2,1..4] OF BYTE =    {addr 58 - 61}
                       ((2,0,0,0),         {PESA}
                        (4,0,0,0));        {VISTEK}


  DefMaxNoCRTFosfor  = 7;                 {Max Antal CRT-fosforer}
  DefNoEEPROMPages   = 2;                 {Antal sider a 256 bytes i EýPROM}
  DefMaxWriteProtectAddr : WORD = 102;    {H›jeste adresse i EýPROM som er}
                                          {skriveskyttet}
  DefPreGain          : ARRAY[1..3] OF BYTE =
                        (251,255,146);    {R,G,B}

  Silent              = FALSE;
  NotSilent           = TRUE;

VAR
  CertifFile          : TEXT;
  COM1Status          : WORD;
  PrinterStatus,
  St,                               {status for I2C-bus}
  EEPROMPass          : BYTE;       {Styrer 1. og 2. pass af testen}
  IICPort64,
  IICPort66           : BYTE;       {status af IýC-port addr 64 og 66}
  KU_No               : STRING[6];  {apparatets KU-nummer}
  SWRevStr            : STR_4;      {m†lehovedets SW-revision}
  GainVal             : STR_40;
  CertifFileName,

  ProbeType           : STR_20;
  ProbeNo             : BYTE;       {1:PESA    2:VISTEK}

  NoEEPROMPages       : BYTE;       {antal sider i EýPROM}
  WriteProtectAddr    : WORD;       {h›jeste skrivebeskyttede adresse}

  IICBusError,
  TestFailed,
  RS232TestOk,
  CurrentTestOk,
  AnalogPos5VTestOk,
  AnalogNeg5VTestOk,
  ADRefTestOk,
  EEPROMTestOk,
  LearnOffsetOk,
  FieldRateTestOk,
  DarkCurrentTestOk,
  BeregnGainFactorOk,
  ReadIdStringOk,
  ReadStatusRequestOk,
  ReadRefColorOk,
  ReadCRTValueOk,
  ReadGainFactorOk,
  ReadPreGainOk,
  ReadWriteProtectOk,
  ReadInternIDOk,
  ReadNoEEPROMPagesOk,
  ReadKaldataOk,
  LearnRGBOk,
  MeasureLCOk,
  TotalTestOk,
  SP6WriteProtectOk,
  ValidKU,
  FilterOk,
  Analog5VTestValgt,
  ADRefTestValgt,
  EEPROMTestValgt,
  TotalTestFlag,
  PrinterFlag       : BOOLEAN;

  MeasStr           : ARRAY[1..AntalMeas] OF STRING[20];
  MeanValue_1,
  MeanValue_2       : REAL;
  KorrFactor        : ARRAY[1..4] OF REAL;
  GainFactor        : ARRAY[1..4] OF WORD;
  PreGain           : ARRAY[1..3] OF BYTE;
  Offset            : ARRAY[0..4,0..3] OF REAL;  {offset values in EEPROM}

  AfslutProgram     : BOOLEAN;

{$I PESAUTIL.INC}
{$I TPTEST.INC}


PROCEDURE LearnRGB;
{Der udf›res kommandoen LR, LG, LB, og LC p† sk‘rmen. Resultatet gemmes som
 CRT-v‘rdier nr 1 - 4.
 Referencerne gemmes derefter med kommandoen SRx
 Setup gemmes derefter med kommandoen SSx}
CONST CRTDelay    = 7000;   {mS}
      RGBDelay    = 2000;
      LCDelay     = 2000;
      SCDelay     = 1000;
      SRDelay     = 500;
VAR   Status      : BYTE;
      MeanValueR,
      MeanValueG,
      MeanValueB  : REAL;
      n           : BYTE;
      s           : STR_2;
      Svar        : STRING;
BEGIN
 LearnRGBOk := TRUE;
 EBURed;
 Beep(1000,200);
 Writeln('Placer PM5639 p† monitoren over det r›de felt  -  tryk en tast');
 WaitForAnyKey;

 COM1Status := WriteCOMPort(_COM1,'MS,MR,');   Delay(10);

 Write('Learn: R ');
 EBURed;
 Delay(CRTDelay);
 COM1Status := WriteCOMPort(_COM1,'LR,');   Delay(10);
 Delay(RGBDelay);

 Write('G ');
 EBUGreen;
 Delay(CRTDelay);
 COM1Status := WriteCOMPort(_COM1,'LG,');   Delay(10);
 Delay(RGBDelay);

 Writeln('B ');
 EBUBlue;
 Delay(CRTDelay);
 COM1Status := WriteCOMPort(_COM1,'LB,');   Delay(10);
 Delay(RGBDelay);

 EBUWhite;
 Write('Store CRT 1,');
 COM1Status := WriteCOMPort(_COM1,'SC1,');
 Delay(SCDelay);
 Write('2,');
 COM1Status := WriteCOMPort(_COM1,'SC2,');
 Delay(SCDelay);
 Write('3,');
 COM1Status := WriteCOMPort(_COM1,'SC3,');
 Delay(SCDelay);
 Write('4,');
 COM1Status := WriteCOMPort(_COM1,'SC4,');
 Delay(SCDelay);
 Write('5,');
 COM1Status := WriteCOMPort(_COM1,'SC5,');
 Delay(SCDelay);
 Write('6,');
 COM1Status := WriteCOMPort(_COM1,'SC6,');
 Delay(SCDelay);
 Writeln('7');
 COM1Status := WriteCOMPort(_COM1,'SC7,');
 Delay(SCDelay);

 Write('CRT 1: ');
 COM1Status := WriteCOMPort(_COM1,'CR1,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);

 Write('CRT 2: ');
 COM1Status := WriteCOMPort(_COM1,'CR2,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);

 Write('CRT 3: ');
 COM1Status := WriteCOMPort(_COM1,'CR3,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);


 Write('CRT 4: ');
 COM1Status := WriteCOMPort(_COM1,'CR4,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);

 Write('CRT 5: ');
 COM1Status := WriteCOMPort(_COM1,'CR5,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);

 Write('CRT 6: ');
 COM1Status := WriteCOMPort(_COM1,'CR6,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);

 Write('CRT 7: ');
 COM1Status := WriteCOMPort(_COM1,'CR7,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'LC,');
 Delay(LCDelay);
 Write('Ref0 ');
 COM1Status := WriteCOMPort(_COM1,'SR0,');
 Delay(SRDelay);
 Write('Ref1 ');
 COM1Status := WriteCOMPort(_COM1,'SR1,');
 Delay(SRDelay);
 Write('Ref2 ');
 COM1Status := WriteCOMPort(_COM1,'SR2,');
 Delay(SRDelay);
 Writeln('Ref3 ');
 COM1Status := WriteCOMPort(_COM1,'SR3,');
 Delay(SRDelay);



 Write('Power-up setup: ');
 COM1Status := WriteCOMPort(_COM1,'CR1,');
 Delay(100);
 COM1Status := WriteCOMPort(_COM1,'RE0,');
 Delay(100);
 FOR n := 0 TO 15 DO
  BEGIN
   Str(n,s);
   Write(s,' ');
   COM1Status := WriteCOMPort(_COM1,'SS' + s + ',');
   Delay(100);
  END;
 Writeln;

 EBUWhite;
 Write('Kontrol af m†ling: ');
 COM1Status := WriteCOMPort(_COM1,'MR,');
 Delay(10);
 COM1Status := WriteCOMPort(_COM1,'MC,');
 Delay(2000);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 REPEAT
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 UNTIL n = AntalMeas + 1;
 COM1Status := WriteCOMPort(_COM1,'MS,');   Delay(10);

 MeanValueR := UdregnMeanValueRed;
 Write('R: ',MeanValueR:7:2);
 MeasureLCOk := (MeanValueR < (LCNom * (1 + MeasureLCTol / 100))) AND
                (MeanValueR > (LCNom * (1 - MeasureLCTol / 100)));

 MeanValueG := UdregnMeanValueGreen;
 Write('  G: ',MeanValueG:7:2);
 MeasureLCOk := (MeanValueG < (LCNom * (1 + MeasureLCTol / 100))) AND
                (MeanValueG > (LCNom * (1 - MeasureLCTol / 100))) AND
                 MeasureLCOk;

 MeanValueB := UdregnMeanValueBlue;
 Write('  B: ',MeanValueB:7:2);
 MeasureLCOk := (MeanValueB < (LCNom * (1 + MeasureLCTol / 100))) AND
                (MeanValueB > (LCNom * (1 - MeasureLCTol / 100))) AND
                 MeasureLCOk;
 TrueOrFalse(MeasureLCOk);

 LearnRGBOk := LearnRGBOk AND MeasureLCOk;
END; {LearnRGB}


PROCEDURE ShowTotalTest;
BEGIN
 CreateWindow(4,3,18,74,' Totaltest ',Red+16*Cyan,Black+16*Cyan,Frame);
 Color(Black,Cyan);
 ClrScr;
 Writeln(' 1) Supply current test            15) L‘s gain factor');
 Writeln(' 2) Analog ñ 5Va test              16) L‘s ID string');
 Writeln(' 3) AD-ref test                    17) L‘s intern ID');
 Writeln(' 4) RS232 test                     18) L‘s pre gain');
 Writeln(' 5) Init PTV EýPROM                19) L‘s status');
 Writeln(' 6) Power OFF ON                   20) L‘s ref color');
 Writeln(' 7) Skriv ID string                21) L‘s CRT');
 Writeln(' 8) Skriv default gain factor      22) L‘s kalibreringsdata');
 Writeln(' 9) Learn offset');
 Writeln('10) Indtast KU nummer');
 Writeln('11) Fieldrate test');
 Writeln('12) M›rkestr›m test');
 Writeln('13) Beregn gainfactor');
 Writeln('14) Learn RGB');
 GotoXY(2,16);
 Write('Tryk en tast . . .');
 WaitForAnyKey;
 CloseWindow;
END;


PROCEDURE ShowHelpWindow;
VAR  Sc : BYTE;
BEGIN
 CreateWindow(2,2,20,78,' Help ',Black+16*Cyan,Black+16*Cyan,Frame);
 Color(Black,Cyan); ClrScr;
 CursorOff;
 Writeln(' Test software no: ',TestSWNo,'   Rev: ',TestSWRev);
 Writeln(' Color Probe: ',ProbeType);
 Writeln;
 Writeln(' Alt-F1    : Skriv OK                         C : Clear PC-screen');
 Writeln(' Alt-F2    : Skriv NOT OK                     D : Skriv dato');
 Writeln(' Alt-F3    : Init EýPROM                      E : Skift LED on/off');
 Writeln(' Alt-F4    : M†l offset                       F : Skift freq');
 Writeln(' Alt-F5    : Skriv default ID string          H : Vis hj‘lp');
 Writeln(' Alt-F6    : Skriv default gain faktorer      L : Skift lampe on/off');
 Writeln(' Alt-F9    : Skriv cal-data til file          P : Power OFF - ON');
 Writeln(' Alt-F10   : Skriv cal-data til printer       R : Skift relay');
 Writeln('                                              T : Test status');
 Writeln(' Ctrl-F4   : SP6 = skrive beskyttet ?         W : SONY monitor=hvid');
 Writeln(' Ctrl-F6   : L‘s offset v‘rdier i EýPROM');
 GotoXY(2,18);
 Write('F10: Se totaltest           ESC: Forts‘t . . .');
 EmptyKeyboardBuffer;
 REPEAT
  Sc := Scancode;
  IF Sc = F10 THEN ShowTotalTest;
 UNTIL Sc <> F10;
 CursorOn;
 CloseWindow;
END;


PROCEDURE WriteCertifData(Device : STR_10);       {AltF9 eller AltF10}
VAR   IOErr,
      Addr       : WORD;
      Data,
      n          : BYTE;
      k          : INTEGER;
      InStr,
      Svar       : STRING;
      MaxAddrStr,
      DataStr    : STR_4;
BEGIN
 ReadKalDataok := TRUE;
 WriteProtectAddr := DefMaxWriteProtectAddr;

 CertifFileName := ReadKU_Nummer + CertifFileNameExtension;
 IF Device = 'FILE' THEN
   Assign(CertifFile,CertifFileName)
  ELSE
   Assign(CertifFile,'PRN');
 {$I-}
 Reset(CertifFile);
 {$I+}

 IF Device = 'PRN' THEN
  IF NOT PrinterReady(1,PrinterStatus) THEN
   BEGIN
    Writeln(Bell,'Printer ikke klar !!');
    Exit;
   END;

  IF (IOResult = 0) AND (Device = 'FILE') THEN
   BEGIN
    Writeln(Bell,'File: "',CertifFileName,'" findes allerede');
    Writeln('Overskriv filen ?  (J/N)');
    EmptyKeyboardBuffer;
    IF Scancode <> TastJ THEN
     BEGIN
      Writeln('File: "',CertifFileName,'" IKKE overskrevet');
      Exit;
     END
     ELSE
      Writeln('File: "',CertifFileName,'" overskrevet');
   END {if IOResult}
   ELSE
    BEGIN
      IF Device = 'FILE' THEN
        Writeln('File: "',CertifFileName,'" oprettet')
       ELSE
        Writeln('Kalibreringsdata skrevet til printer')
    END;  {if IOResult}


 {$I-}
 Rewrite(CertifFile);
 {$I+}
 IOErr := IOResult;
  IF IOErr <> 0 THEN
   BEGIN
    Write(Bell,CertifFileName,' ikke oprettet !!      ');
    Writeln('(I/O-Error: ',IOErr,')');
    Writeln('Tryk en tast'); WaitForAnyKey;
    AfslutProgram := TRUE;
    Exit;
   END;

  FOR n := 1 TO 20 DO
   Writeln(CertifFile);
  Writeln(CertifFile,Space:Marg,'Serial no: ',Read12NC_Nummer,'    KU',ReadKU_Nummer);
  Writeln(CertifFile);
  Writeln(CertifFile,Space:Marg,'Date: ',Dato);
  FOR n := 1 TO 3 DO
   Writeln(CertifFile);

 n := 1;
 Svar[0] := #119;
 COM1Status := WriteCOMPort(_COM1,'MS,');   Delay(10);
 COM1Status := WriteCOMPort(_COM1,'MA2,');   Delay(10);
 COM1Status := ClearInputQue(_COM1);
 REPEAT
  COM1Status := WriteCOMPort(_COM1,'RM,');   Delay(10);
  COM1Status := ReadlnCOMPort(_COM1,InStr,2);  Delay(10);
  Val(Instr,Data,k);
  Svar[n] := Chr(Data);
  Inc(n);
 UNTIL (n > WriteProtectAddr - 1) OR (COM1Status <> 0) OR (k <> 0);
 Svar[0] := Chr(n);

 IF (n <= WriteProtectAddr - 1) THEN
  BEGIN
   Writeln(Bell,' L‘sefejl fra EýPROM !!');
    IF COM1Status <> 0 THEN
     Writeln(' Timeout !!');
   ReadKaldataOk := FALSE;
   TestFailed := TRUE;
   Exit;
  END;


  Writeln(CertifFile);
  Writeln(CertifFile,Space:Marg,'Protected data:');

  Write(CertifFile,Space:Marg,'   2:',Space:12);
  FOR n := 1 TO 8 DO
   BEGIN
    Str(Ord(Svar[n]),DataStr);
    Write(CertifFile,DataStr:5);
   END;

  Addr := 10;
  Writeln(CertifFile);
  Write(CertifFile,Space:Marg,Addr:4,':  ');

  FOR n := 9 TO WriteProtectAddr - 1 DO
   BEGIN
    Str(Ord(Svar[n]),DataStr);
    Write(CertifFile,DataStr:5);
    IF (n MOD 10) = 8 THEN
     BEGIN
      Writeln(CertifFile);
      Inc(Addr,10);
      IF n < (WriteProtectAddr - 1) THEN
       Write(CertifFile,Space:Marg,Addr:4,':  ');
     END; {if}
   END;

  FOR n := 1 TO 10 DO
   Writeln(CertifFile);

  Writeln(CertifFile,Space:Marg,'Calibrated by: ______________');

  Write(CertifFile,FF);
  Close(CertifFile);
END; {WriteCertifFile}

PROCEDURE WriteProtectYesNo;
{unders›ger om SP6 er ON eller OFF, ved at skrive i adresse 1 og
 l‘se igen}
VAR ReadByte,
    WriteByte  : BYTE;
    Svar       : STRING;
BEGIN
 Write('SP6 = skrivebeskyttet ?    ');
 TestModePM5639;
 COM1Status := WriteCOMPort(_COM1,'MA1,');   Delay(10);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RN1,');   Delay(10);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 IF COM1Status = 0 THEN
  BEGIN
   WriteByte := Ord(Svar[1]) XOR $FF;
   WriteEEPROM(1,WriteByte);
   MeasModePM5639;
   COM1Status := WriteCOMPort(_COM1,'MA1,');   Delay(10);
   COM1Status := ClearInputQue(_COM1);
   COM1Status := WriteCOMPort(_COM1,'RN1,');   Delay(10);
   COM1Status := ReadlnCOMPort(_COM1,Svar,1);
   ReadByte := Ord(Svar[1]);
   SP6WriteProtectOk := ReadByte <> WriteByte;
   IF SP6WriteProtectOk THEN
     Writeln('JA')
    ELSE
     Writeln('NEJ');
  END
  ELSE
   Writeln(' L‘sefejl !!');
END;


PROCEDURE TotalTest;
BEGIN
 WriteProtectYesNo;
 IF SP6WriteProtectOk THEN
  BEGIN
   Writeln(Bell,'Color sensor m† IKKE v‘re skrivebeskyttet ved kalibrering !!');
   Exit;
  END;
 ClearTestFlag;
 EEPROMTestOk := TRUE;
{$IFDEF IEEEBUS}
 SupplyCurrentTest;
 AnalogProbeTest;
{$ENDIF}
 RS232Test;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
  BEGIN
   InitEEPROM;
   PowerOffOn;
  END;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
  BEGIN
   WriteIdString;
   WriteDefaultGainFactor;
   LearnOffset;
  END;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
   EnterKU_Nummer;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
   FieldRateTest;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
   DarkCurrentTest;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
   BeregnGainFactor;
 IF TestAfbrudt THEN Exit;
 IF NOT TestFailed THEN
   LearnRGB;
 IF TestAfbrudt THEN Exit;

 IF NOT TestFailed THEN
  BEGIN
   ReadGainFactor;
   ReadIdString;
   ReadInternID;
   ReadPreGainFactor;
   ReadStatusRequest;
   ReadRefColor;
   ReadCRTValue;
 END;

 TotalTestOk :=  RS232TestOk AND
                 CurrentTestOk AND
                 LearnOffsetOk AND
                 FieldRateTestOk AND
                 DarkCurrentTestOk AND
                 BeregnGainFactorOk AND
                 FilterOk AND
                 ValidKU AND
                 ReadStatusRequestOk AND
                 ReadRefColorOk AND
                 ReadCRTValueOk AND
                 ReadIdStringOk AND
                 ReadInternIDOk AND
                 ReadPreGainOk AND
                 MeasureLCOk AND
                 LearnRGBOk AND
                 (NOT TestFailed);

 Write('Totaltest ');
 TrueOrFalse(TotalTestOk);

 IF Analog5VTestValgt THEN
  TotaltestOk :=  TotaltestOk AND AnalogPos5VTestOk AND AnalogNeg5VTestOk;

 IF ADRefTestValgt THEN
  TotaltestOk :=  TotaltestOk AND ADRefTestOk;

 IF TotalTestOk THEN
  BEGIN
   TestModePM5639;
   WriteEEPROM(54,Lo(DefMaxWriteProtectAddr));
   WriteEEPROM(55,Hi(DefMaxWriteProtectAddr));
   WriteOkToEEPROM;
   WriteCertifData('FILE');
  END
  ELSE
   WriteNOT_OkToEEPROM;

  TotaltestOk :=  TotaltestOk AND
                  ReadWriteProtectOk;


 Write('PM5639 kalibrering OK ?:');
 TrueOrFalse(FinalTestOk);

 IF TotalTestOk THEN
   Writeln('PM5639  G O D K E N D T')
  ELSE
   Writeln('PM5639  A F V I S T');
END;


BEGIN
 ProbeType := UpString(ParamStr(1));
 IF ProbeType = 'PESA' THEN
  ProbeNo := 1
  ELSE
 IF ProbeType = 'VISTEK' THEN
  ProbeNo := 2
  ELSE
  BEGIN
   Writeln(Bell,'Ingen probetype valgt ');
   Exit;
  END;
 SetCBreak(TRUE);
 ClrScr;
 Initprinter;
 InitScreen;
 InitWindow;
 InitVar;
{$IFDEF IEEEBUS}
 InitIeeeBus;
{$ENDIF}
 InitI2CBus;
 InitPC_Com1Port;
 RGBGenRemote;
 EBUBlack;
 SMPTEBlack;
 LampOff;


 IF PrinterFlag THEN
   Writeln('Printer ready')
  ELSE
   Writeln('Printer NOT ready');

 REPEAT
  EmptyKeyboardBuffer;

  Set_Timer(SONY_Timeout * 10);         {SONY_Timeout = 1/10 sec}

  REPEAT
  UNTIL Timer_Timeout OR KeyPressed;

  IF Timer_Timeout THEN
   BEGIN
    EBUBlack;
    SMPTEBlack;
    Writeln('SONY-monitorer = OFF');
   END;

   TestFailed := FALSE;
   CASE ScanCode OF
{$IFDEF IEEEBUS}
     F1 : BEGIN
           SupplyCurrentTest;
           AnalogProbeTest;
          END;
{$ENDIF}

     F2 : EnterKU_Nummer;

     F4 : BEGIN
           RS232Test;
            IF RS232TestOk THEN
             BEGIN
               ReadGainFactor;
               ReadIdString;
               ReadPreGainFactor;
               ReadEEPROMWriteProtect(NotSilent);
               ReadNoEEPROMPages;
               ReadInternID;
               ReadStatusRequest;
               ReadRefColor;
               ReadCRTValue;
               Write('PM5639 kalibreret ? '); TrueOrFalse(FinalTestOk);
              END; {if}
          END;

     F5 : FieldRateTest;
     F6 : DarkCurrentTest;
     F7 : BeregnGainFactor;
     F9 : LearnRGB;
    F10 : BEGIN
           TotalTestFlag := TRUE;
           TotalTest;
           TotalTestFlag := FALSE;
          END;

   AltF1  : WriteOKToEEPROM;
   AltF2  : WriteNOT_OKToEEPROM;
   AltF3  : BEGIN
             InitEEPROM;
             PowerOffOn;
            END;
   AltF4  : LearnOffSet;
   AltF5  : WriteIDString;
   AltF6  : WriteDefaultGainFactor;
   AltF9  : WriteCertifData('FILE');
   AltF10 : WriteCertifdata('PRN');

   CtrlF4 : WriteProtectYesNo;
   CtrlF6 : ShowOffsetValues(NotSilent);

    TastC : ClrScr;
    TastD : Writeln('Dato: ',Dato,'  ',Time(0));
    TastE : ToggleLED;
    TastF : ToggleFreq;
    TastH : ShowHelpWindow;
    TastL : ToggleLamp;
    TastP : PowerOffOn;
    TastR : ToggleRelay;
    TastT : ShowTestStatus;
    TastW : BEGIN
             EBUWhite;
             SMPTEWhite;
            END;

 Escape : AfslutProgram := TRUE;
     ELSE
      Write('?? ');
   END; {case}

   Writeln('*');

 UNTIL AfslutProgram;

 COM1Status := CloseCOMPort(_COM1);
 IF IeeeFilesOpen THEN
  CloseIeeeFiles;

 IF NOT IICBusError THEN
  BEGIN
{$IFDEF LYSKASSE}
   Trm8574(I2CIOAddr,64,$FF,St);
   LampOff;
{$ENDIF}
   EBUBlack;
   SMPTEBlack;
   RGBGenLocal;
  END;

 Window(1,1,80,25); Color(LightGray,Black); ClrScr;

 IF COM1Status <> 0 THEN
  Writeln('COM1 ikke lukket korrekt: ',COM1Status);

END.

