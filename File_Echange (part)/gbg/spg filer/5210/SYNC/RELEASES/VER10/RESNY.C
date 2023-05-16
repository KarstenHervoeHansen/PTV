#include <xa.h>	
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
// #include <string.h>
#include <intrpt.h>
#include "serial1.h"
#include "equ.c"
#include "vars.c"
/*
;********************************************************
; filename	: res.c					*
;********************************************************
// HISTORY
960731	doline7 re-activated.

*/

#define UC unsigned char
#define UI unsigned int

extern void Serial1_init();
extern void syssetup();
extern void lock(void);
extern void kommun(void);
extern void sch(void);
extern void line7(void);
extern void mode(void);
extern char get_char1(void);

near char myaddress;
near char header1;
near UC cc;
near unsigned long Number;
bit req,quit,save;	// flags for requests from frontplate

/* RECEIVE/SEND SYNTAX:					*/
/* Serial:	PutCmd(A,B)				*/
/*		PutData(A,B,data);			*/
/*		PutData16(A,B,data);			*/
/*		PutStr("string");			*/

// Definition af interruptfunktion (manual si.82)
ROM_VECTOR(IV_RI1, Serial1_int, IV_SYSTEM+IV_PRI14+IV_BANK2)
ROM_VECTOR(IV_SWI1, serial_end, IV_SYSTEM+IV_PRI01+IV_BANK2)
//ROM_VECTOR(IV_RI1, Serial1_int, IV_PSW)
ROM_VECTOR(IV_EX0, v_int, IV_SYSTEM+IV_PRI15+IV_BANK1)
ROM_VECTOR(IV_T0, vtimer_int, IV_SYSTEM+IV_PRI15+IV_BANK1)
ROM_VECTOR(IV_EX1, line_int, IV_SYSTEM+IV_PRI15+IV_BANK1)

// SOFTWARE-INTERRUPTS:
// 1: serial_end  2: line7  3: sch  4: lock
ROM_VECTOR(IV_SWI2, line7, IV_SYSTEM+IV_PRI02+IV_BANK1)
ROM_VECTOR(IV_SWI3, sch, IV_SYSTEM+IV_PRI03+IV_BANK1)
ROM_VECTOR(IV_SWI4, lock, IV_SYSTEM+IV_PRI04+IV_BANK1)
// ********************************************************************
void NotFound(void) {
}
// ********************************************************************

near struct kommando {
	 char Aa;
	 char Bb;
	 void (*funktion)(void);
};
// ********************************************************************
// ********************************************************************
void main(void)
{
//  UC * dpoint;

// System set-up
  BCR= 0x02;	// set 20 bit BUS, 8 bit DATA
  syssetup();

// set-up counters and interrupts
   IEL= 0;	// reset timers
   IT0= 1;		//INTR0 EDGE TRIG
   IT1= 1;		//INTR1 EDGE TRIG
   ET0= 1; 	// enable V-time-out
   TR0= 0;	// don't start Timer0
   EX0= 1;		//ENABLE INTR FROM EXT0
   EX1= 1;		//ENABLE INTR FROM EXT1

   IPA0= 0xff;		// 0x76 Timer0/EX0 prior.=15
   IPA1= 0x0f;		// 0x05 Timer1/EX1 prior.=15


   TMOD= 0x21;	//T1 =8 bit reload, T0 =16 bit (not used)

/*
   ET1= 0;	// intrpt from timer1 disabled
   RTL1= -48;	// reload value for Timer 1, sets Baud rate to 9600
*/
 // alternativ til timer 1: timer2 til V24 (max-rate)
   T2MOD= 0x31;	// use T2 for V24_1, count up
   T2CAPH= -1;	// set reload values for max Baud
   T2CAPL= -6;
   TH2= -1;	// set for max. Baud
   TL2= -6;
   TR2= 1;	// start V24-timer

   S1CON= 0xf0;	// Transmit mode 3,  9 bit UART,  Address mode, Rec.enable

   IPA5= 0x0e;	// 0x07 serial 1 prior.=14
   IPA4= 0;	// serial 0 Tx/Rx prior.

   ERI1= 1;	// enable ser.1 receive
   // ETI1= 1;	// enable ser.1

   S1ADDR= myaddress= 0x55;	// receive address
   S1ADEN= 0xff;	// mask for receive address
   // receive address = 11000000 AND 11111101 = 110000X0

   SWE= 0x0e;	// enable SW-interrupts for  lock, sch, line7
   SWR= 0;	// no intrpts now
   Serial1_init();

// preset some internals

   g_sel= 1;		// set default
   genmode= 8;

   if (g_sel)
   {  tablepyt= pytg;
      fasekor= fasekorg;
      fasepos= faseposg;
      btel= bmax_g;
      ftel= fmax_g;
   }
   else
   {  tablepyt= pytm;
      fasekor= fasekorm;
      fasepos= faseposm;
      btel= bmax_m;
      ftel= fmax_m;
   }

   tableam= divi;
   tableph= fase;
   errpre= 3;
   alian= 1;
   bulocken= 1;
   atel= amax;
   btel= bmax_g;
   ftel= fmax_g;
   phgod= errpre;
   sphgod= errpre;
   genlocken= 1;

   genphase_g= 0x708a;	//646f;
   hphzero_g= 0x0002fec2;	//0x0002fc80;
   genphase_m= 0x708a;
   hphzero_m= 0x0002fec2;	// skal rettes!

// preset some externals

// start timers and enable interrupts
   EA= 1;			//enable all interrupts
//  TR1= 1;	// timer1

   verr= verrpre;		// count good field
   htel= -vlenmin_g;		// (-623/523) load TEST-TIMER
   cntenab= 1;		//replaces TR0 for line count enable
   doflags= 0;
   outs= 0;

// interrupt waiting loop
// WAIT:
   while(1)
   { // if (dolock)  lock();
      if (docomm)  { docomm= 0; kommun(); }
     /* if (dosch)  sch();
      if (doline7)
      {  line7();
	 mode();
      } */
   }
}

// ********************************************************************

void systatus(void)
{ SndChar(master_addr,'G','S',outs);
}
// ********************************************************************
code UC modetab[]=
{ 0x21,0x0f,0x0d,0x05,0x05,0x04,0x11,0x01,0x41,0x10,0x40,0x00
};

void genlockmode(void)
{  genmode= Number;
   modebits= modetab[genmode];
   if (g_sel)
   {  tablepyt= pytg;
      fasekor= fasekorg;
      fasepos= faseposg;
      btel= bmax_g;
      ftel= fmax_g;
   }
   else
   {  tablepyt= pytm;
      fasekor= fasekorm;
      fasepos= faseposm;
      btel= bmax_g;
      ftel= fmax_g;
   }
}
// ********************************************************************

void autohph(void)
{  if (g_sel)
   {  hphzero_g= hinput-genphase_g;
      if (((hphzero_g & 0xff0000) >> 8 != (hcal1_g & 0xff00)) ||
          ((hphzero_g & 0xf800) >> 8 != (hcal1_g & 0xff)))
      hphzero_g= ((UL)hcal2_g) << 8;
   }
   else
   {  hphzero_m= hinput-genphase_m;
      if (((hphzero_m & 0xff0000) >> 8 != (hcal1_m & 0xff00)) ||
          ((hphzero_m & 0xf800) >> 8 != (hcal1_m & 0xff)))
      hphzero_m= ((UL)hcal2_m) << 8;
   }
}
// ********************************************************************

void hphsetg(void)
{  hphzero_g= Number;
}
// ********************************************************************

void hphsetm(void)
{  hphzero_m= Number;
}
// ********************************************************************

void hphauto(void)
{  if (g_sel)  hphzero_g= hinput - genphase_g;
   else  hphzero_m= hinput - genphase_m;
}
// ********************************************************************

void hphupg(void)
{  hphzero_g= hphzero_g + 390L;
}
// ********************************************************************

void hphupm(void)
{  hphzero_m= hphzero_m + 390L;
}
// ********************************************************************

void gensetg(void)
{  genphase_g= Number;
}
// ********************************************************************

void gensetm(void)
{  genphase_m= Number;
}
// ********************************************************************

void gensetd6(void)
{  genphase_d6= Number;
}
// ********************************************************************

void gensetd5(void)
{  genphase_d5= Number;
}
// ********************************************************************
// TABLE FOR ARRIVING COMMANDOS	FROM FRONTPLATE
// ********************************************************************
void dummy(void)
{ ;
}

code const struct kommando kommandotabel[] = {
'G','S',systatus,
'G','M',genlockmode,
'G','I',hphupg,
'G','J',hphupm,
'G','Z',hphsetg,
'G','Y',hphsetm,
'G','A',hphauto,
'G','F',gensetg,
'G','G',gensetm,
'G','D',gensetd6,
'G','E',gensetd5,
'S','W',dummy,
'D','U',dummy
};
// ********************************************************************
// Command Machine
// ********************************************************************
CommandExecute(char A,char B) {
UC x;
for (x = 0; x < (sizeof(kommandotabel)/sizeof(struct kommando)); x++)
   if (A==kommandotabel[x].Aa)
      if (B==kommandotabel[x].Bb)
      {  header1 = A;
	 kommandotabel[x].funktion();
	 break;
      }
}
// ***************************************************************
CharHandler(char c)
{  static UC MesState;
   static char First,Second;
   switch ( MesState)
   {  case 0: if(isalpha(c) || (c=='?'))
  	      {  First = toupper(c); MesState = 1;
              }  break;
      case 1: if(isalpha(c) || (c=='?'))
    	      {  Second =toupper(c); MesState = 2; Number = 0;
              }  break;
      case 2: if(isdigit(c))  Number = 10 * Number + (c-'0');
	      if (c== 10 || c== '?' || c== '!' || c== '$')
	      {  MesState = 0;
	         if (c == '?') req = 1; else req = 0;
	         if (c == '!') quit = 1; else quit = 0;
	         if (c == '$') save = 1; else save = 0;
	         CommandExecute(First,Second);
	      }  break;
      default:   MesState = 0; break;
   }
}

// ***************************************************************
void kommun(void)
{  while ((cc=get_char1()) != 0)  // handle characters from buffer
      CharHandler(cc);

}
// ***************************************************************

