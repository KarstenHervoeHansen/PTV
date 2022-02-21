Unit Comutil;

{Unit compiled with Turbo Pascal 7.0}

Interface

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



Function InitCOMPort(PortNo,Baud,Databit,Stopbit,Parity,CTS : WORD) : WORD;

Function OpenCOMPort(PortNo : BYTE; InQ,OutQ : WORD) : WORD;

Function CloseCOMPort(PortNo : BYTE) : WORD;

Function WritelnCOMPort(PortNo : WORD; S : STRING) : WORD;

Function WriteCOMPort(PortNo : WORD; S : STRING) : WORD;

Function ReadlnCOMPort(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;

Function ReadCOMPort(PortNo, NoOfBytes : WORD; var InputStr : STRING; Timeout : WORD) : WORD;

Function ClearInputQue(PortNo : WORD) : WORD;




Implementation
Uses Unit_A0, Unit_A1;

const LF = #10;
      CR = #13;

type  STR_1 = STRING[1];

Function InitCOMPort(PortNo,Baud,Databit,Stopbit,Parity,CTS : WORD) : WORD;
var ErrCode,
    e       : WORD;
begin;
e := 0;
ErrCode := __SetOpA1(PortNo,1,Baud);
 if ErrCode > 0 then
  e := e + 1;
ErrCode := __SetOpA1(PortNo,2,Parity);
 if ErrCode > 0 then
  e := e + 8;
ErrCode := __SetOpA1(PortNo,3,Databit);
 if ErrCode > 0 then
  e := e + 2;
ErrCode := __SetOpA1(PortNo,4,Stopbit);
 if ErrCode > 0 then
  e := e + 4;
ErrCode := __SetOpA1(PortNo,9,CTS);
 if ErrCode > 0 then
  e := e + 16;

InitCOMPort := e
end;


Function OpenCOMPort(PortNo : BYTE; InQ,OutQ : WORD) : WORD;
var ErrCode   : WORD;
    IOBufPtr  : POINTER;
begin
 GetMem(IOBufPtr,InQ + OutQ);
 ErrCode := __OpenA1(PortNo, InQ, OutQ, 0, 0, IOBufPtr);
 OpenCOMPort := ErrCode;
 if PortNo = _COM1 then
  if ErrCode = 0 then
    COM1Open := TRUE
   ELSE
    COM1Open := FALSE;

 if PortNo = _COM2 then
  if  ErrCode = 0 then
    COM2Open := TRUE
   ELSE
    COM2Open := FALSE;

end;


Function CloseCOMPort(PortNo : BYTE) : WORD;
var  COMCloseErr : WORD;
begin
 COMCloseErr := __CloseA1(PortNo);

 if PortNo = _COM1 then
  if COMCloseErr = _OK then
   COM1Open := FALSE;

 if PortNo = _COM2 then
  if COMCloseErr = _OK then
   COM2Open := FALSE;

  CloseCOMPort := COMCloseErr;
end;



Function WritelnCOMPort(PortNo:WORD; S : STRING) : WORD;
var  QSize,
     FlushCode,
     Antalsendt,
     SizeCode,
     WrtCode    : WORD;
begin
S := S + LF + CR;
FlushCode := __OFlshA1(PortNo);
WrtCode := __WrtStA1(PortNo,Length(S),@S[1],AntalSendt);
 repeat
  SizeCode := __OQSizeA1(PortNo,QSize);
 until QSize = 0;
WritelnCOMPort :=  WrtCode + (SizeCode SHL 4);
end;

Function WriteCOMPort(PortNo:WORD; S : STRING) : WORD;
var  QSize,
     FlushCode,
     Antalsendt,
     SizeCode,
     WrtCode    : WORD;
begin
FlushCode := __OFlshA1(PortNo);
WrtCode := __WrtStA1(PortNo,Length(S),@S[1],AntalSendt);
 repeat
  SizeCode := __OQSizeA1(PortNo,QSize);
 until QSize = 0;
WriteCOMPort :=  WrtCode + (SizeCode SHL 4);
end;


Function ReadlnCOMPort(PortNo : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
var
 WaitCode,
 RdCode,
 PS,
 InQSize,
 PortStatus : WORD;
 RetChar    : CHAR;
 s          : STR_1;
begin
 InputStr := '';
 PortStatus := 0;
 repeat
 WaitCode := __WaitChA1(PortNo,Timeout * 18,Nil);
 RdCode := __RdChA1(PortNo,RetChar,InQSize,PS);
  if (RdCode = _OK) and (RetChar <> CR) then
   begin
    if PS > 0 then PortStatus := (PS and $3F);  { XON/XOFF  see p.70 in ASYNC manual }
    s[1] := RetChar;
    InputStr := InputStr + s[1];
   end;
 until (RetChar = CR) OR (WaitCode = _TIME_OUT);
ReadlnCOMPort := WaitCode + (RdCode SHL 4) + (PortStatus SHL 8);
end;



Function ReadCOMPort(PortNo, NoOfBytes : WORD; var InputStr : STRING; Timeout : WORD) : WORD;
var
 WaitCode,
 RdCode,
 PS,
 InQSize,
 No,
 PortStatus : WORD;
 RetChar    : CHAR;
 s          : STR_1;
begin
 InputStr := '';
 PortStatus := 0;
 No := 0;
 repeat
 WaitCode := __WaitChA1(PortNo,Timeout * 18,Nil);
 RdCode := __RdChA1(PortNo,RetChar,InQSize,PS);
  if (RdCode = _OK) then
   begin
    if PS > 0 then PortStatus := (PS and $3F);  { XON/XOFF  see p.70 in ASYNC manual };
    s[1] := RetChar;
    InputStr := InputStr + s[1];
    Inc(No);
   end;
 until (No = NoOfBytes) OR (WaitCode = _TIME_OUT);
ReadCOMPort := WaitCode + (RdCode SHL 4) + (PortStatus SHL 8);
end;








Function ClearInputQue(PortNo : WORD) : WORD;
var
 FlushCode : WORD;
begin
 FlushCode := __iflshA1(PortNo);
 ClearInputQue := FlushCode;
end;


begin
 Com1Open := FALSE;
 Com2Open := FALSE;
end.
