unit PhLogo;
{ PTV Start Up screen
  Peter Frederiksen , P-DPE, 930208

  Serial number: EGA, VGA: text som pixels

  Unit for Turbo Pascal 7.0

Retttelser:
 981026    PHILIPS erstattet af PTV. CGA grafik udg†et.
 010419    Copyright changed to DK-Audio: JK
}

interface

var
 KUWidth,                       {Width of KU in pixels}
 KUHeight,                      {Height of KU in pixels}
 MaxX, MaxY          : WORD;

Procedure StartUpScreen(KUNo,SWName,DevDept,Year : string);

Procedure WriteKU(KUNo : string);

Procedure InitLogoGraph;


implementation
uses BGIDriv,BGifont,Graph;


const
 KUPixel  : array[0..2000] of CHAR = 'QQQQQQQQQQQQQQQQQQQQQQQQQQQ';


var
 P : POINTER;
 PixFile   : file;
 NumRd,

 StartX,
 StartY,
 PTVHigh,                       {PTV Height}
 LettW,                         {LettW=LetterWidth}
 Size,                          {antal pixel KU nummer}
 FillColor,
 BkColor             : WORD;
 ErrCode,
 MaxColor,
 GrDriver,
 GrMode              : INTEGER;

Procedure WriteEGALetterP (X,Y: WORD);
begin
 MoveTo(X+LettW+11,Y);
 LineRel(-LettW-11,0);
 LineRel(0,PTVHigh);
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
end;


Procedure WriteLetterT (X,Y: WORD);
begin
 MoveTo(X,Y);
 LineRel(0,12);
 LineRel(22,0);
 LineRel(0,PTVHigh-12);
 LineRel(18,0);
 LineRel(0,-(PTVHigh-12));
 LineRel(22,0);
 LineRel(0,-12);
 LineRel(-(18+22+22),0);
 FloodFill(X+2,Y+2,FillColor);
end;

Procedure WriteLetterV (X,Y: WORD);
begin
 MoveTo(X,Y);
 LineRel(27,PTVHigh);
 LineRel(12,0);
 LineRel(27,-PTVHigh);
 LineRel(-21,0);
 LineRel(-12,31);
 LineRel(-12,-31);
 LineRel(-21,0);
 FloodFill(X+2,Y+2,FillColor);
end;


Procedure WriteKU(KUNo : string);
begin
 SetTextStyle(SmallFont,HorizDir,3);  {18}
 SetUserCharSize(20*MaxX div 640,10,(15*MaxY div 350),10);
 SetTextJustify(CenterText,TopText);
 KUWidth := TextWidth(KUNo);
 KUHeight := TextHeight(KUNo);
 OutTextXY(MaxX div 2,MaxY * 45 div 100,KUNo);
end;




Procedure WriteKUPixel(KUNo : string);
var X1,Y1 : WORD;
begin
{ Assign(PixFile,'KU_PIXEL.DAT');
 Reset(PixFile,1);
 GetMem(R,1514);
 BlockRead(PixFile,R^,1514,NumRd);
 Close(PixFile);}
 New(P);
 P := Addr(KUPixel);
 X1 := (MaxX - 256) div 2;     {256 = width of text}
 Y1 := MaxY * 45 div 100;
 PutImage(X1,Y1,P^,NormalPut);
end;



Procedure WriteSWName(SW : string);
var x,y,tw : WORD;
begin
 SetTextStyle(SmallFont,HorizDir,3);
 SetUserCharSize(30*MaxX div 640,10,(20*MaxY div 350)+1,10);
 SetTextJustify(CenterText,TopText);
 x := (MaxX div 2) + 10;
 y := MaxY * 66 div 100;
 tw := TextWidth(SW);
 OutTextXY(x,y,SW);
 if GrDriver in [EGA,EGA64,VGA] then
  Rectangle(x-(tw div 2)-27,y+6,x-(tw div 2)-10,y+18);
 FloodFill(x-(tw div 2)-12,y+8,FillColor);
end;

Procedure WriteDev(Dev,Year : string);
begin
 SetTextStyle(SmallFont,HorizDir,2);
 if GrDriver in [EGA,EGA64,VGA] then
  SetUserCharSize(10,10,12,10);
 SetTextJustify(CenterText,TopText);
 OutTextXY(MaxX div 2,MaxY * 75 div 100,Dev);                       {EGA=265}
 OutTextXY(MaxX div 2,MaxY * 80 div 100,'(c) copyright    DK - AUDIO A / S,    ' + Year);   {EGA=278}
 OutTextXY(MaxX div 2,MaxY * 85 div 100,'All rights reserved');
end;

Procedure WriteStartCommand(Command : string);
begin
 SetTextStyle(SmallFont,HorizDir,3);
 SetUserCharSize(30*MaxX div 640,10,(20*MaxY div 350)+1,10);
 SetTextJustify(CenterText,TopText);
 OutTextXY(MaxX div 2,MaxY * 90 div 100,Command);         {315}
end;


Procedure GraphAbort(Msg:string; Err:INTEGER);
begin
 Writeln(Msg,': ',GraphErrorMsg(Err));
 Halt(1);
end;


Procedure StartUpScreen(KUNo,SWName,DevDept,Year : string);
begin
 InitLogoGraph;
 SetVisualPage(1);  {1}
 SetActivePage(0);  {0}
 ClearDevice;
 SetLineStyle(SolidLn,0,NormWidth);
 SetFillStyle(SolidFill,FillColor);

 WriteEGALetterP (StartX,StartY);
 WriteLetterT (StartX+72,StartY);
 WriteLetterV (StartX+152,StartY);

 WriteKUPixel(KUNo);      {EGA, VGA}
 WriteSWName(SWName);
 WriteDev(DevDept,Year);
 WriteStartCommand('Press ENTER to continue');
 SetVisualPage(0);

 Readln;

 CloseGraph;
 RestoreCRTMode;
end;

Procedure InitLogoGraph;
begin
 if RegisterBGIDriver(@CGADriverProc) < 0 then GraphAbort('CGA',GraphResult);
 if RegisterBGIDriver(@EGAVGADriverProc) < 0 then GraphAbort('EGA/VGA',GraphResult);
 if RegisterBGifont(@SmallFontProc)      < 0 then GraphAbort('SmallFont',GraphResult);
 if RegisterBGifont(@SansSerifFontProc)    < 0 then GraphAbort('SansSerifFont',GraphResult);
(*
 FontNo:= RegisterBGifont(@TscrFontProc);
 if FontNo < 0 then GraphAbort('TscrFont');
 FontNo := InstallUserFont('tscr');           {install an *.obj file}
*)

 DetectGraph(GrDriver,GrMode);
 if GrDriver < 0 then
 begin
  RestoreCRTMode;
  GraphAbort('Graphic error detected',GraphResult);
  Halt(1);
 end;

 GrMode := EGAHi;            {mode=640x350   color=16 colors}
 FillColor := Blue;
 BkColor := LightGray;
 StartX  := 205;      { x position for PTV }
 StartY  := 47;       { y position for PTV }
 PTVHigh  := 59;
 LettW := 19;         { bredde af bogstaver }

 InitGraph(GrDriver,GrMode,'');
 ErrCode := GraphResult;
 if ErrCode <> GrOk then
 begin
  RestoreCRTMode;
  GraphAbort('Init graphics error',ErrCode);
  Halt(1);
 end;
 Maxcolor := GetMaxColor;
 MaxX := GetMaxX;
 MaxY := GetMaxY;
 SetColor(FillColor);
 SetBkColor(BkColor);
end;  {InitLogoGraph}


begin

end.
