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

#include "display.h"

const char* CIEText[] = {
	"x", "y", "ëx", "ëy", "u'", "v'", "ëu'", "ëv'", "u", "v", "ëu", "ëv"
};

const char* RGBText[] = {
	"XYZ / X               ",
	"XYZ / Y               ",
	"XYZ / Z               ",
	"XYZ / Luminance       ",
	"XYZ / ",
	"XYZ / Measurement     ",

	"RGB / R               ",
	"RGB / G               ",
	"RGB / B               ",
	"RGB / Luminance       ",
	"RGB / ",
	"RGB / Measurement     "
};

const char* LumText[] = {
	"Cd/mý", "NIT", "ftL"
};

const char* ColorErrText[] = {
	"CIELUV", "JND"
};

const char* ZoomRGBText[] = {
	"0%", "200%", "75%", "125%", "90%", "110%"
};

const char* HeadText[] = {
	"CIE 1931 coordinates", "CIE 1976 coordinates", "CIE 1960 coordinates"
};

const char* WhrText[] = {
	"x - coordinate", "y - coordinate",
	"u'- coordinate", "v'- coordinate",
	"u - coordinate", "v - coordinate"
};


/* Some static global variables, ( only known in this module), to speed */
/* things up, especially in the update-routines */

static int xstart, xend;
static int ystart;

static long xmin, x0, xmax, xtmp;
static long ymin, y0, ymax, ytmp;
static long xlen, ylen;

static char valbuf[5][8];
static char headline[23];

static float x1, y1, x2, y2;
static float d1, d2;
static float tmp1, tmp2;
static float uval, vval;
static float CorTemp;
static float deviation;

static float basis;
static float Xval, Yval, Zval;
static float Xbar, Ybar, Zbar;

static int XColor, YColor, ZColor;

/* Table for correlated colour temperature */
/* 	based on calculations from: P.Frederiksen, P-DPE */

static float TT[MaxTable][5] = {
/*	    1/øK       u        v      slope   sqrt( 1+slope*slope) */
	{ 1/16667., 0.18494, 0.28020, -0.3515, 1.059977},
	{ 1/14286., 0.18611, 0.28340, -0.3790, 1.069412},
	{ 1/12500., 0.18739, 0.28666, -0.4094, 1.080559},
	{ 1/11111., 0.18879, 0.28995, -0.4426, 1.093570},
	{ 1/10000., 0.19031, 0.29325, -0.4787, 1.108672},
	{  1/8000., 0.19461, 0.30139, -0.5817, 1.156882},
	{  1/6667., 0.19960, 0.30918, -0.7043, 1.223127},
	{  1/5714., 0.20523, 0.31645, -0.8484, 1.311405},
	{  1/5000., 0.21140, 0.32309, -1.017, 1.426285},
	{  1/4444., 0.21804, 0.32906, -1.216, 1.574375},
	{  1/4000., 0.22507, 0.33436, -1.450, 1.761391},
	{  1/3636., 0.23243, 0.33901, -1.728, 1.996493},
	{  1/3333., 0.24005, 0.34305, -2.061, 2.290790},
	{  1/3077., 0.24787, 0.34653, -2.465, 2.660117},
	{  1/2857., 0.25585, 0.34948, -2.960, 3.124356},
	{  1/2667., 0.26394, 0.35198, -3.576, 3.713189},
	{  1/2500., 0.27210, 0.35405, -4.355, 4.468336},
	{  1/2353., 0.28032, 0.35575, -5.365, 5.457401},
	{  1/2222., 0.28854, 0.35713, -6.711, 6.785096},
	{  1/2105., 0.29676, 0.35822, -8.572, 8.630132},
	{  1/2000., 0.30496, 0.35906, -11.29, 11.334200},
	{  1/1905., 0.31310, 0.35968, -15.56, 15.592101},
	{  1/1818., 0.32119, 0.36011, -23.20, 23.221542},
};


void updateccT( WININFO* newP) {

	int KLine = 12, TempFound = 0;

	uval = newP->meas.whr[4];
	vval = newP->meas.whr[5];

	do {								/* Calculate correlated color-temperature */
		d1 = (( vval-TT[KLine+1][2])-TT[KLine+1][3]*(uval-TT[KLine+1][1]))/
				TT[KLine+1][4];

		tmp1 = vval - TT[KLine][2];
		tmp2 = uval - TT[KLine][1];

		d2 = ( tmp1 - tmp2*TT[KLine][3])/TT[KLine][4];

		if (( d1 > 0) && ( d2 > 0))
			KLine++;
		else
			if (( d1 < 0) && ( d2 < 0))
				KLine--;
		else
			TempFound = 0xFF;

	} while (( KLine >= 0) && ( KLine < ( MaxTable-1)) && !TempFound);

	CorTemp = 0.0;

	if ( TempFound) {

		d1 = fabs( d1);
		d2 = fabs( d2);

		if ( sqrt( tmp1*tmp1 + tmp2*tmp2) <= 0.0385)
			CorTemp = 1/( TT[KLine+1][0] +
						( d1*( TT[KLine][0] - TT[KLine+1][0])/( d1+d2)));
	}

	newP->ccT = CorTemp;
}


void updateCIE( int n) {

	WININFO* newP = WinPtr[n];

	register int i, j;
	int TextOn = INACTIVE;

	setcolor( CIETextColor);
	setfillstyle( SOLID_FILL, EGA_LIGHTGRAY);
	setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

	xstart = newP->rect.left + CIEframe + 40;
	xend = xstart + 64;
	ystart = newP->rect.bottom - WINDOWBARDEEP - 71;

	i = newP->DispMode & 0xFFFE;
	x1 = newP->meas.whr[i];
	x2 = newP->whrinfo.whr[i];
	i++;
	y1 = newP->meas.whr[i];
	y2 = newP->whrinfo.whr[i];

	if ( newP->texton && ( newP->width > CIEWidth) &&
		( newP->height > CIELength))
		TextOn = ACTIVE;

	if ( newP->meas.IOerror) {
		if ( !newP->IOError) {

			newP->IOError = newP->meas.IOerror;

			if ( TextOn)
				for ( j=12, i=1; i < 6; i++, j += 12)
					bar( xstart-2, ystart-2+j, xstart+64, ystart+8+j);

			putimage( newP->crossx - newP->xlen, newP->crossy - newP->ylen,
													CIECrossImage[n], COPY_PUT);

			i = ( newP->rect.left + newP->rect.right)/2 - 72;
			j = ( newP->rect.top + newP->rect.bottom)/2 - 20;

			switch ( newP->meas.IOerror) {
				case 11:
					free( displayMessage( i, j, RED, "Overload"));
					break;
				case 10:
				case 12:
					free( displayMessage( i, j, RED, "Lowlight"));
					break;
			}
		}
		return;
	}

	if ( newP->IOError) {
		newP->IOError = 0;
		drawDisplay( n);
		return;
	}

	if ( TextOn) {
		switch ( newP->colorerror) {
			case CIELUV:
				d1 = newP->meas.whr[2] - newP->whrinfo.whr[2];
				d2 = newP->meas.whr[3] - newP->whrinfo.whr[3];

				deviation =1300. * sqrt( d1*d1 + d2*d2);
				break;

			case JND:
				d1 = newP->meas.whr[4] - newP->whrinfo.whr[4];
				d2 = newP->meas.whr[5] - newP->whrinfo.whr[5];

				deviation = sqrt( d1*d1 + d2*d2) / 0.00384;
				break;
		}

		sprintf( valbuf[3], "%5.1f", deviation);

		switch ( newP->DispMode) {
			case xy:
			case u_v_:
			case uv:
				sprintf( valbuf[0], "%4.3f", x1);
				sprintf( valbuf[1], "%4.3f", y1);
				break;

			default:
				sprintf( valbuf[0], "%+4.3f", x1 - x2);
				sprintf( valbuf[1], "%+4.3f", y1 - y2);
				break;
		}

		switch ( newP->lumunit) {
			case ftL:
				sprintf( valbuf[2], "%5.2f", newP->meas.Y/ftLfactor);
				break;

			default:
				sprintf( valbuf[2], "%5.1f", newP->meas.Y);
				break;
		}

		if ( newP->meas.Y < 0.1)
			strcpy( valbuf[2], "< 0.1");

		updateccT( newP);

		if ( newP->ccT > 0.1)
			sprintf( valbuf[4], "%5.1f", newP->ccT);
		else
			strcpy( valbuf[4], "-----");

		for ( j = ystart, i=0; i<5; i++, j += 12) {
			bar( xstart-2, j + 10, xend, j + 20);
			outtextxy( xstart, j + 12, valbuf[i]);
		}
	}

	putimage( newP->crossx - newP->xlen, newP->crossy - newP->ylen,
														CIECrossImage[n], COPY_PUT);
	newP->dx = x1 =
			((int)(( 1000*x1 + 0.5)) - (int)(( 1000*x2 + 0.5)))*newP->xscale;
	newP->dy = y1 =
			((int)(( 1000*y1 + 0.5)) - (int)(( 1000*y2 + 0.5)))*newP->yscale;

	xmin = abs( xmax = newP->dx + newP->targetx);
	ymin = abs( newP->dy + newP->targety);

	if (( xmin >= newP->kvad.left) &&
		 ( xmin <= newP->kvad.right) &&
		 ( ymin >= newP->kvad.top) &&
		 ( ymin <= newP->kvad.bottom)) {

		newP->crossx = xmax;
		newP->crossy = newP->targety - newP->dy;

		xmin = newP->crossx - newP->xlen;
		xmax = newP->crossx + newP->xlen;

		ymin = newP->crossy - newP->ylen;
		ymax = newP->crossy + newP->ylen;

		getimage( xmin, ymin, xmax, ymax, CIECrossImage[n]);

		switch ( newP->DispMode) {
			case xy:
			case u_v_:
			case uv:
				line( xmin, newP->crossy, xmax, newP->crossy);
				line( newP->crossx, ymin, newP->crossx, ymax);
				break;

			default:
				line( xmin, ymin, xmax, ymax);
				line( xmin, ymax, xmax, ymin);
				break;
		}
	}
}


void writeCIE( int n) {

	WININFO* newP = WinPtr[n];
	int i, j;

	setcolor( CIETextColor);
	setfillstyle( SOLID_FILL, EGA_LIGHTGRAY);

	xstart = newP->rect.left + CIEframe;
	ystart = newP->rect.bottom - WINDOWBARDEEP - 71;

	for ( j = ystart+12, i = 1; i<7; i++, j += 12)
		bar( xstart-2, j-2, xstart+160, j+8);

	i = newP->DispMode << 1;

	outtextxy( xstart, ystart+12, CIEText[i]);
	outtextxy( xstart, ystart+24, CIEText[i+1]);

	moveto( xstart, ystart+36);
	outtext( "Y");
	moverel( 104, 0);
	outtext( LumText[newP->lumunit]);

	moveto( xstart, ystart+48);
	outtext( "dE");
	moverel( 96, 0);
	outtext( ColorErrText[newP->colorerror]);

	moveto( xstart, ystart+60);
	outtext( "ccT");
	moverel( 88, 0);
	outtext( "Kelvin");

	moveto( xstart, ystart+72);
	outtext( "Ref: ");
	outtext( newP->whrinfo.name);

	moveto( xstart+24, ystart+12);

	for ( i=1; i<6; i++) {
		outtext( "=");
		moverel( -8, 12);
	}
}


void drawCIE( int n) {

	WININFO* newP = WinPtr[n];

	float tmp, a1, b1;
	int i, redpos[2], greenpos[2], bluepos[2];

	itoa( newP->WinNo, headline, 10);
	strcat( headline, ": ");

	if (( newP->mode == CIEMeasMode) || newP->windowname[0])
		strcat( headline, newP->windowname);
	else
		strcat( headline, "CIE");

	openWindow( newP, headline);

	newP->width = xlen = newP->rect.right - newP->rect.left;
	newP->height = ylen = newP->rect.bottom - newP->rect.top - WINDOWBARDEEP;

	if (( newP->texton) && ( xlen > CIEWidth) && ( ylen > CIELength)){
		writeCIE( n);
		ylen -= 70;
	}

	setcolor( EGA_BLACK);
	setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

	( ylen < ( 6*xlen/8)) ?	( xlen = 4.*ylen/3.) : ( ylen = 3.*xlen/4.);

	xlen -= 2*CIEframe;
	ylen -= 2*CIEframe;

	if (( tmp = xlen/100.) < 1)
		tmp = 1;

	newP->xlen = 4*tmp;
	newP->ylen = 3*tmp;

	ymax = newP->rect.top + WINDOWBARDEEP + CIEframe;

	switch ( newP->DispMode) {
		case xy:
		case u_v_:
		case uv:
			ymin = ymax + ylen;

			xmin = newP->rect.left + CIEframe;
			xmax = xmin + xlen;

			x0 = xmin + ( xlen >> 1);
			y0 = ymin - ( ylen >> 1);

			line( xmin, ymin, xmin, ymax);
			line( xmin, ymin, xmax, ymin);

			a1 = newP->xlen;
			b1 = newP->ylen;

			rectangle( x0-a1, y0-b1, x0+a1, y0+b1);

			i = newP->DispMode << 1;

			outtextxy( xmax+5, ymin-4, CIEText[i]);
			outtextxy( xmin-3, ymax-12, CIEText[i+1]);
			break;

		default:
			ymin = ylen >> 1;
			y0 = ymax + ymin;
			ymin += y0;

			xmin = newP->rect.left;

			if ( newP->texton)
				xmin += CIEframe;
			else
				xmin += CIEframe/2.;

			xmax = xlen >> 1;
			x0 = xmin + xmax;
			xmax += x0;

			line( xmin, y0, xmax, y0);
			line( x0, ymin, x0, ymax);

			outtextxy( xmax+5, y0-4, CIEText[( newP->DispMode << 1)]);
			outtextxy( x0-6, ymax-12, CIEText[( newP->DispMode << 1)+1]);
			break;
	}

	if ( newP->DispMode == xy) {
		redpos[0] = xmin + 0.98*( xmax - xmin);
		redpos[1] = y0 - 0.1*( y0 - ymax);
		greenpos[0] = xmin + 0.9*( x0 - xmin);
		greenpos[1] = ymax;
		bluepos[0] = xmin + 0.25*( x0 - xmin);
		bluepos[1] = ymin - 0.3*( ymin - y0 + 10);
	}
	else {
		redpos[0] = xmax;
		redpos[1] = y0 - 0.25*( y0 - ymax);
		greenpos[0] = xmin + 0.28*( x0 - xmin);
		greenpos[1] = ymax + 0.09*( y0 - ymax);
		bluepos[0] = x0 - 0.1*( x0 - xmin);
		bluepos[1] = ymin - 0.2*( ymin - y0 + 30);
	}

	if ( xlen > 110)
		settextstyle( DEFAULT_FONT, HORIZ_DIR, 2);

	setcolor( EGA_RED);
	outtextxy( redpos[0], redpos[1], "R");

	setcolor( EGA_GREEN);
	outtextxy( greenpos[0], greenpos[1], "G");

	setcolor( EGA_BLUE);
	outtextxy( bluepos[0], bluepos[1], "B");

	setcolor( EGA_BLACK);
	settextstyle( DEFAULT_FONT, HORIZ_DIR, 1);

	newP->crossx = newP->targetx = x0;
	newP->crossy = newP->targety = y0;

	newP->kvad.left = xmin;
	newP->kvad.top = ymax;
	newP->kvad.right = xmax;
	newP->kvad.bottom = ymin;

	outtextxy( x0-16, ymin+9, "0.05");

	switch ( newP->scalemeas) {			/* x- & y-scale are X dots per 0.05 */
		case LowGain:
			tmp = xlen/25.;

			newP->xscale = 0.8*xlen;
			newP->yscale = 0.8*ylen;

			a1 = 12.*tmp;
			b1 = a1 + tmp;

			line( xmin+a1, ymin+6, xmin+b1, ymin+6);

			line( xmin+a1, ymin+3, xmin+a1, ymin+6);
			line( xmin+b1, ymin+3, xmin+b1, ymin+6);
			break;

		case MedGain:
			tmp = xlen/5.;

			newP->xscale = 4.*xlen;
			newP->yscale = 4.*ylen;

			a1 = 2.*tmp;
			b1 = a1 + tmp;

			line( xmin+a1, ymin+6, xmin+b1, ymin+6);

			line( xmin+a1, ymin+3, xmin+a1, ymin+6);
			line( xmin+b1, ymin+3, xmin+b1, ymin+6);
			break;

		case HighGain:
			tmp = xlen/5.;

			newP->xscale = 20.*xlen;
			newP->yscale = 20.*ylen;

			for ( a1 = tmp, i=1; i<5; i++, a1 += tmp)
				line( xmin+a1, ymin+3, xmin+a1, ymin+6);

			line( xmin, ymin+6, xmax, ymin+6);

			line( xmin, ymin+3, xmin, ymin+9);
			line( xmax, ymin+3, xmax, ymin+9);
			break;
	}

	newP->xscale /= 1000.;
	newP->yscale /= 1000.;

	free( CIECrossImage[n]);

	xmin = x0 - newP->xlen;
	xmax = x0 + newP->xlen;

	ymin = y0 - newP->ylen;
	ymax = y0 + newP->ylen;

	CIECrossSize[n] = imagesize( xmin, ymin, xmax, ymax);
	CIECrossImage[n] = malloc( CIECrossSize[n]);

	getimage( xmin, ymin, xmax, ymax, CIECrossImage[n]);
}


void CalcRGBReference( int n) {

	WININFO* newP = WinPtr[n];
	float X, Y, Z, sumXYZ;

	if ( newP->DispMode != refM) {

		/* Find X, Y, Z from 90.0 NIT and White reference */

		Y = 90.0;
		sumXYZ = Y / newP->whrinfo.whr[1];
		X = newP->whrinfo.whr[0] * sumXYZ;
		Z = sumXYZ - X - Y;
	}

	if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == RGBVer)) {
		newP->Xref = newP->P.pho[0]*X + newP->P.pho[1]*Y + newP->P.pho[2]*Z;
		newP->Yref = newP->P.pho[3]*X + newP->P.pho[4]*Y + newP->P.pho[5]*Z;
		newP->Zref = newP->P.pho[6]*X + newP->P.pho[7]*Y + newP->P.pho[8]*Z;
	}
	else {
		newP->Xref = X;
		newP->Yref = Y;
		newP->Zref = Z;
	}
}


int drawRGBBar( WININFO* newP, float val, int oldval, int i, int color) {

	int fillcolor;

	xmin = newP->minimum;
	xmax = newP->maximum;

	xtmp = ((( float)( xmax - xmin)) / newP->xscale * ( val - newP->yscale) +
									( float) xmin + 0.5);

	if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == XYZHor)) {

		xtmp = max( xtmp, xmin);
		xtmp = min( xtmp, xmax);

		if ( xtmp < oldval)
			fillcolor = WindowBkColor;
		else
			fillcolor = color;

		setfillstyle( SOLID_FILL, fillcolor);

		if ( xtmp != oldval)
			bar( oldval, newP->w[i], xtmp, newP->w[i+1] );
	}
	else {
		xtmp = min( xtmp, xmin);
		xtmp = max( xtmp, xmax);

		if ( xtmp > oldval)
			fillcolor = WindowBkColor;
		else
			fillcolor = color;

		setfillstyle( SOLID_FILL, fillcolor);

		if ( xtmp != oldval)
			bar( newP->w[i], oldval, newP->w[i+1], xtmp);
	}
	return( xtmp);
}


void updateRGB( int n) {

	WININFO* newP = WinPtr[n];
	int xpos, ypos, TextOn=INACTIVE;
	char* txtptr;

	setcolor( RGBTextColor);
	setfillstyle( SOLID_FILL, EGA_LIGHTGRAY);

	if (( newP->RGBmode == RGBVer) || ( newP->RGBmode == XYZVer)) {
		xstart = newP->rect.right - 108;
		ystart = ( newP->rect.bottom+newP->rect.top+WINDOWBARDEEP)/2 - 45;
	}
	else {
		xstart = newP->rect.left + RGBframe + 64;
		ystart = newP->rect.bottom - WINDOWBARDEEP - 36;
	}

	if (( newP->texton) && ( newP->height > RGBLength) &&
		 ( newP->width > RGBWidth))
		TextOn = ACTIVE;

	if ( newP->meas.IOerror) {
		if ( !newP->IOError) {
			if ( TextOn)
				bar( xstart+14, ystart-2, xstart+62, ystart+8);

			newP->IOError = newP->meas.IOerror;

			Xbar = Ybar = Zbar = 0;

			if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == RGBVer)) {
				XColor = EGA_RED;
				YColor = EGA_GREEN;
				ZColor = EGA_BLUE;
			}
			else
				XColor = YColor = ZColor = EGA_BLACK;

			newP->Xbar = drawRGBBar( newP, Xbar, newP->Xbar, 0, XColor);
			newP->Ybar = drawRGBBar( newP, Ybar, newP->Ybar, 2, YColor);
			newP->Zbar = drawRGBBar( newP, Zbar, newP->Zbar, 4, ZColor);

			xpos = ( newP->rect.left + newP->rect.right)/2 - 72;
			ypos = ( newP->rect.top + newP->rect.bottom)/2 - 20;

			switch ( newP->meas.IOerror) {
				case 11:
					txtptr = "Overload";
					break;
				case 10:
				case 12:
					txtptr = "Lowlight";
					break;
			}
			free( displayMessage( xpos, ypos, RED, txtptr));
		}
		return;
	}

	if ( newP->IOError) {
		newP->IOError = 0;
		drawDisplay( n);
		return;
	}

	if ( TextOn) {

		if ( newP->meas.Y < 0.1)
			strcpy( valbuf[0], "< 0.1");
		else {
			if ( newP->lumunit == ftL)
				sprintf( valbuf[0], "%5.2f", newP->meas.Y/ftLfactor);
			else
				sprintf( valbuf[0], "%5.1f", newP->meas.Y);
		}

		bar( xstart+14, ystart-2, xstart+62, ystart+8);
		outtextxy( xstart+16, ystart, valbuf[0]);
	}

	Xval = newP->meas.X;
	Yval = newP->meas.Y;
	Zval = newP->meas.Z;

	if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == RGBVer)) {

		Xbar = newP->P.pho[0]*Xval+newP->P.pho[1]*Yval+newP->P.pho[2]*Zval;
		Ybar = newP->P.pho[3]*Xval+newP->P.pho[4]*Yval+newP->P.pho[5]*Zval;
		Zbar = newP->P.pho[6]*Xval+newP->P.pho[7]*Yval+newP->P.pho[8]*Zval;

		Xval = Xbar;
		Yval = Ybar;
		Zval = Zbar;
	}

	switch ( newP->DispMode) {
		case refX:
			basis = newP->Xref / Xval;

			Xbar = 1.0;
			Ybar = Yval / newP->Yref * basis;
			Zbar = Zval / newP->Zref * basis;

			break;

		case refY:
			basis = newP->Yref / Yval;

			Xbar = Xval / newP->Xref * basis;
			Ybar = 1.0;
			Zbar = Zval / newP->Zref * basis;
			break;

		case refZ:
			basis = newP->Zref / Zval;

			Xbar = Xval / newP->Xref * basis;
			Ybar = Yval / newP->Yref * basis;
			Zbar = 1.0;
			break;

		default:
			switch ( newP->DispMode) {
				case refL:
					basis = 90.0 / newP->meas.Y;
					break;

				case refA:
					basis = 90.0 / newP->ABSref;
					break;

				case refM:
					break;
			}

			Xbar = ( Xval / newP->Xref) * basis;
			Ybar = ( Yval / newP->Yref) * basis;
			Zbar = ( Zval / newP->Zref) * basis;
			break;
	}
	newP->Xval = Xbar;
	newP->Yval = Ybar;
	newP->Zval = Zbar;

	if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == RGBVer)) {
		XColor = EGA_RED;
		YColor = EGA_GREEN;
		ZColor = EGA_BLUE;
	}
	else
		XColor = YColor = ZColor = EGA_BLACK;

	newP->Xbar = drawRGBBar( newP, Xbar, newP->Xbar, 0, XColor);
	newP->Ybar = drawRGBBar( newP, Ybar, newP->Ybar, 2, YColor);
	newP->Zbar = drawRGBBar( newP, Zbar, newP->Zbar, 4, ZColor);

	updateccT( newP);
}


void writeRGB( int n) {

	WININFO* newP = WinPtr[n];

	int i, tmp = newP->DispMode + 6*( newP->RGBmode/2);
	float abs_rgb = newP->ABSref;

	setcolor( RGBTextColor);
	setfillstyle( SOLID_FILL, EGA_LIGHTGRAY);

	if ( newP->DispMode == refA) {

		if ( newP->lumunit == ftL)
			abs_rgb /= ftLfactor;

		if ( abs_rgb > 99.95)
			sprintf( valbuf[0], "%3.0f", abs_rgb);
		else
			if ( abs_rgb > 9.95)
				sprintf( valbuf[0], "%3.1f", abs_rgb);
		else
			sprintf( valbuf[0], "%2.2f", abs_rgb);
	}

	ystart = newP->rect.bottom;

	if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == XYZHor)) {
		xstart = newP->rect.left + RGBframe - 2;
		ystart -= WINDOWBARDEEP + 48;

		xend = xstart + 172;

		bar( xstart, ystart+10, xend, ystart+20);
		bar( xstart, ystart+22, xend, ystart+32);
		bar( xstart, ystart+34, xend, ystart+44);

		xstart += 2;

		outtextxy( xstart, ystart+12, "Lum : Y = ");

		i = xstart + 45 + ( strlen( RGBText[4]) << 3) + 4 + 36;
		outtextxy( i, ystart+12, LumText[newP->lumunit]);

		moveto( xstart, ystart+24);
		outtext( "Mode: ");
		outtext( RGBText[tmp]);

		moveto( xstart, ystart+36);
		outtext( "Ref : ");
		outtext( newP->whrinfo.name);

		if ( newP->DispMode == refA) {
			i = xstart + 45 + ( strlen( RGBText[tmp]) << 3) + 4;

			outtextxy( i, ystart+24, valbuf[0]);
			outtextxy( i + 36, ystart+24, LumText[newP->lumunit]);
		}

		if ( newP->RGBmode == RGBHor) {
			bar( xstart-2, ystart+46, xend, ystart+56);

			moveto( xstart, ystart+48);
			outtext( "Phos: ");
			outtext( newP->P.phoName);
		}
	}
	else {
		ystart = ( newP->rect.bottom+newP->rect.top+WINDOWBARDEEP)/2 - 45;
		xstart = newP->rect.right - 130;
		xend = xstart + 127;

		bar( xstart, ystart-2, xend, ystart+8);
		bar( xstart, ystart+22, xend, ystart+32);
		bar( xstart, ystart+46, xend, ystart+56);

		xstart += 2;

		outtextxy( xstart, ystart-12, "Luminance:");

		outtextxy( xstart, ystart, "Y =");
		outtextxy( xstart+88, ystart, LumText[newP->lumunit]);

		outtextxy( xstart, ystart+12, "Mode: ");
		outtextxy( xstart, ystart+24, RGBText[tmp]);

		if ( newP->DispMode == refA) {

			outtextxy( xstart+( strlen( RGBText[tmp]) << 3)+4,
																		ystart+24, valbuf[0]);
			outtextxy( xstart+88, ystart+24, LumText[newP->lumunit]);
		}

		outtextxy( xstart, ystart+36, "Ref : ");
		outtextxy( xstart, ystart+48, newP->whrinfo.name);

		if ( newP->RGBmode == RGBVer) {
			bar( xstart-2, ystart+70, xend, ystart+80);

			outtextxy( xstart, ystart+60, "Phos:");
			outtextxy( xstart, ystart+72, newP->P.phoName);
		}
	}
}


void drawRGB( int n) {

	WININFO* newP = WinPtr[n];

	int i, j, l, charsize = 8;
	float tmp;

	itoa( newP->WinNo, headline, 10);
	strcat( headline, ": ");

	if (( newP->mode == RGBMeasMode) || newP->windowname[0])
		strcat( headline, newP->windowname);
	else
		strcat( headline, "RGB/XYZ");

	openWindow( newP, headline);

	x0 = newP->rect.left;
	ymax = newP->rect.top + WINDOWBARDEEP;

	xlen = newP->rect.right - x0;
	ylen = newP->rect.bottom - ymax;

	if (( newP->RGBmode == RGBHor) || ( newP->RGBmode == XYZHor)) {
		newP->width = xlen;
		newP->height = ylen -= 16;

		if (( newP->texton) && ( ylen > RGBLength) && ( xlen > RGBWidth)) {
			ylen -= 36;
			writeRGB( n);
		}
		tmp = ylen/6.;

		setcolor( EGA_BLACK);
		setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

		x0 += 2*RGBframe;
		ymax += max( 10, tmp/2.);

		xmax = x0 + ( int) xlen - 4*RGBframe;

		if ( tmp >= 15) {
			if ( tmp < 30)
				charsize = 16;
			else
				charsize = 24;
		}
		xtmp = x0+charsize;

		i = newP->scalemeas << 1;

		outtextxy( xtmp, ymax - 8, ZoomRGBText[i]);
		outtextxy( xmax-24, ymax - 8, ZoomRGBText[i+1]);

		for ( i=1; i<10; i++) {
			l = xtmp + i*( xmax-xtmp)/10.;

			for ( j=1; j<6; j += 2) {
				rectangle( xtmp, ymax + (j-1)*tmp, xmax, ymax + j*tmp);
				line( l, ymax + j*tmp, l, ymax + j*tmp + 4);
			}
		}

		for ( i=0; i<6; i++)
			newP->w[i] = ymax + i*tmp;

		for ( j=i=0; i<3; i++, j = i << 1) {
			newP->w[j]++;
			newP->w[j+1]--;
		}

		newP->minimum = xtmp+1;
		newP->maximum = xmax-1;

		l = ( xmax + xtmp) >> 1;
		line( l, ymax + tmp, l, ymax + 2.*tmp);
		line( l, ymax + 3.*tmp, l, ymax + 4.*tmp);

		xtmp = x0 - RGBframe;
		ytmp = ymax + ( tmp-charsize)/2. + 1;

		i = 0;
		j = 1;
	}
	else {
		newP->width = xlen -= 24;
		newP->height = ylen;

		if (( newP->texton) && ( ylen > RGBLength) && ( xlen > RGBWidth)) {

			xlen -= 16*8;
			writeRGB( n);
		}
		tmp = xlen / 6.;

		setcolor( EGA_BLACK);
		setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

		x0 += tmp/2.;
		ymax += 2.*RGBframe;

		y0 = ymax + ( int) ylen - 4.*RGBframe;

		if ( tmp >= 15) {
			if ( tmp < 30)
				charsize = 16;
			else
				charsize = 24;
		}
		ytmp = y0 - charsize;

		i = newP->scalemeas << 1;

		outtextxy( x0 + 5*tmp + 16, ytmp-8, ZoomRGBText[i++]);
		outtextxy( x0 + 5*tmp + 8, ymax, ZoomRGBText[i]);

		for ( i=1; i<10; i++) {
			l = ytmp - i*( ytmp - ymax)/10.;

			for ( j=1; j<6; j += 2) {
				rectangle( x0 + ( j-1)*tmp, ytmp, x0 + j*tmp, ymax);
				line( x0 + j*tmp, l, x0 + j*tmp + 4, l);
			}
		}

		for ( i=0; i<6; i++)
			newP->w[i] = x0 + i*tmp;

		for ( j=i=0; i<3; i++, j = i << 1) {
			newP->w[j]++;
			newP->w[j+1]--;
		}

		newP->minimum = ytmp-1;
		newP->maximum = ymax+1;

		l = ( ymax + ytmp) >> 1;
		line( x0 + tmp, l, x0 + 2.*tmp, l);
		line( x0 + 3.*tmp, l, x0 + 4.*tmp, l);

		xtmp = x0 + ( tmp-charsize)/2. + 1;
		ytmp += RGBframe;

		i = 1;
		j = 0;
	}

	settextstyle( DEFAULT_FONT, HORIZ_DIR, charsize/8.);

	if (( newP->RGBmode == XYZHor) || ( newP->RGBmode == XYZVer)) {
		outtextxy( xtmp, ytmp, "X");

		tmp *= 2.;
		outtextxy( xtmp + i*tmp, ytmp + j*tmp, "Y");

		tmp *= 2.;
		outtextxy( xtmp + i*tmp, ytmp + j*tmp, "Z");
	}
	else {
		setcolor( EGA_RED);
		outtextxy( xtmp, ytmp, "R");

		tmp *= 2.;
		setcolor( EGA_GREEN);
		outtextxy( xtmp + i*tmp, ytmp + j*tmp, "G");

		tmp *= 2.;
		setcolor( EGA_BLUE);
		outtextxy( xtmp + i*tmp, ytmp + j*tmp, "B");
	}

	switch ( newP->scalemeas) {
		case LowGain:
			newP->xscale = 2.0;
			newP->yscale = 0.0;
			break;
		case MedGain:
			newP->xscale = 0.5;
			newP->yscale = 0.75;
			break;
		case HighGain:
			newP->xscale = 0.2;
			newP->yscale = 0.9;
			break;
	}

	newP->Xbar = newP->minimum;
	newP->Ybar = newP->minimum;
	newP->Zbar = newP->minimum;

	CalcRGBReference( n);

	setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
	settextstyle( DEFAULT_FONT, HORIZ_DIR, 1);
}


int openSetupCIE( void) {

	WININFO* oldP = WinPtr[NoWin-1];
	WININFO* newP = WinPtr[NoWin];

	CHECKBOX cb[15];
	int grpindx[6] = { 0, 2, 4, 7, 13, 15};

	/* cb: 0->1   COM port
		cb: 2->3   Display text
		cb: 4->6   Luminance unit
		cb: 7->12  CIEMode
		cb: 13->14 Color Error */

	BUTTON bu[3];
	/* bu: 0 OK
		bu: 1 Cancel
		bu: 2 Help */

	/* GROUPS:
		0=Com Port, 2=Display text, 3=Luminance unit,
		4=CIEMode, 4=Color error, 5 = Ok, 6=Cancel, 7=Help */

	int ch, i, j, tmp, group, a_inner, alive = 0xFF;

	SetRect( &newP->rect, oCLeft, oCTop, oCRight, oCBottom);

	addCheckbox( &cb[0], oCleftCol, oCtop+24, "Serial COM Port 1");
	addCheckbox( &cb[1],	oCleftCol, oCtop+36, "Serial COM Port 2");
	addCheckbox( &cb[2], oCleftCol, oCtop+82, "Text Off         ");
	addCheckbox( &cb[3],	oCleftCol, oCtop+94, "Text On          ");
	addCheckbox( &cb[4], oCleftCol, oCtop+140, "Cd/mý            ");
	addCheckbox( &cb[5], oCleftCol, oCtop+152, "NIT              ");
	addCheckbox( &cb[6], oCleftCol, oCtop+164, "ftL              ");

	addCheckbox( &cb[7],	oCrightCol, oCtop+24, "CIE 1931 xy      ");
	addCheckbox( &cb[8],	oCrightCol, oCtop+36, "CIE 1931 ëxëy    ");
	addCheckbox( &cb[9], oCrightCol, oCtop+48, "CIE 1976 u'v'    ");
	addCheckbox( &cb[10], oCrightCol, oCtop+60, "CIE 1976 ëu'ëv'  ");
	addCheckbox( &cb[11], oCrightCol, oCtop+72, "CIE 1960 uv      ");
	addCheckbox( &cb[12], oCrightCol, oCtop+84, "CIE 1960 ëuëv    ");
	addCheckbox( &cb[13], oCrightCol, oCtop+130, "CIELUV           ");
	addCheckbox( &cb[14], oCrightCol, oCtop+142, "JND              ");

	cb[grpindx[0]+oldP->sensor].select = 0xFF;
	cb[grpindx[1]+oldP->texton].select = 0xFF;
	cb[grpindx[2]+oldP->lumunit].select = 0xFF;
	cb[grpindx[3]+oldP->DispMode].select = 0xFF;
	cb[grpindx[4]+oldP->colorerror].select = 0xFF;

	addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

	group = i = 0;

	while ( alive) {
		openWindow( newP, "Setup CIE-Display");

		drawSetupRect( oCleftCol-8, oCtop+12, 2, "COM Port", oCRect);
		drawSetupRect( oCleftCol-8, oCtop+70, 2, "Display text", oCRect);
		drawSetupRect( oCleftCol-8, oCtop+128, 3, "Luminance unit", oCRect);

		drawSetupRect( oCrightCol-8, oCtop+12, 6, "Mode", oCRect);
		drawSetupRect( oCrightCol-8, oCtop+118, 2, "Color Error", oCRect);

		for ( j = 0; j < 3; j++)
			drawButton( &bu[j], INACTIVE);

		for ( j = 0; j < 15; j++)
			drawCheckbox( &cb[j], INACTIVE);

		if ( group >= 5)
			drawButton( &bu[group-5], ACTIVE);
		else {
			drawCheckbox( &cb[i], ACTIVE);
			drawButton( &bu[0], ACTIVE);
		}

		a_inner = -1;

		while ( a_inner) {
			switch ( ch = GetKey()) {

				case kbNoKey:
					break;

				case kbEsc:
					alive = a_inner = 0;
					break;

				case kbF1:
					openHelp( HlpEditSetupCIE);
					a_inner = 0;
					break;

				case kbTab:
				case kbShiftTab:
					if ( group >= 5)
						drawButton( &bu[group-5], INACTIVE);
					else {
						drawButton( &bu[0], INACTIVE);
						drawCheckbox( &cb[i], INACTIVE);
					}

					if ( ch == kbTab) {
						if ( ++group > 7)
							group = 0;
					}
					else {
						if ( --group < 0)
							group = 7;
					}

					i = grpindx[group];

					if ( group >= 5)
						drawButton( &bu[group-5], ACTIVE);
					else {
						drawButton( &bu[0], ACTIVE);
						drawCheckbox( &cb[i], ACTIVE);
					}
					break;

				case kbDown:
				case kbUp:
					if ( group < 5) {

						drawCheckbox( &cb[i], INACTIVE);

						if ( ch == kbUp) {
							if ( --i < grpindx[group])
								i = grpindx[group+1]-1;
						}
						else {
							if ( ++i >= grpindx[group+1])
								i = grpindx[group];
						}

						drawCheckbox( &cb[i], ACTIVE);
					}
					break;

				case kbSpace:
					if ( group < 5) {

						for ( j=grpindx[group]; j < grpindx[group+1]; j++) {
							cb[j].select = 0x00;
							drawCheckbox( &cb[j], INACTIVE);
						}

						cb[i].select = 0xFF;

						drawCheckbox( &cb[i], ACTIVE);
						break;
					}
					break;

				case kbEnter:
					switch ( group) {
						case 0:
						case 1:
						case 2:
						case 3:
						case 4:
							for ( j=grpindx[group]; j < grpindx[group+1]; j++) {
								cb[j].select = 0x00;
								drawCheckbox( &cb[j], INACTIVE);
							}
							cb[i].select = 0xFF;		/* break IS missing */
						case 5:							/* Ok: Save and close */
							for ( i=0; i<grpindx[5]; i++) {
								if ( cb[i].select == 0xFF) {
									if ( i < grpindx[1]) {
										if ( !COMopened[i])
											oldP->running = INACTIVE;

										oldP->sensor = i;
									}
									else
										if ( i < grpindx[2])
											oldP->texton = i-grpindx[1];
									else
										if ( i < grpindx[3])
											oldP->lumunit = i-grpindx[2];
									else
										if ( i < grpindx[4])
											oldP->DispMode = i-grpindx[3];
									else
										if ( i < grpindx[5])
											oldP->colorerror = i-grpindx[4];
								}
							}							/* break Is missing */
						case 6:						/* Cancel: Do not save and close */
							alive = a_inner = 0;
							break;
						case 7:						/* Help: Show help screen */
							openHelp( HlpEditSetupCIE);
							a_inner = 0;
							break;
					}
			}
		}
	}

	return( 0);
}


int openSetupRGB( void) {

	WININFO* oldP = WinPtr[NoWin-1];
	WININFO* newP = WinPtr[NoWin];

	CHECKBOX cb[17];

	int grpindx[6] = { 0, 2, 4, 7, 11, 16};
	/* cb: 0->1    COM port
		cb: 2->3    Display text
		cb: 4->6    Luminance unit
		cb: 7->10   Mode i.e. RGB or XYZ ( both horizontal or vertical
		cb: 11->16  Sub mode for RGB or XYZ */

	EDITFIELD ef;

	BUTTON bu[3];
	/* bu: 0 OK
		bu: 1 Cancel
		bu: 2 Help */

	/* GROUPS:
		0=Com Port, 1=Display text, 2=Luminance unit, 3=RGBMode,
		4=RGBdisp, 5=Ok, 6=Cancel, 7=Help */

	int ch, i, j, tmp, group, a_inner, alive = 0xFF;

	float abs_rgb, tmp_abs_rgb;
	int tmp_lumunit;
	char absrgb[7];

	tmp_lumunit = oldP->lumunit;
	tmp_abs_rgb = abs_rgb = oldP->ABSref;

	if ( oldP->lumunit == ftL)
		tmp_abs_rgb /= ftLfactor;

	if ( abs_rgb > 99.95)
		sprintf( absrgb, "%3.0f", tmp_abs_rgb);
	else
		if ( abs_rgb > 9.95)
			sprintf( absrgb, "%3.1f", tmp_abs_rgb);
	else
		sprintf( absrgb, "%2.2f", tmp_abs_rgb);

	SetRect( &newP->rect, oRLeft, oRTop, oRRight, oRBottom);

	tmp = 6*( oldP->RGBmode/2);

	addCheckbox( &cb[0], oRleftCol, oRtop+25, "Serial COM Port 1 ");
	addCheckbox( &cb[1],	oRleftCol, oRtop+38, "Serial COM Port 2 ");
	addCheckbox( &cb[2],	oRleftCol, oRtop+85, "Text Off          ");
	addCheckbox( &cb[3], oRleftCol, oRtop+98, "Text On           ");
	addCheckbox( &cb[4], oRleftCol, oRtop+143, "Cd/mý             ");
	addCheckbox( &cb[5], oRleftCol, oRtop+156, "NIT               ");
	addCheckbox( &cb[6], oRleftCol, oRtop+169, "ftL               ");

	addCheckbox( &cb[7], oRrightCol, oRtop+25, "XYZ - Horizontal bars ");
	addCheckbox( &cb[8], oRrightCol, oRtop+38, "XYZ - Vertical bars   ");
	addCheckbox( &cb[9], oRrightCol, oRtop+51, "RGB - Horizontal bars ");
	addCheckbox( &cb[10], oRrightCol, oRtop+64, "RGB - Vertical bars   ");

	addCheckbox( &cb[11], oRrightCol, oRtop+90, RGBText[0+tmp]);
	addCheckbox( &cb[12], oRrightCol, oRtop+103, RGBText[1+tmp]);
	addCheckbox( &cb[13], oRrightCol, oRtop+116, RGBText[2+tmp]);
	addCheckbox( &cb[14], oRrightCol, oRtop+129, RGBText[3+tmp]);
	addCheckbox( &cb[15], oRrightCol, oRtop+142, RGBText[4+tmp]);
	addCheckbox( &cb[16], oRrightCol, oRtop+155, RGBText[5+tmp]);

	addEditfield( &ef, oRrightCol+10*8, oRtop+142, 5, absrgb, NumberTrap);

	cb[grpindx[0]+oldP->sensor].select = 0xFF;
	cb[grpindx[1]+oldP->texton].select = 0xFF;
	cb[grpindx[2]+oldP->lumunit].select = 0xFF;
	cb[grpindx[3]+oldP->RGBmode].select = 0xFF;
	cb[grpindx[4]+oldP->DispMode].select = 0xFF;

	addButtons( &bu[0], &bu[1], &bu[2], &newP->rect, Ok);

	group = i = 0;

	while ( alive) {
		openWindow( newP,"Setup RGB/XYZ-Display");

		drawSetupRect( oRleftCol-8, oRtop+12, 2, "COM Port", oRRect-32);
		drawSetupRect( oRleftCol-8, oRtop+72, 2, "Display text", oRRect-32);
		drawSetupRect( oRleftCol-8, oRtop+132, 3, "Luminance unit", oRRect-32);

		drawSetupRect( oRrightCol-8, oRtop+12, 11, "Mode", oRRect);
		outtextxy( oRrightCol, oRtop+77, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");
		outtextxy( oRrightCol+16*8, oRtop+142, LumText[oldP->lumunit]);

		for ( j=0; j < 3; j++)
			drawButton( &bu[j], INACTIVE);

		for ( j=0; j < grpindx[5]; j++)
			drawCheckbox( &cb[j], INACTIVE);

		drawEditfield( &ef, INACTIVE);

		if ( group >= 5)
			drawButton( &bu[group-5], ACTIVE);
		else {
			drawCheckbox( &cb[i], ACTIVE);
			drawButton( &bu[0], ACTIVE);
		}

		a_inner = 0xFF;

		while ( a_inner) {
			switch ( ch = GetKey()) {

				case kbNoKey:
					break;

				case kbEsc:
					a_inner = alive = 0;
					break;

				case kbF1:
					openHelp( HlpEditSetupRGB);
					a_inner = 0;
					break;

				case kbTab:
				case kbShiftTab:
					if ( group >= 5)
						drawButton( &bu[group-5], INACTIVE);
					else {
						if (( i == 15) && cb[i].select) {
							if (( abs_rgb < 0.1) || ( abs_rgb > 344)) {
								displayError( 12);
								break;
							}
							drawEditfield( &ef, INACTIVE);
						}
						drawButton( &bu[0], INACTIVE);
						drawCheckbox( &cb[i], INACTIVE);
					}

					if ( ch == kbTab) {
						if ( ++group > 7)
							group = 0;
					}
					else {
						if ( --group < 0)
							group = 7;
					}

					i = grpindx[group];

					if ( group >= 5)
						drawButton( &bu[group-5], ACTIVE);
					else {
						drawButton( &bu[0], ACTIVE);
						drawCheckbox( &cb[i], ACTIVE);

						if (( i == 15) && cb[i].select)
							drawEditfield( &ef, ACTIVE);
					}
					break;

				case kbDown:
				case kbUp:
					if ( group < 5) {
						if (( i == 15) && ( cb[i].select))
							if (( abs_rgb < 0.1) || ( abs_rgb > 344)) {
								displayError( 12);
								break;
							}

						drawCheckbox( &cb[i], INACTIVE);

						if ( i == 15)
							drawEditfield( &ef, INACTIVE);

						if ( ch == kbUp) {
							if ( --i < grpindx[group])
								i = grpindx[group+1]-1;
						}
						else {
							if ( ++i >= grpindx[group+1])
								i = grpindx[group];
						}

						if (( i == 15) && cb[i].select)
							drawEditfield( &ef, ACTIVE);

						drawCheckbox( &cb[i], ACTIVE);
					}
					break;

				case kbSpace:
					if ( group == 2) {
						if (((( i-grpindx[2]) == ftL) && ( tmp_lumunit != ftL)) ||
							 ((( i-grpindx[2]) != ftL) && ( tmp_lumunit == ftL))) {

							tmp_abs_rgb = abs_rgb;

							if ((( i-grpindx[2]) == ftL) && ( tmp_lumunit != ftL))
								tmp_abs_rgb = abs_rgb/ftLfactor;

							if ( abs_rgb > 99.95)
								sprintf( absrgb, "%3.0f", tmp_abs_rgb);
							else
								if ( abs_rgb > 9.95)
									sprintf( absrgb, "%3.1f", tmp_abs_rgb);
							else
								sprintf( absrgb, "%2.2f", tmp_abs_rgb);
						}

						tmp_lumunit = i-grpindx[2];

						setcolor( EGA_BLACK);
						setfillstyle( WindowBkFill, WindowBkColor);

						bar( oRrightCol+16*8, oRtop+142, oRrightCol+21*8, oRtop+154);
						outtextxy( oRrightCol+16*8, oRtop+142,
																LumText[tmp_lumunit]);
					}

					if ( group < 5) {
						for ( j=grpindx[group]; j < grpindx[group+1]; j++) {
							cb[j].select = 0x00;
							drawCheckbox( &cb[j], INACTIVE);
						}

						cb[i].select = 0xFF;
						drawCheckbox( &cb[i], ACTIVE);

						if (( i == 15) && cb[i].select)
							drawEditfield( &ef, ACTIVE);
						else
							drawEditfield( &ef, INACTIVE);

						if (( i > 6) && ( i < 11)){
							tmp = 6*(( i - 7)/2);
							for ( j=11; j < grpindx[5]; j++) {
								cb[j].text = RGBText[j-11+tmp];
								drawCheckbox( &cb[j], INACTIVE);
							}
						}
					}
					break;

				case kbEnter:
					switch ( group) {
						case 0:
						case 1:
						case 2:
						case 3:
						case 4:
							for ( j=grpindx[group]; j < grpindx[group+1]; j++) {
								cb[j].select = 0x00;
								drawCheckbox( &cb[j], INACTIVE);
							}
							cb[i].select = 0xFF;		/* break IS missing */
						case 5:							/* Ok: Save and close */
							if ( cb[15].select) {

								if (( abs_rgb < 0.1) || ( abs_rgb > 344)) {
									displayError( 12);
									drawCheckbox( &cb[i], ACTIVE);
									break;
								}
								oldP->ABSref = abs_rgb;
							}

							for ( i=0; i<grpindx[5]; i++) {
								if ( cb[i].select == 0xFF) {
									if ( i < grpindx[1]) {
										if ( !COMopened[i])
											oldP->running = INACTIVE;

										oldP->sensor = i;
									}
									else
										if ( i < grpindx[2])
											oldP->texton = i-grpindx[1];
									else
										if ( i < grpindx[3])
											oldP->lumunit = i-grpindx[2];
									else
										if ( i < grpindx[4])
											oldP->RGBmode = i-grpindx[3];
									else
										if ( i < grpindx[5])
											oldP->DispMode = i-grpindx[4];
								}
							}							/* break IS missing */
						case 6:						/* Cancel: Do not save and close */
							alive = a_inner = 0;
							break;
						case 7:						/* Help: Show help screen */
							openHelp( HlpEditSetupRGB);
							a_inner = 0;
							break;
					}
				default:
					if (( group == 4) && ( i == 15) && cb[i].select) {
						editEditfield(  &ef, ch);
						abs_rgb = atof( absrgb);

						if ( cb[6].select)
							abs_rgb *= ftLfactor;
					}
					break;
			}
		}
	}
	return( 0);
}


int openSetup( void) {

	if ( NoWin) {
		if ( WinPtr[NoWin]->mode == CIEMode) {

			WinPtr[++NoWin]->mode = SetupMode;
			openSetupCIE();
			closeWindow();
		}
		else
			if ( WinPtr[NoWin]->mode == RGBMode) {

				WinPtr[++NoWin]->mode = SetupMode;
				openSetupRGB();
				closeWindow();
			}
		else
			displayError( 20);
	}
	else
		displayError( 0);

	return( 0);
}


int openCIE_RGB( int mode) {

	int left = 5*(( WinPtr[NoWin]->rect.left + WINDOWBARDEEP + 1)/5);
	int top = 5*(( WinPtr[NoWin]->rect.top + WINDOWBARDEEP + 5)/5);

	int right = min( left+oCRWidth, SCREENWIDE-DROPSHADOW);
	int bottom = min( top+oCRDepth, SCREENDEEP-DROPSHADOW-MANAGERBARDEEP-5);

	WININFO* newP = WinPtr[++NoWin];
	int i;

	shutdownCOMPorts();

	newP->sensor = COM1;
	newP->running = INACTIVE;
	newP->IOError = 0;

	newP->windowname[0] = 0;

	clearSensorId( newP);

	strcpy( newP->whrinfo.name, "D6500");
	for ( i=0; i<6; i++)
		newP->whrinfo.whr[i] = D6500[i];

	newP->meas.X = newP->meas.Y = newP->meas.Z = 0.0;
	newP->IOError = newP->meas.IOerror = 12;

	newP->colorerror = CIELUV;
	newP->texton = TextOn;
	newP->lumunit = Cdm2;
	newP->scalemeas = LowGain;

	switch ( mode) {
		case CIEMode:
			newP->mode = CIEMode;
			newP->DispMode = xy;
			break;

		case RGBMode:
			newP->mode = RGBMode;
			newP->DispMode = refX;
			newP->RGBmode = XYZHor;
			newP->ABSref = 90.0;

			for ( i=0; i<9; i++)
				newP->P.pho[i] = EBUphosphor[i];

			strcpy( newP->P.phoName, photxt[0]);
			strcpy( newP->P.KuNo, "KU_fty.");

			CalcRGBReference( NoWin);
			break;
	}

	if ( NoWin == 1)
		SetRect( &newP->rect, oCRLeft, oCRTop, oCRRight, oCRBottom);
	else
		SetRect( &newP->rect, left, top, right, bottom);

	drawWindows();

	return( 0);
}


int openCIE( void) {

	if ( NoWin < MAXDISPLAY)
		return( openCIE_RGB( CIEMode));

	return( displayError( 1));
}


int openRGB( void) {

	if ( NoWin < MAXDISPLAY)
		return( openCIE_RGB( RGBMode));

	return( displayError( 1));
}
