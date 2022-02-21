/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PM5639 Display Unit                            */
/*  Module:    TABLES.C                                    */
/*  Author:    Preben Christiansen, DEV                        */
/*  Org. date:  901224                                    */
/*  Rev. date:  970822, KEn  DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*        NONE                                        */
/*                                                  */
/* Changes:                                            */
/*  970822 Released as version 2.3                            */
/*  951207 Released as version 2.2                            */
/*  951109 Added variable: MasterResetLock                        */
/*  951107 Added upload and download in test ie. code struct menus menu[0]  */
/* 951030 Added new timer variable: GenericTimecount                */
/* 920520 Now supports projector head                          */
/* 920520 For earlier versions of history see u'v'-version            */
/***************************************************************************/
#pragma large

#include <reg51.h>      /* Define 8051 registers */
#include <stdio.h>      /* Define I/O functions */
#include <stdlib.h>      /* NULL definition */
#include <absacc.h>      /* Absolute addresses */
#include <defs.h>        /* Own definitions */
#include <tables.h>      /* Own definitions */

code char systset[] = {
  0x40, 0x30, 0x87, 0x07, 0x0F, 0xAE, 0x3F, 0x10, 0x00
};

code char scroll[] = {
  0x44, 0x00, 0x00, 0x40, 0x00, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00
};

code char csrform[] = {
  0x5D, 0x07, 0x87
};

code char *asc[] = {        /* ASCII-table */
"\2\0\0",              /* space */
NULL,                  /* ! */
NULL,                  /* " */
"\6\24\76\24\76\24\0",      /* # */
NULL,                  /* $ */
"\7\142\144\10\20\46\106\0",  /* % */
"\5\156\122\154\12\0",      /* & */
"\3\100\200\0",          /* ' */
"\3\74\102\0",            /* ( */
"\3\102\74\0",            /* ) */
"\4\24\10\24\0",          /* * */
"\4\10\34\10\0",          /* + */
"\3\1\6\0",              /* , */
"\4\10\10\10\0",          /* - */
"\2\2\0",              /* . */
"\4\7\30\340\0",          /* / */
"\4\74\102\74\0",          /* zero */
"\4\42\176\2\0",          /* 1 */
"\4\46\112\62\0",          /* 2 */
"\4\102\122\54\0",        /* 3 */
"\4\30\50\176\0",          /* 4 */
"\4\164\122\114\0",        /* 5 */
"\4\36\52\116\0",          /* 6 */
"\4\106\130\140\0",        /* 7 */
"\4\156\122\156\0",        /* 8 */
"\4\162\124\170\0",        /* 9 */
"\2\24\0",              /* : */
"\3\1\26\0",            /* ; */
"\4\10\24\42\0",          /* < */
"\4\24\24\24\0",          /* = */
"\4\42\24\10\0",          /* > */
NULL,                  /* ? */
NULL,                  /* @ */
"\5\76\110\110\76\0",      /* A */
"\5\176\122\122\54\0",      /* B */
"\4\74\102\102\0",        /* C */
"\4\176\102\74\0",        /* D */
"\4\176\122\102\0",        /* E */
"\4\176\120\100\0",        /* F */
"\5\74\102\112\54\0",      /* G */
"\4\176\20\176\0",        /* H */
"\4\102\176\102\0",        /* I */
"\4\4\102\174\0",          /* J */
"\5\176\10\24\142\0",      /* K */
"\4\176\2\2\0",          /* L */
"\6\176\40\30\40\176\0",    /* M */
"\5\176\60\14\176\0",      /* N */
"\5\74\102\102\74\0",      /* O */
"\4\176\110\170\0",        /* P */
"\5\74\102\104\72\0",      /* Q */
"\5\176\110\110\66\0",      /* R */
"\5\44\122\112\44\0",      /* S */
"\4\100\176\100\0",        /* T */
"\5\174\2\2\174\0",        /* U */
"\6\160\14\2\14\160\0",      /* V */
"\6\176\4\30\4\176\0",      /* W */
"\6\102\44\30\44\102\0",    /* X */
"\4\140\36\140\0",        /* Y */
"\5\106\112\122\142\0",      /* Z */
NULL,                  /* [ */
NULL,                  /* \ */
NULL,                  /* ] */
NULL,                  /* ^ */
"\5\01\01\01\01\0",        /* _ */
NULL,                  /* ` */
"\4\56\52\76\0",          /* a */
"\4\176\22\14\0",          /* b */
"\4\34\42\42\0",          /* c */
"\4\14\22\176\0",          /* d */
"\5\34\52\52\32\0",        /* e */
"\4\20\76\120\0",          /* f */
"\5\30\45\45\36\0",        /* g */
"\4\176\20\36\0",          /* h */
"\2\136\0",              /* i */
"\3\1\137\0",            /* j */
"\5\176\10\24\42\0",        /* k */
"\3\176\2\0",            /* l */
"\6\76\40\36\40\36\0",      /* m */
"\4\76\40\36\0",          /* n */
"\4\76\42\76\0",          /* o */
"\4\77\44\74\0",          /* p */
"\4\74\44\77\0",          /* q */
"\4\76\20\40\0",          /* r */
"\4\32\52\54\0",          /* s */
"\3\176\42\0",            /* t */
"\4\74\2\76\0",          /* u */
"\4\74\2\74\0",          /* v */
"\6\74\2\74\2\74\0",        /* w */
"\4\66\10\66\0",          /* x */
"\4\75\5\76\0",          /* y */
"\4\46\52\62\0",          /* z */
NULL,                  /* { */
"\4\77\42\77\0",          /* | (oe) */
NULL                  /* } */
};

/* 1+3 STORES FOR LUMINANCE UNIT CONSTANTS WITH NAMES */
/* NB! Present C51-compiler CAN'T handle scientific notation i.e. X.XE+-Y */

code struct munit measunit[] = {
  0, "",
  0.00001, "Cd/m2",            /*1.1203*/
  0.00001, "NIT",
  0.000002915, "ftL"          /*0.3266*/  /* 1 ftL = 3.43 NIT */
};

/* DEFINITION OF ALL MENUS */
/* The first three numbers defined as:
/*   1: Menu number */
/*    2: Number of active menu lines ( Header NOT included) */
/*    3: Default placing of "bar" in menu */
/*  then a menu-header and max. eleven entries */

code struct menus menu[] = {
  0,7,1,"Test","Version","Sensor ID","Check ROM","Check dots",
    "Non shut-down","Upload RAM","Download RAM","","","","",

  1,11,11,"RGB","R & B / G","G & B / R","R & G / B","RGB / luminance",
    "RGB /","RGB / mem #1","RGB / mem #2","RGB / mem #3",
    "RGB / mem #4","RGB / mem #5", "Exit",

  2,6,1,"Function","Status","CRT/phosphor","White reference",
    "Luminance unit","Chromaticity unit","Exit",
    "","","","","",

  3,10,9,"CRT/phosphor","Scroll","","","","","","","",
    "Exit","Scroll","",

  4,11,11,"White reference","1:","2:","3:","4:","5:","6:",
    "7:","8:","9:","10:","Exit",

  5,4,4,"Luminance unit","Candela/m2","NIT","foot-Lambert","Exit",
    "","","","","","","",

  6,9,1,"Status","CRT/ph:","Wh.ref:","       ","       ",
    "Lumi. unit:","Meas. mode:","","","Exit","","",

  7,9,1,"","","","","","","","","","","","",

  8,3,3,"Store","Store set-up #","Store measurem.","Exit","",
    "","","","","","","",

  9,6,6,"Store measurem.","Memory #1","Memory #2","Memory #3",
    "Memory #4","Memory #5","Exit","","","","","",

  10,11,11,"Store set-up","Set-up #1","Set-up #2","Set-up #3",
    "Set-up #4","Set-up #5","Set-up #6","Set-up #7",
    "Set-up #8","Set-up #9","Set-up #10","Exit",

  11,4,1,"Recall","Toggle setup 1/2","Recall set-up #",
    "Recall measurem.","Exit","","","","","","","",

  12,6,6,"Recall measurem.","Memory #1","Memory #2","Memory #3",
    "Memory #4","Memory #5","Exit","","","","","",

  13,11,11,"Recall set-up","Set-up #1","Set-up #2","Set-up #3",
    "Set-up #4","Set-up #5","Set-up #6","Set-up #7",
    "Set-up #8","Set-up #9","Set-up #10","Exit",

  14,4,4,"Learn","Set-up","CRT/phosphor","White reference","Exit",
    "","","","","","","",

  15,9,9,"Learn CRT/phosph.","Turn on one gun","at a time:","",
    "Red gun only","Green gun only","Blue gun only","",
    "Enter","Exit","","",

  16,10,9,"Store CRT/phosp.","Scroll","","","","","","","",
    "Exit","Scroll","",

  17,9,9,"Name CRT/phosp.","ABCDEF","GHIJKL","MNOPQR","STUVWX",
    "YZ 0123","456789","Space Delete /","Enter","Exit","","",

  18,5,5,"Learn white ref.","","","Use measurem.","Enter",
    "Exit","","","","","","",

  19,11,11,"Store white ref.","1:","2:","3:","4:","5:","6:","7:",
    "8:","9:","10:","Exit",

  20,9,9,"Name white ref.","ABCDEF","GHIJKL","MNOPQR","STUVWX",
    "YZ 0123","456789","Space Delete /","Enter","Exit","","",

  21,7,7,"Select and enter","A","B","C","D","E","F","Exit","","","","",
  22,7,7,"Select and enter","G","H","I","J","K","L","Exit","","","","",
  23,7,7,"Select and enter","M","N","O","P","Q","R","Exit","","","","",
  24,7,7,"Select and enter","S","T","U","V","W","X","Exit","","","","",
  25,7,7,"Select and enter","Y","Z","0","1","2","3","Exit","","","","",
  26,7,7,"Select and enter","4","5","6","7","8","9","Exit","","","","",
  27,4,4,"Select and enter","Space","Delete char","/","Exit","","","",
    "","","","",

  28,5,5,"","","",".","","","","","","","","",

  29,5,5,"Chromaticity unit","CIE 1931 xy","CIE 1931 dx dy",
    "CIE 1976 u'v'","CIE 1976 du' dv'", "Exit","","","","","","",

  30,11,11,"RGB","R & B / G","G & B / R","R & G / B","RGB / mean",
    "RGB /","RGB / mem #1","RGB / mem #2","RGB / mem #3",
    "RGB / mem #4","RGB / mem #5", "Exit",

  31,4,1,"Function","Status","White reference","Conversion ratio",
    "Exit","","","","","","","",

  32,11,11,"Conversion ratio","1:","2:","3:","4:","5:",
    "6:","7:","8:","9:","10:","Exit",

  33,9,1,"Status","Wh.ref:","Conv. ratio:","","Meas. mode:","",
    "","","","Exit","","",

  34,4,4,"Learn","Set-up","White reference","Conversion ratio","Exit",
    "","","","","","","",

  35,3,3,"Learn white ref.","Use measurem.","Enter","Exit","","","","",
    "","","","",

  36,9,9,"Name conv r. unit","ABCDEF","GHIJKL","MNOPQR","STUVWX",
    "YZ 0123","456789","Space Delete /","Enter","Exit","","",

  37,2,2,"Conv ratio source","Use measurem.","Exit","","","","","",
    "","","","",

  38,6,6,"Conversion ratio","   Measurement","         equals","","",
    "Enter","Exit","","","","","",

  39,11,11,"Store conv. ratio","1:","2:","3:","4:","5:",
    "6:","7:","8:","9:","10:","Exit",

  40,9,9,"","","","","","","","","","","","",

  41,9,1,"","","","","","","","","","","","",
};


/* TABLE FOR CORRELATED COLOR TEMPERATURE */

code float TT[MaxTable][5] =
  {
    /*  1/øK       u        v      slope   sqrt( 1+slope*slope) */
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


struct menus* menuptr;      /* Pointer to selected menu */
struct meas* msm[6];        /* 1 + 5 stores for measurements */
struct phosp* phos[101];    /* Stores for phosphor names and lock-key */
struct avg* sample;        /* Store sample of received xyz-values */
struct status* sup[13];      /* 1 + 10 stores for setups + 1 for default */
                    /* +1 for temporary use ( no 12) */
                    /* no 0. is actual temporary setup */

struct tmp* temp;          /* Stores for temp. values and text */
struct menus* testmenuptr;   /* Pointer to test menu */
struct whr* whref[12];      /* 1+10+1 stores for white ref. and names */
struct unit* cfactor[12];    /* 1+10+1 stores for con. factors and names */

char head_version;        /* Info. about meas. head, e.g. CRT or PRJ*/
char last_head_version;

char allocbuf[size_of_allocbuf];  /* Max reserved space in RAM */
                      /* CRT uses 2200, PRJ uses 1400 */

char* allocp;             /* Pointer to next free space */
                    /* Reserved space for date & head-info */

char datebuf[15] = "               ";
char* software_date = datebuf;  /* String that contains date of software */

bit bad_meas;            /* Flag: discard next measurement */
char* barptr[42];          /* Pointers to selected menu-bar no's */
bit batt_low;            /* Flag: battery is low */
bit busy;              /* Flag: RS232 is busy sending a char */

uchar but;              /* Code for pressed button(s) */
uchar butcmd;            /* Code for button command */

bit butenable;            /* Flag: next button code is accepted */

char* checkptr;          /* Pointer to ram check figure */
data char combi;          /* Register for combined buttons code */
bit combi_on;            /* Flag: combination button(s) pressed */

char* contrast;          /* Pointer to contrast code */
char crossx;            /* Dot position X in xyY-mode */
char crossy;            /* Dot position Y in xyY-mode */

uchar csum;              /* Byte for calculation of checksum */
bit err_disp;            /* Flag: error(s) are shown in display */
bit set_setup_no;          /* Flag: show recalled setup number */

char errorno;            /* Error type no. */
char* freeze;            /* Flag: display are a recall. measum. */
uint idle_cnt;            /* Counter for auto shut-down */
float k[9];              /* Coefficients for correction matrix */
float ki[12];            /* Inversed correction matrix */
char* kuno;              /* Pointer to present sensor KU number */
char* new_kuno;          /* Pointer to new sensor KU number */
char* laphr;            /* Time counter for elapsed time */
char* lapmin;            /* In non shut-down mode (testmode) */

char* lapsec;
char laptics;

uint light_cnt;          /* Counter for auto light-off */
bit light_exit;          /* Flag: light time-out has occured */
bit light_on;            /* Flag: light is on */

uint menu_cnt;            /* Counter for auto menu fall-out */

bit menu_exit;            /* Flag: menu time-out has occured */

char* menuno;            /* Pointer to current menu's no. */
char* menuon;            /* Flag: menu is on */
char* mother;            /* Ptr to spelling mother-menu(17 or20) */

bit newbut;              /* Flag: button code not same as before */
char newhead;            /* Flag: Sensor is changed */

int nn_br,nn_gr,nn_bg,nn_rg,nn_gb,nn_rb;      /* Phosp. matrix coeffic. */
int nn_brt,nn_grt,nn_bgt,nn_rgt,nn_gbt,nn_rbt;   /* Tempor. coeffic. */

char non_shdwn;          /* Flag for non shut-down mode (test) */
bit noscan;              /* Flag: Sensor did not send scan-rate */
char* number;            /* Ptr to Sens. 12dig.no. ( last 5dig) */
char* new_number;          /* Ptr to new Sens. 12dig. no. */
char oldbut;            /* current pressed button code */
uchar overload;          /* Flag: Too much light */
char phos_org;            /* Uppermost presented phos.no.in menu */
char phosmax;            /* Maximum no. of phosphors */
char prompts_on;          /* Flag for spell prompts on */
int ramsize;            /* Ramsize read from Sensor */
char readbin;            /* Flag and count for receiving binary */
char rep_cnt;            /* Speeder counter */
char rep_delay;          /* Current load value for speeder counter */
char rep1_delay;          /* Current load value for speeder counter */
bit repeat_on;            /* Flag: button(s) is repeating now */
char resat;         /* Flag: time is reset in test mode */
char rsno;          /* Index no. of received string (RS232) */
                    /*  selects r,g or b-string */

char* rgbptr[4];    /* Pointers to received strings */
                    /*  collects received bytes */

char rgbstring[4][20] = /* Receiving strings (collects chars) */
  {
  "                    ",
  "                    ",
  "                    ",
  "                    "
  };

float rref, gref, bref;       /* RGB value references */
char* sampleno;               /* Ptr to current sample no. for average */
char* samples;                /* Ptr to current sample value */
bit sending;                  /* Flag: string is being send (RS232) */
bit sendok;                   /* Flag: RS232 received RETURN (end) */
char sumon;                   /* Flag: C.Sensor uses checksum */
bit test_on;                  /* Flag: testmenu is wanted (at power-up) */
char testmode;                /* Test mode ( 0 is test menu) */
float unitfact;               /* Current multiply value for luminance */
float convfact;               /* Current multiply value for "units" in */
                              /*  projector*/
int xerr, yerr;               /* Dot deviation from target (xyY mode) */
float xfact, yfact, zfact;    /* Current pre-gain factors for RGB mode */

xdata char timecount;         /* Decremented each 25 ms. Used in main() ONLY */
xdata char GenericTimecount;  /* Decremented each 25 ms. Used in loops */
                              /*  which do NOT call any functions */

bit new_head_version;
uchar KLine = 0;

uchar MasterResetLock;   /* Enable/Disable using the master reset */
                         /*  buttons during start-up. */
uchar sensor_is_94;
