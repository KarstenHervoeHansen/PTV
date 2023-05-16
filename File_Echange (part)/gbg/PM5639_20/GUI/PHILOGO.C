#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

/*
void WriteEGALetterP( unsigned int X, unsigned Y);
void WriteLetterT( unsigned int X, unsigned int Y);
void WriteLetterV( unsigned int X, unsigned int Y);
void WriteKU();
void WriteSWName( char* SW);
void WriteDev( char* Dev, char* Year);
void WriteStartCommand( char* Command);
void StartUpScreen( char* SWName, char* DevDept, char* Year);
*/

const char* KUNUMBER = "KUWWWWWW";
char KUPixel[2100] = "QQQQQQ";
char* SoftwareType = "ZZZZ";
int LogoTimeoutON;

unsigned int StartX, StartY;
unsigned int PTVHigh;					/* PTV Height */
unsigned int LettW;                 /* LettW=LetterWidth */
unsigned	int HH;                    /* HH=H-Height */
unsigned int HW;                    /* HW=H-Width */
unsigned int LH;							/* LH=L-Height */
unsigned int FillColor;
unsigned int BkColor;
unsigned int MaxX, MaxY;
int ErrCode;
int MaxColor;

void WriteEGALetterP( unsigned int X, unsigned Y) {

	moveto( X+LettW+11, Y);
	linerel( -LettW-11, 0);
	linerel( 0, PTVHigh);
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

void WriteLetterT( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	linerel( 0, 12);
	linerel( LettW, 0);
	linerel( 0, PTVHigh-12);
	linerel( 18, 0);
	linerel( 0, -( PTVHigh-12));
	linerel( LettW, 0);
	linerel( 0, -12);
	linerel( -(2*LettW+18), 0);
	floodfill( X+1, Y+1, FillColor);
}

void WriteLetterV( unsigned int X, unsigned int Y) {

	moveto( X, Y);
	linerel( 27, PTVHigh);
	linerel( LettW+1, 0);
	linerel( 27, -PTVHigh);
	linerel( -LettW, 0);
	linerel( -LettW+1, +PTVHigh-LettW);
	linerel( -LettW, -PTVHigh+LettW);
	linerel( -LettW+1, 0);
	floodfill( X+1, Y+1, FillColor);
}

void WriteKU( void) {

	int x1, y1;

	x1 = ( MaxX - 229)/ 2;
	y1 = ( MaxY * 45) / 100;

	if ( strcmp( KUPixel, "QQQQQQ") == NULL)
		;
	else
		putimage( x1, y1, &KUPixel[0], COPY_PUT);
}

void WriteSWName( char* SW) {

	unsigned int x, y, tw;

	settextstyle( SMALL_FONT, HORIZ_DIR, 3);
	setusercharsize(( 30*MaxX)/640, 10, ( 20*MaxY)/350+1, 10);
	settextjustify( CENTER_TEXT, TOP_TEXT);

	x = MaxX/2 + 25; /* former + 10 */
	y = ( MaxY * 66)/100;
	tw = textwidth( SW);
	outtextxy( x, y, SW);

	rectangle( x-( tw/2)-27, y+6, x-( tw/2)-10, y+18);
	floodfill( x-( tw/2)-12, y+8, FillColor);
}

void WriteDev( char* Dev, char* Year) {

	char tmp[100];

	settextstyle( SMALL_FONT, HORIZ_DIR, 2);
	setusercharsize( 10, 10, 12, 10);

	settextjustify( CENTER_TEXT, TOP_TEXT);
	outtextxy( MaxX/2, ( MaxY * 75)/100, Dev);       /* EGA=265 */

	strcpy( tmp, "Copyright (c) 1992, ");
	strcat( tmp, Year);

	outtextxy( MaxX/2, ( MaxY * 80)/100, tmp);			/* EGA=278 */
	outtextxy( MaxX/2, ( MaxY * 85)/100, "All rights reserved");
}

void WriteStartCommand( char* Command) {

	settextstyle( SMALL_FONT, HORIZ_DIR, 3);
	setusercharsize( ( 30*MaxX)/640, 10, ( 20*MaxY)/350+1, 10);
	settextjustify( CENTER_TEXT, TOP_TEXT);
	outtextxy( MaxX/2, ( MaxY * 90)/100, Command);   /* 315 */
}

void StartUpScreen( char* SWName, char* DevDept, char* Year) {

	int ch, i;
	time_t tid;

	FillColor = EGA_BLUE;
	BkColor = EGA_LIGHTGRAY;
	StartX = 205;
	StartY = 47;
	PTVHigh = 59;
	HH = 23;
	HW = 24;
	LettW = 19;
	LH = LettW - 7;

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

	WriteEGALetterP (StartX, StartY);
	WriteLetterT( StartX+72, StartY);
	WriteLetterV( StartX+147, StartY);

	WriteKU();
	WriteSWName( SWName);
	WriteDev( DevDept, Year);
	WriteStartCommand( "Press ENTER to continue");
	setvisualpage( 0);

	tid = time(NULL);

	if (kbhit())
		ch = getch();

	if ( LogoTimeoutON == 0xFF) {
		while (( ch != 0x00D) && (time(NULL) < tid+10)) {
			if (kbhit())
				ch = getch();
		}
	}
	else
		while ( ch != 0x00D) {
			if (kbhit())
				ch = getch();
		}

	setbkcolor( EGA_BLACK);
	cleardevice();
}