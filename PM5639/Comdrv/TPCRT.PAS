{$S-,R-,V-,I-,B-,F-}

{$IFNDEF Ver40}
  {$I OMINUS.INC}
{$ENDIF}

{*********************************************************}
{*                    TPCRT.PAS 5.08                     *}
{*        Copyright (c) TurboPower Software 1987.        *}
{* Portions copyright (c) Sunny Hill Software 1985, 1986 *}
{*     and used under license to TurboPower Software     *}
{*                 All rights reserved.                  *}
{*********************************************************}


unit TPCrt;
  {-Alternate CRT interface unit. Replaces Turbo's CRT unit.}

interface

uses
  TpInline;

type
  FrameCharType = (ULeft, LLeft, URight, LRight, Horiz, Vert);
  FrameArray = array[FrameCharType] of Char;
const
  {video mode constants}
  BW40 = 0;
  CO40 = 1;
  C40 = CO40;
  BW80 = 2;
  CO80 = 3;
  C80 = CO80;
  Mono = 7;
  Font8x8 = 256;

  {color constants}
  Black = 0;
  Blue = 1;
  Green = 2;
  Cyan = 3;
  Red = 4;
  Magenta = 5;
  Brown = 6;
  LightGray = 7;
  DarkGray = 8;
  LightBlue = 9;
  LightGreen = 10;
  LightCyan = 11;
  LightRed = 12;
  LightMagenta = 13;
  Yellow = 14;
  White = 15;
  Blink = 128;
const
 {Set to True to allow programs to run as background tasks under
  DesqView/TaskView. Must be set False for TSR's.}
  DetectMultitasking : Boolean = False;
  BiosScroll : Boolean = True; {False to use TPCRT routines for clean scrolling}
const
  FrameChars : FrameArray = 'ÕÔ¸¾Í³';
const
  MapColors : Boolean = True; {True to let MapColor map colors for mono visibility}

type
  DisplayType = (MonoHerc, CGA, MCGA, EGA, VGA, PGC);
  HercCardType = (HercNone, HercPlain, HercPlus, HercInColor);
  FlexAttrs = array[0..3] of Byte; {attributes for FlexWrite}

  {record used to save/restore window coordinates}
  WindowCoordinates =
    record
      XL, YL, XH, YH : Byte;
    end;
var
  {from Turbo's CRT unit}
  CheckBreak : Boolean;      {enable Ctrl-Break checking}
  CheckEOF : Boolean;        {enable Ctrl-Z checking}
  DirectVideo : Boolean;     {write directly to screen?}
  CheckSnow : Boolean;       {True to prevent snow on CGA's}
  TextAttr : Byte;           {current video attribute}
  WindMin : Word;            {Window XLow and YLow: 0..24, 0..79 format}
  WindMax : Word;            {Window XHigh and YHigh: 0..24, 0..79 format}
  LastMode : Word;           {current video mode in low byte / 8x8 flag in high byte}
  {$IFDEF Ver40}
  SaveInt1B : Pointer;       {old INT $1B handler}
  {$ENDIF}

  {unique to Turbo Professional version}
  CtrlBreakFlag : Boolean;   {set to true when ^Break pressed}
  CurrentPage : Byte;        {current video page}
  CurrentMode : Byte absolute LastMode; {current video mode}
  ScreenWidth : Word;        {current width of screen}
  ScreenHeight : Word;       {current height of screen}
  CurrentDisplay : DisplayType; {currently selected display adapter}
  EnhancedDisplay : DisplayType; {meaningful only if set to MCGA, VGA, or EGA}
  WhichHerc : HercCardType;  {type of Hercules card installed}
  InTextMode : Boolean;      {set to false when in graphics mode}
  NormalAttr : Byte;         {attribute for NormVideo}
  VideoSegment : Word;       {current segment for video memory}
  BufLen : Word;             {maximum length of string for Read/Ln}
  MultiTaskingOn : Boolean;  {needed to support DesqView, TaskView}
  OneMS : LongInt;           { Word loop count used for a 1 ms delay}
  {for backward compatibility}
  CurrentWidth : Word absolute ScreenWidth; {current width of display}
  CurrentHeight : Word;      {current height of display - 1}
var
  {hooks for virtual screens in TPWINDOW}
  VirtualSegment : Word;     {alternate segment for video memory}
  VirtualWidth : Word;       {alternate width of display}
const
  DisplayOverride : ShortInt = -1; {use to override auto-detection of displays}
  {background character used by ClrScr, ScrollWindowUp/Down; gets reset to ' '
   automatically}
  TextChar : Char = ' ';
type
  PackedScreen = array[1..4000] of Byte; {dummy--actual size varies}
  PackedWindow =             {!!do not change!!}
    record
      Size : Word;           {size of packed window, including this header}
      TopRow : Byte;         {coordinates for top left corner of window}
      TopCol : Byte;
      Rows : Byte;           {height of window}
      Cols : Byte;           {width of window}
      AStart : Word;         {index to start of attributes section in Contents}
      CDelta : Word;         {bytes before first PackRec - chars}
      ADelta : Word;         {bytes before first PackRec - attrs}
      Contents : PackedScreen; {the contents of the packed screen}
    end;
  PackedWindowPtr = ^PackedWindow;
const
  deInUse = 0;
  deUnused = -1;
  deDeleted = -2;
type
  LibName = string[12];
  deName = array[1..11] of Char; {first 8 have name, last 3 have extension}
  DirectoryEntry =
    record
      Status : ShortInt;     {0 = in use, -1 = unused, -2 = deleted}
      Name : deName;         {name and extension in FCB format}
      Index : Word;          {index to this member}
      MemberLength : Word;   {# of 128-byte blocks used by member}
      CRC : Word;            {not implemented}
      CreationDate : Word;   {date entry was created--not used}
      LastChangeDate : Word; {date it was last changed--not used}
      CreationTime : Word;   {time entry was created--not used}
      LastChangeTime : Word; {time it was last changed--not used}
      PadCount : Byte;       {unused bytes in last block of member}
      Filler : array[28..32] of Byte; {padded to 32 bytes}
    end;
  DirectoryType = array[0..255] of DirectoryEntry;
  DirectoryPtr = ^DirectoryType;
const
  BlankDeName : deName = '           ';
const
  CrtError : Word = 0;
  {non-standard error codes for CrtError}
  CrtNotEnoughMem = $FFFF;
  CrtNilPointer = $FFFE;
  CrtNotValidLib = $FFFD;
  CrtWinNotFound = $FFFC;
  CrtDirectoryFull = $FFFB;
  CrtBadWindow = $FFFA;

  {unit codes for use in conjunction with TPHELP--see manual}
  HelpForUser  = 00;         {unused}
  HelpForPick  = 01;         {TPPICK}
  HelpForEdit  = 02;         {TPEDIT}
  HelpForMenu  = 03;         {TPMENU}
  HelpForEntry = 04;         {TPENTRY}
  HelpForMacEd = 05;         {TPMACED}
  HelpForMemo  = 06;         {TPMEMO}
  HelpForXXXX2 = 07;         {reserved}
  HelpForXXXX3 = 08;         {reserved}
  HelpForXXXX4 = 09;         {reserved}
  HelpForXXXX5 = 10;         {reserved}

procedure TextMode(Mode : Word);
  {-Switch to/set text mode}

procedure Window(XLow, YLow, XHigh, YHigh : Byte);
  {-Set current window coordinates}

procedure ClrScr;
  {-Clear the current window}

procedure ClrEol;
  {-Clear the remainder of the current screen line}

procedure InsLine;
  {-Insert a new line at the position of the cursor}

procedure DelLine;
  {-Delete current screen line}

procedure GoToXY(X, Y : Byte);
  {-Move cursor to column X, row Y, relative to Window}

function WhereX : Byte;
  {-Return column coordinate of cursor, relative to Window}

function WhereY : Byte;
  {-Return row coordinate of cursor, relative to Window}

procedure TextColor(Color : Byte);
  {-Set foreground color for screen writes}

procedure TextBackground(Color : Byte);
  {-Set background color for screen writes}

procedure LowVideo;
  {-Select low intensity}

procedure HighVideo;
  {-Select high intensity}

procedure NormVideo;
  {-Select video attribute used at start of program}

function KeyPressed : Boolean;
  {-Return true if a key has been pressed}

function ReadKey : Char;
  {-Read a character from the keyboard}

procedure AssignCrt(var F : Text);
  {-Routes input and output through our routines}

procedure Delay(MS : Word);
  {-Delay for MS milliseconds}

procedure Sound(Hz : Word);
  {-Turn on the sound at the designated frequency}

procedure NoSound;
  {-Turn off the sound}

  {****** extensions to Turbo's CRT unit ******}

function GetCrtMode : Byte;
 {-Get the current video mode. Also reinitializes internal variables. May
   reset: CurrentMode, ScreenWidth, ScreenHeight, CurrentPage, and
   VideoSegment.}

procedure GotoXYAbs(X, Y : Byte);
  {-Move cursor to column X, row Y. No error checking done.}

function WhereXY : Word;
 {-Return absolute row and column coordinates of cursor. High byte has current
   row (Y), low byte has current column (X).}

function WhereYAbs : Byte;
  {-Return absolute row coordinate of cursor}

function WhereXAbs : Byte;
  {-Return absolute column coordinate of cursor}

procedure WhereXYdirect(var X, Y : Byte);
  {-Read the current position of the cursor directly from the CRT controller}

procedure SetVisiblePage(PageNum : Byte);
  {-Set current video page}

procedure ScrollWindowUp(XLo, YLo, XHi, YHi, Lines : Byte);
  {-Scrolls the designated window up the specified number of lines.}

procedure ScrollWindowDown(XLo, YLo, XHi, YHi, Lines : Byte);
  {-Scrolls the designated window down the specified number of lines.}

function CursorTypeSL : Word;
  {-Returns a word. High byte has starting scan line, low byte has ending.}

function CursorStartLine : Byte;
  {-Returns the starting scan line of the cursor}

function CursorEndLine : Byte;
  {-Returns the ending scan line of the cursor.}

procedure SetCursorSize(Startline, EndLine : Byte);
  {-Sets the cursor's starting and ending scan lines.}

procedure NormalCursor;
  {-Set normal scan lines for cursor based on current video mode}

procedure FatCursor;
  {-Set larger scan lines for cursor based on current video mode}

procedure BlockCursor;
  {-Set scan lines for a block cursor}

procedure HiddenCursor;
  {-Hide the cursor}

function ReadCharAtCursor : Char;
  {-Returns character at the current cursor location on the selected page.}

function ReadAttrAtCursor : Byte;
  {-Returns attribute at the current cursor location on the selected page.}

procedure GetCursorState(var XY, ScanLines : Word);
  {-Return the current position and size of the cursor}

procedure RestoreCursorState(XY, ScanLines : Word);
  {-Reset the cursor to a position and size saved with GetCursorState}

procedure FastWrite(St : string; Row, Col, Attr : Byte);
  {-Write St at Row,Col in Attr (video attribute) without snow}

procedure FastWriteWindow(St : string; Row, Col, Attr : Byte);
  {-Write a string using window-relative coordinates}

procedure FastText(St : string; Row, Col : Byte);
  {-Write St at Row,Col without changing the underlying video attribute.}

procedure FastTextWindow(St : string; Row, Col : Byte);
  {-Write St at window Row,Col without changing the underlying video attribute.}

procedure FastVert(St : string; Row, Col, Attr : Byte);
  {-Write St vertically at Row,Col in Attr (video attribute)}

procedure FastVertWindow(St : string; Row, Col, Attr : Byte);
  {-Write a string vertically using window-relative coordinates}

procedure FastFill(Number : Word; Ch : Char; Row, Col, Attr : Byte);
  {-Fill Number chs at Row,Col in Attr (video attribute) without snow}

procedure FastFillWindow(Number : Word; Ch : Char; Row, Col, Attr : Byte);
  {-Fill Number chs at window Row,Col in Attr (video attribute) without snow}

procedure FastCenter(St : string; Row, Attr : Byte);
  {-Write St centered on window Row in Attr (video attribute) without snow}

procedure FastFlush(St : string; Row, Attr : Byte);
  {-Write St flush right on window Row in Attr (video attribute) without snow}

procedure FastRead(Number, Row, Col : Byte; var St : string);
  {-Read Number characters from the screen into St starting at Row,Col}

procedure FastReadWindow(Number, Row, Col : Byte; var St : string);
  {-Read Number characters from the screen into St starting at window Row,Col}

procedure ReadAttribute(Number, Row, Col : Byte; var St : string);
  {-Read Number attributes from the screen into St starting at Row,Col}

procedure ReadAttributeWindow(Number, Row, Col : Byte; var St : string);
  {-Read Number attributes from the screen into St starting at window Row,Col}

procedure WriteAttribute(St : String; Row, Col : Byte);
  {-Write string of attributes St at Row,Col without changing characters}

procedure WriteAttributeWindow(St : String; Row, Col : Byte);
  {-Write string of attributes St at window Row,Col without changing characters}

procedure ChangeAttribute(Number : Word; Row, Col, Attr : Byte);
  {-Change Number video attributes to Attr starting at Row,Col}

procedure ChangeAttributeWindow(Number : Word; Row, Col, Attr : Byte);
  {-Change Number video attributes to Attr starting at window Row,Col}

procedure MoveScreen(var Source, Dest; Length : Word);
  {-Move Length words from Source to Dest without snow}

procedure FlexWrite(St : string; Row, Col : Byte; var FAttrs : FlexAttrs);
  {-Write St at Row,Col with flexible color handling}

procedure FlexWriteWindow(St : string; Row, Col : Byte; var FAttrs : FlexAttrs);
  {-Write a string flexibly using window-relative coordinates.}

function SaveWindow(XLow, YLow, XHigh, YHigh : Byte; Allocate : Boolean;
                    var Covers : Pointer) : Boolean;
  {-Allocate buffer space if requested and save window contents}

procedure RestoreWindow(XLow, YLow, XHigh, YHigh : Byte;
                        Deallocate : Boolean; var Covers : Pointer);
  {-Restore screen contents and deallocate buffer space if requested}

procedure SetFrameChars(Vertical, Horizontal, LowerRight, UpperRight,
                        LowerLeft, UpperLeft : Char);
  {-Sets the frame characters to be used on subsequent FrameWindow calls.}

procedure FrameWindow(LeftCol, TopRow, RightCol, BotRow, FAttr, HAttr : Byte;
                      Header : string);
  {-Draws a frame around a window}

procedure StoreWindowCoordinates(var WC : WindowCoordinates);
  {-Store the window coordinates for the active window}

procedure RestoreWindowCoordinates(WC : WindowCoordinates);
  {-Restore previously saved window coordinates}

function PackWindow(XLow, YLow, XHigh, YHigh : Byte) : PackedWindowPtr;
  {-Return a pointer to a packed window, or nil if not enough memory}

procedure DispPackedWindow(PWP : PackedWindowPtr);
  {-Display the packed window pointed to by PWP}

procedure DispPackedWindowAt(PWP : PackedWindowPtr; Row, Col : Byte);
 {-Display the packed window pointed to by PWP at Row,Col. If necessary,
   the coordinates are adjusted to allow it to fit on the screen.}

procedure MapPackedWindowColors(PWP : PackedWindowPtr);
 {-Map the colors in a packed window for improved appearance on mono/B&W
   displays}

procedure DisposePackedWindow(var PWP : PackedWindowPtr);
  {-Dispose of a packed window, setting PWP to nil on exit}

procedure WritePackedWindow(PWP : PackedWindowPtr; FName : string);
 {-Store the packed window pointed to by PWP in FName}

function ReadPackedWindow(FName : string) : PackedWindowPtr;
 {-Read the packed window stored in FName into memory}

function CreateLibrary(var F : file; Name : string;
                       Entries : Byte) : DirectoryPtr;
 {-Create a library with the specified # of directory entries}

function OpenLibrary(var F : file; Name : string) : DirectoryPtr;
 {-Open the specified library and return a pointer to its directory}

procedure CloseLibrary(var F : file; var DP : DirectoryPtr);
  {-Close library F and deallocate its directory}

procedure PackLibrary(LName : string);
 {-Pack a library to remove deleted entries.}

procedure AddWindowToLibrary(PWP : PackedWindowPtr; var F : file;
                             DP : DirectoryPtr; WinName : LibName);
 {-Add a packed window to the specified library}

function ReadWindowFromLibrary(var F : file; DP : DirectoryPtr;
                               WinName : LibName) : PackedWindowPtr;
 {-Read a packed window from a library}

procedure DeleteWindowFromLibrary(var F : file; DP : DirectoryPtr;
                                  WinName : LibName);
 {-Delete a packed window from the specified library}

function MapColor(c : Byte) : Byte;
  {-Map a video attribute for visibility on mono/bw displays}

procedure SetBlink(On : Boolean);
  {-Enable text mode attribute blinking if On is True}

procedure SetCrtBorder(Attr : Byte);
  {-Set border to background color if card type and mode allow}

function Font8x8Selected : Boolean;
  {-Return True if EGA or VGA is active and in 8x8 font}

procedure SelectFont8x8(On : Boolean);
  {-Toggle 8x8 font on or off}

function HercPresent : Boolean;
  {-Return true if a Hercules graphics card is present}

procedure SwitchInColorCard(ColorOn : Boolean);
  {-Activate or deactivate colors on a Hercules InColor card}

function HercGraphicsMode : Boolean;
  {-Return True if a Hercules card is in graphics mode}

function HercModeTestWorks : Boolean;
  {-Return True if HercGraphicsMode will work}

procedure SetHercMode(GraphMode : Boolean; GraphPage : Byte);
 {-Set Hercules card to graphics mode or text mode, and activate specified
   graphics page (if switching to graphics mode).}

function ReadKeyWord : Word;
 {-Waits for keypress, then returns scan and character codes together}

function CheckKbd(var KeyCode : Word) : Boolean;
  {-Returns True (and the key codes) if a keystroke is waiting}

function KbdFlags : Byte;
  {-Returns keyboard status flags as a bit-coded byte}

procedure StuffKey(W : Word);
  {-Stuff one key into the keyboard buffer}

procedure StuffString(S : string);
  {-Stuff the contents of S into the keyboard buffer}

procedure ReInitCrt;
 {-Reinitialize CRT unit's internal variables. For TSR's or programs with
   DOS shells. May reset: CurrentMode, ScreenWidth, ScreenHeight,
   WindMin/WindMax, CurrentPage, CurrentDisplay, CheckSnow, and VideoSegment.}

  {the following are for internal use only}
const
  CrtCheck : Boolean = True;
  TpUnitsFlag : Byte = 0;
  TpScreenFlag = 1;
  TpWindowFlag = 2;
type
  PackRec =                  {!!do not change!!}
    record
      Link : Word;           {number of bytes to next PackRec}
      FillVal : Byte;        {fill value}
      FillCnt : Word;        {fill count}
    end;

  {==========================================================================}

implementation

type
  FIB =
    record
      Handle : Word;
      Mode : Word;
      BufSize : Word;
      Private : Word;
      BufPos : Word;
      BufEnd : Word;
      BufPtr : Pointer;
      OpenProc : Pointer;
      InOutProc : Pointer;
      FlushProc : Pointer;
      CloseProc : Pointer;
      UserData : array[1..16] of Byte;
      Name : array[0..79] of Char;
      Buffer : array[1..128] of Char;
    end;
  SegOfs =
    record
      Ofst, Segm : Word;
    end;
var
  Vector23 : Pointer absolute $0 : $8C;
  SystemSeg : Word;
type
  BufPtr = ^BufferArray;
  BufferArray = array[0..MaxInt] of Char;
var
  BiosScanLines : Word absolute $40 : $60;
  IsCompaq : Boolean;
  CompaqBiosName : array[1..6] of Char absolute $FFFE : $000A;
  IsZenith : Boolean absolute IsCompaq;
  ZenithBiosName : array[1..6] of Char absolute $FB00 : $0000;
  NextChar : Byte;
  SaveExitProc : Pointer;
  {$IFDEF Ver40}
    Save1B : Pointer absolute SaveInt1B;
  {$ELSE}
    Save1B : Pointer;
  {$ENDIF}
const
  {tables for Hercules graphics}
  HercTtable : array[0..11] of Byte = (
    $61, $50, $52, $0F, $19, $06, $19, $19, $02, $0D, $0B, $0C);
  HercGtable : array[0..11] of Byte = (
    $35, $2D, $2E, $07, $5B, $02, $57, $57, $02, $03, $00, $00);

  {$L TPCRT.OBJ}
  {$L TPCRT2.OBJ}
  {$L TPFAST.OBJ}
  {$L TPFAST2.OBJ}
  {$L TPCMISC.OBJ}
  {$L TPPACK.OBJ}
  {$L TPFLEX.OBJ}

  {local routines in TPCRT.OBJ}
  procedure ReadCursorPrim; external;
  procedure SetCursorPrim; external;
  procedure GetCursorPrim; external;
  procedure GetCrtModePrim; external;
  procedure ScrollUpPrim; external;
  procedure ScrollDownPrim; external;
  procedure AdapterCheck; external;
  procedure DelayMS; external;
  procedure GetCharAttr; external;
  procedure SetWindowPrim; external;
  procedure FullWindow; external;
  procedure GetAttribute; external;
  procedure InitCrt; external;
  procedure CalcOffset; external;

  {global routines in TPCRT.OBJ}
  function ReadKey : Char; external;
  procedure AssignCrt(var F : Text); external;
  procedure ReInitCrt; external;

  {routines in TPCRT2.OBJ}
  procedure TextMode(Mode : Word); external;
  procedure Window(XLow, YLow, XHigh, YHigh : Byte); external;
  procedure ClrScr; external;
  procedure GoToXY(X, Y : Byte); external;
  function WhereX : Byte; external;
  function WhereY : Byte; external;
  procedure TextColor(Color : Byte); external;
  procedure TextBackground(Color : Byte); external;
  procedure LowVideo; external;
  procedure HighVideo; external;
  procedure NormVideo; external;
  function KeyPressed : Boolean; external;
  procedure Delay(MS : Word); external;
  function GetCrtMode : Byte; external;
  procedure GotoXYAbs(X, Y : Byte); external;
  function Font8x8Selected : Boolean; external;
  procedure SelectFont8x8(On : Boolean); external;
  function ReadKeyWord : Word; external;

  {routines in TPCMISC.OBJ}
  procedure ClrEol; external;
  procedure InsLine; external;
  procedure DelLine; external;
  procedure Sound(Hz : Word); external;
  procedure NoSound; external;
  function WhereXY : Word; external;
  function WhereXAbs : Byte; external;
  function WhereYAbs : Byte; external;
  function ReadCharAtCursor : Char; external;
  function ReadAttrAtCursor : Byte; external;
  procedure SetVisiblePage(PageNum : Byte); external;
  procedure ScrollWindowUp(XLo, YLo, XHi, YHi, Lines : Byte); external;
  procedure ScrollWindowDown(XLo, YLo, XHi, YHi, Lines : Byte); external;
  function CursorTypeSL : Word; external;
  function CursorStartLine : Byte; external;
  function CursorEndLine : Byte; external;
  procedure SetCursorSize(Startline, EndLine : Byte); external;
  function KbdFlags : Byte; external;
  function CheckKbd(var KeyCode : Word) : Boolean; external;

  {routines in TPFAST.OBJ}
  procedure FastWrite(St : string; Row, Col, Attr : Byte); external;
  procedure FastWriteWindow(St : string; Row, Col, Attr : Byte); external;
  procedure FastRead(Number, Row, Col : Byte; var St : string); external;
  procedure FastReadWindow(Number, Row, Col : Byte; var St : string); external;
  procedure ReadAttribute(Number, Row, Col : Byte; var St : string); external;
  procedure ReadAttributeWindow(Number, Row, Col : Byte; var St : string); external;
  procedure ChangeAttribute(Number : Word; Row, Col, Attr : Byte); external;
  procedure ChangeAttributeWindow(Number : Word; Row, Col, Attr : Byte); external;
  procedure MoveScreen(var Source, Dest; Length : Word); external;

  {routines in TPFAST2.OBJ}
  procedure FastText(St : string; Row, Col : Byte); external;
  procedure FastTextWindow(St : string; Row, Col : Byte); external;
  procedure FastVert(St : string; Row, Col, Attr : Byte); external;
  procedure FastVertWindow(St : string; Row, Col, Attr : Byte); external;
  procedure FastFill(Number : Word; Ch : Char; Row, Col, Attr : Byte); external;
  procedure FastFillWindow(Number : Word; Ch : Char; Row, Col, Attr : Byte); external;
  procedure FastCenter(St : string; Row, Attr : Byte); external;
  procedure FastFlush(St : string; Row, Attr : Byte); external;
  procedure WriteAttribute(St : String; Row, Col : Byte); external;
  procedure WriteAttributeWindow(St : String; Row, Col : Byte); external;

  {routines in TPFLEX.OBJ}
  procedure FlexWrite(St : string; Row, Col : Byte; var FAttrs : FlexAttrs); external;
  procedure FlexWriteWindow(St : string; Row, Col : Byte; var FAttrs : FlexAttrs); external;

  {routines in TPPACK.OBJ}
  function PackPrim(Height, Width : Byte; DstSeg, DstOfs, SrcOfs : Word;
    var Delta : Word) : Word; external;
  procedure DispPackedWindow(PWP : PackedWindowPtr); external;

  procedure CrtTest;
    {-Test for presence of CRT in program}
  begin
    if CrtCheck then
      with SegOfs(FIB(Input).InOutProc) do
        if (Segm <> CSeg) and (Segm <> SystemSeg) then begin
          WriteLn('CRT/TPCRT conflict');
          Halt(1);
        end;
  end;

  procedure NormalCursor;
    {-Set normal scan lines for cursor based on current video mode}
  var
    ScanLines : Word;
  begin
    if (Hi(LastMode) <> 0) then
      ScanLines := $0507
    else
      if CurrentMode = 7 then
        ScanLines := $0B0C
      else
        ScanLines := $0607;
    SetCursorSize(Hi(ScanLines), Lo(ScanLines));
  end;

  procedure FatCursor;
    {-Set larger scan lines for cursor based on current video mode}
  var
    ScanLines : Word;
  begin
    if (Hi(LastMode) <> 0) then
      ScanLines := $0307
    else if CurrentMode = 7 then
      ScanLines := $090C
    else
      ScanLines := $0507;
    SetCursorSize(Hi(ScanLines), Lo(ScanLines));
  end;

  procedure BlockCursor;
    {-Set scan lines for a block cursor}
  var
    EndLine : Byte;
  begin
    if (Hi(LastMode) <> 0) or (CurrentMode <> 7) then
      EndLine := $07
    else
      EndLine := $0C;
    SetCursorSize(0, EndLine);
  end;

  procedure HiddenCursor;
    {-Hide the cursor}
  begin
    SetCursorSize($20, 0);
  end;

  procedure GetCursorState(var XY, ScanLines : Word);
    {-Return the current position and size of the cursor}
  begin
    XY := WhereXY;
    ScanLines := CursorTypeSL;
  end;

  procedure RestoreCursorState(XY, ScanLines : Word);
    {-Reset the cursor to a position and size saved with GetCursorState}
  begin
    SetCursorSize(Hi(ScanLines), Lo(ScanLines));
    GotoXYAbs(Lo(XY), Hi(XY));
  end;

  procedure WhereXYdirect(var X, Y : Byte);
    {-Read the current position of the cursor directly from the CRT controller}
  var
    CrtPort : Word absolute $40:$63;
    CrtWidth : Word absolute $40:$4A;
    XP, XY : Word;
  begin
    Port[CrtPort] := 14;
    XP := Port[CrtPort+1];
    Port[CrtPort] := 15;
    XY := (XP shl 8)+Port[CrtPort+1];
    Y := Succ(XY div CrtWidth);
    X := Succ(XY mod CrtWidth);
  end;

  procedure StuffKey(W : Word);
    {-Stuff one key into the keyboard buffer}
  const
    KbdStart = $1E;
    KbdEnd = $3C;
  var
    KbdHead : Word absolute $40 : $1A;
    KbdTail : Word absolute $40 : $1C;
    SaveKbdTail : Word;
  begin
    SaveKbdTail := KbdTail;
    if KbdTail = KbdEnd then
      KbdTail := KbdStart
    else
      Inc(KbdTail, 2);
    if KbdTail = KbdHead then
      KbdTail := SaveKbdTail
    else
      MemW[$40:SaveKbdTail] := W;
  end;

  procedure StuffString(S : string);
    {-Stuff the contents of S into the keyboard buffer}
  var
    I : Byte;
  begin
    {allow at most 15 characters} {!!.07}
    if Length(S) > 15 then        {!!.07}
      S[0] := #15;                {!!.07}

    {stuff each key}
    for I := 1 to Length(S) do
      StuffKey(Ord(S[I]));
  end;

  function SaveWindow(XLow, YLow, XHigh, YHigh : Byte; Allocate : Boolean;
                      var Covers : Pointer) : Boolean;
    {-Allocate buffer space if requested and save window contents}
  var
    CoversP : BufPtr absolute Covers;
    WordsPerRow : Word;
    BufBytes : Word;
    SrcPos : Word;
    DestPos : Word;
    Row : Word;
  begin
    {assume success}
    SaveWindow := True;

    {compute number of words to move per row}
    WordsPerRow := Succ(XHigh-XLow);

    if Allocate then begin
      {compute bytes needed for screen buffer}
      BufBytes := (WordsPerRow*Succ(YHigh-YLow)) shl 1;

      {make sure enough memory is available}
      if MaxAvail < LongInt(BufBytes) then begin
        SaveWindow := False;
        Exit;
      end
      else
        {allocate the screen buffer}
        GetMem(CoversP, BufBytes);
    end;

    {save current contents to the screen buffer}
    DestPos := 0;
    SrcPos := (Pred(YLow)*ScreenWidth+Pred(XLow)) shl 1;
    for Row := YLow to YHigh do begin
      MoveScreen(Mem[VideoSegment:SrcPos], CoversP^[DestPos], WordsPerRow);
      Inc(SrcPos, ScreenWidth shl 1);
      Inc(DestPos, WordsPerRow shl 1);
    end;
  end;

  procedure RestoreWindow(XLow, YLow, XHigh, YHigh : Byte;
                          Deallocate : Boolean; var Covers : Pointer);
    {-Restore screen contents and deallocate buffer space if requested}
  var
    CoversP : BufPtr absolute Covers;
    WordsPerRow : Word;
    SrcPos : Word;
    DestPos : Word;
    Row : Word;
  begin
    {compute number of words to move per row}
    WordsPerRow := Succ(XHigh-XLow);

    {Restore current contents to the screen buffer}
    DestPos := 0;
    SrcPos := (Pred(YLow)*ScreenWidth+Pred(XLow)) shl 1;
    for Row := YLow to YHigh do begin
      MoveScreen(CoversP^[DestPos], Mem[VideoSegment:SrcPos], WordsPerRow);
      Inc(SrcPos, ScreenWidth shl 1);
      Inc(DestPos, WordsPerRow shl 1);
    end;

    {deallocate buffer space if requested}
    if Deallocate then begin
      FreeMem(CoversP, (WordsPerRow*Succ(YHigh-YLow)) shl 1);
      CoversP := nil;
    end;
  end;

  procedure SetFrameChars(Vertical, Horizontal, LowerRight, UpperRight,
                          LowerLeft, UpperLeft : Char);
    {-Sets the frame characters to be used on subsequent FrameWindow calls.}
  begin
    FrameChars[ULeft] := UpperLeft;
    FrameChars[LLeft] := LowerLeft;
    FrameChars[URight] := UpperRight;
    FrameChars[LRight] := LowerRight;
    FrameChars[Horiz] := Horizontal;
    FrameChars[Vert] := Vertical;
  end;

  procedure FrameWindow(LeftCol, TopRow, RightCol, BotRow, FAttr, HAttr : Byte;
                        Header : string);
    {-Draws a frame around a window}
  var
    HeaderLen : Byte absolute Header;
    Row, Width, HeaderPos : Byte;
    Span : string[132];
    SpanLen : Byte absolute Span;
  begin
    {calculate width of window and position of header}
    SpanLen := Succ(RightCol-LeftCol);
    Width := SpanLen-2;

    {construct the upper border and draw it}
    FillChar(Span[2], Width, FrameChars[Horiz]);
    Span[1] := FrameChars[ULeft];
    Span[SpanLen] := FrameChars[URight];
    FastWrite(Span, TopRow, LeftCol, FAttr);

    {draw the vertical bars}
    for Row := Succ(TopRow) to Pred(BotRow) do begin
      FastWrite(FrameChars[Vert], Row, LeftCol, FAttr);
      FastWrite(FrameChars[Vert], Row, RightCol, FAttr);
    end;

    {draw the bottom border}
    Span[1] := FrameChars[LLeft];
    Span[SpanLen] := FrameChars[LRight];
    FastWrite(Span, BotRow, LeftCol, FAttr);

    if HeaderLen > 0 then begin
      if HeaderLen > Width then
        HeaderLen := Width;
      HeaderPos := (SpanLen-HeaderLen) shr 1;
      FastWrite(Header, TopRow, LeftCol+HeaderPos, HAttr);
    end;
  end;

  function MapColor(c : Byte) : Byte;
    {-Map a video attribute for visibility on mono/bw displays}
  const
    MonoTable : array[0..15] of Byte = (
      0, 1, 7, 7, 7, 7, 7, 7,   7, 7, 7, 7, 7, 7, 15, 15);
  var
    Fore, Back : Byte;
  begin
    if MapColors then begin
      Fore := c and $F;
      Back := (c shr 4) and $7;
      if WhichHerc <> HercInColor then
        case CurrentMode of
          0, 2, 7 :
            {B&W modes}
            begin
              Fore := MonoTable[Fore];
              Back := MonoTable[Back];
            end;
        end;
      if (CurrentMode = 7) and (WhichHerc <> HercInColor) then
        {Monochrome mode}
        if (Fore or Back) <> 0 then
          {Not black on black}
          if (Fore = 0) or (Back = $07) then begin
            {Force to reverse video}
            Fore := 0;
            Back := $7;
          end
          else if Back <> 0 then
            {Force to black background}
            Back := 0;
      if (c and $80) <> 0 then
        {Set blink}
        Back := Back or $08;
      MapColor := Byte(Back shl 4) or Fore;
    end
    else
      MapColor := c;
  end;

  procedure StoreWindowCoordinates(var WC : WindowCoordinates);
    {-Store the window coordinates for the active window}
  type
    XY = record
           X, Y : Byte;
         end;
  begin
    with WC do begin
      XL := Succ(XY(WindMin).X);
      YL := Succ(XY(WindMin).Y);
      XH := Succ(XY(WindMax).X);
      YH := Succ(XY(WindMax).Y);
    end;
  end;

  procedure RestoreWindowCoordinates(WC : WindowCoordinates);
    {-Restore previously saved window coordinates}
  begin
    with WC do
      Window(XL, YL, XH, YH);
  end;

  function HercPresent : Boolean;
    {-Return true if a Hercules graphics card is present}
  begin
    inline(
      $31/$DB/               {xor bx,bx}
      $CD/$11/               {int $11}
      $25/$30/$00/           {and ax,$30     ;check equipment list}
      $3D/$30/$00/           {cmp ax,$30     ;"switches" set for mono?}
      $75/$18/               {jne done       ;no - quit}
      $BA/$BA/$03/           {mov dx,$3BA    ;Take a reading}
      $EC/                   {in al,dx}
      $24/$80/               {and al,$80}
      $88/$C4/               {mov ah,al      ;Save bit 7 for test}
      $B9/$00/$80/           {mov cx,$8000   ;How many times to test}
                             {Examine:}
      $EC/                   {in al,dx       ;Take another reading}
      $24/$80/               {and al,$80     ;Isolate bit 7}
      $38/$E0/               {cmp al,ah}
      $75/$05/               {jne herc       ;If bit 7 changes then it}
      $E2/$F7/               {loop examine   ; is a Hercules}
      $E9/$01/$00/           {jmp done       ;it must be something else}
                             {Herc:}
      $43/                   {inc bx         ;return true}
                             {Done:}
      $88/$5E/$FF);          {mov [bp-1],bl  ;set function result}
  end;

  function WhichHercPrim : HercCardType;
    {-Distinguish HGC, HGC+, and InColor card}
  inline(
    $BA/$BA/$03/             {mov dx,$3ba   ;read status port}
    $EC/                     {in al,dx}
    $88/$C4/                 {mov ah,al     ;result into AH}
    $80/$E4/$70/             {and ah,$70    ;mask out bits 4-6}
    $B0/$03/                 {mov al,3      ;assume HercInColor}
    $80/$FC/$50/             {cmp ah,$50    ;just bits 4 and 6 set?}
    $74/$09/                 {je done       ;if so, done}
    $FE/$C8/                 {dec al        ;assume HercPlus}
    $80/$FC/$10/             {cmp ah,$10    ;just bit 4 set?}
    $74/$02/                 {je done       ;if so, done}
    $FE/$C8);                {dec al        ;else, HercPlain}
                             {done:}

  function WhichHercCard : HercCardType;
    {-Returns Hercules card type}
  begin
    if (CurrentDisplay <> MonoHerc) or not HercPresent then
      WhichHercCard := HercNone
    else if (CurrentDisplay = MonoHerc) then
      WhichHercCard := WhichHercPrim
    else
      WhichHercCard := HercPlain;
  end;

  procedure SwitchInColorCard(ColorOn : Boolean);
    {-Activate or deactivate colors on a Hercules InColor card}
  begin
    inline(
      $8A/$66/<ColorOn/      {mov ah,[bp+<ColorOn] ;AH = ColorOn}
      $80/$F4/$01/           {xor ah,1             ;flip bit 0}
      $B1/$05/               {mov cl,5             ;move it to bit 5}
      $D2/$E4/               {shl ah,cl            ;!!}
      $B0/$17/               {mov al,$17           ;AL = Exception Register #}
      $BA/$B4/$03/           {mov dx,$3B4          ;DX = I/O port}
      $EF);                  {out dx,ax}
  end;

  function HercGraphModePrim : Boolean;
    {-Return True if a Hercules card is in graphics mode}
  begin
    inline(
      $30/$C0/               {XOR AL,AL}
      $BA/$BA/$03/           {MOV DX,$03BA    ;display status port}
                             {Wait1:          ;test for v-sync transition}
      $EC/                   {IN AL,DX}
      $A8/$80/               {TEST AL,$80     ;mask for v-sync}
      $74/$FB/               {JZ Wait1}
                             {Wait2:}
      $EC/                   {IN AL,DX}
      $A8/$80/               {TEST AL,$80     ;mask for v-sync}
      $75/$FB/               {JNZ Wait2}
      $30/$C0/               {XOR AL,AL       ;reset and Set Light Pen.}
      $BA/$BB/$03/           {MOV DX,$03BB    ;reset light pen port}
      $EE/                   {OUT DX,AL}
      $EB/$00/               {JMP SHORT X1    ;Delay for AT}
                             {X1:}
      $BA/$B9/$03/           {MOV DX,$03B9    ;set light pen port}
      $EE/                   {OUT DX,AL}
      $B0/$10/               {MOV AL,16       ;Look in the 6845 for the offset}
      $BA/$B4/$03/           {MOV DX,$03B4    ; into buffer when the light pen}
      $EE/                   {OUT DX,AL       ; is tripped - high byte.}
      $EB/$00/               {JMP SHORT X2    ;Delay for AT}
                             {X2:}
      $42/                   {INC DX}
      $EC/                   {IN AL,DX}
      $88/$C7/               {MOV BH,AL}
      $B0/$11/               {MOV AL,17       ;Get low byte of light pen}
      $BA/$B4/$03/           {MOV DX,$03B4    ; trip address}
      $EE/                   {OUT DX,AL}
      $EB/$00/               {JMP SHORT X3    ;Delay for AT}
                             {X3:}
      $42/                   {INC DX}
      $EC/                   {IN AL,DX}
      $88/$C3/               {MOV BL,AL       ;BX now contains Light Pen trip address}
      $30/$C0/               {XOR AL,AL       ;assume text mode}
      $81/$FB/$8E/$0B/       {CMP BX,$0B8E    ;If BX < threshold value - text mode}
      $72/$02/               {JB HGMPdone     ;Jump to what should happen for text}
      $FE/$C0/               {INC AL          ;return true for graph mode}
                             {HGMPdone:}
      $88/$46/$FF);          {MOV [BP-1],AL}
  end;

  function HercGraphicsMode : Boolean;
    {-Return True if a Hercules card is in graphics mode}
  const
    Votes = 5;               {keep it odd}
  var
    I, Graph : Byte;
  begin
    Graph := 0;
    {run test several times--it doesn't always work, even on a true Herc}
    for I := 1 to Votes do
      if HercGraphModePrim then
        Inc(Graph);
    {if test succeeded more times than it failed, it's a Herc}
    HercGraphicsMode := (Graph > (Votes shr 1));
  end;

  function HercModeTestWorks : Boolean;
    {-Return True if HercGraphicsMode will work}
  begin
    inline(
      $30/$DB/               {XOR BL,BL     ;assume False}
      $BA/$B9/$03/           {MOV DX,$03B9  ;set light pen address}
      $EE/                   {OUT DX,AL}
      $BA/$BA/$03/           {MOV DX,$03BA  ;display status port}
      $EC/                   {IN AL,DX}
      $A8/$02/               {TEST AL,2}
      $74/$0E/               {JZ HMTWdone   ;If light pen stuck low, can't do mode test - abort}
      $BA/$BB/$03/           {MOV DX,$03BB  ;reset light pen address}
      $EE/                   {OUT DX,AL}
      $BA/$BA/$03/           {MOV DX,$03BA  ;display status port}
      $EC/                   {IN AL,DX}
      $A8/$02/               {TEST AL,2}
      $75/$02/               {JNZ HMTWdone  ;If light pen stuck high, can't do mode test - abort}
      $FE/$C3/               {INC BL        ;test works}
                             {HMTWdone:}
      $88/$5E/$FF);          {MOV [BP-1],BL ;set function result}
  end;

  procedure SetHercMode(GraphMode : Boolean; GraphPage : Byte);
    {-Set Hercules card to graphics mode or text mode, and activate specified
      graphics page (if switching to graphics mode).}
  begin
    inline(
      $EB/$21/               {JMP SHORT SetHercMode}
                             {SetHercModePrim:}
      $50/                   {PUSH AX                ;save screen mode}
      $BA/$B8/$03/           {MOV DX,$03B8}
      $EE/                   {OUT DX,AL              ;change mode with screen off}
      $B9/$0C/$00/           {MOV CX,12              ;12 parameters to be output starting from}
      $30/$E4/               {XOR AH,AH              ;  register 0}
      $BA/$B4/$03/           {MOV DX,$03B4           ;index register}
      $FC/                   {CLD                    ;go forward}
                             {Parms:}
      $88/$E0/               {MOV AL,AH              ;AL = 0}
      $EE/                   {OUT DX,AL              ;output register number}
      $42/                   {INC DX                 ;data register}
      $AC/                   {LODSB                  ;next 6845 parameter}
      $EE/                   {OUT DX,AL              ;output data}
      $FE/$C4/               {INC AH                 ;next value}
      $4A/                   {DEC DX                 ;back to index register}
      $E2/$F5/               {LOOP Parms             ;repeat}
      $BA/$B8/$03/           {MOV DX,$03B8}
      $58/                   {POP AX                 ;get back screen mode}
      $04/$08/               {ADD AL,8               ;turn screen back on}
      $EE/                   {OUT DX,AL}
      $C3/                   {RET}
                             {SetHercMode:}
      $BA/$BF/$03/           {MOV DX,$03BF           ;configuration port}
      $B0/$01/               {MOV AL,1               ;allow graphics, page 0 only}
      $EE/                   {OUT DX,AL}
      $8A/$46/<GraphMode/    {MOV AL,[BP+<GraphMode]}
      $08/$C0/               {OR  AL,AL              ;switching to graphics mode?}
      $74/$1D/               {JZ  TMode              ;no - back to text}
                             {;set graphics mode}
      $B0/$02/               {MOV AL,2               ;select graphics mode}
      $BE/>HercGtable/       {MOV SI,>HercGtable     ;point to graphics table}
      $E8/$CA/$FF/           {CALL SetHercModePrim}
                             {;need to set page 1 of graphics?}
      $8A/$46/<GraphPage/    {MOV AL,[BP+<GraphPage] ;AL = Page number}
      $08/$C0/               {OR AL,AL               ;is it 0?}
      $74/$16/               {JZ SHMdone             ;if so, we're done}
      $B0/$03/               {MOV AL,3}
      $BA/$BF/$03/           {MOV DX,$03BF}
      $EE/                   {OUT DX,AL              ;allow graphics, both pages}
      $B0/$8A/               {MOV AL,$8A}
      $BA/$B8/$03/           {MOV DX,$03B8}
      $EE/                   {OUT DX,AL              ;set graphics mode, page 1}
      $EB/$08/               {JMP SHORT SHMdone      ;finished}
      {TMode:                 ;set text mode}
      $B0/$20/               {MOV AL,$20             ;select text mode}
      $BE/>HercTtable/       {MOV SI,>HercTtable     ;point SI to text table}
      $E8/$AD/$FF);          {CALL SetHercModePrim}
                             {SHMdone:}
  end;

  procedure SetBlink(On : Boolean);
    {-Enable text mode attribute blinking if On is True}
  const
    PortVal : array[0..4] of Byte = ($0C, $08, $0D, $09, $09);
  var
    PortNum : Word;
    Index : Byte;
    PVal : Byte;
  begin
    case CurrentDisplay of
      MonoHerc :
        begin
          PortNum := $3B8;
          Index := 4;
        end;
      CGA :
        begin
          PortNum := $3D8;
          case LastMode of
            0..3 : Index := LastMode;
          else Exit;
          end;
        end;
      MCGA..VGA :
        begin
          inline(
            $8A/$5E/<On/     {mov bl,[bp+<On]}
            $B8/$03/$10/     {mov ax,$1003}
            $CD/$10);        {int $10}
          Exit;
        end;
    else Exit;
    end;
    PVal := PortVal[Index];
    if On then
      PVal := PVal or $20;
    Port[PortNum] := PVal;
  end;

  procedure SetCrtBorder(Attr : Byte);
    {-Set border to background color if card type and mode allow}
  begin
    inline(
      $8A/$46/<Attr/         {mov AL,[BP+<Attr] ;get attribute}
      $B1/$04/               {mov CL,4          ;shift count}
      $D2/$E8/               {shr AL,CL         ;get background color}
      $24/$07/               {and AL,$07        ;make sure blink bit off}
      $88/$C3/               {mov BL,AL         ;right place for CGA}
      $A0/>CurrentDisplay/   {mov AL,[>CurrentDisplay];get current display}
      $8A/$26/>CurrentMode/  {mov AH,[>CurrentMode];get current mode}
      $3C/$01/               {cmp AL,CGA        ;check for CGA}
      $75/$0E/               {jne chkega        ;nope, go check EGA}
      $80/$FC/$03/           {cmp AH,3          ;check for modes 0-3}
      $77/$1A/               {ja  exit          ;forget it}
      $B4/$0B/               {mov AH,$0B        ;set color palette}
      $B7/$00/               {mov BH,$00        ;color palette ID}
      $CD/$10/               {int $10           ;let BIOS do it}
      $E9/$11/$00/           {jmp exit          ;and get out}
                             {chkega:}
      $3C/$03/               {cmp AL,EGA        ;check for EGA/VGA}
      $72/$0D/               {jb  exit          ;forget it}
      $80/$FC/$03/           {cmp AH,3          ;check for modes 0-3}
      $77/$08/               {ja  exit          ;forget it}
      $B4/$10/               {mov AH,$10        ;set color palette}
      $B0/$01/               {mov AL,$01        ;set border color}
      $88/$DF/               {mov BH,BL         ;the color}
      $CD/$10);              {int $10           ;let BIOS do it}
                             {exit:}
  end;

  function PackWindow(XLow, YLow, XHigh, YHigh : Byte) : PackedWindowPtr;
    {-Return a pointer to a packed window, or nil if not enough memory}
  var
    I, J : Word;
    InitialSize : Word;
    HeaderSize : Word;
    FinalSize : Word;
    Width : Integer;
    Height : Integer;
    SrcOfs : Word;
    PWP : PackedWindowPtr;
  begin
    {compute bytes needed for screen buffer}
    HeaderSize := SizeOf(PackedWindow)-SizeOf(PackedScreen);
    Width := Succ(Integer(XHigh)-Integer(XLow));
    Height := Succ(Integer(YHigh)-Integer(YLow));
    InitialSize := ((Width*Height) shl 1)+HeaderSize;

    {check available memory}
    if (MaxAvail < InitialSize) or (Width <= 0) or (Height <= 0) then begin
      PackWindow := nil;
      Exit;
    end;
    GetMem(PWP, InitialSize);

    {calculate starting offset on screen}
    SrcOfs := (Pred(YLow)*VirtualWidth+Pred(XLow)) shl 1;

    {pack the characters and attributes}
    with PWP^ do begin
      I := PackPrim(Height, Width, Seg(Contents), Ofs(Contents), SrcOfs, CDelta);
      J := PackPrim(Height, Width, Seg(Contents), Ofs(Contents[I+1]), SrcOfs+1, ADelta);
    end;

    {initialize the rest of the header}
    with PWP^ do begin
      FinalSize := HeaderSize+I+J;
      Size := FinalSize;
      TopRow := YLow;
      TopCol := XLow;
      Rows := Height;
      Cols := Width;
      AStart := I+1;
    end;

    {return pointer to packed window}
    PackWindow := PWP;

    {free up unused portion of window}
    PWP := Normalized(@PWP^.Contents[I+J+1]);
    FreeMem(PWP, InitialSize-FinalSize);
  end;

  procedure DisposePackedWindow(var PWP : PackedWindowPtr);
    {-Dispose of a packed window, setting PWP to nil on exit}
  begin
    if PWP <> nil then begin
      FreeMem(PWP, PWP^.Size);
      PWP := nil;
    end;
  end;

  procedure DispPackedWindowAt(PWP : PackedWindowPtr; Row, Col : Byte);
    {-Display the packed window pointed to by PWP at Row,Col. If necessary,
      the coordinates are adjusted to allow it to fit on the screen.}
  var
    SaveRow, SaveCol : Byte;
  begin
    with PWP^ do begin
      {check for 0}
      if Row = 0 then
        Row := 1;
      if Col = 0 then
        Col := 1;

      {make sure the window can be displayed}
      if Pred(Row)+Rows > ScreenHeight then
        Row := Succ(ScreenHeight-Rows);
      if Pred(Col)+Cols > ScreenWidth then
        Col := Succ(ScreenWidth-Cols);

      {save the current coordinates and reset them temporarily}
      SaveRow := TopRow;
      TopRow := Row;
      SaveCol := TopCol;
      TopCol := Col;

      {display the window}
      DispPackedWindow(PWP);

      {restore the old coordinates}
      TopRow := SaveRow;
      TopCol := SaveCol;
    end;
  end;

  procedure MapPackedWindowColors(PWP : PackedWindowPtr);
    {-Map the colors in a packed window for improved appearance on mono/B&W
      displays}
  var
    Segm, Ofst : Word;
    LastOfst, Count : Word;
  begin
    {get out fast if color mapping not needed}
    if (CurrentMode = 1) or (CurrentMode = 3) or not MapColors then
      Exit;

    with PWP^ do begin
      {get starting address of the attributes in Contents}
      Segm := Seg(Contents);
      Ofst := Ofs(Contents[AStart]);

      {calculate offset of last element in Contents}
      LastOfst := Ofs(Size)+Pred(Size);

      {get number of attributes before the first PackRec}
      Count := ADelta;

      repeat
        {is it a PackRec?}
        if Count = 0 then begin
          {get number of attributes before the next PackRec}
          Count := MemW[Segm:Ofst];

          {map the FillVal}
          Mem[Segm:Ofst+2] := not MapColor(not Mem[Segm:Ofst+2]);

          {skip over the PackRec}
          Inc(Ofst, SizeOf(PackRec));
        end
        else begin
          {map the attribute}
          Mem[Segm:Ofst] := not MapColor(not Mem[Segm:Ofst]);

          {decrement counter to next PackRec}
          Dec(Count);

          {skip ahead}
          Inc(Ofst);
        end;
      until (Ofst > LastOfst) or (Ofst = 0);
    end;
  end;

  procedure WritePackedWindow(PWP : PackedWindowPtr; FName : string);
    {-Store the packed window pointed to by PWP in FName}
  var
    F : file;
    I : Word;
  begin
    {check for nil pointer}
    if PWP = nil then begin
      CrtError := CrtNilPointer;
      Exit;
    end;

    {try to open the file}
    Assign(F, FName);
    Rewrite(F, 1);
    CrtError := IoResult;
    if CrtError = 0 then begin
      {write the packed window to disk}
      BlockWrite(F, PWP^, PWP^.Size);
      CrtError := IoResult;
    end;

    {close the file}
    Close(F);
    if CrtError = 0 then
      CrtError := IoResult
    else
      I := IoResult;
  end;

  function ReadPackedWindow(FName : string) : PackedWindowPtr;
    {-Read the packed window stored in FName into memory}
  var
    PWP : PackedWindowPtr;
    F : file;
    I : Word;
  label
    Done;
  begin
    ReadPackedWindow := nil;

    {try to open the file}
    Assign(F, FName);
    Reset(F, 1);
    CrtError := IoResult;
    if CrtError <> 0 then
      Exit;

    {allocate memory for the window}
    I := FileSize(F);
    if (I > MaxAvail) then begin
      CrtError := CrtNotEnoughMem;
      goto Done;
    end;
    GetMem(PWP, I);

    {read the window into memory}
    BlockRead(F, PWP^, I);
    CrtError := IoResult;
    if CrtError = 0 then begin
      if PWP^.Size <> I then begin
        CrtError := CrtBadWindow;
        FreeMem(PWP, I);
      end
      else
        ReadPackedWindow := PWP;
    end
    else
      FreeMem(PWP, I);

Done:
    Close(F);
    if CrtError = 0 then
      CrtError := IoResult
    else
      I := IoResult;
  end;

  function CreateLibrary(var F : file; Name : string; Entries : Byte) : DirectoryPtr;
    {-Create a library with the specified # of directory entries}
  var
    FHandle : Word absolute F;
    I, J, K : Word;
    DP : DirectoryPtr;
  begin
    CreateLibrary := nil;

    {add main directory entry and round to multiple of 4}
    I := (Word(Entries)+4) and $FFFC;

    {allocate the directory}
    J := I*SizeOf(DirectoryEntry);
    if MaxAvail < J then begin
      CrtError := CrtNotEnoughMem;
      Exit;
    end;
    GetMem(DP, J);

    {open the library}
    Assign(F, Name);
    Rewrite(F, 1);
    CrtError := IoResult;
    if CrtError <> 0 then begin
      FreeMem(DP, J);
      Close(F);
      I := IoResult;
      Exit;
    end;

    {initialize the directory}
    FillChar(DP^, J, 0);

    {initialize the main directory entry}
    with DP^[0] do begin
      {name is all blanks}
      Name := BlankDeName;

      {size of the directory}
      MemberLength := I div 4;
    end;

    {mark the remaining entries as unused}
    for K := 1 to Pred(I) do
      DP^[K].Status := deUnused;

    {write the directory to disk}
    BlockWrite(F, DP^, J);
    CrtError := IoResult;
    if CrtError = 0 then
      CreateLibrary := DP
    else begin
      FreeMem(DP, J);
      Close(F);
      I := IoResult;
    end;
  end;

  function OpenLibrary(var F : file; Name : string) : DirectoryPtr;
    {-Open the specified library and return a pointer to its directory}
  var
    DP : DirectoryPtr;
    DE : DirectoryEntry;
    I : Word;
  begin
    OpenLibrary := nil;

    {open the library}
    Assign(F, Name);
    Reset(F, 1);
    CrtError := IoResult;
    if CrtError <> 0 then
      Exit;

    {read in the first directory entry and check its validity}
    BlockRead(F, DE, SizeOf(DE));
    CrtError := IoResult;
    if CrtError = 0 then
      if (DE.Status <> deInUse) or (DE.Name <> BlankDeName) then
        CrtError := CrtNotValidLib;
    if CrtError <> 0 then begin
      Close(F);
      I := IoResult;
      Exit;
    end;

    {calculate size of directory and allocate it}
    I := DE.MemberLength*128;
    if I > MaxAvail then begin
      CrtError := CrtNotEnoughMem;
      Close(F);
      I := IoResult;
      Exit;
    end;
    GetMem(DP, I);

    {reposition file pointer and read in the directory}
    Seek(F, 0);
    BlockRead(F, DP^, I);
    CrtError := IoResult;
    if CrtError = 0 then
      OpenLibrary := DP
    else begin
      FreeMem(DP, I);
      Close(F);
      I := IoResult;
    end;
  end;

  procedure CloseLibrary(var F : file; var DP : DirectoryPtr);
    {-Close library F and deallocate its directory}
  var
    I : Word;
  begin
    {close the file}
    Close(F);
    CrtError := IoResult;

    {deallocate the directory}
    FreeMem(DP, DP^[0].MemberLength*128);
    DP := nil;
  end;

  procedure MakeEntryName(S : LibName; var N : deName);
    {-Make a directory name out of a regular filename}
  var
    I, J : Word;
  begin
    {initialize with blanks}
    FillChar(N, SizeOf(N), ' ');

    {convert name to upper case}
    for I := 1 to Length(S) do
      S[I] := Upcase(S[I]);

    I := Pos('.', S);
    if I = 0 then begin
      {plug in the name and leave extension blank}
      J := Length(S);
      if J > 8 then
        J := 8;
      Move(S[1], N, J);
    end
    else begin
      {plug in the name, 8 chars max}
      if I > 9 then
        J := 8
      else
        J := Pred(I);
      Move(S[1], N, J);

      {plug in the extension, 3 chars max}
      J := Length(S)-I;
      if J > 3 then
        J := 3;
      Move(S[I+1], N[9], J);
    end;
  end;

  function FindWinName(DP : DirectoryPtr; WinName : LibName; DelCheck : Boolean) : Word;
    {-Find the directory entry for WinName}
  var
    DN : deName;
    I, J : Word;
  begin
    FindWinName := 0;
    MakeEntryName(WinName, DN);
    J := DP^[0].MemberLength*4;
    for I := 1 to Pred(J) do
      with DP^[I] do
        if ((Status = deInUse) or not DelCheck) and (Name = DN) then begin
          FindWinName := I;
          Exit;
        end;
  end;

  function UpdateDirectory(var F : file; DP : DirectoryPtr; Entry : Byte) : Boolean;
    {-Update the specified directory entry}
  begin
    UpdateDirectory := False;
    {seek to the right location}
    Seek(F, Entry*SizeOf(DirectoryEntry));
    CrtError := IoResult;
    if CrtError <> 0 then
      Exit;

    {write the directory entry to disk}
    BlockWrite(F, DP^[Entry], SizeOf(DirectoryEntry));
    CrtError := IoResult;
    UpdateDirectory := (CrtError = 0);
  end;

  procedure AddWindowToLibrary(PWP : PackedWindowPtr; var F : file;
                               DP : DirectoryPtr; WinName : LibName);
    {-Add a packed window to the specified library}
  var
    Padding : array[1..128] of Byte;
    S, I, J, K : Word;
    FP : LongInt;
  begin
    {mark any existing entry with this name as deleted}
    I := FindWinName(DP, WinName, True);
    if I <> 0 then begin
      DP^[I].Status := deDeleted;
      if not UpdateDirectory(F, DP, I) then
        Exit;
    end;

    {find the first blank directory entry}
    J := DP^[0].MemberLength*4;
    I := 1;
    K := 0;
    while (K = 0) and (I < J) do
      if DP^[I].Status = deUnused then
        K := I
      else
        Inc(I);
    if K = 0 then begin
      CrtError := CrtDirectoryFull;
      Exit;
    end;

    {seek to end of file}
    Seek(F, FileSize(F));
    CrtError := IoResult;
    if CrtError <> 0 then
      Exit;
    FP := FilePos(F);

    {write the packed window}
    S := PWP^.Size;
    BlockWrite(F, PWP^, S);
    CrtError := IoResult;
    if CrtError <> 0 then
      Exit;

    {pad to the end of the block with ^Z}
    J := 128-(S and $7F);
    if J <> 0 then begin
      FillChar(Padding, J, ^Z);
      BlockWrite(F, Padding, J);
      CrtError := IoResult;
      if CrtError <> 0 then
        Exit;
    end;

    {update the directory}
    with DP^[K] do begin
      Status := deInUse;
      MakeEntryName(WinName, Name);
      MemberLength := (S+J) div 128;
      Index := FP div 128;
      PadCount := J;
    end;
    if UpdateDirectory(F, DP, K) then {} ;
  end;

  function ReadWindowPrim(var F : file; DP : DirectoryPtr;
                          Entry : Byte) : PackedWindowPtr;
    {-Primitive routine to read a packed window from a library}
  var
    PWP : PackedWindowPtr;
    I : Word;
  begin
    ReadWindowPrim := nil;
    with DP^[Entry] do begin
      {calculate size of entry and allocate the window}
      I := (MemberLength*128)-PadCount;
      if I > MaxAvail then begin
        CrtError := CrtNotEnoughMem;
        Exit;
      end;
      GetMem(PWP, I);

      {seek to the start of the window}
      Seek(F, LongInt(Index)*128);
      CrtError := IoResult;
      if CrtError <> 0 then begin
        FreeMem(PWP, I);
        Exit;
      end;

      {read in the packed window}
      BlockRead(F, PWP^, I);
      CrtError := IoResult;
      if CrtError = 0 then begin
        if PWP^.Size <> I then begin
          CrtError := CrtBadWindow;
          FreeMem(PWP, I);
        end
        else
          ReadWindowPrim := PWP
      end
      else
        FreeMem(PWP, I);
    end;
  end;

  function ReadWindowFromLibrary(var F : file; DP : DirectoryPtr;
                                 WinName : LibName) : PackedWindowPtr;
    {-Read a packed window from a library and return a pointer to it}
  var
    I : Word;
  begin
    {locate the window's directory entry}
    I := FindWinName(DP, WinName, True);
    if I = 0 then
      ReadWindowFromLibrary := nil
    else
      ReadWindowFromLibrary := ReadWindowPrim(F, DP, I);
  end;

  procedure DeleteWindowFromLibrary(var F : file; DP : DirectoryPtr;
                                    WinName : LibName);
    {-Delete a packed window from the specified library}
  var
    I, J : Word;
  begin
    {locate the window's directory entry}
    I := FindWinName(DP, WinName, True);
    if I = 0 then begin
      CrtError := CrtWinNotFound;
      Exit;
    end;

    {mark entry as deleted}
    DP^[I].Status := deDeleted;
    if UpdateDirectory(F, DP, I) then {} ;
  end;

  procedure PackLibrary(LName : string);
    {-Pack a library to remove deleted entries.}
  var
    FOld, FNew : file;
    DPold, DPnew : DirectoryPtr;
    I, J, K : Word;
    PWP : PackedWindowPtr;
  label
    FoundOne;

    function deNameToString(Name : deName) : string;
      {-Convert a deName to a string}
    var
      S : string;
      I : Word;
    begin
      S[0] := #0;
      for I := 1 to 8 do
        if Name[I] <> ' ' then
          S := S+Name[I];
      if Name[9] <> ' ' then begin
        S := S+'.';
        for I := 9 to 11 do
          if Name[I] <> ' ' then
            S := S+Name[I];
      end;
      deNameToString := S;
    end;

  begin
    {open the library}
    DPold := OpenLibrary(FOld, LName);
    if DPold = nil then
      Exit;

    {make sure there are deleted entries}
    J := DPold^[0].MemberLength*4;
    for I := 1 to Pred(J) do
      if DPold^[I].Status = deDeleted then
        goto FoundOne;

    {if we get here, there are no deleted entries}
    CloseLibrary(FOld, DPold);
    Exit;

FoundOne:
    {create new library}
    DPnew := CreateLibrary(FNew, '$$WORK$$.LBR', J-1);
    if DPnew = nil then begin
      CloseLibrary(FOld, DPold);
      Exit;
    end;

    {add all undeleted entries to new lib}
    for I := 1 to Pred(J) do
      with DPold^[I] do
        if Status = deInUse then begin
          PWP := ReadWindowPrim(FOld, DPold, I);
          if PWP <> nil then
            AddWindowToLibrary(PWP, FNew, DPnew, deNameToString(Name));
          if (PWP = nil) or (CrtError <> 0) then begin
            K := CrtError;              {!!.08}
            CloseLibrary(FOld, DPold);
            CloseLibrary(FNew, DPnew);
            if K <> 0 then              {!!.08}
              CrtError := K;            {!!.08}
            Erase(FNew);
            K := IoResult;
            Exit;
          end
          else
            DisposePackedWindow(PWP)
        end;

    {delete old lib, rename new one}
    CloseLibrary(FOld, DPold);
    Erase(FOld);
    K := IoResult;
    CloseLibrary(FNew, DPnew);
    Rename(FNew, LName);
    K := IoResult;
    if CrtError = 0 then
      CrtError := K;
  end;

begin
  {try to detect presence of CRT in the same program}
  Mark(SaveExitProc);
  inline(
    $E8/$00/$00/           {call x}
    $5B/                   {x: pop bx}
    $2E/$8B/$47/$FB/       {mov ax,cs:[bx-5]}
    $A3/>SystemSeg);       {mov [>SystemSeg],ax}

  if SegOfs(FIB(Input).InOutProc).Segm <> SystemSeg then
    CrtTest;

  {initialize global variables}
  CheckBreak := True;
  CheckEOF := False;

  {for internal use}
  IsCompaq := (CompaqBiosName = 'COMPAQ');
  if (ZenithBiosName = 'Zenith') then
    IsZenith := True;

  {initialize internal variables}
  InitCrt;

  {activate colors on Hercules InColor card}
  if WhichHerc = HercInColor then
    SwitchInColorCard(True);

  {$IFNDEF Ver40}
   {disable our exit handler it this isn't TP 4.0}
   ExitProc := SaveExitProc;
 {$ENDIF}

  {reopen Input}
  AssignCrt(Input);
  Reset(Input);

  {reopen Output}
  AssignCrt(Output);
  Rewrite(Output);

  {correct some BIOS bugs involving cursor scan lines}
  case BiosScanLines of
    $0607 :
      if CurrentMode = 7 then
        {mono adapter, but CGA scan lines -- happens on most mono systems}
        BiosScanLines := $0B0C;
    $0067 :
      {incorrect scan lines bug -- most often seen on Compaqs}
      BiosScanLines := $0607;
  end;
end.
