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
#define bbu0_addr		0x00

// SYSTEM VARIABLES
extern near UC c;				// common globals
extern near char cc;
extern near UC slicer;		// time window counter
extern bit Timer_flag;		// control bit for 21 ms timing
extern bit Acknowledge;		// Acknowledge after IIC communication
// ******************************************************************

extern void waitms(UI time);
extern void waitus(UI time);

