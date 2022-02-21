/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		XAINTFC.C																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	980616, KEn	DEV															*/
/*	Status:		Version 0.2																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/* 980616: FreeSpaceInTxBuf(), Rx_Count() and Tx_Count() now returns UC		*/
/*			  instead of char.																*/
/***************************************************************************/

#include <xa.h>

#include "xaexprt.h"
#include "xadrivr.h"

/***************************************************************************/
/*	FreeSpaceInTxBuf															  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980616, KEn, DEV															*/
/*																									*/
/*	Function:	Calculate free space in transmit buffer							*/
/*	Remarks:		-																				*/
/*	Returns:    No of free bytes in transmit buffer									*/
/* Updates:		-																				*/
/***************************************************************************/
UC FreeSpaceInTxBuf( void) {
   
	if ( txBufHead >= txBufTail)
   	return( UDR_TXBUFSIZE + txBufTail - txBufHead);
	else
   	return( txBufTail - txBufHead);
}

/***************************************************************************/
/*	Udr_RxCount																	  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980616, KEn, DEV															*/
/*																									*/
/*	Function:	Calculate the number of characters waiting in RxBuf			*/
/*	Remarks:		-																				*/
/*	Returns:    No of free bytes in receive buffer									*/
/* Updates:		-																				*/
/***************************************************************************/
UC Udr_RxCount( void) {

	return( UDR_RXBUFSIZE - Udr_FreeSpaceInRxBuf());
}

/***************************************************************************/
/*	Udr_RxFlush																	  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		971015, KEn, DEV															*/
/*																									*/
/*	Function:	Flush n characters from the receive buffer						*/
/*	Remarks:		-																				*/
/*	Returns:    0 if n characters out of n++ has been flushed, otherwise EOF*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_RxFlush( char n) {

	if ( n == 0 || ( UDR_RXBUFSIZE - Udr_FreeSpaceInRxBuf() <= n)) {
		rxBufTail = rxBufHead;
		return( UDR_EOF);
	}

	rxBufTail = ( rxBufTail + n) % UDR_RXBUFSIZE;
	return( 0);
}

/***************************************************************************/
/*	Udr_TxCount																	  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980616, KEn, DEV															*/
/*																									*/
/*	Function:	Calculate the number of characters remaining in TxBuf			*/
/*	Remarks:		-																				*/
/*	Returns:    No of occupied bytes in the transmit buffer						*/
/* Updates:		-																				*/
/***************************************************************************/
UC Udr_TxCount( void) {

	return( UDR_TXBUFSIZE - FreeSpaceInTxBuf());
}

/***************************************************************************/
/*	Udr_TxFlush																	  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		971015, KEn, DEV															*/
/*																									*/
/*	Function:	Flush n characters from the transmit buffer						*/
/*	Remarks:		-																				*/
/*	Returns:    0 if n characters out of n++ has been flushed, otherwise EOF*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_TxFlush( char n) {

	if ( n == 0 || ( UDR_TXBUFSIZE - FreeSpaceInTxBuf() <= n)) {
   	txBufHead = txBufTail;
    	return( UDR_EOF);
	}

	txBufHead = ( txBufHead + UDR_TXBUFSIZE - n) % UDR_TXBUFSIZE;
	return( 0);
}

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
void Udr_CPutNChar( code char *s) {

	while ( *s) {
		while ( Udr_SendByte( *s) == UDR_OVL)			// as long as buffer full
			;
		s++;
	}
}

/***************************************************************************/
/*	Udr_PutNewline																  XAINTFC.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		970224, KEn, DEV															*/
/*																									*/
/*	Function:	Send out a linefeed && Carriage return								*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/* Updates:		-																				*/
/***************************************************************************/
void Udr_PutNewline( void) {

	Udr_PutNChar( "\r\n");
}
