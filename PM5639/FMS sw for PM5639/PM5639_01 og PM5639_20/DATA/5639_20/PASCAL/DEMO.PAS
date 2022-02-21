{ Demonstration program for the PM5639 Industrial Color Sensor }
{ This program been written in Turbo Pascal 7.0 }

Program Demo;

Uses DOS, CRT, PM39IO;

Var
   COMPort, COMerror: integer;
   ch: char;

   sensorid: IDINFO;
   meas: MEASINFO;

Procedure InitScreen;
begin
   textbackground( BLUE);
   textcolor( WHITE);

   clrscr;

   textbackground( LIGHTGRAY);
   textcolor( BLACK);
   gotoxy( 1, 1);
   write( '        Philips TV Test Equipment A/S, Kornmarksvej 21-23, DK-2605 Broendby     ');

   textbackground( BLUE);
   textcolor( YELLOW);

   gotoxy( 2, 3);
   write( '            PM5639 Industrial Color Sensor Demonstration Program                    ');

   textcolor( WHITE);
   gotoxy( 2, 5);
   write( '      Shows the measurements from the PM5639 Industrial Color Sensor ');
   gotoxy( 2, 7);
   write('                         in CIE 1931 XYZ coordinates:');

   gotoxy( 2, 13);
   write('                         in CIE 1931 xyY coordinates:');

   gotoxy( 9, 19);
   write('ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄSTATUSÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿');
   gotoxy( 9, 20);
   write('³                                                              ³');
   gotoxy( 9, 21);
   write('³                                                              ³');
   gotoxy( 9, 22);
   write('³                                                              ³');
   gotoxy( 9, 23);
   write('ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ');

   textcolor( YELLOW);

   gotoxy( 22, 9);
   write( '       X         Y         Z');

   gotoxy( 22, 15);
   write( '       x         y         Y');

   gotoxy( 9, 24);
   write('                      PRESS  <ESC>  TO EXIT');

   textcolor( WHITE);

   gotoxy( 21, 21);
   write( 'Initializing Color Sensor. Please Wait!');
end;


Procedure Cursor_Off;
var
   regs: registers;
begin
   with regs do
   begin
      ax := $0100;
      cx := $2000;
   end;
   intr( $10, regs);
end;

Procedure Cursor_On;
var
   regs: registers;
begin
   with regs do
   begin
      ax := $0100;
      cx := $0607;
   end;
   intr( $10, regs);
end;

begin

	clrscr;
	gotoxy( 12, 13);
	write( '1:  COM1');
	gotoxy( 12, 14);
	write( '2:  COM2');
	gotoxy( 12, 15);
	write( '3:  COM3');
	gotoxy( 12, 16);
	write( '4:  COM4');
	gotoxy( 12, 17);
	write( 'q:  Exit');

	gotoxy( 12, 12);
	write( 'To which serial port is the sensor connected ? ');

	ch := ' ';

	while (( ch <> 'q') AND (( ch < '1') OR ( ch > '4'))) do
		ch := readkey;

   if ( ch <> 'q') then
   begin
   	COMPort := ord( ch) - ord( '1');

      Cursor_Off;

      InitScreen;

      COMerror := Open_TIME_TICK;
      COMerror := Open_COM( COMPort);

      if ( COMerror = 0) then
         COMerror := Get_ID( COMPort, sensorid);

      if ( COMerror = 0) then
      begin
         COMerror := Set_Integration_Time( COMPort, 100);

	      COMerror := Command_To_COM( COMPort, 'XY;');
         COMerror := Start_Measuring( COMPort);

         gotoxy( 10, 21);
         write( '                                                       ');

         repeat
            COMerror := Get_Measurement( COMPort, meas);

            if ( meas.IOerror = 0 ) then   { If No Errors write CIE 1931 xyY }
            begin
               gotoxy( 10, 21);
               write( '                                                       ');

               gotoxy( 22, 10);
               write( meas.X:10:2, meas.Y:10:2, meas.Z:10:2);

               gotoxy( 22, 16);
               write( meas.whr[0]:10:3, meas.whr[1]:10:3, meas.Y:10:2);
            end
            else 								{ else write error in status-window }
            begin
               gotoxy( 10, 21);
			      write( '                                                       ');

			      gotoxy( 15, 21);
               write( 'Error No. ',Meas.IOerror, ': ',PM39IOErrorTxt[Meas.IOerror]);
            end;

            if keypressed then
			      ch := readkey;

         until ( ch = chr( $1B));

         gotoxy( 23, 21);
         write( 'Beginning to shut down. Please Wait!');
      end
      else
      begin
         gotoxy( 10, 21);
         write( '                                                       ');

         gotoxy( 15, 21);
         write( 'Error No. ', COMerror, ': ',PM39IOErrorTxt[COMerror]);
      end;

      COMerror := Close_COM( COMPort);
      COMerror := Close_TIME_TICK;

      textbackground( BLACK);
      textcolor( WHITE);

      delay( 1000);

      Cursor_On;
   end;
   clrscr;
end.
