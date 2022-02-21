#include <C8051F320.h>
#include <string.h>
#include "inouts.h"
#include "NavSync_prot.h"
#include "UART.h"

#define RX_SERBUFLEN  115

unsigned char idata rx_serbuf[RX_SERBUFLEN];
unsigned char rx_inptr;
char update_flag;


void setup_UART()
{
	//UART setup
	SCON0 = 0x30;		// 8 bit med interurrupt
	PCON = 0x00;		// ikke i idle eller stop-mode

	RI0 = 0;       	//clear receive interrupt bit
  	TI0 = 0;       	//clear transmit interrupt bit

  	rx_inptr  = 0;
	ES0=1;			//Enable serial interrupt
}

void putch(unsigned char c)
{
  SBUF0 = c;
  while (!TI0)
    ;
  TI0   = 0;
}

void serint0(void) interrupt 4 using 1
{
  if (RI0)
  {
  	//'$' = start på en NMEA besked
  	if(SBUF0=='$')
		rx_inptr=0;

	//'*' = sidste karakter før checksum
	else if(SBUF0=='*')
		update_flag=1;

	else
	{
	    rx_serbuf[rx_inptr] = SBUF0;
    	rx_inptr++;

		if(rx_inptr>RX_SERBUFLEN)
			rx_inptr=0;
	}

  	RI0 = 0;
  }
}

void read_buf(char *out_buffer)
{
	memcpy(out_buffer, rx_serbuf, RX_SERBUFLEN);
}