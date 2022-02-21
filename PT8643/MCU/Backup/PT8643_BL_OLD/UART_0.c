//-----------------------------------------------------------------------------
//	Filename	: UART_0.c
//	Date		: 2010-05-31
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "UART_0.h"
#include <c8051F120.h>                     	// Device-specific SFR Definitions
#include "textstr.h"
#include <string.h>

//-----------------------------------------------------------------------------
// Constant
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
Ttxbuffer TXBuffer[ TXBUFSIZE ];		// Ring buffer for TX. MCU --> PC
Trxbuffer RXBuffer[ RXBUFSIZE ];		// Ring buffer for RX. PC  --> MCU

										// Pointers for ring buffer.
Ttxbuffer * TXRDPtr;					// GET data from TX buffer.
Ttxbuffer * TXWTPtr;					// PUT data into TX buffer.
Trxbuffer * RXRDPtr;					// GET data from RX buffer.
Trxbuffer * RXWTPtr;					// PUT data into RX buffer.

unsigned int TXcount;					// Number of bytes to transmit to PC.
unsigned int RXcount;					// Number of data recieved from PC.
bit TXReady;							// TX buffer has data to transmit.
bit RXReady;							// RX buffer has room to receive.

//-----------------------------------------------------------------------------
// UART Subroutines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Flush_COM0buffers	: Clear UART0 buffers.
//-----------------------------------------------------------------------------
void Flush_COM0buffers(void)
{
	unsigned int i;

	TXRDPtr = TXBuffer;
	TXWTPtr = TXBuffer;
	RXRDPtr = RXBuffer;
	RXWTPtr = RXBuffer;
	TXcount = 0;
	RXcount = 0;
	TXReady = 0;
	RXReady = 1;

	for(i=0; i<RXBUFSIZE; i++)	RXBuffer[i] = 0x00;
	for(i=0; i<TXBUFSIZE; i++)	TXBuffer[i] = 0x00;
}

//-----------------------------------------------------------------------------
// COM0GetByte		:  Get data from RX buffer. (Get data from PC.)
//-----------------------------------------------------------------------------
unsigned char COM0GetByte(void)
{
	unsigned char dt;

	dt = *RXRDPtr;
	if ( RXRDPtr == (RXBuffer + (RXBUFSIZE - 1)) )	// at the end of the buffer
		RXRDPtr = RXBuffer;							// go to the head of the buffer
	else
		RXRDPtr++;

	RXcount--;
	RXReady = (RXcount != 0);

	return dt;
}

//-----------------------------------------------------------------------------
// COM0PutByte		:  Put data into TX buffer. (Send data to PC.)
//-----------------------------------------------------------------------------
void COM0PutByte(unsigned char dt)
{
	*TXWTPtr = dt;
	if ( TXWTPtr == (TXBuffer + (TXBUFSIZE - 1)) )	// at the end of the buffer
		TXWTPtr = TXBuffer;					// go to the head of the buffer
	else
		TXWTPtr++;

	TXcount++;
	TXReady = (TXcount != TXBUFSIZE);
}


//-----------------------------------------------------------------------------
// COM0GetByte_ISR	:  PUT data into RX buffer. (Get data from PC. Called from ISR)
//-----------------------------------------------------------------------------
void COM0GetByte_ISR(unsigned char dt)
{
	if (RXWTPtr > (RXBUFSIZE + RXBuffer - 1))
	{
		RXWTPtr = RXBuffer;
	}

	*RXWTPtr = dt;
	RXWTPtr++;
	RXcount++;
	RXReady = (RXcount != 0);
}

//-----------------------------------------------------------------------------
// COM0PutByte_ISR	:  Get data from TX buffer. (Send data to PC. Called from ISR)
//-----------------------------------------------------------------------------
unsigned char COM0PutByte_ISR(void)
{
	unsigned char dt;

	dt = *TXRDPtr;
	if ( TXRDPtr == (TXBuffer + (TXBUFSIZE - 1)) )	// at the end of the buffer
		TXRDPtr = TXBuffer;					// go to the head of the buffer
	else
		TXRDPtr++;

	TXcount--;
	TXReady = (TXcount != 0);

	return dt;
}

//-----------------------------------------------------------------------------
// Transmit_COM0	:  Start transfer of TX buffer to PC.
//-----------------------------------------------------------------------------
void Transmit_COM0(void)
{
	SFRPAGE = UART0_PAGE;
	TI0	= 1;
}

//-----------------------------------------------------------------------------
// Transmit_COM0	:  Start transfer of TX buffer to PC.
//-----------------------------------------------------------------------------
void str2COM0(unsigned char *dat_ptr, unsigned char length)
{
	int len;
	if (length == 0x00)	{len = strlen(dat_ptr);} else {len = length;}

	while (len != 0)
	{
		COM0PutByte(*dat_ptr);
		dat_ptr++;
		len--;	
	}	
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
