/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997                  */
/*  Project:    PM5639 Display Unit                                        */
/*  Module:     RES.C                                                      */
/*  Author:     Preben Christiansen, DEV                                   */
/*  Org. date:  901224                                                     */
/*  Rev. date:  970822, KEn  DEV                                           */
/*  Rev. date:  030715, JK   DEV                                           */
/*  Status:     Version 2.5                                                */
/*                                                                         */
/*  This module contains the following functions:                          */
/*                                                                         */
/*      void serial(void) interrupt 4                                      */
/*      void errtxt(bit n)                                                 */
/*      void test(void)                                                    */
/*      void initdisplay(void)                                             */
/*      int main(void)                                                     */
/*                                                                         */
/* Changes                                                                 */
/*  030818 Released as version 2.6: JK '94 luminance < 1.0 bug             */
/*  030715 Released as version 2.5: JK                                     */
/*         Modified to run correctly with '94 LCD sensor                   */
/*         Luminance value is multiplied by 10 if sensor is '94            */
/*         In this case value at mem loc 61 is 57 (put by '94 calibration  */
/*         program.  Function: read_id()                                   */
/*  970822 Released as version 2.3                                         */
/*  951207 Released as version 2.2                                         */
/*  951109 Revised startup procedure                                       */
/*  951030 Added transmission in main() of MS, (measure stop), to stop     */
/*         sensor, (necessary starting from serie 4 sensors                */
/* 920520  Now supports projector head                                     */
/* 920520  For earlier versions of history see u'v'-version                */
/***************************************************************************/
#include "pra.h"

#include <reg51.h>          /* Define 8051 registers */
#include <stdio.h>          /* Define I/O functions */
#include <stdlib.h>          /* Standard library */
#include <absacc.h>          /* Absolute addresses */
#include <math.h>            /* Math (abs,...) */
#include <run.h>            /* Own definitions */
#include <com.h>            /* Own definitions */
#include <sub.h>            /* Own definitions */
#include <but.h>            /* Own definitions */
#include <date.h>            /* Own definitions */
#include <defs.h>            /* Own definitions */
#include <tables.h>          /* Own definitions */

/***************************************************************************/
/*  Serial                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    RES.C                                      */
/*  Function:  interrupt from serial port - addr 23                */
/*  Syntax:    void serial(void) interrupt 4;                    */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void serial(void) interrupt 4
  {
  data unsigned char c;

  if (!sending)
    {
    c = SBUF;

    if ((c != ',') || readbin)
      {
      *rgbptr[rsno] = c;            /* Read char from RS232 to string */
      if (rgbptr[rsno] < &rgbstring[rsno][20])
        rgbptr[rsno]++;             /* Keep received string <= 20 chars */
    
      if ((c == '\15') && !readbin)
        {                           /* Check end of message */
        sendok             = 1;
        *(--rgbptr[rsno])  = '\0';  /* Delete 'return' */
        rsno = 0;

        for (c = 0; c < 4; c++)
          {
          *rgbptr[c] = '\0';          /* End string */
          rgbptr[c]  = rgbstring[c];  /* Reset text pointers */
          }
        }
      if (readbin) 
        readbin--;
      }
    else
      if (++rsno > 3)                 /* Comma detected */
        rsno = 0;                     /*  target next string */
    }
  RI = TI = busy = 0;                 /* Reset intrpt */
  }

/***************************************************************************/
/*  errtxt                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    RES.C                                      */
/*  Function:  Display various errors in the display                     */
/*  Syntax:    void errtxt(bit n);                            */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void errtxt(bit n) {

  if (n) {
    if (batt_low) {
       batt_warn(50, 6);            /* Battery low */
         err_disp = 1;

      if (!batlow ) 
        shut_dwn();                /* Battery under 6 V */
    }    

    if (errorno) {
      switch (errorno) {
        case 1:
          outtextxy(0, 16, "Sensor err. ");
          break;

        case 2:
          outtextxy(0, 16, "No Sensor   ");
          break;

        case 3:
          outtextxy(0, 16, "Wrong Sensor");
          break;

        case 4:
          outtextxy(0, 16, "Disp. out of range");
          break;
      }
      err_disp = 1;
    }
    else
      if (overload) {
        outtextxy(0, 16, "Overload          ");
        err_disp = 1;
      }
    else
      if (newhead && (*menuno == 3) ) {
        outtextxy(0, 14, "WARNING:          ");
        outtextxy(0, 15, "Sensor changed    ");
        outtextxy(0, 16, "Select phosphor   ");
        err_disp = 1;
      }
    else
      if (new_head_version || newhead) {
        outtextxy(0, 15, "WARNING:          ");
        outtextxy(0, 16, "Sensor changed    ");
        err_disp = 1;
      }
  }
  else
    if (err_disp) {
      if (errorno || overload || newhead || new_head_version)
        clrtext(16);                      /* erase error line */
      else 
        outtextxy(49, 16, "       ");        /* clear batt. sign */
      err_disp = overload = 0;
    }
}

/***************************************************************************/
/*  test                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    RES.C                                      */
/*  Function:  Handle the display unit in, (power-up), test-mode        */
/*  Syntax:    void test(void);                              */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void test(void) {

  temp->select = 1;

  settestmenu();

  while(1) {
    if (non_shdwn) 
      out_laptime(8);              /* show time in special mode */

    tbutcheck();

    if (light_exit) {              /* if time-out for backlight */
      light_exit = light_on = 0;        /* then turn light off */
      lightoff = 1;
    }

    if (testmode != 4) {                /* not in dots check */
      if (batt_low) {
        batt_warn(50, 6);              /* show battery low */
        if ((testmode == 51) && !resat) {
          resat = 1;            /* reset time when batt. warning */
          out_laptime(15);        /* transfer time to display bottom */
          outtextxy(9, 10, "was");
          outtextxy(15, 15, "last:");
          *laphr = *lapmin = *lapsec = 0;    /* reset time count */
        }
      }
      else
        clrtext(16);
    }
    if (!batlow) 
      shut_dwn();                      /* batt. under 6 V */

    pause(50);
  }
}

/***************************************************************************/
/*  initdisplay                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    RES.C                                      */
/*  Function:  Initialize the display after power-up                */
/*  Syntax:    void initdisplay(void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    Nothing                                    */
/*  Updates:    ----                                      */
/***************************************************************************/
void initdisplay(void) {

  if (newhead || new_head_version) {
    switch (head_version) {
      case CRThead:
        setmenu(3);              /* Set phosphor menu in CRT */
        out_phos_name();
        textbar(temp->select+2, 0);    /* set bar on "Exit" */
        temp->select = menu[3].maxline - 1;
        textbar(temp->select+2, 1);
        break;

      case PRJhead:
        setmenu(33);              /* Set status menu in PROJECTOR */
        show_status();
        break;
    }
  }
  else {
    switch(head_version) {
      case CRThead:
        if (*menuon) {
          switch (*menuno) {
            case 3:
              setmenu(3);
              out_phos_name();
              break;

            case 4:
              setmenu(4);
              out_ref_name();
              break;

            case 6:
              setmenu(6);
              show_status();
              break;

            case 1:
            case 2:
            case 5:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 29:
              setmenu(*menuno);
              break;

            default:
              initmode();
              break;
          }    
        }
        else
          if (*freeze) 
            recall_msm(*barptr[12]);   /* show recalled msm.*/
        else                     /* or init. measuring display */
          initmode();
        break;  

      case PRJhead:
        if (*menuon) {
          switch (*menuno) {
            case 4:
              setmenu(4);
              out_ref_name();
              break;

            case 33:
              setmenu(33);
              show_status();
              break;

            case 32:
              setmenu(32);
              out_cfactor_name();
              break;

            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 30:
            case 31:
              setmenu(*menuno);
              break;

            default:
              initmode();
              break;
          }    
        }
        else
          if (*freeze) 
            recall_msm(*barptr[12]);  /* show recalled msm.*/
        else                     /* or init. measuring display */
          initmode();
        break;
    }
  }
}

/***************************************************************************/
/*  main                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      901224                                    */
/*  Revised:    951109                                    */
/*                                                  */
/* Module:    RES.C                                      */
/*  Function:  PM5639 Display Unit main program                    */
/*  Syntax:    int main(void);                              */
/*  Remarks:    Note that it takes the sensor, (MAX1232), 1000ms to reset  */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
main(void)
  {
  bit          show_err, head_present;
  data char    i, Poweron_Button;
  unsigned int show_err_cnt;
  float        scanrate;

  Poweron_Button = P1;              /* Determine power -up or -down */
  
  if (!(Poweron_Button & 0x41))
    {                               /* If "ON" button pressed */
    XBYTE[0x1FFF] = 0;              /*  then clear power-off flag and */
    contrast_port = 0x80;           /*  bootstrap power-on */
    }
  else
    {
    if (XBYTE[0x1FFF] == power_down)
      {                             /* If wanted power_down */
      contrast_port = 0x00;         /*  bootstrap power-off and 
      while(1)                      /*  remain here until OFF */
        ;
      }
    else
      contrast_port = 0x80;        /* Bootstrap power-on */
    }
              /* SET-UP SERIAL PORT FOR 4800 BAUD */

  SCON = 0xD8;    /* 0xD8 = mode 3;   earlier: 0x50 = mode 1 */
  TMOD = 0x21;    /* Timer 1: 8bit, selfload; timer 0: 16bit */
  TCON = 0x55;    /* */

  TH0 = 0x9E;      /* Load -25000 (25ms) into timer 0 for common timing */
  TL0 = 0x7A;      /*   theoretical value is 0x58; */
                   /*   difference is time for pushes */
  TH1 = 0xF3;      /* Set serial port to 4800 Baud */
  ES = 1;          /* Enable serial port */
  ET0 = 1;         /* Enable timer 0 intrpt (time tick) */
  EA = 1;          /* Enable intrp. */
  
  PCON |= 0x80;    /* Set bit SMOD for double Baud-rate */


  pause(2);        /* Delay for at least 3 ms after RST to display has */
                   /*  been released */

  putcmd(systset[0]);
  for(i = 1; i < 9; i++)    /* Initialize LCD display driver */ 
    putdat(systset[i]);     /*  char wxh, line length, 64 lines */

  putcmd(scroll[0]);  
  for(i = 1; i < 9; i++) 
    putdat(scroll[i]);      /* Addresses */

  putcmd(0x5A);
  putdat(0x00);             /* HDOT SCR no offset */

  putcmd(0x5B); 
  putdat(0x0D);             /* Set overlay (2 layer), graphics, xor */

  putcmd(0x58);
  putdat(0x00);             /* Blank display */

  putcmd(csrform[0]);
  for(i = 1; i < 3; i++) 
    putdat(csrform[i]);     /* Cursor form */

  cls();                    /* Clear both screens */

  putcmd(0x59);
  putdat(0x14);             /* 2 layer; DISP ON/OFF flash and cursor off */

  contrast_port = contrast_default | 0x80;  /* Set default contrast */

  outtextxy(10, 8, "Initialising....");

  rsno = 0;                 /* Reset receive-string index */
  for (i=0; i<4; i++)
    rgbptr[i] = rgbstring[i];    /* Initiate pointers */

  date();                  /* Read software version (ie. date) */

                           /* Delay for Sensor to END resetting */

  pause(250);              /* 500ms */
  pause(250);              /* 500ms */
                           /* Delay for sensor to startup: app. 1s */

  sendi("MS;");            /* Stop ALL sensor activity, (necessary */
  sendi("MS;");            /*  in series 4+ sensors) */

  new_head_version = 0;

  last_head_version = allocbuf[0];


  if (!(head_version = read_id()))
    switch (last_head_version)
      {
      case CRThead:
      case PRJhead:
        head_version = last_head_version;
        break;

      default:
        head_version = last_head_version = allocbuf[0] = CRThead;
        break;
      }


  initall(Poweron_Button);  /* Allocate and initialise the memory */


  if (test_on)
    test();            /* Special functions if TEST-mode selected */

  if (install_head())
    {                  /* Initialize measuring head */
    head_present = 1;
    errorno = 0;
    }
  else
    {                  /* If head not responding, set defaults */
    initdisplay();
    head_present = sendok = 0;
    ramsize = 1024;
    phosmax = 30;
    xfact = yfact = zfact = 1.0;
    for (i = 0; i < 9; i++)
      {
      k[i]  = 1024.0;
      ki[i] = 1.;
      }
    errorno = 2;
    newhead = 0;
    }

  clrtext(16);

  while(1)
    {              /* Main loop */
    if (!*freeze) /* freeze: Flag: display are a recall. measum. */
      {
      if (head_present)
        {
        timecount = 200;            /* Time-out, 200*25ms */
        while (timecount && !sendok)
          butcheck();
        }
      else
        {
        timecount = 8;              /* Pause 8*25ms if no Sensor */
        while (timecount)
          butcheck();
        }
      if (!sendok)
        {                     /* Time-out, answer missing */
        if (!errorno)
          {
          outtextxy(0, 16, "Sensor err.    ");
          errorno = 1;
          }
        if (!install_head())         /* Try to re-install head */
          head_present = sendok = 0; /*  failed */
        else
          {  
          head_present = 1;          /*  succeeded */
          sample->x = sample->y = sample->z = 0.0;
          for (i = 0; i < 4; i++)
            rgbptr[i] = rgbstring[i];  /* Reset text pointers */
          rsno = sendok = 0;           /* Reset receive string no. */
          }
        }                    
      else
        {                         /* There was an answer */
        sendok = 0;
        if (errorno)
          {
          errorno = 0;
          clrtext(16);            /* Clear the error */
          }
        /* Test if any scan value   is received */ 
        noscan = (!rgbstring[3][0]) ? 1 : 0;
        }
      if (newhead)
        if ((head_version == CRThead) && (!*menuon || *menuno != 3))
          initdisplay();

      if (bad_meas)            /* If a bad measurement */
        bad_meas = 0;           /*  skip values until new measurement */
      else
        {                /* else use measurement */
        if (!errorno)
          {
          scanrate = noscan ? 1.0 : (fabs(atof(rgbstring[3])) / 20.0);

          sample->x = atof(rgbstring[0]) / scanrate;
          sample->y = atof(rgbstring[1]) / scanrate;
          sample->z = atof(rgbstring[2]) / scanrate;

          /* If not menu and not recalled meas.*/
          if ((!*menuon) && (!*freeze))
            {
            if (sup[0]->mode_no == xyY_mode)  /* then display measur */
              outxyy();
            else 
              outrgb();
            }
          } 
        }
      }
    butcheck();                /* Test for a button command */
    if (menu_exit)
      {                        /* If time-out for menu */
      menu_exit = 0;           /*  then set measure mode */
      initmode();
      }
    if (light_exit)
      {                        /* If time-out for backlight */
      light_exit = 0; 
      light_on   = 0;          /*  then turn light off */
      lightoff   = 1;          /* P3.5 */
      }
    if (*freeze)
      {                        /* Error handling */
      if (++show_err_cnt > 5000)
        {
        show_err_cnt = 0;      /* If recalled measurem. then */
                               /*  make a pause to match error */
        errtxt(show_err ^= 1); /*  blinking in meas. mode */
        }
      }
    else                       /* Error blinking in menu or */
      errtxt(show_err ^= 1);   /*  measure mode */
    } /* main loop */
  }
