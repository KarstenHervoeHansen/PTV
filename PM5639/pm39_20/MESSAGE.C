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

#include "message.h"

/* Text for About window: doAbout(); */

                   
float SoftWareNO = 3.05*100;
extern char* SoftwareType;

char* abouttxt[] = {
  "PM5639 Industrial CRT Color Analyzer",
  "PM5639 CRT Color Analyzer, PC",
  "Version: 3.05",
  "Serial No. KU000000",
  "(c) copyright Philips Danmark A/S, 1992, 1994",
  "All rights reserved",
  "Philips TV Test Equipment A/S"
};

/* Text for ALL possible errors: displayError(); */
/* Free to use: 17 and 29 */

ERRORINFO errTxt[] = {

/* 00 */  ERROR,
  "No display", "has been opened", NULL,

/* 01 */  ERROR,
  "Error opening display.", "Maximum number of displays exceeded.", NULL,

/* 02 */  WARNING,
  "File already exists.", "Overwrite existing file ?", NULL,

/* 03 */  WARNING,
  "Reference already exists.", "Overwrite existing reference ?", NULL,

/* 04 */ DISKERROR,                   /* Generel disk errors */
  NULL, NULL, NULL,

/* 05 */  ERROR,
  NULL, "File not found.", NULL,

/* 06 */  ERROR,
  "Only filnames with extension", NULL, "are allowed.",

/* 07 */  WARNING,
  "About to delete white reference", NULL, NULL,

/* 08 */  WARNING,
  "About to delete phosphor", NULL, NULL,

/* 09 */  ERROR,
  "Display has to be of RGB-type", "to learn a phosphor.", NULL,

/* 10 */  ERROR,
  "Display has to be of RGB-type", "to restore a phosphor.", NULL,

/* 11 */  ERROR,
  "Display has to be of RGB-type", "to save a phosphor.", NULL,

/* 12 */  ERROR,
  NULL, "Value out of range.", NULL,

/* 13 */  ERROR,
  NULL, "Reference MUST have a name.", NULL,

/* 14 */  ERROR,
  NULL, "Phosphor MUST have a name.", NULL,

/* 15 */  ERROR,
  "Reference already exists", "Default names can not be overwritten.", NULL,

/* 16 */  ERROR,
  "One, ( or more), phophors have", "not been learned.", NULL,

/* 17 */  ERROR,
  NULL, NULL, NULL,

/* 18 */  ERROR,
  "Timeout receiving data from", "the Color Sensor in", NULL,

/* 19 */  ERROR,
  NULL, "Unable to open specified file.", NULL,

/* 20 */  ERROR,
  "Setup not available for", "restored measurement.", NULL,

/* 21 */  ERROR,
  "Restore not available for", "restored measurement.", NULL,

/* 22 */  ERROR,
  "Learn not available for", "restored measurement.", NULL,

/* 23 */  ERROR,
  "Error in received measurement:", "Lowlight in one or more channels", NULL,

/* 24 */  WARNING,
  "Phosphor already exists.", "Overwrite existing phosphor ?", NULL,

/* 25 */  NOTICE,
  "Be sure to apply a", NULL, "window on the CRT.",

/* 26 */ ERROR,
  "Unable to open specified COM port", NULL, NULL,

/* 27 */ YES_NO,
  "About to exit program.", "Overwrite the existing", "configuration file ?",

/* 28 */ YES_NO,
  NULL, "Do you really want to exit ?", NULL,

/* 29 */ ERROR,
  NULL, NULL, NULL,

/* 30 */ WARNING,
  "About to close active display.", "OK to proceed ?", NULL,

/* 31 */  ERROR,
  "Unable to open Help system:", "Help-file not found", NULL,

/* 32 */  ERROR,
  "Unable to open Help system:", "Index-file not found", NULL,

/* 33 */  ERROR,
  "Unable to open Help system:", "Help-file has been corrupted", NULL,

/* 34 */  ERROR,
  "Unable to open Help system:", "Index-file has been corrupted", NULL,

/* 35 */  ERROR,
  NULL, "Help system is not available", NULL,

/* 36 */  ERROR,                    /* Generel Printer errors */
  NULL, NULL, NULL,

/* 37 */  ERROR,
  NULL, "Filename has to be specified", NULL,

/* 38 */ ERROR,
  "Unable to open specified COM port", NULL, NULL,

/* 39 */  ERROR,
  "Phosphor already exists", "Default names can not be overwritten.", NULL,

/* 40 */ WARNING,
  "About to close ALL active displays.", "OK to proceed ?", NULL,

/* 41 */ ERROR,
  "Unable to read file", "File has been saved with an",
                                "old software revision",
/* 42 */ ERROR,
  NULL, "Specified filename is not allowed", NULL,

/* 43 */ ERROR,
  "Illegal or no measurements in display", "file can not be saved", NULL,

/* 44 */ ERROR,
  "FAST mode not supported", "by the connected sensor(s)", NULL
};


/* Open an Error Window */

int displayError( int errorno ) {

  ERRORINFO* errDisplay = &errTxt[errorno];
  BUTTON bu[2];

  void* Image;
  char* txtptr;
  char str[5];
  int ch, next=0, tmp1, tmp2, i;

  int oldcolor;
  struct fillsettingstype oldfillinfo;
  struct linesettingstype oldlineinfo;

  oldcolor = getcolor();
  getfillsettings( &oldfillinfo);
  getlinesettings( &oldlineinfo);

  i = imagesize( ErrLeft, ErrTop,
                ErrRight+DROPSHADOW, ErrBottom+DROPSHADOW);
  Image = malloc( i);

  getimage( ErrLeft, ErrTop,
                ErrRight+DROPSHADOW, ErrBottom+DROPSHADOW, Image);

  setfillstyle( SOLID_FILL, WindowBkColor);

  bar( ErrLeft, ErrTop, ErrRight, ErrBottom);

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);

  line( ErrRight+2, ErrTop+DROPSHADOW, ErrRight+2, ErrBottom+3);
  line( ErrLeft+DROPSHADOW, ErrBottom+2, ErrRight, ErrBottom+2);

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

  switch ( errDisplay->mode) {
    case ERROR:
      addButton( &bu[0], ErrLeft+ErrWidth/2 - 30, ErrBottom-27,
         ErrLeft+ErrWidth/2+30, ErrBottom-12, ButtonText[Ok], ButColor);

      drawButton( &bu[0], ACTIVE);

      tmp1 = EGA_RED;
      txtptr = "!!! ERROR !!!";
      break;

    case YES_NO:
      tmp1 = Yes;
      tmp2 = No;                    /* break IS missing */

    case WARNING:
      if ( errDisplay->mode == WARNING) {
        tmp1 = Ok;
        tmp2 = Cancel;
      }

      addButton( &bu[0], ErrLeft+ErrWidth/5, ErrBottom-27,
        ErrLeft+ErrWidth/5+60, ErrBottom-12, ButtonText[tmp1], ButColor);

      addButton( &bu[1], ErrLeft+ErrWidth/5+120, ErrBottom-27,
        ErrLeft+ErrWidth/5+180, ErrBottom-12, ButtonText[tmp2], ButColor);

      drawButton( &bu[0], INACTIVE);
      drawButton( &bu[1], ACTIVE);

      tmp1 = EGA_BLUE;
      txtptr = "!!! WARNING !!!";
      break;

    case NOTICE:
      addButton( &bu[0], ErrLeft+ErrWidth/2 - 30, ErrBottom-27,
         ErrLeft+ErrWidth/2+30, ErrBottom-12, ButtonText[Ok], ButColor);

      drawButton( &bu[0], ACTIVE);

      tmp1 = EGA_BLUE;
      txtptr = "!!! NOTICE !!!";
      break;

    case DISKERROR:
      tmp1 = Retry;
      tmp2 = Cancel;

      addButton( &bu[0], ErrLeft+ErrWidth/5, ErrBottom-27,
        ErrLeft+ErrWidth/5+60, ErrBottom-12, ButtonText[tmp1], ButColor);

      addButton( &bu[1], ErrLeft+ErrWidth/5+120, ErrBottom-27,
        ErrLeft+ErrWidth/5+180, ErrBottom-12, ButtonText[tmp2], ButColor);

      drawButton( &bu[0], INACTIVE);
      drawButton( &bu[1], ACTIVE);

      tmp1 = EGA_RED;
      txtptr = "!!! ERROR !!!";
      break;
  }

  setcolor( tmp1);

  for ( i=0; i<6; i += 2)
    rectangle( ErrLeft+i, ErrTop+i, ErrRight-i, ErrBottom-i);

  line( ErrLeft+10, ErrTop+25, ErrRight-10, ErrTop+25);

  settextstyle( DEFAULT_FONT, HORIZ_DIR, 2);

  moveto( ErrLeft + ErrWidth/2, ErrTop+8);
  moverel( - ( strlen( txtptr) << 3), 0);
  outtext( txtptr);

  settextstyle( DEFAULT_FONT, HORIZ_DIR, 1);
  setcolor( EGA_BLACK);

  itoa( errorno, str, 10);

  moveto( ErrRight-100, ErrTop+30);
  outtext( "Code no: ");
  outtext( str);

  i = 0;
  if ( errDisplay->txt1)
    i++;
  if ( errDisplay->txt2)
    i++;
  if ( errDisplay->txt3)
    i++;

  if ( i == 2)
    i = ErrTop + 51;
  else
    i = ErrTop + 45;

  if ( errDisplay->txt1) {
    moveto( ErrLeft + ErrWidth/2, i);
    moverel( - strlen( errDisplay->txt1)<<2, 0);
    outtext( errDisplay->txt1);
  }

  if ( errDisplay->txt2) {
    moveto( ErrLeft + ErrWidth/2, i+12);
    moverel( - strlen( errDisplay->txt2)<<2, 0);
    outtext( errDisplay->txt2);
  }

  if ( errDisplay->txt3) {
    moveto( ErrLeft + ErrWidth/2, i+24);
    moverel( - strlen( errDisplay->txt3)<<2, 0);
    outtext( errDisplay->txt3);
  }

  if ( errDisplay->mode == ERROR)
    while ( GetKey() == kbNoKey)
      ;
  else
    do {

      ch = GetKey();

      if ( errDisplay->mode != NOTICE)
        if (( ch == kbTab) || ( ch == kbShiftTab) ||
           ( ch == kbRight) || ( ch == kbLeft))  {

          if ( next++)
            next = 0;

          drawButton( &bu[next], INACTIVE);
          drawButton( &bu[abs(next-1)], ACTIVE);
        }
    } while (( ch != kbEnter) && ( ch != kbEsc));

  putimage( ErrLeft, ErrTop, Image, COPY_PUT);

  free( Image);

  setcolor( oldcolor);
  setfillstyle( oldfillinfo.pattern, oldfillinfo.color);
  setlinestyle( oldlineinfo.linestyle, 0, oldlineinfo.thickness);

  if ( ch == kbEsc)
    return( kbEsc);

  return( next);
}


int doAbout( void) {

  BUTTON bu;

  int i, j;
  void* Image;
  extern const char* KUNUMBER;

  i = imagesize( AbLeft, AbTop, AbRight+DROPSHADOW, AbBottom+DROPSHADOW);
  Image = malloc( i);

  getimage( AbLeft, AbTop, AbRight+DROPSHADOW, AbBottom+DROPSHADOW, Image);

  setfillstyle( SOLID_FILL, WindowBkColor);

  bar( AbLeft, AbTop, AbRight, AbBottom);

  setcolor( EGA_BLACK);
  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);

  line( AbRight+2, AbTop+DROPSHADOW, AbRight+2, AbBottom+3);
  line( AbLeft+DROPSHADOW, AbBottom+2, AbRight, AbBottom+2);

  setcolor( EGA_BLUE);
  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

  for ( i=0; i<6; i += 2)
    rectangle( AbLeft+i, AbTop+i, AbRight-i, AbBottom-i);

  strcpy( &abouttxt[3][11], KUNUMBER);

  i = 0;
  if ( strcmp( SoftwareType, "INDU"))
    i++;

  moveto( AbLeft + AbWidth/2, AbTop + 14);
  moverel( - ( strlen( abouttxt[i]) << 2), 0);
  outtext( abouttxt[i]);

  for ( j = 28, i = 2; i<7; i++, j += 14) {
    moveto( AbLeft + AbWidth/2, AbTop + j);
    moverel( - ( strlen( abouttxt[i]) << 2), 0);
    outtext( abouttxt[i]);
  }

  addButton( &bu, AbLeft+( AbWidth-60)/2, AbBottom-25,
         AbLeft+( AbWidth-60)/2+60, AbBottom-10, "Ok", ButColor);

  drawButton( &bu, ACTIVE);

  while ( GetKey() == kbNoKey)
    ;

  putimage( AbLeft, AbTop, Image, COPY_PUT);
  free( Image);

  return( 0);
}


/* Display a temporary meassage */

void* displayMessage( int left, int top, int color, char* message) {

  int right = left + (( 10 + strlen( message)) << 3);
  int bottom = top + 40;

  void* Image;
  int mid, cen;

  mid = imagesize( left, top, right+DROPSHADOW, bottom+DROPSHADOW);
  Image = malloc( mid);

  getimage( left, top,  right+DROPSHADOW, bottom+DROPSHADOW, Image);

  setcolor( EGA_BLACK);
  setfillstyle( SOLID_FILL, color);

  bar( left, top, right, bottom);

  setlinestyle( SOLID_LINE, 0, THICK_WIDTH);

  line( right+2, top+DROPSHADOW, right+2, bottom+3);
  line( left+DROPSHADOW, bottom+2, right, bottom+2);

  setlinestyle( SOLID_LINE, 0, NORM_WIDTH);

  for ( mid=0; mid<6; mid += 2)
    rectangle( left+mid, top+mid, right-mid, bottom-mid);

  cen = ( right + left - ( strlen( message) << 3)) >> 1;
  mid = ( bottom + top - 8) >> 1;

  setcolor( EGA_WHITE);
  outtextxy( cen, mid, message);

  return( Image);
}
