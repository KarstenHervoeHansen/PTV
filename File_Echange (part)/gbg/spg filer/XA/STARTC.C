/*
File:		fil.c
Author:		PRC
HISTORY:

 Rel.2.3 00202643
951211 Error fixed: Code with timing <11.6us could not be decoded
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <xa.h>

/* ANDRE INCLUDES:
assert.h conio.h errno.h float.h hitech.h intrpt.h
limits.h setjmp.h stdarg.h stddef.h sys.h time.h
*/
/* INCLUDE FRA AKTUELT DIR:
#include "vars.c"
*/
/* KEIL KOMPATIBILITET: */
#define XBYTE ((UC *) 0x00000L)
#define xdata far		// gammel 'xdata'
#define data			// gammel 'data'

/* VARIABLE:
#define UC	unsigned char	// praktiske forkortelser
#define UI	unsigned int
#define sbit 	static bit
					   Ligger i psect sector:¿
near int  var1;		// intern RAM var. (adr.0-1FF) 		'rbss'
near int  var1 = 128;	// intern RAM var. med init.(adr.0-1FF)	'rdata'
far char  var2;		// var. i Banked RAM (adr.0-FFFFF = page 0-15)
** TYPER: bit, char, int, short, long, float

** POINTERE:
far char *  vptr1;	// pointer til Banked RAM
code far *  rptr1;	// pointer til Banked Code
char * far  fptr1;	// pointer i Banked RAM til char i page 0
far char * far fptr2;	// pointer i Banked RAM til char i Banked RAM
code char * cptr;	// opretter pointer til psect code
   NB! code pointer er IKKE kompatibel med normal eller far pointer!
const char * cptr;	// pointer til en konstant

** KONSTANTER:
const char  version = 3;
const char * cptr;		// pointer til en konstant
volatile UC  P1 @ 0x430;	// svarer til assembler:  _P1  equ  430h
   (SFR's er defineret i filen xa.h:  #include <xa.h>)
   NB! alle var's og alle porte som kan ‘ndres af INT-rutiner
   skal erkl‘res som VOLATILE! 
   static bit  flag1;		// opretter bit i: 	psect rbit
static bit  P3_0 @ 0x3a0;	// erkl‘rer SFR-bit (er ikke i xa.h)
static UC  R2H @ 0x05;		// erkl‘rer 8-bit register
 static UI starttab[]= { 0x0000, 0x8A99	    liggeri: 	psect bss
code char count= 0x1234;	// opretter count i: 	psect code
code char * countptr;		// opretter pointer til psect code
*/

/* LINKER:
   psect link-r‘kkef›lge: text, code
*/

/* ********************************************************************	*/
void put_char (UC c)
{   while (!TI0) ;    /* wait for transmitter ready */
    TI0 = 0;
    S0BUF = c;
}
/* ********************************************************************	*/
void delay(UC time)
{ while (time>0)  time--;	// delay= 1 us + time x 5 us
}
/* ********************************************************************	*/
/* Definition af interruptfunktion (her: serial_int) (si.82):
ROM_VECTOR(IV_RI0, serial_int, IV_PSW)
/* Andre interrupt-def:  se sidst i filen: xa.h */
/* ******************************************************************** */
interrupt void serial_int(void)
{ /* en interrupt funktion m† ikke have parametre */
  UC fielcnt, fieldno, lineno, vflag, port2tab[20], v24on;

  if (fieldno & 1)
  { lineno= 18; delay(2);}		// drop a line intr. in field 2
  else lineno= 0; // foer omvendt!!!
  fielcnt++;		// count fields
  delay (39);		// wait n x 3 + 1us
			// change 5 (appr. «line) if jitter on line windows out
  vflag= 1;		// set flag for "field was here"
	// prepare R5 for 1'st send to port2
  R2H= port2tab[lineno] | v24on;
  TL0= 0xff;
  TH0= 0xff;
  TR0= 1;		// start H-counting
  TF0= 0;		// annulate pending H-count intrpt.  
}
/* ********************************************************************	*/
/* Erkl‘ring af extern ASM-rutine (si.84): extern char left(char);
   Selve ASM-rutinen: se si.84-86.
   Bem‘rk: Navne i ASM skal have '_' (underscore) foran C-navnet */
/* ********************************************************************	*/
/* INLINE-ASM:
#asm
	MOV	R0,_var
#endasm      NB! Denne form m† ikke indg† i flow-kontrol (if, while, m.m.)
Alternativ:
 asm("MOV  R0, _var");    Kan indg† alle steder. */
/* ********************************************************************	*/
void main(void)
/* ********************************************************************	*/
{ UC v24cnt,rwords[20];
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

 /* LOAD VPS */
  XBYTE[0x40]= 0;
  for (i=0x45;i<0x5f;i++)  XBYTE[i]= 0x55; // reset transmit store
  XBYTE[0x41]= 0xaa; XBYTE[0x42]= 0xaa;

/* ENDLESS LOOP
/* ********************************************************************	*/
  while(1)
  { if (TR0==0)
    {
 /* ONE-TIME FIELD SERVICE IS DONE, NOW DO BACKGROUND JOBS */
      if (TI0==1)
        if (v24cnt <= 15)
		put_char(rwords[v24cnt++]); 	// send to V24
      else delay(5);		// do not stress master
      if (TR0==0) ;		// and communicate in the rest of the field
    }
  }

} // end of 'main()'


