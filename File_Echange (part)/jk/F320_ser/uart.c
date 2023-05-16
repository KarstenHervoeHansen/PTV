 /*
 ; DK-Technologies A/S
 ;
 ; FILE NAME        uart.c
 ; PURPOSE          Module containing serial port 0 interrupt routine and
 ;                  character in/out routines for C8051F320 Cygnal
 ;                  processor.
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:       17.12.2004
 ;                  
 ;
 ; NOTES:           This module contains serial transmission routines
 ;                  for C8051F320 Cygnal processor.  Both reception and
 ;                  transmission are interrupt controlled.
 ;
 ;     ******* This version has transmission interrupt driven. *******
*/
#include <c8051f320.h>
#include <stdio.h>
#include "defins.h"
#include "uart.h"

#define TX_SERBUFLEN  32 /* BUFFER for sending to sensor     */
#define RX_SERBUFLEN  32 /* buffer for receiving from sensor */

uchar idata rx_serbuf[RX_SERBUFLEN];
uchar rx_inptr;
uchar rx_outptr;

uchar idata tx_serbuf[TX_SERBUFLEN];
uchar tx_inptr;
uchar tx_outptr;

uchar timoutlo;
uchar timouthi;

uchar bdata    serflags;
sbit  tempty = serflags^0; /* transmiter empty flag */


/**********************************************************************
 * NAME       void serinit(void)
 * PURPOSE    To initialize UART.  Timer 1 is used as BR generator.
 *            To initialize buffers for reception and transmission.
 *
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     Baud rate is set to 4800. No parity, 2 stop bit.
 *            In order to get 2 stop bits, we program UART as
 *            9 bit UART with TB80 set to 1.  The ninth bit will
 *            appear as extra stop bit.
 **********************************************************************/
void serinit(void)
  {

  TMOD  = 0x20;   // set timer1 mode: 8bit timer with auto-reload
  CKCON = 0x10;   // clock = system clock/12

  TH1 = 0x96;     //reload value = 0x96
  TL1 = 0x96;
  
  TR1 = 1;       //start timer1
  RI0 = 0;       //clear receive interrupt bit
  TI0 = 0;       //clear transmit interrupt bit

#if 0
  SCON0 |= 0x50; //set 8-bit UART, enable receive
#else
  SCON0 = 0x98; //set 9-bit UART, with 9th bit = 1
#endif

  IE    |= 0x10; //enable UART interrupt

  rx_inptr   = 0;
  rx_outptr  = 0;
  tx_inptr   = 0;
  tx_outptr  = 0;
  tempty     = 1;
  }

/**********************************************************************
 * NAME       char serout(char c)
 * PURPOSE    To place character in transmiter buffer and, if transmiter
 *            is empty, commence transmission (TI0 = 1)
 *
 * ARGUMENTS  Character to output
 *
 * RETURNS    Character to output.
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
char serout(char c)
  {
  ES0 = 0; /* disable serial interrupt while manipulatig buffer */
  tx_serbuf[tx_inptr] = c;
  tx_inptr++;
  if (tx_inptr == TX_SERBUFLEN)
    tx_inptr = 0;

  if (tempty)
    {
    tempty = 0;
    TI0 = 1;     /* restart manually the interrupt routine if tempty */
    }
  ES0 = 1;
  return(c);
  }

/**********************************************************************
 * NAME       void serint0(void) interrupt 4 using 1
 * PURPOSE    This is serial reception/transmission interrut routine.
 *
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     When receiving character interrupt occurs, it is read 
 *            from UART and placed into rx_serbuf[].  Input pointer
 *            to this buffer (rx_serbuf) is incremented and on reaching
 *            RX_SERBUFLEN zeroed, i.e. no buffer overflow check is 
 *            implemented.
 * 
 *            When transmitting (when a character has left the UART),
 *            next character (if any) is fetched from tx_serbuf[] and
 *            moved to the UART.  Output pointer to this buffer is 
 *            incremented and if equal to TX_SERBUFLEN, zeroed.  No 
 *            check for buffer overflow is made.
 **********************************************************************/
void serint0(void) interrupt 4 using 1
  {
  char c;

  if (RI0)
    {
    rx_serbuf[rx_inptr] = SBUF0;
    RI0 = 0;
    rx_inptr++;
    if (rx_inptr == RX_SERBUFLEN)
      rx_inptr = 0;
    }
  else
    {
    if (TI0) /* means that a character has left the transmitter */
      {
      if (tx_outptr != tx_inptr) /* if another character waiting in buffer */
        {
        TI0   = 0;
        c    = tx_serbuf[tx_outptr];
        SBUF0 = c;
        tx_outptr++;
        if (tx_outptr == TX_SERBUFLEN)
          tx_outptr = 0;
        }
      else  /* tx_outptr = tx_inptr and TI00: last character left buffer */
        {
        TI0    = 0;
        tempty = 1;
        }
      }
    }
  }

/**********************************************************************
 * NAME       bit serhit(void)
 * PURPOSE    To check if a character is available from seial connection.
 *
 * ARGUMENTS  None
 *
 * RETURNS    0 - if there is no character.
 *            1 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
bit serhit(void)
  {
  if (rx_inptr != rx_outptr)
    return 1;
  else
    return 0;
  }
/**********************************************************************
 * NAME       void serflush(void)
 * PURPOSE    To empty receive buffer.
 *
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *            This funcion zeroes in and out pointers to receive
 *            buffer, thus emptying it.
 **********************************************************************/
void serflush(void)
  {
  rx_inptr  = 0;
  rx_outptr = 0;
  }

/**********************************************************************
 * NAME       char serinp(void)
 * PURPOSE    To fetch a character from serial buffer.
 *
 * ARGUMENTS  None
 *
 * RETURNS    0 - if there is no character to fetch after timeout
 *            Fetched character otherwise.
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
char serinp(void)
  {
  char c;
  timoutlo = 0xff;
  timouthi = 0x05;
  while(rx_inptr == rx_outptr)
    {
    if (--timoutlo)
      continue;
    timoutlo = 0xff;
    if (--timouthi)
      continue;
    return 0;
    }
  ES0 = 0; 
  c   = rx_serbuf[rx_outptr];
  rx_outptr++;
  if (rx_outptr == RX_SERBUFLEN)
    rx_outptr = 0;
  ES0 = 1; 
  return c;
  }

/* end of uart.c */

