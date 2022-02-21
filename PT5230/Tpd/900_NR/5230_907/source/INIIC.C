/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		INIIC.C																		*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960703																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		bit ByteTx2( UC XS)																	*/
/*		UC RdFrontIIC( UC Address)															*/
/*		bit WrFrontIIC( UC Address, UC Data)											*/
/*		bit WrFrontIIC2( UC Address, UC Data, UC Data2)								*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/***************************************************************************/

// *************************************************************************
// Include files:

#include <string.h>
#include <xa.h>

#include "mas.h"
#include "iic.h"
#include "util.h"

#define PAUS	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define PAUS1	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0

#define SCL_3L  SCL=0;PAUS1;PAUS1;
#define SCL_3H  SCL=1;PAUS1;PAUS1;

#define STARTCond  SDA=0;PAUS1;PAUS1;SCL=0
#define STOPCond   SDA=0;SCL_3L;SCL=1;while(SCL==0);SCL_3H;SDA=1;PAUS;PAUS

#define Retry	3

UC IICErrorCode;

sbit    CY   @ 0x207;	// Carry
sbit    SDA  @ 0x38f;   // P1^7
sbit    SCL  @ 0x38e;   // P1^6

static near UC pau;
bit Acknowledge;

/***************************************************************************/
/*	ByteTx2																 	  		 INIIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970107, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC bus												*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		--																				*/
/***************************************************************************/
bit ByteTx2( UC XS) {

	static bit Ack;
	register UC NoOfBit = 8;

	do {								// Coded to almost fastest execute time
		SCL = 0;
		XS <<= 1;
		SDA = CY; PAUS1;
		SCL = 1; PAUS1; PAUS;
		while( SCL==0)
			;
		SCL = 0; PAUS;
	} while( --NoOfBit);

	SDA = 1; SDA = 1; SDA = 1; PAUS1;
										// Ready for reading Acknowledge bit
	SCL = 1; PAUS; PAUS;
	while( SCL == 0)
		;
	Ack = !SDA;						// Read here ack-bit
	SCL = 0;

	return( Ack);
}

/***************************************************************************/
/*	RdFrontIIC															 	  		 INIIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		980509, KEn			     													*/
/*																									*/
/*	Function:	Read one byte from the IIC bus										*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
UC RdFrontIIC( UC Address) {

	register UC B, NoOfBit;

	IICErrorCode = RxIICTimeout;

	STARTCond;

	for ( B=0; B < Retry; B++) 
		if ( Acknowledge = ByteTx2( Address+1)) {

			IICErrorCode = NoIICError;

			PAUS1; PAUS;
											// Read byte into B
			B = 0;
			NoOfBit = 8;
			do {
				SCL = 1; PAUS; PAUS;
				while( SCL == 0)
					;
				B += ( B + ( char) SDA);
				SCL = 0; SCL = 0; PAUS1; PAUS1;
			} while ( --NoOfBit);
								// Write Acknowledge bit  (`1` for last byte to read) 
			SDA = 1;			// Ready for reading Acknowledge bit
			SCL_3H;

			while(SCL==0)
				;
			SCL=0; SCL=0;
			break;
		}

	STOPCond;

	return( B);
}

/***************************************************************************/
/*	WrFrontIIC															 	  		 INIIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		980509, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC bus												*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit WrFrontIIC( UC Address, UC Data) {

	register UC i;

	IICErrorCode = TxIICTimeout;

	STARTCond;

	for ( i=0; i < Retry; i++)
		if ( Acknowledge = ByteTx2( Address)) {
			IICErrorCode = TxIICDataError;
	
			if ( Acknowledge = ByteTx2( Data))
				IICErrorCode = NoIICError;
			break;
		}

	STOPCond;

	return( Acknowledge);
}

/***************************************************************************/
/*	WrFrontIIC2															 	  		 INIIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		980509, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC bus												*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit WrFrontIIC2( UC Address, UC Data, UC Data2) {

	register UC i;

	IICErrorCode = TxIICTimeout;

	STARTCond;

	for ( i=0; i < Retry; i++)
		if ( Acknowledge = ByteTx2( Address)) {
			IICErrorCode = TxIICDataError;

			if ( Acknowledge = ByteTx2( Data)) {
				if ( Acknowledge = ByteTx2( Data2))
					IICErrorCode = NoIICError;		
			}
			break;
		}

	STOPCond;

	return( Acknowledge);
}
