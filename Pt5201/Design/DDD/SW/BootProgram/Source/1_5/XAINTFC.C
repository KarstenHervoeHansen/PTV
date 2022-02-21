/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		XAINTFC.C																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	990902, KEn	DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

#include "system.h"

#include "xaexprt.h"
#include "xadrivr.h"

/***************************************************************************/
/*	Udr_InChar																	  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		971015, KEn, DEV															*/
/*																									*/
/*	Function:	Check for a received character and return immediate			*/
/*	Remarks:		-																				*/
/*	Returns:		New character or EOF if no char. available						*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_InChar( void) {

	char temp;
	
	if ( Udr_ReceiveByte( &temp) == UDR_EOF)			// No char. available
   	return( UDR_EOF);

	return( temp);
}

/***************************************************************************/
/*	Udr_PutChar																	  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		971015, KEn, DEV															*/
/*																									*/
/*	Function:	-																				*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/* Updates:		-																				*/
/***************************************************************************/
void Udr_PutChar( char ch) {

	while ( Udr_SendByte( ch) == UDR_OVL)       // as long as buffer full
		;
}

/***************************************************************************/
/*	Udr_PutNChar																  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		970224, KEn, DEV															*/
/*																									*/
/*	Function:	-																				*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/* Updates:		-																				*/
/***************************************************************************/
void Udr_PutNChar( char *s) {

	while ( *s) {
		while ( Udr_SendByte( *s) == UDR_OVL)	// as long as buffer full
			;
		s++;
	}
}

/***************************************************************************/
/*	Udr_CPutNChar																  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		970224, KEn, DEV															*/
/*																									*/
/*	Function:	-																				*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/* Updates:		-																				*/
/***************************************************************************/
void Udr_CPutNChar( _rom char *s) {

	while ( *s) {
		while ( Udr_SendByte( *s) == UDR_OVL)			// as long as buffer full
			;
		s++;
	}
}

