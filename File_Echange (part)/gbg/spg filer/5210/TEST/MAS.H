/*
********************************************************************
* Project	: PT5210  MASTER
* Filename	: MAS.H
* Version	: 0.0	961111
* Purpose	: Header file for MAS.C
* Org.date	: 960903
* Author	: PRC
********************************************************************
HISTORY:
961218 Added definition for SPG EEPROM addresses,(KEn)
961018 Added definition of 'multibb_addr'
*/

#define sbit static bit
#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

extern near UC syncstatus;

// PORT CONTROL:
// RAM has address-space 0-0x1fff
extern volatile UC  ind1;
extern volatile UC  ind2;
extern volatile UC  ud1;
extern volatile UC  ud2;
extern volatile UC  ud3;
extern volatile UC  ud4;
extern volatile UC  ud5;
extern code far UC  pgadata;
extern near UC ud1buf;
extern near UC ud4buf;
extern near UC ud5buf;

// INTERNAL V24 CONTROL:
// default V24 address for units:
#define default_addr	0xf0
#define master_addr	0x10
#define bbu0_addr		0x00
#define bbu1_addr		0x11
#define bbu2_addr		0x12
#define bbu3_addr		0x13
#define sync_addr		0x15

#define AnlSig_addr	0x16			//KIM
#define SDISig_addr	0x17			//KIM

extern near UC myaddress;		// V24 address for master

// IIC-addresses, bus 1:
#define lev1det_addr		0x44
#define lev2det_addr		0x46
#define d1lock_addr		0x48
#define ttl_addr			0x4a
#define time_addr			0x4e
#define d1dac_addr		0x90
#define d1ram_addr		0xa0
#define multibb_addr		0xa2
#define timeram_addr		0xa8

// IIC-addresses, bus 2:
#define sound1_addr		0x40
#define sound2_addr		0x42
#define soundram_addr	0xa0
#define mainram1_addr	0xa2
#define mainram2_addr	0xa4
#define spgram_addr		0xa8

// IIC-addresses, bus front:
#define contrast_addr	0x90
#define keyb_addr			0x48
#define temperat_addr	0x90

// IIC-address for content of SPG EEPROM
#define HPHZero_G_addr		0x00		// unsigned integer
#define HPHZero_M_addr		0x02		// unsigned integer
#define GenlPhase_G_addr	0x04		// unsigned long
#define GenlPhase_M_addr	0x08		// unsigned long
#define UserText_addr		0x0C		// 100 bytes

// LEVEL DETECTOR
extern near lev1det;
extern near lev2det;

// SYSTEM VARIABLES (some are old from PM5655)
extern near UC c;				// common globals
extern near char cc;
extern near UC slicer;		// time window counter
extern bit Timer_flag;		// control bit for 21 ms timing
extern bit Acknowledge;		// Acknowledge after IIC communication
// ******************************************************************

// ********** TEMPERATURE VARIABLES *******
#define templimit  	104
				// appr.45gr. sensor - 41 gr. air
#define blowvolt	79
  // Adjust 'blowvolt' to set the lowest blower volt. to appr. 8V (1V pr.12)
#define panictemp	52
				// appr.75gr. sensor - 71 gr. air
#define hottemp		59
				// appr.70gr. sensor - 66 gr. air
extern bit shotdown;
// ******************************************************************

extern void waitms(UI time);
extern void waitus(UI time);

