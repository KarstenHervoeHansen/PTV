Unit Unit_IIC;

(*
 -------------------------------------------------------
 Unit for styring af IýC-bus med I/O-kort 7108 100 71821.
 -------------------------------------------------------
Rettelser:
000121    Rettet Procedure IICPause;
          Delay tilpasset Pentium 75MHz




InitIIC er for initialisering af IýC-kontroller PCD8584.
 IOAddr er adressen i PC'en. Status er 0 ved OK og 1 ved fejl.

TrmDA8591
 Styring af D/A-converter.
 Bit 6 i kontrolregistret s‘ttes = 1 for at enable udgangen.
 IOAddr er I/O adressen i PC.
 Addr er IýC-adressen p† kredsen.
 Data er et tal 0-255.

Trm8574
 Styring af I/O-port PCF8574.
 IOAddr er I/O adressen i PC.
 Addr er IýC-adressen.
 Data er et tal 0-255.

Read8574
 L‘sning af I/O port PCF8574.
 IOAddr er I/O adressen i PC.
 Addr er IýC-adressen.
 Data er et tal 0-255.

TrmAD8591
 Styring af control register i PCF8591.
 IOAddr er I/O adressen i PC.
 Addr er IýC-adressen.
 Data er et tal 0-255 for styring af control register.

ReadAD8591
 L‘sning af AD-konverter i PCF8591.
 IOAddr er I/O adressen i PC.
 Addr er IýC-adressen.
 Data er et tal 0-255.  (datablad for PCF8591 ved AD-input)

Yderligere information:  datablad for PCD8584
                         application note AN-425
*)


INTERFACE
CONST  ADCh8591_0       = 0;
       ADCh8591_1       = 1;
       ADCh8591_2       = 2;
       ADCh8591_3       = 3;
       InputMode8591_0  = $00;
       InputMode8591_1  = $10;
       InputMode8591_2  = $20;
       InputMode8591_3  = $30;

PROCEDURE InitIIC(IOAddr : WORD; VAR Status : BYTE);
PROCEDURE Trm8574(IOAddr : WORD; Addr,Data : BYTE;   VAR Status : BYTE);
PROCEDURE Read8574(IOAddr : WORD; Addr : BYTE; VAR Data : BYTE; VAR Status : BYTE);

PROCEDURE TrmDA8591(IOAddr : WORD; Addr,Data : BYTE;   VAR Status : BYTE);
PROCEDURE TrmAD8591(IOAddr : WORD; Addr,Data : BYTE;   VAR Status : BYTE);
PROCEDURE Read8591(IOAddr : WORD; Addr : BYTE; VAR Data : BYTE; VAR Status : BYTE);


IMPLEMENTATION
uses crt;

VAR _CB8591 : BYTE;        {Control Byte on PCF8591}

Procedure IICPause;
{ Delay tilpasset Pentium 75MHz }
begin
 delay(2);
end;

Procedure InitIIC(IOAddr : WORD; VAR Status : BYTE);
var S,n : BYTE;
BEGIN;
  IF (IOAddr < $300) OR (IOAddr > $31C) THEN
   BEGIN
    Status := 1;
   END
  ELSE
   BEGIN
    Status := 0;
    Port[IOAddr + 1] := 0;        {clear S1-register}
    IICPause;
    Port[IOAddr] := $01;          {dummy-write <> 0: select Intel-mode}
    IICPause;
    Port[IOAddr + 1] := $20;      {X,ES0,ES1,ES2 = 0010   select S2}
    IICPause;
    Port[IOAddr] := $1A;          {S2=00011010  PC-Clock=8Mhz  SCL=11kHz}
                                  {SCL=1.5kHz fungerer ikke}
    IICPause;
    Port[IOAddr + 1] := $40;      {X,ES0,ES1,ES2 = 0100   select serial mode}

    S := Port[IOAddr + 1];
    WHILE  (S AND $01) <> 1 DO    {vent til BB = 1}
    BEGIN
     IICPause;
     S := Port[IOAddr + 1];
    END;
  END;
END;



Procedure Trm8574(IOAddr : WORD; Addr,Data : BYTE;   VAR Status : BYTE);
VAR  S  : BYTE;
BEGIN
  Port[IOAddr + 1] := $40;        {IDLE: ES0=1  STA=0  STO=0  ACK=0}

(*  REPEAT                        {Test for BB = 1 fjernet i TurboPascal 7}
   IICPause;
   S := Port[IOAddr + 1];
  UNTIL (S AND $01) = 1;          {wait for BB = 1}
*)

  IICPause;
  Port[IOAddr + 1] := $41;        {START: ES0=1  STA=0  STO=0  ACK=1}
  IICPause;
  Port[IOAddr] := Addr;           {adresse}
  IICPause;
  Port[IOAddr + 1] := $45;        {START: ES0=1  STA=1  STO=0  ACK=1}
  S := Port[IOAddr + 1];
  WHILE  (S AND $80) <> 0 DO       {vent til PIN = 0}
  BEGIN
   IICPause;
   S := Port[IOAddr + 1];
  END;

 IICPause;

  Port[IOAddr] := Data;           {Send data}
  IICPause;

  S := Port[IOAddr + 1];

 { while loop fjernet ved k›rsel af program p† 486DX40 PC'er
   Delay(5) tilf›jet i stedet                             980918 PF}

(*  WHILE  (S AND $80) <> 0 DO       {vent til PIN = 0}
  BEGIN
   IICPause;
   S := Port[IOAddr + 1];
  END;
 *)
  Delay(5);
  IICPause;


  Port[IOAddr + 1] := $43;        {STOP: ES0=1  STA=0  STO=1  ACK=1}
  IICPause;
  Port[IOAddr] := 0;              {dummy SDA og SCL = high}
  IICPause;
  Status := S;
  Delay(5);
END;


PROCEDURE TrmDA8591(IOAddr : WORD; Addr,Data : BYTE;   VAR Status : BYTE);
VAR  S  : BYTE;
BEGIN
  Port[IOAddr + 1] := $40;        {IDLE: ES0=1  STA=0  STO=0  ACK=0}

  IICPause;
  Port[IOAddr + 1] := $41;        {START: ES0=1  STA=0  STO=0  ACK=1}
  IICPause;
  Port[IOAddr] := Addr;           {adresse}
  IICPause;
  Port[IOAddr + 1] := $45;        {START: ES0=1  STA=1  STO=0  ACK=1}
 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}

  Port[IOAddr] := (_CB8591 OR $40);       {enable output fra D/A PCF8591}
 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}
  IICPause;

  Port[IOAddr] := Data;           {Send data}
 REPEAT
  IICPause;
  S := Port[IOAddr + 1];
 UNTIL (S AND $80) = 0;           {vent til PIN = 0}
  IICPause;

  Port[IOAddr + 1] := $43;        {STOP: ES0=1  STA=0  STO=1  ACK=1}
  IICPause;
  Port[IOAddr] := 0;              {dummy SDA og SCL = high}
  IICPause;
  Status := S;
END;   {TrmDA8591}




PROCEDURE Read8574(IOAddr : WORD; Addr : BYTE; VAR Data : BYTE; VAR Status : BYTE);
VAR  S      : BYTE;
BEGIN
  Inc(Addr);                      {read data: addr is odd}
  IICPause;
  Port[IOAddr + 1] := $41;        {IDLE:  ES0=1 STA=0 STO=0 ACK=1}
  IICPause;
  Port[IOAddr] := Addr;           {adresse}
  IICPause;
  Port[IOAddr + 1] := $45;        {START:  ES0=1 STA=1 STO=0 ACK=1}
  IICPause;

 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}
  Status := S;
  Data := Port[IOAddr];           {start read data}
 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}
  IICPause;
  Port[IOAddr + 1] := $40;        {IDLE:  ES0=1 STA=0 STO=0 ACK=0}
  IICPause;
  Data := Port[IOAddr];           {Read valid data}
 REPEAT
  IICPause;
  S := Port[IOAddr + 1];
 UNTIL (S AND $80) = 0;           {vent til PIN = 0}
  IICPause;
  Port[IOAddr + 1] := $42;        {STOP:  ES0=1,  STA=0  STO=1  ACK=0}
  IICPause;
  Port[IOAddr] := 0;              {dummy SCL of SDA = high}
  IICPause;
  Status := S;
END; {Read8574}


PROCEDURE TrmAD8591(IOAddr : WORD; Addr,Data : BYTE;   VAR Status : BYTE);
VAR  S  : BYTE;
BEGIN
  Port[IOAddr + 1] := $40;        {IDLE: ES0=1  STA=0  STO=0  ACK=0}
  IICPause;
  Port[IOAddr + 1] := $41;        {START: ES0=1  STA=0  STO=0  ACK=1}
  IICPause;
  Port[IOAddr] := Addr;           {adresse}
  IICPause;
  Port[IOAddr + 1] := $45;        {START: ES0=1  STA=1  STO=0  ACK=1}
 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}
  IICPause;
  Port[IOAddr] := (_CB8591 OR Data);       {Control byte to PCF8591}

 REPEAT
  IICPause;
  S := Port[IOAddr + 1];
 UNTIL (S AND $80) = 0;           {vent til PIN = 0}

  IICPause;
  Port[IOAddr + 1] := $43;        {STOP: ES0=1  STA=0  STO=1  ACK=1}
  IICPause;
  Port[IOAddr] := 0;              {dummy SDA og SCL = high}
  IICPause;
  Status := S;
  IICPause;
END;   {TrmAD8591}


PROCEDURE Read8591(IOAddr : WORD; Addr : BYTE; VAR Data : BYTE; VAR Status : BYTE);
VAR  S      : BYTE;
BEGIN
  Inc(Addr);                      {read data: addr is odd}
  IICPause;
  Port[IOAddr + 1] := $41;        {IDLE:  ES0=1 STA=0 STO=0 ACK=1}
  IICPause;
  Port[IOAddr] := Addr;           {adresse}
  IICPause;
  Port[IOAddr + 1] := $45;        {START:  ES0=1 STA=1 STO=0 ACK=1}
  IICPause;

 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}
  Status := S;
  Data := Port[IOAddr];           {start read data}
 REPEAT
  IICPause;
 UNTIL (Port[IOAddr + 1] AND $80) = 0;   {vent til PIN = 0}
  IICPause;
  Port[IOAddr + 1] := $40;        {IDLE:  ES0=1 STA=0 STO=0 ACK=0}
  IICPause;
  Data := Port[IOAddr];           {Read last data}
 REPEAT
  IICPause;
  S := Port[IOAddr + 1];
 UNTIL (S AND $80) = 0;           {vent til PIN = 0}
  IICPause;

  Data := Port[IOAddr];           {Read valid data}
 REPEAT
  IICPause;
  S := Port[IOAddr + 1];
 UNTIL (S AND $80) = 0;           {vent til PIN = 0}
  IICPause;
  Port[IOAddr + 1] := $42;        {STOP:  ES0=1,  STA=0  STO=1  ACK=0}
  IICPause;
  Port[IOAddr] := 0;              {dummy SCL of SDA = high}
  IICPause;
  Status := S;
END; {Read8591}



BEGIN
_CB8591 := 0;
END.



