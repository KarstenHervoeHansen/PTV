/****************************************************************************/
/* MODULE:                                                                  */
/*  util.c - 	General purpose utility function															*/
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/**************************************************************************/
/* Include files:																													*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "define.h"
#include "util.h"

/*************************************************************************/
/*	NONE					   	 																				 		UTIL.C */
/*									  																									 */
/* Author:		Kim Engedahl, DEV, 960828	    		 												 */
/* Revised:		970105				     		   																	 */
/*									   																									 */
/*	Function:	DOES ABSOLUTELY NOTHING!!!			  												 */
/*	Remarks:		This one is primarily used in menu tables  							 */
/*	Returns:		--					   																					 */
/*	Updates:		--					   																					 */
/*************************************************************************/
void None( void)
{
}

/**************************************************************************/
/* BitPos						  																					   UTIL.C */
/*									 																											*/
/* Author:		Kim Engedahl, DEV, 970919	    		 													*/
/* Revised:		970919				     																				  */
/*																																			  */
/* Function:	Return the bit position of a variable											  */
/* Remarks: 	If parameter contains more than one bit the number of the 	*/
/*						first bit is returned		  																	*/
/* Returns:		--																												  */
/* Updates:		--					  																							*/
/**************************************************************************/
UC BitPos( UC value)
{

	UC cnt = 1;

	while ( !( value & 0x01) && ( cnt < 9))
	{
		cnt++;
		value >>= 1;
	}

	return( cnt);
}

/**************************************************************************/
/* HexToByte																 										   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Converts a char defined in hex to binary										*/
/* Remarks:		--																													*/
/* Returns:		A byte from 0-15																						*/
/* Updates:		--																													*/
/**************************************************************************/
UC HexToByte( char s) {

	toupper( s);

	if (( s >= 'A') && ( s <= 'F'))
		return( s - 'A' + 10);

	return( s - '0');
}

/**************************************************************************/
/* htol																		 		 										 UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Converts a string containg a hex number to an unsigned long	*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
UC htol( char* s, UL* result)
{
	*result = 0;

	while ( *s)
		*result = 16*(*result) + HexToByte( *s++);

	return( 0);
}

/**************************************************************************/
/* otol																		 											   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Converts a string containg an octal number to unsinged long	*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
UC otol( char* s, UL* result)
{
	*result = 0;

	while ( *s)
		*result = 8*(*result) + ( *s++ - '0');

	return( 0);
}

/**************************************************************************/
/* btol																											 		   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Converts a string containg a binary number to unsigned long	*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
UC btol( char* s, UL* result)
{
	*result = 0;

	while ( *s)
		*result = 2*(*result) + ( *s++ - '0');

	return( 0);
}

/**************************************************************************/
/* _ultoa																	 											   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970219				     																					*/
/*																																				*/
/* Function:	Converts an unsigned long to a string												*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void _ultoa( UL val, char* result)
{
	UL tmp = val;
	UC i=0, j=0, TmpChar;

	if ( val == 0)
		strcpy( result, "0");
	else
	{
		while ( val)
		{
			tmp = val / 10;
			result[i++] = (val % 10) + '0';
			val = tmp;
		}
		result[i--] = '\0';

		while ( i >= j)							// Reverse ASCII string
		{
			TmpChar = result[i];
			result[i] = result[j];
			result[j] = TmpChar;
			i--;
			j++;
		}
	}
}

/**************************************************************************/
/* Cstrlen																	 										   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    													 		*/
/* Revised:		970105				     																					*/
/*																															 					*/
/* Function:	Calculates the length of a string from code memory					*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
size_t Cstrlen( register char code* s)
{
	register char code *	cp;

	cp = s;
	while( *cp++)
		continue;

	return( cp-s-1);
}

/**************************************************************************/
/* Cstrcat																	 										   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Concatenate a string from code memory to data memory				*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
char* Cstrcat( register char* to, register char code* from)
{
	register char* cp;

	cp = to;
	while( *cp)
		cp++;

	*cp = *from++;
	while( *cp++)
		continue;

	return( to);
}

/**************************************************************************/
/* Cmemcpy																									 		   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Copy a memory-area from code-memory to RAM-memory						*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void* Cmemcpy( void* d1, void code* s1, register size_t n)
{
	register char* d;
	register char code* s;

	s = s1;
	d = d1;

	while( n--)
		*d++ = *s++;

	return( d1);
}

/**************************************************************************/
/* Cstrcpy																									 		   UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 971003	    															*/
/* Revised:		971003				     																					*/
/*																																				*/
/* Function:	Copy a string in code-memory to a string in RAM-memory			*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
char* Cstrcpy( char* to, register char code * from)
{
	register char*	cp;

	for ( cp=to; *from; *cp++ = *from++ )
		;

	*cp = 0;

	return( to);
}

/**************************************************************************/
/* Cstrcmp																	 											 UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:		970105				     																					*/
/*																																				*/
/* Function:	Compare a string in code-memory to a string in RAM-memory		*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
int Cstrcmp( register const char * s1, register char code * s2)
{
	register signed char	r;

	while( !(r = *s1 - *s2++) && *s1++)
		continue;

	return( r);
}

/**************************************************************************/
/* Cstrncmp																							 	         UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 960828	    															*/
/* Revised:	  970105				     																					*/
/*																																				*/
/* Function:	Compare a string in code-memory to a string in RAM-memory		*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
int Cstrncmp( register const char * s1, register char code * s2, size_t len)
{
	while( len--)
	{
		if ( *s1 == 0 || *s1 != *s2)
			return( *s1 - *s2);

		s1++;
		s2++;
	}

	return( 0);
}

/**************************************************************************/
/* ValidateDate																					 	         UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000504	    															*/
/* Revised:	  000504				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
int ValidateDate( UI year, UI month, UI day)
{
	switch ( month)
	{
		case January:
		case March:
		case May:
		case July:
		case September:
		case November:
			if ( day > 31)
				return( FAIL);
			break;

		case February:
			if ( year % 4)
			{
				if ( day > 29)
					return( FAIL);
			}
			else
			{
				if ( day > 28)
					return( FAIL);
			}
			break;

		default:
			if ( day > 30)
				return( FAIL);
			break;
	}

	return( OK);
}

/**************************************************************************/
/* ROMChecksum																					 	         UTIL.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000705	    															*/
/* Revised:	  000705				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
UI ROMChecksum( UL start_addr, UL end_addr)
{
	_rom UC *Sptr;
	_rom UC *Eptr;

	UI CalcChecksum = 0;

	Sptr = ( _rom UC *) ( start_addr);
	Eptr = ( _rom UC *) ( end_addr);

	while ( Sptr <= Eptr)
		CalcChecksum += *Sptr++;

	return( CalcChecksum);
}