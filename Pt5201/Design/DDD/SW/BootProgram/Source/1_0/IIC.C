/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		IIC.C																			*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960703																		*/
/*	Rev. date:	990910, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		_bit ByteTx2( byte XS)																*/
/*		byte RdFrontIIC( byte Address)													*/
/*		_bit WrFrontIIC( byte Address, byte Data)									 	*/
/*		_bit WrFrontIIC2( byte Address, byte Data, byte Data2)				 	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

// *************************************************************************
// Include files:


#include "system.h"
#include "iic.h"

#define PAUS	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define PAUS1	pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0

#define SCL_3L  SCL=0;PAUS1;PAUS1;
#define SCL_3H  SCL=1;PAUS1;PAUS1;

#define STARTCond  SDA=0;PAUS1;PAUS1;SCL=0
#define STOPCond   SDA=0;SCL_3L;SCL=1;while(SCL==0);SCL_3H;SDA=1;PAUS;PAUS

#define Retry	3

byte IICErrorCode;

_sfrbit CY  _atbit( PSWL, 7);	// Carry
_sfrbit SDA _atbit( P1, 6);   // P1^6
_sfrbit SCL _atbit( P1, 7);   // P1^7

static _near byte pau;

_bit Acknowledge;

/***************************************************************************/
/*	ByteTx2																 	  		 	IIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		990910, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC bus												*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		--																				*/
/***************************************************************************/
_bit ByteTx2( byte XS) {

	static _bit Ack;
	register byte NoOfBit = 8;

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
/*	RdFrontIIC															 	  		 	IIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		990910, KEn			     													*/
/*																									*/
/*	Function:	Read one byte from the IIC bus										*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
byte RdFrontIIC( byte Address) {

	register byte B, NoOfBit;

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
/*	WrFrontIIC															 	  		 	IIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		990910, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC bus												*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
_bit WrFrontIIC( byte Address, byte Data) {

	register byte i;

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
/*	WrFrontIIC2															 	  		 	IIC.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		990910, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC bus												*/
/*	Remarks:		--																				*/
/*	Returns:		Acknowledge bit, 1:OK, 0:ERROR										*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
_bit WrFrontIIC2( byte Address, byte Data, byte Data2) {

	register byte i;

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
