Unit PM39IO;

Interface

Uses DOS, CRT;

type
   PM39IOerrcodes = ( noerr, err1, err2, err3, err4, err5, err6, err7,
                      err8, err9, err10, err11, err12, err13, err14);

   errtxt = array[0..14] of string[50];

   IDINFO = record
               Company: string[12];     { e.g. 'PHILIPS' }
               KuNo: string[8];         { e.g. 'KU123456' }
               nc: string[12];          { e.g. '4008xxxxxxxx' }
               SoftWareRev: string[4];  { e.g. '02.0' }
               SensorType: integer;     { MUST be > 31 AND < 47 }
            end;

   MEASINFO = record
                 IOerror: integer;
                 X, Y, Z: real;
                 whr: array[0..5] of real;
              end;

Function Open_TIME_TICK:integer;
Function Close_TIME_TICK:integer;

Function Open_COM( COM_Port, Baud_Rate: integer):integer;
Function Close_COM( COM_Port: integer):integer;

Function Command_To_COM( COM_Port:integer; str: string):integer;

Function Start_Measuring( COM_Port: integer):integer;
Function Stop_Measuring( COM_Port: integer):integer;

Function Get_ID( COM_Port: integer; var SensorID: IDINFO): integer;

Function Set_Integration_Time( COM_Port, time: integer):integer;
Function Get_Integration_Time( COM_Port: integer; var time: real):integer;

Function Get_Measurement( COM_Port: integer; var Meas: MEASINFO):integer;

Const
   COM1 = 0;
   COM2 = 1;
   COM3 = 2;
   COM4 = 3;

   Baud4800 = 4800;
   Baud9600 = 9600;

   PM39IOErrorTxt: errtxt = (
      { 00 }   'No error',
      { 01 }   'Error in received data',
      { 02 }   'Serial COM port not supported',
      { 03 }   'Serial COM port was not installed',
      { 04 }   'Serial COM port already installed',
      { 05 }   'Timeout transmitting data to serial COM port',
      { 06 }   'Timeout receiving data from serial COM port',
      { 07 }   '_TIME_TICK was not installed',
      { 08 }   '_TIME_TICK already installed',
      { 09 }   'Integration time out of range',
      { 10 }   'White reference values NOT calculated',
      { 11 }   'Overload',
      { 12 }   'Lowlight',
      { 13 }   'Wrong sensor',
      { 14 }   'Wrong baudrate'
   );

Implementation

var
   _COM1_RBR,		{ UART Receiver Buffer Register }
   _COM1_THR,		{ UART Transmitter Holding Register }
   _COM1_IER,		{ UART Interrupt Enable Register }
   _COM1_IIR,		{ UART Interrupt Identity Register }
   _COM1_LCR,     { UART Line Control Register }
   _COM1_MCR,		{ UART Modem Control Register }
   _COM1_LSR,		{ UART Line Status Register }
   _COM1_MSR,		{ UART Modem Status Register }

   _COM1_intr,
   _COM1_mask,

   _COM2_RBR,		{ UART Receiver Buffer Register }
   _COM2_THR,		{ UART Transmitter Holding Register }
   _COM2_IER,		{ UART Interrupt Enable Register }
   _COM2_IIR,		{ UART Interrupt Identity Register }
   _COM2_LCR,		{ UART Line Control Register }
   _COM2_MCR,		{ UART Modem Control Register }
   _COM2_LSR,		{ UART Line Status Register }
   _COM2_MSR,		{ UART Modem Status Register }

   _COM2_intr,
   _COM2_mask: integer;

   i,
   ix,
   timeout_ok,
   COM1_rsno,
   COM1_count,
   COM2_rsno,
   COM2_count: byte;

   ch: char;

   tmp_ptr,
   _int01vect,
   _int02vect,
   _int1Cvect: pointer;

   COM1_Str,
   COM2_Str,
   COM1_Tmp_Str,
   COM2_Tmp_Str: array[0..3] of string[25];

   Send_To_COM1,
   Send_To_COM2,
   Data_Ready_COM1,
   Data_Ready_COM2: BOOLEAN;

   sumXYZ, divisor: real;

   X_Code, Y_Code, Z_Code,
   tmp_error: integer;

   reg: registers;

Procedure _TIME_TICK; Interrupt;
begin
   if ( timeout_ok > 0) then
      timeout_ok := timeout_ok - 1;

   port[$20] := $20;
end;


Procedure _COM1_Interrupt; Interrupt;
begin
   if ( NOT Send_To_COM1) then
   begin
       ch := char( port[_COM1_RBR]);

      if ( ch <> ',') then
      begin
         if ( COM1_count < 25) then
         begin
            inc( COM1_count);

            COM1_Tmp_Str[COM1_rsno][COM1_count] := ch;
            COM1_Tmp_Str[COM1_rsno][0] := char( COM1_count);
         end;

         if ( ch = char( $0D)) then
         begin
            byte( COM1_Tmp_Str[COM1_rsno][0]) :=
                  byte( COM1_Tmp_Str[COM1_rsno][0]) - 1;

            COM1_rsno := 0;
            COM1_count := 0;

            for ix:=0 to 3 do
            begin
               COM1_Str[ix] := COM1_Tmp_Str[ix];
               COM1_Tmp_Str[ix] := '';
            end;

            Data_Ready_COM1 := TRUE;
         end;
      end
      else
      begin
        COM1_rsno := COM1_rsno + 1;
        COM1_count := 0;

        if ( COM1_rsno > 3) then
           COM1_rsno := 0;
      end;

      ch := char( port[ _COM1_LSR]);
   end;

   port[$20] := $20;
end;


Procedure _COM2_Interrupt; Interrupt;
begin
   if ( NOT Send_To_COM2) then
   begin

      ch := char( port[ _COM2_RBR]);

      if ( ch <> ',') then
      begin
         if ( COM2_count < 25) then
         begin
            inc( COM2_count);

            COM2_Tmp_Str[COM2_rsno][COM2_count] := ch;
            COM2_Tmp_Str[COM2_rsno][0] := char( COM2_count);
         end;

         if ( ch = char( $0D)) then
         begin
            byte( COM2_Tmp_Str[COM2_rsno][0]) :=
                  byte( COM2_Tmp_Str[COM2_rsno][0]) - 1;

            COM2_rsno := 0;
            COM2_count := 0;

            for ix:=0 to 3 do
            begin
               COM2_Str[ix] := COM2_Tmp_Str[ix];
               COM2_Tmp_Str[ix] := '';
            end;

            Data_Ready_COM2 := TRUE;
         end;
      end
      else
      begin
         COM2_rsno := COM2_rsno + 1;
         COM2_count := 0;

         if ( COM2_rsno > 3) then
            COM2_rsno := 0;
      end;

      ch := char( port[_COM2_LSR]);
   end;

   port[$20] := $20;
end;


Function Open_TIME_TICK:integer;
begin
   GetIntVec( $1C, tmp_ptr);

   if ( Addr( _TIME_TICK) <> tmp_ptr) then
   begin
      GetIntVec( $1C, _int1Cvect);
      SetIntVec( $1C, Addr(_TIME_TICK));

      tmp_error := ord( noerr);
   end
   else
      tmp_error := ord( err8);

   Open_TIME_TICK := tmp_error;
end;


Function Close_TIME_TICK:integer;
begin
   GetIntVec( $1C, tmp_ptr);

   if ( Addr( _TIME_TICK) = tmp_ptr) then
   begin
      SetIntVec( $1C, _int1Cvect);

      tmp_error := ord( noerr);
   end
   else
      tmp_error := ord( err7);

   Close_TIME_TICK := tmp_error;
end;


Function Open_COM( COM_Port, Baud_Rate: integer):integer;
var
   PortBase: integer;
begin
   tmp_error := ord( noerr);

	case Baud_Rate of
		Baud4800:
         begin
         end;
		Baud9600:
			begin
			end;
		else
        tmp_error := ord( err14);
	end;			

   if ( tmp_error = ord( noerr)) then
	begin
	   PortBase := integer( Ptr( $40, 2*COM_Port)^);

   	case COM_Port of
      	COM1:
	        begin
   	        if ( PortBase = 0) then
      	        PortBase := $3F8;

	           _COM1_intr := $0C;
   	        _COM1_mask := $EF;
      	  end;

	      COM2:
   	     begin
      	     if ( PortBase = 0) then
         	     PortBase := $2F8;

	           _COM2_intr := $0B;
   	        _COM2_mask := $F7;
      	  end;

	     COM3:
   	     begin
      	     if ( PortBase = $3220) then
         	  begin
	              _COM1_intr := $0B;
   	           _COM1_mask := $F7;
      	     end
         	  else
	           begin
   	           if ( PortBase = 0) then
      	           PortBase := $3E8;

	              _COM1_intr := $0C;
   	           _COM1_mask := $EF;
      	     end;
	        end;

   	  COM4:
      	  begin
	           if ( PortBase = $3228) then
   	        begin
      	        _COM2_intr := $0C;
         	     _COM2_mask := $EF;
	           end
   	        else
      	     begin
         	     if ( PortBase = 0) then
	                 PortBase := $2E8;

   	           _COM2_intr := $0B;
      	        _COM2_mask := $F7;
         	  end;
	        end;

   	  else
      	  tmp_error := ord( err2);
	   end;
	end;

   if ( tmp_error = ord( noerr)) then
   begin
      case COM_Port of
         COM1, COM3:
            begin
               COM1_count := 0;

               GetIntVec( _COM1_intr, _int01vect);

               if ( Addr( _COM1_Interrupt) <> _int01vect) then
               begin
                  _COM1_RBR := PortBase;
                  _COM1_THR := PortBase;
                  _COM1_IER := PortBase + 1;
                  _COM1_IIR := PortBase + 2;
                  _COM1_LCR := PortBase + 3;
                  _COM1_MCR := PortBase + 4;
                  _COM1_LSR := PortBase + 5;
                  _COM1_MSR := PortBase + 6;

                  Send_To_COM1 := FALSE;
                  Data_Ready_COM1 := FALSE;
                  COM1_rsno := 0;

                  for i:=0 to 3 do
                  begin
                     COM1_Str[i] := '';
                     COM1_Tmp_Str[i] := '';
                  end;

                  { Disable COM1(3) interrupt }

                  port[$21] := port[$21] OR ( _COM1_mask XOR $FF);

                  { Test if specified port exists }
                  port[_COM1_IER] := $02;

                  if ( port[_COM1_IER] <> $02) then
                     tmp_error := ord( err2)
                  else
                  begin
							if ( Baud_Rate = Baud4800) then
							begin
	                     if ( PortBase =$3220) then  { If PC is a PS2 }
   	                  begin
      	                  reg.ax := $0400;     { Subservice 4, no break }
         	               reg.bx := $0001;     { NO parity, 2 stop }
            	            reg.cx := $0306;     { 8 data, 48000 baud }
               	         reg.dx := COM_Port;  { Serial COM port 1(3)}
                  	   end
	                     else
   	                  begin
      	                  reg.ax := $C7;{ 4800 baud, 8 data, 2 stop, NO parity }
         	               reg.dx := $00;{ Serial port COM1 }
            	         end;
							end
							else
							begin								{ Baudrate is 9600 baud }
	                     if ( PortBase =$3220) then  { If PC is a PS2 }
   	                  begin
      	                  reg.ax := $0400;     { Subservice 4, no break }
         	               reg.bx := $0001;     { NO parity, 2 stop }
            	            reg.cx := $0307;     { 8 data, 9600 baud }
               	         reg.dx := COM_Port;  { Serial COM port 1(3)}
                  	   end
	                     else
   	                  begin
      	                  reg.ax := $E7;{ 9600 baud, 8 data, 2 stop, NO parity }
         	               reg.dx := $00;{ Serial port COM1 }
            	         end;
							end;
                     intr( $14, reg);

                     port[_COM1_MCR] := $0B;          { Clear DTR, RTS,& OUT2 }

                     byte( ch) := port[_COM1_RBR];    { Clear input register }
                     byte( ch) := port[_COM1_LSR];    { Clear status register }
                     byte( ch) := port[_COM1_IIR];
                     byte( ch) := port[_COM1_MSR];

                                             { Save old interrupt vector }
                     GetIntVec( _COM1_intr, _int01vect);
                                             { Install new interrupt vector }
                     SetIntVec( _COM1_intr, Addr(_COM1_Interrupt));

                     port[_COM1_IER] := $05;  { Enabel COM1 intr. in UART i.e.
                                             enable Received Data Available &
                                             Receiver Line Status }

                                             { Enable COM1 Interrupt }
                     port[$21] := port[$21] AND _COM1_mask;
                  end
               end
               else
                   tmp_error := ord( err4);
            end;

         COM2, COM4:
            begin
               COM2_count := 0;

               GetIntVec( _COM2_intr, _int02vect);

               if ( Addr( _COM2_Interrupt) <> _int02vect) then
               begin
                  _COM2_RBR := PortBase;
                  _COM2_THR := PortBase;
                  _COM2_IER := PortBase + 1;
                  _COM2_IIR := PortBase + 2;
                  _COM2_LCR := PortBase + 3;
                  _COM2_MCR := PortBase + 4;
                  _COM2_LSR := PortBase + 5;
                  _COM2_MSR := PortBase + 6;

                  Send_To_COM2 := FALSE;
                  Data_Ready_COM2 := FALSE;
                  COM2_rsno := 0;

                  for i:=0 to 3 do
                  begin
                     COM2_Str[i] := '';
                     COM2_Tmp_Str[i] := '';
                  end;

                  { Disable COM2(4) interrupt }
                  port[$21] := port[$21] OR ( _COM2_mask XOR $FF);

                  { Test if specified port exists }
                  port[_COM2_IER] := $02;

                  if ( port[_COM2_IER] <> $02) then
                     tmp_error := ord( err2)
                  else
                  begin
							if ( Baud_Rate = Baud4800) then
							begin
	                     if ( PortBase = $3228) then  { If PC is a PS2 }
   	                  begin
      	                  reg.ax := $0400;     { Subservice 4, no break }
         	               reg.bx := $0001;     { NO parity, 2 stop }
            	            reg.cx := $0306;     { 8 data, 48000 baud }
               	         reg.dx := COM_Port;  { Serial COM port 2(3)}
                  	   end
	                     else
   	                  begin
      	                  reg.ax := $C7;{ 4800 baud, 8 data, 2 stop, NO parity }
         	               reg.dx := $01;{ Serial port COM2(4) }
            	         end;
							end
							else
							begin
	                     if ( PortBase = $3228) then  { If PC is a PS2 }
   	                  begin
      	                  reg.ax := $0400;     { Subservice 4, no break }
         	               reg.bx := $0001;     { NO parity, 2 stop }
            	            reg.cx := $0307;     { 8 data, 9600 baud }
               	         reg.dx := COM_Port;  { Serial COM port 2(3)}
                  	   end
	                     else
   	                  begin
      	                  reg.ax := $E7;{ 9600 baud, 8 data, 2 stop, NO parity }
         	               reg.dx := $01;{ Serial port COM2(4) }
            	         end;
							end;
              	      intr( $14, reg);

                     port[_COM2_MCR] := $0B;         { Clear DTR, RTS,& OUT2 }

                     byte( ch) := port[_COM2_RBR];    { Clear input register }
                     byte( ch) := port[_COM2_LSR];    { Clear status register }
                     byte( ch) := port[_COM2_IIR];
                     byte( ch) := port[_COM2_MSR];
                                             { Save old interrupt vector }
                     GetIntVec( _COM2_intr, _int02vect);
                                             { Install new interrupt vector }
                     SetIntVec( _COM2_intr, Addr(_COM2_Interrupt));

                     port[_COM2_IER] := $05;     { Enabel COM2 intr. in UART i.e.
                                             enable Received Data Available &
                                             Receiver Line Status }
                     port[$21] := port[$21] AND _COM2_mask;      { Enable COM2 Interrupt }
                  end;
               end
               else
                  tmp_error := ord( err4);
            end;
      end;
   end;

   if ( tmp_error = ord( noerr)) then
      port[$20] := $20;                             { Enable 8259 Interrupt }

   Open_COM := tmp_error;
end;


Function Close_COM( COM_Port: integer):integer;
begin
   tmp_error := ord( noerr);

   case COM_Port of
      COM1, COM3:
         begin
            GetIntVec( _COM1_intr, tmp_ptr);

            if ( Addr( _COM1_Interrupt) = tmp_ptr) then
            begin
               tmp_error := Stop_Measuring( COM1);
                                     { Disable ALL serial int. in UART COM1 }
               port[_COM1_IER] := $00;
               port[$21] := port[$21] OR $10;

               byte( ch) := port[_COM1_IIR];      { Reset serial COM1 IIR }

               SetIntVec( _COM1_intr, _int01vect); { Restore old int. vector }
            end
            else
               tmp_error := ord( err3);
         end;

      COM2, COM4:
         begin
            GetIntVec( _COM2_intr, tmp_ptr);

            if ( Addr( _COM2_Interrupt) = tmp_ptr) then
            begin
               tmp_error := Stop_Measuring( COM2);
                                     { Disable ALL serial int. in UART COM2 }
               port[_COM2_IER] := $00;
               port[$21] := port[$21] OR $08;

               byte( ch) := port[_COM2_IIR];      { Reset serial COM2 IIR }

               SetIntVec( _COM2_intr, _int02vect);{ Restore old intr vector }
            end
            else
               tmp_error := ord( err3);
         end;
      else
         tmp_error := ord( err2);
   end;

   Close_COM := tmp_error;
end;


Function Command_To_COM( COM_Port:integer; str: string):integer;
var
   _COM1_IER_tmp, _COM2_IER_tmp: byte;
begin
   timeout_ok := 8;
   tmp_error := ord( noerr);

   case COM_Port of
      COM1, COM3:
         begin
            _COM2_IER_tmp := port[ _COM2_IER];
            port[_COM2_IER] := $05;

            Send_To_COM1 := TRUE;

            ch := char( port[ _COM1_IIR]);

            for i:=1 to Length( str) do
            begin
               if ( tmp_error = ord( noerr)) then
               begin
						Delay( 1);

                  while (( port[_COM1_LSR] AND $20) <> $20) AND
                         ( timeout_ok > 0) do
                     ;

                  port[_COM1_THR] := ord( str[i]);

                  if ( timeout_ok = 0) then
                     tmp_error := ord( err5);

                  ch := char( port[ _COM1_IIR]);
               end;
            end;

            Send_To_COM1 := FALSE;
            Data_Ready_COM1 := FALSE;
            COM1_rsno := 0;

            for i:=0 to 3 do
            begin
               COM1_Str[i] := '';
               COM1_Tmp_Str[i] := '';
            end;

            ch := char( port[ _COM1_LSR]);
            ch := char( port[ _COM1_RBR]);
            ch := char( port[ _COM1_IIR]);
            ch := char( port[ _COM1_MSR]);

            port[_COM2_IER] := _COM2_IER_tmp;
         end;

      COM2, COM4:
         begin
            _COM1_IER_tmp := port[ _COM1_IER];
            port[_COM1_IER] := $05;

            Send_To_COM2 := TRUE;

            ch := char( port[ _COM2_IIR]);

            for i:=1 to Length( str) do
            begin
               if ( tmp_error = ord( noerr)) then
               begin
						Delay( 1);

                  while (( port[_COM2_LSR] AND $20) <> $20) AND
                         ( timeout_ok > 0) do
                     ;

                  port[_COM2_THR] := ord( str[i]);

                  if ( timeout_ok = 0) then
                     tmp_error := ord( err5);

                  ch := char( port[ _COM2_IIR]);
               end;
            end;

            Send_To_COM2 := FALSE;
            Data_Ready_COM2 := FALSE;
            COM2_rsno := 0;

            for i:=0 to 3 do
            begin
               COM2_Str[i] := '';
               COM2_Tmp_Str[i] := '';
            end;

            ch := char( port[ _COM2_LSR]);
            ch := char( port[ _COM2_RBR]);
            ch := char( port[ _COM2_IIR]);
            ch := char( port[ _COM2_MSR]);

            port[_COM1_IER] := _COM1_IER_tmp;
         end;
      else
         tmp_error := ord( err2);
   end;

   Command_To_COM := tmp_error;
end;


Function Start_Measuring( COM_Port: integer):integer;
begin
   Start_Measuring := Command_To_COM( COM_Port, 'MC;');
end;


Function Stop_Measuring( COM_Port: integer):integer;
begin
   Stop_Measuring := Command_To_COM( COM_Port, 'MS;');
end;


Function Set_Integration_Time( Com_Port, time: integer):integer;
var
   str1, str2: string[8];
begin
   if (( time < 25) OR ( time > 250)) then
      tmp_error := ord( err9)
   else
   begin
      str1 := 'SI';
      str( time, str2);
      str1 := str1 + str2 + ';';

      tmp_error := Command_To_COM( COM_Port, str1);
   end;

   Set_Integration_Time := tmp_error;
end;


Function Get_Integration_Time( COM_Port: integer; var time: real):integer;
var
   tmp_time: real;
begin
   tmp_error := Command_To_COM( COM_Port, 'F?;');

   if ( tmp_error = ord( noerr)) then
   begin
      timeout_ok := 8;

      case COM_Port of
         COM1, COM3:
            begin
               while (( NOT Data_Ready_COM1) AND ( timeout_ok > 0)) do
                  ;

               if ( timeout_ok = 0) then
                  tmp_error := ord(err6)
               else
                  val( COM1_Str[0], tmp_time, X_Code);
            end;

         COM2, COM4:
            begin
               while (( NOT Data_Ready_COM2) AND ( timeout_ok > 0)) do
                  ;

               if ( timeout_ok = 0) then
                  tmp_error := ord( err6)
               else
                  val( COM2_Str[0], tmp_time, X_Code);
            end;
         else
            tmp_error := ord( err2);
      end;
   end;

   if ( tmp_error = ord( noerr)) then
   begin
      if ( X_Code <> 0) then
         tmp_error := ord( err1)
      else
         time := tmp_time;
   end;

   Get_Integration_Time := tmp_error
end;


Function Get_ID( COM_Port: integer; var SensorID: IDINFO): integer;
var
   tmp_value: integer;
begin
   tmp_error := Command_To_COM( COM_Port, 'I?;');

   if ( tmp_error = ord(noerr)) then
   begin
      timeout_ok := 8;

      case COM_Port of
         COM1, COM3:
            begin
               while (( NOT Data_Ready_COM1) AND ( timeout_ok > 0)) do
                  ;

               if ( timeout_ok = 0) then
                  tmp_error := ord( err6)
               else
               begin
                  with SensorID do
                  begin
                     Company := COM1_Str[0];
                     nc := COM1_Str[1];
                     KuNo := COM1_Str[2];
                     SoftWareRev := COM1_Str[3];
                  end;

                  tmp_error := Command_To_COM( COM1, 'MA61;');

                  if ( tmp_error = ord( noerr)) then
                  begin
                     tmp_error := Command_To_COM( COM1, 'RM;');

                     if ( tmp_error = ord( noerr)) then
                     begin
                        timeout_ok := 8;

                        while (( NOT Data_Ready_COM1) AND
                               ( timeout_ok > 0)) do
                           ;

                        if ( timeout_ok = 0) then
                           tmp_error := ord( err6)
                        else
                           val( COM1_Str[0], tmp_value, X_Code);
                     end;
                  end;
               end;
            end;

         COM2, COM4:
            begin
               while (( NOT Data_Ready_COM2) AND ( timeout_ok > 0)) do
                  ;

               if ( timeout_ok = 0) then
                  tmp_error := ord( err6)
               else
               begin
                  with SensorID do
                  begin
                     Company := COM2_Str[0];
                     nc := COM2_Str[1];
                     KuNo := COM2_Str[2];
                     SoftWareRev := COM2_Str[3];
                  end;

                  tmp_error := Command_To_COM( COM2, 'MA61;');

                  if ( tmp_error = ord( noerr)) then
                  begin
                     tmp_error := Command_To_COM( COM2, 'RM;');

                     if ( tmp_error = ord( noerr)) then
                     begin
                        timeout_ok := 8;

                        while (( NOT Data_Ready_COM2) AND
                               ( timeout_ok > 0)) do
                           ;

                        if ( timeout_ok = 0) then
                           tmp_error := ord( err6)
                        else
                           val( COM2_Str[0], tmp_value, X_Code);
                     end;
                  end;
               end;
            end;
         else
            tmp_error := ord( err2);
      end;
   end;

   if ( tmp_error = ord( noerr)) then
   begin
      if ( X_Code <> 0) then
         tmp_error := ord( err1)
      else
      begin
         SensorID.SensorType := tmp_value;

         if (( tmp_value = 0) OR ( tmp_value = 16)) then
            tmp_error := ord( err13)
         else
            tmp_error := ord( noerr);
      end;
   end;

   Get_ID := tmp_error;
end;


Function Get_Measurement( COM_Port: integer; var Meas: MEASINFO):integer;
begin
   timeout_ok := 50;
   Meas.IOerror := ord( noerr);

   case COM_Port of
      COM1, COM3:
         begin
           while (( NOT Data_Ready_COM1) AND ( timeout_ok > 0)) do
              ;

           if ( timeout_ok = 0) then
              Meas.IOerror := ord( err6)
           else
           begin
              val( COM1_Str[0], Meas.X, X_Code);
              val( COM1_Str[1], Meas.Y, Y_Code);
              val( COM1_Str[2], Meas.Z, Z_Code);

              Data_Ready_COM1 := FALSE;
           end;
         end;

      COM2, COM4:
         begin
           while (( NOT Data_Ready_COM2) AND ( timeout_ok > 0)) do
              ;

           if ( timeout_ok = 0) then
              Meas.IOerror := ord( err6)
           else
           begin
              val( COM2_Str[0], Meas.X, X_Code);
              val( COM2_Str[1], Meas.Y, Y_Code);
              val( COM2_Str[2], Meas.Z, Z_Code);

              Data_Ready_COM2 := FALSE;
           end;
         end;
      else
         Meas.IOerror := ord( err2);
   end;

   if ( Meas.IOerror = ord( noerr)) then
   begin
      if (( X_Code <> 0) OR ( Y_Code <> 0) OR ( Z_Code <> 0)) then
         Meas.IOerror := ord( err1)
      else
      begin
         if ( Meas.X < -0.5) then
            Meas.IOerror := ord( err11)
         else
         begin
            if (( Meas.X < 0.01) OR ( Meas.Y < 0.01) OR ( Meas.Z < 0.01)) then
               Meas.IOerror := ord( err12)
            else
            begin
               sumXYZ := Meas.X + Meas.Y + Meas.Z;
               divisor := Meas.X + 15*Meas.Y + 3*Meas.Z;

               if (( sumXYZ > 0.01) AND ( divisor > 0.01)) then
               begin
                  Meas.whr[0] := Meas.X / sumXYZ;
                  Meas.whr[1] := Meas.Y / sumXYZ;

                  Meas.whr[2] := 4*Meas.X / divisor;
                  Meas.whr[3] := 9*Meas.Y / divisor;

                  Meas.whr[4] := Meas.whr[2];
                  Meas.whr[5] := 6*Meas.Y / divisor;
               end
               else
                  Meas.IOerror := ord( err10);
            end;
         end;
      end;
   end;

   if ( Meas.IOerror <> ord( noerr)) then
   begin
      Meas.X := 0;
      Meas.Y := 0;
      Meas.Z := 0;
   end;

   Get_Measurement := Meas.IOerror;
end;
end.

