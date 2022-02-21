/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/03 SDI Digital test signal generator						*/
/*	Module:		COMM.C																		*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960703																		*/
/*	Rev. date:	970512, KEn	DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		bit ByteTxb( UC XS)																	*/
/*		UC IIC_Read( UC Address)															*/
/*		bit IIC_Write( UC Address, UC Data)												*/
/*		bit IIC_Write2( UC Address, UC Data, UC Data2)								*/
/*		bit IIC_WrStr( UC Address, UC subadr, char *s)								*/
/*		bit IIC_RdStr( UC Address, UC subadr, char *s)								*/
/*		bit IIC_WrOffset( UC Address, UC subadr, UL a)								*/
/*		UL IIC_RdOffset( UC Address, UC subadr)										*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

#include <string.h>
#include <xa.h>                      // define 8051 registers 

#include "d1gener.h"
#include "comm.h"
#include "d1util.h"

static bit CY @ 0x207;		// Carry
static bit SCL @ 0x38e;   	// P1^6
static bit SDA @ 0x38f;	   // P1^7
											
#define delay2		asm( "nop"); asm( "nop");
#define delay5		delay2; delay2; asm( "nop");
#define delay6		delay2; delay2; delay2;
#define delay7		delay5; delay2;
#define delay8		delay6; delay2;
#define delay12	delay6; delay6;
#define delay13	delay6; delay6; asm( "nop");
#define delay14	delay12; delay2;
#define delay15	delay13; delay2;
#define delay16	delay14; delay2;
#define delay17	delay12; delay5;
#define delay18	delay6; delay12;
#define delay19	delay7; delay12;

#define STARTCond  SDA=0; delay19; SCL=0;
#define STOPCond   SDA=0; delay18; SCL=1;while( !SCL);; delay15;SDA=1;delay18;

#define Retry 3

near UC IICErrorCode;

static bit Acknowledge;

static bit TestPort @0x38d;

/***************************************************************************/
/*	ByteTxb																 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970428, KEn			     													*/
/*																									*/
/*	Function:	EEPROM int read from IIC-bus 											*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:		-																				*/
/***************************************************************************/
bit ByteTxb( UC XS) {

	static bit Ack;
	register UC NoOfBit = 8;

	do {								// Coded to almost fastest execute time
		SCL = 0;
		XS <<= 1;
		SDA = CY;
		delay7;

		SCL = 1; 					// SCL = 1 for 4.0us
		while( !SCL)
			;
		delay15;

		SCL = 0;						// SCL = 0 for 4.7us
	} while( --NoOfBit);

	SDA = 1;

	delay17;

	SCL = 1;							// Ready for reading acknowledge bit
	while( !SCL)
		;
	delay5;
	Ack = !SDA;						// Read acknowledge from IIC

	SCL = 0;

	return( Ack);
}

/***************************************************************************/
/*	IIC_Read															 	  		  	  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970428, KEn			     													*/
/*																									*/
/*	Function:	Read one byte from IIC address <Address>							*/
/*	Remarks:		Check IICErrorCode for status of function							*/
/*	Returns:		Byte read from address													*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
UC IIC_Read( UC Address) {

	register UC Data, NoOfBit;

	for ( Data = 0; Data < Retry; Data++) {

		IICErrorCode = IIC_NoAck;

		STARTCond;

		if ( Acknowledge = ByteTxb( Address+1)) {

			IICErrorCode = NoError;
					
			Data = 0;							// Read byte into Data
			NoOfBit = 8;

			do {
				SCL = 1;
				while( !SCL)
					;
				delay8;
				Data += ( Data + ( char) SDA);

				SCL = 0;
				while ( SCL)
					;
				delay12;
			} while ( --NoOfBit);

			SDA = 1;				// Write Acknowledge bit  (`1` for last byte to read) 
			delay17;

			SCL = 1;
			while( !SCL)
				;
			delay15;

			SCL=0;
			break;
		}

		SCL = 1;					// Prepare for repeated start condition
		while( !SCL)
			;
		delay15;
	}
	STOPCond;

	return( Data);
}

/***************************************************************************/
/*	IIC_Write															 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970429, KEn			     													*/
/*																									*/
/*	Function:	Write one byte to IIC component at address <Address>			*/
/*	Remarks:		--																				*/
/*	Returns:		1:OK, 0:ERROR																*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit IIC_Write( UC Address, UC Data) {

	register UC i;

	for ( i = 0; i < Retry; i++) {

		IICErrorCode = IIC_NoAck;

		STARTCond;

		if ( Acknowledge = ByteTxb( Address)) {
			IICErrorCode = IIC_DataErr;

			if ( Acknowledge = ByteTxb( Data))
				IICErrorCode = NoError;
			break;
		}

		SCL = 1;						// Prepare for repeated start condition
		while( !SCL)
			;
		delay15;
	}
	STOPCond;

	Wait10ms();						// Wait 10ms before next write

	return( Acknowledge);
}

/***************************************************************************/
/*	IIC_Write2															 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970429, KEn			     													*/
/*																									*/
/*	Function:	Write two bytes to IIC component at address <Address>			*/
/*	Remarks:		--																				*/
/*	Returns:		1:OK, 0:ERROR																*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit IIC_Write2( UC Address, UC Data, UC Data2) {

	register UC i;

	for ( i = 0; i < Retry; i++) {

		IICErrorCode = IIC_NoAck;

		STARTCond;

		if ( Acknowledge = ByteTxb( Address)) {
			IICErrorCode = IIC_DataErr;

			if ( Acknowledge = ByteTxb( Data))
				if ( Acknowledge = ByteTxb( Data2))
					IICErrorCode = NoError;
			break;
		}

		SCL = 1;						// Prepare for repeated start condition
		while( !SCL)
			;
		delay15;
	}
	STOPCond;

	Wait10ms();						// Wait 10ms before next write

	return( Acknowledge);
}

/***************************************************************************/
/*	IIC_WrStr															 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970429, KEn			     													*/
/*																									*/
/*	Function:	Write string[<10] to IIC component at address <Address>		*/
/*	Remarks:		--																				*/
/*	Returns:		1:OK, 0:ERROR																*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit IIC_WrStr( UC Address, UC subadr, char *s) {

	register UC i;
	UC chksum = 0;

	for ( i = 0; i< Retry; i++) {

		IICErrorCode = IIC_NoAck;

		STARTCond;

		if ( Acknowledge = ByteTxb( Address)) {

			IICErrorCode = NoError;
			Acknowledge = ByteTxb( subadr);

			for ( i = 0; i < 10; i++) {
				if ( Acknowledge)
					Acknowledge = ByteTxb( s[i]);

				chksum += s[i];
	
				if ( !Acknowledge) {
					if ( IICErrorCode == NoError)
						IICErrorCode = IIC_DataErr;
					break;
				}

				if (((( i+1) & 3) == 0) || (( subadr+i+1) % 16) == 0) {
					STOPCond;			// Packet write

					Wait10ms();

					STARTCond;			// Store next (1-4) bytes

					if ( Acknowledge = ByteTxb( Address))
						Acknowledge = ByteTxb( subadr+i+1);
					else
						IICErrorCode = IIC_NoAck;
	   	   }
	   	}

			if ( Acknowledge && ( IICErrorCode == NoError))
				if ( !( Acknowledge = ByteTxb( chksum)))
					IICErrorCode = IIC_DataErr;
			break;
		}

		SCL = 1;						// Prepare for repeated start condition
		while( !SCL)
			;
		delay15;
	}
   STOPCond;

	Wait10ms();						// Wait 10ms before next write

   return( Acknowledge);
}

/***************************************************************************/
/*	IIC_RdStr															 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970429, KEn			     													*/
/*																									*/
/*	Function:	Read string[<10] from IIC component at address <Address>		*/
/*	Remarks:		Check IICErrorCode for status of function							*/
/*	Returns:		1:OK, 0:ERROR																*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit IIC_RdStr( UC Address, UC subadr, char *s) {

	register UC i;
	UC chksum = 0;

	if ( Acknowledge = IIC_Write( Address, subadr)) {

		for ( i = 0; i < 10; i++) {
			s[i] = IIC_Read( Address);		// Read char from IIC component
			chksum += s[i];

			if ( IICErrorCode)
				return( 0);
		}
		s[10]= '\0';							// If MAX. string terminate with 0

		if ( chksum - IIC_Read( Address)) {
			IICErrorCode = IIC_CheckSum;
			return( 0);
		}
	}

	return( Acknowledge);
}

/***************************************************************************/
/*	IIC_WrOffset														 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970428, KEn			     													*/
/*																									*/
/*	Function:	EEPROM long read from IIC-bus 2										*/
/*	Remarks:																						*/
/*	Returns:		1:OK, 0:ERROR																*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
bit IIC_WrOffset( UC Address, UC subadr, UL a) {

	register UC i;
	UC chksum, tmp;

	for ( i = 0; i < Retry; i++) {

		IICErrorCode = IIC_NoAck;

		STARTCond;

		if ( Acknowledge = ByteTxb( Address)) {

			IICErrorCode = IIC_DataErr;

			if ( Acknowledge = ByteTxb( subadr)) {

				chksum = tmp = ( a & 0xff000000) >> 24;
				if ( !( Acknowledge = ByteTxb( tmp)))
					break;

				tmp = ( a & 0x00ff0000) >> 16;
				chksum += tmp;
				if ( !( Acknowledge = ByteTxb( tmp)))
					break;
	
				tmp = ( a & 0x0000ff00) >> 8;
				chksum += tmp;
				if ( !( Acknowledge = ByteTxb( tmp)))
					break;

				tmp = a & 0x000000ff;
				chksum += tmp;
				if ( !( Acknowledge = ByteTxb( tmp)))
					break;

				if ( !( Acknowledge = ByteTxb( chksum)))
					break;

				IICErrorCode = NoError;
			}
			break;
		}

		SCL = 1;						// Prepare for repeated start condition
		while( !SCL)
			;
		delay15;
	}
   STOPCond;

	Wait10ms();						// Wait 10ms before next write

	return( Acknowledge);
}

/***************************************************************************/
/*	IIC_RdOffset														 	  		  COMM.C */
/*																									*/
/* Author:		Preben Christiansen, DEV, 941124	 									*/
/* Revised:		970428, KEn			     													*/
/*																									*/
/*	Function:	Read unsigned long from IIC ( address, subaddres)				*/
/*	Remarks:		Check IICErrorCode for status of function							*/
/*	Returns:		Unsigned long																*/
/*	Updates:		IICErrorCode																*/
/***************************************************************************/
UL IIC_RdOffset( UC Address, UC subadr) {

	register UC i;
	UC tmp, chksum = 0;
	UL y = 0;

	if ( IIC_Write( Address, subadr)) {		// Transmit RAM address, (retry < 3)

		for ( i = 0; i < 4; i++) {
			if ( IICErrorCode == NoError) {
				tmp = IIC_Read( Address);
				y = ( UL) tmp + ( y << 8);

				chksum += tmp;
			}
		}

		if ( IICErrorCode == NoError)
			if ( chksum - IIC_Read( Address))
				IICErrorCode = IIC_CheckSum; 
	}

	return( y);
}
