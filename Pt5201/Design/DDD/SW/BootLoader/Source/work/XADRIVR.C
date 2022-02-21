/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		XADRIVR.C																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	990910, KEn	DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

#include <stdlib.h>

#include "system.h"

#include "xaexprt.h"
#include "xadrivr.h"

_near char rxBufHead;	 	 			  	  	// Index to next available slot in buffer
_near char rxBufTail;	 				  	  	// Index to next slot emptied from buffer
_near char txBufHead;	 		 	  	  		// Index to next available slot in buffer
_near char txBufTail;	 			  	  		// Index to next slot emptied from buffer

static _near char rxBuf[UDR_RXBUFSIZE];	// Driver Receive Buffer
static _near char txBuf[UDR_TXBUFSIZE];	// Driver Transmit Buffer

static _near char rcvChar;						// Temporary holds the received character

static _bit rcvStopped;							// TRUE: remote sender stopped
static _bit trxStopped;							// TRUE: transmitter disabled

static _bit trxEmpty;	 						// TRUE: transmitter not busy

static _bit UART_Handshake;					// TRUE: XOFF/XON handshake enabled

/***************************************************************************/
/*	Udr_InitializeUart														  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		990607, KEn, DEV															*/
/*																									*/
/*	Function:	Initialize HW registers of Uart										*/
/*	Remarks:		Baud Rate constants with oscillator frequency 29.4912 MHz,	*/
/*					prescaler = 4.																*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
void Udr_InitUart( byte Baudrate, byte Handshake) {

	if ( UART_Handshake == UDR_NO_HANDSHAKE)
		Handshake = FALSE;
	else
		Handshake = TRUE;


	ETI1 = 0;								// Disable serial TxD interrupt
	ERI1 = 0;								// Disable serial RxD interrupt

	S1CON &= 0xEC;							// Disable serial reception & clear
												//  pending possible TxD- & RxD interrupts

	TR1 = 0;									// Stop Timer 1, just in case you know

	TXD1 = RXD1 = 1;						// (Re)initialize TxD- & RxD pin to 1

  	txBufHead = txBufTail = 0;			// Initialize RX- & TX-buffer pointers
  	rxBufHead = rxBufTail = 0;

	trxStopped = FALSE;
	trxEmpty = TRUE;

	S1CON = 0xD8;							// UART mode 1: 1 start, 9 data, 1 stop,
												//  (9th bit used as stop bit)

	switch ( Baudrate) {
		case UDR_BAUD_38400:				// 38400 baud
			RTH1 = 0xFF;					// -12
			RTL1 = 0xF4;
			break;

		case UDR_BAUD_19200:				// 19200 baud
			RTH1 = 0xFF;					// -24
			RTL1 = 0xE8;				
			break;

		case UDR_BAUD_115K:				// 115200 baud
			RTH1 = 0xFF;					// -4
			RTL1 = 0xFC;
			break;

		default:								// 9600 baud
			RTH1 = 0xFF;					// -48
			RTL1 = 0xD0;
			break;
	}


	TMOD &= 0x0F;							// Timer 1: 16-bit auto-reload timer

	TR1 = 1;									// Start Timer 1

	S1STAT = 1;								// Enable: framing error, break detect &
												//  overrun error

	ETI1 = 1;								// Enable serial TxD interrupt
	ERI1 = 1;								// Enable serial RxD interrupt
}

/***************************************************************************/
/*	Udr_Uart_RX_Interrupt													  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		990607, KEn, DEV															*/
/*																									*/
/*	Function:	Interrupt routine for the serial RX-communication				*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
_interrupt( IV_RI1/4) _using( IV_SYSTEM+IV_PRI14) void Udr_Uart_RX_Interrupt( void) {

	if ( S1STAT & 0x0E) {
		if ( FE1) 									// If framing error..
			rcvChar = UDR_FRAMING;

		if ( BR1) 									// If break detect error..
			rcvChar = UDR_BREAK;

		if ( OE1) 									// If overrun error..
			rcvChar = UDR_OVERRUN;

		S1STAT &= 0xF1;							// Reset ALL of above errors
	}
	else
		rcvChar = S1BUF;							// Read received character

	RI_1 = 0;					 					// Clear Interrupt flag RI_1


	if ( !UART_Handshake) {
														// If buffer not full..
		if ( Udr_FreeSpaceInRxBuf() > UDR_FLOWSPACE) {
														//  store character
			rxBufHead = ( ++rxBufHead) % UDR_RXBUFSIZE;
			rxBuf[rxBufHead] = rcvChar;
		}
	}
	else {											// if XON/XOFF handshake..
		if ( rcvChar == XOFF)					// If received character is XOFF
			trxStopped = TRUE;					//  then disable the transmitter
		else {
			if ( rcvChar == XON) {				// If received character is XON
				trxStopped = FALSE;				//  then enable the transmitter
				TI_1 = 1;	  						// Generate TI to (re)start
			}											//   transmission
			else {
				if ( Udr_FreeSpaceInRxBuf() <= UDR_FLOWSPACE) {
										
					rcvStopped = TRUE;			// Stop the remote sender
					trxEmpty = FALSE;

					EA = 0;							// Disable interrupt while loading
														//  S1BUF to ensure that TI1 = 0
														//  clears a possible UART empty
														//  flag and NOT a transmission
														//  done flag.

					S1BUF = XOFF;					// Send XOFF
					TI_1 = 0;						// Clear TI again, i.e. bypass
														//  double buffering.
					EA = 1;							// Enable interrupt again.
				}
	
				rxBufHead = ( ++rxBufHead) % UDR_RXBUFSIZE;
				rxBuf[rxBufHead] = rcvChar;
			}
		}
	}
}

/***************************************************************************/
/*	Udr_Uart_TX_Interrupt													  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		990528, KEn, DEV															*/
/*																									*/
/*	Function:	Interrupt routine for the serial TX-communication				*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
_interrupt( IV_TI1/4) _using( IV_SYSTEM+IV_PRI14) void Udr_Uart_TX_Interrupt( void) {

	TI_1 = 0;		 						 		// Clear Interrupt flag TI_1 in S0CON

	if ( !trxStopped && ( txBufTail != txBufHead)) {

		txBufTail = ( ++txBufTail) % UDR_TXBUFSIZE;

		EA = 0; 										// Disable interrupt while loading
														//  S1BUF to ensure that TI_1 = 0
														//  clears a possible UART empty flag
														//  and NOT a transmission done flag.

		S1BUF = txBuf[txBufTail];
		TI_1 = 0;		 							// Clear TI_1 again, i.e. bypass
														//  double buffering.

		EA = 1;										// Enable interrupt again.
	}
	else
		trxEmpty = TRUE;
}

/***************************************************************************/
/*	Udr_ReceiveByte															  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		990607, KEn, DEV															*/
/*																									*/
/*	Function:	Read one character from the receive data buffer					*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    UDR_OK: if new char available, UDR_EOF: if buffer is empty	*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_ReceiveByte( char *c) {

	if ( rxBufHead != rxBufTail) {		  	// If rx buffer is not empty 

		rxBufTail = ( rxBufTail+1) % UDR_RXBUFSIZE;

		if ( UART_Handshake) {
			if ( rcvStopped && ( Udr_FreeSpaceInRxBuf() > ( UDR_FLOWSPACE+2))) {
											 
				rcvStopped = FALSE; 				// start incoming stream again
				trxEmpty	= FALSE;

				EA = 0;								// Disable interrupt while loading
														//  S1BUF to ensure that TI_1 = 0
														//  clears a possible UART empty
														//  flag and NOT a transmission
														//  done flag.

				S1BUF = XON;						// Send XON
				TI_1 = 0;	  						// Clear TI_1 again, i.e. bypass
														//  double buffering.

				EA = 1;								// Enable interrupt again.
			}
		}
		*c = rxBuf[rxBufTail];

		return( UDR_OK);
	}

	return( UDR_EOF);
}

/***************************************************************************/
/*	Udr_FreeSpaceInRxBuf														  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		990528, KEn, DEV															*/
/*																									*/
/*	Function:	Calculate free space in receive buffer								*/
/*	Remarks:		-																				*/
/*	Returns:    No of free bytes in receive buffer									*/
/* Updates:		-																				*/
/***************************************************************************/
byte Udr_FreeSpaceInRxBuf( void) {

	if ( rxBufHead >= rxBufTail)
		return( UDR_RXBUFSIZE + rxBufTail - rxBufHead);
	else
		return( rxBufTail - rxBufHead);
}

/***************************************************************************/
/*	Udr_SendByte																  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		990524, KEn, DEV															*/
/*																									*/
/*	Function:	Write one character to the transmit data buffer					*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    UDR_OK: if no error, UDR_OVL: if buffer is full					*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_SendByte( char ch) {

																 // If buffer is not full..
	if ((( txBufHead+1) % UDR_TXBUFSIZE) != txBufTail) {
	
		txBufHead = ( ++txBufHead) % UDR_TXBUFSIZE;
		txBuf[txBufHead] = ch;

		if ( trxEmpty && !trxStopped) {

			trxEmpty = FALSE;
			txBufTail = ( ++txBufTail) % UDR_TXBUFSIZE;

			EA = 0; 											// Disable interrupt while loading
																//  S1BUF to ensure that TI_1 = 0
																//  clears a possible UART empty
																//  flag and NOT a transmission
																//  done flag.

			S1BUF = txBuf[txBufTail];
			TI_1 = 0;										// Clear TI_1 again, i.e. bypass
																//  double buffering.

			EA = 1;											// Enable interrupt again.
		}
		return( UDR_OK);
	}
	return( UDR_OVL);
}
