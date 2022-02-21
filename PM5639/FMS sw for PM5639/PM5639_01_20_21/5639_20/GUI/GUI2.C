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

#include "gui2.h"

extern float SoftWareNO;

int TestIfFastMode( WININFO* wptr){
	int type;

	type = wptr->sensorid.SensorType;

	if ((( type >= 32) && ( type <= 47)) ||
		(( type >= 8) && ( type <= 15)) ||
		(( type >= 56) && ( type <= 79)))
		return( ACTIVE);

	return( INACTIVE);
}


void openCOMPort( int COMPort, WININFO* wptr) {

	int int_time;
	int COMerror;
	unsigned int type;

	COMopened[COMPort] = ACTIVE;
	Open_COM( COMPort);

	clearSensorId( wptr);

	COMerror = Get_ID( COMPort, &wptr->sensorid);

	if (( COMerror != 0) && ( COMerror != 13)) {

		COMopened[COMPort] = INACTIVE;
		Close_COM( COMPort);

		errTxt[26].txt2 = COMText[COMPort];
		displayError( 26);
	}
	else {
		type = wptr->sensorid.SensorType;

		if ((( type >= 1) && ( type <= 15)) ||
			 (( type >= 32) && ( type <= 79))) {

			if ( FASTmode) {
				if( TestIfFastMode( wptr))
					int_time = 25;
				else {
					displayError( 44);

					FASTmode = INACTIVE;
					int_time = 250;
				}
			}
			else {
				FASTmode = INACTIVE;
				int_time = 250;
			}

			Command_To_COM( COMPort, "XY;");
			Set_Integration_Time( COMPort, int_time);
			Start_Measuring( COMPort);
		}
		else {
			COMopened[COMPort] = INACTIVE;
			Close_COM( COMPort);

			errTxt[38].txt2 = "Wrong Sensor connected to";
			errTxt[38].txt3 = COMText[COMPort];
			displayError( 38);
		}
	}
}

void shutdownCOMPorts( void) {
	int i;

	for ( i = COM1; i <= COM2; i++)
		if ( COMopened[i]) {
			Stop_Measuring( i);
			Close_COM( i);
			COMopened[i] = INACTIVE;
		}

	for ( i=1; i<= NoWin; i++)
		WinPtr[i]->running = INACTIVE;

	signalSensorStatus();
}


void clearSensorId( WININFO* wptr) {

	wptr->sensorid.KuNo[0] = wptr->sensorid.SoftWareRev[0] = 0;
	wptr->sensorid.Company[0] = wptr->sensorid.nc[0] = 0;
}


int doQuit( void) {

	int i;
	FILE* stream;

	switch ( displayError( 27)) {
		case YES:
			if (( stream = openFile( CONFIG_FILE, "wb", -1)) != NULL) {

				fwrite( &SoftWareNO, sizeof( SoftWareNO), 1, stream);

				fwrite( &FASTmode, sizeof( FASTmode), 1, stream);

				for ( i=1; i <= NoWin; i++) {
					clearSensorId( WinPtr[i]);
					fwrite( WinPtr[i], sizeof( Win[0]), 1, stream);
				}
				fclose( stream);
				break;
			}
			return( -1);

		case NO:
			break;

		case kbEsc:
			return( -1);
	}

	if ( displayError( 28) == YES) {

		shutdownCOMPorts();
		Close_TIME_TICK();

		for ( i=0; i <= MAXDISPLAY; i++)
			free( CIECrossImage[i]);

		free( MenuWindow.back);

		closegraph();
		return( 0);
	}

	return( -1);
}


int doClose( void){

	int i;
	int testCOM[2]= { INACTIVE, INACTIVE };

	if ( NoWin) {
		i = displayError( 30);
		if ( i && ( i != kbEsc)) {
			free( CIECrossImage[NoWin]);

			CIECrossImage[NoWin] = NULL;
			CIECrossSize[NoWin] = -1;

			if ( --NoWin == 0) {
				shutdownCOMPorts();

				for ( i=0; i <= MAXDISPLAY; i++)
					WinPtr[i] = &Win[i];
			}
			else {
				for ( i=1; i<=NoWin; i++)
					testCOM[WinPtr[i]->sensor] = WinPtr[i]->running;

				for ( i=COM1; i <= COM2; i++)
					if ( COMopened[i] && !testCOM[i]) {
						Stop_Measuring( i);
						Close_COM( i);
						COMopened[i] = INACTIVE;
					}

				signalSensorStatus();
			}
			drawWindows();
		}
	}
	return( 0);
}


int doCloseAll( void){

	int i;

	if ( NoWin) {
		i = displayError( 40);

		if ( i && ( i != kbEsc)) {
			shutdownCOMPorts();

			for ( i=0; i <= MAXDISPLAY; i++) {
				free( CIECrossImage[i]);

				CIECrossImage[i] = NULL;
				CIECrossSize[i] = -1;

				WinPtr[i] = &Win[i];
			}

			clearWindowManager();

			return( NoWin = 0);
		}
	}
	return( NoWin);
}


int doTextOnOff( void) {

	if ( NoWin) {
		if ( WinPtr[NoWin]->texton)
			WinPtr[NoWin]->texton = TextOff;
		else
			WinPtr[NoWin]->texton = TextOn;

		drawDisplay( NoWin);
		return( 0);
	}

	return( displayError( 0));
}


int doScaleUp( void) {

	if ( NoWin) {
		if ( ++WinPtr[NoWin]->scalemeas > HighGain)
			WinPtr[NoWin]->scalemeas = LowGain;

		drawDisplay( NoWin);
		return( 0);
	}

	return( displayError( 0));
}


int doScaleDown( void) {

	if ( NoWin) {
		if ( --WinPtr[NoWin]->scalemeas < LowGain)
			WinPtr[NoWin]->scalemeas = HighGain;

		drawDisplay( NoWin);
		return( 0);
	}

	return( displayError( 0));
}


int doAutoScale( void) {

	WININFO* newP = WinPtr[NoWin];

	int mode = newP->mode;
	float x1, y1, Xval, Yval, Zval;

	if ( NoWin) {
		if (( mode == CIEMode) || ( mode == RGBMode)) {
			if ( COMopened[newP->sensor]) {

				if ( mode == CIEMode) {

					x1 = fabs( newP->dx / newP->xscale);
					y1 = fabs( newP->dy / newP->yscale);

					if (( x1 < 25) && ( y1 < 25))
						newP->scalemeas = HighGain;
					else
						if (( x1 < 125) && ( y1 < 125))
							newP->scalemeas = MedGain;
					else
						newP->scalemeas = LowGain;

					drawCIE( NoWin);
					return( 0);
				}

				if ( mode == RGBMode) {
					Xval = fabs( newP->Xval - 1.0);
					Yval = fabs( newP->Yval - 1.0);
					Zval = fabs( newP->Zval - 1.0);

					if (( Xval < 0.1) && ( Yval < 0.1) && ( Zval < 0.1))
						newP->scalemeas = HighGain;
					else
						if (( Xval < .25) && ( Yval < .25) && ( Zval < .25))
							newP->scalemeas = MedGain;
					else
						newP->scalemeas = LowGain;

					drawRGB( NoWin);
					return( 0);
				}
			}
		}
		return( 0);
	}
	else
		return( displayError( 0));
}


int doNext( void) {

	WININFO* w = WinPtr[NoWin];
	int i;

	if ( NoWin > 1) {
		WinPtr[NoWin] = WinPtr[1];

		for ( i=1; i < NoWin-1; i++)
			WinPtr[i] = WinPtr[i+1];

		WinPtr[NoWin-1] = w;

		drawWindows();
	}
	return( 0);
}


int doTopWindow( int no) {

	WININFO* w;
	int i;

	for ( i=1; i <= NoWin; i++)
		if ( WinPtr[i]->WinNo == no) {

			while( WinPtr[NoWin]->WinNo != no) {

				w = WinPtr[NoWin];
				WinPtr[NoWin] = WinPtr[1];

				for ( i=1; i < NoWin-1; i++)
					WinPtr[i] = WinPtr[i+1];

				WinPtr[NoWin-1] = w;
			}
		}

	drawWindows();
	return( 0);
}


int doMove( void) {

	RECT* r = &WinPtr[NoWin]->rect;
	RECT oldr;

	extern volatile timeout_ok;				/* timeout_ok factor = 53,4 ms */

	int ch, userpat = 0x00FF, alive = -1;

	if ( NoWin) {
		oldr.left = r->left;
		oldr.top = r->top;
		oldr.right = r->right;
		oldr.bottom = r->bottom;

		setcolor( EGA_LIGHTGRAY);
		setwritemode( XOR_PUT);

		setlinestyle( USERBIT_LINE, userpat, NORM_WIDTH);
		rectangle( r->left, r->top, r->right, r->bottom);

		while ( alive) {
			switch ( ch = GetKey()) {
				case kbSpace:
				case kbEnter:
				case kbEsc:
					alive = 0;
					break;

				case kbRight:
				case kbLeft:
				case kbUp:
				case kbDown:
				case kbShiftRight:
				case kbShiftLeft:
				case kbShiftDown:
				case kbShiftUp:
					rectangle( r->left, r->top, r->right, r->bottom);
					switch ( ch) {
						case kbRight:
							if ( r->right < ( SCREENWIDE-DROPSHADOW-5)) {
								r->right += 5;
								r->left += 5;
							}
							break;

						case kbLeft:
							if ( r->left > 5) {
								r->right -= 5;
								r->left -= 5;
							}
							break;

						case kbUp:
							if ( r->top > ( MANAGERBARDEEP+7)) {
								r->top -= 5;
								r->bottom -= 5;
							}
							break;

						case kbDown:
							if ( r->bottom < ( SCREENDEEP-DROPSHADOW-MANAGERBARDEEP-7)) {
								r->bottom += 5;
								r->top += 5;
							}
							break;

						case kbShiftRight:
							if ( r->right < ( SCREENWIDE-DROPSHADOW-5))
								r->right += 5;
							break;

						case kbShiftLeft:
							if ( r->right > ( r->left+DispMinWidth+5))
								r->right -= 5;
							break;

						case kbShiftUp:
							if ( r->bottom > ( r->top+DispMinHeight+ 5))
								r->bottom -= 5;
							break;

						case kbShiftDown:
							if ( r->bottom < ( SCREENDEEP-DROPSHADOW-MANAGERBARDEEP-7))
								r->bottom += 5;
							break;
					}
					rectangle( r->left, r->top, r->right, r->bottom);
					break;
			}

			timeout_ok = 1;
			while ( timeout_ok)
				;

			rectangle( r->left, r->top, r->right, r->bottom);

			setlinestyle( USERBIT_LINE, userpat ^= 0xFFFF, NORM_WIDTH);
			rectangle( r->left, r->top, r->right, r->bottom);
		}

		setwritemode( COPY_PUT);

		if ( ch == kbEsc) {
			r->left = oldr.left;
			r->top = oldr.top;
			r->right = oldr.right;
			r->bottom = oldr.bottom;
		}

		drawWindows();
		return( 0);
	}

	return( displayError( 0));
}


void writeInfo( char* txt1, char* txt2, int x, int y) {

	int i = x + ( strlen( txt1) << 3) - 3;

	setcolor( EGA_BLACK);
	setfillstyle( SOLID_FILL, EGA_GREEN);

	moveto( x, y);
	outtext( txt1);

	if ( strcmp( txt2, ""))
		bar( i, y-2, i+( strlen( txt2) << 3)+4, y+8);

	setcolor( EGA_WHITE);
	outtext( txt2);
	setcolor( EGA_BLACK);
}


void findDisplayOverlap( void) {

	RECT* s, *t;

	int i, j;
	int xa, xc, yb, yd, za, zc, vb, vd;

	if ( COMopened[COM1] || COMopened[COM2]) {

		for ( i=1; i <= NoWin; i++) {
			if (( COMopened[WinPtr[i]->sensor] == ACTIVE) &&
				 (( WinPtr[i]->mode != CIEMeasMode) &&
					( WinPtr[i]->mode != RGBMeasMode)))
					WinPtr[i]->running = ACTIVE;
		}

		for ( i = NoWin; i > 0; i--) {
			s = &WinPtr[i]->rect;

			for ( j = i-1; j > 0; j--) {
				t = &WinPtr[j]->rect;

				xa = (( t->left < s->left) ? 1 : 0);
				xc = (( t->left < s->right) ? 1 : 0);

				yb = (( t->top < s->top) ? 1 : 0);
				yd = (( t->top < s->bottom) ? 1 : 0);

				za = (( t->right < s->left) ? 1 : 0);
				zc = (( t->right < s->right) ? 1 : 0);

				vb = (( t->bottom < s->top) ? 1 : 0);
				vd = (( t->bottom < s->bottom) ? 1 : 0);

				if (( xa && !za && xc && !zc && yb && !vb && yd && !vd) ||
					 ( !xa && xc && yd && !vb) ||
					 ( !yb && yd && xc && !za) ||
					 ( !za && zc && yd && !vb) ||
					 ( !vb && vd && xc && !za) ||

					 ( !xa && zc && !yb && vd) ||
					 ( xa && !za && !vb && yd) ||
					 ( yb && !vb && !za && xc) ||
					 ( xc && !zc && !vb && yd) ||
					 ( yd && !vd && !za && xc))
					 WinPtr[j]->running = INACTIVE;
			}
		}
	}
}


void signalSensorStatus( void) {

	char* status;
	int maxx = getmaxx();
	int maxy = getmaxy();

	setcolor( EGA_WHITE);
	setfillstyle( SOLID_FILL, EGA_BLUE);

	if ( FASTmode)
		status = " FAST  ";
	else
		status = "NORMAL ";

	moveto( maxx-208, maxy-MANAGERBARDEEP+1);

	bar( maxx-216, maxy-MANAGERBARDEEP-1, maxx-152, maxy);

	outtext( status);

	if ( COMopened[COM1] || COMopened[COM2]) {
		setcolor( EGA_BLACK);
		setfillstyle( SOLID_FILL, EGA_GREEN);
		status = " SENSOR(S) RUNNING";
	}
	else {
		setfillstyle( SOLID_FILL, EGA_RED);
		status = " SENSOR(S) STOPPED";
	}

	bar( maxx-152, maxy-MANAGERBARDEEP-1, maxx, maxy);
	outtext( status);
}


int toggleMeasuring( void) {

	WININFO COMPtr[2];
	int mode = WinPtr[1]->mode;
	int i;

	if ( NoWin) {
		if (( COMopened[COM1] || COMopened[COM2]))
			shutdownCOMPorts();
		else {
			for ( i=1; i<= NoWin; i++, mode = WinPtr[i]->mode)
				if (( mode == CIEMode) || ( mode == RGBMode))
					COMopened[WinPtr[i]->sensor] = ACTIVE;

			for ( i=COM1; i <= COM2; i++)
				if ( COMopened[i] == ACTIVE)
					openCOMPort( i, &COMPtr[i]);

			for ( i=1; i <= NoWin; i++)
				if ( COMopened[WinPtr[i]->sensor] == ACTIVE)
					memcpy( &WinPtr[i]->sensorid,
						&COMPtr[WinPtr[i]->sensor].sensorid,
							sizeof( COMPtr[0].sensorid));

			signalSensorStatus();
		}
		drawWindows();
		return( 0);
	}

	return( displayError( 0));
}


int toggleFastMode( void) {

	int i, int_time, FastModeTest;;

	if ( NoWin) {

		FastModeTest = ACTIVE;

		if ( COMopened[0] || COMopened[1])
			for ( i=0; i<NoWin; i++)
				if ( WinPtr[i+1]->running == ACTIVE)
					if ( !TestIfFastMode( WinPtr[i+1]))
						FastModeTest = INACTIVE;

		if ( FastModeTest) {
			if ( FASTmode == INACTIVE) {
				FASTmode = ACTIVE;
				int_time = 25;
			}
			else {
				FASTmode = INACTIVE;
				int_time = 250;
			}

			for ( i = COM2; i >= COM1; i--)
				if ( COMopened[i]) {
					Stop_Measuring( i);
					Set_Integration_Time( i, int_time);
					Start_Measuring( i);
				}

			signalSensorStatus();
			return( 0);
		}

		return( displayError( 44));
	}

	return( displayError( 0));
}


int openInformation( void) {

	WININFO* newP = WinPtr[++NoWin];
	BUTTON bu[3];

	void* Image;
	char* txtptr;

	int equip_list = biosequip();
	struct diskfree_t freedisk;

	int i, ch, group=0, alive = -1, a_inner;
	char namebuf[25];
	extern void* displayMessage( int left, int top, int color, char* message);

	newP->mode = HelpMode;
	SetRect( &newP->rect, InfLeft, InfTop, InfRight, InfBottom);

	while ( alive) {
		openWindow( newP, "Information");
		setcolor( EGA_BLACK);

		moveto( InfLeft+10, InfTop+WINDOWBARDEEP+12);
		outtext( "Free memory on heap:  ");

		ltoa( coreleft(), namebuf, 10);
		outtext( namebuf);

		if ( equip_list & CO_PROCESSOR_MASK)
			txtptr = "Math coprocessor installed";
		else
			txtptr = "No math coprocessor installed";

		outtextxy( InfLeft+10, InfTop+WINDOWBARDEEP+24, txtptr);

		moveto( InfLeft+10, InfTop+WINDOWBARDEEP+36);
		outtext( "Number of Serial Ports: ");

		itoa( ( equip_list & NO_SERIAL_PORTS) >> 9, namebuf, 10);
		outtext( namebuf);

		moveto( InfLeft+10, InfTop+WINDOWBARDEEP+48);
		outtext( "Number of windows open: ");

		itoa( NoWin-1, namebuf, 10);
		outtext( namebuf);

		if ( _dos_getdiskfree(0, &freedisk) == 0) {

			moveto( InfLeft+10, InfTop+WINDOWBARDEEP+60);
			outtext( "Free disk space on current drive: ");

			Image = displayMessage(( InfLeft + InfRight)/2,
							( InfTop + InfBottom)/2, EGA_GREEN, "Please Wait!!");

			ltoa((( long) freedisk.avail_clusters *
					( long) freedisk.bytes_per_sector *
					( long) freedisk.sectors_per_cluster), namebuf, 10);

			setcolor( EGA_BLACK);

			putimage(( InfLeft + InfRight)/2,
											( InfTop + InfBottom)/2, Image, COPY_PUT);
			free( Image);

			outtext( namebuf);
			outtext( "  bytes.");
		}

		addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

		for ( i=0; i<3; i++)
			drawButton( &bu[i], INACTIVE);

		drawButton( &bu[group], ACTIVE);

		a_inner = -1;

		while ( a_inner) {
			switch ( ch = GetKey()) {
				case kbTab:
				case kbShiftTab:
					drawButton( &bu[group], INACTIVE);

					if ( ch == kbTab) {
						if ( ++group > 2)
							group = 0;
					}
					else {
						if ( --group < 0)
							group = 2;
					}

					drawButton( &bu[group], ACTIVE);
					break;

				case kbF1:
					openHelp( HlpFileInformation);
					a_inner = 0;
					break;

				case kbEnter:
					if ( group == 2) {
						openHelp( HlpFileInformation);
						a_inner = 0;
						break;
					}											/* break IS missing */
				case kbEsc:
					a_inner = alive = 0;
					break;
			}
		}
	}
	closeWindow();

	return( 1);
}


int openDispStatus( void) {

	WININFO* newP, *oldP;
	BUTTON bu[3];

	int top = DispTop, bottom = DispBottom;
	int i, ch, group = 0, alive = -1, a_inner;

	char valbuf[6][8];
	char headline[25];

	if ( NoWin) {

		oldP = WinPtr[NoWin++];
		newP = WinPtr[NoWin];

		newP->mode = HelpMode;

		strcpy( headline, "Display Status no: ");
		itoa( oldP->WinNo, valbuf[0], 10);
		strcat( headline, valbuf[0]);

		if (( oldP->RGBmode != RGBHor) && ( oldP->RGBmode != RGBVer)) {
			top += 30;
			bottom -= 30;
		}

		SetRect( &newP->rect, DispLeft, top, DispRight, bottom);

		top += WINDOWBARDEEP;

		while( alive) {

			openWindow( newP, headline);

			for ( i=0; i<6; i++)
				sprintf( valbuf[i], "%5.4f", oldP->whrinfo.whr[i]);

			setcolor( EGA_BLACK);
			setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

			rectangle( DispLeft+4, top+7, DispRight-4, top+51);

			writeInfo( "Serial port : ",
					COMText[oldP->sensor], DispLeft+16, top+12);

			writeInfo( "CRT Sensor  : ",
					oldP->sensorid.KuNo, DispLeft+16, top+26);

			writeInfo( "Software revision in CRT Sensor : ",
					oldP->sensorid.SoftWareRev, DispLeft+16, top+40);

			rectangle( DispLeft+4, top+65, DispRight-4, top+123);

			writeInfo( "Active white reference : ",
							oldP->whrinfo.name, DispLeft+16, top+70);

			outtextxy( DispLeft+24, top+84, "CIE 1931");
			writeInfo( "x = ", valbuf[0], DispLeft+24, top+98);
			writeInfo( "y = ", valbuf[1], DispLeft+3*8, top+112);

			outtextxy( DispLeft+144, top+84, "CIE 1976");
			writeInfo( "u' = ", valbuf[2], DispLeft+144, top+98);
			writeInfo( "v' = ", valbuf[3], DispLeft+144, top+112);

			outtextxy( DispLeft+280, top+84, "CIE 1960");
			writeInfo( "u = ", valbuf[4], DispLeft+280, top+98);
			writeInfo( "v = ", valbuf[5], DispLeft+280, top+112);

			if (( oldP->RGBmode == RGBHor) || ( oldP->RGBmode == RGBVer)) {

				rectangle( DispLeft+4, top+137, DispRight-4, top+166);

				writeInfo( "Selected phosphor : ",
								oldP->P.phoName, DispLeft+16, top+142);

				writeInfo( "Learned by        : ",
								oldP->P.KuNo, DispLeft+16, top+156);
			}
			addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

			for ( i=0; i<3; i++)
				drawButton( &bu[i], INACTIVE);

			drawButton( &bu[group], ACTIVE);

			a_inner = -1;

			while ( a_inner) {
				switch ( ch = GetKey()) {
					case kbTab:
					case kbShiftTab:
						drawButton( &bu[group], INACTIVE);

						if ( ch == kbTab) {
							if ( ++group > 2)
								group = 0;
						}
						else {
							if ( --group < 0)
								group = 2;
						}

						drawButton( &bu[group], ACTIVE);
						break;

					case kbF1:
						openHelp( HlpEditStatus);
						a_inner = 0;
						break;

					case kbEnter:
						if ( group == 2) {
							openHelp( HlpEditStatus);
							a_inner = 0;
							break;
						}												/* break IS missing */
					case kbEsc:
						a_inner = alive = 0;
						break;
				}
			}
		}
		closeWindow();

		return( 1);
	}
	return( displayError( 0));
}


/* do nothing */

int idle( void) {
	return( 0);
}


/* get a key */

int GetKey( void) {

	int ch;

	if ( bioskey( 1)) {
		ch = bioskey( 0);

		switch ( bioskey( 2) & 0x0003) {

			case kbRightShift:
			case kbLeftShift:
				if ( ch & 0xFF)
					return( ch);
				else
					return( ch | 0x01);

			default:
				return( ch);
		}
	}
	return( 0);
}


int NumberTrap( int c) {

	c = toupper( c);
	if ( isdigit( c) || ( c == '.'))
		return( c);

	return( 0);
}

