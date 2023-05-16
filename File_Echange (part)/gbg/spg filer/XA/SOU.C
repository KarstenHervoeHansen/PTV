/*
File:		sou.c
Author:		PRC
HISTORY:

 Rel.2.3 00202643
951211 Error fixed: Code with timing <11.6us could not be decoded
       because too many run-in's were skipped.
       Some 'Leer-kode's for default VPS corrected.
951205 Error fixed: fieldjobs() disturbed biphase().
 Ver.2.2 00202642 lagt p† net 941222
941018 Communication now safer by use of "genread" time-out in kommun().
940921 Now init of rcmode (=2).

 Ver.2.1 00202641
940913 Command 0x15 removed (saves time).
940912 BTS now 700 mV and run-in code AA A5, read chars is inverted.
940713 Full access to VPS data words.
940624 No. of interrupts to Master lowered.
940617 Ready to Beta-test.
940504 Test software.
*/

/* #include <absacc.h>
#include <reg51.h>               */       /* define 8051 registers */
/* #include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h> */
#include "vars.c"

#include <xa.h>

static bit P3_0 @ 0x3a0;
static bit P3_5 @ 0x3a5;
static UC R2H @ 0x05;
// ********** HARDWARE DEFINITIONS *******
// Port definitions:
#define port1	XBYTE[0xc000]
#define port2	XBYTE[0x8000]
#define inport	XBYTE[0xc000]
#define iicport	P1
// Port Bit definitions:
#define	window1	1
#define	detpos2	2
#define	window3	4
#define	window4	8
#define	_window5 16
#define	_window6 32
#define	detpos1	128
#define	play	1
#define	ampl	2
#define v24enab	8
#define	sel0	16
#define	sel1	32
#define	sel2	64
#define	sel3	128

// ********** SYSTEM VARIABLES *******
// ********************************************************************
xdata UC port1tab[]=
{ 48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,
 48,48,48,48,48,48,48,48,48,48,48,48,48,48,48
};	// Idle value for port1 in lines 6-23+319-327
// ********************************************************************
xdata UC port2tab[]=
{ 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
};	// Idle value for port2 in lines 6-23+319-327
// ********************************************************************
// Test-VPS texts:
UC vpstab[4][9]=
{ "\324ESTVPS1",
  "\324ESTVPS2",
  "\324ESTVPS3",
  "\324ESTVPS4"
};
// ********************************************************************
// Start codes:
static /*code*/ UI starttab[]=
{ 0x0000,	// dummy
  0x91D5,	// EBU
  0x8A99,	// VPS
  0x8A99,	// PDC
  0x0000,	// WSC
  0x0000,	// TEK
  0xA86A,	// BTS
  0x8A99,	// VPS1
  0x8A99,	// VPS2
  0x8A99,	// VPS3
  0x8A99,	// VPS4
  0x0000,	// Dummy (test-mode)
  0x0000	// Dummy (test-mode)
};
// ********************************************************************
static /*code*/ UC idtab[]=
{ ' ','E','V','P',' ',' ','B'};	  // V24 out headers EBU, VPS, PDC,BTS
// ********************************************************************
static /*UC*/ masktab[]=
{ 0,0,16,32,48,64,80,96,112,128,144,240,240};	// transmit mode mask
// ********************************************************************
static /*code*/ UC amptab[]=
{ 0,2,2,2,2,0,0,2,2,2,2,2,0};	// transmit amplitude mask (2=500mV, 0=700mV)
// ********************************************************************
// Test-VPS codes:
static /*code*/ UC vpsdat1[]=
{ 0,0,0x9f,0,0x5f,0x2f,0,0xff,0xff,0x41,0x82,0x20,0x16,0xbf,0xff };
static /*code*/ UC vpsdat2[]=
{ 0,0,0x5f,0,0x8f,0x4f,0,0xff,0xff,0x42,0xcb,0x54,0x07,0x42,0xff };
static /*code*/ UC vpsdat3[]=
{ 0,0,0xdf,0,0x2f,0xbf,0,0xff,0xff,0x43,0x81,0xfe,0xfd,0x87,0xff };
static /*code*/ UC vpsdat4[]=
{ 0,0,0xbf,0,0xff,0xcf,0,0xff,0xff,0x44,0xc1,0xfd,0xfe,0x6c,0xff };
// ********************************************************************
// Default codes:
static /*code*/ UC defcod8[]=
{ 0x00,	// WSC word hi
  0x00,	// WSC word lo
  0x20,	// TEK word hi
  0x01,	// TEK word lo
  0x04,	// VPS word 3
  0xf6,	// VPS word 5
  0xff,	// VPS word 6
  0x00,	// VPS word 8
  0x00,	// VPS word 9
  0x00,	// VPS word 10
  0x81,	// VPS word 11
  0xff,	// VPS word 12
  0xff,	// VPS word 13
  0xff,	// VPS word 14
  0xff,	// VPS word 15
  0x6f,	// PDC word 5
  0x80,	// PDC word 11
  0x3f,	// PDC word 12
  0xff,	// PDC word 13
  0x7f,	// PDC word 14
  0xff	// PDC word 15
};


// COMMON DATA STRUCTURES
UC cmdbuf[9];		/* command receiver buffer */
UC bufptr;		/* pointer into receive buffer */
bit running;		/* flag for download is done */
bit fieldjob;		/* flag for ready to do field background job */
#define field8 P3^3
// sbit field8= P3^3;	/* field 8 input */
char lineno;	/* line index no. (0..17,19..35)for counting in V-interval */
UC fieldno;		/* actual field no. */

// ********** COMMUNICATION VARIABLES *******
UC ccc;			/* Character variable */
UC oldread;		/* The second last character read */
UC bufofs;		/* Offset into receive byffer */
UC genread;		/* Counter for repeated reading */
bit cmdin;		/* Flag for command is recieved */
#define portisread P3^5
			/* Test input for port is ready */
#define signal	P3_0
			/* Output for clocking */
bit vflag;		/* flag to inhibit "ready" if Kommun intrpted */
// xdata UC bufcnt, loadcnt;	// for TEST

// ********** SOURCE DECODER VARIABLES *******
UC fielcnt;		/* field counter */
char tries;		/* no. of attempts to read a certain bit */
UI errorcopy;
UC word;		/* used for collecting bits to words */
UC worderrcnt;		/* counts word read biphase-errors */
bit wrderr;		/* flag for last read word had an error */
UI ricode;		/* received run-in code */
UI strtcode;		/* received startcode */
UI rstartcode;		/* defined decoder start code */
xdata UC rwords[16];	/* store for received words */
UC ascofs;		/* ofset into text collect string */
UC soutxt[9];		/* dynamic text collect string for receiver */
xdata UC txtbuf[]= "????????";	/* store for received text */
// #define REGBANK1 ((char *)0x40008L)
UC rcmode;		/* receiver mode */
xdata UC startok;	/* flag for startcode OK */
extern xdata UC invtab[]; /* invert table for BTS received chars */
// xdata UI errorcnt;	TEST
// UC runinerrcnt;	for TEST /* counts run-in read biphase-errors */
// UC strterrcnt;	for TEST /* counts startword read errors */

// ********** SOURCE CODER VARIABLES *******
UC v24on;		/* mask for V24 on/off */
UC v24cnt;		/* word counter */
UI sendptr;		/* offset into transmit array */
UC trampli;		/* transmitter amplitude */
UC trmode;		/* transmitter mode */
UC trmask;		/* transmitter mode port mask */
 UC atrtext[]= "\240       ";	/* store for transmit-text */
 UC atrtext7[]= "\240       ";	/* store for do. VPS, Word 7 */
UC *trtext=atrtext;
UC *trtext7=atrtext7;
static /* code */  UC trtext2[]= "\240       ";	/* VPS-text for opposite field */
bit testmode;		/* flag for testmode on */
UI wscword;		/* WSC code */
UI tekword;		/* TEK code */
UC bitno;		/* for WSC packing */
UC sum;			/* collect byte for WSC packing */
UC wscadr;		/* offset addr. from WSC store start */

// ********** DETECTOR VARIABLES *******
UC detmask;			/* detector position mask */
xdata UC vdet1lin, vdet2lin;	/* ITS detector lines */
xdata UC ddet1lin, ddet2lin;	/* Data detector lines */
xdata UC decodlin, codlin;	/* Source decoder/coder lines */
xdata UC status, oldstat1, oldstat2;	/* status from detectors, filtered */
/* ********************************************************************	*/
void put_char (UC c)
{ // if (TI0)
  {
    while (!TI0) ;    /* wait for transmitter ready */
    TI0 = 0;
    S0BUF = c;
  }
}
/* ********************************************************************	*/
void delay(UC time)
{ while (time>0)  time--;	// delay= 1 us + time x 5 us
}
/* ********************************************************************	*/
// Interrupts:  0: EXT0  1: T0  2: EXT1  3: T1  4: SER
/* ********************************************************************	*/
/* ******************************************************************** */
interrupt void fieldint(void)  // 0 using 1
/* ******************************************************************** */
{ // TR0= 0;		// stop H-counting
  if (field8==0 && fieldno>8)  fieldno= 0; else fieldno++;
  if (fieldno & 1)
  { lineno= 18; delay(2);}		// drop a line intr. in field 2
  else lineno= 0; // foer omvendt!!!
  fielcnt++;		// count fields
  delay (39);		// wait n x 3 + 1us
			// change 5 (appr. «line) if jitter on line windows out
  vflag= 1;		// set flag for "field was here"
	// prepare R5 for 1'st send to port2
  R2H= port2tab[lineno] | v24on | trmask | trampli;
  TL0= 0xff;
  TH0= 0xff;
  TR0= 1;		// start H-counting
  TF0= 0;		// annulate pending H-count intrpt.  
}
/* ********************************************************************	*/
// TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST
/* ********************************************************************	*/
static /*code*/ UC testtab[]=
{ 0x00,0xaa,0xaa,0xaa,0xaa,0xcc,0xcc,0xcc,0xcc,0xf0,0xf0,0xf0,0xf0,0xff,0x00,0xff,
  0x00,0xff,0xff,0x00,0x00,0x08,0x08,0x08,0x08,0xf7,0xf7,0xf7,0xf7,0x55,0x55,0x00,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
// ********************************************************************
void testsend(void)
{ data UC i;
  for (i=0;i<64;i++)  XBYTE[0x3c0+i]= testtab[i];
}
// ********************************************************************
void bipha(UC a)
{ data UC m,b;
 b= 0;
 for (m=0;m<3;m++)
 { if (a & 1)  b |= 2; else b |= 1;
   b<<=2; a>>=1;
 }
 if (a & 1)  b |= 2; else b |= 1;
 a>>=1;
 XBYTE[sendptr++]= b;
 b= 0;
 for (m=0;m<3;m++)
 { if (a & 1)  b |= 2; else b |= 1;
   b<<=2; a>>=1;
 }
 if (a & 1)  b |= 2; else b |= 1;
 a>>=1;
 XBYTE[sendptr++]= b;
}
/* ********************************************************************	*/
void bipha2(UC a) // using 2
{ data UC m,b;
 b= 0;
 for (m=0;m<3;m++)
 { if (a & 1)  b |= 2; else b |= 1;
   b<<=2; a>>=1;
 }
 if (a & 1)  b |= 2; else b |= 1;
 a>>=1;
 XBYTE[sendptr++]= b;
 b= 0;
 for (m=0;m<3;m++)
 { if (a & 1)  b |= 2; else b |= 1;
   b<<=2; a>>=1;
 }
 if (a & 1)  b |= 2; else b |= 1;
 a>>=1;
 XBYTE[sendptr++]= b;
}
/* ********************************************************************	*/
void copys(UC  *s,  UC *t, UC n)
{ UC i;				// copy n chars from string t to s
  for (i=0;i<n;i++)
  		s[i]= t[i];
}
/* ********************************************************************	*/
UC readn(void)
{ UC c;				// DERIVE A BYTE FROM RECEIVE BUFFER
  c= cmdbuf[bufptr++] - '0';
  c= 10 * c + cmdbuf[bufptr++] -'0';
  c= 10 * c + cmdbuf[bufptr++] -'0';
  return(c);
}  
/* ********************************************************************	*/
void putwsc(UC i, UC n)
{ while (i-- > 0)
  { sum<<=1;  if (n==1) sum |= 1;		// shift and set new bit
    bitno++;
    if (bitno==8)
  	{ XBYTE[sendptr+wscadr++]= sum; sum= 0; bitno= 0;}
  }
}  
/* ********************************************************************	*/
void docmd(UC cmd)
{ UC i, svar, wordno;
  UC *t;
  // loadcnt++;		for TEST
  bufptr= 1;	// point to 1'st buffer pos. after header

  if (!(cmd & 0x40))
  switch (cmd)
  { 
    case 0x01:	running= 1; testmode= 0; // EA= 1;
		break;
    case 0x10:	i= readn(); if ((i & 0x3f) > 34)  break;
    		if (running)
		{ port1tab[vdet1lin & 0x3f] |= _window5;} // erase old ITSDET1
		vdet1lin= i;		// set new if on
		if (!(i&128))  port1tab[vdet1lin & 0x3f] &= ~(_window5);
		if (i&64) detmask |= detpos1; else detmask &= ~detpos1;
		break;
    case 0x11:	i= readn(); if ((i & 0x3f) > 34)  break;
	    	if (running)
		{ port1tab[vdet2lin & 0x3f] |= _window6;} // erase old ITSDET2
    		vdet2lin= i;		// set new if on
		if (!(i&128))  port1tab[vdet2lin & 0x3f] &= ~(_window6);
		if (i&64) detmask |= detpos2; else detmask &= ~detpos2;
		break;
    case 0x12:	i= readn(); if ((i & 0x3f) > 34)  break;
    		if (running)
    		{ port1tab[ddet1lin & 0x3f] &= ~window3;} // erase old DATADET1
		ddet1lin= i;		// set new if on
		if (!(i&128)) { port1tab[ddet1lin & 0x3f] |= window3;}
  		break;
    case 0x13:	i= readn(); if ((i & 0x3f) > 34)  break;
    		if (running)
    		{ port1tab[ddet2lin & 0x3f] &= ~window4;} // erase old DATADET2
		ddet2lin= i;		// set new if on
		if (!(i&128)) { port1tab[ddet2lin & 0x3f] |= window4;}
  		break;
    case 0x14:	i= readn(); if ((i & 0x3f) > 34)  break;
    		if (running)
    		{ if (decodlin>17)  decodlin -= 18;
		  port1tab[decodlin] &= ~window1; // erase old DECODER LINE
		  port1tab[decodlin+18] &= ~window1;
		}
		decodlin= i & 0x3f;		// set new
		if (!(i&128))
		{ port1tab[decodlin] |= window1;
		  if (decodlin>17)  port1tab[decodlin-18] |= window1;
		  else  port1tab[decodlin+18] |= window1;
		}
  		break;
/*    case 0x15:	if (running)
    		{ port2tab[codlin] &= ~play; // erase old CODER LINE
		  port2tab[codlin+18] &= ~play;
		}
		i= readn(); codlin= i & 0x3f;	// set new
		if (codlin>17)  codlin -= 18;
		if (!(i&128))
		{ port2tab[codlin] |= play;
		  port2tab[codlin+18] |= play;
		}
  		break;
*/
    case 0x16:	i= readn(); if (i>6)  break;
    		rcmode= i; if (rcmode==4)  rcmode= 6;
  		rstartcode= starttab[rcmode];  // DECODER start code
  		break;
    case 0x17:	i= readn(); if (i>12)  break;
		trmode= i;		  // CODER start code
		trmask= masktab[i];	  // select play RAM area
		trampli= amptab[i];
		if (i>10)  testmode=1;
		else if (trmode>=7)
		// { copys(trtext,&vpstab[trmode-7][0],8); // set VPSTEXT
//  		{ t= &vpstab[trmode-7][0]; // set VPSTEXT

  		{ t= &vpstab[trmode-7][0]; // set VPSTEXT
		  copys(trtext,t,8);
		}
		break;
    case 0x18:	i= readn(); if (i>1)  break;
  		if (i==0) v24on= 0; else v24on= v24enab;  // V24 ON/OFF
  		break;
    case 0x20:	if (trmode<7)
  		{ copys(trtext,&cmdbuf[0]+1,8);	// new ASCII text for transmit
		  if (trmode==2)  trtext[0] |= 0x80; // VPS flag
		  if (trmode==6)
		  { sendptr= 0x147; 		// BTS-mode
		    bipha(invtab[trtext[0]]);	// bits are inverted in BTS
		    bipha(invtab[trtext[1]]); bipha(invtab[trtext[2]]);
		    bipha(invtab[trtext[3]]); bipha(invtab[trtext[4]]);
		  }
		}
  		break;
    case 0x21:	if (trmode==2)
  		{ copys(trtext7,&cmdbuf[0]+1,8); // new ASCII text word 7
		  trtext7[0] |= 0x80; // VPS flag
		}
  		break;
    case 0x22:	wordno= readn();	// VPS word 3,5,6 + 8-15
		if (wordno>15) break;
		sendptr= 0x40 + 2 * wordno - 1;
		bipha(readn());
  		break;
    case 0x23:	wordno= readn();	// PDC word 5 + 11-15
		if (wordno>15) break;
		sendptr= 0x80 + 2 * wordno - 1;
		bipha(readn());
  		break;
    case 0x24:	sendptr= 0xc0; wscadr= 7;	// WSC WORD (takes ap. 2ms)
		i= readn() & 0x3f; 		// point to send store
  		wscword= (UI)i * 256 + readn();
		sum= 1; bitno= 1;		// initiate loop
  		while (wscadr<0x12)
		{ if (wscword & 1)
		  { putwsc(3,1); putwsc(3,0); }
		  else
		  { putwsc(3,0); putwsc(3,1); }
		  wscword >>= 1;
		}
  		break;
    case 0x25:	sendptr= 0x100; wscadr= 0x0b;	// TEK WORD (takes ap. 2.4ms)
		i= readn() & 0x3f; 		// point to send store
  		wscword= (UI)i * 256 + readn();
		sum= 0; bitno= 4;		// initiate loop
  		while (wscadr<0x1d)
		{ if (wscword & 1)
		  { putwsc(5,1); putwsc(5,0); }
		  else  putwsc(10,0);
		  wscword >>= 1;
		}
  		break;
    case 0x28:	/* RESET CODE */
    		if (trmode==2)
  		{ sendptr= 0x45; bipha(defcod8[4]); // word 3
  		  sendptr= 0x49; bipha(defcod8[5]); // word 5
  		  bipha(defcod8[6]); // word 6
		  sendptr= 0x4f; bipha(defcod8[7]); bipha(defcod8[8]); // 8-9
		  bipha(defcod8[9]); bipha(defcod8[10]); // word 10-11
		  bipha(defcod8[11]); bipha(defcod8[12]); // word 12-13
		  bipha(defcod8[13]); bipha(defcod8[14]); // word 14-15
		}
		else if (trmode==3)
  		{ sendptr= 0x89; bipha(defcod8[15]); // word 5
  		  sendptr= 0x95; bipha(defcod8[16]); bipha(defcod8[17]); // 11-12
  		  bipha(defcod8[18]); bipha(defcod8[19]); // word 13-14
  		  bipha(defcod8[20]);			// word 15
		}
		else if (trmode==4)
		{ sendptr= 0xc0; wscadr= 7;	// WSC WORD (takes app. 2ms)
  		  wscword= defcod8[0]*256 + defcod8[1];
		  sum= 1; bitno= 1;		// initiate loop
  		  while (wscadr<0x12)
		  { if (wscword & 1)
		    { putwsc(3,1); putwsc(3,0); }
		    else
		    { putwsc(3,0); putwsc(3,1); }
		    wscword >>= 1;
		  }
		}
		else if (trmode==5)
		{ sendptr= 0x100; wscadr= 0x0b;	// TEK WORD (takes app. 2.4ms)
  		  wscword= defcod8[2]*256 + defcod8[3];
		  sum= 0; bitno= 4;		// initiate loop
  		  while (wscadr<0x1d)
		  { if (wscword & 1)
		    { putwsc(5,1); putwsc(5,0); }
		    else  putwsc(10,0);
		    wscword >>= 1;
		  }
		}
  		break;
  default:	
  		break;
  }
  else
  switch (cmd)
  {
    case 0x40:	i= status; svar= 0;
		if (!(ddet1lin & 128) || !(ddet2lin & 128)) svar |= 64;
		if (!(vdet1lin & 128) || !(vdet2lin & 128)) svar |= 16;
					// set loss flags
		if (!(i&16 || ddet2lin&128) || !(i&8 || ddet1lin&128)) svar |= 128;
		if (!(i&4 || vdet2lin&128) || !(i&2 || vdet1lin&128)) svar |= 32;
		iicport= svar;
		break;
    case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:
    case 0x56: case 0x57:
		iicport= txtbuf[cmd-0x50]; break;
    default:	
  		break;
  }
}
/* ********************************************************************	*/
void kommun(void)
{ vflag= 0;		// clear flag for Field interrupt during Kommun() 
  if (!portisread)
  { delay(10);		// old answer not read (delay * 3 us)
    if (!vflag)  { signal= 0; signal= 0; signal= 1;} // signal ready to master
  }
  else 
  { iicport = 0xff;			// prepare read of port
    ccc= iicport;
    if (ccc!=iicport)
    { ccc= iicport;	// reread if not equal reading
    }
    if (ccc==oldread)
    { genread++;
      if (!vflag && genread>100)
      { signal= 0; signal= 0; signal= 1; genread= 0;} // signal ready to master
      delay(10);	// skip if not new byte
    }
    else
    { /* if (((ccc & 128)^(oldread & 128))== 0 && ccc!=255)
    	{ // delay(1); // bit 7 SHALL toggle, TEST
	  XBYTE[0x7003]= oldread;
	  XBYTE[0x7003]= ccc;
	  XBYTE[0x7003]= bufofs;
	} */
      oldread= ccc;			// new byte, save
      genread= 0;
      if (ccc!= 0xff)
      { if (!(ccc & 128) && !cmdin)	// command or data, set flag
	{ cmdin= 1; 	// bufofs= 0; bufcnt=0; TEST
/*	  if (ccc > 0x25)
	  { 	XBYTE[7000]=ccc;	// TEST
		XBYTE[0x7000]= cmdbuf[0];
		XBYTE[0x7000]= loadcnt;
		XBYTE[0x7000]= bufcnt;
	  } */
	}
        cmdbuf[bufofs++]= ccc & 0x7f;	// put in buffer
	// bufcnt++;	TEST
      }
      else if (cmdin)
      { docmd(cmdbuf[0]);	// FF read, do command
	bufofs= 0;
      }
      if (!vflag) { signal= 0; signal= 0; signal= 1;} // signal ready to master
    }
  }
}
/* ********************************************************************	*/
// SOURCE DECODE SOFTWARE
// ********************************************************************
UC read1(void)
{ port1= 0x70;			// read from shift register
  port1= 0x30;
  --tries;
//  XBYTE[0x7010]= inport&1;	// TEST
  return(inport&1);
}
// ********************************************************************
UC readword(void) // using 2
{ data UC b;		// read a 8 bit word
  static bit val;
  wrderr = 0;
  word = 0;

  for (b=0;b<8;b++)
  { word >>= 1;
    val= 0;
    port1= 0x70; port1= 0x30;	// read from shift register
    if (inport & 1)  { word |= 128; val=1; }	// set read-bit on bit7 place
    port1= 0x70; port1= 0x30;	// read from shift register
    if ((inport & 1) == val)
	wrderr = 1; 			// error if next bit is the same
  }
  if (wrderr)  worderrcnt++;
  return(word);
}
// ********************************************************************
void fieldjobs(void)  // using 2
      { // extern void decode(void);
	UC i,j;

        port2= 0xf0 | v24on | trampli;  // set dual-port RAM OUT dummy address

	// PERIODICALLY 
	if ((fielcnt & 31) == 0)
	{ /* if (decodlin==9) 
          { errorcopy= errorcnt;	// TESTTESTTEST
	    for (i=0;i<6;i++)
	    { txtbuf[7-i]= errorcopy%10 +'0'; errorcopy /= 10;}
	  } 
	  else */

	  if (rcmode==6)
	  { txtbuf[5]= ' ';txtbuf[6]= ' ';txtbuf[7]= ' ';
	    for (i=0;i<5;i++)
	    {   txtbuf[i]= invtab[soutxt[i]]; // copy string from BTS
		soutxt[i]= 0xfc;		// reset collected text
	    }
	  }
          else for (i=0;i<8;i++)
	  { txtbuf[i]= soutxt[i];	// copy collected text every 0.6s
	    soutxt[i]= '?';		// reset collected text
	  }
	}


// *** DECODE ROUTINE ***

// window opens at 11.5us. Latest run-in start is for VPS at 14us
// window runs 51.2us. All cells are then used in SHReg.
// Run-in starts at 12.5us +- 1.5 = from 11 to 14 us,
// with lock adjusting (+-550ns): 10.4 to 14.6 us and finishes
// 16 x 0.2us after = from 13.6 to 17.8 us after sync.
// ID-word then starts at from (13.6-11.5=) 2.1 to 6.3 us, that is 
// 10.5 to 31.5 samples inside the shift register.
// ): skip 8 samples and find ID in the next (31.5+16-9)=38.5->45 samples.

{  startok= 0;		// flag for startcode ok
  
// SKIP RUN-IN
  for (i=0;i<8;i++)  read1();

// NOW FIND START CODE *************
  { tries = 45;		// read max 45 bits to find start code
    strtcode = 0;	// collect the code

    while (tries>0 && strtcode != rstartcode)
    		strtcode = strtcode<<1 | read1();	// insert new bit

    if (tries==0)
		{ 
		    tries++; /* errorcnt++;  strterrcnt++; */ }
    else

// START WAS FOUND, NOW READ THE DATA WORDS (USE BIPHASE TEST) ********
    {
 // RUN-IN
    	put_char(0xff);		// send run-in on V24
	startok= 1;
 // START-ID
	put_char(idtab[rcmode]); // send startcode-ID on V24
 // WORD 3
    	put_char(i=readword());	// read and send word 3 on V24
	if (rcmode==1 && !wrderr)  ascofs= i>>5; // EBU, read field counter
 // WORD 4
	word = readword();	// read ASCII-word 4
	if (!wrderr)
	{ if (rcmode==1)
	  { soutxt[ascofs%8] = word & 0x7f;	// EBU save char in string
	  }
	  else if (rcmode==2)
	  { if (!(fieldno & 1) && decodlin<18)
	    {	if ((word & 128) && ascofs>9 )  ascofs= 0;
		soutxt[ascofs>>1] = word & 0x7f; // VPS save char in string
	    }
	    else if ((fieldno & 1) && decodlin>17)
	    {	if ((word & 128)  && ascofs>9 )  ascofs= 0;
		soutxt[(ascofs)>>1] = word & 0x7f;
	    }
	  }
	}
    	put_char(word);		// send word 4 on V24
	rwords[4]= word;	// save in collect string
	
 // WORDS 5-15
	v24cnt= 5;		// remaining 11 words to V24
	for (i=5;i<16;i++)  rwords[i]= readword(); // read remaining words

	if (rcmode==6 && startok)
	{ for (i=0;i<5;i++)
	        soutxt[i]= rwords[4+i]; // copy string from BTS
	}
    } // end of 'if not startcodeerror'
  } // end of 'if not runerr'
}

// END OF DECODE


        ascofs++; if(ascofs>15)  ascofs= 16; // count receive string offset

 /* CHANGE WORD 4 (and 7 in VPS) AND SET FIELD COUNTER IN EBU */
	if (trmode==1)
	{ sendptr= 5;		// point to counter and ascii in send buffer
	  bipha2(((fieldno+1)%8)<<5);	// put counter
	  bipha2(trtext[(fieldno+1)%8]);	// put ascii as biphase code
	}			// new char for every field
	else if ((trmode==2) && (fieldno & 1))
	{ sendptr= (trmode-1)*64 +7;	// point to ascii (VPS)
	  if (fieldno<16) 
	  { bipha2(trtext[(fieldno+1)%16 >>1]);	// put ascii as biphase code
	  			// the same char is sent for 2 fields
	    sendptr= (trmode-1)*64 +0x0d;	// point to ascii (VPS)
				// send as word 7
	    bipha2(trtext7[(fieldno+1)%16 >>1]); // put ascii as biphase code
	  }
	}
	else if ((trmode>6) && (fieldno & 1))
	{ sendptr= (trmode-1)*64 +7;	// point to ascii (VPS)
	  if (fieldno<16) 
	  { bipha2(trtext[(fieldno+1)%16 >>1]);	// put ascii as biphase code
	  			// the same char is sent for 2 fields
	    sendptr= (trmode-1)*64 +0x0d;	// point to ascii (VPS)
				// send as word 7
	    bipha2(trtext2[(fieldno+1)%16 >>1]); // put ascii as biphase code
	  }
	}
 
 /* CHECK DATA AND VITS DETECTORS */
	j= inport;
	if (fieldno & 1)
	{ if ((ddet1lin & 0x3f)>17)
	  { if (j & 8 & oldstat1) status |= 8; else status &= ~8;
	    oldstat1= j;
	  }
	  if ((ddet2lin & 0x3f)>17)
	  { if (j & 16 & oldstat2) status |= 16; else status &= ~16;
	    oldstat2= j;
	  }
	  if (!(vdet1lin & 128) && (vdet1lin & 0x3f)>17)
		{ status &= ~2; status |= (j & 2);}
	  if (!(vdet2lin & 128) && (vdet2lin & 0x3f)>17)
		{ status &= ~4; status |= (j & 4);}
	}
	else
	{ if ((ddet1lin & 0x3f)<17)
	  { if (j & 8 & oldstat1) status |= 8; else status &= ~8;
	    oldstat1= j;
	  }
	  if ((ddet2lin & 0x3f)<17)
	  { if (j & 16 & oldstat2) status |= 16; else status &= ~16;
	    oldstat2= j;
	  }
	  if (!(vdet1lin & 128) && (vdet1lin & 0x3f)<17)
		{ status &= ~2; status |= (j & 2);}
	  if (!(vdet2lin & 128) && (vdet2lin & 0x3f)<17)
		{ status &= ~4; status |= (j & 4);}
	}

	fieldjob= 0;		// end of field services
      }
/* ********************************************************************	*/
/* ********************************************************************	*/
void main(void)
/* ********************************************************************	*/
{ 
  UI i;

  S0CON= 0x40;	// Transmit mode 3 (1 stopbit)	(2 stopbit: 0xC0)
  PCON= 0;	// => SMOD = 0, not double Baud Rate
  TMOD= 0x26;	// timer 1: 8 bit autoload; timer 0: 8 bit count autoload
  IT0= 1;	// edge trig on ext0 (field)
  EX0= 1;	// enable ext0
  ET0= 1;	// enable timer0 intrpt
  ET1= 0;	// intrpt from timer1 disabled
  TH1= 0xfb;	// sets Baud rate to 9600 (fd:11. fc:14. fb:18.)
  TR1= 1;	// start timer 1
  TI0= 1;	// set transmitter ready

/* INIT SETTINGS */

  v24cnt= 0;		// force no remaining v24 words

  rcmode= 2;		// default receive mode
  trmode= 2;		// default transmit mode

  cmdbuf[0]= 0;		// init communication machine
  oldread= 0xff;	
  genread= 0;		// time-out counter for kommun()
  bufofs= 0;

  trampli= 2;		// set mask for transmit amplitude (500mV)
  trmask= 0;		// and corresponding mode (code-address)

 /* LOAD EBU */
  XBYTE[0]= 0;
  for (i=5;i<0x1f;i++)  XBYTE[i]= 0x55;	// reset transmit store
  XBYTE[0x1f]= 0;
  XBYTE[1]= 0xaa; XBYTE[2]= 0xaa;
  XBYTE[3]= starttab[1] >> 8;	// put startcode to generator
  XBYTE[4]= starttab[1] & 0xff;

 /* LOAD VPS */
  XBYTE[0x40]= 0;
  for (i=0x45;i<0x5f;i++)  XBYTE[i]= 0x55; // reset transmit store
  XBYTE[0x5f]= 0;
  XBYTE[0x41]= 0xaa; XBYTE[0x42]= 0xaa;
  XBYTE[0x43]= starttab[2] >> 8; // put startcode to generator
  XBYTE[0x44]= starttab[2] & 0xff;

 /* LOAD PDC */
  XBYTE[0x80]= 0;
  for (i=0x85;i<0x9f;i++)  XBYTE[i]= 0x55;	// reset transmit store
  XBYTE[0x9f]= 0;
  XBYTE[0x81]= 0xaa; XBYTE[0x82]= 0xaa;
  XBYTE[0x83]= starttab[3] >> 8; // put startcode to generator
  XBYTE[0x84]= starttab[3] & 0xff;

 /* LOAD WSC */
  for (i=0xd0;i<0xe1;i++)  XBYTE[i]= 0;	// reset transmit store
  XBYTE[0xc0]= 0x0F; XBYTE[0xc1]= 0x8e; XBYTE[0xc2]= 0x38;
  XBYTE[0xc3]= 0xe3; XBYTE[0xc4]= 0x8f; XBYTE[0xc5]= 0x1e;
  XBYTE[0xc6]= 0x0f;  

 /* LOAD TEK */
  for (i=0x100;i<0x121;i++)  XBYTE[i]= 0;	// reset transmit store
  XBYTE[0x10A]= 0x3e; XBYTE[0x11d]= 0x7c;  

 /* LOAD BTS */
  XBYTE[0x140]= 0;
  for (i=0x145;i<0x15f;i++)  XBYTE[i]= 0x55;	// reset transmit store
  XBYTE[0x15f]= 0;
  XBYTE[0x141]= 0xaa; XBYTE[0x142]= 0xa5;
  XBYTE[0x143]= starttab[6] >> 8; // put startcode to generator
  XBYTE[0x144]= starttab[6] & 0xff;

 /* LOAD VPS USER 1 */
  XBYTE[0x180]= 0; XBYTE[0x181]= 0xaa; XBYTE[0x182]= 0xaa;
  ccc= starttab[2] >> 8; sum= starttab[2] & 0xff;
  XBYTE[0x183]= ccc; XBYTE[0x184]= sum; // put startcode to generator
  sendptr= 0x185;  
  for (word=2;word<15;word++)  bipha(vpsdat1[word]);
  XBYTE[0x19f]= 0;

 /* LOAD VPS USER 2 */
  XBYTE[0x1c0]= 0; XBYTE[0x1c1]= 0xaa; XBYTE[0x1c2]= 0xaa;
  XBYTE[0x1c3]= ccc; XBYTE[0x1c4]= sum; // put startcode to generator
  sendptr= 0x1c5;  
  for (word=2;word<15;word++)  bipha(vpsdat2[word]);
  XBYTE[0x1df]= 0;

 /* LOAD VPS USER 3 */
  XBYTE[0x200]= 0; XBYTE[0x201]= 0xaa; XBYTE[0x202]= 0xaa;
  XBYTE[0x203]= ccc; XBYTE[0x204]= sum; // put startcode to generator
  sendptr= 0x205;  
  for (word=2;word<15;word++)  bipha(vpsdat3[word]);
  XBYTE[0x21f]= 0;

 /* LOAD VPS USER 4 */
  XBYTE[0x240]= 0; XBYTE[0x241]= 0xaa; XBYTE[0x242]= 0xaa;
  XBYTE[0x243]= ccc; XBYTE[0x244]= sum; // put startcode to generator
  sendptr= 0x245;  
  for (word=2;word<15;word++)  bipha(vpsdat4[word]);
  XBYTE[0x25f]= 0;

  /* LOAD PAGE 16 WITH TEST DATA */
  testsend();


  /* SIGNAL READY TO MASTER */
  signal= 0; signal= 0; signal= 1;

 running= 0;
 while(!running)
 { kommun();		// load from master
 }
 EA= 1;	// enable all, but timer 0 is not started (TR0)
// errorcnt= 0;	TEST

/* ********************************************************************	*/
/* ENDLESS LOOP
/* ********************************************************************	*/
  while(1)
  { if (TR0==0)
    { // while (fieljob)

 /* ONE-TIME FIELD SERVICE IS DONE, NOW DO BACKGROUND JOBS */
      if (TI0==1)
        if (v24cnt <= 15)
		put_char(rwords[v24cnt++]); 	// send to V24
      else delay(5);		// do not stress master
      if (TR0==0)  kommun();		// and communicate in the rest of the field
    }
  }

} // end of 'main()'


