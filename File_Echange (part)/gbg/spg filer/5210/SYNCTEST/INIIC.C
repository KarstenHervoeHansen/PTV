/*
FILE: INIIC.C  (IIC via uP-PORT)
HISTORY:
960215 Adapted to XA processor.
*/
#include <xa.h>                      /* define 8051 registers */

#define UC 	unsigned char
#define UI 	unsigned int
#define sbit 	static bit

#define PAUS	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define PAUS1	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define SCL_3L  SCL=0;PAUS1;PAUS1;
#define SCL_3H  SCL=1;PAUS1;PAUS1;

#define STARTCond  SDA=0;PAUS1;PAUS1;SCL=0
#define STOPCond   SDA=0;SCL_3L;SCL=1;while(SCL==0);SCL_3H;SDA=1;PAUS;PAUS

// static near UC  B @ 9; // =R4H
sbit    SDA  @ 0x38f;   // P1^7
sbit    SCL  @ 0x38e;   // P1^6
sbit    B_7  @ 0x4f;	// R4H^7
static near UC pau;

extern   bit     Acknowledge;

static UC X;

bit ByteTx2(UC XS) {
static bit Ack;

  X = XS;
  asm("MOV R4H, _X");
  X = 8;             /* Use X as loop - counter */

  do                       /* Coded to fastest execute time */
    {  SCL=0;
        SDA= B_7; PAUS1;
       asm("ADD R4H, R4H");
       SCL=1; PAUS1; PAUS;
       while(SCL==0) ;
       SCL= 0; PAUS;
    }
  while (--X != 0);

  SDA= 1; SDA= 1; SDA=1; PAUS1;  /* Ready for reading Acknowledge bit */
  SCL = 1; PAUS; PAUS;
  while(SCL==0) ;
  Ack = !SDA;             /*Read here ack-bit */
  SCL = 0;
  return(Ack);
}

UC Rd_iic(UC Address)
{ register UC B;
  STARTCond;
  Acknowledge = ByteTx2(Address+1);
  PAUS1; PAUS;
  
/* Read byte into B  */
  B = 0;
  Address = 8;                /* Use Address as loop - counter */
  do
  {  SCL = 1; PAUS; PAUS;
     while(SCL==0);
     B += B + (char)SDA;
     SCL= 0; SCL= 0; PAUS1; PAUS1;
  }
  while (--Address != 0);

/* Write Acknowledge bit  (`1` for last byte to read ) */

  SDA=1;  /* Ready for reading Acknowledge bit */
  SCL_3H;
  while(SCL==0) ;
  SCL=0; SCL=0;

  STOPCond;
  return (B);
}

void Wr_iic(UC Address,UC Data)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  STOPCond;
}

void Wr_iic2(UC Address,UC Data,UC Data2)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  ByteTx2(Data2);
  STOPCond;
}

/* 
void main(void)
{ UI i;
  while(1) 
  { Wr_iic(0xa0,0x55);
    i= Rd_iic(0xa0);
    for (i=0;i<500;i++)  ;	// pause
  }

}
*/
