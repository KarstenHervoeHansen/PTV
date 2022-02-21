
{*******************************************************}
{                                                       }
{       Turbo Pascal Version 5.5 og 6.0                 }
{       Window Interface Unit                           }
{       Rettet 891105 af Peter Frederiksen fra WinDemo  }
{       Rettet 900615 af Peter Frederiksen              }
{       Rettet 910128 af Peter Frederiksen 6.0          }
{       Copyright (C) 1989 Borland International        }
{                                                       }
{*******************************************************}

UNIT Win;

{$D+,S-}

INTERFACE

USES Crt;

TYPE
  TitleStr = STRING[63];
  FrameChars = ARRAY[1..8] OF CHAR;

  WinState = RECORD
    WindMin, WindMax : WORD;
    WhereX, WhereY   : BYTE;
    TextAttr         : BYTE;
  END;

CONST
  SingleFrame: FrameChars = 'ÚÄ¿³³ÀÄÙ';
  DoubleFrame: FrameChars = 'ÉÍ»ººÈÍ¼';
  BlankFrame : FrameChars = '        ';
  BoldFrame  : FrameChars = '²²²²²²²²';
  Frame                   = 1;
  NoFrame                 = 0;


TYPE
  TitleStrPtr = ^TitleStr;

  WinRecPtr = ^WinRec;
  WinRec = RECORD
    Next                 : WinRecPtr;
    State                : WinState;
    Title                : TitleStrPtr;
    TitleAttr, FrameAttr : BYTE;
    Buffer               : POINTER;
    FrameState           : BYTE;
                           {FrameState = 1: Ramme rundt om vindue}
                           {FrameState = 0: Ingen ramme rundt om vindue}
  END;

VAR  {fra vindue1}
  TopWindow   : WinRecPtr;
  WindowCount : INTEGER;   {antal †bne vinduer}


{ Direct write routines }

PROCEDURE WriteStr(X, Y: BYTE; S: String; Attr: BYTE);
PROCEDURE WriteChar(X, Y, Count: BYTE; Ch: CHAR; Attr: BYTE);


{ Window handling routines }

PROCEDURE FillWin(Ch: CHAR; Attr: BYTE);
PROCEDURE ReadWin(VAR Buf);
PROCEDURE WriteWin(VAR Buf);
FUNCTION WinSize: WORD;



PROCEDURE CloseWindow;

PROCEDURE InitWindow;

PROCEDURE CreateWindow(X, Y, H, W : INTEGER; HeadText : TitleStr;
                       HeadColor, FrameColor, FState: BYTE);



IMPLEMENTATION

{$L WIN.OBJ}

PROCEDURE WriteStr(X, Y: BYTE; S: String; Attr: BYTE);
external {WIN};

PROCEDURE WriteChar(X, Y, Count: BYTE; Ch: CHAR; Attr: BYTE);
external {WIN};

PROCEDURE FillWin(Ch: CHAR; Attr: BYTE);
external {WIN};

PROCEDURE WriteWin(VAR Buf);
external {WIN};

PROCEDURE ReadWin(VAR Buf);
external {WIN};

FUNCTION WinSize: WORD;
external {WIN};

PROCEDURE SaveWin(VAR W: WinState);
BEGIN
  W.WindMin := WindMin;
  W.WindMax := WindMax;
  W.WhereX := WhereX;
  W.WhereY := WhereY;
  W.TextAttr := TextAttr;
END;

PROCEDURE RestoreWin(VAR W: WinState);
BEGIN
  WindMin := W.WindMin;
  WindMax := W.WindMax;
  GotoXY(W.WhereX, W.WhereY);
  TextAttr := W.TextAttr;
END;

PROCEDURE FrameWin(Title: TitleStr; VAR Frame: FrameChars;
                   TitleAttr, FrameAttr: BYTE);
VAR
  W, H, Y: WORD;
BEGIN
  W := Lo(WindMax) - Lo(WindMin) + 1;
  H := Hi(WindMax) - Hi(WindMin) + 1;
  WriteChar(1, 1, 1, Frame[1], FrameAttr);
  WriteChar(2, 1, W - 2, Frame[2], FrameAttr);
  WriteChar(W, 1, 1, Frame[3], FrameAttr);
   IF Length(Title) > W - 2 THEN
    Title[0] := Chr(W - 2);
  WriteStr((W - Length(Title)) shr 1 + 1, 1, Title, TitleAttr);
    FOR Y := 2 TO H - 1 DO
    BEGIN
      WriteChar(1, Y, 1, Frame[4], FrameAttr);
      WriteChar(W, Y, 1, Frame[5], FrameAttr);
    END;
    WriteChar(1, H, 1, Frame[6], FrameAttr);
    WriteChar(2, H, W - 2, Frame[7], FrameAttr);
    WriteChar(W, H, 1, Frame[8], FrameAttr);

  Inc(WindMin, $0101);            {formindsker det aktive vinduer med 1 i}
  Dec(WindMax, $0101);            {h›jde og bredde}
END;

PROCEDURE UnFrameWin;
BEGIN
   Dec(WindMin, $0101);
   Inc(WindMax, $0101);
END;


{$S-}

PROCEDURE ActiveWindow(Active: Boolean);
BEGIN
  IF TopWindow <> nil then
  BEGIN
    UnFrameWin;

    with TopWindow^ DO
      CASE Active OF
	TRUE: BEGIN
               CASE FrameState OF
                0: FrameWin(Title^, BlankFrame, TitleAttr, FrameAttr);
                1: FrameWin(Title^, DoubleFrame, TitleAttr, FrameAttr);
                2: FrameWin(Title^, BoldFrame, TitleAttr, FrameAttr);
               END; {case}
              END;

	FALSE: BEGIN
                CASE FrameState OF
                 0: FrameWin(Title^, BlankFrame, TitleAttr, FrameAttr);
                 1: FrameWin(Title^, SingleFrame, FrameAttr, FrameAttr);
                 2: FrameWin(Title^, SingleFrame, FrameAttr, FrameAttr);
                END; {case}
               END;
       END; {case}
  END;
END;

PROCEDURE OpenWindow(X1, Y1, X2, Y2: BYTE; T: TitleStr;
                     TAttr, FAttr, FState : BYTE);
VAR
  W: WinRecPtr;
BEGIN
  ActiveWindow(FALSE);
  New(W);
  with W^ DO
  BEGIN
    Next := TopWindow;
    SaveWin(State);
    GetMem(Title, Length(T) + 1);
    Title^ := T;
    TitleAttr := TAttr;
    FrameAttr := FAttr;
    FrameState := FState;
    Window(X1, Y1, X2, Y2);
    GetMem(Buffer, WinSize);
    ReadWin(Buffer^);
     CASE FState OF
       0: FrameWin(T, BlankFrame, TAttr, FAttr);
       1: FrameWin(T, DoubleFrame, TAttr, FAttr);
       2: FrameWin(T, BoldFrame, TAttr, FAttr);
     END; {case}

  END;
  TopWindow := W;
  Inc(WindowCount);
END;

PROCEDURE CloseWindow;
VAR
  W: WinRecPtr;
BEGIN
  IF TopWindow <> nil then
  BEGIN
    W := TopWindow;
    with W^ DO
    BEGIN
      UnFrameWin;
      WriteWin(Buffer^);
      FreeMem(Buffer, WinSize);
      FreeMem(Title, Length(Title^) + 1);
      RestoreWin(State);
      TopWindow := Next;
    END;
    Dispose(W);
    ActiveWindow(TRUE);
    Dec(WindowCount);
  END;
END;

PROCEDURE InitWindow;
BEGIN
  TopWindow := nil;
  WindowCount := 0;
END;


PROCEDURE CreateWindow(X, Y, H, W : INTEGER; HeadText : TitleStr;
                      HeadColor, FrameColor, FState: BYTE);
VAR
  Color: BYTE;
BEGIN
  IF X + W > 81 THEN Writeln('Error: Window too wide!  ',X+W);
  IF Y + H > 26 THEN Writeln('Error: Window too high!  ',Y+H);
  IF LastMode <> CO80 then
    Color := Black ELSE Color := WindowCount mod 6 + 1;
  OpenWindow(X,Y,X+W-1,Y+H-1,HeadText ,HeadColor, FrameColor, FState);
  TextAttr := LightGray;
  ClrScr;
END;

END.  {win unit}
