Unit Comutil;

{Unit compiled with Turbo Pascal 7.0}

Interface
uses Crt;

const  TIMEOUTTIME = 50;

const _Baud19200 = 8;              {see ASYNC PLUS User Reference page 108...}
      _Baud9600  = 7;
      _Baud4800  = 6;
      _Baud2400  = 5;
      _Baud1200  = 4;
      _Baud600   = 3;
      _Baud300   = 2;
      _Baud150   = 1;
      _Baud110   = 0;

      _None      = 0;
      _Odd       = 1;
      _Even      = 2;

      _Databit5  = 0;
      _Databit6  = 1;
      _Databit7  = 2;
      _Databit8  = 3;

      _Stopbit1  = 0;
      _Stopbit2  = 1;

      _CTSNotRequired = 0;
      _CTSRequired    = 1;

      _Com1        = 1;
      _Com2        = 2;
      _Com3        = 3;
      _Com4        = 4;

var   Com1Open,
      Com2Open   : BOOLEAN;

{Timeout variables}
var CurrentTime : WORD ABSOLUTE $0040:$006C; {System Clock}

Function InitCOMPort(PortNo,Baud,Databit,Stopbit,Parity,CTS : WORD) : WORD;

Function OpenCOMPort(PortNo : BYTE; InQ,OutQ : WORD) : WORD;

Function CloseCOMPort(PortNo : BYTE) : WORD;

Function WritelnCOMPort(PortNo : WORD; S : STRING) : WORD;

Function WriteCOMPort(PortNo : WORD; S : STRING) : WORD;

Function ReadlnCOMPort(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;

Function ReadCOMPort(PortNo, NoOfBytes : WORD; var InputStr : STRING; Timeout : WORD) : WORD;

Function ClearInputQue(PortNo : WORD) : WORD;

{************}

Function jk_InitCOMPort(PortNo,Baud,Databit,Stopbit,Parity,CTS : WORD) : WORD;

Function jk_OpenCOMPort(PortNo : BYTE; InQ,OutQ : WORD) : WORD;

Function jk_CloseCOMPort(PortNo : BYTE) : WORD;

Function jk_WritelnCOMPort(PortNo : WORD; S : STRING) : WORD;

Function jk_WriteCOMPort(PortNo : WORD; S : STRING) : WORD;

Function jk_ReadlnCOMPort(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
Function jk_ReadlnMX(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;

Function jk_ReadCOMPort(PortNo, NoOfBytes : WORD; var InputStr : STRING; Timeout : WORD) : WORD;

Function jk_ClearInputQue(PortNo : WORD) : WORD;

{************}


Implementation
{Uses Unit_A0, Unit_A1;}
Uses
  async4u;

const LF = #10;
      CR = #13;

type  STR_1 = STRING[1];

Function InitCOMPort(PortNo,Baud,Databit,Stopbit,Parity,CTS : WORD) : WORD;
var ErrCode, e : WORD;
begin
  ErrCode := jk_InitCOMPort(PortNo, Baud, Databit, Stopbit, Parity, CTS);
  InitCOMPort := ErrCode;
end;

Function OpenCOMPort(PortNo : BYTE; InQ,OutQ : WORD) : WORD;
var ErrCode   : WORD;
    IOBufPtr  : POINTER;
begin
  ErrCode := jk_OpenCOMPort(PortNo, InQ, OutQ);
  OpenCOMPort := ErrCode;
 end;

Function CloseCOMPort(PortNo : BYTE) : WORD;
var  COMCloseErr : WORD;
begin
  COMCloseErr := jk_CloseCOMPort(PortNo);
  CloseCOMPort := COMCloseErr;
 end;

Function WritelnCOMPort(PortNo:WORD; S : STRING) : WORD;
begin
  WritelnCOMPort := 1;
  Delay(20);
 end;

Function WriteCOMPort(PortNo:WORD; S : STRING) : WORD;
var WrtCode    : WORD;
begin
  WrtCode := jk_WriteCOMPort(PortNo, S);
  Delay(20);
  WriteCOMPort := WrtCode;
end;

Function ReadlnCOMPort(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
var Err : WORD;
begin
  {Delay(100);}
  Err := jk_ReadlnCOMPort(PortNo, InputStr, Timeout);
  ReadlnCOMPort := Err;
end;

Function ReadCOMPort(PortNo, NoOfBytes : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
var Err : WORD;
begin
  {Delay(100);}
  Err := jk_ReadCOMPort(PortNo, NoOfBytes, InputStr, Timeout);
  ReadCOMPort := Err;
end;


Function ClearInputQue(PortNo : WORD) : WORD;
var Err :WORD;
begin
  Err := jk_ClearInputQue(PortNo);
  ClearInputQue := Err;
end;

{***********}

Function jk_InitCOMPort(PortNo,Baud,Databit,Stopbit,Parity,CTS : WORD) : WORD;
var ErrCode : WORD;
var e       : Boolean;
  begin
    e := Async_Open(PortNo, Baud, 'n', 8, 2);
    if PortNo = _COM1 then
      if e = true then
         COM1Open := TRUE
      ELSE
         COM1Open := FALSE;

    if PortNo = _COM2 then
      if  e = true then
        COM2Open := TRUE
      ELSE
        COM2Open := FALSE;

    if e = true then
      jk_InitCOMPort := 0
    else
      jk_InitCOMPort := 1;
  end;


Function jk_OpenCOMPort(PortNo : BYTE; InQ,OutQ : WORD) : WORD;
begin
  jk_OpenCOMPort := 0;
end;

Function jk_CloseCOMPort(PortNo : BYTE) : WORD;
var  COMCloseErr : WORD;
var e            : Boolean;
begin
  Async_Close;
  COMCloseErr := 1;
  if PortNo = _COM1 then
    if COMCloseErr = 1 then
      COM1Open := FALSE;

  if PortNo = _COM2 then
    if COMCloseErr = 1 then
      COM2Open := FALSE;

  jk_CloseCOMPort := 1;
end;

Function jk_WritelnCOMPort(PortNo : WORD; S : STRING) : WORD;
begin
  jk_WritelnCOMPort := 1;
end;

Function jk_WriteCOMPort(PortNo : WORD; S : STRING) : WORD;
begin
  Async_Send_String(S);
  jk_WriteCOMPort := 1;
end;

{Timeout functions}

Procedure StartTimer(var start: WORD);
begin
  start := CurrentTime;
end;

Function TimeElapsed(StartTime: WORD) : WORD;
var HoldTime : WORD;
begin
  HoldTime := CurrentTime;
  TimeElapsed := HoldTime - StartTime;
end;

{jk_ReadCOMPort() modified with timeout based on system timer: jk}
{variable Timeout in function input is not used}
Function jk_ReadlnCOMPort(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
var
 StartTime  : WORD;
 z          : WORD;
 RetChar    : CHAR;
 s          : STR_1;
 RdCode     : Boolean;
 TimOut     : Boolean;

begin
  InputStr := '';
  StartTimer(StartTime);
  TimOut := false;
  repeat
    RdCode := Async_Buffer_Check(RetChar);
      if (RdCode = true) then
        begin
          s[1] := RetChar;
          StartTimer(StartTime);
          TimOut := false;
          if (RetChar <> CR) then
            InputStr := InputStr + s[1];
        end
      else
        begin
          z := TimeElapsed(StartTime);
          if z > TIMEOUTTIME then
          TimOut := true;
        end
  until (RetChar = CR) OR (TimOut = true);
  if TimOut = true then
    jk_ReadlnCOMPort := 1
  else
    jk_ReadlnCOMPort := 0;
end;


{ Function jk_ReadlnMX() reads results in MX (unmodified sensor output).
  In contrast to other modes, this mode outputs only CR and not LF.

  In order to avoid synchronization problems (this function might be 
  entered in the middle of transmission), we first wait until a CR
  arrives and THEN collect sent characters

}
Function jk_ReadlnMX(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
var
 StartTime  : WORD;
 z          : WORD;
 RetChar    : CHAR;
 s          : STR_1;
 RdCode     : Boolean;
 TimOut     : Boolean;
begin

  StartTimer(StartTime);
  InputStr := '';
  TimOut   := false;

{ first wait until CR arrives }
  repeat
    RdCode := Async_Buffer_Check(RetChar);
    if (RdCode = true) then
      begin
        StartTimer(StartTime);
        TimOut := false;
      end
    else
      begin
        z := TimeElapsed(StartTime);
        if z > TIMEOUTTIME then
        TimOut := true;
      end
  until (RetChar = CR) OR (TimOut = true);

{ Then collect all chars }
  RetChar := 'K';
  repeat
    RdCode := Async_Buffer_Check(RetChar);
    if (RdCode = true) then
      begin
        s[1] := RetChar;
        StartTimer(StartTime);
        TimOut := false;
        if (RetChar <> CR) then
          InputStr := InputStr + s[1];
      end
    else
      begin
        z := TimeElapsed(StartTime);
        if z > TIMEOUTTIME then
          TimOut := true;
      end
  until (RetChar = CR) OR (TimOut = true);

  if TimOut = true then
    jk_ReadlnMX := 1
  else
    jk_ReadlnMX := 0;
end;


Function jk_ReadCOMPort(PortNo, NoOfBytes : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
begin
  jk_ReadCOMPort := 1;
end;

Function jk_ClearInputQue(PortNo : WORD) : WORD;
begin
  Async_Clear_cueue;
  jk_ClearInputQue := 1;
end;

begin
 Com1Open := FALSE;
 Com2Open := FALSE;
end.
