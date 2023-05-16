/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PM5639/20 Industrial Color Analyzer									*/
/*	Module:		MAIN.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	------																		*/
/*	Rev. date:	970826, KEn	DEV															*/
/*	Status:		Version -.-																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/* 970826:	Added new text for PM5639/25 Miniature CRT sensor					*/
/* 970820: Added command line parameter /B:nn										*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <string.h>
#include <alloc.h>
#include <bios.h>
#include <dir.h>
#include <ctype.h>
#include <math.h>
#include "pm5639.h"

#pragma hdrstop

#include "main.h"

void TypeHeadLines( char* errortxt) {

	extern char* SoftwareType;
	extern char* abouttxt[];
	extern const char* KUNUMBER;

	int ch;

	if ( !strcmp( SoftwareType, "INDU"))
		printf( "\n%s\n", abouttxt[0]);
	else {
		if ( !strcmp( SoftwareType, "SMLL"))
			printf( "\n%s\n", abouttxt[1]);
		else
			printf( "\n%s\n", abouttxt[2]);
	}

	printf( "%s\n", abouttxt[3]);

	strcpy( &abouttxt[4][11], KUNUMBER);
	printf( "%s\n", abouttxt[4]);
	printf( "%s\n", abouttxt[5]);
	printf( "%s\n\n", abouttxt[6]);

	if ( *errortxt)
		printf( "%s\n\n", errortxt);

	if ( !strcmp( SoftwareType, "INDU"))
		printf( "PM39_20 [/B:nn] [/M:<ON | OFF>] [/F:file] \n\n");
	else {
		if ( !strcmp( SoftwareType, "SMLL"))
			printf( "PM39_25 [/B:nn] [/M:<ON | OFF>] [/F:file] \n\n");
		else
			printf( "PM39_01 [/B:nn] [/M:<ON | OFF>] [/F:file] \n\n");
	}

	printf( "  /B:nn    -  Specifies the baudrate of the COM-port(s). nn=48 sets\n");
	printf( "              the baudrate at 4800 baud, while nn=96 sets the baudrate\n");
	printf( "              at 9600 baud.\n");
	printf( "              /B:48 is the default setting.\n");
	printf( "  /M:ON    -  Specifies the measurements to start automatically after\n");
	printf( "              after display of the Philips logo for 10 sec.\n");
	printf( "  /M:OFF   -  Specifies the measurements NOT to start automatically\n");
	printf( "              /M:OFF is the default setting.\n");
	printf( "  /F:file  -  Specifies setup-file file.PGM to be used at startup.\n");
	printf( "              The PGM file is a program setup file previously saved in\n");
	printf( "              the program. The default file is the default configuration\n");
	printf( "              file PM5639.CFG\n");
	printf( "  /?       -  This help display, (also /H or /h)\n\n");

	if ( !strcmp( SoftwareType, "INDU"))
		printf( "Example: PM39_20 /B:96 /M:ON /F:MYFILE\n\n");
	else {
		if ( !strcmp( SoftwareType, "SMLL"))
			printf( "Example: PM39_25 /B:96 /M:ON /F:MYFILE\n\n");
		else
			printf( "Example: PM39_01 /B:96 /M:ON /F:MYFILE\n\n");
	}

	return( 0);
}

main( int argc, char* argv[]) {
	MENU file = { 6, 0, 33, 0, " File     ",
		" Start/Stop Measuring  Ctrl+F9", toggleMeasuring,
		" Normal/Fast Mode", toggleFastMode,
		" 컴컴컴컴컴컴컴컴컴컴컴컴컴컴�", idle,
		" Information", openInformation,
		" 컴컴컴컴컴컴컴컴컴컴컴컴컴컴�", idle,
		" Exit                   Alt+X", idle,
	/* "123456789012345678901234567890 */
	};

	MENU edit = { 8, 0, 28, 0, " Edit     ",
		" Display Setup", openSetup,
		" Text On/Off           F9", doTextOnOff,
		" Display Status", openDispStatus,
		" Generate Report", doPrintReport,
		" 컴컴컴컴컴컴컴컴컴컴컴컴", idle,
		" Scale Down       Ctrl+F8", doScaleDown,
		" Scale Up          Alt+F8", doScaleUp,
		" Autoscale             F8", doAutoScale,
	/* "123456789012345678901234567890 */
	};

	MENU learn = { 2, 0, 19, 0, " Learn    ",
		" White Reference", doLearnWhr,
		" Phosphor", doLearnPhosphor,
	/* "123456789012345678901234567890 */
	};

	MENU save = { 5, 0, 19, 0, " Save    ",
		" Program Setup", doSaveProgram,
		" Display Setup", doSaveDisplay,
		" Measurement", doSaveMeasurement,
		" White Reference", doSaveWhr,
		" Phosphor", doSavePhosphor,
	/* "123456789012345678901234567890 */
	};

	MENU restore = { 5, 0, 19, 0, " Restore    ",
		" Program Setup", doRestoreProgram,
		" Display Setup", doRestoreDisplay,
		" Measurement", doRestoreMeasurement,
		" White Reference", doRestoreWhr,
		" Phosphor", doRestorePhosphor,
	/* "123456789012345678901234567890 */
	};

	MENU display = { 7, 0, 32, 0, " Display    ",
		" Open CIE Display      Alt+F1", openCIE,
		" Open RGB/XYZ Display  Alt+F2", openRGB,
		" 컴컴컴컴컴컴컴컴컴컴컴컴컴컴", idle,
		" Size/Move            Ctrl+F5", doMove,
		" Next                      F6", doNext,
		" Close                 Alt+F3", doClose,
		" Close All", doCloseAll,
	/* "123456789012345678901234567890 */
	};

	MENU help = { 4, 0, 16, 0, " Help  ",
		" Index", openHelpIndex,
		" Help on Help", openHelpHelp,
		" 컴컴컴컴컴", idle,
		" About....   ", doAbout,
	/* "123456789012345678901234567890 */
	};

	int ch, i, j, alive = 0xFF, DotDetected = 0;
	char* tmpArg;
	int AutoStart = 0;
	char filNavn[128];

	MEASINFO Meas[2];

	filNavn[0] = '\x0';
	LogoTimeoutON = 0;
	Baud9600 = 0;							/* Default baudrate at 4800 baud */

	if ( argc > 1) {

		printf("\n\n %s\n\n", argv[1]);

		if ( argc > 3) {
			TypeHeadLines( "ERROR: Too many arguments in command line.");
			return(0);
		}

		for (i=1; i<argc; i++) {
			tmpArg = argv[i];

			while ( *tmpArg) {
				*tmpArg = toupper( *tmpArg);
				tmpArg++;
			}

			if ( !strcmp( argv[i], "/?") || !strcmp( argv[i], "/H")) {
				TypeHeadLines( "") ;
				return( 0);
			}
		}

		printf("\n\n %s\n\n", argv[1]);

		for ( i=1; i<argc; i++) {
			if ( argv[i][0] != '/') {
				TypeHeadLines( "ERROR: Not a command in command line.");
				return(0);
			}

			if ( !strcmp( argv[i], "/M:ON")) {
				LogoTimeoutON = 0xFF;
				AutoStart = 0xFF;
			}

			if ( !strcmp( argv[i], "/M:OFF"))
					AutoStart = 0x00;

			if ( !strcmp( argv[i], "/B:48"))
				Baud9600 = 0;

			if ( !strcmp( argv[i], "/B:96"))
				Baud9600 = 1;

			if (( argv[i][1] == 'F') && ( argv[i][2] == ':')) {
				j = 0;
				tmpArg = &argv[i][3];

				while (( *tmpArg) && (j < 128))
					filNavn[j++] = *tmpArg++;

				if ( filNavn[j-4] == '.') {
					if (( filNavn[j-3] != 'P') || ( filNavn[j-2] != 'G') ||
							( filNavn[j-1] != 'M'))
						filNavn[0] = ' ';
				}
				else {
					filNavn[j++] = '.';
					filNavn[j++] = 'P';
					filNavn[j++] = 'G';
					filNavn[j++] = 'M';
					filNavn[j] = '\x0';
				}
			}
		}
	}

	if ( initGraphics()) {

		/* Initializing GLOBAL varaiables */

		NoWin = ActiveMenu = MenuMode = 0;
		COMopened[COM1] = COMopened[COM2] = INACTIVE;
		FASTmode = INACTIVE;

		Open_TIME_TICK();

		addMenu( &file);
		addMenu( &edit);
		addMenu( &learn);
		addMenu( &save);
		addMenu( &restore);
		addMenu( &display);
		addMenu( &help);

		initWindowManager();
		initFilesSystem( filNavn);

		if ( i = initHelpSystem())
			displayError( i);

		signalSensorStatus();
		drawWindows();

		if ( AutoStart == 0xFF)
			toggleMeasuring();

		while ( alive) {
			switch ( ch = GetKey()) {
				case kbNoKey:
					break;

				case kbRight:
				case kbLeft:
				case kbTab:
				case kbShiftTab:
					if ( MenuMode) {
						NextMenu( ch);
						if ( MenuMode == 2)
							openMenu();
					}
					break;

				case kbUp:
				case kbDown:
					if ( MenuMode == 1) {
						MenuMode++;
						openMenu();
					}
					else
						if ( MenuMode == 2)
							NextMenuText( ch);
					break;

				case kbF10:
					if ( !MenuMode) {
						MenuMode++;
						InvMenuTitle( ActiveMenu, ACTIVE);
					}
					break;

				case kbEnter:
					if ( MenuMode ==1) {
						MenuMode++;
						openMenu();
					}
					else
						if ( MenuMode == 2) {
							MenuMode = 0;
							InvMenuTitle( ActiveMenu, INACTIVE);
							closeMenu();

							/* Special cond. for EXIT in menu */
							if ((ActiveMenu == 0) && ( Menu[0]->lastitem == 5))
								alive = doQuit();
							else
								(Menu[ActiveMenu]->item[Menu[ActiveMenu]->lastitem].proc)(Menu[ActiveMenu]->lastitem);
						}
					break;

				case kbAltX:
					alive = doQuit();
					break;

				case kbEsc:
					closeMenuBar();
					break;

				case kbF1:
					openHelp( HlpIntroduction);

					if ( MenuMode == 2) {
						closeMenu();
						MenuMode = 2;
						openMenu();
					}
					break;

				case kbF6:
					closeMenuBar();
					doNext();
					break;

				case kbF8:
					closeMenuBar();
					doAutoScale();
					break;

				case kbAltF8:
					closeMenuBar();
					doScaleUp();
					break;

				case kbCtrlF8:
					closeMenuBar();
					doScaleDown();
					break;

				case kbF9:
					closeMenuBar();
					doTextOnOff();
					break;

				case kbAltF1:
					closeMenuBar();
					openCIE();
					break;

				case kbAltF2:
					closeMenuBar();
					openRGB();
					break;

				case kbCtrlF5:
					closeMenuBar();
					doMove();
					break;

				case kbAltF3:
					closeMenuBar();
					doClose();
					break;

				case kbCtrlF9:
					closeMenuBar();
					toggleMeasuring();
					break;

				case kbAlt1:
				case kbAlt2:
				case kbAlt3:
				case kbAlt4:
				case kbAlt5:
				case kbAlt6:
					doTopWindow((( ch - kbAlt1) >> 8)+1);
					break;

				case kbAltF:
				case kbAltE:
				case kbAltL:
				case kbAltS:
				case kbAltR:
				case kbAltD:
				case kbAltH:
					closeMenuBar();
					MenuMode = 2;
					switch ( ch) {
						case kbAltF:
							ActiveMenu = 0;
							break;
						case kbAltE:
							ActiveMenu = 1;
							break;
						case kbAltL:
							ActiveMenu = 2;
							break;
						case kbAltS:
							ActiveMenu = 3;
							break;
						case kbAltR:
							ActiveMenu = 4;
							break;
						case kbAltD:
							ActiveMenu = 5;
							break;
						case kbAltH:
							ActiveMenu = 6;
							break;
					}
					InvMenuTitle( ActiveMenu, ACTIVE);
					openMenu();
					break;

				default:
					if ( MenuMode == 1) {
						ch = toupper( ch & 0x00FF);
						switch ( ch) {
							case 'F':
							case 'E':
							case 'L':
							case 'S':
							case 'R':
							case 'D':
							case 'H':
								InvMenuTitle( ActiveMenu, INACTIVE);
								switch ( ch) {
									case 'F':
										ActiveMenu = 0;
										break;
									case 'E':
										ActiveMenu = 1;
										break;
									case 'L':
										ActiveMenu = 2;
										break;
									case 'S':
										ActiveMenu = 3;
										break;
									case 'R':
										ActiveMenu = 4;
										break;
									case 'D':
										ActiveMenu = 5;
										break;
									case 'H':
										ActiveMenu = 6;
										break;
								}
								MenuMode++;
								InvMenuTitle( ActiveMenu, ACTIVE);
								openMenu();
								break;
						}
					}
					break;
			}

			if ( !MenuMode & alive) {

				if ( COMopened[COM1])
					if ( Get_Measurement( COM1, &Meas[COM1]) == 6) {

						/* Timeout from sensor in COM1 */

						errTxt[18].txt3 = COMText[COM1];
						displayError( 18);
						toggleMeasuring();
					}

				if ( COMopened[COM2])
					if ( Get_Measurement( COM2, &Meas[COM2]) == 6) {

						/* Timeout from sensor in COM2 */

						errTxt[18].txt3 = COMText[COM2];
						displayError( 18);
						toggleMeasuring();
					}

				for ( i=1; i <= NoWin; i++)
					if ( WinPtr[i]->running == ACTIVE) {

						memcpy( &WinPtr[i]->meas, &Meas[WinPtr[i]->sensor],
																		sizeof( Meas[COM1]));
						switch ( WinPtr[i]->mode) {
							case CIEMode:
								updateCIE( i);
								break;
							case RGBMode:
								updateRGB( i);
								break;
						}
					}
			}
		}
	}
	return( 0);
}