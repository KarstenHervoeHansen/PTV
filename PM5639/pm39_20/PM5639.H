#include "pm39io.h"

#define MAXDISPLAY    6   /* Max. number of DISPLAYS at one time */
                    /* Display are either RGB- or CIE-display */
#define MAXWINDOW     9   /* Max. number of WINDOWS at one time */
                    /* Windows also includes setup-windows etc */

#define SCREENWIDE getmaxx()  /* Max. x-coordinate in 16-color EGA mode */
#define SCREENDEEP getmaxy()  /* Max. y-coordinate in 16-color EGA mode */

#define MANAGERBARDEEP  11    /* Depth of menu bar in Window Manager */
#define WINDOWBARDEEP   11    /* Depth of menu bar in active Window */
#define DROPSHADOW    4   /* Size of window shadows */

#define MENULINEDEEP    10    /* Depth of menu line */

#define MAXMENUITEM   8   /* Maximum number of menu items */
#define MENUCOUNT     7   /* Number of menus */
#define MENUTITLESIZE 16    /* Maximum length of menu title */

#define DispMinWidth    208 /* Minimum width of a display Window */
#define DispMinHeight 115 /* Minimum height of a display Window */

#define NAME_WHR_FILE   "FILE_OF.WHR"
#define NAME_PHO_FILE   "FILE_OF.PHO"
#define CONFIG_FILE   "PM5639.CFG"

#define ManagerBkColor    EGA_LIGHTGRAY
#define ManagerBkFill   SOLID_FILL
#define ManagerBarText    EGA_BLACK
#define ManagerBarColor   EGA_LIGHTGRAY
#define ManagerBarFill    SOLID_FILL
#define ManagerInvBarText EGA_WHITE
#define ManagerInvBarColor EGA_BLACK
#define ManagerInvBarFill SOLID_FILL

#define WindowBkColor   EGA_WHITE
#define WindowBkFill      SOLID_FILL
#define WindowBarText   EGA_BLACK
#define WindowBarColor    EGA_LIGHTBLUE
#define WindowBarFill   SOLID_FILL
#define WindowInvBarText  EGA_WHITE
#define WindowInvBarColor EGA_LIGHTGRAY
#define WindowInvBarFill  SOLID_FILL
#define WindowBarRestore  EGA_GREEN

#define MenuBkColor     EGA_WHITE
#define MenuBkFill      SOLID_FILL
#define MenuBarText     EGA_BLACK
#define MenuBarColor      MenuBkColor
#define MenuBarFill     SOLID_FILL
#define MenuInvBarText    EGA_WHITE
#define MenuInvBarColor   EGA_BLACK
#define MenuInvBarFill    SOLID_FILL
#define MenuOffText     EGA_DARKGRAY

#define ChkboxBarText   EGA_WHITE
#define ChkboxBarColor    EGA_GREEN
#define ChkboxText      EGA_BLACK
#define ChkboxRectangle   EGA_BLACK
#define ChkboxOnColor   EGA_BLACK
#define ChkboxOffColor    EGA_WHITE

#define ButColor        EGA_GREEN
#define ButFill       SOLID_FILL
#define ButOnText       EGA_WHITE
#define ButOffText      EGA_BLACK

#define MenuKeyColor      EGA_RED

#define CIETextColor      EGA_BLACK
#define RGBTextColor      EGA_BLACK

#define ftLfactor       3.43053
#define int_scale       8192

#define errorbeep()   putchar(7)

#define WARNING   0
#define ERROR     1
#define YES_NO      2
#define NOTICE      3
#define DISKERROR   4

#define INACTIVE    0
#define ACTIVE      0xFF

#define NO        0
#define YES       1

#define XYZ       0
#define RGB       1

/* Some extended keyboard characters */

#define kbEsc        0x011b
#define kbBack       0x0e08
#define kbCtrlBack   0x0e7f
#define kbShiftTab   0x0f01
#define kbTab        0x0f09
#define kbEnter      0x1c0d
#define kbAltX       0x2d00
#define kbSpace   0x3920
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
#define kbShiftUp   0x4801
#define kbPgUp       0x4900
#define kbGrayMinus  0x4a2d
#define kbLeft       0x4b00
#define kbShiftLeft   0x4b01
#define kbRight      0x4d00
#define kbShiftRight  0x4d01
#define kbGrayPlus   0x4e2b
#define kbEnd        0x4f00
#define kbDown       0x5000
#define kbShiftDown 0x5001
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
#define kbAltE       0x1200
#define kbAltF       0x2100
#define kbAltL       0x2600
#define kbAltS       0x1f00
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

/* enumerated types */

enum ButtonChoice { Ok=0, Restore, Save, Cancel, Yes, No, Print, Retry };
enum Displaymodes { CIEMode=0, RGBMode, HelpMode, SetupMode, RestoreMode,
              SaveMode, LearnMode, CIEMeasMode, RGBMeasMode,
              PrintMode, PrintPrintMode, FilePrintMode };
enum CIEmodes { xy=0, dxdy, u_v_, du_dv_, uv, dudv };
enum RGBmodes { refX=0, refY, refZ, refL, refA, refM };
enum RGBdisplays { XYZHor=0, XYZVer, RGBHor, RGBVer };
enum LUMunits { Cdm2=0, NIT, ftL };
enum ColorErrors { CIELUV=0, JND };
enum TextOnOff { TextOff=0, TextOn };
enum BarDefinition { HorBar=0, VerBar };
enum ZoomDefinition { LowGain=0, MedGain, HighGain };

/* defined structures */


typedef struct {
  char KuNo[9];     /* Ku Number phos. was learned with */
  char phoName[16];   /* Phosphor name */
  int pho[9];       /* Phosphor matrix */
} PHOSINFO;

typedef struct {
  char name[16];      /* White reference name */
  float whr[16];      /* x, y, u', v', u, v */
} WHRINFO;

typedef struct {
  int mode;
  char* txt1;
  char* txt2;
  char* txt3;
} ERRORINFO;

typedef struct FileNode {
  char kuno[9];
  char name[16];
  float val[9];
  struct FileNode* next;
} FILENODE;

typedef struct {
  int left, top, right, bottom;
} RECT;

typedef struct {
  char* name;
  int (*proc) ();
} MENUITEM;

typedef struct {
  int count;
  int titlestart;
  int menulength;
  int lastitem;
  char title[MENUTITLESIZE+1];
  MENUITEM item[MAXMENUITEM];
} MENU;

typedef struct {
  RECT frame;
  char* back;
} WINDOW;

typedef struct {
  RECT frame;
  char* text;
  int textx;
  int color;
} BUTTON;

typedef struct {
  RECT frame;
  int x, y;
  int length, depth;
  int count;
  int top;
  int ptr;
  char* base;
} LIST;

typedef struct {
  RECT frame;
  int x, y;
  int select;
  char* text;
} CHECKBOX;

typedef struct {
  RECT frame;
  int x, y;
  int length;
  int curpos;
  int (*proc)();
  char* text;
} EDITFIELD;


typedef struct {

  RECT  rect;

  int WinNo;

  int mode;
  char windowname[16];    /* Name of opened window */

  int IOError;          /* <> 0; error in received measurement */

  int sensor;           /* COM1 or COM2 */
  int running;          /* Measuring active or not */
  int height, width;      /* Height and width of ACTIVE display */

  IDINFO sensorid;        /* Sensor ID */
  WHRINFO whrinfo;        /* Active White Reference */
  MEASINFO meas;          /* IOerror, X, Y, Z, xy, u'v' & uv values */
  PHOSINFO P;           /* Phosphor matrix */

  int texton;           /* Show results also as text in display */
  int lumunit;          /* Specified luminance unit */
  int scalemeas;          /* Scalefactor for measurements ( in display) */
  float xscale, yscale;   /* Scale factor values for graphics */

  int DispMode;         /* xy-, u'v'- or uv-display ( or delta) */
                    /* Different modes in RGB  */

  /* Variables specific to CIE-display */

  RECT kvad;            /* Rectangle in which cross can resides */

  int targetx, targety;   /* Origo of CIE coordinate-system */
  int crossx, crossy;     /* Point to display cross */
  int xlen, ylen;       /* Length of cross in x- and y-direction */
  int dx, dy;           /* Deviation of cross from origo */

  int colorerror;       /* CIELUV or JND */
  float ccT;            /* Correlated color temperature */

  /* Variables specific to RGB-display */

  int RGBmode;          /* RGB or XYZ display ( hor/ver bars) */
                    /* Different modes in DispMode */

  float Xref, Yref, Zref;   /* Reference values */

  float ABSref;         /* Absolute reference value. This value */
                    /* is ALWAYS expressed in NIT, i.e. it */
                    /* has to be multiplied/divided with ftL_ */
                    /* factor, in order to obtain the correct */
                    /* value */

  int w[6];           /* Info. about width and length of bars */
  int minimum, maximum;   /*  Do. */
  int Xbar, Ybar, Zbar;   /* Info. about last bar-pos. in graphic*/

  float Xval, Yval, Zval;   /* Info. about last bar-pos. in value*/
} WININFO;


/* Procedures defined in GUI.C: */

void GraphAbort( char* Msg, int Err);
int initGraphics( void);
void initWindowManager( void);
void clearWindowManager( void);
void openWindow( WININFO* wptr, char* headline);
void closeWindow( void);
void addMenu( MENU* m);
int openMenu( void);
void closeMenu( void);
void closeMenuBar( void);
void InvMenuTitle( int index, int active);
void NextMenu( int kbValue);
void InvMenuText( int index, int active);
void NextMenuText( int kbValue);
void SetRect( RECT* r, int left, int top, int right, int bottom);
void drawSetupRect( int left, int top, int rows, char* str, int length);
int addCheckbox( CHECKBOX* cb, int x, int y, char* text);
int drawCheckbox( CHECKBOX* cb, int active);
int addButton( BUTTON* b, int left, int top, int right, int bottom,
                 char* tx, int color);
int drawButton( BUTTON* b, int active);
int addButtons( BUTTON* b0, BUTTON* b1, BUTTON* b2, RECT* r, int b0text);
int addEditfield( EDITFIELD* t, int x, int y, int l, char* p, int (*proc)());
int drawEditfield( EDITFIELD* t, int active);
int editEditfield( EDITFIELD* f, int c);
int addList( LIST* l, int x, int y, int width, int depth,
            int count, int top, char* base);
int drawList( LIST* l, int active);
int updateList( LIST* l, int kbKey);
int drawFileList( LIST* l, FILENODE* p, int active);
void drawDisplay( int n);
void drawWindows( void);


/* Procedures defined in GUI1.C: */

int WhrYmin( float x, float y);
int WhrYmax( float x, float y);
int openWhrWindow( int WhrMode, int helptxt);
int doRestoreWhr( void);
int doSaveWhr( void);
int doLearnWhr( void);
int LearnRedPhosphor( WININFO* wptr, FILENODE* F);
int LearnGreenPhosphor( WININFO* wptr, FILENODE* F);
int LearnBluePhosphor( WININFO* wptr, FILENODE* F);
void calculatePhosphor( PHOSINFO* new, FILENODE* tmp);
void drawLearnOk( int left, int top, int color);
int openPhosWindow( int PhosMode, int helptxt);
int doRestorePhosphor( void);
int doSavePhosphor( void);
int doLearnPhosphor( void);


/* Procedures defined in GUI2.C: */

void openCOMPort( int COMPort, WININFO* wptr);
void shutdownCOMPorts( void);
void clearSensorId( WININFO* wptr);
int doQuit( void);
int doClose( void);
int doCloseAll( void);
int doTextOnOff( void);
int doScaleUp( void);
int doScaleDown( void);
int doAutoScale( void);
int doNext( void);
int doTopWindow( int no);
int doMove( void);
void writeInfo( char* txt1, char* txt2, int x, int y);
void findDisplayOverlap( void);
void signalSensorStatus( void);
int toggleMeasuring( void);
int toggleFastMode( void);
int openInformation( void);
int openDispStatus( void);
int idle( void);
int GetKey( void);
int NumberTrap( int c);

/* Procedures defined in FILES.C: */

void initFilesSystem( char* filNavn);
int SaveRestoreWindow( int type, int mode, int helptxt);
int doSaveDisplay( void);
int doRestoreDisplay( void);
int doSaveProgram( void);
int doRestoreProgram( void);
int doSaveMeasurement( void);
int doRestoreMeasurement( void);
void printToMedia( FILE* stream);
void drawFilePrintWindow( void);
int printFilePrint( void);
int doPrintReport( void);
int isname( int c);
char* getfiles( char* spec, int* count, char* drivemap);
int displayDiskError( int diskErr);
FILE* openFile( char* filename, char* filemode, int errCode);
int testDisk( int n);
int diskErrorHandler( int errval, int ax, int bp, int si);
int restoreDir( char* s);
int chooseFile( char* spec, char* name, char* drivemap,
                      char* headline, int helptxt, int mode);


/* Procedures defined in DISPLAY.C: */

void updateccT( WININFO* newP);
void updateCIE( int n);
void writeCIE( int n);
void drawCIE( int n);
void CalcRGBReference( int n);
int drawRGBBar( WININFO* newP, float val, int oldval, int i, int color);
void updateRGB( int n);
void writeRGB( int n);
void drawRGB( int n);
int openSetupCIE( void);
int openSetupRGB( void);
int openSetup( void);
int openCIE_RGB( int mode);
int openCIE( void);
int openRGB( void);

/* Procedures defined in HELP.C: */

int initHelpSystem( void);
int GetHelpFile( fpos_t filepos);
int GetHelpIndex( void);
int openHelpIndex( void);
int openHelpHelp( void);
int openHelp( int helptxt);


/* Procedures defined in MESSAGE.C: */

int displayError( int errorno );
int doAbout( void);
void* displayMessage( int left, int top, int color, char* message);
