/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		TEST.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990819																		*/
/*	Rev. date:	990909, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains no functions													*/
/*																									*/
/*	Changes:																						*/
/*																									*/
/***************************************************************************/
#ifndef _TEST_H
#define _TEST_H


#define maxHWTest	7

extern byte user_memory_test( void);
extern byte timekeeper_memory_test( void);
extern byte HWmonitor_test( void);
extern byte ioport_test( void);
extern byte rs232_test( void);
extern byte parallelport_test( void);
extern byte PLD_test( void);

#endif
