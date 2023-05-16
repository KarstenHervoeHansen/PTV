/*
********************************************************************
* Project	: PT5210  Time Code Converter
* Filename	: TIME.C
* Version	: 1.3
* Purpose	: Main File
* Org.date	: 960717
* Author	: PRC
********************************************************************
HISTORY:
980811 Corrected phase error (when user bits in use -Norway)
970602 new loop body
970515 Corrected frame count (routine 'konverter()' etc.)
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
UC flag;		// polarity flag for use in 'bipha()'
bit found;		// flag for data found
bit done;		// not used
bit load1, load2;	// flags for ready to load frame 1,2 -data
bit invert;		// flag for uneven number of ones in data.
bit useok;		// flag for good data
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
   a= invtab[a];		// a is flipped left-right before sending
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
      readbuf[i]= n;	// save in RAM
   }
   clear= 1;		// reset FIFO-reg.
   clear= 0;
}
// ********************************************************************

void inchour(void)
{  UC n1, n2;
   n1= readbuf[6] & 15;		// add '1' to hour
   n2= readbuf[7] & 3;
   if (n1==9)
   {  n1= 0; n2 += 1;
   }
   else if (n1==4 && n2>=2)
   {  n1= 0; n2= 0;
   }
   else  n1 += 1;

   readbuf[6] &= ~15;  readbuf[6] |= n1;
   readbuf[7] &= ~3;  readbuf[7] |= n2;
}
// ********************************************************************

void incminut(void)
{  UC n1, n2;
   n1= readbuf[4] & 15;		// add '1' to minutes
   n2= readbuf[5] & 7;
   if (n1==9)
   {  n1= 0; n2 += 1;
      if (n2>=6)
      {  n2= 0; inchour();
      }
   }
   else  n1 += 1;

   readbuf[4] &= ~15;  readbuf[4] |= n1;
   readbuf[5] &= ~7;  readbuf[5] |= n2;

}
// ********************************************************************

void incsecond(void)
{  UC n1, n2;
   n1= readbuf[2] & 15;		// add '1' to seconds
   n2= readbuf[3] & 7;
   if (n1==9)
   {  n1= 0; n2 += 1;
      if (n2>=6)
      {  n2= 0; incminut();
      }
   }
   else  n1 += 1;

   readbuf[2] &= ~15;  readbuf[2] |= n1;
   readbuf[3] &= ~7;  readbuf[3] |= n2;
}
// ********************************************************************

void konverter(void)
{  UC n1, n2;
   n1= readbuf[0] & 15;		// add '1' to Frame no.
   n2= readbuf[1] & 3;
   if (gsys)
   {  if (n2>=2 && n1>=4)
      {  incsecond();		// G frame from 0 to 24
	 n1= 0; n2= 0;
      }
      else if (n1==9)
      {  n1= 0; n2 += 1;
      }
      else  n1 += 1;
   }
   else
   {  if (n1==9)
      {  n1= 0; n2 += 1;	// M frame from 0 to 29
	 if (n2>=3)
	 {  incsecond();
	    n2= 0;
	 }
      }
      else  n1 += 1;
   }

   readbuf[0] &= ~15;  readbuf[0] |= n1;
   readbuf[1] &= ~3;  readbuf[1] |= n2;

}
// ********************************************************************

void checkfase(void)
{  UC i, n, a;
   a= 0;	// a is used for counting inversions
   n= 1;	// count inversions, the sync word will invert the phase

   for (i=0;i<8;i++)
      a= a ^ (~readbuf[i]);	// zeroes will invert the phase
   for (i=0;i<8;i++)
   {  if (!(a & 1))  n += 1;	// count remaining phase inversions (0's)
      a >>= 1;
   }
   if (n & 1) invert= 1;	// uneven number will invert the phase
   else invert= 0;
}
// ********************************************************************

// Default 23:59:59.01
UC deftab[]=
{ 1,0,0x90,0xa0,0x90,0xa0,0xc0,0x40,0xfc,0xbf
};

// ********************************************************************

void loaddel1(void)
{ UC i;
   flag= 0;	// reset polarity flag
   sendptr= 0;	// point to address in RAM
   if (gsys)
   {  for (i=0;i<5;i++)  bipha(readbuf[i]);	// G: load data 0-4
   }
   else
   {  for (i=0;i<3;i++)  bipha(readbuf[i]);	// M: load data 0-4
      i= readbuf[3];
      if (invert)  i ^= 8;		// change phase (bit 27)
      bipha(i);
      bipha(readbuf[4]);
   }
}
// ********************************************************************

void loaddel2(void)
{  UC i;
   sendptr= 10;		// point to address in RAM for data 5
   if (gsys)
   {  for (i=5;i<7;i++)  bipha(readbuf[i]);	// output word 6-8
      i= readbuf[7];
      if (invert)  i ^= 8;	// change phase (bit 59)
      bipha(i);
   }
   else
   {  for (i=5;i<8;i++)  bipha(readbuf[i]);	// output word 6-8
   }
   sendptr= 16; bipha(0xfc); bipha(0xbf); // output word 9-10
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
  for (i=0;i<10;i++)  readbuf[i]= testtab[i]; // put data i RAM fra 20h

 EA= 1;	// enable all, but timer 0 is not started (TR0)
/*
// ********************************************************************	
// ENDLESS LOOP
// Funktionel Beskrivelse:
// Hvert Frame-interrupt s‘tter FOUND=0. I linierne 6-22 soeges der efter
// tidskode i modul 'hint.a'. hvis den ikke findes i field 1 soeges der
// ogsaa i field 2. Hvis tidskoden findes, s‘ttes FOUND=1.
// Efter linie 22 i field 1 s‘ttes LOAD2=1 som tegn til at field 2
// output data kan loades ind i Dual Port RAM'en (maa kun ske i field 1
// for at undgaa adressesammenfald for R/W). Tilsvarende i field 2 s‘ttes
// LOAD1=1. Nu er begge field's timekode ankommet. Hvis den er laest korrekt
// er FOUND=1 og nu laeses FIFO'en med Readwords(), den korrigeres 1 billede
// i Konverter(), faseudligningen bestemmes i Checkfase() og flaget
// USEOK saettes=1 som tegn paa at data er OK. Saa loades RAM'en med data for
// field1 og LOAD1 saettes=0. Naar i LOAD2=1 i field 1 og USEOK=1, loades
// RAM'en med data for field2 og der saettes LOAD2=0 og USEOK=0.
// ********************************************************************	
*/
   while(1)
   { 
         if (load2)
	 { if (useok)
	    {  loaddel2();		// first field
	       load2= 0;
	       useok= 0;		// data is used
	    }
	    load2= 0;
	 }
         else if (load1)
	 {			// second field
	    if (found)
            {  readwords();	// read new words from FIFO
	       konverter();	// correct for delay
	       checkfase();
	       loaddel1();
	       useok= 1;	// flag good data
	    }
	    load1= 0;
         }
   }
}  // end of 'main()'
