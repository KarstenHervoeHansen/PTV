
/*
********************************************************************
* Project	: 
* Filename	: 
* Version	: 
* Purpose	: Main and start-up module
* Org.date	: 
* Author	: PRC
********************************************************************
HISTORY:

960214 Created from mas.c in PM5655 to be used in PT5210
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <xa.h>
#include <intrpt.h>
#include "vars.h"
#include "serial0.h"
#include "start.h"

extern void syssetup();

near UC syncstatus;

// HARDWARE DEFINES
sbit P3_4 @ 0x39c;


// PORT CONTROL:
// RAM has address-space 0-0x1fff
volatile UC  ind1 @ 0x2001;	// input port 1
volatile UC  ind2 @ 0x4001;	// input port 2
volatile UC  ud1 @ 0x2001;	// output port 1
volatile UC  ud2 @ 0x4001;	// output port 2
volatile UC  ud3 @ 0x6001;	// output port 3
volatile UC  ud4 @ 0x8001;	// output port 4
volatile UC  ud5 @ 0xa001;	// output port 5
code far UC  pgadata @ 0x20000;	// PGA-PROM
near UC ud1buf;			// buffer for output port 1
near UC ud4buf;			// buffer for output port 4
near UC ud5buf;			// buffer for output port 5
// CONTROL OF OPTIONAL UNITS:
// Resets:
#define    MRES1_on	{ ud4buf &= ~0x20; ud4= ud4buf;}
#define    MRES1_off	{ ud4buf |= 0x20; ud4= ud4buf;}
#define    MRES2_on	{ ud4buf &= ~0x40; ud4= ud4buf;}
#define    MRES2_off	{ ud4buf |= 0x40; ud4= ud4buf;}
#define    MRES3_on	{ ud4buf &= ~0x80; ud4= ud4buf;}
#define    MRES3_off	{ ud4buf |= 0x80; ud4= ud4buf;}


near UC myaddress;	// V24 address for master
near UL Number, Number1;	// V24 number transfer data
near char header1;	// store for 1'st letter in V24 cmd.

#define v24ok 0

// WATCHDOG
#define feed_dog	{ ud4buf ^= 16; ud4= ud4buf;}

// LEVEL DETECTOR
near lev1det;
near lev2det;

// FOR TEST
near long testbbu= 0;
near UC speed;


// SYSTEM VARIABLES (some are old from PM5655)
near UI teller;		// time-out counter
near UC c;		// common globals
near UC slicer;	// time window counter
near UI options;	//
near UC reread;	// counter for re-read of sync gen. (time-out)
near UI fejl;
bit Timer_flag;		// control bit for 21 ms timing
bit genbusy, bufbusy;	// generator not ready to receive, bus busy
bit req_ready;		// ready to answer requests
bit Acknowledge;	// Acknowledge after IIC communication
near char cc;
bit req,quit,save;	// flags for requests from frontplate

// ******************************************************************

void waitms(UI time)	// longer wait routine
{  UI n;
   while (time-- > 0) 	// wait 1 ms x time
   { n= 750;
     while (n>0)  n--;
   }
}
// ******************************************************************

void waitus(UI time)	// short wait routine
{
   while (time-- > 0) ;	// wait 1.1 us x time
}
// ******************************************************************
extern void put_char0(char n);

void backchar(UC n)
{  UC i;
   put_char0 (' ');
   for (i=0;i<n;i++)  put_char0(AnswerBuf[i]);
   put_char0 (13);
}
// ******************************************************************

char instring[11];	// receive string
char *instrptr;
UC instr_index;		// index into receive string (instring)
// *****************************************************************

void NotFound(void)		// old eksample-routine
{ ;
}
// ******************************************************************

struct kommando {
	 char Aa;
	 char Bb;
	 void (*funktion)(void);
};

// ******************************************************************
// TABLE FOR ARRIVING COMMANDOS	FROM RS232 (FOR TEST)
// ******************************************************************

code const struct kommando kommandotabel[] = {

'x','x',NotFound
};
// ******************************************************************
// Command Machine
// ******************************************************************

CommandExecute(char A,char B) {
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
// ******************************************************************
// NB! '$' ER ERSTATTET AF '%' I DENNE TEST-VERSION!!!!!


void CharHandler(char c)
{  static UC MesState;
   static char First,Second;
   switch ( MesState)
   {  case 0: if(isalpha(c) || (c=='?'))
  	      {  First = toupper(c); MesState = 1;} break;
      case 1: if(isalpha(c) || (c=='?'))
    	      {  Second =toupper(c); MesState = 2; Number = 0;} break;
      case 2: if(isdigit(c))  Number = 10 * Number + (c-'0');
	      if (c==',')  MesState= 3;
	      else if (c==34)
	      {  MesState= 4;	// quote
		 instr_index= 0;
	      }
	      else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '%')
	      {  MesState = 0;
	         if (c == '?') req = 1; else req = 0;
	         if (c == '!') quit = 1; else quit = 0;
	         if (c == '%') save = 1; else save = 0;
	         CommandExecute(First,Second);
	      }  break;
      case 3: if(isdigit(c))  Number1 = 10 * Number1 + (c-'0');
	      else if (c==34)
	      {  MesState= 4;	// quote
		 instr_index= 0;
	      }
	      else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '%')
	      {  MesState = 0;
	         if (c == '?') req = 1; else req = 0;
	         if (c == '!') quit = 1; else quit = 0;
	         if (c == '%') save = 1; else save = 0;
	         CommandExecute(First,Second);
	      }  break;
      case 4: if (c==34)  instring[instr_index]= 0;  // end the string
	      else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '%')
	      {  MesState = 0;
	         if (c == '?') req = 1; else req = 0;
	         if (c == '!') quit = 1; else quit = 0;
	         if (c == '%') save = 1; else save = 0;
	         CommandExecute(First,Second);
	      }
	      else  instring[instr_index++]= c; 
	      break;
      default: MesState = 0; break;
   }
}
// ******************************************************************

/* Interrupts:	T0 prior:     9  bank 1
		Ser0_rec     15   -   0
		Ser1_rec  -  15   -   0
		Ser0_trm  -   ?   -   0
		Ser1_trm  -   ?   -   0
*/
ROM_VECTOR(IV_T0, Timer_int, IV_SYSTEM+IV_PRI09+IV_BANK1)
ROM_VECTOR(IV_RI0, Serial0_int, IV_PSW)
ROM_VECTOR(IV_RI1, Serial1_int, IV_PSW)


#define xtal 29491200
// ******************************************************************

interrupt void Timer_int(void)
{  static near UC ticker;
   TH0 = -xtal/256/4/3/50;  // Load timer to 50 Hz (20ms)
   if (++ticker>2)
   {  ticker= 0;
      Timer_flag= 1;
   }
}
// ******************************************************************

void checkopt(void)	// Find options
{

}
// ******************************************************************

void opstart(void)	// Initialize units
{

}
// ******************************************************************
// ******************************************************************

void main(void)
{ UC i;
  UC testtel;
   // TEST


// System set-up
   BCR= 0x02;	// set 20 bit BUS, 8 bit DATA
   syssetup();

// Serial set-up
   S0CON= 0x50;	// (D2) Transmit mode 1(3), 8(9) bit UART, Receive enable
   S1CON= 0xF2;	// Transmit mode 3,  9 bit UART,  Address mode, Rec.enable
   // PCON= 0;	// => SMOD = 0, not double Baud Rate
   //  TI0= 1;	// set transmitter ready
   S1ADDR= master_addr;	// receive address
   S1ADEN= 0xCC;	// mask for receive address
   // receive address = 00010000 AND 11001100 = 00XX00XX (00 to 33)

// Timer set-up
   TMOD= 0x21;	// timer 1: 8 bit autoload; timer 0: 16 bit count
   RTL1= -48;	// reload value for Timer 1, sets Baud rate to 9600
   TH0= -255;	// start value
   T2MOD= 0x31;	// use T2 for V24_1, count up
   T2CAPH= -1;	// set reload values
   T2CAPL= -6;
//  T2CON= 0x30;	// use T2 for V24_0
   TH2= -1;	// set for max. Baud
   TL2= -6;
   TR2= 1;	// start V24-timer

// Interrupt setting
   IT0 = 1;	// edge trig on ext0
   IT1 = 1;	// edge trig on ext1
   EX0= 0;	// disable ext0
   ET0= 1;	// enable timer0 intrpt
   ET1= 0;	// disable timer1 intrpt

// NB! IPAx PRIORITETS-SETTING SKAL MATCHE SETTING I ROM_VECTOR() !!!
// (Brug kun 9-F, 0 ell. 8 er intrpt off!)
   IPA0= 0x90;	// 0x76 prior. for Timer0/EX0 
   IPA4= 0xff;	// 0xbb serial 0 intrpt.
   IPA5= 0xff;	// 0xaa serial 1 intrpt.

   Serial0_init();
//   Serial1_init();

   ERI0 =1;
//   ETI0= 1;
   
   TR0 = 1;	// start loop timer
   TR1= 1;	// start V24-timer

   EA = 1 ;	// enable all

   slicer=0;
   Number=0;
   teller=0;
   options=0;
   reread=0;

// ******************************************************************
/* Reset all units. There are 3 resets to options, MRES1 to MRES3.
   In order to identify the position of the options, one reset is
   released at a time and an option address is send to this option.
   If the option then answeres back its address, the Master then
   knows the presens of this option.
   to allow all units to do their own initialisation. */
// ******************************************************************

   feed_dog;	// watch-dog feed

   MRES1_on;	// reset all options
   waitms(100);	// reset for at least 100 ms
   
   MRES1_off;	// release reset 1
   waitms(100);	// Wait for unit to init its interface

// ******************************************************************
 // RECALL OLD SETTINGS

   opstart();

   // TEST: SET DEFAULT VALUES



// ******************************************************************
// ENDLESS LOOP
// ******************************************************************
   while(1)
   {  while(Timer_flag == 0)	// while waiting for timer flag
      {  while ((cc=get_char0()) != 0)	// handle characters from buffer
		  CharHandler(cc);
      }

// 20 ms TIMER EXPIRED
// ******************************************************************

      Timer_flag = 0;

      slicer++; if (slicer > 10) slicer = 1;	// divide in 10 windows

      if (slicer == 1)		// window 1
// ******************************************************************
      {
      }

      if (slicer == 2) 		// window 2
// ******************************************************************
      {
      }

    if (slicer == 3) 		// window 3
// ******************************************************************
      { feed_dog;
      }

      if (slicer == 4)		// window 4
// ******************************************************************
      {
      }

      if (slicer == 5)		// window 5
// ******************************************************************
      {
      }

      if (slicer == 6)		// window 6
// *****************************************************************
      {
      }

      if (slicer == 7)		// window 7
// ******************************************************************
      {
      }

      if (slicer == 8) 		// window 8
// ******************************************************************
       { 
       }

      if (slicer == 9) 		// window 9
// ******************************************************************
      { 
      }
// ******************************************************************
   }
}

