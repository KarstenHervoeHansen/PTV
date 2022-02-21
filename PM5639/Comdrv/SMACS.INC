
  {*********************************************************}
  {*                    SMACS.INC 5.08                     *}
  {*           Low-level routines for SMACS.PAS            *}
  {*        Copyright (c) TurboPower Software 1987.        *}
  {* Portions copyright (c) Sunny Hill Software 1985, 1986 *}
  {*     and used under license to TurboPower Software     *}
  {*                 All rights reserved.                  *}
  {*********************************************************}

  procedure SetAttributes;
    {-Set video attribute variables based on the current video mode}
  var
    MonoColors : Boolean;
  begin
    {set video attributes}
    case CurrentMode of
      2, 7 :
        MonoColors := WhichHerc <> HercInColor;
      else
        MonoColors := False;
    end;

    if MonoColors then begin
      Bright := $F;          {white on black}
      Dim := $7;             {light gray on black}
      Reverse := $70;        {black on light gray}
    end
    else begin
      Bright := $F;          {white on black}
      Dim := $E;             {yellow on black}
      Reverse := $1F;        {white on blue}
    end;
  end;

  function Decimal(L : LongInt; Width : Byte) : String10;
    {-Return a string representing a decimal number}
  var
    S : String10;
  begin
    Str(L:Width, S);
    Decimal := S;
  end;

  procedure PromptBox(var Title : string; TopRow, BotRow,
                      LeftCol, RightCol : Byte);
    {-Draw a prompt box}
  var
    Blank : String80;
    BLen : Byte absolute Blank;
    Row : Word;
  begin
    if not(InTextMode and (ScreenWidth = 80)) then
      Exit;

    {save the screen if it hasn't been done yet}
    if not WholeScreenSaved then begin
      if SaveWindow(1, 1, 80, MinWord(RowsToSave, ScreenHeight), False, MainBufPtr) then
        {won't fail} ;
      WholeScreenSaved := True;
    end;

    {draw a box around the window}
    FrameWindow(LeftCol, TopRow, RightCol, BotRow, Dim, Bright, Title);

    {clear the inside of the window}
    BLen := Pred(RightCol-LeftCol);
    FillChar(Blank[1], BLen, ' ');
    for Row := Succ(TopRow) to Pred(BotRow) do
      FastWrite(Blank, Row, Succ(LeftCol), Dim);
  end;

  procedure Message(var Title, Msg : string);
    {-Display a message box}
  begin
    if not(InTextMode and (ScreenWidth = 80)) then
      Exit;
    PromptBox(Title, MsgTop, MsgBot, MsgLeft, MsgRight);
    FastWrite(Msg, Pred(MsgBot), MsgLeft+2, Dim);
  end;

  procedure RestoreMessageWindow;
    {-Restore the screen beneath the message window}
  begin
    if InhibitRestore then
      Exit;

    {restore the message window using the main screen buffer}
    RestoreWindow(1, MsgTop, 80, MsgBot, False, MessageBufPtr);
  end;

  function MessageChar(var Title, Msg : string) : Word;
    {-Display a message box, wait for a key, and return it}
  var
    SaveState : Boolean;
  begin
    if not(InTextMode and (ScreenWidth = 80)) then
      Exit;
    Message(Title, Msg);
    SaveState := MacrosAreOn;
    MacrosOff;
    MessageChar := ReadKeyWord;
    MacrosAreOn := SaveState;
    RestoreMessageWindow;
  end;

  function MessageYesOrNo(var Title, Msg : string; Default : Char) : Boolean;
    {-Display a message box and ask a yes-no question. Returns true for yes.}
  var
    SaveState : Boolean;
  begin
    if not(InTextMode and (ScreenWidth = 80)) then
      Exit;
    Message(Title, NullString);
    SaveState := MacrosAreOn;
    MacrosOff;
    MessageYesOrNo := YesOrNo(Msg, Pred(MsgBot), MsgLeft+2, Dim, Default);
    MacrosAreOn := SaveState;
    RestoreMessageWindow;
  end;

  procedure MessageDelay(Msg : String80; N : Word);
    {-Display a message box, and delay for a count of N}
  begin
    Message(NullString, Msg);
    Delay(N);
    RestoreMessageWindow;
  end;

  procedure MessageString(var Title, Msg, Reply : string; var Escaped : Boolean);
    {-Display a message box, and get the user's reply to the prompt}
  var
    MaxLen : Word;
  begin
    {show message box}
    Message(Title, NullString);

    {calculate maximum length of string}
    MaxLen := (MsgRight-Succ(MsgLeft))-Succ(Length(Msg))-2;
    if Length(Msg) = 0 then
      Inc(MaxLen);

    {read the string and restore the window}
    ReadString(Msg, Pred(MsgBot), MsgLeft+2, MaxLen, Dim, Dim, Dim, Escaped, Reply);
    RestoreMessageWindow;
  end;

  procedure Beep;
    {-Ring that obnoxious bell}
  begin
    Write(^G);
  end;

  procedure ErrorMessage(Msg : String80);
    {-Display an error message and wait for a keystroke}
  var
    Junk : Word;
  begin
    Msg := Msg+'. Press any key...';
    Junk := MessageChar(NullString, Msg);
  end;

  function BlockReadOk(var F : file; var Buf; Num : Word; var NRead : Word) : Boolean;
    {-Do a blockread and check for critical error}
  begin
    BlockRead(F, Buf, Num, NRead);
    BlockReadOk := (Int24Result = 0) and (TpInt24.Int24Result = 0);
  end;

  function BlockWriteOk(var F : file; var Buf; Num : Word; var NWritten : Word) : Boolean;
    {-Do a blockwrite and check for critical error}
  begin
    BlockWrite(F, Buf, Num, NWritten);
    BlockWriteOk := (Int24Result = 0) and (NWritten = Num);
  end;
