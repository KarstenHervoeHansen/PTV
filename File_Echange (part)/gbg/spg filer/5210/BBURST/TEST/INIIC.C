/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: INIIC.C
* Version	: 1.1
* Purpose	: IIC via uP-PORT
* Org.date	: 960215
* Author	: PRC
********************************************************************
HISTORY:
970114 RELEASED 1.1
970109 Inline assembler code removed. NOW OPTIMIZED FOR BTRH=AE,BTRL=C9.
960215 Adapted to XA processor.
*/

#include <xa.h>			// define 8051 registers
#include "equ.h"

#define UC 	unsigned char
#define UI 	unsigned int
#define sbit 	static bit

#define PAUS1	pau=0;pau=0;pau=0
#define PAUS2	PAUS1;PAUS1
#define PAUS4	PAUS2;PAUS2
#define SCL_3L  SCL=0;PAUS4;PAUS4
#define SCL_3H  SCL=1;PAUS4;PAUS4;PAUS4;PAUS1

#define STARTCond  SDA=0;PAUS4;PAUS4;PAUS4;SCL=0
#define STOPCond   SDA=0;SCL_3L;SCL=1;while(SCL==0);SCL_3H;SDA=1;PAUS4;PAUS4

sbit    SDA  @ 0x38f;   // P1^7
sbit    SCL  @ 0x38e;   // P1^6

static near UC pau;
sbit  CY @ 0x207;				     // Carry

extern   bit     Acknowledge;

// *****************************************************************

bit ByteTx2(UC XS) {
static bit Ack;
register UC NoOfBit = 8;

  do                       	// Coded to fastest execute time
    {  SCL=0;
       XS <<= 1;
	if ( CY)
		SDA=1;
	else
		SDA=0;
       PAUS4;
       SCL=1; PAUS4;PAUS4;PAUS2;PAUS1;
       while(SCL==0) ;
       SCL= 0; PAUS2;PAUS1;
    }
//  while (--X != 0);
    while( --NoOfBit);

  SDA= 1; PAUS4;PAUS4;PAUS2; 	// Ready for reading Acknowledge bit
  SCL = 1; PAUS4;PAUS2;
  while(SCL==0) ;
  Ack = !SDA;             // Read here ack-bit
  SCL = 0; PAUS2;PAUS1;
  return(Ack);
}
// *****************************************************************

UC Rd_iic(UC Address)
{ register UC B;
  STARTCond;
  Acknowledge = ByteTx2(Address+1);
  PAUS4;PAUS2;PAUS1;
  
/* Read byte into B  */
  B = 0;
  Address = 8;                /* Use Address as loop - counter */
  do
  {  SCL = 1; PAUS4;PAUS2;PAUS1;
     while(SCL==0);
     B += B + (char)SDA;
     SCL= 0; PAUS4;PAUS4;PAUS4;
  }
  while (--Address != 0);

/* Write Acknowledge bit  (`1` for last byte to read ) */

  SDA=1;  /* Ready for reading Acknowledge bit */
  SCL_3H;
  while(SCL==0) ;
  SCL=0; PAUS2;PAUS2;

  STOPCond;
  return (B);
}
// *****************************************************************
/*
 // EEPROM read routine for n bytes from address adr.
UL retval;

UL Rd_adr_n(UC Address, UC adr, UC n)   // read (max 4) bytes from adr (NVRAM)
{  register UC B;
   UC i;

   retval= 0;
   STARTCond;
   Acknowledge = ByteTx2(Address);
   Acknowledge = ByteTx2(adr);	// leaves SDA=1 and SCL= 0
   PAUS4;PAUS4;			// pause to set SCL HIGH
   SCL= 1;
   while(SCL==0)  ;

   STARTCond;			// restart with READ address
   Acknowledge = ByteTx2(Address+1);

   PAUS4;PAUS2;PAUS1;
   for (i=0;i<n;i++)		// read n bytes
   {	// Read byte into B 
  	B = 0;
  	Address = 8;                // Use Address as loop - counter
  	do
  	{  SCL = 1; PAUS4;PAUS2;PAUS1;
     	while(SCL==0);
     	B += B + (char)SDA;
     	SCL= 0; PAUS4;PAUS4;PAUS4;
  	}

  	while (--Address != 0);
        retval= (UL)B + retval;

	// Write Acknowledge bit  (`1` for last byte to read )

  	SDA=1;  // Ready for reading Acknowledge bit
  	SCL_3H;
  	while(SCL==0) ;
  	SCL=0; PAUS2;PAUS2;
   }
   STOPCond;
   return(retval);
}
*/
// *****************************************************************

void Wr_iic(UC Address,UC Data)
{  STARTCond;
   Acknowledge = ByteTx2(Address);
   ByteTx2(Data);
   STOPCond;
}
// *****************************************************************

void Wr_iic2(UC Address,UC Data,UC Data2)
{  STARTCond;
   Acknowledge = ByteTx2(Address);
   ByteTx2(Data);
   ByteTx2(Data2);
   STOPCond;
}
// *****************************************************************

void waitms(UI time)	// longer wait routine
{  UI n;
   while (time-- > 0) 	// wait 1 ms x time
   { n= 750;
     while (n>0)  n--;
   }
}
// ******************************************************************

void store_byte(UC Address,UC Data,UC Data2)
{  STARTCond;
   Acknowledge = ByteTx2(Address);
   ByteTx2(Data);
   ByteTx2(Data2);
   STOPCond;
   waitms(20);			// wait while NVRAM store bytes
}
// *****************************************************************

void store_phases(UC Address, UC subadr, NUL a, NUI b)
{  STARTCond;
   Acknowledge = ByteTx2(Address);
   ByteTx2(subadr);
   ByteTx2((UC)((a & 0xff000000)>>24));
   ByteTx2((UC)((a & 0x00ff0000)>>16));
   ByteTx2((UC)((a & 0x0000ff00)>>8));
   ByteTx2((UC)(a & 0x000000ff));
   STOPCond;
   waitms(20);			// wait while NVRAM store bytes
   STARTCond;			// store next (1-4) bytes
   Acknowledge = ByteTx2(Address);
   ByteTx2(subadr+4);
   ByteTx2((UC)((b & 0xff00)>>8));
   ByteTx2((UC)(b & 0x00ff));
   STOPCond;
   waitms(20);			// wait while NVRAM store bytes
}
// ******************************************************************

void store_string(UC Address, UC subadr, UC s[])
{ 
   STARTCond;
   Acknowledge = ByteTx2(Address);
   ByteTx2(subadr);

   ByteTx2(s[0]);
   ByteTx2(s[1]);
   ByteTx2(s[2]);
   ByteTx2(s[3]);
   STOPCond;
   waitms(20);			// wait while NVRAM store bytes
   STARTCond;			// store next (1-4) bytes
   Acknowledge = ByteTx2(Address);
   ByteTx2(subadr+4);
   ByteTx2(s[4]);
   ByteTx2(s[5]);
   ByteTx2(s[6]);
   ByteTx2(s[7]);
   STOPCond;
   waitms(20);			// wait while NVRAM store bytes
   STARTCond;			// store next (1-4) bytes
   Acknowledge = ByteTx2(Address);
   ByteTx2(subadr+8);
   ByteTx2(s[8]);
   ByteTx2(s[9]);
   STOPCond;
   waitms(20);			// wait while NVRAM store bytes
}
