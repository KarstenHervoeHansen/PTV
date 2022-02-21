/*
********************************************************************
* Project   : PT5210  SYNC
* Filename  : FIEL.C
* Version   : 1.1
* Purpose   : Field interrupts (or frame interrupts?)
* Org.date  : 960426
* Author    : PRC
*
* this file has been modified following introduction of GPS Genlock
* module.  compared to SDI Genlock module, the GPS one generates 
* frame pulses (F_D1) once per 4 frames (G-system).  The function
* banked interrupt void v_int(void) is modified that it checks if 
* the GPS Genlock is the case, then it performs the following:
* ftel = fmax_g; and ftel = fmax_m;.
* The other modification is in equ.h, where we have different
* value of vlenmin_g in case on GPS Genlock:
* vlenmin_g = (int)(3 * 625 + 623).  This is used to load Timer0
* and compensates for missing frame pulses in case of GPS Genlock.
* vlenmin_g and vlenmin_m are now global variables (then were macros
* before) and are set according to gps_sel in genlockmode()
* function.

*
********************************************************************
HISTORY:
970114 RELEASED 1.1
*/

#include <xa.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <intrpt.h>
#include "equ.h"
#include "ext.h"

#define UC unsigned char
#define UI unsigned int

extern bit genlocken;

#if 0
NUC atel;        // taeller rundt til 5   paa line | disse 3 signaler
NUC btel;        // taeller rundt til 125 paa carry fra a | udgoer tilsammen en
NUC ftel;        // taeller rundt til 4   paa carry fra b | 8 field linie taeller
#endif

/* External Interrupt 0 (INT0, pin 14), triggered by SPGVINT signal */
/* SPGVINT signal comes from V12, pin 20.  How it is generated is   */
/* unknown.                                                         */
/* V-interrupt (2 fields) (frame interrupt)                         */
/* SPGVINT = (videoframe & !d1genlock & !freqlock) # (d1frame & d1genlock & !freqlock); */
banked interrupt void v_int(void)
  {

  if (!freq_sel)
    {   
    if (window)
      {
      if (verr == 0) //NUC verr; // taeller der taeller ned paa gode fields
        {
        if (syncav && genlocken) //#define vdetect 3  // v-teller vaerdi inden for window for 625 linier
          {
          if (TH0 == 255 && TL0 == 256 - vdetect) // if (htel== -vdetect) // check if slow-lock (623,624,626,627 lines)
            {
            slowlock = 0;
            atel     = apreset; //apreset = 2      // a vaerdi til brug ved field1 interrupt
            if (g_sel)
              {
              btel = bpreset_g; //bpreset_g = 208  // vaerdi til brug ved field1 interrupt
              if (gps_sel)
                ftel = fmax_g;    // 8 fields gone, re-load frame counter (fmax_g = 4) ************* JK ******************
              }    
            else
              {
              btel = bpreset_m; //bpreset_m = 175  // vaerdi til brug ved field1 interrupt
              if (gps_sel)
                ftel = fmax_m;    // 4 fields gone, re-load frame counter (fmax_m = 2) ************* JK ******************
              }
            }
          else  
            slowlock = 1;
          }
        }
      else
        verr -= 1;
      // TIMMAX:
      if (g_sel)  // htel= -vlenmin_g;  // (-623) load TEST-TIMER
        {
        TL0 = -vlenmin_g.charval[0];
        TH0 = -vlenmin_g.charval[1] - 1;
        }
      else  // htel= -vlenmin_m;    // (-523)
        {
        TL0 = -vlenmin_m.charval[0];
        TH0 = -vlenmin_m.charval[1] - 1;
        }
      cntenab = 1;
      window  = 0;
      }
    else // OUTSIDE: v-intr. not in window *
      {
      verr = verrpre;            // count good field (3)
      if (g_sel)  // htel= -vlenmin_g;  // (-623) load TEST-TIMER
        {
        TL0 = -vlenmin_g.charval[0];
        TH0 = -vlenmin_g.charval[1] - 1;
        }
      else  // htel= -vlenmin_m;    // (-523)
        {
        TL0 = -vlenmin_m.charval[0];
        TH0 = -vlenmin_m.charval[1] - 1;
        }
      cntenab = 1;     // start timer
      }
    }
  }

// ****************************************************************
// field time-out routine (routine no. 3)
banked interrupt void vtimer_int(void)
  {
  if (!freq_sel)
    {
    if (window == 1)
      {
      window  = 0;
      verr    = verrpre;
      cntenab = 0;
      }
    else // NOTINT
      {
      window  = 1;
      TL0     = 0xFB;  // values for WINDOW WIDTH (5)
      TH0     = 0xFF;
      cntenab = 1;
      }
    }
  }

#if 0
banked interrupt void vtimer_int(void)
  {
  if (!freq_sel)
    {
    if (window == 1)
      {
      window   = 0;
      verr     = verrpre;
      cntenab  = 0;
      }
    else // NOTINT
      {
      window  = 1;
      TL0     = -windlen & 0xff;
      TH0     = -windlen / 256 - 1;
      cntenab = 1;
      }
    }
  }
#endif


