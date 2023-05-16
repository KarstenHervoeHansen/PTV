/* Demonstration program for the PM5639 Industrial Color Sensor */
/* This program been written in Borland C++ 3.0 */

#include <conio.h>
#include <stdio.h>
#include <dos.h>

#include "pm39io.h"

int ch = 0, COMerror, COMPort = COM1;

IDINFO sensorid;
MEASINFO meas;

void InitScreen( void) {

  textbackground( BLUE);
  textcolor( WHITE);

  clrscr();

  textbackground( LIGHTGRAY);
  textcolor( BLACK);
  gotoxy( 1, 1);
  cprintf( "                 ProTeleVision Technologies A/S, DK-2605 Broendby               ");

  textbackground( BLUE);
  textcolor( YELLOW);

  gotoxy( 2, 3);
  cprintf( "            PM5639 Industrial Color Sensor Demonstration Program                    ");

  textcolor( WHITE);
  gotoxy( 2, 5);
  cprintf( "      Shows the measurements from the PM5639 Industrial Color Sensor ");
  gotoxy( 2, 7);
  cprintf("                         in CIE 1931 XYZ coordinates:");

  gotoxy( 2, 13);
  cprintf("                         in CIE 1931 xyY coordinates:");

  gotoxy( 9, 19);
  cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄSTATUSÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
  gotoxy( 9, 20);
  cprintf("³                                                              ³");
  gotoxy( 9, 21);
  cprintf("³                                                              ³");
  gotoxy( 9, 22);
  cprintf("³                                                              ³");
  gotoxy( 9, 23);
  cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");

  textcolor( YELLOW);

  gotoxy( 22, 9);
  cprintf( "       X         Y         Z");

  gotoxy( 22, 15);
  cprintf( "       x         y         Y");

  gotoxy( 9, 24);
  cprintf("                      PRESS  <ESC>  TO EXIT");

  textcolor( WHITE);

  gotoxy( 21, 21);
  cprintf( "Initializing Color Sensor. Please Wait!");
}

void Cursor_Off( void) {

  struct REGPACK regs;

  regs.r_ax = 0x0100;
  regs.r_cx = 0x2000;
  intr( 0x10, &regs);
}

void Cursor_On( void) {

  struct REGPACK regs;

  regs.r_ax = 0x0100;
  regs.r_cx = 0x0607;
  intr( 0x10, &regs);
}

main() {

	clrscr();
	gotoxy( 12, 13);
	printf( "1:  COM1");
	gotoxy( 12, 14);
	printf( "2:  COM2");
	gotoxy( 12, 15);
	printf( "3:  COM3");
	gotoxy( 12, 16);
	printf( "4:  COM4");
	gotoxy( 12, 17);
	printf( "q:  Exit");

	gotoxy( 12, 12);
	printf( "To which serial port is the sensor connected ? ");

	ch = ' ';

	while (( ch != 'q') && (( ch < '1') || ( ch > '4')))
		ch = getch();

	if ( ch == 'q')
		return( 0);

	COMPort = ch - '1';

	Cursor_Off();

	InitScreen();

	Open_TIME_TICK();

	COMerror = Open_COM( COMPort, Baud4800);

	if ( COMerror == 0)
		COMerror = Get_ID( COMPort, &sensorid);

	if ( COMerror == 0) {
		Set_Integration_Time( COMPort, 100);

		Command_To_COM( COMPort, "XY;");
		Start_Measuring( COMPort);

		gotoxy( 10, 21);
		cprintf( "                                                       ");

		do {
			Get_Measurement( COMPort, &meas);

			if ( meas.IOerror == 0 ) {  	/* If No Errors write CIE 1931 xyY */
				gotoxy( 10, 21);
				cprintf( "                                                       ");

				gotoxy( 22, 10);
				cprintf( "%10.2f%10.2f%10.2f", meas.X, meas.Y, meas.Z);

				gotoxy( 22, 16);
				cprintf( "%10.3f%10.3f%10.2f", meas.whr[0], meas.whr[1], meas.Y);
			}
			else {								/* else write error in status-window */
				gotoxy( 10, 21);
				cprintf( "                                                       ");

				gotoxy( 15, 21);
				cprintf( "Error No. %d: %s", meas.IOerror, PM39IOErrorTxt[meas.IOerror]);
			}

			if ( kbhit())
				ch = getch();

		} while ( ch != 0x1B);

		 gotoxy( 23, 21);
		 cprintf( "Beginning to shut down. Please Wait!");
	}
	else {
		gotoxy( 10, 21);
		cprintf( "                                                       ");

		gotoxy( 15, 21);
		cprintf( "Error No. %d: %s", COMerror, PM39IOErrorTxt[COMerror]);

		delay( 1000);
	}

	Close_COM( COMPort);
	Close_TIME_TICK();

	textbackground( BLACK);
	textcolor( WHITE);

	delay( 1000);

	clrscr();
	Cursor_On();
	return( 0);
}
