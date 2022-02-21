Program test;

Uses
  tpcrt, async4u;

(*
procedure Async_Close;
{ reset the interrupt system when UART interrupts no longer needed }

function Async_Open(ComPort       : Integer;
                    BaudRate      : Integer;
                    Parity        : Char;
                    WordSize      : Integer;
                    StopBits      : Integer) : Boolean;
{ open a communications port }

function Async_Buffer_Check(var C : Char) : Boolean;
{ see if a character has been received; return it if yes }

FUNCTION Async_Send(C : Char):Boolean;
{ transmit a character }

procedure Async_Send_String(S : LStr);
{ transmit a string }

*)
var
  Timeout : integer;
  Error : integer;

Function GetChar( var ch : char ) : integer;
var
  modt : integer;
begin
  modt := 0;
  Timeout := 10000;
  repeat
    if Async_Buffer_Check( Ch ) then
      modt := 1
    else
      begin
        Ch := ' ';
        modt := -1;
        Dec( Timeout );
      end;
  until ( ( modt = 1 ) or ( Timeout = 0 ) );
  GetChar := modt;
end;


Procedure WriteRS232( txt : string );
var
  buf, Ch : char;

begin
  Async_Send_String( 'MS' + #13 + 'MA58' + #13 + 'RM' + #13 );
  repeat
    Error := GetChar( ch );
    if Error = -1 then
      begin
{        Writeln;
        Write('Timeout ');
}      end;
    if Error = 1 then
      begin
        write( ch );
        delay(1000);
      end;
    if keypressed then
      BEGIN
        buf := readkey;
        if buf <> #27 then
          Async_Send_String( 'RM' + #13 );
      end;
  until buf = #27;
end;


begin
  writeln('Start');
  if Async_Open( 1, 4800, 'N', 8, 2 ) then
    begin
      WriteRS232( 'MS' );
    end;
  Async_Close;
  writeln('Slut');
end.