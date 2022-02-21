#pragma code symbols debug objectextend
 /*
 ; DK-Technologies A/S
 ;
 ; FILE NAME        serial.c
 ; PURPOSE          Module containing serial port 0 interrupt routine and
 ;                  character output routine for 8051F320 Cygnal
 ;                  processor.
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:       28.06.2006.
 ;
 ; NOTES:           This module contains serial comms routines
 ;                  for 8051F320 Cygnal processor.
 ;
 ;                  This module contains alternative serial routines used 
 ;                  only during downloading file for FPGA programming via
 ;                  JTAG.
 ;                  Timer2 is used as receive timeout provider.
*/



#include <C8051F320.h>
#include "define.h"
#include "serial.h"
#include "io.c"

#define RX_SERBUFLEN  50 //64

#define DEFDEL        50
#define T2START  Timer2_Init(0x7fff)

UC idata rx_serbuf[60];
UC rx_inptr;
UC rx_outptr;

UC bdata serflags;
sbit  T2timeout = serflags^1;

sfr16 TMR2RL    = 0xca;                    // Timer2 reload value
sfr16 TMR2      = 0xcc;                    // Timer2 counter

UC    t2del;
UC    csum = 0;

void serinit(void)
  {

  rx_inptr  = 0;
  rx_outptr = 0;
  }

/**********************************************************************
 * NAME       void flush(void)
 * PURPOSE    
 *            To initialize buffers for reception
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
/*  Notes:    
 **********************************************************************/
void flush(void)
  {
  UC i;
  ES0       = 0;
  rx_inptr  = 0;
  rx_outptr = 0;

  for (i = 0; i < 60; i++)
    {
    rx_serbuf[i] = 0;
    }
  ES0       = 1;
  }

/**********************************************************************
 * NAME       char putch (char c)
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
void putch(UC c)
  {
  SBUF0 = c;
  while (!TI0)
    ;
  TI0   = 0;
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
 *  interrupt 4 using 1
 **********************************************************************/
void serint0(void)
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

#if 0
bit serhit(void)
  {
  if (rx_inptr != rx_outptr)
    return 1;
  else
    return 0;
  }
#endif
/**********************************************************************
 * NAME       UC getch(UC * c)
 * PURPOSE    To fetch a character from serial buffer.
 *
 * ARGUMENTS  None
 *
 * RETURNS    0 - if there is no character to fetch after timeout
 *            1 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
UC getch(UC * c)
  {

  ES0 = 0;
  if (rx_outptr == 0)
    {
#if 1
    LED = ~LED;
#endif
    putch(START);
    }
  ES0 = 1;

  T2START;

  while (rx_inptr == rx_outptr)
    {
    if (T2timeout)
      {
      TR2 = 0;   // stop Timer2
      return (0);
      }
    }

  TR2 = 0;       // stop Timer2
  ES0 = 0;
  *c = rx_serbuf[rx_outptr];
  rx_outptr++;
  if (rx_outptr == RX_SERBUFLEN)
    rx_outptr = 0;
  ES0 = 1;
  return (1);
  }


/**********************************************************************
 * NAME       void Timer2_Init (int counts)
 * PURPOSE    To initialize Timer2 for delay function.
 *
 * ARGUMENTS  int counts - 
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
void Timer2_Init (int counts)
  {
  TMR2CN    = 0x00;        // Stop Timer2; Clear TF2;
                           // use SYSCLK/12 as timebase
  TMR2RL    = -counts;     // Init reload values
  TMR2      = 0xffff;      // set to reload immediately
  ET2       = 1;           // enable Timer2 interrupts
  TR2       = 1;           // start Timer2
  t2del     = DEFDEL;
  T2timeout = 0;
  }

void delay(void)
  {
  T2START;

  while (1)
    {
    if (T2timeout)
      {
      TR2 = 0;   // stop Timer2
      return;
      }
    }
  }
//-----------------------------------------------------------------------------
// Timer2_ISR
//-----------------------------------------------------------------------------
void Timer2_ISR (void) interrupt 5
  {
  TF2H = 0;               // clear Timer2 interrupt flag

  t2del--;
  if (!t2del)
    {
    T2timeout = 1;
    t2del = DEFDEL;
    }
  }
/*
UC crc8(UC* buff, UC buffLen)
  {
  UC i;
  UC chk;
  UC *ptr_stop;

  ptr_stop = buff + buffLen;
  chk = STARTCRCVALUE;

  while(buff < ptr_stop)
    {
    chk ^= *buff;
    for(i = 0 ; i < 8; i++)
      chk = (UC)((chk & 1) ? (chk >> 1) ^ CRC8CONST : (chk >> 1));
    buff++;
    }
  return chk;
  }
*/
/* end of serial.c */

