/* Size of setup CIE window: openSetupCIE(); */

#define oCLeft    100
#define oCTop   60
#define oCRight   540
#define oCBottom  290
#define oCWidth   ( oCRight - oCLeft)

#define oCRect 180
#define oCtop   ( oCTop + WINDOWBARDEEP)

#define oCleftCol ( oCLeft + ( oCWidth - 2*oCRect)/3. + 8)
#define oCrightCol  ( oCLeft + oCRect + 2*( oCWidth - 2*oCRect)/3. + 8)


/* Size of setup RGB window: openSetupRGB(); */

#define oRLeft    85
#define oRTop     60
#define oRRight   555
#define oRBottom    290
#define oRWidth   ( oRRight - oRLeft)

#define oRRect    226
#define oRtop     ( oRTop + WINDOWBARDEEP)
#define oRleftCol ( oRLeft + ( oRWidth - 2*oRRect + 32)/3. + 8)
#define oRrightCol  ( oRLeft + oRRect + 2*( oRWidth - 2*oRRect+32)/3. -24)

/* Size of display Window if NONE open: openCIE_RGB(); */

#define oCRLeft   10
#define oCRTop    ( MENULINEDEEP + 10)
#define oCRRight    310
#define oCRBottom ( MENULINEDEEP + 250)

#define oCRWidth    ( oCRRight - oCRLeft)
#define oCRDepth    ( oCRBottom - oCRTop)


#define MaxTable      23    /* Number of entries in tables used to */
                    /* calculate correlated color temperature */

#define CIEWidth      185 /* Minimum width for text in CIE-display */
#define CIELength     135 /* Minimum height for text in CIE-display */

#define RGBWidth      200 /* Minimum width for text in RGB-display */
#define RGBLength     150 /* Minimum height for text in RGB-display */

#define CIEframe      20
#define RGBframe      10

/* External variables used in DISPLAY.C: */

extern const int HlpEditSetupCIE;
extern const int HlpEditSetupRGB;

extern const int EBUphosphor[];
extern const float D6500[];
extern char* photxt[];


extern WININFO* WinPtr[MAXWINDOW];

extern void* CIECrossImage[MAXDISPLAY+1];
extern int CIECrossSize[MAXDISPLAY+1];

extern int COMopened[2];
extern int NoWin;

