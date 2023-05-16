/*
********************************************************************
* Project	: PT5210  Time Code Converter
* Filename	: TIME.C
* Version	: 1.1
* Purpose	: Main File
* Org.date	: 960717
* Author	: PRC
********************************************************************
HISTORY:
970xxx RELEASED 1.1
970127 Line count table for M added
960823
*/

#include <absacc.h>
#include <reg51.h>                      // define 8051 registers 
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// ***** DESCRIPTION *****
//	Background job in main() includes send to RS232 and kommunication
//	routine kommun().
//	V-interrupt presets the H counter and H-intrpt is enabled (sets TR0=1).
//	H has high priority. After line 23 H-interrupt disables itself
//	(sets TR0=0). V-interrupt, stil running, now calls fieldjobs()
//	which reads incoming code and prepare send buffers. After that,
//	the background job can run until the next V-interrupt.
//
// ***** HARDWARE DEFINITIONS *****
// Port definitions:
#define dualram	XBYTE[0x0000]
#define inport	XBYTE[0xc000]
#define iicport	P1


extern code UC linecountg[];
extern code UC linecountm[];
extern code UC invtab[];

// ***** COMMON DATA STRUCTURES *****
sbit locked=	P1^3;
sbit enable=	P1^4;
sbit kill=	P1^5;
sbit read=	P1^6;
sbit clear=	P1^7;
sbit crc=	P3^0;
sbit window=	P3^1;
sbit tenbit=	P3^2;
sbit gsys=	P3^5;

// ***** SYSTEM VARIABLES *****
UC slice;		// slice no. in the sliced frame
//UC found;		// flag for legal code found/state of output
UC wordno;		// word number in code
UC fieldno;		// actual field no. 
UC sendptr;		// pointer into dual port RAM
UC readbuf[9];		// buffer for words, read from input
UC flag;
bit found, done, load1, load2;	// flags for do load to output RAM
// ********************************************************************	

void put_char (char c)
{ // if (TI)
  {
    while (!TI) ;    // wait for transmitter ready 
    SBUF = c;	// set buffer before TI or else a interrupt between these
		// lines could hang up by calling put_char again.
    TI = 0;
  }
}
// ********************************************************************	

void delay(UC time)
{ while (time>0)  time--;	// delay= 1 us + time x 5 us
}
// ********************************************************************	
// Interrupts:  0: EXT0  1: T0  2: EXT1  3: T1  4: SER
// ********************************************************************	

void fieldint(void) interrupt 2 using 1
{
   fieldno= 0;
   slice= 0;
   if (gsys)
   {  TL0= linecountg[slice<<1];
      TH0= linecountg[(slice<<1) + 1];
   }
   else
   {  TL0= linecountm[slice<<1];
      TH0= linecountm[(slice<<1) + 1];
   }
   found= 0;
   clear= 1;		// reset FIFO-reg.
   clear= 0;
   TR0= 1;		// start H-counting
   TF0= 0;		// annulate pending H-count intrpt.  
}
// ********************************************************************	

code UC bitab[]=
{ 0xcc, 0xcd, 0xcb, 0xca, 0xd3, 0xd2, 0xd4, 0xd5,
  0xb3, 0xb2, 0xb4, 0xb5, 0xac, 0xad, 0xab, 0xaa
};

void bipha(UC a)
{  UC n;
   a= invtab[a];		// a is inverted before sending
   if (flag)  n= ~bitab[a/16];
   else n= bitab[a/16];
   XBYTE[sendptr++]= n;
   flag= n & 1;
   if (flag)  n= ~bitab[a & 15];
   else n= bitab[a & 15];
   XBYTE[sendptr++]= n;
   flag= n & 1;
}
// ********************************************************************	

void copys(UC *s, UC *t, UC n)
{ UC i;				// copy n chars from string t to s
  for (i=0;i<n;i++)
  		s[i]= t[i];
}
// ********************************************************************

void readwords(void)
{  UC i, n;
   for (i=0;i<8;i++)
   {  read= 1;		// shift data in FIFO reg.
      read= 0;
      n= invtab[XBYTE[0x8000]];	// read data
      XBYTE[32+i]= n;	// save in RAM
   }
   clear= 1;		// reset FIFO-reg.
   clear= 0;
}
// ********************************************************************

void konverter(void)
{  UC n1, n2;
   n1= XBYTE[32] & 15;		// add '1' to Frame no.
   n2= XBYTE[33] & 3;
   if (n1==9)
   {  n1= 0; n2 += 1;
   }
   else if (n1==5 && n2>=2)
   { n1= 1; n2= 0;
   }
   else  n1 += 1;

   XBYTE[32] &= ~15;  XBYTE[32] |= n1;
   XBYTE[33] &= ~3;  XBYTE[32] |= n2;

}
// ********************************************************************

// Default 23:59:59.01
UC deftab[]=
{ 1,0,0x90,0xa0,0x90,0xa0,0xc0,0x40,0xfc,0xbf
};

void loaddel1(void)
{ UC i;
   sendptr= 0;	// point to address in RAM
//   if (found)
      for (i=32;i<37;i++)  bipha(XBYTE[i]);	// convert data
//   else 
//      for (i=0;i<5;i++)  bipha(XBYTE[deftab[i]]);	// convert data
}
// ********************************************************************

void loaddel2(void)
{ UC i, n, a;
   sendptr= 10;	// point to address in RAM
//   if (found)
   {  for (i=37;i<39;i++)  bipha(XBYTE[i]);	// output word 6-7
      n= 0;
      a= XBYTE[39];		// check phase (count bit in  8'th byte)
      for (i=0;i<7;i++)
      {  if (a & 1)  n += 1;
         a >>= 1;
      }
      a= XBYTE[39];		// check phase (count bit in  8'th byte)

      if (flag) n += 1;		// add phase from byte 1-7
      if (!(n & 1))  a ^= 8;	// then change phase (bit 59)
      bipha(a);			// output word 8
      sendptr= 16; bipha(0xfc); bipha(0xbf); // output word 9-10
   }
//   else for (i=5;i<10;i++)  bipha(XBYTE[deftab[i]]);	// convert data
}
// ********************************************************************

UC testtab[]=
{ 0xe0,0,0x10,0xa0,0x60,0xa0,0xe0,0x00,0xfc,0xbf
};
// ********************************************************************
// ********************************************************************	

void main(void)
{ 
  UI i;

  SCON= 0x40;	// Transmit mode 3 (1 stopbit)	(2 stopbit: 0xC0)
  PCON= 0;	// => SMOD = 0, not double Baud Rate
  TMOD= 0x25;	// timer 1: 8 bit autoload; timer 0: 16 bit count
//  IT0= 1;	// edge trig on ext0 (90Bit-int)
//  EX0= 1;	// enable ext0
  IT1= 1;	// edge trig on ext1 (frame-int)
  EX1= 1;	// enable ext1
  ET0= 1;	// enable timer0 intrpt
  PT0= 1;	// High priority on line intrpt.  <9601
  ET1= 0;	// intrpt from timer1 disabled
  TH1= 0xfb;	// sets Baud rate to 9600 (fd:11. fc:14. fb:18.)
  TR0= 1;	// start timer 0
//  TI= 1;	// set transmitter ready

// INIT SETTINGS 

  window= 0;		// force no remaining v24 words


 // LOAD TIME - TEST 
  for (i=0;i<10;i++)  XBYTE[i+32]= testtab[i]; // put data i RAM fra 20h

 EA= 1;	// enable all, but timer 0 is not started (TR0)

// ********************************************************************	
// ENDLESS LOOP
// ********************************************************************	
   while(1)
   {  // if (found)
      {  if (found && !done)
         {  readwords();
	    konverter();
	    done= 1;
         }
         if (load1)
	 { loaddel1();
	   load1= 0;
	 }
         else if (load2)
	 { loaddel2();
	   load2= 0;
	 }
      }
   }
}  // end of 'main()'


