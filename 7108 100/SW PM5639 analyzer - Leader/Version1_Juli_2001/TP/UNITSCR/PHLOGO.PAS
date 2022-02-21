UNIT PhLogo;
{ PHILIPS Start Up screen
  Peter Frederiksen , P-DPE, 930208

  Serial number: EGA, VGA: text som pixels
                 CGA: text via OutTextXY

  Unit for Turbo Pascal 7.0
}

INTERFACE

VAR
 KUWidth,                       {Width of KU in pixels}
 KUHeight,                      {Height of KU in pixels}
 MaxX, MaxY          : WORD;

PROCEDURE StartUpScreen(KUNo,SWName,DevDept,Year : STRING);

PROCEDURE WriteKU(KUNo : STRING);

PROCEDURE InitLogoGraph;


IMPLEMENTATION
USES BGIDriv,BGIFont,Graph;


CONST
 KUPixel  : ARRAY[0..2000] OF CHAR = 'QQQQQQQQQQQQQQQQQQQQQQQQQQQ';


VAR
 P : POINTER;
 PixFile   : FILE;
 NumRd,

 StartX,
 StartY,
 PhHigh,                        {Philips Height}
 LettW,                         {LettW=LetterWidth}
 HH,                            {HH=H-Height}
 HW,                            {HW=H-Width}
 LH,                            {LH=L-Height}
 Size,                          {antal pixel KU nummer}
 FillColor,
 BkColor             : WORD;
 ErrCode,
 MaxColor,
 GrDriver,
 GrMode              : INTEGER;

PROCEDURE WriteEGALetterP (X,Y: WORD);
BEGIN
 MoveTo(X+LettW+11,Y);
 LineRel(-LettW-11,0);
 LineRel(0,PhHigh);
 LineRel(LettW,0);
 LineRel(0,-13);
 LineRel(11,0);
 Arc(GetX,GetY-23,270,90,30);
 Arc(GetX-2,GetY-23,270,90,13);
 MoveTo(X+LettW+9,Y+13);
 LineRel(-8,0);
 LineRel(0,20);
 LineRel(8,0);
 FloodFill(X+1,Y+1,FillColor);
END;

PROCEDURE WriteCGALetterP (X,Y: WORD);
BEGIN
 MoveTo(X+LettW+5,Y);
 LineRel(-LettW-5,0);
 LineRel(0,PhHigh);
 LineRel(LettW,0);
 LineRel(0,-9);
 LineRel(5,0);
 Arc(GetX,GetY-12,270,90,15);
 Arc(GetX-1,GetY-12,270,90,8);
 MoveTo(X+LettW+4,Y+6);
 LineRel(-4,0);
 LineRel(0,12);
 LineRel(4,0);
 FloodFill(X+1,Y+1,FillColor);
END;


PROCEDURE WriteLetterH (X,Y: WORD);
BEGIN
 MoveTo(X,Y);
 LineRel(0,PhHigh);
 LineRel(LettW,0);
 LineRel(0,-HH);
 LineRel(HW,0);
 LineRel(0,HH);
 LineRel(LettW,0);
 LineRel(0,-PhHigh);
 LineRel(-LettW,0);
 LineRel(0,HH);
 LineRel(-HW,0);
 LineRel(0,-HH);
 LineRel(-LettW,0);
 FloodFill(X+1,Y+1,FillColor);
END;

PROCEDURE WriteLetterI (X,Y: WORD);
BEGIN
 MoveTo(X,Y);
 Rectangle(X,Y,X+LettW,Y+PhHigh);
 FloodFill(X+1,Y+1,FillColor);
END;

PROCEDURE WriteLetterL (X,Y: WORD);
BEGIN
 MoveTo(X,Y);
 LineRel(0,PhHigh);
 LineRel(2*LettW,0);
 LineRel(0,-LH);
 LineRel(-LettW,0);
 LineRel(0,-PhHigh+LH);
 LineRel(-LettW,0);
 FloodFill(X+1,Y+1,FillColor);
END;


PROCEDURE WriteEGALetterS (X,Y: WORD);
BEGIN
 MoveTo(X,Y);
 LineRel(-6,-1);      {A}
 LineRel(-19,0);
 LineRel(-11,2);
 LineRel(-5,3);
 LineRel(-3,3);
 LineRel(-2,5);       {til B}
 LineRel(0,8);
 LineRel(2,4);
 LineRel(4,4);
 LineRel(6,3);
 LineRel(20,8);       {til C}
 LineRel(0,3);
 LineRel(-2,3);
 LineRel(-6,2);
 LineRel(-8,0);
 LineRel(-15,-2);      {til D}
 LineRel(0,13);
 LineRel(8,2);
 LineRel(18,0);
 LineRel(11,-2);
 LineRel(8,-4);
 LineRel(4,-5);
 LineRel(1,-3);        {til E}
 LineRel(0,-9);
 LineRel(-3,-4);
 LineRel(-4,-3);
 LineRel(-18,-8);
 LineRel(-5,-2);
 LineRel(-2,-2);      {til F}
 LineRel(0,-3);
 LineRel(2,-2);
 LineRel(5,-1);
 LineRel(12,0);
 LineRel(9,2);
 LineRel(0,-13);       {til A}
 FloodFill(X-3,Y+3,FillColor);
END;

PROCEDURE WriteCGALetterS (X,Y: WORD);
BEGIN
 MoveTo(X,Y);
 LineRel(-3,-1);      {A}
 LineRel(-10,0);
 LineRel(-5,1);
 LineRel(-2,1);
 LineRel(-2,2);
 LineRel(-2,4);       {til B}
 LineRel(0,4);
 LineRel(1,3);
 LineRel(2,2);
 LineRel(12,5);
 LineRel(1,1);       {til C}
 LineRel(0,3);
 LineRel(-1,1);
 LineRel(-5,1);
 LineRel(-3,0);
 LineRel(-6,-1);      {til D}
 LineRel(0,7);
 LineRel(3,1);
 LineRel(8,0);
 LineRel(5,-1);
 LineRel(4,-2);
 LineRel(2,-2);
 LineRel(1,-3);        {til E}
 LineRel(0,-5);
 LineRel(-1,-3);
 LineRel(-3,-3);
 LineRel(-7,-3);
 LineRel(-3,-1);
 LineRel(-1,-1);      {til F}
 LineRel(0,-2);
 LineRel(1,-1);
 LineRel(3,-1);
 LineRel(6,0);
 LineRel(5,1);
 LineRel(0,-7);       {til A}
 FloodFill(X-3,Y+3,FillColor);
END;

PROCEDURE WriteKU(KUNo : STRING);
BEGIN
 SetTextStyle(SmallFont,HorizDir,3);  {18}
 SetUserCharSize(20*MaxX DIV 640,10,(15*MaxY DIV 350),10);
 SetTextJustify(CenterText,TopText);
 KUWidth := TextWidth(KUNo);
 KUHeight := TextHeight(KUNo);
 OutTextXY(MaxX DIV 2,MaxY * 45 DIV 100,KUNo);
END;




PROCEDURE WriteKUPixel(KUNo : STRING);
VAR X1,Y1 : WORD;
BEGIN
{ Assign(PixFile,'KU_PIXEL.DAT');
 Reset(PixFile,1);
 GetMem(R,1514);
 BlockRead(PixFile,R^,1514,NumRd);
 Close(PixFile);}
 New(P);
 P := Addr(KUPixel);
 X1 := (MaxX - 256) DIV 2;     {256 = width of text}
 Y1 := MaxY * 45 DIV 100;
 PutImage(X1,Y1,P^,NormalPut);
END;



PROCEDURE WriteSWName(SW : STRING);
VAR x,y,tw : WORD;
BEGIN
 SetTextStyle(SmallFont,HorizDir,3);
 SetUserCharSize(30*MaxX DIV 640,10,(20*MaxY DIV 350)+1,10);
 SetTextJustify(CenterText,TopText);
 x := (MaxX DIV 2) + 10;
 y := MaxY * 66 DIV 100;
 tw := TextWidth(SW);
 OutTextXY(x,y,SW);
 IF GrDriver = CGA THEN
  Rectangle(x-(tw DIV 2)-15,y+3,x-(tw DIV 2)-7,y+10)
 ELSE
 IF GrDriver IN [EGA,EGA64,VGA] THEN
  Rectangle(x-(tw DIV 2)-27,y+6,x-(tw DIV 2)-10,y+18);
 FloodFill(x-(tw DIV 2)-12,y+8,FillColor);
END;

PROCEDURE WriteDev(Dev,Year : STRING);
BEGIN
 SetTextStyle(SmallFont,HorizDir,2);
 IF GrDriver = CGA THEN
  SetUserCharSize(10,12,10,10)
 ELSE
 IF GrDriver IN [EGA,EGA64,VGA] THEN
  SetUserCharSize(10,10,12,10);
 SetTextJustify(CenterText,TopText);
 OutTextXY(MaxX DIV 2,MaxY * 75 DIV 100,Dev);                       {EGA=265}
 OutTextXY(MaxX DIV 2,MaxY * 80 DIV 100,'(c) copyright Philips Danmark A/S,' + Year);   {EGA=278}
 OutTextXY(MaxX DIV 2,MaxY * 85 DIV 100,'All rights reserved');
END;

PROCEDURE WriteStartCommand(Command : STRING);
BEGIN
 SetTextStyle(SmallFont,HorizDir,3);
 SetUserCharSize(30*MaxX DIV 640,10,(20*MaxY DIV 350)+1,10);
 SetTextJustify(CenterText,TopText);
 OutTextXY(MaxX DIV 2,MaxY * 90 DIV 100,Command);         {315}
END;


PROCEDURE GraphAbort(Msg:STRING; Err:INTEGER);
BEGIN
 Writeln(Msg,': ',GraphErrorMsg(Err));
 Halt(1);
END;


PROCEDURE StartUpScreen(KUNo,SWName,DevDept,Year : STRING);
BEGIN
 InitLogoGraph;
 SetVisualPage(1);  {1}
 SetActivePage(0);  {0}
 ClearDevice;
 SetLineStyle(SolidLn,0,NormWidth);
 SetFillStyle(SolidFill,FillColor);

 IF GrDriver IN [EGA,EGA64,VGA] THEN
  BEGIN
   WriteEGALetterP (StartX,StartY);
   WriteLetterH (StartX+72,StartY);
   WriteLetterI (StartX+156,StartY);
   WriteLetterL (StartX+198,StartY);
   WriteLetterI (StartX+251,StartY);
   WriteEGALetterP (StartX+291,StartY);
   WriteEGALetterS (StartX+408,StartY);
  END
 ELSE
 IF GrDriver = CGA THEN
  BEGIN
   WriteCGALetterP (StartX,StartY);
   WriteLetterH (StartX+36,StartY);
   WriteLetterI (StartX+78,StartY);
   WriteLetterL (StartX+99,StartY);
   WriteLetterI (StartX+124,StartY);
   WriteCGALetterP (StartX+144,StartY);
   WriteCGALetterS (StartX+203,StartY);
  END;

 IF GrDriver = CGA THEN
  WriteKU(KUNo)            {CGA}
   ELSE
  WriteKUPixel(KUNo);      {EGA, VGA}
 WriteSWName(SWName);
 WriteDev(DevDept,Year);
 WriteStartCommand('Press ENTER to continue');
 SetVisualPage(0);

 Readln;

 CloseGraph;
 RestoreCRTMode;
END;

PROCEDURE InitLogoGraph;
BEGIN
 IF RegisterBGIDriver(@CGADriverProc) < 0 THEN GraphAbort('CGA',GraphResult);
 IF RegisterBGIDriver(@EGAVGADriverProc) < 0 THEN GraphAbort('EGA/VGA',GraphResult);
 IF RegisterBGIFont(@SmallFontProc)      < 0 THEN GraphAbort('SmallFont',GraphResult);
 IF RegisterBGIFont(@SansSerifFontProc)    < 0 THEN GraphAbort('SansSerifFont',GraphResult);
(*
 FontNo:= RegisterBGIFont(@TscrFontProc);
 IF FontNo < 0 THEN GraphAbort('TscrFont');
 FontNo := InstallUserFont('tscr');           {install an *.obj file}
*)

 DetectGraph(GrDriver,GrMode);
 IF GrDriver < 0 THEN
 BEGIN
  RestoreCRTMode;
  GraphAbort('Graphic error detected',GraphResult);
  Halt(1);
 END;

 {GrDriver := CGA;}            {for test of CGA display }
 IF GrDriver = CGA THEN
  BEGIN
  GrMode := CGAC1;            {CGA:   320x200 pixels}
  FillColor := 3;             {CGAC0: 1=LightGreen  2=LightRed  3=LightYellow  }
  BkColor := Black;           {CGAC1: 1=Cyan  2=Magenta  3=white  }
  StartX  := 56;              {CGAC2: 1=Green  2=Red  3=Brown  }
  StartY  := 26;              {CGAC3: 1=Cyan  2=Magenta  3=white  }
  PhHigh  := 33;
  HH := 13;
  HW := 14;
  LettW := 8;
  LH := LettW - 2;
  END
 ELSE
 IF GrDriver IN [EGA,EGA64,VGA] THEN
  BEGIN
  GrDriver := EGA;
  GrMode := EGAHi;            {mode=640x350   color=16 colors}
  FillColor := Blue;
  BkColor := LightGray;
  StartX  := 112;
  StartY  := 47;
  PhHigh  := 59;
  HH := 23;
  HW := 24;
  LettW := 19;
  LH := LettW - 7;
  END
 ELSE
  Halt;

 InitGraph(GrDriver,GrMode,'');
 ErrCode := GraphResult;
 IF ErrCode <> GrOk THEN
 BEGIN
  RestoreCRTMode;
  GraphAbort('Init graphics error',ErrCode);
  Halt(1);
 END;
 Maxcolor := GetMaxColor;
 MaxX := GetMaxX;
 MaxY := GetMaxY;
 SetColor(FillColor);
 SetBkColor(BkColor);
END;  {InitLogoGraph}


BEGIN

END.
