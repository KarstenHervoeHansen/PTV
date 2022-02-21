//#define DEBUG_MCU

#include <C8051F120.h>
#include <string.h>
#include <stdio.h>
#include "uart_0.h"
#include "uart_1.h"
#include "main.h"
#include "ublox_prot.h"

//buffers og pointers til seriel indlæsning
#define RX1_SERBUFLEN  300//300 //8
unsigned char xdata rx1_serbuf[RX1_SERBUFLEN] _at_ 0x1d30;
unsigned char rx1_inptr=0, rx1_outptr=0;



//Tserbuffer RX1Buffer[ RX1BUFSIZE ];
//Tserbuffer * RX1RDPtr;
//Tserbuffer * RX1WTPtr;
//unsigned int RX1count;						// number of data to be recieved by host (Unsigned INT)
//bit RX1Ready;								// RX buffer has room

//unsigned char RX1Timeout;					// When timeout a whole u-Blox packet has been received.
unsigned char NTPResetDelay;				// When timeout a whole u-Blox packet has been received.

//	Write 1 byte to UART1.
void putch(unsigned char c)
{
	unsigned char SFRPAGE_Save;

	SFRPAGE_Save = SFRPAGE;    // Save Current SFR page

	SFRPAGE = UART1_PAGE;
	SBUF1 = c;
	while (TI1 == 0);
	TI1   = 0;
	SFRPAGE = SFRPAGE_Save;    // Restore SFR page
}

//	UART1 interrupt handler.
void UART1(void) interrupt 20 using 3
{
	unsigned char SFRPAGE_Save;

	SFRPAGE_Save = SFRPAGE;    // Save Current SFR page
	SFRPAGE = UART1_PAGE;		

	if (RI1)
		{
			RI1 = 0;
		    rx1_serbuf[rx1_inptr] = SBUF1;
		    rx1_inptr++;
		    if (rx1_inptr == RX1_SERBUFLEN)
	        rx1_inptr = 0;
		}

	SFRPAGE = SFRPAGE_Save;    // Restore SFR page
}
/*
void Flush_COMbuffers( void )
{
	unsigned char i;

	RX1WTPtr = RX1Buffer;
    for (i = 0; i < RX1BUFSIZE; i++)
	{
		*RX1WTPtr = 0xFF;	
		RX1WTPtr++;
	}

	RX1RDPtr = RX1Buffer;
	RX1WTPtr = RX1Buffer;
	RX1count = 0;
	RX1Ready = 1;

}
*/
/*
unsigned char COMGetByte( void )
{
	unsigned char dt;

	dt = *RX1RDPtr;
	if ( RX1RDPtr == (RX1Buffer + (RX1BUFSIZE - 1)) )	// at the end of the buffer
		RX1RDPtr = RX1Buffer;					// go to the head of the buffer
	else
		RX1RDPtr++;

	RX1count--;
	RX1Ready = (RX1count != 0);

	return dt;
}
*/
/*
unsigned char getch(unsigned char * c)
{
  unsigned char timer=0;

  while (RXcount == 0)
  {
  	timer++;
    if (timer==255)
      {
      *c  = 0;   // zero the holder to signal timeout
      return (0);
      }
    }

  *c = COMGetByte();
  return (1);
}
*/
unsigned char getch(unsigned char * c)
{
	unsigned char timer=0;

	if (ublox_timeout == 255) // Workaround 20120224 against lockup.
	{
		rx1_inptr = 0x00;
		rx1_outptr = 0x00;

		#ifdef DEBUG_MCU		
			sprintf(DebugString, "UBLOX Timeout.\n\r");
			str2COM0(&DebugString, 0x00);	
		#endif

		return 0;
	}

	while (rx1_inptr == rx1_outptr)
	{
		timer++;
		if (timer==255)
		{
			*c  = 0;   // zero the holder to signal timeout
			return (0);
		}
	}

	*c = rx1_serbuf[rx1_outptr];
	rx1_outptr++;
	if (rx1_outptr == RX1_SERBUFLEN)
	rx1_outptr = 0;

	return (1);
}
   