(**********************************************************************
   PTV39.PAS                               Code-nummer:  7108 100 72811
   Philips TV Test Equipment A/S.
   Peter Frederiksen, 910514,   afd P-DPE.
   Rettet 910913:  Der bruges RGB-generator 7108 100 72721.
                   Rettet til nyt m†lehoved med skr† m†lekanaler.
                   EýPROM-memory map rettet.
   Rettet 911118:  Til software 5.6.
   Rettet 911212:  Til software 5.7.
   Rettet 920318:  Ny RGB-generator 7108 100 72801 (kun til SONY 1410/1310).
                   Check af offset-v‘rdier.
                   Shutdown control voltage (display unit).
                   Til software 5.7 eller software 5.8.
                   Ny beregning af gainfaktorer styret af MeasheadSWRev.
                   Visning af teststatus (tast T).
                   Timeout af monitor.
   Rettet 920424:  xy,Y, CIELUV og u'v' m†ling med color sensor.
                   Fixed fieldrate (25mS) ved offset kalibrering.
                   Kontrol af skrivebeskyttelse ved start af totaltest.
   Rettet 920504:  ST-m†ling med color sensor.
                   Skrivebeskyttelse for SW-ver 5.7 og >= 5.8
   Rettet 920820:  Skrivebeskyttelsesadresserne 54 og 55 initialiseres
                   til 255 255.
   Rettet 920911:  Tast Ctrl-F7 tilf›jet.
   Rettet 920930:  Dark current limit ‘ndret fra 0.2 til 0.03.
   Rettet 921027:  12NC rettet fra 4008- til 9449-nummer (KUT40-7656).
   Rettet 921120:  12NC rettet fra 9449- til 4008-nummer (jvf N.Be.).
   Rettet 930119:  Cal status s‘ttes = FACTORY ved learn K-matrix.
   Rettet 931104:  Sporbar kalibrering ‘ndres fra NPL til NIST
                   I EýPROM adresse 96 skrives konstanten TraceFlag.
                   Indtil d.d. har der st†et 255.
                   Denne ‘ndring g‘lder ogs† 5639/90 sensor serie > 2 med
                   Se program CRT39.PAS hertil.
   Rettet 950501:  Kalibreringsdato skrives til EEPROM.
                   Inden InternID skrives til EEPROM kontrolleres, at
                   data i adresserne 58-61 er 255. Dette er indf›rt for
                   at undg†, at sensorer kan ‘ndres fra /90 til /92 eller
                   fra /92 til /90. Hvis der skal ‘ndres type, skal alle
                   adresserne ‘ndres til 255 inden test med COM5639.EXE.
  Rettet 960213:   Kalibreringsdata gemmes p† LAN og udskrives til
                   laser printer.
  Rettet 980128    Firmanavn rettet til ProTeleVision Technologies A/S
  Rettet 000124    Log til M:\MEASDATA....
                   Dato korrekt †r 2000. (se peputil.pas)


   Dette program bruges til at teste, kalibrere PM5639 color sensor.
   NB!! Programmet kan kun bruges til PM5639/90 serie 1 og 2.
        Serie > 2: se programmet CRT39.PAS.

   Programmet kommunikerer med PM5639 via COM1-porten. Porten er
   initialiseret til 4800 baud, 8 bit, 2 stopbit og ingen paritet.
   Dette g›res i proceduren InitPC_COM1Port.

   Korrekt SW-rev for PM5639-m†lehoved er i konstanten PTV5639SWRev.

   Funktionstasterne bruges til at styre de enkelte test og kalibre-
   ringer, i den r‘kkef›lge man ›nsker.

   Tast F10 afvikler en totaltest i en forud bestemt r‘kkef›lge, og det
   kontrolleres at SP6-skrivebeskyttelsen = OFF inden totaltesten.

   N†r test og kalibrering er godkendt, skrives der et "O" og et "K" i
   adresserne 56 og 57 i EýPROM'men.
   Totaltesten kan afbrydes undervejs ved at taste p† SPACE.

   Nominelle m†lev‘rdier, tolerancer, gainfaktorer, EýPROM-skrivetid,
   ID-string m.v., er fastlagt i konstanter efter keyword CONST.

   Tastes der en ikke defineret tast, skrives ??.
   Mellem hver kommando skrives *.

   Programmet afbrydes ved at taste p† ESC.

   Der kr‘ves f›lgende udstyr:
      PC med:   Seriel-port  (COM1),
                Printer-port (LPT1),
                IýC-interface 7108 100 71821,
                IOTech GP-488 interface (incl 488 software-driver).
      Lyskasse        7108 100 72331.
      RGB-generator   7108 100 72801   (for SONY BVM 1310/1410 monitors).
      PM2534 Digital multimeter p† IEEE-adresse 22.
      RGB-monitor med 4 x Coax-kabel med BNC-stik.
      IEEE-kabel.
      COM1-kabel    (DB9   pin 2-2 3-3 5-5).
      IýC-bus kabel (DB25  pin 24-24  25-25 + GND).

   I/O-adresse p† IýC-interfacet skal svare til konstanten I2CIOAddr.

   N†r programmet starter op, kontrolleres om f›lgende enheder er for-
   bundet:
      Lyskasse
      RGB-generator
      PM2534 Digital Multimeter

   N†r programmet starter op, indl‘ses filen XYFileName med xy-koordinater
   samt luminance. Kan filen ikke findes, afbrydes programmet.

   Kan programmet ikke f† kontakt med enhederne, afbrydes programmet.
   NB: RGB-generator og lyskasse SKAL v‘re t‘ndt, n†r programmet startes.
       IýC-interface i PC'en kan 'h‘nge op' af uforklarlige †rsager - muligvis
       ligger fejlen i PCD8584-controlleren ??.
       Hvis det sker m† man slukke/t‘nde for PC'en.

  Beregning af diverse matricer: se .doc fil.
 *)


Program PTV39;

(*{$DEFINE RGBGEN}*)
{$DEFINE LYSKASSE}    (* lyskasse                                          *)
{$DEFINE IEEEBUS}     (* PM2534 multimeter  HUSK: 488 software-driver      *)
                      (* /D skal bruges ved kompilering med TPC            *)

{$M 25000,0,150000}   (* Stack m† ikke g›res mindre end 25000 !!           *)

{$R+}    {Range check}
{$I+}    {I/O check}
{$S+}    {Stack check}
{$B-}    {Boolean short}

USES Crt,
     Dos,
     Printer,
     PepUtil,
     Matrix,
     ComUtil,
     Win,
     Timer,
     PTVMeas,
     Unit_Las,
     Unit_IIC;


const
  TraceFlag               = 1;             {1:   cal traceable to NIST}
                                           {255: cal traceable to NPL}
  MeasHeadSWRev           : array[1..2] OF STR_4 =
                            ('05.7','05.8');
  TestSWNo                = '7108 100 72811';     {12 NC}
  TestSWRev               = '000124';
  SONY_Timeout            = 60;   {sec}           {SONY=black timeout}
  XYFilename              = 'CRTXY.DAT';          {xy-v‘rdier for monitor}
                                                  {samt nominel luminans}
  Marg                    = '        ';           {v. margin i certiffile}
 { CertifFileNameExtension = '.T39';}
  NameExt                 = '.T39';
  MeasFileName            = 'NEW_MEAS._39';       {file med nyeste data}
                                                  {overskrives ved ny m†ling}
  PrinterPort             = 'LPT2';
  PathName                = 'M:\MEASDATA\5639SEN\';
  MaxLuminans             = 150;                  {max luminans i filen}
                                                  {XYFilename}
  D6500Ref           : array[1..2] OF REAL = (0.1978,0.4684); {u',v'}
  NIT_fL_Factor          = 3.426259;
  LightLevel         : array[1..4] OF BYTE =
                       (255,220,150,120);            {lysstyrke ved gain-kal}

  DefaultGainFactor  : array[1..2,1..10] OF BYTE =
                    (($FF,$FF,$00,$40,$00,$10,$00,$04,$00,$01), {SWRev=5.6+5.7}
                    ($FF,$FF,$00,$40,$00,$40,$00,$40,$00,$40)); {SWRev=5.8}

  DefaultPreGain     : array[1..3] OF REAL =
                       (1.0,1.0,1.0);
  IdStr              : array[1..2] OF string[29]
                               = ('PHILIPS,400810975430,KU',
                                  '');
  ProbeNo            = 1;                {1: PTV   2: bruges ikke }
  ProbeType          = 'PTV';
  VoltMax            : REAL = 9.0;       {display unit shutdown control}
  VoltMin            : REAL = 3.95;      {   -      -      -       -   }
  VoltStepSize       : REAL = 0.05;      {   -      -      -       -   }
  VoltStep           : array[1..5] OF REAL
                     = (5.5,6.0,6.6,7.1,7.5); {display unit shutdown control}
  R29                : REAL = 1.12;      { ohm }
  ADRef              : REAL = 0.9;       {volt  (ved beregning af offset)}
  ADCounts           : WORD = 50000;     {ved beregning af offset}
  TolUpper5Va        : BYTE = 8;         {›vre ñ 5V analog tolerance i %}
  TolLower5Va        : BYTE = 8;         {nedre ñ 5V analog tolerance i %}
  TolRef             : BYTE = 8;         {AD-ref tolerance i %}
  ValuePos5Va        : REAL = 4.9;       {Nominel positiv analog sp‘nding}
  ValueNeg5Va        : REAL = -4.9;      {Nominel negativ analog sp‘nding}
  ValueRef           : REAL = 0.88;      {Nominel AD-ref sp‘nding}
  CableLoose         : REAL = 52E-3;     {Tab i stelledning i Volt}
  CurrentMin         : REAL = 25;        {Min str›m i mA til 5V}
  CurrentMax         : REAL = 35;        {Max str›m i mA til 5V}
  DarkLimitRed       : REAL = 0.03;      {Max dark current}
  DarkLimitGreen     : REAL = 0.03;      {MX-value / fieldtime}
  DarkLimitBlue      : REAL = 0.03;
  GA1Nom57           : WORD = 16384;     {Nom gainfaktorer for SW-rev <= 5.7}
  GA2Nom57           : WORD = 4096;
  GA3Nom57           : WORD = 1024;
  GA4Nom57           : WORD = 256;
  GA1Nom             : WORD = 16384;     {Nom gainfaktorer for SW-rev >= 5.8}
  GA2Nom             : WORD = 16384;
  GA3Nom             : WORD = 16384;
  GA4Nom             : WORD = 16384;
  GainFactorTol      = 7;            {gainfaktor tolerance i %}
  AntalMeas          = 7;            {ved m†ling af korrektionsfaktorer,}
                                     {K-matrix og CRT-matrix}
  EEPROMWriteDelay   = 30;           {mS}
  LearnDelay         = 8000;         {mS ved learn af K-matrix of CRT-matrix}
  I2CIOAddr          = $300;         {IýC-PC-interface I/O-adresse}
  InQueSize          = 500;          {COM1 port}
  OutQueSize         = 100;          {  -    - }

  PreGainMulFactor   = 1024;         {Multiplikationsfaktor for}
                                     {XFact, YFact og ZFact}
  CRTMulFactor       = 8192;         {Multiplikationsfaktor for}
                                     {CRT-matrix}
  KorrMulFactor      : LONGINT =
                       100000;       {Multiplikationsfaktor for}
                                     {korrektionsmatrix}
  KorrAddr           = 103;          {Start-adresse i EýPROM for}
                                     {korrektionsmatrix}
  PreGainAddr        = 97;           {Start-adresse i EýPROM for}
                                     {for pre gain}
  CRTNameAddr        = 1016;         {H›jeste start-adresse i EýPROM}
                                     {for CRT-navne}
  CRTMatrixAddr      = 1004;         {H›jeste start-adresse i EýPROM}
                                     {for CRT-matrix}
  DefInternID        : array[1..4] OF BYTE =
                       (1,0,0,0);       {addr 58 - 61}
  DefMaxNoCRTFosfor             = 30;   {Max antal CRT-fosforer}
  DefNoEEPROMPages              = 4;    {Antal sider a 256 bytes i EýPROM}
  DefMaxWriteProtectAddr : WORD = 120;  {H›jeste adresse i EýPROM som er}
                                        {skriveskyttet}
  Silent              = FALSE;
  NotSilent           = TRUE;

var
  BottomWin,
  TopWin,
  MeasFile,
  CertifFile,
  XYFile              : text;
  COM1Status          : word;       {status for COM1}
  PrinterStatus,                    {status af printer}
  St,                               {status for I2C-bus}
  EEPROMPass          : byte;       {Styrer 1. og 2. pass af testen}
  IICPort64,
  IICPort66           : byte;       {status af IýC-port addr 64 og 66}
  SWRevStr            : str_4;      {m†lehovedets SW-revision}
  DateStr,
  KU_No               : str_6;      {m†lehovedets KU-nummer}
  CertifFileName      : str_80;     {fil med testcertifikat}
  DirInfo             : SearchRec;

  IICBusError,
  TestFailed,
  RS232TestOk,
  CurrentTestOk,
  AnalogPos5VTestOk,
  AnalogNeg5VTestOk,
  ADRefTestOk,
  EEPROMTestOk,
  FieldRateTestOk,
  DarkCurrentTestOk,
  BeregnGainFactorOk,
  ReadIdStringOk,
  ReadGainFactorOk,
  ReadCRTNameOk,
  ReadKorrMatrixOk,
  ReadCRTMatrixOk,
  ReadInternIDOk,
  ReadPreGainOk,
  ReadWriteProtectOk,
  ReadNoCRTFosforOk,
  ReadNoEEPROMPagesOk,
  ReadKalDataOk,
  ReadCalDateOk,
  LearnOffsetOk,
  LearnKMatrixOk,
  LearnEBUOk,
  LearnSMPTE_COk,
  TotalTestOk,
  ValidKU,
  FilterOk,
  Analog5VTestValgt,
  ADRefTestValgt,
  EEPROMTestValgt,
  SP6WriteProtectOk,
  TotalTestFlag,
  PrinterFlag,
  AfslutProgram     : BOOLEAN;

  MeasStr           : array[1..AntalMeas] OF string[30];
  MeanValue_1,
  MeanValue_2       : REAL;
  KalStr            : array[1..25] of STR_120;
  Offset            : OffSetType;                     {offset values in EEPROM}
  KorrMatrix        : KMEType;
  CRTMatrix         : array[1..30,1..12] of BYTE;
  CRTName           : array[1..30] of STR_10;
  InternIDRead      : array[1..4] of byte;
  WriteProtectAddr  : WORD;
  Xfact,
  YFact,
  ZFact             : REAL;
  NoCRTFosfor,
  NoEEPROMPages     : BYTE;


  XR,YR,ZR,                    {m†lte v‘rdier i MX-mode ved beregning af}
  XG,YG,ZG,                    {matricer.}
  XB,YB,ZB,                    {XR=R›d kanal v. r›d sk‘rm}
  X65,Y65,Z65,                 {YR=Gr›n kanal v. r›d sk‘rm}
                               {XG=R›d kanal v. gr›n sk‘rm}
                               {Y65=Gr›n kanal v. hvid sk‘rm     osv}

  a,b,c,d,e,f,g,h,j,           {K-matrice}
  XpR,YpR,ZpR,                 {red       xyz-v‘rdier i CIE-diagram for monitor}
  XpG,YpG,ZpG,                 {green     l‘ses fra filen 'XYFileName'}
  XpB,YpB,ZpB,                 {blue}
  Xp65,Yp65,Zp65,              {white ~ D6500}

  NominelLum        : REAL;    {luminans ved hvid ~D6500}
                               {l‘ses fra filen 'XYFilename'}


{$I PTVUTIL.INC}
{$I TPTEST.INC}

Procedure ReadKMatrix(Flag : BOOLEAN);    {AltF7}
{L‘ser K-matricen fra EýPROM, gemmer v‘rdierne i en variabel men viser dem
 kun p† sk‘rmen hvis Flag=TRUE}
var  n,k     : BYTE;
     Svar    : string;
     Kode    : INTEGER;
     Value   : BYTE;
     KUStr   : STR_6;
begin
 FillChar(KorrMatrix,SizeOf(KorrMatrix),0);
 ReadKorrMatrixOk := TRUE;
 n := 0;
 COM1Status := WriteCOMPort(_COM1,'MS,');delay(20);
 COM1Status := WriteCOMPort(_COM1,'MA103,');delay(20);
 COM1Status := ClearInputQue(_COM1);
 repeat
  Inc(n);
  COM1Status := WriteCOMPort(_COM1,'RM,'); delay(20);
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  Val(Svar,Value,Kode);delay(5);
  if (Kode = 0) AND (COM1Status = 0) then
    KorrMatrix[n] := Value
   else
    begin
     ReadKorrMatrixOk := FALSE;
     TestFailed := TRUE;
    end;
 until (n = 18) OR (COM1Status <> 0) OR (Kode <> 0);

 if Flag then
  Writeln('Adresse: ',KorrAddr,' - ',KorrAddr + 17);

 if Flag then
  begin
    FOR k := 1 TO n DO
     Write(KorrMatrix[k],' ');
    Writeln;
   {$R-}
    Write(KorrMatrix[1]    * 256 + KorrMatrix[2]:6);
    Write(KorrMatrix[3]    * 256 + KorrMatrix[4]:6);
    Writeln(KorrMatrix[5]  * 256 + KorrMatrix[6]:6);
    Write(KorrMatrix[7]    * 256 + KorrMatrix[8]:6);
    Write(KorrMatrix[9]    * 256 + KorrMatrix[10]:6);
    Writeln(KorrMatrix[11] * 256 + KorrMatrix[12]:6);
    Write(KorrMatrix[13]   * 256 + KorrMatrix[14]:6);
    Write(KorrMatrix[15]   * 256 + KorrMatrix[16]:6);
    Writeln(KorrMatrix[17] * 256 + KorrMatrix[18]:6);
   {$R+}
   if not ReadKorrMatrixOk then
    Writeln('Fejl ved l‘sning af korrektionsmatrix');
  end; {if}
end;  {ReadKorrektionsMatrix}


Procedure  ReadNoCRTFosfor( Flag : BOOLEAN);
var Kode   : INTEGER;
    Svar   : string;
begin
 if Flag then
  Write('Antal CRT-phosphors: ');
 COM1Status := WriteCOMPort(_COM1,'MA53,');delay(20);
 COM1Status := ClearInputQue(_COM1);
 COM1Status := WriteCOMPort(_COM1,'RM,');delay(20);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 Val(Svar,NoCRTFosfor,Kode);
 if Kode = 0 then
  begin
   ReadNoCRTFosforOk := DefMaxNoCRTFosfor = NoCRTFosfor;
   if Flag then
    begin
     Write(NoCRTFosfor);
    TrueOrFalse(ReadNoCRTFosforOk);
    end;
  end
 else
  begin
   if Flag then
    Writeln('L‘se fejl !!');
   ReadNoCRTFosforOk := FALSE;
   TestFailed := TRUE;
  end; {if}
 if not ReadNoCRTFosforOk then
  TestFailed := TRUE;
end; {ReadNoCRTFosfor}


Procedure ReadCRTMatrixFromEEPROM(Flag : BOOLEAN);   {AltF6}
{L‘ser CRT-matricer fra EýPROM, gemmer v‘rdierne i en variabel og viser dem
 p† sk‘rmen. Hver matrice best†r af 12 bytes.
 Kun matricer, som har et navn, l‘ses}
var  n,k,
     Value   : BYTE;
     Svar    : string;
     Kode    : INTEGER;
     AddrStr : STR_6;
     CRTNo,
     Addr    : WORD;
begin
 ReadNoCRTFosfor(Silent);
 if Flag then
  Writeln('CRT-matrix:');
 ReadCRTMatrixOk := TRUE;
 FillChar(CRTMatrix,SizeOf(CRTMatrix),0);
 CRTNo := 1;

 repeat
  n := 1;
  Addr := 1004 - (CRTNo * 20) + 20;
  Str(Addr,AddrStr);
  COM1Status := WriteCOMPort(_COM1,'MA'+ AddrStr + ',');
  delay(20);
  COM1Status := ClearInputQue(_COM1);
  if Length(CRTName[CRTNo]) > 0 then
   begin
    repeat
     COM1Status := WriteCOMPort(_COM1,'RM,');
     delay(20);
     COM1Status := ReadlnCOMPort(_COM1,Svar,1);
     Val(Svar,Value,Kode);
      if Kode = 0 then
       CRTMatrix[CRTNo,n] := Value
      else
       begin
        ReadCRTMatrixOk := FALSE;
        TestFailed := TRUE;
       end;
     Inc(n);
    until (n > 12) OR (COM1Status <> 0) OR (Kode <> 0) OR (not ReadCRTMatrixOk);

    if Flag then
     begin
       Write('#',CRTNo,': ');
       FOR k := 1 TO n - 1 DO
        Write(CRTMatrix[CRTNo,k],' ');
       Writeln;
     end; {if flag}
    if (not ReadCRTMatrixOk) AND Flag then
     Writeln('Fejl ved l‘sning af CRT-matrix #',CRTNo);
   end  {if Length(  )}
  else
   begin
    if Flag then
     Writeln('#',CRTNo,': intet navn');
   end; {if Length(  )}

 if Flag AND (CRTNo = (NoCRTFosfor / 2)) then
   begin
    Writeln(' Tryk en tast ');
    WaitForAnykey;
   end;

 Inc(CRTNo);
 until (CRTNo > NoCRTFosfor) OR (COM1Status <> 0) OR (Kode <> 0) OR (not ReadCRTMatrixOk);

(*
{$R-}
 Writeln(CRTMatrix[1,1] * 256 + CRTMatrix[1,2]);
 Writeln(CRTMatrix[1,3] * 256 + CRTMatrix[1,4]);
 Writeln(CRTMatrix[1,5] * 256 + CRTMatrix[1,6]);
 Writeln(CRTMatrix[1,7] * 256 + CRTMatrix[1,8]);
 Writeln(CRTMatrix[1,9] * 256 + CRTMatrix[1,10]);
 Writeln(CRTMatrix[1,11] * 256 + CRTMatrix[1,12]);
 Writeln;
 Writeln(CRTMatrix[3,1] * 256 + CRTMatrix[3,2]);
 Writeln(CRTMatrix[3,3] * 256 + CRTMatrix[3,4]);
 Writeln(CRTMatrix[3,5] * 256 + CRTMatrix[3,6]);
 Writeln(CRTMatrix[3,7] * 256 + CRTMatrix[3,8]);
 Writeln(CRTMatrix[3,9] * 256 + CRTMatrix[3,10]);
 Writeln(CRTMatrix[3,11] * 256 + CRTMatrix[3,12]);
{$R+}
  *)
end;  {ReadCRTMatrix}


Procedure WriteMeasFileToLAN;
var   IOErr   : word;
      PNo     : byte;
      SaveError,
      TooManyFiles,
      FileCopied,
      FileFound  : boolean;

begin
 FileCopied := false;
 PNo := 64;                     { first measfile = xxxxxxA }
  repeat
   Inc(PNo);
   CertifFileName := PathName+KU_No+Chr(Pno)+NameExt;
   FindFirst(CertifFileName,anyfile,DirInfo);
   FileFound := DosError = 0;
   TooManyFiles := Pno > Ord('Z');
  until (not FileFound) or TooManyFiles;

  if TooManyFiles then
   begin
    write(Bell);
    writeln('ADVARSEL: Der er for mange filer med m†ledata.');
    write(  '          Der kan kun oprettes filer med m†ledata fra ');
    writeln('xxxxxxA',NameExt,' -> xxxxxxZ',NameExt);
    writeln;
    writeln('Stop yderligere m†linger og tilkald hj‘lp!');
    TestFailed := true;
    exit;
   end; { if }

   Assign(MeasFile,MeasFileName);
   {$I-} Reset(MeasFile); {$I+}
   IOErr := IOResult;
   if (not FileCopied) and (IOErr = 0) then
    begin
     if ((GetEnv('G_NAME') = 'MEASDATA') or (GetEnv('NAME') = 'PF') and
        (DiskFree(ord('S')-64) > 100000)) then    { check S disk drive > 100000 bytes}
        exec('\COMMAND.COM','/C COPY '+MeasFileName+' '+CertifFileName+' > NUL')
      else
       begin
        SaveError := true;
        CertifFileName := KU_No+Chr(Pno)+NameExt;       { lokalt drev }
        exec('\COMMAND.COM','/C COPY '+MeasFileName+' '+CertifFileName+' > NUL');
        write(Bell); delay(1000); write(Bell);
        writeln('ADVARSEL: ',MeasFileName,' kopieres ikke til LAN, men i stedet til C-drevet.');
        writeln('          Dette kan skyldes:');
        writeln('           a) du ikke er logget p† LAN.');
        writeln('           b) du ikke er medlem af LAN-gruppen MEASDATA.');
        writeln('           c) for lidt plads p† S drev.   (min. 100000 bytes).');
        writeln('          Tast fra DOS-prompt: SET <ENTER> og kontroller at G_NAME=MEASDATA.');
        writeln('          Kontakt LAN supervisor vedr. medlemskab af gruppen MEASDATA.');
        writeln('      >>> S›rg straks for at filen ',CertifFileName,' bliver gemt p† LAN i');
        writeln('          directory ',PathName);
        writeln;
       end; { if GetEnv..}

      case DosError of
       0 : begin
            writeln(MeasFileName,' kopieret til ',CertifFileName);
            FileCopied := true;
           end;
       8 : writeln(Bell,'For lidt memory ved kopiering til LAN');
       else
        writeln(Bell,'Problemer ved kopiering af filer til LAN!  ');
        writeln('DOS error: ',DosError);
        writeln;
        writeln('Stop yderligere m†linger og tilkald hj‘lp');
      end;  { case }
    end; { if not FileCopied }

   if IOErr <> 0 then
     writeln(Bell,'Filen ',MeasFileName,' findes ikke !!');

end; { WriteMeasFileToLAN }




Procedure ReadKalDataFromEEPROM;            { Alt F9 }
var   Addr       : WorD;
      CRTNo,
      n          : byte;
      k          : integer;
      Data       : byte;
      InStr,
      Svar       : string;
      CRTStr,
      DataStr,
      AddrStr    : STR_10;

Function AddSpace(InStr: STR_10) : STR_10;
begin
 case Length(InStr) of
  1 :  InStr := '    ' + InStr;
  2 :  InStr := '   ' + InStr;
  3 :  InStr := '  ' + InStr;
 end; {case}
AddSpace := Instr;
end; {addspace}

begin
 write('L‘ser kalibreringsdata .');
 ReadKaldataOk := true;
 FillChar(KalStr,SizeOf(KalStr),0);
 KU_No := ReadKU_Nummer;
 delay(50);

 ReadCRTNamesFromEEPROM(Silent);
 write('.');
 ReadCRTMatrixFromEEPROM(Silent);
 write('.');

 KalStr[1] := Marg + 'Serial no: ' +  Read12NC_Nummer + '    KU' +  KU_No;
 KalStr[2] := Marg + 'Date: ' + Dato;

 CRTNo := 1;                                    {data fra CRT #1 + #2}
 repeat
  if Length(CRTName[CRTNo]) > 0 then
   begin
     Addr := CRTMatrixAddr - (CRTNo * 20) + 20;
     Str(Addr,AddrStr);
     if Length(AddrStr) = 3 then
      AddrStr := ' ' + AddrStr;
     Str(CRTNo,CRTStr);
     KalStr[CRTNo + 2] := Marg + 'CRT-matrix #' + CRTStr + ': ' +
                           CR + LF + Marg + AddrStr + ':  ';
     for n := 1 to 12 DO
      begin
       Str(CRTMatrix[CRTNo,n],DataStr);
       DataStr := AddSpace(DataStr);
       KalStr[CRTNo + 2] := KalStr[CRTNo + 2] + DataStr;
      end; { for }
   end; { if }
  Inc(CRTNo);
 until (CRTNo > 2);

 n := 1;
 Svar[0] := #120;
 COM1Status := writeCOMPort(_COM1,'MS,'); delay(20);
 COM1Status := writeCOMPort(_COM1,'MA2,'); delay(20);
 COM1Status := ClearInputQue(_COM1);
 repeat
  COM1Status := writeCOMPort(_COM1,'RM,'); delay(20);
  COM1Status := ReadlnCOMPort(_COM1,InStr,1);
  Val(Instr,Data,k);
  if (n mod 20) = 0 then
   write('.');
  Svar[n] := Chr(Data);
  Inc(n);
 until (n > 156-1) or (COM1Status <> 0) or (k <> 0);
 Svar[0] := Chr(n);
 writeln('.');

 if (n <= 156 - 1) then
  begin
   writeln(Bell,' L‘sefejl fra color sensor');
    if COM1Status <> 0 then
     writeln(' Timeout !!');
   ReadKaldataOk := false;
   TestFailed := true;
   Exit;
  end;


 for n := 1 to 8 do                        {data fra addr 2 - 9}
  begin
   Str(Ord(Svar[n]),DataStr);
   DataStr := AddSpace(DataStr);
   KalStr[5] := KalStr[5] + DataStr;
  end;

 Addr := 10;
 for n := 9 to 120 - 1 do     {data fra addr 10 ...}
  begin
   Str(Ord(Svar[n]),DataStr);
   DataStr := AddSpace(DataStr);
   KalStr[Addr div 10+5] := KalStr[Addr DIV 10+5] + DataStr;
   if (n mod 10) = 8 then
    Inc(Addr,10);
  end;

 assign(MeasFile,MeasFileName);        { new_meas._39 }
 rewrite(MeasFile);

 writeln(MeasFile,KalStr[1]);
 writeln(MeasFile,KalStr[2]);

 writeln(MeasFile,KalStr[3]);      {CRT #1}
 writeln(MeasFile,KalStr[4]);      {CRT #2}

 writeln(MeasFile,Marg,'Protected data:');
 write(MeasFile,Marg,'   2:',Space:12);
 writeln(MeasFile,KalStr[5]);               {addr 2 - 9}

 Addr := 10;
 for n := 1 to (120 DIV 10) do              {fra addr 10 ...}
  begin
   writeln(MeasFile,Marg,Addr:4,':  ',KalStr[n + 5]);
   Inc(Addr,10);
  end; {for}

 writeln(MeasFile,Marg,'Color sensor calibration is traceable to NIST, USA');

 writeln(MeasFile,Marg,'Calibrated by: ______________');

 Close(MeasFile);


 WriteMeasFileToLAN;

end;  {ReadKalDataFromEEPROM}



Procedure WriteCertifFileToLaser(Flag : boolean);  { Alt F10 }
{ flag = true:  brugeren skal indtaste sensorens KU-nummer (evt incl version)
  flag = false  nyeste fil med data udskrives
  Hvis der kun indtastes de 6 cifre i KU-nummeret udskrives nyeste fil,
  hvorimod hvis der indtastes 6 cifre+version(A..Z) udskrives denne.
}
var
 SVer,
 IOErr     : word;
 n,
 PNo       : byte;
 SaveError,
 TooManyFiles,
 FileCopied,
 FileFound,
 CurrentFileFound  : boolean;
 InStr             : array[1..27] of str_80;
 DateStr           : str_6;
 KU_Str            : str_10;

begin
 if Flag then
     begin
      write('Indtast KU nummer: ');
      readln(KU_Str);
     end
    else
     begin
      KU_Str := ReadKU_Nummer;
      if KU_Str = '' then
       begin
        writeln(Bell,'Timeout - ingen forbindelse med sensor');
        exit;
       end;
     end;

 FileCopied := false;
 PNo := 64;                 { ‘ldste fil = xxxxxxA }
 TooManyFiles := false;

 if length(KU_Str) = 6 then
   repeat                    { s›ger efter nyeste fil }
    Inc(PNo);
    CertifFileName := PathName+KU_Str+Chr(Pno)+NameExt;
    FindFirst(CertifFileName,anyfile,DirInfo);
    FileFound := DosError = 0;

    CertifFileName := PathName+KU_Str+Chr(Pno-1)+NameExt;
    FindFirst(CertifFileName,anyfile,DirInfo);
    CurrentFileFound := DosError = 0;

    TooManyFiles := Pno > Ord('Z');
   until ((not FileFound) and CurrentFileFound) or TooManyFiles;

   if TooManyFiles then
   { s›ger efter filer med gammelt filnavn, dvs uden bogstav efter KU-nummer }
    begin
      CertifFileName := PathName+KU_Str+NameExt;
      FindFirst(CertifFileName,anyfile,DirInfo);
      FileFound := DosError = 0;
      TooManyFiles := (not FileFound);
    end;

  if length(KU_Str) = 7 then
    CertifFileName := PathName+KU_Str+NameExt;


  if TooManyFiles then
    begin
     writeln(Bell,'Sensor KU',KU_Str,': Kalibreringsdata ikke fundet i ',PathName);
     if ( not ((GetEnv('G_NAME') = 'MEASDATA') or (GetEnv('NAME') = 'PF'))) then
      begin
       writeln('OBS: ',CertifFileName,' findes ikke.');
       writeln('     Dette kan skyldes:');
       writeln('      a) du ikke er logget p† LAN.');
       writeln('      b) du ikke er medlem af LAN-gruppen MEASDATA.');
       writeln('     Tast fra DOS-prompt: SET <ENTER> og kontroller at G_NAME=MEASDATA.');
       writeln('     Kontakt LAN supervisor vedr. medlemskab af gruppen MEASDATA.');
      end;
     exit;
    end; { if }


 Assign(CertifFile,CertifFileName);
 {$I-} Reset(CertifFile); {$I+}
 IOErr := IOResult;
 if IOErr <> 0 then
   begin
    writeln(Bell,'File ',KU_Str+NameExt,' ikke fundet i ',PathName);
    exit;
   end; { if }

 SVer := 0;
 n := 1;
 while (not Eof(CertifFile)) do   { L‘ser fra fil og finder SVer og DateStr }
   begin
    Readln(CertifFile,InStr[n]);

    if (pos('Date',Instr[n]) > 0) then
     DateStr := copy(Instr[n],15,6);

    if InStr[n] <> '' then
     inc(n);
   end; { while }

 close(CertifFile);

 FLasName := 'laser.hp';
 assign(FLas,FLasName);        { file til laserjet printer }
 rewrite(FLas);

 ResetPrinter;

 SetFont(Univers18);
 MoveToXY(LeftMargin+15,13);
 write(FLas,'ProTeleVision Technologies A/S');
 HorLine(LeftMargin+15,14,0.7,170);

 SetFont(Times18);
 MoveToXY(LeftMargin+57,42);
 write(FLas,'PM 5639/90 CRT Color Sensor');

 MoveToXY(LeftMargin+77,49);
 write(FLas,'Calibration Data');

 SetFont(Courier10);
 MoveToXY(LeftMargin,80);
 write(FLas,InStr[1]);

 MoveToXY(LeftMargin,95);      { CRT #1 }
 write(FLas,InStr[3]);
 MoveToXY(LeftMargin,100);
 write(FLas,InStr[4]);

 MoveToXY(LeftMargin,110);     { CRT #2 }
 write(FLas,InStr[5]);
 MoveToXY(LeftMargin,115);
 write(FLas,InStr[6]);

 MoveToXY(LeftMargin,125);     { protected data }
 write(FLas,InStr[7]);

 for n := 8 to 20 do
  begin
   MoveToXY(LeftMargin,130+(5*(n-8)));
   write(FLas,InStr[n]);
  end;

 MoveToXY(LeftMargin,240);     { Color sensor calibration is traceable to NIST, USA }
 write(FLas,InStr[21]);

 MoveToXY(LeftMargin,255);     { calibrated by _________ }
 write(FLas,InStr[22],'        Date: ',DateStr);

 EjectPage;
 ResetPrinter;

 close(FLas);

 exec('\COMMAND.COM','/C COPY '+FLasName+' '+PrinterPort+' /B > NUL');

  case DosError of
   0 : writeln(CertifFileName,' udskrevet til printerport ',PrinterPort);
   8 : writeln(Bell,'   For lidt memory til COPY  -  tilkald hj‘lp!!');
  end;

 if DosError <> 0 then
  TestFailed := true;

end; { WriteCertifFileToLaser }




Procedure UdregnFosforMatrix(StartAddr : WORD; CRTName : STR_16);
var
 Dimen : INTEGER;          { Dimension of the square matrix }
 Data : TNmatrix;          { The matrix }
 Inv : TNMatrix;           { Inverse of Data}
 Error,                    { Errorflags }
 n        : BYTE;
 aa,rr1   : REAL;

Procedure WriteCRTMatrixToEEPROM(StAddr : WORD);
 begin
  WriteEEPROM(StAddr    ,Hi(Round(Trunc(Inv[1,2]))));
  WriteEEPROM(StAddr + 1,Lo(Round(Trunc(Inv[1,2]))));
  WriteEEPROM(StAddr + 2,Hi(Round(Trunc(Inv[1,3]))));
  WriteEEPROM(StAddr + 3,Lo(Round(Trunc(Inv[1,3]))));
  WriteEEPROM(StAddr + 4,Hi(Round(Trunc(Inv[2,1]))));
  WriteEEPROM(StAddr + 5,Lo(Round(Trunc(Inv[2,1]))));
  WriteEEPROM(StAddr + 6,Hi(Round(Trunc(Inv[2,3]))));
  WriteEEPROM(StAddr + 7,Lo(Round(Trunc(Inv[2,3]))));
  WriteEEPROM(StAddr + 8,Hi(Round(Trunc(Inv[3,1]))));
  WriteEEPROM(StAddr + 9,Lo(Round(Trunc(Inv[3,1]))));
  WriteEEPROM(StAddr + 10,Hi(Round(Trunc(Inv[3,2]))));
  WriteEEPROM(StAddr + 11,Lo(Round(Trunc(Inv[3,2]))));
  Writeln('CRT-matrix ',CRTName,' skrevet til EýPROM-adr ',StAddr,' - ',StAddr + 11);
 end;

begin
 (*                  red-gun      green-gun      blu-gun
 red   channel:        1           data 1,2      data 1,3
 green channel:      data 2,1       1            data 2,3
 blue  channel:      data 3,1      data 3,2          1
 *)

 Data[1,1] := 1;
 Data[2,1] := YR / (XR * XFact);
 Data[3,1] := (ZR * ZFact) / (XR * XFact);
 Data[1,2] := (XG * XFact) / YG;
 Data[2,2] := 1;
 Data[3,2] := (ZG * ZFact) / YG;
 Data[1,3] := (XB * XFact) / (ZB * ZFact);
 Data[2,3] := YB / (ZB * ZFact);
 Data[3,3] := 1;

 {<<<<    Red  >>>>}
 aa := Data[1,3];
 FOR n := 1 TO 10 DO
  begin
   rr1 := Data[1,2] - (aa * Data[3,2]);
   aa  := Data[1,3] - (rr1 * Data[2,3]);
  end; {for}
 Inv[1,3] := (-1 * aa  * CRTMulFactor);
 Inv[1,2] := (-1 * rr1 * CRTMulFactor);

 {<<<<    Green  >>>>}
 aa := Data[2,3];
 FOR n := 1 TO 10 DO
  begin
   rr1 := Data[2,1] - (aa * Data[3,1]);
   aa  := Data[2,3] - (rr1 * Data[1,3]);
  end; {for}
 Inv[2,3] := (-1 * aa  * CRTMulFactor);
 Inv[2,1] := (-1 * rr1 * CRTMulFactor);


 {<<<<    Blue  >>>>}
 aa := Data[3,2];
 FOR n := 1 TO 10 DO
  begin
   rr1 := Data[3,1] - (aa * Data[2,1]);
   aa  := Data[3,2] - (rr1 * Data[1,2]);
  end; {for}
 Inv[3,2] := (-1 * aa  * CRTMulFactor);
 Inv[3,1] := (-1 * rr1 * CRTMulFactor);

 Write(CRTMulFactor:10); Write(Inv[1,2]:10:2); Writeln(Inv[1,3]:10:2);
 Write(Inv[2,1]:10:2); Write(CRTMulFactor:10); Writeln(Inv[2,3]:10:2);
 Write(Inv[3,1]:10:2); Write(Inv[3,2]:10:2); Writeln(CRTMulFactor:10);

 if CRTName = 'EBU' then
  begin
  LearnEBUOk := (Inv[1,2] < Inv[1,3]) AND
                (Inv[2,1] < Inv[2,3]) AND
                (Inv[3,2] < Inv[3,1]);
  if LearnEBUOk then
    WriteCRTMatrixToEEPROM(StartAddr)
   else
    Writeln('CRT-matrix IKKE beregnet korrekt');
 end;

 if CRTName = 'SMPTE C' then
  begin
  LearnSMPTE_COk := (Inv[1,2] < Inv[1,3]) AND
                (Inv[2,1] < Inv[2,3]) AND
                (Inv[3,2] < Inv[3,1]);
  if LearnSMPTE_COk then
    WriteCRTMatrixToEEPROM(StartAddr)
   else
    Writeln('CRT-matrix IKKE beregnet korrekt');
 end;
end;  {UdregnFosforMatrix}



Procedure LearnKorrektionsMatrix;
var  n,
     Status      : BYTE;
     Svar        : string;
     Dimen : INTEGER;          { Dimension of the square matrix }
     Coef : TNmatrix;          { The matrix }
     Constants : TNvector;     { Constant terms in the equations }
     Solution : TNvector;      { Solution to the set of equations }
     Error : BYTE;             { Flags if something went wrong }

FUNCTION ValidSolution : BOOLEAN;
{Kontrollerer om en af de 9 K-matrixelementer er absolut st›rre
 end 32000/KorrMulFactor. Inden matrixelementerne skrives i EýPROM,
 multipliceres de med KorrMulFactor. Hver element skal kunne indeholdes
 i en INTEGER;
 }
var Valid : BOOLEAN;
    n     : BYTE;
    Lim   : REAL;
 begin
  Valid := TRUE;
  Lim := 32000 / KorrMulFactor;
  FOR n := 1 TO 9 DO
   if Abs(Solution[n]) > Lim then
    Valid := FALSE;
  ValidSolution := Valid;
 end;

begin
 if ReadKalStatus = 'USER' then
  begin
   Writeln(Bell,'Color sensor er USER kalibreret - forts‘t ?    J/N');
   if TastJ <> ScanCode then
    begin
     Writeln('Kalibrering afbrudt');
     Exit;
    end
  end;
 EBUWhite;
 SMPTEBlack;
 CRT625Lines;
 Beep(1000,200);
 LearnKMatrixOk := TRUE;
 Writeln('Placer PM5639 p† EBU monitoren over det hvide felt  -  tryk en tast');
 WaitForAnyKey;
 COM1Status := WriteCOMPort(_COM1,'MS,');
 COM1Status := WriteCOMPort(_COM1,'MX,MC,');
 Write('M†ling: R - ');
 EBURed;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);
 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnKMatrixOk := FALSE;
   TestFailed := TRUE;
   EBUWhite;
   Exit;
  end;
 XR := UdregnMeanValueRed;
 YR := UdregnMeanValueGreen;
 ZR := UdregnMeanValueBlue;

 Write('G - ');
 EBUGreen;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);
 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnKMatrixOk := FALSE;
   TestFailed := TRUE;
   EBUWhite;
   Exit;
  end;
 XG := UdregnMeanValueRed;
 YG := UdregnMeanValueGreen;
 ZG := UdregnMeanValueBlue;

 Write('B - ');
 EBUBlue;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);
 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnKMatrixOk := FALSE;
   TestFailed := TRUE;
   EBUWhite;
   Exit;
  end;
 XB := UdregnMeanValueRed;
 YB := UdregnMeanValueGreen;
 ZB := UdregnMeanValueBlue;

 Write('W  ');
 EBUWhite;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);
 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnKMatrixOk := FALSE;
   TestFailed := TRUE;
   EBUWhite;
   Exit;
  end;

 Writeln;
 COM1Status := WriteCOMPort(_COM1,'MS,');

 X65 := UdregnMeanValueRed;
 Y65 := UdregnMeanValueGreen;
 Z65 := UdregnMeanValueBlue;
 Write(' R›d:  ',XR:10:2); Write(YR:10:2); Writeln(ZR:10:2,'   ');
 Write(' Gr›n: ',XG:10:2); Write(YG:10:2); Writeln(ZG:10:2,'   ');
 Write(' Bl†:  ',XB:10:2); Write(YB:10:2); Writeln(ZB:10:2,'   ');
 Write(' Hvid: ',X65:10:2); Write(Y65:10:2); Writeln(Z65:10:2,'   ');

 if X65 <> 0 then
   XFact := Y65 / X65
  else
   begin
    Writeln('Error:  X65 = 0');
    LearnKMatrixOk := FALSE;
    TestFailed := TRUE;
    Exit;
   end;

 YFact := 1;

 if Z65 <> 0 then
   ZFact := Y65 / Z65
  else
   begin
    Writeln('Z65 = 0');
    LearnKMatrixOk := FALSE;
    TestFailed := TRUE;
    Exit;
  end;

 Writeln(' XFact:',XFact:6:3,'  YFact:',YFact:6:3,'  ZFact :',ZFact:6:3);

 Dimen := 12;                                {define 12 x 12 matrix}
 FillChar(Coef, SizeOf(Coef), 0);            {init 12 x 12 matrix}
 FillChar(Constants, SizeOf(Constants), 0);  {init constants}
 {R›d sk‘rm}
 Coef[1,1] := XR; Coef[1,2] := YR; Coef[1,3] := ZR; Coef[1,10] := -XpR;
 Coef[2,4] := XR; Coef[2,5] := YR; Coef[2,6] := ZR; Coef[2,10] := -YpR;
 Coef[3,7] := XR; Coef[3,8] := YR; Coef[3,9] := ZR; Coef[3,10] := -ZpR;
 {gr›n sk‘rm}
 Coef[4,1] := XG; Coef[4,2] := YG; Coef[4,3] := ZG; Coef[4,11] := -XpG;
 Coef[5,4] := XG; Coef[5,5] := YG; Coef[5,6] := ZG; Coef[5,11] := -YpG;
 Coef[6,7] := XG; Coef[6,8] := YG; Coef[6,9] := ZG; Coef[6,11] := -ZpG;
 {bl† sk‘rm}
 Coef[7,1] := XB; Coef[7,2] := YB; Coef[7,3] := ZB; Coef[7,12] := -XpB;
 Coef[8,4] := XB; Coef[8,5] := YB; Coef[8,6] := ZB; Coef[8,12] := -YpB;
 Coef[9,7] := XB; Coef[9,8] := YB; Coef[9,9] := ZB; Coef[9,12] := -ZpB;
 {hvid sk‘rm}
 Coef[10,1] := X65; Coef[10,2] := Y65; Coef[10,3] := Z65;
 Coef[11,4] := X65; Coef[11,5] := Y65; Coef[11,6] := Z65;
 Coef[12,7] := X65; Coef[12,8] := Y65; Coef[12,9] := Z65;

 Constants[10] := NominelLum / Yp65 * Xp65;
 Constants[11] := NominelLum;
 Constants[12] := NominelLum / Yp65 * Zp65;

 Gaussian_Elimination(Dimen, Coef, Constants, Solution, Error);

 Write('K-matrix ');
 if (Error <> 0) OR (not ValidSolution) then
   begin
    Writeln('IKKE beregnet korrekt');
    Writeln('Ingen CRT-matrix beregnet');
    LearnKMatrixOk := FALSE;
    TestFailed := TRUE;
   end
  else
   begin
    Writeln('beregnet ved ',NominelLum:4:1,' cd/mý');
    FOR n := 0 TO 8 DO
     begin
      Solution[n + 1] := Solution[n + 1] * KorrMulFactor;
      Write(Solution[n + 1]:10:2);
       if (n + 1) MOD 3 = 0 then Writeln;
     end; {for}
    FOR n := 0 TO 8 DO
     begin
      WriteEEPROM(KorrAddr + (n * 2),    Hi(Trunc(Round(Solution[n + 1]))));
      WriteEEPROM(KorrAddr + (n * 2) + 1,Lo(Trunc(Round(Solution[n + 1]))));
     end; {for}

    WriteEEPROM(PreGainAddr    ,Hi(Trunc(Round(XFact * PreGainMulFactor))));
    WriteEEPROM(PreGainAddr + 1,Lo(Trunc(Round(XFact * PreGainMulFactor))));
    WriteEEPROM(PreGainAddr + 2,Hi(Trunc(Round(YFact * PreGainMulFactor))));
    WriteEEPROM(PreGainAddr + 3,Lo(Trunc(Round(YFact * PreGainMulFactor))));
    WriteEEPROM(PreGainAddr + 4,Hi(Trunc(Round(ZFact * PreGainMulFactor))));
    WriteEEPROM(PreGainAddr + 5,Lo(Trunc(Round(ZFact * PreGainMulFactor))));

    WriteEEPROM(157,$FF);      {Cal status = FACTORY   (vedr. PM8550)}

    Writeln('K-matrix og Pre-gain skrevet til EýPROM');
    Writeln('Kalibreringsstatus = FACTORY');
    WriteCalDate;
   end; {if}

 if LearnKMatrixOk then
 UdregnFosforMatrix(CRTMatrixAddr,'EBU');
end; {LearnKorrektionsMatrix}



Procedure LearnSMPTE_CFosfor;
var  n,
     Status      : BYTE;
     Svar        : string;
     Dimen : integer;          { Dimension of the square matrix }
     Coef : TNmatrix;          { The matrix }
     Constants : TNvector;     { Constant terms in the equations }
     Solution : TNvector;      { Solution to the set of equations }
     Error : byte;             { Flags if something went wrong }
begin
 SMPTEWhite;
 EBUBlack;
 ReadPreGainFactor;
 LearnSMPTE_COk := TRUE;
 XFact := XFact;
 YFact := YFact;
 ZFact := ZFact;
 Beep(1000,200);
 Writeln('Placer PM5639 p† SMPTE C monitoren over det hvide felt  -  tryk en tast');
 WaitForAnyKey;
 COM1Status := WriteCOMPort(_COM1,'MX,MC,');
 Write('M†ling: R - ');
 SMPTERed;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);

 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnSMPTE_COk := FALSE;
   TestFailed := TRUE;
   SMPTEWhite;
   Exit;
  end;

 XR := UdregnMeanValueRed;
 YR := UdregnMeanValueGreen;
 ZR := UdregnMeanValueBlue;

 Write('G - ');
 SMPTEGreen;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);

 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnSMPTE_COk := FALSE;
   TestFailed := TRUE;
   SMPTEWhite;
   Exit;
  end;

 XG := UdregnMeanValueRed;
 YG := UdregnMeanValueGreen;
 ZG := UdregnMeanValueBlue;

 Write('B  ');
 SMPTEBlue;
 Delay(LearnDelay);
 n := 1;
 COM1Status := ClearInputQue(_COM1);
 COM1Status := ReadlnCOMPort(_COM1,Svar,1);
 repeat
  COM1Status := ReadlnCOMPort(_COM1,Svar,1);
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) OR (COM1Status <> 0);

 if (COM1Status <> 0) then
  begin
   Writeln('Ingen m†ling');
   LearnSMPTE_COk := FALSE;
   TestFailed := TRUE;
   SMPTEWhite;
   Exit;
  end;

 XB := UdregnMeanValueRed;
 YB := UdregnMeanValueGreen;
 ZB := UdregnMeanValueBlue;

 Writeln;
 SMPTEBlack;
 Write(' R›d:  ',XR:10:2); Write(YR:10:2); Writeln(ZR:10:2,'   ');
 Write(' Gr›n: ',XG:10:2); Write(YG:10:2); Writeln(ZG:10:2,'   ');
 Write(' Bl†:  ',XB:10:2); Write(YB:10:2); Writeln(ZB:10:2,'   ');

if LearnSMPTE_COk then
 UdregnFosforMatrix(CRTMatrixAddr - 20,'SMPTE C');
end; {LearnSMPTE_CFosfor}


Procedure TotalTest;
begin
 WriteProtectYesOrNo(Silent);
 if SP6WriteProtectOk then
  begin
   Writeln(Bell,'Color sensor m† IKKE v‘re skrivebeskyttet ved kalibrering !!');
   Exit;
  end;

 if ReadKalStatus = 'USER' then
  begin
   Writeln(Bell,'Color sensor er USER kalibreret - forts‘t ?    J/N');
   if TastJ <> ScanCode then
    begin
     Writeln('Kalibrering afbrudt');
     Exit;
    end
  end;
 ClearTestFlag;
 EEPROMTestOk := TRUE;
{$ifdef IEEEBUS}
 SupplyCurrentTest;
 AnalogProbeTest;
{$endif}
 RS232Test;
 if TestAfbrudt then Exit;
 if not TestFailed then
  begin
   InitPtvEEPROM;
   PowerOffOn;
  end;

 if TestAfbrudt then Exit;
 if not TestFailed then
  begin
   WriteCRTNamesToEEPROM;
   WriteIdString;
   WriteDefaultGainFactor;
   WriteDefaultPreGain;
   LearnOffset;
  end;
 if TestAfbrudt then Exit;
 if not TestFailed then
   EnterKU_Nummer;
 if TestAfbrudt then Exit;
 if not TestFailed then
   FieldRateTest;
 if TestAfbrudt then Exit;
 if not TestFailed then
   DarkCurrentTest;
 if TestAfbrudt then Exit;
 if not TestFailed then
   BeregnGainFactor;
 if TestAfbrudt then Exit;
 if not TestFailed then
   LearnKorrektionsMatrix;
 if TestAfbrudt then Exit;
 if not TestFailed then
   LearnSMPTE_CFosfor;
 if TestAfbrudt then Exit;
 if not TestFailed then
  begin
   ReadGainFactor;
   ReadIdString;
   ReadPreGainFactor;
   ReadNoCRTFosfor(NotSilent);
   ReadNoEEPROMPages;
   ReadInternID;
   ReadCRTNamesFromEEPROM(Silent);
   ReadCRTMatrixFromEEPROM(Silent);
   ReadKMatrix(NotSilent);
 end;

 TotalTestOk :=  RS232TestOk AND
                 CurrentTestOk AND
                 EEPROMTestOk AND
                 FieldRateTestOk AND
                 DarkCurrentTestOk AND
                 BeregnGainFactorOk AND
                 FilterOk AND
                 ValidKU AND
                 ReadCRTNameOk AND
                 ReadKorrMatrixOk AND
                 ReadCRTMatrixOk AND
                 ReadIDStringOk AND
                 ReadInternIDOk AND
                 ReadPreGainOk AND
                 ReadNoCRTFosforOk AND
                 ReadNoEEPROMPagesOk AND
                 LearnKMatrixOk AND
                 LearnEBUOk AND
                 LearnSMPTE_COk AND
                 (not TestFailed);

  Write('Totaltest ');
  TrueOrFalse(TotalTestOk);

 if Analog5VTestValgt then
  TotaltestOk :=  TotaltestOk AND AnalogPos5VTestOk AND AnalogNeg5VTestOk;
 if ADRefTestValgt then
  TotaltestOk :=  TotaltestOk AND ADRefTestOk;

 if TotalTestOk then
  begin
   WriteEEPROM(54,Lo(DefMaxWriteProtectAddr));
   WriteEEPROM(55,Hi(DefMaxWriteProtectAddr));
   WriteOkToEEPROM;
   ReadEEPROMWriteProtect(NotSilent);
   ReadKalDataFromEEPROM;
   WriteCertifFileToLaser(false);
  end
  else
   Writenot_OkToEEPROM;

  TotaltestOk :=  TotaltestOk AND
                  ReadKaldataOk AND
                  ReadWriteProtectOk;

 Write('Kalibrering OK ?:');
 TrueOrFalse(FinalTestOk);

 if TotalTestOk then
   Writeln('Color sensor  G O D K E N D T')
  else
   Writeln('Color sensor  A F V I S T');
end; {TotalTestNormal}


begin           {>>>> main program <<<<}
 SetCBreak(TRUE);
 ClrScr;
 Initprinter;
 InitWindow;
 InitScreen;
 InitVar;
 InitPC_Com1Port;
{$ifdef IEEEBUS}
 InitIeeeBus;
{$endif}
 InitI2CBus;
 RGBGenRemote;
 SMPTEBlack;
 EBUBlack;
{$ifdef RGBGEN}
 DAVoltage(7.1);  {sp‘nding fra D/A-converter i RGB-generator}
{$endif}
 LampOff;

 if not AfslutProgram then
 repeat
  EmptyKeyboardBuffer;
  TestFailed := FALSE;

  Set_Timer(SONY_Timeout*10);

  repeat
  until Timer_Timeout OR KeyPressed;

  if Timer_Timeout then
   begin
    EBUBlack;
    SMPTEBlack;
    Writeln('SONY-monitorer = OFF');
   end;

   CASE ScanCode OF
{$ifdef IEEEBUS}
       F1 : begin
             SupplyCurrentTest;
             AnalogProbeTest;
            end;
{$endif}
       F2 : EnterKU_Nummer;
       F3 : ShutdownControl;

       F4 : begin
             RS232Test;
              if RS232TestOk then
               begin
                ReadGainFactor;
                ReadIdString;
                ReadPreGainFactor;
                ReadEEPROMWriteProtect(NotSilent);
                ReadNoCRTFosfor(NotSilent);
                ReadNoEEPROMPages;
                ReadInternID;
                WriteProtectYesOrNo(NotSilent);
                Writeln('Kal status:   ',ReadKalStatus);
                ReadCalDate(NotSilent);
                Writeln('Trace status: ',ReadTraceFlag);
               end; {if}
            end;
       F5 : FieldRateTest;
       F6 : DarkCurrentTest;
       F7 : BeregnGainFactor;
{$ifdef RGBGEN}
       F8 : LearnSMPTE_CFosfor;
       F9 : LearnKorrektionsMatrix;
{$endif}
      F10 : begin
             TotalTestFlag := TRUE;
             TotalTest;
             TotalTestFlag := FALSE;
            end;
   AltF3  : begin
             InitPtvEEPROM;
             PowerOffOn;
            end;
   AltF4  : LearnOffSet;
   AltF5  : WriteIDString;
   AltF6  : WriteDefaultGainFactor;
   AltF7  : WriteDefaultPreGain;
   AltF8  : WriteCRTNamesToEEPROM;
   AltF9  : begin
             ReadKalDataFromEEPROM;
            end;
   AltF10 : begin
	     WriteCertifFileToLaser(true);
            end;

{   AltF9  : begin
             ReadKalDataFromEEPROM;
	     WriteCertifData('FILE',false);
            end;
   AltF10 : begin
             ReadKalDataFromEEPROM;
	     WriteCertifData('PRN',false);
            end;}

   CtrlF1 : ReadCRTMatrixFromEEPROM(NotSilent);
   CtrlF2 : ReadKMatrix(NotSilent);
   CtrlF3 : ReadCRTNamesFromEEPROM(NotSilent);
   CtrlF4 : WriteProtectYesOrNo(NotSilent);
   CtrlF5 : ShowActualXYValues;
   CtrlF6 : ShowOffsetValues(NotSilent);
   CtrlF7 : begin
             TestModePM5639;
             WriteEEPROM(54,Lo(DefMaxWriteProtectAddr));
             WriteEEPROM(55,Hi(DefMaxWriteProtectAddr));
            end;

   TastA : Enterlevel;
   TastC : ClrScr;
   TastE : ToggleLED;
   TastF : ToggleFreq;
   TastH : ShowHelpWindow;
   TastL : ToggleLamp;
   TastM :  begin
             ReadKMatrix(Silent);
             PTVxyMeasure(KorrMatrix,KorrMulFactor,
                          D6500Ref[1],D6500Ref[2],NIT_fL_Factor);
            end;
   TastO :  begin
             ShowOffsetValues(Silent);
             STMeasure(Offset,ADRef,ADCounts,4);
            end;
   TastP : PowerOffOn;
   TastR : ToggleRelay;
   AltR  : TestRelayRGB;
   TastW : begin
            EBUWhite;
            SMPTEWhite;
           end;


   Escape : AfslutProgram := TRUE;
     else
      Write('?? ');
   end; {case ScanCode}

   Writeln('*');

 until AfslutProgram;


 if IeeeFilesOpen then
  CloseIeeeFiles;
 Window(1,1,80,25);

 if not IICBusError then
  begin
{$ifdef LYSKASSE}
   Trm8574(I2CIOAddr,64,$FF,St);
   LampOff;
{$endif}
   EBUBlack;
   SMPTEBlack;
   RGBGenLocal;
  end;

 Window(1,1,80,25); Color(LightGray,Black); ClrScr;

 COM1Status := CloseCOMPort(_COM1);
 if COM1Status <> 0 then
  Writeln('COM1 ikke lukket korrekt: ',COM1Status);

end.
