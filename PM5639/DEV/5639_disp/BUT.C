/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PM5639 Display Unit                            */
/*  Module:    BUT.C                                      */
/*  Author:    Preben Christiansen, DEV                        */
/*  Org. date:  910212                                    */
/*  Rev. date:  970822, KEn  DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*      void addtxt(char c);                              */
/*      void settxt(char no);                              */
/*      void add_prompts(void);                            */
/*      void add_letter(char menu);                          */
/*      char take_meas(void);                              */
/*      float luminans(void);                              */
/*      void show_message(char message, char line)                */
/*      char testlow(char line, float limit)                    */
/*      char learn_red(void);                              */
/*      char learn_green(void)                              */
/*      char learn_blue(void);                              */
/*      void matrixinvert(void);                            */
/*      bit ymax(void);                                  */
/*      bit ymin(void);                                  */
/*      void learn_whref( char tmp, char maxline)                  */
/*      void func_select( char tmp)                          */
/*      void EnterMenu_nn(void) nn=1-41                        */
/*                    nn=1-39 -(2,6,7,12,13,15,18,21-28,31,33,35)  */
/*      void enterbut(void);                               */
/*      void autozoom(void);                              */
/*      void count(char xy, char dwnup);                      */
/*      void countRGB(char dwnup);                          */
/*      void tbutcheck(void);                              */
/*      void phos_up(char tmp);                            */
/*      void phos_dwn(char tmp);                            */
/*      void UP_but(void);                                 */
/*      void DOWN_but(void);                               */
/*      void RGB_but(void);                               */
/*      void xyY_but(void);                                */
/*      void FUNCT_but(void);                              */
/*      void RECALL_but(void);                              */
/*      void STORE_but(void);                              */
/*      void butcheck(void);                               */
/*      void timetick(void) interrupt 1                       */
/*                                                  */
/* Remarks:                                            */
/*    after linking, check that no routine exceeds 800H in size.        */
/*                                                  */
/* Changes:                                            */
/*  970822 Released as version 2.3                            */
/*  951207 Released as version 2.2                            */
/*  951127 Added the functions to testmode 6 in tbutcheck()            */
/*  951107 Added definitions for buttons in tbutcheck() and butcheck() and  */
/*       timetick()                                      */
/*  951107 Added two new testmodes (6 & 7) in tbutcheck()              */
/* 951030 timecount variable replaced by GenericTimecount            */
/* 951030 Added new timeout timer: GenericTimecount in timetick()        */
/* 951030 One test in ymin() returned 0 for both true and false. Corrected  */
/*       to 0 and 1.                                    */
/* 920520 Now supports projector head                           */
/* 920520 For earlier versions of history see u'v'-version             */
/***************************************************************************/
#pragma large

#include <reg51.h>          /* Define 8051 registers */
#include <stdio.h>          /* Define I/O functions */
#include <stdlib.h>          /* Standard library */
#include <absacc.h>          /* Absolute addresses */
#include <math.h>            /* Math (abs,...) */
#include <sub.h>            /* Own definitions */
#include <com.h>            /* Own definitions */
#include <defs.h>            /* Own definitions */
#include <tables.h>          /* Own definitions */

bit do_checksum_test(unsigned int *);

/***************************************************************************/
/*  addtxt                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Add a letter at name spelling                      */
/*  Syntax:    void addtxt(char c);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void addtxt(char c) {
  char maxchar, charno_tmp;

  charno_tmp = temp->charno;

  maxchar = 4;
  if (*mother != 36)
    maxchar += 2;

  if (c == '%') {                  /* Erase code */
    if (charno_tmp > 0) 
      charno_tmp -= 1;
    temp->txt[charno_tmp] = '\0';
  }
  else {                        /* Normal letter */
    if (charno_tmp > maxchar-1) 
      charno_tmp = maxchar-1;
    temp->txt[charno_tmp++] = c;        /* Point to next free place */
    temp->txt[charno_tmp] = '\0';        /* End the string */
  }
  prompts_on = 1;

  temp->charno = charno_tmp;
}

/***************************************************************************/
/*  settxt                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Display temporary name                          */
/*  Syntax:    void settxt(char no);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void settxt(char no)  {
  switch (no) {
    case 17: 
      no = sup[0]->phos_no; 
      break;
    case 20: 
      no = sup[0]->whref_no; 
      break;
    case 40: 
      no = sup[0]->cfactor_no; 
      break;
  }

  if (no == 36)
    outtextxy(1, 15, "unit = ");
  else {
    outtextxy(1, 15, "#");

    if (no<10) 
      outchar('0'+ no);
    else {
      outchar('0'+  no / 10);
      outchar('0' + no% 10);
    }
    outtext(": ");
  }
  outtext(temp->txt );
}

/***************************************************************************/
/*  add_prompts                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Show spaceholders after temporary name                */
/*  Syntax:    void add_prompts(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void add_prompts(void) {
  char i, maxchar;

  maxchar = 4;
  if (*mother != 36)
    maxchar += 2;

  if (prompts_on)
    for (i=0 ; i < (maxchar - temp->charno) ; i++)
      outtext("_");
}

/***************************************************************************/
/*  add_letter                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Name spelling routine                          */
/*  Syntax:    void add_letter(char menu);                      */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void add_letter(char menu) {
  char  tmp, letter;

  tmp = temp->select;

  *barptr[menu] = tmp;

  if ((tmp != 79) && !((menu == 27) && (tmp == 4))) {
    if ((menu < 25) || ((menu == 25) && (tmp < 3)))
      letter = 'A' + menu*6 + tmp - 127;
    else 
      if (menu < 27)
        letter = '0' + menu*6 + tmp - 153;
    else {
      switch(tmp) {
        case 1: 
          letter = ' ';
          break;
        case 2: 
          letter = '%';
          break;
        case 3: 
          letter = '/';
          break;
      }
    }
    addtxt(letter);
  }

  setmenu(*mother);
  settxt(*mother);
  add_prompts();
}

/***************************************************************************/
/*  take_meas                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951030                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Catch a single measurement for phosphor learning        */
/*  Syntax:    char take_meas(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
char take_meas(void)
  {
  char i;

  for (i=0; i<2; i++)
    {
    sendok = 0;

    GenericTimecount = 49;              /* Time-out value 49*53.5ms */
    while ((!sendok) && GenericTimecount)
      {                                 /* Skip current measurement */
      feed_dog = 1;
      feed_dog = 0;
      }
    }
  return(sendok ? 1:0);                 /* return OK if not time-out */
  }


static float g_at_r, b_at_r, r_at_g, b_at_g, r_at_b, g_at_b;
static bit r_done, g_done, b_done, whref_done;


/***************************************************************************/
/*  luminans                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Calculate absolute luminance Y                    */
/*  Syntax:    float luminans(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
#if 1
float luminans(void)
  {
  char* tmp = "20.0";
  if (sensor_is_94)    /* Change for 94 sensor, luminance multiplied by 10: JK */
    return 10 * (atof(rgbstring[0]) * k[1] + atof(rgbstring[1]) * k[4] + \
           atof(rgbstring[2]) * k[7]) * 20.0 / fabs(atof(noscan ? tmp : rgbstring[3] ));
  else
    return(atof(rgbstring[0]) * k[1] + atof(rgbstring[1]) * k[4] + \
           atof(rgbstring[2]) * k[7]) * 20.0 / fabs(atof(noscan ? tmp : rgbstring[3] ));

  }
#else /* Below is original function */
float luminans( void)
  {
  char* tmp = "20.0";

  return( atof( rgbstring[0]) * k[1] + atof( rgbstring[1]) * k[4] + \
          atof( rgbstring[2]) * k[7]) * 20.0 \
          / fabs( atof( noscan ? tmp : rgbstring[3] ));
  }
#endif
/***************************************************************************/
/*  show_message                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Show message at line for a period                  */
/*  Syntax:    void show_message(char message, char line);            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void show_message(char message, char line) {
  switch (message) {
    case overflow:
      outtextxy(28, line, "Overload ");
      break;
    case lowlight:
      outtextxy(28, line, "Low light ");
      break;
    case error:
      outtextxy(1, line, "Ill. coordinates ");
      break;
  }

  pause(255);
  pause(255);
  pause(255);
  clrtext(line);
}

/***************************************************************************/
/*  testlow                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Test if low- or high-light                        */
/*  Syntax:    char testlow(char line, float limit);                */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
char testlow(char line, float limit)
  {
  if (luminans() < limit / measunit[1].unitfact)
    outtextxy(28, line, "LOW LIGHT ");
  else 
    if (atof(rgbstring[0]) < -0.5)       /* Test indication for overload */
      outtextxy(28, line, "OVERLOAD ");  /* (red value = -1.0) */
  else 
    return(1);

  pause(255);
  pause(255);
  pause(255);
  clrtext(line);
  return(0);
  }

/***************************************************************************/
/*  learn_red                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Manage learning red phosphor                      */
/*  Syntax:    char learn_red(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
char learn_red(void) {
  float refcol;

  if (!take_meas() || !testlow(9, 6.0))
    return(0);                /* Skip if no measurement or low light*/

  refcol = xfact * atof(rgbstring[0]);
  g_at_r = atof(rgbstring[1]) / refcol;    /* NB! yfact is 1 */
  b_at_r = zfact * atof(rgbstring[2]) / refcol;
  r_done = 1;
  bardown();                    /* Place bar over learn green */
  outtextxy(56, 6, "ok");
  return(1);
}

/***************************************************************************/
/*  learn_green                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Manage learning green phosphor                    */
/*  Syntax:    char learn_green(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
char learn_green(void) {
  float refcol;

  if (!take_meas() || !testlow(9, 12.0))
    return(0);                /* Skip if no measurement or low light*/

  refcol = atof(rgbstring[1]);
  r_at_g = xfact * atof(rgbstring[0]) / refcol;
  b_at_g = zfact * atof(rgbstring[2]) / refcol;
  g_done = 1;
  bardown();                    /* Place bar over learn blue */
  outtextxy(56, 7, "ok");
  return(1);
}

/***************************************************************************/
/*  learn_blue                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Manage learning blue phosphor                      */
/*  Syntax:    char learn_blue(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
char learn_blue(void) {
  float refcol;

  if (!take_meas() || !testlow(9, 2.0))
    return(0);                /* Skip if no measurement or low light*/

  refcol = zfact * atof(rgbstring[2]);
  r_at_b = xfact * atof(rgbstring[0]) / refcol;
  g_at_b = atof(rgbstring[1]) / refcol;
  b_done = 1;
  bardown();
  bardown();                    /* Place bar over enter */
  outtextxy(56, 8, "ok");
  return(1);
}

/***************************************************************************/
/*  matrixinvert                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Calculate phosphor matrix after learn                */
/*  Syntax:    void matrixinvert(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void matrixinvert(void) {
  float rr1, aa;
  char i;

  aa = r_at_b;                /* Calculate figures for red */
  for (i=0; i<10; i++) {
    rr1 = r_at_g - aa * b_at_g;
    aa = r_at_b - rr1 * g_at_b;
  }
  nn_brt = -(int)(aa * int_scale);    /* Normalize with int_scale (8192) */
  nn_grt = -(int)(rr1 * int_scale);

  aa = g_at_b;                /* Calculate figures for green */
  for (i=0; i<10; i++) {
    rr1 = g_at_r - aa * b_at_r;
    aa = g_at_b - rr1 * r_at_b;
  }
  nn_bgt = -(int)(aa * int_scale);    /* Normalize with int_scale (8192) */
  nn_rgt = -(int)(rr1 * int_scale);

  aa = b_at_g;                /* Calculate figures for blue */
  for (i=0; i<10; i++) {
    rr1 = b_at_r - aa * g_at_r;
    aa = b_at_g - rr1 * r_at_g;
  }
  nn_gbt = -(int)(aa * int_scale);    /* Normalize with int_scale (8192) */
  nn_rbt = -(int)(rr1 * int_scale);
}

/***************************************************************************/
/*  ymax                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951030                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Calculate if learned white reference is in valid approxi-  */
/*          mated colo(u)r chromaticity diagram.                */
/*  Syntax:    bit ymax(void);                               */
/*  Remarks:    Used in conjunction with ymin                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit ymax(void)
  {
  if (whref[6]->x < 0.0082) 
    return(1);
  else 
    if ((whref[6]->x >= 0.0082) && (whref[6]->x < 0.0139))
      {
      if(whref[6]->y <= (37.1579*whref[6]->x + 0.2337))
        return(0);
      else 
        return (1);
      }
    else 
      if ((whref[6]->x >= 0.0139) && (whref[6]->x < 0.0743))
        {
        if (whref[6]->y <= (1.3841*whref[6]->x + 0.7310)) 
          return(0);
        else 
          return (1);
        }
      else 
        if ((whref[6]->x >= 0.0743) && (whref[6]->x < 0.2296))
          {
          if (whref[6]->y <= (-0.5441*whref[6]->x + 0.8792))
            return (0);
          else
            return (1);
          }
        else 
          if ((whref[6]->x >= 0.2296) && (whref[6]->x < 0.6658))
            {
            if (whref[6]->y <= (-0.9779*whref[6]->x + 0.9851))
              return (0);
            else
              return (1);
            }
          else 
            if ((whref[6]->x >= 0.6658) && (whref[6]->x <= 0.7347))
              {
              if (whref[6]->y <= (-1.068*whref[6]->x + 1.050))
                return (0);
              else 
                return (1);
              }
            else 
              return (1);
  }

/***************************************************************************/
/*  ymin                                                                   */
/*                                                                         */
/* Written by:  Preben Christiansen, DEV                                   */
/*  Revised by:  Kim Engedahl, DEV                                         */
/*  Date:      910212                                                      */
/*  Revised:    951030                                                     */
/*                                                                         */
/* Module:    BUT.C                                                        */
/*  Function:  Calculate if learned white reference is in valid approxi-   */
/*             mated colo(u)r chromaticity diagram.                        */
/*  Syntax:    bit ymin(void);                                             */
/*  Remarks:   Used in conjunction with ymax                               */
/*  Returns:    ----                                                       */
/*  Updates:    ----                                                       */
/***************************************************************************/
bit ymin(void)
  {
  if (whref[6]->x < 0.0082) 
    return (1);
  else 
    if ((whref[6]->x >= 0.0082) && (whref[6]->x < 0.0913))
      {
      if (whref[6]->y >= (-4.8821*whref[6]->x + 0.5784)) 
        return (0);
      else 
        return (1);
      }
    else 
      if ((whref[6]->x >= 0.0913) && (whref[6]->x < 0.1741))
        {
        if (whref[6]->y >= (-1.5422*whref[6]->x + 0.2735)) 
          return (0);
        else 
          return (1);
        }
      else 
        if ((whref[6]->x >= 0.1741) && (whref[6]->x <= 0.7347))
          {
          if (whref[6]->y >= (0.4643*whref[6]->x - 0.0758)) 
            return (0);
          else 
            return (1);
          }
        else 
          return (1);
  }

/***************************************************************************/
/*  learn_whref                                                            */
/*                                                                         */
/* Written by:  Preben Christiansen, DEV                                   */
/*  Revised by: Kim Engedahl, DEV                                          */
/*  Date:       910212                                                     */
/*  Revised:    951005                                                     */
/*                                                                         */
/* Module:      BUT.C                                                      */
/*  Function:   Manage learning white reference                            */
/*  Args:       char tmp:     current menu bar position                    */
/*              char maxline: last line in menu, mostly "Exit"             */
/*  Remarks:    ----                                                       */
/*  Returns:    ----                                                       */
/*  Updates:    ----                                                       */
/***************************************************************************/
void learn_whref(char tmp, char maxline)
  {
  float out1, out2;

  if (sensor_is_94)    /* Change for 94 sensor JK */
    out1 = 1.0;
  else
    out1 = 10.0;

  switch(head_version)
    {
    case CRThead:
      *barptr[18] = tmp;             /* Selected menu bar */
      if (tmp == maxline - 1)
        {                            /* if "Enter" selected */
        if (whref_done)
          {                          /* and new wh.ref. is learned */
          if (ymax() || ymin())      /* and valid colo(u)r */
            show_message(error, menu[18].maxline + 3);
          else
            {
            setmenu(19);             /* show "Store wh.ref." menu */
            out_ref_name();
            }
          }
        }
      else 
        if (tmp == maxline)      /* if "Exit" selected */
          setmenu(14);           /* then set learn head-menu */
        else 
          if (tmp == 3) /* if "Use measurement" selected test if overload or low light */
            {
            if ((msm[0]->xm / xfact) < -0.5)    
              show_message(overflow, menu[18].maxline + 3);
            else 
              if (msm[0]->yy < out1 / measunit[1].unitfact)
                show_message(lowlight, menu[18].maxline + 3);
              else
                {                                /* Light was OK */
                whref[6]->u  = msm[0]->uval;      /* Store references */
                whref[6]->v  = msm[0]->vval;
                whref[6]->v_ = msm[0]->v_val;
                whref[6]->x  = msm[0]->xval;
                whref[6]->y  = msm[0]->yval;
    
                switch (sup[0]->xymode_no )
                  {
                  case 1:
                  case 2:
                    out1 = whref[6]->x;
                    out2 = whref[6]->y;
                    break;
                  case 3:
                  case 4:
                    out1 = whref[6]->u;
                    out2 = whref[6]->v_;
                    break;
                  case 5:
                  case 6:
                    out1 = whref[6]->u;
                    out2 = whref[6]->v;
                    break;
                  }
  
              outtextxy(15,3,ftoa(out1, 3));    /* and show in display */
              outtextxy(15,4,ftoa(out2, 3));
  
              bardown();                   /* Move bar to "Enter" */
              outtextxy(56,5,"ok");
              whref_done = 1;
              }
            }
        break;
      case PRJhead:
        *barptr[35] = tmp;          /* Selected menu bar */
        if (tmp == 1)
          {            /* If "Use measurement" selected Test if overload or low light */
          if ((msm[0]->xm / xfact) < -0.5)
            show_message(overflow, menu[35].maxline+3);
          else 
            if (msm[0]->yy < 0.01 / (measunit[1].unitfact*default_cfactor))
              show_message(lowlight, menu[35].maxline+3);
            else
              {                             /* Light was OK */
              whref[6]->x = msm[0]->xval;   /* Store references */
              whref[6]->y = msm[0]->yval;
              /* u,v and u'v' are not necessary in PROJECTOR version*/
              bardown();                    /* Move bar to "Enter" */
              outtextxy(56, 3, "ok");
              whref_done = 1;
              }
          }
        else 
          if (tmp == maxline - 1)
            {                          /* If "Enter" selected */
            if (whref_done)
              {                        /* and new wh.ref. is learned */
              if (ymax() || ymin())    /* and valid colo(u)r */
                show_message(error, menu[18].maxline+3);
              else
                {
                setmenu(19);          /* show "Store wh.ref." menu */
                out_ref_name();
                }
              }
            }
          else 
            if (tmp == maxline)       /* if "Exit" selected */
              setmenu(34);            /* then set learn head-menu */
          break;
    }
  }

/***************************************************************************/
/*  func_select                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Manage selections in ""Function" menu                */
/*  Syntax:    void func_select(char tmp);                      */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void func_select(char tmp) {
  switch (head_version) {
    case CRThead:
      switch(tmp) {
        case 1:                /* "Status" */
          setmenu(6);
          show_status();
          break;
        case 2:                /* "Crt/phosphor" */
          setmenu(3);
          out_phos_name();
          break;
        case 3:                /* "White reference" */
          setmenu(4);
          out_ref_name();
          break;
        case 4:                /* "Luminance unit" */
          setmenu(5);
          break;
        case 5:                /* Chromaticity unit" */
          setmenu(29);
          break;
        case 6:                /* "Exit" */
          initmode();
          break;
      }
      break;

    case PRJhead:
      switch(tmp) {
        case 1:                /* "Status" */
          setmenu(33);
          show_status();
          break;
        case 2:                /* "White reference" */
          setmenu(4);
          out_ref_name();
          break;
        case 3:                /* "Conversion factor" */
          setmenu(32);
          out_cfactor_name();
          break;
        case 4:                /* "Exit" */
          initmode();
          break;
      }  
      break;
  }
}

/* Below are some more global variables.  As if there was not enough of */
/* them already in tables.c.  Try to count how many functions in this   */
/* project use variable named tmp (note that there is a structure named */
/* tmp too.  The author should be nominated for Nobel Prize for this!   */
/* 13.09.2004: JK                                                       */

char select, tmp, maxline;


/***************************************************************************/
/*  EnterMenu_1                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_1(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_1(void) {

  if (tmp != menu[1].maxline) {                 /* if not "Exit" */
    if ((tmp > 5) && (msm[tmp-5]->key != 0x55)) 
      return;              /* exit if measurem. ref. not available */
    *barptr[1] = tmp;                        /* accept selection */
    set_setup_no = 0;
  }
  initmode();                              /* Go to RGB-mode */
}

/***************************************************************************/
/*  EnterMenu_3                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_3(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_3(void) {

  if (tmp == maxline-1)                 /* if "Exit" selected */
    setmenu(2);                        /* then show Function menu */
  else 
    if ((tmp < maxline) && (tmp > 1)) {
      select = tmp + phos_org - 2;          /* else if selected phosphor */
      if (phos[select]->key != 0x55)      /* is available */
        return;
      recall_phosphor(select);            /* then install new phosphor */
      *barptr[3] = tmp;
      newhead = new_head_version = 0;      /* accept the new head */
      scopy(new_number,number,5);        /* with it's numbers */
      scopy(new_kuno,kuno,6);
      set_setup_no = 0;
      initmode();
    }
}

/***************************************************************************/
/*  EnterMenu_4                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_4(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_4(void) {

  if (tmp == maxline)                 /* if "Exit" selected */
    switch (head_version) {          /* then set Function menu */
      case CRThead:
        setmenu(2);
        break;
      case PRJhead:
        setmenu(31);
        break;
    }
  else {                        /* else if selected wh.ref */
    if (whref[tmp]->key != 0x55)
      return;                     /* is available */
    recall_whiteref(tmp);            /* then install new wh.ref. */
    *barptr[4] = tmp;
    initmode();                    /* go to Measure mode */
  }
}

/***************************************************************************/
/*  EnterMenu_5                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_5(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_5(void) {

  if (tmp == maxline)                 /* if "Exit" selected */
    setmenu(2);                      /* then set FUNCTION menu */
  else {                        /* else accept new unit */
    set_measunit(tmp);
    *barptr[5] = tmp;
    initmode();                    /* go to Measure mode */
  }
}

/***************************************************************************/
/*  EnterMenu_8                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_8(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_8(void) {

  *barptr[8] = tmp;

  switch(tmp) {
    case 1:                      /* STORE SET-UP menu */
      setmenu(10);
      break;
    case 2:                       /* STORE MEASUREM. menu */
      setmenu(9);
      break;
    case 3:
      initmode();                  /* go to Measuring mode */
      break;
  }
}

/***************************************************************************/
/*  EnterMenu_9                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_9(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_9(void) {

  if (tmp == maxline)                /* if Exit selected */
    setmenu(8);                    /* then go to STORE menu */
  else {
    store_msm(tmp);                /* else store measurement */
    *barptr[9] = tmp;
    initmode();                    /* go to Measure mode */
  }
}

/***************************************************************************/
/*  EnterMenu_10                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_10(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_10(void) {

  if (tmp == maxline)                /* if "Exit" selected */
    setmenu(8);                    /* then go to STORE menu */
  else {
    store_setup(tmp);
    *barptr[10] = tmp;
    initmode();                    /* go to Measure mode */
  }
}

/***************************************************************************/
/*  EnterMenu_11                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_11(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_11(void) {

  *barptr[11] = tmp;

  switch(tmp) {
    case 1:
      if (sup[0]->toggle_setup) {
        sup[0]->toggle_setup = 0;
        *barptr[13] = 1;
      }
      else {
        sup[0]->toggle_setup = 1;
        *barptr[13] = 2;
      }
      recall_setup(sup[0]->toggle_setup + 1);
      initmode();                      /* go to Measure mode */
      break;

    case 2:                      /* RECALL SETUP menu */
      setmenu(13);
      break;
    case 3:                      /* RECALL MEASUREM. menu */
      setmenu(12);
      break;
    case 4:                      /* Exit */
      initmode();
      break;                     /* go to Measuring mode */
  }
}

/***************************************************************************/
/*  EnterMenu_14                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_14(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_14(void) {
  float out1, out2;

  *barptr[14] = tmp;

  switch(tmp) {
    case 1:
      store_setup(11);                /* store as default set-up */
      setmenu(14);                /* go to "Learn" menu */
      break;

    case 2:                      /* "Learn CRT/phosp" menu */
      temp->select = maxline;
      setmenu(15);
      r_done = g_done = b_done = 0;      /* prepare learning */
      break;

    case 3:                      /* "Learn White ref" menu */
      whref_done = 0;              /* prepare learning */
                              /* copy to operate on */
      whref[6]->u = (float)((int)(1000*(whref[0]->u+0.0005)))/1000;
      whref[6]->v_ = (float)((int)(1000*(whref[0]->v_+0.0005)))/1000;
      whref[6]->v = (float)((int)(1000*(whref[0]->v+0.0005)))/1000;
      whref[6]->x =  (float)((int)(1000*(whref[0]->x+0.0005)))/1000;
      whref[6]->y =  (float)((int)(1000*(whref[0]->y+0.0005)))/1000;

      setmenu(18);

      switch (sup[0]->xymode_no) {
        case 1:
        case 2:
          out1 = whref[6]->x;
          out2 = whref[6]->y;
          break;
        case 3:
        case 4:
          out1 = whref[6]->u;
          out2 = whref[6]->v_;
          break;
        case 5:
        case 6:
          out1 = whref[6]->u;
          out2 = whref[6]->v;
          break;
      }
      outtextxy(15, 3, ftoa(out1, 3));
      outtextxy(15, 4, ftoa(out2, 3));
      break;

    case 4:                       /* "Exit" select */
      initmode();                  /* go to Measure mode */
      break;
  }
}

/***************************************************************************/
/*  EnterMenu_16                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_16(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_16(void) {

  *barptr[16] = tmp;

  if (tmp == maxline-1)               /* if "Exit" selected */
    setmenu(14);                   /* then go to "Learn" menu */
  else 
    if ((tmp > 1) && (tmp < maxline)) {
      select = tmp + phos_org - 2;      /* else store phosphor */
      if (store_phosphor(select)) {
        setmenu(17);              /* go to name menu */
        settxt(17);                /* show old name */
      }
    }
}

/***************************************************************************/
/*  EnterMenu_17                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_17(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_17(void) {

  *barptr[17] = tmp;                 /* "Name CRT/Phosp" menu */

  switch(tmp) {
    case 8:
      *barptr[16] = menu[16].maxline;      /* "Enter" select */
      strput(temp->txt, phos[sup[0]->phos_no]->name);
      setmenu(3);              /* store name and show phosphor list */
      out_phos_name();
      sendname(sup[0]->phos_no);          /* send name to C.Sensor */
      break;

    case 9:
      setmenu(16);                  /* if "Exit" selected */
      out_phos_name();
      break;                      /* and write own names */

    default:                        /* default = 1->7 */
      setmenu(20+tmp);                /* display selected letters */
      settxt(17);                    /* show old name */
      add_prompts();                 /* and add space holders */
      break;
  }
  *mother = 17;
}

/***************************************************************************/
/*  EnterMenu_19                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_19(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_19(void) {

  *barptr[19] = tmp;              /* "Store white ref." menu */

  if (tmp == maxline)              /* if "Exit" selected */
    switch (head_version) {
      case CRThead:
        setmenu(14);             /* then go to "Learn" menu */
        break;
      case PRJhead:
        setmenu(34);             /* then go to "Learn" menu */
        break;
    }
  else
    switch (head_version) {
      case CRThead:
        if (tmp > 3) {          /* Reference 1-3 must not be erased*/
          store_whiteref(tmp);    /* Store reference */
          setmenu(20);          /* and go to name menu */
          settxt(20);            /* Show old name */
        }
        break;
      case PRJhead:
        if (tmp != 1) {          /* Reference 1 must not be erased */
          store_whiteref(tmp);    /* Store reference */
          setmenu(20);          /* and go to name menu */
          settxt(20);            /* Show old name */
        }
        break;
    }
}

/***************************************************************************/
/*  EnterMenu_20                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_20(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_20(void) {

  *barptr[20] = tmp;                  /* "Name white ref." menu */

  switch(tmp)  {                      /* name wh. ref */
    case 8:
      *barptr[19] = menu[19].maxline;      /* "Enter" select */
      strput(temp->txt, whref[sup[0]->whref_no]->name);
      setmenu(4);                /* store name and show Wh.ref list */
      out_ref_name();
      break;

    case 9:
      setmenu(19);                  /* if "Exit" selected */
      out_ref_name();
      break;                      /* and write own names */

    default:                        /* default = 1->7 */
      setmenu(20+tmp);                /* display selected letters */
      settxt(20);                    /* show old name */
      add_prompts();                  /*  and add space holders */
      break;

  }
  *mother = 20;
}

/***************************************************************************/
/*  EnterMenu_29                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_29(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_29(void) {

  if (tmp == maxline)                  /* if "Exit" selected */
    setmenu(2);                      /* then set Function menu */
  else {                          /* else accept new unit */
    *barptr[29] = sup[0]->xymode_no = tmp;
    set_setup_no = 0;
    initmode();
  }
}

/***************************************************************************/
/*  EnterMenu_30                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_30(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_30(void) {

  if (tmp != menu[30].maxline) {          /* if not "Exit" */
    if ((tmp > 5) && (msm[tmp-5]->key != 0x55))
      return;              /* exit if measurem. ref. not available */
    *barptr[30] = tmp;                /* accept selection */
    set_setup_no = 0;
  }
  initmode();                        /* Go to RGB-mode */
}

/***************************************************************************/
/*  EnterMenu_32                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_32(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_32(void) {

  if (tmp == maxline)                  /* if "Exit" selected */
    setmenu(31);                      /* then set Function menu */
  else {                        /* else if selected con. factor */
    if (cfactor[tmp]->key != 0x55) return;   /* is available */
    recall_cfactor(tmp);              /* then install new cfactor */
    *barptr[32] = tmp;
    initmode();                      /* go to Measure mode */
  }
}

/***************************************************************************/
/*  EnterMenu_34                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_34(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_34(void) {

  *barptr[34] = tmp;

  switch(tmp) {
    case 1:
      store_setup(11);                  /* store as default set-up */
      setmenu(34);                  /* go to "Learn" menu */
      break;

    case 2:                        /* "Learn White ref" menu */
      whref_done = 0;                /* prepare learning */
      setmenu(35);
      break;

    case 3:
      setmenu(36);                  /* "Name Conv F. Unit" select */
      temp->txt[0] = '\0';
      temp->charno = 0;
      prompts_on = 0;              /* spell prompts off for now */
      settxt(36);
      break;

    case 4:                         /* "Exit" select */
      initmode();                    /* go to Measure mode */
      break;
  }
}

/***************************************************************************/
/*  EnterMenu_36                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_36(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_36(void) {

  *barptr[36] = tmp;                  /* "Name Conv Fac. unit" menu */

  switch(tmp) {
    case 8:
      *barptr[39] = menu[39].maxline;      /* "Enter" select */
      strput(temp->txt, cfactor[11]->name);
      setmenu(37);            /* store name and show conv fac. list */
      break;

    case 9:
      setmenu(34);                  /* if "Exit" selected */
      break;                      /* and write own names */

    default:                        /* default = 1->7 */
      setmenu(20+tmp);                /* display selected letters */
      settxt(36);                    /* show old name */
      add_prompts();                 /* and add space holders */
      break;
  }
  *mother = 36;
}

/***************************************************************************/
/*  EnterMenu_37                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_37(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_37(void) {

  *barptr[37] = tmp;

  switch(tmp) {
    case 1:
      setmenu(38);                  /* "Conv Fact" select */
      cfactor[11]->cfactor = 100;
      outtextxy(14, 5, ftoa(cfactor[11]->cfactor, 0));
      outtextxy(36, 5, cfactor[11]->name);
      break;
    case 2:                         /* "Exit" select */
      setmenu(34);
      break;
  }
}

/***************************************************************************/
/*  EnterMenu_38                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_38(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_38(void) {

  *barptr[38] = tmp;

  switch(tmp) {
    case 3:
      break;                     /* No action in up/down units */
    case 5:
      setmenu(39);                  /* "Store conv fac" select */
      outtextxy(1, 15, "Store unit: ");
      outtext(cfactor[11]->name);
      out_cfactor_name();
      break;
    case 6:                         /* "Exit" select */
      setmenu(34);
      break;
  }
}

/***************************************************************************/
/*  EnterMenu_39                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:                                          */
/*  Syntax:    void EnterMenu_39(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void EnterMenu_39(void) {

  *barptr[39] = tmp;                /* "Store Conv Fac." menu */

  if (tmp == maxline)                /* if "Exit" selected */
    setmenu(34);                   /* then go to "Learn" menu */
  else {
    if (tmp != 1) {                /* If not factory unit do */
      strput(cfactor[11]->name, cfactor[tmp]->name);
                              /* Copy name to temp. store */
      store_cfactor(tmp);            /* Store con. factor */
      initmode();                  /* go to Measure mode */
    }
  }
}

/***************************************************************************/
/*  enterbut                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by: Kim Engedahl, DEV                              */
/*  Date:       910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:      BUT.C                                      */
/*  Function:   Manage the command when pressing <ENTER>              */
/*  Syntax:     void enterbut(void);                          */
/*  Remarks:    This routine decodes all the various functions of the      */
/*              ENTER button                                */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void enterbut(void)
  {
  maxline = menuptr->maxline;       /* last line in menu, mostly "Exit" */
  tmp     = temp->select;           /* current menu bar position */

  if (*menuon)
    switch(*menuno)
      {
      case 1:                    /* RGB menu */
        EnterMenu_1();
        break;
      case 2:                    /* FUNCTION menu */
        *barptr[2] = tmp;
        func_select(tmp);
        break;
      case 3:                     /* CRT/PHOSPHOR menu */
        EnterMenu_3();
        break;
      case 4:                     /* WHITE REFERENCE menu */
        EnterMenu_4();
        break;
      case 5:                     /* MEASURING UNIT menu */
        EnterMenu_5();
        break;
      case 6:                    /* STATUS u'v' menu */
        setmenu(2);
        break;
      case 8:                    /* STORE menu */
        EnterMenu_8();
        break;
      case 9:                    /* STORE MEASUREM. menu */
        EnterMenu_9();
        break;
      case 10:                    /* STORE SET-UP menu */
        EnterMenu_10();
        break;
      case 11:                    /* RECALL menu */
        EnterMenu_11();
        break;
      case 12:                   /* RECALL MEASUREM. menu */
        if (tmp == maxline)           /* if "Exit" selected */
          setmenu(11);              /* then go to RECALL menu */
        else
          {
          recall_msm(tmp);           /* else recall measurement */
          *barptr[12] = tmp;
          }
        break;
      case 13:                   /* RECALL SET-UP menu */
        if (tmp == maxline)           /* if "Exit" selected */
          setmenu(11);            /* then go to RECALL menu */
        else
          {
          recall_setup(tmp);        /* else recall set-up */
          *barptr[13] = tmp;
          initmode();              /* go to Measure mode */
          }
        break;
      case 14:                    /* LEARN menu */
        EnterMenu_14();
        break;
      case 15:                    /* LEARN CRT/phosp menu */
        switch(tmp)
          {
          case 4:                 /* sense red */
            learn_red();
            break;
          case 5:                 /* sense green */
            learn_green();
            break;
          case 6:                 /* sense blue */
            learn_blue();
            break;
          case 8:
            if (r_done & g_done & b_done)
              {                   /* "Enter" */
              matrixinvert();     /* calc. phosphor matrix */
              setmenu(16);        /* go to menu "Store CRT/Ph" */
              out_phos_name();
              }
            break;
          case 9:
            setmenu(14);          /* Exit, go to "Learn" menu */
            break;
          }
        break;
      case 16:                            /* STORE CRT/Phosp menu */
        EnterMenu_16();
        break;
      case 17:                            /* NAME CRT/Phosp menu */
        EnterMenu_17();
        break;
      case 18:                            /* LEARN Wh.ref. xy menu ------ */
        learn_whref(tmp,maxline);
        break;
      case 19:                            /* STORE Wh. ref menu */
        EnterMenu_19();
        break;
      case 20:                            /* NAME Wh. ref menu */
        EnterMenu_20();
        break;
                                          /* LETTER SELECT menus */
      case 21:                            /* ABCDEF select */
      case 22:                            /* GHIJKL select */
      case 23:                            /* MNOPQR select */
      case 24:                            /* STUVWX select */
      case 25:                            /* YZ0123 select */
      case 26:                            /* 456789 select */
      case 27:                            /* space, delete select */
        add_letter(*menuno);
        break;
      case 28:                            /* LEARN Wh.ref uv menu -------- */
        learn_whref(tmp, maxline);
        break;
      case 29:                            /* CHROMATICITY UNIT menu */
        EnterMenu_29();
        break;

      /* Additional menus for PROJECTOR version */

      case 30:                    /* RGB menu */
        EnterMenu_30();
        break;
      case 31:                    /* FUNCTION menu */
        *barptr[31] = tmp;
        func_select(tmp);
        break;
      case 32:                   /* CONVERSION FACTOR menu */
        EnterMenu_32();
        break;
      case 33:                    /* STATUS menu */
        newhead = 0;              /* accept the new head */
        scopy(new_number,number,5);    /* with it's numbers */
        scopy(new_kuno,kuno,6);
        setmenu(31);
        break;
      case 34:                    /* LEARN menu */
        EnterMenu_34();
        break;
      case 35:                    /* LEARN Wh. ref. menu */
        learn_whref(tmp, maxline);
        break;
      case 36:                    /* NAME CONV f. unit menu */
        EnterMenu_36();
        break;
      case 37:                    /* CONV F. Source menu */
        EnterMenu_37();
        break;
      case 38:                    /* CONVER. FACTOR menu */
        EnterMenu_38();
        break;
      case 39:                    /* STORE Conv. Fac. menu */
        EnterMenu_39();
        break;
    }
  }      /* END enterbut() */

/***************************************************************************/
/*  autozoom                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Set magnification to best fit                      */
/*  Syntax:    void autozoom(void);                          */
/*  Remarks:                                            */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void autozoom(void) {
  float rvol, gvol, bvol;
  char zoom_org;

  zoom_org = sup[0]->zoom;

  if ((!*menuon) && (!*freeze)) {
    if (sup[0]->mode_no == xyY_mode) {        /* if in xyY mode */

      if ((abs(xerr)<25) && (abs(yerr)<25))    /* for deviation < 25 */
        sup[0]->zoom = 1;                /* set zoom 1 */
      else 
        if((abs(xerr)<125) && (abs(yerr)<125))  /* for deviation < 125 */
          sup[0]->zoom = 5;              /* set zoom 5 */
      else 
        sup[0]->zoom = 25;              /* else zoom 25 */

      draw_refline(sup[0]->zoom);          /* show reference line */
    }
    else {                          /* in RGB mode */
      rvol = fabs(msm[0]->rbar - 1.);        /* find deviations */
      gvol = fabs(msm[0]->gbar - 1.);        /* from 1.0 */
      bvol = fabs(msm[0]->bbar - 1.);

      if (rvol < .1 && gvol < .1 && bvol < .1)  /* for deviation < 0.1 */
        sup[0]->zoom = 1;                /* set zoom 1 */
          
      else                          /* for deviation < 0.25 */
        if (rvol < .25 && gvol < .25 && bvol < .25)
          sup[0]->zoom = 5;              /* set zoom 5 */
      else 
        sup[0]->zoom = 25;              /* else zoom 25 */
    }
  }

  if (sup[0]->zoom != zoom_org) {
    set_setup_no = 0;
    outtextxy(49, sup[0]->mode_no, "       ");
  }
}

/***************************************************************************/
/*  count                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Count x or y in "Learn White refe." menu              */
/*  Syntax:    void count(char xy, char dwnup);                  */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void count(char xy, char dwnup) {

  float ref_tmp, whref_tmp;
  char xymode_tmp;

  switch (sup[0]->xymode_no) {
    case 1:
    case 2:
      xymode_tmp = 1;
      break;
    case 3:
    case 4:
      xymode_tmp = 2;
      break;
    case 5:
    case 6:
      xymode_tmp = 3;
      break;
  }

  xymode_tmp += 3*xy;          /* 3*xy depends on how many units */
                        /* at present: xy, u'v' & uv */
  switch (xymode_tmp ) {
    case 1:
      whref_tmp = whref[6]->x;
      break;

    case 2:
    case 3:
      whref_tmp = whref[6]->u;
      break;

    case 4:
      whref_tmp = whref[6]->y;
      break;

    case 5:
      whref_tmp = whref[6]->v_;
      break;

    case 6:
      whref_tmp = whref[6]->v;
      break;
  }

  if (dwnup) 
    whref_tmp += .001;
  else 
    whref_tmp -= .001;
                                  /* keep inside limits */
  if (whref_tmp > whrefxy_max) 
    whref_tmp = whrefxy_max;
  if (whref_tmp < whrefxy_min) 
    whref_tmp = whrefxy_min;

  switch (xymode_tmp) {
    case 1:
      ref_tmp = 12.*whref[6]->y + 3. - 2.*whref_tmp;

      whref[6]->u =(4.*whref_tmp)/ref_tmp;
      whref[6]->v_=(9.*whref[6]->y)/ref_tmp;
      whref[6]->v =(6.*whref[6]->y)/ref_tmp;

      whref[6]->x = whref_tmp;
      break;

    case 2:
    case 3:
      ref_tmp = 16.*whref[6]->v - 12. - 6.*whref_tmp;

      whref[6]->x = (-9.*whref_tmp)/ref_tmp;
      whref[6]->y = (-4.*whref[6]->v)/ref_tmp;

      whref[6]->u = whref_tmp;
      break;

    case 4:
      ref_tmp = 12.*whref_tmp + 3. - 2.*whref[6]->x;

      whref[6]->u  = (4.*whref[6]->x ) / ref_tmp;
      whref[6]->v_ = (9.*whref_tmp ) / ref_tmp;
      whref[6]->v  = (6.*whref_tmp ) / ref_tmp;

      whref[6]->y = whref_tmp;
      break;

    case 5:
      ref_tmp = 16.*whref_tmp - 12. - 6.*whref[6]->u;

      whref[6]->y = (-4.*whref_tmp)/ref_tmp;
      whref[6]->x = (-9.*whref[6]->u)/ref_tmp;
      whref[6]->v = (2./3.)*whref_tmp;

      whref[6]->v_ = whref_tmp;
      break;

    case 6:
      ref_tmp = 16.*(3./2.)*whref_tmp - 12. - 6.*whref[6]->u;

      whref[6]->y = (-4.*(3./2.)*whref_tmp)/ref_tmp;
      whref[6]->x = (-9.*whref[6]->u)/ref_tmp;
      whref[6]->v_ = (3./2.)*whref_tmp;

      whref[6]->v = whref_tmp;
      break;
  }

  outtextxy(15, 3+xy, ftoa(whref_tmp, 3));    /* write new value */
}

/***************************************************************************/
/*  countRGB                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Count CFACTOR, (projector ONLY) or RGB ABS up/down        */
/*  Syntax:    void countRGB(char dwnup);                      */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void countRGB(char dwnup) {
  float step, count_tmp, fac_tmp;

  fac_tmp = 1;

  if (!rep1_delay)
    fac_tmp = 10;

  if (*menuno == 38) 
    count_tmp = cfactor[11]->cfactor;
  else
    count_tmp = (sup[0]->abs_rgb*unitfact*convfact)/measunit[1].unitfact;

  if (dwnup) {                                /* UP selected */
    if (count_tmp + 0.01*fac_tmp < 1.01) 
      step = 0.01;
    else 
      if (count_tmp + 0.1*fac_tmp < 100.1) 
        step = 0.1;
    else 
      if (count_tmp + 1*fac_tmp < 10000.1)
        step = 1.;
    else 
      step = 10.;
    count_tmp += step*fac_tmp;
  }
  else {                                                  /* DOWN selected */
    if (count_tmp - 0.01 < 1.01*fac_tmp) 
      step = 0.01;
    else 
      if (count_tmp - 0.1*fac_tmp < 100.1)
        step = 0.1;
    else 
      if (count_tmp - 1*fac_tmp < 10000.1)
        step = 1.;
    else
      step = 10.;
    count_tmp -= step*fac_tmp;
  }

  if (head_version == CRThead) {
    if  (*barptr[5] == 3) {
      if (count_tmp > 100.)
        count_tmp = 100.;
      if (count_tmp < 0.03)
        count_tmp = 0.03;
    }
    else {
      if (count_tmp > 343)
        count_tmp = 343.;
      if (count_tmp < 0.1)
        count_tmp = 0.1;
    }
  }

  if (head_version == PRJhead) {
    if (count_tmp > 99999)
      count_tmp = 99999;
    if  (count_tmp < 0.01) 
      count_tmp = 0.01;
  }

  if (*menuno == 38) {
    cfactor[11]->cfactor = count_tmp;
    outtextxy(14, 5, ftoa(count_tmp, no_of_decimals(count_tmp)));
    outtext(" ");
  }
  else {
    sup[0]->abs_rgb = count_tmp*measunit[1].unitfact/(unitfact*convfact);

    if (*menuon) {                          /* write new value */
      outtextxy(22, 7, ftoa(count_tmp, no_of_decimals(count_tmp)));
      outtext(" ");
    }
    else {
      outtextxy(38, 8, ftoa(count_tmp, no_of_decimals(count_tmp)));
      outtext("   ");
    }
  }
}

/***************************************************************************/
/*  tbutcheck                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951127                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Command decode for TEST mode                      */
/*  Syntax:    void tbutcheck(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void tbutcheck(void)
  {
  char* t;
  char i, j, romstring[9];
  unsigned int sum;
  unsigned int k;

  feed_dog = 1;                        /* Feed watchdog */
  feed_dog = 0;

  if (newbut)
    {
    idle_cnt = idle_delay;                /* Refresh time-outs */
    light_cnt = light_delay;

    switch (butcmd)
      {
      case UP_Button:
        if (!testmode)
          {
          textbar(temp->select + 2, 0);     /* Erase old bar */
          temp->select -= 1;
          if (temp->select < 1)
            temp->select = testmenuptr->maxline;
          textbar(temp->select + 2, 1);      /* set new bar */
          }
        break;

      case DOWN_Button:
        if (!testmode)
          {
          textbar(temp->select + 2, 0);       /* erase old bar */
          temp->select += 1;
          if (temp->select > testmenuptr->maxline)
            temp->select = 1;
          textbar(temp->select + 2, 1);        /* set new bar */
          }
        break;

      case ENTER_Button:
        if (!testmode)
          {
          cls();
          *barptr[0] = temp->select;

          switch (temp->select)
            {
            case 1:
              testmode = 1;
              outtextxy(1, 3, "Version: ");
              outtextxy(5, 5, software_date);
              break;
            case 2:
              testmode = 2;
              outtextxy(1, 4, "Sensor ID:");
              read_headid();
              break;
            case 3:
              testmode = 3;
              outtextxy(10, 5, "ROM check");
              outtextxy(23, 7, "checking...");
              
              if (do_checksum_test(&sum))
                {
                outtextxy(5, 7, "ROM OK   ");
                sprintf(romstring, "%02X", sum);
                outtext(romstring);
                }
              else 
                outtextxy(3, 8, "checksum error");

              break;
            case 4:                      /* dots check */
              testmode = 4;
              for (i = 127; i >= 0; i--)
                {
                hline(0,63,i,1);    /* write lines, one at a time */
                feed_dog = 1;      /* feed watchdog */
                feed_dog = 0;
                }
              break;
            case 5:              /* non shut-down mode */
              testmode = 5;
              outtextxy(3, 5, "NON SHUT-DOWN");
              outtextxy(20, 6, "MODE");
              outtextxy(15, 15, "last:");
              out_laptime(15);      /* show last on-time */
              laptics = 0;        /* reset time counter */
              *laphr = *lapmin = *lapsec = 0;
              outtextxy(0,8,"elapsed:");
              non_shdwn = 1;        /* set mode flag */
              break;
            case 6:              /* Upload to PC */
              testmode = 6;
              /* Upload implemented as: UPxxxxaaaaaz  */
              /*    xxxxx = Hex address, a = binary data and */
              /*    z = Checksum */
              /* 951127: size_of_allocbuf: 4500 */

              /* char allocbuf[0]         Head version  */
              /* char allocbuf[1-2499]    CRT Sensor    */
              /* char allocbuf[2500-4498] PRJ Sensor    */
              for (k = 0; k < 300; k++)
                {                           /* 300 * 16 */
                sendi("UP");
                sprintf(romstring, "%04X", (int)(k*16));
                sendi(romstring);
                sum = 0;
                for (j = 0; j < 16; j++)
                  {
                  i = allocbuf[k * 16 + j];
                  sum += i;
                  sendbyte(i);
                  feed_dog = 1;            /* feed watchdog */
                  feed_dog = 0;
                  }
                sendbyte(sum);
                outtextxy(6, 7, "Address:");
                outtextxy(43, 7, romstring);
                }
              clrtext(7);                /* Clear line 7 */
              outtextxy(8, 7, "RAM has been");
              outtextxy(17, 8, "uploaded");
              break;
            case 7:              /* Download from PC */
              testmode = 7;
              /* Download implemented as: DNxxxx  */
              /*    xxxxx = Hex address, a = binary data and */
              /*    z = Checksum */
              /* 951127: size_of_allocbuf: 4500 */

              /* char allocbuf[0]         Head version  */
              /* char allocbuf[1-2499]    CRT Sensor    */
              /* char allocbuf[2500-4498] PRJ Sensor    */
              for (k = 0; k < 300; k++)
                {
                feed_dog = 1;            /* feed watchdog */
                feed_dog = 0;
                sendi("DN");
                sprintf(romstring, "%04X", (int)(k*16));
                sendi(romstring);
                sendi(";");
                readbin = 16+sumon;
                sendi("RN16;");          /* Read 16 chars */
                if (!readstr())
                  {
                  clrtext(7);          /* Clear line 7 */
                  outtextxy(3, 7, "Receiver error");
                  break;
                  }
                t = rgbstring[0];
                sum = 0;
                for (i = 0; i < 16; i++) 
                  sum += *t++; 
                if (csum != *t++)
                  {
                  clrtext(7);          /* Clear line 7 */
                  outtextxy(3, 7, "Checksum error");
                  break;
                  }
                for (j = 0; j < 16; j++)
                  {
                  allocbuf[k * 16 + j] = rgbstring[j];
                  feed_dog = 1;            /* feed watchdog */
                  feed_dog = 0;
                  }
                outtextxy(8, 7, "Address:");
                outtextxy(45, 7, romstring);
                }
              if (k >= 300)
                {
                clrtext(7);              /* Clear line 7 */
                outtextxy(8, 7, "RAM has been");
                outtextxy(11, 8, "downloaded");
                }
              break;
            }
          }
        else
          {
          settestmenu();            /* go back to main test menu */
          non_shdwn = 0;
          }
        break;
      case ILLUM_Button:
        light_on ^= 1;
        lightoff = !light_on;        /* invert light status */
        break;
      case ON_OFF_Button:
        shut_dwn();                /* shut down */
        break;
      case CIE_DOWN_Buttons:
        if (testmode == 5)
          {
          testmode = 51;
          resat = 0;
          clrtext(15);            /* erase 'last' time */
          outtextxy(9, 10, "will reset at");
          outtextxy(38, 9, " ");
          putdat(0x10);
          putdat(0x30);
          putdat(0x7e);
          putdat(0x30);
          putdat(0x10);
          outtextxy(9, 11, "Batt. warning");
          }
        break;
      }
    newbut = 0;
    }
  }

/***************************************************************************/
/*  phos_up                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle <UP> command in phosphor menu                */
/*  Syntax:    void phos_up(char tmp);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void phos_up(char tmp) {
  char i;

  if (phos_org + tmp - 2 > 0) {          /* if not first phosphor */
    textbar(tmp+2, 0);                /* then erase old bar */

    if (tmp > 1) 
      temp->select -=1;                /* bar up */

    if ((temp->select == 1) && (phos_org > 1)) {

      for (i=4; i < menu[3].maxline+1; i++)
        clrtext(i);

      phos_org -= 1;
      out_phos_name();
    }
    textbar(temp->select+2, 1);          /* set new bar */
    pause(100);
    repeat_on = butenable = 1;
  }
}

/***************************************************************************/
/*  phos_dwn                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle <DOWN> command in phosphor menu                */
/*  Syntax:    void phos_dwn(char tmp);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void phos_dwn(char tmp) {
  char i;

  if (phos_org + tmp - 4 < phosmax) {          /* if not last phosphor */
    textbar(tmp+2, 0);                  /* then erase old bar */

    if (tmp < menuptr->maxline)
      temp->select +=1;                   /* bar down */
    if ((temp->select==menuptr->maxline)&&\
              (phos_org<phosmax-menu[3].maxline+4)) {

      for (i=4; i < menu[3].maxline+1; i++)
        clrtext(i);

      phos_org += 1;
      out_phos_name();
    }
    textbar(temp->select+2, 1);            /* set new bar */
    pause(100);
    repeat_on = butenable = 1;
  }
}

/***************************************************************************/
/*  UP_but                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <UP> button                          */
/*  Syntax:    void UP_but(void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void UP_but(void) {
  if (*menuon) {
    if (*menuno == 15) {            /* special for "Learn CRT/Phosp" menu */
      select = tmp;
      textbar(select + 2, 0);              /* erase old bar */
      select -=1;
      if (select == 7)
        select = 6;                   /* only 4,5,6,8,9 ok */
      if (select < 4)
        select = 9;
      temp->select = select;              /* store temporary */
      textbar(select+2, 1);              /* set new bar */
    }
    else
      if (*menuno == 3 || *menuno == 16)       /* Phosphor menus */
        phos_up(tmp);
    else 
      if (*menuno == 38) {
        select = tmp;
        textbar(select + 2, 0);              /* erase old bar */
        select -=1;
        if (select < 3)
          select = maxline;
        else
          if (select == 4)
            select = 3;                  /* only 3,5,6, ok */
        temp->select = select;              /* store temporary */
        textbar(select+2, 1);              /* set new bar */
      }
    else
      if ((*menuno != 7) && (*menuno != 6) && (*menuno != 33)) {
        barup();
        if (((*menuno == 18) || (*menuno == 28)) && (temp->select < 3))
          whref_done = 1;            /* touched "x=" or "y=" in */
                                /* "Learn White ref." */
    }
  }
  else 
    if (!*freeze) {                  /* zoom up if measuring mode */
      set_setup_no = 0;
      outtextxy(49, sup[0]->mode_no, "       ");
      if (sup[0]->zoom > 4)
        sup[0]->zoom /= 5;
      else
        sup[0]->zoom = 25;
      draw_refline(sup[0]->zoom);
    }
}

/***************************************************************************/
/*  DOWN_but                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <DOWN> button                        */
/*  Syntax:    void DOWN_but(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void DOWN_but(void) {

  if (*menuon) {
    if (*menuno == 15) {          /* special for "Learn CRT/Phosp" menu */
      select = tmp;
      textbar(select + 2, 0);            /* erase old bar */
      select +=1;
      if (select == 7)
        select = 8;                  /* only 4,5,6,8,9 ok */
      if (select > 9)
        select = 4;
      temp->select = select;            /* store temporary */
      textbar(select+2, 1);            /* set new bar */
    }
    else
      if (*menuno ==3 || *menuno ==16)     /* Phosphor menu */
        phos_dwn(tmp);
    else
      if (*menuno == 38) {
        select = tmp;
        textbar(select + 2, 0);        /* erase old bar */
        select +=1;
        if (select > maxline)
          select = 3;
        else
          if (select == 4)
            select = 5;               /* only 3,5,6, ok */
        temp->select = select;          /* store temporary */
        textbar(select+2, 1);          /* set new bar */
      }
    else
      if ((*menuno != 7) && (*menuno!=6) && (*menuno !=33)) {
        bardown();
        if (((*menuno == 18) || (*menuno==28)) && (temp->select < 3))
          whref_done = 1;            /* touched "x=" or "y="*/ 
                                /* in "Learn White ref." */
    }
  }
  else 
    if (!*freeze) {                  /* zoom down if measur. mode */
      outtextxy(49, sup[0]->mode_no, "       ");
      set_setup_no = 0;
      if (sup[0]->zoom < 6)
        sup[0]->zoom *= 5;
      else
        sup[0]->zoom = 1;
      draw_refline(sup[0]->zoom);
    }
}

/***************************************************************************/
/*  RGB_but                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <RGB> button                          */
/*  Syntax:    void RGB_but(void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void RGB_but(void) {
  if (*menuon && ((*menuno == 1) || (*menuno == 30)))    /* if in rgb menu */
    bardown();
  else 
    if ((!*menuon) && (sup[0]->mode_no == RGB_mode)) {
      if (*freeze) {
        *freeze = 0;
        initmode();                    /* if in RGB measur. mode */
      }
      else 
        switch (head_version) {
          case CRThead:
            setmenu(1); 
            break;
          case PRJhead:
            setmenu(30); 
            break;
        }
    }
  else {
    set_setup_no = *freeze = 0;
    sup[0]->mode_no = RGB_mode;
    initmode();
  }                             /* if in xyY mode, set RGB mode */
}

/***************************************************************************/
/*  xyY_but                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <xyY> button                          */
/*  Syntax:    void xyY_but(void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void xyY_but(void) {
  switch (head_version) {
    case CRThead:
      set_setup_no = *freeze = 0;
      sup[0]->mode_no = xyY_mode;
      initmode();                  /* Set xyY mode */
      break;

    case PRJhead:
      cls();
      outtextxy(4,  7, " CIE measuring  ");
      outtextxy(4,  8, "    mode is not   ");
      outtextxy(4,  9, "  available with ");
      outtextxy(4, 10, " applied sensor ");
      *freeze = 0;
      pause(255);
      pause(255);
      pause(255);
      pause(255);
      initmode();
      break;
  }
}

/***************************************************************************/
/*  FUNCT_but                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <FUNCT> button                        */
/*  Syntax:    void FUNCT_but(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void FUNCT_but(void) {
  if (*menuon && ((*menuno == 2) || (*menuno == 31)))
    bardown();
  else                             /* else set "Function" menu */
    switch (head_version) {
      case CRThead:
        setmenu(2); 
        break;
      case PRJhead:
        setmenu(31); 
        break;
    }
}

/***************************************************************************/
/*  RECALL_but                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <RECALL> button                        */
/*  Syntax:    void RECALL_but(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void RECALL_but(void) {
  if (*menuon && (*menuno == 11) && (tmp == 1)) {
    if (sup[0]->toggle_setup) {
      sup[0]->toggle_setup = 0;
      *barptr[13] = 1;
    }
    else {
      sup[0]->toggle_setup = 1;
      *barptr[13] = 2;
    }
    recall_setup(sup[0]->toggle_setup + 1);
    initmode();                      /* go to Measure mode */
  }
  else
    setmenu(11);                       /* else set "Recall" menu */
}

/***************************************************************************/
/*  STORE_but                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the <STORE> button                        */
/*  Syntax:    void STORE_but(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void STORE_but(void) {
  if(*menuon && (*menuno == 8))
    bardown();
  else 
    setmenu(8);                      /* else set "Store" menu */
}

/***************************************************************************/
/*  butcheck                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951107                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the command decode for normal operation          */
/*  Syntax:    void butcheck(void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void butcheck(void) {

  feed_dog = 1;
  feed_dog = 0;

  if (newbut) {
    if ((head_version == PRJhead) && (new_head_version || newhead)) {
      clrtext(15);
      clrtext(16);
      newhead = new_head_version = 0;
    }

    if ((head_version == CRThead) && !newhead && new_head_version) {
      clrtext(15);
      clrtext(16);
      new_head_version = 0;
    }
    
    maxline = menuptr->maxline;
    tmp = temp->select;             /* HERE: tmp is assigned some value!!!! JK */

    idle_cnt = idle_delay;            /* refresh time-outs */
    light_cnt = light_delay;

    if (*menuon)
      menu_cnt = menu_delay;

    switch (butcmd) {
      case UP_Button:              /* "UP" BUTTON */
        UP_but();
        break;

      case DOWN_Button:              /* "DOWN" BUTTON */
        DOWN_but();
        break;

      case ENTER_Button:            /* "ENTER" BUTTON */
        enterbut();
        break;

      case RGB_Button:              /* "RGB" BUTTON */
        RGB_but();
        break;

      case CIE_Button:              /* "xyY" BUTTON */
        xyY_but();
        break;

      case ZOOM_Button:                 /* "ZOOM" BUTTON */
        if (!*menuon)
          autozoom();
        break;

      case FUNCT_Button:            /* "FUNCTION" BUTTON */
        FUNCT_but();
        break;

      case RECALL_Button:            /* "RECALL" BUTTON*/
        RECALL_but();
        break;

      case STORE_Button:            /* "STORE" BUTTON */
        STORE_but();
        break;

      case ON_OFF_Button:            /* "ON/OFF" BUTTON */
        shut_dwn();
        break;

      case ILLUM_Button:            /* "ILLUM" BUTTON */
        light_on ^= 1;
        lightoff = !light_on;        /* invert light status */
        break;

      case RESET_Button:              /* "RESET" BUTTON */
        *barptr[13] = menu[13].maxline;          
        recall_setup(11);            /* set master set-up */
        set_setup_no = *freeze = 0;
        initmode();                /* go to Measure mode */
        break;

      case RGB_DOWN_Buttons:                /* CIE_UP_Buttons */
        if (*menuon) {
          if ((*menuno == 18) || (*menuno==28))
            switch (tmp) {                /* count White-ref up */
              case 1: count(0,1); break;
              case 2: count(1,1); break;
            }
          else 
            if ((((*menuno == 30) || (*menuno == 1)) && (tmp == 5)) ||
                    ((*menuno == 38) && (tmp == 3)))
              countRGB(0);
        }
        else 
          if ((sup[0]->mode_no == RGB_mode) && (
                ((head_version == PRJhead) && (*barptr[30] == 5)) ||
                ((head_version == CRThead) && (*barptr[1] == 5))))
            countRGB(0);
        break;

      case CIE_DOWN_Buttons:
        if (*menuon && ((*menuno == 18) || (*menuno==28)))
          switch (tmp) {                 /* count White-ref down */
            case 1: count(0,0); break;
            case 2: count(1,0); break;
          }
        break;

      case RGB_UP_Buttons:
        if ((*menuon) && ((*menuno == 30) || (*menuno == 1)) && (tmp == 5))
          countRGB(1);                        /* count ABS RGB ref up */
        else 
          if (((*menuon) && ((*menuno == 38) || (*menuno == 1))
              && (tmp == 3)) || 
              ((sup[0]->mode_no == RGB_mode) &&
              (((head_version == PRJhead) && (*barptr[30] == 5)) ||
              ((head_version == CRThead) && (*barptr[1] == 5))) && 
              !*menuon))
            countRGB(1);
        break;

      case ON_OFF_ENTER_Buttons:
        switch (head_version) {         /* set "Learn" menu */
          case CRThead:
            setmenu(14);
            break;  
          case PRJhead:
            setmenu(34);
            break;
        }
        break;

      case ILLUM_UP_Buttons:
        if (++(*contrast) > contrast_max)
          *contrast = contrast_max;    /* turn contrast up (to limit) */
        else 
          contrast_port = *contrast | 0x80;
        break;

      case ILLUM_DOWN_Buttons:
        if (--(*contrast) < contrast_min)
          *contrast = contrast_min;   /* turn contrast down (to limit) */
        else 
          contrast_port = *contrast | 0x80;
        break;
    }

    newbut = 0;                        /* enable for new command */
  }
}

/***************************************************************************/
/*  timetick                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951107                                    */
/*                                                  */
/* Module:    BUT.C                                      */
/*  Function:  Handle the timer tick interrupt                    */
/*  Syntax:    void timetick(void) interrupt 1;                  */
/*  Remarks:    DO NOT CALL!! THIS IS AN INTERRUPT ROUTINE ONLY          */
/*          Interrupt from timer 0 - address B                  */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void timetick(void) interrupt 1
  {
  TH0 = 0x9E;            /* reload timer for 25 ms count */
  TL0 = 0x7A;            /* theoretical 0x58; diff. is due to pushes */

  but = P1;                  /* detect button pressure */
  batt_low = ~but & 0x80;    /* and battery status */
  but |= 0x80;

  /* now sort button pressures. This is a complicated routine - */
  /* see special documentation for more explanation */
  if (butenable)
    {            /* skip if buttons not enabled */
    if (repeat_on)
      {          /* sort in repeat_on and combi_on */
      if (but == oldbut)
        {
        if (!rep_cnt)
          {
          butcmd  = but;
          newbut  =1;
          rep_cnt = rep_delay;
          if (rep_delay) 
            rep_delay -= 1;
          if (!rep_delay && rep1_delay)
            rep1_delay -= 1;
          }
        else 
          rep_cnt -= 1;
        }
      else
        repeat_on = combi = 0;          /* so that a release do not */
    }                                   /* produce a legal code */
    else
      {
      if (combi_on) /* combi_on: Flag: combination button(s) pressed */
        {
        if (but != oldbut)
          {
          if ((but == RGB_UP_Buttons)  ||
             (but == CIE_UP_Buttons)   ||
             (but == CIE_DOWN_Buttons) ||
             (but == ILLUM_UP_Buttons) ||
             (but == ILLUM_DOWN_Buttons))
            {
            rep_cnt = rep_delay = rep_start; 
            rep1_delay = rep1_start;
            newbut = repeat_on = 1;
            butcmd = oldbut = but;
            }
          else
            if ((but == RGB_Button)   ||
               (but == UP_Button)     ||
               (but == DOWN_Button)   ||
               (but == ENTER_Button)  ||
               (but == CIE_Button)    ||
               (but == ON_OFF_Button) ||
               (but == ILLUM_Button)  ||
               (but == ON_OFF_ENTER_Buttons))
              {
              combi &= but; 
              oldbut = but;
              }
          else
            if (but == NO_Button)
              {
              butcmd = combi; 
              newbut = butenable = 1;
              combi_on = 0; 
              combi = 0xff; 
              }
          }
        }
      else                              /* not combi_on */
      if (but != NO_Button)
        {
        if (but == oldbut)
          {
          if ((((but == UP_Button) ||
               (but == DOWN_Button)) && (*menuno!=3) && (*menuno!=16)) ||
               (but == ENTER_Button)  ||
               (but == RGB_Button)    ||
               (but == CIE_Button)    ||
               (but == ON_OFF_Button) ||
               (but == ILLUM_Button)  ||
               (but == ON_OFF_ENTER_Buttons))
            {
            combi = but; 
            combi_on = 1;
            }
          else
            {           /* Not combi type */
            if ((but == 0xfe) || (but == 0xfd)  ||
               (but == 0xfb)  || (but == 0xf7)  ||
               (but == 0xef)  || (but == 0xdf)  ||
               (but == 0xbf))
              ;            /* if only 1 bit is zero, do nothing */
            else
              {            /* at least 2 bits are zero */
              butcmd = but; 
              newbut = 1; 
              butenable = 0;
              }
            }
          }
        else 
          oldbut = but;
        }
      else 
        oldbut = 0xff;
      }
    }                     /* end butenable */
  else
    if (but == NO_Button)
      {                   /* Not butenable*/
      butenable = 1; 
      oldbut = combi = 0xff; 
      repeat_on = combi_on = 0;
      }  

  if (non_shdwn)
    {    /* if in non shut down (test) */
    laptics++;
     if (laptics == 40)
       {
       laptics = 0;
         (*lapsec)++;
         if (*lapsec == 60)
           {
           *lapsec = 0;
           (*lapmin)++;
           if (*lapmin == 60)
             {
             *lapmin = 0;
             (*laphr)++;
             if (*laphr == 99)
               *laphr=0;
             }
           }
         }
     }
   else
     {
     if (menu_cnt )
       {              /* then test for menu time-out */
       menu_cnt--;
       if (!menu_cnt) 
         menu_exit = 1;
       }
     if (idle_cnt)
       {              /* test for main time-out */
       idle_cnt--;
       if  (!idle_cnt ) 
         shut_dwn();
       }
     if (light_on)
       {              /* test for light time-out */
       light_cnt--;
       if  (!light_cnt)
         { 
         light_exit = 1; 
         light_on = 0; 
         }
       }
     }

  if (timecount)
    timecount--;
  if (GenericTimecount)
    GenericTimecount--;
  }


/* NAME         bit do_checksum_test(unsigned int * csum)
 *
 * PURPOSE      To calculate checksum of 64k ROM contents and
 *              compare it to checksum value stored in ROM
 *
 * ARGUMENTS    unsigned int * csum: on return holds calculated
 *              checksum value
 *              
 * RETURNS      0 if checksum is not correct.
 *              1 otherwise
 *              csum is filled with calculated checksum value on return
 * EXTERNS      None.
 *
 * NOTES        5639 display ROM contains 16 bit checksum at
 *              two last bytes: 0xFFFE and 0xFFFF.  This checsum
 *              is calculated and written to binary file by 
 *              dksum.c program.  The checksum is calculated
 *              in the range of 0 - 0xFFFF in such manner that
 *              it is equal to to the int at 0xFFFE.
 */

bit do_checksum_test(unsigned int * csum)
  {
  unsigned long k;
  unsigned int  i; /* holds checksum value present at 0xFFFE */

  *csum = 0;
  k     = 0;
  i     = 0;
 
  while (k < SUMADR + 2)  /* #define SUMADR 0xFFFE */
    {                     /* #define CBYTE ((unsigned char *) 0x50000L) */
    *csum += CBYTE[i];
    k++;
    i++;
    feed_dog = 1;
    feed_dog = 0;
    }
  clrtext(7);
  i = 256 * CBYTE[0xFFFE] + CBYTE[0xFFFF];
  if (!(*csum - i))
    return (1);
  else 
    return (0);
  }

/* end of but.c */