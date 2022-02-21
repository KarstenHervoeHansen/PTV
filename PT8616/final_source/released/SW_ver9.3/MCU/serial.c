#include <C8051F320.h>
#include <string.h>
#include "serial.h"

//buffers og pointers til seriel indlæsning
#define RX_SERBUFLEN  8
unsigned char xdata rx_serbuf[RX_SERBUFLEN];
unsigned char rx_inptr=0, rx_outptr=0;


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
   
 