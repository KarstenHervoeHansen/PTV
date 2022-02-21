/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		UNIT_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960713																		*/
/*	Rev. date:	971217, KEn DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		UC ConfigureSPG()																		*/
/*		UC ConfigureAnlBlkUnit( UC no)													*/
/*		UC ConfigureSDIBlkUnit( UC no)													*/
/*		UC ConfigureAnalogSignalUnit()													*/
/*		UC ConfigureSDISignalUnit()														*/
/*		UC AES_EBUUnitDrv( UC Signal, UC Level, UC Timing)							*/
/*		UC GenlockUnitDrv( UC GenlockInput, UC GenlockSystem)						*/
/*		UC TrxAnlBlkSystem( UC index, UC system)										*/
/*		UC TrxAnlBlkDelay( UC index, UL delay)											*/
/*		UC TrxAnlBlkScHPhase( UC index, int ScHPhase)								*/
/*		UC TrxSDIBlkSystem( UC index, UC system)										*/
/*		UC TrxSDIBlkPattern( UC index, UC pattern)									*/
/*		UC TrxSDIBlkDelay( UC index, UL delay)											*/
/*		UC TrxSDIBlkEDH( UC index, UC state)											*/
/*		UC TrxSDIBlkAudio( UC index, UC audio)											*/
/*		UC TrxAnalogPattern( UC pattern)													*/
/*		UC TrxAnalogText( UC state, char* text)										*/
/*		UC TrxAnalogSystem( UC system)													*/
/*		UC TrxAnalogDelay( UL delay)														*/
/*		UC TrxAnalogScHPhase( int ScHPhase)												*/
/*		UC TrxSDIPattern( UC pattern)														*/
/*		UC TrxSDIText( UC state, char* text)											*/
/*		UC TrxSDISystem( UC system)														*/
/*		UC TrxSDIDelay( UL delay)															*/
/*		UC TrxSDIEDH( UC state)																*/
/*		UC TrxSDIAudioSignal( UC audiosignal)											*/
/*		UC TrxSDIAudioLevel( UC audiolevel)												*/
/*		UC TrxGenlockDelay( UL delay)														*/
/*		UC ConfigurePreset( UC No)															*/
/*		void StorePreset( UC No)															*/
/*		void GetV24Version( UC address, UC type, 										*/
/*												struct CONN_STRUCTURE* Connector)		*/
/*		void GetIICVersion( UC bus, UC address, UC KU_address, UL* KU)			*/
/*		UC TestMainboard( void)																*/
/*		UC TestSPGUnit( void)																*/
/*		UC TestAES_EBUUnit( void)															*/
/*		UC TestAnlBlkUnit( UC conn)														*/
/*		UC TestSDIBlkUnit( UC conn)														*/
/*		UC TestAnalogSignalUnit( void)													*/
/*		UC TestSDISignalUnit( void)														*/
/*		UC TestBBMulUnit( void)																*/
/*		UC TestSDIGenlockUnit( void)														*/
/*		UC TestTimecodeUnit( void)															*/
/*		UC TestHighPrecisionOCXUnit( void)												*/
/*																									*/
/*	Changes:																						*/
/* 971217: /906 version. Support for PT8603/905										*/
/* 971127: Added parameter val to RecInt() & RecLong								*/
/*			  No /900 version Analog- or SDI-test signal generator will be 	*/
/*			  recognized. A configuration error will occur, but the menu will	*/
/*			  NOT be available																*/
/* 971118: Alterations in GetV24Version()												*/
/* 971031: Added calls to RecStr()														*/
/* 971029: All test functions now implemented										*/
/* 971028: GenlockUnitDrv() has been rewritten										*/
/* 971027: No more references to SndRequest. Replaced by RecXXX functions	*/
/* 		  Added TestMainboard(), TestSPGUnit()										*/
/* 971024: Added TestAES_EBUUnit()														*/
/* 971023: All requests are replaced by calls to RecInt, RecLong or RecChar*/
/* 		  Added TestAnlBlkUnit, TestSDIBlkUnit, TestAnalogSignalUnit,		*/
/*			   & TestSDISignalUnit															*/
/* 971021: Altered V24Cmd from 4 to 3, 'cause SndRequest automatically		*/
/*				inserts the questionmark.													*/
/* 970917: Added parameter offset to GetIICVersion() and updated calls to	*/
/*				this routine																	*/
/* 970909: In ConfigureSPG(): This routine now gets the SW version and		*/
/*				stores it in its own connector.											*/
/* 970904: In ConfigureSDISignalUnit() & ConfigureAnalogSignalUnit(): The	*/
/*				routines now returns the error Configuration error, if the KU	*/
/*				number and version is 0, ie. it is NOT the correct software		*/
/*				mounted in the unit which is PT8601/02/03								*/
/* 970625: BB 1&2 did not return SW version in ConfigureAnlBlkUnit			*/
/* 970506: ConfigureSDISignal update.													*/
/*         Error in SDI Black/Signal: TrxxxSystem forgot to multiply	with	*/
/*         256																					*/
/*         ConfigureSDIBlack: EDH transmitted twice								*/
/* 970225: Major changes eg. added GetIIC- and GetV24Version					*/
/* 970220: RS232 setup has been removed from preset								*/
/* 970131: Corrected in TrxGenlockDelay: 12 was sent to SDISignal & 			*/
/*			  AnalogSignal instead of the variable delay								*/
/* 970124: Changed sequence in ConfigureSDIBlkUnit, ConfigureSDIUnit &		*/
/*			  ConfigureAnalogUnit.															*/
/*	970120: Changed SndLong & SndInt to SndChar where approoriate				*/
/* 970114: Added in ConfigurePreset: recall of active genlock number			*/
/* 970113: Added function ConfigurePreset( UC no);									*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>

#include <stdlib.h>
#include <string.h>

#include "mas.h"

#include "52sync.h"
#include "serial1.h"
#include "unit_drv.h"
#include "tables.h"
#include "menutree.h"
#include "piic.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "keyb_drv.h"
#include "message.h"

static char V24Cmd[3] = { 0, 0, 0};
static UC error;

/***************************************************************************/
/*	ConfigureSPG																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971023				     													*/
/*																									*/
/*	Function:	Download calibration data to SPG										*/
/*	Remarks:	  																					*/
/*	Returns:		Error code																	*/
/*	Updates:																						*/
/***************************************************************************/
UC ConfigureSPG() {

	UL TmpUL;
	UI TmpUI;

	TmpUL = IIC2_RdLong( spgram_addr, HPHZero_G_addr);
	SndLong( sync_addr, "GV", TmpUL);

	TmpUI = IIC2_RdInt( spgram_addr, GenlPhase_G_addr);
	SndInt( sync_addr, "GF", TmpUI);

	TmpUL = IIC2_RdLong( spgram_addr, HPHZero_M_addr);
	SndLong( sync_addr, "GY", TmpUL);

	TmpUI = IIC2_RdInt( spgram_addr, GenlPhase_M_addr);
	SndInt( sync_addr, "GG", TmpUI);

	GetIICVersion( IICBus2, spgram_addr, SPGUserText_addr + ProductKUNo_addr,\
															 &Connectors[OSCConn].KUNumber);

	TmpUI = RecInt( sync_addr, "GI", 0xFF, &error);	// Get information

	if ( error)
		Connectors[OSCConn].SWVersion = 0;
	else
		Connectors[OSCConn].SWVersion = TmpUI;

	return( 0);
}

/***************************************************************************/
/*	ConfigureAnlBlkUnit														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971118				     													*/
/*																									*/
/*	Function:	Download all parameters to a black burst unit					*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 ErrorType code														*/
/*	Updates:		-																				*/
/***************************************************************************/
UC ConfigureAnlBlkUnit( UC no) {

	UC tmp = BB12Conn;

	switch( no) {
		case BB7Item:
			tmp++;
		case BB5Item:
			tmp++;
		case BB3Item:
			tmp++;
		case BB1Item:
			GetV24Version( AnlBlkUnit[no].Address, PT8608, &Connectors[tmp]);
			break;
	}

	if ( no == BB1Item) {
		Connectors[BB12Conn].HWType = 0;
		Connectors[BB12Conn].KUNumber = Connectors[MainConn].KUNumber;
	}

	return( TrxAnlBlkSystem( no, AnlBlkConfig[no].System));
}

/***************************************************************************/
/*	ConfigureSDIBlkUnit														 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971118				     													*/
/*																									*/
/*	Function:	Download all parameters to a SDI black unit						*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 ErrorType code														*/
/*	Updates:		-																				*/
/***************************************************************************/
UC ConfigureSDIBlkUnit( UC no) {

	UC tmp = BB34Conn;

	switch( no) {
		case SB78Item:
			tmp++;
		case SB56Item:
			tmp++;
		case SB34Item:
			GetV24Version( SDIBlkUnit[no].Address, PT8609, &Connectors[tmp]);
			break;
	}
											// The below sequence of initialization should
											//  NOT be changed due to the fact that the
											//  system update is VERY time consuming in
											//  the unit
	if ( !error) {
		waitms( 5);
		error = TrxSDIBlkEDH( no, SDIBlkConfig[no].EDH);
	}

	if ( !error) {
		waitms( 5);
		error = TrxSDIBlkAudio( no, SDIBlkConfig[no].EMBAudio);
	}

	if ( !error) {
		waitms( 5);
		error = TrxSDIBlkSystem( no, SDIBlkConfig[no].System);
	}

	return( error);
}

/***************************************************************************/
/*	ConfigureAnalogSignalUnit												 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971127				     													*/
/*																									*/
/*	Function:	Download all parameters to the analog generator					*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 ErrorType code														*/
/*	Updates:		-																				*/
/***************************************************************************/
UC ConfigureAnalogSignalUnit() {

	AnalogSignalUnit.Present = TRUE;

	GetV24Version( AnalogSignalUnit.Address, PT8601, &Connectors[BB78Conn]);

												// Test if the board has been tampered with
	if ( !Connectors[BB78Conn].KUNumber && !Connectors[BB78Conn].SWVersion)
		return( SystemErrorType + ConfigurationError);

												// If /900 version DO NOT RECOGNIZE UNIT
	if ( Connectors[BB78Conn].HWVersion) {
		AnalogSignalUnit.Present = FALSE;		
		Connectors[BB78Conn].Present = FALSE;

		return( SystemErrorType + ConfigurationError);
	}
											// The below sequence of initialization should
											//  NOT be changed due to the fact that the
											//  system update is VERY time consuming in
											//  the unit
	if ( !error) {
		waitms( 5);
		error = TrxAnalogText( AnalogConfig.TextOn, AnalogConfig.TextInfo);
	}

	if ( !error) {
		waitms( 5);
		error = TrxAnalogSystem( AnalogConfig.System);
	}

	return( error);
}

/***************************************************************************/
/*	ConfigureSDISignalUnit													 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971215			     														*/
/*																									*/
/*	Function:	Download all parameters to the SDI generator						*/
/*	Remarks:	  	--																				*/
/*	Returns:		V24 ErrorType code														*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureSDISignalUnit() {

	SDISignalUnit.Present = TRUE;					// SDI test signal generator

	GetV24Version( SDISignalUnit.Address, PT8602, &Connectors[SDISConn]);

												// Test if the board has been tampered with
	if ( !Connectors[SDISConn].KUNumber && !Connectors[SDISConn].SWVersion)
		return( SystemErrorType + ConfigurationError);

												// If /900 version DO NOT RECOGNIZE UNIT
	if ( Connectors[SDISConn].HWVersion) {
		if ( Connectors[SDISConn].HWVersion != 5) {
			SDISignalUnit.Present = FALSE;		
			Connectors[SDISConn].Present = FALSE;
	
			return( SystemErrorType + ConfigurationError);
		}
	}
											// The below sequence of initialization should
											//  NOT be changed due to the fact that the
											//  system update is VERY time consuming in
											//  the unit
	waitms( 5);
	error = TrxSDIText( SDIConfig.TextOn, SDIConfig.TextInfo);

	if ( !error) {
		waitms( 5);
		error = TrxSDIEDH( SDIConfig.EDH);
	}

	if ( !error) {
		waitms( 5);
		error = TrxSDIAudioSignal( SDIConfig.AudioSignal);
	}

	if ( !error) {
		waitms( 5);
		error = TrxSDIAudioLevel( SDIConfig.AudioLevel);
	}

	if ( !error) {
		waitms( 5);
		error = TrxSDISystem( SDIConfig.System);
	}

	return( error);
}

/***************************************************************************/
/*	AES_EBUUnitDrv																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:	  	Level : 3;					(o)                                 */
/*					Signal : 4;					(o)                                 */
/*					Lock27MHz : 1;				(i): 1: PLL locked                  */
/*					Timing : 3;					(o):											*/
/*					Vidlock : 1;				(i): 1: Video locked to audio       */
/*					Y1Y0 : 2;				        Not used								*/
/*	Returns:		IIC ErrorType code														*/
/*	Updates:																						*/
/***************************************************************************/
UC AES_EBUUnitDrv( UC Signal, UC Level, UC Timing) {

	UI AES_EBUConfigPort;

	if ( Signal == Audio48kHz)
		AES_EBUConfigPort = AES_EBUSignalTable[Signal] +
								  0x0080 +

								  0x0800 +
								  0xF000;
	else
		AES_EBUConfigPort = AES_EBULevelTable[Level] +
								  AES_EBUSignalTable[Signal] +
								  0x0080 +

								  AES_EBUTimingTable[Timing] +
								  0x0800 +
								  0xF000;

	if ( IIC2_Write( sound1_addr, (UC) AES_EBUConfigPort))
		if( IIC2_Write( sound2_addr, (UC)( AES_EBUConfigPort >> 8)))
			return( NoError);

	return( AES_EBUErrorType + IIC2ErrorCode);
}

/***************************************************************************/
/*	GenlockUnitDrv																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971028				     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC GenlockUnitDrv( UC GenlockInput, UC GenlockSystem) {

	UC tmp, Mode, Port, ErrorCnt = 0, ErrorTmp;

	Port = GenlockInputTable[tmp = 2*GenlockInput];
	Mode = GenlockInputTable[++tmp];

	if ( GenlockInput < Internal) {
		Port |= GenlockSystemTable[tmp = 2*GenlockSystem];
		Mode = GenlockSystemTable[++tmp];
	}

	ud5 = Port;

	if ( TimeCodeUnit.Present) {						// If timecode unit is present.
		if ( GenlockInput < SDIGenlock) {			//  update if A, B or LOOP

			if ( GenlockSystem == BurstLockPAL)
				ErrorTmp = IIC1_Write( time_addr, 0xFF);
			else
				ErrorTmp = IIC1_Write( time_addr, 0xFE);

			if ( ErrorTmp == 0) {
				ErrorTmp = TimeCodeErrorType + IIC1ErrorCode;
				ErrorCnt++;
			}
		}
	}

	Genlock_LED = !( ActiveGenlockNo == Internal);

	if ( error = Update_LEDs()) {
		ErrorTmp = error;
		ErrorCnt++;
	}

	if ( error = SndChar( sync_addr, "GM", Mode)) {
		ErrorTmp = OSCErrorType + error;
		ErrorCnt++;
	}

	if ( error = TrxGenlockDelay( GenlockConfig[ExternalGenlockNo].Delay)) {
		ErrorTmp = error;
		ErrorCnt++;
	}

	if ( ErrorCnt > 1) 								// Multiple Errors detected
	  return( SystemErrorType + MultipleError);

	if ( ErrorCnt)
		return( ErrorTmp);

	return( 0);
}

/***************************************************************************/
/*	TrxAnlBlkSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit TV system to a black burst unit							*/
/*	Remarks:		Resets delay if it is illegal in selected system				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnlBlkSystem( UC index, UC system) {

	if ( system > PAL_PALID) {							// If system is NTSC..
																//  test if delay is valid
		if ( AnlBlkConfig[index].Delay >= (UL) 4*525*1716*256)
			AnlBlkConfig[index].Delay = 0;
	}
																		// Transmit system
	error = TrxAnlBlkDelay( index, AnlBlkConfig[index].Delay);

	if ( !error)													// Transmit ScH-Phase
		error = TrxAnlBlkScHPhase( index, AnlBlkConfig[index].ScHPhase);

	return( error);
}

/***************************************************************************/
/*	TrxAnlBlkDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit a delay, (ie. phase), to a black burst unit			*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnlBlkDelay( UC index, UL delay) {

	( index % 2) ? ( V24Cmd[0] = 'J') : ( V24Cmd[0] = 'H');

	switch ( AnlBlkConfig[index].System) {
		case PAL:
			V24Cmd[1] = 'G';
			break;

		case PAL_PALID:
			V24Cmd[1] = 'K';
			break;

		case NTSC:
			V24Cmd[1] = 'M';
			break;

		case NTSCwo:
			V24Cmd[1] = 'L';
			break;
	}

	if ( error = SndLong( AnlBlkUnit[index].Address, V24Cmd, delay))
		error += AnlBlkErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxAnlBlkScHPhase															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Convert and transmit a ScH-Phase to a black burst unit		*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnlBlkScHPhase( UC index, int ScHPhase) {

	long tmp = ScHPhase;

	( index % 2) ? ( V24Cmd[0] = 'J') : ( V24Cmd[0] = 'H');

	V24Cmd[1] = 'H';

	if ( ScHPhase < 0)				// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;							// tmp = ( ScHPhase*2048)/360;

	if ( error = SndInt( AnlBlkUnit[index].Address, V24Cmd, (UI) tmp))
		error += AnlBlkErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIBlkPattern															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit TV system to a SDI black unit								*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIBlkPattern( UC index, UC pattern) {

	if ( error = SndChar( SDIBlkUnit[index].Address, "HP", pattern))
		error += SDIBlkErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIBlkSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970506				     													*/
/*																									*/
/*	Function:	Transmit TV system to a SDI to black unit							*/
/*	Remarks:		Resets delay and/or pattern if it is illegal in selected		*/
/*					system																		*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIBlkSystem( UC index, UC system) {

	UC tmp = SDIBlkConfig[index].Pattern;

	if ( system == SDI525) {								// If system is SDI525..
																	// If delay is invalid..
		if ( SDIBlkConfig[index].Delay >= (long) 525*1716*256)
			SDIBlkConfig[index].Delay = 0;				// ..reset the delay

		if ( tmp > SBBlack)									// If pattern is invalid..
			SDIBlkConfig[index].Pattern = SBSMPTECBar;// ..reset pattern
	}
	else {
		if ( tmp == SBSMPTECBar)							// If pattern is invalid..
			SDIBlkConfig[index].Pattern = SBEBUCBar;	// ..reset pattern
	}
																	// Transmit system and
	error = TrxSDIBlkDelay( index, SDIBlkConfig[index].Delay);

	if ( !error)												// Transmit pattern
		error = TrxSDIBlkPattern( index, SDIBlkConfig[index].Pattern);

	return( error);
}

/***************************************************************************/
/*	TrxSDIBlkDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit delay, (ie. phase), to a SDI black unit				*/
/*	Remarks:	  	-																				*/
/*	Returns:		Error code																	*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIBlkDelay( UC index, UL delay) {

	V24Cmd[0] = 'H';

	if ( SDIBlkConfig[index].System == SDI625)
		V24Cmd[1] = 'G';
	else
		V24Cmd[1] = 'M';

	if ( error = SndLong( SDIBlkUnit[index].Address, V24Cmd, delay))
		error += SDIBlkErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIBlkEDH																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit	EDH on/off to a SDI black unit							*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIBlkEDH( UC index, UC state) {

	if ( error = SndChar( SDIBlkUnit[index].Address, "HE", state))
		error += SDIBlkErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIBlkAudio																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit embedded audio to a SDI black unit						*/
/*	Remarks:	  	-																				*/
/*	Returns:		Error code																	*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIBlkAudio( UC index, UC audio) {

	if ( error = SndChar( SDIBlkUnit[index].Address, "HU", audio))
		error += SDIBlkErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxAnalogPattern															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit a pattern to the analog generator						*/
/*	Remarks:	  	-																				*/
/*	Returns:		Error code																	*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnalogPattern( UC pattern) {

	if ( error = SndChar( AnalogSignalUnit.Address, "AP", pattern))
		error += AnlSignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxAnalogText																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit user text to the analog generator						*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnalogText( UC state, char* text) {

	error = SndChar( AnalogSignalUnit.Address, "AF", state);

	if ( !error)
		error = SndAsc( AnalogSignalUnit.Address, "AN", 0xFF, text);

	if ( error)
		error += AnlSignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxAnalogSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit TV system to the analog generator						*/
/*	Remarks:		Resets delay and/or pattern if it is illegal in selected		*/
/*					system																		*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnalogSystem( UC system) {

	UC tmp = AnalogConfig.Pattern;

	if ( system < NTSC) {								// If system is PAL..
		if ( tmp == SMPTECBar)							// If pattern is invalid..
			AnalogConfig.Pattern = EBUCBar;			// ..set new pattern
	}
	else {													// ..system is NTSC
																// If delay is invalid..
		if ( AnalogConfig.Delay >= (long) 4*525*1716*256)
			AnalogConfig.Delay = 0;						// ..reset the delay
																// If pattern is invalid..
		if (( tmp > SMPTECBar) && ( tmp < Window15))
			AnalogConfig.Pattern = SMPTECBar;		// ..reset the pattern
	}
																// Transmit system & delay
	error = TrxAnalogDelay( AnalogConfig.Delay);

	if ( !error)											// Transmit pattern
		error = TrxAnalogPattern( AnalogConfig.Pattern);

	if ( !error)											// Transmit ScHPhase
		error = TrxAnalogScHPhase( AnalogConfig.ScHPhase);

	return( error);
}

/***************************************************************************/
/*	TrxAnalogDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit delay, (ie. phase), to the analog generator			*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnalogDelay( UL delay) {

	V24Cmd[0] = 'A';

	switch ( AnalogConfig.System) {
		case PAL:
			V24Cmd[1] = 'G';
			break;

		case PAL_PALID:
			V24Cmd[1] = 'K';
			break;

		case NTSC:
			V24Cmd[1] = 'M';
			break;

		case NTSCwo:
			V24Cmd[1] = 'L';
			break;
	}

	if ( error = SndLong( AnalogSignalUnit.Address, V24Cmd, delay))
		error += AnlSignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxAnalogScHPhase															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Convert and transmit a ScH-Phase to the analog generator		*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxAnalogScHPhase( int ScHPhase) {

	long tmp = ScHPhase;

	if ( ScHPhase < 0)				// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;							// tmp = ( ScHPhase*2048)/360;

	if ( error = SndInt( AnalogSignalUnit.Address, "AH", (UI) tmp))
		error += AnlSignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIPattern																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit a pattern to the SDI generator							*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIPattern( UC pattern) {

	if ( error = SndChar( SDISignalUnit.Address, "SP", pattern))
		error += SDISignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIText																	 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	  																				*/
/*	Remarks:	  																					*/
/*	Returns:		Error code																	*/
/*	Updates:																						*/
/***************************************************************************/
UC TrxSDIText( UC state, char* text) {

	error = SndChar( SDISignalUnit.Address, "SF", state);

	if ( !error)
		error = SndAsc( SDISignalUnit.Address, "SN", 0xFF, text);

	if ( error)
		error += SDISignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDISystem																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		971217				     													*/
/*																									*/
/*	Function:	Transmit TV system to the SDI generator							*/
/*	Remarks:		Resets delay and/or pattern if it is illegal in selected		*/
/*					system																		*/
/*	Returns:		V24 error code																*/
/*	Updates:																						*/
/***************************************************************************/
UC TrxSDISystem( UC system) {

	UC tmp = SDIConfig.Pattern;

	if ( system == SDI625) {							// If system is SDII625..
		if ( tmp == SDISMPTECBar)						// If pattern is invalid..
			SDIConfig.Pattern = SDIEBUCBar;			// ..reset pattern
	}
	else {													// ..system is SDI525
		if ( SDIConfig.Delay >= ( long) 525*1716*256)	// If delay is invalid..
			SDIConfig.Delay = 0;							// ..reset the delay
																// If pattern is invalid..

		if ((( tmp > SDISMPTECBar) && ( tmp < SDICheck)) || ( tmp > SDIRed75))
			SDIConfig.Pattern = SDISMPTECBar;		// ..reset pattern
	}

	error = TrxSDIDelay( SDIConfig.Delay);			// Transmit delay

	if ( !error)											// Transmit pattern
		error = TrxSDIPattern( SDIConfig.Pattern);

	return( error);
}

/***************************************************************************/
/*	TrxSDIDelay																	 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit delay to the SDI generator									*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIDelay( UL delay) {

	V24Cmd[0] = 'S';

	if ( SDIConfig.System == PAL)
		V24Cmd[1] = 'G';
	else
		V24Cmd[1] = 'M';

	if ( error = SndLong( SDISignalUnit.Address, V24Cmd, delay))
		error += SDISignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIEDH																	 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit EDH on/off to the SDI generator							*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIEDH( UC state) {

	if ( error = SndChar( SDISignalUnit.Address, "SE", state))
		error += SDISignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIAudioSignal															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit embedded audio signal to the SDI generator			*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIAudioSignal( UC audiosignal) {

	if ( error = SndChar( SDISignalUnit.Address, "SU", audiosignal))
		error += SDISignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxSDIAudiolevel															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970202				     													*/
/*																									*/
/*	Function:	Transmit embedded audio level to the SDI generator				*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxSDIAudioLevel( UC audiolevel) {

	if ( error = SndChar( SDISignalUnit.Address, "SL", audiolevel))
		error += SDISignalErrorType;

	return( error);
}

/***************************************************************************/
/*	TrxGenlockDelay															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		970217				     													*/
/*																									*/
/*	Function:	Transmit genlock offset to ALL installed units					*/
/*	Remarks:	  	-																				*/
/*	Returns:		V24 error code																*/
/*	Updates:		-																				*/
/***************************************************************************/
UC TrxGenlockDelay( UL delay) {

	register UC i;
	UC ErrorCnt = 0, ErrorTmp;

	V24Cmd[1] = 'O';

	for ( i=0; i < NoOfAnlBlkUnits; i++)
		if ( AnlBlkUnit[i].Present) {

			( i % 2) ? ( V24Cmd[0] = 'J') : ( V24Cmd[0] = 'H');

			if ( error = SndLong( AnlBlkUnit[i].Address, V24Cmd, delay)) {
				ErrorTmp = AnlBlkErrorType + error;
				ErrorCnt++;
			}
		}

	for ( i=0; i < NoOfSDIBlkUnits; i++)
		if ( SDIBlkUnit[i].Present) {
			if ( error = SndLong( SDIBlkUnit[i].Address, "HO", delay)) {
				ErrorTmp = SDIBlkErrorType + error;
				ErrorCnt++;
			}
		}

	if ( SDISignalUnit.Present)
		if ( error = SndLong( SDISignalUnit.Address, "SO", delay)) {
			ErrorTmp = SDISignalErrorType + error;
			ErrorCnt++;
		}

	if ( AnalogSignalUnit.Present)
		if ( error = SndLong( AnalogSignalUnit.Address, "AO", delay)) {
			ErrorTmp = AnlSignalErrorType + error;
			ErrorCnt++;
		}

	if ( ErrorCnt > 1) 								// Multiple Errors detected
	  return( SystemErrorType + MultipleError);

	if ( ErrorCnt)
		return( ErrorTmp);

	return( 0);
}

/***************************************************************************/
/* ConfigurePreset															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970220 			     														*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		Nothing																		*/
/*	Updates:																						*/
/***************************************************************************/
UC ConfigurePreset( UC No) {

	register UC i;
	UC ErrorCnt = 0, ErrorTmp;

	for ( i = 0; i < NoOfAnlBlkUnits; i++) {
		memcpy( &AnlBlkConfig[i], &Preset[No].AnlBlkStore[i], sizeof( AnlBlkConfig[0]));

		if ( AnlBlkUnit[i].Present)
			if ( error = ConfigureAnlBlkUnit( i)) {
				ErrorTmp = error;
				ErrorCnt++;
			}
	}

	for ( i = 0; i < NoOfSDIBlkUnits; i++) {
		memcpy( &SDIBlkConfig[i], &Preset[No].SDIBlkStore[i], sizeof( SDIBlkConfig[0]));

		if ( SDIBlkUnit[i].Present)
			if ( error = ConfigureSDIBlkUnit( i)) {
				ErrorTmp = error;
				ErrorCnt++;
			}
	}

	memcpy( &AnalogConfig, &Preset[No].AnalogStore, sizeof( AnalogConfig));
	if ( AnalogSignalUnit.Present)
		if ( error = ConfigureAnalogSignalUnit()) {
			ErrorTmp = error;
			ErrorCnt++;
		}

	memcpy( &SDIConfig, &Preset[No].SDIStore, sizeof( SDIConfig));
	if ( SDISignalUnit.Present)
		if ( error = ConfigureSDISignalUnit()) {
			ErrorTmp = error;
			ErrorCnt++;
		}

	memcpy( &AES_EBUConfig, &Preset[No].AES_EBUStore, sizeof( AES_EBUConfig));
	if ( AES_EBUUnit.Present)
		if ( error = AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level, AES_EBUConfig.Timing)) {
			ErrorTmp = error;
			ErrorCnt++;
		}

	for ( i = 0; i < NoOfGenlockUnits; i++)
		memcpy( &GenlockConfig[i], &Preset[No].GenlockStore[i], sizeof( GenlockConfig[0]));

	ActiveGenlockNo = Preset[No].ActiveGenlockNo;
	ExternalGenlockNo = Preset[No].ExternalGenlockNo;

	if ( error = GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System)) {
		ErrorTmp = error;
		ErrorCnt++;
	}

	if ( ErrorCnt > 1) 								// Multiple Errors detected
	  return( SystemErrorType + MultipleError);

	if ( ErrorCnt)
		return( ErrorTmp);

	return( 0);
}

/***************************************************************************/
/*	GetV24Version														 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970216	    										*/
/* Revised:		971118				     													*/
/*																									*/
/*	Function:	Get the SW version and KU number from a RS232 unit.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GetV24Version( UC address, UC type, struct CONN_STRUCTURE* Connector) {

	UI tmp;
	char* ascptr;

	Connector->Present = TRUE;							// Set connector occupied

	switch ( type) {
		case PT8601:
		case PT8602:
			if ( type == PT8601)
				V24Cmd[0] = 'A';
			else
				V24Cmd[0] = 'S';

			V24Cmd[1] = 'V';								// Get HW type & version

			tmp = RecInt( address, V24Cmd, 0xFF, &error);

			if ( error) {
				Connector->HWType = type;
				Connector->HWVersion = 0;
			}
			else {
				Connector->HWType = tmp & 0xFF;
				Connector->HWVersion = tmp >> 8;
			}
			break;

		case PT8608:
		case PT8609:
			V24Cmd[0] = 'H';
			Connector->HWType = type;
			break;
	}

	V24Cmd[1] = 'R';											// Get KU number

	ascptr = RecStr( address, V24Cmd, ProductKUNo_addr/10, &error);

	if ( !error && ( strlen( ascptr) == 8))
		Connector->KUNumber = atol( &ascptr[2]);
	else
		Connector->KUNumber = 0;


	V24Cmd[1] = 'I';											// Get SW version

	tmp = RecInt( address, V24Cmd, 0xFF, &error);

	if ( error)
		Connector->SWVersion = 0;
	else
		Connector->SWVersion = ( UC) tmp;
}

/***************************************************************************/
/*	GetIICVersion														 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970216	    										*/
/* Revised:		970917				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		errocode																		*/
/*	Updates:																						*/
/***************************************************************************/
void GetIICVersion( UC bus, UC address, UC KU_address, UL* KU) {

	UC tmp;
	char TmpBuffer[9];

	if ( bus == IICBus2) {
		IIC2_RdStr( address, KU_address, TmpBuffer);
		tmp = IIC2ErrorCode;
	}
	else {
		IIC1_RdStr( address, KU_address, TmpBuffer);
		tmp = IIC1ErrorCode;
	}

	TmpBuffer[8] = 0;

	if ( tmp)
		*KU = 0;
	else
		*KU = atol( &TmpBuffer[2]);
}

/***************************************************************************/
/*	TestMainboard																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971027	    										*/
/* Revised:		971029, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the main board														*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: error															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestMainboard( void) {
															// Test power calibration values
	if ( !IIC2_Write( mainram1_addr, VoltCalib_addr))
		return( 1);

	if ( (UC) VoltCalib - IIC2_Read( mainram1_addr))
		return( 1);

	if ( VoltN5Min - IIC2_Read( mainram1_addr))
		return( 1);

	if ( VoltN5Max - IIC2_Read( mainram1_addr))
		return( 1);

	if ( Volt12Min - IIC2_Read( mainram1_addr))
		return( 1);

	if ( Volt12Max - IIC2_Read( mainram1_addr))
		return( 1);

	IIC1_Read( ttl_addr);					// Read the parallel remote IIC port
	if ( IIC1ErrorCode)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestSPGUnit																	 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971027	    										*/
/* Revised:		971027, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the SPG unit															*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: error															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSPGUnit( void) {

	UL tmp;

	tmp = IIC2_RdLong( spgram_addr, HPHZero_G_addr) - RecLong( sync_addr, "GA", 0xFF, &error);
	if ( error || tmp)
		return( 1);

	tmp = IIC2_RdInt( spgram_addr, GenlPhase_G_addr) - RecInt( sync_addr, "GF", 0xFF, &error);
	if ( error || tmp)
		return( 1);

	tmp = IIC2_RdLong( spgram_addr, HPHZero_M_addr) - RecLong( sync_addr, "GU", 0xFF, &error);
	if ( error || tmp)
		return( 1);

	tmp = IIC2_RdInt( spgram_addr, GenlPhase_M_addr) - RecInt( sync_addr, "GG", 0xFF, &error);
	if ( error || tmp)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestAES_EBUUnit															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971023	    										*/
/* Revised:		971024, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8605 AES/EBU Audio generator							*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: error															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestAES_EBUUnit( void) {

	UI tmp;

	if ( AES_EBUConfig.Signal == Audio48kHz)
		tmp = AES_EBUSignalTable[AES_EBUConfig.Signal] +
			   0x0080 +

			   0x0800 +
				0xF000;
	else
		tmp = AES_EBULevelTable[AES_EBUConfig.Level] +
			   AES_EBUSignalTable[AES_EBUConfig.Signal] +
			   0x0080 +

			   AES_EBUTimingTable[AES_EBUConfig.Timing] +
			   0x0800 +
				0xF000;

	if (( tmp & 0x00FF) - IIC2_Read( sound1_addr))
		return( 1);

	if (( tmp >> 8) - IIC2_Read( sound2_addr))
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestAnlBlkUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971023	    										*/
/* Revised:		971024, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8608 Black Burst generator								*/
/*	Remarks:		--																				*/
/*	Returns:																						*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestAnlBlkUnit( UC conn) {

	UC no, addr;

	switch ( conn) {
		case BB12Conn:
			no = BB1Item;
			addr = bbu0_addr;
			break;

		case BB34Conn:
			no = BB3Item;
			addr = bbu1_addr;
			break;

		case BB56Conn:
			no = BB5Item;
			addr = bbu3_addr;
			break;

		case BB78Conn:
			no = BB7Item;
			addr = bbu2_addr;
			break;
	}

	if ( RecInt( addr, "HS", 0xFF, &error))
		return( 1);

											// Not implemented in ver 2.2 and earlier
	if ( AnlBlkConfig[no].System - ( UC) RecInt( addr, "HQ", 0xFF, &error)) {
		if ( error != RecV24Timeout)
			return( 2);
	}

											// Not implemented in ver 2.2 and earlier
	if ( AnlBlkConfig[no+1].System - ( UC) RecInt( addr, "JQ", 0xFF, &error)) {
		if ( error != RecV24Timeout)
			return( 2+32);
	}

	if ( AnlBlkConfig[no].ScHPhase - ( UC) RecInt( addr, "HH", 0xFF, &error))
		return( 9);

	if ( AnlBlkConfig[no+1].ScHPhase - ( UC) RecInt( addr, "JH", 0xFF, &error))
		return( 9+32);

											// Not implemented in ver 2.2 and earlier
	if ( AnlBlkConfig[no].Delay - RecLong( addr, "HB", 0xFF, &error)) {
		if ( error != RecV24Timeout)
			return( 10);
	}

											// Not implemented in ver 2.2 and earlier
	if ( AnlBlkConfig[no+1].Delay - RecLong( addr, "JB", 0xFF, &error)) {
		if ( error != RecV24Timeout)
			return( 10+32);
	}

	return( 0);
}

/***************************************************************************/
/*	TestSDIBlkUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971023	    										*/
/* Revised:		971023				     													*/
/*																									*/
/*	Function:	Test the PT8609 SDI Black/Colorbar generator						*/
/*	Remarks:																						*/
/*	Returns:		errocode																		*/
/*	Updates:																						*/
/***************************************************************************/
UC TestSDIBlkUnit( UC conn) {

	UC no, addr;

	switch ( conn) {
		case BB34Conn:
			no = SB34Item;
			addr = bbu1_addr;
			break;

		case BB56Conn:
			no = SB56Item;
			addr = bbu3_addr;
			break;

		case BB78Conn:
			no = SB78Item;
			addr = bbu2_addr;
			break;
	}

	if ( RecInt( addr, "HS", 0xFF, &error))
		return( 1);

	if ( SDIBlkConfig[no].System - ( UC) RecInt( addr, "HQ", 0xFF, &error))
		return( 2);

	if ( SDIBlkConfig[no].Pattern - ( UC) RecInt( addr, "HP", 0xFF, &error))
		return( 3);

	if ( SDIBlkConfig[no].EDH - ( UC) RecInt( addr, "HE", 0xFF, &error))
		return( 4);

	if ( SDIBlkConfig[no].EMBAudio - ( UC) RecInt( addr, "HU", 0xFF, &error))
		return( 5);

	if ( SDIBlkConfig[no].Delay - RecLong( addr, "HB", 0xFF, &error))
		return( 10);

	return( 0);
}

/***************************************************************************/
/*	TestAnalogSignalUnit														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971023	    										*/
/* Revised:		971031				     													*/
/*																									*/
/*	Function:	Test the PT8601 Analog signal generator							*/
/*	Remarks:		--																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestAnalogSignalUnit( void) {

	if ( RecInt( AnlSig_addr, "AS", 0xFF, &error))
		return( 1);
							
	if ( AnalogConfig.System - ( UC) RecInt( AnlSig_addr, "AQ", 0xFF, &error))
		return( 2);

	if ( AnalogConfig.Pattern - ( UC) RecInt( AnlSig_addr, "AP", 0xFF, &error))
		return( 3);

	if ( AnalogConfig.TextOn - ( UC) RecInt( AnlSig_addr, "AF", 0xFF, &error))
		return( 7);

	if ( strcmp( AnalogConfig.TextInfo, RecStr( AnlSig_addr, "AN", 0xFF, &error)))
		return( 8);

	if ( AnalogConfig.ScHPhase - RecInt( AnlSig_addr, "AH", 0xFF, &error))
		return( 9);

	if ( AnalogConfig.Delay - RecLong( AnlSig_addr, "AB", 0xFF, &error))
		return( 10);

	return( 0);
}

/***************************************************************************/
/*	TestSDISignalUnit															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971023	    										*/
/* Revised:		971031				     													*/
/*																									*/
/*	Function:	Test the PT8602/03 SDI Digital signal generator					*/
/*	Remarks:		--																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSDISignalUnit( void) {

	if ( RecInt( SDISig_addr, "SS", 0xFF, &error))
		return( 1);

	if ( SDIConfig.System - ( UC) RecInt( SDISig_addr, "SQ", 0xFF, &error))
		return( 2);

	if ( SDIConfig.Pattern - ( UC) RecInt( SDISig_addr, "SP", 0xFF, &error))
		return( 3);

	if ( SDIConfig.TextOn - ( UC) RecInt( SDISig_addr, "SF", 0xFF, &error))
		return( 7);

	if ( strcmp( SDIConfig.TextInfo, RecStr( SDISig_addr, "SN", 0xFF, &error)))
		return( 8);

	if ( SDIConfig.EDH - ( UC) RecInt( SDISig_addr, "SE", 0xFF, &error))
		return( 4);

	if ( SDIConfig.AudioSignal - ( UC) RecInt( SDISig_addr, "SU", 0xFF, &error))
		return( 5);

	if ( SDIConfig.AudioLevel - ( UC) RecInt( SDISig_addr, "SL", 0xFF, &error))
		return( 6);

	if ( SDIConfig.Delay - RecLong( SDISig_addr, "SB", 0xFF, &error))
		return( 10);

	return( 0);
}

/***************************************************************************/
/*	TestBBMulUnit														 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971024	    										*/
/* Revised:		971029, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8604 Multiple Parallel Black Burst Option			*/
/*	Remarks:		--																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestBBMulUnit( void) {

	UL tmp;

	GetIICVersion( IICBus1, multibb_addr, ProductKUNo_addr, &tmp);

	return( tmp == 0);
}

/***************************************************************************/
/*	TestSDIGenlockUnit												 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971024	    										*/
/* Revised:		971029, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8606 SDI Digital Genlock									*/
/*	Remarks:		--																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSDIGenlockUnit( void) {

	UL tmp;

	GetIICVersion( IICBus1, d1ram_addr, ProductKUNo_addr, &tmp);

	return( tmp == 0);
}

/***************************************************************************/
/*	TestTimecodeUnit															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971024	    										*/
/* Revised:		971029, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8607 Longitudinal Time Code Module					*/
/*	Remarks:		--																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestTimecodeUnit( void) {

	UL tmp;

	GetIICVersion( IICBus1, timeram_addr, ProductKUNo_addr, &tmp);

	return( tmp == 0);
}

/***************************************************************************/
/*	TestHighPrecisionOCXUnit												 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971024	    										*/
/* Revised:		971029, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8610 High Precision OCXO									*/
/*	Remarks:		Presently NOT tested														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestHighPrecisionOCXUnit( void) {

	return( 0);
}
