(**********************************************************************
   5639_94.PAS                                Code-nummer:  7108 100 xxxxx
   (c) ProTeleVision Technologies A/S.
   Peter Frederiksen, 920303

   Dette program bruges til at teste og kalibrere:
       PM5639/94  Small industrial color sensor with lens front

   CRT matrix genereres ud fra MX-m†linger.

   Programmet kommunikerer med PM5639 via COM1-porten. Porten er
   initialiseret til 4800 eller 9600 baud, 8 bit, 2 stopbit og ingen paritet.
   Dette g›res i proceduren InitPC_COM1Port.

   Korrekt SW-rev for color sensoren er i konstanten MeasHeadSWRev.

   Funktionstasterne bruges til at styre de enkelte test og kalibre-
   ringer, i den r‘kkef›lge man ›nsker.

   Tast F10 afvikler en totaltest i en forud bestemt r‘kkef›lge, og det
   kontrolleres at SP6-skrivebeskyttelsen = OFF inden totaltesten.

   I filen "Certiffile" skrives kalibreringsdata, som beskrevet
   i justeringsvejledningen. Filen er en ASCII-fil.
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
   (default I/O-addr = $300)

   N†r programmet starter op, kontrolleres om f›lgende enheder er for-
   bundet:
      Lyskasse
      RGB-generator
      PM2534 Digital multimeter  (IEEE adresse = 22)

   N†r programmet starter op, indl‘ses filen XYFileName med xy-koordinater
   samt luminance. Kan filen ikke findes, afbrydes programmet.

   Kan programmet ikke f† kontakt med enhederne, afbrydes programmet.
   NB: RGB-generator og lyskasse SKAL v‘re t‘ndt, n†r programmet startes.
       IýC-interface i PC'en kan 'h‘nge op' af uforklarlige †rsager - muligvis
       ligger fejlen i PCD8584-controlleren ??.
       Hvis det sker m† man slukke/t‘nde for PC'en.

  Beregning af diverse matricer: se .doc fil.

  Rettelser:
  930623           Kodenummer for 5639/90 ‘ndret til: 4008 109 79300
  930716           Intern ID for 5639/90 er ‘ndret til: 1 0 0 1
                   Intern ID for 5639/92 er u‘ndret.
  931104           Sporbar kalibrering ‘ndres fra NPL til NIST
                   I EýPROM adresse 96 skrives konstanten TraceFlag.
                   Indtil d.d. har der st†et 255 (= NPL).
                   Denne ‘ndring g‘lder ogs† 5639/90 sensorer med
                   12NC = 4008 109 75430. Se program hertil.
                   Kalibreringsudskrift viser sporbarhed.
  940622           Timeout ved learn ‘ndret til 2 sek.
                   Sensor SW rev g‘lder ogs† for 03.0.
                   Sensor /916 indf›res (9600 baud).
                   EýPROM addr 95 bestemmer output format:
                       <> 0      R,G,B,I
                        = 0      R,G,B
  941118           Kontrol af indtastning af KU-nummer:
                   2.ciffer skal v‘re serie nr.
                   Se afvisningsrapport SAL94012.

  950203           Sensor /90 serie IV og /92 serie II indf›res
  950210           PM5639/02 barco option indf›res.
  950221           Check for overflow ved offsetm†linger.
  950315           KU-nummer skal ikke ‘ndres ved kundeopdateringer.
                   Referencem†linger logges til file. Udskrives med
                   programmet PRINTLOG.
  950501           Kalibreringsdato skrives til EEPROM.
                   Inden InternID skrives til EEPROM kontrolleres, at
                   data i adresserne 58-61 er 255. Dette er indf›rt for
                   at undg†, at sensorer kan ‘ndres fra /90 til /92 eller
                   fra /92 til /90. Hvis der skal ‘ndres type, skal alle
                   adresserne ‘ndres til 255 inden test. ’ndringen foretages
                   med programmet COM5639.EXE.
  950616           L‘sning af K-matrix ved reference m†ling forbedret.
  951005           Udl‘sning af xy-koordinater ‘ndres fra 3 til 4
                   decimaler i proceduren xyYMeasure.
                   I filen SENSOR.LOG ‘ndres xy-koordinater fra 3 til 4
                   decimaler og Y ‘ndres fra 0 til 1 decimal ved EBU-m†linger.
  951013           Philips/Brgge SW-revision er rettet fra kun 3.0 til
                   b†de 3.0 og 3.2.
  960111           Kalibreringsdata gemmes p† LAN og udskrives til
                   laser printer.
  961011           PM5639/93 miniature CRT color sensor tilf›jes.
  970617           Kalibreringsdato flyttes fra addr 176-181 til
                   addr 187-192, idet usertekst i PM8550 starter p†
                   addr 176. Ved l‘sning af dato l‘ses f›rst fra
                   addr 176-181 og dato kontrolleres for korrekt
                   format. Hvis format er forkert l‘ses fra addr 187-192.
                   Se procedurerne ReadCalDate og WriteCalDate.
  970710           Test af baudrate skift i 5639/93. Se 'CheckSWBaudrateShift'
                   Baudrate kan kun skiftes, hvis intern ID addr 61 = 56-63.
                   Sensor software version rettes til 03.3
  970905           Der var fejl i sensor software version 03.3. Indstillet
                   baudrate huskes ikke ved power off/on. Version 03.3 udg†r.
                   Sensor software version rettes til 03.4
  980128           Firmanavn p† certifikater ‘ndret til ProTeleVision Technologies A/S
  980210           Diverse ‘ndringer ved m†linger:
                    Datofelt ‘ndres til ogs† at omfatte tid.
                    M†linger med ref.sensor p† EBU monitor tilf›jes.
                    Felterne adskilles med komma i stedet for |
                    Der laves m†ling med ref.sensor efter max 30 minutter
                   Ved kalibrering:
                    Sk‘rmen skal have v‘ret hvid i mindst 1 minut inden
                    kalibrering. Efter 15 minutter uden aktivitet slukkes
                    sk‘rmen.
  980515            Udskrift af kalibreringsdata fra EEPROM skal kun udskrives
                    efter sidste kalibrering. Brugeren skal derfor bekr‘fte
                    hver udskrivning med Ja/Nej.
  980528            SMPTE sk‘rm slukker efter kalibrering. Det m† den ikke.
                    Fejlen rettet.
                    Specialudgave til test af miniature sensor med linseforsats
                    K-matrix multiplikationsfaktor er ‘ndret fra 100000 til 10000.

 *)


Program P5639_94;
{$M 20000,0,120000}


{$DEFINE LYSKASSE}
{$DEFINE RGBGEN}
{$DEFINE IEEEBUS}


{$R+}    {Range check}
{$I+}    {I/O check}
{$S+}    {Stack check}
{$B-}    {Boolean short}

uses Crt,
     Dos,
     Printer,
     PepUtil,
     Matrix,
     ComUtil,
     Win,
     Timer,
     Unit_S39,
     Unit_Las,
     Unit_Mea,
     Unit_IIC;



{$I TPTEST.INC}
{$I UTIL92.INC}



Procedure ReadCRTMatrix(Flag : boolean);   {CtrlF1}
{L‘ser CRT-matricer fra EýPROM, gemmer v‘rdierne i en variabel og viser dem
 p† sk‘rmen. Hver matrice best†r af 12 bytes.
 Kun matricer, som har et navn, l‘ses}
var  n,k,
     Value   : byte;
     Svar    : STRING;
     Kode    : integer;
     AddrStr : STR_6;
     CRTNo,
     Addr    : WorD;
begin
 ReadNoCRTFosfor(Silent);
 if Flag then
  writeln('CRT-matrix:');
 ReadCRTMatrixOk := true;
 FillChar(CRTMatrix,SizeOf(CRTMatrix),0);
 CRTNo := 1;

 repeat
  n := 1;
  Addr := 1004 - (CRTNo * 20) + 20;
  Str(Addr,AddrStr);
  COMStatus := writeCOMPort(COMNo,'MA'+ AddrStr + ',');
  COMStatus := ClearInputQue(COMNo);
  if Length(CRTName[CRTNo]) > 0 then
   begin
    repeat
     COMStatus := writeCOMPort(COMNo,'RM,');
     COMStatus := ReadlnCOMPort(COMNo,Svar,1);
     Val(Svar,Value,Kode);
      if Kode = 0 then
       CRTMatrix[CRTNo,n] := Value
      else
       begin
        ReadCRTMatrixOk := false;
        TestFailed := true;
       end;
     Inc(n);
    until (n > 12) or (COMStatus <> 0) or (Kode <> 0) or (not ReadCRTMatrixOk);

    if Flag then
     begin
       write('#',CRTNo,': ');
       for k := 1 to n - 1 DO
        write(CRTMatrix[CRTNo,k],' ');
       writeln;
     end; {if flag}
    if (not ReadCRTMatrixOk) AND Flag then
     writeln('Fejl ved l‘sning af CRT-matrix #',CRTNo);
   end  {if Length(  )}
  else
   begin
    if Flag then
     writeln('#',CRTNo,': intet navn');
   end; {if Length(  )}

 if Flag AND (CRTNo = (NoCRTFosfor / 2)) then
   begin
    writeln(' Tryk en tast ');
    WaitForAnykey;
   end;

 Inc(CRTNo);
 until (CRTNo > NoCRTFosfor) or (COMStatus <> 0) or (Kode <> 0) or (not ReadCRTMatrixOk);


 if Flag then
  begin
{$R-}
   write('CRT #1:');GotoXY(30,WhereY); writeln('CRT #2:');

   write('8192':8);
   write(CRTMatrix[1,1] * 256 + CRTMatrix[1,2]:8);
   write(CRTMatrix[1,3] * 256 + CRTMatrix[1,4]:8);
   GotoXY(30,WhereY); write('8192':8);
   write(CRTMatrix[2,1] * 256 + CRTMatrix[2,2]:8);
   writeln(CRTMatrix[2,3] * 256 + CRTMatrix[2,4]:8);

   write(CRTMatrix[1,5] * 256 + CRTMatrix[1,6]:8);
   write('8192':8);
   write(CRTMatrix[1,7] * 256 + CRTMatrix[1,8]:8);
   GotoXY(30,WhereY); write(CRTMatrix[2,5] * 256 + CRTMatrix[2,6]:8);
   write('8192':8);
   writeln(CRTMatrix[2,7] * 256 + CRTMatrix[2,8]:8);

   write(CRTMatrix[1,9] * 256 + CRTMatrix[1,10]:8);
   write(CRTMatrix[1,11] * 256 + CRTMatrix[1,12]:8);
   write('8192':8);
   GotoXY(30,WhereY); write(CRTMatrix[2,9] * 256 + CRTMatrix[2,10]:8);
   write(CRTMatrix[2,11] * 256 + CRTMatrix[2,12]:8);
   writeln('8192':8);
{$R+}
  end;

end;  {ReadCRTMatrix}


Procedure ReadXYCrtMatrix(Flag : boolean);   {Tast F}
{L‘ser XY CRT-matrix fra EýPROM addr 200, gemmer v‘rdierne i en variabel og
 hvis Flag=true, viser dem p† sk‘rmen. Matricen best†r af 12 bytes.}
var  n,k,
     Value   : byte;
     Svar    : STRING;
     Kode    : integer;
     ReadxyPhOk : boolean;
     PhElement  : array[1..12] OF byte;
begin
 ReadxyPhOk := true;

 if Flag then
  writeln('XY CRT-matrix:');
  FillChar(XYCRTMatrix,SizeOf(XYCRTMatrix),0);
  COMStatus := writeCOMPort(COMNo,'MS,MS,');

  n := 1;
  COMStatus := writeCOMPort(COMNo,'MA200,');
  COMStatus := ClearInputQue(COMNo);
    repeat
     COMStatus := writeCOMPort(COMNo,'RM,'); Delay(5);
     COMStatus := ReadlnCOMPort(COMNo,Svar,1);
     Val(Svar,Value,Kode);
      if Kode = 0 then
        PhElement[n] := Value
       else
        ReadxyPhOk := false;
     Inc(n);
    until (n > 12) or (COMStatus <> 0) or (Kode <> 0) or (not ReadxyPhOk);

    if not ReadxyPhOk then
     Exit;

{$R-}
 XYCRTMatrix[1] := ((PhElement[1] * 256 + PhElement[2]))   / CRTMulFactor;
 XYCRTMatrix[2] := ((PhElement[3] * 256 + PhElement[4]))   / CRTMulFactor;
 XYCRTMatrix[3] := ((PhElement[5] * 256 + PhElement[6]))   / CRTMulFactor;
 XYCRTMatrix[4] := ((PhElement[7] * 256 + PhElement[8]))   / CRTMulFactor;
 XYCRTMatrix[5] := ((PhElement[9] * 256 + PhElement[10]))  / CRTMulFactor;
 XYCRTMatrix[6] := ((PhElement[11] * 256 + PhElement[12])) / CRTMulFactor;
{$R+}

    if Flag then
     begin
       for k := 1 to 12 DO
        write(PhElement[k]:5);
       writeln;
      write('1':9);
      write(XYCRTMatrix[1]:9:4);
      writeln(XYCRTMatrix[2]:9:4);
      write(XYCRTMatrix[3]:9:4);
      write('1':9);
      writeln(XYCRTMatrix[4]:9:4);
      write(XYCRTMatrix[5]:9:4);
      write(XYCRTMatrix[6]:9:4);
      writeln('1':9);
      writeln;
     end; {if flag}

    if (not ReadxyPhOk) AND Flag then
     writeln('Fejl ved l‘sning af XY CRT-matrix');
end;  {ReadXYCRTMatrix}



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
{   writeln('Free heap: ',memavail,'     Free block: ',maxavail); <<<<<<< }
   if (not FileCopied) and (IOErr = 0) then
    begin
     if (DiskFree(ord('M')-64) > 100000) then    { check S disk drive > 100000 bytes}
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
        writeln('           b) for lidt plads p† M drev.   (min. 100000 bytes).');
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
      Svar       : STRING;
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
 KU_No := ReadKU;
 delay(50);

 ReadCRTNames(Silent);
 write('.');
 ReadCRTMatrix(Silent);
 write('.');

 KalStr[1] := Marg + 'Serial no: ' +  Read12NC + '    KU' +  KU_No;
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
 Svar[0] := #156;
 COMStatus := writeCOMPort(COMNo,'MS,'); delay(20);
 COMStatus := writeCOMPort(COMNo,'MA2,'); delay(20);
 COMStatus := ClearInputQue(COMNo);
 repeat
  COMStatus := writeCOMPort(COMNo,'RM,'); delay(20);
  COMStatus := ReadlnCOMPort(COMNo,InStr,1);
  Val(Instr,Data,k);
  if (n mod 20) = 0 then
   write('.');
  Svar[n] := Chr(Data);
  Inc(n);
 until (n > 156-1) or (COMStatus <> 0) or (k <> 0);
 Svar[0] := Chr(n);
 writeln('.');

 if (n <= 156 - 1) then
  begin
   writeln(Bell,' L‘sefejl fra color sensor');
    if COMStatus <> 0 then
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
 for n := 9 to 156 - 1 do     {data fra addr 10 ...}
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
 for n := 1 to (156 DIV 10) do              {fra addr 10 ...}
  begin
   writeln(MeasFile,Marg,Addr:4,':  ',KalStr[n + 5]);
   Inc(Addr,10);
  end; {for}

 writeln(MeasFile,Marg,'Color sensor calibration is traceable to NIST, USA');

 writeln(MeasFile,Marg,'Calibrated by: ______________');

 write(MeasFile,FF);
 Close(MeasFile);


 WriteMeasFileToLAN;

end;  {ReadKalDataFromEEPROM}



Procedure WriteCertifFileToLaser(Flag : boolean);  { Alt F10 }
{ flag = true:  brugeren skal indtaste sensorens KU-nummer (evt incl version)
  flag = false: nyeste fil med data udskrives
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
      KU_Str := ReadKU;
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

    if (pos('400810979300',Instr[n]) > 0) then
     SVer := 90;

    if (pos('400810978930',Instr[n]) > 0) then
     SVer := 92;

    if (pos('400810983120',Instr[n]) > 0) then
     SVer := 93;

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
 case SVer of
  93 : begin
        MoveToXY(LeftMargin+43,42);
        write(FLas,'PM 5639/93 Miniature CRT Color Sensor');
       end;
  92 : begin
        MoveToXY(LeftMargin+43,42);
        write(FLas,'PM 5639/92 Industrial CRT Color Sensor');
       end;
  90 : begin
        MoveToXY(LeftMargin+57,42);
        write(FLas,'PM 5639/90 CRT Color Sensor');
       end;
  end; { case }

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

 for n := 8 to 23 do
  begin
   MoveToXY(LeftMargin,130+(5*(n-8)));
   write(FLas,InStr[n]);
  end;

 MoveToXY(LeftMargin,240);     { Color sensor calibration is traceable to NIST, USA }
 write(FLas,InStr[24]);

 MoveToXY(LeftMargin,255);     { calibrated by _________ }
 write(FLas,InStr[25],'        Date: ',DateStr);

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




Procedure UdregnMXCRTMatrix(StartAddr : WorD; CRTName : STR_16);
var
 Dimen : integer;          { Dimension of the square matrix }
 Data  : TNmatrix;          { The matrix }
 Inv   : TNMatrix;           { Inverse of Data}
 Error,                    { Errorflags }
 n        : byte;
 aa,rr1   : real;

Procedure writeCRTMatrixToEEPROM(StAddr : word);
 begin
  writeEEPROM(StAddr    ,Hi(Round(Trunc(Inv[1,2]))));
  writeEEPROM(StAddr + 1,Lo(Round(Trunc(Inv[1,2]))));
  writeEEPROM(StAddr + 2,Hi(Round(Trunc(Inv[1,3]))));
  writeEEPROM(StAddr + 3,Lo(Round(Trunc(Inv[1,3]))));
  writeEEPROM(StAddr + 4,Hi(Round(Trunc(Inv[2,1]))));
  writeEEPROM(StAddr + 5,Lo(Round(Trunc(Inv[2,1]))));
  writeEEPROM(StAddr + 6,Hi(Round(Trunc(Inv[2,3]))));
  writeEEPROM(StAddr + 7,Lo(Round(Trunc(Inv[2,3]))));
  writeEEPROM(StAddr + 8,Hi(Round(Trunc(Inv[3,1]))));
  writeEEPROM(StAddr + 9,Lo(Round(Trunc(Inv[3,1]))));
  writeEEPROM(StAddr + 10,Hi(Round(Trunc(Inv[3,2]))));
  writeEEPROM(StAddr + 11,Lo(Round(Trunc(Inv[3,2]))));
  writeln('MX type CRT-matrix ',CRTName,' skrevet til EýPROM');
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
 for n := 1 to 10 DO
  begin
   rr1 := Data[1,2] - (aa * Data[3,2]);
   aa  := Data[1,3] - (rr1 * Data[2,3]);
  end; {for}
 Inv[1,3] := (-1 * aa  * CRTMulFactor);
 Inv[1,2] := (-1 * rr1 * CRTMulFactor);

 {<<<<    Green  >>>>}
 aa := Data[2,3];
 for n := 1 to 10 DO
  begin
   rr1 := Data[2,1] - (aa * Data[3,1]);
   aa  := Data[2,3] - (rr1 * Data[1,3]);
  end; {for}
 Inv[2,3] := (-1 * aa  * CRTMulFactor);
 Inv[2,1] := (-1 * rr1 * CRTMulFactor);


 {<<<<    Blue  >>>>}
 aa := Data[3,2];
 for n := 1 to 10 DO
  begin
   rr1 := Data[3,1] - (aa * Data[2,1]);
   aa  := Data[3,2] - (rr1 * Data[1,2]);
  end; {for}
 Inv[3,2] := (-1 * aa  * CRTMulFactor);
 Inv[3,1] := (-1 * rr1 * CRTMulFactor);

 write(CRTMulFactor:10); write(Inv[1,2]:10:2); writeln(Inv[1,3]:10:2);
 write(Inv[2,1]:10:2); write(CRTMulFactor:10); writeln(Inv[2,3]:10:2);
 write(Inv[3,1]:10:2); write(Inv[3,2]:10:2); writeln(CRTMulFactor:10);

 if CRTName = 'EBU' then
  begin
  LearnEBUOk := (Inv[1,2] < Inv[1,3]) AND
                (Inv[2,1] < Inv[2,3]) AND
                (Inv[3,2] < Inv[3,1]);
  if LearnEBUOk then
    writeCRTMatrixToEEPROM(StartAddr)
   else
    writeln('MX type CRT-matrix IKKE beregnet korrekt');
 end;

 if CRTName = 'SMPTE C' then
  begin
  LearnSMPTE_COk := (Inv[1,2] < Inv[1,3]) AND
                (Inv[2,1] < Inv[2,3]) AND
                (Inv[3,2] < Inv[3,1]);
  if LearnSMPTE_COk then
    writeCRTMatrixToEEPROM(StartAddr)
   else
    writeln('MX type CRT-matrix IKKE beregnet korrekt');
 end;
end;  {UdregnMXCRTMatrix}



Procedure LearnKMatrix(SaveData : boolean);
{ Hvis SaveData = true, l‘ses kalibreringsdata efter kalibrering
  fra sensor EEPROM og gemmes p† disk
}
var  n,
     Status      : byte;
     Svar        : string;
     Dimen       : integer;      { Dimension of the square matrix }
     Coef        : TNmatrix;     { The matrix }
     Constants   : TNvector;     { Constant terms in the equations }
     Solution    : TNvector;     { Solution to the set of equations }
     Error       : byte;         { Flags if something went wrong }
     St          : boolean;
     MSxx        : MeasStrType;

Function ValidSolution : boolean;
{Kontrollerer om en af de 9 K-matrixelementer er absolut st›rre
 end 32000/KorrMulFactor.
 Inden matrixelementerne skrives i EýPROM, multipliceres de med KorrMulFactor.
 Hver element skal kunne indeholdes i en integer;
 }
var Valid : boolean;
    n     : byte;
    Lim   : REAL;
 begin
  Valid := true;
  Lim := 32000 / KorrMulFactor;
  for n := 1 to 9 DO
   if Abs(Solution[n]) > Lim then
    Valid := false;

  Valid := Valid AND
  ((Solution[1] > 0) AND
  (Solution[5] > 0) AND
  (Solution[9] > 0));

  ValidSolution := Valid;
 end;

begin
 EEPROMwriteError := false;

 if ReadKalStatus = 'USER' then
  begin
   writeln(Bell,'Color sensor er USER kalibreret - forts‘t ?    J/N');
   if TastJ <> ScanCode then
    begin
     writeln('Kalibrering afbrudt');
     Exit;
    end
  end;

 if CheckMonitorOnTime = false then
  exit;

 writeln;

 CRT625Lines;
 Beep(1000,200);
 LearnKMatrixOk := true;
 writeln('Placer sensoren p† EBU monitoren over det hvide felt  -  tryk en tast');
 WaitForAnyKey;
 COMStatus := writeCOMPort(COMNo,'MS,'); Delay(20);
 COMStatus := writeCOMPort(COMNo,'NR,'); Delay(20);
 COMStatus := writeCOMPort(COMNo,'MX,'); Delay(20);
 COMStatus := writeCOMPort(COMNo,'MC,'); Delay(20);
 write('M†ling: R .');
 EBURed;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,3);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,3);
  MeasStr[n] := Svar;
  write('.');
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   writeln('Timeout: ingen m†ling');
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnKMatrixOk := false;
   TestFailed := true;
   EBUWhite;
   Exit;
  end;
 CalculateMean(MeasStr,AntalMeas,XR,YR,ZR,St);

 write('        G .');
 EBUGreen;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,3);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,3);
  MeasStr[n] := Svar;
  write('.');
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnKMatrixOk := false;
   TestFailed := true;
   EBUWhite;
   Exit;
  end;
 CalculateMean(MeasStr,AntalMeas,XG,YG,ZG,St);

 write('        B .');
 EBUBlue;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,3);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,3);
  write('.');
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnKMatrixOk := false;
   TestFailed := true;
   EBUWhite;
   Exit;
  end;
 CalculateMean(MeasStr,AntalMeas,XB,YB,ZB,St);

 write('        W .');
 EBUWhite;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,3);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,3);
  write('.');
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnKMatrixOk := false;
   TestFailed := true;
   EBUWhite;
   Exit;
  end;

 COMStatus := writeCOMPort(COMNo,'MS,'); Delay(20);

 CalculateMean(MeasStr,AntalMeas,X65,Y65,Z65,St);

 write(' R›d:  ',XR:10:2);  write(YR:10:2);  writeln(ZR:10:2,'   ');
 write(' Gr›n: ',XG:10:2);  write(YG:10:2);  writeln(ZG:10:2,'   ');
 write(' Bl†:  ',XB:10:2);  write(YB:10:2);  writeln(ZB:10:2,'   ');
 write(' Hvid: ',X65:10:2); write(Y65:10:2); writeln(Z65:10:2,'   ');

 if X65 <> 0 then
   XFact := Y65 / X65
  else
   begin
    writeln('Error:  X65 = 0');
    LearnKMatrixOk := false;
    TestFailed := true;
    Exit;
   end;

 YFact := 1;

 if Z65 <> 0 then
   ZFact := Y65 / Z65
  else
   begin
    writeln('Z65 = 0');
    LearnKMatrixOk := false;
    TestFailed := true;
    Exit;
  end;

 writeln(' XFact:',XFact:6:3,'  YFact:',YFact:6:3,'  ZFact :',ZFact:6:3);

 Dimen := 12;                                {define 12 x 12 matrix}
 FillChar(Coef, SizeOf(Coef), 0);            {init 12 x 12 matrix}
 FillChar(Constants, SizeOf(Constants), 0);  {init constants}
 {R›d sk‘rm}                                       {SP-m†lte xyz CIE }
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

 write('K-matrix ');
 if (Error <> 0) or (not ValidSolution) then
   begin
    writeln('IKKE beregnet korrekt');
    writeln('Ingen CRT-matrix beregnet');
    LearnKMatrixOk := false;
    TestFailed := true;
   end
  else
   begin
    writeln('beregnet:');
    for n := 0 to 8 do
     begin
      Solution[n + 1] := Solution[n + 1] * KorrMulFactor;
      write(Solution[n + 1]:10:2);
       if (n + 1) mod 3 = 0 then writeln;
     end; {for}

    for n := 0 to 8 do
     begin
      KMatrwrited[n+1] := 256 * Hi(Trunc(Round(Solution[n + 1]))) +
                          Lo(Trunc(Round(Solution[n + 1])));
      writeEEPROM(KorrAddr + (n * 2),    Hi(Trunc(Round(Solution[n + 1]))));
      writeEEPROM(KorrAddr + (n * 2) + 1,Lo(Trunc(Round(Solution[n + 1]))));
     end; {for}

    writeEEPROM(PreGainAddr    ,Hi(Trunc(Round(XFact * PreGainMulFactor))));
    writeEEPROM(PreGainAddr + 1,Lo(Trunc(Round(XFact * PreGainMulFactor))));
    writeEEPROM(PreGainAddr + 2,Hi(Trunc(Round(YFact * PreGainMulFactor))));
    writeEEPROM(PreGainAddr + 3,Lo(Trunc(Round(YFact * PreGainMulFactor))));
    writeEEPROM(PreGainAddr + 4,Hi(Trunc(Round(ZFact * PreGainMulFactor))));
    writeEEPROM(PreGainAddr + 5,Lo(Trunc(Round(ZFact * PreGainMulFactor))));

    writeEEPROM(157,255);    {Cal status = FACTORY  (bruges i PM8550)}

    WriteCalDate;

(*
    ReadKMatrix(Silent);     { check at K-Matrix er skrevet korrekt til EEPROM}
    for n := 1 to 9 do
      if KMatr[n] <> KMatrwrited[n] then
       begin
        EEPROMwriteError := true;
        LearnKMatrixOk := false;
        TestFailed := true;
       end;

    if EEPROMwriteError then
      writeln(bell,'Data til EýPROM ikke skrevet korrekt')
     else
      writeln('K-matrix og PreGain skrevet til EýPROM,  Kal status = FACTORY');
 *)

   writeln('K-matrix og PreGain skrevet til EýPROM,  Kal status = FACTORY');
 end; {if}

 if LearnKMatrixOk then
  begin
   UdregnMXCRTMatrix(CRTMatrixAddr,'EBU');
   if SaveData = true then
    ReadKalDataFromEEPROM;
  end;
end; {LearnKMatrix}



Procedure LearnSMPTE_CFosfor;
const Timeout = 3;              { sec }
var  n,
     Error,                     { Flags if something went wrong }
     Status    : byte;
     Svar      : STRING;
     Dimen     : integer;       { Dimension of the square matrix }
     Coef      : TNmatrix;      { The matrix }
     Constants : TNvector;      { Constant terms in the equations }
     Solution  : TNvector;      { Solution to the set of equations }
     St        : boolean;
begin
 SMPTEWhite;
 EBUWhite;
 ReadPreGainFactor(notSilent);
 LearnSMPTE_COk := true;
 Beep(1000,200);
 writeln('Placer sensoren p† SMPTE C monitoren over det hvide felt  -  tryk en tast');
 WaitForAnyKey;
 COMStatus := writeCOMPort(COMNo,'MX,');
 Delay(20);
 COMStatus := writeCOMPort(COMNo,'MC,');
 write('M†ling: R .');
 SMPTERed;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,Timeout);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,Timeout);
  write('.');
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   writeln('Timeout: ingen m†ling  (',Timeout,' sec)');
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnSMPTE_COk := false;
   TestFailed := true;
   SMPTEWhite;
   Exit;
  end;

 CalculateMean(MeasStr,AntalMeas,XR,YR,ZR,St);


 write('        G .');
 SMPTEGreen;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,Timeout);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,Timeout);
  write('.');
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   writeln('Timeout: ingen m†ling  (',Timeout,' sec)');
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnSMPTE_COk := false;
   TestFailed := true;
   SMPTEWhite;
   Exit;
  end;

 CalculateMean(MeasStr,AntalMeas,XG,YG,ZG,St);

 write('        B .');
 SMPTEBlue;
 for n := 1 to LearnDelay do begin Delay(1000); write('.'); end;
 n := 1;
 COMStatus := ClearInputQue(COMNo);
 COMStatus := ReadlnCOMPort(COMNo,Svar,Timeout);
 repeat
  COMStatus := ReadlnCOMPort(COMNo,Svar,Timeout);
  write('.');
  MeasStr[n] := Svar;
  Inc(n);
 until (n = AntalMeas + 1) or (COMStatus <> 0);
 writeln;
 if (COMStatus <> 0) then
  begin
   writeln('Timeout: ingen m†ling  (',Timeout,' sec)');
   COMStatus := writeCOMPort(COMNo,'MS,');
   LearnSMPTE_COk := false;
   TestFailed := true;
   SMPTEWhite;
   Exit;
  end;

 COMStatus := writeCOMPort(COMNo,'MS,');

 CalculateMean(MeasStr,AntalMeas,XB,YB,ZB,St);

 writeln;
 SMPTEWhite;
 write(' R›d:  ',XR:10:2); write(YR:10:2); writeln(ZR:10:2,'   ');
 write(' Gr›n: ',XG:10:2); write(YG:10:2); writeln(ZG:10:2,'   ');
 write(' Bl†:  ',XB:10:2); write(YB:10:2); writeln(ZB:10:2,'   ');

 if LearnSMPTE_COk then
   UdregnMXCrtMatrix(CRTMatrixAddr - 20,'SMPTE C')
end; {LearnSMPTE_CFosfor}







Procedure SensorTotalTest;
begin
 WPOff;
 Delay(100);

 EBUWhite;
 SMPTEWhite;

 writeProtectYesOrNo(Silent);

 if writeProtectOk then
  begin
   writeln(Bell,'Color sensor m† IKKE v‘re skrivebeskyttet ved kalibrering !!');
   writeln(     'Skrivebeskyttelsen fjernes med Ctrl-F9');
   Exit;
  end;

 if ReadKalStatus = 'USER' then
  begin
   writeln(Bell,'Color sensor er USER kalibreret - forts‘t ?    J/N');
   if TastJ <> ScanCode then
    begin
     writeln('Kalibrering afbrudt');
     Exit;
    end
  end;



 ClearTestFlag;
{$ifDEF IEEEBUS}
 SupplyCurrentTest;
 AnalogProbeTest;
{$endif}
 RS232Test;

 if not TestFailed then
   EnterKU_Nummer;

 if TestAfbrudt then Exit;
 if not TestFailed then
  begin
   InitPtvEEPROM;
  end;

 if TestFailed  then Exit;
 if TestAfbrudt then Exit;

 if not TestFailed then
  begin
   writeCRTNamesToEEPROM;
   writeIdString;
   writeDefaultGainFactor;
   writeDefaultPreGain;
   LearnOffset;
  end;
 if TestAfbrudt then Exit;

 if TestFailed  then Exit;

 if not TestFailed then
   DarkCurrentTest;

 if TestAfbrudt then Exit;
 if TestFailed  then Exit;

 if not TestFailed then
   BeregnGainFactor;

 if TestAfbrudt then Exit;
 if TestFailed  then Exit;

 if not TestFailed then
   LearnKMatrix(NotSaveCalData);
 if TestAfbrudt then Exit;
 if not TestFailed then
   LearnSMPTE_CFosfor;
 if TestAfbrudt then Exit;
 if not TestFailed then
  begin
   ReadGainFactor;
   ReadIdString(true);
   ReadPreGainFactor(notSilent);
   ReadNoCRTFosfor(NotSilent);
   ReadNoEEPROMPages;
   ReadInternID;
   ReadCRTNames(Silent);
   ReadCRTMatrix(Silent);
   ReadKMatrix(NotSilent);
 end;

  if SensorVer = 93 then
   CheckSWBaudrateShift;

 TotalTestOk :=  RS232TestOk AND
                 CurrentTestOk AND
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

  write('Totaltest ');
  TrueOrFalse(TotalTestOk);

 if Analog5VTestValgt then
  TotaltestOk :=  TotaltestOk AND AnalogPos5VTestOk AND AnalogNeg5VTestOk;
 if ADRefTestValgt then
  TotaltestOk :=  TotaltestOk AND ADRefTestOk;

 if TotalTestOk then
  begin
   writeOkToEEPROM;
   ReadWPAddr(NotSilent);
   WPon;                               { write protect = on }
   ReadKalDataFromEEPROM;
   write(Bell,'Skal kalibreringsdata udskrives p† printer?   Ja/Nej  ');
   if GetKeycode = TastJ then
     WriteCertifFileToLaser(false);
   writeln;
  end
  else
   writenot_OkToEEPROM;

  if SensorVer = 93 then
   TotaltestOk := TotaltestOk and
                  BaudrateShiftOk;

  TotaltestOk :=  TotaltestOk and
                  ReadKaldataOk and
                  ReadwriteProtectOk;

 write('PM5639 kalibrering OK ?:');
 TrueOrFalse(FinalTestOk);

 if TotalTestOk then
   writeln('PM5639 sensor G O D K E N D T')
  else
   writeln('PM5639 sensor A F V I S T');
end; {TotalTest}





Procedure BarcoTotalTest;
begin
 ClrScr;
 writeln;
 writeln;
 writeln('Forbind Barco interface med:  Lyskasse (LEMO stik)');
 writeln('                              Color sensor PM5639/90 serie IV');
 writeln;
 writeln('Tryk en tast');
 WaitForAnyKey;




 ClearTestFlag;
{$ifDEF IEEEBUS}
 AnalogProbeTestBarco;
{$endif}


 BarcoMeasure;

 TotalTestOk := not TestFailed;

 gotoxy(25,12);
 if TotalTestOk then
   writeln('PM5639/02   G O D K E N D T')
  else
   writeln('PM5639/02   A F V I S T');
end; {BarcoTotalTest}



begin           {>>>> main program <<<<}

 SelectSensor;
 SetCBreak(true);
 ClrScr;
 InitWindow;
 InitTopWindow;
 InitBottomWindow;
 InitVar;
 InitPC_ComPort;
{$ifDEF IEEEBUS}
 InitIeeeBus;
{$endif}
 InitI2CBus;

 SaveMonitorOnTime;   {<<<<<<<<<<<<<<<<<<}

 RGBGenRemote;
 SMPTEWhite;
 EBUWhite;
 SaveMonitorOnTime;
{$ifDEF RGBGEN}
 DAVoltage(7.1);  {sp‘nding fra D/A-converter til PM5639/80  (RGB-generator)}
{$endif}
 LampOff;

 if not AfslutProgram then
 repeat
  EmptyKeyboardBuffer;
  TestFailed := false;

  Set_Timer(SONY_Timeout*10);

  writeln('Specialprogram for PM5639/94 sensor ');

  repeat
  until Timer_Timeout or KeyPressed;

   if SensorVer <> Barco then
   if Timer_Timeout then
    begin
     EBUBlack;
     SMPTEBlack;
     writeln('SONY-monitorer = OFF');
    end;


 { ------------------ PM5639   SENSORER ----------------------}
  if SensorVer <> Barco then
  case ScanCode of
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
                ReadIdString(true);
                ReadPreGainFactor(notSilent);
                ReadWPAddr(NotSilent);
                ReadNoCRTFosfor(NotSilent);
                ReadNoEEPROMPages;
                ReadInternID;
                writeProtectYesOrNo(NotSilent);
                writeln('Kal status: ',ReadKalStatus);
                ReadCalDate(NotSilent);
                writeln('Trace status: ',ReadTraceFlag);
                writeln('Output format: ',ReadOutputFormat);
               end; {if}
            end;
       F5 : LearnOffset;
       F6 : DarkCurrentTest;
       F7 : BeregnGainFactor;
       F8 : LearnSMPTE_CFosfor;
       F9 : LearnKMatrix(SaveCalData);
      F10 : begin
             TotalTestFlag := true;
             SensorTotalTest;
             TotalTestFlag := false;
            end;

   AltF3  : InitPtvEEPROM;
   AltF5  : writeIDString;
   AltF6  : writeDefaultGainFactor;
   AltF7  : writeDefaultPreGain;
   AltF8  : writeCRTNamesToEEPROM;
   AltF9  : ReadKalDataFromEEPROM;
   AltF10 : WriteCertifFileToLaser(true);

   CtrlF1 : ReadCRTMatrix(NotSilent);
   CtrlF2 : ReadKMatrix(NotSilent);
   CtrlF3 : ReadCRTNames(NotSilent);
   CtrlF4 : writeProtectYesOrNo(NotSilent);
   CtrlF5 : ShowActualXYValues;
   CtrlF6 : ShowOffsetValues(NotSilent);
   CtrlF8 : WPOn;
   CtrlF9 : WPOff;
   CtrlF10 : if SensorVer = 93 then
               CheckSWBaudrateShift
              else
               write('?? ');

   TastA : EnterLevel;
   TastC : ClrScr;
   TastF : ReadXYCrtMatrix(NotSilent);
   TastG : RGBMeasure(XYCrtMatrix);
   TastH : ShowHelpWindow;
   TastL : ToggleLamp;
   TastM :  begin
              ReadKMatrix(Silent);
              case SensorVer of
               90,92,93 : xyYMeasure(KorrMatrix,KorrMulFactor,
                             D6500Ref[1],D6500Ref[2],NIT_fL_Factor,1);
               916      : xyYMeasure(KorrMatrix,KorrMulFactor,
                             D6500Ref[1],D6500Ref[2],NIT_fL_Factor,3);
               else
                writeln('Ingen sensor type valgt - check command line parameter');
              end; {case}
            end;
   TastO :  begin
             ShowOffsetValues(Silent);
              case SensorVer of
               90,92,93  : STMeasure(Offset,ADRef,ADCounts,4,1);
               916       : STMeasure(Offset,ADRef,ADCounts,4,3);
              end;
            end;
   TastP : PowerOffOn;
   TastR : RefMeasurements;
   TastY : ToggleRelay;
   AltR  : TestRelayRGB;
   TastW : begin
            if EBUSetOn = false then
             begin
              EBUWhite;
              SMPTEWhite;
              SaveMonitorOnTime;
             end;
           end;


   Escape : AfslutProgram := true;
     else
      write('?? ');
   end; {case ScanCode}


 { ------------------ PM5639/02  Barco option ----------------------}
  if SensorVer = Barco then
  case ScanCode of
{$ifdef IEEEBUS}
      F1  :  AnalogProbeTestBarco;
{$endif}
      TastM,
      F2  :  BarcoMeasure;

      F10 : begin
             TotalTestFlag := true;
             BarcoTotalTest;
             TotalTestFlag := false;
            end;

   TastP : PowerOffOn;

   Escape : AfslutProgram := true;
     else
      write('?? ');
   end; {case ScanCode}


   writeln('*');

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
{$ifdef RGBGEN}
   EBUBlack;
   SMPTEBlack;
   RGBGenLocal;
{$endif}
  end;

 Window(1,1,80,25); Color(LightGray,Black); ClrScr;

 COMStatus := CloseCOMPort(COMNo);
 if COMStatus <> 0 then
  writeln('COM',COMNo,' port ikke lukket korrekt: ',COMStatus);

end.

