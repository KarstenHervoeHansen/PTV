/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator								*/
/*	Module:		RS232APL.C51																*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960713																		*/
/*	Rev. date:	960828, KEn DEV															*/
/*	Status:		Version 0.03																*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes: 961121 Added several test-functions(PRC)																		*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include "mas.h"

#include "52sync.h"
#include "serial1.h"
#include "rs232apl.h"

UL Number, Number1;
bit req,quit,save;	// flags for requests from frontplate
near char header1;		// store for 1'st letter in V24 cmd.

void syncstataa(void) {
	SndChar( SDISignalUnit.Address, 'S','P',(UC) Number);
}

// ******************************************************************

// OBS!! denne rutine er til TEST og snakker i munden p† KIM's RS232!!

void put_char0 (char c)  {
    S0BUF = c;
    ERI0 = 0;         /* Disable serial interrupt  */
    while (!TI0);    /* while waiting for transmitter ready */
    TI0 = 0;
    ERI0 = 1;
}
// ******************************************************************

void backchar(UC n)
{  UC i;
   put_char0 (' ');
   for (i=0;i<n;i++)  put_char0(AnswerBuf[i]);
   put_char0 (13);
}
// ******************************************************************

void get_syncstat(void)	// receive sync status
{ 
   if (SndRequest(sync_addr,'G','S')==0)
      if (AnswerCheck()==0)
      {  syncstatus= AnswerBuf[2] & 1;
	 backchar(3);
      }
}
// ******************************************************************

void gfase1(void)	// old eksample-routine
{  SndLong(bbu0_addr,'H','G',Number);
}
// ******************************************************************

void gfase2(void)	// old eksample-routine
{  SndLong(bbu0_addr,'J','G',Number);
}
// ******************************************************************

void load1g7(void)	// HK  new phase
{  SndLong(bbu0_addr,'H','K',Number);
}
// *****************************************************************

void load2g7(void)	// JK  new phase
{  SndLong(bbu0_addr,'J','K',Number);
}
// *****************************************************************

void mfase1(void)	// old eksample-routine
{  SndLong(bbu0_addr,'H','M',Number);
}
// ******************************************************************

void mfase2(void)	// old eksample-routine
{  SndLong(bbu0_addr,'J','M',Number);
}
// ******************************************************************

void load1ms(void)	// HL  new phase
{  SndLong(bbu0_addr,'H','L',Number);
}
// *****************************************************************

void load2ms(void)	// JL  new phase
{  SndLong(bbu0_addr,'J','L',Number);
}
// *****************************************************************

void global1(void)		// HO  -set global offset phase
{  SndLong(bbu0_addr,'H','O',Number);
}
// *****************************************************************

void global2(void)		// JO  -set global offset phase
{  SndLong(bbu0_addr,'J','O',Number);
}
// *****************************************************************

void scload1(void)	// HH  -set sch-phase, channel 1
{  SndInt(bbu0_addr,'H','H',Number);
}
// *****************************************************************

void scload2(void)	// JH  -set sch-phase, channel 2
{  SndInt(bbu0_addr,'J','H',Number);
}
// *****************************************************************
void sch1(void)		// old eksample-routine
{  SndLong(bbu0_addr,'H','S',(UI)Number);
}
// ******************************************************************

void sch2(void)		// old eksample-routine
{  SndLong(bbu0_addr,'J','S',(UI)Number);
}
// ******************************************************************

void phases1(void)
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr,'H','X');
      waitms(50);
      backchar(16);
   }
   else  SndSpecial(bbu0_addr, 'H','X','$');
}
// ******************************************************************

void phases2(void)
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr, 'J','X');
      waitms(50);
      backchar(16);
   }
   else  SndSpecial(bbu0_addr, 'J','X','$');
}
// ******************************************************************

void phases1m(void)
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr,'H','Y');
      waitms(50);
      backchar(16);
   }
   else  SndSpecial(bbu0_addr, 'H','Y','$');
}
// ******************************************************************

void phases2m(void)
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr, 'J','Y');
      waitms(50);
      backchar(16);
   }
   else  SndSpecial(bbu0_addr, 'J','Y','$');
}
// ******************************************************************

void daclev1(void)		// HD  -set/store/read DAC level
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr, 'H','D');
      waitms(20);
      backchar(3);
   }
   else if (save)
   {  save= 0;
      SndSpecial(bbu0_addr, 'H','D','$');
   }
   else SndChar(bbu0_addr,'H','D',(UC)Number);
}
// *****************************************************************

void dacoffset1(void)		// HC  -set/store/read DAC offset
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr, 'H','C');
      waitms(20);
      backchar(3);
   }
   else if (save)
   {  save= 0;
      SndSpecial(bbu0_addr, 'H','C','$');
   }
   else SndChar(bbu0_addr,'H','C',(UC)Number);
}
// *****************************************************************

void daclev2(void)		// JD  -set/store/read DAC level
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr, 'J','D');
      waitms(20);
      backchar(3);
   }
   else if (save)
   {  save= 0;
      SndSpecial(bbu0_addr, 'J','D','$');
   }
   else SndChar(bbu0_addr,'J','D',(UC)Number);
}
// *****************************************************************

void dacoffset2(void)		// JC  -set/store/read DAC offset
{  if (req)
   {  req= 0;
      SndRequest(bbu0_addr, 'J','C');
      waitms(20);
      backchar(3);
   }
   else if (save)
   {  save= 0;
      SndSpecial(bbu0_addr, 'J','C','$');
   }
   else SndChar(bbu0_addr,'J','C',(UC)Number);
}
// *****************************************************************

void reset1(void)
{  SndVoid(sync_addr,'H','Z');
}
// ******************************************************************

void reset2(void)
{  SndVoid(sync_addr,'J','Z');
}
// ******************************************************************

void hphsetg(void)
{  SndLong(sync_addr,'G','Z',Number);
}
// ******************************************************************

void hphsetm(void)
{  SndLong(sync_addr,'G','Y',Number);
}
// ******************************************************************

void gensetg(void)
{  SndLong(sync_addr,'G','F',Number);
}
// ******************************************************************

void gensetm(void)
{  SndLong(sync_addr,'G','G',Number);
}
// ******************************************************************

void gensetd6(void)
{  SndLong(sync_addr,'G','D',Number);
}
// ******************************************************************

void gensetd5(void)
{  SndLong(sync_addr,'G','E',Number);
}
// ******************************************************************

void hphcalc(void)
{  SndVoid(sync_addr,'G','A');
}
// ******************************************************************

void hphupg(void)
{  SndInt(sync_addr,'G','I',100);
}
// ******************************************************************

void hphupm(void)
{  SndInt(sync_addr,'G','J',100);
}
// ******************************************************************
code UC modetab[]=
{ 0xa1,0xaf,0xad,0xa5,0xa5,0xa4,0xb1,0xa1,0xa1,0xb0,0xa0,0xa0
};

void genmode(void)
{  SndChar(sync_addr,'G','M',(UC)Number);
   ud5buf= modetab[(UC)Number];	// default sync config
   ud5= ud5buf;
}
// ******************************************************************

char instring[11];	// receive string
char *instrptr;
UC instr_index;		// index into receive string (instring)
// *****************************************************************

void wrtext1(void)
{  instrptr= instring;
	  SndAsc_n(bbu0_addr, 'H','W',(UC)Number,instrptr);
}
// ******************************************************************

void rdtext1(void)
{  SndRequest_n(bbu0_addr, 'H','R',(UC)Number);
   waitms(100);
   backchar(12);
}
// ******************************************************************

void wrtext2(void)
{  instrptr= instring;
	  SndAsc_n(bbu0_addr, 'J','W',(UC)Number,instrptr);
}
// ******************************************************************

void rdtext2(void)
{  SndRequest_n(bbu0_addr, 'J','R',(UC)Number);
   waitms(100);
   backchar(12);
}
// ******************************************************************

void dummy(void)
{  SndVoid(sync_addr,'D','U');
}
// ******************************************************************
void NotFound(void)		// old eksample-routine
{ ;
}

struct kommando {
	 char Aa;
	 char Bb;
	 void (*funktion)(void);
};

code const struct kommando kommandotabel[] = {
'X','Y',syncstataa,
'G','M',genmode,
'G','Z',hphsetg,
'G','Y',hphsetm,
'G','A',hphcalc,
'G','I',hphupg,
'G','J',hphupm,
'G','F',gensetg,
'G','G',gensetm,
'G','D',gensetd6,
'G','E',gensetd5,

'H','G',gfase1,
'H','K',load1g7,
'H','M',mfase1,
'H','L',load1ms,
'H','O',global1,
'H','H',scload1,
'H','X',phases1,
'H','Y',phases1m,
'H','D',daclev1,
'H','C',dacoffset1,
'H','Z',reset1,
'H','W',wrtext1,
'H','R',rdtext1,

'J','G',gfase2,
'J','K',load2g7,
'J','M',mfase2,
'J','L',load2ms,
'J','O',global2,
'J','H',scload2,
'J','X',phases2,
'J','Y',phases2m,
'J','D',daclev2,
'J','C',dacoffset2,
'J','Z',reset2,
'J','W',wrtext2,
'J','R',rdtext2,

'S','C',sch1,
'S','D',sch2,
'J','X',phases2,
'J','Y',phases2m,
'D','U',dummy,
'x','x',NotFound
};

// Command Machine
// ******************************************************************

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
	      else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '$')
	      {  MesState = 0;
	         if (c == '?') req = 1; else req = 0;
	         if (c == '!') quit = 1; else quit = 0;
	         if (c == '$') save = 1; else save = 0;
	         CommandExecute(First,Second);
	      }  break;
      case 3: if(isdigit(c))  Number1 = 10 * Number1 + (c-'0');
	      else if (c==34)
	      {  MesState= 4;	// quote
		 instr_index= 0;
	      }
	      else if (c==10 || c==';' || c==13 || c== '?' || c== '!' || c== '$')
	      {  MesState = 0;
	         if (c == '?') req = 1; else req = 0;
	         if (c == '!') quit = 1; else quit = 0;
	         if (c == '$') save = 1; else save = 0;
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
// *****************************************************************
