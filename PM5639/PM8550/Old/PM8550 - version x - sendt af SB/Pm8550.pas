{$A+,B-,D+,E+,F-,G-,I+,L+,N-,O-,P-,Q-,R-,S+,T-,V+,X+,Y+}
{$M 16384,0,655360}
Program PM8550;
{
 >>>>>>>>>>>>>>>>  BEM’RK  <<<<<<<<<<<<<<<<

 941108   PF/NBe
 Ved ‘ndring af 8550 skal overvejes en yderligere kontrol af elementerne
 i K-matricen. Nu kontrolleres kun om elementerne er mindre end 32000.
 En yderligere kontrol kunne v‘re en nedre gr‘nse, f.eks 1000.
 Se function ValidSolution.

}

{***********************************************************************}
{ Main for Calibration software PM8550			              	}
{ RS232 uses 4800 or 9600 baud

 921229:
 Programmet testet med monochrome video adaptor "ATI Graphics Solution"
 SW1=on  SW2=off  SW3=on  SW4=off  JP1=open  JP2=op  JP3=open
 Monitor = Philips BM7713
}

{
 Rettelser:
 921124 V1.1  - Beskrivelse af write protect ON/OFF ved sensor SW version 5.7
                tilf›jet. SW version l‘ses ved opstart af program.
              - Fejlmeddelelse ved indtastning af koordinater rettet.
 921210 V1.2  - Menustyring lagt helt om til Turbo Vision farver og taster.
              - User text indf›rt.
              - Kalibrering af kun luminance indf›rt.
              - WP styres af software version i sensor: I version 02.0 kan WP
                s‘ttes ON/OFF med kommandoer til sensoren.
 930423 V1.3  - Ekstra forsinkelser (10mS) ved l‘sning fra sensoren indf›rt.
 931201 V1.31 - 'All rights reserved' tilf›jet opstartsbillede
 941005 V1.40 - 9600 or 4800 baud can be selected on command line.
                Output format l‘ses i adresse 95.
 990914 V1.50 - Opstartsbillede ‘ndret fra Philips til PTV
                Opstartsgrafik i CGA slettes.
                Ny TURBO.TPL fil anvendt for at eliminere 'error 200'
                ved start af programmet p† > ca 300MHz PC.
 000321 V2.00 - Luminance gr‘nse ‘ndret fra 200 til 1000 NIT (=291 ftL)
                README-fil rettet, HLP8550.TXT rettet, Manual rettet

}


{$DEFINE PTVLOGO}
{ DEFINE DEBUGMODE}

{$G-}      {8086 instructions only}

uses	Dos,Crt,		{ Turbo Pascal DOS and CRT units }
	Unit_Sup,		{ POWER TOOLS PLUS Support  unit }
	Unit_Hlp,		{ POWER TOOLS PLUS Help     unit }
	Unit_Key,		{ POWER TOOLS PLUS Keyboard unit }
	Unit_Scn,		{ POWER TOOLS PLUS Screen   unit }
	Unit_Win,		{ POWER TOOLS PLUS Window   unit }
	Unit_Mnu,		{ POWER TOOLS PLUS Menu	    unit }
	Unit1Edt,		{ POWER TOOLS PLUS Edit	    unit ** }
	ComUtil,		{ ASYNC PLUS       RS232    unit }
{$ifdef PTVLOGO}
        PhLogo,                {Opstarts billede med PHILIPS}
{$endif}
        KM;                     { Calculations              unit }

{  **   Unit_Edt ‘ndret i routinen __FieldEdt (version = 7.01)}

type	STR_10           = string[10];
	STR_5            = string[5];
	STR_30           = string[30];
	STR_60           = string[60];
        IntIDType        = ARRAY[1..4] OF BYTE;


const
        SerialNo      = 'KU'+'WWWWWW';
        SWVer         = '2.00';              {Software version}
	SWName        = 'PM8550';
        SWYear        = '1992-2000';

        UserTxt : array[1..26] of STR_60 =
                  (' Writing user calibration data failed',
                   ' Program stopped ...',
                   ' Error #20: Sensor not a CRT Color Sensor',
                   ' Calibration failed!',
                   ' Old calibration not changed',
                   ' Error #21: Check that display unit is ON',
                   ' Check sensor connection with display unit',
                   '      User calibration will be overwritten',
                   '                   Continue  ?',
           {10}    ' Error #22: No connection with sensor',
                   ' Write Protection is set to OFF',
                   ' Write Protection is set to ON',
                   '           User calibration passed  ',
                   '           Set "Write Protect ON" ? ',
                   ' Error #23: Sensor is write protected and user     ',
                   ' calibration is not possible.',
                   ' Please use the menu "Write Protect".',
                   '    Status is read  -  see the Status Field',
                   '                 Exit program ?',
          {20}     ' Error #24: Color sensor is FACTORY calibrated',
                   ' Color sensor is restored to factory calibration.',
                   ' User calibration is overwritten.',
                   ' "User Text" in color sensor is unchanged.',
                   ' "User Text" written to color sensor.',
                   ' Color and luminance calibration is unchanged.',
                   ' Please wait . . .');


	Col1               = 1;          {menu item column/row positions}
	Col2               = 10;
        Col3               = 16;
        Col4               = 27;
        Col5               = 33;
        Col6               = 47;
        Col7               = 42;
        Col8               = 58;
        Col9               = 50;
        Col10              = 61;
        Col11              = 75;
        Row1               = 1;
        Row3               = 5;
        Row5               = 7;
        Row6               = 8;
        Row7               = 9;
        Row9               = 11;
        Row11              = 13;
        Row13              = 15;
        Row14              = 16;
        Row15              = 17;
        Row16              = 18;

        RGB_OutputFormat   = 1;
        RGBI_OutputFormat  = 2;

        DefaultMaxLum  : real = 1000;       {max luminance in NIT}
        DefaultMinLum  : real = 10;         {min luminance in NIT}
        MaxCoor : REAL     = 0.60;       {max xy/u'v' koordinat}
        MinCoor : REAL     = 0.10;       {max xy/u'v' koordinat}

        AntalMeas          = 7;          {antal m†linger ved MX-mode}
	EEPROMWriteDelay   = 40;         {delay in mS}
	KorrMulFactor      = 1E5;        {multiplikationsfaktor for K-matrix}
	Cd_ftL_Factor : real = 0.29186;  {fra Television Engineering Handbook}
                                         {McGraw Hill, 1986}
                                         {table 22-10, page 22.25}

type    MeasStrType      = array[1..AntalMeas] of string[30];

var
	TitleWin,
	MainWin, BottomWin,
	EditxWin,
        EdityWin,EditLumWin,
        EditUserTxtWin,
        MessWin, MeasWin,
        CalSetupWin,
        WPOnOffWin, WPWin,
        YesOrNoWin, RetryWin,
        WaitWin                  	: _WindowPtr;
        CalSetupMnu,
	MainMnu, WPOnOffMnu,
        YesOrNoMnu, RetryMnu 	        : _MenuPtr;
        CalSetupPtr,
	MainPtr, IPtr,
        WPPtr, RetryPtr                 : _ItemPtr;
	KPtr                            : _KeyPtr;
	Key                             : _Keys;
        EditKey                         : _KeyEdt;

	TopFore,TopBack,
	MenuFore,MenuBack,
	BarFore,BarBack,
	BordFore,BordBack,
	LFore,LBack,
	PFore,PBack,
	HFore,HBack,
	MainFore,MainBack,
        TittleFore,TittleBack,
        MessFore,MessBack,
        RetryFore, RetryBack,
        ShaFore,ShaBack,                        {shadow}
        ErrorFore,ErrorBack,
        EditFore,EditBack,
        OutputFormat,
	CIEMode,                                {1=xy     2=u'v'  3=uv}
	LumMode                 : byte;         {1=cd/mý  2=NIT   3=ftL}
        FactoryCal,                             {Aktiv K-matrix = Factory eller ej}
        SensorConnected,
        SensorWP,
        SensorValid,
        SensorPhilips,
        ReadKMOk,
	Finished,
        WaitMessageOn,
        Check,
        COMPortOpen            	: boolean;
        COMStatus,
        ErrCode,
        NumMess,                              {antal help index}
        PortNo      		: WORD;
	EditCtrl		: _CtrlEdt;
        SensorSWStr,
        SensorUserText,
        KUStr                   : STR_10;
        Svar                    : string;
        IntID                   : IntIDType;
	KElement		: KMType;


        MaxLum,
        MinLum,
        XR,YR,ZR,                    {m†lte/beregnede v‘rdier i MX-mode ved }
        XG,YG,ZG,                    {beregning af matricer.}
        XB,YB,ZB,                    {XR=R›d kanal v. r›d sk‘rm}
        X65,Y65,Z65,                 {YR=Gr›n kanal v. r›d sk‘rm}
                                     {XG=R›d kanal v. gr›n sk‘rm}
                                     {Y65=Gr›n kanal v. hvid sk‘rm     osv}

        XpR,YpR,ZpR,                 {red       xyz-v‘rdier i for monitor}
        XpG,YpG,ZpG,                 {green     fra felterne x= og y=}
        XpB,YpB,ZpB,                 {blue}
        Xp65,Yp65,Zp65,              {white ~ D6500}

        LumValue                : real;


Procedure WaitMessage(Mess1: string);
begin
 Check := __DispWin(WaitWin);
 if Check then
  begin
   WaitMessageOn := TRUE;
   Write(Mess1);
  end;
end;

Procedure CloseWaitMessage;
begin
 if WaitMessageOn then
  begin
   Check := __RemWin;
   if Check then
    WaitMessageOn := FALSE;
  end;
end;



Function  RetryYesOrNo(St : BOOLEAN; Mess1, Mess2 : string;
                                              BeepMode : BYTE) : BOOLEAN;
begin
 RetryPtr := nil;
 if St then
  begin
   RetryYesOrNo := FALSE;
  end
 else
  begin
   case BeepMode of
   1 : begin
        Sound(1500); Delay(150); NoSound;
       end;
   2 : begin
        Sound(400); Delay(500); NoSound;
       end;
   end;  {case}
  CloseWaitMessage;
  Check := __DispWin(RetryWin);
  Writeln(Mess1);
  Writeln(Mess2);
  RetryPtr := nil;
  __FlushKey;
  if __DispMnu(RetryMnu) then
   RetryPtr := __ReadMnu(RetryMnu,RetryPtr,_MBEEP_UNKNOWN_MNU + _MKEEP_HIGHLIGHT_MNU,Key);
  if RetryPtr^._Row = 5 then
    RetryYesOrNo := TRUE
   else
    RetryYesOrNo := FALSE;

  Check := __RemWin;
 end; {if}
end;

Procedure Help8550; external;
{$L HLP8550.OBJ}

{$I HELP.INC}

{$I CAL1.INC}

{$I WP.INC}

{$I CAL2.INC}

{ ------------------------ main ----------------------------------}

begin

 PortNo := 1;                  {COM port #}

 InitPC_COMPort(COMPortOpen);

 if Finished then
  halt;


{$ifdef PTVLOGO}
 StartUpScreen('Serial number: '+ SerialNo,SWName+'  V'+SWVer,'ProTeleVision Technologies A/S, Denmark ',
               SWYear);
{$endif}


 SensorValid := FALSE;
 SensorPhilips := FALSE;
 WaitMessageOn := FALSE;

 CIEMode := 1;              {1: xy      2:u'v'   3:uv}
 LumMode := 1;              {1: cd/mý   2:NIT    3:ftL}

 MaxLum := DefaultMaxLum;
 MinLum := DefaultMinLum;

 Xp65     := 0;
 Yp65     := 0;
 LumValue := 0;

 Finished := FALSE;

 MainPtr     := nil;              { bar on default item }
 WPPtr       := nil;
 CalSetupPtr := nil;
 IPtr        := nil;
 RetryPtr    := nil;

 KPtr        := nil;              {keys}

 __COffScn(TRUE);              {Cursor off}


 if _CurRows <> 25 then          {_CurRows is set during unit initialization}
  begin
   __NewDvScn(_UNKNOWN,25,ErrCode);   {current video adaptor set to 25 lines}
   if ErrCode <> 0 then
    begin
     Writeln('Error #2: Video adaptor initialization failed - program halted');
     Halt;
    end;
  end;

 InitScreenColor;
 CreateAllWindows;
 CreateAllMenus;
 WriteBottomWindow;
 Check := __DispMnu(MainMnu);


 __OpenHlp('',@Help8550,TRUE,NumMess);

 DrawBox1(Col1+5,Row3,Col1+74,Row16,' CRT Color Sensor Calibration ');
 DrawBox1(Col1+6,Row5,Col5+6,Row14,' Calibration Status ');
 DrawBox1(Col5+8,Row5,Col1+73,Row14,' Calibration Data ');






 repeat
  GetStatus(SensorConnected,
            SensorPhilips,
            SensorValid,
            FactoryCal,
            SensorWP,
            KUStr,
            SensorSWStr,
            SensorUserText,
            OutputFormat);
  Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
 until SensorConnected OR Finished;

  __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
  __QuikWin(Col8,Row13,MainFore,MainBack,SensorUserText);

 if (not SensorPhilips) and SensorConnected then
  begin
   Finished := TRUE;
   UserMessage(UserTxt[10],UserTxt[2],2);
  end;


 if (not Finished) and SensorConnected then
  begin
   if (not SensorValid) then
    begin
     Finished := TRUE;
     UserMessage(UserTxt[3],UserTxt[2],2);
    end;

   if (not Finished) and SensorWP then
     UserMessage(UserTxt[15]+UserTxt[16], UserTxt[17],2);
  end; {if}


 while not Finished do
 begin
  __FlushKey;
  MainPtr := __ReadMnu(MainMnu,MainPtr,
             _MBEEP_UNKNOWN_MNU + _MKEEP_HIGHLIGHT_MNU,
             Key);

  case Key of
    _Key_a_x  :  Finished := TRUE;
    _Key_F1   :  case MainPtr^._Col of
                  Col1:  Help('readstatus');
                  Col3:  Help('writeprotect');
                  Col5:  Help('calsetup');
                  Col7:  Help('lumcieunit');
                  Col9:  Help('restore');
                  Col10: Help('startcal');
                  Col11: Help('index');
                 end;
  {$ifdef DEBUGMODE}
    _Key_F9  :  WriteTittleWindow;
    _Key_c   :  SetFactoryCal(SensorConnected);
    _Key_s_c :  SetUserCal(SensorConnected);
    _Key_k   :  CalFactoryKM;
    _Key_r   :  begin
                 ReadKMatrix(103,SensorConnected,KElement);
                 ShowKMatrix;
                end;
    _Key_w   :  WriteKMatrix(103,SensorConnected);
  {$endif}
  else  {case}
   if MainPtr^._Row = Row1 then
    begin
     case MainPtr^._Col of
       Col1  : begin
                repeat
                  GetStatus(SensorConnected,
                            SensorPhilips,
                            SensorValid,
                            FactoryCal,
                            SensorWP,
                            KUStr,
                            SensorSWStr,
                            SensorUserText,
                            OutputFormat);
                 Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
                until SensorConnected OR Finished;
                  __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
                  __QuikWin(Col8,Row13,MainFore,MainBack,SensorUserText);

                 if (not SensorPhilips) then
                  begin
                   Finished := TRUE;
                   UserMessage(UserTxt[10],UserTxt[2],2);
                  end;


                 if (not Finished) and SensorConnected then
                  begin
                   if (not SensorValid) then
                    begin
                     Finished := TRUE;
                     UserMessage(UserTxt[3],'',2);
                    end;

                   if (not Finished) and SensorWP then
                    UserMessage(UserTxt[15]+UserTxt[16],UserTxt[17],2);
                  end; {if}

                if SensorConnected and (not Finished) then
                 UserMessage(UserTxt[18],'',1);
               end;
       Col3  : WriteProtectMenu;
       Col5  : EnterCalSetup;
       Col7  : EnterCalData;
       Col9  : RestoreFactoryCal;
       Col10 : Calibration;
       Col11 : Help('index');

     end; {case}
    end {if}
  end; {case Key}


 if Finished and SensorValid then
  if not YesOrNo(UserTxt[19],'') then
   Finished := FALSE;

 end; {while}

 Check := __CurWin(MainWin);

 COMStatus := CloseCOMPort(PortNo);
 Check := __ZapWin(MainWin);
 Check := __ZapWin(BottomWin);
 __COffScn(FALSE);     {cursor=on}
end.




