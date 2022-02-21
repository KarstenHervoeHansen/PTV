/***************************************************************************/
/*  Copyright DK-Audio A/S, Herlev 2002                                    */
/*  Project:  Removing RTOS from PT5201 Generator                          */
/*  Module:   ser1drv.C                                                    */
/*  Author:   JK                                                           */
/*  Date:     31.05.2002                                                   */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/*  Changes:                                                               */
/*  This module contains all necessary functions for implementing serial
    communication via uart1 of the XA processor.  The uart is initialized
    for one protocol only, 96,n,8,1 and works in Mode 1 with Timer 1 as
    BR generator.  Reception is interrupt controlled, via RI.1, vector at
    0x00A8 - 0x00AB.  Transmission is not interrupt controlled, i.e. the 
    sending function blocks until a character has left the transmiter.
    On reception, the characters are placed into receive buffer rx1_buffer
    using rx1_in_ptr as index.  This index is incremented and zeroed if it
    equals RX_BUFFER_SIZE.  On retrieving, the characters are retrieved 
    from buffer using rx1_out_ptr, which is then incremented and zeroed if
    it equals RX_BUFFER_SIZE.  Thus, when these two indexes are equal, there
    is no character to fetch.
    This module contains the following functions:
    
    void uart1_init(void) - this function initializes uart1 to 96, n, 8, 1
    with Timer 1 as baud rate generator, zeroes in and out indexes and
    enables uart1 receive interrupt.
    
    interrupt void uart1_rx_nterrupt(void) - this function retirieves 
    received character from S1BUF, places it into buffer using rx1_in_ptr
    as index and increments the index.
    
    char uart1_getchar(char *c) - this function returns 0 if there is no
    character in the buffer and 1 if there is.  The character is returned 
    in c.  It uses rx1_out_ptr as index to buffer and if there is a character,
    it increments the index.
    
    void uart1_putchar(char ch) - this function loads ch into transmitter and
    waits until the character has been sent.
    
    07.05.03: Transparent mode implemented in uart1_rx_nterrupt().
              Received char is loaded into S0BUF
             
              STINT1 flag is cleared: communication errors will NOT cause 
              serial 1 receive interrupt (page 393) 

/***************************************************************************/

#include  <stdio.h>
#include  <stdlib.h>
#include  "define.h"
#include  <string.h>
#include  "rs232.h"
#include "priori.h"
#include "unitprg.h"

#define RX_BUFFER_SIZE    (UC) 255
#define TIMER1_RANGE      (unsigned long)65536     /* UART mode 2 using Timer 1 */
#define PRESCALER         (unsigned long)4         /* selected also in SCR */
#define CRYSTAL           29.4912E6

/* =================== */
/* S1CON - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define U1_SM0   (0 << 7)    /* allow for 8-bit UART */
#define U1_SM1   (1 << 6)    /* allow for 8-bit UART */
#define U1_SM2   (0 << 5)    /* allow for 8-bit UART */
#define U1_REN_0 (0 << 4)    /* enable serial reception */
     
#define U1_TB8_0 (0 << 3)    /* clear to disallow 9 bit data */
#define U1_RB8_0 (0 << 2)    /* clear to disallow 9 bit data */
     
#define U1_TI_0  (0 << 1)   /* the transmit interrupt flag */
#define U1_RI_0  (0 << 0)   /* the receive interrupt flag */ 

#define xS1CON (U1_SM0 | U1_SM1 | U1_SM2 | U1_REN_0 | U1_TB8_0 | U1_RB8_0 | U1_TI_0 | U1_RI_0 )
/* =================== */

/* =================== */
/* TMOD - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define T0_GATE (0 << 3)   /* not applicable - OR in a zero (0) */
#define T0_CT   (0 << 2)   /* not applicable - OR in a zero (0) */
#define T0_M1   (0 << 1)   /* not applicable - OR in a zero (0) */
#define T0_M0   (1 << 0)   /* not applicable - OR in a zero (0) */

#define T0_TMOD  (T0_GATE | T0_CT | T0_M1 | T0_M0)

/* ------------------- */
/* TIMER 1             */
/* ------------------- */
#define T1_GATE (0 << 7)   /* disable gate control */
#define T1_CT   (0 << 6)   /* cleared for timer operation */
#define T1_M1   (0 << 5)   /* 16-bit auto-reload */
#define T1_M0   (0 << 4)   /* 16-bit auto-reload */

#define T1_TMOD  (T1_GATE | T1_CT | T1_M1 | T1_M0)

#define xTMOD  (T0_TMOD | T1_TMOD)
/* =================== */

/* =================== */
/* TCON - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define T0_TF0  (0 << 5)   /* not applicable - OR in a zero (0) */
#define T0_TR0  (1 << 4)   /* not applicable - OR in a zero (0) */
#define T0_IE0  (0 << 1)   /* not applicable - OR in a zero (0) */
#define T0_IT0  (0 << 0)   /* not applicable - OR in a zero (0) */

#define T0_CON (T0_TF0 | T0_TR0 | T0_IE0 | T0_IT0)

/* ------------------- */
/* TIMER 1             */
/* ------------------- */
#define T1_TF1  (0 << 7)   /* overflow flag - manipulated by hardware */
#define T1_TR1  (1 << 6)   /* run control bit - enabled */
#define T1_IE1  (0 << 3)   /* interrupt edge flag - enabled */
#define T1_IT1  (0 << 2)   /* interrupt type control bit - disabled */

#define T1_CON (T1_TF1 | T1_TR1 | T1_IE1 | T1_IT1)

#define xTCON (T0_CON | T1_CON)
/* =================== */

_near UC rx1_in_ptr;              // Index to next available slot in buffer
_near UC rx1_out_ptr;             // Index to next slot emptied from buffer

char rx1_buffer[RX_BUFFER_SIZE];  // Driver Receive Buffer
static union
  {
  struct
    {
    unsigned RI:1;
    unsigned TI:1;
    unsigned RB8:1;
    unsigned TB8:1;
    signed   Enable:1;
    unsigned Multi:1;
    unsigned Mode:2;
    }Bits;
  unsigned char Byte;
  }_S1CON;


static union
  {
  struct
    {
    unsigned EnableInt:1;
    unsigned OverFlowErr:1;
    unsigned BreakInt:1;
    unsigned FramingErr:1;
    unsigned :4;
    }Bits;
  char Byte;
  }_S1STAT;


/***************************************************************************/
/*  void uart1_init(void)                                        ser1drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   03.04.2002                                                   */
/*                                                                         */
/*  Function: Initialize Uart1 to 96,n,8,1                                 */
/*  Remarks:    -                                                          */
/*  Returns:    Nothing                                                    */
/*  Updates:    -                                                          */
/***************************************************************************/
void uart1_init(void)
  {
  union
    {
    unsigned int  Int;
    unsigned char Byte[2];
    } BaudRate;
    
  /* IEL - Interrupt Enable Register */
  ET1 = 0;    /* Timer 1 enable bit  - disnabled */
  // ET0 = 1;    // jk
  /* S1CON - serial port 1 control register */
  S1CON = _S1CON.Byte = xS1CON;

  /* S1STAT - serial port 1 control/status register */
  /* STINT1 flag is clear: errors will NOT cause serial 1 receive interrupt (page 393) */
  _S1STAT.Byte = 0x00;
  S1STAT       = _S1STAT.Byte;
  
  /* Timer/Counter Mode Control Register */
  TMOD &= 0x00;   /* clear timer1 settings */
  TMOD |= xTMOD;
  
  /* Timer/Counter Control Register */
  TCON &= 0x2F;   /* clear timer1 settings */
  TCON |= xTCON;

  BaudRate.Int=(unsigned short)(TIMER1_RANGE-(CRYSTAL/(9600*PRESCALER*(unsigned long)16)));
  
  ETI1 = 0;   // transmit port 1 - disabled
  ERI1 = 0;   // receive  port 1 - disabled
  REN_1= 0;
  TR1  = 0;

  IPA5 = RX1INTLVL;      // Priority PTI1/PRI1, (serial 1) 14 for RX

  TL1 = RTL1 = BaudRate.Byte[0];  // Timer/Counter 1 used as BR generator for uart1
  TH1 = RTH1 = BaudRate.Byte[1];

  _S1CON.Bits.Mode = 1;       //  8 bit serial mode
  S1CON = _S1CON.Byte;

  TR1  = 1;   // run timer1
  REN_1= 1;   // enable reception on uart1
  ERI1 = 1;   // receive  port 1 interrupt - enabled
  ETI1 = 0;   // transmit port 1 interrupt - disabled
/***/  

  rx1_in_ptr  = 0;
  rx1_out_ptr = 0;
  }

/***************************************************************************/
/*  void uart1_rx_nterrupt(void)                                 ser1drv.C */
/*                                                                         */
/* Author:     JK                                                          */
/* Revised:    03.04.2002                                                  */
/*                                                                         */
/*  Function:  Interrupt routine for receiving on serial channel 1         */
/*  Returns:    Nothing                                                    */
/*  Note:      The argument to _using() is PSW (PSWH and PSWL)             */
/*             which this routine will be using. The PSWH is as follows:   */
/*             SM = 1                                                      */
/*             TM = 0                                                      */
/*             RS0 - RS1 = 10, i.e reg bank 2 is used                      */
/*             IM0 - IM3 = 14, i.e this interrupt priority is 14           */
/*             The argument to _interrupt() is vector number.              */
/*             It is obtained as follows:                                  */
/*             vector_number = vector address / 4                          */
/*             In this case: 0xA8 / 4 = 42                                 */
/* Updates:    -                                                           */
/* _interrupt(42) _using(0x8E00) void uart1_rx_nterrupt(void)              */
/*                                                                         */
/*             If in transparent mode (UnitPgmMode = 1), all received      */
/*             characters are loaded into S0BUF                            */
/***************************************************************************/
/*
#define ERR_NONE     0x00
#define ERR_PARITY   0x01
#define ERR_OVERFLOW 0x02
#define ERR_BREAK    0x04
#define ERR_FRAMING  0x08
*/
#define ARG (SYSMODE | (RX1REGBANK << 4) | RX1INTLVL) << 8

#if 1
_interrupt(42) _using(0xAE00) void uart1_rx_nterrupt(void)
  {
  RI_1 = 0;                         // Clear Interrupt flag RI1

  if (!UnitPgmMode)
    {
    rx1_buffer[rx1_in_ptr] = S1BUF;    // place char in buffer
    rx1_in_ptr++;
    if (rx1_in_ptr == RX_BUFFER_SIZE)
      rx1_in_ptr = 0;
    }
  else
    S0BUF = S1BUF;      // send it to UART0 if in transparent mode
  }
#else
_interrupt(42) _using(0xAE00) void uart1_rx_nterrupt(void)
  {
  RI_1 = 0;                         // Clear Interrupt flag RI1

  if ((S1STAT & 0x0E) < ERR_OVERFLOW)
    {
    rx1_buffer[rx1_in_ptr] = S1BUF;    // place char in buffer
    rx1_in_ptr++;
    if (rx1_in_ptr == RX_BUFFER_SIZE)
      rx1_in_ptr = 0;
    }
  S1STAT = _S1STAT.Byte;         // clear status byte if error, otherwise
  }                              // we will get continous serial interrupt!
#endif
/***************************************************************************/
/*  char uart1_getchar( char *c)                                 ser1drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   03.04.2002                                                   */
/*                                                                         */
/*  Function:  Read one character from the receive data buffer             */
/*  Returns:   0 if buffer empty, 1 otherwise                              */
/* Updates:    -                                                           */
/***************************************************************************/
char uart1_getchar(char *c) 
  {
  if (rx1_in_ptr != rx1_out_ptr)
    {                                // If rx buffer is not empty 
    *c = rx1_buffer[rx1_out_ptr];
    rx1_out_ptr++;
    if (rx1_out_ptr == RX_BUFFER_SIZE)
      rx1_out_ptr = 0;
    return(1);
    }
  return(0);    // if no chars in buffer
  }

/***************************************************************************/
/*  void uart1_putchar(char ch)                                  ser1drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   03.04.2002                                                   */
/*                                                                         */
/*  Function:  To transmit one character                                   */
/*  Returns:   Nothing                                                     */
/* Updates:    -                                                           */
/***************************************************************************/
void uart1_putchar(unsigned char ch)
  {
  S1BUF = ch;
  while(!TI_1)
    ;
  TI_1 = 0;  
  }


void uart1_puts(char * source)
  {
  while (*source)
    uart1_putchar(*source++);
  }
  
/***********************************************/
/* Functions to interface to existing software */
/***********************************************/

int rs232putchar(unsigned char ch)
  {
  uart1_putchar(ch);
  return 0;
  }

int rs232puts( char* str)
  {
  uart1_puts(str);
  return 0;
  }

int rs232printf(int cnt, const char* format, ...)
  {
  char buf[40];
  va_list ptr;
  int ret;

  va_start(ptr, format);
  ret = vsprintf(buf, format, ptr);
  va_end(ptr);

  uart1_puts(buf);
  return(ret);
  }      

int rs232getchar(void)
  {
  char c;
  if (uart1_getchar(&c))
    return (c);
  else
    return (-1);
  }

int rs232GetLastError( void)
  {
  return 0;
  }