Unit Unit_CER;

{
  Unit for test certificate

  Philips TV Test Equipment A/S
  Peter Frederiksen, 941017

  Unit to print test results on HP laserjet printers.
  Used Printer Control Language = PCL 4.

  Se "HP LaserJet h†ndbogen", Teknisk Forlag 1993, ISBN 87-571-1430-7.
  Se manualer for diverse HP LaserJet printere.

  HP LaserJet:                       PCL 3
  HP LaserJet+ to HP Laserjet IIP:   PCL 4
  HP Laserjet III and up:            PCL 5
  Program tested on Laserjet4 - ok.

  Test certificate skrives til en fil med navn: xxxxxx.HP
  hvor xxxxxx = serienummer. Serienummer l‘ses fra Flash prom i
  generatoren.
  Filen xxxxxx.HP skrives til directory = CertifNamePath. Se filen
  UNIT_G39.PAS
  Filen kopieres derefter til LPTx (x=2,3):   copy xxxxxx.HP LPTx /b
  Se konstanten PrinterPort.


  Rettelser:

}

Interface

Uses Crt,
     Comutil,
     Unit_G39,
     Unit_VLM,          { Video level meter         unit }
     Unit_Fil,		{ POWER TOOLS PLUS File     unit }
     Unit_Sup,		{ POWER TOOLS PLUS Support  unit }
     Unit_Key,		{ POWER TOOLS PLUS Keyboard unit }
     Unit_Scn,		{ POWER TOOLS PLUS Screen   unit }
     Unit_Win,		{ POWER TOOLS PLUS Window   unit }
     Unit_Pgm,		{ POWER TOOLS PLUS Program  unit }
     PepUtil;


Procedure CopyFileToLaserjet(Flag : boolean);  { Alt F2 }
Procedure MakeCertif(SLev,SubcF,LineF,         { Sync level, Subc freq, Line freq }
                     G100,BG100,RG100,         { Green,BlueGreen,RedGreen 100% }
                     G75,BG75,RG75,            {   -       -         -     75% }
                     G50,BG50,RG50,            {   -       -         -     50% }
                     G25,BG25,RG25 : real);    {   -       -         -     25% }
Procedure PAL_Certif;        { Alt F1 }
Procedure NTSC_Certif;       { Alt F1 }
Procedure Component_Certif;  { Alt F1 }


Implementation


const
 { 'Á' = 'ñ' in symbol set 7J}
 { '²' = 'æ' in symbol set 7J}

 PaperMargin   = 5.1; { mm }
 LeftMargin    = 20;  { mm }
 YStartPos     = 43;  { placement of 'PM5639x/8x Color ...'  mm from top of paper }

 Col1          = LeftMargin;
 Col2          = LeftMargin + 55;
 Col3          = LeftMargin + 130;

 DotPerInch    = 300;
 lj2           = 1;
 lj2D          = 2;

 Times6        = 1;
 Times8        = 2;
 Times10       = 3;
 Times12       = 4;
 Times14       = 5;
 Times18       = 6;
 Times24       = 7;
 Times8Bold    = 8;
 Times10Bold   = 9;
 Times12Bold   = 10;
 Times14Bold   = 11;
 Times18Bold   = 6;
 Times24Bold   = 7;
 Times8Italic  = 12;
 Times10Italic = 13;
 Times12Italic = 14;
 Times14Italic = 15;
 Courier10     = 16;
 Courier10Bold = 17;
 Univers14     = 18;
 Univers18     = 19;
 Univers24     = 20;
 Courier17     = 21;


 Font : array[Times6..Courier17] of string[50] =
                (ESC+'(7J'+ESC+'(s1p6v0s0b5T',          { 1  Font = Times 6        }
                 ESC+'(7J'+ESC+'(s1p8v0s0b5T',          { 2  Font = Times 8        }
                 ESC+'(7J'+ESC+'(s1p10v0s0b5T',         { 3  Font = Times 10       }
                 ESC+'(7J'+ESC+'(s1p12v0s0b5T',         { 4  Font = Times 12       }
                 ESC+'(7J'+ESC+'(s1p14v0s0b5T',         { 5  Font = Times 14       }
                 ESC+'(7J'+ESC+'(s1p18v0s3b5T',         { 6  Font = Times 18 Bold   }
                 ESC+'(7J'+ESC+'(s1p24v0s3b5T',         { 7  Font = Times 24 Bold   }
                 ESC+'(7J'+ESC+'(s1p8v0s3b5T',          { 8  Font = Times 8 Bold    }
                 ESC+'(7J'+ESC+'(s1p10v0s3b5T',         { 9  Font = Times 10 Bold   }
                 ESC+'(7J'+ESC+'(s1p12v0s3b5T',         { 10 Font = Times 12 Bold   }
                 ESC+'(7J'+ESC+'(s1p14v0s3b5T',         { 11 Font = Times 14 Bold   }
                 ESC+'(7J'+ESC+'(s1p8v1s0b5T',          { 12 Font = Times 8 Italic  }
                 ESC+'(7J'+ESC+'(s1p10v1s0b5T',         { 13 Font = Times 10 Italic }
                 ESC+'(7J'+ESC+'(s1p12v1s0b5T',         { 14 Font = Times 12 Italic }
                 ESC+'(7J'+ESC+'(s1p14v1s0b5T',         { 15 Font = Times 14 Italic }
                 ESC+'(10U'+ESC+'(sp10h12vs0b3T',       { 16 Font = Courier 10      }
                 ESC+'(10U'+ESC+'(sp10h12vs3b3T',       { 17 Font = Courier 10 Bold }
                 ESC+'(7J'+ESC+'(s1p14v0s0b52T',        { 18 Font = Univers 14      }
                 ESC+'(7J'+ESC+'(s1p18v0s0b52T',        { 19 Font = Univers 18      }
                 ESC+'(7J'+ESC+'(s1p24v0s0b52T',        { 20 Font = Univers 24      }
                 ESC+'(10U'+ESC+'(sp16.66h8.5vs0b0T');      { 21 Font = Courier 16.67   }

    {(10U(sp10h12vsb3T*p337Y*p246XCourier*p389Y*p246X(7J(s1p12vsb5TTimes}


var F     : text;
    FName : string;

Procedure ResetPrinter;
begin
 write(F,ESC+'E');
end;


{ ------ PAGE ORIENTATION ----------------------------------- }
Procedure SetLandscape;
begin
 write(F,ESC+'&l1O');
end;

Procedure SetPortrait;
begin
 write(F,ESC+'&l0O');
end;

{ ------ UNDERLINE ------------------------------------------ }
Procedure SetUnderlineOn;
begin
 write(F,ESC+'&dD');
end;
Procedure SetUnderlineOff;
begin
 write(F,ESC+'&d@');
end;

{ ------ EJECT PAGE ---------------------------------------- }
Procedure EjectPage;
begin
 write(F,ESC+'&l0H');
end;

{ ------ FONTS ---------------------------------------------- }
Procedure SetFont(FontNo : byte );
begin
 write(F,Font[FontNo]);
end;

{ ------ MARGINS -------------------------------------------- }
Procedure SetLeftMargin(ColNo : word);
begin
 write(F,#27+#38+#97+IntToStr(ColNo)+#76);
end;

{ ------ XY Position ---------------------------------------- }
Procedure MoveToXY(XPos, YPos : real);
var X,Y  : string[5];
begin
 if XPos < PaperMargin then XPos := PaperMargin;
 if YPos < PaperMargin then YPos := PaperMargin;
 X := IntToStr(Trunc(((XPos-PaperMargin)/25.4*DotPerInch)));
 Y := IntToStr(Trunc(((YPos-PaperMargin)/25.4*DotPerInch)));
 write(F,ESC+'*p'+X+'X');
 write(F,ESC+'*p'+Y+'Y');
end;

{ ------ Hor & Ver Lines -------------------------------------- }
Procedure HorLine(XPos,YPos,Thicknes,Length : real);
var L,T  : string[5];
begin
 MoveToXY(XPos,YPos);
 T := IntToStr(Trunc(Thicknes/25.4*DotPerInch));
 L := IntToStr(Trunc(Length/25.4*DotPerInch));
 write(F,ESC+'*c'+L+'a'+T+'b0P');
end;

Procedure VerLine(XPos,YPos,Thicknes,Length : real);
var L,T  : string[5];
begin
 MoveToXY(XPos,YPos);
 T := IntToStr(Trunc(Thicknes/25.4*DotPerInch));
 L := IntToStr(Trunc(Length/25.4*DotPerInch));
 write(F,ESC+'*c'+T+'a'+L+'b0P');
end;


Procedure CopyFileToLaserjet(Flag : boolean);   { Alt F2 }
{ Flag=true   FName = xxxxxx.HP
  Flag=false  FName = Serienummer skal indtastes af brugeren
  Til udskrivning bruges DOS-kommando:   COPY xxxxxx.HP LPTx /b > NUL
}
var SN        : string;
    ExErrCode : word;
begin
 if not Flag then
  begin
   write('Indtast serienummer til udskrivning (6 tal)   ');
   readln(SN);
   FName := CertifNamePath + SN + CertifNameExt;
  end;

 write('Udskriv "',FName,'" til Laser printer  (Ja / Nej)'  );

 __FlushKey;
 if Scancode <> TastJ then
  begin
   Writeln('  n† ikke ??');
   Exit;
  end;

 writeln('  Ja');

 assign(F,FName);
 {$I-}
 Reset(F);
 {$I+}
 if (IOResult <> 0) then
  begin
   writeln('Filen "',FName,'" findes ikke',Bell);
   Exit;
  end;

 {$I-}
 Close(F);
 {$I+}

{ Check := __DispWin(DOSWin);}
 __WinExPgm('DOS','COPY '+FName+' '+PrinterPort+' /B > NUL',1,2,_CurColumns,_CurRows,DosFore,DosBack,ExErrCode);

  case ExErrCode of
  {0 : __QuikWin(2,10,White,DosBack,'Tryk en tast for at forts‘tte');}
   8 : __QuikWin(2,10,White,DosBack,'   For lidt memory til COPY !  ');
  end;

 if ExErrCode <> 0 then
  TestFailed := true;

{ WaitforAnykey;
 Check := __RemWin;}
end; { CopyFileToLaserjet }



Procedure MakeCertif(SLev,SubcF,LineF,       { Sync level, Subc freq, Line freq }
                     G100,BG100,RG100,       { Green,BlueGreen,RedGreen 100% }
                     G75,BG75,RG75,          {   -       -         -     75% }
                     G50,BG50,RG50,          {   -       -         -     50% }
                     G25,BG25,RG25 : real);  {   -       -         -     25% }
var
 svar  : string;

begin
 SelPatt(rwPatt);

 St := ClearInputQue(COMNo);
 St := WriteCOMPort(COMNo,'GSECT0,');
 St := WriteCOMPort(COMNo,'GADR4032,');
 St := WriteCOMPort(COMNo,'GREAD30,'); delay(10);

 St := ReadCOMPort(COMNo,30,Svar,1);

 if St <> 0 then
  begin
   writeln('Error:  RS232 timeout');
   SelPatt(cbPatt);
   TestFailed := true;
   exit;
  end;

 SelPatt(cbPatt);

 KU_No := Copy(svar,24,6);

 FName := CertifNamePath + KU_No + CertifNameExt;
{ FName := 'lpt2';}
 assign(F,FName);
 {$I-}
 Reset(F);
 {$I+}
 if (IOResult = 0) then
  begin
   Write('Filen "',FName,'"  findes - overskriv filen ?  (Ja / Nej)',Bell);
   EmptyKeyboardBuffer;
    if Scancode <> TastJ then
     begin
      Writeln;
      Writeln('"',FName,'" IKKE overskrevet');
      TestFailed := true;
      Exit;
     end
     else
     begin
      Rewrite(F);
      Writeln;
      Writeln('"',FName,'" overskrevet');
     end {if scancode}
  end {if IOResult}
  else
  begin
    Rewrite(F);
    Writeln('Filen "',FName,'" oprettet')
  end;  {if IOResult}


 ResetPrinter;
 SetFont(Univers18);
 MoveToXY(LeftMargin,13);
 write(F,'Philips TV Test Equipment A/S');
 HorLine(LeftMargin,14,0.7,170);


 SetFont(Univers18);
 MoveToXY(LeftMargin,YStartPos-9);
 write(F,'Test results:');

 MoveToXY(LeftMargin,YStartPos);

 case GenVer of
  PAL  :  write(F,'PM 5639G/83 Color Alignment Generator');
  NTSC :  write(F,'PM 5639M/83 Color Alignment Generator');
  G625 :  write(F,'PM 5639G/82 Color Alignment Generator');
  M525 :  write(F,'PM 5639M/82 Color Alignment Generator');
 end;

 SetFont(Times12);
 MoveToXY(LeftMargin,YStartPos + 13);
 write(F,'Serial number: KU',KU_No);
 MoveToXY(LeftMargin,270);
 write(F,'Signed ______________          date: ',dato);

 SetFont(Times14Bold);
 MoveToXY(Col1,YStartPos + 22);
 write(F,'Function');
 MoveToXY(Col2,YStartPos + 22);
 write(F,'Product data');
 MoveToXY(Col3,YStartPos + 22);
 write(F,'Obtained value');
 HorLine(LeftMargin,YStartPos + 24,0.4,170);
 VerLine(Col2-5,YStartPos + 19,0.4,240 - YStartPos);
 VerLine(Col3-5,YStartPos + 19,0.4,240 - YStartPos);


{  ---------------  Sync level  ------------------------}
 SetFont(Times12);
 MoveToXY(Col1,YStartPos + 37);
 write(F,'Sync amplitude');
 MoveToXY(Col2,YStartPos + 37);
 if GenVer = NTSC then
  write(F,SyncLevelNomNTSC:5:0,' mV Á 5%')
 else
  write(F,SyncLevelNomPAL:5:0,' mV Á 5%');
 SetFont(Courier10);
 MoveToXY(Col3,YStartPos + 37);
 write(F,SLev:5:0,' mV');
 SetFont(Times12);




{  ---------------  Subc freq  ------------------------}
 if GenVer in [PAL,NTSC] then
  begin
   MoveToXY(Col1,YStartPos + 47);
   write(F,'Subcarrier frequency');
   MoveToXY(Col2,YStartPos + 47);
   if GenVer = PAL then
     write(F,(SUBCFreqNomPAL/1E6):8:6,' MHz Á 50 Hz')
    else
     write(F,(SUBCFreqNomNTSC/1E6):8:6,' MHz Á 50 Hz');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 47);
   write(F,(SubcF/1E6):8:6,' MHz');
   SetFont(Times12);
  end;


{  ---------------  Line freq  ------------------------}
 MoveToXY(Col1,YStartPos + 57);
 write(F,'Line frequency');
 MoveToXY(Col2,YStartPos + 57);
 if GenVer in [PAL,G625] then
   write(F,(LineFreqNom625/80000):6:3,' kHz Á ',LineFreqTolProdData:4:2,' Hz')
  else
   write(F,(LineFreqNom525/80000):8:5,' kHz Á ',LineFreqTolProdData:4:2,' Hz');
 SetFont(Courier10);
 MoveToXY(Col3,YStartPos + 57);
 write(F,(LineF/1000):9:6,' kHz');
 SetFont(Times12);


{  ---------------  Luminance level  ------------------------}
 MoveToXY(Col1,YStartPos + 67);
 write(F,'Luminance amplitude');
 MoveToXY(Col2,YStartPos + 67);
 if GenVer = NTSC then
   write(F,'Level 100%  714 mV Á 1%')
  else
   write(F,'Level 100%  700 mV Á 1%');
 SetFont(Courier10);
 MoveToXY(Col3,YStartPos + 67);
 write(F,G100:5:1,' mV');
 SetFont(Times12);

 if GenVer in [PAL,NTSC] then
  begin
   MoveToXY(Col2,YStartPos + 77);
   if GenVer = NTSC then
     write(F,'Level 75%  549 mV Á 1.5% Á 3 mV')
    else
     write(F,'Level 75%  525 mV Á 1.5% Á 3 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 77);
   write(F,G75:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 87);
   if GenVer = NTSC then
     write(F,'Level 50%  384 mV Á 1.5% Á 3 mV')
    else
     write(F,'Level 50%  350 mV Á 1.5% Á 3 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 87);
   write(F,G50:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 97);
   if GenVer = NTSC then
     write(F,'Level 25%  219 mV Á 1.5% Á 3 mV')
    else
     write(F,'Level 25%  175 mV Á 1.5% Á 3 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 97);
   write(F,G25:5:1,' mV');
   SetFont(Times12);

  end; { if GenVer in [PAL,NTSC] }


{  ---------------  Component 525 & 625  ---------------------}
 if GenVer in [G625,M525] then
  begin
   MoveToXY(Col1,YStartPos + 87);
   write(F,'Luminance tracking');
   MoveToXY(Col1,YStartPos + 92);
   write(F,'GBR, differential');
   MoveToXY(Col1,YStartPos + 97);
   write(F,'accuarcy');

{  ---------------  100% level ------ ------------------------}
   MoveToXY(Col2,YStartPos + 87);
   write(F,'Green 100%');
   MoveToXY(Col2,YStartPos + 92);
   write(F,'Blue - Green Á 1.4 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 92);
   write(F,BG100:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 97);
   write(F,'Red - Green Á 1.4 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 97);
   write(F,RG100:5:1,' mV');
   SetFont(Times12);

{  ---------------  75% level ------ ------------------------}
   MoveToXY(Col2,YStartPos + 107);
   write(F,'Green 75%  525 Á 1.5% Á 3 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 107);
   write(F,G75:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 112);
   write(F,'Blue - Green Á 1.1 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 112);
   write(F,BG75:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 117);
   write(F,'Red - Green Á 1.1 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 117);
   write(F,RG75:5:1,' mV');
   SetFont(Times12);

{  ---------------  50% level ------ ------------------------}
   MoveToXY(Col2,YStartPos + 127);
   write(F,'Green 50%  350 Á 1.5% Á 3 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 127);
   write(F,G50:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 132);
   write(F,'Blue - Green Á 0.7 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 132);
   write(F,BG50:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 137);
   write(F,'Red - Green Á 0.7 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 137);
   write(F,RG50:5:1,' mV');
   SetFont(Times12);

{  ---------------  25% level ------ ------------------------}
   MoveToXY(Col2,YStartPos + 147);
   write(F,'Green 25%  175 Á 1.5% Á 3 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 147);
   write(F,G25:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 152);
   write(F,'Blue - Green Á 0.6 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 152);
   write(F,BG25:5:1,' mV');
   SetFont(Times12);

   MoveToXY(Col2,YStartPos + 157);
   write(F,'Red - Green Á 0.6 mV');
   SetFont(Courier10);
   MoveToXY(Col3,YStartPos + 157);
   write(F,RG25:5:1,' mV');
   SetFont(Times12);
  end; { if GenVer in [G625,M525] }

 EjectPage;

 close(F);
end; { MakeCertif }




Procedure PAL_Certif;
const   MPos  = 24;
var VLMMeasOk : boolean;
    Msg       : str_10;
begin
 clrscr;
 writeln('Test certificate PAL');
 CertifTestOk := true;
 SUBCFreqNom := SUBCFreqNomPAL;
 SyncLevelMeas(GChannel);
 if (not SyncLevelGOk) then
  begin
   CertifTestOk := false;
   writeln('M†linger til test certificate:   FAILED');
   TestFailed := true;
   exit;
  end;

 {------------------ G channel ------- PAL ------------------}
 SelPatt(winPatt);
 SendToReg(1,5,0,0);
 Delay(1000);
 VLMMeas(140,170,350,380,NomLevel625_25,AbsTol_25,GLevel_25,Msg,VLMMeasOk); { 25 % window level}
 writeln('   25% level: ',GLevel_25:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;

 SendToReg(1,10,0,0);
 VLMMeas(140,170,350,380,NomLevel625_50,AbsTol_50,GLevel_50,Msg,VLMMeasOk); { 50 % window level}
 writeln('   50% level: ',GLevel_50:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;

 SendToReg(1,15,0,0);
 VLMMeas(140,170,350,380,NomLevel625_75,AbsTol_75,GLevel_75,Msg,VLMMeasOk); { 75 % window level}
 writeln('   75% level: ',GLevel_75:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;

 SendToReg(1,20,0,0);
 VLMMeas(140,170,350,380,NomLevel625_100,AbsTol_100,GLevel_100,Msg,VLMMeasOk); { 100 % window level}
 writeln('  100% level: ',GLevel_100:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;


{------------------ Subc freq ---- PAL ---------------------}
 InitPM6669;
 write('Forbind counter probe til TP 4  -  tryk en tast');
 WaitForAnyKey;
 write(' .');
 ReadPM6669(SUBCFreq);
 write(' .');
 ReadPM6669(SUBCFreq);
 SUBCFreqOk := (Abs(SUBCFreq - SUBCFreqNom)) < (SUBCFreqTol * 2);
 gotoxy(1,wherey); ClrEol;
 write('   SUBC freq: ',SUBCFreq:8:0,' Hz                  ');
 if SUBCFreqOk then
    writeln('OK')
  else
   begin
    writeln('FAIL',(SubcFreqTol*2):8:0,' Hz');
    CertifTestOk := false;
   end;

{------------------ Line freq ---- PAL ---------------------}
 write('Forbind counter probe til V6 pin 22 (SAA1101)  -  tryk en tast');
 WaitForAnyKey;
 ReadPM6669(LineFreq);              { 15625 Hz }
 write(' .');
 ReadPM6669(LineFreq);
 write(' .');
 LineFreqOk := (Abs(LineFreq - (LineFreqNom625/80))) < (LineFreqTolProdData);
 gotoxy(1,wherey); ClrEol;
 write('   Line freq: ',LineFreq:9:3,' Hz                 ');
 if LineFreqOk then
    writeln('OK')
  else
   begin
    writeln('FAIL',LineFreqTolProdData:8:3,' Hz');
    CertifTestOk := false;
   end;


writeln;
write('M†linger til test certificate:             ');
if CertifTestOk then
 begin
  writeln('OK');
  writeln;
  MakeCertif(SyncGLevel,Subcfreq,LineFreq,   { Sync level, Subc freq, Line freq }
      GLevel_100,0,0,                        { Green,BlueGreen,RedGreen 100% }
      GLevel_75,0,0,                         {   -       -         -     75% }
      GLevel_50,0,0,                         {   -       -         -     50% }
      GLevel_25,0,0);                        {   -       -         -     25% }

  CopyFileToLaserjet(true);
 end
 else
  begin
   writeln('FAIL');
   TestFailed := true;
  end;

end; { PAL_Certif }





Procedure NTSC_Certif;
const  MPos      = 24;
var VLMMeasOk : boolean;
    Msg       : str_10;
begin
 clrscr;
 writeln('Test certificate NTSC');
 CertifTestOk := true;
 SUBCFreqNom := SUBCFreqNomNTSC;
 SyncLevelMeas(GChannel);
 if (not SyncLevelGOk) then
  begin
   CertifTestOk := false;
   writeln('M†linger til test certificate:   FAILED');
   TestFailed := true;
   exit;
  end;

 {------------------ G channel ------- NTSC ------------------}
 SelPatt(winPatt);
 SendToReg(1,5,0,0);
 Delay(1000);
 VLMMeas(140,170,350,380,NomLevelNTSC_25,AbsTol_25,GLevel_25,Msg,VLMMeasOk); { 25 % window level}
 writeln('   25% level: ',GLevel_25:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;

 SendToReg(1,10,0,0);
 VLMMeas(140,170,350,380,NomLevelNTSC_50,AbsTol_50,GLevel_50,Msg,VLMMeasOk); { 50 % window level}
 writeln('   50% level: ',GLevel_50:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;

 SendToReg(1,15,0,0);
 VLMMeas(140,170,350,380,NomLevelNTSC_75,AbsTol_75,GLevel_75,Msg,VLMMeasOk); { 75 % window level}
 writeln('   75% level: ',GLevel_75:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;

 SendToReg(1,20,0,0);
 VLMMeas(140,170,350,380,NomLevelNTSC_100,AbsTol_100,GLevel_100,Msg,VLMMeasOk); { 100 % window level}
 writeln('  100% level: ',GLevel_100:6:1,' mV',Msg:MPos);
 if (not VLMMeasOk) then
  CertifTestOk := false;


{------------------ Subc freq ---- NTSC ---------------------}
 InitPM6669;
 write('Forbind counter probe til TP 4  -  tryk en tast');
 WaitForAnyKey;
 write(' .');
 ReadPM6669(SUBCFreq);
 write(' .');
 ReadPM6669(SUBCFreq);
 SUBCFreqOk := (Abs(SUBCFreq - SUBCFreqNom)) < (SUBCFreqTol * 2);
 gotoxy(1,wherey); ClrEol;
 write('   SUBC freq: ',SUBCFreq:8:0,' Hz                  ');
 if SUBCFreqOk then
    writeln('OK')
  else
   begin
    writeln('FAIL',(SubcFreqTol*2):8:0,' Hz');
    CertifTestOk := false;
   end;

{------------------ Line freq ---- NTSC ---------------------}
 write('Forbind counter probe til V6 pin 22 (SAA1101)  -  tryk en tast');
 WaitForAnyKey;
 ReadPM6669(LineFreq);              { 15734.264 Hz }
 write(' .');
 ReadPM6669(LineFreq);
 write(' .');
 LineFreqOk := (Abs(LineFreq - (LineFreqNom525/80))) < LineFreqTolProdData;
 gotoxy(1,wherey); ClrEol;
 write('   Line freq: ',LineFreq:9:3,' Hz                 ');
 if LineFreqOk then
    writeln('OK')
  else
   begin
    writeln('FAIL',LineFreqTolProdData:8:3,' Hz');
    CertifTestOk := false;
   end;


writeln;
write('M†linger til test certificate:             ');
if CertifTestOk then
 begin
  writeln('OK');
  writeln;
  MakeCertif(SyncGLevel,Subcfreq,LineFreq,   { Sync level, Subc freq, Line freq }
      GLevel_100,0,0,                        { Green,BlueGreen,RedGreen 100% }
      GLevel_75,0,0,                         {   -       -         -     75% }
      GLevel_50,0,0,                         {   -       -         -     50% }
      GLevel_25,0,0);                        {   -       -         -     25% }

  CopyFileToLaserjet(true);
 end
 else
  begin
   writeln('FAIL');
   TestFailed := true;
  end;

end; { NTSC_Certif }



Procedure Component_Certif;
const GKeyDelay = 200;       { delay to store setup in flash PROM }
      MPos      = 22;
var VLMMeasOk,
    GBRDiffOk : boolean;
    Msg       : str_10;
begin
clrscr;
write('Test certificate ');
case GenVer of
 G625  : writeln('625 component');
 M525  : writeln('525 component');
end;

CertifTestOk := true;
SyncLevelMeas(GChannel);

if (not SyncLevelGOk) then
 begin
  CertifTestOk := false;
  writeln('M†linger til test certificate:   FAILED');
  TestFailed := true;
  exit;
 end;

delay(200);
if GenVer = M525 then
 St := WriteCOMPort(COMNo,'GKEY20,');   { setup=off }
Delay(GKeyDelay);
St := WriteCOMPort(COMNo,'GKEY15,');   { sync=on }
Delay(GKeyDelay);
St := WriteCOMPort(COMNo,'GKEY13,');   { mode=GBR }
Delay(GKeyDelay);

{------------------ G channel ------- Component ------------------}
SelPatt(winPatt);
Delay(1000);
SendToReg(1,5,0,0);
VLMMeas(140,170,350,380,NomLevel625_25,AbsTol_25,GLevel_25,Msg,VLMMeasOk); { 25 % window level}
writeln('   25% G-level: ',GLevel_25:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,10,0,0);
VLMMeas(140,170,350,380,NomLevel625_50,AbsTol_50,GLevel_50,Msg,VLMMeasOk); { 50 % window level}
writeln('   50% G-level: ',GLevel_50:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,15,0,0);
VLMMeas(140,170,350,380,NomLevel625_75,AbsTol_75,GLevel_75,Msg,VLMMeasOk); { 75 % window level}
writeln('   75% G-level: ',GLevel_75:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,20,0,0);
VLMMeas(140,170,350,380,NomLevel625_100,AbsTol_100,GLevel_100,Msg,VLMMeasOk); { 100 % window level}
writeln('  100% G-level: ',GLevel_100:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;


{------------------ B channel ------- Component ------------------}
write('Forbind B-kanal til VLM-input  -  tryk en tast');
WaitForAnyKey;
gotoxy(1,wherey); ClrEol;
SendToReg(1,5,0,0);
VLMMeas(140,170,350,380,NomLevel625_25,AbsTol_25,BLevel_25,Msg,VLMMeasOk); { 25 % window level}
writeln('   25% B-level: ',BLevel_25:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,10,0,0);
VLMMeas(140,170,350,380,NomLevel625_50,AbsTol_50,BLevel_50,Msg,VLMMeasOk); { 50 % window level}
writeln('   50% B-level: ',BLevel_50:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,15,0,0);
VLMMeas(140,170,350,380,NomLevel625_75,AbsTol_75,BLevel_75,Msg,VLMMeasOk); { 75 % window level}
writeln('   75% B-level: ',BLevel_75:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,20,0,0);
VLMMeas(140,170,350,380,NomLevel625_100,AbsTol_100,BLevel_100,Msg,VLMMeasOk); { 100 % window level}
writeln('  100% B-level: ',BLevel_100:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;



{------------------ R channel ------- Component ------------------}
write('Forbind R-kanal til VLM-input  -  tryk en tast');
WaitForAnyKey;
gotoxy(1,wherey); ClrEol;
SendToReg(1,5,0,0);
VLMMeas(140,170,350,380,NomLevel625_25,AbsTol_25,RLevel_25,Msg,VLMMeasOk); { 25 % window level}
writeln('   25% R-level: ',RLevel_25:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,10,0,0);
VLMMeas(140,170,350,380,NomLevel625_50,AbsTol_50,RLevel_50,Msg,VLMMeasOk); { 50 % window level}
writeln('   50% R-level: ',RLevel_50:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,15,0,0);
VLMMeas(140,170,350,380,NomLevel625_75,AbsTol_75,RLevel_75,Msg,VLMMeasOk); { 75 % window level}
writeln('   75% R-level: ',RLevel_75:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

SendToReg(1,20,0,0);
VLMMeas(140,170,350,380,NomLevel625_100,AbsTol_100,RLevel_100,Msg,VLMMeasOk); { 100 % window level}
writeln('  100% R-level: ',RLevel_100:6:1,' mV',Msg:MPos);
if (not VLMMeasOk) then
 CertifTestOk := false;

{------------------ GBR diff level ----- Component --------------------}

 BlueGreenDiff_25 := BLevel_25 - GLevel_25;
 BlueGreenDiff_25Ok := Abs(BlueGreenDiff_25) < DiffTol_25;
 RedGreenDiff_25 := RLevel_25 - GLevel_25;
 RedGreenDiff_25Ok := Abs(RedGreenDiff_25) < DiffTol_25;

 BlueGreenDiff_50 := BLevel_50 - GLevel_50;
 BlueGreenDiff_50Ok := Abs(BlueGreenDiff_50) < DiffTol_50;
 RedGreenDiff_50 := RLevel_50 - GLevel_50;
 RedGreenDiff_50Ok := Abs(RedGreenDiff_50) < DiffTol_50;

 BlueGreenDiff_75 := BLevel_75 - GLevel_75;
 BlueGreenDiff_75Ok := Abs(BlueGreenDiff_75) < DiffTol_75;
 RedGreenDiff_75 := RLevel_75 - GLevel_75;
 RedGreenDiff_75Ok := Abs(RedGreenDiff_75) < DiffTol_75;

 BlueGreenDiff_100 := BLevel_100 - GLevel_100;
 BlueGreenDiff_100Ok := Abs(BlueGreenDiff_100) < DiffTol_100;
 RedGreenDiff_100 := RLevel_100 - GLevel_100;
 RedGreenDiff_100Ok := Abs(RedGreenDiff_100) < DiffTol_100;


 write(' Blue - Green tracking  25%    ',BlueGreenDiff_25:5:1,' mV    ');
 if BlueGreenDiff_25Ok then writeln('OK') else writeln('FAIL',DiffTol_25:6:1,' mV');
 write(' Red  - Green tracking  25%    ',RedGreenDiff_25:5:1,' mV    ');
 if RedGreenDiff_25Ok then writeln('OK') else writeln('FAIL',DiffTol_25:6:1,' mV');

 write(' Blue - Green tracking  50%    ',BlueGreenDiff_50:5:1,' mV    ');
 if BlueGreenDiff_50Ok then writeln('OK') else writeln('FAIL',DiffTol_50:6:1,' mV');
 write(' Red  - Green tracking  50%    ',RedGreenDiff_50:5:1,' mV    ');
 if RedGreenDiff_50Ok then writeln('OK') else writeln('FAIL',DiffTol_50:6:1,' mV');

 write(' Blue - Green tracking  75%    ',BlueGreenDiff_75:5:1,' mV    ');
 if BlueGreenDiff_75Ok then writeln('OK') else writeln('FAIL',DiffTol_75:6:1,' mV');
 write(' Red  - Green tracking  75%    ',RedGreenDiff_75:5:1,' mV    ');
 if RedGreenDiff_75Ok then writeln('OK') else writeln('FAIL',DiffTol_75:6:1,' mV');

 write(' Blue - Green tracking 100%    ',BlueGreenDiff_100:5:1,' mV    ');
 if BlueGreenDiff_100Ok then writeln('OK') else writeln('FAIL',DiffTol_100:6:1,' mV');
 write(' Red  - Green tracking 100%    ',RedGreenDiff_100:5:1,' mV    ');
 if RedGreenDiff_100Ok then writeln('OK') else writeln('FAIL',DiffTol_100:6:1,' mV');

 GBRDiffOk := BlueGreenDiff_25Ok and RedGreenDiff_25Ok and
              BlueGreenDiff_50Ok and RedGreenDiff_50Ok and
              BlueGreenDiff_75Ok and RedGreenDiff_75Ok and
              BlueGreenDiff_100Ok and RedGreenDiff_100Ok;

 if (not GBRDiffOk) then
  begin
   CertifTestOk := false;
   TestFailed := true;
  end;

 if GenVer = M525 then
  St := WriteCOMPort(COMNo,'GKEY19,');   { setup=on }
 Delay(GKeyDelay);

 {------------------ Line freq ----- 625/525 --------------------}
  SelPatt(calPatt);
  Delay(1000);
  InitPM6669;
  write('Forbind counter probe til TP 4  -  tryk en tast');
  WaitForAnyKey;
  write(' .');
  ReadPM6669(LineFreq);              { 1250000 / 1258741.12 Hz }
  write(' .');
  ReadPM6669(LineFreq);
  LineFreq := LineFreq/80;           { 15625   / 15734.264 Hz }
  if GenVer = M525 then
    LineFreqOk := (Abs(LineFreq - (LineFreqNom525/80))) < LineFreqTolProdData
   else
    LineFreqOk := (Abs(LineFreq - (LineFreqNom625/80))) < LineFreqTolProdData;
  gotoxy(1,wherey); ClrEol;
  write('  Line freq: ',LineFreq:9:3,' Hz                  ');
  if LineFreqOk then
     writeln('OK')
   else
    begin
     writeln('FAIL');
     CertifTestOk := false;
    end;

 writeln;
 write('M†linger til test certificate:             ');
 if CertifTestOk then
  begin
   writeln('OK');
   writeln;
   MakeCertif(SyncGLevel,0,LineFreq,                          { Sync level, Subc freq, Line freq }
       GLevel_100,BlueGreenDiff_100,RedGreenDiff_100, { Green,BlueGreen,RedGreen 100% }
       GLevel_75,BlueGreenDiff_75,RedGreenDiff_75,    {   -       -         -     75% }
       GLevel_50,BlueGreenDiff_50,RedGreenDiff_50,    {   -       -         -     50% }
       GLevel_25,BlueGreenDiff_25,RedGreenDiff_25);   {   -       -         -     25% }

   CopyFileToLaserjet(true);
  end
  else
   begin
    writeln('FAIL');
    TestFailed := true;
   end;

end; { Component_Certif }


end.