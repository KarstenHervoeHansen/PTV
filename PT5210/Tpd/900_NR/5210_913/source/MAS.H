/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MAS.H																			*/
/*	Author:		Preben Christiansen/Kim Engedahl, DEV								*/
/*	Org. date:	960903																		*/
/*	Rev. date:	980616, KEn DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	Changes:																						*/
/* 980619 Added sound3_addr & sound4_addr												*/
/* 980609 New values for temperature variables, (blower).						*/
/* 971013 Added RAMCheckOK_Addr & RAMCheckVal_addr									*/
/* 970917 Renamed UserText_addr to SPGUserText_addr								*/
/* 970821 Redesign of level detector variables & handling						*/
/* 970820 Added variables for -5V & 12V power supply limits, (KEn)			*/
/* 970209 Added address definitions for main EEPROM, (KEn)						*/
/* 970124 Added ADC conversion variables, (KEn)										*/
/* 970115 Moved feed_dog definition here from MAS.C, (KEn)						*/
/* 961231 Added led_addr. Changed keyb_addr. Removed unused declarations	*/
/*			  , (KEn)																			*/
/* 961218 Added definition for SPG EEPROM addresses, (KEn)						*/
/* 961018 Added definition of 'multibb_addr'											*/
/***************************************************************************/

// ******************************************************************
// GLOBAL DEFINITIONS
// ******************************************************************
#define sbit static bit

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

// ******************************************************************
// PORT AND MEMEORY CONTROL:
// RAM has address-space 0-0x1fff
// ******************************************************************
extern volatile UC  ind1;
extern volatile UC  ind2;

extern volatile UC  ud1;
extern volatile UC  ud2;
extern volatile UC  ud3;
extern volatile UC  ud4;
extern volatile UC  ud5;

extern near UC ud1buf;
extern near UC ud4buf;
extern near UC ud5buf;

// ******************************************************************
// INTERNAL V24 CONTROL:
// default V24 address for units:
// ******************************************************************
#define default_addr		0xF0
#define master_addr		0x10
#define bbu0_addr			0x00
#define bbu1_addr			0x11
#define bbu2_addr			0x12
#define bbu3_addr			0x13
#define sync_addr			0x15
#define AnlSig_addr		0x16
#define SDISig_addr		0x17

// ******************************************************************
// IIC-addresses, bus 1:
// ******************************************************************
#define lev1det_addr		0x44
#define lev2det_addr		0x46
#define ttl_addr			0x4A	// Ground closure parallel port
#define time_addr			0x4E	// Time code unit
#define d1ram_addr		0xA0	// SDI Genlock
#define multibb_addr		0xA2	// Multi black burst RAM
#define timeram_addr		0xA8	// Time code unit RAM

// ******************************************************************
// IIC-addresses, bus 2:
// ******************************************************************
#define sound1_addr		0x40		// PT8605/35 Audio generator
#define sound2_addr		0x42		// do.
#define sound3_addr		0x44		// PT8635 Audio generator
#define sound4_addr		0x46		// do.
#define soundram_addr	0xA0
#define mainram1_addr	0xA2
#define spgram_addr		0xA8	// SPG, (OSC), RAM

// ******************************************************************
// IIC-addresses, bus front:
// ******************************************************************
#define keyb_addr			0x40
#define led_addr			0x42
#define ADC_addr			0x90
#define DAC_addr			0x90

// ******************************************************************
// IIC-addresses/offsets for textstrings in MAINBOARD AND UNITS EEPROM
// ******************************************************************
#define CompanyName_addr		(UC) 00		// PHILIPS
#define Product12NC_addr		(UC) 10		// 108 12345, ie. Mounted board
#define ProductKUNo_addr		(UC) 20		// KU123456
#define ProductionDate_addr	(UC) 30		//
#define ProductName1_addr		(UC) 60		// PT5210
#define ProductName2_addr		(UC) 70		//

// ******************************************************************
// IIC-addresses for constants in MAINBOARD EEPROM
// ******************************************************************
#define VoltCalib_addr			(UC) 80
#define VoltN5Min_addr			VoltCalib_addr+1
#define VoltN5Max_addr			VoltCalib_addr+2
#define Volt12Min_addr			VoltCalib_addr+3
#define Volt12Max_addr			VoltCalib_addr+4

// ******************************************************************
// IIC-addresses for user-variables in MAINBOARD EEPROM
// ******************************************************************
#define RAMCheckOK_addr			(UC) 85
#define RAMCheckVal_addr		RAMCheckOK_addr+1

// ******************************************************************
// IIC-addresses for constants in SPG EEPROM
// ******************************************************************
#define HPHZero_G_addr			(UC) 0		// unsigned long
#define GenlPhase_G_addr		(UC) 4		// unsigned integer
#define HPHZero_M_addr			(UC) 6		// unsigned long
#define GenlPhase_M_addr		(UC) 10		// unsigned integer
#define SPGUserText_addr		(UC) 12		// Start of usertext area, 100 bytes

// ******************************************************************
// LEVEL DETECTOR STATUS
// ******************************************************************
extern UI LevelStatus;

// ******************************************************************
// A/D CONVERSION VARIABLES
// ******************************************************************
extern near UC TempMeas;
extern near UC Volt5Meas;

extern near UC VoltN5Meas;
extern UC VoltN5Min;
extern UC VoltN5Max;

extern near UC Volt12Meas;
extern UC Volt12Min;
extern UC Volt12Max;

extern bit VoltCalib;				// 1: Power has been calibrated

// ******************************************************************
// WATCHDOG
// ******************************************************************
#define feed_dog	{ ud4buf ^= 16; ud4= ud4buf;}

// **************************************************************************
// TEMPERATURE VARIABLES, corrected 980507/PRC
// **************************************************************************
#define templimit  	106				// appr.45gr. sensor - 41 gr. air
#define panictemp		55					// appr.75gr. sensor - 71 gr. air
#define hottemp		62					// appr.69gr. sensor - 66 gr. air

extern bit shotdown;

// ******************************************************************
// FUNCTIONS
// ******************************************************************
extern void waitms( UI time);
extern void waitus( UI time);

