/*
  help.h
  include file for use fo procedures in HELP.C
*/


/* Size and location of HELP-window */

#define HlpLeft 90
#define HlpTop    60
#define HlpRight  550
#define HlpBottom 290
#define HlpWidth  ( HlpRight - HlpLeft)

#define NDXSIZE     100 /* No of MENU entries in help index-file */
#define HEADLINESIZE    40    /* Size of text for help-headline */

#define HELPBUFSIZE   400 /* No of helplines in ONE help-"file" */
#define HELPLINESIZE    60    /* Max. no of characters in one help-line */

#define HELP_FILE     "PM5639.HLP"
#define INDEX_FILE    "PM5639.NDX"

typedef struct {
  char headline[HEADLINESIZE];
  fpos_t filepos;
} NDXTYPE;


/* External variables used in HELP.C: */

extern MENU* Menu[MENUCOUNT];
extern int ActiveMenu;
extern int MenuMode;

/* External procedure used in HELP.C:

addButton
drawButton
addList
updateList
drawWindows
displayError
GetKey
*/

/* Variables defined in HELP.C: */

const int HlpIntroduction     = 1;
const int HlpFileInformation    = 12;

const int HlpEditSetupCIE     = 17;
const int HlpEditSetupRGB     = 18;
const int HlpEditStatus       = 20;
const int HlpEditReport       = 21;
const int HlpEditReportPrint    = 22;
const int HlpEditReportFile   = 23;

const int HlpLearnWhrRef      = 29;
const int HlpLearnPhosphor      = 30;

const int HlpSaveProgram      = 33;
const int HlpSaveDisplay      = 34;
const int HlpSaveMeasurement    = 35;
const int HlpSaveWhrRef       = 36;
const int HlpSavePhosphor     = 37;

const int HlpRestoreProgram   = 40;
const int HlpRestoreDisplay   = 41;
const int HlpRestoreMeasurement = 42;
const int HlpRestoreWhrRef      = 43;
const int HlpRestorePhosphor    = 44;

const int HlpHelpIndex        = 55;
const int HlpHelpHelpOnHelp   = 56;
