/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		KEYB_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980203																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		UC UpdateLEDs();																		*/
/*		void UpdateGrpLEDs( UC group);													*/
/*		bit ButtonReady();																	*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>

#include "mas.h"

#include "keyb_drv.h"
#include "iic.h"
#include "tables.h"
#include "message.h"

UC Button;							// The last pressed button
UC OldButton;						// The 2nd last pressed button

UC ButtonCommand;					// The last decoded button-command
UC OldButtonCommand;				// The 2nd last decoded button-command

UC DelayFastRepeat;				//
UC RepeatCounter;					//
UC FastRepeatCount = FastRepeatFreq;	//

bit ButtonEnable;					// 1: Enables handling of pressing the keyboard
bit HorizontalRepeat;			// 1: Enable horizontal repetetion.
bit VerticalRepeat;				// 1: Enable vertical repetetion.
bit RepeatOn;						// 1: Repeat function for the pressed button
										//  is ENABLED
bit CombiOn;						// 1: A combination of the pressed button(s)
										//  is/are possible

/***************************************************************************/
/*	UpdateLEDs															 	  	 KEYB_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 960709	 											*/
/* Revised:		980524, KEn			     													*/
/*																									*/
/*	Function:	Update the eight front plate LEDs.									*/
/*	Remarks:		--																				*/
/*	Returns:		System error code, i.e. type incl. IIC specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC UpdateLEDs() {

	UC tmp = 0xFF;

	FeedTheWatchdog;

	if ( CBarLED)							// If C.BAR group ..
		tmp &= 0xDF;						//  turn on c.bar LED

	if ( MBurstLED)						// If M.BURST group ..
		tmp &= 0xEF;						//  turn on m.burst LED

	if ( WindowLED)						// If WINDOW/FLAT group ..
		tmp &= 0xF7;						//  turn on window/flat LED

	if ( SpecialLED)						// If SPECIAL group ..
		tmp &= 0xBF;						//  turn on special LED

	if ( LinearityLED)					// If LINEARITY group ..
		tmp &= 0x7F;						//  turn on c.bar LED

	if ( PatternLED)						// If PATTERN group ..
		tmp &= 0xFB;						//  turn on c.bar LED

	if ( RemoteLED)						// If REMOTE .. 
		tmp &= 0xFE;						//  turn on remote LED

	if ( UnlockedLED)						// If UNLOCKED ..
		tmp &= 0xFD;						//  turn on unlocked LED

	if ( !WrFrontIIC( led_addr, tmp))
		return( KeyboardErrorType + IICErrorCode);

	return( NoIICError);
}

/***************************************************************************/
/*	UpdateGrpLEDs														 	  	 KEYB_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980203	 											*/
/* Revised:		980524, KEn			     													*/
/*																									*/
/*	Function:	Illuminate the group LEDs according to group selected			*/
/*	Remarks:		The group <NoGroup> will switch OFF all the LEDs				*/
/*	Returns:		System error code, i.e. type incl. IIC specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC UpdateGrpLEDs( UC group) {

	UC tmp = 0xFF;

	FeedTheWatchdog;

	CBarLED = OFF;							// Switch OFF all LEDs
	MBurstLED = OFF;
	WindowLED = OFF;
	SpecialLED = OFF;
	LinearityLED = OFF;
	PatternLED = OFF;

	switch ( group) {
		case CBarGroup:
			CBarLED = ON;
			break;

		case MBurstGroup:
			MBurstLED = ON;
			break;

		case WindowGroup:
			WindowLED = ON;
			break;

		case SpecialGroup:
			SpecialLED = ON;
			break;

		case LinearityGroup:
			LinearityLED = ON;
			break;

		case PatternGroup:
			PatternLED = ON;
			break;
	}

	if ( CBarLED)							// If C.BAR group ..
		tmp &= 0xDF;						//  turn on c.bar LED

	if ( MBurstLED)						// If M.BURST group ..
		tmp &= 0xEF;						//  turn on m.burst LED

	if ( WindowLED)						// If WINDOW/FLAT group ..
		tmp &= 0xF7;						//  turn on window/flat LED

	if ( SpecialLED)						// If SPECIAL group ..
		tmp &= 0xBF;						//  turn on special LED

	if ( LinearityLED)					// If LINEARITY group ..
		tmp &= 0x7F;						//  turn on c.bar LED

	if ( PatternLED)						// If PATTERN group ..
		tmp &= 0xFB;						//  turn on c.bar LED

	if ( RemoteLED)						// If REMOTE ..
		tmp &= 0xFE;						//  turn on remote LED

	if ( UnlockedLED)						// If UNLOCKED ..
		tmp &= 0xFD;						//  turn on unlocked LED

	if ( !WrFrontIIC( led_addr, tmp))
		return( KeyboardErrorType + IICErrorCode);

	return ( NoIICError);
}

/***************************************************************************/
/*	ButtonReady															 	  	 KEYB_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 950509	 											*/
/* Revised:		980524, KEn			     													*/
/*																									*/
/*	Function:	Read and decode the keyboard.											*/
/*	Remarks:		--																				*/
/*	Returns:		TRUE: If new button ready												*/
/*	Updates:		TBD																			*
/***************************************************************************/
bit ButtonReady() {

	FeedTheWatchdog;

	if ( !DoKeyboardScan)
		return( FALSE);

	DoKeyboardScan = FALSE;

	Button = RdFrontIIC( keyb_addr);			// Read the keyboard

	if ( IICErrorCode)							// In case of any errors...
		return( FALSE);							//  discard the read value
	
	if ( ButtonEnable) {
		if ( RepeatOn) {
			if ( Button == OldButton) {
				if ( !RepeatCounter) {
					ButtonCommand = Button;

					if ( DelayFastRepeat) {
						DelayFastRepeat--;
						RepeatCounter = NormalRepeatFreq;
					}
					else
						RepeatCounter = FastRepeatCount;

					return( TRUE);
				}
				else
					RepeatCounter--;
			}
			else
				RepeatOn = FALSE;
		} 												// End of if (Repeat_On)
		else {
			if ( Button != NOButton) {

				if ( Button == OldButton) {

					AutoESCSignal = OFF;			// reset AUTO ESCape timer, ie. auto-
					AutoESCTimer = 0;				//  matic return to status display

					switch ( Button) {
						case RIGHTButton:			// RIGHT/LEFT CONDITIONALLY repeat 
						case LEFTButton:
							if ( HorizontalRepeat)
								RepeatOn = ButtonEnable = TRUE;
							else
								ButtonEnable = FALSE;

							DelayFastRepeat = 5;
							RepeatCounter = NormalRepeatFreq;

							ButtonCommand = Button;
							return( TRUE);

						case UPButton:				// UP/DOWN CONDITIONALLY repeat
						case DOWNButton:
							if ( VerticalRepeat)
								RepeatOn = ButtonEnable = TRUE;
							else
								ButtonEnable = FALSE;

							DelayFastRepeat = 5;
							RepeatCounter = NormalRepeatFreq;

							ButtonCommand = Button;
							return( TRUE);

						case EXECButton:				// EXECUTE NEVER repeat

						case CBARButton:				// CBAR NEVER repeat
						case MBURSTButton:			// MBURST NEVER repeat
						case WINDOWButton:			// WINDOW NEVER repeat
						case PRESETButton:			// PRESET NEVER repeat
						case SPECIALButton:			// SPECIAL NEVER repeat
						case LINEARITYButton:		// LINEARITY NEVER repeat
						case PATTERNButton:			// PATTERN NEVER repeat
						case OUTPUTButton:			// OUTPUT NEVER repeat
							ButtonEnable = FALSE;

							ButtonCommand = Button;
							return( TRUE);
					}
				}
				else
					OldButton = Button;
			}
			else
				OldButton = NOButton;
		}
	}
	else 
		if ( Button == NOButton) {				// If no buttons pressed
			ButtonEnable = TRUE;
			ButtonCommand = NOButton;
			OldButton = NOButton;
			RepeatOn = CombiOn = FALSE;
		}

	return( FALSE);
}
