 /*
 ; DK-Technologies A/S
 ;
 ; FILE NAME        uart.c
 ; PURPOSE          Module containing serial port 1 interrupt routine and
 ;                  character in/out routines for c8051F120 Cygnal
 ;                  processor.
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:       06.12.2007
 ;                  
 ;
 ; NOTES:           This module contains serial transmission routines
 ;                  for c8051F120 Cygnal processor.  Both reception and
 ;                  transmission are interrupt controlled.
 ;
 ;                  Note use of SFRPAGE in places where it is necessary.
 ;
 ;     ******* This version has transmission interrupt driven or not *******
*/
#include <c8051F120.h> 
#include <stdio.h>
#include "defins.h"
#include "uart.h"

#if 0
#define  TX_INTERRUPT    /* Transmission is interrupt driven or not */
#endif

#define UC unsigned char

#define TX_SERBUFLEN  32 /* BUFFER for sending to sensor     */
#define RX_SERBUFLEN  32 /* buffer for receiving from sensor */


UC rx_serbuf[RX_SERBUFLEN];
UC rx_inptr;
UC rx_outptr;

#ifdef TX_INTERRUPT
UC idata tx_serbuf[TX_SERBUFLEN];
UC tx_inptr;
UC tx_outptr;

UC bdata    serflags;
sbit  tempty = serflags^0; /* transmiter empty flag */
#endif

uchar timoutlo;
uchar timouthi;

/**********************************************************************
 * NAME       void serinit(void)
 * PURPOSE    To initialize UART.
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
void serinit(void)
  {

  rx_inptr   = 0;
  rx_outptr  = 0;

#ifdef TX_INTERRUPT
  tx_inptr   = 0;
  tx_outptr  = 0;
  tempty     = 1;
#endif
  TI1 = 0;
  RI1 = 0;
  EIE2      |= 0x40; /* set ES1, page 160 */
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
  char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page
  
  SFRPAGE = UART1_PAGE;
#ifdef TX_INTERRUPT
  tx_serbuf[tx_inptr] = c;  /* but available from all SFR pages.     */
  tx_inptr++;
  if (tx_inptr == TX_SERBUFLEN)
    tx_inptr = 0;

  if (tempty)
    {
    tempty = 0;
    TI1 = 1;     /* restart manually the interrupt routine if tempty */
    }
  SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page
  return(c);
#else
  SBUF1 = c;
  while (!TI1)
    ;
  TI1   = 0;
  SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page
  return(c);
#endif
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
#ifdef TX_INTERRUPT
void serint1(void) interrupt 20
  {
  char c;

  if (RI1)
    {
    rx_serbuf[rx_inptr] = SBUF1;
    RI1 = 0;
    rx_inptr++;
    if (rx_inptr == RX_SERBUFLEN)
      rx_inptr = 0;
    }
  else
    {
    if (TI1) /* means that a character has left the transmitter */
      {
      if (tx_outptr != tx_inptr) /* if another character waiting in buffer */
        {
        TI1   = 0;
        c    = tx_serbuf[tx_outptr];
        SBUF1 = c;
        tx_outptr++;
        if (tx_outptr == TX_SERBUFLEN)
          tx_outptr = 0;
        }
      else  /* tx_outptr = tx_inptr and TI00: last character left buffer */
        {
        TI1    = 0;
        tempty = 1;
        }
      }
    }
  }
#else
void serint1(void) interrupt 20
  {
  if (RI1)
    {
    rx_serbuf[rx_inptr] = SBUF1;
    RI1 = 0;
    rx_inptr++;
    if (rx_inptr == RX_SERBUFLEN)
      rx_inptr = 0;
    }
  }
#endif


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
  c   = rx_serbuf[rx_outptr];
  rx_outptr++;
  if (rx_outptr == RX_SERBUFLEN)
    rx_outptr = 0;
  return c;
  }

/* end of uart.c */
#if 0
char putchar(char c)
  {
  char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page
  
  SFRPAGE = UART1_PAGE;

  tx_serbuf[tx_inptr] = c;  /* but available from all SFR pages.     */
  tx_inptr++;
  if (tx_inptr == TX_SERBUFLEN)
    tx_inptr = 0;

  if (tempty)
    {
    tempty = 0;
    TI1 = 1;     /* restart manually the interrupt routine if tempty */
    }
  SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page

  return(c);
  }
#endif