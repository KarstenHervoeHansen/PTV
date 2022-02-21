/****************************************************************************/
/* MODULE:                                                                  */
/*  unitprg.c - 							                                              */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "reg8051.h"
#include "define.h"        // Standard PTV defines
#include "nv_drv.h"
#include "unitprg.h"
#include "led_hw.h"
#include "rs232.h"

volatile UI BBSPGResetPort _at( 0x6a018);		// Port defined in PLD

_bit UnitPgmMode;

/**************************************************************************/
/* BBSPGReset																									  UNITPRG.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000330																		*/
/* Revised:	 000330, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void BBSPGReset( bool SPGReset, bool BBReset, bool SPGPrg)
{
	UI tmp = 3;

	if ( SPGReset)
		tmp &= 0xFE;

	if ( BBReset)
		tmp &= 0xFD;

	if ( SPGPrg)
		tmp |= 0x04;

	BBSPGResetPort = tmp;
}

/**************************************************************************/
/* BBSPGCommunication																					  UNITPRG.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000407																		*/
/* Revised:	 000607, KEn DEV 	    																				*/
/*							         																									*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void BBSPGCommunication( int unit)
{
	UI i;
 	bool Test1LEDFlag = FALSE;

	EA = 0;

	BBSPGReset( true, true, true); 			// Set program bit and reset units

	for ( i = 0; i < 60000; i++)	 			//  Wait
		;

	if ( unit == 0)
 		BBSPGReset( true, false, true);		// Release reset for unit to be programmed
	else
 		BBSPGReset( false, true, true);		// Release reset for unit to be programmed

	IEH = 0;										// Disable ALL interrupts
	IEL = 0;

	UnitPgmMode = 1;

	TR2 = 0;										// Stop timer 2
	T2CON &= 0xCF;							// Use timer 1 as baud rate generator for both
	T2MOD &= 0xCF;							//  serial ports.

	S0CON = 0xD8;								// UART mode 1: 1 start, 9 data, 1 stop,
															//  (9th bit used as stop bit)

	S1CON = 0xD8;								// UART mode 1: 1 start, 9 data, 1 stop,
															//  (9th bit used as stop bit)

	TR1 = 0;										// Stop Timer 1, just in case you know

	TMOD &= 0x0F;								// Timer 1: 16-bit auto-reload timer

	TH1 = RTH1 = 0xFF; 					// 115200 baud
	TL1 = RTL1 = 0xFC;

	TR1 = 1;										// Start Timer 1

	RI_0 = 0;					 					// Clear possible Interrupt flag RI_0
	TI_0 = 0;					 					// Clear possible Interrupt flag TI_0

	RI_1 = 0;					 					// Clear possible Interrupt flag RI_1
	TI_1 = 0;					 					// Clear possible Interrupt flag TI_1

	ERI0 = 1;										// Enable SIO0 Rx-interrupt
	ETI0 = 1;										// Enable SIO0 Tx-interrupt

	ERI1 = 1;										// Enable SIO1 Rx-interrupt
	ETI1 = 1;										// Enable SIO1 Tx-interrupt

	EA = 1;

	while ( true)
	{
		for ( i = 0; i < 60000; i++)
			;

		Test1LEDFlag ^= 1;
		TestLEDSet( Test1LED, Test1LEDFlag);
	}
}

