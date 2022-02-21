/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PT5201 Compact VariTime Sync Generator								*/
/*	Module:		TEST.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990824																		*/
/*	Rev. date:	000315, KEn, DEV															*/
/*	Status:		Version 1.0 																*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		byte user_memory_test( void);														*/
/*		byte NVRAM_memory_test( void);													*/
/*																									*/
/*	Changes:																						*/
/*																									*/
/***************************************************************************/
#include <stdlib.h>

#include "system.h"
#include "main.h"
#include "test.h"
#include "iic.h"
#include "xaexprt.h"
#include "xadrivr.h"

#if BootProgram
#else

/***************************************************************************/
/*	user_memory_test														 		  TEST.C	*/
/*																									*/
/* Author:		Kim Engedahl																*/
/* Revised:		000315, KEn, DEV	     													*/
/*																									*/
/*	Function:	Test the user memory area 256KBytes									*/
/*	Remarks:		--																				*/
/*	Returns:		0: OK, 1: error writing 0x5555, 2: error writing 0xAAAA		*/
/*	Updates:		--																				*/
/***************************************************************************/
byte user_memory_test( void) {

	
	dword i, j;
//	word tmp;
	byte error = 0;
	volatile _huge word *ptr;


	j = 0;
	i = 0x500;						// NOTE!! Start address MUST exceed the
										//  start address for the SP. Reason unknown!!
	ptr = ( _huge word *) i;

	do {
		do {
			EA = 0;						// Disable interrupt while testing

			*ptr= (0xFFFF-i-9*j);

			if ( *ptr++ != (0xFFFF-i-9*j))
				error = 1;

			EA = 1;					 	// Enable interrupt again

		} while(( ++i < 0x8000) && !error);

		i = 0;

	} while (( ++j < 4) && !error);


	do {
		j = 0;
		i = 0x500;						// NOTE!! Start address MUST exceed the
										//  start address for the SP. Reason unknown!!
		ptr = ( _huge word *) i;

		do {
			do {
				EA = 0;						// Disable interrupt while testing

				if ( *ptr++ != (0xFFFF-i-9*j))
					error = 1;

				EA = 1;					 	// Enable interrupt again

			} while(( ++i < 0x8000) && !error);

			i = 0;

		} while (( ++j < 4) && !error);

		Udr_PutChar( '*');

	} while ( !error);

/*	dword i;
	word tmp;
	byte error = 0;
	volatile _huge word *ptr;

	i = 0x500;						// NOTE!! Start address MUST exceed the
										//  start address for the SP. Reason unknown!!
	ptr = ( _huge word *) i;

	do {
		EA = 0;						// Disable interrupt while testing

		tmp = *ptr;					// Save contents temporarily

		*ptr = 0x5555;
		if ( *ptr != 0x5555)
			error = 1;
		else {
			*ptr = 0xAAAA;
			if ( *ptr != 0xAAAA)
				error = 2;
		}
		*ptr++ = tmp;			 	// Reload contents and increment pointer

		EA = 1;					 	// Enable interrupt again

	} while(( ++i < 0x20000) && !error);

*/
	return( error);
}

/***************************************************************************/
/*	NVRAM_memory_test												 				  TEST.C	*/
/*																									*/
/* Author:		Kim Engedahl																*/
/* Revised:		000315, KEn, DEV	     													*/
/*																									*/
/*	Function:	Reading/writing the timekeeper memory								*/
/*	Remarks:		The NVRAM occupies the memory area 0x80000 to 0x84000.		*/
/*	Returns:		0: OK, 1: error writing 0x55, 2: error writing 0xAA			*/
/*	Updates:		--																				*/
/***************************************************************************/
byte NVRAM_memory_test( void) {

	dword i;
	word tmp;
	byte error = 0;
	volatile _huge word *ptr;

	i = 0;

	ptr = ( _huge word *) ( 0x80000);

	do {
		EA = 0;						// Disable interrupt while testing

		tmp = *ptr;					// Save contents temporarily

		*ptr = 0x0055;
		if (( *ptr & 0x00FF) != 0x0055)
			error = 1;
		else {
			*ptr = 0x00AA;
			if (( *ptr & 0x00AA) != 0x00AA)
				error = 2;
		}
		*ptr++ = tmp;			 	// Reload contents and increment pointer

		EA = 1;					 	// Enable interrupt again

	} while (( ++i < 0x4000) && !error);


	return( error);
}


#endif