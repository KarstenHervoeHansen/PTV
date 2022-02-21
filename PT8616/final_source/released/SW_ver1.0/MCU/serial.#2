#include <C8051F320.h>
#include <string.h>
#include "serial.h"

//buffers og pointers til seriel indlæsning
#define RX_SERBUFLEN  32 //90
unsigned char xdata rx_serbuf[RX_SERBUFLEN];
unsigned char rx_inptr, rx_outptr;

//header for ub_protokol
static unsigned char ub_header[2]= {0xb5, 0x62};

void setup_UART()
{
	//UART setup
	SCON0 = 0x30;	// 8 bit med interurrupt
	PCON = 0x00;	// ikke i idle eller stop-mode

	RI0 = 0;       	//clear receive interrupt bit
  	TI0 = 0;       	//clear transmit interrupt bit

  	rx_inptr  = 0;
	rx_outptr = 0;
	ES0=1;			//Enable serial interrupt
}

void set_UART9600()
{
	rx_inptr  = 0;
	rx_outptr = 0;

	CKCON = 0x00;     // Timer1 og timer0 bruger SYSCLK/12
	TL1   = 0x00;     // Timer 1 Low Byte
    TH1   = 0xd0;     // BaudRate = 9600
    TMOD  = 0x21;     // Timer Mode Register
    TCON  = 0x45;     // Timer Control Register, timer1 enabled
	TR1 = 1;       	  // Start timer1
}

void set_UART38400()
{
	rx_inptr  = 0;
	rx_outptr = 0;

	CKCON = 0x08;     // Timer1 bruger SYSCLK, timer 0 bruger SYSCLK/12
	TL1   = 0x00;     // Timer 1 Low Byte
    TH1   = 0x70;     // BaudRate = 38400
    TMOD  = 0x21;     // Timer Mode Register
    TCON  = 0x45;     // Timer Control Register, timer1 enabled
	TR1 = 1;       	  // Start timer1
}

//skriver en byte til seriel-bussen
void putch(unsigned char c)
{
  SBUF0 = c;
  while (!TI0)
    ;
  TI0   = 0;
}

//interrupt rutine til indgående seriel bytes
void serint0(void) interrupt 4 using 1
{
  if (RI0)
    {
    rx_serbuf[rx_inptr] = SBUF0;
    RI0 = 0;
    rx_inptr++;
    if (rx_inptr == RX_SERBUFLEN)
      rx_inptr = 0;
    }
}

unsigned char getch(unsigned char * c)
{
  unsigned char timer=0;

  while (rx_inptr == rx_outptr)
  {
  	timer++;
    if (timer==255)
      {
      *c  = 0;   // zero the holder to signal timeout
      return (0);
      }
    }

  ES0 = 0;
  *c = rx_serbuf[rx_outptr];
  rx_outptr++;
  if (rx_outptr == RX_SERBUFLEN)
    rx_outptr = 0;
  ES0 = 1;
  return (1);
}
   
 