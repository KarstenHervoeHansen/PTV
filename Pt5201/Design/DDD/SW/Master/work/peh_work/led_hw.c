/****************************************************************************/
/* MODULE:                                                                  */
/*  led_hw.c - LED driver                                                   */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*	void LEDInit( void);																										*/
/*	int LEDSet( UC LED, bool value);																				*/
/*	int TestLEDSet( UC LED, bool value);																		*/
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
#include "reg8051.h"
#include "define.h"        // Standard PTV defines
#include "led_hw.h"

volatile UI LEDPort _at( 0x6a00C); 	 		// Port defined in PLD
volatile UC TestLEDPort _at( 0x6a014);	// Port defined in PLD

static UI LEDMirror;
static UC TestLEDMirror;
static UC LEDState = NormLEDs;

/**************************************************************************/
/* LEDInit																										   LED_HW.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000321																		*/
/* Revised:		000321, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void LEDInit( void)
{
	LEDMirror = 0;
	TestLEDMirror = 0;
}

/**************************************************************************/
/* LEDSet																											   LED_HW.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000321																		*/
/* Revised:		001128, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int LEDSet( UI LED, bool value)
{
 	switch ( LED)
	{
		case BB1PALLED:	
			if ( value)
	 			LEDMirror |= BB1PALLED;
			else
				LEDMirror &= ~BB1PALLED;
			break;

		case BB1NTSCLED:	
			if ( value)
		 		LEDMirror |= BB1NTSCLED;
			else
				LEDMirror &= ~BB1NTSCLED;
			break;

		case BB2PALLED:	
			if ( value)
	 			LEDMirror |= BB2PALLED;
			else
				LEDMirror &= ~BB2PALLED;
			break;

		case BB2NTSCLED:	
			if ( value)
		 		LEDMirror |= BB2NTSCLED;
			else
				LEDMirror &= ~BB2NTSCLED;
			break;

		case BB3PALLED:	
			if ( value)
	 			LEDMirror |= BB3PALLED;
			else
				LEDMirror &= ~BB3PALLED;
			break;

		case BB3NTSCLED:	
			if ( value)
		 		LEDMirror |= BB3NTSCLED;
			else
				LEDMirror &= ~BB3NTSCLED;
			break;

		case CBPALLED:	
			if ( value)
		 		LEDMirror |= CBPALLED;
			else
				LEDMirror &= ~CBPALLED;
			break;

		case CBNTSCLED:	
			if ( value)
	 			LEDMirror |= CBNTSCLED;
			else
				LEDMirror &= ~CBNTSCLED;
			break;

		case AnalogAudioLED:	
			if ( value)
		 		LEDMirror |= AnalogAudioLED;
			else
				LEDMirror &= ~ AnalogAudioLED;
			break;

		case AESEBUAudioLED:	
			if ( value)
		 		LEDMirror |= AESEBUAudioLED;
			else
				LEDMirror &= ~ AESEBUAudioLED;
			break;

		case D25LED:	
			if ( value)
		 		LEDMirror |= D25LED;
			else
				LEDMirror &= ~ D25LED;
			break;

		case D27LED:	
			if ( value)
	 			LEDMirror |= D27LED;
			else
				LEDMirror &= ~ D27LED;
			break;

		case WordClk441LED:	
			if ( value)
		 		LEDMirror |= WordClk441LED;
			else
				LEDMirror &= ~ WordClk441LED;
			break;

		case WordClk48LED:	
			if ( value)
	 			LEDMirror |= WordClk48LED;
			else
				LEDMirror &= ~ WordClk48LED;
			break;

		case GenlockLED:	
			if ( value)
		 		LEDMirror |= GenlockLED;
			else
				LEDMirror &= ~ GenlockLED;
			break;

		case D37LED:	
			if ( value)
		 		LEDMirror |= D37LED;
			else
				LEDMirror &= ~ D37LED;
			break;
	}

	if ( LEDState == NormLEDs)
		LEDPort = LEDMirror;

	return( OK);
}

/**************************************************************************/
/* TestLEDSet																									   LED_HW.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000321																		*/
/* Revised:		000321, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int TestLEDSet( UI LED, bool value)
{
	switch ( LED)
	{
		case Test1LED:	
			if ( value)
		 		TestLEDMirror |= Test1LED;
			else
				TestLEDMirror &= ~Test1LED;
			break;

		case Test2LED:	
			if ( value)
		 		TestLEDMirror |= Test2LED;
			else
				TestLEDMirror &= ~Test2LED;
			break;
	}

	TestLEDPort = TestLEDMirror;

	return( OK);
}

/**************************************************************************/
/* SystemsLEDSet		 																					   LED_HW.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000618																		*/
/* Revised:		001128, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SystemsLEDSet( UC state, UC column)
{
	UI tmp;

	switch ( LEDState = state)
	{
		case NormLEDs:	
			LEDPort = LEDMirror;			// Back to normal
			break;

		case PresetLEDs:	
			switch ( column)
			{
				case 0:
			 		tmp = 0;
					break;

				case 1:
			 		tmp = ( BB1PALLED + BB1NTSCLED);
					break;

				case 2:
			 		tmp = ( BB2PALLED + BB2NTSCLED);
					break;

				case 3:
			 		tmp = ( BB3PALLED + BB3NTSCLED);
					break;

				case 4:
			 		tmp = ( CBPALLED + CBNTSCLED);
					break;
			}

			LEDPort = tmp;
			break;

		case ErrorLEDs:		
		case ResetLEDs:		
			LEDPort = 0xFFFF;
			break;
	}

	return( OK);
}
