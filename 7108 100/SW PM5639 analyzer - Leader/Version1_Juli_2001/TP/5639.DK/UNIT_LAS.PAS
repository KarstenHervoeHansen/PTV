Unit Unit_Las;

{
  Unit laserprinter

  Philips TV Test Equipment A/S
  Peter Frederiksen, 941017

  Unit to print on HP laserjet printers.
  Used Printer Control Language = PCL 4.

  Se "HP LaserJet h†ndbogen", Teknisk Forlag 1993, ISBN 87-571-1430-7.
  Se manualer for diverse HP LaserJet printere.

  HP LaserJet:                       PCL 3
  HP LaserJet+ to HP Laserjet IIP:   PCL 4
  HP Laserjet III and up:            PCL 5
  Program tested on Laserjet4 - ok.

}

Interface

uses peputil;

var FLas  : text;
    FLasName : string;

const
 { 'Á' = 'ñ' in symbol set 7J}
 { '²' = 'æ' in symbol set 7J}

 PaperMargin   = 5.1; { mm }
 LeftMargin    = 5.1; { mm }

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

Procedure ResetPrinter;
Procedure SetLandscape;
Procedure SetPortrait;
Procedure SetUnderlineOn;
Procedure SetUnderlineOff;
Procedure EjectPage;
Procedure SetFont(FontNo : byte );
Procedure SetLeftMargin(ColNo : word);
Procedure MoveToXY(XPos, YPos : real);
Procedure HorLine(XPos,YPos,Thicknes,Length : real);
Procedure VerLine(XPos,YPos,Thicknes,Length : real);



Implementation


Procedure ResetPrinter;
begin
 write(FLas,ESC+'E');
end;


{ ------ PAGE ORIENTATION ----------------------------------- }
Procedure SetLandscape;
begin
 write(FLas,ESC+'&l1O');
end;

Procedure SetPortrait;
begin
 write(FLas,ESC+'&l0O');
end;

{ ------ UNDERLINE ------------------------------------------ }
Procedure SetUnderlineOn;
begin
 write(FLas,ESC+'&dD');
end;
Procedure SetUnderlineOff;
begin
 write(FLas,ESC+'&d@');
end;

{ ------ EJECT PAGE ---------------------------------------- }
Procedure EjectPage;
begin
 write(FLas,ESC+'&l0H');
end;

{ ------ FONTS ---------------------------------------------- }
Procedure SetFont(FontNo : byte );
begin
 write(FLas,Font[FontNo]);
end;

{ ------ MARGINS -------------------------------------------- }
Procedure SetLeftMargin(ColNo : word);
begin
 write(FLas,#27+#38+#97+IntToStr(ColNo)+#76);
end;

{ ------ XY Position ---------------------------------------- }
Procedure MoveToXY(XPos, YPos : real);
var X,Y  : string[5];
begin
 if XPos < PaperMargin then XPos := PaperMargin;
 if YPos < PaperMargin then YPos := PaperMargin;
 X := IntToStr(Trunc(((XPos-PaperMargin)/25.4*DotPerInch)));
 Y := IntToStr(Trunc(((YPos-PaperMargin)/25.4*DotPerInch)));
 write(FLas,ESC+'*p'+X+'X');
 write(FLas,ESC+'*p'+Y+'Y');
end;

{ ------ Hor & Ver Lines -------------------------------------- }
Procedure HorLine(XPos,YPos,Thicknes,Length : real);
var L,T  : string[5];
begin
 MoveToXY(XPos,YPos);
 T := IntToStr(Trunc(Thicknes/25.4*DotPerInch));
 L := IntToStr(Trunc(Length/25.4*DotPerInch));
 write(FLas,ESC+'*c'+L+'a'+T+'b0P');
end;

Procedure VerLine(XPos,YPos,Thicknes,Length : real);
var L,T  : string[5];
begin
 MoveToXY(XPos,YPos);
 T := IntToStr(Trunc(Thicknes/25.4*DotPerInch));
 L := IntToStr(Trunc(Length/25.4*DotPerInch));
 write(FLas,ESC+'*c'+T+'a'+L+'b0P');
end;



end.