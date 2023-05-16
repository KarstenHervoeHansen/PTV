/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: BBU.C
* Version	: 0.0	961205
* Purpose	: Main module
* Org.date	: 960515
* Author	: PRC
********************************************************************
HISTORY:
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
#include "vars.c"

//extern void syssetup(void);

NUC myaddress;
near char header1;
NUC cc;
NUL Number;		// Number received on v24-bus
NUI Number1;		// second number received on v24-bus
bit req,quit,save;	// flags for requests from frontplate

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
{ 


   hfasefport1= ~((UC)(hfase1%256));

  // hfasecport1= (UC)(hfase1/256);
 #asm
   mov.w r1,#_hfasecport1
   mov.b[r1], _hfase1+1
#endasm
  
   bulowport1= (UC)(subfa1 & 0x00ff);
   buhiport1= (UC)((subfa1 & 0xff00)>>8);

   P3_0= 1;	// FOR TEST
}
// *****************************************************************
// *****************************************************************

void main(void)
{
// System set-up
  // BCR= 0x02;		// set 20 bit BUS, 8 bit DATA
  // syssetup();

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
//ROM_VECTOR(IV_EX1, ext1_int, IV_SYSTEM+IV_PRI15+IV_BANK2)
//ROM_VECTOR(IV_RI1, serial1_int, IV_SYSTEM+IV_PRI14+IV_BANK1)
//ROM_VECTOR(IV_SWI1, serial_end, IV_SYSTEM+IV_PRI01+IV_BANK0)

   IPA0= 0x0f;	// TIM0/EX0 prior.
   IPA1= 0x0f;	// TIM1/EX1 prior.
   IPA5= 0x0e;	// serial 1 TX/Rx prior.


   S1ADDR= myaddress= defaddr;	// default receive address
   S1ADEN= 0xFF;  // mask for receive address (zeroes makes don't care)

   EA= 1;			// enable all interrupts

   // SET DEFAULT VALUES


// *****************************************************************

// main interrupt waiting loop

   while(1)
   { ;
   }
}
