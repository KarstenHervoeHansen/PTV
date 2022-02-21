/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PM5639 Display Unit                            */
/*  Module:    SUB.C                                      */
/*  Author:    Preben Christiansen, DEV                        */
/*  Org. date:  901224                                    */
/*  Rev. date:  970822, KEn  DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*        void putcmd(char c);                             */
/*        void putdat(unsigned char c);                      */
/*        unsigned int readcuradr(void);                      */
/*        void setcuradr (unsigned int adr);                    */
/*        bit putpixel(char x, char y, char plan, char mode);        */
/*        void clrcross(void);                            */
/*        void putcross(int dx, int dy);                      */
/*        void outchar(char c);                            */
/*        void outtext(char* s);                            */
/*        void outtextxy(char x, char line, char* s);              */
/*        void textbar(char line, char mode);                  */
/*        void clrtext(char line);                          */
/*        void vline(char x, char ymin, char ymax, char mode);        */
/*        void hline(char xmin, char xmax, char y, char mode);        */
/*        void cls(void);                                 */
/*        void setline(void);                              */
/*        void drawbar(char barposx, float valmax, float valmin,\      */
/*                                       float val);    */
/*        void strput(char* txt, char* target);                  */
/*        char* ftoa(float f, char d);                        */
/*        void findorg(void);                              */
/*        void settestmenu(void);                          */
/*        void batt_warn(char x, char y);                      */
/*        void recall_whiteref(char no);                      */
/*        void store_whiteref(char no);                      */
/*        void recall_cfactor(char no);                      */
/*        void store_cfactor(char no);                        */
/*        void set_measunit(char no);                        */
/*        void store_setup(char no);                        */
/*        void recall_setup(char no);                        */
/*        void store_msm(char no);                          */
/*        void recall_msm(char no);                          */
/*        void invert_k(void);                            */
/*        void rgbref(char n);                            */
/*        void draw_refline(unsigned char zoom);                */
/*        void initmode(void);                            */
/*        void scopy(char* s, char* t, char no);                */
/*        void show_status(void);                          */
/*        char* ctos(unsigned char no);                      */
/*        void out_phos_name(void);                          */
/*        void out_ref_name(void);                          */
/*        void out_cfactor_name(void);                        */
/*        void bardown(void);                              */
/*        void barup(void);                              */
/*        void out_laptime(char line);                        */
/*        void shut_dwn(void);                            */
/*        char no_of_decimals(float value);                    */
/*        void setmenu(char n);                            */
/*                                                  */
/* Changes:                                            */
/*  970822 Released as version 2.3                            */
/*  951207 Released as version 2.2                            */
/*  951204 In function cls(); all reference to function putdat() and put-  */
/*       cmd() and setcuradr() has been replaced by code from the above  */
/* 951030 Removed writing the wrong white ref. in recall_msm RGB-mode.    */
/*       Remote control command, (to generator), now transmits GS10; and  */
/*       NOT GS;;. Correction in recall_setup()                  */
/*  951027 Added timeout to display controller polling in putdat() and     */
/*       putcmd(), (uses GenericTimecount)                      */
/* 920830 Added command send("GS#") to recall_setup to be used in the    */
/*       future with video generator                           */
/* 920520 Now supports projector head                           */
/* 920520 For earlier versions of history see u'v'-version             */
/***************************************************************************/
#include "pra.h"

#include <reg51.h>              /* Define 8051 registers */
#include <stdio.h>              /* Define I/O functions */
#include <stdlib.h>              /* Standard library */
#include <absacc.h>              /* Absolute addresses */
#include <math.h>                /* Math (abs,...) */
#include <sub.h>                /* Own definitions */
#include <com.h>                /* Own definitions */
#include <defs.h>                /* Own definitions */
#include <tables.h>              /* Own definitions */

char no_of_decimals(float value);
char floatstring[10];            /* For building string */


/* NB! STATIC VARIABLES ARE NOT KNOWN OUTSIDE THIS MODUL */

/* Global data common for drawbar(), initmode() */
static char ymax, ymin;

/* Global data common for invert_k(), out_phos_name(), out_ref_name(), */
/*    out_cfactor_name(), shut_dwn() */
static data unsigned char ic, jc, rc, sc;

/* Global data common for vline(), hline(), clrcross(), putcross() */
static data unsigned char xpos, ypos;

/* Global data for outchar(), outtext(), outtextxy(), textbar(), clrtxt() */
static data char ii, nn, *pp;

/***************************************************************************/
/*  putcmd                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951030                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write a command byte to the display controller            */
/*  Syntax:    void putcmd(char c);                          */
/*  Remarks:    Basic I/O for display controller                    */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void putcmd(char c) {

  feed_dog = 1;
  feed_dog = 0;

  GenericTimecount = 5;            /* Timeout after 5*25 ms */

  while ((controlstatus & 0x40) && GenericTimecount)
    ;                        /* Wait for controller ready */

  controlcmd = c;
}

/***************************************************************************/
/*  putdat                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951030                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write a date byte to the display controller              */
/*  Syntax:    void putdat(unsigned char c);                    */
/*  Remarks:    Basic I/O for display controller                    */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void putdat(unsigned char c) {

  feed_dog = 1;
  feed_dog = 0;

  GenericTimecount = 5;            /* Timeout after 5*25 ms */

  while ((controlstatus & 0x40) && GenericTimecount)
    ;                        /* Wait for controller ready */

  controldat = c;
}

/***************************************************************************/
/*  readcuradr                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Read the display cursor address                      */
/*  Syntax:    unsigned int readcuradr(void);                    */
/*  Remarks:    Basic I/O for the display                        */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
unsigned int readcuradr(void) {

  data unsigned int adr;

  while (controlstatus & 0x40)
    ;                        /* Wait for controller ready */
  controlcmd = 0x47;

  adr = controlread;              /* Low(adr) */
  adr += (int) controlread2 << 8;      /* Hi(adr) = 256 * controlread2; */

  return (adr);
}

/***************************************************************************/
/*  setcuradr                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write a new curos address to the display              */
/*  Syntax:    void setcuradr(unsigned int adr);                  */
/*  Remarks:    Basic I/O for the display                        */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void setcuradr(unsigned int adr) {

  while (controlstatus & 0x40)
    ;                        /* Wait for controller ready */
  controlcmd = 0x46;


  while (controlstatus & 0x40)
    ;
  controldat =  adr & 0x00ff;        /* Set low nipple af address */


  while (controlstatus & 0x40)
    ;
  controldat =  adr >> 8;            /* Set high nipple af address */
}

/***************************************************************************/
/*  putpixel                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Set, erase or toggle a dot in display                 */
/*  Syntax:    bit putpixel(char x, char y, char plan, char mode);      */
/*  Remarks:    Basic I/O for the display                        */
/*          DO NOT use putdat, putcmd, setcuradr; the stack-depth MAY   */
/*          become too deep                              */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
bit putpixel(char x, char y, char plan, char mode) {

  unsigned char n, getdis;
  unsigned int adr;

  feed_dog = 1;
  feed_dog = 0;

  if ((x>=0) && (x<64) && (y>=0)) {     /* Test if inside screen */
    n = 1 << (y % 8);              /* Form bit pattern for dot */

    adr = -y/8 - 16* (int)x;

    if (plan == 1 )
      adr += screen2max;          /* Plane 1 */
    else 
      adr += screen1max;          /* Plane 0 */


    /* SET CURSOR ADDRESS */

    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controlcmd = 0x46;            /* Command for set cursor address */

    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controldat =  adr & 0x00ff;      /* Set low nipple af address */

    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controldat =  adr >> 8;          /* Set high nipple af address */

    /* END OF SET CURSOR ADDRESS */


    /* GET CONTENTS OF DATA AT ADDRESS */

    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controlcmd = 0x43;            /* Command for read data at cursor */

    getdis = controlread;          /* Get data */

    /* END OF GET CONTENTS OF DATA AT ADDRESS */


    /* SET CURSOR ADDRESS */
    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controlcmd = 0x46;


    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controldat =  adr & 0x00ff;      /* Set low nipple af address */


    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controldat =  adr >> 8;          /* Set high nipple af address */


    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controlcmd = 0x42;
    /* END OF SET CURSOR ADDRESS */


    switch (mode) {
      case 0: 
        getdis &= ~n;            /* Clear dot */
        break;      
      case 1: 
        getdis |= n;            /* Set dot */
        break;    
      case 2: 
        getdis ^= n;            /* XOR dot */
        break;    
    }

    /* WRITE TO DATA AT ADDRESS */

    while (controlstatus & 0x40)
      ;                      /* Wait for controller ready */
    controldat = getdis;

    /* END OF WRITE TO DATA AT ADDRESS */

    return(1);                  /* OK */
  }
  else return(0);                /* not OK */
}

/***************************************************************************/
/*  clrcross                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Clear measurement "cross" in the xyY display            */
/*  Syntax:    void clrcross(void);                          */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void clrcross(void) {

  for (xpos=crossx-1; xpos<crossx+2; xpos++)
     for (ypos=crossy-1; ypos<crossy+2; ypos++)
      putpixel(xpos, ypos, 1, 0);                /* erase cross */
}

/***************************************************************************/
/*  putcross                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Set measurement "cross" in the xyY display            */
/*  Syntax:    void putcross(int dx, int dy);                    */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void putcross(int dx, int dy) {

  if ((abs(dx) < 30) && (abs(dy) < 30)) {

    crossx = targetx + (char)dx; 
    crossy = targety + (char)dy;

    for (xpos=crossx-1; xpos<crossx+2; xpos++)
      for (ypos=crossy-1; ypos<crossy+2; ypos++)
        putpixel(xpos, ypos, 1, 1);                /* set cross */
  }
}

/***************************************************************************/
/*  outchar                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Display character at current cursor                  */
/*  Syntax:    void outchar(char c);                          */
/*  Remarks:    I/O for the display                            */
/*          TO BE USED ONLY AFTER outtext() OR outtextxy()          */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void outchar(char c) {

  if ((c < ' ') || (c > 'z')) 
    c = '.';
  pp = asc[c-' '];            /* point to char in alfabet */
  nn = *pp; 
  pp++;
  for(ii=1; ii<= nn; ii++)       /* output char */
    putdat(*pp++);
}

/***************************************************************************/
/*  outtext                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Put character string in display                    */
/*  Syntax:    void outtext(char* s);                          */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void outtext(char *s) {
  char c;

  putcmd(0x42);
  while (c = *s++) {
    if ((c < ' ') || (c > 'z')) 
      c = '.';
    pp = asc[c-' '];          /* point to char in alfabet */
    nn = *pp; 
    pp++;
    for(ii=1; ii<= nn; ii++)     /* output char */
      putdat(*pp++);
  }
}

/***************************************************************************/
/*  outtextxy                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Put character string in display at x,y.              */
/*  Syntax:    void outtextxy(char x, char line, char* s);            */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void outtextxy(char x, char line, char *s) {

  char c;

  setcuradr(screen1max - 16 + line - 16 * (int)x);
  putcmd(0x42);

  while (c = *s++) {
    if ((c < ' ') || (c > 'z')) 
      c = '.';

    pp = asc[c-' '];          /* point to char in alfabet */
    nn = *pp;               /* Number of pixel columns to display */
    pp++;
    for(ii=1; ii<= nn; ii++)     /* output char */
      putdat(*pp++);
  }
}

/***************************************************************************/
/*  textbar                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Set, erase or toggle  a bar under a textline.          */
/*  Syntax:    void textbar(char line, char mode);                */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void textbar(char line, char mode) {

  char c;
  unsigned int adr;

  adr = screen2max - 16 + line;
  setcuradr(adr); 
  putcmd(0x42);
  switch (mode) {
     case 0: 
      for (nn=0; nn<64; nn++)         /* bar off */
        putdat(0); 
      break;
    case 1: 
      for (nn=0; nn<64; nn++)        /* bar on */
        putdat(0xff); 
      break;
    case 2: 
      setcuradr(adr);              /* set cursor adr. */
      putcmd(0x43);                /* header for read */
      c = controlread; 
      for (nn=0; nn<64; nn++)         /* toggle bar */
        putdat(~c); 
      break;
  }
}

/***************************************************************************/
/*  clrtext                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Clear a text line.                            */
/*  Syntax:    void clrtext(char line);                        */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void clrtext(char line) {
  unsigned int adr;

  adr = screen1max - 16 + line;
  setcuradr(adr); 
  putcmd(0x42);
  for (nn=0; nn<64; nn++) 
    putdat(0);
}

/***************************************************************************/
/*  vline                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw, erase or toggle a vertical line.                */
/*  Syntax:    void vline(char x, char ymin, char ymax, char mode);      */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void vline(char x, char ymin, char ymax, char mode) {

  for (ypos = ymin; ypos <= ymax; ypos++) 
    putpixel(x, ypos, 0, mode);
}

/***************************************************************************/
/*  hline                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw, erase or toggle a horizontal line.              */
/*  Syntax:    void hline(char xmin, char xmax, char y, char mode);      */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void hline(char xmin, char xmax, char y, char mode) {

  for (xpos = xmin; xpos <= xmax; xpos++) 
    putpixel(xpos, y, 0, mode);
}

/***************************************************************************/
/*  cls                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951204                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Clear the screen                              */
/*  Syntax:    void cls(void);                              */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void cls(void) {

  data unsigned int adr;

  while (controlstatus & 0x40)
    ;                  /* Wait for controller ready */

  controlcmd = 0x4C;        /* 0x4C is code for normal moving cursor */

                      /* Set start address screen 1 at 0x0000 */
  while (controlstatus & 0x40)  
    ;                  /* Wait for controller ready */
  controlcmd = 0x46;

  while (controlstatus & 0x40)
    ;
  controldat = 0x00;        /* Set low nipple screen 1 start address */

  while (controlstatus & 0x40)
    ;
  controldat = 0x00;        /* Set high nipple screen 1 start address */


  while (controlstatus & 0x40)
    ;                  /* Wait for controller ready */
  controlcmd = 0x42;

  for (adr = 0; adr<1024; adr++) {
    feed_dog = 1;
    feed_dog = 0;

    while (controlstatus & 0x40)
      ;                /* Wait for controller ready */
    controldat = 0x00;
  }

                      /* Set cursor address screen 2 at 0x0400 */
  while (controlstatus & 0x40)  
    ;                  /* Wait for controller ready */
  controlcmd = 0x46;

  while (controlstatus & 0x40)
    ;
  controldat = 0x00;        /* Set low nipple screen 2 start address */

  while (controlstatus & 0x40)
    ;
  controldat = 0x04;        /* Set high nipple screen 2 start address */


  while (controlstatus & 0x40)
    ;                  /* Wait for controller ready */
  controlcmd = 0x42;

  for (adr=0; adr<1024; adr++) {
    feed_dog = 1;
    feed_dog = 0;

    while (controlstatus & 0x40)
      ;                /* Wait for controller ready */
    controldat = 0x00;
  }
  
  while (controlstatus & 0x40)
    ;                  /* Wait for controller ready */
  controlcmd = 0x4E;        /* 0x4E is code for cursor moves north */
}

/***************************************************************************/
/*  setline                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw the line in the menu heads.                    */
/*  Syntax:    void setline(void);                            */
/*  Remarks:    I/O for the display                            */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void setline(void) {

  char i;

  setcuradr(screen1max - 14);

  putcmd(0x42);

  for (i=0; i<64; i++) 
    putdat(8);
}

/***************************************************************************/
/*  drawbar                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw a value bar in RGB display.                    */
/*  Syntax:    void drawbar(char barposx, char zoom, float val);        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void drawbar(char barposx, char zoom, float val) {

  int oldadr;
  float y;
  data char rowpos, r, x;        /* in rows 13 and 1 below */
  data unsigned char c;
  char *s, *t;
  float barmax, barmin;

  ymax = rgbbar_max+1;          /* max and min bar pos. */
  ymin = rgbbar_min+1;          /* REM: if these are altered, it is */
                        /* nessesary to change conversion of c */

  switch (zoom) {            /* determine max. and min. bar value */
    case 1:  barmax = 1.1; 
          barmin = 0.9; 
          s = "110%"; 
          t = "90%  ";
          break;
    case 5:  barmax = 1.25; 
          barmin = 0.75; 
          s = "125%"; 
          t = "75%  ";
          break;
    case 25:  barmax = 2.0; 
          barmin = 0.; 
          s = "200%"; 
          t = "0%    ";
          break;
  }

  outtextxy(38, 1, s);
  outtextxy(38, 13, t);                  /* write them to display */

  y = (((float)(ymax - ymin))/(barmax - barmin) * (val - barmin) +\
       (float)ymin + 0.5);
  if (y > ymax) 
    y = ymax;              /* keep inside bar box */
  if (y < ymin) 
    y = ymin;

  y = (int) y;

  oldadr = readcuradr();
  rowpos = 16 - ((char)y) /8;
  for (r=1; r<14; r++) {
    c = 0;
    setcuradr(screen1max - 16 + (int)r - 16 * (int)barposx);
    putcmd(0x42);
    if (r == rowpos) {
      c = ~(0xff << (char)y % 8);
      if (r == 13) 
        c |= 1;              /* draw in lower box line */
    }                      /* to be altered with rgbbar_min! */
    else 
      if (r > rowpos) 
        c = 0xff;            /* make black vert. stribe of 8 bit */
                          /* (in middle rows) */
    if (r == 1) {              /* draw in upper box line */
      c |= 0x10; 
      c &= 0x1f; 
    }
                          /* to be altered with rgbbar_max! */
    for (x=barposx; x<barposx+5; x++) 
      putdat(c);
  }
  setcuradr(oldadr);            /* restore text cursor */
}

/***************************************************************************/
/*  strput                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Copy a string to a target.                        */
/*  Syntax:    void strput(char* txt, char* target);                */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void strput(char* txt, char* target) {

  char *s = target;

  while (*(s++) = *(txt++))
    ;
}

/***************************************************************************/
/*  ftoa                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Convert floating point number to string with d decimals    */
/*  Syntax:    char* ftoa(ftoa f, char d);                      */
/*  Remarks:    floatstring is an assigned space for containing the      */
/*          converted float.                              */
/*  Returns:    A pointer to a string containing the converted number      */
/*  Updates:    ----                                      */
/***************************************************************************/
char* ftoa(float f, char d) {

  char *fptr;

  fptr = floatstring;                /* point to workstring */

  if (d == 0) 
    sprintf(fptr,"%5.0f",f);          /* no decimals */
  else 
    if (d == 1) 
      sprintf(fptr,"%5.1f",f);        /* 1 decimal */
  else 
    if (d == 2) 
      sprintf(fptr,"%5.2f",f);        /* 2 decimals */
  else 
    if (d == 3) 
      sprintf(fptr,"%5.3f",f);        /* 3 decimals */

  return(fptr);                    /* return pointer to the string */
}

/***************************************************************************/
/*  findorg                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Find top line phosphor no. for phosphor list display.      */
/*  Syntax:    void findorg(void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void findorg(void) {

  phos_org = sup[0]->phos_no - menu[3].maxline / 2 + 2;

  if (phos_org < 1) 
    phos_org = 1;
  if (phos_org > (phosmax - menu[3].maxline + 4))
    phos_org = phosmax- menu[3].maxline + 4;
}

/***************************************************************************/
/*  settestmenu                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw testmenu.                                */
/*  Syntax:    void settestmenu(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void settestmenu(void) {

  char i;

  cls();

  testmenuptr = &menu[0];
  testmode = 0;
  outtextxy(1, 1, testmenuptr->txtline[0]);      /* write headline */
  setline();                            /* draw line in head */

  for (i=1; i<12; i++)
    outtextxy(1, i+2, testmenuptr->txtline[i]);    /* write text lines */

  putcmd(0x5b);                /* set Overlay mode for display, xor */
  putdat(0x0d);

  textbar(*barptr[0] + 2,1);
  temp->select = *barptr[0];
}

/***************************************************************************/
/*  batt_warn                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw battery warning sign.                        */
/*  Syntax:    void batt_warn(char x, char y);                    */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void batt_warn(char x, char y) {

  hline(x, x+12, y, 1);
  hline(x, x+12, y-5, 1);
  vline(x, y-4, y-1, 1);
  vline(x+12, y-4, y-1, 1);
  vline(x-1, y-3, y-2, 1);
}

/***************************************************************************/
/*  recall_whiteref                                      */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Recall saved white ref.                          */
/*  Syntax:    void recall_whiteref(char no);                    */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void recall_whiteref(char no) {


  whref[0]->x = whref[no]->x;
  whref[0]->y = whref[no]->y; 
  whref[0]->u = whref[no]->u;
  whref[0]->v = whref[no]->v;
  whref[0]->v_ = whref[no]->v_;

  sup[0]->whref_no = no;          /* set current wh.ref no. */

  set_setup_no = 0;
}

/***************************************************************************/
/*  store_whiteref                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Save a new white reference as no.                  */
/*  Syntax:    void store_whiteref(char no);                    */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void store_whiteref(char no) {

  whref[no]->u = whref[6]->u;      /* save new reference */
  whref[no]->v = whref[6]->v; 
  whref[no]->v_ = whref[6]->v_; 
  whref[no]->x = whref[6]->x;      /* save new reference */
  whref[no]->y = whref[6]->y; 

  recall_whiteref(no);            /* make new reference active */
  *barptr[4] = no;              /* save new select bar position */
  strput(whref[no]->name, temp->txt);  /* copy name to temp. store */
  temp->charno = 0;
  whref[no]->key = 0x55;          /* unlock this wh.ref for selection */
  prompts_on = 0;              /* spell prompts off for now */
}

/***************************************************************************/
/*  recall_cfactor                                        */
/*                                                  */
/* Written by:  Kim Engedahl, DEV                               */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Recall a saved cfactor.                          */
/*  Syntax:    void recall_cfactor(char no);                    */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void recall_cfactor(char no) {

  sup[0]->cfactor_no = no;
  convfact = cfactor[no]->cfactor;

  set_setup_no = 0;
}

/***************************************************************************/
/*  store_cfactor                                        */
/*                                                  */
/* Written by:  Kim Engedahl, DEV                               */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Save a new con factor in no.                      */
/*  Syntax:    void store_cfactor(char no);                      */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void store_cfactor(char no) {

  cfactor[no]->cfactor = cfactor[11]->cfactor/(msm[0]->yy*unitfact);

  recall_cfactor(no);              /* make new con. factor active */
  *barptr[32] = no;                /* save new select bar position */
  strput(cfactor[no]->name, temp->txt);  /* copy name to temp. store */
  temp->charno = 0;
  cfactor[no]->key = 0x55;         /* unlock this cfactor for selection */
  prompts_on = 0;                /* spell prompts off for now */
}

/***************************************************************************/
/*  set_measunit                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Select measuring value units.                      */
/*  Syntax:    void set_measunit(char no);                      */
/*  Remarks:    NOTE!! This function can NOT be compiled with an optimize  */
/*          factor greater than four, 4.                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
/*#pragma optimize (4)*/
void set_measunit(char no) {

  unitfact = measunit[no].unitfact;
  sup[0]->munit_no = no;

  set_setup_no = 0;
}
/*#pragma optimize (6)*/

/***************************************************************************/
/*  store_setup                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Store a setup in no.                            */
/*  Syntax:    void store_setup(char no);                      */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void store_setup(char no) {

  sup[no]->phos_no = sup[0]->phos_no;      /* store current set-up as no. */
  sup[no]->whref_no = sup[0]->whref_no;
  sup[no]->munit_no = sup[0]->munit_no;
  sup[no]->mode_no = sup[0]->mode_no;
  sup[no]->cfactor_no = sup[0]->cfactor_no;
  sup[no]->mem_no = sup[0]->mem_no;
  sup[no]->zoom = sup[0]->zoom;
  sup[no]->xymode_no = sup[0]->xymode_no;
  sup[no]->abs_rgb = sup[0]->abs_rgb;

  switch (head_version) {
    case CRThead:
      sup[no]->rgbmode_no = *barptr[1];
      break;
    case PRJhead:
      sup[no]->rgbmode_no = *barptr[30];
      break;
  }

  set_setup_no = 0;
}

/***************************************************************************/
/*  recall_setup                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951030                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Recall a stored setup.                          */
/*  Syntax:    void recall_setup(char no);                      */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
#if 0
struct status* sup[13];      /* 1 + 10 stores for setups + 1 for default */
                             /* +1 for temporary use ( no 12) */
                             /* no 0. is actual temporary setup */
#endif
/***************************************************************************/
void recall_setup(char no)
   {
  char s[5] = "GS ;";

  *barptr[3] = sup[no]->phos_no;
  recall_phosphor(*barptr[3]);           /* Insert the new phosphor */

  *barptr[4] = sup[no]->whref_no;
  recall_whiteref(*barptr[4]);            /* Insert the new wh.ref */

  *barptr[5] = sup[no]->munit_no;          /* Set measuring unit */
  sup[0]->munit_no = *barptr[5];

  *barptr[32] = sup[no]->cfactor_no;
  recall_cfactor(*barptr[32]);

  sup[0]->mode_no = sup[no]->mode_no;        /* Set measuring mode */

  sup[0]->abs_rgb = sup[no]->abs_rgb;

  switch (head_version)
    {
    case CRThead:
      *barptr[1] = sup[no]->rgbmode_no;
      unitfact = measunit[sup[no]->munit_no].unitfact;
      convfact = 1.0;
      break;

    case PRJhead:
      *barptr[30] = sup[no]->rgbmode_no;
      break;
    }

  sup[0]->zoom   = sup[no]->zoom;            /* Set zoom */
  sup[0]->mem_no = sup[no]->mem_no;        /* Set last used mem no. */

  *barptr[29] = sup[0]->xymode_no = sup[no]->xymode_no;

  if (no == 10)            /* Transmit GS to generator IMMEDIATELY */
    sendi("GS10;");        /* What the fuck is generator?          */
  else
    {
    s[2] = 0x30 + no;
    sendi(s);
    }
  set_setup_no = 1;
  }

/***************************************************************************/
/*  store_msm                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Store measurement in memory no.                    */
/*  Syntax:    void store_msm(char no);                        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
#if 0

struct meas* msm[6];  /* 1 + 5 stores for measurements */

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
  float yy;           /* Normalized luminance (true big Y) */
  float xm;           /* Red   measurem. mult. by xfact */
  float ym;           /* Green measurem. mult. by yfact */
  float zm;           /* Blue  measurem. mult. by zfact */
  float rbar;         /* Normalized red   bar value (0-2) */
  float gbar;         /* Normalized green bar value (0-2) */
  float bbar;         /* Normalized blue  bar value (0-2) */
  int dx;             /* x offset from target in xyY mode */
  int dy;             /* y offset from target in xyY mode */
  float deltax;
  float deltay;
  char zoom;          /* Zoom value (1,5,25) */
  char munit_no;      /* Measuring unit no. */
  char cfactor_no;    /* Conversion factor no */
  char mem_no;        /* Memory no. for stored measurement */
  char* whrefname;    /* Pointer to white reference name */
  char key;           /* Flag: 0= this store can not be selected */
  float abs_rgb;
  float colortemp;
  char rgbmode_no;
  };
#endif
/***************************************************************************/
void store_msm(char no)
  {
  if ((msm[0]->xm / xfact) < -0.5)
    {                                           /* test if overload */
    outtextxy(28, menu[9].maxline+3, "Overload ");
    pause(255);
    pause(255);
    pause(255);
    clrtext(menu[9].maxline+3);
    }
  else
    {
    msm[no]->mode_no = sup[0]->mode_no;        /* save display mode no. */
    msm[no]->xm      = msm[0]->xm;              /* save measuring values */
    msm[no]->ym      = msm[0]->ym;
    msm[no]->zm      = msm[0]->zm;
    msm[no]->xval    = msm[0]->xval;
    msm[no]->yval    = msm[0]->yval;
    msm[no]->uval    = msm[0]->uval;
    msm[no]->vval    = msm[0]->vval;
    msm[no]->v_val   = msm[0]->v_val;
    msm[no]->dev     = msm[0]->dev;
    msm[no]->dx      = msm[0]->dx;        /* save deviations in xyY/uvY mode */
    msm[no]->dy      = msm[0]->dy;
    msm[no]->deltax  = msm[0]->deltax;
    msm[no]->deltay  = msm[0]->deltay;
    msm[no]->rbar    = msm[0]->rbar;              /* save rgb bars in rgb mode */
    msm[no]->gbar    = msm[0]->gbar;
    msm[no]->bbar    = msm[0]->bbar;
    msm[no]->zoom    = sup[0]->zoom;              /* save zoom value */
    msm[no]->yy      = msm[0]->yy;                  /* save luminance */
    msm[no]->munit_no   = sup[0]->munit_no;      /* save measur. unit */
    msm[no]->cfactor_no = sup[0]->cfactor_no;  /* save conv. factor no */
    msm[no]->mem_no     = sup[0]->mem_no;          /* save last used mem no. */
    msm[no]->whrefname  = whref[sup[0]->whref_no]->name;
    msm[no]->xymode_no  = sup[0]->xymode_no;
    msm[no]->abs_rgb    = sup[0]->abs_rgb;
    msm[no]->colortemp  = msm[0]->colortemp;
    msm[no]->key        = 0x55;

    switch (head_version)
      {
      case CRThead:
        msm[no]->rgbmode_no = *barptr[1];
        break;
      case PRJhead:

        msm[no]->rgbmode_no = *barptr[30];
        break;
      }
    }
  }

/***************************************************************************/
/*  recall_msm                                          */
/*                                                  */
/*  Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:        901224                                    */
/*  Revised:     951030                                    */
/*                                                  */
/*  Module:      SUB.C                                      */
/*  Function:    Recall a stored measurement.                      */
/*  Syntax:      void recall_msm(char no);                        */
/*  Remarks:      ----                                      */
/*  Returns:     Nothing                                    */
/*  Updates:      ----                                      */
/*               14.09.2004 Luminance multiplied by 10 in case of 94 sensor*/
/***************************************************************************/
void recall_msm(char no)
  {
  char res, decimals;
  float barmax, barmin, rgb_tmp, out1, out2;

  char barptr_tmp;
  char begin_line;
  char* s;

  bit tmp_setup_no;

  if (msm[no]->key == 0x55)
    {                              /* only if not locked */
    tmp_setup_no = set_setup_no;
    set_setup_no = 0;

    store_setup(12);

    sup[0]->xymode_no  = msm[no]->xymode_no;
    sup[0]->abs_rgb    = msm[no]->abs_rgb;
    sup[0]->munit_no   = msm[no]->munit_no;
    sup[0]->zoom       = msm[no]->zoom;
    sup[0]->cfactor_no = msm[no]->cfactor_no;

    if (msm[no]->mode_no == RGB_mode)
      {                                  /* if in RGB mode */
      switch (head_version)
        {
        case CRThead:
          barptr_tmp = *barptr[1];
          *barptr[1] = msm[no]->rgbmode_no;
          break;
        case PRJhead:
          barptr_tmp  = *barptr[30];
          *barptr[30] = msm[no]->rgbmode_no;
          break;
        }  
      sup[0]->mode_no = RGB_mode;
      *freeze = 1; /* function initmode() has the line below:            */
      initmode();  /* outtextxy(43, 9, measunit[sup[0]->munit_no].name); */
      *freeze = 1;

      switch (head_version)
        {
        case CRThead:
          *barptr[1] = barptr_tmp;
          decimals = 1;
          if (msm[no]->munit_no == 3) 
            decimals += 1;
         /* function below probably calculates and writes Luminance value for RGB mode */
         if (sensor_is_94)    /* Change for 94 sensor JK */
           outtextxy(16, 15, ftoa(msm[no]->yy * measunit[msm[no]->munit_no].unitfact * 10.0, decimals));
         else
           outtextxy(16, 15, ftoa(msm[no]->yy * measunit[msm[no]->munit_no].unitfact, decimals));
          /* Below probably unit for luminance is written in RGB mode */
          outtextxy(40, 15, measunit[msm[no]->munit_no].name);
          break;

        case PRJhead:
          *barptr[30] = barptr_tmp;
          rgb_tmp = msm[no]->yy * measunit[msm[no]->munit_no].unitfact* cfactor[msm[no]->cfactor_no]->cfactor;
          if (((rgb_tmp < 0.01) || (rgb_tmp > 99999)))
            {
            if (rgb_tmp < 0.01) 
              s = "< 0.01 ";
            else
              s =  "> 99999 ";
            outtextxy(16, 15, s);
            }
          else
            outtextxy(16, 15, ftoa(rgb_tmp, no_of_decimals(rgb_tmp)));

          outtextxy(40, 15, cfactor[msm[no]->cfactor_no].name);
          break;
        }  
      if ((barptr_tmp < 5) )
        {                                        /* and wh.ref name */
        outtextxy(4, 16, "Ref: ");
        outtext(msm[no]->whrefname);
        }

      outtextxy(38, 4, "MEM#");
      outchar('0' + no);

      drawbar(8,  msm[no]->zoom, msm[no]->rbar); /* draw the bars */
      drawbar(18, msm[no]->zoom, msm[no]->gbar);
      drawbar(28, msm[no]->zoom, msm[no]->bbar);
      }
    else
      {                               /* if in xyY-mode */
      sup[0]->mode_no = xyY_mode;
      *freeze = 1;                  /* char* freeze Flag: display are a recall. measum. */
      initmode();                          /* in init of mode */
      *freeze = 1;
      outtextxy(2, 15, "Ref: ");
      outtext(msm[no]->whrefname);
      if (msm[no]->colortemp == 0)
        {
        outtextxy(11, 14, " ");
        outtext("----");
        }
      else
        outtextxy(11, 14, ftoa(msm[no]->colortemp, 0));

      bad_meas = 1;                /* do not use next measurement */
      if (msm[no]->yy >= 0.1 / measunit[1].unitfact)
        {                                          /* if low light */
        putcross(msm[no]->dx, msm[no]->dy);        /* then output  */
        switch (msm[no]->xymode_no)
          { 
          case 1:
            begin_line = 12;
            out1 = msm[no]->xval;
            out2 = msm[no]->yval;
            break;
          case 3:
            begin_line = 15;
            out1 = msm[no]->uval;
            out2 = msm[no]->v_val;
            break;
          case 5:
            begin_line = 12;
            out1 = msm[no]->uval;
            out2 = msm[no]->vval;
            break;
          case 2:
          case 6:
            begin_line = 15;
            break;
          case 4:
            begin_line = 18;
            break;
          }

        switch (msm[no]->xymode_no)
          { 
          case 1:
          case 3:
          case 5:
            outtextxy(begin_line, 10, ftoa(out1, 3));
            outtextxy(begin_line, 11, ftoa(out2, 3));
            break;
          case 2:
          case 4:
          case 6:
            if (msm[no]->deltax > 0 )
              outtextxy(begin_line, 10, "+");
            else
              outtextxy(begin_line, 10, "-");

            if (msm[no]->deltay > 0 )
              outtextxy(begin_line, 11, "+");
            else
              outtextxy(begin_line, 11, "-");

            outtextxy(begin_line+5, 10, ftoa(fabs(msm[no]->deltax), 3));
            outtextxy(begin_line+5, 11, ftoa(fabs(msm[no]->deltay), 3));
            break;
          }
        }

      decimals = 1;
      if (msm[no]->munit_no == 3) 
        decimals++;
      /* function below probably calculates and writes Luminance value in xy mode */
      if (sensor_is_94)    /* Change for 94 sensor JK */
        outtextxy(14, 12, ftoa(msm[no]->yy * measunit[msm[no]->munit_no].unitfact * 10.0, decimals));
      else
        outtextxy(14, 12, ftoa(msm[no]->yy * measunit[msm[no]->munit_no].unitfact, decimals));
      /* Below probably unit for luminance is written for xy mode */
      outtextxy(36, 12, measunit[msm[no]->munit_no].name);
                                   
      if (msm[no]->yy >= 0.1 / measunit[1].unitfact)   /* if not low light */
        outtextxy(16, 13, ftoa(msm[no]->dev, 0));      /* write deviation in CIELUV */

      outtextxy(38, 1, "MEM#");                        /* then write mem. no. */
      outchar('0' + no);
      }

    recall_setup(12);              /* What is this? */
    set_setup_no = tmp_setup_no;

    msm[0]->xval  = msm[no]->xval;          /* accept as current values */
    msm[0]->yval  = msm[no]->yval;
    msm[0]->uval  = msm[no]->uval;          /* accept as current values */
    msm[0]->vval  = msm[no]->vval;
    msm[0]->v_val = msm[no]->v_val;
    msm[0]->yy    = msm[no]->yy;
  }
}

/***************************************************************************/
/*  invert_k                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Calculate inverse of correction matrix.              */
/*  Syntax:    void invert_k(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void invert_k(void)
  {
  float t;

  for (rc = 0; rc < 3 ; rc++)                  /* r is row and s in column */
    for (sc = 0; sc < 3; sc++)
      ki[rc + sc * 3] = k[rc + sc * 3];

  for (ic = 0; ic < 3; ic++)
    {
    ki[ic + 9] = 1.;
    t = ki[ic];
    for (sc = 1; sc < 4; sc++)
      ki[ic + (sc - 1)*3] = ki[ic + sc * 3] / t;
    for (rc = 0; rc < 3; rc++)
      {
      if (ic == rc) 
        continue;
      ki[rc + 9] = 0.;
      t = ki[rc];
      for (sc = 1; sc < 4; sc++)
        ki[rc + (sc - 1)*3] = ki[rc + sc * 3] - t * ki[ic + (sc - 1)*3];
      }
    }
  }

/***************************************************************************/
/*  rgbref                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Calculate references for RGB bars.                  */
/*  Syntax:    void rgbref(char n);                          */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void rgbref(char n)
  {
  float xxr, yyr, zzr, xmref, ymref, zmref, xxyyzz, temp, tmp;

                /* calculate backwards from an arbitrary luminance value */
  switch (n)    /* (here 90) and the selected x,y value and find the */
    {
    case 1:  ;  /* equivalent Color Sensor values by multiplying with */
    case 2:  ;  /* the inverse correction matrix, skale with pregain */
    case 3:  ;  /* factors and multiply with phosphor matrix */
    case 4:  ;
    case 5:            
                            /* xxr is true X, yyr=Y and zzr=Z */
      yyr = 90.0 / (convfact*unitfact);
      xxyyzz = yyr / whref[0]->y;
      xxr = whref[0]->x * xxyyzz;
      zzr = xxyyzz - xxr - yyr;
                            /* multiply with inverse K-matrix */
      xmref = (ki[0]*xxr + ki[3]*yyr + ki[6]*zzr) * xfact;
      ymref = (ki[1]*xxr + ki[4]*yyr + ki[7]*zzr);
      zmref = (ki[2]*xxr + ki[5]*yyr + ki[8]*zzr) * zfact;
      break;
    case 6:  ;
    case 7:  ;
    case 8:  ;
    case 9:  ;
    case 10:  
      xmref = msm[n-5]->xm;
      ymref = msm[n-5]->ym;
      zmref = msm[n-5]->zm;
      break;
    }
                            /* multiply with Phosphor-matrix */
  rref = int_scale * xmref + nn_gr * ymref     + nn_br * zmref;
  gref = nn_rg * xmref     + int_scale * ymref + nn_bg * zmref;
  bref = nn_rb * xmref     + nn_gb * ymref     + int_scale * zmref;
  }

/***************************************************************************/
/*  draw_refline                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw reference line for xyY mode.                  */
/*  Syntax:    void draw_refline(unsigned char zoom);              */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void draw_refline(unsigned char zoom) {

  unsigned char res;

  if ((sup[0]->mode_no == xyY_mode) || (*freeze)) {  /* if xyY mode */

    res = 25/zoom;

    hline(5, 55, yaxismin-2, 0);        /* erase resolution line */
    hline(5, 55, yaxismin-3, 0);
    putpixel(5, yaxismin-4, 0, 0);
    putpixel(55, yaxismin-4, 0, 0);
    hline(30-res ,30+res, yaxismin-3, 1);  /* set resolution line */
    putpixel(30-res, yaxismin-2, 0, 1);  /* and -dots */
    putpixel(30+res, yaxismin-2, 0, 1);

    if (res == 25) {                /* draw tics in highest resol. */
      putpixel(15, yaxismin-2, 0, 1);
      putpixel(25, yaxismin-2, 0, 1);
      putpixel(35, yaxismin-2, 0, 1);
      putpixel(45, yaxismin-2, 0, 1);
      putpixel(5, yaxismin-4, 0, 1);
      putpixel(55, yaxismin-4, 0, 1);
    }
  }
}

/***************************************************************************/
/*  initmode                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Initialize display for measuring mode                */
/*  Syntax:    void initmode(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void initmode() {

  char barposx,i,j,row;
  char barptr_tmp, decimals;
  float rgb_tmp;
  char *s = "   ";
  char *ch1;
  char *ch2;
  unsigned char c1,c2;

  cls();
  putcmd(0x5b);
  putdat(0x0c);                      /* set layer addition */

  if (sup[0]->mode_no == xyY_mode)
    {    /* set up screen for xyY display */
    vline(xaxismin,yaxismin,yaxismax,1);    /* draw axes */
    hline(xaxismin+1,xaxismax,yaxismin,1);

    vline(targetx-2,targety-2,targety+2,1);  /* draw target */
    vline(targetx+2,targety-2,targety+2,1);
    hline(targetx-1,targetx+1,targety-2,1);
    hline(targetx-1,targetx+1,targety+2,1);

    switch (sup[0]->xymode_no) {
      case 1:
      case 2:
        ch1 = "x";              /* write axes names */
        ch2 = "y";
  
        outtextxy(29, 2, "G");
        outtextxy(54, 5, "R");
        outtextxy(12, 7, "B");
        break;

      case 3:
      case 4:
        ch1 = "u'";
        ch2 = "v'";

        outtextxy(10, 2, "G");
        outtextxy(52, 4, "R");
        outtextxy(30, 7, "B");
        break;

      case 5:
      case 6:
        ch1 = "u";
        ch2 = "v";

        outtextxy(10, 2, "G");
        outtextxy(52, 4, "R");
        outtextxy(30, 7, "B");
        break;
    }

    outtextxy(xaxismax+3, 8, ch1);        /* write text for axes */
    outtextxy(xaxismin-1, 1, ch2);

    switch (sup[0]->xymode_no) {
      case 1:
        ch1 = "x=";
        ch2 = "y=";
        break;
      case 2:
        ch1 = "dx=";
        ch2 = "dy=";
        break;
      case 3:
        ch1 = "u'=";
        ch2 = "v'=";
        break;
      case 4:
        ch1 = "du'=";
        ch2 = "dv'=";
        break;
      case 5:
        ch1 = "u=";
        ch2 = "v=";
        break;
      case 6:
        ch1 = "du=";
        ch2 = "dv=";
        break;
    }

    outtextxy(2, 10, ch1);
    outtextxy(2, 11, ch2);

    outtextxy(2, 12, "Y=");
    outtextxy(2, 13, "dE=");
    outtextxy(36, 13, "CIELUV");

    outtextxy(2, 14, "T=");
    outtextxy(36, 14, "Kelvin");

    if (!*freeze) {
      outtextxy(2, 15, "Ref: ");
      outtext(whref[sup[0]->whref_no]->name);
    }

    draw_refline(sup[0]->zoom);        /* draw reference line */

    sup[0]->mode_no = xyY_mode;

    outtextxy(24, 9, "0.05");
    crossx = targetx; 
    crossy = targety;                /* initiate "cross" pos. */

    if ((*barptr[13] != menu[13].maxline) && set_setup_no) {
      outtextxy(49, 1, "#");
      if (*barptr[13] < 10)
        outchar('0' + *barptr[13]);
      else {
        outchar('1');
        outchar('0');
        }
      }
    }
  else
    {                    /* set up screen for RGB display */
    ymax = rgbbar_max;
    ymin = rgbbar_min;

    for (row=1;row<15;row++) {        /* draw boxes */
      setcuradr(screen1max - 16 + row);
      putcmd(0x42);
      barposx = 0;
      if (((ymax >> 3) > 16 - row) && ((ymin >> 3) < 16 - row)) {
        c1 = 0xff;
        c2 = 0;
        barposx = 8;
      }
      else 
        if ((ymax >> 3) == 16 - row) {
          c1 = ~(0xff << ((ymax & 0x07)+1));  /* top row */
          c2 = 0x01 << (ymax & 0x07);
          barposx = 8;
        }
      else 
        if ((ymin >> 3) == 16 - row) {
          c1 = 0xff << (ymin & 0x07);    /* bottom row */
          c2 = 0x01 << (ymin & 0x07);
          barposx = 8;
        }

      if (barposx) {
        for (i=1; i<barposx; i++) 
          putdat(0);
        for (j=0; j<3; j++) {
          putdat(c1);

          for (i=0; i<5; i++) 
            putdat(c2);

          putdat(c1);
          for (i=0; i<3; i++) 
            putdat(0);
        }
      }
    }
    for (barposx=8; barposx<30; barposx += 10) {
      for (i=1; i<10; i++) {                    /* set tic marks */
        putpixel(barposx+6, ymin + 10*i, 0, 1);
        if (i==5 && barposx<20) {
          putpixel(barposx+7, ymin + 10*i, 0, 1);
          putpixel(barposx+8, ymin + 10*i, 0, 1);
        }
      }
    }
    outtextxy(38, 7, "Ref:");                      /* write text */

    switch (head_version) {
      case CRThead:
        barptr_tmp = *barptr[1];
        break;
      case PRJhead:
        barptr_tmp = *barptr[30];
        break;
    }

    switch (barptr_tmp) {
      case 1:
        s = "  G";
        break;
      case 2:
        s = "  R";
        break;
      case 3:
        s = "  B";
        break;
      case 4:
        switch(head_version) {
          case CRThead:
            s = "Lum";
            break;
          case PRJhead:
            s = "mean";
            break;
        }
        break;
      case 5:
        rgb_tmp = (sup[0]->abs_rgb*convfact*unitfact)/measunit[1].unitfact;

        switch (head_version) {
          case CRThead:

            outtextxy(38, 8 ,ftoa(rgb_tmp, no_of_decimals(rgb_tmp)));

            outtextxy(43, 9, measunit[sup[0]->munit_no].name); 
            break;

          case PRJhead:
            if ((rgb_tmp > 99999) || (rgb_tmp < 0.01))
              outtextxy(38, 8 , "---- ");
            else
              outtextxy(38, 8 ,ftoa(rgb_tmp, 
                              no_of_decimals(rgb_tmp)));
  
            outtextxy(43, 9, cfactor[sup[0]->cfactor_no]->name);
            break;
        }
        s = "";
        break;
      case 6:
        s = " #1";
        break;
      case 7:
        s = " #2";
        break;
      case 8:
        s = " #3";
        break;
      case 9:
        s = " #4";
        break;
      case 10:
        s = " #5";
        break;
    }

    rgbref(barptr_tmp);           /* calculate references */

    outtextxy(45,  8,  s);
    outtextxy( 8, 14, "R");
    outtextxy(18, 14, "G");
    outtextxy(28, 14, "B");
    outtextxy( 4, 15, "Y=");

    sup[0]->mode_no = RGB_mode;    /* set mode 2 */

    if ((*barptr[13] != menu[13].maxline) && set_setup_no) {
      outtextxy(49, 2, "#");
      if (*barptr[13] < 10)
        outchar('0' + *barptr[13]);
      else {
        outchar('1');
        outchar('0');
        }
      }
    }
  *freeze = 0;                /* turn off recall mode */
  *menuon = menu_cnt = 0;          /* menu off & immediate menu fall-out */
  }

/***************************************************************************/
/*  scopy                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Copy string s to t for no. characters.                */
/*  Syntax:    void scopy(char* s, char* t, char no);              */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void scopy(char* s, char* t, char no) {

  char i;

  for (i=0; i<no; i++) 
    *t++ = *s++;
  *t='\0';
}

/***************************************************************************/
/*  show_status                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write status values in status menu.                  */
/*  Syntax:    void show_status(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void show_status(void) {

  float rgb_tmp, out1, out2;
  char whref_no;

  whref_no = sup[0]->whref_no;

  switch (head_version) {
    case CRThead:

      outtextxy(28, 3, phos[sup[0]->phos_no]->name);
      outtextxy(28, 4, whref[whref_no]->name);
      outtextxy(43, 7, measunit[sup[0]->munit_no].name);

      switch (sup[0]->xymode_no) {
        case 1:
        case 2:
          out1 = whref[whref_no]->x;
          out2 = whref[whref_no]->y;
          break;
        case 3:
        case 4:
          out1 = whref[whref_no]->u;
          out2 = whref[whref_no]->v_;
          break;
        case 5:
        case 6:
          out1 = whref[whref_no]->u;
          out2 = whref[whref_no]->v;
          break;
      }
      outtextxy(28, 5, ftoa(out1, 3));
      outtextxy(28, 6, ftoa(out2, 3));

      if (sup[0]->mode_no == RGB_mode) {
        outtextxy(2, 9, menu[1].txtline[*barptr[1]]);

        if (*barptr[1] == 5) {
          rgb_tmp = sup[0]->abs_rgb*unitfact/measunit[1].unitfact;

          outtext(ftoa(rgb_tmp, no_of_decimals(rgb_tmp)));
          outtext(" ");
          outtext(measunit[sup[0]->munit_no].name);
        }
      }
      else 
        outtextxy(5, 9, menu[29].txtline[*barptr[29]]);
      break;

    case PRJhead:

      outtextxy(29, 3, whref[whref_no]->name);
      outtextxy(2, 5, "#");

      if (sup[0]->cfactor_no < 10) {
        outchar('0' + sup[0]->cfactor_no);
        outtext(": ");
      }
      else {
        outchar('1');
        outchar('0');
        outtext(": ");
      }

      outtext(cfactor[sup[0]->cfactor_no]->name);
      outtextxy(2, 7, menu[30].txtline[*barptr[30]]);

      if (*barptr[30] == 5) {
        rgb_tmp = sup[0]->abs_rgb*(unitfact*convfact)/measunit[1].unitfact;

        if ((rgb_tmp > 99999) || (rgb_tmp < 0.01))
          outtextxy(23, 7 , "---- ");
        else
          outtextxy(23, 7 ,ftoa(rgb_tmp, no_of_decimals(rgb_tmp)));

        outtext(" ");
        outtext(cfactor[sup[0]->cfactor_no]->name);
      }
      break;
  }
}

/***************************************************************************/
/*  ctos                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Take an unsigned char and convert it to a string where each  */
/*         leading zero is replaced by 2 spaces which in printout     */
/*          fills for one digit                             */
/*  Syntax:    char* ctos(unsigned char no);                    */
/*  Remarks:    ----                                      */
/*  Returns:    A pointer to a string, (defined locally), containing the   */
/*          converted number                              */
/*  Updates:    ----                                      */
/***************************************************************************/
char* ctos(unsigned char no) {

  char s[6];
  char i;

  i = 0;
  if (no >= 100) {          /* find houndreds */
    s[i++] = '1';
    no -= 100;
  }
  else {
    s[i++] = ' ';          /* no houndreds, set 2 spaces */
    s[i++] = ' ';
  }
  if (no >= 10) {          /* find tens */
    s[i++] = '0' + no / 10;
    no %= 10;
  }
  else {
    s[i++] = ' ';          /* no tens, set 2 spaces */
    s[i++] = ' ';
  }
  s[i++] = '0' + no;        /* last digit */
  s[i] = '\0';            /* end string */

  return(s);
}

/***************************************************************************/
/*  out_phos_name                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write the phosphor names.                        */
/*  Syntax:    void out_phos_name(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void out_phos_name(void) {

  char pno;

  for (ic=0; ic < menu[3].maxline - 3; ic++) {
    pno = phos_org + ic;
    outtextxy(1, ic+4, ctos(pno));
    outtext(": ");
    outtext(phos[pno]->name);
  }
}

/***************************************************************************/
/*  out_ref_name                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write customer white reference names                */
/*  Syntax:    void out_ref_name(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void out_ref_name(void) {

  for (ic=1; ic<=refmax; ic++)
    outtextxy(13, ic+2, whref[ic]->name);
}

/***************************************************************************/
/*  out_cfactor_name                                      */
/*                                                  */
/* Written by:  Kim Engedahl, DEV                              */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write customer cfactor names                      */
/*  Syntax:    void out_cfactor_name(void);                      */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void out_cfactor_name(void) {

  for (ic=1; ic<=factormax; ic++)
    outtextxy(13, ic+2, cfactor[ic]->name);
}

/***************************************************************************/
/*  bardown                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Move the selection bar one line down                */
/*  Syntax:    void bardown(void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void bardown(void) {

  textbar(temp->select + 2, 0);                    /* erase old bar */
  temp->select += 1;
  if (temp->select > menuptr->maxline) 
    temp->select = 1;
  textbar(temp->select+2, 1);                     /* set new bar */
}

/***************************************************************************/
/*  barup                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Move the selection bar one line up                  */
/*  Syntax:    void barup(void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void barup(void) {

  textbar(temp->select + 2, 0);                    /* erase old bar */
  temp->select -= 1;
  if (temp->select < 1) 
    temp->select = menuptr->maxline;
  textbar(temp->select+2, 1);                     /* set new bar */
}

/***************************************************************************/
/*  out_laptime                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Write the elapsed time in test-mode                  */
/*  Syntax:    void out_laptime(char line);                      */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void out_laptime(char line) {

  outtextxy(32, line, " ");
  outchar('0'+ *laphr/10);
  outchar('0'+ *laphr%10);
  outchar(':');
  outchar('0'+ *lapmin/10);
  outchar('0'+ *lapmin%10);
  outchar(':');
  outchar('0'+ *lapsec/10);
  outchar('0'+ *lapsec%10);
}
    
/***************************************************************************/
/*  shut_dwn                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                        */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Shut down the instrument                        */
/*  Syntax:    void shut_dwn(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void shut_dwn(void) {

  contrast_port = 0x80;        /* remove backplane voltage */
  for (ic=1; ic!=0; ic++)        /* pause specified by LCD manufact. */
    for (jc=1; jc!=0; jc++)
      ;
  contrast_port = 0;          /* shut down */
  XBYTE[0x1fff] = power_down;    /* set power-down flag */

  while(1)
    ;                /* stay here until power down */
}

/***************************************************************************/
/*  no_of_decimals                                        */
/*                                                  */
/* Written by:  Kim Engedahl, DEV                              */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Return number of decimals a value should display        */
/*  Syntax:    char no_of_decimals(float value);                  */
/*  Remarks:    ----                                      */
/*  Returns:    A char containing the number of decimals the value should  */
/*          display/contain.                              */
/*  Updates:    ----                                      */
/***************************************************************************/
char no_of_decimals(float value) {

  if (value >= 100.0) 
    return(0);
  else 
    if (value >= 1.0) 
      return(1);
  else 
    return(2);
}

/***************************************************************************/
/*  setmenu                                            */
/*                                                  */
/* Written by:  Kim Engedahl, DEV                              */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    SUB.C                                      */
/*  Function:  Draw a menu.                                */
/*  Syntax:    void setmenu(char n);                          */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void setmenu(char n) {
  char i, y;
  char *s, *t;
  float rgb_tmp;

  *menuno = n;
  cls();
  menuptr = &menu[*menuno];
  *freeze = 0;                      /* stop recall meas. mode */
  *menuon = 1;                      /* text mode */
  outtextxy(1, 1, menuptr->txtline[0]);      /* write headline */

  setline();                  /* JK deletion draw line in head */

  feed_dog = 1;                      /* feed watch dog */
  feed_dog = 0;

  for (i=1;i<12;i++)
    outtextxy(1, i+2, menuptr->txtline[i]);  /* write menu lines */

  putcmd(0x5b);                /* set Overlay mode for display, xor */
  putdat(0x0d);
  switch (*menuno) {                  /* find place for select bar */
    case 1:  
      rgb_tmp = sup[0]->abs_rgb*unitfact/measunit[1].unitfact;

      outtextxy(22, 7, ftoa(rgb_tmp, no_of_decimals(rgb_tmp)));
      outtext("  ");
      outtextxy(42, 7, measunit[sup[0]->munit_no].name);
      y = *barptr[1]; 
      break;

    case 3:
      findorg();
      y = sup[0]->phos_no - phos_org +2;
      break;

    case 6: 
      switch (sup[0]->xymode_no) {
        case 1:
        case 2:
          s = "       x =";
          t = "       y =";
          break;
        case 3:
        case 4:
          s = "       u'=";
          t = "       v'=";
          break;
        case 5:
        case 6:
          s = "       u =";
          t = "       v =";
          break;
      }

      outtextxy(1, 5, s);
      outtextxy(1, 6, t);

      y = menu[6].maxline;
      break;

    case 16:  
      findorg();
      y = menu[16].maxline; 
      break;

    case 18:
      switch(sup[0]->xymode_no) {
        case 1:
        case 2:
          s = "x=";
          t = "y=";
          break;
        case 3:
        case 4:
          s = "u'=";
          t = "v'=";
          break;
        case 5:
        case 6:
          s = "u=";
          t = "v=";
          break;
      }
      outtextxy(1, 3, s);
      outtextxy(1, 4, t);

      y = *barptr[18]; 
      break;

    case 30:  
      rgb_tmp = sup[0]->abs_rgb*(unitfact*convfact)/measunit[1].unitfact;

      if ((rgb_tmp > 99999) || (rgb_tmp < 0.01))
        outtextxy(22, 7 , "---- ");
      else
        outtextxy(22, 7 ,ftoa(rgb_tmp, no_of_decimals(rgb_tmp)));

      outtext(" ");
      outtextxy(44, 7, cfactor[sup[0]->cfactor_no]->name);
      y = *barptr[30];
      break;

    case 2:
    case 11:
    case 31:  
      y = 1;
      break;
    case 9:  
    case 10:  
    case 15:  
    case 33:  
    case 37:  
    case 41:
      y = menu[*menuno].maxline;
      break;

    default:  
      y = *barptr[*menuno]; 
      break;
  }
  textbar(y+2,1);                /* set the select bar */
  temp->select = y;
  menu_cnt = menu_delay;            /* start menu time-out */
}
