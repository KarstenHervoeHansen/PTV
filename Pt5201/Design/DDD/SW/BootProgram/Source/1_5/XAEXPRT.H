/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		XAEXPRT.H																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	990902, KEn	DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains exported declarations of the XA, Uart Driver 		*/
/*	package. Include this module in your source file if you want to make		*/
/*	use of one of the interface functions of the package.							*/
/*																									*/
/*	History:																						*/
/***************************************************************************/
#ifndef _XAEXPRT_H
#define _XAEXPRT_H

#define UDR_FLOWSPACE	2
#define UDR_RXBUFSIZE	64
#define UDR_TXBUFSIZE	16

#define XON					0x11		// character defined as XON
#define XOFF				0x13		// character defined as XOFF
#define CTRLC				0x03		// character defined as <CTRL-C>

											// User programmable baudrates
enum UDR_Baudrate			{ UDR_BAUD_9600 = 0, UDR_BAUD_19200, UDR_BAUD_38400,
								  UDR_BAUD_57600, UDR_BAUD_115K };

											// User programmable handshake
enum UDR_Handshake		{ UDR_NO_HANDSHAKE = 0, UDR_XON_XOFF };

											// Status Messages/Errors
#define UDR_OK			( char)  0
#define UDR_EOF		( char) -1
#define UDR_OVL		( char) -2

#define UDR_OVERRUN	( char) -3
#define UDR_PARITY	( char) -4
#define UDR_FRAMING	( char) -5
#define UDR_BREAK		( char) -6

//**************************************************************************
//			I N T E R F A C E   F U N C T I O N   P R O T O T Y P E S			*
//**************************************************************************
extern char Udr_InChar( void);
extern void Udr_PutChar( char ch);
extern void Udr_PutNChar( char *s);
extern void Udr_CPutNChar( _rom char *s);

extern void Udr_InitUart( byte Baudrate, byte Handshake);

#endif
