/*
FILE: INIIC.C  (IIC via uP-PORT)
HISTORY:
960215 Adapted to XA processor.
*/
#include <xa.h>                      /* define 8051 registers */

#define PCF8574P_KEYB	0x40
#define PCF8574P_LED	0x40
#define PCF8582A_RAM	0xA0
#define PCF8591P_ADC	0x90
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

static near UC X;

// ******************************************************************

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
// ******************************************************************

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
// ******************************************************************

void Wr_iic(UC Address,UC Data)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  STOPCond;
}
// ******************************************************************

void Wr_iic2(UC Address,UC Data,UC Data2)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  ByteTx2(Data2);
  STOPCond;
}
// ******************************************************************

UI Read_Keyb_IIC(UC Address)
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
// ******************************************************************

bit Write_X_Disp_IIC(UC Address, UC Data, UC Data2)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  ByteTx2(Data2);
  STOPCond;
  return(Acknowledge);  
}
// ******************************************************************

bit Write_Disp_IIC(UC Address, UC Data)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  STOPCond;
  return(Acknowledge);
}
// ******************************************************************

UI Read_Disp_IIC(UC Address)
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
// ******************************************************************

bit Write_Keyb_IIC(UC Address,UC Data)
{
  STARTCond;
  Acknowledge = ByteTx2(Address);
  ByteTx2(Data);
  STOPCond;
  return(Acknowledge);
}

void Stop_Disp_IIC() {
	STOPCond;			/* This is a macro. */
}

/***************************************************************************/
/*	Init_Disp_IIC																				*/
/*																									*/
/* Written by:	Kim Engedahl, DEV															*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			950602																		*/
/*	Revised:		950627																		*/
/*																									*/
/* Module:		IIC_DISP.C51																*/
/*	Function:	Initialize communication on the ADC-/EEPROM IIC-bus			*/
/*	Syntax:		uint Init_Disp_IIC();													*/
/*	Remarks:		----																			*/
/*	Returns:		unsigned integer															*/
/*	Updates:		----																			*/
/***************************************************************************/
UI Init_Disp_IIC() {
/*
	UC i = 16;

	do {
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		Disp_SCL = 1;
		while ( !Disp_SCL)
			;				

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		Disp_SCL = 0;
	} while ( --i);
*/
	return( Read_Disp_IIC( PCF8591P_ADC));
}

void Stop_Keyb_IIC() {
	STOPCond;		/* This is a macro. */
}

/***************************************************************************/
/*	Init_Keyb_IIC																				*/
/*																									*/
/* Written by:	Kim Engedahl, DEV															*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			950602																		*/
/*	Revised:		950602																		*/
/*																									*/
/* Module:		IIC_KEYB.C51																*/
/*	Function:	Initialize communication on the IIC-bus							*/
/*	Syntax:		bit Init_Keyb_IIC();														*/
/*	Remarks:		----																			*/
/*	Returns:    0 if NO acknowledge from slave,										*/
/*					1 if acknowledge from slave											*/
/*	Updates:		----																			*/
/***************************************************************************/
bit Init_Keyb_IIC() {
/*
	uchar i = 16;

	do {
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		Keyb_SCL = 1;
		while ( !Keyb_SCL)
			;				

		_nop_();
		_nop_();
		_nop_();
		_nop_();

		Keyb_SCL = 0;
	} while ( --i);

	STOPCond;
*/
	return( Write_Keyb_IIC( PCF8574P_KEYB, 0xFF));
}
