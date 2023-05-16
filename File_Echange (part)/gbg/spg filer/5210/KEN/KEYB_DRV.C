/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		KEYB_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	980428, KEn	DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		UC Update_LEDs();																		*/
/*		bit Button_Ready();																	*/
/*																									*/
/*	Changes:																						*/
/* 980428: Added ButtonCommand = FALSE if no buttons pressed					*/
/* 970224: Reset of AutoESCTimer moved. Only reset when button pressed		*/
/* 970115: Keyb_Scan() renamed to Button_Ready										*/
/* 970115: Keyb_Scan() now returns if new button ready							*/
/* 970112: Update_LEDs() now returns an UC unstead of bit						*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdio.h>
#include <xa.h>

#include "mas.h"

#include "52sync.h"
#include "keyb_drv.h"
#include "iniic.h"
#include "tables.h"
#include "util.h"

UC Button;							// The last pressed button
UC Old_Button;						// The 2nd last pressed button

UC Button_Command;				// The last decoded button-command
UC Old_Button_Command;			// The 2nd last decoded button-command

UC Delay_Fast_Repeat;			//
UC Repeat_Counter;				//
UC Fast_Repeat_Count = Fast_Repeat_Freq;	//

bit Button_Enable;				// 1: Enables handling of pressing the keyboard
bit Horizontal_Repeat;			// 1: Enable horizontal repetetion.
bit Vertical_Repeat;				// 1: Enable vertical repetetion.
bit Repeat_On;						// 1: Repeat function for the pressed button
										//  is ENABLED
bit Combi_On;						// 1: A combination of the pressed button(s)
										//  is/are possible

/***************************************************************************/
/*	Update_LEDs															 	  	 KEYB_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 960709	 											*/
/* Revised:		970114, KEn			     													*/
/*																									*/
/*	Function:	Update the three front plate LEDs									*/
/*	Remarks:		--																				*/
/*	Returns:		0 if the LEDs was updated succesfully								*/
/*					>0 if an error from IIC-circuit.										*/
/*	Updates:		--																				*/
/***************************************************************************/
UC Update_LEDs() {

	UC tmp = 0xFF;

	if ( Warning_LED)						// If WARNINGS .. 
		tmp &= 0xFE;						//  turn on warning LED

	if ( Unlocked_LED)					// If UNLOCKED ..
		tmp &= 0xFD;						//  turn on genlock LED

	if ( Genlock_LED)						// If EXTERNAL GENLOCK ..
		tmp &= 0xFB;						//  turn on genlock LED, (ie. button LED)

	if ( !Wr_Front_IIC( led_addr, tmp))
		return( KeyboardErrorType + ErrorCode);

	return( NoError);
}

/***************************************************************************/
/*	Button_Ready														 	  	 KEYB_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 950509	 											*/
/* Revised:		980428, KEn			     													*/
/*																									*/
/*	Function:	Handle the decoding of the keyboard									*/
/*	Remarks:		--																				*/
/*	Returns:		1: If new button ready													*/
/*	Updates:		TBD																			*/
/***************************************************************************/
bit Button_Ready() {

	feed_dog;							// Watch-dog feed

	if ( !Do_Keyboard_Scan)
		return( FALSE);

	Do_Keyboard_Scan = FALSE;
											// TBD Test for any error condition
	Button = Rd_Front_IIC( keyb_addr) & Keyb_Mask;

	if ( Button_Enable) {
		if ( Repeat_On) {
			if ( Button == Old_Button) {
				if ( !Repeat_Counter) {
					Button_Command = Button;

					if ( Delay_Fast_Repeat) {
						Delay_Fast_Repeat--;
						Repeat_Counter = Normal_Repeat_Freq;
					}
					else
						Repeat_Counter = Fast_Repeat_Count;

					return( TRUE);
				}
				else
					Repeat_Counter--;
			}
			else
				Repeat_On = FALSE;
		} 												// End of if (Repeat_On)
		else {
			if ( Button != NO_Button) {

				if ( Button == Old_Button) {

					AutoESCSignal = OFF;			// reset AUTO ESCape timer, ie. auto-
					AutoESCTimer = 0;				//  matic return to status display

					switch ( Button) {
						case RIGHT_Button:		// RIGHT/LEFT CONDITIONALLY repeat 
						case LEFT_Button:
							if ( Horizontal_Repeat)
								Repeat_On = Button_Enable = TRUE;
							else
								Button_Enable = FALSE;

							Delay_Fast_Repeat = 5;
							Repeat_Counter = Normal_Repeat_Freq;

							Button_Command = Button;
							return( TRUE);

						case UP_Button:			// UP/DOWN CONDITIONALLY repeat
						case DOWN_Button:
							if ( Vertical_Repeat)
								Repeat_On = Button_Enable = TRUE;
							else
								Button_Enable = FALSE;

							Delay_Fast_Repeat = 5;
							Repeat_Counter = Normal_Repeat_Freq;

							Button_Command = Button;
							return( TRUE);

						case EXEC_Button:				// EXECUTE NEVER repeat
						case OUTPUT_Button:			// OUTPUT NEVER repeat
						case PRESET_Button:			// PRESET NEVER repeat
						case GENLOCK_Button:			// GENLOCK NEVER repeat
							Button_Enable = FALSE;

							Button_Command = Button;
							return( TRUE);
					}
				}
				else
					Old_Button = Button;
			}
			else
				Old_Button = NO_Button;
		}
	}
	else 
		if ( Button == NO_Button) {				// If no buttons pressed
			Button_Enable = TRUE;
			Button_Command = NO_Button;
			Old_Button = NO_Button;
			Repeat_On = Combi_On = FALSE;
		}

	return( FALSE);
}
