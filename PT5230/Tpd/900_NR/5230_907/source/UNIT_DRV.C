/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		UNIT_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980119																		*/
/*	Rev. date:	000608, KEn, DEV															*/
/*	Status:		Version 1.5																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void FindStaticOptions( UC conn_addr);											*/
/*		UC FindDynamicOptions( UC conn_addr);											*/
/*																									*/
/*		UC FindUnit( UC dir, UC unit);													*/
/*		UC FindNextTSGTPG( UC unit);                                         */
/*																									*/
/*		UC ConfigureSPG()																		*/
/*		UC ConfigureAnlBlkUnit( UC ndx)													*/
/*		UC ConfigureAnlTPGUnit( UC ndx)													*/
/*		UC ConfigureAnlTPGText( UC ndx, UC pattern)									*/
/*		UC ConfigureSDITSGUnit( UC ndx)													*/
/*		UC ConfigureSDITPGUnit( UC ndx)													*/
/*		UC ConfigureSDITPGText( UC ndx, UC pattern)									*/
/*		UC ConfigureAESEBUUnit( UC ndx)													*/
/*																									*/
/*		UC AESEBUUnitDrv( UC ndx, UC Signal, UC Level, UC Timing)				*/
/*		UC GenlockUnitDrv( UC GenlockInput, UC GenlockSystem)						*/
/*																									*/
/*		UC BroadcastTCITime()																*/
/*																									*/
/*		UC TrxAnlBlkSystem( UC ndx, UC system, UL delay)							*/
/*		UC TrxAnlBlkDelay( UC ndx, UC system, UL delay)								*/
/*		UC TrxAnlBlkScHPhase( UC ndx, int ScHPhase)									*/
/*																									*/
/*		UC TrxAnlTPGSystem( UC ndx, UC system, UL delay, UC pattern,\			*/
/*																				 UC attrib);	*/
/*		UC TrxAnlTPGDelay( UC ndx, UC system, UL delay)								*/
/*		UC TrxAnlTPGPattern( UC ndx, UC pattern, UC attrib)						*/
/*		UC TrxAnlTPGScHPhase( UC ndx, int ScHPhase)									*/
/*		UC TrxAnlTPGTextStyle( UC ndx, UC pattern, UC style)						*/
/*		UC TrxAnlTPGTextPos( UC ndx, UC style, UC Xpos, UC Ypos)					*/
/*		UC TrxAnlTPGTextLineEnable( UC ndx, UC no, UC state) 						*/
/*		UC TrxAnlTPGText( UC ndx, UC no, char* text)									*/
/*																									*/
/*		UC TrxSDITSGSystem( UC ndx, UC system, UL delay, UC pattern)			*/
/*		UC TrxSDITSGDelay( UC ndx, UC system, UL delay)								*/
/*		UC TrxSDITSGPattern( UC ndx, UC pattern)										*/
/*		UC TrxSDITSGEDH( UC ndx, UC state)												*/
/*		UC TrxSDITSGAudioSignal( UC ndx, UC signal)									*/
/*		UC TrxSDITSGAudioLevel( UC ndx, UC level)										*/
/*																									*/
/*		UC TrxSDITPGSystem( UC ndx, UC system, UL delay, UC pattern,\			*/
/*																				 UC attrib);	*/
/*		UC TrxSDITPGDelay( UC ndx, UC system, UL delay)								*/
/*		UC TrxSDITPGPattern( UC ndx, UC pattern, UC attrib)						*/
/*		UC TrxSDITPGEDH( UC ndx, UC state)												*/
/*		UC TrxSDITPGTextStyle( UC ndx, UC pattern, UC style)						*/
/*		UC TrxSDITPGTextPos( UC ndx, UC style, UC Xpos, UC Ypos)					*/
/*		UC TrxSDITPGTextLineEnable( UC ndx, UC no, UC state) 						*/
/*		UC TrxSDITPGText( UC ndx, UC no, char* text)									*/
/*		UC TrxSDITPGAudioSignal( UC ndx, UC signal)									*/
/*		UC TrxSDITPGAudioLevel( UC ndx, UC level)										*/
/*		UC TrxSDITPGAudioGroup( UC ndx, UC group)										*/
/*																									*/
/*		UC TrxTimeClockControl( UC control)												*/
/*		UC TrxTimeClockData( UC TCIControl, UC noOfBytes, UL val)				*/
/*																									*/
/*		UC TrxGenlockDelay( UL delay)														*/
/*																									*/
/*		UC ConfigurePreset( UC No)															*/
/*																									*/
/*		UC GetV24Version( UC ndx, UC address, UC type);								*/
/*		UC GetIICVersion( UC bus, UC address, UC KU_address, UL* KU)			*/
/*																									*/
/*		UC TestMainboard( void)																*/
/*		UC TestSPGUnit( void)																*/
/*		UC TestAESEBUUnit( UC ndx)															*/
/*		UC TestAnlBlkUnit( UC ndx)															*/
/*		UC TestAnlTPGUnit( UC ndx)															*/
/*		UC TestSDITSGUnit( UC ndx)															*/
/*		UC TestSDITPGUnit( UC ndx)															*/
/*		UC TestBBMulUnit( void)																*/
/*		UC TestSDIGenlockUnit( void)														*/
/*		UC TestTimeClockUnit( void)														*/
/*		UC TestHighPrecisionOCXUnit( void)												*/
/*																									*/
/*	Changes:																						*/
/* 991026: Error in TestAESEBUUnit() corrected										*/
/* 990304: HWInfo has changed; see MAS.H												*/
/* 981002: In FindStaticOptions(), PT8631: when deciding if the AnlTPG in	*/
/*			  bbu_2 was AnlTPG2 or AnlTPG5, it was NOT yet tested if a PT8601 */
/*			  was present. The PT8631 was then accepted as AnlTPG2. When the 	*/
/*			  PT8601 LATER on was detected it was ALSO accepted as AnlTPG2,	*/
/*			  as this unit is locked to this position.								*/
/* 980921: In ConfigureSDITPGUnit() & ConfigureAnlTPGUnit(): time/date		*/
/*			  enable is now cleared if the P8637 TCI is NOT present.				*/
/* 980918: In FindStaticOptions(): PT8637 is now tested for a legal time	*/
/*			  and date in the TCI, AND reset if illegal.								*/
/* 980827: Added BroadcastTCITime()														*/
/* 980825: Update Time Clock Interface													*/
/* 980825: Added TrxTimeClockControl()													*/
/* 980825: Released as 4008 002 06813													*/
/*			  Error in PT8635 powerup initialization. Level on output 2 was	*/
/*			  the same as level on output 1.												*/
/* 980616: Released as 4008 002 06812													*/
/* 980528: Implemented detection of 900-versions, ( in GetV24Version())		*/
/* 980522: Optimizations in Test functions.											*/
/* 980515: Released as 4008 002 06811													*/
/* 980515: In FindStaticOptions() & ConfigurePreset():						 	*/
/*			  The Hi-TECH compiler can NOT handle the construction :				*/
/*					for ( i = A; i <= B + ( HWtype == PT8635)							*/
/*                                                                         */
/* 30.04.03: Function ConfigureSDITPGUnit() has now much longer delays,    */
/*           as it involves loading moving logo (45k) which takes long time*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <xa.h>

#include "mas.h"

#include "serial1.h"
#include "unit_drv.h"
#include "tables.h"
#include "menutree.h"
#include "iic.h"
#include "keyb_drv.h"
#include "message.h"
#include "util.h"
#include "factory.h"

// Temporary variables PRIVATE NOT overlayable

static char cmdBuffer[3] = { 0, 0, 0};

static char tmpBuffer[11];

/***************************************************************************/
/*	FindStaticOptions															 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980208												*/
/* Revised:		980918				     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:    This function should only be called AFTER all the resets		*/
/*					have been released.														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FindStaticOptions() {

	register UC i;
	UC error;
	UL tmpLong;

	FeedTheWatchdog;

	//***********************************************************************
	// Option PT8601 SDI Analog signal/Pattern Generator.
	// This unit SHOULD only be placed in upper space TXG2.
	// This unit has a dedicated address.
	//***********************************************************************
	if ( SndChar( AnlSig_addr, "AI", 0) == V24Ok)
		if ( GetV24Version( 0, AnlSig_addr, PT8601) == 0)
			ConfigureAnlTPGUnit( 0);

	//***********************************************************************
	// Option PT8602/03 SDI Test signal/Pattern Generator 1.
	// This unit can ONLY be placed in connector TXG1.
	// This unit has a dedicated address.
	//***********************************************************************
	if ( SndChar( SDISig_addr, "SI", 0) == V24Ok)
		if ( GetV24Version( 0, SDISig_addr, PT8602) == 0)
			ConfigureSDITPGUnit( 0);


	//***********************************************************************
	// Option PT8632 SDI Test signal/Pattern Generator 1.
	// This unit can ONLY be placed in connector TXG1.
	// This unit has a dedicated address.
	//***********************************************************************
	if ( SndChar( SDITPG_addr, "SI", 0) == V24Ok)
		if ( GetV24Version( 0, SDITPG_addr, PT8632) ==0)
			ConfigureSDITPGUnit( 0);


	//***********************************************************************
	// Option PT8604 Multiple Black Burst Generator.
	//***********************************************************************
	if ( IIC1Write( multibb_addr, 0xff)) {

		AnlBlkUnit[BB2Item].Present = TRUE;
		AnlBlkUnit[BB2Item].HWType = PT8604;

		GetIICVersion( IICBus1, multibb_addr, ProductKUNo_addr,\
															 &AnlBlkUnit[BB2Item].KUNumber);
	}


	//***********************************************************************
	// Option PT8605/35 AES/EBU Audio Generator.
	//***********************************************************************
	if ( IIC2Write( sound1_addr, 0xff)) {

		AESEBUUnit.Present = TRUE;
		AESEBUUnit.HWType = PT8605;

		if ( IIC2Write( sound3_addr, 0xff)) 	 		// Option PT8635
			AESEBUUnit.HWType = PT8635;

		GetIICVersion( IICBus2, soundram_addr, ProductKUNo_addr,\
																		 &AESEBUUnit.KUNumber);

		error = AESEBUUnitDrv( 0, AESEBUConfig[0].Signal, AESEBUConfig[0].Level, AESEBUConfig[0].Timing);

		if ( !error && ( AESEBUUnit.HWType == PT8635))
			error = AESEBUUnitDrv( 1, AESEBUConfig[1].Signal, AESEBUConfig[1].Level, AESEBUConfig[1].Timing);

		if ( error)
			DisplayErrorLine( error, ErrorDisplayTimeout);
	}


	//***********************************************************************
	// Option PT8606 SDI Genlock Module.
	//***********************************************************************
	if ( IIC1Write( d1ram_addr, 0xff))	{

		SDIGenlockUnit.Present = TRUE;
		SDIGenlockUnit.HWType = PT8606;

		GetIICVersion( IICBus1, d1ram_addr, ProductKUNo_addr,\
																	 &SDIGenlockUnit.KUNumber);
	}
	else {
		if ( ActiveGenlockNo == SDIGenlock) {
			ActiveGenlockNo = Internal;

			DisplayErrorLine( SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
		}
	}


	//***********************************************************************
	// Option PT8607 TimeCode unit.
	//***********************************************************************
	if ( IIC1Write( time_addr, 0xff)) {

		TimeClockUnit.Present = TRUE;
		TimeClockUnit.HWType = PT8607;

		GetIICVersion( IICBus1, timeram_addr, ProductKUNo_addr,\
																	 &TimeClockUnit.KUNumber);
	}


	//***********************************************************************
	// Option PT8637 TimeClock unit.
	//***********************************************************************
	if ( IIC1Write( time1_addr, TimeClockConfig.TCIControl)) {

		TimeClockUnit.Present = TRUE;
		TimeClockUnit.HWType = PT8637;

		GetIICVersion( IICBus1, timeram_addr, ProductKUNo_addr,\
																	 &TimeClockUnit.KUNumber);

													// Get TCI generel information.
		TimeClockConfig.TCIControl &= 0x1F;
		TimeClockConfig.TCIControl |= ( TCIInfoRegister << 5);
		IIC1Write( time1_addr, TimeClockConfig.TCIControl);

		FeedTheWatchdog;

		IIC1ReadN( time1_addr, 4, (UC*) tmpBuffer);

		TimeClockUnit.HWType = tmpBuffer[1];
		TimeClockUnit.HWVersion = tmpBuffer[2];
		TimeClockUnit.SWVersion = tmpBuffer[3];

		FeedTheWatchdog;

													// Set TCI time reference.
		TimeClockConfig.TCIControl &= 0x1F;
		TimeClockConfig.TCIControl |= ( TCITimeRefRegister << 5);
		TrxTimeClockData( TimeClockConfig.TCIControl, 1, (UL) TimeClockConfig.Reference);

		FeedTheWatchdog;

													// Set TCI time offset.
		TimeClockConfig.TCIControl &= 0x1F;
		TimeClockConfig.TCIControl |= ( TCIOffsetRegister << 5);
		TrxTimeClockData( TimeClockConfig.TCIControl, 3, (UL) TimeClockConfig.Offset);

		FeedTheWatchdog;

													// Read TCI time.
		TimeClockConfig.TCIControl &= 0x1F;
		TimeClockConfig.TCIControl |= ( TCITimeRegister << 5);
		IIC1Write( time1_addr, TimeClockConfig.TCIControl);

		FeedTheWatchdog;

		IIC1ReadN( time1_addr, 4, (UC*) tmpBuffer);

		tmpLong = tmpBuffer[1];
		tmpLong += ((UL) tmpBuffer[2] << 8);
		tmpLong += ((UL) tmpBuffer[3] << 16);

		if ( !ValidTime( tmpLong)) {		// Validate TCI time information.
			tmpLong = TimeClockConfigFactory.Time;
													// Update TCI
			TrxTimeClockData( TimeClockConfig.TCIControl, 3, tmpLong);
		}
													// Convert time to user format
		TimeFormat(( TimeClockConfig.Format & 0x01), tmpLong, 1, tmpBuffer);
			
		SndBroadcast( "BT", tmpBuffer);	// Broadcast time information

		FeedTheWatchdog;

													// Read TCI date.
		TimeClockConfig.TCIControl &= 0x1F;
		TimeClockConfig.TCIControl |= ( TCIDateRegister << 5);
		IIC1Write( time1_addr, TimeClockConfig.TCIControl);
	
		FeedTheWatchdog;

		IIC1ReadN( time1_addr, 4, (UC*) tmpBuffer);

		tmpLong = tmpBuffer[1];
		tmpLong += ((UL) tmpBuffer[2] << 8);
		tmpLong += ((UL) tmpBuffer[3] << 16);

		if ( !ValidDate( tmpLong)) {		// Validate TCI date information.
			tmpLong = TimeClockConfigFactory.Date;
													// Update TCI
			TrxTimeClockData( TimeClockConfig.TCIControl, 3, tmpLong);
		}
													// Convert date to user format
		DateFormat(( TimeClockConfig.Format >> 1), tmpLong, 1, tmpBuffer);

		SndBroadcast( "BD", tmpBuffer);	// Broadcast date information
	}


	//***********************************************************************
	// Option PT8610 High Precision OCX oscillator.
	//***********************************************************************
	i = (UC) RecInt( sync_addr, "GS", 0xFF, &error);

	if ( !error) {
		if ( i & OvenMask) {
			Int2GenlockUnit.Present = TRUE;
			Int2GenlockUnit.HWType = PT8610;
		}
		else {
			if ( ActiveGenlockNo == Internal2) {
				ActiveGenlockNo = Internal;

				DisplayErrorLine( SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
			}
		}
	}
}

/***************************************************************************/
/*	FindDynamicOptions														 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980208												*/
/* Revised:		981002				     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		This function MUST be called as bbu1_addr, bbu2_addr & 		*/
/*					bbu3_addr.																	*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FindDynamicOptions( UC conn_addr)
{
	UC ndx;//, tmp;

	FeedTheWatchdog;

	switch ( conn_addr) {
		case bbu1_addr:
			//*****************************************************************
			// Option PT8608 Black Burst & PT8609 Serial Black.
			//*****************************************************************
			SndChar( DefaultAnlBlk_addr, "HA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			switch ( S1BUF) {
				case bbu1_addr:					// BB3&4 at TXG3Conn
					GetV24Version( BB3Item, bbu1_addr, PT8608);

					ConfigureAnlBlkUnit( BB3Item);
					ConfigureAnlBlkUnit( BB4Item);
					return;

				case bbu1_addr+0x40:				// SDI-BLK3&4 at TXG3Conn
					if ( GetV24Version( 1, bbu1_addr, PT8609) == 0)
						ConfigureSDITSGUnit( 1);
					return;
			}

			//*****************************************************************
			// Option PT8631 Analog Test signal/Pattern Generator.
			//*****************************************************************
			SndChar( DefaultAnlTPG_addr, "AA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu1_addr) {			// ANALOG-TPG2 at TXG2Conn
				if ( GetV24Version( 0, bbu1_addr, PT8631) == 0)
					ConfigureAnlTPGUnit( 0);
				return;
			}

			//*****************************************************************
			// Option PT8633 Analog Test signal/Pattern Generator.
			//*****************************************************************
			SndChar( DefaultSDITPG_addr, "SA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu1_addr) {			// SDI-TPG2 at TXG2Conn
				if ( GetV24Version( 1, bbu1_addr, PT8633) == 0)
					ConfigureSDITPGUnit( 1);
				return;
			}

			//*****************************************************************
			// Option PT8639 SDI Test Signal Generator.
			//*****************************************************************
			SndChar( DefaultSDITSG_addr, "HA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu1_addr+0x40) {	// SDI-TSG3 at TXG3Conn
				if ( GetV24Version( 1, bbu1_addr, PT8639) == 0)
					ConfigureSDITSGUnit( 1);
				return;
			}
			return;

		case bbu2_addr:
			//*****************************************************************
			// Option PT8608 Black Burst & PT8609 Serial Black.
			//*****************************************************************
			SndChar( DefaultAnlBlk_addr, "HA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			switch ( S1BUF) {
				case bbu2_addr:					// BB7&8 at TXG2Conn
					GetV24Version( BB7Item, bbu2_addr, PT8608);

					ConfigureAnlBlkUnit( BB7Item);
					ConfigureAnlBlkUnit( BB8Item);
					return;

				case bbu2_addr+0x40:     		// SDI-BLK7&8 at TXG2Conn
					if ( GetV24Version( 0, bbu2_addr, PT8609) == 0)
						ConfigureSDITSGUnit( 0);
					return;
			}

			//*****************************************************************
			// Option PT8631 Analog Test signal/Pattern Generator.
			// A preliminary test to see whether a PT8601 is installed MUST
			//  be performed.
			//*****************************************************************
			SndChar( DefaultAnlTPG_addr, "AA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu2_addr) {			// ANALOG-TPG2/5 at TXG2
				if (( SndChar( AnlSig_addr, "AI", 0) == V24Ok) || AnlTPGUnit[0].Present || SDITPGUnit[1].Present)
					ndx = 1;							// ANALOG-TPG5
				else
					ndx = 0;							// ANALOG-TPG2

				if ( GetV24Version( ndx, bbu2_addr, PT8631) == 0)
					ConfigureAnlTPGUnit( ndx);
				return;
			}

			//*****************************************************************
			// Option PT8633 Analog Test signal/Pattern Generator.
			//*****************************************************************
			SndChar( DefaultSDITPG_addr, "SA", conn_addr); // Unit Address Command

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu2_addr) {			// SDI-TPG2/5 at TXG2Conn
				if ( AnlTPGUnit[0].Present || SDITPGUnit[1].Present)
					ndx = 2;							// SDI-TPG5
				else
					ndx = 1;							// SDI-TPG2

				if ( GetV24Version( ndx, bbu2_addr, PT8633) == 0)
					ConfigureSDITPGUnit( ndx);
				return;
			}

			//*****************************************************************
			// Option PT8639 SDI Test Signal Generator.
			//*****************************************************************
			SndChar( DefaultSDITSG_addr, "HA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu2_addr+0x40) {	// SDI-TSG2 at TXG2Conn
				if ( GetV24Version( 0, bbu2_addr, PT8639) == 0)
					ConfigureSDITSGUnit( 0);
				return;
			}
			break;

		case bbu3_addr:
			//*****************************************************************
			// Option PT8608 Black Burst & PT8609 Serial Black.
			//*****************************************************************
			SndChar( DefaultAnlBlk_addr, "HA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			switch ( S1BUF) {
				case bbu3_addr:              	// BB5&6 at TXG4Conn
					GetV24Version( BB5Item, bbu3_addr, PT8608);

					ConfigureAnlBlkUnit( BB5Item);
					ConfigureAnlBlkUnit( BB6Item);
					return;

				case bbu3_addr+0x40:				// SDI-BLK5&6 at TXG4Conn
					if ( GetV24Version( 2, bbu3_addr, PT8609) == 0)
						ConfigureSDITSGUnit( 2);
					return;
			}

			//*****************************************************************
			// Option PT8639 SDI Test Signal Generator
			//*****************************************************************
			SndChar( DefaultSDITSG_addr, "HA", conn_addr);

			waitms( 10);							// Wait for an answer from the unit
			FeedTheWatchdog;

			if ( S1BUF == bbu3_addr+0x40) {	// SDI-TSG4 at TXG4Conn
				if ( GetV24Version( 2, bbu3_addr, PT8639) == 0)
					ConfigureSDITSGUnit( 2);
				return;
			}
	}
}

/***************************************************************************/
/* FindUnit																		 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980407	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Find the next/previous unit to describe/test etc.				*/
/*	Remarks:		Only options detected during power-up will be found.			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC FindUnit( UC dir, UC unit) {

	UC found;

	do {
		if ( dir == UP) {
			if ( unit++ == 21)
				unit = 0;
		}
		else
			if ( dir == DOWN) {
				if( unit-- == 0)
					unit = 21;
			}

		found = FALSE;

		switch ( unit ) {
			case 0:								// MainBoard
			case 1:								// OSCBoard
				found = TRUE;
				break;

			case 2:								// AnlBlkBurst
			case 4:
			case 6:
			case 8:
				if ( AnlBlkUnit[unit-2].Present)
					found = TRUE;
				break;

			case 10:								// AnlTPG
			case 11:
				if ( AnlTPGUnit[unit-10].Present)
					found = TRUE;
				break;

			case 12:								// SDITSG
			case 13:
			case 14:
				if ( SDITSGUnit[unit-12].Present)
					found = TRUE;
				break;

			case 15:								// SDITPG
			case 16:
			case 17:
				if ( SDITPGUnit[unit-15].Present)
					found = TRUE;
				break;

			case 18:								// AES-EBU
				if ( AESEBUUnit.Present)
					found = TRUE;
				break;

			case 19:								// SDI Genlock
				if ( SDIGenlockUnit.Present)
					found = TRUE;
				break;

			case 20:								// Internal 2
				if ( Int2GenlockUnit.Present)
					found = TRUE;
				break;

			case 21:								// TimeClock
				if ( TimeClockUnit.Present)
					found = TRUE;
				break;
		}
	} while ( !found);

	return( unit);
}

/***************************************************************************/
/*	FindNextTSGTPG																 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980204	    										*/
/* Revised:		980205				     													*/
/*																									*/
/*	Function:	Point to the next intalled TSG/TPG.									*/
/*	Remarks:		Since entry point could be in the middle of the array, the	*/
/*					test should be run twice, to ensure that the first elements	*/
/*					are tested, (it is actually a circular array).					*/
/*	Returns:		Next unit for the OUTPUT button.                            */
/*					If NO unit is detected 255	will be returned.						*/
/*	Updates:		--																				*/
/***************************************************************************/
UC FindNextTSGTPG( UC unit) {

	register UC i = 0;

	do {
		switch ( unit) {
			case AnlBlkItem:
				if ( AnlTPGUnit[0].Present)
					return( AnlTPG2Item);

			case AnlTPG2Item:
				if ( AnlTPGUnit[1].Present)
					return( AnlTPG5Item);

			case AnlTPG5Item:
				if ( SDITSGUnit[0].Present)
					return( SDITSG2Item);

			case SDITSG2Item:
				if ( SDITSGUnit[1].Present)
					return( SDITSG3Item);

			case SDITSG3Item:
				if ( SDITSGUnit[2].Present)
					return( SDITSG4Item);

			case SDITSG4Item:
				if ( SDITPGUnit[0].Present)
					return( SDITPG1Item);

			case SDITPG1Item:
				if ( SDITPGUnit[1].Present)
					return( SDITPG2Item);

			case SDITPG2Item:
				if ( SDITPGUnit[2].Present)
					return( SDITPG5Item);

			case SDITPG5Item:
				if ( AnlTPGUnit[0].Present)
					return( AnlTPG2Item);

			default:
				unit = AnlBlkItem;
		}									// If unit not found try once more,
	} while ( i++ < 1);				//  starting with the "first" unit.

	return( AnlBlkItem);
}

/***************************************************************************/
/*	ConfigureSPG																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Download calibration data to the SPG.								*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24//IIC specific error.	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureSPG() {

	UC error, lastError = 0;
	UL tmp;

	FeedTheWatchdog;

	tmp = IIC2RdLong( spgram_addr, HPHZero_G_addr);
	if ( IIC2ErrorCode)
		lastError = IIC2ErrorCode;
	else {
		if ( error = SndLong( sync_addr, "GV", tmp))
			lastError = error;
	}

	tmp = IIC2RdInt( spgram_addr, GenlPhase_G_addr);
	if ( IIC2ErrorCode)
		lastError = IIC2ErrorCode;
	else {
		if ( error = SndInt( sync_addr, "GF", (UI) tmp))
			lastError = error;
	}

	FeedTheWatchdog;

	tmp = IIC2RdLong( spgram_addr, HPHZero_M_addr);
	if ( IIC2ErrorCode)
		lastError = IIC2ErrorCode;
	else {
		if ( error = SndLong( sync_addr, "GY", tmp))
			lastError = error;
	}

	tmp = IIC2RdInt( spgram_addr, GenlPhase_M_addr);
	if ( IIC2ErrorCode)
		lastError = IIC2ErrorCode;
	else {
		if ( error = SndInt( sync_addr, "GG", (UI) tmp))
			lastError = error;
	}

	if ( error = GetIICVersion( IICBus2, spgram_addr, SPGUserText_addr+ProductKUNo_addr, &SPGBoard.KUNumber))
		lastError = error;
																// Get information
	tmp = RecInt( sync_addr, "GI", 0xFF, &error);

	if ( error) {
		lastError = error;
		tmp = 0;
	}

	SPGBoard.SWVersion = (UC) tmp;

	return( lastError ? ( OSCErrorType + lastError) : 0);
}

/***************************************************************************/
/*	ConfigureAnlBlkUnit														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980526				     													*/
/*																									*/
/*	Function:	Download all parameters to a Black Burst unit.					*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureAnlBlkUnit( UC ndx) {

	UC error;

	AnlBlkUnit[ndx].Present = TRUE;
															// Just validate delay
	AnlValidateSetting( AnlBlkUnit[ndx].HWType, ndx);

															// Transmit system/delay/ScHPhase
	error = TrxAnlBlkSystem( ndx, AnlBlkConfig[ndx].System,\
								 AnlBlkConfig[ndx].Delay, AnlBlkConfig[ndx].ScHPhase);

	return( error);
}

/***************************************************************************/
/*	ConfigureAnlTPGUnit														 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Download all parameters to an Analog-TPG.							*/
/*	Remarks:	  	The sequence of initialization should NOT be changed 'cause	*/
/*					the system update in the unit is VERY time consuming.			*/
/*					NOTE! When entering this function it is necessary to vali-	*/
/*					date the configuration, due to the possibilty of the user	*/
/*					having exchanged the unit e.g. a PT8631 with a PT8601,		*/
/*					which might make some of the configurations invalid.			*/
/*					NOTE! When validating the TIME/DATE information, the PT8637	*/
/*					has NOT yet been intialized, hence it is NOT possible to		*/
/*					test the TimeClockUnit.nn variables.								*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureAnlTPGUnit( UC ndx) {

	UC i, error, lastError = 0, pattern;

	AnlTPGUnit[ndx].Present = TRUE;
																// Validate the configuration
	AnlValidateSetting( AnlTPGUnit[ndx].HWType, ndx);

	pattern = AnlTPGConfig[ndx].Pattern;			// Get validated pattern

											 					// If TCI (PT8637) NOT present..
	if ( !IIC1Write( time1_addr, TimeClockConfig.TCIControl))
		for ( i = 0; i < 2; i++)						// Clear possible TCI settings
			AnlTPGConfig[ndx].TextStyle[i] &= 0x0F;

																// Transmit system/delay
	if ( error = TrxAnlTPGDelay( ndx, AnlTPGConfig[ndx].System, AnlTPGConfig[ndx].Delay))
		lastError = error;

	waitms( 5);												//
																// Transmit pattern/attribute
	if ( error = TrxAnlTPGPattern( ndx, pattern, AnlTPGAttrib( ndx, pattern)))
		lastError = error;

	waitms( 5);												//
																// Transmit ScHPhase
	if ( error = TrxAnlTPGScHPhase( ndx, AnlTPGConfig[ndx].ScHPhase))
		lastError = error;

	waitms( 5);												//
																// Configure text setting
	if ( error = ConfigureAnlTPGText( ndx, pattern))
		lastError = error;

	return( lastError);
}

/***************************************************************************/
/*	ConfigureAnlTPGText														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980304	    										*/
/* Revised:		000608			     														*/
/*																									*/
/*	Function:	Download all text parameters to an Analog-TPG.					*/
/*	Remarks:	  	The sequence of initialization should NOT be changed 'cause	*/
/*					the system update in the unit is VERY time consuming.			*/
/*					The function will differ between the PT8601 & PT8631 units.	*/
/*					The function will stop transmitting immediately an error	is */
/*					found.																		*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureAnlTPGText( UC ndx, UC pattern) {

	register UC i;
	UC error, tmp, type = AnlFindPatternType( pattern);

	if ( AnlTPGUnit[ndx].HWType == PT8601) {
		error = TrxAnlTPGTextLineEnable( ndx, 1,
										( AnlTPGConfig[ndx].TextEnable & 0x01));

		waitms( 5);										//

		if ( !error)
			error = TrxAnlTPGText( ndx, 1, AnlTPGConfig[ndx].Text[0]);
	}
	else {
															// Transmit text style
		error = TrxAnlTPGTextStyle( ndx, pattern, AnlTPGConfig[ndx].TextStyle[type]);

		for ( i = 0; i < ( 3 - type); i++) {	// Transmit text information
			tmp = 3*type + i;

			waitms( 10);								//

			if ( !error)                   	  // Transmit text line enable
				error = TrxAnlTPGTextLineEnable( ndx, i+1,\
								( AnlTPGConfig[ndx].TextEnable >> tmp) & 0x01);

			waitms( 5);									//

			if ( !error) 								// Transmit text line string
				error = TrxAnlTPGText( ndx, i+1, AnlTPGConfig[ndx].Text[tmp]);
		}
	}

	return( error);
}

/***************************************************************************/
/*	ConfigureSDITSGUnit														 UNIT_DRV.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980526				     													*/
/*																									*/
/*	Function:	Download all parameters to a SDI-TSG, (former B/CB).			*/
/*	Remarks:	  	The sequence of initialization should NOT be changed 'cause	*/
/*					the system update in the unit is VERY time consuming.			*/
/*					The function will differ between the PT8609 & PT8639 units.	*/
/*					The function will stop transmitting immediately an error	is */
/*					found.																		*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		-																				*/
/***************************************************************************/
UC ConfigureSDITSGUnit( UC ndx) {

	UC error, lastError = 0;

	SDITSGUnit[ndx].Present = TRUE;
																// Validate the configuration
	SDIValidateSetting( SDITSGUnit[ndx].HWType, ndx);

																// Transmit system/delay
	if ( error = TrxSDITSGDelay( ndx, SDITSGConfig[ndx].System, SDITSGConfig[ndx].Delay))
		lastError = error;

	waitms( 5);
																// Transmit pattern
	if ( error = TrxSDITSGPattern( ndx, SDITSGConfig[ndx].Pattern))
		lastError = error;

	waitms( 5);
																// Transmit EDH insertion
	if ( error = TrxSDITSGEDH( ndx, SDITSGConfig[ndx].EDH))
		lastError = error;

	waitms( 5);
																// Transmit audio signal
	if ( error = TrxSDITSGAudioSignal( ndx, SDITSGConfig[ndx].AudioSignal))
		lastError = error;


	if ( SDITSGUnit[ndx].HWType == PT8639)	{		// If PT8639..
		waitms( 5);
																// Transmit audio level
		if ( error = TrxSDITSGAudioLevel( ndx, SDITSGConfig[ndx].AudioLevel))
			lastError = error;
	}

	return( lastError);
}

/***************************************************************************/
/*	ConfigureSDITPGUnit														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		000608			     														*/
/*																									*/
/*	Function:	Download all parameters to a SDI-TPG.								*/
/*	Remarks:	  	The sequence of initialization should NOT be changed 'cause	*/
/*					the system update in the unit is VERY time consuming.			*/
/*					NOTE! When entering this function it is necessary to vali-	*/
/*					date the configuration, due to the possibilty of the user	*/
/*					having exchanged the unit e.g. a PT8633 with a PT8602, 		*/
/*					which might make some of the configurations invalid.			*/
/*					The function will stop transmitting immediately an error		*/
/*					occurs.																		*/
/*					NOTE! When validating the TIME/DATE information, the PT8637	*/
/*					has NOT yet been intialized, hence it is NOT possible to		*/
/*					test the TimeClockUnit.nn variables.								*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureSDITPGUnit( UC ndx) {

	UC i, error, lastError = 0, pattern;

	SDITPGUnit[ndx].Present = TRUE;
																// Validate the configuration
	SDIValidateSetting( SDITPGUnit[ndx].HWType, ndx);

	pattern = SDITPGConfig[ndx].Pattern;			// Get validated pattern

											 					// If TCI (PT8637) NOT present..
	if ( !IIC1Write( time1_addr, TimeClockConfig.TCIControl))
		for ( i = 0; i < 2; i++)						// Clear possible TCI settings
			SDITPGConfig[ndx].TextStyle[i] &= 0x0F;

																// Transmit system/delay
	if ( error = TrxSDITPGDelay( ndx, SDITPGConfig[ndx].System, SDITPGConfig[ndx].Delay))
		lastError = error;

	waitms(300);
																// Transmit pattern/attribute
	if ( error = TrxSDITPGPattern( ndx, pattern, SDITPGAttrib( ndx, pattern)))
		lastError = error;

	waitms(300);
																// Configure text setting
	if ( error = ConfigureSDITPGText( ndx, pattern))
		lastError = error;

	waitms(300);
																// Transmit EDH insertion
	if ( error = TrxSDITPGEDH( ndx, SDITPGConfig[ndx].EDH))
		lastError = error;

	waitms(300);
																// Transmit audio signal
	if ( error = TrxSDITPGAudioSignal( ndx, SDITPGConfig[ndx].AudioSignal))
		lastError = error;

	waitms(300);
																// Transmit audio level
	if ( error = TrxSDITPGAudioLevel( ndx, SDITPGConfig[ndx].AudioLevel))
		lastError = error;

	if ( SDITPGUnit[ndx].HWType == PT8633)	{		// If PT8633..
	waitms(300);
																// Transmit audio group
		if ( error = TrxSDITPGAudioGroup( ndx, SDITPGConfig[ndx].AudioGroup))
			lastError = error;
	}

	return( lastError);
}

/***************************************************************************/
/*	ConfigureSDITPGText														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980304	    										*/
/* Revised:		000608			     														*/
/*																									*/
/*	Function:	Download all text parameters to a SDI-TPG.						*/
/*	Remarks:	  	The sequence of initialization should NOT be changed 'cause	*/
/*					the system update in the unit is VERY time consuming.			*/
/*					The function will differ between the PT860X & PT863X units.	*/
/*					The function will stop transmitting immediately an error	is */
/*					found.																		*/
/*	Returns:		System error code, i.e. type incl. IIC specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigureSDITPGText( UC ndx, UC pattern) {

	register UC i;
	UC error=0, tmp, type = SDIFindPatternType( pattern);

	switch ( SDITPGUnit[ndx].HWType) {
		case PT8602:
		case PT8603:
			error = TrxSDITPGTextLineEnable( ndx, 1,
											( SDITPGConfig[ndx].TextEnable & 0x01));

			waitms( 5);									//

			if ( !error)
				error = TrxSDITPGText( ndx, 1, SDITPGConfig[ndx].Text[0]);
			break;

		default:
															// Transmit text style
			if ( !error) 
				error = TrxSDITPGTextStyle( ndx, pattern, SDITPGConfig[ndx].TextStyle[type]);

															// Transmit text information
			for ( i = 0; i < ( 3 - type); i++)
			{
				tmp = 3*type + i;

				waitms( 10);							//

				if ( !error)                     // Transmit text line enable
					error = TrxSDITPGTextLineEnable( ndx, i+1,\
									( SDITPGConfig[ndx].TextEnable >> tmp) & 0x01);

				waitms( 5);								//

				if ( !error) 							// Transmit text line string
					error = TrxSDITPGText( ndx, i+1, SDITPGConfig[ndx].Text[tmp]);
			}
			break;
	}

	return( error);
}

/***************************************************************************/
/*	AESEBUUnitDrv																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980216	    										*/
/* Revised:		980511				     													*/
/*																									*/
/*	Function:	Configure the AES/EBU generator.										*/
/*	Remarks:		AESEBU1:																		*/
/*				  	Level : 3;						(o)                              */
/*					Signal : 4;						(o)                              */
/*					Lock27MHz : 1;					(i): 1: PLL locked               */
/*					Timing : 3;						(o):										*/
/*					Vidlock : 1;					(i): 1: Video locked to audio    */
/*					Level BNC Generator1 : 1;	(i): 1: Level OK  					*/
/*					Level BNC Generator2 : 1;	(i): 1: Level OK  					*/
/*					Level XLR Generator1 : 1;	(i): 1: Level OK  					*/
/*					Not used : 1;			   	(i): 										*/
/*																									*/
/*					AESEBU2:																		*/
/*				  	Level : 3;						(o)                              */
/*					Signal : 4;						(o)                              */
/*					Not used : 1;					(i):										*/
/*					Timing : 3;						(o):										*/
/*					Vidlock : 1;					(i): 1: Video locked to audio    */
/*					Level XLR Generator2 : 1;	(i): 1: Level OK  					*/
/*					Level BNC Wordclock1 : 1;	(i): 1: Level OK  					*/
/*					Level BNC Wordclock2 : 1;	(i): 1: Level OK  					*/
/*					Signal X2 : 1;			  		(o): 0: Board is in PT5230			*/
/*																									*/
/*					This function always attempts to update BOTH IIC ports,		*/
/*					even though the first initialized fails.							*/
/*	Returns:		System error code, i.e. type incl. IIC specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC AESEBUUnitDrv( UC ndx, UC signal, UC level, UC timing) {

	UI port;
	UC error = 0, addr1, addr2;

	FeedTheWatchdog;

	port = AESEBUSignalTable[signal] + 0xF880;

	if ( signal != AESEBUWordClock)
		port |= AESEBULevelTable[level] + AESEBUTimingTable[timing];

	if ( ndx == AESEBU1Item) {
		addr1 = sound1_addr;
		addr2 = sound2_addr;
	}
	else {
		addr1 = sound3_addr;
		addr2 = sound4_addr;

		port &= 0x7FFF;									// Clear PT5230 bit
	}

	if ( !IIC2Write( addr1, ( UC) port))
		error = IIC2ErrorCode;

	if( !IIC2Write( addr2, ( UC)( port >> 8)))
		error = IIC2ErrorCode;

	return( error ? ( AESEBUErrorType + error) : 0);
}

/***************************************************************************/
/*	GenlockUnitDrv																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961010	    										*/
/* Revised:		980907				     													*/
/*																									*/
/*	Function:	Configure the SPG to handle the chosen genlock.					*/
/*	Remarks:		Only a possible LAST detected error will be reported.			*/
/*	Returns:		System error code, i.e. type incl. V24/IIC specific error.	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC GenlockUnitDrv( UC GenlockInput, UC GenlockSystem) {

	UC error, lastError = 0, errorCnt = 0, mode, port, tmp;

	FeedTheWatchdog;

	port = GenlockInputTable[tmp = 2*GenlockInput];
	mode = GenlockInputTable[++tmp];

	if ( GenlockInput < Internal) {
		port |= GenlockSystemTable[tmp = 2*GenlockSystem];
		mode = GenlockSystemTable[++tmp];
	}

	ud5 = port;

	if ( TimeClockUnit.Present) 					// If timeclock unit present..
		switch ( TimeClockUnit.HWType) {
			case PT8607:								// Timecode unit..

				if ( GenlockInput < SDIGenlock) {// If external analog genlocked..

					if ( GenlockSystem == BurstLockPAL)
						tmp = 0xFF;
					else
						tmp = 0xFE;

					if ( !IIC1Write( time_addr, tmp)) {
						lastError = TimeClockErrorType + IIC1ErrorCode;
						errorCnt++;
					}
				}
				break;

			case PT8637:								// Time Clock Interface
															// Force unlocked status AND PAL
				TimeClockConfig.TCIControl &= 0xFC;

				if (( GenlockSystem == BurstLockNTSC) || ( GenlockSystem == SyncLock525))
					TimeClockConfig.TCIControl |= 0x02;
			
				TrxTimeClockControl( TimeClockConfig.TCIControl);
				break;
		}
															// Update UNLOCKED LED status
	UnlockedLED = !( ActiveGenlockNo == Internal);

	if ( error = UpdateLEDs()) {					// Update front plate LEDs
		lastError = error;
		errorCnt++;
	}

	FeedTheWatchdog;

															// Update the OSC board
	if ( error = SndChar( sync_addr, "GM", mode)) {
		lastError = OSCErrorType + error;
		errorCnt++;
	}

	if ( error = TrxGenlockDelay( GenlockConfig[ExternalGenlockNo].Delay)) {
		lastError = error;
		errorCnt++;
	}

	if ( errorCnt > 1) 								// Multiple Errors detected
	  return( SystemErrorType + MultipleError);

	return( lastError);
}

/***************************************************************************/
/*	BroadcastTCITime															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980827	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Read and broadcast time/date from a PT8637 Time Clock 		*/
/*					Interface.																	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC BroadcastTCITime() {
												// Get TCI time information
	TimeClockConfig.TCIControl &= 0x1F;
	TimeClockConfig.TCIControl |= ( TCITimeRegister << 5);
	IIC1Write( time1_addr, TimeClockConfig.TCIControl);

	FeedTheWatchdog;

	IIC1ReadN( time1_addr, 4, (UC*) tmpBuffer);

	TimeClockConfig.Time = tmpBuffer[1];
	TimeClockConfig.Time += ((UL) tmpBuffer[2] << 8);
	TimeClockConfig.Time += ((UL) tmpBuffer[3] << 16);

												// Convert time to user format
	TimeFormat(( TimeClockConfig.Format & 0x01),\
								TimeClockConfig.Time, 1, tmpBuffer);
			
												// Broadcast time information
	SndBroadcast( "BT", tmpBuffer);

	FeedTheWatchdog;

												// Get TCI date information
	TimeClockConfig.TCIControl &= 0x1F;
	TimeClockConfig.TCIControl |= ( TCIDateRegister << 5);
	IIC1Write( time1_addr, TimeClockConfig.TCIControl);

	FeedTheWatchdog;

	IIC1ReadN( time1_addr, 4, (UC*) tmpBuffer);

	TimeClockConfig.Date = tmpBuffer[1];
	TimeClockConfig.Date += ((UL) tmpBuffer[2] << 8);
	TimeClockConfig.Date += ((UL) tmpBuffer[3] << 16);

												// Convert date to user format
	DateFormat(( TimeClockConfig.Format >> 1),\
								TimeClockConfig.Date, 1, tmpBuffer);
		
												// Broadcast date information
	SndBroadcast( "BD", tmpBuffer);

	return( 0);
}

/***************************************************************************/
/*	TrxAnlBlkSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980428	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit a system setup to a Black Burst unit.					*/
/*	Remarks:	  	Although the ScHPhase are not dependant of the system, it 	*/
/*					MUST be transmitted after a system change, due to the			*/
/*					nature of the Black Burst unit.										*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlBlkSystem( UC ndx, UC system, UL delay, int ScHPhase) {

	UC error;

	error = TrxAnlBlkDelay( ndx, system, delay);	// Transmit system/delay

	waitms( 5);

	if ( !error)											// Transmit ScH-Phase
		error = TrxAnlBlkScHPhase( ndx, ScHPhase);

	return( error);
}

/***************************************************************************/
/*	TrxAnlBlkDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit system/delay to a Black Burst unit.						*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlBlkDelay( UC ndx, UC system, UL delay) {

	UC error;
	char V24char;

	FeedTheWatchdog;

	( ndx & 0x01) ? ( V24char = 'J') : ( V24char = 'H');

	cmdBuffer[0] = V24char;

	switch ( system) {
		case PAL_PALID:
			V24char = 'K';
			break;

		case NTSC:
			V24char = 'M';
			break;

		case NTSCwoSetup:
			V24char = 'L';
			break;

		default:								// PAL et.al
			V24char = 'G';
			break;
	}

	cmdBuffer[1] = V24char;

	error = SndLong( AnlBlkUnit[ndx].Address, cmdBuffer, delay);

	return( error ? ( AnlBlkErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlBlkScHPhase															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Convert and transmit a ScH-Phase to a Black Burst unit.		*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlBlkScHPhase( UC ndx, int ScHPhase) {

	UC error;
	char V24char;
	long tmp = ScHPhase;

	FeedTheWatchdog;

	( ndx & 0x01) ? ( V24char = 'J') : ( V24char = 'H');

	cmdBuffer[0] = V24char;
	cmdBuffer[1] = 'H';

	if ( ScHPhase < 0)				// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;							// tmp = ( ScHPhase*2048)/360;

	error = SndInt( AnlBlkUnit[ndx].Address, cmdBuffer, (UI) tmp);

	return( error ? ( AnlBlkErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980428	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Transmit a system setup to an Analog-TPG.							*/
/*	Remarks:	  	Only parameters influenced by the system are transmitted,	*/
/*					i.e. delay and the pattern. It is for the calling function 	*/
/*					to ensure that the pattern is valid in the selected system.	*/
/*					Pattern should be transmitted first in case the current		*/
/*					pattern is invalid in the new system.								*/
/*					NOTE! It is not necessary to transmit the ScHPhase.			*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGSystem( UC ndx, UC system, UL delay, UC pattern, UC attrib) {

	UC error;
					 											// Transmit system/delay
	error = TrxAnlTPGDelay( ndx, system, delay);

	waitms( 5);

	if ( !error)											// Transmit pattern/attribute
		error = TrxAnlTPGPattern( ndx, pattern, attrib);

	waitms( 5);

	if ( !error) 											// Transmit text setting
		error = ConfigureAnlTPGText( ndx, pattern);

	return( error);
}

/***************************************************************************/
/*	TrxAnlTPGDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit system and delay to an Analog-TPG.						*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGDelay( UC ndx, UC system, UL delay) {

	UC error;
	char V24char;

	FeedTheWatchdog;

	switch ( system) {
		case PAL_PALID:
			V24char = 'K';
			break;

		case NTSC:
			V24char = 'M';
			break;

		case NTSCwoSetup:
			V24char = 'L';
			break;

		default:								// PAL et.al
			V24char = 'G';
			break;
	}

	cmdBuffer[0] = 'A';
	cmdBuffer[1] = V24char;

	error = SndLong( AnlTPGUnit[ndx].Address, cmdBuffer, delay);

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGPattern															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit pattern and attribute to an Analog-TPG.				*/
/*	Remarks:	  	The argument <attrib> is not specified for PT8601.				*/
/*					The pattern MUST be valid when calling this function.			*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGPattern( UC ndx, UC pattern, UC attrib) {

	UC error, HWtype = AnlTPGUnit[ndx].HWType;

	FeedTheWatchdog;

															// Lookup analog pattern code
	pattern = FindPatternTable( HWtype)[pattern];

	if ( HWtype == PT8601)
		error = SndInt( AnlTPGUnit[ndx].Address, "AP", pattern);
	else
		error = SndInt2( AnlTPGUnit[ndx].Address, "AP", pattern, attrib);

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGScHPhase															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Convert and transmit a ScH-Phase to an Analog-TPG.				*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGScHPhase( UC ndx, int ScHPhase) {

	UC error;
	long tmp = ScHPhase;

	FeedTheWatchdog;

	if ( ScHPhase < 0)				// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;							// tmp = ( ScHPhase*2048)/360;

	error = SndInt( AnlTPGUnit[ndx].Address, "AH", ( UI) tmp);

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGTextStyle														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980301	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Transmit user text style to an Analog-TPG.						*/
/*	Remarks:	  	The function is not specified for PT8601.							*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGTextStyle( UC ndx, UC pattern, UC style) {

	UC error, type = AnlFindPatternType( pattern);

	FeedTheWatchdog;

	error = SndChar( AnlTPGUnit[ndx].Address, "AU", ( style & 0xF0) + TPGStandardTextStyle);

	waitms( 5);

	error = SndChar( AnlTPGUnit[ndx].Address, "AU", style);

	if (( style & 0x0F)== TPGFreeTextStyle)
	{
		error = TrxAnlTPGTextPos( ndx, style,
										  AnlTPGConfig[ndx].TextPos[type][0],
										  AnlTPGConfig[ndx].TextPos[type][1]);
	}

	if ( style & 0xF0)
	{
		switch ( pattern)
		{
			case AnlPhilips4x3:
			case AnlPhilips16x9:
				if ( AnlTPGConfig[ndx].System < NTSC)
					error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 3, 0, 159);
				else
					error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 3, 0, 135);
				break;

			case AnlFuBK4x3:
			case AnlFuBK16x9:
				error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 3, 0, 268);
				break;

			default:
				if ( AnlTPGConfig[ndx].System < NTSC)
					error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 3, 0, 287);
				else
					error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 3, 0, 241);
				break;
		}
	}
	else
	{
		if ( type == ComplexPattern)

			// If NO TIME/CLOCK information, ALWAYS disable text line 3.

			if ( !( style & 0xF0))
				error = SndInt2( AnlTPGUnit[ndx].Address, "AF", 3, 0);
	}

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGTextPos															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 000509	    										*/
/* Revised:		000608			     														*/
/*																									*/
/*	Function:	Transmit user text position to a Analog-TPG.						*/
/*	Remarks:	  	This function is not specified for PT8601.						*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGTextPos( UC ndx, UC style, UC Xpos, UC Ypos)
{
	UC error, tmp;

	FeedTheWatchdog;

	if ( AnlTPGConfig[ndx].System < NTSC)
		tmp = 22*Ypos+9;
	else
		tmp = 18*Ypos+7;

	error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 1, 15+Xpos*2, tmp);

	if ( !error)
		error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 2, 15+Xpos*2, tmp+18);

	FeedTheWatchdog;

	if ( !error && !( style & 0xF0))
		error = SndInt3( AnlTPGUnit[ndx].Address, "AJ", 3, 15+Xpos*2, tmp+36);

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGTextLineEnable													 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980301	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit text line enable to an Analog-TPG.						*/
/*	Remarks:	  	The argument <no> is not specified for PT8601, i.e. there	*/
/*					is only one text line to enable in PT8601.						*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGTextLineEnable( UC ndx, UC no, UC state) {

	UC error;

	FeedTheWatchdog;

	if ( AnlTPGUnit[ndx].HWType == PT8601)
		error = SndInt( AnlTPGUnit[ndx].Address, "AF", state);
	else
		error = SndInt2( AnlTPGUnit[ndx].Address, "AF", no, state);

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxAnlTPGText																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980301	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit user text to a Analog-TPG.									*/
/*	Remarks:	  	The argument <no> is not specified for PT8601, i.e. there	*/
/*					is only one text line in PT8601.	For the PT8601 the text-	*/
/*					line will be converted to fixed 8 character width.				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxAnlTPGText( UC ndx, UC no, char* text) {

	register UC i;
	UC error;
	char tmp;

	FeedTheWatchdog;

	if ( AnlTPGUnit[ndx].HWType == PT8601) {
		i = 0;
		while (( tmp = text[i]) && ( i < 8))
			tmpBuffer[i++] = tmp;

		while ( i < 8)
			tmpBuffer[i++] = ' ';

		tmpBuffer[8] = '\0';

		error = SndAsc( AnlTPGUnit[ndx].Address, "AN", 0xFF, tmpBuffer);
	}
	else
		error = SndAsc( AnlTPGUnit[ndx].Address, "AN", no, text);

	return( error ? ( AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITSGSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980504	    										*/
/* Revised:		980514				     													*/
/*																									*/
/*	Function:	Transmit a system setup to a SDI-TSG.								*/
/*	Remarks:	  	Only parameters influenced by the system are transmitted,	*/
/*					i.e. delay and the pattern. It is for the calling function 	*/
/*					to ensure that the pattern is valid in the selected system.	*/
/*					Then pattern is transmitted LAST to be compatible with the	*/
/*					PT8609.																		*/
/*	Returns:		System error code, i.e. type incl. V24 specific error			*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITSGSystem( UC ndx, UC system, UL delay, UC pattern) {

	UC error;

	error = TrxSDITSGDelay( ndx, system, delay);	// Transmit system/delay

	waitms( 5);

	if ( !error) 											// Transmit pattern
		error = TrxSDITSGPattern( ndx, pattern);

	return ( error);
}

/***************************************************************************/
/*	TrxSDITSGDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit system and delay to a SDI-TSG.							*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITSGDelay( UC ndx, UC system, UL delay) {

	UC error, V24char;

	FeedTheWatchdog;

	( system == SDI625) ? ( V24char = 'G') : ( V24char = 'M');

	cmdBuffer[0] = 'H';
	cmdBuffer[1] = V24char;

	error = SndLong( SDITSGUnit[ndx].Address, cmdBuffer, delay);

	return( error ? ( SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITSGPattern															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit a pattern to a SDI-TSG.										*/
/*	Remarks:		The pattern MUST be valid when calling this function.			*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITSGPattern( UC ndx, UC pattern) {

	UC error;

	FeedTheWatchdog;
															// Lookup pattern code
	pattern = FindPatternTable( SDITSGUnit[ndx].HWType)[pattern];

	error = SndChar( SDITSGUnit[ndx].Address, "HP", pattern);

	return( error ? ( SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITSGEDH																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit	EDH on/off to a SDI-TSG.									*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITSGEDH( UC ndx, UC state) {

	UC error;

	FeedTheWatchdog;

	error = SndChar( SDITSGUnit[ndx].Address, "HE", state);

	return( error ? ( SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITSGAudioSignal														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit embedded audio signal to a SDI-TSG.						*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITSGAudioSignal( UC ndx, UC signal) {

	UC error;

	FeedTheWatchdog;

	error = SndChar( SDITSGUnit[ndx].Address, "HU", signal);

	return( error ? ( SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITSGAudioLevel														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit embedded audio level to a SDI-TSG.						*/
/*	Remarks:	  	This function is not specified for PT8609.						*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITSGAudioLevel( UC ndx, UC level) {

	UC error;

	FeedTheWatchdog;

	error = SndChar( SDITSGUnit[ndx].Address, "HL", level);

	return( error ? ( SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGSystem															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980429	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Transmit a system setup to a SDI-TPG.								*/
/*	Remarks:	  	Only parameters influenced by the system are transmitted,	*/
/*					i.e. delay and the pattern. It is for the calling function 	*/
/*					to ensure that the pattern is valid in the selected system.	*/
/*					Pattern should be transmitted first in case the current		*/
/*					pattern is invalid in the new system.								*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGSystem( UC ndx, UC system, UL delay, UC pattern, UC attrib) {

	UC error;

	error = TrxSDITPGDelay( ndx, system, delay);	// Transmit system/delay

	waitms( 5);

	if ( !error) 											// Transmit pattern/attribute
		error = TrxSDITPGPattern( ndx, pattern, attrib);

	waitms( 5);

	if ( !error) 											// Transmit text setting
		error = ConfigureSDITPGText( ndx, pattern);

	return ( error);
}

/***************************************************************************/
/*	TrxSDITPGDelay																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit delay to a SDI-TPG.											*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGDelay( UC ndx, UC system, UL delay) {

	UC error, V24char;

	FeedTheWatchdog;

	( system == SDI625) ? ( V24char = 'G') : ( V24char = 'M');

	cmdBuffer[0] = 'S';
	cmdBuffer[1] = V24char;

	error = SndLong( SDITPGUnit[ndx].Address, cmdBuffer, delay);

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGPattern															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit pattern and attribute to a SDI-TPG.						*/
/*	Remarks:	  	The argument <attrib> is not specified for PT8602/03.			*/
/*					The pattern MUST be valid when calling this function.			*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGPattern( UC ndx, UC pattern, UC attrib) {

	UC	error, HWtype = SDITPGUnit[ndx].HWType;

	FeedTheWatchdog;
															// Lookup digital pattern code
	pattern = FindPatternTable( HWtype)[pattern];

	switch ( HWtype) {
		case PT8602:
		case PT8603:
			error = SndInt( SDITPGUnit[ndx].Address, "SP", pattern);
			break;

		default:
			error = SndInt2( SDITPGUnit[ndx].Address, "SP", pattern, attrib);
			break;
	}

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGEDH																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit EDH on/off to a SDI-TPG.									*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGEDH( UC ndx, UC state) {

	UC error;

	FeedTheWatchdog;

	error = SndChar( SDITPGUnit[ndx].Address, "SE", state);

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGTextStyle														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		000608			     														*/
/*																									*/
/*	Function:	Transmit user text style to a SDI-TPG.								*/
/*	Remarks:	  	This function is not specified for PT8602/03.					*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGTextStyle( UC ndx, UC pattern, UC style) {

	UC error, type = SDIFindPatternType( pattern);

	FeedTheWatchdog;

	error = SndChar( SDITPGUnit[ndx].Address, "SD", ( style & 0xF0) + TPGStandardTextStyle);

	waitms( 5);

	error = SndChar( SDITPGUnit[ndx].Address, "SD", style);

	if (( style & 0x0F)== TPGFreeTextStyle)
	{
		error = TrxSDITPGTextPos( ndx, style, 
										  SDITPGConfig[ndx].TextPos[type][0],
										  SDITPGConfig[ndx].TextPos[type][1]);
	}

	if ( style & 0xF0)
	{
		switch ( pattern)
		{
			case SDIPhilips4x3:
			case SDIPhilips16x9:
				if ( SDITPGConfig[ndx].System == SDI525)
					error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 3, 0, 134);
				else
					error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 3, 0, 158);
				break;

			case SDIFuBK4x3:
			case SDIFuBK16x9:
				error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 3, 0, 268);
				break;

			default:
				if ( SDITPGConfig[ndx].System == SDI525)
					error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 3, 0, 240);
				else
					error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 3, 0, 286);
				break;
		}
	}
	else
	{
		if ( type == ComplexPattern)

			// If NO TIME/CLOCK information, ALWAYS disable text line 3.

			if ( !( style & 0xF0))
				error = SndInt2( SDITPGUnit[ndx].Address, "SF", 3, 0);
	}

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGTextPos															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000608			     														*/
/*																									*/
/*	Function:	Transmit user text position to a SDI-TPG.							*/
/*	Remarks:	  	This function is not specified for PT8602/03.					*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGTextPos( UC ndx, UC style, UC Xpos, UC Ypos)
{
	UC error, tmp;

	FeedTheWatchdog;

	if ( SDITPGConfig[ndx].System == SDI625)
		tmp = 22*Ypos+8;
	else
		tmp = 18*Ypos+6;

	error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 1, 4+Xpos*2, tmp);

	if ( !error)
		error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 2, 4+Xpos*2, tmp+18);

	if ( !error && !( style & 0xF0))
		error = SndInt3( SDITPGUnit[ndx].Address, "SJ", 3, 4+Xpos*2, tmp+36);

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGTextLineEnable													 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit text line enable to a SDI-TPG.							*/
/*	Remarks:	  	The argument <no> is not specified in PT8602/03, i.e. there	*/
/*					is only one text line to enable in PT8602/03.					*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGTextLineEnable( UC ndx, UC no, UC state) {

	UC error;

	FeedTheWatchdog;

	switch ( SDITPGUnit[ndx].HWType) {
		case PT8602:
		case PT8603:
			error = SndInt( SDITPGUnit[ndx].Address, "SF", state);
			break;

		default:
			error = SndInt2( SDITPGUnit[ndx].Address, "SF", no, state);
			break;
	}

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGText																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit user text to a SDI-TPG.										*/
/*	Remarks:	  	The argument <no> is not used in PT8602/03, i.e. there is   */
/*					only one text line in PT8602/03.	For the PT8602/03 the 		*/
/*					text-line will be converted to fixed 8 character width.		*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGText( UC ndx, UC no, char* text) {

	register UC i;
	UC error;
	char tmp;

	FeedTheWatchdog;

	switch ( SDITPGUnit[ndx].HWType) {
		case PT8602:
		case PT8603:
			i = 0;
			while (( tmp = text[i]) && ( i < 8))
				tmpBuffer[i++] = tmp;

			while ( i < 8)
				tmpBuffer[i++] = ' ';

			tmpBuffer[8] = '\0';

			error = SndAsc( SDITPGUnit[ndx].Address, "SN", 0xFF, tmpBuffer);
			break;

		default:
			error = SndAsc( SDITPGUnit[ndx].Address, "SN", no, text);
			break;
	}

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGAudioSignal														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit embedded audio signal to a SDI-TPG.						*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGAudioSignal( UC ndx, UC signal) {

	UC error;

	FeedTheWatchdog;

	error = SndChar( SDITPGUnit[ndx].Address, "SU", signal);

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGAudioLevel														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit embedded audio level to a SDI-TPG.			 			*/
/*	Remarks:		The audio level MUST be valid when calling this function.	*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGAudioLevel( UC ndx, UC level) {

	UC error, HWtype = SDITPGUnit[ndx].HWType;

	FeedTheWatchdog;

	level = FindAudioLevelTable( HWtype)[level];

	error = SndChar( SDITPGUnit[ndx].Address, "SL", level);

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxSDITPGAudioGroup														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit embedded audio group to a SDI-TPG.			 			*/
/*	Remarks:	  	This function is only used for PT8633.								*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxSDITPGAudioGroup( UC ndx, UC group) {

	UC error;

	FeedTheWatchdog;

	error = SndChar( SDITPGUnit[ndx].Address, "SC", group);

	return( error ? ( SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*	TrxTimeClockControl														 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980708	    										*/
/* Revised:		980722				     													*/
/*																									*/
/*	Function:	Transmit control information to the Time Clock Interface.	*/
/*	Remarks:	  	--																				*/
/*	Returns:		System error code, i.e. type incl. IIC specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxTimeClockControl( UC control) {

	FeedTheWatchdog;

	IIC1Write( time1_addr, control);

	return( IIC1ErrorCode ? ( TimeClockErrorType + IIC1ErrorCode) : 0);
}

/***************************************************************************/
/*	TrxTimeClockData															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980708	    										*/
/* Revised:		980722				     													*/
/*																									*/
/*	Function:	Transmit data information to the Time Clock Interface.		*/
/*	Remarks:	  	No of bytes should be restricted to 4.								*/
/*	Returns:		System error code, i.e. type incl. IIC specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxTimeClockData( UC TCIControl, UC noOfBytes, UL val) {

	UC i, tmp[5];

	FeedTheWatchdog;

	tmp[0] = TCIControl;

	for ( i = 1; i <= noOfBytes; i++) {
		tmp[i] = val & 0x000000FF;
		val >>= 8;
	}

	IIC1WriteN( time1_addr, noOfBytes+1, tmp);

	FeedTheWatchdog;

	return( IIC1ErrorCode ? ( TimeClockErrorType + IIC1ErrorCode) : 0);
}

/***************************************************************************/
/*	TrxGenlockDelay															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Transmit genlock offset to ALL of the installed units.		*/
/*	Remarks:	  	This function will try to transmit the offset to ALL units	*/
/*					even if one or more units fails.										*/
/*	Returns:		System error code, i.e. type incl. V24 specific error.		*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TrxGenlockDelay( UL delay) {

	register UC i;
	UC error, errorCnt = 0, lastError = 0;

	FeedTheWatchdog;

	for ( i = 0; i < MaxAnlBlkUnits; i++)
		if ( AnlBlkUnit[i].Present) {
			if ( i % 2)
				error = SndLong( AnlBlkUnit[i].Address, "JO", delay);
			else
				error = SndLong( AnlBlkUnit[i].Address, "HO", delay);

			if ( error) {
				lastError = AnlBlkErrorType + error;
				errorCnt++;
			}
		}

	FeedTheWatchdog;

	for ( i = 0; i < MaxAnlTPGUnits; i++)
		if ( AnlTPGUnit[i].Present) {
			if ( error = SndLong( AnlTPGUnit[i].Address, "AO", delay)) {
				lastError = AnlTPGErrorType + error;
				errorCnt++;
			}
		}

	FeedTheWatchdog;

	for ( i = 0; i < MaxSDITSGUnits; i++)
		if ( SDITSGUnit[i].Present) {
			if ( error = SndLong( SDITSGUnit[i].Address, "HO", delay)) {
				lastError = SDITSGErrorType + error;
				errorCnt++;
			}
		}

	FeedTheWatchdog;

	for ( i = 0; i < MaxSDITPGUnits; i++)
		if ( SDITPGUnit[i].Present) {
			if ( error = SndLong( SDITPGUnit[i].Address, "SO", delay)) {
				lastError = SDITPGErrorType + error;
				errorCnt++;
			}
		}

	FeedTheWatchdog;

	if ( errorCnt > 1) 									// Multiple Errors detected
	  return( SystemErrorType + MultipleError);

	return( lastError);
}

/***************************************************************************/
/* ConfigurePreset															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524 			     														*/
/*																									*/
/*	Function:	Configure the PT5230 with a complete preset setup.				*/
/*	Remarks:		--																				*/
/*	Returns:		TBD																			*/
/*	Updates:		--																				*/
/***************************************************************************/
UC ConfigurePreset( UC no) {

	register UC i;
	UC last, error, errorCnt = 0, lastError = 0;

	for ( i = 0; i < MaxAnlBlkUnits; i++) {
		memcpy( &AnlBlkConfig[i], &Preset[no].AnlBlkStore[i], sizeof( AnlBlkConfig[0]));

		if ( AnlBlkUnit[i].Present)
			if ( error = ConfigureAnlBlkUnit( i)) {
				lastError = error;
				errorCnt++;
			}
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxAnlTPGUnits; i++) {
		memcpy( &AnlTPGConfig[i], &Preset[no].AnlTPGStore[i], sizeof( AnlTPGConfig[0]));

		if ( AnlTPGUnit[i].Present)
			if ( error = ConfigureAnlTPGUnit( i)) {
				lastError = error;
				errorCnt++;
			}
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxSDITSGUnits; i++) {
		memcpy( &SDITSGConfig[i], &Preset[no].SDITSGStore[i], sizeof( SDITSGConfig[0]));

		if ( SDITSGUnit[i].Present)
			if ( error = ConfigureSDITSGUnit( i)) {
				lastError = error;
				errorCnt++;
			}
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxSDITPGUnits; i++) {
		memcpy( &SDITPGConfig[i], &Preset[no].SDITPGStore[i], sizeof( SDITPGConfig[0]));

		if ( SDITPGUnit[i].Present)
			if ( error = ConfigureSDITPGUnit( i)) {
				lastError = error;
				errorCnt++;
			}
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxAESEBUUnits; i++)
		memcpy( &AESEBUConfig[i], &Preset[no].AESEBUStore[i], sizeof( AESEBUConfig[0]));

	if ( AESEBUUnit.HWType == PT8635)
		last = AESEBU2Item;
	else
		last = AESEBU1Item;

	if ( AESEBUUnit.Present) {
		for ( i = AESEBU1Item; i <= last; i++)
			if ( error = AESEBUUnitDrv( i, AESEBUConfig[i].Signal, AESEBUConfig[i].Level, AESEBUConfig[i].Timing)) {
				lastError = error;
				errorCnt++;
			}
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxGenlockUnits; i++)
		memcpy( &GenlockConfig[i], &Preset[no].GenlockStore[i], sizeof( GenlockConfig[0]));

	ActiveGenlockNo = Preset[no].ActiveGenlockNo;
	ExternalGenlockNo = Preset[no].ExternalGenlockNo;

	if ( error = GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System)) {
		lastError = error;
		errorCnt++;
	}

	if ( errorCnt > 1) 								// Multiple Errors detected
	  return( SystemErrorType + MultipleError);

	return( lastError);
}

/***************************************************************************/
/*	GetV24Version														 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		990304				     													*/
/*																									*/
/*	Function:	Get the SW version and KU number from a RS232 unit.			*/
/*	Remarks:		Not all of the PT86XX units returns the HW type & version.	*/
/*					900-versions is NOT recognized UNLESS bit 6&7 in the			*/
/*					hardware information is set, indicating that customized		*/
/*					patterns are available.													*/
/*	Returns:		TRUE: The unit exists and is valid in this master.				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC GetV24Version( UC ndx, UC address, UC HWtype) {

	char* ascPtr;
	UL KUNumber;
	UI tmp;
	UC error;

	FeedTheWatchdog;

	switch ( HWtype) {
		case PT8608:								// Black Burst generator
			//***************************************************************
			//							PT8608
			// HW type:					-
			//	HW version:			 	-
			// HW user pattern:	 	-
			// HW information :	 	-
			// KU Number:				+
			// SW version:				+
			//***************************************************************
			AnlBlkUnit[ndx].Address = address;

			AnlBlkUnit[ndx].HWType = HWtype;	// Set the hardware type
			AnlBlkUnit[ndx].HWVersion = 0;	// Reset the hardware version

														// Get the software version
			tmp = RecInt( address, "HI", 0xFF, &error);
			if ( error)
				tmp = 0;

			AnlBlkUnit[ndx].SWVersion = (UC) tmp;

			AnlBlkUnit[ndx].HWCustomPattern = 0;	// Reset the hardware information
			AnlBlkUnit[ndx].HWInformation = 0;

														// Get the KU number
			ascPtr = RecStr( address, "HR", ProductKUNo_addr/10, &error);

			if ( !error && ( strlen( ascPtr) == 8))
				KUNumber = atol( &ascPtr[2]);
			else
				KUNumber = 0;

			AnlBlkUnit[ndx].KUNumber = KUNumber;

			error = 0;
			break;

		case PT8601:								// Analog-Signal Generator (PT5210)
		case PT8631:								// Analog-TPG
			//***************************************************************
			//							PT8601			PT8631
			// HW type/version:		+	 				+
			// HW user pattern:   	0 (>=2.5)    	0
			// HW information:   	0 (>=2.5)    	0
			// KU Number:				+					+
			// SW version:				+					+
			//***************************************************************
			AnlTPGUnit[ndx].Address = address;

														// Get the hardware type/version
			tmp = RecInt( address, "AV", 0xFF, &error);

			if ( error)
				tmp = HWtype;

			AnlTPGUnit[ndx].HWType = tmp & 0xFF;
			AnlTPGUnit[ndx].HWVersion = tmp >> 8;

														// Get the software version
			tmp = RecInt( address, "AI", 0xFF, &error);
			if ( error)
				tmp = 0;

			AnlTPGUnit[ndx].SWVersion = (UC) tmp;

														// Get the hardware information
			if (( AnlTPGUnit[ndx].HWType == PT8601) && ( AnlTPGUnit[ndx].SWVersion < 30))
				tmp = 0;
			else {
				tmp = RecInt( address, "AT", 0xFF, &error);
				if ( error)
					tmp = 0;
			}

			AnlTPGUnit[ndx].HWCustomPattern = (UC) tmp;
			AnlTPGUnit[ndx].HWInformation = 0;

														// Get the KU number
			ascPtr = RecStr( address, "AR", ProductKUNo_addr/10, &error);

			if ( !error && ( strlen( ascPtr) == 8))
				KUNumber = atol( &ascPtr[2]);
			else
				KUNumber = 0;

			AnlTPGUnit[ndx].KUNumber = KUNumber;

			error = 0;
													// If /900 version & NO custom patterns
			if ( AnlTPGUnit[ndx].HWVersion)
				if ( !( AnlTPGUnit[ndx].HWCustomPattern & 0x88))
					error = SystemErrorType + ConfigurationError;
			break;

		case PT8602:								// SDI Signal Generator 1 (PT5210)
		case PT8603:								// SDI Signal Generator 2 (PT5210)
		case PT8632:								// SDI-TPG 1
		case PT8633:								// SDI-TPG 2
			//***************************************************************
			//							PT8602/3			PT8632/3
			// HW type/version:		+	 				+
			// HW user patterm:   	0 (>=4.0)    	0 (>1.0)
			// HW information:   	0 (>=4.0)    	0 (>1.0)
			// KU Number:				+					+
			// SW version:				+					+
			//***************************************************************
			SDITPGUnit[ndx].Address = address;

														// Get the hardware type/version
			tmp = RecInt( address, "SV", 0xFF, &error);
			if ( error)
				tmp = HWtype;

			SDITPGUnit[ndx].HWType = tmp & 0xFF;
			SDITPGUnit[ndx].HWVersion = tmp >> 8;

														// Get the software version
			tmp = RecInt( address, "SI", 0xFF, &error);
			if ( error)
				tmp = 0;

			SDITPGUnit[ndx].SWVersion = (UC) tmp;

														// Get the hardware information
			if (( SDITPGUnit[ndx].HWType < PT8632) && ( SDITPGUnit[ndx].SWVersion < 40))
				tmp = 0;
			else {
				if ( SDITPGUnit[ndx].SWVersion <= 10)
					tmp = 0;
				else {
					tmp = RecInt( address, "ST", 0xFF, &error);
					if ( error)
						tmp = 0;
				}
			}

			SDITPGUnit[ndx].HWCustomPattern = (UC) tmp;

			if ( SDITPGUnit[ndx].HWType == PT8632)
				tmp >>= 8;
			else
				tmp = 0;

			SDITPGUnit[ndx].HWInformation = (UC) tmp;

														// Get the KU number
			ascPtr = RecStr( address, "SR", ProductKUNo_addr/10, &error);

			if ( !error && ( strlen( ascPtr) == 8))
				KUNumber = atol( &ascPtr[2]);
			else
				KUNumber = 0;

			SDITPGUnit[ndx].KUNumber = KUNumber;

			error = 0;
													// If /900 version & NO custom patterns
			if ( SDITPGUnit[ndx].HWVersion)
				if ( !( SDITPGUnit[ndx].HWCustomPattern & 0x88))
					error = SystemErrorType + ConfigurationError;
			break;

		case PT8609:								// SDI-Black/Colorbar (PT5210)
		case PT8639:								// SDI-TSG
			//***************************************************************
			//							PT8609			PT8639
			// HW type/version:		-	 				+
			// HW information 0:   	0 (>=3.0)   	0
			// HW information 1:   	0 (>=3.0)   	0
			// KU Number:				+					+
			// SW version:				+					+
			//***************************************************************
			SDITSGUnit[ndx].Address = address;

														// Get the hardware type/version
			if ( HWtype == PT8609)
				tmp = HWtype;
			else {
				tmp = RecInt( address, "HV", 0xFF, &error);
				if ( error)
					tmp = HWtype;
			}

			SDITSGUnit[ndx].HWType = tmp & 0xFF;
			SDITSGUnit[ndx].HWVersion = tmp >> 8;

														// Get the software version
			tmp = RecInt( address, "HI", 0xFF, &error);
			if ( error)
				tmp = 0;

			SDITSGUnit[ndx].SWVersion = (UC) tmp;

														// Get the hardware information
			if (( SDITSGUnit[ndx].HWType == PT8609)  && ( SDITSGUnit[ndx].SWVersion < 30))
				tmp = 0;
			else {
				tmp = RecInt( address, "HT", 0xFF, &error);
				if ( error)
					tmp = 0;
			}

			SDITSGUnit[ndx].HWCustomPattern = (UC) tmp;
			SDITSGUnit[ndx].HWInformation = 0;

														// Get the KU number
			ascPtr = RecStr( address, "HR", ProductKUNo_addr/10, &error);

			if ( !error && ( strlen( ascPtr) == 8))
				KUNumber = atol( &ascPtr[2]);
			else
				KUNumber = 0;

			SDITSGUnit[ndx].KUNumber = KUNumber;

			error = 0;
													// If /900 version & NO custom patterns
			if ( SDITSGUnit[ndx].HWVersion)
				if ( !( SDITSGUnit[ndx].HWCustomPattern & 0x88))
					error = SystemErrorType + ConfigurationError;
			break;
	}

	if ( error)
		DisplayErrorLine( error, ErrorDisplayTimeout);

	return( error);
}

/***************************************************************************/
/*	GetIICVersion														 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Get the KU number from an IIC unit.									*/
/*	Remarks:		IIC2RdStr ALWAYS reads 10+1 characters, hence the string to	*/
/*					hold the characters MUST be at least 11 characters				*/
/*	Returns:		An IIC error code.														*/
/*	Updates:		--																				*/
/***************************************************************************/
UC GetIICVersion( UC bus, UC address, UC KU_address, UL* KU) {

	UC error;

	if ( bus == IICBus2) {
		IIC2RdStr( address, KU_address, tmpBuffer);
		error = IIC2ErrorCode;
	}
	else {
		IIC1RdStr( address, KU_address, tmpBuffer);
		error = IIC1ErrorCode;
	}

	if ( error)
		*KU = 0;
	else {
		tmpBuffer[8] = 0;

		*KU = atol( &tmpBuffer[2]);
	}

	return( error);
}

/***************************************************************************/
/*	TestMainboard																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the main board.														*/
/*	Remarks:		This is mainly a test of the mainboard EEPROM.					*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestMainboard( void) {

	FeedTheWatchdog;
															// Test power calibration values
	if( !IIC2Write( mainram1_addr, VoltCalib_addr))
		return( 1);

	if ((UC) VoltCalib - IIC2Read( mainram1_addr))
		return( 1);

	if ( !IIC2ErrorCode)
		if ( VoltN5Min - IIC2Read( mainram1_addr))
			return( 1);

	if ( !IIC2ErrorCode)
		if ( VoltN5Max - IIC2Read( mainram1_addr))
			return( 1);

	if ( !IIC2ErrorCode)
		if ( Volt12Min - IIC2Read( mainram1_addr))
			return( 1);

	if ( !IIC2ErrorCode)
		if ( Volt12Max - IIC2Read( mainram1_addr))
			return( 1);

	IIC1Read( ttl_addr);						// Read the parallel remote IIC port

	if ( IIC1ErrorCode || IIC2ErrorCode)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestSPGUnit																	 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the SPG unit.														*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSPGUnit( void) {

	UC error, tmp;

	FeedTheWatchdog;

	tmp = (UC) ( IIC2RdLong( spgram_addr, HPHZero_G_addr) - RecLong( sync_addr, "GA", 0xFF, &error));
	if ( error || tmp)
		return( 1);

	tmp = (UC) ( IIC2RdInt( spgram_addr, GenlPhase_G_addr) - RecInt( sync_addr, "GF", 0xFF, &error));
	if ( error || tmp)
		return( 1);

	tmp = (UC) ( IIC2RdLong( spgram_addr, HPHZero_M_addr) - RecLong( sync_addr, "GU", 0xFF, &error));
	if ( error || tmp)
		return( 1);

	tmp = (UC) ( IIC2RdInt( spgram_addr, GenlPhase_M_addr) - RecInt( sync_addr, "GG", 0xFF, &error));
	if ( error || tmp)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestAESEBUUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		991026, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8605/35 AES/EBU Audio generator						*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestAESEBUUnit() {

	register UC i;
	UC tmp;
	UI port[2];

	FeedTheWatchdog;

	for ( i = 0; i < 2; i++) {
		tmp = AESEBUConfig[i].Signal;

		port[i] = AESEBUSignalTable[tmp] + 0xF880;

		if ( tmp != AESEBUWordClock)
			port[i] |= AESEBULevelTable[AESEBUConfig[i].Level] +
						AESEBUTimingTable[AESEBUConfig[i].Timing];
	}
											// Compare level/signal for AES_EBU1
	if ((( port[0] & 0x00FF) - IIC2Read( sound1_addr)) & 0x7F)
		return( 1);

											// Compare timing for AES_EBU1
	if ( !IIC2ErrorCode)
		if ((( port[0] >> 8) - IIC2Read( sound2_addr)) & 0x07)
			return( 1);

	if ( !IIC2ErrorCode)
		if ( AESEBUUnit.HWType == PT8635) {

											// Compare level/signal for AES_EBU2
			if ((( port[1] & 0x00FF) - IIC2Read( sound3_addr)) & 0x7F)
				return( 1);

											// Compare timing for AES_EBU2
			if ( !IIC2ErrorCode)
				if ((( port[1] >> 8) - IIC2Read( sound4_addr)) & 0x07)
					return( 1);
		}

	return( IIC2ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*	TestAnlBlkUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 971023	    										*/
/* Revised:		980524, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8608 Black Burst generator.								*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestAnlBlkUnit( UC ndx) {

	UC tmp, error, addr;
	float ScHPhase;

	FeedTheWatchdog;

	addr = AnlBlkUnit[ndx].Address;

												// Examine status byte
	tmp = (UC) RecInt( addr, "HS", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test ScHPhase in BBn
	ScHPhase = ( RecInt( addr, "HH", 0xFF, &error) * 360.)/2048.;

	( ScHPhase > 180.) ? ( ScHPhase -= 360.) : 0;

	if (( fabs( ScHPhase - (float) AnlBlkConfig[ndx].ScHPhase) > 0.5) || error)
		return( 1);

												// Test ScHPhase in BBn+1
	ScHPhase = ( RecInt( addr, "JH", 0xFF, &error) * 360.)/2048.;

	( ScHPhase > 180.) ? ( ScHPhase -= 360.) : 0;

	if (( fabs( ScHPhase - (float) AnlBlkConfig[ndx+1].ScHPhase) > 0.5) || error)
		return( 1);
												// Test system in BBn
												// Not implemented in ver 2.2 and earlier
	tmp = AnlBlkConfig[ndx].System - (UC) RecInt( addr, "HQ", 0xFF, &error);
	if ( tmp || error)
		if ( error != RxV24Timeout)
			return( 1);
												// Test system in BBn+1
												// Not implemented in ver 2.2 and earlier
	tmp = AnlBlkConfig[ndx+1].System - (UC) RecInt( addr, "JQ", 0xFF, &error);
	if ( tmp || error)
		if ( error != RxV24Timeout)
			return( 1);
												// Test delay in BBn
												// Not implemented in ver 2.2 and earlier
	tmp = (UC) ( AnlBlkConfig[ndx].Delay - RecLong( addr, "HB", 0xFF, &error));
	if ( tmp || error)
		if ( error != RxV24Timeout)
			return( 1);
												// Test delay in BBn+1
												// Not implemented in ver 2.2 and earlier
	tmp = (UC) ( AnlBlkConfig[ndx+1].Delay - RecLong( addr, "JB", 0xFF, &error));
	if ( tmp || error)
		if ( error != RxV24Timeout)
			return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestAnlTPGUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Test the PT8601/PT8631 Analog-TPGs.									*/
/*	Remarks:		Will test both PT8601 and PT8631										*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestAnlTPGUnit( UC ndx) {

	UC error, tmp, HWtype, addr;
	float ScHPhase;

	FeedTheWatchdog;

	HWtype = AnlTPGUnit[ndx].HWType;
	addr = AnlTPGUnit[ndx].Address;

												// Examine status byte
	tmp = (UC) RecInt( addr, "AS", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test system
	tmp = AnlTPGConfig[ndx].System - ( UC) RecInt( addr, "AQ", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Lookup analog pattern code
	tmp = FindPatternTable( HWtype)[AnlTPGConfig[ndx].Pattern];

												// Test pattern
	tmp -= (UC) RecInt( addr, "AP", 0xFF, &error);
	if ( tmp || error)
		return( 1);
												// Test text enable
//	if ( AnalogConfig.TextOn - ( UC) RecInt( AnlSig_addr, "AF", 0xFF, &error))
//		return( 7);

												// Test text information
//	if ( strcmp( AnalogConfig.TextInfo, RecStr( AnlSig_addr, "AN", 0xFF, &error)))
//		return( 8);

												// Test ScHPhase
	ScHPhase = ( RecInt( addr, "AH", 0xFF, &error) * 360.)/2048.;

	( ScHPhase > 180.) ? ( ScHPhase -= 360.) : 0;

	if (( abs( ScHPhase - (float) AnlTPGConfig[ndx].ScHPhase) > 0.5) || error)
		return( 1);

												// Test delay
	tmp = (UC) ( AnlTPGConfig[ndx].Delay - RecLong( addr, "AB", 0xFF, &error));
	if ( tmp || error)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestSDITSGUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Test the PT8609/39 SDI-TSGs.											*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSDITSGUnit( UC ndx) {

	UC error, tmp, HWtype, addr;

	FeedTheWatchdog;

	HWtype = SDITSGUnit[ndx].HWType;
	addr = SDITSGUnit[ndx].Address;

												// Examine status byte
	tmp = (UC) RecInt( addr, "HS", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test system, (only for PT8639)
	if ( HWtype == PT8639) {
		tmp = SDITSGConfig[ndx].System - (UC) RecInt( addr, "HQ", 0xFF, &error);
		if ( tmp || error)
			return( 1);
	}
												// Lookup digital pattern code
	tmp = FindPatternTable( HWtype)[SDITSGConfig[ndx].Pattern];

												// Test pattern
	tmp -= (UC) RecInt( addr, "HP", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test EDH insertion
	tmp = SDITSGConfig[ndx].EDH - (UC) RecInt( addr, "HE", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test audio signal
	tmp = SDITSGConfig[ndx].AudioSignal - (UC) RecInt( addr, "HU", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test audio level, (only for PT8639)
	if ( HWtype == PT8639) {
		tmp = SDITSGConfig[ndx].AudioLevel - (UC) RecInt( addr, "HL", 0xFF, &error);
		if ( tmp || error)
			return( 1);
	}

												// Test delay
	tmp = (UC) ( SDITSGConfig[ndx].Delay - RecLong( addr, "HB", 0xFF, &error));
	if ( tmp || error)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestSDITPGUnit																 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524				     													*/
/*																									*/
/*	Function:	Test the  PT8602/03/32/33 SDI-TPGs.									*/
/*	Remarks:		Will test both PT8602/03 and PT8632/33								*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSDITPGUnit( UC ndx) {

	UC error, tmp, HWtype, addr;

	FeedTheWatchdog;

	HWtype = SDITPGUnit[ndx].HWType;
	addr = SDITPGUnit[ndx].Address;

												// Examine status byte
	tmp = (UC) RecInt( addr, "SS", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test system
	tmp = SDITPGConfig[ndx].System - (UC) RecInt( addr, "SQ", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Lookup digital pattern code
	tmp = FindPatternTable( HWtype)[SDITPGConfig[ndx].Pattern];

												// Test pattern
	tmp -= (UC) RecInt( addr, "SP", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test text enable
//	tmp = SDITPGConfig[ndx].TextOn - ( UC) RecInt( addr, "SF", 0xFF, &error);
//	if ( tmp || error)
//		return( 1);

												// Test text information
//	tmp = strcmp( SDITPGConfig[ndx].TextInfo, RecStr( addr, "SN", 0xFF, &error)))
//	if ( tmp || error)
//		return( 1);

												// Test EDH insertion
	tmp = SDITPGConfig[ndx].EDH - (UC) RecInt( addr, "SE", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test audio signal
	tmp = SDITPGConfig[ndx].AudioSignal - (UC) RecInt( addr, "SU", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Lookup audio level code
	tmp = FindAudioLevelTable( HWtype)[SDITPGConfig[ndx].AudioLevel];

												// Test audio level
	tmp -= (UC) RecInt( addr, "SL", 0xFF, &error);
	if ( tmp || error)
		return( 1);

												// Test audio group, (only for PT8633)
	if ( HWtype == PT8633) {
		tmp = SDITPGConfig[ndx].AudioGroup - (UC) RecInt( addr, "SC", 0xFF, &error);
		if ( tmp || error)
			return( 1);
	}

												// Test delay
	tmp = (UC) ( SDITPGConfig[ndx].Delay - RecLong( addr, "SB", 0xFF, &error));
	if ( tmp || error)
		return( 1);

	return( 0);
}

/***************************************************************************/
/*	TestBBMulUnit														 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8604 Multiple Parallel Black Burst Option			*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: FAIL																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestBBMulUnit( void) {

	FeedTheWatchdog;

	IIC1RdStr( multibb_addr, ProductKUNo_addr, tmpBuffer);

	return( IIC1ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*	TestSDIGenlockUnit												 		 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8606 SDI Digital Genlock									*/
/*	Remarks:		--																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestSDIGenlockUnit( void) {

	FeedTheWatchdog;

	IIC1RdStr( d1ram_addr, ProductKUNo_addr, tmpBuffer);

	return( IIC1ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*	TestTimeClockUnit															 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980730, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8607/37 Time Code-/Time Clock Module					*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: ERROR															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestTimeClockUnit( void) {

	FeedTheWatchdog;

	if ( TimeClockUnit.HWType == PT8637) {
													// Get TCI error information.
		TimeClockConfig.TCIControl &= 0x1F;
		TimeClockConfig.TCIControl |= ( TCIErrorRegister << 5);
		IIC1Write( time1_addr, TimeClockConfig.TCIControl);

		FeedTheWatchdog;

		IIC1ReadN( time1_addr, 2, ( UC*) tmpBuffer);

		if ( tmpBuffer[1] || IIC1ErrorCode)
			return( 1);							// FAIL
	}

	IIC1RdStr( timeram_addr, ProductKUNo_addr, tmpBuffer);

	return( IIC1ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*	TestHighPrecisionOCXUnit												 UNIT_DRV.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980524, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the PT8610 High Precision OCXO									*/
/*	Remarks:		Presently NOT tested														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC TestHighPrecisionOCXUnit( void) {

	FeedTheWatchdog;

	return( 0);
}
