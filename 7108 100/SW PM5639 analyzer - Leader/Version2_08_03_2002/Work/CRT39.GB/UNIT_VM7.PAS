Unit Unit_vm7;

{$define laser}

Interface
Uses Crt,
     Dos,
     Unit_Sup,          { POWER TOOLS PLUS Support   unit }
     Unit_Key,          { POWER TOOLS PLUS Keyboard  unit }
     Unit_Scn,          { POWER TOOLS PLUS Screen    unit }
     Unit_Win,          { POWER TOOLS PLUS Window    unit }
     Unit_Mnu,          { POWER TOOLS PLUS Menu      unit }
     Unit_Pgm,          { POWER TOOLS PLUS Program   unit }
     Unit_Sel,          { POWER TOOLS PLUS Pick List unit }
     Unit_LJ2,
     PepUtil,
     Comutil;


const
{ ------------ SW version   og   12NC -----------------------------------}
 SWVer              = 'VM7_5655   ver. 960104';
 _12NC              = '7108 100 74251';


type
 TDType          = array[1..71,1..3] of real;         { Technical Data }
 MeasUnitsType   = array[1..71] of str_3;
 MeasParamType   = array[1..71] of str_30;
 NomRefDataType  = array[1..65,1..2] of real;
 DiffMeasType    = array[1..19] of byte;
 CerDiffMeasType = array[1..17] of byte;
 CerMeasType     = array[1..33] of byte;
 PD55Type        = array[1..71] of str_20;
 InstrumentListType = array[1..12,1..2] of str_40;
const

{ ================   PTV diff m†lespecifikationer   ========================}
 NoOfDiffMeasG      : byte = 17;            { G  PROG IN/OUT diff m†linger }
 DiffMeasG          : DiffMeasType =
                      (71,5,11,13,14,16,18,20,43,44,45,46,47,48,59,65,68,0,0);

 NoOfDiffMeasM      : byte = 17;            { M  PROG IN/OUT diff m†linger }
 DiffMeasM          : DiffMeasType =
                      (71,5,11,13,14,16,18,20,43,44,45,46,47,48,58,65,68,0,0);


{ ================   Certificate m†lespecifikationer   =====================}
 NoOfCerMeasG       = 31;         { G  Certificate m†linger }
 CerMeasG           : CerMeasType =
                      (66,1,5,67,9,11,12,13,14,16,18,20,22,23,26,27,29,33,34,35,36,
                       39,40,43,44,45,46,47,48,59,70,0,0);
 NoOfCerDiffMeasG   = 15;         { G  PROG IN/OUT certificate diff m†linger }
 CerDiffMeasG       : CerDiffMeasType =
                      (71,5,13,14,16,18,20,43,44,45,46,47,48,59,68,0,0);


 NoOfCerMeasM       = 31;         { M  Certificate m†linger }
 CerMeasM           : CerMeasType =
                      (66,1,5,67,11,12,13,14,16,18,20,22,23,26,28,29,33,34,35,36,
                       39,40,41,43,44,45,46,47,48,58,70,0,0);
 NoOfCerDiffMeasM   = 16;         { M  PROG IN/OUT certificate diff m†linger }
 CerDiffMeasM       : CerDiffMeasType =
                      (71,5,13,14,16,18,20,43,44,45,46,47,48,65,58,68,0);


{ ------------ M†letyper --------------------------------------------}
 AbsoluteMeas    = 1;   { 5655 PROGRAM OUT, FULL FIELD, VITS on }
 RefMeas         = 2;   { 5640 PROGRAM OUT }
 PROGOUTMeas     = 3;   { 5655 PROGRAM OUT med 5640 tilsluttet PROGRAM IN.
                          VITS signaler fra 5640, (5655 med VITS off)    }

{ ------------ Apparat versioner -------------------------------------}
 PM5655G         = 1;
 PM5655M         = 2;

 Apparat         : array[1..2] of str_10 =
                   ('PM5655G',
                    'PM5655M');

{ disse 2 filer indeholder tekniske data og produkt data med tolerancer }
{$I DEF_VM7.INC}
{$I 5655.INC}

 COMNo           = _COM2;
 PrinterPort     = 'LPT2';
 BLTFileName     = 'VM7_BLT.DAT';    { Bar Line Time measurements }
 CLGDFileName    = 'VM7_CLGD.DAT';   { Chr/Lum Gain Delay measurements }
 CertifName      = 'CERTIF.HP';      { udskrivningsfil for kunde certificate }
 FName           = 'VM7_AUTO.DAT';   { m†ledataformat direkte fra VM700 }
 AutoRefFileName = 'REFMEAS.VM7';    { m†ledata ved ref m†ling p† PROGRAM IN }
 PROGOUTFileName = 'PROGOUT.VM7';    { m†ledata ved m†ling p† PROGRAM OUT }
 PathName        = 'S:\MEASDATA\5655\'; { placering af m†lerapport p† LAN }
 NameExt         = '._55';           { PTV m†lerapport for 5655 serie }
 MeasFileName    = 'NEW_MEAS'+NameExt;

 COMPortTimeout = 4;
 VM700AutoTimeout = 116;   { max sec. m†letid for et s‘t auto m†linger }

 Col1   =  2;    { placering af edit/tekst felter p† sk‘rm }
 Col2   = 28;
 Col3   = 47;
 Col4   = 70;
 Row1   =  5;
 Row2   =  7;
 Row3   =  9;
 Row4   = 11;
 Row5   = 13;

 EditFore      = White;
 EditBack      = Blue;
 EditMainFore  = Black;
 EditMainBack  = Cyan;

var
 ioerr          : integer;
 AppVer         : byte;          { Apparat version }

 MeasStr        : str_80;
 CertifNo,
 KUNo           : str_20;
 Subc_Str       : str_10;
 RL_Str,
 OU_Str,
 DD_Str        : str_5;
 F,G,
 MeasFile,
 AutoFile       : text;
 Meas           : array[1..71] of
                     record
                      RefData  : real;       { ref m†linger fra f.eks. 5640 }
                      RefOK    : boolean;    { ref m†ling OK/FAIL}
                      Data     : real;       { m†linger fra DUT }
                      OK       : boolean;    { m†ling OK/FAIL}
                      ResStr   : str_6;      { m†leresultat string }
                      DiffData : real;       { PROG IN/OUT diff m†ling }
                      DiffOK   : boolean;    { diff m†ling OK/FAIL }
                     end;
 InputSNRatio   : real;                      { udskrives p† m†leblad }
                                             { ved diff m†linger }

 TD             : TDType;                    { Technical Data }
 MeasParam      : MeasParamType;
 MeasUnits      : MeasUnitsType;
 NomRefData     : NomRefDataType;
 DiffMeas       : DiffMeasType;
 CerMeas        : CerMeasType;
 CerDiffMeas    : CerDiffMeasType;
 InstrumentList : InstrumentListType;
 PD55,
 PD55_Diff      : PD55Type;
 NoOfDiffMeas,                       { antal diff m†linger }
 NoOfCerMeas,                        { antal absolute certificate m†linger }
 NoOfCerDiffMeas : byte;             { antal diff certificate m†linger }


 ValidSubcFreq,
 ValidRL,
 ValidOU,
 ValidDD       : boolean;

 VM700Test,
 VM700Connected,
 FileFound,
 ProgramSlut,
 Average,
 Check          : boolean;

 St             : word;          { Status for COM port}

 EditMainWin,                    { edit windows }
 EditSubcWin,
 EditRLWin,
 EditOUWin,
 EditDDWin     : _WindowPtr;



Procedure InitVar;
Procedure InitMeasData;
Procedure CleanUp;
Procedure InitPC_COMPort;
Procedure CreateAllWindows;
Procedure CreateAllMenus;
Procedure ShowVM700Setup;              { F2 }
Procedure WriteBottomWindow(No : byte);
Procedure WriteTopWindow;
Procedure WriteMainWindow;
Procedure ShowStartupMessage;
Procedure Show5640Setup;              { F1 }
Procedure EnterKUNo;
Procedure EnterCertifNo;
Procedure SelectVer;
Procedure CopyFileToLaserjet(CerFlag : boolean);
Procedure Meas_ChromLumGainDelay;
Procedure Meas_BarLineTime;
Procedure Meas_Auto(MeasType : byte);
Procedure Read_Auto(MeasType : byte);
Procedure ComparePROG_IN_OUT;
Procedure MakeCertif;
Procedure DisplaySavedMeasFile;
Procedure DisplayFile;
Procedure SaveMeasFileToLAN;


Implementation
const
 TopFore     =   Black;
 TopBack     =   LightGray;
 MainFore    =   Yellow;
 MainBack    =   Blue;
 BarFore     =   Black;
 BarBack     =   Green;
 HelpFore    =   Black;
 HelpBack    =   Cyan;
 LFore       =   Red;
 LBack       =   Blue;
 ErrFore     =   White;
 ErrBack     =   Black;
 PickFore    =   Black;
 PickBack    =   LightGray;
 ViewFore    =   Black;
 ViewBack    =   LightGray;
 MessFore    =   White;
 MessBack    =   Red;
 DosFore     =   White;
 DosBack     =   Black;
 SetupFore   =   Black;
 SetupBack   =   Cyan;
 HFore       =   Red;
 HBack       =   LightGray;
 RetryFore   =   Black;
 RetryBack   =   LightGray;
 ShaFore     =   DarkGray;
 ShaBack     =   Black;
 PFore       =   LightGray;
 PBack       =   Black;

 InQueSize          = 1000;         {COM port}
 OutQueSize         = 100;          {  -   - }


var
 RetryWin,
 SaveWin,
 SetupWin,
 TopWin,
 MainWin,
 BottomWin,
 MessWin,
 ViewWin,
 PickWin,
 HelpWin           : _WindowPtr;
 SaveMnu,
 RetryMnu          : _MenuPtr;
 IPtr,
 SavePtr,
 RetryPtr          : _ItemPtr;
 Key               : _Keys;


 DateStr           : str_6;
 LaserFileName     : str_50;
 svar              : string;
 AutoFileName      : str_80;        { Intern PTV m†lerapport }


{$I VM700.INC}


Procedure CopyFileToLaserjet(CerFlag : boolean);
{
Til udskrivning bruges DOS-kommando:   COPY <file> LPTx /b > NUL
}
var
 PickPtr  : _PickPtr;
 RetPath  : _Path;
 KeySeq   : _KeySeq;
 ExErrCode : word;
begin
 if not CerFlag then
  begin
   Check    := __TitleWin(PickWin,' Print file: '+PathName+'*'+NameExt+' ',
                         White,PickBack,_TopCenter);
   PickPtr := __MakeSel(PickWin,White,Green,nil,0,0,0,1,1,nil,false);
   RetPath := __DirSel(PickPtr,_MREMOVE_MNU,_DOS_FMT_SEL,@LessName,
                       PathName+'*'+NameExt,AnyFile,false,KeySeq);
   Check := __Zapsel(PickPtr);  { release pick memory }
  __COffScn(false);             { cursor on }
  LaserFileName := RetPath;
 end;

 if CerFlag then
   LaserFileName := CertifName;    { certif.hp }

 if KeySeq._Ch <> _KeyTable[_Key_Esc]._Ch then
  begin
   assign(F,LaserFileName);
   {$I-} Reset(F); {$I+}
   if (IOResult <> 0) then
    Exit;
   {$I-} Close(F); {$I+}

{$ifdef laser}
   if not CerFlag then
    __WinExPgm('DOS','COPY RESET.HP '+PrinterPort+' /B > NUL',1,2,_CurColumns,_CurRows,DosFore,DosBack,ExErrCode);

   __WinExPgm('DOS','COPY '+LaserFileName+' '+PrinterPort+' /B > NUL',1,2,_CurColumns,_CurRows,DosFore,DosBack,ExErrCode);

   if not CerFlag then
    __WinExPgm('DOS','COPY EJECT.HP '+PrinterPort+' /B > NUL',1,2,_CurColumns,_CurRows,DosFore,DosBack,ExErrCode);
{$endif}

   case ExErrCode of
    0 : if CerFlag then
         begin
          writeln('Certificate ',CertifNo,' printed on port ',PrinterPort);
         end
         else
          writeln(LaserFileName,' printed on port ',PrinterPort);

    8 : writeln('Not enough memory to print');
    else
     writeln(' Problems with writing to printer ??');
   end;  { case }
  end; { if }

{ if ExErrCode <> 0 then
  TestFailed := true;}

end; { CopyFileToLaserjet }



Procedure SaveMeas(MType : byte; MeasNo : byte ; str : string);
{ MType:  absolute: M†linger gemmes i Meas.Data.
                    M†linger skrives til file AutoFileName = xxxxxx._aa
                    (xxxxxx = KUNo    aa = apparat).
          ref:      M†linger gemmes i Meas.RefData.
                    M†linger skrives til file AutoRefFileName.
          progout:  M†ling p† PROG OUT gemmes i Meas.Data.
                    M†linger skrives til file PROGOUTFileName.
}

const digits = ['0'..'9','.','-'];
var kode     : integer;
    s        : str_20;
    sp1,sp2,
    n        : byte;
begin

 if pos('--',str) > 0 then          { auto parameter not found }
  Meas[MeasNo].ResStr := '------';

 s := '';
 for n := 1 to length(str) do
  begin
   if str[n] in digits then         {delete all chars except digits}
   s := s + str[n];
  end;

 case MType of
  AbsoluteMeas,
  PROGOUTMeas   : begin
                    val(s,Meas[MeasNo].Data,kode);

                     { NTSC m†linger er i % Bar, men skal angives i IRE }
                     { Meas[1].Data = Bar Amplitude m†lt i IRE }
                    if AppVer = PM5655M then
                     case MeasNo of
 {sync amplitude}     23 : Meas[23].Data := Meas[23].Data * Meas[1].Data / 100;
 {burst amplitude}    29 : Meas[29].Data := Meas[29].Data * Meas[1].Data / 100;
                     end;

                    if MType = AbsoluteMeas then
                     Meas[MeasNo].OK :=          { check for ñ limits }
                        (Meas[MeasNo].Data >= (TD[MeasNo,1] - TD[MeasNo,2])) and
                        (Meas[MeasNo].Data <= (TD[MeasNo,1] + TD[MeasNo,2])) and
                        (kode = 0);

                    if (MType = AbsoluteMeas) and (MeasNo in [53,58,59,70]) then   { check only for greater than }
                     Meas[MeasNo].OK := (Meas[MeasNo].Data > TD[MeasNo,1]) and
                                        (kode = 0);

                    if MType = PROGOUTMeas then
                     Meas[MeasNo].OK := kode = 0;

                    if not Meas[MeasNo].OK then
                     VM700Test := false;

                  end;
        RefMeas : begin
                    val(s,Meas[MeasNo].RefData,kode);

                     { NTSC m†linger er i % Bar, men skal angives i IRE }
                     { Meas[1].RefData = Bar Amplitude m†lt i IRE }
                    if AppVer = PM5655M then
                     case MeasNo of
 {sync amplitude}     23 : Meas[23].RefData := Meas[23].RefData * Meas[1].RefData / 100;
 {burst amplitude}    29 : Meas[29].RefData := Meas[29].RefData * Meas[1].RefData / 100;
                     end;

                    Meas[MeasNo].RefOK :=          { check for ñ limits }
                       (Meas[MeasNo].RefData >= (NomRefData[MeasNo,1] - NomRefData[MeasNo,2])) and
                       (Meas[MeasNo].RefData <= (NomRefData[MeasNo,1] + NomRefData[MeasNo,2])) and
                       (kode = 0);

                    if MeasNo in [53,58,59,70] then      { check only for greater than }
                     Meas[MeasNo].RefOK := (Meas[MeasNo].RefData > NomRefData[MeasNo,1]) and
                                           (kode = 0);

                    if not Meas[MeasNo].RefOK then
                     VM700Test := false;
                  end;
 end; { case }

 if MeasNo in [68,69] then    { Delay Diff, NORMAL+BYPASS}
  exit;

 case MType of
  AbsoluteMeas,
  PROGOUTMeas :
    begin
     sp1 := 30-length(MeasParam[MeasNo]);
     sp2 := 4-length(MeasUnits[MeasNo]);

     if MeasNo in [66] then  { subc freq }
       write(MeasFile,'    ',MeasParam[MeasNo],' ':sp1-4,Meas[MeasNo].Data:10:2,' ',MeasUnits[MeasNo],' ':sp2,' ')
      else
       if Meas[MeasNo].ResStr = '------' then
         write(MeasFile,'    ',MeasParam[MeasNo],' ':sp1,'------',' ',MeasUnits[MeasNo],' ':sp2,' ')
        else
         write(MeasFile,'    ',MeasParam[MeasNo],' ':sp1,Meas[MeasNo].Data:6:1,' ',MeasUnits[MeasNo],' ':sp2,' ');

     if MType = AbsoluteMeas then
      begin
         if MeasNo in [53,58,59,70] then
           write(MeasFile,'   >',TD[MeasNo,1]:6:1,'        ')
          else
            if MeasNo in [66] then   { subc freq }
              write(MeasFile,TD[MeasNo,1]:10:2,' ñ',TD[MeasNo,2]:6:2)
             else
              write(MeasFile,'    ',TD[MeasNo,1]:6:1,' ñ',TD[MeasNo,2]:6:2);

         if Meas[MeasNo].OK then
           writeln(MeasFile,'        OK')
          else
           writeln(MeasFile,'        FAILED');

      end; { if }

     if MType = PROGOUTMeas then
      writeln(MeasFile);
    end;


 RefMeas:
    begin
     sp1 := 30-length(MeasParam[MeasNo]);
     sp2 := 4-length(MeasUnits[MeasNo]);

     if Meas[MeasNo].ResStr = '------' then
       write(MeasFile,'    ',MeasParam[MeasNo],' ':sp1,'------',' ',MeasUnits[MeasNo],' ':sp2,' ')
      else
       write(MeasFile,'    ',MeasParam[MeasNo],' ':sp1,Meas[MeasNo].RefData:6:1,' ',MeasUnits[MeasNo],' ':sp2,' ');

      if MeasNo in [53,58,59,70] then
        write(MeasFile,'   >',NomRefData[MeasNo,1]:6:1,'        ')
       else
        write(MeasFile,'    ',NomRefData[MeasNo,1]:6:1,' ñ',NomRefData[MeasNo,2]:6:2);

      if Meas[MeasNo].RefOK then
        writeln(MeasFile,'        OK')
       else
        writeln(MeasFile,'        FAILED');
    end;

  end; { case }
end; { SaveMeas }


Procedure ReadData(MType : byte; MeasNo : byte ; str : string);
{ kaldes fra ReadDiffData.
  l‘ser m†ledata fra str afh‘ngig af MType:
  MType = RefMeas:  M†linger gemmes i Meas.RefData.
        = PROGOUT:  M†ling p† PROG OUT gemmes i Meas.Data.
}

const digits = ['0'..'9','.','-'];
var kode     : integer;
    s        : str_20;
    n        : byte;
begin
 s := '';
 for n := 1 to length(str) do
  begin
   if str[n] in digits then         {delete all chars except digits}
   s := s + str[n];
  end;

 case MType of
  PROGOUTMeas   : begin
                    val(s,Meas[MeasNo].Data,kode);
                    if MeasNo = 1 then              { gain }
                      Meas[71].Data := Meas[MeasNo].Data;
                    if kode <> 0 then
                     VM700Test := false;
                  end;
  RefMeas       : begin
                    val(s,Meas[MeasNo].RefData,kode);
                    if MeasNo = 1 then              { gain }
                      Meas[71].RefData := Meas[MeasNo].RefData;
                    if kode <> 0 then
                     VM700Test := false;
                  end;
 end; { case }

end; { ReadData }



Procedure ReadCData(DF : boolean; MeasNo : byte ; str : string);
{ kaldes fra ReadDataForCertif.
  l‘ser m†ledata fra str til brug i certificate.
  DF = false: absolute m†linger  (DF = Diff Flag)
  DF = true:  diff m†linger
}

const digits = ['0'..'9','.','-'];
var kode     : integer;
    s        : str_20;
    n        : byte;
begin
 s := '';
 for n := 1 to length(str) do
  begin
   if str[n] in digits then         {delete all chars except digits}
   s := s + str[n];
  end;

 if DF then
   val(s,Meas[MeasNo].DiffData,kode)
  else
   val(s,Meas[MeasNo].Data,kode);

{ case MeasNo of
  68 : Meas[68].Data := Meas[68].RefData * 2;
  69 : Meas[69].Data := Meas[69].RefData * 2;
 end;
 }

 if kode <> 0 then
  VM700Test := false;

end; { ReadCData }


Procedure ReadInputSNRatio(str : string);
{ l‘ser Input S/N ratio brugt ved Random HF noise m†ling. Bruges ved
  udskrift af certificate }
const digits = ['0'..'9','.'];
var kode     : integer;
    s        : str_20;
    n        : byte;
begin
 s := '';
 for n := 1 to length(str) do
  begin
   if str[n] in digits then         {delete all chars except digits}
   s := s + str[n];
  end;

 val(s,InputSNRatio,kode);
 if kode <> 0 then
  VM700Test := false;

end;



Procedure Meas_ChromLumGainDelay;
{ Manuel m†ling af Chroma Gain/Delay med Average=on.
  m†ledata tilf›jes filen VM7_AUTO.DAT. }
var
 Slut       : boolean;
 n,
 p          : byte;
 GainStr,
 DelayStr   : str_50;
begin
 St := WritelnCOMPort(COMNo,'remote'); delay(50);
 St := WritelnCOMPort(COMNo,'terminal'); delay(50);

 repeat
   Assign(G,CLGDFileName);
   Rewrite(G);
   St := WritelnCOMPort(COMNo,'execute ChromLum~GainDelay'); delay(50);
   St := ClearInputQue(COMNo);
   St := WritelnCOMPort(COMNo,'getresults');
   Slut := false;
   write('Waiting for Chr/Lum data from VM700 .');
   repeat
    St := ReadlnCOMPort(COMNo,svar,5);
    if svar[1] = LF then
     delete(svar,1,1);
    write('.');
    p := pos('Results',svar);
    Slut := copy(svar,p,16) = 'Results in file:';
   until Slut or (St <> 0);
   writeln;

   delay(200);
   St := ClearInputQue(COMNo);
   St := WritelnCOMPort(COMNo,'show ChromLum~GainDelay');
   slut := false;
   repeat
    St := ReadlnCOMPort(COMNo,svar,10);
    if svar[1] = LF then
     delete(svar,1,1);
   { writeln(svar);}
    writeln(G,svar);
   until (svar = 'VM700> ') or (St <> 0);
   Close(G);


   { ----------  add Gain Delay measurements to VM700 autofile --------------- }

   Assign(G,CLGDFileName);
   Reset(G);
   Average := true;
   while not Eof(G) do
    begin
     readln(G,MeasStr);
     if pos('Chroma Gain',MeasStr) = 1 then
      begin
       delete(MeasStr,15,3);
       GainStr := MeasStr;
      end
      else
     if pos('Chroma Delay',MeasStr) = 1 then
      begin
       delete(MeasStr,15,3);
       DelayStr := MeasStr;
      end
      else
     if pos('Average Off',MeasStr) = 1 then
      Average := false;
    end; {while}

   Close(G);

   if Average then
    begin
     Assign(F,FName);
     Append(F);
     writeln(F,GainStr);
     writeln(F,DelayStr);
     Close(F);
    end;

   if not Average then
     St := WritelnCOMPort(COMNo,'hardkey Average'); delay(50);

 until Average;
end; { Meas_ChromLumGainDelay }




Procedure Meas_BarLineTime;
{ Manuel m†ling af Bar/Line Time med Average=on.
  m†ledata tilf›jes filen VM7_AUTO.DAT. }
var
 Slut       : boolean;
 n,
 p          : byte;
 BarTiltStr : str_50;
begin
 St := WritelnCOMPort(COMNo,'remote'); delay(50);
 St := WritelnCOMPort(COMNo,'terminal'); delay(50);

 repeat
   Assign(G,BLTFileName);
   Rewrite(G);
   St := WritelnCOMPort(COMNo,'execute Bar~LineTime'); delay(50);
   St := ClearInputQue(COMNo);
   St := WritelnCOMPort(COMNo,'getresults');
   Slut := false;
   write('Waiting for Bar Line Time data from VM700 .');
   repeat
    St := ReadlnCOMPort(COMNo,svar,5);
    if svar[1] = LF then
     delete(svar,1,1);
    write('.');
    p := pos('Results',svar);
    Slut := copy(svar,p,16) = 'Results in file:';
   until Slut or (St <> 0);
   writeln;

   delay(200);
   St := ClearInputQue(COMNo);
   St := WritelnCOMPort(COMNo,'show Bar~LineTime');
   slut := false;
   repeat
    St := ReadlnCOMPort(COMNo,svar,10);
    if svar[1] = LF then
     delete(svar,1,1);
    {writeln(svar);}
    writeln(G,svar);
   until (svar = 'VM700> ') or (St <> 0);
   Close(G);


   { ----------  add Bar Tilt measurements to VM700 autofile --------------- }

   Assign(G,BLTFileName);
   Reset(G);
   Average := true;
   while not Eof(G) do
    begin
     readln(G,MeasStr);
     if pos('Bar Tilt',MeasStr) = 1 then
      begin
       delete(MeasStr,21,3);
       BarTiltStr := MeasStr;
      end
      else
     if pos('Average Off',MeasStr) = 1 then
      Average := false;
    end; {while}

   Close(G);

   if Average then
    begin
     Assign(F,FName);
     Append(F);
     writeln(F,BarTiltStr);
     Close(F);
    end;

   if not Average then
     St := WritelnCOMPort(COMNo,'hardkey Average'); delay(50);

 until Average;
end; { Meas_BarLineTime }



Procedure Meas_Auto(MeasType : byte);
var
 slut : boolean;
 n,
 p,
 Timeout : byte;
 Sc      : byte;
begin
 Check := __DispWin(MessWin); __COffScn(true);

 case MeasType of
  AbsoluteMeas  : begin
                   if AppVer = PM5655G then
                     writeln(' Connect VM700 CH-A to PROGRAM OUT on PM5655G')
                    else
                     writeln(' Connect VM700 CH-C to PROGRAM OUT on PM5655M');

                   writeln(' PM5655: FULL FIELD');
                   if AppVer = PM5655G then
                     writeln('         PATTERN: 50 Hz (Menu: TESTSIGNAL -> PATTERNS -> 50 Hz -> SAVE)')
                    else
                     writeln('         PATTERN: 60 Hz (Menu: TESTSIGNAL -> PATTERNS -> 60 Hz -> SAVE)');
                   writeln('         VITS ON        (Menu: VITS -> GROUP -> Grp1 -> SAVE)');
                   writeln;
                   writeln(' Remember 75 ê termination on VM700');
                  end;
  RefMeas      :  begin
                   if AppVer = PM5655G then
                    begin
                     writeln(' Connect VM700 CH-A to PROGRAM OUT on PM5640G');
                     writeln;
                     writeln(' PM5640G: FULL FIELD');
                     writeln('          PATTERN 110');
                     writeln('          VITS ON');
                     writeln('          VITS linie 17=A  18=B  22=ERASE  330=C  331=D');
                    end
                    else
                    begin
                     writeln(' Connect VM700 CH-C to PROGRAM OUT on PM5640M');
                     writeln;
                     writeln(' PM5640M: FULL FIELD');
                     writeln('          PATTERN 110');
                     writeln('          VITS ON');
                     writeln('          VITS linie F1,17=A   F1,19=E   F1,12=ERASE');
                     writeln('                     F2,17=B   F2,19=E');
                    end;   { if }
                   writeln;
                   writeln(' Remember 75 ê termination on VM700');
                  end;
  PROGOUTMeas :   begin
                   if AppVer = PM5655G then
                    begin
                     writeln(' Connect VM700 CH-A to PROGRAM OUT on PM5655G');
                     writeln(' Connect PM5640G PROGRAM OUT to PM5655G PROGRAM IN');
                     writeln;
                     writeln(' PM5640G: FULL FIELD');
                     writeln('          PATTERN 110');
                     writeln('          VITS ON');
                     writeln('          VITS linie 17=A  18=B  22=ERASE  330=C  331=D');
                     writeln;
                     writeln(' PM5655G: NORMAL');
                     writeln('          VITS OFF  (Menu: VITS -> GROUP -> DISABLED -> SAVE)');
                    end
                    else
                    begin
                     writeln(' Connect VM700 CH-C to PROGRAM OUT on PM5655M');
                     writeln(' Connect PM5640M PROGRAM OUT to PM5655M PROGRAM IN');
                     writeln;
                     writeln(' PM5640M: FULL FIELD');
                     writeln('          PATTERN 110');
                     writeln('          VITS ON');
                     writeln('          VITS linie F1,17=A   F1,19=E   F1,12=ERASE');
                     writeln('                     F2,17=B   F2,19=E');
                     writeln;
                     writeln(' PM5655M: NORMAL');
                     writeln('          VITS OFF  (Menu: VITS -> GROUP -> DISABLED -> SAVE)');
                    end;  { if }
                  end;
 end; { case }
 writeln;
 writeln(' Continue: ENTER         Stop: ESC');

 repeat
  __FlushKey;
  Sc := scancode
 until Sc in [Enter,Escape];

 Check := __RemWin;
 __COffScn(false);

 if Sc = Escape then
  begin
   writeln('Measurement cancelled');
   VM700Test := false;
   exit;
  end;

 Timeout := VM700AutoTimeout div COMPortTimeout;
 St := WritelnCOMPort(COMNo,'remote');
 delay(100);
 St := WritelnCOMPort(COMNo,'terminal');
 delay(100);
 if AppVer = PM5655G then
   St := WritelnCOMPort(COMNo,'hardkey A')
  else
   St := WritelnCOMPort(COMNo,'hardkey C');
 delay(200);
 St := WritelnCOMPort(COMNo,'execute Auto');
 delay(500);

 St := ClearInputQue(COMNo);
 St := WritelnCOMPort(COMNo,'getresults');

 slut := false;
 n := 1;
 write('Waitng for data from VM700 .');
 repeat
  St := ReadlnCOMPort(COMNo,svar,COMPortTimeout);
  Inc(n);
  write('.');
  if svar[1] = LF then
   delete(svar,1,1);
  slut := copy(svar,pos('Results',svar),16) = 'Results in file:';
  if slut then
   begin
    writeln(' ',n * COMPortTimeout,' sec');
    {writeln(svar);}
   end;
 until slut or (n > Timeout);

 if not slut then
  begin
   writeln(' ',n * COMPortTimeout,' sec - Timeout');
   exit;
  end;

 assign(F,FName);
 rewrite(F);
 delay(1000);
 n := 1;
 write('Getting data from VM700 .');
 St := ClearInputQue(COMNo);
 St := WritelnCOMPort(COMNo,'show Auto');
 slut := false;
 repeat
  St := ReadlnCOMPort(COMNo,svar,5);
  if svar[1] = LF then
   delete(svar,1,1);
  Inc(n);
  if (n mod 3) = 0 then
   write('.');
  writeln(F,svar);
{  if (n in [2,3,9,10]) then
  writeln(n,'=',svar);   }
 until (svar = 'VM700> ') or (St <> 0);
 writeln;
 writeln('Data from VM700 saved in file ''',FName,'''');
 close(F);

end; { Meas_Auto }




Procedure Read_Auto(MeasType : byte);
{ L‘ser data fra VM700-fil med m†ledata, og gemmer m†lingerne i
  filen AutoFileName }
var
 IOErr    : integer;
begin

 case MeasType of
  AbsoluteMeas  : begin
                   EnterKUNo;                     { KU nummer for DUT}
                   Assign(MeasFile,MeasFileName);
                   {$I-} Rewrite(MeasFile);{$I+}
                   case IOResult of
                    0 : begin
                         writeln(MeasFile,'    Philips TV Test Equipment A/S');
                         writeln(MeasFile,'    VM700 testresults for ',Apparat[AppVer],'     KU',KuNo);
                         writeln(MeasFile,  '    Name: ',GetEnv('NAME'),'     Date: ',dato,'  ',Time);
                        end;
                    3 : begin
                         Check := __DispWin(MessWin); __COffScn(true);
                         writeln;
                         writeln(Bell,' Failure!  Directory not found: ',PathName);
                         writeln(' Directory to store results in must exist.');
                         writeln(' Make the directory and start again/');
                         VM700Test := false;
                         writeln; writeln; writeln;
                         writeln(' Press a key . . ');
                         WaitForAnyKey;
                         Check := __RemWin;
                         __COffScn(false);
                         exit;
                        end;
                   end; { case }
                  end;
  RefMeas      : begin
                   Assign(MeasFile,AutoRefFileName);
                   Rewrite(MeasFile);
                   writeln(MeasFile,'    Philips TV Test Equipment A/S');
                   writeln(MeasFile,'    PM5640 reference measurements for ',Apparat[AppVer],' (PROG IN)');
                   writeln(MeasFile,'    Name: ',GetEnv('NAME'),'     Date: ',dato);
                  end;
  PROGOUTMeas  : begin
                   Assign(MeasFile,PROGOUTFileName);
                   Rewrite(MeasFile);
                   writeln(MeasFile,'    Philips TV Test Equipment A/S');
                   writeln(MeasFile,'    PROG OUT measurements for ',Apparat[AppVer]);
                   writeln(MeasFile,'    Name: ',GetEnv('NAME'),'     Date: ',dato);
                  end;
 end; { case }


 writeln(MeasFile);
 writeln(MeasFile,'    PROGRAM OUT, FULL FIELD absolute measurements:');
 write(MeasFile,'    ');
 write(MeasFile,'Parameter                 Measured value');
 if MeasType in [AbsoluteMeas,RefMeas] then
  write(MeasFile,'       Technical data     Status');
 writeln(MeasFile);

 Assign(F,FName);
 Reset(F);       { Read from auto report file: VM7_AUTO.DAT }

 if AppVer = PM5655G then
 while not Eof(F) do
  begin
   readln(F,MeasStr);
     if (pos('Luminance Bar Ampl',MeasStr) = 1) and
        (pos('mV',MeasStr) > 20) then
        begin
       {writeln(MeasStr);     <<<<<<<<<<<<<<<<<< }
       SaveMeas(Meastype,1,copy(MeasStr,20,15))
       end
      else
     if pos('Bar Tilt (Rec 569)',MeasStr) = 1 then
       SaveMeas(Meastype,5,copy(MeasStr,20,15))
      else
     if pos('Sync/Bar (Rel 3/7)',MeasStr) = 1 then
       SaveMeas(Meastype,9,copy(MeasStr,20,15))
      else
     if pos('Pulse/Bar Ratio Err',MeasStr) = 1 then
       SaveMeas(Meastype,11,copy(MeasStr,20,15))
      else
     if pos('2T Pulse K-factor',MeasStr) = 1 then
       SaveMeas(Meastype,12,copy(MeasStr,20,15))
      else
     if pos('Chroma Gain',MeasStr) = 1 then
       SaveMeas(Meastype,13,copy(MeasStr,20,15))
      else
     if pos('Chroma Delay',MeasStr) = 1 then
       SaveMeas(Meastype,14,copy(MeasStr,20,15))
      else
     if pos('Lum. Nonlin. Dist.',MeasStr) = 1 then
       SaveMeas(Meastype,16,copy(MeasStr,20,15))
      else
     if pos('Pk-Pk Diff Gain',MeasStr) = 1 then
       SaveMeas(Meastype,18,copy(MeasStr,20,15))
      else
     if pos('Pk-Pk Diff Phase',MeasStr) = 1 then
       SaveMeas(Meastype,20,copy(MeasStr,20,15))
      else
     if pos('Sync Amplitude',MeasStr) = 1 then
       SaveMeas(Meastype,23,copy(MeasStr,20,15))
      else
     if pos('Sync-to-Burst Start',MeasStr) = 1 then
       SaveMeas(Meastype,26,copy(MeasStr,20,15))
      else
     if (pos('Burst Duration',MeasStr) = 1) and
        (pos('us',MeasStr) > 15) then
       SaveMeas(Meastype,27,copy(MeasStr,20,15))
      else
     if (pos('Burst Duration',MeasStr) = 1) and
        (pos('Cycles',MeasStr) > 15) then
       SaveMeas(Meastype,28,copy(MeasStr,20,15))
      else
     if pos('Burst Amplitude',MeasStr) = 1 then
       SaveMeas(Meastype,29,copy(MeasStr,20,15))
      else
     if pos('SCH Phase',MeasStr) = 1 then
       SaveMeas(Meastype,33,copy(MeasStr,20,15))
      else
     if pos('Sync Duration',MeasStr) = 1 then
       SaveMeas(Meastype,34,copy(MeasStr,20,15))
      else
     if pos('Sync Rise Time',MeasStr) = 1 then
       SaveMeas(Meastype,35,copy(MeasStr,20,15))
      else
     if pos('Sync Fall Time',MeasStr) = 1 then
       SaveMeas(Meastype,36,copy(MeasStr,20,15))
      else
     if pos('Broad Pulse Sep',MeasStr) = 1 then
       SaveMeas(Meastype,39,copy(MeasStr,20,15))
      else
     if pos('Equalizing Pulse',MeasStr) = 1 then
       SaveMeas(Meastype,40,copy(MeasStr,20,15))
      else
     if pos('MB Packet #1',MeasStr) = 1 then
       SaveMeas(Meastype,43,copy(MeasStr,20,15))
      else
     if pos('MB Packet #2',MeasStr) = 1 then
       SaveMeas(Meastype,44,copy(MeasStr,20,15))
      else
     if pos('MB Packet #3',MeasStr) = 1 then
       SaveMeas(Meastype,45,copy(MeasStr,20,15))
      else
     if pos('MB Packet #4',MeasStr) = 1 then
       SaveMeas(Meastype,46,copy(MeasStr,20,15))
      else
     if pos('MB Packet #5',MeasStr) = 1 then
       SaveMeas(Meastype,47,copy(MeasStr,20,15))
      else
     if pos('MB Packet #6',MeasStr) = 1 then
       SaveMeas(Meastype,48,copy(MeasStr,20,15))
      else
     if pos('S/N.2 Lum-wgtd (567)',MeasStr) = 1 then
       SaveMeas(Meastype,59,copy(MeasStr,22,13))
      else
     if pos('Field Time Dist',MeasStr) = 1 then
       SaveMeas(Meastype,65,copy(MeasStr,20,15))
      else

  { ----------  Absolutte m†linger manuelt indtastet    ----------- }
     if pos('Subcarrier Freq',MeasStr) = 1 then
       SaveMeas(Meastype,66,copy(MeasStr,25,length(MeasStr)-24))
      else
     if pos('Bar over/undershoot',MeasStr) = 1 then
       SaveMeas(Meastype,67,copy(MeasStr,25,length(MeasStr)-24))
      else
     if pos('Return loss, PROGRAM IN',MeasStr) = 1 then
       SaveMeas(Meastype,70,copy(MeasStr,25,length(MeasStr)-24));

  end; { while }


 if AppVer = PM5655M then
 while not Eof(F) do
  begin
   readln(F,MeasStr);
     if (pos('Bar Amplitude',MeasStr) = 1) and
        (pos('IRE',MeasStr) > 20) then
       SaveMeas(Meastype,1,copy(MeasStr,20,15))
      else
     if pos('Bar Tilt (Rec. 569)',MeasStr) = 1 then
       SaveMeas(Meastype,5,copy(MeasStr,20,15))
      else
     if pos('Pulse/Bar Ratio',MeasStr) = 1 then
       SaveMeas(Meastype,11,copy(MeasStr,20,15))
      else
     if pos('2T Pulse K-Factor',MeasStr) = 1 then
       SaveMeas(Meastype,12,copy(MeasStr,20,15))
      else
     if pos('Chroma Gain',MeasStr) = 1 then
       SaveMeas(Meastype,13,copy(MeasStr,20,15))
      else
     if pos('Chroma Delay',MeasStr) = 1 then
       SaveMeas(Meastype,14,copy(MeasStr,20,15))
      else
     if pos('Lum Non-Linearity',MeasStr) = 1 then
       SaveMeas(Meastype,16,copy(MeasStr,20,15))
      else
     if pos('Differential Gain',MeasStr) = 1 then
       SaveMeas(Meastype,18,copy(MeasStr,20,15))
      else
     if pos('Differential Phase',MeasStr) = 1 then
       SaveMeas(Meastype,20,copy(MeasStr,20,15))
      else
     if pos('Sync Amplitude',MeasStr) = 1 then
       SaveMeas(Meastype,23,copy(MeasStr,20,15))
      else
     if pos('Sync to Burst Start',MeasStr) = 1 then
       SaveMeas(Meastype,26,copy(MeasStr,20,15))
      else
     if (pos('RS-170A Burst Width',MeasStr) = 1) and
        (pos('Cycles',MeasStr) > 15) then
       SaveMeas(Meastype,28,copy(MeasStr,20,15))
      else
     if (pos('Burst Amplitude',MeasStr) = 1) and
        (pos('Bar',MeasStr) > 20) then
       SaveMeas(Meastype,29,copy(MeasStr,20,15))
      else
     if pos('SCH Phase',MeasStr) = 1 then
       SaveMeas(Meastype,33,copy(MeasStr,20,15))
      else
     if pos('RS-170A Sync Width',MeasStr) = 1 then
       SaveMeas(Meastype,34,copy(MeasStr,20,15))
      else
     if pos('Sync Risetime',MeasStr) = 1 then
       SaveMeas(Meastype,35,copy(MeasStr,20,15))
      else
     if pos('Sync Falltime',MeasStr) = 1 then
       SaveMeas(Meastype,36,copy(MeasStr,20,15))
      else
     if pos('RS-170A Serration',MeasStr) = 1 then
       SaveMeas(Meastype,39,copy(MeasStr,20,15))
      else
     if pos('RS-170A Equalizer',MeasStr) = 1 then
       SaveMeas(Meastype,40,copy(MeasStr,20,15))
      else
     if (pos('NTC7 Multiburst Flag',MeasStr) = 1) and
        (pos('Bar',MeasStr) > 20) then
       SaveMeas(Meastype,41,copy(MeasStr,20,15))
      else
     if pos('NTC7 MB Packet #1',MeasStr) = 1 then
       SaveMeas(Meastype,43,copy(MeasStr,20,15))
      else
     if pos('NTC7 MB Packet #2',MeasStr) = 1 then
       SaveMeas(Meastype,44,copy(MeasStr,20,15))
      else
     if pos('NTC7 MB Packet #3',MeasStr) = 1 then
       SaveMeas(Meastype,45,copy(MeasStr,20,15))
      else
     if pos('NTC7 MB Packet #4',MeasStr) = 1 then
       SaveMeas(Meastype,46,copy(MeasStr,20,15))
      else
     if pos('NTC7 MB Packet #5',MeasStr) = 1 then
       SaveMeas(Meastype,47,copy(MeasStr,20,15))
      else
     if pos('NTC7 MB Packet #6',MeasStr) = 1 then
       SaveMeas(Meastype,48,copy(MeasStr,20,15))
      else
     if pos('NTC7 Chr-Lum Intmd',MeasStr) = 1 then
       SaveMeas(Meastype,22,copy(MeasStr,20,15))
      else
     if pos('S/N.2 NTC7 Unwghtd',MeasStr) = 1 then
       SaveMeas(Meastype,58,copy(MeasStr,22,13))
      else
     if pos('Field Time Dist',MeasStr) = 1 then
       SaveMeas(Meastype,65,copy(MeasStr,20,15))
      else

  { ----------  Absolutte m†linger manuelt indtastet    ----------- }
     if pos('Subcarrier Freq',MeasStr) = 1 then
       SaveMeas(Meastype,66,copy(MeasStr,25,length(MeasStr)-24))
      else
     if pos('Bar over/undershoot',MeasStr) = 1 then
       SaveMeas(Meastype,67,copy(MeasStr,25,length(MeasStr)-24))
      else
     if pos('Return loss, PROGRAM IN',MeasStr) = 1 then
       SaveMeas(Meastype,70,copy(MeasStr,25,length(MeasStr)-24));

  end; { while }


 writeln(MeasFile);

 Close(F);
 Close(MeasFile);

  case Meastype of
   AbsoluteMeas  : writeln('Auto report saved to file ''',MeasFileName,'''');
   RefMeas       : writeln('PM5640 ref report saved to file ''',AutoRefFileName,'''');
   PROGOUTMeas   : writeln('PROG OUT report saved to file ''',PROGOUTFileName,'''')
  end;

end; { Read_Auto }




Procedure ReadDiffData(Meastype : byte);
{ L‘ser m†ledata fra 2 filer til brug ved beregning af diff m†ledata.
  Kaldes fra ComparePROG_IN_OUT. }
begin

 case Meastype of
  RefMeas     : Assign(F,AutoRefFileName);
  PROGOUTMeas : Assign(F,PROGOUTFileName);
 end;

 Reset(F);   { Read from file }

 if AppVer = PM5655G then
 while not Eof(F) do
  begin
   readln(F,MeasStr);
     if (pos('Luminance Bar Level',MeasStr) = 5) then
       ReadData(Meastype,1,copy(MeasStr,34,7))
      else
     if pos('Bar Tilt (Rec 569)',MeasStr) = 5 then
       ReadData(Meastype,5,copy(MeasStr,34,7))
      else
     if pos('Pulse/Bar Ratio',MeasStr) = 5 then
       ReadData(Meastype,11,copy(MeasStr,34,7))
      else
     if pos('Chrom/Lum Gain',MeasStr) = 5 then
       ReadData(Meastype,13,copy(MeasStr,34,7))
      else
     if pos('Chrom/Lum Delay',MeasStr) = 5 then
       ReadData(Meastype,14,copy(MeasStr,34,7))
      else
     if pos('Lum Non Lin',MeasStr) = 5 then
       ReadData(Meastype,16,copy(MeasStr,34,7))
      else
     if pos('Diff. Gain',MeasStr) = 5 then
       ReadData(Meastype,18,copy(MeasStr,34,7))
      else
     if pos('Diff. Phase',MeasStr) = 5 then
       ReadData(Meastype,20,copy(MeasStr,34,7))
      else
     if pos('Freq 0.5',MeasStr) > 0 then
       ReadData(Meastype,43,copy(MeasStr,34,7))
      else
     if pos('Freq 1.0',MeasStr) > 0 then
       ReadData(Meastype,44,copy(MeasStr,34,7))
      else
     if pos('Freq 2.0',MeasStr) > 0 then
       ReadData(Meastype,45,copy(MeasStr,34,7))
      else
     if pos('Freq 4.0',MeasStr) > 0 then
       ReadData(Meastype,46,copy(MeasStr,34,7))
      else
     if pos('Freq 4.8',MeasStr) > 0 then
       ReadData(Meastype,47,copy(MeasStr,34,7))
      else
     if pos('Freq 5.8',MeasStr) > 0 then
       ReadData(Meastype,48,copy(MeasStr,34,7))
      else
     if pos('Random HF noise-wgtd (567)',MeasStr) = 5 then
       ReadData(Meastype,59,copy(MeasStr,34,7))
      else
     if pos('Field Time Dist',MeasStr) = 5 then
       ReadData(Meastype,65,copy(MeasStr,34,7))
  end; { while }         { 5655G }

 if AppVer = PM5655M then
 while not Eof(F) do
  begin
   readln(F,MeasStr);
     if (pos('Luminance Bar Level',MeasStr) = 5) then
       ReadData(Meastype,1,copy(MeasStr,34,7))
      else
     if pos('Bar Tilt (Rec 569)',MeasStr) = 5 then
       ReadData(Meastype,5,copy(MeasStr,34,7))
      else
     if pos('Pulse/Bar Ratio',MeasStr) = 5 then
       ReadData(Meastype,11,copy(MeasStr,34,7))
      else
     if pos('Chrom/Lum Gain',MeasStr) = 5 then
       ReadData(Meastype,13,copy(MeasStr,34,7))
      else
     if pos('Chrom/Lum Delay',MeasStr) = 5 then
       ReadData(Meastype,14,copy(MeasStr,34,7))
      else
     if pos('Lum Non Linearity',MeasStr) = 5 then
       ReadData(Meastype,16,copy(MeasStr,34,7))
      else
     if pos('Diff. Gain',MeasStr) = 5 then
       ReadData(Meastype,18,copy(MeasStr,34,7))
      else
     if pos('Diff. Phase',MeasStr) = 5 then
       ReadData(Meastype,20,copy(MeasStr,34,7))
      else
     if pos('Freq 0.5',MeasStr) > 0 then
       ReadData(Meastype,43,copy(MeasStr,34,7))
      else
     if pos('Freq 1.0',MeasStr) > 0 then
       ReadData(Meastype,44,copy(MeasStr,34,7))
      else
     if pos('Freq 2.0',MeasStr) > 0 then
       ReadData(Meastype,45,copy(MeasStr,34,7))
      else
     if pos('Freq 3.0',MeasStr) > 0 then
       ReadData(Meastype,46,copy(MeasStr,34,7))
      else
     if pos('Freq 3.58',MeasStr) > 0 then
       ReadData(Meastype,47,copy(MeasStr,34,7))
      else
     if pos('Freq 4.2',MeasStr) > 0 then
       ReadData(Meastype,48,copy(MeasStr,34,7))
      else
     if pos('S/N Ratio Unweighted',MeasStr) = 5 then
       ReadData(Meastype,58,copy(MeasStr,34,7))
      else
     if pos('Field Time Dist',MeasStr) = 5 then
       ReadData(Meastype,65,copy(MeasStr,34,7))
  end; { while }      { 5655M }


 Close(F);
end; { ReadDiffData }



Procedure ReadDataForCertif(CFileName : _Path);
{ L‘ser m†ledata fra xxxxxxy._55 for udskrivning til certificate.
  Dato hentes fra xxxxxxy._55 og skrives i certificate.
  Input S/N ratio hentes fra xxxxxxy._55.
}
var
 DiffFlag : boolean;
 p        : byte;

begin
 DiffFlag := false;
 FileFound := true;

 Assign(F,CFileName);
 {$I-}
 Reset(F);     { Read from file  S:\MEASDATA\5655\xxxxxxy._55 }
 {$I+}
 if (IOResult <> 0) then
  begin
   writeln(Bell,'File "',CFileName,'" does not exist !!');
   writeln('Call for help !!');
   FileFound := false;
   Exit;
  end;

 if AppVer = PM5655G then
 while not Eof(F) do
  begin
   readln(F,MeasStr);
     if (pos('PROGRAM IN/OUT difference',MeasStr) > 0) then
      DiffFlag := true;

     p := pos('Date: ',MeasStr);  { get date to certificate }
     if p > 0 then
      DateStr := copy(MeasStr,p+6,6);

     p := pos('KU',MeasStr);  { get KU number to certificate }
     if p > 0 then
      KUNo := copy(MeasStr,p+2,6);

     if pos('Input source S/N ratio',MeasStr) > 0 then
       ReadInputSNRatio(copy(MeasStr,30,8))
      else
     if pos('Luminance Bar Level',MeasStr) = 5 then
       ReadCData(DiffFlag,1,copy(MeasStr,32,12))
      else
     if pos('Gain',MeasStr) = 5 then
       ReadCData(DiffFlag,71,copy(MeasStr,32,12))
      else
     if pos('Bar Tilt (Rec 569)',MeasStr) = 5 then
       ReadCData(DiffFlag,5,copy(MeasStr,29,12))
      else
     if pos('Sync/Bar (Rel 3/7)',MeasStr) = 5 then
       ReadCData(DiffFlag,9,copy(MeasStr,29,12))
      else
     if pos('Pulse/Bar Ratio',MeasStr) = 5 then
       ReadCData(DiffFlag,11,copy(MeasStr,29,12))
      else
     if pos('2T Pulse K-Factor',MeasStr) = 5 then
       ReadCData(DiffFlag,12,copy(MeasStr,29,12))
      else
     if pos('Chrom/Lum Gain',MeasStr) = 5 then
       ReadCData(DiffFlag,13,copy(MeasStr,29,12))
      else
     if pos('Chrom/Lum Delay',MeasStr) = 5 then
       ReadCData(DiffFlag,14,copy(MeasStr,29,12))
      else
     if pos('Lum Non Lin',MeasStr) = 5 then
       ReadCData(DiffFlag,16,copy(MeasStr,29,12))
      else
     if pos('Diff. Gain',MeasStr) = 5 then
       ReadCData(DiffFlag,18,copy(MeasStr,29,12))
      else
     if pos('Diff. Phase',MeasStr) = 5 then
       ReadCData(DiffFlag,20,copy(MeasStr,29,12))
      else
     if pos('Sync Amplitude',MeasStr) = 5 then
       ReadCData(DiffFlag,23,copy(MeasStr,29,12))
      else
     if pos('Sync to Burst Start',MeasStr) = 5 then
       ReadCData(DiffFlag,26,copy(MeasStr,29,12))
      else
     if (pos('Burst Duration',MeasStr) = 5) and
        (pos('æs',MeasStr) > 15) then
       ReadCData(DiffFlag,27,copy(MeasStr,29,12))
      else
     if pos('Burst Amplitude',MeasStr) = 5 then
       ReadCData(DiffFlag,29,copy(MeasStr,29,12))
      else
     if pos('Sc-H Phase',MeasStr) = 5 then
       ReadCData(DiffFlag,33,copy(MeasStr,29,12))
      else
     if pos('Sync Duration',MeasStr) = 5 then
       ReadCData(DiffFlag,34,copy(MeasStr,29,12))
      else
     if pos('Sync Rise Time',MeasStr) = 5 then
       ReadCData(DiffFlag,35,copy(MeasStr,29,12))
      else
     if pos('Sync Fall Time',MeasStr) = 5 then
       ReadCData(DiffFlag,36,copy(MeasStr,29,12))
      else
     if pos('Broad Pulse Sep',MeasStr) = 5 then
       ReadCData(DiffFlag,39,copy(MeasStr,29,12))
      else
     if pos('Equalizing Pulse',MeasStr) = 5 then
       ReadCData(DiffFlag,40,copy(MeasStr,29,12))
      else
     if pos('Freq 0.5',MeasStr) > 0 then
       ReadCData(DiffFlag,43,copy(MeasStr,29,12))
      else
     if pos('Freq 1.0',MeasStr) > 0 then
       ReadCData(DiffFlag,44,copy(MeasStr,29,12))
      else
     if pos('Freq 2.0',MeasStr) > 0 then
       ReadCData(DiffFlag,45,copy(MeasStr,29,12))
      else
     if pos('Freq 4.0',MeasStr) > 0 then
       ReadCData(DiffFlag,46,copy(MeasStr,29,12))
      else
     if pos('Freq 4.8',MeasStr) > 0 then
       ReadCData(DiffFlag,47,copy(MeasStr,29,12))
      else
     if pos('Freq 5.8',MeasStr) > 0 then
       ReadCData(DiffFlag,48,copy(MeasStr,29,12))
      else
     if pos('Random HF noise-wgtd (567)',MeasStr) = 5 then
       ReadCData(DiffFlag,59,copy(MeasStr,33,12))
      else

{ ----------  Absolutte G-m†linger indtastet af brugeren   ----------- }
     if pos('Subcarrier Freq',MeasStr) = 5 then
       ReadCData(DiffFlag,66,copy(MeasStr,29,12))
      else
     if pos('Return loss, PROGRAM IN',MeasStr) = 5 then
       ReadCData(DiffFlag,70,copy(MeasStr,29,12))
      else
     if pos('Bar over/undershoot',MeasStr) = 5 then
       ReadCData(DiffFlag,67,copy(MeasStr,29,12))
      else

{ ----------  Diff G-m†linger indtastet af brugeren   ----------- }
     if pos('Delay diff, NORM',MeasStr) = 5 then
       ReadCData(DiffFlag,68,copy(MeasStr,29,12));
  end; { while }




 if AppVer = PM5655M then
 while not Eof(F) do
  begin
   readln(F,MeasStr);
     if (pos('PROGRAM IN/OUT difference',MeasStr) > 0) then
      DiffFlag := true;

     p := pos('Date: ',MeasStr);  { get date to certificate }
     if p > 0 then
      DateStr := copy(MeasStr,p+6,6);

     p := pos('KU',MeasStr);  { get KU number to certificate }
     if p > 0 then
      KUNo := copy(MeasStr,p+2,6);

     if pos('Input source S/N ratio',MeasStr) > 0 then
       ReadInputSNRatio(copy(MeasStr,30,8))
      else
     if pos('Luminance Bar Level',MeasStr) = 5 then
       ReadCData(DiffFlag,1,copy(MeasStr,32,12))
      else
     if pos('Gain',MeasStr) = 5 then
       ReadCData(DiffFlag,71,copy(MeasStr,32,12))
      else
     if pos('Bar Tilt (Rec 569)',MeasStr) = 5 then
       ReadCData(DiffFlag,5,copy(MeasStr,29,12))
      else
     if pos('Pulse/Bar Ratio',MeasStr) = 5 then
       ReadCData(DiffFlag,11,copy(MeasStr,29,12))
      else
     if pos('2T Pulse K-Factor',MeasStr) = 5 then
       ReadCData(DiffFlag,12,copy(MeasStr,29,12))
      else
     if pos('Chrom/Lum Gain',MeasStr) = 5 then
       ReadCData(DiffFlag,13,copy(MeasStr,29,12))
      else
     if pos('Chrom/Lum Delay',MeasStr) = 5 then
       ReadCData(DiffFlag,14,copy(MeasStr,29,12))
      else
     if pos('Lum Non Linearity',MeasStr) = 5 then
       ReadCData(DiffFlag,16,copy(MeasStr,29,12))
      else
     if pos('Diff. Gain',MeasStr) = 5 then
       ReadCData(DiffFlag,18,copy(MeasStr,29,12))
      else
     if pos('Diff. Phase',MeasStr) = 5 then
       ReadCData(DiffFlag,20,copy(MeasStr,29,12))
      else
     if pos('Chr/Lum Intermod.',MeasStr) = 5 then
       ReadCData(DiffFlag,22,copy(MeasStr,29,12))
      else
     if pos('Sync Amplitude',MeasStr) = 5 then
       ReadCData(DiffFlag,23,copy(MeasStr,29,12))
      else
     if pos('Sync to Burst Start',MeasStr) = 5 then
       ReadCData(DiffFlag,26,copy(MeasStr,29,12))
      else
     if (pos('Burst Duration',MeasStr) = 5) and
        (pos('cyc',MeasStr) > 15) then
       ReadCData(DiffFlag,28,copy(MeasStr,29,12))
      else
     if pos('Burst Amplitude',MeasStr) = 5 then
       ReadCData(DiffFlag,29,copy(MeasStr,29,12))
      else
     if pos('Sc-H Phase',MeasStr) = 5 then
       ReadCData(DiffFlag,33,copy(MeasStr,29,12))
      else
     if pos('Sync Duration',MeasStr) = 5 then
       ReadCData(DiffFlag,34,copy(MeasStr,29,12))
      else
     if pos('Sync Risetime',MeasStr) = 5 then
       ReadCData(DiffFlag,35,copy(MeasStr,29,12))
      else
     if pos('Sync Falltime',MeasStr) = 5 then
       ReadCData(DiffFlag,36,copy(MeasStr,29,12))
      else
     if pos('Vertical Serration',MeasStr) = 5 then
       ReadCData(DiffFlag,39,copy(MeasStr,29,12))
      else
     if pos('Equalizing Pulse',MeasStr) = 5 then
       ReadCData(DiffFlag,40,copy(MeasStr,29,12))
      else
     if pos('NTC7 Multiburst Flag',MeasStr) = 5 then
       ReadCData(DiffFlag,41,copy(MeasStr,29,12))
      else
     if pos('Freq 0.5',MeasStr) > 0 then
       ReadCData(DiffFlag,43,copy(MeasStr,29,12))
      else
     if pos('Freq 1.0',MeasStr) > 0 then
       ReadCData(DiffFlag,44,copy(MeasStr,29,12))
      else
     if pos('Freq 2.0',MeasStr) > 0 then
       ReadCData(DiffFlag,45,copy(MeasStr,29,12))
      else
     if pos('Freq 3.0',MeasStr) > 0 then
       ReadCData(DiffFlag,46,copy(MeasStr,29,12))
      else
     if pos('Freq 3.58',MeasStr) > 0 then
       ReadCData(DiffFlag,47,copy(MeasStr,29,12))
      else
     if pos('Freq 4.2',MeasStr) > 0 then
       ReadCData(DiffFlag,48,copy(MeasStr,29,12))
      else
     if pos('S/N Ratio Unweighted',MeasStr) = 5 then
       ReadCData(DiffFlag,58,copy(MeasStr,33,12))
      else
     if pos('Field Time Dist.',MeasStr) = 5 then
       ReadCData(DiffFlag,65,copy(MeasStr,33,12))
      else

{ ----------  Absolutte M-m†linger indtastet af brugeren   ----------- }
     if pos('Subcarrier Freq',MeasStr) = 5 then
       ReadCData(DiffFlag,66,copy(MeasStr,29,12))
      else
     if pos('Return loss, PROGRAM IN',MeasStr) = 5 then
       ReadCData(DiffFlag,70,copy(MeasStr,29,12))
      else
     if pos('Bar over/undershoot',MeasStr) = 5 then
       ReadCData(DiffFlag,67,copy(MeasStr,29,12))
      else

{ ----------  Diff M-m†linger indtastet af brugeren   ----------- }
     if pos('Delay diff, NORM',MeasStr) = 5 then
       ReadCData(DiffFlag,68,copy(MeasStr,29,12));
  end; { while }


 {$I-} Close(F); {$I+}

 writeln('Data from file ',CFileName,' read in');
end; { ReadDataForCertif }




Procedure ComparePROG_IN_OUT;
(*
 PROGRAM IN/OUT diff m†linger:
 Ref m†linger og absolute m†linger p† henholdsvis PROG IN og PROG OUT
 sammenlignes med tolerancerne, som er specificeret i konstanten TD[[],3].
 De udvalgte diff m†linger er specificeret i konstanten DiffMeas[]
*)
var
 n,
 sp1,sp2   : byte;
 DiffValue : real;
begin

 ReadDiffData(RefMeas);

 ReadDiffData(PROGOUTMeas);


 Assign(MeasFile,MeasFileName);
 {Assign(MeasFile,'con');   }        { kun for test }
 Append(MeasFile);                   { tilf›j til filen   new_meas._55 }

 writeln(MeasFile,'    PROGRAM IN/OUT difference measurements:');
 write(MeasFile,'    ');
 write(MeasFile,'Parameter                     Diff value       Max diff           Status');
 writeln(MeasFile);

 for n := 1 to NoOfDiffMeas do
  begin
   if DiffMeas[n] = 71 then      { gain PROG IN/OUT }
    begin
     if Meas[DiffMeas[n]].Data <> 0 then
       DiffValue := Meas[DiffMeas[n]].RefData / Meas[DiffMeas[n]].Data
      else
       DiffValue := 5;
     Meas[DiffMeas[n]].DiffOK := Abs((1-DiffValue)*100) <= TD[DiffMeas[n],3];
     sp1 := 30-length(MeasParam[DiffMeas[n]]);
     sp2 := 4-length(MeasUnits[DiffMeas[n]]);
     write(MeasFile,'    ',MeasParam[DiffMeas[n]],' ':sp1,DiffValue:8:3);
     write(MeasFile,MeasUnits[DiffMeas[n]],' ':sp2);
     write(MeasFile,'  ',TD[DiffMeas[n],3]:6:0,' %');
     if Meas[DiffMeas[n]].DiffOK then
       writeln(MeasFile,'                OK')
      else
       writeln(MeasFile,'                FAIL');
    end     { if = [71] }
    else
    begin
     DiffValue := Meas[DiffMeas[n]].RefData - Meas[DiffMeas[n]].Data;

     Meas[DiffMeas[n]].DiffOK := Abs(DiffValue) <= TD[DiffMeas[n],3];

     if DiffMeas[n] = 20 then     { diff phase m†linger #20 }
      begin
      write('DP:in=',Meas[DiffMeas[n]].RefData:6:2);
      write('  DP:out=',Meas[DiffMeas[n]].Data:6:2);
      write('    DP:in-out=',DiffValue:6:2);
      writeln('   ',Meas[DiffMeas[n]].DiffOK);
      end;

     if DiffMeas[n] in [58,59] then    { S/N measurements }
      Meas[DiffMeas[n]].DiffOK := Meas[DiffMeas[n]].Data >= TD[DiffMeas[n],3];

     sp1 := 30-length(MeasParam[DiffMeas[n]]);
     sp2 := 4-length(MeasUnits[DiffMeas[n]]);

     case DiffMeas[n] of              { S/N   measurements }
      58 : write(MeasFile,'    ',MeasParam[DiffMeas[n]],'  *)',Meas[DiffMeas[n]].Data:12:1,' ');
      59 : write(MeasFile,'    ',MeasParam[DiffMeas[n]],'  *)',Meas[DiffMeas[n]].Data:6:1,' ');
      else
           write(MeasFile,'    ',MeasParam[DiffMeas[n]],' ':sp1,DiffValue:6:1,' ');
     end; { case }

     write(MeasFile,MeasUnits[DiffMeas[n]],' ':sp2,'  ');    { diff value }
     write(MeasFile,'    ',TD[DiffMeas[n],3]:6:2);           { tech data diff value }

     if Meas[DiffMeas[n]].DiffOK then
       writeln(MeasFile,'               OK')
      else
       writeln(MeasFile,'               FAILED');
    end;     { if }

   if not Meas[DiffMeas[n]].DiffOK then
    VM700Test := false;

  end; { for }

 writeln(MeasFile);
 writeln(MeasFile,'    *) Input source S/N ratio:',Meas[DiffMeas[15]].RefData:5:1,' ñ 2 dB');
 write(MeasFile,'    Signed ___________________');
 Close(MeasFile);

 writeln('PROG IN/OUT differential measurements added to file ''',MeasFileName,'''');
end; { ComparePROG_IN_OUT }





Procedure MakeHeader(PageNo : byte);
begin
 SetFont(Univers18);
 MoveToXY(LeftMargin,10);
 write(LJ2File,'Philips TV Test Equipment A/S');
 HorLine(LeftMargin,11,0.7,170);
 SetFont(Univers14);
 MoveToXY(LeftMargin,20);
 write(LJ2File,Apparat[AppVer],'  VITS Generator & Inserter');
 SetFont(Times12);
 MoveToXY(LeftMargin,25);
 write(LJ2File,'Serial number: KU',KuNo);
 MoveToXY(LeftMargin,30);
 write(LJ2File,'Certificate number: ',CertifNo);
 MoveToXY(LeftMargin,35);
 write(LJ2File,'Date: ',DateStr);
 MoveToXY(LeftMargin,270);
 write(LJ2file,'Signed ______________       Date ______________');
 MoveToXY(LeftMargin+155,270);
 write(LJ2file,'Page ',PageNo,'/3');
 SetFont(Times14Bold);
 MoveToXY(PaperCol1,62);
 write(LJ2File,'Parameter');
 MoveToXY(PaperCol2,62);
 write(LJ2File,'Product data');
 MoveToXY(PaperCol3,62);
 write(LJ2File,'Obtained value');
 HorLine(LeftMargin,57,0.4,170);
 HorLine(LeftMargin,64,0.4,170);
 VerLine(PaperCol2-5,57,0.4,195);
 VerLine(PaperCol3-5,57,0.4,195);
end;  { MakeHeader }


Procedure MakeCertif;
var
 PickPtr  : _PickPtr;
 RetPath  : _Path;
 KeySeq   : _KeySeq;
 i,n : byte;
begin
 EnterCertifNo;
 Check    := __TitleWin(PickWin,' '+PathName+'*'+NameExt+' certificate ',
                        White,PickBack,_TopCenter);
 PickPtr := __MakeSel(PickWin,White,Green,nil,0,0,0,1,1,nil,false);
 RetPath := __DirSel(PickPtr,_MREMOVE_MNU,_DOS_FMT_SEL,@LessName,
                       PathName+'*'+NameExt,AnyFile,false,KeySeq);
 Check := __Zapsel(PickPtr);  { release pick memory }

 __COffScn(false);                      { cursor=on }

 if KeySeq._Ch <> _KeyTable[_Key_Enter]._Ch then
  exit;

 ReadDataForCertif(RetPath);

 Assign(LJ2File,CertifName);
{   Assign(LJ2File,'con');  }     { kun for test af udskrift p† sk‘rm }
 Rewrite(LJ2File);

 ResetPrinter;

 MakeHeader(1);
 SetFont(Times12);
 MoveToXY(LeftMargin,48);
 write(LJ2File,'FULL FIELD, PROGRAM OUT: Measured in absolute values');
 MoveToXY(LeftMargin,53);
 write(LJ2File,'Same performance on TEST OUT connector');

 SetFont(Times12);
 { ----------------  Absolute measurements ------------------------}
 for n := 1 to 23 do                       { meas parameters 1-23 }
  begin
   MoveToXY(PaperCol1,YStartPos + (n-1)*5.5);
   write(LJ2File,MeasParam[CerMeas[n]]);
  end;

 MoveToXY(PaperCol1,YStartPos + (24-1)*5.5);
 if AppVer = PM5655G then
  write(LJ2File,'Freq Response:')
 else
  write(LJ2File,'Freq Response (NTC7):');

 for n := 24 to NoOfCerMeas do            { meas parameters 24-> }
  begin
   MoveToXY(PaperCol1,YStartPos + (n+1)*5.5);
   write(LJ2File,MeasParam[CerMeas[n]]);
  end;



 for n := 1 to 23 do         { product data 1-23 }
  begin
   MoveToXY(PaperCol2,YStartPos + (n-1)*5.5);
   for i := 1 to length(PD55[CerMeas[n]]) do
    begin
     if PD55[CerMeas[n]][i] = 'ó' then
      begin                      { 'ó' findes ikke i symbols‘t 7J (Times12) }
       SetFont(Courier10);
       write(LJ2File,PD55[CerMeas[n]][i]);
       SetFont(Times12);
      end
      else
       write(LJ2File,PD55[CerMeas[n]][i]);
    end; { for i }
  end; { for n }

 MoveToXY(PaperCol2,YStartPos + (24-1)*5.5);
 write(LJ2File,'Á1% up to 6 MHz');
 MoveToXY(PaperCol2,YStartPos + (25-1)*5.5);
 write(LJ2File,'+1 to -5% from 6-10 MHz');

 for n := 24 to NoOfCerMeas do            { product data 24-> }
  begin
   MoveToXY(PaperCol2,YStartPos + (n+1)*5.5);
   write(LJ2File,PD55[CerMeas[n]]);
  end;


 SetFont(Courier10);
 for n := 1 to 23 do         { obtained values 1-23 }
  begin
   MoveToXY(PaperCol3,YStartPos + (n-1)*5.5);
   write(LJ2File,Meas[CerMeas[n]].Data:9:1,' ',MeasUnits[CerMeas[n]]);
  end;

 for n := 24 to NoOfCerMeas do            { obtained values 24-> }
  begin
   MoveToXY(PaperCol3,YStartPos + (n+1)*5.5);
   write(LJ2File,Meas[CerMeas[n]].Data:9:1,' ',MeasUnits[CerMeas[n]]);
  end;

 EjectPage;


 { ----------------  Difference measurements ------------------------}
 MakeHeader(2);

 SetFont(Times12);
 MoveToXY(LeftMargin,53);
 write(LJ2File,'Measured differentially between PROGRAM IN and PROGRAM OUT');

 for n := 1 to 7 do     { meas parameters 1-7 }
  begin
   MoveToXY(PaperCol1,YStartPos + (n-1)*5.5);
   write(LJ2File,MeasParam[CerDiffMeas[n]]);
  end;

 MoveToXY(PaperCol1,YStartPos + (8-1)*5.5);
 write(LJ2File,'Freq Response:');

 for n := 8 to NoOfCerDiffMeas do     { meas parameters 8-> }
  begin
   MoveToXY(PaperCol1,YStartPos + (n)*5.5);
   write(LJ2File,MeasParam[CerDiffMeas[n]]);
   if CerDiffMeas[n] in [58,59] then    { S/N Ratio    Random HF noise }
    begin
     SetFont(Times8);
     write(LJ2File,' *)');
     SetFont(Times12);
    end;
  end;


 MoveToXY(PaperCol1,YStartPos + (20)*5.5);
 SetFont(Times8);
 write(LJ2File,'*) Input source with S/N ratio of',InputSNRatio:5:1,' Á 2 dB');
 SetFont(Times12);


 for n := 1 to 7 do              { product data 1-7 }
  begin
   MoveToXY(PaperCol2,YStartPos + (n-1)*5.5);
   for i := 1 to length(PD55_Diff[CerDiffMeas[n]]) do
    begin
     if PD55_Diff[CerDiffMeas[n]][i] = 'ó' then
      begin                      { 'ó' findes ikke i symbol 7J (Times12) }
       SetFont(Courier10);
       write(LJ2File,PD55_Diff[CerDiffMeas[n]][i]);
       SetFont(Times12);
      end
      else
      write(LJ2File,PD55_Diff[CerDiffMeas[n]][i]);
    end; { for i }
  end; { for n }

 for n := 8 to NoOfCerDiffMeas do     { product data 8-> }
  begin
   MoveToXY(PaperCol2,YStartPos + (n)*5.5);
   for i := 1 to length(PD55_Diff[CerDiffMeas[n]]) do
    begin
     if PD55_Diff[CerDiffMeas[n]][i] = 'ó' then
      begin                      { 'ó' findes ikke i symbol 7J (Times12) }
       SetFont(Courier10);
       write(LJ2File,PD55_Diff[CerDiffMeas[n]][i]);
       SetFont(Times12);
      end
      else
      write(LJ2File,PD55_Diff[CerDiffMeas[n]][i]);
    end; { for i }
  end; { for n }



 SetFont(Courier10);
 for n := 1 to 7 do                   { obtained values 1-7 }
  begin
   MoveToXY(PaperCol3,YStartPos + (n-1)*5.5);
   if CerDiffMeas[n] = 71 then            { gain }
     write(LJ2File,'  ',Meas[CerDiffMeas[n]].DiffData:6:3)
    else
     write(LJ2File,Meas[CerDiffMeas[n]].DiffData:6:1,' ',MeasUnits[CerDiffMeas[n]])
  end;


 for n := 8 to NoOfCerDiffMeas do     { obtained values 8-> }
  begin
   MoveToXY(PaperCol3,YStartPos + (n)*5.5);
   write(LJ2File,Meas[CerDiffMeas[n]].DiffData:6:1,' ',MeasUnits[CerDiffMeas[n]])
  end;

 EjectPage;


 { ----------------  Instrumentliste ----------------------------------------}
 SetFont(Univers18);
 MoveToXY(LeftMargin,10);
 write(LJ2File,'Philips TV Test Equipment A/S');
 HorLine(LeftMargin,11,0.7,170);
 SetFont(Univers14);
 MoveToXY(LeftMargin,20);
 write(LJ2File,Apparat[AppVer],'  VITS Generator & Inserter');
 SetFont(Times12);
 MoveToXY(LeftMargin,25);
 write(LJ2File,'Serial number: KU',KuNo);
 MoveToXY(LeftMargin,30);
 write(LJ2File,'Certificate number: ',CertifNo);
 MoveToXY(LeftMargin,35);
 write(LJ2File,'Date: ',DateStr);
 MoveToXY(LeftMargin,270);
 write(LJ2file,'Signed ______________       Date ______________');
 MoveToXY(LeftMargin+155,270);
 write(LJ2file,'Page 3/3');
 SetFont(Times14Bold);
 MoveToXY(PaperCol1,62);
 write(LJ2File,'Instrument');
 MoveToXY(PaperCol2+10,62);
 write(LJ2File,'Reference Number');
 HorLine(LeftMargin,57,0.4,170);
 HorLine(LeftMargin,64,0.4,170);

 SetFont(Times12);
 MoveToXY(LeftMargin,53);
 write(LJ2File,'Equipment used for the calibration');
 for n := 1 to 12 do
  begin
   MoveToXY(PaperCol1,71+(n-1)*5.5);        { instrument }
   write(LJ2File,InstrumentList[n,1]);
   MoveToXY(PaperCol2+10,71+(n-1)*5.5);     { ref no ( inventar no)}
   write(LJ2File,InstrumentList[n,2]);
  end;



 EjectPage;




 Close(LJ2File);

end; { MakeCertif }



Procedure ViewFile(ViewFileName : _Path);
const MaxRows = 150;    { max antal linier i virtuel sk‘rm }
var
 MF       : file;
 FileBuf  : array[1..6000] of char;
 VScnPtr  : _VScnPtr;
 n,
 NumRows  : byte;
 NumRead,
 NumChars : word;
 ViewEnd  : boolean;
begin
 Check    := __TitleWin(ViewWin,' File: '+ViewFileName+' ',
                                   Red,ViewBack,_TopCenter);
 Assign(MF,ViewFileName);
 {$I-} Reset(MF,1); {$I+}
 if IOResult <> 0 then exit;
 BlockRead(MF,FileBuf,sizeof(Filebuf),NumRead);
 Close(MF);
 VScnPtr := __TxBufScn(@FileBuf,NumRead,80,MaxRows,Black,LightGray,
                       _VCHARATTR_SCN,NumRows,NumChars);
 ViewEnd := false;
 Check := __DispWin(ViewWin);
 __COffScn(true);
 __ScrlScn(2,2,_CurColumns-1,_CurRows-1,VScnPtr,_Home,NumRows,0,0);
 repeat
  case scancode of
   Home   : __ScrlScn(2,2,_CurColumns-1,_CurRows-1,VScnPtr,_Home,NumRows,0,0);
   Up     : __ScrlScn(2,2,_CurColumns-1,_CurRows-1,VScnPtr,_ScrollDown,NumRows,0,0);
   Down   : __ScrlScn(2,2,_CurColumns-1,_CurRows-1,VScnPtr,_ScrollUp,NumRows,0,0);
   PgUp   : for n := 1 to 23 do
             __ScrlScn(2,2,_CurColumns-1,_CurRows-1,VScnPtr,_ScrollDown,NumRows,0,0);
   PgDn   : for n := 1 to 23 do
             __ScrlScn(2,2,_CurColumns-1,_CurRows-1,VScnPtr,_ScrollUp,NumRows,0,0);
   Escape : ViewEnd := true;
  end; { case }
 until ViewEnd;

 __ZapVScn(VScnPtr);  { release virtuel crt memory }
 Check := __RemWin;

end; { ViewFile }




Procedure DisplaySavedMeasFile;
var
 PickPtr  : _PickPtr;
 RetPath  : _Path;
 KeySeq   : _KeySeq;
begin
 Check    := __TitleWin(PickWin,' View file: '+PathName+'*'+NameExt+' ',
                        White,PickBack,_TopCenter);
 PickPtr := __MakeSel(PickWin,White,Green,nil,0,0,0,1,1,nil,false);
 RetPath := __DirSel(PickPtr,_MREMOVE_MNU,_DOS_FMT_SEL,@LessName,
                       PathName+'*'+NameExt,AnyFile,false,KeySeq);
 Check := __Zapsel(PickPtr);                     { release pick memory }

 if KeySeq._Ch = _KeyTable[_Key_Enter]._Ch then
  ViewFile(RetPath);

 __COffScn(false);

end; { DisplayMeasFile }


Procedure DisplayFile;
var
 PickPtr  : _PickPtr;
 RetPath  : _Path;
 KeySeq   : _KeySeq;
begin
 Check    := __TitleWin(PickWin,' View file: '+'*.* ',
                        White,PickBack,_TopCenter);
 PickPtr := __MakeSel(PickWin,White,Green,nil,0,0,0,1,1,nil,false);
 RetPath := __DirSel(PickPtr,_MREMOVE_MNU,_DOS_FMT_SEL,@LessName,
                       '*.*',AnyFile,false,KeySeq);
 Check := __Zapsel(PickPtr);                     { release pick memory }

 if KeySeq._Ch = _KeyTable[_Key_Enter]._Ch then
  ViewFile(RetPath);

 __COffScn(false);

end; { DisplayMeasFile }



Procedure SaveMeasFileToLAN;
var
 ExErrCode    : word;
 PNo          : byte;
 SaveError,
 TooManyFiles : boolean;
 FileCopied   : boolean;
 DirInfo      : SearchRec;
begin
 FileCopied := false;
 Check := __DispWin(SaveWin);
 SavePtr := nil;
 Writeln(' Report is OK');
 Writeln(' Choose:');
 __FlushKey;
 SaveError := false;  { no error }
 repeat
   __COffScn(true);
   if __DispMnu(SaveMnu) then
    SavePtr := __ReadMnu(SaveMnu,SavePtr,_MBEEP_UNKNOWN_MNU + _MKEEP_HIGHLIGHT_MNU,Key);

   if SavePtr <> nil then
    case SavePtr^._Row of
     4 : ViewFile(MeasFileName);
     5 : begin  { save file to LAN }
          PNo := 64;                     { first measfile = xxxxxxA }
          repeat
           Inc(PNo);
           AutoFileName := PathName+KUNo+Chr(Pno)+NameExt;
           FindFirst(AutoFileName,anyfile,DirInfo);
           FileFound := DosError = 0;
           TooManyFiles := Pno > Ord('Z');
          until (not FileFound) or TooManyFiles;
          Check := __RemWin;

          if TooManyFiles then
           begin
            write(Bell);
            writeln('WARNING:  Too many data files.');
            write(  '          Can only create data files from ');
            writeln('xxxxxxA',NameExt,' -> xxxxxxZ',NameExt);
            writeln;
            writeln('Stop measurements and call for help!');
            VM700Test := false;
            exit;
           end; { if }

           Assign(MeasFile,MeasFileName);
           {$I-} Reset(MeasFile); {$I+}
           IOErr := IOResult;
           if (not FileCopied) and (IOErr = 0) then
            begin
             if ((GetEnv('G_NAME') = 'MEASDATA') or (GetEnv('NAME') = 'PF') and
                (DiskFree(ord('S')-64) > 100000)) then    { check S disk drive > 100000 bytes}
                __WinExPgm('DOS','COPY '+MeasFileName+' '+AutoFileName+' > NUL',1,2,
                           _CurColumns,_CurRows,DosFore,DosBack,ExErrCode)
              else
               begin
                SaveError := true;
                AutoFileName := KUNo+Chr(Pno)+NameExt;       { lokalt drev }
                __WinExPgm('DOS','COPY '+MeasFileName+' '+AutoFileName+' > NUL',1,2,
                           _CurColumns,_CurRows,DosFore,DosBack,ExErrCode);
                write(Bell); delay(1000); write(Bell);
                writeln('WARNINNG: ',MeasFileName,' is not copied to LAN, but to C-drive.');
                writeln('          This is because:');
                writeln('           a) you are not logged to LAN.');
                writeln('           b) you are not a member of MEASDATA LAN-group.');
                writeln('           c) too little space on S drive.   (min. 100000 bytes).');
                writeln('          From DOS, type SET <ENTER> and check that G_NAME=MEASDATA.');
                writeln('          Contact LAN supervisor for membership of the MEASDATA group.');
                writeln('      >>> Take care that file ',AutoFileName,' is saved on LAN in');
                writeln('          the directory ',PathName);
                writeln;
               end; { if }

              case ExErrCode of
               0 : begin
                    writeln(MeasFileName,' copied to ',AutoFileName);
                    FileCopied := true;
                   end;
               8 : writeln(Bell,'Too little memory for copying to LAN');
               else
                writeln(Bell,'Problems during copying files to LAN!  (',ExErrCode,')');
                writeln('DOS error: ',DosError);
                writeln('DOS exit code: ',DosExitCode);
                writeln;
                writeln('Stop further measurements and call for help');
              end;  { case }
            end; { if }

           if IOErr <> 0 then
             writeln(Bell,'File ',MeasFileName,' does not exist !!');

         end;
     6 : begin
          Check := __RemWin;
          if not FileCopied then
           begin
            writeln(Bell,'WARNING: File ',MeasFileName,' with data is NOT copied to LAN, and will');
            writeln(     '          be overwritten at next measurement!!');
           end;
         end;
    end; { case }

 until ((SavePtr^._Row <> 4) and (SavePtr <> nil)) or SaveError;


end; { SaveMeasFileToLAN }



begin

 { unit initialization }

end.


