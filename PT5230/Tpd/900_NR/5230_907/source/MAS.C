/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MAS.C																			*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980122																		*/
/*	Rev. date:	991105, KEn, DEV															*/
/*	Status:		Version 1.3																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void waitms( UI time)																*/
/*		void waitus( UI time)																*/
/*		interrupt void Timer_int(void)													*/
/*		void main(void)																		*/
/*																									*/
/*	Changes:																						*/
/* 991105: UserTimer is now an unsigned integer										*/
/* 991103: Added DispTimer to be used solely in the disp_drv.c module		*/
/* 991025: "Master reset" has been removed, i.e. it is no longer supported	*/
/* 990420: Error in masking of bit in parallel remote corrected.				*/
/*			  Only 6 presets are allowed from the parallel remote.				*/
/* 981005: Releases as 4008 002 06812 software ver. 2.0							*/
/* 980924: Error intializing ConfigNdx, (PT8607 error).							*/
/* 980915: Added bit butstLockOn.														*/
/* 980827: The number of time slices in main routine has been increased		*/
/* 980710: Implementation of Time Clock Interface PT8637							*/
/* 980616: Released as 4008 002 06812													*/
/* 980611: Extended levelstatus handling with 6 audio levels for PT8635		*/
/* 980609: Updated level error handling												*/
/* 980514: Released as 4008 002 06811													*/
/* 980514: The fan is switched ON during internal test							*/
/***************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <xa.h>
#include <intrpt.h>

#include "mas.h"

#include "iic.h"
#include "serial1.h"
#include "sw5230.h"
#include "cont_drv.h"
#include "keyb_drv.h"
#include "disp_drv.h"
#include "text.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "message.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "rs232par.h"
#include "util.h"
#include "unit_drv.h"
#include "tables.h"
#include "factory.h"

#define xtal 29491200				// Crystal frequency for the mainboard

// *************************************************************************
// PORT CONTROL:
// RAM has address-space 0-0x1fff
// *************************************************************************
volatile UC ind1 @ 0x2001;			// input port 1
volatile UC ind2 @ 0x4001;			// input port 2

volatile UC ud1 @ 0x2001;			// output port 1
volatile UC ud2 @ 0x4001;			// output port 2
volatile UC ud3 @ 0x6001;			// output port 3
volatile UC ud4 @ 0x8001;			// output port 4
volatile UC ud5 @ 0xA001;			// output port 5

near UC ud1buf;						// buffer for output port 1
near UC ud4buf;						// buffer for output port 4
near UC ud5buf;						// buffer for output port 5


// *************************************************************************
// CONTROL OF OPTIONAL UNITS:
// Resets:
// *************************************************************************
#define	MRES1_on		{ ud4buf &= ~0x20; ud4 = ud4buf;}
#define	MRES1_off	{ ud4buf |= 0x20; ud4 = ud4buf;}
#define	MRES2_on		{ ud4buf &= ~0x40; ud4 = ud4buf;}
#define	MRES2_off	{ ud4buf |= 0x40; ud4 = ud4buf;}
#define	MRES3_on		{ ud4buf &= ~0x80; ud4 = ud4buf;}
#define	MRES3_off	{ ud4buf |= 0x80; ud4 = ud4buf;}


// *************************************************************************
// CONTROL OF FAN (BLOWER)
// *************************************************************************
#define	fan_on	{ ud1buf |= 16; ud1 = ud1buf;}
#define	fan_off	{ ud1buf &= ~16; ud1 = ud1buf;}

near int temp;						// Temperature
near UI oldtemp, grense;	 	// Temperature stores
near UC tempcnt;					// Counter for speed change filter
bit shotdown;						// Flag for very hot apparatus
near UC tempwarn;					// Flag for hot apparatus


// *************************************************************************
// A/D CONVERSION VARIABLES
// *************************************************************************
near UC TempMeas;					// Temperature read from AD converter
near UC Volt5Meas;				// 5 volt read from AD converter

near UC VoltN5Meas;				// -5 volt read from AD converter
UC VoltN5Min;						// Min. value for -5 volt
UC VoltN5Max;						// Max. value for -5 volt

near UC Volt12Meas;				// 12 volt read from AD converter
UC Volt12Min;						// Min. value for 12 volt
UC Volt12Max;						// Max. value for 12 volt

bit VoltCalib;						// 1: Power has been calibrated


// *************************************************************************
// SYSTEM VARIABLES
// *************************************************************************
near UC syncstatus;
near int RS232Char;
near UC slicer;					// Time window counter
bit TimerFlag;						// Control bit for 21 ms timing
bit SecondFlag;					// Control bit for 1 s timing


// *************************************************************************
// STATIC VARIABLES
// *************************************************************************
PNP_STRUCT MainBoard = {
	TRUE, master_addr, NA, 0, 0, 0, 0, PT5230SWVersion,	// MAIN-BOARD
};

PNP_STRUCT SPGBoard = {
	TRUE, sync_addr, NA, 0, 0, 0, 0, 0,							// SPG-BOARD
};

PNP_STRUCT AnlBlkUnit[MaxAnlBlkUnits] = {
	{  TRUE, bbu0_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST1
	{  TRUE, bbu0_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST2
	{ FALSE, bbu1_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST3
	{ FALSE, bbu1_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST4
	{ FALSE, bbu3_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST5
	{ FALSE, bbu3_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST6
	{ FALSE, bbu2_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST7
	{ FALSE, bbu2_addr, PT8608, 0, 0, 0, 0, 0 },				// BLACK-BURST8
};

PNP_STRUCT AnlTPGUnit[MaxAnlTPGUnits] = {
	{ FALSE, bbu2_addr, PT8631, 0, 0, 0, 0, 0 },				// ANALOG-TPG2
	{ FALSE, bbu2_addr, PT8631, 0, 0, 0, 0, 0 }				// ANALOG-TPG5
};

PNP_STRUCT SDITSGUnit[MaxSDITSGUnits] = {
	{ FALSE, bbu2_addr, PT8639, 0, 0, 0, 0, 0},				// SDI-TSG2
	{ FALSE, bbu1_addr, PT8639, 0, 0, 0, 0, 0},				// SDI-TSG3
	{ FALSE, bbu3_addr, PT8639, 0, 0, 0, 0, 0}				// SDI-TSG4
};

PNP_STRUCT SDITPGUnit[MaxSDITPGUnits] = {
	{ FALSE, SDITPG_addr, PT8632, 0, 0, 0, 0, 0},			// SDI-TPG1
	{ FALSE, bbu2_addr, PT8633, 0, 0, 0, 0, 0},				// SDI-TPG2
	{ FALSE, bbu2_addr, PT8633, 0, 0, 0, 0, 0} 				// SDI-TPG5
};

PNP_STRUCT AESEBUUnit = {
	FALSE, NA, PT8635, 0, 0, 0, 0, 0,							// AES-EBU1
};

PNP_STRUCT SDIGenlockUnit = {										// SDI Genlock
	FALSE, NA, PT8606, 0, 0, 0, 0, 0
};

PNP_STRUCT Int2GenlockUnit = {									// High Precision OCX
	FALSE, NA, PT8610, 0, 0, 0, 0, 0
};

PNP_STRUCT TimeClockUnit = {										// Time code/clock
	FALSE, NA, PT8637, 0, 0, 0, 0, 0
};

// *************************************************************************
// DYNAMIC VARIABLES, IE. VARIABLES "CONTINUOSLY" BEING UPDATED.
// *************************************************************************
persistent ANLBLK_STRUCT AnlBlkConfig[MaxAnlBlkUnits];
persistent ANLTPG_STRUCT AnlTPGConfig[MaxAnlTPGUnits];
persistent SDITSG_STRUCT SDITSGConfig[MaxSDITSGUnits];
persistent SDITPG_STRUCT SDITPGConfig[MaxSDITPGUnits];
persistent AESEBU_STRUCT AESEBUConfig[MaxAESEBUUnits];
persistent GENLOCK_STRUCT GenlockConfig[MaxGenlockUnits];
persistent TIMECLOCK_STRUCT TimeClockConfig;

persistent RS232_STRUCT RS232Config;
persistent PRESET_STRUCT Preset[NoOfPresets];
persistent STATUS_FLAGS Flags;

persistent UC ActivePresetNo;		// Currently active preset, (0 = NONE)
persistent UC ActiveOutputNo;		// Currently active output, ie. OUTPUT BUTTON

persistent UC ExternalGenlockNo;	// Last selected EXTERNAL genlock
persistent UC ActiveGenlockNo;	// Currently active genlock

persistent UC LCDContrast;			// Contrast on LCD-display
persistent UC PasswordState;		// OFF: Apparatus is NOT password protected

persistent UC RAMCheckArea[24];	// Area used for PowerUp RAM check

code struct MENU_ITEM* near Ptr;
code struct MENU_ITEM* near PrevPtr;

UI ErrorStatus;							// Last active global error code, 32 BIT
												// 8 MSB 	8 LSB
												// [index]	ErrorType+ErrorNo

UI LevelStatus;							// Status of level detectors 1-9, (0x1FF)

UL ErrorQueue[NoInErrorQueue] = {	// Global error code queue, 32 BIT
	0, 0, 0, 0, 0							// 16 MSB		  8 BIT	 8 LSB
};												// leveldetector [Index] ErrorType+ErrorNo

UC ErrorQueuePtr = NoInErrorQueue-1;
bit ErrorWindowOn;						// 1: Displaying an "error" window

bit MessageWindowOn;						// 1: Displaying a "message" window
near UC MessageCntDwn;					// Counter for display of message

volatile near UC SecTimer;				// Seconds timer: updated every second
volatile near UC AutoESCTimer;		// Timer for auto return to status display
volatile near UC DispTimer;			// Display driver timer: updated every 20ms
volatile near UI UserTimer;			// Generel user timer: updated every 20ms
volatile near UC DisplayTimer;		// Error display timer: updated every 20ms

UC UpdateTimer;							// Timer for display animation
UC UpdateFrequency;						// Frequency for the above
bit UpdateEnable;							// 1: Display animation is active
bit UpdateSignal;							// 1: Display animatino caused the call

bit AutoESCSignal;						// 1: Return to status display

bit DoKeyboardScan;						// 1:	Scan the keyboard

bit TestModeEnable;						// 1: Enable test mode

bit MenuModeOn;							// 1: Menu mode active
bit SelectModeOn;							// 1: Selection mode active

bit CBarLED;								// 1: Illumination of the C.BAR LED
bit MBurstLED;								// 1: Illumination of the M.BURST LED
bit WindowLED;								// 1: Illumination of the WINDOW LED
bit SpecialLED;							// 1: Illumination of the SPECIAL LED
bit LinearityLED;							// 1: Illumination of the LINEARITY LED
bit PatternLED;							// 1: Illumination of the PATTERN LED

bit RemoteLED;								// 1: Illumination of the REMOTE LED
bit UnlockedLED;							// 1: Illumination of the UNLOCKED LED

bit burstLockOn;							// 1: if burst available in genlock
												//  systems BurstLockPAL & BurstLockNTSC.

bit SerialRemotePresent;				// 1: Remote cable placed in serial conn.
bit SerialRemoteActive;  				// 1: Serial remote active
bit SerialRemoteLockoutOn;				// 1: Serial remote lockout ( ctrl-L)

bit ParallelRemotePresent;				// 1: Remote cable placed in parallel conn.
bit ParallelRemoteActive;				// 1: Parallel remote active
UC ParallelRemoteStatus;

bit RS232DiagnoseMode;					// 1: RS232 Loopback test in progress

/***************************************************************************/
/*	waitms																	 			MAS.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960828										*/
/* Revised:		960828				     													*/
/*																									*/
/*	Function:	A <n>ms wait routine where n is user definable.					*/
/*	Remarks:		This is a software based loop. Altering the crystal or		*/
/*					tampering with the waitstates might have serious influence	*/
/*					on the timing.																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void waitms( UI time) {				// Longer wait routine
	UI n;

	while ( time-- > 0) {			// Wait 1 ms x time
		n = 750;
		while ( n > 0)
			n--;
	}
}

/***************************************************************************/
/*	waitus																	 			MAS.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960828										*/
/* Revised:		960828				     													*/
/*																									*/
/*	Function:	A <n>us wait routine where n is user definable.					*/
/*	Remarks:		This is a software based loop. Altering the crystal or		*/
/*					tampering with the waitstates might have serious influence	*/
/*					on the timing.																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void waitus( UI time) {				// Short wait routine

	while ( time-- > 0)
		;									// Wait 1.1 us x time
}

/***************************************************************************/
/*	VECTOR DEFINITIONS													 			MAS.C	*/
/*																									*/
/* Interrupts:T0 prior:     9  bank 1													*/
/*		Ser0_rec     15   -   0																*/
/*		Ser1_rec  -  15   -   0																*/
/*		Ser0_trm  -   ?   -   0																*/
/*		Ser1_trm  -   ?   -   0																*/
/***************************************************************************/
ROM_VECTOR( IV_T0, Timer_int, IV_SYSTEM+IV_PRI15+IV_BANK1)

ROM_VECTOR( IV_RI0, Udr_Uart_RX_Interrupt, IV_SYSTEM+IV_PRI14+IV_BANK0)
ROM_VECTOR( IV_TI0, Udr_Uart_TX_Interrupt, IV_SYSTEM+IV_PRI14+IV_BANK0)

ROM_VECTOR( IV_EX0, Udr_CTS_Interrupt, IV_SYSTEM+IV_PRI14+IV_BANK0)

ROM_VECTOR( IV_RI1, Serial1_int, IV_SYSTEM+IV_PRI14+IV_BANK0)

/***************************************************************************/
/*	Timer_int																 			MAS.C	*/
/*																									*/
/* Author:		Preben Christiansen, DEV, 960828										*/
/* Revised:		991105, KEn, DEV	     													*/
/*																									*/
/*	Function:	Interrupt routine which handles ALL timing.						*/
/*	Remarks:		Changing the crystal WILL affect the timing.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
interrupt void Timer_int(void) {

	static near UC ticker;

	TH0 = -xtal/256/4/3/50;				// Load timer to 50 Hz (20ms)

	if ( ++ticker > 2) {
		ticker = 0;
		TimerFlag = TRUE;					// Set 20ms flag

		if ( !--SecTimer)	{				// This routine is executed each second.
			SecTimer = 50;					// Reload seconds timer.

			if ( Flags.AutoESCEnable)						// If auto escape enabled
				if ( ++AutoESCTimer > AutoESCTimeOut)	// and time-out reached
					AutoESCSignal = TRUE;					// set time-out flag
		}

		if ( DispTimer)					// DispTimer is decremented each 20 ms.
			DispTimer--;

		if ( UserTimer)					// UserTimer is decremented each 20 ms.
			UserTimer--;

		if ( DisplayTimer)				// DisplayTimer is decremented each 20 ms.
			DisplayTimer--;

		DoKeyboardScan = TRUE;			// Set flag for keyboard-scan
	}
}

/***************************************************************************/
/*	main																		 			MAS.C	*/
/*																									*/
/* Author:		Kim Engedahl DEV, 980122												*/
/* Revised:		991025, KEn, DEV	     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void main(void) {

	UI i;
	UC TCIStatus, error;
	static bit TCISecond;
	char textBuffer[10];

// System set-up
	BCR = 0x02;		// Set 20 bit BUS, 8 bit DATA
	PSWH = 0x8F;	// Set system priority high
	BTRH = 0xAE;	// To be trimmed (was 0xef) EXT. BUS timing, Data Cycle
	BTRL = 0xCE;	// To be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
	PSWL = 0;		// Clear flags
	SCR = 0;			// Timer speed prescaler ( 0 gives 4, 4 gives 16, 8 gives 64)
	PSWH &= 0xF0;	// Set system priority low

   MRES1_on;		// Reset all options NB! DO NOT MOVE!!!!!
   MRES2_on;		//       do          NB! DO NOT MOVE!!!!!
	MRES3_on;		//       do          NB! DO NOT MOVE!!!!!

// Serial set-up
	S1CON = 0xF2;	// Transmit mode 3,  9 bit UART,  Address mode, Rec.enable
	S1ADDR = master_addr;	// receive address
	S1ADEN = 0xCC;	// mask for receive address
						// receive address = 00010000 AND 11001100 = 00XX00XX (0 to 33)

// Timer set-up
	TMOD = 0x21;	// Timer 1: 8 bit autoload; Timer 0: 16 bit count
	TH0 = -255;		// Start value
	T2MOD = 0x31;	// Use T2 for V24_1, count up
	T2CAPH = -1;	// Set reload values
	T2CAPL = -6;
	TH2 = -1;		// Set for max. Baud
	TL2 = -6;
	TR2 = 1;			// Start V24-timer

// Interrupt setting
   IT0 = 1;			// Edge trig on ext0
   IT1 = 1;			// Edge trig on ext1
	EX0 = 0;			// Disable ext0
	ET0 = 1;			// Enable timer0 intrpt
	ET1 = 0;			// Disable timer1 intrpt

// NB! IPAx PRIORITETS-SETTING SKAL MATCHE SETTING I ROM_VECTOR() !!!
// (Brug kun 9-F, 0 ell. 8 er intrpt off!)
	IPA0 = 0xFE;			// Priority Timer0/EX0 
	IPA4 = 0xEE;			// Priority PTI0/PRI0, (serial 0)
	IPA5 = 0x0E;			// Priority PTI1/PRI1, (serial 1)

	Serial1_init();		// Initialize internal serial v24 bus

	FeedTheWatchdog;

	RdFrontIIC( 0xF0);	// Initialize front IIC bus by reading dummy
	IIC1Read( 0xF0);		// Initialize IIC bus 1 by reading dummy
	IIC2Read( 0xF0);		// Initialize IIC bus 2 by reading dummy

								// Initialize level detector ports
	IIC1Write( lev1det_addr, 0xFF);
	IIC1Write( lev2det_addr, 0xFF);

	ud4buf &= 0xF8;		// Disable display
	ud4 = ud4buf;

	TR0 = 1;					// Start loop timer
	TR1= 1;					// Start V24-timer

   EA = 1 ;					// Enable all interrupts

	fan_on;					// Turn blower on while performing internal test

	oldtemp = 255;			// Initiate temperature control
	tempwarn = 0;
	slicer = 0;

	SecTimer = 50;			// Seconds timer: 50 gives a 1 second timer, ie. 5*20ms
	UpdateTimer = 0;		// Update timer used for animation. Reset to 0

	AutoESCSignal = 0;	// 1: returnSignal to escape from menu to display
	UpdateEnable = 0;		// 1: Animation enabled
	UpdateSignal = 0;		// 1: The call to the routine caused by animation

	OldButtonCommand = ButtonCommand = NOCommand;

	ParallelRemotePresent = FALSE;
	ParallelRemoteActive = FALSE;
	ParallelRemoteStatus = 0xFF;

	SerialRemotePresent = FALSE;
	SerialRemoteActive = FALSE;
	SerialRemoteLockoutOn = FALSE;	// Disable serial remote lockout

	RS232DiagnoseMode = FALSE;			//

	TestModeEnable = OFF;

	// **********************************************************************
	// Reset all units. There are 3 resets to options, MRES1 to MRES3.
	//	In order to identify the position of the options, one reset is
	//	released at a time and an option address is send to this option.
	// If the option then answers back its address, the Master then
	//	nows the presence of this option.
	//	Do allow all units to do their own initialisation.
	// Variable 'options' (for Test purposes)  has a bit for each option.
	// **********************************************************************

	UserTimer = i = 10;			// Reset for 80-100 ms

	while ( UserTimer) {
		FeedTheWatchdog;

		if ( i == UserTimer) {
			InitDisplay();			// Initialize display, this is done 4 or 5 times
			i--;
		}
	}

	i = ~ind1;							// Detect type of remote connector

	if ( i & SerialRemoteMask)		// RS232 remote installed
		SerialRemotePresent = TRUE;

	if ( i & ParallelRemoteMask)	// Parallel remote installed
		ParallelRemotePresent = TRUE;

	CBarLED = ON;						// Switch ON all LEDs during power-up
	MBurstLED = ON;
	WindowLED = ON;
	SpecialLED = ON;
	LinearityLED = ON;
	PatternLED = ON;
	RemoteLED = ON;
	UnlockedLED = ON;

	UpdateLEDs();

	CBarLED = OFF;					// Switch OFF all LEDs after LED testing
	MBurstLED = OFF;
	WindowLED = OFF;
	SpecialLED = OFF;
	LinearityLED = OFF;
	PatternLED = OFF;
	RemoteLED = OFF;
	UnlockedLED = OFF;

	TempMeas = 128;					// Intialize TempMeasurement to dummy
	InitContrast();					// Initialize the display contrast

											// Test buttons during power-on
	switch ( RdFrontIIC( keyb_addr)) {
		case FACTORYButton:
			WriteCodeLN1( 0, PowerUpFactResetTxt);

			FactoryPreset();
			break;

		default:
			WriteCodeLN1( 0, PowerUpDefaultTxt);
			break;
	}
	ClearRestLN1();

											// Get information about the main-board
	GetIICVersion( IICBus2, mainram1_addr, ProductKUNo_addr,\
																		 &MainBoard.KUNumber);

	UserTimer = 20;					// Set timer to 380-400ms

	IIC2Write( mainram1_addr, VoltCalib_addr);		// Get powersupply limits
	VoltCalib = ( IIC2Read( mainram1_addr) == ON);	// Calibration
	VoltN5Min = IIC2Read( mainram1_addr);				// Min. -5 volt
	VoltN5Max = IIC2Read( mainram1_addr);				// Max. -5 volt
	Volt12Min = IIC2Read( mainram1_addr);				// Min. 12 volt
	Volt12Max = IIC2Read( mainram1_addr);				// Max. 12 volt

	WrFrontIIC( ADC_addr, 0x44);							// ADC. Auto increment
	Volt12Meas = RdFrontIIC( ADC_addr);					// Read 12 volt A/D
	TempMeas = RdFrontIIC( ADC_addr);					// Read temperature A/D
	Volt5Meas = RdFrontIIC( ADC_addr);					// Read +5V A/D
	VoltN5Meas = RdFrontIIC( ADC_addr);					// Read -5V A/D
				

	while ( UserTimer)				// Short delay for the eye
		FeedTheWatchdog;

	WriteCharLN2( 0, ' ');
	ClearRestLN2();
	WriteCodeLN2( 8, SelfTestTxt);// Write Selftest in progress

	if ( !PowerUpRAMCheckOK())	{	// If RAM test fails..
		FactoryPreset();				//  perform a factory preset
		DisplayErrorLine( RAMMemoryErrorType, ErrorDisplayTimeout);
	}

	MRES1_off;							// Release reset 1
	UserTimer = 11;					// Set timer to 200-220ms

											// Initialize the remote RS232
	Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
								 RS232Config.Parity, RS232Config.Handshake);

	// **********************************************************************
	// Initialize mainboards Black Burst
	// **********************************************************************
	for ( i = BB1Item; i <= BB2Item; i++) {
		GetV24Version( i, bbu0_addr, PT8608);

		AnlBlkUnit[i].KUNumber = MainBoard.KUNumber;

		if ( error = ConfigureAnlBlkUnit( i))
			DisplayErrorLine( error, ErrorDisplayTimeout);
	}

	while ( UserTimer)					// While waiting for BBU_1 unit to
		FeedTheWatchdog;

	// **********************************************************************
	// Intialize options at connector TXG3Conn, TXG2Conn & TXG4Conn
	// **********************************************************************
	FindDynamicOptions( bbu1_addr);

	MRES2_off;								// Release reset 2
	UserTimer = 13;						// Set timer to 240-260ms

	while ( UserTimer)					// While waiting for BBU_2 unit to
		FeedTheWatchdog;

	FindDynamicOptions( bbu2_addr);

	MRES3_off;								// Release reset 3
	UserTimer = 13;						// Set timer to 240-260ms

	while ( UserTimer)					// While waiting for BBU_3 unit to
		FeedTheWatchdog;

	FindDynamicOptions( bbu3_addr);

	FindStaticOptions();					// Static options should only be intialized
												//  AFTER all the resets have been released.

	if ( error = GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System))
		DisplayErrorLine( error, ErrorDisplayTimeout);

												// Download calibration data to SPG
	if ( error = ConfigureSPG())
		DisplayErrorLine( error, ErrorDisplayTimeout);

	// **********************************************************************
	// RECALL OLD SETTINGS
	// **********************************************************************
	grense = templimit - 2;				// Limit for blower (high temp.)

	UpdateLEDs();

	UserTimer = 75;						// Start delay for at least 1.5 second

	if ( !ROMCheckOK( &i))				// PROM Cheksum test
		DisplayErrorLine( ROMMemoryErrorType + ChecksumError, ErrorDisplayTimeout);

	if ( !ErrorStatus) {	
		WriteCodeLN2( 8, InternalTestTxt);
		ClearRestLN2();

		while ( UserTimer)				// Simulates delay for internal test
			FeedTheWatchdog;
	}

	UserTimer = 100;						// Start delay 2 second

	BaseNdx = 0;							// Reset base menu
	AnlBlkNdx = 0;							// Reset last active Analog Black menu item

												// Reset last active Analog TPG menu item
	for ( i = 0; i < MaxAnlBlkUnits; i++)
		BBNdx[i] = 0;
												// Reset last active Analog TPG menu item
	for ( i = 0; i < MaxAnlTPGUnits; i++)
		AnlTPGNdx[i] = 0;
												// Reset last active SDI TSG menu item
	for ( i = 0; i < MaxSDITSGUnits; i++)
		SDITSGNdx[i] = 0;
												// Reset last active SDI TPG menu item
	for ( i = 0; i < MaxSDITPGUnits; i++)
		SDITPGNdx[i] = 0;
												// Reset last active AES-EBUn menu item
	AESEBUnNdx = 0;
												// Reset last active AES-EBU menu item
	for ( i = 0; i < MaxAESEBUUnits; i++)
		AESEBUNdx[i] = 0;

	PresetNdx = 0;	  						// Reset last active Preset menu item

												// Reset last active Config menu item
	ConfigNdx = ( !TimeClockUnit.Present || ( TimeClockUnit.HWType == PT8607));

	DateTimeNdx = 0;	 					// Reset last active Date-Teim menu item
	RS232Ndx = 0; 							// Reset last active RS232 menu item
	DiagnoseNdx = 0; 						// Reset last active Diagnose menu item

	while ( UserTimer)					// Delay to show internal test passed
		FeedTheWatchdog;

	WriteCodeLN1( 0, PT5230HeadlineTxt);
	ClearRestLN1();

	GetPT5230SWVersion( textBuffer);

	WriteCharLN2( 0, ' ');
	ClearToPosLN2( 8);
	WriteCode2( VersionHdrTxt);		// Write software version
	WriteTxt2( textBuffer);
	ClearRestLN2();

	UserTimer = 250;						// Delay 5.0 second or until key pressed
	while ( UserTimer)
		if ( ButtonReady())
			break;

	SendContrast( LCDContrast);		// Send contrast

	MenuModeOn = SelectModeOn = OFF;

	if ( ErrorStatus)						// If any errors during powerup
		StatusNdx = StatusErrorItem;	//  start in Error/Warnings display
	else {
		if ( ActivePresetNo)
			StatusNdx = StatusPresetItem;
		else
			StatusNdx = StatusGenlockItem;
	}

	Ptr = &status_menu[StatusNdx];	// Prepare pointer for status display
	Ptr->DispFunct();						// Display status-display

	// **********************************************************************
	// ENDLESS LOOP
	// **********************************************************************

	while( TRUE) {

		while( !TimerFlag) {			// While waiting for timer flag..

			// ****************************************************************
			// HANDLE SERIAL REMOTE INTERFACE, IF PRESENT
			// ****************************************************************
			if ( SerialRemotePresent) {	// If serial remote interface enabled..

				if ( !RS232DiagnoseMode)	// Do not respond if in diagnose mode
					while (( RS232Char = Udr_InChar()) != UDR_EOF) {
						CmdHandler((UC) RS232Char);

						FeedTheWatchdog;

						if ( SerialRemoteActive == FALSE) {
							RemoteLED = ON;
							UpdateLEDs();

							SerialRemoteActive = TRUE;		// Set serial remote flag
							SetupRS232GotoLocalMessage();	// Prepare for "Goto.." display
						}
					}
			}
		}
		TimerFlag = FALSE;									// 20 ms timer expired

		// *******************************************************************
		// FOR THE PT8637 ONLY: TIME CLOCK INTERFACE READ/UPDATE EVERY 20 ms
		// *******************************************************************
		if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

			// NOTE! The time and display will ONLY be updated if a new time
			//  update is ready from the TCI.

			TCIStatus = IIC1Read( time1_addr);	// Get TCI status information

			if ( IIC1ErrorCode) {
				;
			}
			else {
				if ( TCISecond != ( TCIStatus & 0x01)) {

					TCISecond ^= 1;					// Toggle seconds flag.

															// Update TCI status.
					TimeClockConfig.TCIStatus = TCIStatus;

					BroadcastTCITime();				// Read & broadcast TCI time.

					if ( !MenuModeOn && ( Ptr == &status_menu[StatusDateTimeItem]))
						Ptr->DispFunct();
				}
			}
		}

		// *******************************************************************
		// HANDLE KEYBOARD SCANNING AND DECODING EVERY 20 ms
		// *******************************************************************
		if ( ButtonReady()) {

			if ( ParallelRemoteActive || SerialRemoteLockoutOn) {
				if ( ParallelRemoteActive)
					MessageHandling( ParallelLockoutMessage);
				else
					MessageHandling( SerialLockoutMessage);
			}
			else {
				if ( ErrorWindowOn)			// If error window on..
					ErrorWindowOn = FALSE;	//  pressing a button will close display
				else {
					if ( SerialRemoteActive && !MenuModeOn)
						Ptr = &RS232local_menu;
					else
						switch ( ButtonCommand) {
							case UPButton:
								Ptr->UpBut();
								break;

							case DOWNButton:
								Ptr->DownBut();
								break;

							case RIGHTButton:
								Ptr->RightBut();
								break;

							case LEFTButton:
								Ptr->LeftBut();
								break;

							case EXECButton:
								Ptr->ExecBut();
								break;

							case CBARButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										GroupButtonSelect( CBarGroup);
								break;

							case MBURSTButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										GroupButtonSelect( MBurstGroup);
								break;

							case WINDOWButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										GroupButtonSelect( WindowGroup);
								break;

							case PRESETButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										PresetButtonSelect();
								break;

							case SPECIALButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										GroupButtonSelect( SpecialGroup);
								break;

							case LINEARITYButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										GroupButtonSelect( LinearityGroup);
								break;

							case PATTERNButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										GroupButtonSelect( PatternGroup);
								break;

							case OUTPUTButton:
								if ( !SerialRemoteActive)
									if ( Ptr != &diagnose_submenu[DiagKeyboardItem])
										OutputButtonSelect();
								break;

						}
				}
				Ptr->DispFunct();
			}
		}

		// *******************************************************************
		// SLICER HANDLING
		// *******************************************************************
		// The number to "slices" have been increased to 20, giving each
		//  slice free every 1/400ms, ie. 2.5 times/second

		if ( ++slicer > 20)							// Divide in 20 windows
			slicer = 1;

		// *******************************************************************
		// HANDLE AUTO ESCAPE FROM MENUMODE									  WINDOW 1
		// *******************************************************************
		if ( slicer == 1) {

			if ( Flags.AutoESCEnable) {
				if ( MenuModeOn) {
					if ( AutoESCSignal) {

						AutoESCSignal = FALSE;
						AutoESCTimer = 0;				// Reset Escape timer
			
						while ( SelectModeOn) {		// If in selection mode ..
							Ptr->EscapeFct();			//  be sure to terminate NICELY!!
							Ptr->DispFunct();
						}

						if ( Flags.AutoESCToStatus) {
							UpToStatus();				// Update ptr to status-display
							Ptr->DispFunct();			// Display status-display
						}
					}
				}
			}
		}

		// *******************************************************************
		// HANDLE MESSAGE WINDOWS AND RS232 WINDOWS UPDATE				  WINDOW 3
		// *******************************************************************
		if ( slicer == 3) {

			// Message windows:
			// Presently only two windows messages are used: Local lockout
			//  for both parallel and serial remote.

			// RS232 windows update:
			// This "routine" is ONLY used when in serial remote. If a serial
			//  command alters the content of a variable, and this variable
			//  presently is shown in the status display, the display must be
			//  updated in order to show the new value. NO update should occur
			//  if a message window is presently displayed.

			if ( MessageWindowOn) {					// Message windows..
				if ( MessageCntDwn-- == 0) {
					MessageWindowOn = OFF;			// Remove message window flag and
					Ptr->DispFunct();					//  update status-display
				}
			}
			else {										// RS232 windows update..
				if ( SerialRemoteActive && !MenuModeOn) {
					if ( NewCmdExecuted) {			// If a new command executed..
						NewCmdExecuted = FALSE;		//  unflag the information and
						Ptr->DispFunct();				//  update the status display
					}
				}
			}
		}

		// *******************************************************************
		// HANDLE DISPLAY ANIMATION											  WINDOW 5
		// *******************************************************************
		if ( slicer == 5) { 

			if ( UpdateEnable)
				if ( --UpdateTimer == 0) {
					UpdateSignal = TRUE;					// Signal update ready
					UpdateTimer = UpdateFrequency;	// Reset update timer

					Ptr->DispFunct();						// Update selected display
				}
		}

		// *******************************************************************
		// FOR THE PT8637 ONLY: UPDATE STATUS & TIME INFORMATION      WINDOW 7
		// *******************************************************************
		if ( slicer == 7) {

			// NOTE! This update is performed to make sure that any changes in the
			//  status of the TCI is displayed to the user, whether a new time
			//  update is ready or not.

			if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

															// Get TCI status information
				TCIStatus = IIC1Read( time1_addr);

				if ( IIC1ErrorCode) {
					;
				}
				else {
															// Update TCI status.
					TimeClockConfig.TCIStatus = TCIStatus;

					BroadcastTCITime();				// Read & broadcast TCI time.

					if ( !MenuModeOn && ( Ptr == &status_menu[StatusDateTimeItem]))
						Ptr->DispFunct();
				}
			}
		}

		// *******************************************************************
		// HANDLE READING THE A/D CONVERTERS								  WINDOW 9
		// *******************************************************************
		if ( slicer == 9) {

			if ( !WrFrontIIC( ADC_addr, 0x44))				// Auto increment
				GotoErrorStatus( TempADCErrorType + IICErrorCode);
			else {
				Volt12Meas = RdFrontIIC( ADC_addr);			// Read 12 volt A/D

				FeedTheWatchdog;

				if ( !IICErrorCode)
					TempMeas = RdFrontIIC( ADC_addr);		// Read temperature A/D

				FeedTheWatchdog;

				if ( !IICErrorCode)
					Volt5Meas = RdFrontIIC( ADC_addr);		// Read +5V A/D

				FeedTheWatchdog;
	
				if ( !IICErrorCode)
					VoltN5Meas = RdFrontIIC( ADC_addr);		// Read -5V A/D
				
				if ( IICErrorCode)
					GotoErrorStatus( VoltageADCErrorType + IICErrorCode);
				else {
					if ( SendContrast( LCDContrast))			// Update contrast
						GotoErrorStatus( LCD_ADCErrorType + IICErrorCode);
				}
			}
      }

		// *******************************************************************
		// HANDLE POWER-SUPPLY MONITORING									 WINDOW 11
		// *******************************************************************
		if ( slicer == 11) { 

			if ( VoltCalib) {							// If voltage has been calibrated.

				// Only the -5V and the 12V will be examined as the +5V is
				//  handled by the watch dog

															// If -5 volt is out of range..
				if (( VoltN5Meas < VoltN5Min) || ( VoltN5Meas > VoltN5Max))
					GotoErrorStatus( SystemErrorType+PowerN5Failure);
				else {									// else if -5 volt error on..
					if ( ErrorStatus == ( SystemErrorType+PowerN5Failure)) {
						ErrorStatus = 0;				//  clear error

						if ( !MessageWindowOn && !MenuModeOn)
							Ptr->DispFunct();
					}
				}
															// If 12 volt is out of range..
				if (( Volt12Meas < Volt12Min) || ( Volt12Meas > Volt12Max))
					GotoErrorStatus( SystemErrorType+Power12Failure);
				else {									// else if 12 volt error on..
					if ( ErrorStatus == ( SystemErrorType+Power12Failure)) {
						ErrorStatus = 0;				//  clear error

						if ( !MessageWindowOn && !MenuModeOn)
							Ptr->DispFunct();
					}
				}
			}
		}

		// *******************************************************************
		// HANDLE TEMPERATURE CONTROL/MONITORING							 WINDOW 13
		// *******************************************************************
		if ( slicer == 13) {

			temp = TempMeas;				// Set blower speed from NTC resistor temp

			if ( temp == 0)
				temp = 255;

			if ( temp == oldtemp)
				tempcnt = 25;
			else
				if (( --tempcnt == 0) || ( oldtemp > temp+1)) {
					tempcnt = 25;
					oldtemp = temp;
																// MANAGE TEMPERATURE WARNINGS
					if ( temp > hottemp+2) {			// If colder than HOT..
						if ( tempwarn != 0) {			//  warning OFF
							tempwarn = 0;
																// If temperature error..
							if (( ErrorStatus == ( SystemErrorType+TemperatureWarning)) ||\
								 ( ErrorStatus == ( SystemErrorType+TemperatureFailure))) {
								ErrorStatus = 0;			//  clear error

								if ( !MessageWindowOn && !MenuModeOn)
									Ptr->DispFunct();
							}
						}
					}
					else
						if ( temp < panictemp) {		// If warmer than TOOHOT..
							shotdown = 1;					//  shut down
							if ( tempwarn != 2) {		//  FAILURE to frontplate
								tempwarn = 2;
								GotoErrorStatus( SystemErrorType+TemperatureFailure);
							}
						}
			      else
						if ( temp < hottemp)	{			// If warmer than HOT..
							if ( tempwarn != 1) {		//  WARNING to frontplate
								tempwarn = 1;
								GotoErrorStatus( SystemErrorType+TemperatureWarning);
							}
				      }

					if ( temp > panictemp+10)
				      if ( shotdown) {
							shotdown=0;						// Not TOOHOT, cancel shut down
						}
																// Set blower speed
					if ( temp> grense) {
						temp = 0;							// Cold, blower off
						if ( ud1buf & 16)
							fan_off;
						grense = templimit-2;
					}
					else {
						grense = templimit;
						if ( !( ud1buf & 16))
							fan_on;							// blower on
					}
				}
		}

		// *******************************************************************
		// HANDLE LEVEL-DETECTORS												 WINDOW 15
		// *******************************************************************
		if ( slicer == 15) {

			i = IIC1Read( lev1det_addr);					// 8 input

			if ( !IIC1ErrorCode)
				i += ( IIC1Read( lev2det_addr) << 8);	// 1 input (LSB)
																	// 1 output (MSB)
			i &= 0x1FF;											// Mask out level bits

			error = IIC1ErrorCode;

			if ( !error) {
				if ( AESEBUUnit.Present && ( AESEBUUnit.HWType == PT8635)) {
					i += (( IIC2Read( sound2_addr) & 0x70) << 5);		// 3 input

					if ( !IIC2ErrorCode)
						i += (( IIC2Read( sound4_addr) & 0x70) << 8);	// 3 input

					if ( !IIC2ErrorCode) {
						if (( i & 0x7E00) != 0x7E00)
							IIC1Write( lev2det_addr, 0x7F);
						else
							IIC1Write( lev2det_addr, 0xFF);
					}
								
					error = IIC2ErrorCode;
				}
				else											// If AESEBU unit not present
					i |= 0x7E00;							//  set bit to level OK
			}

			if ( error)											// If IIC Circuit error ..
				GotoErrorStatus( LevelDetectorErrorType + error);
			else {
				if ( i != LevelStatus) {
					LevelStatus = i;							// Save LevelStatus

					if ( LevelStatus ^ 0x7FFF)
						GotoErrorStatus( SystemErrorType + LevelDetectorError);
					else {
						if ( ErrorStatus == ( SystemErrorType + LevelDetectorError)) {
							ErrorStatus = 0;

							if ( !MessageWindowOn && !MenuModeOn)
								Ptr->DispFunct();
						}
					}
				}
			}
		}

		// *******************************************************************
		// HANDLE SPG GENLOCKED/UNLOCKED UPDATE							 WINDOW 17
		// *******************************************************************
		if ( slicer == 17) {

			if ( ActiveGenlockNo != Internal) {

				i = RecInt( sync_addr, "GS", 0xFF, &error);	// Send request

				if ( error)
					GotoErrorStatus( OSCErrorType + error);
				else
					syncstatus = (UC) i;

				i = syncstatus & 0x08;			// Mask out burstlock flag

				if (( i && !burstLockOn) || ( !i && burstLockOn)) {

					burstLockOn = !burstLockOn;

					i = 1;							// Set flag "UPDATE STATUS"
				}
				else
					i = 0;							// Clear flag "UPDATE STATUS"

				if (( syncstatus & 0x01) ^ !UnlockedLED) {

					UnlockedLED = !UnlockedLED;

					UpdateLEDs();

					i = 1;							// Set flag "UPDATE STATUS"
				}

				if ( i && ( !MessageWindowOn && !MenuModeOn))
					Ptr->DispFunct();
			}
			else {
				UnlockedLED = OFF;
				UpdateLEDs();
			}

			if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {
				if ( !UnlockedLED && ( ActiveGenlockNo < SDIGenlock) &&
					( 	GenlockConfig[ActiveGenlockNo].System < SDI625Lock))
					TimeClockConfig.TCIControl |= 0x01;
				else
					TimeClockConfig.TCIControl &= 0xFE;

				TrxTimeClockControl( TimeClockConfig.TCIControl);
			}
		}

		// *******************************************************************
		// HANDLE PARALLEL REMOTE INTERFACE, IF PRESENT					 WINDOW 19
		// *******************************************************************
		if ( slicer == 19) {

			if ( ParallelRemotePresent) {

				// 8574    Remote Connector
				// 0   ->  pin 1 (LSB)		Preset no
				// 1   ->  pin 2
				// 2   ->  pin 3 (MSB)
				// 3   ->  pin 4				0: Genlock active 	1: Preset active
				// 4   ->  pin 6				0: External genlock	1: Internal genlock
				// 7   ->  pin 9				0: Remote enable		1: Remote disable

				i = IIC1Read( ttl_addr) & 0x9F;

				if ( i != ParallelRemoteStatus) {
					ParallelRemoteStatus = i;

					if ( ParallelRemoteStatus & 0x80)
						ParallelRemoteActive = OFF;
					else
						ParallelRemoteActive = ON;

					if ( ParallelRemoteActive) {

						if ( MenuModeOn) {
							if ( SelectModeOn) {		// If in selection mode..
								Ptr->EscapeFct();		//  be sure to terminate NICELY!!
								Ptr->DispFunct();
							}

							UpToStatus();			// Update ptr to status-display
							Ptr->DispFunct();		// Display status-display
						}

						if ( ParallelRemoteStatus & 0x08) {		// Preset active

							if (( i = (UC) ( ParallelRemoteStatus & 0x07)) < 6) {
								ActivePresetNo = (UC) i;

								ConfigurePreset( ActivePresetNo++);

								StatusNdx = StatusPresetItem;
							}
						}
						else {											// Genlock active
							if ( ParallelRemoteStatus & 0x10)
								ActiveGenlockNo = Internal;
							else
								ActiveGenlockNo = ExternalGenlockNo;

							GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);

							StatusNdx = StatusGenlockItem;
						}

						PrevPtr = Ptr;
						Ptr = &status_menu[StatusNdx];

						if ( !MessageWindowOn)
							Ptr->DispFunct();
					}
				}
				i = 0xFF;							// Set Genlock & warning output

				if ( UnlockedLED)
					i &= 0xDF;

				IIC1Write( ttl_addr, i);
			}
		}
	}
}
