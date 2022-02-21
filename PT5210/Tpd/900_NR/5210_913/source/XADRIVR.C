/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator								*/
/*	Module:		XADRIVR.C																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	980915, KEn	DEV															*/
/*	Status:		Version 0.5																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/* 980915 In Udr_Uart_TX_Interrupt(), Udr_Uart_RX_Interrupt(), Udr_Send-	*/
/*			  byte & Udr_ReceiveByte(): due to the change of specification		*/
/*			  in the XA-G3 processor introducing double buffering in the UART	*/
/*			  transmitter, this routine now FORCES the single buffering			*/
/*			  method. NOTE! Since the ninth bit in the transmitter, T0B8, is	*/
/*			  used for parity, double buffering should NOT be used, because	*/
/*			  this bit is NOT double buffered.											*/
/* 980616 The return value of Udr_FreeSpaceInRxBuf( void) is now UC			*/
/*			 instead of char, (problems with software handshake)					*/
/* 971015 Implemented 19200 baud & No handshake										*/
/* 971014 Errors in the parity handling corrected									*/
/* 971010 Optimized Udr_InitializeUart() & Udr_LoopbackTest()					*/
/* 971008 Added Udr_LoopbackTest()														*/
/* 970625 declaration of delay altered to static int								*/
/* 970303 Added UDR_PARITY & UDR_FRAMING etc.										*/
/* 970226 Now driver ALWAYS use 8 databit, 1 RX- & 2 TX-stopbit				*/
/* 970217 Removed parameters in Udr_InitializeUart									*/
/***************************************************************************/

#include <stdlib.h>
#include <xa.h>
#include <intrpt.h>

#include "mas.h"
#include "52sync.h"

#include "tables.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "rs232err.h"
#include "util.h"

near char rxBufHead;						// Index to next available slot in buffer
near char rxBufTail;						// Index to next slot emptied from buffer
near char txBufHead;						// Index to next available slot in buffer
near char txBufTail;						// Index to next slot emptied from buffer

static char rxBuf[UDR_RXBUFSIZE];	// Driver Receive Buffer
static char txBuf[UDR_TXBUFSIZE];	// Driver Transmit Buffer

static bit trxStopped;					// TRUE: transmitter disabled
static bit trxEmpty;						// TRUE: transmitter not busy

static char rcvStopped;					// TRUE: remote sender stopped
static char rcvChar;						// Temp. holds the received character
static bit rcvParity;					// Temp. holds the parity of the received
												//  character

static UC UART_Parity;					// User selected parity
static UC UART_Databit;					// User selected no of databits
static UC UART_Handshake;				// User selected handshake

static int delay;							// Delay equal to 1 stop bit	

static bit tmpbit;						// Generel user bit

/***************************************************************************/
/*	Udr_InitializeUart														  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		971015, KEn, DEV															*/
/*																									*/
/*	Function:	Initialize HW registers of Uart										*/
/*	Remarks:		-																				*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
void Udr_InitializeUart( UC Baudrate, UC Databit, UC Parity, UC Handshake) {

	int tmp;

	UART_Parity = Parity;
	UART_Databit = Databit;
	UART_Handshake = Handshake;

// Baud Rate constants with oscillator frequency 29.4912 MHz, prescaler = 4
// Delay same as above

	switch ( Baudrate) {
		case UDR_BAUD_300:
			delay = 3333;						// 3.333 ms
			tmp = -1536;						// 300 baud
			break;
		case UDR_BAUD_600:
			delay = 1667;						// 1.667 ms
			tmp = -768;							// 600 baud
			break;
		case UDR_BAUD_1200:
			delay = 833;						// 0.833 ms
			tmp = -384;							// 1200 baud
			break;
		case UDR_BAUD_2400:
			delay = 417;						// 0.417 ms
			tmp = -192;							// 2400 baud
			break;
		case UDR_BAUD_4800:
			delay = 209;						// 0.208 ms
			tmp = -96;							// 4800 baud
			break;
		case UDR_BAUD_9600:
			delay = 104;						// 0.104 ms
			tmp = -48;							// 9600 baud
			break;
		case UDR_BAUD_19200:
			delay = 53;							// 0.052 ms
			tmp = -24;							// 19200 baud
			break;
	}

	ETI0 = 0;								// Disable serial TxD interrupt
	ERI0 = 0;								// Disable serial RxD interrupt
	EX0 = 0;									// Disable CTS interrupt

	S0CON &= 0xEC;							// Disable serial reception & clear
												//  pending possible TxD- & RxD interrupts

	TR1 = 0;									// Stop Timer 1, just in case you know

	TXD0 = RXD0 = 1;						// (Re)initialize TxD- & RxD pin to 1

  	txBufHead = txBufTail = 0;			// Initialize RX- & TX-buffer pointers
  	rxBufHead = rxBufTail = 0;

	trxStopped = FALSE;
	trxEmpty = TRUE;

	if (( UART_Databit == UDR_DATA7) || ( UART_Parity == UDR_NONE))
		S0CON = 0x58;			// UART mode 1: 1 start, 8 data, 1 stop
	else
		S0CON = 0xD8;			// UART mode 3: 1 start, 8 data, 1 prgm., 1 stop

	switch ( UART_Handshake) {
		case UDR_NO_HANDSHAKE:
		case UDR_XON_XOFF:
			RTS_OFF;							// Set RTS output
			EX0 = 0;							// Disable CTS interrupt
			break;

		case UDR_RTS_CTS:
			RTS_ON;							// Activate RTS
			IT0 = 1;							// Specify falling edge triggered interrupt
			EX0 = 1;							// Enable CTS interrupt
			break;
	}

	RTH1 = (UC) ( tmp >> 8);			// Set baud rate
	RTL1 = (UC) ( tmp & 0x00FF);

	TMOD &= 0x0F;							// Timer 1: 16-bit auto-reload timer

	TR1 = 1;									// Start Timer 1

	S0STAT = 1;								// Enable: framing error, break detect &
												//  overrun error

	ETI0 = 1;								// Enable serial TxD interrupt
	ERI0 = 1;								// Enable serial RxD interrupt
}

/***************************************************************************/
/*	Udr_Uart_RX_Interrupt													  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980915, KEn, DEV															*/
/*																									*/
/*	Function:	Interrupt routine for the serial RX-communication				*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
interrupt void Udr_Uart_RX_Interrupt( void) {

	if ( S0STAT & 0x0E) {
		if ( FE0) 									// If framing error..
			rcvChar = UDR_FRAMING;

		if ( BR0) 									// If break detect error..
			rcvChar = UDR_BREAK;

		if ( OE0) 									// If overrun error..
			rcvChar = UDR_OVERRUN;

		S0STAT &= 0xF1;							// Reset ALL of above errors
	}
	else {
		rcvChar = S0BUF;							// Read received character
		rcvParity = GetParity( rcvChar);

		switch ( UART_Parity) {
			case UDR_NONE:
				if ( UART_Databit == UDR_DATA7)
					rcvChar &= 0x7F;
				break;

			case UDR_ODD:
				if ( UART_Databit == UDR_DATA8) {
					if ( !( rcvParity ^ R0B8))
						rcvChar = UDR_PARITY;
				}
				else {
					if ( !rcvParity)
						rcvChar = UDR_PARITY;
					else
						rcvChar &= 0x7F;
				}
				break;

			case UDR_EVEN:
				if ( UART_Databit == UDR_DATA8) {
					if ( rcvParity ^ R0B8)
						rcvChar = UDR_PARITY;
				}
				else {
					if ( rcvParity)
						rcvChar = UDR_PARITY;
					else
						rcvChar &= 0x7F;
				}
				break;
		}
	}
	RI0 = 0;					 							// Clear Interrupt flag RI0

	if ( rcvChar == LOCK) {							// If local lockout switchover..
		if ( SerialRemoteLockoutOn)
			SerialRemoteLockoutOn = FALSE;
		else
			SerialRemoteLockoutOn = TRUE;
	}
	else {
		switch ( UART_Handshake) {
			case UDR_NO_HANDSHAKE:					// if no handshake..
															// If buffer not full..
				if ( Udr_FreeSpaceInRxBuf() > UDR_FLOWSPACE) {
															//  store character
					rxBufHead = ( ++rxBufHead) % UDR_RXBUFSIZE;
					rxBuf[rxBufHead] = rcvChar;
				}
				break;

			case UDR_XON_XOFF:						// if XON/XOFF handshake..
				if ( rcvChar == XOFF)				// If received character is XOFF
					trxStopped = TRUE;				//  then disable the transmitter
				else {
					if ( rcvChar == XON) {			// If received character is XON
						trxStopped = FALSE;			//  then enable the transmitter
						TI0 = 1;							// Generate TI to (re)start
					}										//   transmission
					else {
						if ( Udr_FreeSpaceInRxBuf() <= UDR_FLOWSPACE) {
											
							rcvStopped = TRUE;		// Stop the remote sender
							trxEmpty = FALSE;

							waitus( delay);			// Wait for one stop bit

							di();							// Disable interrupt while loading
															//  S0BUF to ensure that TI0 = 0
															//  clears a possible UART empty
															//  flag and NOT a transmission
															//  done flag.

							S0BUF = XOFF;				// Send XOFF
							TI0 = 0;						// Clear TI again, i.e. bypass
															//  double buffering.

							ei();							// Enable interrupt again.
						}
	
						rxBufHead = ( ++rxBufHead) % UDR_RXBUFSIZE;
						rxBuf[rxBufHead] = rcvChar;
					}
				}
				break;

			case UDR_RTS_CTS:							// if RTS/CTS Handshake..
				if ( Udr_FreeSpaceInRxBuf() <= UDR_FLOWSPACE) {
															// stop the remote sender
					RTS_OFF;								// set RTS  (active low)
					rcvStopped = TRUE;
				}

				rxBufHead = ( ++rxBufHead) % UDR_RXBUFSIZE;
				rxBuf[rxBufHead] = rcvChar;
				break;
		}
	}
}

/***************************************************************************/
/*	Udr_Uart_TX_Interrupt													  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980915, KEn, DEV															*/
/*																									*/
/*	Function:	Interrupt routine for the serial TX-communication				*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
interrupt void Udr_Uart_TX_Interrupt( void) {

	TI0 = 0;									 		// Clear Interrupt flag TI in S0CON

	if ( UART_Handshake == UDR_RTS_CTS) {	// if RTS/CTS Handshake..
		if ( INT0 == 1)
			trxStopped = TRUE;
	}

	if ( !trxStopped && ( txBufTail != txBufHead)) {

		txBufTail = ( ++txBufTail) % UDR_TXBUFSIZE;

		waitus( delay);							// Wait for one stop bit

		if ( UART_Databit == UDR_DATA8)
			Set_8Bit_Parity( txBuf[txBufTail]);

		di();											// Disable interrupt while loading
														//  S0BUF to ensure that TI0 = 0
														//  clears a possible UART empty flag
														//  and NOT a transmission done flag.

		S0BUF = txBuf[txBufTail];
		TI0 = 0;										// Clear TI again, i.e. bypass
														//  double buffering.

		ei();											// Enable interrupt again.
	}
	else
		trxEmpty = TRUE;
}

/***************************************************************************/
/*	Udr_CTS_Interrupt															  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		971010, KEn, DEV															*/
/*																									*/
/*	Function:	Interrupt routine for the CTS interrupt line						*/
/*	Remarks:		This routine is executed after a status change from passive	*/
/*					to active on the CTS Interrupt line (INT0)						*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
interrupt void Udr_CTS_Interrupt( void) {

	trxStopped = FALSE;				// return EOF if no character is available

	if ( trxEmpty) {					// extra check to be sure
		trxEmpty = FALSE;				// Transmitter buffer empty
		TI0 = 1;							// Generate TI to (re)start transmission
	}
}

/***************************************************************************/
/*	Udr_ReceiveByte															  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980915, KEn, DEV															*/
/*																									*/
/*	Function:	Read one character from the receive data buffer					*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    UDR_OK: if new char available, UDR_EOF: if buffer is empty	*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_ReceiveByte( char *c) {

	if ( rxBufHead != rxBufTail) {					// If rx buffer is not empty 

		rxBufTail = ( rxBufTail+1) % UDR_RXBUFSIZE;

		switch ( UART_Handshake) {
			case UDR_NO_HANDSHAKE:
				break;

			case UDR_XON_XOFF:
				if ( rcvStopped && ( Udr_FreeSpaceInRxBuf() > ( UDR_FLOWSPACE+2))) {
											 
					rcvStopped = FALSE; 					// start incoming stream again
					trxEmpty	= FALSE;

					waitus( delay);						// Wait for one stop bit

					di();									// Disable interrupt while loading
															//  S0BUF to ensure that TI0 = 0
															//  clears a possible UART empty
															//  flag and NOT a transmission
															//  done flag.

					S0BUF = XON;						// Send XON
					TI0 = 0;								// Clear TI again, i.e. bypass
															//  double buffering.

					ei();									// Enable interrupt again.
				}
				break;

			case UDR_RTS_CTS:
				if ( rcvStopped && ( Udr_FreeSpaceInRxBuf() > UDR_FLOWSPACE)) {
					RTS_ON;									// activate RTS  (active low)
					rcvStopped = FALSE;	  				// start incoming stream again
				}
				break;
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
/* Revised:		980616, KEn, DEV															*/
/*																									*/
/*	Function:	Calculate free space in receive buffer								*/
/*	Remarks:		-																				*/
/*	Returns:    No of free bytes in receive buffer									*/
/* Updates:		-																				*/
/***************************************************************************/
UC Udr_FreeSpaceInRxBuf( void) {

	if ( rxBufHead >= rxBufTail)
		return( UDR_RXBUFSIZE + rxBufTail - rxBufHead);
	else
		return( rxBufTail - rxBufHead);
}

/***************************************************************************/
/*	Udr_SendByte																  XADRIVR.C */
/*																									*/
/* Author:		P.H.Seerden/Kim Engedahl, DEV											*/
/* Revised:		980915, KEn, DEV															*/
/*																									*/
/*	Function:	Write one character to the transmit data buffer					*/
/*	Remarks:		The routine has been redesigned to explicitly run as a		*/
/*					single buffered transmitter.											*/
/*	Returns:    UDR_OK: if no error, UDR_OVL: if buffer is full					*/
/* Updates:		-																				*/
/***************************************************************************/
char Udr_SendByte( char ch) {

	feed_dog;
																 // If buffer is not full..
	if ((( txBufHead+1) % UDR_TXBUFSIZE) != txBufTail) {
	
		if ( UART_Databit == UDR_DATA7)
			Set_7Bit_Parity( &ch);
			
		txBufHead = ( ++txBufHead) % UDR_TXBUFSIZE;
		txBuf[txBufHead] = ch;

		if ( trxEmpty && !trxStopped) {

			trxEmpty = FALSE;
			txBufTail = ( ++txBufTail) % UDR_TXBUFSIZE;

			waitus( delay);								// Wait for one stop bit

			if ( UART_Databit == UDR_DATA8)
				Set_8Bit_Parity( txBuf[txBufTail]);

			di();											// Disable interrupt while loading
															//  S0BUF to ensure that TI0 = 0
															//  clears a possible UART empty
															//  flag and NOT a transmission
															//  done flag.

			S0BUF = txBuf[txBufTail];
			TI0 = 0;										// Clear TI again, i.e. bypass
															//  double buffering.

			ei();											// Enable interrupt again.
		}
		return( UDR_OK);
	}
	return( UDR_OVL);
}

/***************************************************************************/
/*	GetParity																 		UTIL.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		971014				     													*/
/*																									*/
/*	Function:	Check the parity of a 8-bit character								*/
/*	Remarks:																						*/
/*	Returns:		-------																		*/
/*	Updates:		-------																		*/
/***************************************************************************/
bit GetParity( char ch) {

	char j;

	for ( tmpbit = 0, j = 1; j != 0; j <<= 1)
		if ( ch & j)
			tmpbit ^= 1;

	return( tmpbit);
}

/***************************************************************************/
/*	Set_7Bit_Parity														 		UTIL.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		971014				     													*/
/*																									*/
/*	Function:	Find and set/clear the parity of a 7-bit character.			*/
/*	Remarks:		Parity is set/cleared in the 8th bit								*/
/*	Returns:		-------																		*/
/*	Updates:		-------																		*/
/***************************************************************************/
void Set_7Bit_Parity( char* ch) {

	char j;

	if ( UART_Parity == UDR_NONE)
		*ch |= '\x80';							// MUST be one, used as stop bit
	else {
		for ( tmpbit = 0, j = 1; j != '\x80'; j <<= 1) 
			if ( *ch & j)
				tmpbit ^= 1;

		if ( UART_Parity == UDR_ODD) {
			if ( tmpbit)
				*ch &= '\x7F';
			else
				*ch |= '\x80';
		}
		else {
			if ( tmpbit)
				*ch |= '\x80';
			else
				*ch &= '\x7F';
		}
	}
}

/***************************************************************************/
/*	Set_8Bit_Parity														 		UTIL.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960828	    										*/
/* Revised:		971015				     													*/
/*																									*/
/*	Function:	Find and set/clear the parity of a 8-bit character.			*/
/*	Remarks:		Parity is set/cleared in the register T0B8						*/
/*	Returns:		-------																		*/
/*	Updates:		-------																		*/
/***************************************************************************/
void Set_8Bit_Parity( char ch) {

	char j;

	if ( UART_Parity > UDR_NONE) {

		for ( tmpbit = 0, j = 1; j != 0; j <<= 1) 
			if ( ch & j)
				tmpbit ^= 1;

		if ( UART_Parity == UDR_ODD)
			T0B8 = tmpbit ^ 1;
		else 
			T0B8 = tmpbit & 1;
	}
}

/***************************************************************************/
/*	Udr_RS232LoopbackTest													  XADRIVR.C */
/*																									*/
/* Author:		Kim Engedahl, DEV															*/
/* Revised:		971010, KEn, DEV															*/
/*																									*/
/*	Function:	Test if loopback connector is present								*/
/*	Remarks:		-																				*/
/*	Returns:    Nothing																		*/
/* Updates:		-																				*/
/***************************************************************************/
bit Udr_RS232LoopbackTest( void) {

	UC present = FALSE;

	ETI0 = 0;										// Disable serial TxD interrupt
	ERI0 = 0;										// Disable serial RxD interrupt
	EX0 = 0;											// Disable CTS interrupt

	TXD0 = 0;										// TxD = 0
	INT1 = 0;										// RTS = 0

	waitms( 1);
	feed_dog;										// 
	if (( RXD0 == 0) && ( INT0 == 0)) {
		TXD0 = 0;									// TxD = 0
		INT1 = 1;									// RTS = 1

		waitms( 1);
		feed_dog;
		if (( RXD0 == 0) && ( INT0 == 1)) {
			TXD0 = 1;								// TxD = 1
			INT1 = 0;								// RTS = 0

			waitms( 1);
			feed_dog;
			if (( RXD0 == 1) && ( INT0 == 0)) {
				TXD0 = 1;							// TxD = 1
				INT1 = 1;							// RTS = 1

				waitms( 1);
				feed_dog;
				if (( RXD0 == 1) && ( INT0 == 1))
					present = TRUE;
			}
		}
	}
																// Re-initialize UART
	Udr_InitializeUart( UDR_BAUD_9600, UDR_DATA8, UDR_NONE, UDR_RTS_CTS);

	if ( present)
		return( TRUE);

	return( FALSE);
}
