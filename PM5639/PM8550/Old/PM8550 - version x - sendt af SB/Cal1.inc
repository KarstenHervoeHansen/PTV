(*
CALUTIL1.INC   Utility for PM8550.PAS file

Procedure InitPC_COMPort
Function RealToStr
Procedure ReadKU
Procedure ReadIntID
Procedure WriteEEPROM
Function  WriteProtectYesOrNo
Procedure ReadKMatrix
Procedure WriteKMatrix
Procedure ShowKMatrix
Procedure WriteBottomWindow
Procedure WriteTittleWindow
Procedure InitScreenColor
Procedure CreateAllWindows
Procedure CreateAllMenus
Procedure UserMessage
Function YesOrNo
Procedure  ReadUserText
Procedure WriteUserText
Procedure GetStatus
Procedure SetUserCal
Procedure SetFactoryCal
Procedure Help
Procedure Enter_x
Procedure Enter_y
Procedure Enter_LumValue
Procedure RestoreFactoryCal
Procedure CalFactoryKM
*)


Procedure InitPC_COMPort(var St : boolean);
{Init serial COM-port.}
var Baud : byte;
begin
 ErrCode := 0;
 Baud := _Baud4800;

 if Paramcount > 0 then
 begin
  if (ParamStr(1) = '/B:48') or (ParamStr(1) = '/b:48') then
    Baud := _Baud4800
   else
  if (ParamStr(1) = '/B:96') or (ParamStr(1) = '/b:96') then
    Baud := _Baud9600
   else
  if (ParamStr(1) = '/H') or (ParamStr(1) = '/h') or (ParamStr(1) = '/?') then
   begin
    ErrCode := 1;   { help command line parameter }
    Finished := true;
   end
  else
   begin
    ErrCode := 2;   { not a valid command line parameter }
    Finished := true;
   end;
 end; { if Paramcount }

 if ErrCode <> 1 then
  begin
   COMStatus := OpenCOMPort(PortNo,100,100);      {com#, inquesize, outquesize}
   St := COMStatus = 0;
   COMStatus := InitCOMPort(PortNo,Baud,_Databit8,_Stopbit2,_None,_CTSNotRequired);
   St := St and (COMStatus = 0);

   if (not St) then
    begin
     ErrCode := 3;  { init COM port failed }
     Finished := true;
    end;
  end;


 if (ErrCode > 0) then
 begin
  writeln('PM8550 Color Sensor Calibration');
  writeln('Version: ',SWVer);
  writeln('Serial No. ',SerialNo);
  writeln('(c) ProTeleVision Technologies A/S, 1992-1999');
  writeln;
  writeln('PM8550 [/B:48 | 96]  [/?]');
  writeln;
  writeln(' /B:48    -  Select 4800 baud rate (default)');
  writeln(' /B:96    -  Select 9600 baud rate');
  writeln(' /?       -  This help display  (also /H or /h)');
  writeln;
  writeln('Example:   PM8550 /B:96');
 end;

 if (ErrCode = 3) then
  begin
   writeln;
   Writeln('Error #1: COM',PortNo,' initialization failed - program halted');
   Finished := true;
  end;

end; { InitPC_COMPort }





Function RealToStr(i : REAL; w,d: BYTE): string;
var
  s: string[15];
 begin
   Str(i:w:d, s);
   RealToStr := s;
 end;



Procedure ReadKU(var KU, SensorSW : STR_10; var St : BOOLEAN);
{L‘ser KU-nummer fra EýPROM}
var  Svar : string;
begin
 COMStatus := WriteCOMPort(PortNo,'MS,');
 COMStatus := ClearInputQue(PortNo);
 COMStatus := WriteCOMPort(PortNo,'I?,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 if COMStatus <> 0 then
  begin
   COMStatus := ClearInputQue(PortNo);
   COMStatus := WriteCOMPort(PortNo,'I?,');
   COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
  end;
 St := COMStatus = 0;
 if St then
   begin
    KU := Copy(Svar,22,8);
    SensorSW := Copy(Svar,31,4);
   end
  else
   begin
    KU := '------';
    SensorSW := '----';
   end;
end;

Procedure ReadIntID(var ID : IntIDType; var SensPh,SensValid, St : BOOLEAN;
          WaitOn : BOOLEAN);
var  k : INTEGER;
begin
 FillChar(ID,SizeOf(ID),0);
 Svar := '';
 COMStatus := WriteCOMPort(PortNo,'MS,');
 Delay(5);
 COMStatus := WriteCOMPort(PortNo,'MA58,');
 Delay(5);
 COMStatus := ClearInputQue(PortNo);
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 if COMStatus <> 0 then
  begin
  if WaitOn then
   WaitMessage(' Please wait . . .');
   COMStatus := WriteCOMPort(PortNo,'MS,');
   Delay(5);
   COMStatus := WriteCOMPort(PortNo,'MA58,');
   Delay(5);
   COMStatus := ClearInputQue(PortNo);
   COMStatus := WriteCOMPort(PortNo,'RM,');
   COMStatus := ReadlnCOMPort(PortNo,Svar,1);
   Delay(10);
  end;
 St := COMStatus = 0;
 Val(Svar,ID[1],k);
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 St := St and (COMStatus = 0);
 Val(Svar,ID[2],k);
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 St := St and (COMStatus = 0);
 Val(Svar,ID[3],k);
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 St := St and (COMStatus = 0);
 Val(Svar,ID[4],k);
 SensPh := ID[1] = 1;
 SensValid := SensPh and
              ((ID[4] IN [0..15]) OR (ID[4] IN [32..63]));
                               {EýPROM addr      58 59 60  61   }
                               {Int ID: 5639/90:  1  0  0  0-15 }
                               {      : 5639/91:  1  0  0  16-31}
                               {      : 5639/92:  1  0  0  32-47}
                               {      : 5639/9x:  1  0  0  48-63}
 CloseWaitMessage;
end;

Procedure WriteEEPROM(Addr,Data : BYTE);
{Skriver en byte til adressen Addr. Sensor skal v‘re i MX-mode}
var s  : STR_5;
begin
 Str(Addr,s);
 COMStatus := WriteCOMPort(PortNo,'MA' + s + ',');
 Str(Data,s);
 COMStatus := WriteCOMPort(PortNo,'SM' + s + ',');
 Delay(EEPROMWriteDelay);
end;


Procedure CheckWriteProtected(var WP,St : BOOLEAN);
{unders›ger om SP6 er ON eller OFF ved at pr›ve at ‘ndre data i adresse 1.
 SP6=ON: writeprotect ON     SP6=OFF: writeprotect OFF
 St = TRUE hvis sensoren svarer}
var ReadByte,
    WriteByte  : BYTE;
    Kode       : INTEGER;
begin
 St := FALSE;
 COMStatus := WriteCOMPort(PortNo,'MS,');
 Delay(5);
 COMStatus := WriteCOMPort(PortNo,'MX,');
 Delay(5);
 COMStatus := WriteCOMPort(PortNo,'MA1,');
 Delay(5);
 COMStatus := ClearInputQue(PortNo);
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 Val(Svar,ReadByte,Kode);
 if COMStatus = 0 then
  begin
   WriteByte := ReadByte XOR $FF;
   WriteEEPROM(1,WriteByte);
   COMStatus := WriteCOMPort(PortNo,'MA1,');
   COMStatus := ClearInputQue(PortNo);
   COMStatus := WriteCOMPort(PortNo,'RM,');
   COMStatus := ReadlnCOMPort(PortNo,Svar,1);
   Delay(10);
   Val(Svar,ReadByte,Kode);
   WP := ReadByte <> WriteByte;  {<> = OK}
   St := COMStatus = 0;
  end;
end;






Procedure ReadKMatrix (Addr : BYTE; var ReadOk : BOOLEAN; var KMe : KMType);
{L‘ser matricen fra EýPROM, returnerer de 9 elementer i variablen KMe.
 Hvis der opst†r l‘sefejl, returnerer variablen ReadOk = FALSE }
var  n,k,
     Value   : BYTE;
     Kode    : INTEGER;
     s       : STR_5;
     Svar    : string;
     KMatrix  : array[1..18] of BYTE;
begin
 FillChar(KMe,SizeOf(KMe),0);
 ReadOk := TRUE;
 n := 1;
 COMStatus := WriteCOMPort(PortNo,'MS,');
 Str(Addr,s);
 COMStatus := WriteCOMPort(PortNo,'MA' + s + ',');
 COMStatus := ClearInputQue(PortNo);
 repeat
   COMStatus := WriteCOMPort(PortNo,'RM,');
   COMStatus := ReadlnCOMPort(PortNo,Svar,1);
   Val(Svar,Value,Kode); Delay(10);
   if Kode = 0 then
     KMatrix[n] := Value
    else
     ReadOk := FALSE;
   Inc(n);
 until (n > 18) OR (COMStatus <> 0) OR (Kode <> 0);

 ReadOk := ReadOk and (COMStatus = 0) and (n > 18);
{$R-}
 if ReadOk then
  for n := 1 TO 9 do
   KMe[n] := KMatrix[n*2-1]*256+KMatrix[n*2];
{$R+}
end;  {ReadKMatrix}


Procedure WriteKMatrix(Addr : BYTE; var St : BOOLEAN);
var n           : BYTE;
    KM          : KMType;       {for check of data written to sensor}
    CompareOk,
    ReadOk      : BOOLEAN;
begin
 repeat
   COMStatus := WriteCOMPort(PortNo,'MS,');
   Delay(5);
   COMStatus := WriteCOMPort(PortNo,'MX,');
   for n := 0 TO 8 do
    begin
     WriteEEPROM(Addr + (n * 2),    Hi(Trunc(KElement[n + 1])));
     WriteEEPROM(Addr + (n * 2) + 1,Lo(Trunc(KElement[n + 1])));
    end; {for}

   ReadKMatrix(Addr,ReadOk,KM);
   CompareOk := TRUE;
    for n := 1 TO 9 do
     if Trunc(KElement[n]) <> Trunc(KM[n])
      then CompareOk := FALSE;

   if not ReadOk then
     Finished := RetryYesOrNo(ReadOk and CompareOk,UserTxt[6],UserTxt[7],2)
    else
     Finished := RetryYesOrNo(ReadOk and CompareOk,UserTxt[1],'',2);
 until (ReadOk and CompareOk) OR Finished;

  St := ReadOk and CompareOk;
end;




Procedure ShowKMatrix;  {Key_r   debug mode only}
begin
  GotoXY(1,Row5+2);
  Writeln(KElement[1]:6:0,KElement[2]:6:0,KElement[3]:6:0);
  Writeln(KElement[4]:6:0,KElement[5]:6:0,KElement[6]:6:0);
  Writeln(KElement[7]:6:0,KElement[8]:6:0,KElement[9]:6:0);
end;


Procedure WriteBottomWindow;
begin
 Check := __DispWin(BottomWin);
 __QuikWin(2,1,HFore,HBack,'F1');
 __QuikWin(5,1,MainFore,MainBack,'Help');
 __QuikWin(17,1,HFore,HBack,'ENTER');
 __QuikWin(23,1,MainFore,MainBack,'Select');
 __QuikWin(39,1,HFore,HBack,'Tab '+#16+#17+#30+#31);
 __QuikWin(48,1,MainFore,MainBack,'Move bar');
 __QuikWin(61,1,HFore,HBack,'Alt+X');
 __QuikWin(67,1,MainFore,MainBack,'Exit program');
end;


Procedure WriteTittleWindow;       {Tast F9}
var Y : WORD;
begin
 Check := __DispWin(TitleWin);
 __GotoWin(8,2);
 Writeln('CRT Color Sensor Calibration');
 __GotoWin(14,4);
 Writeln(SWName,'   V',SWVer);
 __GotoWin(7,6);
 Y := __PauseKey('');
 Check := __RemWin;
end;

Procedure InitScreenColor;
begin
 if (_CurDevice = _MONO) then
   begin
     BarFore     := Black;
     BarBack     := LightGray;
     BordFore    := Black;
     BordBack    := LightGray;
     ErrorFore   := LightGray;
     ErrorBack   := Black;
     EditFore    := Black;
     EditBack    := LightGray;
     HFore	 := Black;
     HBack	 := LightGray;
     LFore	 := LightGray;
     LBack	 := Black;
     MenuFore	 := LightGray;
     MenuBack	 := Black;
     MainFore	 := LightGray;
     MainBack	 := Black;
     MessFore    := LightGray;
     MessBack    := Black;
     PFore	 := LightGray;
     PBack	 := Black;
     RetryFore   := LightGray;
     RetryBack   := Black;
     ShaFore     := LightGray;
     ShaBack     := Black;
     TittleFore	 := LightGray;
     TittleBack	 := Black;
     TopFore     := LightGray;
     TopBack     := Black;
   end
 else
   begin
     BarFore     := Black;
     BarBack     := Green;
     BordFore    := Black;
     BordBack    := LightGray;
     ErrorFore   := White;
     ErrorBack   := Red;
     EditFore    := White;
     EditBack    := Blue;
     HFore	 := Red;
     HBack	 := LightGray;
     LFore	 := Yellow;             {Lotus style - not used}
     LBack	 := Blue;
     MenuFore	 := Black;
     MenuBack	 := LightGray;
     MainFore	 := Black;
     MainBack	 := LightGray;
     MessFore    := White;              {message window}
     MessBack    := Red;
     PFore	 := DarkGray;          {Protected Item}
     PBack	 := LightGray;
     RetryFore   := Black;
     RetryBack   := LightGray;
     ShaFore     := DarkGray;
     ShaBack     := Black;
     TittleFore	 := Red;
     TittleBack	 := LightGray;
     TopFore     := Red;
     TopBack     := LightGray;
   end;
end;

Procedure CreateAllWindows;
var n : BYTE;
begin
{  --------- Tittle window ------------------------------------------}
    TitleWin := __MakeWin(20,10,60,16,6,7,MessFore,MessBack,_DLBORD_WIN,
						      MessFore,MessBack);
  {  Check := __ShadeWin(TitleWin,2,1,ShaFore,ShaBack,#0,$0001);}

{  --------- Bottom window ------------------------------------------}
    BottomWin   := __MakeWin(1,_CurRows,_CurColumns,_CurRows,6,7,MenuFore,MenuBack,_NOBORD_WIN,
						      MenuFore,MenuBack);

{  --------- Main window ------------------------------------------}
   MainWin  := __MakeWin(1,1,_CurColumns,_CurRows - 1,6,7,MainFore,MainBack,
					  _NOBORD_WIN,MainFore,MainBack);
   Check := __DispWin(MainWin);
   __GotoWin(1,2);

 if (_CurDevice = _COLOR) then
  begin
    __AttrWin(#176,1840,LightGray,Blue);
    for n := 1 TO 12 do
     begin
      __GotoWin(Col1+6,Row3+n);
      __AttrWin(' ',69,Black,LightGray);
     end;
  end;

 if (_CurDevice = _MONO) then
  begin
    __AttrWin(#178,1840,Black,LightGray);
    for n := 1 TO 12 do
     begin
      __GotoWin(Col1+6,Row3+n);
      __AttrWin(' ',69,LightGray,Black);
     end;
  end;

 case CIEMode of
  1: begin
      __QuikWin(Col6,Row6,MainFore,MainBack,'CIE 1931 Unit');
      __QuikWin(Col6,Row7,MainFore,MainBack,'x =');
      __QuikWin(Col6,Row9,MainFore,MainBack,'y =');
     end;
  2: begin
      __QuikWin(Col6,Row6,MainFore,MainBack,'CIE 1976 Unit');
      __QuikWin(Col6,Row7,MainFore,MainBack,'u''=');
      __QuikWin(Col6,Row9,MainFore,MainBack,'v''=');
     end;
  3: begin
      __QuikWin(Col6,Row6,MainFore,MainBack,'CIE 1960 Unit');
      __QuikWin(Col6,Row7,MainFore,MainBack,'u =');
      __QuikWin(Col6,Row9,MainFore,MainBack,'v =');
     end;
 end; {case}
 __QuikWin(Col6,Row11,MainFore,MainBack,'Y =');

 case LumMode of
  1: __QuikWin(Col8+10,Row11,MainFore,MainBack,'cd/mý');
  2: __QuikWin(Col8+10,Row11,MainFore,MainBack,'NIT  ');
  3: __QuikWin(Col8+10,Row11,MainFore,MainBack,'ftL  ');
 end;

 __QuikWin(Col6,Row13,MainFore,MainBack,'User Text:');

 __QuikWin(Col2,Row7,MainFore,MainBack,'Sensor:');
 __QuikWin(Col2,Row9,MainFore,MainBack,'Cal Status:');
 __QuikWin(Col2,Row11,MainFore,MainBack,'Write Protect:');
 __QuikWin(Col2,Row13,MainFore,MainBack,'User Text:');

{  --------- Cal Setup window -------------------------------------------}
    CalSetupWin  := __MakeWin(Col2+2,Row1+2,Col2+30,Row1+8,6,7,MenuFore,MenuBack,
					  _SLBORD_WIN,MenuFore,MenuBack);
    Check    := __TitleWin(CalSetupWin,' Calibration Setup ',
					   TittleFore,TittleBack,_TopCenter);
    Check := __ShadeWin(CalSetupWin,2,1,ShaFore,ShaBack,#0,$0001);

{  --------- Edit x,y,Y + usertext windows ---------------------------}
    EditxWin  := __MakeWin(Col8,Row7,Col8+5,Row7,6,7,EditFore,EditBack,
					  _NOBORD_WIN,Black,LightGray);
    EdityWin  := __MakeWin(Col8,Row9,Col8+5,Row9,6,7,EditFore,EditBack,
					  _NOBORD_WIN,Black,LightGray);
    EditLumWin  := __MakeWin(Col8,Row11,Col8+4,Row11,6,7,EditFore,EditBack,
					  _NOBORD_WIN,Black,LightGray);
    EditUserTxtWin  := __MakeWin(Col8,Row13,Col8+9,Row13,6,7,EditFore,EditBack,
					  _NOBORD_WIN,Black,LightGray);
{  --------- Yes or No window --------------------------------------}
    YesOrNoWin := __MakeWin(17,18,63,22,6,7,RetryFore,RetryBack,_SLBORD_WIN,
						       RetryFore,RetryBack);
    Check := __ShadeWin(YesOrNoWin,2,1,ShaFore,ShaBack,#0,$0001);
{  --------- Message window ---------------------------------------}
    MessWin := __MakeWin(15,18,65,21,6,7,MessFore,MessBack,_SLBORD_WIN,
						       MessFore,MessBack);
    Check := __ShadeWin(MessWin,2,1,ShaFore,ShaBack,#0,$0001);
{  --------- Measure window -----------------------------------------}
    MeasWin := __MakeWin(17,18,63,21,6,7,MessFore,MessBack,_SLBORD_WIN,
						       MessFore,MessBack);
    Check := __ShadeWin(MeasWin,2,1,ShaFore,ShaBack,#0,$0001);
{  --------- Write protect ON/OFF window ----------------------------}
    WPOnOffWin := __MakeWin(Col3-2,3,Col3+16,4,6,7,MenuFore,MenuBack,_SLBORD_WIN,
						       MenuFore,MenuBack);
    Check    := __TitleWin(WPOnOffWin,' Write Protect ',
					   TittleFore,TittleBack,_TopCenter);
    Check := __ShadeWin(WPOnOffWin,2,1,ShaFore,ShaBack,#0,$0001);
{  --------- Write protect window -----------------------------------}
    WPWin := __MakeWin(11,3,69,22,6,7,MenuFore,MenuBack,_SLBORD_WIN,
						       MenuFore,MenuBack);
{   Check    := __TitleWin(WPWin,' Write Protection ',
					   TittleFore,TittleBack,_TopCenter);}

{  --------- Retry window ---------------------------------------}
    RetryWin := __MakeWin(17,18,63,22,6,7,RetryFore,RetryBack,_SLBORD_WIN,
						       RetryFore,RetryBack);
    Check := __ShadeWin(RetryWin,2,1,ShaFore,ShaBack,#0,$0001);
{  --------- Wait window ---------------------------------------}
    WaitWin := __MakeWin(25,18,55,18,6,7,MessFore,MessBack,_SLBORD_WIN,
						       MessFore,MessBack);
    Check := __ShadeWin(WaitWin,2,1,ShaFore,ShaBack,#0,$0001);
end;





Procedure CreateAllMenus;
begin
 { Add keys to default keys --------------------------------------------}
 __DefKyMnu(_Key_a_x,_KABORT_MNU,_NoMove);        {exit program}
 __DefKyMnu(_Key_F1,_KTRANSMIT_MNU,_NextCh);      {help program}

 { Main menu, its items and keys.  ------------------------------------- }
 MainMnu := __MakeMnu(MainWin,BarFore,BarBack,LFore,LBack,PFore,PBack);

 IPtr    := __ItemMnu(MainMnu,Col1,Row1,0,HFore,HBack,FALSE,' Read Status ');
 IPtr    := __ItemMnu(MainMnu,Col3,Row1,0,HFore,HBack,FALSE,' Write Protect ');
 IPtr    := __ItemMnu(MainMnu,Col5,Row1,0,HFore,HBack,FALSE,' Setup ');
 IPtr    := __ItemMnu(MainMnu,Col7,Row1,0,HFore,HBack,FALSE,' Data ');
 IPtr    := __ItemMnu(MainMnu,Col9,Row1,0,HFore,HBack,FALSE,' Restore ');
 IPtr    := __ItemMnu(MainMnu,Col10,Row1,2,HFore,HBack,FALSE,' Start Cal ');
 IPtr    := __ItemMnu(MainMnu,Col11,Row1,0,HFore,HBack,FALSE,' Help ');

{ IPtr    := __ItemMnu(MainMnu,Col7,Row7,0, HFore,HBack,FALSE,'      ');
 IPtr    := __ItemMnu(MainMnu,Col7,Row9,0, HFore,HBack,FALSE,'      ');
 IPtr    := __ItemMnu(MainMnu,Col7,Row11,0,HFore,HBack,FALSE,'     ');
 IPtr    := __ItemMnu(MainMnu,Col7,Row13,0,HFore,HBack,FALSE,'          ');}

 KPtr := __KeyMnu(MainMnu,nil,_Key_s,  _KTRANSMIT_MNU,_NextCh);  {Start cal}
 KPtr := __KeyMnu(MainMnu,nil,_Key_s_s,_KTRANSMIT_MNU,_NextCh);  { ------- }
{$ifDEF DEBUGMODE}
 KPtr := __KeyMnu(MainMnu,nil,_Key_F9,  _KTRANSMIT_MNU,_NextCh);
 KPtr := __KeyMnu(MainMnu,nil,_Key_c,   _KTRANSMIT_MNU,_NextCh);
 KPtr := __KeyMnu(MainMnu,nil,_Key_s_c, _KTRANSMIT_MNU,_NextCh);
 KPtr := __KeyMnu(MainMnu,nil,_Key_k,   _KTRANSMIT_MNU,_NextCh);
 KPtr := __KeyMnu(MainMnu,nil,_Key_r,   _KTRANSMIT_MNU,_NextCh);
 KPtr := __KeyMnu(MainMnu,nil,_Key_w,   _KTRANSMIT_MNU,_NextCh);
{$endif}

{ Cal Setup menu, its items ------------------------------------------- }
 CalSetupMnu := __MakeMnu(CalSetupWin,BarFore,BarBack,LFore,LBack,PFore,PBack);

 IPtr := __ItemMnu(CalSetupMnu,3,1,0,HFore,HBack,FALSE,'Chromaticity xy   CIE 1931 ');
 IPtr := __ItemMnu(CalSetupMnu,3,2,0,HFore,HBack,FALSE,'Chromaticity u''v'' CIE 1976 ');
 IPtr := __ItemMnu(CalSetupMnu,3,3,0,HFore,HBack,FALSE,'Chromaticity uv   CIE 1960 ');
 IPtr := __ItemMnu(CalSetupMnu,3,5,0,HFore,HBack,FALSE,'Luminance    cd/mý         ');
 IPtr := __ItemMnu(CalSetupMnu,3,6,0,HFore,HBack,FALSE,'Luminance    NIT           ');
 IPtr := __ItemMnu(CalSetupMnu,3,7,0,HFore,HBack,FALSE,'Luminance    ftL           ');


{ Write protect ON/OFF menu, its items ----------------------------  }

 WPOnOffMnu := __MakeMnu(WPOnOffWin,BarFore,BarBack,LFore,LBack,PFore,PBack);

 IPtr    := __ItemMnu(WPOnOffMnu,1,1,0,HFore,HBack,FALSE,' Write Protect ON  ');
 IPtr    := __ItemMnu(WPOnOffMnu,1,2,0,HFore,HBack,FALSE,' Write Protect OFF ');

{ Retry menu, its items ---------------------------------------------  }

 RetryMnu := __MakeMnu(RetryWin,BarFore,BarBack,LFore,LBack,PFore,PBack);

 IPtr    := __ItemMnu(RetryMnu,17,4,0,HFore,HBack,FALSE,'    Retry     ');
 IPtr    := __ItemMnu(RetryMnu,17,5,0,HFore,HBack,FALSE,' Exit Program ');

{ Write Protect Yes or No menu, its items ---------------------------------------------  }

 YesOrNoMnu := __MakeMnu(YesOrNoWin,BarFore,BarBack,LFore,LBack,PFore,PBack);

 IPtr    := __ItemMnu(YesOrNoMnu,19,5,0,HFore,HBack,FALSE,'     No     ');
 IPtr    := __ItemMnu(YesOrNoMnu,19,4,0,HFore,HBack,FALSE,'    Yes     ');

end;


Procedure UserMessage(Mess1,Mess2 : string; BeepMode : BYTE);
var Y : WORD;
begin
  case BeepMode of
  1 : begin
       Sound(1500); Delay(150); NoSound;
      end;
  2 : begin
       Sound(400); Delay(500); NoSound;
      end;
  end;

  Check := __DispWin(MessWin);
  Writeln(Mess1);
  Write(Mess2);
  __GotoWin(2,4);
  Y := __PauseKey('');
  Check := __RemWin;
end;

Function YesOrNo (Mess1,Mess2 : STR_60) : BOOLEAN;
 begin
  CloseWaitMessage;
  Check := __DispWin(YesOrNoWin);
  Sound(1500); Delay(150); NoSound;
  Writeln(Mess1);
  Write(Mess2);
  WPPtr := nil;
  __FlushKey;
  if __DispMnu(YesOrNoMnu) then
   WPPtr := __ReadMnu(YesOrNoMnu,WPPtr,_MBEEP_UNKNOWN_MNU + _MKEEP_HIGHLIGHT_MNU,Key);
  Check := __RemWin;
  YesOrNo := WPPtr^._Row = 4;
 end;


Procedure  ReadUserText(var UserTxt : STR_10; var SensCon : BOOLEAN);
{L‘ser user text fra adresse 176 til der findes et ASCII NULL (#0)
 eller der er l‘st 11 tegn ialt.
}

var n,v : BYTE;
    k  : INTEGER;
begin
 UserTxt := '';
 COMStatus := WriteCOMPort(PortNo,'MS,');
 COMStatus := WriteCOMPort(PortNo,'MA176,');
 COMStatus := ClearInputQue(PortNo);
 n := 1;
 repeat
  COMStatus := WriteCOMPort(PortNo,'RM,');
  COMStatus := ReadlnCOMPort(PortNo,Svar,1);
  Delay(10);
  Val(Svar,v,k);
  if v <> 0 then
   UserTxt := UserTxt + Chr(v);
  Inc(n);
 until (v = 0) OR (COMStatus <> 0) OR (n > 11) OR (k <> 0);
 SensCon := COMStatus = 0;
end;

Procedure WriteUserText(UsTxt : STR_10);
{Skriver teksten i UsTxt til adresse 176 og fremefter. Afslutter med NULL
 Som kontrol l‘ses teksten igen fra EEPROM'men}
var ReadTxt : STR_10;
    SensCon : BOOLEAN;
    n       : BYTE;
begin
 if UsTxt = '' then         {do not change user text in sensor}
  Exit;

 repeat
  COMStatus := WriteCOMPort(PortNo,'MS,');
  COMStatus := WriteCOMPort(PortNo,'MX,');
  for n := 1 TO Length(UsTxt) do
   WriteEEPROM(n+175,Ord(UsTxt[n]));

  WriteEEPROM(n+175+1,0);     {write ASCII NULL}

  ReadUserText(ReadTxt,SensCon);
  Finished := RetryYesOrNo(SensCon,UserTxt[6],UserTxt[7],2);
 until ((ReadTxt = UsTxt) and SensCon)  OR  Finished;
 if Finished then
  Exit;
 SensorUserText := ReadTxt;

end;


Procedure GetStatus(var SensCon,
                        SensPhil,
                        SensValid,
                        SensFactoryCal,
                        SensWP      : BOOLEAN;
                    var KUNoStr,
                        SensSWStr,
                        UserTxt     : STR_10;
                        OutForm     : byte );

{ L‘ser f›lgende status i color sensoren:
   1.  Om sensoren er i kontakt med COM1.
   2.  Om sensoren er en Philips type.
   3.  Om sensoren er en CRT sensor. Kun CRT sensorer er gyldige.
   4.  Om sensoren er Factory kalibreret eller User kalibreret.
   5.  Om sensoren er write protected.
   6.  KU nummer.
   7.  Sensor user text.
   8.  Software version (bestemmer behandlingen af skrivebeskyttelsen).
   9.  Output format   R,G,B,I   eller   R,G,B.
}
begin
{---- Sensor Valid (kun /90 og /IND CRT Color Sensor) ---------------------}
 ReadIntID(IntID,SensPhil,SensValid,SensCon,TRUE);

 if not SensPhil then
  begin
   KUNoStr := '';
   UserTxt := '';
   __QuikWin(Col4,Row7,MainFore,MainBack,'        ');     {KU No}
   __QuikWin(Col4,Row9,MainFore,MainBack,'            '); {Cal Status}
   __QuikWin(Col4,Row11,MainFore,MainBack,'   ');         {WP Statsu}
   __QuikWin(Col4,Row13,MainFore,MainBack,'          ');  {User Text}
   Exit;
  end;

 if not SensValid then
  begin
   UserTxt := '';
   __QuikWin(Col4,Row7,MainFore,MainBack,KUNoStr);         {KU No}
   __QuikWin(Col4,Row9,MainFore,MainBack,'            '); {Cal Status}
   __QuikWin(Col4,Row11,MainFore,MainBack,'   ');         {WP Statsu}
   __QuikWin(Col4,Row13,MainFore,MainBack,'          ');  {User Text}
   Exit;
  end;


 if SensCon then
  begin
   COMStatus := WriteCOMPort(PortNo,'MS,');
   COMStatus := WriteCOMPort(PortNo,'MA157,');
   COMStatus := ClearInputQue(PortNo);
   COMStatus := WriteCOMPort(PortNo,'RM,');
   COMStatus := ReadlnCOMPort(PortNo,Svar,1);
   Delay(10);
   if COMStatus <> 0 then
    begin
     COMStatus := WriteCOMPort(PortNo,'MA157,');
     COMStatus := ClearInputQue(PortNo);
     COMStatus := WriteCOMPort(PortNo,'RM,');
     COMStatus := ReadlnCOMPort(PortNo,Svar,1);
     Delay(10);
   end;
   SensFactoryCal := not (Svar = '000');
   SensCon := COMStatus = 0;
   if FactoryCal and SensCon then
    __QuikWin(Col4,Row9,MainFore,MainBack,'FACTORY')
   else
   if (not FactoryCal) and SensCon then
    __QuikWin(Col4,Row9,MainFore,MainBack,'USER   ')
   else
    __QuikWin(Col4,Row9,MainFore,MainBack,'       ');
  end;

{---- Read output format (addr 95) -------------------------}
 if SensCon then
  begin
   COMStatus := WriteCOMPort(PortNo,'MS,');
   COMStatus := WriteCOMPort(PortNo,'MA95,');
   COMStatus := ClearInputQue(PortNo);
   COMStatus := WriteCOMPort(PortNo,'RM,');
   COMStatus := ReadlnCOMPort(PortNo,Svar,1);
   Delay(10);
   if COMStatus <> 0 then
    begin
     COMStatus := WriteCOMPort(PortNo,'MA95,');
     COMStatus := ClearInputQue(PortNo);
     COMStatus := WriteCOMPort(PortNo,'RM,');
     COMStatus := ReadlnCOMPort(PortNo,Svar,1);
     Delay(10);
   end;
  end;

   if (Svar = '000') then
     OutputFormat := RGB_OutputFormat
    else
     OutputFormat := RGBI_OutputFormat;
   SensCon := COMStatus = 0;


{---- KU nummer      Sensor SW  -----------------------}
 if SensCon then
 begin
  ReadKU(KUNoStr,SensSWStr,SensCon);
    if SensCon then
      __QuikWin(Col4,Row7,MainFore,MainBack,KUStr)
     else
      __QuikWin(Col4,Row7,MainFore,MainBack,'        ');
 end
 else
  __QuikWin(Col4,Row7,MainFore,MainBack,'        ');

{---- Sensor WP ----------------------------------------------------}
 if SensCon then
  begin
   CheckWriteProtected(SensWP,SensCon);
    if SensCon then
     begin
      if SensWP then
       __QuikWin(Col4,Row11,MainFore,MainBack,'ON ')
      else
       __QuikWin(Col4,Row11,MainFore,MainBack,'OFF');
     end
     else
       __QuikWin(Col4,Row11,MainFore,MainBack,'   ');
  end
  else
       __QuikWin(Col4,Row11,MainFore,MainBack,'   ');

{---- Sensor UserText --------------------------------------------------}
 if SensCon then
  ReadUserText(UserTxt,SensCon);

 if SensCon then
  begin
   __QuikWin(Col4,Row13,MainFore,MainBack,'          ');
   __QuikWin(Col4,Row13,MainFore,MainBack,UserTxt);
  end
 else
  begin
   __QuikWin(Col4,Row13,MainFore,MainBack,'          ');
   __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
  end;

end;  {getstatus}




Procedure SetUserCal( Var St : BOOLEAN);
begin
 COMStatus := WriteCOMPort(PortNo,'MS,');
 COMStatus := WriteCOMPort(PortNo,'MX,');
 COMStatus := WriteCOMPort(PortNo,'MA157,');
 COMStatus := WriteCOMPort(PortNo,'SM0,');
 Delay(EEPROMWriteDelay);
 COMStatus := ClearInputQue(PortNo);
 COMStatus := WriteCOMPort(PortNo,'MA157,');
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 St := (COMStatus = 0) and (Svar = '000');
end;

Procedure SetFactoryCal( var St : BOOLEAN);
begin
 Svar := '';
 COMStatus := WriteCOMPort(PortNo,'MS,');
 COMStatus := WriteCOMPort(PortNo,'MX,');
 COMStatus := WriteCOMPort(PortNo,'MA157,');
 COMStatus := WriteCOMPort(PortNo,'SM1,');
 Delay(EEPROMWriteDelay);
 COMStatus := ClearInputQue(PortNo);
 COMStatus := WriteCOMPort(PortNo,'MA157,');
 COMStatus := WriteCOMPort(PortNo,'RM,');
 COMStatus := ReadlnCOMPort(PortNo,Svar,1);
 Delay(10);
 St := (COMStatus = 0) and (Svar = '001')
end;


Procedure Enter_x (var TransmitKey : _KeySeq; var No : BYTE);
{ Tildeler variablen Xp65 en v‘rdi. Kontrollerer om v‘rdien er mellem
  MinCoor og MaxCoor}
var x_Str,RetStr : string;
    ErrorCode    : WORD;
    p,Kode       : INTEGER;
    ValidData    : BOOLEAN;
begin
 if Xp65 = 0 then
  x_Str := ''
 else
  Str(Xp65:6:4,x_Str);

 Check := __DispWin(EditxWin);
 FillChar(EditCtrl,SizeOf(EditCtrl),0);
 WITH EditCtrl do
  begin
   _VScnFore := EditFore; _VScnBack := EditBack;
   _InsCursor := Unit_Scn._CBlock;
   _Mask := '######';                  {0123456789.-}
  end; {with}
 __GotoWin(1,1);
 repeat
  __FieldEdt(x_Str,RetStr,EditCtrl,ErrorCode,TransmitKey);
   repeat
    p := Pos(' ',RetStr);
    if p > 0 then
     Delete(RetStr,p,1);
   until p = 0;
  Val(RetStr,Xp65,Kode);
  ValidData := ((Kode = 0) and (Xp65 <= MaxCoor) and (Xp65 >= MinCoor) and
               ((Xp65 + Yp65) < 1)) OR
               (RetStr = '');

  if not ValidData then
   UserMessage(' Error #10: ',' Valid data:  ' + RealToStr(MinCoor,5,3) + ' - ' +
               RealToStr(MaxCoor,5,3) + #10 + #13 +
               ' Sum of coordinates: < 1',2);
 until ValidData and (ErrorCode <= 1);

 No := 1;

 Str(Xp65:6:4,x_Str);
 Check := __RemWin;                      {close edit window}
 __QuikWin(Col8,Row7,MainFore,MainBack,'      ');
 if RetStr <> '' then
 __QuikWin(Col8,Row7,MainFore,MainBack,x_Str);
end;


Procedure Enter_y (var TransmitKey : _KeySeq; var No : BYTE);
{ Tildeler variablen Yp65 en v‘rdi. Kontrollerer om v‘rdien er mellem
  MinCoor og MaxCoor}
var y_Str,RetStr : string;
    ErrorCode    : WORD;
    p,Kode       : INTEGER;
    ValidData    : BOOLEAN;
begin
 if Yp65 = 0 then
  y_Str := ''
 else
  Str(Yp65:6:4,y_Str);
 Check := __DispWin(EdityWin);
 FillChar(EditCtrl,SizeOf(EditCtrl),0);
 WITH EditCtrl do
  begin
   _VScnFore := EditFore; _VScnBack := EditBack;
   _InsCursor := Unit_Scn._CBlock;
   _Mask := '######';
  end; {with}
 __GotoWin(1,1);
 repeat
  __FieldEdt(y_Str,RetStr,EditCtrl,ErrorCode,TransmitKey);
   repeat
    p := Pos(' ',RetStr);
    if p > 0 then
     Delete(RetStr,p,1);
   until p = 0;
  Val(RetStr,Yp65,Kode);
  ValidData := ((Kode = 0) and (Yp65 <= MaxCoor) and (Yp65 >= MinCoor) and
               ((Xp65 + Yp65) < 1)) OR
               (RetStr = '');

 if not ValidData then
  UserMessage(' Error #10:',' Valid data:  ' + RealToStr(MinCoor,5,3) + ' - ' +
              RealToStr(MaxCoor,5,3) + #10 + #13 +
              ' Sum of coordinates: < 1',2);
 until ValidData and (ErrorCode <= 1);

 No := 2;

 Str(Yp65:6:4,y_Str);
 Check := __RemWin;                      {close edit window}
 __QuikWin(Col8,Row9,MainFore,MainBack,'      ');
 if RetStr <> '' then
  __QuikWin(Col8,Row9,MainFore,MainBack,y_Str);
end;


Procedure Enter_LumValue (var TransmitKey : _KeySeq; var No : BYTE);
{ Tildeler variablen LumValue en v‘rdi. Kontrollerer om v‘rdien er mellem
  MinLum og MaxLum}
var Lum_Str,RetStr : string;
    ErrorCode      : WORD;
    p,Kode         : INTEGER;
    MaxL, MinL     : REAL;
    ValidData      : BOOLEAN;
begin
 if LumValue = 0 then
  Lum_Str := ''
 else
 Str(LumValue:5:1,Lum_Str);
 repeat
  p := Pos(' ',Lum_Str);
  if p > 0 then
   Delete(Lum_Str,p,1);
 until p = 0;
 Check := __DispWin(EditLumWin);
 FillChar(EditCtrl,SizeOf(EditCtrl),0);
 MaxL := MaxLum;
 MinL := MinLum;
 WITH EditCtrl do
  begin
   _VScnFore := EditFore; _VScnBack := EditBack;
   _InsCursor := Unit_Scn._CBlock;
   _Mask := '#####';
  end; {with}

 __GotoWin(1,1);
 repeat
  __FieldEdt(Lum_Str,RetStr,EditCtrl,ErrorCode,TransmitKey);
   repeat
    p := Pos(' ',RetStr);      {delete spaces}
    if p > 0 then
     Delete(RetStr,p,1);
   until p = 0;
  Val(RetStr,LumValue,Kode);
  Validdata := ((Kode = 0) and (LumValue <= (MaxL)) and (Lumvalue >= (MinL))) OR
               (RetStr = '');

 if not ValidData then
  if LumMode = 3 then
   UserMessage(' Error #11:',' Valid data:  ' + RealToStr(MinL+0.1,4,1) + '  - ' +
               RealToStr(MaxL-0.1,6,1),2)
  else
   UserMessage(' Error #11:',' Valid data:  ' + RealToStr(MinL,4,1) + '  - ' +
               RealToStr(MaxL,6,1),2);
 until ValidData and (ErrorCode <= 1);

 No := 3;

 Str(LumValue:5:1,Lum_Str);
 Check := __RemWin;                      {close edit window}
 __QuikWin(Col8,Row11,MainFore,MainBack,'      ');
 if RetStr <> '' then
  __QuikWin(Col8,Row11,MainFore,MainBack,Lum_Str);
end;

Procedure Enter_UserText(var TransmitKey : _KeySeq; var No : BYTE);
{ Skriver en streng til variablen NewUserText}
var RetStr       : string;
    ErrorCode    : WORD;
    p,Kode       : INTEGER;
begin
 Check := __DispWin(EditUserTxtWin);
 FillChar(EditCtrl,SizeOf(EditCtrl),0);
 WITH EditCtrl do
  begin
   _VScnFore := EditFore; _VScnBack := EditBack;
   _InsCursor := Unit_Scn._CBlock;
   _Mask := '&&&&&&&&&&';
{   _KeyProcPtr := @KeyProc;}
  end; {with}
 __GotoWin(1,1);

 repeat
  __FieldEdt(SensorUserText,RetStr,EditCtrl,ErrorCode,TransmitKey);
 until ErrorCode <= 1;

 No := 4;

 if ErrorCode = 0 then
   SensorUserText := RetStr;

 Check := __RemWin;                      {close edit window}
 __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
 __QuikWin(Col8,Row13,MainFore,MainBack,SensorUserText);
end;


Procedure EnterCalData;
var NewKeyPtr   : _KeyEdtPtr;
    N           : BYTE;
    RKey        : _KeySeq;
    Oldxx,
    Oldyy,
    OldY        : REAL;
    x_Str,
    y_Str,
    Lum_Str,
    OldTxt      : string;

begin
 EditKey._Action_Edt := _Transmit_Edt;
 EditKey._Attrib_Edt := $0;
 EditKey._KeySeq_Edt := _KeyTable[_Key_Up];
 NewKeyPtr := __ChgKyEdt(EditKey);
 EditKey._KeySeq_Edt := _KeyTable[_Key_Down];
 NewKeyPtr := __ChgKyEdt(EditKey);
 EditKey._KeySeq_Edt := _KeyTable[_Key_Pad_Up];
 NewKeyPtr := __ChgKyEdt(EditKey);
 EditKey._KeySeq_Edt := _KeyTable[_Key_Pad_Down];
 NewKeyPtr := __ChgKyEdt(EditKey);
 Oldxx := Xp65;
 Oldyy := Yp65;
 OldY  := LumValue;
 OldTxt := SensorUserText;

 Enter_x(RKey,N);
 WHILE (Word(RKey) <> Word(_KeyTable[_Key_Enter])) and
       (Word(RKey) <> Word(_KeyTable[_Key_Pad_Enter])) and
       (Word(RKey) <> Word(_KeyTable[_Key_Esc])) do
  case N of
   1 : if (RKey._ChScan = _KeyTable[_Key_Up]._ChScan) OR
          (RKey._ChScan = _KeyTable[_Key_Pad_Up]._ChScan) then
         Enter_UserText(RKey,N)
        else
         Enter_y(RKey,N);

   2 : if (RKey._ChScan = _KeyTable[_Key_Up]._ChScan) OR
          (RKey._ChScan = _KeyTable[_Key_Pad_Up]._ChScan) then
         Enter_x(RKey,N)
        else
         Enter_LumValue(RKey,N);

   3 : if (RKey._ChScan = _KeyTable[_Key_Up]._ChScan) OR
          (RKey._ChScan = _KeyTable[_Key_Pad_Up]._ChScan) then
         Enter_y(RKey,N)
        else
         Enter_UserText(RKey,N);

   4 : if (RKey._ChScan = _KeyTable[_Key_Up]._ChScan) OR
          (RKey._ChScan = _KeyTable[_Key_Pad_Up]._ChScan) then
         Enter_LumValue(RKey,N)
        else
         Enter_x(RKey,N);
  end; {case}

 { ------- Restore old values if ESC is presed ----------------------------}
 if (RKey._ChScan = (_KeyTable[_Key_Esc]._ChScan)) then
  begin
   Xp65 := Oldxx;
   Yp65 := Oldyy;
   LumValue := OldY;
   SensorUserText := OldTxt;

   if Xp65 = 0 then
     x_Str := '      '
    else
     Str(Xp65:6:4,x_Str);

   if Yp65 = 0 then
     y_Str := '      '
    else
     Str(Yp65:6:4,y_Str);

   if LumValue = 0 then
     Lum_Str := '      '
    else
     Str(LumValue:5:1,Lum_Str);

   __QuikWin(Col8,Row7,MainFore,MainBack,x_Str);
   __QuikWin(Col8,Row9,MainFore,MainBack,y_Str);
   __QuikWin(Col8,Row11,MainFore,MainBack,Lum_Str);
   __QuikWin(Col8,Row13,MainFore,MainBack,SensorUserText);
  end;

end;



Procedure EnterCalSetup;
var MKey        : _Keys;
    OldCIEMode,
    OldLumMode  : BYTE;
begin
 Check := __DispWin(CalSetupWin);
 __QuikWin(1,CIEMode,MainFore,MainBack,_RLapMarker);
 __QuikWin(1,LumMode+4,MainFore,MainBack,_RLapMarker);

 OldCIEMode := CIEMode;
 OldLumMode := LumMode;
 Check := __DispMnu(CalSetupMnu);

 repeat
  __FlushKey;
  CalSetupPtr := __ReadMnu(CalSetupMnu,CalSetupPtr,_MBEEP_UNKNOWN_MNU + _MKEEP_HIGHLIGHT_MNU,MKey);

 if MKey = _Key_a_x then
   Finished := TRUE
  else
 if MKey = _Key_F1 then
  Help('lumcieunit')
  else
 if MKey <> _Key_Esc then
  case CalSetupPtr^._Row of
   1 : begin
        CIEMode := 1;
       __QuikWin(1,1,MainFore,MainBack,_RLapMarker);
       __QuikWin(1,2,MainFore,MainBack,' ');
       __QuikWin(1,3,MainFore,MainBack,' ');
       end;
   2 : begin
        CIEMode := 2;
       __QuikWin(1,2,MainFore,MainBack,_RLapMarker);
       __QuikWin(1,1,MainFore,MainBack,' ');
       __QuikWin(1,3,MainFore,MainBack,' ');
       end;
   3 : begin
        CIEMode := 3;
       __QuikWin(1,3,MainFore,MainBack,_RLapMarker);
       __QuikWin(1,1,MainFore,MainBack,' ');
       __QuikWin(1,2,MainFore,MainBack,' ');
       end;
   5 : begin
        MaxLum := DefaultMaxLum;
        MinLum := DefaultMinLum;
        LumMode := 1;
       __QuikWin(1,5,MainFore,MainBack,_RLapMarker);
       __QuikWin(1,6,MainFore,MainBack,' ');
       __QuikWin(1,7,MainFore,MainBack,' ');
       end;
   6 : begin
        MaxLum := DefaultMaxLum;
        MinLum := DefaultMinLum;
        LumMode := 2;
       __QuikWin(1,6,MainFore,MainBack,_RLapMarker);
       __QuikWin(1,5,MainFore,MainBack,' ');
       __QuikWin(1,7,MainFore,MainBack,' ');
       end;
   7 : begin
        MaxLum := DefaultMaxLum * Cd_ftL_Factor;
        MinLum := DefaultMinLum * Cd_ftL_Factor;
        LumMode := 3;
       __QuikWin(1,7,MainFore,MainBack,_RLapMarker);
       __QuikWin(1,5,MainFore,MainBack,' ');
       __QuikWin(1,6,MainFore,MainBack,' ');
       end;
  end; {case}

 until (MKey = _Key_Esc) OR (MKey = _Key_a_x);

 Check := __RemWin;

 case LumMode of
  1 : __QuikWin(Col8+10,Row11,MainFore,MainBack,'cd/mý');
  2 : __QuikWin(Col8+10,Row11,MainFore,MainBack,'NIT  ');
  3 : __QuikWin(Col8+10,Row11,MainFore,MainBack,'ftL  ');
 end;

 if OldLumMode <> LumMode then
  begin
   __QuikWin(Col8,Row11,MainFore,MainBack,'       ');      {Lum}
   LumValue := 0;
  end;

 if OldCIEMode <> CIEMode then
  begin
   __QuikWin(Col8,Row7,MainFore,MainBack,'       ');
   __QuikWin(Col8,Row9,MainFore,MainBack,'       ');
   Xp65 := 0;
   Yp65 := 0;
  end;

 case CIEMode of
   1 : begin
        __QuikWin(Col6+6,Row6,MainFore,MainBack,'31');
        __QuikWin(Col6,Row7,MainFore,MainBack,'x =');
        __QuikWin(Col6,Row9,MainFore,MainBack,'y =');
       end;
   2 : begin
        __QuikWin(Col6+6,Row6,MainFore,MainBack,'76');
        __QuikWin(Col6,Row7,MainFore,MainBack,'u''=');
        __QuikWin(Col6,Row9,MainFore,MainBack,'v''=');
       end;
   3 : begin
        __QuikWin(Col6+6,Row6,MainFore,MainBack,'60');
        __QuikWin(Col6,Row7,MainFore,MainBack,'u =');
        __QuikWin(Col6,Row9,MainFore,MainBack,'v =');
       end;
  end; {case}
end;



Procedure RestoreFactoryCal;
begin
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
 if Finished then
  Exit;

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
     UserMessage(UserTxt[3],UserTxt[2],2);
     Exit;
    end;

   if (not Finished) and SensorWP then
    begin
     UserMessage(UserTxt[15]+UserTxt[16], UserTxt[17],2);
     Exit;
    end;
  end; {if}

  __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
  __QuikWin(Col8,Row13,MainFore,MainBack,SensorUserText);

 if FactoryCal then
  begin
   UserMessage(UserTxt[20],'',1);
   Exit;
  end;

 if not YesOrNo(UserTxt[8],UserTxt[9]) then
   Exit;

 WaitMessage(' Please wait  . . .');
 repeat
  ReadKMatrix (158,ReadKMOk,KElement);
  Finished := RetryYesOrNo(ReadKMOk,UserTxt[6],UserTxt[7],2);
 until ReadKMOk OR Finished;
 if Finished then
  Exit;

 WriteKMatrix(103,SensorConnected);

 if Finished then
  Exit;

 repeat
  SetFactoryCal(SensorConnected);
  Finished := RetryYesOrNo(SensorConnected,UserTxt[6],UserTxt[7],2);
 until SensorConnected OR Finished;
 if Finished then
  Exit;

 WriteUserText(' ');          {clear user text}
 __QuikWin(Col8,Row13,MainFore,MainBack,'          ');
 if Finished then
  Exit;

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
 if Finished then
  Exit;

 UserMessage(UserTxt[21],UserTxt[22],1);
end;



Procedure CalFactoryKM;   {tast k   debug mode only!!}
const KMa : array[0..8] of INTEGER = (3909,-763,-76,354,2392,-94,5,-31,3346);
var n : BYTE;
begin
 COMStatus := WriteCOMPort(PortNo,'MX,');    {testmode}
 for n := 0 TO 8 do
  begin
   WriteEEPROM(103 + (n * 2),    Hi(KMa[n]));
   WriteEEPROM(103 + (n * 2) + 1,Lo(KMa[n]));
  end; {for}
end;

