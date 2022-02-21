/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1CMD.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990304																		*/
/*	Rev. date:	99xxxx, KEn, DEV															*/
/*	Status:		Version 2.2																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void UnitAddressCommand()															*/
/*		void GOffsetCommand()																*/
/*		void MOffsetCommand()																*/
/*		void ReadSystemCommand()															*/
/*		void ReadOffsetCommand() 															*/
/*		void GenlockOffsetCommand()														*/
/*		void PatternCommand()																*/
/*		void EDHCommand()																		*/
/*		void AudioSignalCommand()															*/
/*		void AudioLevelCommand()															*/
/*		void AudioGroupCommand()															*/
/*		void TextCommand()																	*/
/*		void TextONCommand()																	*/
/*		void TextPositionCommand()															*/
/*		void TextStyleCommand()																*/
/*		void ReadStatusCommand()															*/
/*		void GPhaseCommand()																	*/
/*		void MPhaseCommand()																	*/
/*		void ResetCommand()																	*/
/*		void WriteUserTextCommand()														*/
/*		void ReadUserTextCommand()															*/
/*		void ReadSWVersionCommand()														*/
/*		void ReadHWVersionCommand()														*/
/*		void ReadHWInformationCommand()													*/
/*		void CalculateOffset()																*/
/*		void TimeCommand()																	*/
/*		void DateCommand()																	*/
/*																									*/
/*	Changes:																						*/
/* 990319: In ResetCommand(): HWUserPattern & HWInformation is no longer	*/
/*				stored in EEPROM																*/
/* 990304: Update of generator information in ResetCommand()					*/
/* 981109: New parameter in ResetCommand()											*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/* 021113: Modified for TDC moving logo [control_synchro()]                 */
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <string.h>
#include <xa.h>

#include "version.h"

#include "d1gener.h"
#include "d1cmd.h"
#include "d1tables.h"
#include "d1util.h"
#include "rs232.h"
#include "comm.h"

/***************************************************************************/
/*	UnitAddressCommand										 	  	    		D1CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Set/read the unit address.												*/
/*	Remarks:		The broadcast address will also be set, i.e. in S0ADEN. 		*/
/*					NOTE! Setting the command is only allowed in PT8633.			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void UnitAddressCommand() {

	switch ( CmdDelimiter) {
		case ';':
#if PT8633Version
			MyAddress = Parameter[0];
			S0ADDR = MyAddress;				// Receive address
			S0ADEN = 0x74;						//

			WriteChar( MyAddress);			// Transmit address
#else
			ErrorStatus |= Illegal_Cmd;
#endif
			break;

		case '?':
			WriteEndOfTx( WriteValue(( UL) MyAddress));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	GOffsetCommand												 	  	    		 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Set the TV system to SDI625 and/or set phase offset.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GOffsetCommand() {

	if ( CmdDelimiter == ';') {

		if ( TVSystem != SDI625) {
			TVSystem = SDI625;
			CalOffset = IIC_RdOffset( EEPROM_IIC_Adr, IIC_G_Address);

			if ( IICErrorCode) {					// If any errors in IIC..
				ErrorStatus |= IICErrorCode;
				CalOffset = 0;						//  reset timing
			}

			UpdatePattern( PatternNo);			// Update pattern
		}
		UserOffset = Parameter[0];
		CalculateOffset();
	}
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	MOffsetCommand													 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Set the TV system to SDI525 and/or set phase offset.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void MOffsetCommand() {

	if ( CmdDelimiter == ';') {

		if ( TVSystem != SDI525) {
			TVSystem = SDI525;
			CalOffset = IIC_RdOffset( EEPROM_IIC_Adr, IIC_M_Address);

			if ( IICErrorCode) {					// If any errors in IIC..
				ErrorStatus |= IICErrorCode;
				CalOffset = 0;						//  reset timing
			}

			UpdatePattern( PatternNo);			// Update pattern
		}
		UserOffset = Parameter[0];
		CalculateOffset();
	}
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	ReadSystemCommand												 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Read the current system.												*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadSystemCommand() {

	if ( CmdDelimiter == '?')
		WriteEndOfTx( WriteValue((UL) TVSystem));
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	ReadOffsetCommand												 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Read the user offset.													*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadOffsetCommand() {

	if ( CmdDelimiter == '?') 
		WriteEndOfTx( WriteValue( UserOffset));
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	GenlockOffsetCommand											 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980513, KEn			     													*/
/*																									*/
/*	Function:	Set/read the genlock offset.											*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GenlockOffsetCommand() {

	switch ( CmdDelimiter) {
		case ';':
			GenlockOffset = Parameter[0];

			CalculateOffset();
			break;

		case '?':
			WriteEndOfTx( WriteValue( GenlockOffset));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	PatternCommand													 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980513, KEn			     													*/
/*																									*/
/*	Function:	Set/read the output pattern.											*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void PatternCommand() {

	UC tmp;

	switch ( CmdDelimiter) {
		case ';':
			if ( ParamCnt == 2)
				PatternAttr = (UC) Parameter[1];

			UpdatePattern((UC) Parameter[0]);
			break;

		case '?':
			tmp = WriteValue((UL) PatternNo);

			WriteChar( ',');
			WriteEndOfTx( tmp + ',' + WriteValue((UL) PatternAttr));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	EDHCommand											 	  	   			 	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Set/read the EDH insertion.											*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void EDHCommand() {

	switch ( CmdDelimiter) {
		case ';':
			if ( EDHInsertion = (bit) Parameter[0])
				Status &= ~EDHMask;
			else
				Status |= EDHMask;
			break;

		case '?':
			WriteEndOfTx( WriteValue((UL) EDHInsertion));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	AudioSignalCommand											 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Set/read the embedded audio signal.									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AudioSignalCommand() 
  {
	switch (CmdDelimiter)
    {
		case ';':
			AudioSignal = (UC) Parameter[0];
			if ( AudioSignal == AudioSignalOff)
				AudioEnable = OFF;			// Disable audio
			else
        {
				CopyAudioRAM();				// Download audio pattern to RAM
				AudioEnable = ON;				// Enable audio
			  }
      //control_synchro(); // set BBC signal accordingly
			break;

		case '?':
			WriteEndOfTx( WriteValue((UL) AudioSignal));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	  }
  }

/***************************************************************************/
/*	AudioLevelCommand												 	 			 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Set/read the embedded audio level.									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AudioLevelCommand()
  {
	switch ( CmdDelimiter)
    {
		case ';':
			AudioLevel = (UC) Parameter[0];

			if ( AudioEnable == ON)
				CopyAudioRAM();
      //control_synchro(); // set BBC signal accordingly
			break;

		case '?':
			WriteEndOfTx( WriteValue((UL) AudioLevel));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	  }
  }

/***************************************************************************/
/*	AudioGroupCommand											 		  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980216	 											*/
/* Revised:		980513, KEn			     													*/
/*																									*/
/*	Function:	Set/read the embedded audio group.									*/
/*	Remarks:		This command does ONLY apply to the PT8633.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AudioGroupCommand()
  {
#if PT8633Version
	switch ( CmdDelimiter)
    {
		case ';':
			if ( Parameter[0] < 4)
        {
				AudioGroup = (UC) Parameter[0];

				if ( AudioEnable == ON)
					CopyAudioRAM();
			  }
      //control_synchro(); // set BBC signal accordingly
			break;

		case '?':
			WriteEndOfTx( WriteValue((UL) AudioGroup));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	  }
#else
	ErrorStatus |= Illegal_Cmd;
#endif
  }

/***************************************************************************/
/*	TextCommand												 	  	    			 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980513, KEn			     													*/
/*																									*/
/*	Function:	Insert/read a text string.												*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TextCommand() {

	UC tmp, i, j;
	char tmpChar;

	switch ( CmdDelimiter) {
		case ';':
			if (( ParamCnt == 1) && (( tmp = (UC) Parameter[0]) < 4)) {

				tmp--;								// OSD Index is LineNo - 1;

				i = j = 0;
				while ( tmpChar = String[i++]) {
					if ( tmpChar == 0x0E)
						tmpChar = String[i++] + 128;

					OSDText[tmp][j++] = tmpChar;
				}
				OSDText[tmp][j] = tmpChar;
				
				tmp++;								// LineNo is OSD Index + 1;

				if ( TextStyle == StandardTextStyle) {
					if ( tmp == 1)
						XPosLine1 = 92 - stringlen( OSDText[0]);
					else						
						if ( tmp == 2)
							XPosLine2 = 92 - stringlen( OSDText[1]);
					else
						XPosLine3 = 92 - stringlen( OSDText[2]);
				}

				WriteOSDText( tmp);
			}
			else
				ErrorStatus |= Illegal_Par;
			break;

		case '?':
			if (( ParamCnt == 1)  && (( tmp = (UC) Parameter[0]) < 4))
				WriteEndOfTx( WriteString( OSDText[tmp-1]));
			else
				ErrorStatus |= Illegal_Par;
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	TextONCommand													 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980513, KEn			     													*/
/*																									*/
/*	Function:	Enable/disable display of OSD textlines.							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TextONCommand() {

	UC tmp;

	switch ( CmdDelimiter) {
		case ';':
			if (( ParamCnt == 2) && (( tmp = ( UC) Parameter[0]) < 4)) {

				if ( tmp == 1)
					TextLN1Enable = (bit) Parameter[1];
				else
					if ( tmp == 2)
						TextLN2Enable = (bit) Parameter[1];
				else
					TextLN3Enable = (bit) Parameter[1];

				UpdateTextEnable();
			}
			else
				ErrorStatus |= Illegal_Par;
			break;

		case '?':
			if (( ParamCnt == 1) && (( tmp = (UC) Parameter[0]) < 4)) {
				if ( tmp == 1)
					WriteEndOfTx( WriteValue((UL) TextLN1Enable));
				else
					if ( tmp == 2)
						WriteEndOfTx( WriteValue((UL) TextLN2Enable));
				else
					WriteEndOfTx( WriteValue((UL) TextLN3Enable));
			}
			else
				ErrorStatus |= Illegal_Par;
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	TextPositionCommand											 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980513, KEn			     													*/
/*																									*/
/*	Function:	Set/read the (x,y) text position.									*/
/*	Remarks:		This command should only be used in free text style.			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TextPositionCommand() {

	UC tmp;

	switch ( CmdDelimiter) {
		case ';':
			if ( ParamCnt == 3)
				switch ( (UC) Parameter[0]) {
					case 1:
						XPosLine1 = ( UI) Parameter[1];
						YPosLine1 = ( UI) Parameter[2];

						WriteOSDText( 1);
						break;

					case 2:
						XPosLine2 = ( UI) Parameter[1];
						YPosLine2 = ( UI) Parameter[2];

						WriteOSDText( 2);
						break;

					case 3:
						XPosLine3 = ( UI) Parameter[1];
						YPosLine3 = ( UI) Parameter[2];

						WriteOSDText( 3);
						break;

					default:
						ErrorStatus |= Illegal_Par;
						break;
				}
			else
				ErrorStatus |= Illegal_Par;
			break;

		case '?':
			if ( ParamCnt == 1)
				switch ( (UC) Parameter[0]) {
					case 1:
						tmp = WriteValue((UI) XPosLine1);
						WriteChar( ',');
						WriteEndOfTx( ',' + tmp +  WriteValue((UI) YPosLine1));
						break;

					case 2:
						tmp = WriteValue((UI) XPosLine2);
						WriteChar( ',');
						WriteEndOfTx( ',' + tmp +  WriteValue((UI) YPosLine2));
						break;

					case 3:
						tmp = WriteValue((UI) XPosLine3);
						WriteChar( ',');
						WriteEndOfTx( ',' + tmp +  WriteValue((UI) YPosLine3));
						break;

					default:
						ErrorStatus |= Illegal_Par;
						break;
				}
			else
				ErrorStatus |= Illegal_Par;
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	TextStyleCommand												 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980227	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	Set the text-style and/or the date/time on/off					*/
/*	Remarks:		ClockStyle should be tested BEFORE TextStyle because a		*/
/*					change in the clock setting will result in a pattern update,*/
/*					whereas a change in the TextStyle typical results in a		*/
/*					change of text positions.												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TextStyleCommand() {

	UC tmpClockStyle, tmpTextStyle;

	switch ( CmdDelimiter) {
		case ';':
			tmpTextStyle = (UC) Parameter[0];

			tmpClockStyle = tmpTextStyle >> 4;
			tmpTextStyle &= 0x0F;

			if ( ClockStyle != tmpClockStyle) {
				ClockStyle = tmpClockStyle;
				ClockEnable = (bit) tmpClockStyle;

				UpdatePattern( PatternNo);
			}

			if ( TextStyle != tmpTextStyle) {
				TextStyle = tmpTextStyle;

				UpdateTextStyle();
			}
			break;

		case '?':
			WriteEndOfTx( WriteValue((UL) ( TextStyle + ( ClockStyle << 4))));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	ReadStatusCommand												 	  	    	 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980506, KEn			     													*/
/*																									*/
/*	Function:	Get the error status.													*/
/*	Remarks:		Request only.																*/
/*					ErrorStatus is reset after readout.									*/
/*	Returns:																						*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadStatusCommand() {

	if ( CmdDelimiter == '?') {
		WriteEndOfTx( WriteValue( ErrorStatus));
		ErrorStatus = 0;
	}
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	GPhaseCommand												 	  	    		 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		970429, KEn			     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:		--																				*/
/***************************************************************************/
void GPhaseCommand() {

	UL tmp;

	switch ( CmdDelimiter) {
		case '$':
			if ( IIC_WrOffset( EEPROM_IIC_Adr, IIC_G_Address, UserOffset)) {
				CalOffset = UserOffset;
				UserOffset = 0;
			}
			else
				ErrorStatus |= IICErrorCode;
			break;

		case '?':
			tmp = IIC_RdOffset( EEPROM_IIC_Adr, IIC_G_Address);
			if ( IICErrorCode) {
				ErrorStatus |= IICErrorCode;
				tmp = 0;
			}

			WriteEndOfTx( WriteValue( tmp));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	MPhaseCommand												 	  	    		 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		970429, KEn			     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:		--																				*/
/***************************************************************************/
void MPhaseCommand() {

	UL tmp;
	
	switch ( CmdDelimiter) {
		case '$':
			if ( IIC_WrOffset( EEPROM_IIC_Adr, IIC_M_Address, UserOffset)) {
				CalOffset = UserOffset;
				UserOffset = 0;
			}
			else
				ErrorStatus |= IICErrorCode;
			break;

		case '?':
			tmp = IIC_RdOffset( EEPROM_IIC_Adr, IIC_M_Address);
			if ( IICErrorCode) {
				ErrorStatus |= IICErrorCode;
				tmp = 0;
			}

			WriteEndOfTx( WriteValue( tmp));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	ResetCommand											 	  	    			 D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		990319, KEn			     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:		--																				*/
/***************************************************************************/
void ResetCommand() {

	UC i;

	if ( CmdDelimiter == ';') {
		switch ( ParamCnt) {
			case 1:
				if ((UC) Parameter[0] == 1) {
					GeneratorRunning = TRUE;

					IIC_Write2( EEPROM_IIC_Adr, PT_HWType, HWType);
					IIC_Write2( EEPROM_IIC_Adr, PT_HWVersion, HWVersion);
					break;
				}

				if ((UC) Parameter[0] != 0)
					break;
													// Break IS missing!!!
			case 0:
				GeneratorRunning = TRUE;

				UserOffset = GenlockOffset = CalOffset = 0;

				IIC_Write2( EEPROM_IIC_Adr, PT_HWType, HWType);
				IIC_Write2( EEPROM_IIC_Adr, PT_HWVersion, HWVersion);

				if ( !IIC_WrOffset( EEPROM_IIC_Adr, IIC_G_Address, UserOffset))
					ErrorStatus |= IICErrorCode;

				if ( !IIC_WrOffset( EEPROM_IIC_Adr, IIC_M_Address, UserOffset))
					ErrorStatus |= IICErrorCode;
		
				for ( i = 0; i < 10; i++) 
					IIC_WrStr( EEPROM_IIC_Adr, IIC_UserTxt+( 11*i), "\0\0\0\0\0\0\0\0\0\0\0");
				break;
		}
	}
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	WriteUserTextCommand												 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		970217, KEn			     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:		--																				*/
/***************************************************************************/
void WriteUserTextCommand() {

	UC no;

	if ( CmdDelimiter == ';') {
		if (( no = (UC) Parameter[0]) < 10)
			if ( !IIC_WrStr( EEPROM_IIC_Adr, IIC_UserTxt+( 11*no), String))
				ErrorStatus |= IICErrorCode;
	}
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	ReadUserTextCommand												 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/* Function:	Read m, (<11), bytes of user text from IIC area n, (n: 0-9)	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadUserTextCommand() {

	UC no;
	char str[11];

	if ( CmdDelimiter == '?') {
		if (( no = (UC) Parameter[0]) < 10) {
			if ( IIC_RdStr( EEPROM_IIC_Adr, IIC_UserTxt+( 11*no), str))

				WriteEndOfTx( WriteString( str));
			else
				ErrorStatus |= IICErrorCode;
		}
	}
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	ReadSWVersionCommand												 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadSWVersionCommand() {

	switch ( CmdDelimiter) {
		case ';':						// Dummy command
			break;

		case '?':						// Request
			WriteEndOfTx( WriteValue( SWVersion));
			break;

		default:
			ErrorStatus |= Illegal_Del;
			break;
	}
}

/***************************************************************************/
/*	ReadHWVersionCommand												 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		990304, KEn			     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadHWVersionCommand() {
	
	if ( CmdDelimiter == '?')
		WriteEndOfTx( WriteValue((UL) ( HWVersion*256 + HWType)));
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	ReadHWInformationCommand										 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971120	 											*/
/* Revised:		990304, KEn			     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void ReadHWInformationCommand() {
	
	if ( CmdDelimiter == '?')
		WriteEndOfTx( WriteValue((UL) HWInformation*256 + HWUserPattern));
	else
		ErrorStatus |= Illegal_Del;
}

/***************************************************************************/
/*	CalculateOffset													 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980507, KEn			     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void CalculateOffset() {

	UL tmpOffset;

	tmpOffset = UserOffset + GenlockOffset + CalOffset;

	if ( TVSystem == SDI625) {

		tmpOffset %= 276480000;
		EA = 0;								// Disable interrupt while updating offset

												// Lineoffset must be converted to counter
		LineOffset = 0xFFFF - ( tmpOffset / 442368);
		PhaseOffset = 1727 - (( tmpOffset % 442368) / 256);
	}
	else {
		tmpOffset %= 230630400;

		EA = 0;								// Disable interrupt while updating offset
												// Lineoffset must be converted to counter
		LineOffset = 0xFFFF - ( tmpOffset / 439296);
		PhaseOffset = 1715 - (( tmpOffset % 439296) / 256);

		if ( PhaseOffset > 1687)
			PhaseOffset += 12;
	}

	EA = 1;
}

/***************************************************************************/
/*	TimeCommand															 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980506	 											*/
/* Revised:		980521, KEn			     													*/
/*																									*/
/*	Function:	Set/read the time text field											*/
/*	Remarks:		This function is optimized for speed and do not follow the	*/
/*					normal syntax check.														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeCommand() {

	if ( CmdDelimiter == '?')
		WriteEndOfTx( WriteString( TimeText));
	else {
		TimeText[0] = String[0];
		TimeText[1] = String[1];
		TimeText[3] = String[2];
		TimeText[4] = String[3];
		TimeText[6] = String[4];
		TimeText[7] = String[5];
			
		if ( ClockEnable)
			if (( ClockStyle == TextClockTime) || ( ClockStyle == TextClockTimeDate))
				InsertOSDTime();
	}
}

/***************************************************************************/
/*	DateCommand															 	  	    D1CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980506	 											*/
/* Revised:		980521, KEn			     													*/
/*																									*/
/*	Function:	Set/read the date text field											*/
/*	Remarks:		This function is optimized for speed and do not follow the	*/
/*					normal syntax check.														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DateCommand() {

	if ( CmdDelimiter == '?')
		WriteEndOfTx( WriteString( DateText));
	else {
		DateText[0] = String[0];
		DateText[1] = String[1];
		DateText[3] = String[2];
		DateText[4] = String[3];
		DateText[6] = String[4];
		DateText[7] = String[5];
			
		if ( ClockEnable)
			if (( ClockStyle == TextClockDate) || ( ClockStyle == TextClockTimeDate))
				InsertOSDDate();
	}
}

