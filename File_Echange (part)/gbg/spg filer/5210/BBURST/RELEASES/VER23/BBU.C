/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: BBU.C
* Version	: 2.3
* Purpose	: Main module, Interrupt- and Command-Handling.
* Org.date	: 960515
* Author	: PRC
********************************************************************
HISTORY:
	V2.3
980428 Error: SCH-phase is not correct after a system change(eg.HG to HM),
	as 'sch1buf' should be corrected with 'schcalm1' instead of
	'schcalg1' (modulus 2048).
971201 fixed: 'HQ','JQ','HB','JB' was not in kommand list.
	RELEASED 2.2
970925 Added two IIC dummy-reads to start-up to prevent doublestart error.
970908 #define version 021  corrected to ... 21 (021 is OCTAL).
9707	RELEASED 2.1
970703 'std' was not set in state 2 in CharHandler, now corrected.
	RELEASED 2.0
970224 Now check of 'std' if not 'req', 'save' or 'quit' (else a faulty
	command, e.g. HG? would be executed as HG <unknown number>)	
970120 VARS.H changed name to BBU.H
970114	RELEASED 1.1
970106 Added HA?, HQ?, HB?, HO?, HH?, -also for channel 2 (J..)
961205 Optimized for speed
961118 Calibration routines added
960925 Rearrangement of buffer load
*/

#include <xa.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <intrpt.h>
#include "equ.h"
#include "seri1.h"
#include "iniic.h"
#include "bbu.h"

// VERSION SETTING:     xxy = version xx.y, first 'x' can not be '0'.
 #define version	23

// V24 ADDRESS: (set to 0x00 if NOT option-BBU, 0xf0 if OPTION):
// #define defaddr 0xf0
#define defaddr 0x00



extern void syssetup(void);

NUC myaddress;
near char header1;
NUC cc;
NUL Number;		// Number received on v24-bus
NUI Number1;		// second number received on v24-bus
bit std,req,quit,save;	// flags for requests from frontplate
NUC errstatus;		// error status

NUL ts;				// total phase (from 'phases1' and 'phases2')
NUI vfase1, vfase2;		// no. of lines from origin (0-2499 in G)
NUI vfase1old, vfase2old;	// old no. of lines from origin
NUI vbuf1, vbuf2;		// new no. of lines from origin
NUI lincnt1, lincnt2;		// actual line no.
NUL hfase1, hfase2;		// no. of minor-steps from origin (a 0.14ns)
NUL hcalg1, hcalg2;		// H calibration values
NUL hcalm1, hcalm2;		// H calibration values
NUL lastphase1, lastphase2;	// store for latest H-phase
NUL offsetg1, offsetg2;		// global offset G ch.1 and 2 (a 0.14ns)
NUL offsetm1, offsetm2;		// global offset M ch.1 and 2 (a 0.14ns)
NUI subfa1, subfa2;		// no. of subc-steps (0-2047 in G)
NUI sf1buf, sf2buf;		// new no. of subc-steps
NUI subcoffs1, subcoffs2;	// subcarr. offset steps
bit syst1, syst2;		// TV system (1=G)
bit sys1buf, sys2buf;		// new TV system (1=G)
bit poff1, poff2;		// flag for setup OFF (M) or line7 ON (G)
bit poff1buf, poff2buf;		// buffers for poff
NUL hf1buf, hf2buf;		// new no. of minor-steps from origin
NUI sch1, sch2;			// SCH-phase in minor-steps
NUI schcalg1, schcalg2;		// SCH-calibration values
NUI schcalm1, schcalm2;		// SCH-calibration values
NUI sch1buf, sch2buf;		// new SCH-phase in sub-steps
bit loaded1, loaded2;		// flags for new values available
NUC dalev1, dalev2;		// DAC levels
NUC daoff1, daoff2;		// DAC offsets
// *****************************************************************

void NotFound(void)
{
}
// *****************************************************************

near struct kommando
{	 char Aa;
	 char Bb;
	 void (*funktion)(void);
};
// *****************************************************************

banked interrupt void ext0_int(void)
{  UC lintyp;

   P3_0= 0;	// for test

   if (fieldpuls1)
   {
      if (loaded1)
      { // COPY NEW PHASES
	 hfase1= hf1buf;
	 vfase1= vbuf1;
	 syst1= sys1buf;
	 poff1= poff1buf;
	 subcoffs1= sf1buf + sch1buf;
	 loaded1= 0;
      }
      lincnt1= vfase1;
   }

   if (vfase1==vfase1old)  lincnt1 += 1;
   else
   {  if (vfase1<vfase1old)  lincnt1 += 2;
      vfase1old= vfase1;
   }

   if (syst1)
   {  if (lincnt1 >= gmod)  lincnt1= 0;
      lintyp= typeg[lincnt1] << 4;
      subfa1= (sfaseg[lincnt1] + subcoffs1) | 0x8000;
   }
    else
   {  if (lincnt1 >= mmod)  lincnt1= 0;
      lintyp= typem[lincnt1] << 4;
      subfa1= (sfasem[lincnt1] + subcoffs1) & ~0x8000;
   }

   if (poff1)  lintyp |= 8;
   typeport1= lintyp | (hfase1>>16 & 7);
   hfasefport1= ~((UC)(hfase1%256));

//   hfasecport1= (UC)(hfase1/256);
 #asm
   mov.w r1,#_hfasecport1
   mov.b[r1], _hfase1+1
#endasm

   bulowport1= (UC)(subfa1 & 0x00ff);
   buhiport1= (UC)((subfa1 & 0xff00)>>8);

   P3_0= 1;	// FOR TEST
}
// *****************************************************************
banked interrupt void ext1_int(void) // FV_BB2
{  UC lintyp;

   P3_0= 0;	// for test

   if (fieldpuls2)
   {  
      if (loaded2)
      {  // COPY NEW PHASES
	 hfase2= hf2buf;
	 vfase2= vbuf2;
	 syst2= sys2buf;
	 poff2= poff2buf;
	 subcoffs2= sf2buf + sch2buf;
	 loaded2= 0;
      }
      lincnt2= vfase2;
   }

   if (vfase2==vfase2old)  lincnt2 += 1;
   else
   {  if (vfase2<vfase2old)  lincnt2 += 2;
      vfase2old= vfase2;
   }

   if (syst2)
   {  if (lincnt2 >= gmod)  lincnt2= 0;
      lintyp= typeg[lincnt2] << 4;
      subfa2= (sfaseg[lincnt2] + subcoffs2) | 0x8000;
   }
   else
   {  if (lincnt2 >= mmod)  lincnt2= 0;
      lintyp= typem[lincnt2] << 4;
      subfa2= (sfasem[lincnt2] + subcoffs2) & ~0x8000;
   }

   if (poff2)  lintyp |= 8;
   typeport2= lintyp | (hfase2>>16 & 7);
   hfasefport2= ~((UC)(hfase2%256));

//   hfasecport2= (UC)(hfase2/256);
 #asm  
   mov.w r1,#_hfasecport2
   mov.b[r1], _hfase2+1
#endasm
   
   bulowport2= (UC)(subfa2 & 0x00ff);
   buhiport2= (UC)((subfa2 & 0xff00)>>8);

   P3_0= 1;	// FOR TEST
}
// *****************************************************************


// *****************************************************************
// TABLE FOR ARRIVING COMMANDS	FROM V24
// *****************************************************************
// definitions for calculation of phases (used in phases1() and phases2()).
//   antal subc-steps = faktor * sync-steps (a 0,14 ns)
//   faktor= 3.579545   * 8 / 27    for G-system (1.31366481..)
//   faktor= 4.43361875 * 8 / 27    for M-system (1.06060592..)
//         = (fak1 * 65536 + fak2 ) / 2^31
#define fak1g	43046
#define fak2g	11053
#define vmodg	442368
#define fak1m	34753
#define fak2m	61275
#define vmodm	439296
#define tmodg	1105920000
#define tmodm	461260800
// *****************************************************************

void dummy(void)
{ ;
}
// *****************************************************************

void phases1(void)		// puts new phase in first buffer
{ 				// channel 1
   UL b, r;

   while (loaded1) ;

   if (sys1buf)
   {  ts= tmodg - Number + hcalg1 + offsetg1;
      vbuf1= ((UI)(ts / vmodg)+1)%gmod;	// no. of lines (pr. 1728*256)
      hf1buf= ts % vmodg;	// the rest
      b= ts & 0x000000ff;
      r= (b * fak1g) * 2 +  ((b*fak2g)>>15);
   }
   else
   {  ts= tmodm - Number + hcalm1 + offsetm1;
      vbuf1= ((UI)(ts / vmodm)+1) % mmod; // no. of lines (pr. 1716*256)
      hf1buf= ts % vmodm;	// the rest
      b= ts & 0x000000ff;
      r= (b * fak1m) * 2 +  ((b*fak2m)>>15);
   }
   sf1buf= r>>16;
   loaded1= 1;
}
// *****************************************************************

void phases2(void)		// puts new phase in first buffer
{ 				// channel 2
   UL b, r;

   while (loaded2) ;

   if (sys2buf)
   {  ts= tmodg - Number + hcalg2 + offsetg2;
      vbuf2= ((UI)(ts / vmodg)+1) % gmod; // no. of lines (pr. 1728*256)
      hf2buf= ts % vmodg;	// the rest
      b= ts & 0x000000ff;
      r= (b * fak1g) * 2 +  ((b*fak2g)>>15);
   }
   else
   {  ts= tmodm - Number + hcalm2 + offsetm2;
      vbuf2= ((UI)(ts / vmodm)+1) % mmod; // no. of lines 
      hf2buf= ts % vmodm;	// the rest
      b= ts & 0x000000ff;
      r= (b * fak1m) * 2 +  ((b*fak2m)>>15);
   }
   sf2buf= r>>16;
   loaded2= 1;
}
// *****************************************************************

void setadr(void)	// HA  -set V24 address (at start-up)
{  if (req)
   {  req= 0;
      return_char(myaddress);
   }
   else if (std)
   {  S1ADDR= myaddress= (UC)Number;
      T1B8= 1;		// set 9 bit transmit
      put_char1(myaddress); // send address back for control
   }
}
// *****************************************************************

void load1g(void)	// HG  -receive new phase
{  if (std)
   {  sys1buf= 1;		// system G, channel 1
      poff1buf= 0;		// line7 off
      lastphase1= Number;	// save new phase
      phases1();
   }
}
// *****************************************************************

void load2g(void)	// JG  -receive new phase
{  if (std)
   {  sys2buf= 1;		// system G, channel 2
      poff2buf= 0;		// line7 off
      lastphase2= Number;	// save new phase
      phases2();
   }
}
// *****************************************************************

void load1g7(void)	// HK  -receive new phase
{  if (std)
   {  sys1buf= 1;		// system G with line 7, channel 1
      poff1buf= 1;		// line7 on
      lastphase1= Number;	// save new phase
      phases1();
   }
}
// *****************************************************************

void load2g7(void)	// JK  -receive new phase
{  if (std)
   {  sys2buf= 1;		// system G, channel 2
      poff2buf= 1;		// line7 on
      lastphase2= Number;	// save new phase
      phases2();
   }
}
// *****************************************************************

void load1m(void)	// HM  -receive new phase
{  if (std)
   {  sys1buf= 0;		// system M, channel 1
      poff1buf= 0;		// pedestal on
      lastphase1= Number;	// save new phase
      phases1();
   }
}
// *****************************************************************

void load2m(void)	// JM  -receive new phase
{  if (std)
   {  sys2buf= 0;		// system M, channel 2
      poff2buf= 0;		// pedestal on
      lastphase2= Number;	// save new phase
      phases2();
   }
}
// *****************************************************************

void load1ms(void)	// HL  -receive new phase
{  if (std)
   {  sys1buf= 0;		// system M, channel 1
      poff1buf= 1;		// pedestal off
      lastphase1= Number;	// save new phase
      phases1();
   }
}
// *****************************************************************

void rdphase1(void)	// HB	-return phase
{  if (req)
   {  req= 0;
      return_long(lastphase1);
   }
}
// *****************************************************************

void rdphase2(void)	// JB	-return phase
{  if (req)
   {  req= 0;
      return_long(lastphase2);
   }
}
// *****************************************************************

void load2ms(void)	// JL  -receive new phase
{  if (std)
   {  sys2buf= 0;		// system M, channel 2
      poff2buf= 1;		// pedestal off
      lastphase2= Number;	// save new phase
      phases2();
   }
}
// *****************************************************************

void rdsyst1(void)		// HQ  -return system
{  if (req)
   {  req= 0;
      return_char((UC)(!sys1buf) * 2 + (UC)poff1buf);
   }
}
// *****************************************************************

void rdsyst2(void)		// JQ  -return system
{  if (req)
   {  req= 0;
      return_char((UC)(!sys2buf) * 2 + (UC)poff2buf);
   }
}
// *****************************************************************

void global1(void)		// HO  -set global offset phase
{  if (req)
   {  req= 0;
      if (sys1buf)  return_long(offsetg1);
      else  return_long(offsetm1);
   }
   else if (std)
   {  if (sys1buf)  offsetg1= tmodg - Number;
      else offsetm1= tmodm - Number;
      Number= lastphase1;		// use latest H-phase
      phases1();
   }
}
// *****************************************************************

void global2(void)		// JO  -set global offset phase
{  if (req)
   {  req= 0;
      if (sys2buf)  return_long(offsetg2);
      else  return_long(offsetm2);
   }
   else if (std)
   {  if (sys2buf)  offsetg2= tmodg - Number;
      else offsetm2= tmodm - Number;
      Number= lastphase2;		// use latest H-phase
      phases2();
   }
}
// *****************************************************************

void scload1(void)	// HH  -set sch-phase, channel 1
{  UI x;
   if (sys1buf)  x= schcalg1; else x= schcalm1;	// select correction
   if (req)
   {  req= 0;
      return_int((sch1buf-x) % 2048);
   }
   else if (std)
   {  // while (loaded1)  ;
      sch1buf= ((UI)Number + x) % 2048;		// adjust value
      loaded1= 1;		// signal new value
   }
}
// *****************************************************************

void scload2(void)	// JH  -set sch-phase, channel 2
{  UI x;
   if (sys2buf)  x= schcalg2; else x= schcalm2;
   if (req)
   {  req= 0;
      return_int((sch2buf-x) % 2048);
   }
   else if (std)
   {  // while (loaded2)  ;
      sch2buf= ((UI)Number + x) % 2048;
      loaded2= 1;		// signal new value
   }
}
// *****************************************************************
near UL tmp;

UL Rd_iic_long(void)
{  UL x, y;
   UC i;
   x= 0; tmp= 0;		// read H-phase
   for (i=0;i<4;i++)
   {  y= (UL)Rd_iic(ram_addr);
      x= tmp<<8;
      tmp= x + y;
   }
   return(tmp);
}
// *****************************************************************

UI Rd_iic_int(void)
{  UI y;
   y= Rd_iic(ram_addr);		// read SCH-phase
   y= 256 * y + Rd_iic(ram_addr);
   return(y);
}
// *****************************************************************

void trimphag1(void)		// HX  -store/read H- & SCH-phases
{  if (req)
   {  Wr_iic(ram_addr, trimphag1_addr);	// set RAM address
      return_long_int(Rd_iic_long(), Rd_iic_int());
   }
   else if (save)
   {  save= 0;
//      hcalg1= (ts-offsetg1) % 1105920000;
      hcalg1= (tmodg-lastphase1+hcalg1) % 1105920000;
      schcalg1= sch1buf; // % 2048;
      store_phases(ram_addr, trimphag1_addr, hcalg1, schcalg1);
   }
}
// *****************************************************************

void trimphag2(void)		// JX  -store/read H- & SCH-phases
{  if (req)
   {  Wr_iic(ram_addr, trimphag2_addr);	// set RAM address
      return_long_int(Rd_iic_long(), Rd_iic_int());
   }
   else if (save)
   {  save= 0;
//      hcalg2= (ts-offsetg2) % 1105920000;
      hcalg2= (tmodg-lastphase2+hcalg2) % 1105920000;
      schcalg2= sch2buf; // % 2048;
      store_phases(ram_addr, trimphag2_addr, hcalg2, schcalg2);
   }
}
// *****************************************************************

void trimpham1(void)		// HY  -store/read H- & SCH-phases
{  if (req)
   {  req= 0;
      Wr_iic(ram_addr, trimpham1_addr);	// set RAM address
      return_long_int(Rd_iic_long(), Rd_iic_int());
   }
   else if (save)
   {  save= 0;
//      hcalm1= (ts-offsetm1) % 461260800;
      hcalm1= (tmodm-lastphase1+hcalm1) % 461260800;
      schcalm1= sch1buf; // % 2048;
      store_phases(ram_addr, trimpham1_addr, hcalm1, schcalm1);
   }
}
// *****************************************************************

void trimpham2(void)		// JY  -store/read H- & SCH-phases
{  if (req)
   {  Wr_iic(ram_addr, trimpham2_addr);	// set RAM address
      return_long_int(Rd_iic_long(), Rd_iic_int());
   }
   else if (save)
   {  save= 0;
//      hcalm2= (ts-offsetm2) % 461260800;
      hcalm2= (tmodm-lastphase2+hcalm2) % 461260800;
      schcalm2= sch2buf; // % 2048;
      store_phases(ram_addr, trimpham2_addr, hcalm2, schcalm2);
   }
}
// *****************************************************************

void daclev1(void)		// HD  -set/store/read DAC level
{  if (req)
   {  req= 0;
      Wr_iic(ram_addr, daclev1_addr);	// set RAM address
      return_char(Rd_iic(ram_addr));	// read value
   }
   else if (save)
   {  save= 0;
      store_byte(ram_addr, daclev1_addr, dalev1);	// store value
   }
   else if (std)
   {  dalev1= (UC)Number;
      Wr_iic2(dac1_addr, 0, (UC)Number);	// set value
   }
}
// *****************************************************************

void dacoffset1(void)		// HC  -set/store/read DAC offset
{  if (req)
   {  req= 0;
      Wr_iic(ram_addr, dacofs1_addr);	// set RAM address
      return_char(Rd_iic(ram_addr));	// read value
   }
   else if (save)
   {  save= 0;
      store_byte(ram_addr, dacofs1_addr, daoff1);	// store value
   }
   else if (std)
   {  daoff1= (UC)Number;
      Wr_iic2(dac1_addr, 1, (UC)Number);	// set value
   }
}
// *****************************************************************

void daclev2(void)		// JD  -set/store/read DAC level
{  if (req)
   {  req= 0;
      Wr_iic(ram_addr, daclev2_addr);	// set RAM address
      return_char(Rd_iic(ram_addr));	// read value
   }
   else if (save)
   {  save= 0;
      store_byte(ram_addr, daclev2_addr, dalev2);	// store value
   }
   else if (std)
   {  dalev2= (UC)Number;
      Wr_iic2(dac2_addr, 0, (UC)Number);	// set value
   }
}
// *****************************************************************

void dacoffset2(void)		// JC  -set/store/read DAC offset
{  if (req)
   {  req= 0;
      Wr_iic(ram_addr, dacofs2_addr);	// set RAM address
      return_char(Rd_iic(ram_addr));	// read value
   }
   else if (save)
   {  save= 0;
      store_byte(ram_addr, dacofs2_addr, daoff2);	// store value
   }
   else if (std)
   {  daoff2= (UC)Number;
      Wr_iic2(dac2_addr, 1, (UC)Number);	// set value
   }
}
// *****************************************************************

UC instring[11];	// receive string
UC instr_index;		// index into receive string (instring)
// *****************************************************************

void wrtext(void)	// HW  -write user text
{  if (std)
   store_string(ram_addr, text_addr + 16 * (UC)Number, instring);
}
// *****************************************************************

void rdtext(void)	// HR  -read user text
{  UC i, s[10];

   if (req)
   {  req= 0;
      Wr_iic(ram_addr, text_addr + 16 * (UC)Number);	// set RAM address
      for (i=0;i<10;i++)  s[i]= Rd_iic(ram_addr);	// read string
      return_string(s);
   }
}
// *****************************************************************

void verreport(void)
{  if (req)                     // HI?
   {  req= 0;
      return_char(version);
   }
}
// *****************************************************************
void opstart(void)
{
   hfase1= hcaldefg;
   hfase2= hcaldefg;
   lastphase1= -hcaldefg;
   lastphase2= -hcaldefg;
   loaded1= loaded2= 0;		// initiate fase-transfer
   sf1buf= sf2buf= 0;
   subfa1= subfa2= 0;
   syst1= syst2= 1;		// default system= G
   sys1buf= sys2buf= 1;
   poff1= poff2= 0;		// set-up= off
   offsetg1= offsetg2= 0;	// global offset 0
   offsetm1= offsetm2= 0;

   // GET TRIM-POT VALUES
   Wr_iic(ram_addr, daclev1_addr);		// set RAM address
   Wr_iic2(dac1_addr, 0, Rd_iic(ram_addr));	// transfer value
   Wr_iic2(dac1_addr, 1, Rd_iic(ram_addr));	// there is auto-increment
   Wr_iic2(dac2_addr, 0, Rd_iic(ram_addr));	// transfer value
   Wr_iic2(dac2_addr, 1, Rd_iic(ram_addr));

   // GET H-PHASE AND SCH-PHASE CORRECTION VALUES
   hcalg1= Rd_iic_long();			// read h-phase
   schcalg1= Rd_iic_int();			// read sch-phase
   hcalg2= Rd_iic_long();
   schcalg2= Rd_iic_int();
   hcalm1= Rd_iic_long();
   schcalm1= Rd_iic_int();
   hcalm2= Rd_iic_long();
   schcalm2= Rd_iic_int();

   sch1buf= schcalg1;
   sch2buf= schcalg2;

   bulowport1= bulowport2= 0;	// init ports
   buhiport1= buhiport2= 0;
   hfasefport1= hfasefport2= 0;
   hfasecport1= hfasecport2= 0;
   typeport1= typeport2= 0;
   errstatus= 0;		// no errors
}
// *****************************************************************

void reset1(void)	// HZ  -Reset channel 1 to default
{  // download default trim
   store_byte(ram_addr, daclev1_addr, daclevcal);	// store def. value
   Wr_iic2(dac1_addr, 0, daclevcal);	// set value
   store_byte(ram_addr, dacofs1_addr, dacofscal);	// store def. value
   Wr_iic2(dac1_addr, 1, dacofscal);	// set value
//   if (syst1)
      store_phases(ram_addr, trimphag1_addr, hcaldefg, schcaldefg);
//   else
      store_phases(ram_addr, trimpham1_addr, hcaldefm, schcaldefm);

   dalev1= daclevcal;
   daoff1= dacofscal;
   hcalg1= hcaldefg;
   hcalm1= hcaldefm;
   schcalg1= schcaldefg;
   schcalm1= schcaldefm;
//   opstart();
}
// *****************************************************************

void reset2(void)	// JZ  -Reset channel 2 to default
{  // download default trim
   store_byte(ram_addr, daclev2_addr, daclevcal);	// store def. value
   Wr_iic2(dac2_addr, 0, daclevcal);	// set value
   store_byte(ram_addr, dacofs2_addr, dacofscal);	// store def. value
   Wr_iic2(dac2_addr, 1, dacofscal);	// set value
//   if (syst2)
      store_phases(ram_addr, trimphag2_addr, hcaldefg, schcaldefg);
//   else
      store_phases(ram_addr, trimpham2_addr, hcaldefm, schcaldefm);

   dalev2= daclevcal;
   daoff2= dacofscal;
   hcalg2= hcaldefg;
   hcalm2= hcaldefm;
   schcalg2= schcaldefg;
   schcalm2= schcaldefm;
//   opstart();
}
// *****************************************************************

void readstat(void)	// HS  -Read status
{  if (req)
   {  req= 0;
      return_char(errstatus);      
   }
}
// *****************************************************************

code const struct kommando kommandotabel[] = {

'H','A',setadr,
'H','G',load1g,
'J','G',load2g,
'H','K',load1g7,
'J','K',load2g7,
'H','M',load1m,
'J','M',load2m,
'H','L',load1ms,
'J','L',load2ms,
'H','Q',rdsyst1,
'J','Q',rdsyst2,
'H','B',rdphase1,
'J','B',rdphase2,
'H','O',global1,
'J','O',global2,
'H','H',scload1,
'J','H',scload2,
'H','S',readstat,
'H','X',trimphag1,
'H','Y',trimpham1,
'J','X',trimphag2,
'J','Y',trimpham2,
'H','D',daclev1,
'J','D',daclev2,
'H','C',dacoffset1,
'J','C',dacoffset2,
'H','Z',reset1,
'J','Z',reset2,
'H','W',wrtext,
'H','R',rdtext,
'H','I',verreport,
'S','W',dummy
};
// *****************************************************************
// Command Machine
// *****************************************************************

void CommandExecute(char A,char B) {
UC x;
for (x = 0; x < (sizeof(kommandotabel)/sizeof(struct kommando)); x++)
   if (A==kommandotabel[x].Aa)
      if (B==kommandotabel[x].Bb)
      {
	 header1 = A;
	 kommandotabel[x].funktion();
	 break;
      }
}
// *****************************************************************

void CharHandler(UC c)
{  static UC MesState;
   static char First,Second;
   switch ( MesState)
   {  case 0: 	if(isalpha(c))
  	      	{  First = toupper(c); MesState = 1;
	      	}  break;
      case 1: 	if(isalpha(c))
    	      	{  Second =toupper(c); MesState = 2; Number = 0;
	      	}
		else MesState= 0;
	      	break;
      case 2:	if(isdigit(c))  Number = (c-'0') + 10 * Number;
		else if (c==',')  MesState= 3;
		else if (c==34)
		{  MesState= 4;		// was quote, start string
		   instr_index= 0;
		}
		else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '$')
		{  MesState = 5;
		   std= 0; quit= 0; save= 0; req= 0;
	           if (c == '?') req = 1;
	           else if (c == '!') quit = 1;
	           else if (c == '$') save = 1;
		   else  std= 1;
	           // CommandExecute(First,Second);
		}
		else MesState= 0;
		break;
      case 3:	if(isdigit(c))  Number1 = 10 * Number1 + (c-'0');
		else if (c==34)
		{  MesState= 4;		// was quote, start string
		   instr_index= 0;
		}
		else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '$')
		{  MesState = 5;
		   std= 0; quit= 0; save= 0; req= 0;
		   if (c == '?') req = 1;
		   else if (c == '!') quit = 1;
		   else if (c == '$') save = 1;
		   else  std= 1;
		   // CommandExecute(First,Second);
		}
		else MesState= 0;
		break;
      case 4:	if (c==34)  instring[instr_index]= 0;  // end the string
		else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '$')
		{  MesState = 5;
		   std= 0; quit= 0; save= 0; req= 0;
		   if (c == '?') req = 1;
		   else if (c == '!') quit = 1;
		   else if (c == '$') save = 1;
		   else  std= 1;
		   // CommandExecute(First,Second);
		}
		else  instring[instr_index++]= c; // save the char in string
		break;
      case 5:	MesState = 0;
		if (c==0x80)  CommandExecute(First,Second);
		break;
      default:	MesState = 0; break;
   }
}
// *****************************************************************

void kommun(void)
{  UC gc;
   while ((gc=get_char1()) != 0)	/* handle characters from buffer */
		  CharHandler(gc);
}
// *****************************************************************
// *****************************************************************

void main(void)
{
// System set-up
   BCR= 0x02;		// set 20 bit BUS, 8 bit DATA
   syssetup();

 // set-up counters and interrupts
   IEL= 0;		// reset timers
   IT0= 1;		//INTR0 EDGE TRIG
   IT1= 1;		//INTR1 EDGE TRIG
   EX0= 1;		//ENABLE INTR FROM EXT0
   EX1= 1;		//ENABLE INTR FROM EXT1
 /*
   TMOD= 0x21;	// T1 =8 bit reload, T0 =16 bit (not used)
   ET1= 0;	// intrpt from timer1 disabled
   ET0= 0;
   RTL1= -48;	// reload value for Timer 1, sets Baud rate to 9600
 */
 // alternativ til timer 1: timer2 til V24 (max-rate)
   T2MOD= 0x31;	// use T2 for V24_1, count up
   T2CAPH= -1;	// set reload values for max Baud
   T2CAPL= -6;
   TH2= -1;	// set for max. Baud
   TL2= -6;
   TR2= 1;	// start V24-timer
   SWE |= 1;	// enable SW-interrupt 1 for moving command to cyclic buffer

   S1CON= 0xf0;	// 0xf2,Transmit mode 3, 9 bit UART, Address mode, Rec.enable


/* DEFINITION OF INTERRUPTS (see manual pg.82)
  Interrupt:	Level:	Reg-bank:
EX0 (line1)	15	2	banked
EX1 (line2)	15	2	banked
serial1		14	1	banked
ser.1-end	 1	0	not banked
// SOFTWARE-INTERRUPTS:
none
*/
ROM_VECTOR(IV_EX0, ext0_int, IV_SYSTEM+IV_PRI15+IV_BANK2)
ROM_VECTOR(IV_EX1, ext1_int, IV_SYSTEM+IV_PRI15+IV_BANK2)
ROM_VECTOR(IV_RI1, serial1_int, IV_SYSTEM+IV_PRI14+IV_BANK1)
ROM_VECTOR(IV_SWI1, serial_end, IV_SYSTEM+IV_PRI01+IV_BANK0)

   IPA0= 0x0f;	// TIM0/EX0 prior.
   IPA1= 0x0f;	// TIM1/EX1 prior.
   IPA5= 0x0e;	// serial 1 TX/Rx prior.


   S1ADDR= myaddress= defaddr;	// default receive address
   S1ADEN= 0xFF;  // mask for receive address (zeroes makes don't care)

   serial1_init();
//   EA= 1;			// enable all interrupts

   // SET DEFAULT VALUES
   Rd_iic(ram_addr);		// make 2 dummy reads to clear IIC-bus
   Rd_iic(ram_addr);

   opstart();

   EA= 1;			// enable all interrupts


// *****************************************************************

// main interrupt waiting loop

   while(1)
   {
      kommun();
   }
}
