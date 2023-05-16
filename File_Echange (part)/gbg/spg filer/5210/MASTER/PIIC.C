#include <stdio.h>
#include <xa.h>                      /* define 8051 registers */

/*
FILE: PIIC.C	(IIC via EXT. PORT)
HISTORY:
!!! while ((SCL&1)==0) er midlertidigt udeladt (for TEST) 5 steder.
960215 Adapted to XA processor, related to UIIC.C
*/

#define UC 	unsigned char
#define UI 	unsigned int
#define sbit 	static bit

#define SDA_0	iicptr &= ~2
#define SCL_0	iicptr &= ~1
#define SDA_1	iicptr |= 2
#define SCL_1	iicptr |= 1
// Each "pau=0" delays 0.15us
#define PAUS0	pau=0;pau=0;pau=0
#define PAUS	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define PAUS1	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define SCL_3L  SCL_0;PAUS1;PAUS;PAUS
#define SCL_3H  SCL_1;PAUS1;PAUS1
#define STARTCond  SDA_0;PAUS1;PAUS1;PAUS;SCL_0
#define STOPCond   SDA_0;SCL_3L;SCL_1;while((SCL&1)==0); SCL_3H;SDA_1;PAUS1;PAUS

static near UC pau;

volatile UC iicptr @ 0x2001;	// SKAL V’RE 0x2000!
#define SDA	iicptr
#define SCL	iicptr

static UC  B	@ 9;
sbit    B_7  @ 0x4f;	// R4H^7
extern bit  Acknowledge;

static UC X;

bit ByteTx(UC XX) {
static bit Ack;

  X = XX;
  asm("MOV R4H, _X");
  XX = 8;		//Use X as loop - counter

  do			// Coded to fastest execute time
    {	SCL_0;
	if (B_7) SDA_1; else SDA_0;	// SDA = B_7;
	pau=0;
        asm("ADD R4H, R4H");
	SCL_1; PAUS1; PAUS1;
	while((SCL&1)==0) ; // ****** GENINDS’TTES!
	SCL_0; PAUS; PAUS;
    }
  while (--XX != 0);

  SDA_1; PAUS; PAUS0;	// Ready for reading Acknowledge bit
  SCL_1; PAUS; // PAUS;
  while((SCL&1)==0) ; // ****** GENINDS’TTES!
  Ack = !(SDA & 2);	//Ack = !SDA	// Read here ack-bit
  SCL_0;
  return(Ack);
}

UC Read_iic(UC Address)
{ register UC B;
  STARTCond;
  Acknowledge = ByteTx(Address+1);
  PAUS1; PAUS;

  /* Read byte into B  */
  B = 0;
  Address = 8;                /* Use Address as loop - counter */
  do
  {  PAUS0;
     SCL_1; PAUS1;
     while((SCL & 1)==0); // ******* GENINDS’TTES!
     B += (B + (SDA & 2)>>1);	// B += B + (char)SDA;
     SCL_0; PAUS1; PAUS; PAUS;
  }
  while (--Address != 0);

/* Write Acknowledge bit  (`1` for last byte to read ) */

SDA_1;
SCL_1; PAUS1; PAUS;pau=0;
while((SCL&1)==0) ;	// ***** GENINDS’TTES!
SCL_0;

STOPCond;
return (B);
}


void Write_iic(UC Address,UC Data)
{
STARTCond;
Acknowledge = ByteTx(Address);
Acknowledge = ByteTx(Data);
STOPCond;
}

void Write_iic2(UC Address,UC Data,UC Data2)
{
STARTCond;
Acknowledge = ByteTx(Address);
ByteTx(Data);
ByteTx(Data2);
STOPCond;
}

//Write_iicn(UC Address,UC *Datstr,UC len)
//{
//  UC i;
//  i=0;
//  STARTCond;
//  Acknowledge = ByteTx(Address);
//  while (i<len)  ByteTx( *(Datstr + i++));
//  STOPCond;
//}
  
/* ********************************************************************	*/
void WrStrIIC(UC Address,UC *Data)
{  UC x;
   STARTCond;
   Acknowledge = ByteTx(Address);
   for (x = 0 ; ; ++x  ) {
     if(Data[x] == 0) break;
     ByteTx(Data[x]);
   }
   STOPCond;
}
/* ********************************************************************* */
void PutIICData(UC Address,char Aa,char Bb,UC Nn)
{ STARTCond;
  Acknowledge = ByteTx(Address);
  ByteTx(Aa);
  ByteTx(Bb);
  ByteTx(  Nn/100      + '0');
  ByteTx(( Nn/10) % 10 + '0');
  ByteTx(  Nn%10       + '0');
  ByteTx( '\n');
  STOPCond;
}
/* ********************************************************************* */
/* PutIICData16(UC Address,char Aa,char Bb,UI Nn)
{
STARTCond;
Acknowledge = ByteTx(Address);
ByteTx(Aa);
ByteTx(Bb);
ByteTx(  Nn/100      + '0');    // OBS! Handles only Nn < 1000 (3 digits)
ByteTx(( Nn/10) % 10 + '0');
ByteTx(  Nn%10       + '0');
ByteTx( '\n');
STOPCond;
} */

/*
void main(void)
{ UI i;
  BTRL= 0xff;	// 5f min. EXT. BUS timing
  BTRH= 0xff;	// 9f min. do., DATA
  BCR= 0x02;	// set 20 bit BUS, 8 bit DATA
  while(1) 
  { Write_iic(0xa0,0x55);
    i= Read_iic(0xa0);
    for (i=0;i<500;i++)  ;	// pause
  }

}
*/
