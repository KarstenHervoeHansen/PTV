/**************************************************************************/
/* Copyright ProTeleVision Technologies A/S, BRONDBY 2000			 		  */
/* Project:		PT5201 Compact VariTime Sync Generator							  */
/* Module:		MAIN.C 																	  */
/* Author:		Kim Engedahl, DEV														  */
/* Org. date:	990521																	  */
/* Rev. date:	000307, KEn, DEV														  */
/* Status:		Version 1.0																  */
/*																								  */
/* This module contains the following functions:								  */
/*																								  */
/* 	_interrupt( IV_T0/4) _using( IV_SYSTEM+IV_PRI15) void Timer_int(void)	*/
/*	void main( void)																		  */
/*																								  */
/* Changes:																					  */
/**************************************************************************/
#include "system.h"
#include "main.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "rs232par.h"
#include "flash.h"
#include "util.h"
#include "iic.h"

#include "xadrivr.h"
#include "xaexprt.h"


#define xtal 29491200				// Crystal frequency for the mainboard

/**************************************************************************/
/* PLD PORT DEFINITIONS																	  */
/**************************************************************************/
//volatile int LEDPort _at( 0x6a00C); 	 		// Port defined in PLD
volatile char TestLEDPort _at( 0xEa014);		// Port defined in PLD
//volatile int PushButtonPort _at(0x6a010); 	// Port defined in PLD

/**************************************************************************/
/* GLOBAL VARIABLES																		  */
/**************************************************************************/
volatile _near byte UserTimer;	// Generel user timer: updated every 20ms

_bit LEDFlashON;
unsigned char LEDs;

/**************************************************************************/
/* timer_int																		 MAIN.C */
/*																								  */
/* Author:		Preben Christiansen, DEV, 960828									  */
/* Revised:		000307, KEn, DEV	     												  */
/*																								  */
/* Function:	Interrupt routine which handles ALL timing.					  */
/* Remarks:		Changing the crystal WILL affect the timing.					  */
/* Returns:		--																			  */
/* Updates:		--																			  */
/**************************************************************************/
_interrupt( IV_T0/4) _using( IV_SYSTEM+IV_PRI15) void timer_int(void) {

	static byte ticker;
	static _bit LEDON = FALSE;
	static byte LEDTimer = 0;

	TH0 = -xtal/256/4/3/50;		 	// (Re)-load timer to 150 Hz ( 6.6ms)

	if ( ++ticker > 2) {
		ticker = 0;

		if ( UserTimer)			 	// UserTimer is decremented each 20 ms.
			UserTimer--;

		if ( LEDFlashON)
			if ( LEDTimer-- == 0) {
				LEDTimer = 20;

				LEDON ^= 1;

				if ( LEDON)
					TestLEDPort = LEDs;
				else
					TestLEDPort = 0x00;
			}
	}
}

/**************************************************************************/
/* main																				 MAIN.C */
/*																								  */
/* Author:		Kim Engedahl DEV, 990521											  */
/* Revised:		000712, KEn, DEV	     												  */
/*																								  */
/* Function:	--																			  */
/* Remarks:		--																			  */
/* Returns:		--																			  */
/* Updates:		--																			  */
/**************************************************************************/
void main( void) {

	static int RS232Char;
	byte  manufacturer;
	word ID;

	// System setup
	BCR = 0x06;						// Set 20 bit BUS, 16 bit DATA
	PSWH = 0xAF;					// Set system priority high

//	BTRH = 0xAA;					// EXT. BUS timing, Data Cycle
//	BTRL = 0x8A;					// EXT. BUS timing, Wr+ALE-pulses+Code
	BTRH = 0xFA;					// EXT. BUS timing, Data Cycle
	BTRL = 0xCA;					// EXT. BUS timing, Wr+ALE-pulses+Code

	PSWL = 0;						// Clear flags
	SCR = 0;							// Timer speed prescaler ( 0->4, 4->16, 8->64)
	PSWH &= 0xF0;					// Set system priority low

	// Timer setup
	TMOD = 0x01;					// Timer 1: not used; Timer 0: 16 bit counter
	TH0 = -255;						// Start value

	// Interrupt setting
	ET0 = 1;							// Enable timer0 intrpt

	// Interrupt priority setting
	// Note! IPAx PRIORITY-SETTING MUST match the setting in the interrupt vector.
	// (Use only 9-F, 0 or 8 is interrupt off!)
	IPA0 = 0xF0;					// Priority Timer0/EX0
	IPA5 = 0xEE;					// Priority PTI1/PRI1, (serial port 1)

	TR0 = 1;					   	// Start loop timer

	EA = 1 ;							// Enable all interrupts

	flash_get_device_info( &manufacturer, &ID);

	switch( ID)
	{
		case ID_29DL800T:
			ID = Ndx_29DL800T;
			break;

		case ID_29DL800B:
			ID = Ndx_29DL800B;
			break;

		case ID_29LV800T:
			ID = Ndx_29LV800T;
			break;

		case ID_29LV800B:
			ID = Ndx_29LV800B;
			break;

		case ID_29F400T:
		case ID_29LV400T:
		case ST_M29F400T:
			ID = Ndx_29FLV400T;
			break;

		case ID_29F400B:
		case ID_29LV400B:
		case ST_M29F400B:
			ID = Ndx_29FLV400B;
			break;

		case AT_29F4096T:
			ID = Ndx_29F4096T;
			break;

		case AT_29F4096:
			ID = Ndx_29F4096;
			break;

		default:
			ID = Ndx_29FLV400T;
			break;
	}

	flash_init( ID);			// Initialise the flash EPROM device.
	LEDs = 0x03;

									// Initialise serial port
 	Udr_InitUart( UDR_BAUD_115K, UDR_NO_HANDSHAKE);

	UserTimer = 50;

	while ( UserTimer) 	  	// Delay for 1 second or until PLD is loaded
		;

	TestLEDPort = 0x03;	  	// Switch on LEDs. This should be done BEFORE setting
									//  FLASH_PRG, as this will disable programming this

	LEDFlashON = TRUE;

	while( TRUE) 				// Endless loop
	{
		while (( RS232Char = Udr_InChar()) != UDR_EOF)
		{
  			CmdHandler((byte) RS232Char);
		}
	}
 }
