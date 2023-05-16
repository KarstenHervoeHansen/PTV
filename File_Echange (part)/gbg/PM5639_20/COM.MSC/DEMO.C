/* Demonstration program for the PM5639 Industrial Color Sensor */
/* This program been written in Borland C++ 3.0 */
/* and converted to Microsoft C/C++ 7.0 */

#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <graph.h>
#include <time.h>

#include "pm39io.h"

#define BLUE	   1
#define WHITE	   7
#define YELLOW	  14
#define BLACK	   0

char buffer[80];

int ch = 0, COMerror, COMPort = COM1;

IDINFO sensorid;
MEASINFO meas;

void InitScreen( void) {

  _setbkcolor( BLUE);
  _settextcolor( WHITE);

  _clearscreen( _GCLEARSCREEN);

  _setbkcolor( WHITE);
  _settextcolor( BLACK);
  _settextposition( 1, 1);
  _outtext( "        Philips TV Test Equipment A/S, Kornmarksvej 21-23, DK-2605 Broendby     \n");

  _setbkcolor( BLUE);
  _settextcolor( YELLOW);
  _settextposition( 3, 2);
  _outtext( "            PM5639 Industrial Color Sensor Demonstration Program                    ");

  _settextcolor( WHITE);
  _settextposition( 5, 2);
  _outtext( "      Shows the measurements from the PM5639 Industrial Color Sensor ");
  _settextposition( 7, 2);
  _outtext("                         in CIE 1931 XYZ coordinates:");

  _settextposition( 13, 2);
  _outtext("                         in CIE 1931 xyY coordinates:");

  _settextposition( 19, 9);
  _outtext("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄSTATUSÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
  _settextposition( 20, 9);
  _outtext("³                                                              ³");
  _settextposition( 21, 9);
  _outtext("³                                                              ³");
  _settextposition( 22, 9);
  _outtext("³                                                              ³");
  _settextposition( 23, 9);
  _outtext("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");

  _settextcolor( YELLOW);

  _settextposition( 9, 22);
  _outtext( "       X         Y         Z");

  _settextposition( 15, 22);
  _outtext( "       x         y         Y");

  _settextposition( 24, 9);
  _outtext("                      PRESS  <ESC>  TO EXIT");

  _settextcolor( WHITE);

  _settextposition( 21, 21);
  _outtext( "Initializing Color Sensor. Please Wait!");
}

void Cursor_Off( void) {

  union REGS regs;

  regs.x.ax = 0x0100;
  regs.x.cx = 0x2000;
  _int86( 0x10, &regs, &regs);
}

void Cursor_On( void) {

  union REGS regs;

  regs.x.ax = 0x0100;
  regs.x.cx = 0x0607;
  _int86( 0x10, &regs, &regs);
}

void delay( clock_t wait) {		/* Delay in microseconds */

	clock_t goal;

	goal = wait + clock();		/* Delay in milliseconds */
	while ( goal > clock())
	   ;
}

main() {

	_clearscreen( _GCLEARSCREEN);
	_settextposition( 13, 12);
	_outtext( "1:  COM1");
	_settextposition( 14, 12);
	_outtext( "2:  COM2");
	_settextposition( 15, 12);
	_outtext( "3:  COM3");
	_settextposition( 16, 12);
	_outtext( "4:  COM4");
	_settextposition( 17, 12);
	_outtext( "q:  Exit");

	_settextposition( 12, 12);
	_outtext( "To which serial port is the sensor connected ? ");

	ch = ' ';

	while (( ch != 'q') && (( ch < '1') || ( ch > '4')))
		ch = getch();

	if ( ch == 'q')
		return( 0);

	COMPort = ch - '1';

	Cursor_Off();

	InitScreen();

	Open_TIME_TICK();

	COMerror = Open_COM( COMPort);

	if ( COMerror == 0)
		COMerror = Get_ID( COMPort, &sensorid);

	if ( COMerror == 0) {
		Set_Integration_Time( COMPort, 100);

		Command_To_COM( COMPort, "XY;");
		Start_Measuring( COMPort);

		_settextposition( 21, 10);
		_outtext( "                   asa                                 ");

		do {
			Get_Measurement( COMPort, &meas);

			if ( meas.IOerror == 0 ) {  	/* If No Errors write CIE 1931 xyY */
				_settextposition( 21, 10);
				_outtext( "                                                       ");

				_settextposition( 10, 22);
				sprintf( buffer, "%10.2f%10.2f%10.2f", meas.X, meas.Y, meas.Z);
				_outtext( buffer);

				_settextposition( 16, 22);
				sprintf( buffer, "%10.3f%10.3f%10.2f", meas.whr[0], meas.whr[1], meas.Y);
				_outtext( buffer);
			}
			else {								/* else write error in status-window */
				_settextposition( 21, 10);
				_outtext( "                                                       ");

				_settextposition( 21, 15);
				sprintf( buffer, "Error No. %d: %s", meas.IOerror, PM39IOErrorTxt[meas.IOerror]);
				_outtext( buffer);
			}

			if ( kbhit())
				ch = getch();

		} while ( ch != 0x1B);

		 _settextposition( 21, 23);
		 _outtext( "Beginning to shut down. Please Wait!");
	}
	else {
		_settextposition( 21, 10);
		_outtext( "                                                       ");

		_settextposition( 21, 15);
		sprintf( buffer, "Error No. %d: %s", COMerror, PM39IOErrorTxt[COMerror]);
		_outtext( buffer);

		delay(( clock_t) 2*CLOCKS_PER_SEC);
	}

	Close_COM( COMPort);
	Close_TIME_TICK();

	_setbkcolor( BLACK);
	_settextcolor( WHITE);

	delay(( clock_t) 2*CLOCKS_PER_SEC);

	_clearscreen( _GCLEARSCREEN);

	Cursor_On();
	return( 0);
}
