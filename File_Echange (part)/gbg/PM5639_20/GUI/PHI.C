#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>

void WriteEGALetterP( unsigned int X, unsigned Y);
void WriteCGALetterP( unsigned int X, unsigned int Y);
void WriteLetterH( unsigned int X, unsigned int Y);
void WriteLetterI( unsigned int X, unsigned int Y);
void WriteLetterL( unsigned int X, unsigned int Y);
void WriteEGALetterS( unsigned int X, unsigned int Y);
void WriteCGALetterS( unsigned int X, unsigned int Y);
void WriteKU( char* KU);
void WriteSWName( char* SW);
void WriteDev( char* Dev, char* Year);
void WriteStartCommand( char* Command);
void GraphAbort( char* Msg, int Err);
void StartUpScreen( char* KUNo, char* SWName, char* DevDept, char* Year);

unsigned int StartX, StartY;
unsigned int PhHigh;						/* Philips Height */
unsigned int LettW;                 /* LettW=LetterWidth */
unsigned	int HH;                    /* HH=H-Height */
unsigned int HW;                    /* HW=H-Width */
unsigned int LH;							/* LH=L-Height */
unsigned int FillColor;
unsigned int BkColor;
unsigned int MaxX, MaxY;
int ErrCode;
int MaxColor;
int GrDriver, GrMode;

void WriteEGALetterP( unsigned int X, unsigned Y) {

	moveto( X+LettW+11, Y);
	linerel( -LettW-11, 0);
	linerel( 0, PhHigh);
	linerel( LettW, 0);
	linerel( 0, -13);
	linerel( 11, 0);
	arc( getx(), gety()-23, 270, 90, 30);
	arc( getx()-2, gety()-23, 270, 90, 13);
	moveto( X+LettW+9, Y+13);
	linerel( -8, 0);
	linerel( 0, 20);
	linerel( 8, 0);
	floodfill( X+1, Y+1, FillColor);
}

void WriteCGALetterP( unsigned int X, unsigned int Y) {

	moveto( X+LettW+5, Y);
	linerel( -LettW-5, 0);
	linerel( 0, PhHigh);
	linerel( LettW, 0);
	linerel( 0, -9);
	linerel( 5, 0);
	arc( getx(), gety()-12, 270, 90, 15);
	arc( getx()-1, gety()-12, 270, 90, 8);
	moveto( X+LettW+4, Y+6);
	linerel( -4, 0);
	linerel( 0, 12);
	linerel( 4, 0);
	floodfill( X+1, Y+1, FillColor);
}

void WriteLetterH( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	linerel( 0, PhHigh);
	linerel( LettW, 0);
	linerel( 0, -HH);
	linerel( HW, 0);
	linerel( 0, HH);
	linerel( LettW, 0);
	linerel( 0, -PhHigh);
	linerel( -LettW, 0);
	linerel( 0, HH);
	linerel( -HW, 0);
	linerel( 0, -HH);
	linerel( -LettW, 0);
	floodfill( X+1, Y+1, FillColor);
}

void WriteLetterI( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	rectangle( X, Y, X+LettW,Y +PhHigh);
	floodfill( X+1, Y+1, FillColor);
}

void WriteLetterL( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	linerel( 0, PhHigh);
	linerel( 2*LettW, 0);
	linerel( 0, -LH);
	linerel( -LettW, 0);
	linerel( 0, -PhHigh+LH);
	linerel( -LettW, 0);
	floodfill( X+1, Y+1, FillColor);
}

void WriteEGALetterS( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	linerel( -6, -1);      					/* A */
	linerel( -19, 0);
	linerel( -11, 2);
	linerel( -5, 3);
	linerel( -3, 3);
	linerel( -2, 5);       					/* til B */
	linerel( 0, 8);
	linerel( 2, 4);
	linerel( 4, 4);
	linerel( 6, 3);
	linerel( 20, 8);       					/* til C */
	linerel( 0, 3);
	linerel( -2, 3);
	linerel( -6, 2);
	linerel( -8, 0);
	linerel( -15, -2);      				/* til D */
	linerel( 0, 13);
	linerel( 8, 2);
	linerel( 18, 0);
	linerel( 11, -2);
	linerel( 8, -4);
	linerel( 4, -5);
	linerel( 1, -3);       					/* til E */
	linerel( 0, -9);
	linerel( -3, -4);
	linerel( -4, -3);
	linerel( -18, -8);
	linerel( -5, -2);
	linerel( -2, -2);      					/* til F */
	linerel( 0, -3);
	linerel( 2, -2);
	linerel( 5, -1);
	linerel( 12, 0);
	linerel( 9, 2);
	linerel( 0, -13);							/* til A */
	floodfill( X-3, Y+3, FillColor);
}

void WriteCGALetterS( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	linerel( -3, -1);      			/* A */
	linerel( -10, 0);
	linerel( -5, 1);
	linerel( -2, 1);
	linerel( -2, 2);
	linerel( -2, 4);       			/* til B */
	linerel( 0, 4);
	linerel( 1, 3);
	linerel( 2, 2);
	linerel( 12, 5);
	linerel( 1, 1);       			/* til C */
	linerel( 0, 3);
	linerel( -1, 1);
	linerel( -5, 1);
	linerel( -3, 0);
	linerel( -6, -1);      			/* til D */
	linerel( 0, 7);
	linerel( 3, 1);
	linerel( 8, 0);
	linerel( 5, -1);
	linerel( 4, -2);
	linerel( 2, -2);
	linerel( 1, -3);        		/* til E */
	linerel( 0, -5);
	linerel( -1, -3);
	linerel( -3, -3);
	linerel( -7, -3);
	linerel( -3, -1);
	linerel( -1, -1);      			/* til F */
	linerel( 0, -2);
	linerel( 1, -1);
	linerel( 3, -1);
	linerel( 6, 0);
	linerel( 5, 1);
	linerel( 0,-7);       			/* til A */
	floodfill( X-3, Y+3, FillColor);
}


void WriteKU( char* KU) {
	volatile int size;
	void* picture;
	int x1, x2, y1, y2;
	FILE* stream;
	char kim[1500] = "QQQQQQQQQQQQQQQQQQQQQQQQQQQQ";

	array[0..1500] of char
	settextstyle( SMALL_FONT, HORIZ_DIR, 3);

	if ( GrDriver == CGA)
		setusercharsize(( 23*MaxX)/640, 10, ( 18*MaxY)/350, 10);
	else
		setusercharsize(( 18*MaxX)/640, 10, ( 15*MaxY)/350, 10);

	settextjustify( CENTER_TEXT, TOP_TEXT);

	x1 = MaxX/2;
	x2 = MaxX/2+strlen( KU)*8;

	y1 = ( MaxY*45)/100 - 2;
	y2 = ( MaxY*45)/100 + 12;

	outtextxy( x1, y1, KU);

	picture = kim;
/*	size = imagesize( x1, y1, x2, y2);

	picture = malloc( size);

	getimage( x1, y1, x2, y2, picture);

	stream = fopen( "grafik", "w");
	fwrite( picture, size, 1, stream);
	fclose( stream);

	stream = fopen( "grafik", "rb");
	fread( picture, size, 1, stream);
	fclose( stream);*/

	putimage( x1, y1, picture, NOT_PUT);

	free( picture);
	x1 = y1;
}

void WriteSWName( char* SW) {

	unsigned int x, y, tw;

	settextstyle( SMALL_FONT, HORIZ_DIR, 3);
	setusercharsize(( 30*MaxX)/640, 10, ( 20*MaxY)/350+1, 10);
	settextjustify( CENTER_TEXT, TOP_TEXT);

	x = MaxX/2 + 10;
	y = ( MaxY * 66)/100;
	tw = textwidth( SW);
	outtextxy( x, y, SW);


	if ( GrDriver == CGA)
		rectangle( x-( tw/2)-15, y+3, x-( tw/2)-7, y+10);
	else
		if (( GrDriver == EGA) || ( GrDriver == VGA))
			rectangle( x-( tw/2)-27, y+6, x-( tw/2)-10, y+18);

	floodfill( x-( tw/2)-12, y+8, FillColor);
}

void WriteDev( char* Dev, char* Year) {

	char tmp[100];

	settextstyle( SMALL_FONT, HORIZ_DIR, 2);
	if ( GrDriver == CGA)
		setusercharsize( 10, 12, 10, 10);
	else
		if (( GrDriver == EGA) || ( GrDriver == VGA))
			setusercharsize( 10, 10, 12, 10);

	settextjustify( CENTER_TEXT, TOP_TEXT);
	outtextxy( MaxX/2, ( MaxY * 75)/100, Dev);       /* EGA=265 */

	strcpy( tmp, "(c) copyright Philips Danmark A/S,");
	strcat( tmp, Year);

	outtextxy( MaxX/2, ( MaxY * 80)/100, tmp);			/* EGA=278 */
}

void WriteStartCommand( char* Command) {

	settextstyle( SMALL_FONT, HORIZ_DIR, 3);
	setusercharsize( ( 30*MaxX)/640, 10, ( 20*MaxY)/350+1, 10);
	settextjustify( CENTER_TEXT, TOP_TEXT);
	outtextxy( MaxX/2, ( MaxY * 90)/100, Command);   /* 315 */
}


void GraphAbort( char* Msg, int Err) {

	printf( "%s%s%s\n", Msg, ': ', grapherrormsg( Err));
	exit( EXIT_SUCCESS);
}


main() {
	int ch;
	int i;
	char* KUNo = "Serial number: KU123456";
	char* SWName = "PM5639 Industrial Analyzer";
	char* DevDept = "Philips TV Test Equipment A/S, Denmark";
	char* Year = "1992";

/*	if ( registerbgidriver( CGA_driver) < 0)
		GraphAbort( "CGA", graphresult());*/

	if ( registerbgidriver( EGAVGA_driver) < 0)
		GraphAbort( "EGA/VGA", graphresult());

	if ( registerbgifont( small_font) < 0 )
		GraphAbort( "SmallFont", graphresult());

	if ( registerbgifont( sansserif_font) < 0)
		GraphAbort( "SansSerifFont", graphresult());
/* (*
 FontNo:= RegisterBGIFont(@TscrFontProc);
 IF FontNo < 0 THEN GraphAbort('TscrFont');
 FontNo := InstallUserFont('tscr');           {install an *.obj file}
*) */

	detectgraph( &GrDriver, &GrMode);
	if ( GrDriver < 0) {
		restorecrtmode();
		GraphAbort( "Graphic error detected", graphresult());
		exit( 1);
	}

	/* GrDriver := CGA; */            /* for test of CGA display */
	if ( GrDriver == CGA) {
		GrMode = CGAC1;		/* CGA:   320x200 pixels */
		FillColor = 3;       /* CGAC0: 1=LightGreen  2=LightRed  3=LightYellow*/
		BkColor = BLACK;		/* CGAC1: 1=Cyan  2=Magenta  3=white */
		StartX  = 56;        /* CGAC2: 1=Green  2=Red  3=Brown */
		StartY  = 26;        /* CGAC3: 1=Cyan  2=Magenta  3=white */
		PhHigh  = 33;
		HH = 13;
		HW = 14;
		LettW = 8;
		LH = LettW - 2;
	}
	else
		if (( GrDriver == EGA) || ( GrDriver == EGA64) || ( GrDriver == VGA)) {
			GrMode = EGAHI;      /* mode=640x350   color=16 colors */
			FillColor = EGA_BLUE;
			BkColor = EGA_LIGHTGRAY;
			StartX = 112;
			StartY = 47;
			PhHigh = 59;
			HH = 23;
			HW = 24;
			LettW = 19;
			LH = LettW - 7;
		}
	else
		exit( 1);

	initgraph( &GrDriver, &GrMode, "");
	ErrCode = graphresult();
	if ( ErrCode != grOk) {
		restorecrtmode();
		GraphAbort( "Init graphics error", ErrCode);
		exit( 1);
	}

	MaxColor = getmaxcolor();
	MaxX = getmaxx();
	MaxY = getmaxy();
	setcolor( FillColor);
	setbkcolor( BkColor);

	setvisualpage( 1);
	setactivepage( 0);
	cleardevice();
	setlinestyle( SOLID_LINE, 0, NORM_WIDTH);
	setfillstyle( SOLID_FILL, FillColor);

	if (( GrDriver == EGA) || ( GrDriver == VGA) || ( GrDriver == EGA64)) {
		WriteEGALetterP (StartX, StartY);
		WriteLetterH( StartX+72, StartY);
		WriteLetterI( StartX+156, StartY);
		WriteLetterL( StartX+198, StartY);
		WriteLetterI( StartX+251, StartY);
		WriteEGALetterP( StartX+291, StartY);
		WriteEGALetterS( StartX+408, StartY);
	}
	else
		if ( GrDriver == CGA) {
			WriteCGALetterP( StartX, StartY);
			WriteLetterH( StartX+36, StartY);
			WriteLetterI( StartX+78, StartY);
			WriteLetterL( StartX+99, StartY);
			WriteLetterI( StartX+124, StartY);
			WriteCGALetterP( StartX+144, StartY);
			WriteCGALetterS( StartX+203, StartY);
		}

	WriteKU( KUNo);
	WriteSWName( SWName);
	WriteDev( DevDept, Year);
	WriteStartCommand( "Press ENTER to continue");
	setvisualpage( 0);

	while (( ch = getch()) != 0x00D)
		;

	closegraph();
	restorecrtmode();
}
