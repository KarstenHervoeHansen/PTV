/* Size of Information window: openInformation(); */

#define InfLeft   100
#define InfTop    100
#define InfRight    540
#define InfBottom 250
#define InfWidth    ( InfRight - InfLeft)


/* Size of adjust colors window: adjustColors(); */

#define AdjLeft   5
#define AdjTop    100
#define AdjRight    155
#define AdjBottom 320
#define AdjWidth    ( AdjRight - AdjLeft)
#define AdjDepth    ( AdjBottom - AdjTop)


/* Size of Display Status window: openDispStatus(); */

#define DispLeft    105
#define DispTop   60
#define DispRight 540
#define DispBottom  285
#define DispWidth ( DispRight - DispLeft)


#define CO_PROCESSOR_MASK 0x0002
#define NO_SERIAL_PORTS   0x0E00

/* External variables used in GUI2.C: */

extern const int HlpFileInformation;  /* HELP.H */
extern const int HlpEditStatus;     /* HELP.H */

extern ERRORINFO errTxt[];          /* MESSAGE.C */
extern const char* COMText[];

extern int COMopened[2];
extern int FASTmode;
extern int NoWin;

extern WINDOW MenuWindow;
extern WININFO* WinPtr[MAXWINDOW];
extern WININFO Win[MAXWINDOW];

extern int CIECrossSize[MAXDISPLAY+1];
extern void* CIECrossImage[MAXDISPLAY+1];
