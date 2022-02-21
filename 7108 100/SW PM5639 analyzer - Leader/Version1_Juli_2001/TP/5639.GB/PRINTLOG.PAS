Program PrintLog;
(*
 PRINTLOG  <file > [LPTx]
 Programmet udskriver file (SENSOR.LOG) til en HP Laser Jet printer,
 som p† netv‘rket er tilsluttet LPTx. Hvis LPTx er udeladt v‘lges LPT2.
 Hvis file ikke findes, stopper programmet.
 Programmet er skrevet til en HP LaserJet II (PCL4), og fungerer derfor
 ogs† p† en Laser Jet III og IV.
 HUSK: Ved rettelser skal konstanten SWRev opdateres.

 Philips TV Test, Peter Frederiksen, 950227.

 Rettelser:
 950227   V1.00
 950328   V1.10    Udskrift ‘ndret fra portrait til landscape, idet
                   EBU-m†linger og bem‘rkninger er tilf›jet i filen
                   SENSOR.LOG.

*)


Uses
 Crt,
 PepUtil;

const
 SWRev          = 'V1.10';
 PaperMargin    = 5.1; { mm }
 LeftMargin     = 20;  { mm }
 DotPerInch     = 300;
 NoLinesPerPage = 30;

 Courier17      = 1;
 Courier10      = 2;
 Courier10Bold  = 3;
 MainLine       = 'PM5639 color sensor calibration                                              Philips TV Test Equipment A/S';

 Font : array[Courier17..Courier10Bold] of string[50] =
    (ESC+'(10U'+ESC+'(sp16.66h8.5vs0b0T',    { 1  Font = Lineprinter 16.67 }
     ESC+'(10U'+ESC+'(sp10h12vs0b3T',        { 2  Font = Courier 10        }
     ESC+'(10U'+ESC+'(sp10h12vs3b3T');       { 3  Font = Courier 10 Bold   }


var
 F,P         : text;
 TopText     : array[1..3] of string;
 Line        : string;
 PageNo,
 NoLines     : byte;
 FName,
 OutDev      : string;



Procedure ResetPrinter;
begin
 write(P,ESC+'E');
end;

Procedure EjectPage;
begin
 write(P,ESC+'&l0H');
end;

{ ------ ORIENTATION ------------------------------------------- }
Procedure SetLandscape;
begin
 write(P,ESC+'&l1O');
end;

Procedure SetPortrait;
begin
 write(P,ESC+'&l0O');
end;

{ ------ MARGIN --------------------------------------------- }
Procedure SetLeftMargin(ColNo : word);
begin
 write(P,ESC+'&a'+IntToStr(ColNo)+'L');
end;

{ ------ FONTS ---------------------------------------------- }
Procedure SetFont(FontNo : byte );
begin
 write(P,Font[FontNo]);
end;

{ ------ POSITION ------------------------------------------- }
Procedure MoveToXY(XPos, YPos : real); { XPos,YPos in millimeters}
var X,Y  : string[5];
begin
 if XPos < PaperMargin then XPos := PaperMargin;
 if YPos < PaperMargin then YPos := PaperMargin;
 X := IntToStr(Trunc(((XPos-PaperMargin)/25.4*DotPerInch)));
 Y := IntToStr(Trunc(((YPos-PaperMargin)/25.4*DotPerInch)));
 write(P,ESC+'*p'+X+'X');
 write(P,ESC+'*p'+Y+'Y');
end;

Procedure WriteBottomText;
begin
 MoveToXY(9,187);
 write(P,'-   = measurement data as above');
 MoveToXY(9,190);
 write(P,'*   = filter not changed');
 MoveToXY(9,193);
 write(P,'ref = Ref sensor I (KU000004)');
 MoveToXY(270,193);
 write(P,'side ',PageNo);
 EjectPage;
end;






begin

 if Paramcount = 0 then
  begin
   write(Bell,'PRINTLOG ',SWRev);
   writeln('     Print file not given');
   writeln('PRINTLOG  <file>  [LPTx]');
   writeln('          LPTx = printerport  (default = LPT2)');
   halt;
  end;

 if Paramcount = 1 then
  begin
   FName := ParamStr(1);
   OutDev := 'LPT2';
  end;

 if Paramcount = 2 then
  begin
   FName := ParamStr(1);
   OutDev := ParamStr(2);
  end;


 Assign(F,FName);
 {$I-}
 Reset(F);
 {$I+}
 if IOResult <> 0 then
  begin
   writeln(Bell,'File "',FName,'" not found - program is stopped');
   halt;
  end;


 Assign(P,OutDev);
 {$I-}
 Rewrite(P);
 {$I+}
 if IOResult <> 0 then
  begin
   writeln(Bell,'Printerport "',OutDev,'" not opened - program is stopped');
   halt;
  end;

 ResetPrinter;
 SetLandscape;
 SetFont(Courier10Bold);
 SetLeftMargin(2);
 writeln(P);
 writeln(P,MainLine);
 writeln(P);

 SetFont(Courier17);

 PageNo := 1;
 NoLines := 2;

 while not (Eof(F)) do
  begin
   Readln(F,Line);
   if pos('KU   ',Line) = 1 then
    TopText[1] := Line
    else
   if pos('                  x   y   Y',Line) = 1 then
    TopText[2] := Line
    else
   if pos('--------',Line) = 1 then
    TopText[3] := Line;
   writeln(P,Line);
   Inc(NoLines);

    if Nolines >= (NoLinesPerPage + 5) then
     begin
      WriteBottomText;
      Inc(PageNo);
      SetFont(Courier10Bold);
      writeln(P,MainLine);
      writeln(P);
      SetFont(Courier17);
      writeln(P,TopText[1]);
      writeln(P,TopText[2]);
      writeln(P,TopText[3]);
      NoLines := 5;
     end;

  end;

 WriteBottomText;
 Close(F);
 Close(P);


 if IOResult = 0 then
  begin
   writeln('PRINTLOG ',SWRev,':    file "',FName,'" send to "',OutDev,'"');
  end;

end.
