/*
********************************************************************
* Project	: PT5210  SYNC
* Filename	: LINE.C
* Version	: 1.1
* Purpose	: Line interrupt and lock routines
* Org.date	: 960731
* Author	: PRC
********************************************************************
HISTORY:
011126: function line7() modified, following problems with AutoHPH
        due to DC offsets in V101, V103 phase detector: JK
970226 in mode(): HREF calculation changed.
970114 RELEASED 1.1
*/
#include <xa.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <intrpt.h>
#include "equ.h"
#include "ext.h"
#include "tables.c"


// extern UC vargod;	// TEST
extern bit genlocken;
extern void mode(void);

code UC * near tableptr;

banked interrupt void line_int(void)
{  // UI i;

   if (freq_sel)  goto adown;
/*
   if (cntenab)		// count only H-lines if enabled
   {  htel += 1;
     if (htel==0)
		TF0= 1;	// counter was zero, set vtimer-interrupt
   }
*/
//   else	<970212d2
//      i= htel;
   if (ns==1)
      verr= verrpre;

adown:
   atel -= 1;		// count lines
   if (atel==0)
   {  btel -= 1;		// count 3xlines
     if (btel==0)
	// 1 frame gone
      {  if (freq_sel)  goto seta;
       	 if (syncav)
	 {  if (levelcnt)  levelcnt -= 1;	// good/bad field-filter
	 }
	 else
// NOSNC:
	 {  if (levelcnt & 128)
	    {  level = !level;
	       levelcnt= 0;
	    }
	    else levelcnt += 20;
	 }
// CKLOCK:
	 if (lockbit)
	 {  if (blackav > level2)
		level= 1;
	    else if (blackav <= level1)
		level= 0;
	 }
// SETA:
seta:
	 if (g_sel)
	 {  atel= afield_g; // re-load x-line counter
	    btel= bmax_g;	// re-load 70*9 counter
	    ftel -= 1;
	    if (ftel==0) ftel= fmax_g; // 8 fields gone, re-load frame counter
	 }
	 else
	 {  atel= afield_m; // re-load x-line counter
	    btel= bmax_m;	// re-load 70*9 counter
	    ftel -= 1;
	    if (ftel==0) ftel= fmax_m; // re-load frame counter
	 }
// GETDUR:
		// set subcref (correct later for frame no.) *
	 if (a10m_sel || genmode==3)
	 {  subcref= 0;
	    tableptr= tablestart_g + (4-ftel) * 28;
	 }
         else if (g_sel)
/*	 {  subcref= subcpot + (4-ftel) * 0x4000; // set new reference, G
		// #define tblsize 28
	    tableptr= tablestart_g + (4-ftel) * 28;
	 } */
	 {  subcref= subcpot;
	    if (ftel & 1)
	    {  if (ftel==1)  { subcref += 0xc000;
		tableptr= tablestart_g + (3 * 28);
	       }
	       else { subcref += 0x4000;
	    	tableptr= tablestart_g + 28;
	       }
	    }
	    else if (ftel==2)  { subcref += 0x8000;
	    	tableptr= tablestart_g + (2 * 28);
	       }
	    else tableptr= tablestart_g;
	 }
	 else
	 {  subcref= subcpot + (2-ftel) * 0x8000; // set new reference, M
		// #define tblsize 26
	    tableptr= tablestart_m + (2-ftel) * 26;
	 }

// DPOK:
// get new duration and line type from table **
	 lintyp= *tableptr; tableptr++;
	 cvartel= *tableptr; tableptr++;
	 typeport= lintyp;
      }
      else
      // SETAMAX:
      {  atel= amax;		// re-load 9-line counter
         cvartel -= 1;	// count duration (c= c-1)
    
         if (cvartel==0)
         {  lintyp= *tableptr++;	// Get new type and duration
	    cvartel= *tableptr++;
         }
         // NOTFRAME:
         typeport= lintyp;
      }
   }
   else
   // TSACAL:
   {  cvartel -= 1;	// count duration (c= c-1)
      if (cvartel==0)
      {  lintyp= *tableptr++;  // Get type and duration
	 cvartel= *tableptr++;
      }
      // TSACAL2:
      if (atel==acal)
      {	 typeport= (lintyp | linadd);	// line 6 of 9 is found
	 linadd= 0;
	 if (g_sel)  
	 {  if (a10m_sel || genmode==3)  subcref= 0;
	    else subcref += subcinc_g; // add 25Hz offset to subc
	 }
	 else  subcref += subcinc_m;	// add offset to subc
	 hpos= 256 * ehporth + ehportl;	// get ext. hpos
	 if (hpos & 0x8000)  lockdet= 1; else lockdet= 0;
	 hpos &= 0x07ff;
	 // find special lines

	 if (freq_sel || d1_sel)  doflags |= btable_10[btel];
	 else if (g_sel)  doflags |= btable_g[btel];
	 else  doflags |= btable_m[btel];

         SWR |= (doflags & 0x0e); // set request for lock, sch, line7
      }
      else
      // TSSAMP:
      {	 if (atel==asamp)  
            {  typeport= lintyp | samp;	// atel = 8 found
	    }
	 else  typeport= lintyp;
      }
   }
// ** END of 1: lineint *****
}

// ********************* modified by JK *****************
interrupt line7(void)
  { 
  UI i;
  SWR &= ~line7req;
  if (genlocken && syncav && !verr && buav) //<960509d
    {
    if (g_sel)
      {  
      if (ftel == 4)
        { 
        i = (oldph >> 8) & 0xfff0;  // original value: 0xfffc :JK
        if (i == PHA40 || i == PHA45)
          { // TSG1:
          phgod = errpre;
          if ((g_sel && phdiff < sch61_g) || (!g_sel && phdiff < sch61_m))
		        sphgod = errpre;
 //       else if (schph> sch62) sphgod= errpre; <960509d
          else 
            if (sphgod == 0)
              {  
              sphgod = errpre; 
  	          ftel   = 2;
  	          linadd = add2;
              }
            else  
              sphgod -= 1;
          }
        else 
          {  //TSM40:
          sphgod = errpre;
          if (i == PHAM40 || i == PHAM45)
            {// TSTPHGOD:
            if (phgod == 0)
              {  
              phgod  = errpre;
              ftel   = 3;
              linadd = add1;
              }
            else 
              phgod -= 1;
            }
          else 
            phgod = errpre;
          }
        } // if (ftel == 4)
      }
    else	// M-version
      { 
      if (ftel == 1)
        {  // TSG1:
        phgod = errpre;
        if ((g_sel && phdiff < sch61_g) || (!g_sel && phdiff < sch61_m))
	        sphgod = errpre;
 //    else if (schph> sch62) sphgod= errpre; <960509d
       else 
         if (sphgod == 0)
           {  
           sphgod = errpre; 
	         ftel   = 2;
	         linadd = add2;
           }
         else  
           sphgod -= 1;
        }
      }
    }
  doline7 = 0;
  mode();
  }
// ** END of 1.2: line7 (and mode) *****
// ***************************************************************
void palpuls(void)
{  UI i;
   i= hpos + papupos;
   if (g_sel)  { if (i >= hlen_g)  i= i - hlen_g; }
   else  if (i >= hlen_m)  i= i - hlen_m; 
   papua += dual[i];
   papua -= black;
   if (papua > papug)  epapu= 1;
   else epapu= 0;
}
// ***************************************************************
void mode(void)
{  if (g_sel)
   {  subcpot= (((UL)genphase_g * 1135 / hlen_g) % 1024) << 6;
      href= hphzero_g + (UL)genphase_g;
      if (href & 0x80000000)
	 href= href + ((long)hlen_g<<8);
      else
	 if (href >= ((long)hlen_g<<8))
	    href= href - ((long)hlen_g<<8);
   }
   else
   {  subcpot= (((UL)genphase_m * 910 / hlen_m) % 1024) << 6;
      href= hphzero_m + (UL)genphase_m;
      if (href & 0x80000000)
	 href= href + ((long)hlen_m<<8);
      else
 	 if (href >= ((long)hlen_m<<8))
	 href= href - ((long)hlen_m<<8);
   }
   domode= 0;
}
// ***************************************************************
banked interrupt sch(void)
{  SWR &= ~schreq;
   if (genlocken && syncav && !verr && buav)
   {  hfasef();
      phdiff= href - hinput;
//    schph= (phdiff % 1024) >> 2; //<960509d
      if (phdiff & 0x80000000)
      {  phdiff= hinput - href;
//         if (phdiff < 0x00020000)
         if (!(phdiff & 0xFFFE0000))
         {  if ((g_sel && phdiff>phmar_g) || (!g_sel && phdiff>phmar_m))
            {  if (godgren==1)
               {  if (schgod==0)
	          { linadd= add4; schgod= errpre;
	          }
	          else schgod -= 1;
	       }
	       else
	       {  godgren= 1; // SETGREN: 
	          schgod= errpre;
	       }
            }
            else
            {  // line7();
	       schgod= errpre;
            }
         }
         else // NEWPHD:
         {  if (g_sel)  phdiff= ((UL)hlen_g*256) - phdiff;
            else  phdiff= ((UL)hlen_m*256) - phdiff;
            if ((g_sel && phdiff>phmar_g) || (!g_sel && phdiff>phmar_m))
            {  if (godgren==2)
               {  if (schgod==0)
                  {  linadd= supr4; schgod= errpre;
                  }
                  else schgod -= 1;
               }
               else
               {  godgren= 2;
                  schgod= errpre;
               }
            }
            else
            {  // line7();
               schgod= errpre;
            }
         }
      }
      else // POSIT:
      {
//         if (phdiff < 0x00020000)
         if (!(phdiff & 0xFFFE0000))
         {  if ((g_sel && phdiff>phmar_g) || (!g_sel && phdiff>phmar_m))
            {  if (godgren==3)
	       {  if (schgod==0)
	          {  linadd= supr4; schgod= errpre;
	          }
	          else schgod -= 1;
	       }
	       else
	       {  godgren= 3;
	          schgod= errpre;
	       }
            }
            else
            {  // line7();
               schgod= errpre;
            }
         }
         else // TSPHAD:
         {  if (g_sel)  phdiff= (UL)hlen_g*256 - phdiff;
            else  phdiff= (UL)hlen_m*256 - phdiff;
            if ((g_sel && phdiff>phmar_g) || (!g_sel && phdiff>phmar_m))
            {  if (godgren==4)
               {  if (schgod==0)
                  {  linadd= add4;
                     schgod= errpre;
                  }
                  else schgod -= 1;
               }
               else
               {  godgren= 4;
                  schgod= errpre;
               }
            }
            else
            {  // line7();
               schgod= errpre;
            }
         }
      }
   }
   dosch= 0;
}	
// ** END of 1.5: sch *****

// ***********************************************************
//static near UI j;
void hfasef(void)
{  UI i,j;
   UC s1, s2, s3, x, y;

   if (g_sel)
   {  i= hpos - hstart_g;
      if (i & 0x8000)  i += hlen_g;
      // SETTRIG:
      trig= black>>1;

gets1:
      s1= dual[i];
      if (s1>trig)
      // S1BIG:
      {  j= i + 1;
         if (j==hlen_g)  j= 0;
         s2= dual[j];
         x= s1 - s2;
         y= s1 - trig;
         s3= tableam[y*256 +x];
         hinput= (UL)i <<8;
         hinput += s3;
      }
      else
      {  i -= 1;
         // TSI15:
         if (i & 0x8000)  i += hlen_g;
         goto gets1;
      }
   }
   else
   {  i= hpos - hstart_m;
      if (i & 0x8000)  i += hlen_m;
      // SETTRIG:
      trig= black>>1;

gets1m:
      s1= dual[i];
      if (s1>trig)
      // S1BIG:
      {  j= i + 1;
         if (j==hlen_m)  j= 0;
         s2= dual[j];
         x= s1 - s2;
         y= s1 - trig;
         s3= tableam[y*256 +x];
         hinput= (UL)i <<8;
         hinput += s3;
      }
      else
      {  i -= 1;
         // TSI15:
         if (i & 0x8000)  i += hlen_m;
         goto gets1m;
      }
   }
}
// ** END of 1.5.1: hfasef *****

