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
#include <string.h>
#include "mn_defs.h"						// Library Type definitions
#include "telnet.h"

//-----------------------------------------------------------------------------
// Constant
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
Ttx0buffer TX0Buffer[ TX0BUFSIZE ];		// Ring buffer for TX. MCU --> PC
Trx0buffer RX0Buffer[ RX0BUFSIZE ];		// Ring buffer for RX. PC  --> MCU

										// Pointers for ring buffer.
Ttx0buffer * TX0RDPtr;					// GET data from TX buffer.
Ttx0buffer * TX0WTPtr;					// PUT data into TX buffer.
Trx0buffer * RX0RDPtr;					// GET data from RX buffer.
Trx0buffer * RX0WTPtr;					// PUT data into RX buffer.

unsigned int TX0count;					// Number of bytes to transmit to PC.
unsigned int RX0count;					// Number of data recieved from PC.
unsigned int TX0Ready;							// TX buffer has data to transmit.
unsigned int RX0Ready;							// RX buffer has room to receive.

//-----------------------------------------------------------------------------
// UART Subroutines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Flush_COM0buffers	: Clear UART0 buffers.
//-----------------------------------------------------------------------------
void Flush_COM0buffers(void)
{
	unsigned int i;

	TX0RDPtr = TX0Buffer;
	TX0WTPtr = TX0Buffer;
	RX0RDPtr = RX0Buffer;
	RX0WTPtr = RX0Buffer;
	TX0count = 0;
	RX0count = 0;
	TX0Ready = 0;
	RX0Ready = 1;

	for(i=0; i<RX0BUFSIZE; i++)	RX0Buffer[i] = 0x00;
	for(i=0; i<TX0BUFSIZE; i++)	TX0Buffer[i] = 0x00;
}

//-----------------------------------------------------------------------------
// COM0GetByte		:  Get data from RX buffer. (Get data from PC.)
//-----------------------------------------------------------------------------
unsigned char COM0GetByte(void)
{
	unsigned char dt;

	dt = *RX0RDPtr;
	if ( RX0RDPtr == (RX0Buffer + (RX0BUFSIZE - 1)) )	// at the end of the buffer
		RX0RDPtr = RX0Buffer;							// go to the head of the buffer
	else
		RX0RDPtr++;

	if (RX0count > 0) RX0count--;
//	RX0Ready = (RX0count != 0);

	return dt;
}

//-----------------------------------------------------------------------------
// COM0PutByte		:  Put data into TX buffer. (Send data to PC.)
//-----------------------------------------------------------------------------
void COM0PutByte(unsigned char dt)
{
	*TX0WTPtr = dt;
	if ( TX0WTPtr == (TX0Buffer + (TX0BUFSIZE - 1)) )	// at the end of the buffer
		TX0WTPtr = TX0Buffer;					// go to the head of the buffer
	else
		TX0WTPtr++;

	TX0count++;
	TX0Ready = (TX0count != TX0BUFSIZE);
}


//-----------------------------------------------------------------------------
// COM0GetByte_ISR	:  PUT data into RX buffer. (Get data from PC. Called from ISR)
//-----------------------------------------------------------------------------
void COM0GetByte_ISR(unsigned char dt)
{
	if (RX0WTPtr > (RX0BUFSIZE + RX0Buffer - 1))
	{
		RX0WTPtr = RX0Buffer;
	}

	*RX0WTPtr = dt;
	RX0WTPtr++;
	RX0count++;
	RX0Ready = 5000; //500;//5000;//(RX0count != 0);
	if (dt == 0x0A) 
	{
		//telnet_send_length = RX0count;
		//telnet_SendTXBUFF(telnet_send_length);
		RX0Ready = 0;
//		telnet_server_idle();
	} 
}

//-----------------------------------------------------------------------------
// COM0PutByte_ISR	:  Get data from TX buffer. (Send data to PC. Called from ISR)
//-----------------------------------------------------------------------------
unsigned char COM0PutByte_ISR(void)
{
	unsigned char dt;

	dt = *TX0RDPtr;
	if ( TX0RDPtr == (TX0Buffer + (TX0BUFSIZE - 1)) )	// at the end of the buffer
		TX0RDPtr = TX0Buffer;					// go to the head of the buffer
	else
		TX0RDPtr++;

	TX0count--;
	TX0Ready = (TX0count != 0);

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
	Transmit_COM0();
	while(TX0count != 0); 
}

//-----------------------------------------------------------------------------
// UART0_Interrupt
//-----------------------------------------------------------------------------
void UART0_Interrupt (void) interrupt 4
{
	char SFRPAGE_SAVE = SFRPAGE;     		// Save Current SFR page
	SFRPAGE = UART0_PAGE;

	if (RI0 == 1)
	{
		RI0 = 0;							// Clear interrupt flag
		COM0GetByte_ISR(SBUF0);
	}

	if (TI0 == 1)							// Should something be transmitted...
	{
		TI0 = 0;							// Clear interrupt flag
		if(TX0count > 0) 
			SBUF0 = COM0PutByte_ISR();
	}

	SFRPAGE = SFRPAGE_SAVE;       			// Restore SFR page
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
