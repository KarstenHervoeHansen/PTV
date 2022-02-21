/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		UTIL.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void NONE(void)																		*/
/*		byte HexToByte( char s)														 		*/
/*		byte htol( char* s, dword* result)			 									*/
/*		byte otol( char* s, dword* result)			 									*/
/*		byte btol( char* s, dword* result)			 									*/
/*		void dwordtoa( dword val, char* result)										*/
/*		void bytetohex( byte val, char *result)										*/
/*		void wordtohex( word val, char *result)										*/
/*		int Cstrcmp( char * s1, _rom char * s2)										*/
/*                                                                         */
/* Changes:                                                                */
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include "system.h"
#include "util.h"

/***************************************************************************/
/*	NONE																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	DOES ABSOLUTELY NOTHING!!!												*/
/*	Remarks:		This one is primarily used in menu tables							*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void NONE( void) {
}

/***************************************************************************/
/*	HexToByte																 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		990707				     													*/
/*																									*/
/*	Function:	Converts a char defined in hex to binary							*/
/*	Remarks:		--																				*/
/*	Returns:		A byte from 0-15															*/
/*	Updates:		--																				*/
/***************************************************************************/
byte HexToByte( char s) {

	if ( isdigit( s))
		return( s - '0');

	return( toupper( s) - 'A' + 10);
}
  
/***************************************************************************/
/*	htol																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		990707				     													*/
/*																									*/
/*	Function:	Converts a string containg a hex number to an unsigned long	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte htol( char *s, dword *result) {

	*result = 0;

	while ( *s)
		*result = 16*(*result) + HexToByte( *s++);

	return( 0);
}

/***************************************************************************/
/*	otol																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		990707				     													*/
/*																									*/
/*	Function:	Converts a string containg an octal number to unsinged long	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte otol( char *s, dword *result) {

	*result = 0;

	while ( *s)
		*result = 8*(*result) + ( *s++ - '0');

	return( 0);
}
  
/***************************************************************************/
/*	btol																		 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		990707				     													*/
/*																									*/
/*	Function:	Converts a string containg a binary number to unsigned long	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte btol( char *s, dword *result) {

	*result = 0;

	while ( *s)
		*result = 2*(*result) + ( *s++ - '0');

	return( 0);
}
  
/***************************************************************************/
/*	dwordtoa																 		 	 UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		990718				     													*/
/*																									*/
/*	Function:	Converts an unsigned long to a string								*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void dwordtoa( dword val, char *result) {

	dword tmp;
	char i, j, TmpChar;

	i = j = 0;
	tmp = val;				

	if ( val == 0) {
		result[0] = '0';
		result[1] = '\0';
	}
	else {
		while ( val) {
			tmp = val / 10;
			result[i++] = ( val % 10) + '0';
			val = tmp;
		}
		result[i--] = '\0';

		while ( i >= j) {							// Reverse ASCII string
			TmpChar = result[i];
			result[i] = result[j];
			result[j] = TmpChar;
			i--;
			j++;
		}
	}
}
  
/***************************************************************************/
/*	bytetohex															 		 	 UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990718	    										*/
/* Revised:		990718				     													*/
/*																									*/
/*	Function:	Converts an unsigned char to a hex string							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void bytetohex( byte val, char *result) {

	byte i, c;
		
	for ( i = 0; i < 2; i++) {
		c = val % 16;
		val /= 16;
				
		if ( c < 10)
			result[1-i] = '0' + c;
		else
			result[1-i] = 'A' + c - 10;

	}

	result[2] = 0;
}
  
/***************************************************************************/
/*	wordtohex															 		 	 UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990716	    										*/
/* Revised:		990718				     													*/
/*																									*/
/*	Function:	Converts an unsigned integer to a hex string						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void wordtohex( word val, char *result) {

	byte i, c;
		
	for ( i = 0; i < 4; i++) {
		c = val % 16;
		val /= 16;
				
		if ( c < 10)
			result[3-i] = '0' + c;
		else
			result[3-i] = 'A' + c - 10;
	}

	result[4] = 0;
}
  
/***************************************************************************/
/*	Cstrcmp																	 		  UTIL.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		990707				     													*/
/*																									*/
/*	Function:	Compare a string in _rom-memory to a string in RAM-memory	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
int Cstrcmp( char *s1, _rom char *s2) {

	signed char	r;

	while( !(r = *s1 - *s2++) && *s1++)
		continue;

	return( r);
}
  
