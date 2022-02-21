  function HexIfA(C : MenuColorType) : string;
    {-Return hex color code if available}
  begin
    if User = nil then
      HexIfA := ''
    else if User^.Active = nil then
      HexIfA := ''
    else
      HexIfA := HexB(User^.Active^.Colors[C]);
  end;

  {$F+}
  function DisplayFunc(S : string; K : MenuKey) : string;
    {-Customizes the display string for each make menu item}
  const
    HorVert : array[Orientation] of Char = ('V', 'H', #0);
    OffOn : array[Boolean] of string[3] = ('Off', ' On');
  var
    SS : string;
  begin
    case MakeCommands(K) of
      MmenuColorFrame : SS := HexB(UserColors[FrameColor]);
      MmenuColorHeader : SS := HexB(UserColors[HeaderColor]);
      MmenuColorBody : SS := HexB(UserColors[BodyColor]);
      MmenuColorSelect : SS := HexB(UserColors[SelectColor]);
      MmenuColorHilite : SS := HexB(UserColors[HiliteColor]);
      MmenuColorHelp : SS := HexB(UserColors[HelpColor]);
      {$IFDEF Tpro5Menu}
      MmenuColorDisabled : SS := HexB(UserColors[DisabledColor]);
      MmenuColorShadow : SS := HexB(UserColors[ShadowColor]);
      MmenuExplode : SS := OffOn[UserExplode];
      MmenuShadow : SS := OffOn[UserShadow];
      MmenuExpDelay : SS := Long2Str(UserExpDelay);
      MmenuMapColors : SS := OffOn[MapColors];
      {$ENDIF}
      MmenuHelpRow : SS := Long2Str(UserHelpRow);
      MmenuOrient : SS := HorVert[UserOrient];

      MsubColorFrame : SS := HexIfA(FrameColor);
      MsubColorHeader : SS := HexIfA(HeaderColor);
      MsubColorBody : SS := HexIfA(BodyColor);
      MsubColorSelect : SS := HexIfA(SelectColor);
      MsubColorHilite : SS := HexIfA(HiliteColor);
      MsubColorHelp : SS := HexIfA(HelpColor);
      {$IFDEF Tpro5Menu}
      MsubColorDisabled : SS := HexIfA(DisabledColor);
      MsubColorShadow : SS := HexIfA(ShadowColor);
      MsubExplode :
        if User = nil then
          SS := ''
        else if User^.Active = nil then
          SS := ''
        else
          SS := OffOn[User^.Active^.Draw.Exploding];
      MsubShadow :
        if User = nil then
          SS := ''
        else if User^.Active = nil then
          SS := ''
        else
          SS := OffOn[User^.Active^.Draw.Shadowed];
      {$ENDIF}

      MitemKey :
        if User = nil then
          SS := ''
        else if User^.Active = nil then
          SS := ''
        else if User^.Active^.Items.Current = nil then
          SS := ''
        else
          SS := Long2Str(LongInt(User^.Active^.Items.Current^.Key));
    else
      DisplayFunc := S;
      Exit;
    end;
    Move(SS[1], S[Length(S)-Length(SS)], Length(SS));
    DisplayFunc := S;
  end;
  {$F-}

  {$IFDEF UseMouse}
  procedure SaveMousePos;
    {-Save mouse coordinates in a global for later restore}
  begin
    if MouseInstalled then begin
      MX := MouseWhereX;
      MY := MouseWhereY;
    end;
  end;

  procedure RestoreMousePos;
    {-Restore previously saved mouse coordinates}
  begin
    if MouseInstalled then
      MouseGotoXY(MX, MY);
  end;

  function Sign(X : Integer) : Integer;
    {-Clamp integer to range -1..+1}
  begin
    if X < 0 then
      Sign := -1
    else if X > 0 then
      Sign := +1
    else
      Sign := 0;
  end;

  function ReadKeyMouseXY : Word;
    {-ReadKeyWord with mouse buttons mapped to <Enter> and <Esc>}
  const
    {Keys to return for various mouse directions}
    ArrowKeys : array[-1..1, -1..1] of Word =
    {DX=-1  DX=0   DX=+1}
    (($4700, $4800, $4900),  {DY = -1}
      ($4B00, $FFFF, $4D00), {DY = 0}
      ($4F00, $5000, $5100)); {DY = +1}
  var
    KW : Word;
    MW : Word;
    InitMX : Byte;
    InitMY : Byte;
    Xdel : Integer;
    Ydel : Integer;
  begin
    SetMickeyToPixelRatio(16, 32);
    InitMX := MouseWhereX;
    InitMY := MouseWhereY;
    KW := $FFFF;
    repeat
      if KeyPressed then
        KW := ReadKeyWord
      else if MousePressed then begin
        MW := MouseKeyWord;
        if MW = MouseLft then
          KW := $1C0D
        else if MW = MouseRt then
          KW := $011B;
      end else begin
        Xdel := Integer(MouseWhereX)-InitMX;
        Ydel := Integer(MouseWhereY)-InitMY;
        if Abs(Xdel) > 2*Abs(Ydel) then
          Ydel := 0
        else if Abs(Ydel) > 2*Abs(Xdel) then
          Xdel := 0;
        KW := ArrowKeys[Sign(Ydel), Sign(Xdel)];
      end;
    until KW <> $FFFF;
    ReadKeyMouseXY := KW;
    SetMickeyToPixelRatio(8, 16);
  end;
  {$ENDIF}

  procedure DisableMakeWhenSaved;
    {-Disable Make menu items when User menu has been saved}
  begin
    {$IFDEF Tpro5Menu}
    DisableMenuItem(Make, Ord(MfileWrite));
    HotKeySet := HotKeySet-[^W];
    SetMenuSelectKeys(Make, HotKeySet);
    {$ENDIF}
    Saved := True;
  end;

  procedure DisableMakeNoItems;
    {-Disable Make menu items and hot keys when no User items are available}
  begin
    {$IFDEF Tpro5Menu}
    DisableMenuItem(Make, Ord(MmenuChoose));
    DisableMenuItem(Make, Ord(MmenuNew));
    DisableMenuItem(Make, Ord(MmenuKeys));
    DisableMenuItem(Make, Ord(Msubmenu));
    DisableMenuItem(Make, Ord(MitemEdit));
    DisableMenuItem(Make, Ord(MitemKey));
    DisableMenuItem(Make, Ord(MitemMove));
    DisableMenuItem(Make, Ord(MitemOrder));
    DisableMenuItem(Make, Ord(MitemHelp));
    DisableMenuItem(Make, Ord(MitemSelectChar));
    DisableMenuItem(Make, Ord(MitemRemove));
    DisableMenuItem(Make, Ord(MfileWrite));
    DisableMenuItem(Make, Ord(MfileGenerate));
    HotKeySet := HotKeySet-[^C, ^S, ^D, ^E, ^A, ^W, ^G, ^O, ^N];
    SetMenuSelectKeys(Make, HotKeySet);
    ItemsDisabled := True;
    {$ENDIF}
  end;

  procedure EnableMakeWhenItems;
    {-Re-enable Make menu items and hot keys when User items are available}
  begin
    {$IFDEF Tpro5Menu}
    if ItemsDisabled then begin
      EnableMenuItem(Make, Ord(MmenuChoose));
      EnableMenuItem(Make, Ord(MmenuNew));
      EnableMenuItem(Make, Ord(MmenuKeys));
      EnableMenuItem(Make, Ord(Msubmenu));
      EnableMenuItem(Make, Ord(MitemEdit));
      EnableMenuItem(Make, Ord(MitemKey));
      EnableMenuItem(Make, Ord(MitemMove));
      EnableMenuItem(Make, Ord(MitemOrder));
      EnableMenuItem(Make, Ord(MitemHelp));
      EnableMenuItem(Make, Ord(MitemSelectChar));
      EnableMenuItem(Make, Ord(MitemRemove));
      EnableMenuItem(Make, Ord(MfileWrite));
      EnableMenuItem(Make, Ord(MfileGenerate));
      HotKeySet := HotKeySet+[^C, ^S, ^D, ^E, ^A, ^W, ^G, ^O, ^N];
      SetMenuSelectKeys(Make, HotKeySet);
      ItemsDisabled := False;
    end;
    {$ENDIF}
  end;

  procedure MarkNotSaved;
    {-Enable certain items when menus not saved to disk}
  begin
    {$IFDEF Tpro5Menu}
    if Saved then begin
      EnableMenuItem(Make, Ord(MfileWrite));
      HotKeySet := HotKeySet+[^W];
      SetMenuSelectKeys(Make, HotKeySet);
    end;
    {$ENDIF}
    Saved := False;
  end;

  procedure SetFactoryDefaults;
    {-Set default user items to the factory values}
  begin
    UserColors := DefColors;
    UserExplode := DefExplode;
    UserShadow := DefShadow;
    UserShadowMode := DefShadowMode;
    UserExpDelay := DefExpDelay;
    UserHelpRow := DefHelpRow;
    UserStartRow := DefStartRow;
    UserFrame := DefFrame;
    UserOrient := DefOrient;
  end;

  procedure SetDefaultsFromRoot;
    {-Set default user items from the current root submenu}
  begin
    if User <> nil then
      with User^.Root^ do begin
        UserHelpRow := YHelp;
        UserOrient := Orient;
        UserFrame := Frame;
        UserColors := Colors;
        with Draw do begin
          UserExplode := Exploding;
          UserShadow := Shadowed;
          UserExpDelay := ExploDelay;
        end;
      end;
  end;

  procedure BuildMakeMenu;
    {-Build the menu system for for MakeMenu}
  begin
    {start a new menu system}
    Make := NewMenu(HotKeySet, @DisplayFunc);

    {add structure to the menu}
    {.F-}
    SubMenu(7, 6, MakeHR, Horizontal, Frame1, Colors, ' Turbo Professional Menu Maker 5.08 ');
      MenuItem(' Global ', 2, 2, Ord(Mmenu), ' Create a new menu, choose item to edit, or select default settings');
      SubMenu(8, 8, MakeHR, Vertical, Frame2, Colors, '');
        MenuItem('Choose ^C', 1, 1, Ord(MmenuChoose), ' Browse the menu and select the current item to edit');
        MenuItem('New    ^N', 2, 1, Ord(MmenuNew), ' Create a new menu. Be sure to save any existing menu first');
        MenuItem('Defaults', 3, 1, Ord(MmenuSettings), ' Select default colors, frame, orientation and help row');
        SubMenu(14, 12, MakeHR, Vertical, Frame1, Colors, '');
          MenuItem('Colors', 1, 1, Ord(MmenuColors), ' Choose default colors for frame, title, body, select, hilite, help');
          SubMenu(22, 14, MakeHR, Vertical, Frame1, Colors, '');
            MenuItem('Frame', 1, 1, Ord(MmenuColorFrame), ' Choose default color for menu frames');
            MenuItem('Title', 2, 1, Ord(MmenuColorHeader), ' Choose default color for menu title strings');
            MenuItem('Body', 3, 1, Ord(MmenuColorBody), ' Choose default color for unselected menu items');
            MenuItem('Selected   ', 4, 1, Ord(MmenuColorSelect), ' Choose default color for selected menu items');
            MenuItem('Pick', 5, 1, Ord(MmenuColorHilite), ' Choose default color for character highlighted to pick item');
            MenuItem('Help', 6, 1, Ord(MmenuColorHelp), ' Choose default color for help lines');
            {$IFDEF Tpro5Menu}
            MenuItem('Disabled', 7, 1, Ord(MmenuColorDisabled), ' Choose default color for disabled items');
            MenuItem('Shadow', 8, 1, Ord(MmenuColorShadow), ' Choose default color for shadows');
            {$ENDIF}
            PopSubLevel;
          MenuItem('Frame', 2, 1, Ord(MmenuFrame), ' Choose default type of frame for submenus');
          SubMenu(22, 13, MakeHR, Vertical, Frame1, Colors, '');
            MenuItem('1 É»È¼Íº', 1, 1, Ord(MmenuFrame1), ' Select double bar border');
            MenuItem('2 Ú¿ÀÙÄ³', 2, 1, Ord(MmenuFrame2), ' Select single bar border');
            MenuItem('3 none  ', 3, 1, Ord(MmenuFrame3), ' Select no border (Lotus style menus)');
            MenuItem('4 ËËÈ¼Íº', 4, 1, Ord(MmenuFrame4), ' Select double bar border to attach to pulldown menu');
            MenuItem('5 ÂÂÀÙÄ³', 5, 1, Ord(MmenuFrame5), ' Select single bar border to attach to pulldown menu');
            MenuItem('6 Õ¸Ô¾Í³', 6, 1, Ord(MmenuFrame6), ' Select mixed bar border');
            MenuItem('7 ÑÑÔ¾Í³', 7, 1, Ord(MmenuFrame7), ' Select mixed bar border to attach to pulldown menu');
            PopSubLevel;
          MenuItem('Help row', 3, 1, Ord(MmenuHelpRow), ' Select default row to display help');
          MenuItem('Orientation  ', 4, 1, Ord(MmenuOrient), ' Select default orientation - horizontal or vertical');
          SubMenu(22, 17, MakeHR, Vertical, Frame1, Colors, '');
            MenuItem('Horizontal', 1, 1, Ord(MmenuHoriz), ' Menus scroll horizontally');
            MenuItem('Vertical', 2, 1, Ord(MmenuVert), ' Menus scroll vertically');
            PopSubLevel;
          {$IFDEF Tpro5Menu}
          MenuItem('Explode      ', 5, 1, Ord(MmenuExplode), 'Select whether menus explode by default');
          MenuItem('Shadow       ', 6, 1, Ord(MmenuShadow), 'Select whether menus are shadowed by default');
          MenuItem('Delay        ', 7, 1, Ord(MmenuExpDelay), 'Select delay (ms) between explosion stages');
          MenuItem('Map', 8, 1, Ord(MmenuMapColors), 'Toggle color mapping');
          MenuItem('Restore', 9, 1, Ord(MmenuRestore), 'Restore default settings to original values');
          {$ELSE}
          MenuItem('Map', 5, 1, Ord(MmenuMapColors), 'Toggle color mapping');
          MenuItem('Restore', 6, 1, Ord(MmenuRestore), 'Restore default settings to original values');
          {$ENDIF}
          PopSubLevel;
        MenuItem('Redraw', 4, 1, Ord(MmenuRedraw), ' Redraw screen if it becomes corrupted');
        MenuItem('Info', 5, 1, Ord(MmenuInfo), ' Show information about menu system');
        MenuItem('Keys', 6, 1, Ord(MmenuKeys), ' Sequentially renumber all menu item keys');
        PopSubLevel;
      MenuItem(' Submenu ', 21, 2, Ord(Msubmenu), ' Add, move or remove a submenu, or change current submenu settings');
      SubMenu(27, 8, MakeHR, Vertical, Frame2, Colors, '');
        MenuItem('Add   ^S', 1, 1, Ord(MsubAdd), ' Add a new submenu to the current item');
        MenuItem('Drag  ^D', 2, 1, ord(MsubDrag), ' Change screen position of submenu and all descendents');
        MenuItem('Settings', 3, 1, Ord(MsubSettings), ' Change color, frame, orientation, etc. of current submenu');
        SubMenu(34, 12, MakeHR, Vertical, Frame1, Colors, '');
          MenuItem('Colors', 1, 1, Ord(MsubColors), ' Change colors for frame, title, body, select, hilite, or help');
          SubMenu(42, 14, MakeHR, Vertical, Frame1, Colors, '');
            MenuItem('Frame', 1, 1, Ord(MsubColorFrame), ' Change color of menu frame');
            MenuItem('Title', 2, 1, Ord(MsubColorHeader), ' Change color of menu title string');
            MenuItem('Body', 3, 1, Ord(MsubColorBody), ' Change color of unselected menu item');
            MenuItem('Selected   ', 4, 1, Ord(MsubColorSelect), ' Change color of selected menu item');
            MenuItem('Pick', 5, 1, Ord(MsubColorHilite), ' Change color of character highlighted to pick item');
            MenuItem('Help', 6, 1, Ord(MsubColorHelp), ' Change color of help lines');
            {$IFDEF Tpro5Menu}
            MenuItem('Disabled', 7, 1, Ord(MsubColorDisabled), ' Change color of disabled items' );
            MenuItem('Shadow', 8, 1, Ord(MsubColorShadow), ' Change color of menu shadows' );
            {$ENDIF}
            PopSubLevel;
          MenuItem('Frame', 2, 1, Ord(MsubFrame), ' Choose type of frame for this submenu');
          SubMenu(42, 13, MakeHR, Vertical, Frame1, Colors, '');
            MenuItem('1 É»È¼Íº', 1, 1, Ord(MsubFrame1), ' Select double bar border');
            MenuItem('2 Ú¿ÀÙÄ³', 2, 1, Ord(MsubFrame2), ' Select single bar border');
            MenuItem('3 none  ', 3, 1, Ord(MsubFrame3), ' Select no border (Lotus style menus)');
            MenuItem('4 ËËÈ¼Íº', 4, 1, Ord(MsubFrame4), ' Select double bar border to attach to pulldown menu');
            MenuItem('5 ÂÂÀÙÄ³', 5, 1, Ord(MsubFrame5), ' Select single bar border to attach to pulldown menu');
            MenuItem('6 Õ¸Ô¾Í³', 6, 1, Ord(MsubFrame6), ' Select mixed bar border');
            MenuItem('7 ÑÑÔ¾Í³', 7, 1, Ord(MsubFrame7), ' Select mixed bar border to attach to pulldown menu');
            PopSubLevel;
          MenuItem('Help row', 3, 1, Ord(MsubHelpRow), ' Select row to display help');
          MenuItem('Orientation', 4, 1, Ord(MsubOrient), ' Select orientation of menu - horizontal or vertical');
          SubMenu(42, 17, MakeHR, Vertical, Frame1, Colors, '');
            MenuItem('Horizontal', 1, 1, Ord(MsubHoriz), ' Menus scroll horizontally');
            MenuItem('Vertical', 2, 1, Ord(MsubVert), ' Menus scroll vertically');
            PopSubLevel;
          {$IFDEF Tpro5Menu}
          MenuItem('Explode', 5, 1, Ord(MsubExplode), 'Toggle whether menu explodes');
          MenuItem('Shadow', 6, 1, Ord(MsubShadow), 'Toggle whether menu is shadowed');
          {$ENDIF}
          PopSubLevel;
        MenuItem('Title', 4, 1, Ord(MsubHeader), ' Edit title string for menu');
        MenuItem('Move', 5, 1, Ord(MsubMove), ' Change the screen position of a single submenu');
        MenuItem('Resize', 6, 1, Ord(MsubResize), ' Change the dimensions of the submenu');
        MenuItem('Kill', 7, 1, Ord(MsubRemove), ' Delete all descendents of the current item');
        PopSubLevel;
      MenuItem(' Item ', 41, 2, Ord(Mitem), ' Add, move, edit or remove a menu item');
      SubMenu(43, 8, MakeHR, Vertical, Frame2, Colors, '');
        MenuItem('Add       ^I', 1, 1, Ord(MitemAdd), ' Add a new item to the current submenu');
        MenuItem('Edit      ^E', 2, 1, Ord(MitemEdit), ' Edit the name of the current item');
        MenuItem('Move      ^A', 3, 1, Ord(MitemMove), ' Adjust the position of the current item');
        MenuItem('Order     ^O', 4, 1, Ord(MitemOrder), ' Adjust the order of the current item');
        MenuItem('Key         ', 5, 1, Ord(MitemKey), ' Modify numeric key value returned when item is selected');
        MenuItem('Help', 6, 1, Ord(MitemHelp), ' Edit the help line for the current item');
        MenuItem('Select char', 7, 1, Ord(MitemSelectChar), ' Change the selection character for the current item');
        MenuItem('Remove', 8, 1, Ord(MitemRemove), ' Remove the current item');
        PopSubLevel;
      MenuItem(' File ', 61, 2, Ord(Mfile), ' Read or write menus to disk, or quit the menu maker');
      SubMenu(53, 8, MakeHR, Vertical, Frame2, Colors, '');
        MenuItem('Read lib        ^R', 1, 1, Ord(MfileRead), ' Read a menu from a menu library');
        MenuItem('Write lib       ^W', 2, 1, Ord(MfileWrite), ' Write the current menu to a menu library');
        MenuItem('Generate source ^G', 3, 1, Ord(MfileGenerate), ' Generate Pascal source code to build the current menu');
        MenuItem('Pack lib        ^P', 4, 1, Ord(MfilePack), ' Pack an existing menu library, removing obsolete menus');
        MenuItem('Quit            ^Q', 5, 1, Ord(MfileQuit), ' Quit the Turbo Professional menu maker');
        PopSubLevel;
      PopSubLevel;
    {.F+}
    ResetMenu(Make);

    {Set menu mode as desired}
    ShadowMode := MakeShadowMode;
    SetMenuMode(Make, UseExplode, UseShadow, False);
    SetMenuDelay(Make, ExpDelay);

    {Disable certain Make items when no User items are available}
    DisableMakeNoItems;
    DisableMakeWhenSaved;

    ShadowMode := DefShadowMode;
  end;

  procedure Initialize;
    {-Initialize globals and screen}
  var
    Junk : Boolean;
    UseMono : Boolean;
    C : MenuColorType;
  begin
    ClrScr;
    GetCursorState(InitXY, InitSL);
    if HideCursor then
      HiddenCursor;

    User := nil;
    UserStack := nil;
    MakeStack := nil;
    Saved := False;
    LastRead := '';
    LastWrote := '';
    LastId := '';
    TrimBlanks := False;
    MakeHR := ScreenHeight;

    SetFactoryDefaults;

    {Map make menu colors}
    MapColors := False;
    case CurrentMode of
      2, 7 : UseMono := (WhichHerc <> HercInColor);
    else
      UseMono := False;
    end;
    if UseMono then begin
      Colors := MonoColors;
      PromptAttr := MonoPromptAttr;
      StringAttr := MonoStringAttr;
    end else begin
      Colors := ColorColors;
      PromptAttr := ColorPromptAttr;
      StringAttr := ColorStringAttr;
    end;

    {$IFDEF UseMouse}
    if MouseInstalled then begin
      if UseMono then begin
        MouseScreenMask := $0000;
        MouseCursorMask := $0F04;
      end else begin
        MouseScreenMask := $00FF;
        MouseCursorMask := MouseAttr shl 8;
      end;

      {Map both buttons pressed to a user exit}
      if AddMenuCommand(MKSUser0, 1, $ED00, 0) then
        ;
    end;
    {$ENDIF}

    {Get line buffer for prompts}
    GetMem(Covers, 2*ScreenWidth);

    {Disable some WordStar keys in TPMENU command set - interferes with hot keys}
    Junk := AddMenuCommand(MKSNone, 1, $05, 0); {^E}
    Junk := AddMenuCommand(MKSNone, 1, $17, 0); {^W}
    Junk := AddMenuCommand(MKSNone, 1, $13, 0); {^S}
    Junk := AddMenuCommand(MKSNone, 1, $04, 0); {^D}
    Junk := AddMenuCommand(MKSNone, 2, $11, $12); {^QR}
    Junk := AddMenuCommand(MKSNone, 2, $11, $03); {^QC}
  end;

  function ReadChar(var Scan : Char) : Char;
    {-Read a character and scan code if any}
  var
    KW : Word;
  begin
    {$IFDEF UseMouse}
    if MouseInstalled then
      KW := ReadKeyMouseXY
    else
      KW := ReadKeyWord;
    {$ELSE}
    KW := ReadKeyWord;
    {$ENDIF}
    ReadChar := Char(lo(KW));
    if lo(KW) = 0 then
      Scan := Char(hi(KW))
    else
      Scan := #0;
  end;

  function Confirm(Msg : string) : Boolean;
    {-Write a message to confirm an operation}
  var
    XY : Word;
    ScanLines : Word;
  begin
    {Store contents of screen underneath where we'll write}
    if SaveWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers) then
      {Won't fail} ;
    GetCursorState(XY, ScanLines);
    ClearWindow(1, MakeHR, ScreenWidth, MakeHR, PromptAttr);
    Confirm := YesOrNo(Msg, MakeHR, 1, PromptAttr, 'N');
    RestoreWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers);
    RestoreCursorState(XY, ScanLines);
  end;

  function NoSave : Boolean;
    {-Check for unsaved menu, prompt whether to save}
  begin
    if Saved then
      NoSave := True
    else
      NoSave := Confirm('Current menu not written to library. Abandon changes?');
  end;

  procedure PressEsc(Msg : string);
    {-Display a message and wait for <Esc> to continue}
  var
    XY : Word;
    ScanLines : Word;
    Ch : Char;
  begin
    {Store contents of screen underneath where we'll write}
    if SaveWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers) then
      {Won't fail} ;
    GetCursorState(XY, ScanLines);
    ClearWindow(1, MakeHR, ScreenWidth, MakeHR, PromptAttr);
    ReadCharacter(Msg+' Press <Esc> to continue', MakeHR, 1,
      PromptAttr, [#27], Ch);
    RestoreWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers);
    RestoreCursorState(XY, ScanLines);
  end;

  function PromptString(Prompt, Default : string; var Escaped : Boolean) : string;
    {-Prompt for and return a string}
  var
    S : string;
  begin
    {Store contents of screen underneath where we'll write}
    if SaveWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers) then
      {Won't fail} ;
    S := Default;
    EditSize := ScreenWidth-1-Length(Prompt);
    ReadString(Prompt, MakeHR, 1, 125, PromptAttr, StringAttr,
      StringAttr, Escaped, S);
    PromptString := S;
    RestoreWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers);
  end;

  function PromptLongInt(Prompt : string; Min, Max, Default : LongInt) : LongInt;
    {-Prompt for and return longint}
  var
    I : LongInt;
    Escaped : Boolean;
  begin
    {Store contents of screen underneath where we'll write}
    if SaveWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers) then
      {Won't fail} ;
    I := Default;
    EditSize := 0;
    ReadLongInt(Prompt, MakeHR, 1, ScreenWidth-Length(Prompt), PromptAttr, StringAttr,
      Min, Max, Escaped, I);
    if Escaped then
      PromptLongInt := Default
    else
      PromptLongInt := I;
    RestoreWindow(1, MakeHR, ScreenWidth, MakeHR, False, Covers);
  end;

  procedure Help(Msg : string);
    {-Write a help message on the prompt row}
  begin
    FastWrite(Msg, MakeHR, 1, PromptAttr);
  end;

  procedure ClearHelp;
    {-Clear a temporary help line}
  begin
    Help(Pad('', ScreenWidth));
  end;

  function HaveActive : Boolean;
    {-Assure active menu exists. Otherwise report error and return false}
  begin
    HaveActive := False;
    if User = nil then
      PressEsc('Create new menu first.')
    else if User^.Active = nil then
      PressEsc('Create a submenu first.')
    else
      HaveActive := True;
  end;

  function HaveItem : Boolean;
    {-Return true if active submenu has a current item}
  begin
    with User^.Active^ do
      if Items.Current = nil then begin
        PressEsc('No items on current submenu.');
        HaveItem := False;
      end else
        HaveItem := True;
  end;

  function HaveActiveNoError : Boolean;
    {-Assure active menu exists}
  begin
    if User = nil then
      HaveActiveNoError := False
    else if User^.Active = nil then
      HaveActiveNoError := False
    else
      HaveActiveNoError := True;
  end;

  function FirstNonBlank(S : string) : Byte;
    {-Return position of first non-blank character in s, 1 if none}
  var
    I : Word;
  begin
    for I := 1 to Length(S) do
      if S[I] > ' ' then begin
        FirstNonBlank := I;
        Exit;
      end;
    FirstNonBlank := 1;
  end;

  procedure Undraw;
    {-Undo the screen, storing the state of both menus}
  begin
    EraseMenuOntoStack(Make, MakeStack);
    if User <> nil then begin
      TmpDelay := User^.Active^.Draw.ExploDelay;
      SetMenuDelay(User, 0);
      EraseMenuOntoStack(User, UserStack);
    end else
      UserStack := nil;
  end;

  procedure Redraw;
    {-Redraw the screen, restoring the state of both menus}
  begin
    if UserStack <> nil then begin
      DrawMenuFromStack(User, UserStack);
      SetMenuDelay(User, TmpDelay);
    end;
    DrawMenuFromStack(Make, MakeStack);
  end;

  procedure ClearUserStack;
    {-Dispose of space for user stack}
  var
    P : MenuStackP;
  begin
    while UserStack <> nil do begin
      P := UserStack^.Next;
      FreeMem(UserStack, SizeOf(MenuStackRec));
      UserStack := P;
    end;
  end;

  procedure PushRootMenuOntoStack;
    {-Put the root menu onto the user display stack}
  begin
    {Put root menu onto temporary stack}
    GetMem(UserStack, SizeOf(MenuStackRec));
    UserStack^.Top := User^.Root;
    UserStack^.Next := nil;
  end;

  procedure AdjustXY(SubMnu : SubMenuP; SetDisp : Boolean);
    {-Adjust the dimensions and items of a menu after changes}
  var
    Item : ItemP;
    MinX : Byte;
    MinY : Byte;
    MinH : Byte;
    Disp : Byte;
  begin
    with SubMnu^ do begin
      MinX := XL-1+4*FrameDelta[LotusStyle];

      if Orient = Vertical then begin
        {Space for two horizontal bars}
        MinY := YL-1+2*FrameDelta[LotusStyle];
        Disp := 1;
      end else begin
        {Space for two horizontal bars plus one row of items}
        MinY := YL+2*FrameDelta[LotusStyle];
        Disp := 1+FrameDelta[LotusStyle];
      end;

      Item := Items.First;
      while Item <> nil do begin
        {Handle automatic window sizing}
        with Item^ do
          case Orient of
            Horizontal :
              begin
                if SetDisp then
                  DisplayPos := Disp;
                MinH := 3*FrameDelta[LotusStyle]+XL+DisplayPos-2+Byte(Name^);
                Inc(Disp, Byte(Name^)+2);
              end;
            Vertical :
              begin
                {Add vertical space for the new item}
                if SetDisp then
                  DisplayPos := Disp;
                Inc(Disp);
                MinH := YL-1+2*FrameDelta[LotusStyle]+DisplayPos;
                if MinY < MinH then
                  MinY := MinH;
                MinH := XL-1+Byte(Name^)+4*FrameDelta[LotusStyle];
              end;
          end;
        if MinX < MinH then
          MinX := MinH;

        Item := Item^.Next;
      end;

      {Adjust menu dimensions only if they're too small}
      if XH < MinX then
        XH := MinX;
      if YH < MinY then
        YH := MinY;
      with Draw do begin
        XH1 := XH;
        YH1 := YH;
      end;
    end;
  end;

  procedure SetActiveFrame(F : FrameArray; SubMnu : SubMenuP);
    {-Change the frame type of the specified submenu}
  var
    SaveLotus : Boolean;
  begin
    Undraw;
    with SubMnu^ do begin
      SaveLotus := LotusStyle;
      LotusStyle := (F = LotusFrame);
      Frame := F;
      if SaveLotus <> LotusStyle then begin
        {Reset window dimensions, AdjustXY will recompute}
        XH := XL;
        YH := YL;
      end;
      AdjustXY(SubMnu, (SaveLotus <> LotusStyle)); {!!.07}
    end;
    Redraw;
  end;

  procedure SetFrameSubMenu(SubMnu : SubMenuP);
    {-Change the frame type of specified menu and its children}
  var
    P : ItemP;
    SaveLotus : Boolean;
  begin
    with SubMnu^ do begin
      SaveLotus := LotusStyle;
      LotusStyle := (UserFrame = LotusFrame);
      Frame := UserFrame;
      if SaveLotus <> LotusStyle then begin
        {Reset window dimensions, AdjustXY will recompute}
        XH := XL;
        YH := YL;
      end;
      AdjustXY(SubMnu, (SaveLotus <> LotusStyle)); {!!.07}
      {Scan the list of items}
      P := Items.First;
      while P <> nil do begin
        with P^ do
          if Sub <> nil then
            {Recolor this item's submenu}
            SetFrameSubMenu(Sub);
        {Get next item}
        P := P^.Next;
      end;
    end;
  end;

  procedure UpdateFrames;
    {-Update the frames of active menu and children}
  var
    SubMnu : SubMenuP;
  begin
    SubMnu := User^.Active;
    Undraw;
    {Update frames of current menu and all children}
    SetFrameSubMenu(SubMnu);
    MarkNotSaved;
    Redraw;
  end;

  procedure SetHelpRowSubMenu(SubMnu : SubMenuP; HRow : Integer);
    {-Set the help row for submnu and its children}
  var
    P : ItemP;
  begin
    with SubMnu^ do begin
      YHelp := HRow;
      {Scan the list of items}
      P := Items.First;
      while P <> nil do begin
        with P^ do
          if Sub <> nil then
            {Set help row for this item's submenu}
            SetHelpRowSubMenu(Sub, HRow);
        {Get next item}
        P := P^.Next;
      end;
    end;
  end;

  procedure ShiftXY(SubMnu : SubMenuP; dX, dY : Integer; Recursive : Boolean);
    {-Change coordinates of a menu and its children by DX,DY}
  var
    P : ItemP;
    TXL, TXH, TYL, TYH : Byte;
  begin
    with SubMnu^ do begin

      {Change the menu borders, if they remain on-screen}
      TXL := XL+dX;
      TXH := XH+dX;
      TYL := YL+dY;
      TYH := YH+dY;
      if (((TXL >= 1) and (dX < 0)) or
        ((TXH <= ScreenWidth-2*ShadowDelta[Draw.Shadowed]) and (dX > 0))) then begin
        XL := TXL;
        XH := TXH;
        Draw.XL1 := XL;
        Draw.XH1 := XH;
      end;
      if (((TYL >= 1) and (dY <= 0)) or
        ((TYH <= ScreenHeight-ShadowDelta[Draw.Shadowed]) and (dY >= 0))) then begin
        YL := TYL;
        YH := TYH;
        Draw.YL1 := YL;
        Draw.YH1 := YH;
      end;

      if Recursive then begin
        {Scan its items for submenus}
        P := Items.First;
        while P <> nil do begin
          if P^.Sub <> nil then
            ShiftXY(P^.Sub, dX, dY, True);
          P := P^.Next;
        end;
      end;
    end;
  end;

  procedure GetMinXY(SubMnu : SubMenuP; var MinX, MinY : Byte);
    {-Compute minimum values for XH and YH}
  var
    Item : ItemP;
    MinH : Byte;
  begin
    with SubMnu^ do begin
      MinX := XL-1+4*FrameDelta[LotusStyle];

      if Orient = Vertical then
        {Space for two horizontal bars}
        MinY := YL-1+2*FrameDelta[LotusStyle]
      else
        {Space for two horizontal bars plus one row of items}
        MinY := YL+2*FrameDelta[LotusStyle];

      Item := Items.First;
      while Item <> nil do begin
        {Handle automatic window sizing}
        with Item^ do
          case Orient of
            Horizontal :
              MinH := 3*FrameDelta[LotusStyle]+XL+DisplayPos-2+Byte(Name^);
            Vertical :
              begin
                {Add vertical space for the new item}
                MinH := YL-1+2*FrameDelta[LotusStyle]+DisplayPos;
                if MinY < MinH then
                  MinY := MinH;
                MinH := XL-1+Byte(Name^)+4*FrameDelta[LotusStyle];
              end;
          end;
        if MinX < MinH then
          MinX := MinH;

        Item := Item^.Next;
      end;
    end;
  end;

  procedure GetDirection(Scan : Char; var DelX, DelY : Integer);
    {-Return direction indicators for Scan char}
  begin
    DelX := 0;
    DelY := 0;
    case Scan of
      #71 : begin            {home}
              DelX := -1;
              DelY := -1;
            end;
      #72 : DelY := -1;      {up}
      #73 : begin            {pgup}
              DelX := +1;
              DelY := -1;
            end;
      #75 : DelX := -1;      {left}
      #77 : DelX := +1;      {right}
      #79 : begin            {end}
              DelX := -1;
              DelY := +1;
            end;
      #80 : DelY := +1;      {down}
      #81 : begin            {pgdn}
              DelX := +1;
              DelY := +1;
            end;
    end;
  end;

  procedure ResizeMenu(SubMnu : SubMenuP);
    {-Interactively resize a submenu}
  var
    MinX : Byte;
    MinY : Byte;
    Ch : Char;
    Scan : Char;
    DelX : Integer;
    DelY : Integer;
    EDelay : Word;
  begin
    {Erase the Make menu}
    EraseMenuOntoStack(Make, MakeStack);

    {Write some help}
    Help('Use cursor keys to resize. Press <Enter> or <Space> to accept');

    {Compute the smallest allowable window size}
    GetMinXY(SubMnu, MinX, MinY);

    with SubMnu^ do begin
      {Temporarily disable explosions}
      EDelay := Draw.ExploDelay;
      Draw.ExploDelay := 0;
      {Update cursor}
      if Items.Current <> nil then
        DrawItem(SubMnu, Items.Current, nil);
      {$IFDEF UseMouse}
      SaveMousePos;
      {$ENDIF}

      repeat
        {$IFDEF UseMouse}
        if MouseInstalled then
          MouseGotoXY(XH, YH);
        {$ENDIF}
        Ch := ReadChar(Scan);
        GetDirection(Scan, DelX, DelY);
        if (DelX <> 0) or (DelY <> 0) then begin
          if XH+DelX < MinX then
            DelX := 0;
          if XH+DelX > ScreenWidth-2*ShadowDelta[Draw.Shadowed] then
            DelX := 0;
          if YH+DelY < MinY then
            DelY := 0;
          if YH+DelY > ScreenHeight-ShadowDelta[Draw.Shadowed] then
            DelY := 0;
          if (DelX <> 0) or (DelY <> 0) then begin
            EraseSubMenu(SubMnu);
            Inc(XH, DelX);
            Inc(YH, DelY);
            Draw.XH1 := XH;
            Draw.YH1 := YH;
            DrawSubMenu(SubMnu, nil);
          end;
        end;
      until (Ch in AcceptSet);

      {$IFDEF UseMouse}
      RestoreMousePos;
      {$ENDIF}
      Draw.ExploDelay := EDelay;
    end;

    ClearHelp;
    DrawMenuFromStack(Make, MakeStack);
  end;


  procedure DragMenu(SubMnu : SubMenuP; Recursive : Boolean);
    {-Interactively move a submenu}
  var
    Ch : Char;
    Scan : Char;
    DelX : Integer;
    DelY : Integer;
    EDelay : Word;
  begin
    {Erase the Make menu}
    EraseMenuOntoStack(Make, MakeStack);
    {Write some help}
    Help(CursorKeyHelp);

    with SubMnu^ do begin
      {Temporarily disable explosions}
      EDelay := Draw.ExploDelay;
      Draw.ExploDelay := 0;
      {Update cursor}
      if Items.Current <> nil then
        DrawItem(SubMnu, Items.Current, nil);
      {$IFDEF UseMouse}
      SaveMousePos;
      {$ENDIF}

      repeat
        {$IFDEF UseMouse}
        if MouseInstalled then
          MouseGotoXY(XL, YL);
        {$ENDIF}
        Ch := ReadChar(Scan);
        GetDirection(Scan, DelX, DelY);
        if (DelX <> 0) or (DelY <> 0) then begin
          if XL+DelX < 1 then
            DelX := 0;
          if XH+DelX > ScreenWidth-2*ShadowDelta[Draw.Shadowed] then
            DelX := 0;
          if YL+DelY < 1 then
            DelY := 0;
          if YH+DelY > ScreenHeight-ShadowDelta[Draw.Shadowed] then
            DelY := 0;
          if (DelX <> 0) or (DelY <> 0) then begin
            EraseSubMenu(SubMnu);
            ShiftXY(SubMnu, DelX, DelY, Recursive);
            DrawSubMenu(SubMnu, nil);
          end;
        end;
      until (Ch in AcceptSet);

      {$IFDEF UseMouse}
      RestoreMousePos;
      {$ENDIF}
      Draw.ExploDelay := EDelay;
    end;

    ClearHelp;
    DrawMenuFromStack(Make, MakeStack);
  end;
