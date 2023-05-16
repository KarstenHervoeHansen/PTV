/*
********************************************************************
* Project	: PT5210  SYNC
* Filename	: FIEL.C
* Version	: 0.0	961030
* Purpose	: Field interrupts
* Org.date	: 960426
* Author	: PRC
********************************************************************
HISTORY:
*/

#include <xa.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <intrpt.h>
#include "equ.c"
#include "ext.h"

#define UC unsigned char
#define UI unsigned int

extern bit genlocken;

near UC vargod;	// TEST

banked interrupt void v_int(void)
{  if (!freq_sel)
 {   
   if (window)
   {  if (verr==0)
      {  if (syncav && genlocken)
         {  if (htel== -vdetect) // check if slow-lock (623,624,626,627 lines)
      	    {  slowlock= 0;
	       atel= apreset;
	       if (g_sel)  btel= bpreset_g;
	       else  btel= bpreset_m;
	    }
	    else  slowlock= 1;
         }
      }
      else  verr -= 1;

      if (verr==0)
    	 vargod= 1;	// TEST
      // TIMMAX:
      if (g_sel)  htel= -vlenmin_g;	// (-623) load TEST-TIMER
      else  htel= -vlenmin_m;		// (-523)
      cntenab= 1;
      window= 0;
   }
   else	// OUTSIDE:	v-intr. not in window *
   {  if (vargod)	
         vargod= 0;	//TEST

      verr= verrpre;		// count good field
      if (g_sel)  htel= -vlenmin_g;	// (-623) load TEST-TIMER
      else  htel= -vlenmin_m;		// (-523)
      cntenab= 1;			// start timer
   }
 }
// ** END of 2: fiel **
}

// ****************************************************************
// field time-out routine (routine no. 3)
banked interrupt void vtimer_int(void)
{  if (!freq_sel)
 {
   if (window==1)
   {  if (vargod)
	 vargod= 0;	//TEST

      window= 0;
      verr= verrpre;
      cntenab= 0;
   }
   else // NOTINT
   {  window= 1;
      htel= -windlen;
      cntenab= 1;
   }
 }
// END of 3: vtimerint **
}
