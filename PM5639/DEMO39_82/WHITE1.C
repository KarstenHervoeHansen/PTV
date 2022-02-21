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
#include <time.h>

#define kbEsc 		   0x011b
#define kbBack       0x0e08
#define kbCtrlBack   0x0e7f
#define kbShiftTab   0x0f01
#define kbTab        0x0f09
#define kbEnter      0x1c0d
#define kbAltX       0x2d00
#define kbSpace		0x3920
#define kbF1         0x3b00
#define kbF2         0x3c00
#define kbF3         0x3d00
#define kbF4         0x3e00
#define kbF5         0x3f00
#define kbF6         0x4000
#define kbF7         0x4100
#define kbF8         0x4200
#define kbF9         0x4300
#define kbF10        0x4400
#define kbHome       0x4700
#define kbUp         0x4800
#define kbShiftUp		0x4801
#define kbPgUp       0x4900
#define kbGrayMinus  0x4a2d
#define kbLeft       0x4b00
#define kbShiftLeft 	0x4b01
#define kbRight      0x4d00
#define kbShiftRight	0x4d01
#define kbGrayPlus   0x4e2b
#define kbEnd        0x4f00
#define kbDown       0x5000
#define kbShiftDown	0x5001
#define kbPgDn       0x5100
#define kbIns        0x5200
#define kbDel        0x5300
#define kbShiftF1    0x5400
#define kbShiftF2    0x5500
#define kbShiftF3    0x5600
#define kbShiftF4    0x5700
#define kbShiftF5    0x5800
#define kbShiftF6    0x5900
#define kbShiftF7    0x5a00
#define kbShiftF8    0x5b00
#define kbShiftF9    0x5c00
#define kbShiftF10   0x5d00
#define kbCtrlF1     0x5e00
#define kbCtrlF2     0x5f00
#define kbCtrlF3     0x6000
#define kbCtrlF4     0x6100
#define kbCtrlF5     0x6200
#define kbCtrlF6     0x6300
#define kbCtrlF7     0x6400
#define kbCtrlF8     0x6500
#define kbCtrlF9     0x6600
#define kbCtrlF10    0x6700
#define kbAltF1      0x6800
#define kbAltF2      0x6900
#define kbAltF3      0x6a00
#define kbAltF4      0x6b00
#define kbAltF5      0x6c00
#define kbAltF6      0x6d00
#define kbAltF7      0x6e00
#define kbAltF8      0x6f00
#define kbAltF9      0x7000
#define kbAltF10     0x7100

#define kbAltD       0x2000
#define kbAltF       0x2100
#define kbAltL       0x2600
#define kbAltS       0x1f00
#define kbAltW       0x1100
#define kbAltR       0x1300
#define kbAltH       0x2300

#define kbAlt1       0x7800
#define kbAlt2       0x7900
#define kbAlt3       0x7a00
#define kbAlt4       0x7b00
#define kbAlt5       0x7c00
#define kbAlt6       0x7d00
#define kbAlt7       0x7e00
#define kbAlt8       0x7f00
#define kbAlt9       0x8000
#define kbAlt0       0x8100
#define kbNoKey      0x0000
#define kbRightShift  0x0001
#define kbLeftShift   0x0002

#define WindowSignalType	0
#define PlugeSignalType		1
#define ColorSignalType		2
#define TestPicture 			3
#define CirclePicture 		4
#define BjarneSmallField	5
#define BjarneBigField		6

int SignalType = WindowSignalType;
struct palettetype pal;

int ScreenCenterX;
int ScreenCenterY;

int ScreenWidth;
int ScreenHeight;

int width, height;

int Xstart, Ystart;

time_t tid;

char tmptr1[30];
char tmptr2[30];


/* Graphics initialization aborted - write message and exit */

void GraphAbort( char* Msg, int Err) {

	printf( "%s%s%s\n", Msg, ': ', grapherrormsg( Err));
	exit( EXIT_FAILURE);
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

/* Turn on graphics */

int ShowStartUpScreen( void) {

	int GrDriver = EGA;
	int GrMode = EGAHI;
	int ErrCode;
	extern int LogoTimeoutON;

	if ( registerbgidriver( EGAVGA_driver) < 0)
		GraphAbort( "EGA/VGA", graphresult());

	if ( registerbgifont( small_font) < 0 )
		GraphAbort( "SmallFont", graphresult());

	if ( registerbgifont( sansserif_font) < 0)
		GraphAbort( "SansSerifFont", graphresult());

	initgraph( &GrDriver, &GrMode, "");

	if (( ErrCode = graphresult()) != grOk) {
		restorecrtmode();
		GraphAbort( "Init graphics error", ErrCode);
	}

	LogoTimeoutON = 0x00;

	StartUpScreen( "Monitor setup software, (PRELIMINARY).", "(VGA monitors only).",
						"Philips TV Test Equipment A/S Denmark", "1994");

	closegraph();

	return( -1);
}

int initGraphics( void) {

	int GrDriver = VGA;
	int GrMode = VGAHI;
	int ErrCode;

	if ( registerbgidriver( EGAVGA_driver) < 0)
		GraphAbort( "EGA/VGA", graphresult());

	if ( registerbgifont( small_font) < 0 )
		GraphAbort( "SmallFont", graphresult());

	if ( registerbgifont( sansserif_font) < 0)
		GraphAbort( "SansSerifFont", graphresult());

	initgraph( &GrDriver, &GrMode, "");

	if (( ErrCode = graphresult()) != grOk) {
		restorecrtmode();
		GraphAbort( "Init graphics error", ErrCode);
	}

	ScreenCenterX = getmaxx()/2;
	ScreenCenterY = getmaxy()/2;

	ScreenWidth = getmaxx();
	ScreenHeight = getmaxy();

	width = ScreenWidth/18.2;
	height = ScreenHeight/13.66667;

	Xstart = ( ScreenWidth - 17*width)/2.;
	Ystart = ( ScreenHeight - 13*height)/2.;


	return( -1);
}


void WindowSignal( int old_grey_number, int grey_number) {

	float j;

	setfillstyle( SOLID_FILL, BLACK);
	bar((( old_grey_number*(59/14.)+37)/132.)*ScreenWidth, (21/242.)*ScreenHeight,
			(( old_grey_number*(59/14.)+37+1)/132.)*ScreenWidth, (22/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, grey_number);
	bar(( 37/132.)*ScreenWidth, (68/242.)*ScreenHeight,
			(97/132.)*ScreenWidth, (168/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, WHITE);
	bar( (37/132.)*ScreenWidth, (15/242.)*ScreenHeight,
			(97/132.)*ScreenWidth, (16/242.)*ScreenHeight);

	for (j=37; j<100; j+=59/7.)
		bar((j/132.)*ScreenWidth, (17/242.)*ScreenHeight,
				((j+1)/132.)*ScreenWidth, (18/242.)*ScreenHeight);

	bar(((grey_number*(59/14.)+37)/132.)*ScreenWidth, (21/242.)*ScreenHeight,
			((grey_number*(59/14.)+37+1)/132.)*ScreenWidth, (22/242.)*ScreenHeight);
}


void PlugeSignal( void) {

	float j;


	cleardevice();
	setfillstyle( SOLID_FILL, 1);
	bar( 0, 0, ScreenWidth, ScreenHeight);

	setfillstyle( SOLID_FILL, 0);
	bar( (26/132.)*ScreenWidth, (199/242.)*ScreenHeight,
				(32/132.)*ScreenWidth, (40/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, 2);
	bar( (38/132.)*ScreenWidth, (199/242.)*ScreenHeight,
				(43/132.)*ScreenWidth, (40/242.)*ScreenHeight);


	setfillstyle( SOLID_FILL, 15);
	bar( (73/132.)*ScreenWidth, (40/242.)*ScreenHeight,
				(101/132.)*ScreenWidth, (79/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, 10);
	bar( (73/132.)*ScreenWidth, (79/242.)*ScreenHeight,
				(101/132.)*ScreenWidth, (119/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, 6);
	bar( (73/132.)*ScreenWidth, (119/242.)*ScreenHeight,
				(101/132.)*ScreenWidth, (159/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, 3);
	bar( (73/132.)*ScreenWidth, (159/242.)*ScreenHeight,
				(101/132.)*ScreenWidth, (199/242.)*ScreenHeight);
}


void WindowColorSignal( int REDCol, int GREENCol, int BLUECol) {

	float j;

	setfillstyle( SOLID_FILL, getbkcolor());
	for ( j=0; j<46; j += 15)
		bar( 37/132.*ScreenWidth, ((20+j)/242.)*ScreenHeight,
				((14*(59/14.)+37+1)/132.)*ScreenWidth, ((21+j)/242.)*ScreenHeight);

	setrgbpalette(pal.colors[5], REDCol, GREENCol, BLUECol);

	setfillstyle( SOLID_FILL, 5);
	bar(( 37/132.)*ScreenWidth, (68/242.)*ScreenHeight,
			(97/132.)*ScreenWidth, (168/242.)*ScreenHeight);

	setrgbpalette(pal.colors[RED], 63, 0, 0);
	setrgbpalette(pal.colors[GREEN], 0, 63, 0);
	setrgbpalette(pal.colors[BLUE], 0, 0, 63);


	setfillstyle( SOLID_FILL, RED);
	bar((37/132.)*ScreenWidth, (15/242.)*ScreenHeight,
			(97/132.)*ScreenWidth, (16/242.)*ScreenHeight);

	for (j=37; j<100; j+=59/7.)
		bar((j/132.)*ScreenWidth, (17/242.)*ScreenHeight,
				((j+1)/132.)*ScreenWidth, (18/242.)*ScreenHeight);

	bar((((REDCol-49)*(59/14.)+37)/132.)*ScreenWidth, (20/242.)*ScreenHeight,
			(((REDCol-49)*(59/14.)+37+1)/132.)*ScreenWidth, (21/242.)*ScreenHeight);

	setfillstyle( SOLID_FILL, GREEN);
	bar((37/132.)*ScreenWidth, (30/242.)*ScreenHeight,
			(97/132.)*ScreenWidth, (31/242.)*ScreenHeight);

	for (j=37; j<100; j+=59/7.)
		bar((j/132.)*ScreenWidth, (32/242.)*ScreenHeight,
				((j+1)/132.)*ScreenWidth, (33/242.)*ScreenHeight);

	bar((((GREENCol-49)*(59/14.)+37)/132.)*ScreenWidth, (35/242.)*ScreenHeight,
			(((GREENCol-49)*(59/14.)+37+1)/132.)*ScreenWidth, (36/242.)*ScreenHeight);


	setfillstyle( SOLID_FILL, BLUE);
	bar((37/132.)*ScreenWidth, (45/242.)*ScreenHeight,
			(97/132.)*ScreenWidth, (46/242.)*ScreenHeight);

	for (j=37; j<100; j+=59/7.)
		bar((j/132.)*ScreenWidth, (47/242.)*ScreenHeight,
				((j+1)/132.)*ScreenWidth, (48/242.)*ScreenHeight);

	bar((((BLUECol-49)*(59/14.)+37)/132.)*ScreenWidth, (50/242.)*ScreenHeight,
			(((BLUECol-49)*(59/14.)+37+1)/132.)*ScreenWidth, (51/242.)*ScreenHeight);
}


void DrawArc( int StartAng, int EndAng, int dyS, int dyE, int Xpos,
				  int Color, struct arccoordstype* arcinfo) {

	setcolor( Color);
	setfillstyle( SOLID_FILL, Color);

	arc( ScreenCenterX, ScreenCenterY, StartAng, EndAng, 6*width);
	getarccoords( arcinfo);

	arcinfo->ystart += dyS;
	arcinfo->yend += dyE;

	moveto( arcinfo->xstart, arcinfo->ystart-dyS);
	lineto( arcinfo->xstart, arcinfo->ystart);
	lineto( Xpos, arcinfo->ystart);

	lineto( Xpos, arcinfo->yend);
	lineto( arcinfo->xend, arcinfo->yend);

	lineto( arcinfo->xend, arcinfo->yend-dyE);

	floodfill( arcinfo->xstart+( arcinfo->xend - arcinfo->xstart)/2.,
				  arcinfo->ystart+( arcinfo->yend - arcinfo->ystart)/2., Color);
}


void testpict( void) {

	enum ColorDefs { Black=0, Color1, Color2, Color3, Color4, Blue,
						  Red, Magenta, Green, Cyan, Yellow,
						  White20, White40, White60, White80, White };

	int Sinus[] = {	White20, White40, White60, White80, White,
							White80, White60, White40, White20 };
	int i, j, k;
	int x[5], y[6];
	char t[2];

	struct arccoordstype arcinfo;
	struct palettetype pal;

	char rpattern[8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
	char r1pattern[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
	char r2pattern[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};

	char* tmptr;

	getpalette( &pal);

	/* BLACK */
	setrgbpalette( pal.colors[0], 0, 0, 0);

	/* (B-Y) 0 */
	setrgbpalette( pal.colors[1], 38, 32, 59);

	/* (R-Y) 90 */
	setrgbpalette( pal.colors[2], 54, 20, 34);

	/* (G-Y) 326 */
	setrgbpalette( pal.colors[3], 10, 10, 60);

	/* (R-Y) 270  */
	setrgbpalette( pal.colors[4], 9, 40, 31);

	/* Blue */
	setrgbpalette( pal.colors[5], 0, 0, 55);

	/* Red */
	setrgbpalette( pal.colors[6], 55, 0, 0);

	/* Magenta */
	setrgbpalette( pal.colors[7], 61, 0, 49);

	/* Green */
	setrgbpalette( pal.colors[8], 0, 57, 0);

	/* Cyan */
	setrgbpalette( pal.colors[9], 4, 57, 62);

	/* Yellow */
	setrgbpalette( pal.colors[10], 54, 54, 0);

	/* White20 -> White */
	for ( j=63, i=4; i>=0; i--, j -= 12)
		setrgbpalette( pal.colors[i+11], j, j, j);

	setfillstyle( SOLID_FILL, White60);
	bar( 0, 0, ScreenWidth, ScreenHeight);

	setcolor( White);
	setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

	for ( j = 18*width, i = Xstart; i < j; i += width)
		line( i, 0, i, ScreenHeight);

	for ( j = 14*height, i = Ystart; i < j; i += height)
		line( 0, i, ScreenWidth, i);


	/* FIRST */

	x[1] = Xstart + width + 1;
	x[2] = x[1] + width - 2;
	x[3] = x[2] + 1;
	x[4] = x[3] + width - 1;

	y[1] = Ystart + height + 1;
	y[2] = y[1] + 5.5*height;
	y[3] = y[1] + 2*height - 2;
	y[4] = Ystart + 12*height - 1;
	y[5] = y[4] - 2*height + 2;

	setfillstyle( SOLID_FILL, Color4);
	bar( x[1], y[1], x[2], y[2]); 			/* (R-Y) 270 */

	setfillpattern( rpattern, Cyan);
	bar( x[3], y[1], x[4], y[3]);				/* (G-Y) 326 */

	setfillstyle( SOLID_FILL, Color2);
	bar( x[1], y[4], x[2], y[2]);				/* (R-Y) 90 */

	setfillpattern( rpattern, Yellow);
	bar( x[3], y[4], x[4], y[5]);				/* (G-Y) 146 */


	/* NEXT */

	x[1] = Xstart + 15*width + 1;
	x[2] = x[1] + width - 2;
	x[3] = x[2] + 2 - 2*width;
	x[4] = x[3] + width - 1;

	y[1] = Ystart + height + 1;
	y[2] = y[1] + 5.5*height;
	y[3] = y[1] + 2*height - 2;
	y[4] = Ystart + 12*height - 1;
	y[5] = y[4] - 2*height + 2;

	setfillpattern( rpattern, Green);
	bar( x[1], y[1], x[2], y[2]); 			/* (B-Y) 180 */

	setfillpattern( rpattern, Cyan);
	bar( x[3], y[1], x[4], y[3]);				/* (G-Y) 326 */

	setfillstyle( SOLID_FILL, Color1);
	bar( x[1], y[4], x[2], y[2]);				/* (B-Y) 0 */

	setfillpattern( rpattern, Yellow);
	bar( x[3], y[4], x[4], y[5]);				/* (G-Y) 146 */



	/* BLACK CIRCLE */

	setcolor( 0);
	setfillstyle( SOLID_FILL, Black);
	fillellipse( ScreenCenterX, ScreenCenterY, 6*width, 6*height);



	/* COLOUR BAR */

	x[1] = Xstart + 2.5*width;
	x[3] = 2*width;
	x[2] = x[1] + x[3];

	DrawArc( 155, 175, 2, 0, x[1]+x[3], Yellow, &arcinfo);

	for ( j=x[3], i=1; i<5; i++, j += x[3]) {
		setfillstyle( SOLID_FILL, Yellow-i);
		bar( x[1] + j, arcinfo.ystart, x[2] + j, arcinfo.yend);
	}

	DrawArc( 5, 25, 0, 2, x[1]+5*x[3], Blue, &arcinfo);



	/* 250kHZ SQUAREWAVE */

	x[1] = 11*width/16.;
	x[2] = Xstart + 3*width;
	x[3] = x[2] + x[1];

	arcinfo.yend--;
	y[2] = Ystart + 3*height;

	setfillstyle( SOLID_FILL, White80);

	for ( j=x[1], i=1; i<15; i+=2, j=i*x[1])
		bar( x[2]+j, arcinfo.yend, x[3]+j, y[2]);

	DrawArc( 25, 36, 1, 0, x[2]+15*x[1], White80, &arcinfo);



	/* CHECK CORRECT TIMING CHROMINANCE-LUMINANCE */

	x[1] = Xstart + 8*width;

	DrawArc( 228, 265, 0, 0, x[1], Yellow, &arcinfo);
	DrawArc( 266, 312, 0, 0, x[1], Red, &arcinfo);
	DrawArc( 275, 312, 0, 0, x[1]+width, Yellow, &arcinfo);

	DrawArc( 216, 228, 0, 0, Xstart+5.5*width, White, &arcinfo);
	DrawArc( 312, 324, 0, 0, Xstart+11.5*width, White, &arcinfo);



	/* GREY SCALE FOR CHECKING OF LINEARITY AND COLOUR TRACKING */

	x[0] = Xstart + 2.5*width;
	x[1] = x[2] = j = width<<1;
	x[1] += x[0];

	DrawArc( 204, 216, 2, -1, x[0]+x[2], Black, &arcinfo);

	for ( i=1; i<5; i++, j += x[2]) {
		setfillstyle( SOLID_FILL, i+Yellow);
		bar( x[0] + j, arcinfo.ystart, x[1] + j, arcinfo.yend);
	}

	DrawArc( 324, 336, -1, 2, x[0]+5*x[2], White, &arcinfo);




	/* DEFINITION LINES, SINUSOIDAL */

	x[1] = Xstart + 3.5*width;

	y[1] = Ystart + 7*height+1;
	y[2] = y[1] + 2*height-2;

	setfillpattern( r1pattern, White);
	bar( x[1]+6*width, y[1], x[1]+8*width, y[2]);

	setfillpattern( rpattern, White);
	bar( x[1]+8*width, y[1], x[1]+10*width, y[2]);

	setfillpattern( r2pattern, White);
	bar( x[1]+4*width, y[1], x[1]+6*width, y[2]);


	for ( i=0; i < 4*16; i += 18 ) {
		for (j=0; j <= 18; j +=2) {
			setfillstyle( SOLID_FILL, Sinus[j/2]);
			bar( x[1]+i+j, y[1], x[1]+i+j+1, y[2]);
		}
	}

	x[1] += i;

	for ( i=0; i < 3.5*16; i += 10 ) {
		for (j=0; j < 9; j++) {
			setcolor( Sinus[j]);
			if ( Sinus[j] == White) {
				setfillstyle( SOLID_FILL, Sinus[j]);
				bar( x[1]+i+j, y[1], x[1]+i+j+2, y[2]);
				x[1] += 2;
			}
			else
				line( x[1]+i+j, y[1], x[1]+i+j, y[2]);
		}
	}


	/* REFLECTION CHECK */

	x[3] = x[2] = x[1] = Xstart + 5.5*width;
	x[2] += 6*width;
	x[3] += 0.5*width;

	y[2] = y[1] = Ystart + 2*height;
	y[2] += height-1;

	setfillstyle( SOLID_FILL, White);
	bar( x[1], y[1], x[2], y[2]);

	setcolor( Black);
	line ( x[3], y[1], x[3], y[2]);



	/* WHITE AROUND LOW FREQUENCY CHECK (INT THE TOP) */

	x[1] = Xstart+10.5*width;

	DrawArc( 49, 67, 0, -1, x[1], White, &arcinfo);
	DrawArc( 68, 113, 0, 0, x[1], White, &arcinfo);
	DrawArc( 113, 131, 0, 0, x[1]-4*width, White, &arcinfo);


	/* IN THE MIDDLE NOT INCLUDING TIME */

	x[0] = Xstart + 8*width + 1;

	y[2] = y[1] = Ystart + 5*height;
	y[2] += 3*height;

	y[4] = y[3] = y[1] + height;
	y[4] += height;

	setfillstyle( SOLID_FILL, Black);
	bar( x[0], y[1], x[0]+width-2, y[2]);

	setcolor( White);
	line( ScreenCenterX, y[1], ScreenCenterX, y[2]);

	line( Xstart+2.5*width, ScreenCenterY,
							Xstart+10*width, ScreenCenterY);

	x[1] = 8*width;

	for ( j=Xstart+3*width, i=0; i < x[1]; i += width)
		line( j+i, y[3], j+i, y[4]);

	i += 3*width;
	line( j+i, y[3], j+i, y[4]);

	line( j+i, ScreenCenterY, j+i+0.5*width, ScreenCenterY);




	/* SMALL BLACK/WHITE SQUARES AROUND IT ALL */

	setfillstyle( SOLID_FILL, Black);

	for ( k=height, j=width, i=1; i<19; i += 2, j += 2*width, k += 2*height) {
		floodfill( j+1, Ystart-1, White);
		floodfill( j+1, ScreenHeight-1, White);

		if ( i < 14) {
			floodfill( 1, k+1, White);
			floodfill( ScreenWidth - 1, k+1, White);
		}
	}

	setfillstyle( SOLID_FILL, White);
	for ( k=j=i=0; i<19; i += 2, j += 2*width, k += 2*height) {
		floodfill( j+1, Ystart-1, White);
		floodfill( j+1, ScreenHeight-1, White);

		if ( i < 14) {
			floodfill( 1, k+1, White);
			floodfill( ScreenWidth-1, k+1, White);
		}
	}


	/*  BOTTOM TEXT */

	settextstyle( SANS_SERIF_FONT, 0, 0);
	setusercharsize( 14, 8, 1, 1);
	outtextxy( Xstart+6.15*width, Ystart+9.90*height, "PHILIPS");



	/* TOP TEXT */

	settextstyle( SANS_SERIF_FONT, 0, 0);
	setusercharsize( 14, 8, 1, 1);
	outtextxy( Xstart+7.3*width, Ystart+0.9*height, "PTV");

	tid = time( NULL);

	strcpy( tmptr1, ctime( &tid));

	setcolor( WHITE);
	settextstyle( SANS_SERIF_FONT, 0, 4);

	for ( i=0; i<8; i++) {
		strncpy( t, &tmptr1[i+11], 1);
		t[1] = '\x0';
		if ( t[0] != ':')
			outtextxy( Xstart+10.2*width+16*i, Ystart+5.9*height, t);
	}

	outtextxy( Xstart+10.2*width+16*2.5, Ystart+5.85*height, ":");
	outtextxy( Xstart+10.2*width+16*5.4, Ystart+5.85*height, ":");
}


void updateClock( void) {

	int i;
	char t[2];

	tid = time( NULL);

	settextstyle( SANS_SERIF_FONT, 0, 4);

	strcpy( tmptr2, ctime( &tid));

	t[1] = '\x0';

	for ( i=0; i<8; i++) {
		if ( strncmp( &tmptr1[i+11], &tmptr2[i+11], 1)) {

			setcolor( BLACK);
			strncpy( t, &tmptr1[i+11], 1);
			outtextxy( Xstart+10.2*width+16*i, Ystart+5.9*height, t);

			setcolor( WHITE);
			strncpy( t, &tmptr2[i+11], 1);
			outtextxy( Xstart+10.2*width+16*i, Ystart+5.9*height, t);
		}
	}

	strcpy( tmptr1, tmptr2);
}


void CirclePict( void) {


	struct palettetype pal;

	getpalette( &pal);

	/* BLACK */
	setrgbpalette( pal.colors[0], 0, 0, 0);

	/* Blue */
	setrgbpalette( pal.colors[5], 0, 0, 63);

	/* Red */
	setrgbpalette( pal.colors[6], 63, 0, 0);

	/* Green */
	setrgbpalette( pal.colors[8], 0, 63, 0);

	/* Red-Green */
	setrgbpalette( pal.colors[9], 63, 63, 0);

	/* Red-Blue */
	setrgbpalette( pal.colors[10], 63, 0, 63);

	/* Blue-Green */
	setrgbpalette( pal.colors[11], 0, 63, 63);

	/* LightGray */
	setrgbpalette( pal.colors[14], 58, 58, 58);

	/* White */
	setrgbpalette( pal.colors[15], 63, 63, 63);

	setfillstyle( SOLID_FILL, 15);
	bar( 0, 0, ScreenWidth, ScreenHeight);

	setcolor( 14);
	circle( 0.68*ScreenWidth, ScreenHeight/2.+0.06*ScreenWidth, 0.12*ScreenWidth);
	setfillstyle( SOLID_FILL, 6); /* RED */
	floodfill( 0.68*ScreenWidth, ScreenHeight/2.+0.06*ScreenWidth, 14);

	circle( 0.82*ScreenWidth, ScreenHeight/2.+0.06*ScreenWidth, 0.12*ScreenWidth);
	setfillstyle( SOLID_FILL, 5); /* BLUE */
	floodfill( 0.82*ScreenWidth, ScreenHeight/2.+0.06*ScreenWidth, 14);

	circle( 0.75*ScreenWidth, ScreenHeight/2.-0.06*ScreenWidth, 0.12*ScreenWidth);
	setfillstyle( SOLID_FILL, 8); /* GREEN */
	floodfill( 0.75*ScreenWidth, ScreenHeight/2.-0.06*ScreenWidth, 14);


	setfillstyle( SOLID_FILL, 10); /* RED-BLUE */
	floodfill( 0.68*ScreenWidth + 0.1*ScreenWidth, ScreenHeight/2.+0.06*ScreenWidth, 14);

	setfillstyle( SOLID_FILL, 9); /* RED-GREEN */
	floodfill( 0.68*ScreenWidth, ScreenHeight/2.+0.05*ScreenWidth - 0.1*ScreenWidth, 14);

	setfillstyle( SOLID_FILL, 11); /* GREEN-BLUE */
	floodfill( 0.82*ScreenWidth, ScreenHeight/2.+0.05*ScreenWidth - 0.1*ScreenWidth, 14);

	setfillstyle( SOLID_FILL, 15); /* WHITE */
	floodfill( 0.68*ScreenWidth + 0.05*ScreenWidth, ScreenHeight/2.+0.05*ScreenWidth - 0.05*ScreenWidth, 14);
}


void DisplayMenuText( int TextOn) {

	if ( TextOn) {
		if ( SignalType == CirclePicture)
			setcolor( BLACK);
		else
			setcolor( WHITE);

		settextstyle( SMALL_FONT, 0, 0);
		setusercharsize( 6, 5, 4, 3);

		outtextxy( 10, ScreenHeight - 120, " F1:  Window Signal         Use Up/Down to change window level.");
		outtextxy( 10, ScreenHeight - 105, " F2:  Pluge Signal");
		outtextxy( 10, ScreenHeight - 90, " F3:  Window Color Signal   RED: Insert/Delete   GREEN: Home/End   BLUE: Page Up/Page Down");
		outtextxy( 10, ScreenHeight - 75, " F4:  Philips Test Pattern");
/*		outtextxy( 10, ScreenHeight - 60, " F5:  TriCircle Signal");*/
		outtextxy( 10, ScreenHeight - 60, "ESC:  Exit");

		outtextxy(( ScreenWidth-textwidth("Press SPACE BAR to remove this text"))/2,
					ScreenHeight - 30, "Press SPACE BAR to remove this text");
	}
	else {
		if ( SignalType == PlugeSignalType)
			PlugeSignal();
		else
			if ( SignalType == TestPicture)
				testpict();
		else
			if ( SignalType == CirclePicture)
				CirclePict();
		else {
			setfillstyle( SOLID_FILL, getbkcolor());
			bar( 10, ScreenHeight-120, ScreenWidth, ScreenHeight);
		}
	}
}

void BjarneSmallPicture( int X, int Y) {
	setfillstyle( SOLID_FILL, WHITE);

	bar( 270+X, 190+Y, ScreenWidth-270-X, ScreenHeight-190-Y);
}

void BjarneBigPicture(int X, int Y) {
	setfillstyle( SOLID_FILL, WHITE);

	bar( 220+X, 140+Y, ScreenWidth-220-X, ScreenHeight-140-Y);
}


main() {

	int ch, old_i, i, rgb_i, alive = -1;
	int REDNum=63, GREENNum=63, BLUENum=63;
	int BjarneSmallX=0, BjarneSmallY=0;
	int BjarneBigX=0, BjarneBigY=0;
	int MenuTextOn;

	ShowStartUpScreen();

	initGraphics();

	getpalette(&pal);

	for ( rgb_i=0; rgb_i<16; rgb_i++)
		setrgbpalette(pal.colors[rgb_i], rgb_i*4, rgb_i*4, rgb_i*4);

	WindowSignal( old_i = 7, i = 7);
	DisplayMenuText( MenuTextOn = 1);

	while ( alive) {
		switch( ch = GetKey()) {
			case kbEsc:
				alive = 0;
				break;

			case kbNoKey:
				break;

			case kbSpace:

				if ( MenuTextOn == 1)
					DisplayMenuText( MenuTextOn = 0);
				else
					DisplayMenuText( MenuTextOn = 1);
				break;

			case kbF1:
				cleardevice();
				MenuTextOn = 0;

				for ( rgb_i=0; rgb_i<16; rgb_i++)
					setrgbpalette(pal.colors[rgb_i], rgb_i*4, rgb_i*4, rgb_i*4);

				SignalType = WindowSignalType;

				setfillstyle( SOLID_FILL, 0);
				bar( 0, 0, ScreenWidth, ScreenHeight);

				WindowSignal( old_i, i);
				break;

			case kbF2:
				cleardevice();
				MenuTextOn = 0;

				for ( rgb_i=0; rgb_i<16; rgb_i++)
					setrgbpalette(pal.colors[rgb_i], rgb_i*4, rgb_i*4, rgb_i*4);

				SignalType = PlugeSignalType;
				PlugeSignal();
				break;

			case kbF3:
				cleardevice();
				MenuTextOn = 0;

				for ( rgb_i=0; rgb_i<16; rgb_i++)
					setrgbpalette(pal.colors[rgb_i], rgb_i*4, rgb_i*4, rgb_i*4);

				SignalType = ColorSignalType;
				WindowColorSignal( REDNum, GREENNum, BLUENum);
				break;

			case kbF4:
				cleardevice();
				MenuTextOn = 0;

				SignalType = TestPicture;
				testpict();

				updateClock();
				break;

			case kbF5:
				cleardevice();
				MenuTextOn = 0;

				SignalType = BjarneSmallField;

				BjarneSmallPicture( BjarneSmallX, BjarneSmallY);
				break;

			case kbF6:
				cleardevice();
				MenuTextOn = 0;

				SignalType = BjarneBigField;

				BjarneBigPicture( BjarneBigX, BjarneBigY);
				break;

			case kbUp:
			case kbDown:
				if ( SignalType == WindowSignalType) {
					if (ch == kbDown) {
						i--;
						if ( i < 0)
							i = 0;
					}
					else {
						i++;
						if ( i > 14)
							i = 14;
					}

					WindowSignal( old_i, i);
					old_i = i;
				}

				if ( SignalType == BjarneSmallField) {
					cleardevice();
					if (ch == kbDown) {
						BjarneSmallX--;
						if ( BjarneSmallX < -100)
							BjarneSmallX = -100;

						BjarneSmallY--;
						if ( BjarneSmallY < -100)
							BjarneSmallY = -100;
					}
					else {
						BjarneSmallX++;
						if ( BjarneSmallX > 100)
							BjarneSmallX = 100;

						BjarneSmallY++;
						if ( BjarneSmallY > 100)
							BjarneSmallY = 100;
					}
					BjarneSmallPicture( BjarneSmallX, BjarneSmallY);
				}

				if ( SignalType == BjarneBigField) {
					cleardevice();
					if (ch == kbDown) {
						BjarneBigX--;
						if ( BjarneBigX < -100)
							BjarneBigX = -100;

						BjarneBigY--;
						if ( BjarneBigY < -100)
							BjarneBigY = -100;
					}
					else {
						BjarneBigX++;
						if ( BjarneBigX > 100)
							BjarneBigX = 100;

						BjarneBigY++;
						if ( BjarneBigY > 100)
							BjarneBigY = 100;
					}
					BjarneBigPicture( BjarneBigX, BjarneBigY);
				}
				break;

			case kbIns:
			case kbHome:
			case kbPgUp:
				if ( SignalType == ColorSignalType) {
					switch ( ch) {
						case kbIns:
							if ( ++REDNum > 63)
								REDNum = 63;
							break;

						case kbHome:
							if ( ++GREENNum > 63)
								GREENNum = 63;
							break;

						case kbPgUp:
							if( ++BLUENum > 63)
								BLUENum = 63;
							break;
					}
					WindowColorSignal( REDNum, GREENNum, BLUENum);
				}
				break;

			case kbDel:
			case kbEnd:
			case kbPgDn:
				if ( SignalType == ColorSignalType) {
					switch ( ch) {
						case kbDel:
							if ( --REDNum < 49)
								REDNum = 49;
							break;

						case kbEnd:
							if ( --GREENNum < 49)
								GREENNum = 49;
							break;

						case kbPgDn:
							if ( --BLUENum < 49)
							BLUENum = 49;
							break;
					}
					WindowColorSignal( REDNum, GREENNum, BLUENum);
				}
				break;
		}

		if ( SignalType == TestPicture)
			updateClock();
	}

	closegraph();
	return( 0);
}