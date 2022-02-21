/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		RS232CMD.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990917, KEn, DEV															*/
/*	Status:		Version 1.0 																*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void SystemIDN( void) 																*/
/*		void SystemError( void)																*/
/*		void HWInfo( void)																	*/
/*		void HWTest( void)																	*/
/*		void FlashReset( void)																*/
/*		void TKCalibrate( void)																*/
/*		void FlashInfo( void)																*/
/*		void FlashStatus( void)																*/
/*		void FlashChecksum( void)															*/
/*		void FlashSectorSize( void)														*/
/*		void FlashSectorProtectVerify( void)											*/
/*		void FlashSectorErase( void)														*/
/*		void FlashBankErase( void)															*/
/*		void FlashRead( void)																*/
/*		byte FindParameter( char* par, _rom struct PAR_STRUCT* TmpPtr)			*/
/*																									*/
/*	Changes:																						*/
/*																									*/
/***************************************************************************/
#include <stdlib.h>

#include "system.h"
#include "main.h"
#include "util.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "rs232par.h"
#include "rs232err.h"
#include "rs232cmd.h"
#include "flash.h"
#include "test.h"

#if BootProgram
_rom char SWTypeTxt[]					= "PT5201 BOOT_PROGRAM(C)";
#else
_rom char SWTypeTxt[]					= "PT5201 BOOT_LOADER(C)";
#endif

_rom char CompanyTxt[]					= "PTV";

_rom char SWDateTxt[] 					= "14032000";
_rom char SWVersionTxt[] 				= "01.1";

_rom char PLDNotLoadedTxt[]			= "PLD NOT LOADED";
_rom char TestPLDNotAvailableTxt[]	= "TEST PLD NOT AVAILABLE";
_rom char CmdNotAvailableTxt[]		= "COMMAND NOT AVAILABLE";

_rom char ErasingSectorTxt[] 			= "ERASING SECTOR";
_rom char ErasedTxt[] 					= "ERASED";

_rom char NewlineTxt[] 					= "\r\n";

_rom char OKTxt[] 						= "OK";
_rom char ERRORTxt[] 					= "ERROR";

_rom char IDNCmdTxt[] 					= "IDN";
_rom char ERRORCmdTxt[] 				= "ERROR";
_rom char ERRCmdTxt[] 					= "ERR";
_rom char HWINFOCmdTxt[] 				= "HWINFO";
_rom char HWINCmdTxt[] 					= "HWIN";
_rom char HWTESTCmdTxt[] 				= "HWTEST";
_rom char HWTCmdTxt[] 					= "HWT";
_rom char TKCALIBRATECmdTxt[] 	  	= "TKCALIBRATE";
_rom char TKCCmdTxt[] 				  	= "TKC";
_rom char FRESETCmdTxt[] 				= "FRESET";
_rom char FRESCmdTxt[] 					= "FRES";
_rom char FINFOCmdTxt[] 				= "FINFO";
_rom char FINFCmdTxt[] 					= "FINF";
_rom char FSTATUSCmdTxt[] 				= "FSTATUS";
_rom char FSTCmdTxt[] 					= "FST";
_rom char FCHECKSUMCmdTxt[] 	  		= "FCHECKSUM";
_rom char FCHCmdTxt[] 					= "FCH";
_rom char FSSIZECmdTxt[]  				= "FSSIZE";
_rom char FSSCmdTxt[] 			  		= "FSS";
_rom char FSPVERIFYCmdTxt[]  			= "FSPVERIFY";
_rom char FSPVCmdTxt[] 			  		= "FSPV";
_rom char FSERASECmdTxt[] 				= "FSERASE";
_rom char FSERCmdTxt[] 					= "FSER";
_rom char FBERASECmdTxt[] 				= "FBERASE";
_rom char FBERCmdTxt[] 					= "FBER";
_rom char FREADCmdTxt[] 				= "FREAD";
_rom char FRECmdTxt[] 					= "FRE";

_rom char ExtLinAddrRecTxt[]			= ":02000004";
_rom char EndOfFileRecTxt[]			= ":00000001FF";

_rom struct CMD_STRUCT Subsystem[14] = {
	{ IDNCmdTxt, IDNCmdTxt, &Subsystem[1], SystemIDN},
	{ ERRORCmdTxt, ERRCmdTxt, &Subsystem[2], SystemError},
	{ HWINFOCmdTxt, HWINCmdTxt, &Subsystem[3], HWInfo},
	{ HWTESTCmdTxt, HWTCmdTxt, &Subsystem[4], HWTest},
	{ FRESETCmdTxt, FRESCmdTxt, &Subsystem[5], FlashReset},
	{ TKCALIBRATECmdTxt, TKCCmdTxt, &Subsystem[6], TKCalibrate},
	{ FINFOCmdTxt, FINFCmdTxt, &Subsystem[7], FlashInfo},
	{ FSTATUSCmdTxt, FSTCmdTxt, &Subsystem[8], FlashStatus},
	{ FCHECKSUMCmdTxt, FCHCmdTxt, &Subsystem[9], FlashChecksum},
	{ FSSIZECmdTxt, FSSCmdTxt, &Subsystem[10], FlashSectorSize},
	{ FSPVERIFYCmdTxt, FSPVCmdTxt, &Subsystem[11], FlashSectorProtectVerify},
	{ FSERASECmdTxt, FSERCmdTxt, &Subsystem[12], FlashSectorErase},
	{ FBERASECmdTxt, FBERCmdTxt, &Subsystem[13], FlashBankErase},
	{ FREADCmdTxt, FRECmdTxt, NULL, FlashRead},
};

/***************************************************************************/
// Temporary variables PRIVATE NOT overlayable

static _near byte ParState;
static _near byte byteVal;
static _near dword dwordVal1;
static _near dword dwordVal2;

static _near char strout[8];

/***************************************************************************/
/*	SystemIDN															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the version of the boot program.							*/
/*	Remarks:		No parameters, query only												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SystemIDN( void) {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {

			Udr_CPutNChar( CompanyTxt);
			Udr_PutChar( ',');

			Udr_CPutNChar( SWTypeTxt);
			Udr_PutChar( ',');

			Udr_CPutNChar( SWDateTxt);
			Udr_PutChar( ',');

			Udr_CPutNChar( SWVersionTxt);
			Udr_CPutNChar( NewlineTxt);
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemError																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the next entry in the error queue (FIFO). 	 			*/
/*	Remarks:		No parameters, query only												*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemError( void) {

	word i, j;

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if (( i = ReadErrorEventQueue()) >= 0) {

				j = -ErrorTxt_Array[i].ErrorNo;

				dwordtoa((dword) j, strout);

				if ( j)
					Udr_PutChar( '-');

				Udr_PutNChar( strout);

				Udr_PutChar( ',');
				Udr_PutChar( '"');

				Udr_CPutNChar( ErrorTxt_Array[i].ErrorTxt);

				Udr_PutChar( '"');
				Udr_CPutNChar( NewlineTxt);
			}
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	HWInfo																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990716	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the PLD and/or other hardware information.				*/
/*	Remarks:		No parameters, Query only												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void HWInfo( void) {

/*	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {

			if ( !PLDInitialised)
				Udr_CPutNChar( PLDNotLoadedTxt);
			else {
				dwordtoa((dword) PLDSWCodeNumber, strout);
				Udr_PutNChar( strout);

				Udr_PutChar( ',');

				dwordtoa((dword) PLDSWRevision, strout);
				Udr_PutNChar( strout);
			}
			Udr_CPutNChar( NewlineTxt);
		}
		else
			SCPIError = SyntaxErr;
	}
*/
	SCPIError = SyntaxErr;
}

/***************************************************************************/
/*	HWTest																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990716	    										*/
/* Revised:		990913				     													*/
/*																									*/
/*	Function:	Start hardware test program.											*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					Query only.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void HWTest( void) {

/*	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

			if (( atoi( Cmd) == 0) || ( atoi( Cmd) > maxHWTest) )
				SCPIError = DataOutOfRange;
			else
				byteVal = (byte) atoi( Cmd);
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( !ParState)
				SCPIError = MissingPar;
			else {
			#if BootProgram
				Udr_CPutNChar( CmdNotAvailableTxt);
			#else
				if ( !PLDInitialised)
					Udr_CPutNChar( PLDNotLoadedTxt);
				else {
					if ( PLDInitDonePort != 0x5430)
						Udr_CPutNChar( TestPLDNotAvailableTxt);
					else {
						switch ( byteVal) {
							case 1:
								Udr_PutChar( '0' + user_memory_test());
								break;

							case 2:
								Udr_PutChar( '0' + timekeeper_memory_test());
								break;

							case 3:
								Udr_PutChar( '0' + HWmonitor_test());
								break;

							case 4:
								Udr_PutChar( '0' + ioport_test());
								break;

							case 5:
								Udr_PutChar( '0' + rs232_test());
								break;

							case 6:
								Udr_PutChar( '0' + parallelport_test());
								break;

							case 7:
								Udr_PutChar( '0' + PLD_test());
								break;
						}
					}
				}
			#endif
				Udr_CPutNChar( NewlineTxt);					 
			}
		}
		else
			SCPIError = SyntaxErr;
	}
*/
	SCPIError = SyntaxErr;
}

/***************************************************************************/
/*	FlashReset															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Reset the Flash EPROM device.											*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FlashReset( void) {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else
			flash_reset();
	}
}

/***************************************************************************/
/*	TKCalibrate															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990904	    										*/
/* Revised:		990917				     													*/
/*																									*/
/*	Function:	Calibrate the onboard time keeper.							  		*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum,DecNum												*/
/*					ParState 2: DecNum														*/
/*					NOTE!!!! There seems to be a problem regarding the M48T35 in*/
/*					calibration mode. After the FT bit has been set, the minute	*/
/*					the seconds register is selected, the M48T35 will take over	*/
/*					the bus, WITHOUT the read strobe being active, creating a	*/
/*			 		conflict on the multiplexed address/data bus, (bit 0 will	*/
/*					be forced to ZERO).														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TKCalibrate( void) {

/*	volatile _huge word *ptr;
	word tmp;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			switch ( ParState) {
				case 0:
					ParState++;

					if (( atol( Cmd) == 0) || ( atol( Cmd) > 2))
						SCPIError = DataOutOfRange;
					else
						byteVal = atol( Cmd);
					break;

				case 1:
					ParState++;

					if ( atol( Cmd) > 63)
						SCPIError = DataOutOfRange;
					else
						dwordVal2 = atol( Cmd);
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 0) {
			#if BootProgram
				Udr_CPutNChar( CmdNotAvailableTxt);
				Udr_CPutNChar( NewlineTxt);
			#else	
				if ( !PLDInitialised)
					Udr_CPutNChar( PLDNotLoadedTxt);
				else {
													// Select the calibration register
					ptr = ( _huge word *) ( 0x8FFF0);
					dwordtoa((dword) ( *ptr & 0x3F), strout);

					Udr_PutNChar( strout);
					Udr_CPutNChar( NewlineTxt);
				}
			#endif
			}
			else
				SCPIError = ParNotAllowed;
		}
		else {
		#if BootProgram
			Udr_CPutNChar( CmdNotAvailableTxt);
			Udr_CPutNChar( NewlineTxt);
		#else	
			if ( !PLDInitialised) {
				Udr_CPutNChar( PLDNotLoadedTxt);
				Udr_CPutNChar( NewlineTxt);
			}
			else {
				if ( ParState == 2) {
					if ( byteVal == 1) {
														// Select the day register
						ptr = ( _huge word *) ( 0x8FFF8);
						*ptr &= 0x00BF;			// Clear the FT bit to enable calibration.

														// Select the calibration register
						ptr = ( _huge word *) ( 0x8FFF0);
						*ptr = 0xC0 + dwordVal2;// Write the calibration value, W=1
						*ptr = 0x40 + dwordVal2;// Write the calibration value, W=0
					}
					else {
						if ( PLDInitDonePort != 0x5430)
							Udr_CPutNChar( TestPLDNotAvailableTxt);
						else {
							if ( !dwordVal2) {
														// Select the seconds register
								ptr = ( _huge word *) ( 0x8FFF2);
								*ptr &= 0x007F;  	// Start the timekeeper, (both M48T35 & M48T37)
	
								TKCalibratePort = 1;
														// Select the day register
								ptr = ( _huge word *) ( 0x8FFF8);
								*ptr |= 0x0040;	// Set the FT bit to enable calibration.

								ptr = ( _huge word *) ( 0x8FFF2);
								tmp = *ptr;		 	// Read the seconds register to activate calibration
							}
							else
								SCPIError = DataOutOfRange;
						}
					}
				}
				else
					SCPIError = MissingPar;
			}
		#endif
		}
	}
*/
	SCPIError = SyntaxErr;
}

/***************************************************************************/
/*	FlashInfo															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the Flash EPROM device information.						*/
/*	Remarks:		No parameters, Query only												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FlashInfo( void) {

	word DeviceID;
	byte manufacturer;

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
													// Get information from writable bank
			flash_get_device_info( WritableSector, &manufacturer, &DeviceID);

			dwordtoa((dword) manufacturer, strout);  	// Manufacturer

			Udr_PutNChar( strout);
			Udr_PutChar( ',');

			dwordtoa((dword) DeviceID, strout);  	  	// Device ID

			Udr_PutNChar( strout);
			Udr_PutChar( ',');
																	// Number of sectors
			dwordtoa((dword) flash_get_numsectors(), strout);

			Udr_PutNChar( strout);
			Udr_CPutNChar( NewlineTxt);
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	FlashStatus															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the status of the Flash EPROM device.						*/
/*	Remarks:		No parameters, query only												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FlashStatus( void) {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {											  
															// Get status from writable bank
			switch ( flash_get_status( WritableSector)) {
				case STATUS_READY: 
					Udr_CPutNChar( _FL_STATUSREADYTxt);
					break;

				case STATUS_BUSY: 
					Udr_CPutNChar( _FL_STATUSBUSYTxt);
					break;

				case STATUS_ERSUSP: 
					Udr_CPutNChar( _FL_STATUSERSUSPTxt);
					break;

				case STATUS_TIMEOUT: 
					Udr_CPutNChar( _FL_STATUSTIMEOUTTxt);
					break;

				case STATUS_ERROR: 
					Udr_CPutNChar( _FL_STATUSERRORTxt);
					break;
			}
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	FlashChecksum																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the checksum of an address range or a sector in the	*/
/*					Flash EPROM device.														*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum,DecNum												*/
/*					Query only.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void FlashChecksum( void) {

/*	word tmp, checksum;
	dword i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			switch ( ParState) {
				case 0:
					ParState++;

					if ( atol( Cmd) > 0x000FFFFF)
						SCPIError = DataOutOfRange;
					else
						dwordVal1 = atol( Cmd);
					break;

				case 1:
					ParState++;

					if ( atol( Cmd) > 0x000FFFFF)
						SCPIError = DataOutOfRange;
					else
						dwordVal2 = atol( Cmd);
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( !ParState)
				SCPIError = MissingPar;
			else {
				switch ( ParState) {
					case 1:
						if ( dwordVal1 > 22)
							SCPIError = SyntaxErr;
						else {
							checksum = 0;

							for ( i = meminfo->sec[dwordVal1].base; i < meminfo->sec[dwordVal1+1].base; i += 2) {

							#if BootProgram
								tmp = *(( _rom word *) ( i ^ 0xE0000));
							#else
								tmp = *(( _rom word *) i);
							#endif

								checksum += ( tmp / 0x100) + ( tmp % 0x100);
							}

							dwordtoa((dword) checksum, strout);

							Udr_PutNChar( strout);
							Udr_CPutNChar( NewlineTxt);
						}
						break;

					case 2:
						if ( dwordVal2 < dwordVal1)
							SCPIError = SyntaxErr;
						else {
							checksum = 0;

							for ( i = dwordVal1; i < dwordVal2; i += 2) {

							#if BootProgram
								tmp = *(( _rom word *) ( i ^0xE0000));
							#else
								tmp = *(( _rom word *) i);
							#endif

								checksum += ( tmp / 0x100) + ( tmp % 0x100);
							}
													
							dwordtoa((dword) checksum, strout);

							Udr_PutNChar( strout);
							Udr_CPutNChar( NewlineTxt);
						}
						break;

					default:
						SCPIError = SyntaxErr;
						break;
				}
			}
		}
		else
			SCPIError = SyntaxErr;
	}
*/
			SCPIError = SyntaxErr;

}

/***************************************************************************/
/*	FlashSectorSize															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the size of a sector in the Flash EPROM device.	 	*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					Query only.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void FlashSectorSize( void) {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				if ( atoi( Cmd) > meminfo->nsect)
					SCPIError = DataOutOfRange;
				else
					byteVal = (byte) atoi( Cmd);
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( !ParState)
				SCPIError = MissingPar;
			else {
				dwordtoa((dword) flash_get_sector_size( byteVal), strout);

				Udr_PutNChar( strout);
				Udr_CPutNChar( NewlineTxt);
			}
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	FlashSectorProtectVerify												 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 990715	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Return the protect information of a sector in the Flash		*/
/*					EPROM device. 																*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					No query.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void FlashSectorProtectVerify( void) {

/*	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

			#if BootProgram
				if ( atol( Cmd) >= meminfo->bank1start)
					SCPIError = DataOutOfRange;
				else
					byteVal = (byte) atoi( Cmd);
			#else
				if ( atol( Cmd) < meminfo->bank1start)
					SCPIError = DataOutOfRange;
				else
					byteVal = (byte) atoi( Cmd);
			#endif
			}
			else
				SCPIError = ParNotAllowed;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( !ParState)
				SCPIError = MissingPar;
			else {
				if ( flash_sector_protect_verify( byteVal))
					Udr_CPutNChar( _FL_OFFTxt);
				else
					Udr_CPutNChar( _FL_ONTxt);
			}
		}
		else {
			SCPIError = SyntaxErr;
		}
	}
*/
	SCPIError = SyntaxErr;
}

/***************************************************************************/
/*	FlashSectorErase															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Commence erasing a sector in the Flash EPROM device. 			*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					No query.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void FlashSectorErase( void) {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				if ( atol( Cmd) >= meminfo->nsect)
					SCPIError = DataOutOfRange;
				else
					byteVal = (byte) atoi( Cmd);
			}
			else
				SCPIError = ParNotAllowed;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else {
			if ( !ParState)
				SCPIError = MissingPar;
			else {
				Udr_CPutNChar( ErasingSectorTxt);

				Udr_PutChar( '(');

				dwordtoa((dword) byteVal, strout);
				Udr_PutNChar( strout);

				Udr_PutChar( ')');
				Udr_PutChar( ' ');

				flash_sector_erase_int( byteVal);

				Udr_CPutNChar( ErasedTxt);

				Udr_CPutNChar( NewlineTxt);
			}
		}
	}
}

/***************************************************************************/
/*	FlashBankErase																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Commence erasing a bank in the Flash EPROM device. 			*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					No query.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void FlashBankErase( void) {

	byte i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

			#if BootProgram
				if ( atol( Cmd) != 2)
					SCPIError = DataOutOfRange;
				else
					byteVal = (byte) atoi( Cmd);
			#else
				if ( atol( Cmd) != 1)
					SCPIError = DataOutOfRange;
				else
					byteVal = (byte) atoi( Cmd);
			#endif
			}
			else
				SCPIError = ParNotAllowed;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else {
			if ( !ParState)
				SCPIError = MissingPar;
			else {
				Udr_CPutNChar( ErasingSectorTxt);

				for ( i = 0; i < meminfo->nsect; i++) {
					Udr_PutChar( '(');

					dwordtoa((dword) i, strout);
					Udr_PutNChar( strout);

					Udr_PutChar( ')');

					flash_sector_erase_int( i);
				}
				Udr_CPutNChar( ErasedTxt);
				Udr_CPutNChar( NewlineTxt);
			}
		}
	}
}

/***************************************************************************/
/*	FlashRead														 	  	  RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990914				     													*/
/*																									*/
/*	Function:	Return the flash EPROM device content using Intel Hex			*/
/*					Records formatted with 32 bytes in each string.					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum,DecNum												*/
/*					Query only.																	*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void FlashRead( void) {

	dword i;
	word tmp;
	byte bytecnt, checksum;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			switch ( ParState) {
				case 0:
					ParState++;

					if ( atol( Cmd) > 0x000FFFFF)
						SCPIError = DataOutOfRange;
					else
						dwordVal1 = atol( Cmd);
					break;

				case 1:
					ParState++;

					if ( atol( Cmd) > 0x000FFFFF)
						SCPIError = DataOutOfRange;
					else
						dwordVal2 = atol( Cmd);
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( !ParState)
				SCPIError = MissingPar;
			else
				if ( ParState > 2)
					SCPIError = SyntaxErr;
			else {
				if ( dwordVal1 & 0x01)			// Only even addresses are allowed
					dwordVal1--;
				
				if ( ParState == 1)
					dwordVal2 = dwordVal1 + 32;

				if ( dwordVal2 & 0x01) 			// Only even addresses are allowed
					dwordVal2--;

				if ( dwordVal2 < dwordVal1)
					SCPIError = SyntaxErr;
				else {
					Udr_CPutNChar( ExtLinAddrRecTxt);

					tmp = dwordVal1 / 0x10000;

					wordtohex( tmp, strout);
					Udr_PutNChar( strout);		// Address

					bytetohex(((( 6 + tmp) ^ 0x00FF) + 1), strout);
					Udr_PutNChar( strout);		// Checksum

					Udr_CPutNChar( NewlineTxt);

					bytecnt = 0;

					do {
						if ((( dwordVal1 + bytecnt) & 0x10000) != ( dwordVal1 & 0x10000)) {

							Udr_CPutNChar( ExtLinAddrRecTxt);

							tmp = ( dwordVal1 + bytecnt) / 0x10000;

							wordtohex( tmp, strout);
							Udr_PutNChar( strout);		// Address

							bytetohex(((( 6 + tmp) ^ 0x00FF) + 1), strout);

							Udr_PutNChar( strout);		// Checksum
							Udr_CPutNChar( NewlineTxt);
						}

						dwordVal1 += bytecnt;

						if (( dwordVal2 - dwordVal1) > 32)
							bytecnt = 32;
						else
							bytecnt = dwordVal2 - dwordVal1;

						if ((( dwordVal1 + bytecnt) & 0x10000) != ( dwordVal1 & 0x10000))
							bytecnt = (( dwordVal1 + 0x10000) & 0x10000) - dwordVal1;

						Udr_PutChar( ':');	 	// Data Record
						bytetohex( bytecnt, strout);
						Udr_PutNChar( strout);

						tmp = dwordVal1 % 0x10000;

						wordtohex( tmp, strout);
						Udr_PutNChar( strout);	// Address offset
						
						Udr_PutChar( '0'); 		// Record type, (data)
						Udr_PutChar( '0');

						checksum = 0x20 + ( tmp / 0x100) + ( tmp % 0x100);

						for ( i = 0; i < bytecnt; i += 2) {

						#if BootProgram
							tmp = *(( _rom word *) (( dwordVal1 + i) ^ 0x80000));
						#else
							tmp = *(( _rom word *) ( dwordVal1 + i));
						#endif

							wordtohex(( tmp / 0x100) + ( tmp * 0x100), strout);

							checksum += ( tmp / 0x100) + ( tmp % 0x100);

							Udr_PutNChar( strout);
						}
						bytetohex(( checksum ^ 0xFF)+1, strout);
						Udr_PutNChar( strout);
						Udr_CPutNChar( NewlineTxt);
					} while (( dwordVal1 + bytecnt) < dwordVal2);

					Udr_CPutNChar( EndOfFileRecTxt);
					Udr_CPutNChar( NewlineTxt);
				}
			}
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	FindParameter														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990910				     													*/
/*																									*/
/*	Function:	Find the character defined parameter								*/
/*	Remarks:		--																				*/
/*	Returns:		The array number containing the parameter or max. array+1	*/
/*	Updates:		--																				*/
/***************************************************************************/
byte FindParameter( char* par, _rom struct PAR_STRUCT* ParPtr) {

	byte tmp = 0;

	while ( ParPtr) {

		if (( !Cstrcmp( par, ParPtr->LongName)) ||
			 ( !Cstrcmp( par, ParPtr->ShortName)))
			break;

		ParPtr = ParPtr->SameLevel;

		if ( tmp < 255)
			tmp++;
		else
			break;
	}

	return( tmp);
}

