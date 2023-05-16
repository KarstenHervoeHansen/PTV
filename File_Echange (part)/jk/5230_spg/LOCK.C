/*
********************************************************************
* Project	: PT5210  SYNC
* Filename	: LOCK.C
* Version	: 1.2
* Purpose	: Genlock routines
* Org.date	: 960430
* Author	: PRC
********************************************************************
HISTORY:
980915 prepared for compiler 7.73
970219 HLOCK speeded up.
970114 RELEASED 1.1
*/
#include	<xa.h>
#include	"equ.h"
#include	"ext.h"

extern void burstlock(void);
extern bit genlocken;
// extern UC vargod;	//TEST

/*
sbit P1_5 @ 0x38d;	//TEST
volatile UC testa000 @ 0xa000;	//TEST
volatile UC testa002 @ 0xa002;	//TEST
volatile UC testa004 @ 0xa004;	//TEST
volatile UC testa006 @ 0xa006;	//TEST
volatile UC testa008 @ 0xa008;	//TEST
volatile UC testa00a @ 0xa00a;	//TEST
near UI testtal, testgltal;	//TEST
near UC gem1, gem2, gem3, gem4;	//TEST
near UC fejltel;	//TEST
*/

banked interrupt void lock(void)
{  SWR &= ~lockreq;		// turn off request 
//   P1_5= 0;	//TEST

 if (!freq_sel)		// NOT FREQUENCY-LOCK
 {   if (!genlocken)
	goto SETINT;
   if (verr>0)
	goto SETINT;
   if (d1_sel)
	goto BULOD1;
   burstfase();
   if (blackav<=syg1)
	goto SETINT;
   if (g_sel) { if (!syncav && blackav<=syg2_g)  goto SETINT; }
   else if (!syncav && blackav<=syg2_m)  goto SETINT;

    // CKSLOW:
   if (slowlock)
      goto SETSLOW;
   if (!bulocken)
      goto NOBU;
   if (g_sel)
   {  if (buamav<=buamg1_g)  goto NOBU;
      if (!buav && buamav<=buamg2_g)  goto NOBU;
   }
   else
   {  if (buamav<=buamg1_m)  goto NOBU;
      if (!buav && buamav<=buamg2_m)  goto NOBU;
   }
   // PALG:
   if (lockdet)
   {  errpre= 3;
      outs= palglock;
   }
   else
   {  errpre= 1;
      outs= palgunl;
   }
   // BULO:
   burstlock();
   goto OUTLOCK;

BULOD1:
   outs= d1lock;
   goto OUTLOCK;
   
NOBU:
   if (lockdet=1)
   {  errpre= 3;
      outs= hlocked;
   }
   else
   {  errpre= 1;
      outs= hunl;
   }
   // HLOC:
   hlock();
   goto OUTLOCK;

SETSLOW:
   outs= slowerr;	
   intern();
   goto OUTLOCK;
    
SETINT:
/*   if (vargod)
   {  vargod= 0;
   }
*/
   outs= internl;
   intern();

   OUTLOCK:
   if (level)  outport= outs | 0x20;
   else  outport= outs & ~0x20;
   dolock= 0;
 }

 else			// FREQUENCY-LOCK
 {   if (!genlocken)
      goto SETINTF;
   // if (verr>0)
   //   goto SETINTF;
   freqfase();
   if (ns)
      goto SETINTF;

   // BULO:
   outs= flock;
   burstlock();
   goto OUTLOCKF;
   
SETINTF:
   outs= internl;
   intern();

OUTLOCKF:
   outport= outs;
   dolock= 0;
 }
//   P1_5= 1;	//TEST
   
} // END of 1.6: lock *****

// ************************************************************
// static bit yneg;


void burstfase(void)
{  UC s1,s2,s3,s4;
   UC y,x;
   UI i;
   UC z;
   static bit yneg;
   
   if (g_sel)
   {  i= hpos + bupos_g;
      if (i>hlen_g)  i -= hlen_g;
   }
   else
   {  i= hpos + bupos_m;
      if (i>hlen_m)  i -= hlen_m;
   }

   if (i>255)  i -= 4;

   // GETDUAL:
   s1= dual[i];
   s2= dual[i+1];
   s3= dual[i+2];
   s4= dual[i+3];
   if (g_sel)   black= (s1+s2+s3) /3;
   else  black= (s1+s2+s3+s4) /4;

   if (black > blackav)
   {  if (blackav!=255)
	  blackav += 1;
   }
   else if (blackav!=0)
	 blackav -= 1;

   // MAKEN:
   if (s4>s2)
   {  y= s4-s2;
      yneg= 1;
   }
   else
   {  y= s2-s4;
      yneg= 0;
   }

   x= s1 - s3;

   if (s3>s1)
   // NOTPOS:
   {  x= s3-s1;
      if (!yneg)
      {  buph= tableph[256 * x + y] + 0x0100;
	 z= tablepyt[256 * (x/2 + 128) + y/2];
      }
      else // NEGAY3:
      {	 buph= tableph[256 * y + x] + 0xfe00;
	 z= tablepyt[256 * ((UI)x/2 + 128) + y/2 + 128];
      }
 //    buph= buph*64;
   }
   else if (x==0)
   {  if (!yneg)
      {	 buph= 0x4000>>6;
	 z= tablepyt[y/2];
      }
      else
      {	 buph= 0xc000>>6;
	 z= tablepyt[y/2 + 128];
      }
   }
   else if (yneg)
   {  buph= tableph[256 * x + y] + 0xff00;
      z= tablepyt[128 * x + y/2 + 128];
   }
   else
   {  buph= tableph[256 * y + x];
      z= tablepyt[128 * x + y/2];
   }

/*
   testtal= (buph >> 2) & 0x7f;		//TESTAFSNIT
   {  if (testtal>testgltal)
      {  if (testtal-testgltal > 10)
            fejltel++;
      }
      else
      {  if (testgltal-testtal > 10)
            fejltel++;
      }
   }
   if (fejltel==255)
   	fejltel= 0;
   testgltal= testtal;
*/
 
   // ROTBUPH:

   buph= fasekor[buph & 0x3ff];

//   testa006= (UC)(buph >> 8);	//TEST

/*
   // TEST:
   // outf 12 bits to port freqh and freql, freqh-bit3 invert.
   //buph &= 0xfff;
   freqh= ((UC)(buph>>12)) ^8;  // high byte, bit 3 inverted
   a= (UC)(buph>>4);	// low byte
   freql= a;
   freqload= a;
*/

   buph= buph + fasepos[i];

//   testa008= (UC)(i >> 4);	//TEST

   
/*
   // TEST:
   // outf 12 bits to port freqh and freql, freqh-bit3 invert.
   //buph &= 0xfff;
   freqh= ((UC)(buph>>12)) ^8;  // high byte, bit 3 inverted
   a= (UC)(buph>>4);	// low byte
   freql= a;
   freqload= a;
*/

   // OUTBUFA:
    if (z>buamav)
    {  if (buamav!=255)
	  buamav += 1;
    }
    else if (buamav!=0)
	 buamav -= 1;

}
// ** END of 1.6.1: burstfase *****

// ************************************************************

void freqfase(void)
{  UC s1,s2,s3,s4;
   UC y,x;
   UI i;
//   UC z;
   static bit yneg;
   
   i= 0;
   // GETDUAL:
   s1= dual[i];
   s2= dual[i+1];
   s3= dual[i+2];
   s4= dual[i+3];

   // MAKEN:
   if (s4>s2)
   {  y= s4-s2;
      yneg= 1;
   }
   else
   {  y= s2-s4;
      yneg= 0;
   }

   x= s1 - s3;

   if (s3>s1)
   // NOTPOS:
   {  x= s3-s1;
      if (!yneg)
      {  buph= tableph[256 * x + y] + 0x0100;
      }
      else // NEGAY3:
      {	 buph= tableph[256 * y + x] + 0xfe00;
      }
   }
   else if (x==0)
   {  if (!yneg)
      {	 buph= 0x4000>>6;
      }
      else
      {	 buph= 0xc000>>6;
      }
   }
   else if (yneg)
   {  buph= tableph[256 * x + y] + 0xff00;
   }
   else
   {  buph= tableph[256 * y + x];
   }
 
   // ROTBUPH:
   buph= fasekor[buph & 0x3ff];

   // OUTBUFA:
}
// ** END of 1.6.1.f: freqfase *****

// *****************************************************************
void burstlock(void)
{  UC a;
   UI i;
   eph= subcref - buph;

/*
   // TEST:
   // outf 12 bits to port freqh and freql, freqh-bit3 invert.
   freqh= ((UC)(eph>>12)) ^8;  // high byte, bit 3 inverted
   a= (UC)(eph>>4);	// low byte
   freql= a;
   freqload= a;
*/

   i= oldph;
   oldph= eph;
   minus= 0;

   // make eph = (eph+i)/2  - (circular mean value) *

   if (eph > i)
   {  if ((eph - i) >= 0x8000)
	 minus= 1;
   }
   else if (i-eph >= 0x8000)
	 minus= 1;

   // CALC:
   eph= eph/2 + i/2 + (0x8000 * minus);

/*
   testtal= eph;		//TESTAFSNIT
   testa00a= testtal>>8;
   if (vargod)
   {  if (testtal>testgltal)
      {  if (testtal-testgltal > 0x800)
            testgltal= testtal;
      }
      else
      {  if (testgltal-testtal > 0x800)
            testgltal= testtal;
      }
   }
   testgltal= testtal;
*/



/*
   // TEST:
   // outf 12 bits to port freqh and freql, freqh-bit3 invert.
   freqh= ((UC)(eph>>12)) ^8;  // high byte, bit 3 inverted
   a= (UC)(eph>>4);	// low byte
   freql= a;
   freqload= a;
*/
   

   if ((phdet & 0xe000)==0 || (phdet & 0xe000)== 0xe000)
   {  phdet= eph>>1;
      if (phdet & 0x4000)  phdet |= 0x8000;
      else  phdet &= ~0x8000;
   }
   else
   { // TSPH15:
      if ((phdet & 0x8000)==0)
            phdet= eph>>1;
      else  phdet= (eph>>1) | 0x8000;
/*      
      {  if ((eph & 0x8000)==0)
            phdet= eph>>1;
         else  phdet= eph>>1;
      }
      else // TSPH2:
      {  if ((eph & 0x8000)==0)
	    phdet= (eph>>1) + 0x8000;
         else
         {  phdet= (eph>>1) + 0x8000;;
            //phdet |= 0x8000;
         }
      }
*/
   }


   // OUTDAC:  phdet upper 12 bits to port freqh and freql, freqh-bit3 invert.
   freqh= ((UC)(phdet>>12)) ^8;  // nipple 3 (bit 3 inverted)
   a= (UC)(phdet>>4);	// nipple 2+1
   freql= a;
   freqload= a;


   if (!freq_sel)	// NOT FREQUENCY-LOCK
      hfasec();

// ** END of 1.6.2 **
}
// ************************************************************
void hlock(void)
{  UC a;
   hfasef();
   phdiff= href - hinput;
   if (phdiff & 0x80000000)
   {//  phdiff= hinput - href;
	phdiff= -phdiff;
//      if (phdiff < 0x00020000)
      if (!(phdiff & 0xFFFE0000))
      { // TS400:
//	 if (phdiff >= 0x00000400)
	 if (phdiff & 0xFFFFFC00)
             outf= 0x401;
	 else outf= -phdiff;
      }
      else  // SETPHD:
      {	 if (g_sel)  phdiff= (UL)hlen_g * 256 - phdiff;
       	 else  phdiff= (UL)hlen_m * 256 - phdiff;
//         if (phdiff >= 0x00000400)
	 if (phdiff & 0xFFFFFC00)
            outf= 0x3ff;
	 else outf= phdiff;
      }
   }
   else // TSB1817:
//   {  if (phdiff < 0x00020000)
   {  if (!(phdiff & 0xFFFE0000))
      { // TS4002:
	 if (phdiff & 0xFFFFFC00)
//	 if (phdiff >= 0x00000400)
            outf= 0x3ff;
	 else outf= phdiff;
      }
      else
      {	 if (g_sel)  phdiff= (UL)hlen_g * 256 - phdiff;
       	 else  phdiff= (UL)hlen_m * 256 - phdiff;

//         if (phdiff >= 0x00000400)
	 if (phdiff & 0xFFFFFC00)
            outf= 0x401;
	 else outf= -phdiff;
      }
   }
   // SNDOUTF:
   outf <<= 1;

   // outf LOWER 12 bits to port freqh and freql, freqh-bit3 invert.
   freqh= ((UC)(outf/256)) ^8;  // high byte, bit 3 inverted
   a= (UC)(outf & 0x00ff);	// low byte
   freql= a;
   freqload= a;
   hfasec();
// ** END of 1.6.3 **
}
// ************************************************************
void intern(void)
{  UC a;
   freqh= (UC)(freqref>>8);
   a= (UC)(freqref & 0x00ff);
   freql= a;
   freqload= a;
}
// ** END of 1.6.4 **
// ************************************************************
void hfasec(void)
{  UC s1, s2;
   s1= (UC)(href>>11);
   s2= (UC)(hpos>>3);

   if (g_sel)
   {  if (s1>s2)
      {  s1 = s1-s2;
         if (s1>hcen_g)
         {  // S1BIG:
            s1= hcyk_g-s1;
	    if (s1>4)  linadd= add4;
         }
         else if (s1>4)  linadd= supr4;
      }
      else // S2BIG:
      {  s2= s2-s1;
         if (s2>hcen_g)
         { // S2BIG2:
	    s2= hcyk_g-s2;
	    if (s2>4)  linadd= supr4;
         }
         else if (s2>4)  linadd= add4;
      }
   }
   else
   {  if (s1>s2)
      {  s1 = s1-s2;
         if (s1>hcen_m)
         {  // S1BIG:
            s1= hcyk_m-s1;
	    if (s1>4)  linadd= add4;
         }
         else if (s1>4)  linadd= supr4;
      }
      else // S2BIG:
      {  s2= s2-s1;
         if (s2>hcen_m)
         { // S2BIG2:
	    s2= hcyk_m-s2;
	    if (s2>4)  linadd= supr4;
         }
         else if (s2>4)  linadd= add4;
      }
   }
}
