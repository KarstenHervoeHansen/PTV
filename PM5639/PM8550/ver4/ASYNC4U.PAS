{----------------------------------------------------------------------}
{                          ASYNC4U.PAS                                 }
{                                                                      }
{  This is a faithful translation of the famous ASYNC.INC by Michael   }
{  Quinlan into a Turbo 4.0 unit.  No extra frills, no modification of }
{  types, nothing fancy.  But with this code you should be able to     }
{  delete your $I ASYNC.INC directive, add a USES ASYNC4U statement,   }
{  and recompile your existing program.  If you want to add support    }
{  for more ports, other computers, or change to use the new data      }
{  types, all good ideas, go right ahead. With this you don't have to. }
{                                                                      }
{                                      Scott Gurvey, November 29 1987  }
{----------------------------------------------------------------------}
{                                                                      }
{                          ASYNC.INC                                   }
{                                                                      }
{  Async Communication Routines                                        }
{  by Michael Quinlan                                                  }
{  with a bug fixed by Scott Herr                                      }
{  made PCjr-compatible by W. M. Miller                                }
{  Highly dependant on the IBM PC and PC DOS 2.0                       }
{                                                                      }
{  based on the DUMBTERM program by CJ Dunford in the January 1984     }
{  issue of PC Tech Journal.                                           }
{                                                                      }
{  Entry points:                                                       }
{                                                                      }
{    Async_Init                                                        }
{      Performs initialization.                                        }
{                                                                      }
{    Async_Open(Port, Baud : Integer;                                  }
{               Parity : Char;                                         }
{               WordSize, StpBits : Integer) : Boolean                 }
{      Sets up interrupt vector, initialies the COM port for           }
{      processing, sets pointers to the buffer.  Returns FALSE if COM  }
{      port not installed.                                             }
{                                                                      }
{    Async_Buffer_Check(var C : Char) : Boolean                        }
{      If a character is available, returns TRUE and moves the         }
{        character from the buffer to the parameter                    }
{      Otherwise, returns FALSE                                        }
{                                                                      }
{    Async_Send(C : Char)                                              }
{      Transmits the character.                                        }
{                                                                      }
{    Async_Send_String(S : LStr)                                       }
{      Calls Async_Send to send each character of S.                   }
{                                                                      }
{    Async_Close                                                       }
{      Turn off the COM port interrupts.                               }
{      **MUST** BE CALLED BEFORE EXITING YOUR PROGRAM; otherwise you   }
{      will see some really strange errors and have to re-boot.        }
{                                                                      }
{----------------------------------------------------------------------}
{$B-} { Short circuit boolean ON }
{$I-} { I/O hecking OFF }
{$R-} { Range checking OFF }
{$S-} { Stack checking OFF }
{$V-} { Var-str checking OFF}

UNIT ASYNC4U;

INTERFACE

USES Dos;

{ global declarations }

type
  LStr = String[255];  { generic string type for parameters }

const
  Async_Buffer_Max = 4095;

var
  Async_OriginalVector : pointer;
  Async_Buffer       : Array[0..Async_Buffer_Max] of char;

  Async_Open_Flag    : Boolean;   { true if Open but no Close }
  Async_Port         : Integer;   { current Open port number (1 or 2) }
  Async_Base         : Integer;   { base for current open port }
  Async_Irq          : Integer;   { irq for current open port }

  Async_Buffer_Overflow : Boolean;  { True if buffer overflow has happened }
  Async_Buffer_Used     : Integer;
  Async_MaxBufferUsed   : Integer;

    { Async_Buffer is empty if Head = Tail }
  Async_Buffer_Head  : Integer;   { Locn in Async_Buffer to put next char }
  Async_Buffer_Tail  : Integer;   { Locn in Async_Buffer to get next char }
  Async_Buffer_NewTail : Integer;

{----------------------------------------------------------------------------}
{                          USER CALLABLE ROUTINES                            }
{----------------------------------------------------------------------------}
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

procedure Async_Clear_Cueue;
{----------------------------------------------------------------------------}

IMPLEMENTATION


const
  UART_THR = $00;    { offset from base of UART Registers for IBM PC }
  UART_RBR = $00;
  UART_IER = $01;
  UART_IIR = $02;
  UART_LCR = $03;
  UART_MCR = $04;
  UART_LSR = $05;
  UART_MSR = $06;

  I8088_IMR = $21;   { port address of the Interrupt Mask Register }


var

  Async_BIOS_Port_Table : Array[1..2] of Integer absolute $40:0;
               { This table is initialized by BIOS equipment determination
                 code at boot time to contain the base addresses for the
                 installed async adapters.  A value of 0 means "not in-
                 stalled." }

const
  Async_Num_Bauds = 10;
  Async_Baud_Table : array [1..Async_Num_Bauds] of record
                                                     Baud, Bits : integer
                                                   end
                   = ((Baud:110;  Bits:$00),
                      (Baud:150;  Bits:$20),
                      (Baud:300;  Bits:$40),
                      (Baud:600;  Bits:$60),
                      (Baud:1200; Bits:$80),
                      (Baud:2400; Bits:$A0),
                      (Baud:4800; Bits:$C0),
                      (Baud:9600; Bits:$E0),
                      (Baud:14400;Bits:$E0),
                      (Baud:19200;Bits:$E0));


PROCEDURE DisableInterrupts; inline($FA {cli} );     {MACROS}
PROCEDURE EnableInterrupts;  inline($FB {sti} );


procedure Async_Clear_Cueue;
begin
      Async_Buffer_Head := 0;
      Async_Buffer_Tail := 0;
      Async_Buffer_Overflow := FALSE;
end;

procedure BIOS_RS232_Init(ComPort, ComParm : Integer);
{ Issue Interrupt $14 to initialize the UART }
{ See the IBM PC Technical Reference Manual for the format of ComParm }
var
  Regs : registers;
begin
  with Regs do
    begin
      ax := ComParm and $00FF;  { AH=0; AL=ComParm }
      dx := ComPort;
      Intr($14, Regs)
    end
end; { BIOS_RS232_Init }


{----------------------------------------------------------------------}
{                                                                      }
{  ISR - Interrupt Service Routine                                     }
{                                                                      }
{----------------------------------------------------------------------}

procedure Async_Isr;  INTERRUPT;
{ Interrupt Service Routine }
{ Invoked when the UART has received a byte of data from the
  communication line }

{ re-written 9/10/84 to be entirely in machine language; original source
  left as comments }

begin

  Inline(
    $FB/                           { STI }
      { get the incomming character }
      { Async_Buffer[Async_Buffer_Head] := Chr(Port[UART_RBR + Async_Base]); }
    $8B/$16/Async_Base/            { MOV DX,Async_Base }
    $EC/                           { IN AL,DX }
    $8B/$1E/Async_Buffer_Head/     { MOV BX,Async_Buffer_Head }
    $88/$87/Async_Buffer/          { MOV Async_Buffer[BX],AL }
      { Async_Buffer_NewHead := Async_Buffer_Head + 1; }
    $43/                           { INC BX }
      { if Async_Buffer_NewHead > Async_Buffer_Max then
          Async_Buffer_NewHead := 0; }
    $81/$FB/Async_Buffer_Max/      { CMP BX,Async_Buffer_Max }
    $7E/$02/                       { JLE L001 }
    $33/$DB/                       { XOR BX,BX }
      { if Async_Buffer_NewHead = Async_Buffer_Tail then
          Async_Buffer_Overflow := TRUE
        else }
{L001:}
    $3B/$1E/Async_Buffer_Tail/     { CMP BX,Async_Buffer_Tail }
    $75/$08/                       { JNE L002 }
    $C6/$06/Async_Buffer_Overflow/$01/ { MOV Async_Buffer_Overflow,1 }
    $90/                           { NOP generated by assembler for some reason }
    $EB/$16/                       { JMP SHORT L003 }
      { begin
          Async_Buffer_Head := Async_Buffer_NewHead;
          Async_Buffer_Used := Async_Buffer_Used + 1;
          if Async_Buffer_Used > Async_MaxBufferUsed then
            Async_MaxBufferUsed := Async_Buffer_Used
        end; }
{L002:}
    $89/$1E/Async_Buffer_Head/     { MOV Async_Buffer_Head,BX }
    $FF/$06/Async_Buffer_Used/     { INC Async_Buffer_Used }
    $8B/$1E/Async_Buffer_Used/     { MOV BX,Async_Buffer_Used }
    $3B/$1E/Async_MaxBufferUsed/   { CMP BX,Async_MaxBufferUsed }
    $7E/$04/                       { JLE L003 }
    $89/$1E/Async_MaxBufferUsed/   { MOV Async_MaxBufferUsed,BX }
{L003:}
      { disable interrupts }
    $FA/                           { CLI }
      { Port[$20] := $20; }  { use non-specific EOI }
    $B0/$20/                       { MOV AL,20h }
    $E6/$20                        { OUT 20h,AL }
       )
end; { Async_Isr }


procedure Async_Close;
{ reset the interrupt system when UART interrupts no longer needed }
var
  i, m : Integer;
begin
  if Async_Open_Flag then
    begin

      { disable the IRQ on the 8259 }
      DisableInterrupts;
      i := Port[I8088_IMR];        { get the interrupt mask register }
      m := 1 shl Async_Irq;        { set mask to turn off interrupt }
      Port[I8088_IMR] := i or m;

      { disable the 8250 data ready interrupt }
      Port[UART_IER + Async_Base] := 0;

      { disable OUT2 on the 8250 }
      Port[UART_MCR + Async_Base] := 0;
      EnableInterrupts;

      SetIntVec(Async_Irq + 8,Async_OriginalVector);

      { re-initialize our data areas so we know the port is closed }
      Async_Open_Flag := FALSE

    end
end; { Async_Close }

function Async_Open(ComPort       : Integer;
                    BaudRate      : Integer;
                    Parity        : Char;
                    WordSize      : Integer;
                    StopBits      : Integer) : Boolean;
{ open a communications port }
var
  ComParm : Integer;
  Temp    : Integer;
  i, m : Integer;
  Skip : Boolean;

begin
  if Async_Open_Flag then Async_Close;
  Skip:=TRUE;

  if (ComPort = 2)  then
  BEGIN
    IF Async_BIOS_Port_Table[2] <> 0 THEN Skip:=FALSE;
    Async_Port := 2
  END
  else BEGIN
    Async_Port := 1;  { default to COM1 }
    Skip:=FALSE;
  END;
  IF NOT Skip THEN
  BEGIN
    Async_Base := Async_BIOS_Port_Table[Async_Port];
    Async_Irq := Hi(Async_Base) + 1;

    if (Port[UART_IIR + Async_Base] and $00F8) <> 0 then
      Async_Open := FALSE
    else
      begin
      Async_Buffer_Head := 0;
      Async_Buffer_Tail := 0;
      Async_Buffer_Overflow := FALSE;

  { Build the ComParm for RS232_Init }
  { See Technical Reference Manual for description }

      ComParm := $0000;

  { Set up the bits for the baud rate }
      i := 0;
      repeat
        i := i + 1
      until (Async_Baud_Table[i].Baud = BaudRate) or (i = Async_Num_Bauds);
      ComParm := ComParm or Async_Baud_Table[i].Bits;

      if Parity in ['E', 'e'] then ComParm := ComParm or $0018
      else if Parity in ['O', 'o'] then ComParm := ComParm or $0008
      else ComParm := ComParm or $0000;  { default to No parity }

      if WordSize = 7 then ComParm := ComParm or $0002
      else ComParm := ComParm or $0003;  { default to 8 data bits }

      if StopBits = 2 then ComParm := ComParm or $0004
      else ComParm := ComParm or $0000;  { default to 1 stop bit }

  { use the BIOS COM port initialization routine to save typing the code }
      BIOS_RS232_Init(Async_Port - 1, ComParm);

      IF Baudrate=14400 THEN
      BEGIN
        Temp                     :=Port[Async_Base+UART_LCR];
        Port[Async_Base+UART_LCR]:=128;
        Port[Async_Base+UART_RBR]:=8;
        Port[Async_Base+UART_IER]:=0;
        Port[Async_Base+UART_LCR]:=Temp; { store control line data }
      END;

      IF Baudrate=19200 THEN
      BEGIN
        Temp                     :=Port[Async_Base+UART_LCR];
        Port[Async_Base+UART_LCR]:=128;
        Port[Async_Base+UART_RBR]:=6;
        Port[Async_Base+UART_IER]:=0;
        Port[Async_Base+UART_LCR]:=Temp; { store control line data }
      END;

      GetIntVec(Async_Irq + 8, Async_OriginalVector);
      SetIntVec(Async_Irq + 8, @Async_Isr);

  { read the RBR and reset any possible pending error conditions }
  { first turn off the Divisor Access Latch Bit to allow access to RBR, etc. }

      DisableInterrupts;

      Port[UART_LCR + Async_Base] := Port[UART_LCR + Async_Base] and $7F;
  { read the Line Status Register to reset any errors it indicates }
      i := Port[UART_LSR + Async_Base];
  { read the Receiver Buffer Register in case it contains a character }
      i := Port[UART_RBR + Async_Base];

  { enable the irq on the 8259 controller }
      i := Port[I8088_IMR];  { get the interrupt mask register }
      m := (1 shl Async_Irq) xor $00FF;
      Port[I8088_IMR] := i and m;

  { enable the data ready interrupt on the 8250 }
      Port[UART_IER + Async_Base] := $01; { enable data ready interrupt }

  { enable OUT2 on 8250 }
      i := Port[UART_MCR + Async_Base];
      Port[UART_MCR + Async_Base] := i or $08;

      EnableInterrupts;
      Async_Open_Flag := TRUE;  { bug fix by Scott Herr }
      Async_Open := TRUE;

    end
    END
    ELSE Async_Open:=FALSE;

end; { Async_Open }

function Async_Buffer_Check(var C : Char) : Boolean;
{ see if a character has been received; return it if yes }
begin
  if Async_Buffer_Head = Async_Buffer_Tail then
    Async_Buffer_Check := FALSE
  else
    begin
      C := Async_Buffer[Async_Buffer_Tail];
      Async_Buffer_Tail := Async_Buffer_Tail + 1;
      if Async_Buffer_Tail > Async_Buffer_Max then
        Async_Buffer_Tail := 0;
      Async_Buffer_Used := Async_Buffer_Used - 1;
      Async_Buffer_Check := TRUE;
    end
end; { Async_Buffer_Check }





FUNCTION Async_Send(C : Char):Boolean;
{ transmit a character }
var
  i, m, counter : Integer;
begin
  Port[UART_MCR + Async_Base] := $0B; { turn on OUT2, DTR, and RTS }
(*
  { wait for CTS }
  counter := MaxInt;
  while (counter <> 0) and ((Port[UART_MSR + Async_Base] and $10) = 0) do
    counter := counter - 1;

  { wait for Transmit Hold Register Empty (THRE) }
  if counter <> 0 then counter := MaxInt;
  *)
  Counter:=maxint;
  while (counter <> 0) and ((Port[UART_LSR + Async_Base] and $20) = 0) do
    Dec(counter);
  Async_send:=FALSE;
  if counter <> 0 then
    begin
      { send the character }
      DisableInterrupts;
      Port[UART_THR + Async_Base] := Ord(C);
      EnableInterrupts;
      Async_send:=TRUE;
    end;
{  else
    writeln('<<<TIMEOUT>>>');}


end; { Async_Send }

procedure Async_Send_String(S : LStr);
{ transmit a string }
var
  i : Integer;

begin
  for i := 1 to length(S) do
    Async_Send(S[i])
end; { Async_Send_String }


{ initialize variables }
begin
  Async_Open_Flag := FALSE;
  Async_Buffer_Overflow := FALSE;
  Async_Buffer_Used := 0;
  Async_MaxBufferUsed := 0;
end. { ASYNC4U UNIT }
