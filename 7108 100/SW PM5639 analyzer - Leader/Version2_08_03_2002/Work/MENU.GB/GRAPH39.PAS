unit Graph39;
{
 Unit til brug i test og kalibrering af color sensor i PM5639
 Peter Frederiksen,  *  programmeret 911104

 980918   Philips logo fjernet. Philips rettet til ProteleVision Tech.
 990212   /90 serie V og /92,/93 serie III tilfõjet
 010108:  Translated to English: JK
}

INTERFACE

var
 Gd,Gm  : INTEGER;

Procedure InitialiseGraphics;
Procedure GraphTextXY(X,Y: WORD; s : STRING; Color : INTEGER);
Procedure TegnPM5639Sens(X1,Y1: WORD);
Procedure TegnPM5639Gen(X1,Y1: WORD);
Procedure TegnRamme(Msg:STRING);
Procedure HovedmenuSens;
Procedure MakeTestMenuSens;
Procedure HovedmenuGen;
Procedure MakeTestMenuGen;
procedure SubMenu;



IMPLEMENTATION
USES Dos,Graph,PepUtil,Crt;

Procedure InitialiseGraphics;
begin
   DetectGraph(Gd,Gm);
   Gm := EGAHi;
   InitGraph(Gd,Gm,'');
     IF Graphresult <> GrOK THEN
      begin
        Writeln('Graphics init result: ',GraphErrorMsg(GraphResult));
        Halt(1);
      end;
   SetActivePage(1);
   ClearDevice;
   SetVisualPage(1);
end;



Procedure GraphTextXY(X,Y: WORD; s : STRING; Color : INTEGER);
begin
 SetColor(Color);
 MoveTo(X,Y);
 OutText(s);
end;





Procedure TegnPM5639Sens(X1,Y1: WORD);
var  Firkant : ARRAY [1..4] OF Pointtype;

Procedure TegnPM5639Tast(X,Y: WORD; Col : BYTE);
CONST Width = 12;
      Height = 6;
begin
 MoveTo(X1 + X,Y1 + Y);
 Firkant[1].X := X1 + X;          Firkant[1].Y := Y1 + Y;
 Firkant[2].X := X1 + X + Width;  Firkant[2].Y := Y1 + Y;
 Firkant[3].X := X1 + X + Width;  Firkant[3].Y := Y1 + Y + Height;
 Firkant[4].X := X1 + X;          Firkant[4].Y := Y1 + Y + Height;
 SetFillStyle(1,Col);
 FillPoly(SizeOf(Firkant) DIV SizeOf(Pointtype),Firkant);
end;


begin
 SetColor(White);
 SetBkColor(Blue);
 SetLineStyle(SolidLn,0,NormWidth);
 Rectangle(X1,Y1,X1+90,Y1+200);    {kabinet}
 Firkant[1].X := X1 + 16;       Firkant[1].Y := Y1 + 20;
 Firkant[2].X := X1 + 16 + 58;  Firkant[2].Y := Y1 + 20;
 Firkant[3].X := X1 + 16 + 58;  Firkant[3].Y := Y1 + 20 + 80;
 Firkant[4].X := X1 + 16;       Firkant[4].Y := Y1 + 20 + 80;
 SetFillStyle(1,LightGray);
 FillPoly(SizeOf(Firkant) DIV SizeOf(Pointtype),Firkant);   {LCD display}
 SetColor(Black);
 MoveTo(X1+22,Y1+23);
 LineRel(0,35);                             {x-koordinat}
 Linerel(46,0);                             {y-koordinat}
 Rectangle(X1+42,Y1+38,X1+42+5,Y1+38+4);    {white ref}
 SetUserCharSize(10,15,10,15);
 SetTextStyle(DefaultFont,Horizdir,1);
 GraphTextXY(X1+22,Y1+62,'x=',Black);
 GraphTextXY(X1+22,Y1+69,'y=',Black);
 GraphTextXY(X1+22,Y1+78,'Y=',Black);

 SetColor(White);
 TegnPM5639Tast(16,122,LightGray);                {pil op}
 TegnPM5639Tast(39,122,LightGray);                {pil ned}
 TegnPM5639Tast(62,122,Red);                      {ENTER}
 TegnPM5639Tast(16,136,LightGray);                {RGB}
 TegnPM5639Tast(39,136,LightGray);                {xy Y}
 TegnPM5639Tast(62,136,LightGray);                {ZOOM}
 TegnPM5639Tast(16,150,LightGray);                {FUNCT}
 TegnPM5639Tast(39,150,LightGray);                {RECALL}
 TegnPM5639Tast(62,150,LightGray);                {STORE}
 TegnPM5639Tast(16,164,Green);                    {ON/OFF}
 TegnPM5639Tast(39,164,LightGray);                {ILLUM}
 TegnPM5639Tast(62,164,Blue);                     {RESET}

 Rectangle(X1+10,Y1+116,X1+10+70,Y1+116+75);    {Ramme om taster}
 SetTextStyle(DefaultFont,Horizdir,1);
 SetUserCharSize(10,12,10,12);
{GraphTextXY(X1+19,Y1+6,'  PTV  ',White);}
 GraphTextXY(X1+15,Y1+6,'DK-Audio',White);

 X1 := X1 - 220;                   {afstand til display unit}
 Y1 := Y1 + 165;
 MoveTo(X1,Y1+30);
 LineRel(-6,6);                    {sugeskive}
 LineRel(182,0);
 LineRel(-6,-6);
 MoveTo(X1,Y1);
 Rectangle(X1,Y1,X1+170,Y1+30);    {color sensor body}
 LineRel(0,-80);                   {hÜndtag}
 LineRel(30,5);
 LineRel(140,-30);
 LineRel(0,30);
 LineRel(-80,20);
 LineRel(0,55);
 LineRel(-10,0);
 LineRel(0,-52);
 LineRel(-45,10);
 LineRel(0,42);
 MoveRel(0,-42);
 LineRel(5,10);                      {trigger}
 LineRel(36,-8);
 LineRel(0,-10);

end; { TegnPM5639Sens }


Procedure TegnPM5639Gen(X1,Y1: WORD);
var  Firkant : ARRAY [1..4] OF Pointtype;

Procedure TegnPM5639Tast(X,Y: WORD; Col : BYTE);
CONST Width = 12;
      Height = 6;
begin
 MoveTo(X1 + X,Y1 + Y);
 Firkant[1].X := X1 + X;          Firkant[1].Y := Y1 + Y;
 Firkant[2].X := X1 + X + Width;  Firkant[2].Y := Y1 + Y;
 Firkant[3].X := X1 + X + Width;  Firkant[3].Y := Y1 + Y + Height;
 Firkant[4].X := X1 + X;          Firkant[4].Y := Y1 + Y + Height;
 SetFillStyle(1,Col);
 FillPoly(SizeOf(Firkant) DIV SizeOf(Pointtype),Firkant);
end;


begin
 SetColor(White);
 SetBkColor(Blue);
 SetLineStyle(SolidLn,0,NormWidth);
 Rectangle(X1,Y1,X1+90,Y1+200);    {kabinet}
 Firkant[1].X := X1 + 16;       Firkant[1].Y := Y1 + 20;
 Firkant[2].X := X1 + 16 + 58;  Firkant[2].Y := Y1 + 20;
 Firkant[3].X := X1 + 16 + 58;  Firkant[3].Y := Y1 + 20 + 80;
 Firkant[4].X := X1 + 16;       Firkant[4].Y := Y1 + 20 + 80;
 SetFillStyle(1,LightGray);
 FillPoly(SizeOf(Firkant) DIV SizeOf(Pointtype),Firkant);   {LCD display}
 SetColor(Black);
 SetTextStyle(SmallFont,Horizdir,1);
 SetUserCharSize(10,15,10,15);
 GraphTextXY(X1+22,Y1+22,'PM5639/82/83',Black);

 SetColor(White);
 TegnPM5639Tast(16,122,LightGray);                {LO LEVEL}
 TegnPM5639Tast(39,122,LightGray);                {HI LEVEL}
 TegnPM5639Tast(62,122,LightGray);                {PLUGE}
 TegnPM5639Tast(16,136,LightGray);                {C-BAR}
 TegnPM5639Tast(39,136,LightGray);                {X-HTC}
 TegnPM5639Tast(62,136,LightGray);                {STAIR}
 TegnPM5639Tast(16,150,LightGray);                {RED/NDL}
 TegnPM5639Tast(39,150,LightGray);                {up}
 TegnPM5639Tast(62,150,LightGray);                {down}
 TegnPM5639Tast(16,164,Red);                      {FORMAT}
 TegnPM5639Tast(39,164,Red);                      {RECALL}
 TegnPM5639Tast(62,164,Red);                      {STORE}
 Rectangle(X1+19,Y1+180,X1+19+8,Y1+180+4);      {ON}

 Rectangle(X1+10,Y1+116,X1+10+70,Y1+116+75);    {Ramme om taster}
 SetTextStyle(DefaultFont,Horizdir,1);
 SetUserCharSize(10,12,10,12);
 {GraphTextXY(X1+19,Y1+6,'  PTV  ',White);}
 GraphTextXY(X1+15,Y1+6,'DK-Audio',White);

end; { TegnPM5639Gen }






Procedure TegnRamme(Msg:STRING);
CONST X1 = 5;
      Y1 = 5;
      Y2 = 75;
begin
   ClearDevice;
   SetViewPort(0,0,GetMaxX,GetMaxY,ClipOn);
   SetColor(Yellow);
   SetBkColor(Blue);

   SetLineStyle(SolidLn,0,ThickWidth);
   MoveTo(X1,Y1);
   LineTo(X1,GetMaxY-Y1);
   LineTo(GetMaxX-X1,GetMaxY-Y1);
   LineTo(GetMaxX-X1,Y1);
   LineTo(X1,Y1);
   Line(X1,Y2,GetMaxX-X1,Y2);

   SetTextStyle(Triplexfont,Horizdir,5);
   SetUserCharSize(100,85,10,7);
   SetTextJustify(Centertext,CenterText);
   OutTextXY(320,30,Msg);
end; {TegnRamme}




Procedure HovedmenuSens;
CONST X1 = 5;
      Y1 = 5;
      Y2 = 75;
begin
   SetActivePage(0);
   ClearDevice;
   SetViewPort(0,0,GetMaxX,GetMaxY,ClipOn);
   SetColor(Yellow);
   SetBkColor(Blue);

   SetLineStyle(SolidLn,0,ThickWidth);
   MoveTo(X1,Y1);
   LineTo(X1,GetMaxY-Y1);
   LineTo(GetMaxX-X1,GetMaxY-Y1);
   LineTo(GetMaxX-X1,Y1);
   LineTo(X1,Y1);
   SetTextStyle(Triplexfont,Horizdir,5);
   SetUserCharSize(10,9,10,8);
{   OutTextXY(50,5,'ProTeleVision Technologies A/S');}
   OutTextXY(50,5,'        DK-Audio A/S          ');

   Dannebrog(40,200);
   TegnPM5639Sens(460,70);
   SetTextStyle(Triplexfont,Horizdir,5);
   SetUserCharSize(10,10,10,10);
   SetColor(Yellow);
   OutTextXY(100,290,'PM5639 Sensor Calibration');

   SetTextStyle(DefaultFont,Horizdir,1);
   SetUserCharSize(10,12,10,12);
   OutTextXY(500,330,'Press a key ...');
   SetVisualPage(0);
   WaitForAnyKey;
 end; {HovedmenuSens}

Procedure HovedmenuGen;
CONST X1 = 5;
      Y1 = 5;
      Y2 = 75;
begin
   SetActivePage(0);
   ClearDevice;
   SetViewPort(0,0,GetMaxX,GetMaxY,ClipOn);
   SetColor(Yellow);
   SetBkColor(Blue);

   SetLineStyle(SolidLn,0,ThickWidth);
   MoveTo(X1,Y1);
   LineTo(X1,GetMaxY-Y1);
   LineTo(GetMaxX-X1,GetMaxY-Y1);
   LineTo(GetMaxX-X1,Y1);
   LineTo(X1,Y1);
   SetTextStyle(Triplexfont,Horizdir,5);
   SetUserCharSize(10,9,10,8);
{   OutTextXY(50,5,'ProTeleVision Technologies A/S');}
   OutTextXY(50,5,'        DK-Audio A/S          ');

   Dannebrog(85,180);
   TegnPM5639Gen(460,70);
   SetTextStyle(Triplexfont,Horizdir,5);
   SetUserCharSize(10,10,10,10);
   SetColor(Yellow);
   OutTextXY(80,290,'PM5639 Generator Calibration');

   SetTextStyle(DefaultFont,Horizdir,1);
   SetUserCharSize(10,12,10,12);
   OutTextXY(500,332,'Press s key ...');
   SetVisualPage(0);
   WaitForAnyKey;
 end; {HovedmenuGen}


Procedure MakeTestMenuSens;
CONST XPos = 100;
begin
   SetActivePage(1);
   ClearDevice;
   TegnRamme('PM5639 Sensor Calibration');
   SetColor(White);
   SetTextJustify(LeftText,TopText);
   SetUserCharSize(10,12,10,16);
   GraphTextXY(XPos,80, 'F1:   PM5639/90 series II',White);
   GraphTextXY(XPos,100,'F2:   PM5639/90 series III',White);
   GraphTextXY(XPos,120,'F3:   PM5639/90 series IV',White);
   GraphTextXY(XPos,140,'F4:   PM5639/90 series V',White);
   GraphTextXY(XPos,170,'F5:   PM5639/92 series I',White);
   GraphTextXY(XPos,190,'F6:   PM5639/92 series II',White);
   GraphTextXY(XPos,210,'F7:   PM5639/92 series III',White);
   GraphTextXY(XPos,240,'F8:   PM5639/93 series II',White);
   GraphTextXY(XPos,260,'F9:   PM5639/93 series III',White);
   GraphTextXY(XPos,290,'F10:  Other sensors ',White);
   GraphTextXY(XPos,310,'ESC:  DOS',White);
   SetVisualPage(1);
end;



Procedure MakeSubMenu;
CONST XPos = 100;
begin
   SetActivePage(1);
   ClearDevice;
   TegnRamme('PM5639 Sensor Calibration');
   SetColor(White);
   SetTextJustify(LeftText,TopText);
   SetUserCharSize(10,12,10,16);
   GraphTextXY(XPos,80, 'F1:  PM5639/916 BrÅgge',White);
   GraphTextXY(XPos,105,'F2:  PM5639/91 Tri Light Sensor',White);
   GraphTextXY(XPos,130,'F3:  PM5639/02 Barco',White);
   GraphTextXY(XPos,180,'F4:  Color probe PESA',White);
   GraphTextXY(XPos,205,'F5:  Color probe VISTEK',White);
   GraphTextXY(XPos,250,'F8:  COM5639  (4800 baud)',White);
   GraphTextXY(XPos,275,'F9:  COM5639  (9600 baud)',White);
   GraphTextXY(XPos,305,'ESC: Return',White);
   SetVisualPage(1);
end;

procedure SubMenu;
var FKey  : byte;
    DosErr : integer;
    Slut   : boolean;
begin
 Slut := false;
 MakeSubMenu;
 repeat
    EmptyKeyboardBuffer;
    FKey := ScanCode;
      CASE FKey OF
       F1: Exec('\BIN\CRT39.EXE','916');    {/916 Industrial sensor BrÅgge}
       F2: Exec('\BIN\PROJ39.EXE ','');     {5639/91 tri light sensor}
       F3: Exec('\BIN\CRT39.EXE','2');      {5639/02 Barco option}
       F4: Exec('\BIN\PESA39.EXE','PESA');
       F5: Exec('\BIN\PESA39.EXE','VISTEK');
       F8: Exec('\BIN\COM5639.EXE','');
       F9: Exec('\BIN\COM5639.EXE','96');

   Escape: Slut := TRUE;

      end; {case}

 DosErr := DosError;
 until FKey in [F1..F5,F8,F9,Escape];
end;

Procedure MakeTestMenuGen;
CONST XPos = 100;
begin
   SetActivePage(1);
   ClearDevice;
   TegnRamme('PM5639 Generator Calibration');
   SetColor(White);
   SetTextJustify(LeftText,TopText);
   SetUserCharSize(10,10,10,12);
   GraphTextXY(XPos,80, 'F1:   PM5639/83  PAL',White);
   GraphTextXY(XPos,110,'F2:   PM5639/83  NTSC',White);
   GraphTextXY(XPos,140,'F3:   PM5639/82  G625',White);
   GraphTextXY(XPos,170,'F4:   PM5639/82  M525',White);
   GraphTextXY(XPos,240,'F9:   COM82.EXE',White);
   GraphTextXY(XPos,270,'F10:  SHUTDOWN',White);
   GraphTextXY(XPos,300,'ESC:  DOS',White);
   SetVisualPage(1);
end; { MakeTestMenuGen }






begin

end.
