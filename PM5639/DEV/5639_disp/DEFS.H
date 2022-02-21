/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997             */
/*  Project:    PM5639 Display Unit                           */
/*  Module:   DEFS.H                                    */
/*  Author:   Preben Christiansen, DEV                        */
/*  Org. date:  910213                                    */
/*  Rev. date:  970822 KEn  DEV                             */
/*  Status:   Version 2.3                                 */
/*                                                  */
/*  Changes:                                            */
/*  970822 Released as version 2.3                            */
/*  951207 Released as version 2.2                            */
/*  951128 Added definitions: LockOFF and LockON                    */
/*  951109 Added definitions: contrast_default, size_of_allocbuf        */
/*  951107 Added definitions for BUTTONS                        */
/***************************************************************************/

/* CONSTANT DEFINITIONS */

#define uchar unsigned char
#define uint unsigned int

#define size_of_allocbuf  4500    /* Number of bytes reserved for white */
                        /*  refs etc. */

#define contrast_max 60         /* Maximal LCD contrast */
#define contrast_min 45         /* Minimal LCD contrast */
#define contrast_default 50     /* Default LCD contrast */

#define contrast_port XBYTE[0xE000] /* Addr. for contrast port */
#define controlcmd XBYTE[0xC001]    /* Addr. for command to LCD-ctrl */
#define controldat XBYTE[0xC000]    /* Addr. for data to LCD-RAM */
#define controlread XBYTE[0xC001] /* Addr. for read from LCD-RAM  */
#define controlread2 XBYTE[0xC011]  /* Addr. for a second read */
#define controlstatus XBYTE[0xC000] /* Addr. for contr. status read */
#define idle_delay 24000        /* Time-out for power (a 25ms) */
#define int_scale 8192          /* Scale value for phosphor matrices */
#define light_delay 7200        /* Time-out for light (a 25ms) */

#define measmax 5             /* No. of "Stored measurem." stores */
  /* OBS: if changed, then menu definition should also be changed */

#define menu_delay 2400         /* Time-out for menus (a 25ms) */
#define phosname1 "EBU"         /* Phosphor name 1 definition */
#define phosname2 "SMPTE'C'"      /* Phosphor name 2 definition */
#define phosabsmax 99         /* Max. no. of phosphors in C.Sensor */
#define power_down 1            /* flag for power_down wanted */

#define refmax 10             /* No. of "White references" stores */
  /* OBS: if changed, then menu definition should also be changed */

#define factormax 10            /* No. of "Conversion factors" stores */
  /* OBS: if changed, then menu definition should also be changed */

#define rep_start 10            /* Start value for rep_delay (speeder) */
#define rep1_start 100          /* Start value for rep_delay (speeder1) */
#define rgbbar_max 124          /* Max y coordinate for RGB columns */
#define rgbbar_min 24         /* Min y coordinate for RGB columns */
#define SUMADR (unsigned long) 0xFFFE /* Address of PROM check sum (fixed!) */

#define supmax 10             /* No. of "Store set-up" stores */
  /* OBS: if changed, then menu definition should also be changed */

#define scr1size 1024         /* Screen size in bytes */
#define scr2size 1024         /* Alternative screens size */
#define screen1max 0x03ff       /* Top address for screen */
#define screen2max 0x07ff       /* Top address for alternative screen */
#define targetx 30            /* Target position, x */
#define targety 93            /* Target position, y */
#define xaxismin 5            /* x,y axes in xyY mode: min x */
#define xaxismax 55           /* x,y axes in xyY mode: max x */
#define yaxismin 67           /* x,y axes in xyY mode: min y */
#define yaxismax 118            /* x,y axes in xyY mode: max y */

#define whrefxy_min 0.002   /* Min x or y value in "Learn White ref" */
#define whrefxy_max 0.9     /* Max x or y value in "Learn White ref" */
#define whrefuv_min 0.001   /* Min u' or v' value in "Learn White ref" */
#define whrefuv_max 0.65    /* Max u' or v' value in "Learn White ref" */

#define whx6500 .3127       /* D6500 x-coordinate */
#define why6500 .3291       /* D6500 y-coordinate */
#define whx3200 .423          /* 3200K x-coordinate */
#define why3200 .399          /* 3200K y-coordinate */
#define whx9300 .285          /* 9300K x-coordinate */
#define why9300 .293          /* 9300K y-coordinate */

#define whu6500 .1978       /* D6500 u' & u--coordinate */
#define whv_6500 .4683        /* D6500 v'-coordinate */
#define whv6500 (2./3.)*.4683   /* D6500 v-coordinate */

#define whu3200 .2437       /* 3200K u'- & u-coordinate */
#define whv_3200 .5173        /* 3200K v'-coordinate */
#define whv3200 (2./3.)*.5173   /* 3200K v-coordinate */

#define whu9300 .1917       /* 9300K u' & u--coordinate */
#define whv_9300 .4435        /* 9300K v'-coordinate */
#define whv9300 (2./3.)*.4435   /* 9300K v-coordinate */


#define default_cfactor 14.28571429
#define default_abs_rgb 100

#define PTVhead 1

#define CRThead 1
#define PRJhead 2

#define error 1
#define overflow 2
#define lowlight 3

#define MaxTable 23

#define xyY_mode 1
#define RGB_mode 2

#define LockOFF         0xFF
#define LockON            0x00

/* BUTTON DEFINITIONS */

#define UP_Button         0xF6
#define DOWN_Button       0xF5
#define ENTER_Button        0xF3
#define RGB_Button        0xEE
#define CIE_Button        0xED
#define ZOOM_Button       0xEB
#define FUNCT_Button        0xDE
#define RECALL_Button     0xDD
#define STORE_Button        0xDB
#define ON_OFF_Button     0xBE
#define ILLUM_Button        0xBD
#define RESET_Button        0xBB

#define RGB_DOWN_Buttons    0xE4
#define CIE_DOWN_Buttons    0xE5

#define RGB_UP_Buttons      0xE6
#define CIE_UP_Buttons      0xE4

#define ON_OFF_ENTER_Buttons  0xB2

#define ILLUM_UP_Buttons    0xB4
#define ILLUM_DOWN_Buttons    0xB5

#define ON_OFF_FUNCT_Buttons  0x1E

#define NO_Button         0xFF


/* PORT DEFINITIONS */

sbit batlow = P3^4;     /* Input:  0= turn on battery warning */
sbit feed_dog = P3^2;   /* Watchdog is feeded by changing state */
sbit lightoff = P3^5;   /* Output: 0= turn on light */
sbit protect = P3^6;      /* Input:  0= turn off power */


/* STRUCTURE DEFINITIONS */

struct phosp {          /* Stores for phosphor names */
  char name[9];       /* Phosphor name */
  char key;         /* Flag: 0= phosphor can not be selected */
};

struct whr {          /* Stores for white references */
  float x;            /* White reference x */
  float y;            /* White reference y */
  float u;            /* White reference u' and u */
  float v;            /* White reference v */
  float v_;         /* White reference v' */
  char name[11];        /* Reference name */
  char key;         /* Flag: 0= reference can not be selected */
};

struct unit {         /* Stores for conversion factors */
  float cfactor;        /* Con factor */
  char name[11];        /* Con Factor name */
  char key;         /* flag: 0= Con. Factor can not be selected */
};

struct status {       /* Stores for instrument set-ups */
  char phos_no;       /* Phosphor no. */
  char whref_no;        /* White reference no. */
  char cfactor_no;      /* Conversion factor no */
  char munit_no;        /* Measuring unit no. */
  char mode_no;       /* Measur. mode no.: 1= xyY, 2= RGB */
  char rgbmode_no;      /* RGB mode no. (1-9) */
  char xymode_no;     /* XY mode no */
  char mem_no;        /* Memory no. for stored measurement */
  char zoom;          /* Zoom value (1,5,25) */
  float abs_rgb;
  char toggle_setup;
};

struct meas
  {                   /* Stores for saving measurements */
  char mode_no;       /* Measur. mode no.: 1= xyY, 2= RGB */
  char xymode_no;
  float xval;         /* x value */
  float yval;         /* y value */
  float uval;         /* u and u' value */
  float vval;         /* v value */
  float v_val;        /* v' value */
  float dev;          /* Offset from target in CIELUV units */
  float yy;         /* Normalized luminance (true big Y) */
  float xm;         /* Red   measurem. mult. by xfact */
  float ym;         /* Green measurem. mult. by yfact */
  float zm;         /* Blue  measurem. mult. by zfact */
  float rbar;         /* Normalized red   bar value (0-2) */
  float gbar;         /* Normalized green bar value (0-2) */
  float bbar;         /*Normalized blue  bar value (0-2) */
  int dx;           /* x offset from target in xyY mode */
  int dy;           /* y offset from target in xyY mode */
  float deltax;
  float deltay;
  char zoom;          /* Zoom value (1,5,25) */
  char munit_no;        /* Measuring unit no. */
  char cfactor_no;      /* Conversion factor no */
  char mem_no;        /* Memory no. for stored measurement */
  char* whrefname;      /* Pointer to white reference name */
  char key;         /* Flag: 0= this store can not be selected */
  float abs_rgb;
  float colortemp;
  char rgbmode_no;
  };

struct tmp {          /* Store for temporary settings */
  char select;        /* Selected bar no. in menu */
  char txt[20];       /* Name being spelled */
  char charno;        /* Pos. of char currently being spelled */
};

struct avg {          /* Stores for samples for averaging */
  float x;            /* Sample from red channel */
  float y;            /* Sample from green challel */
  float z;            /* Sample from blue channel */
};

struct munit {          /* Stores for measuring units */
  float unitfact;     /* Factor for calcul. luminance */
  char name[6];       /* Units names */
};

struct menus {          /* Stores for menu informations */
  char no;            /* Menu no. */
  char maxline;       /* Last line no. in menu - 2  */
  char select;        /* Currently selected menu point */
  char* txtline[12];    /* Menu text lines */
};
