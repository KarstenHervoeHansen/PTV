/***************************************************************************/
/*  Copyright DK-Audio A/S, Herlev 2002                                    */
/*  Project:  Removing RTOS from  PT5201 Generator                         */
/*  Module:   ser0drv.C                                                    */
/*  Author:   JK                                                           */
/*  Date:     17.07.2002                                                   */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/*  Changes:                                                               */
/*           07.05.03: Transparent mode implemented in uart1_rx_nterrupt() */
/*                     Received char is loaded into S1BUF                  */


/***************************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  "define.h"
#include  <string.h>
#include  "sio0drv.h"
#include  "rs232.h"
#include  "disp_02.h"
#include  "priori.h"
#include "unitprg.h"

#define RX_BUFFER_SIZE    64
#define CRYSTAL           29.4912E6

#define RxTimeout     (unsigned char)30   // Timeout pr. character in milliseconds
#define RxRetransmit  (unsigned char)3    // Number of retransmission if failure

#define TIMER2_RANGE  (unsigned long)65536

#define PRESCALER     (unsigned long)4       // selected also in SCR. NOTE!!! This value 
                                             //  MUST be the same as deinfed in start.asm

#define BAUDRATE     (unsigned long)57600    // select 57600 baud
                                             //         9600
#define RELOAD2      (unsigned short)(TIMER2_RANGE-(CRYSTAL/(BAUDRATE*PRESCALER*(unsigned long)16)))

#define THI2_RESET  ( RELOAD2 >> 8) & 0xFF
#define TLOW2_RESET ( RELOAD2 & 0xFF)


/* ==================== */
/* S0CON - bit settings */
/* -------------------- */
/* UART0                */
/* -------------------- */
#define U0_SM0   (1 << 7)   /* allow for 8-bit UART */
#define U0_SM1   (1 << 6)   /* allow for 8-bit UART */
#define U0_SM2   (0 << 5)   /* set to enable multiprocessor communication */
#define U0_REN   (1 << 4)   /* enable serial reception */
     
#define U0_TB8   (0 << 3)   /* the 9th databit transmitted in mode 2 or 3 */
#define U0_RB8   (0 << 2)   /* the 9th databit received in mode 2 or 3 */

#define U0_TI    (0 << 1)   /* set the transmit interrupt flag */
#define U0_RI    (0 << 0)   /* set the receive interrupt flag */ 

#define xS0CON (U0_SM0 | U0_SM1 | U0_SM2 | U0_REN | U0_TB8 | U0_RB8 | U0_TI | U0_RI )
/* =================== */

/* =================== */
/* TMOD - bit settings */
/* ------------------- */
/* TIMER 2             */
/* ------------------- */
#define T2_RCLK1  (0 << 5)   /* set not to use timer 2 as clock source for UART1,(AND select autoreload) */
#define T2_TCLK1  (0 << 4)   /* set not to use timer 2 as clock source for UART1,(AND select autoreload) */
#define T2_T2OE   (0 << 1)   /* not applicable - OR in a zero (0) */
#define T2_DCEN   (1 << 0)   /* set to count up for Timer 2 in auto reload mode */

#define T2_TMOD   (T2_RCLK1 | T2_TCLK1 | T2_T2OE | T2_DCEN)

#define xTMOD     T2_TMOD
/* =================== */

/* =================== */
/* TCON - bit settings */
/* ------------------- */
/* TIMER 2             */
/* ------------------- */
#define T2_TF2    (0 << 7)   /* overflow flag - NOT cleared by hardware */
#define T2_EXF2   (0 << 6)   /* not applicable - OR in a zero (0) */
#define T2_RCLK0  (1 << 5)   /* set to use timer 2 as clock source for UART0,(AND select autoreload) */
#define T2_TCLK0  (1 << 4)   /* set to use timer 2 as clock source for UART0,(AND select autoreload) */
#define T2_EXEN2  (0 << 3)   /* not applicable - OR in a zero (0) */
#define T2_TR2    (0 << 2)   /* not applicable - OR in a zero (0) */
#define T2_CT2    (0 << 1)   /* not applicable - OR in a zero (0) */
#define T2_CPRL2  (0 << 0)   /* not applicable - OR in a zero (0) */

#define T2_CON    (T2_TF2 | T2_EXF2 | T2_RCLK0| T2_TCLK0 | T2_EXEN2 | T2_TR2 | T2_CT2 | T2_CPRL2)

#define xTCON     T2_CON
/* =================== */

_near UC rx0_in_ptr;              // Index to next available slot in buffer
_near UC rx0_out_ptr;             // Index to next slot emptied from buffer

static char rx0_buffer[RX_BUFFER_SIZE];  // Driver Receive Buffer

//static char jkbuffer[30];
//extern UC rec_timeout;

/***************************************************************************/
/*  void uart0_init(void)                                        ser0drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   17.07.2002                                                   */
/*                                                                         */
/*  Function: Initialize Uart0 to 57600,n,8,1                              */
/*  Remarks:    -                                                          */
/*  Returns:    Nothing                                                    */
/*  Updates:    -                                                          */
/***************************************************************************/
void uart0_init(void)
  {
  // S0CON - serial port 0 control register
  S0CON = xS0CON;

  // Timer/Counter 2 Mode Control Register
  T2MOD = xTMOD;

  // Timer/Counter 2 Control Register
  T2CON = xTCON;

  // Interrupt priority for receiving on channel 0
  IPA4 = RX0INTLVL; 

  T2CAPH = THI2_RESET;
  T2CAPL = TLOW2_RESET;
  
  //tmp = S0BUF;           // read input to clear

  TR2  = 1;              // Start Timer 2
  REN_0 = 1;             // Enable seriel reception

  TI_0 = 0;              // Clear possible transmit interrupt 
  RI_0 = 0;              // Clear possible receive interrupt 

  ERI0 = 1;              // Receive  port 0 - enabled
  ETI0 = 0;              // Transmit port 0 - disabled

  rx0_in_ptr  = 0;
  rx0_out_ptr = 0;
  }

/***************************************************************************/
/*  void uart0_rx_nterrupt(void)                                 ser0drv.C */
/*                                                                         */
/* Author:     JK                                                          */
/* Revised:    03.04.2002                                                  */
/*                                                                         */
/*  Function:  Interrupt routine for receiving on serial channel 0         */
/*  Returns:    Nothing                                                    */
/*  Note:      The argument to _using() is PSW (PSWH and PSWL)             */
/*             which this routine will be using. The PSWH is as follows:   */
/*             SM = 1                                                      */
/*             TM = 0                                                      */
/*             RS0 - RS1 = 11, i.e reg bank 3 is used                      */
/*             IM0 - IM3 = 13, i.e this interrupt priority is 13           */
/*             The argument to _interrupt() is vector number.              */
/*             It is obtained as follows:                                  */
/*             vector_number = vector address / 4                          */
/*             In this case: 0xA0 / 4 = 40                                 */
/*                                                                         */
/*             If in transparent mode (UnitPgmMode = 1), all received      */
/*             characters are loaded into S1BUF                            */
/***************************************************************************/
#define ARG (SYSMODE | (RX0REGBANK << 4) | RX0INTLVL) << 8

#if 1
_interrupt(40) _using(0xBD00) void uart0_rx_nterrupt(void)
  {
  RI_0 = 0;                         // Clear Interrupt flag RI0

  if (!UnitPgmMode)
    {
    rx0_buffer[rx0_in_ptr] = S0BUF;    // place char in buffer
    rx0_in_ptr++;
    if (rx0_in_ptr == RX_BUFFER_SIZE)
      rx0_in_ptr = 0;
    }
  else
    S1BUF = S0BUF;  // send it to UART1
  }
#else
_interrupt(40) _using(0xBD00) void uart0_rx_nterrupt(void)
  {
  RI_0 = 0;                         // Clear Interrupt flag RI0

  rx0_buffer[rx0_in_ptr] = S0BUF;    // place char in buffer
  rx0_in_ptr++;
  if (rx0_in_ptr == RX_BUFFER_SIZE)
    rx0_in_ptr = 0;
  }
#endif


/***************************************************************************/
/*  char uart0_getchar( char *c)                                 ser0drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   03.04.2002                                                   */
/*                                                                         */
/*  Function:  Read one character from the receive data buffer             */
/*  Returns:   0 if buffer empty, 1 otherwise                              */
/* Updates:    -                                                           */
/***************************************************************************/
char uart0_getchar(char *c) 
  {
  if (rx0_in_ptr != rx0_out_ptr)
    {                                // If rx buffer is not empty 
    *c = rx0_buffer[rx0_out_ptr];
    rx0_out_ptr++;
    if (rx0_out_ptr == RX_BUFFER_SIZE)
      rx0_out_ptr = 0;
    return(1);
    }
  return(0);    // if no chars in buffer
  }

/***************************************************************************/
/*  char uart0_getchar_timeout( char *c)                         ser0drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   03.04.2002                                                   */
/*                                                                         */
/* Function: To read one character with timeout                            */
/* Returns:  0 if no character arrived during timeout, 1 otherwise         */
/* Notes:    The timeout is implemented as follows:                        */
/***************************************************************************/
char uart0_getchar_timeout(char * c)
  {
  UI n;
  UI time;

  time = RxTimeout;
  while (time-- > 0) 
    {                       // Wait 1 ms x time
    n = 750;
    while ( n > 0)
      {
      n--;
      if (rx0_in_ptr == rx0_out_ptr)
        continue;
      else
        {
        uart0_getchar(c);
        return (1);
        }
      }
    }
  //uart1_puts("Timeout!\n");
  return(0);
  }
/***************************************************************************/
/*  void uart0_putchar(char ch)                                  ser0drv.C */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   03.04.2002                                                   */
/*                                                                         */
/* Function:  To transmit one character                                    */
/* Returns:   Nothing                                                      */
/* Updates:    -                                                           */
/***************************************************************************/
void uart0_putchar(int ch)
  {
  TB8_0 = (ch >> 8) & 0x01;
  S0BUF = ch & 0xFF;    // Write char to port
  while(!TI_0)
    ;
  TI_0 = 0;  
  }

void uart0_puts(char * source)
  {
  while (*source)
    uart0_putchar(*source++);
  }
  
/*********************************************************/
/* Functions for interfacing with existing software      */
/* remove file:sio0drv.c                                 */
/* Note: functions below originally return quite         */
/*       extensive array of error codes, which then      */
/*       are never checked for.  In this implementation, */
/*       they do not return any errors.                  */
/*********************************************************/
int sio0WaitAcknowledge(int addr, char *cmdbuf)
  {
  char *ptr;
  int tmp;
  UC checksum, retransmit = RxRetransmit;
  char c;

  do
    {
    tmp  = addr + 0x100;      // Transmit unit address
    //sio0oStream->Put( sio0oStream, &tmp);
    uart0_putchar(tmp);
    checksum = 0;
    ptr      = cmdbuf;
    waitms(1);
    while (tmp = *ptr++)
      {
      //sio0oStream->Put( sio0oStream, &tmp);
      uart0_putchar(tmp);
      checksum += tmp;
      waitms(1);
      }
    tmp = checksum & 0x7F;    // Transmit checksum
    //sio0oStream->Put( sio0oStream, &tmp);
    uart0_putchar(tmp);
     // Wait for unit to acknowledge
    waitms(1);
    if (uart0_getchar_timeout(&c))
      {
      return (OK);
      }
    } while (retransmit--);
#if 0   
  sprintf(jkbuffer, "Ack=%d\n", (int) 0);
  rs232puts(jkbuffer);
#endif

  return(OK);  //3401
  }                    

/**************************************************************************/
/* sio0WaitAnswer                                              SIO0DRV.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000119                                   */
/* Revised:    000409, KEn, DEV                                           */
/*                                                                        */
/* Function:  Wait for an answer                                          */
/* Remarks:                                                               */
/* Returns:                                                               */
/* Updates:    --                                                         */
/**************************************************************************/
int sio0WaitAnswer( void *buf)
  {
  char timeout;
  char tmp, next = TRUE;
  UC   checksum  = 0;
  char *ptr      = buf;

  do
    {
    if (timeout = uart0_getchar_timeout(&tmp))
      {
      checksum += tmp;
      *ptr++ = tmp;
      switch (tmp)
        {
        case 0x0A:
        case 0x0D:
        case ';':
        case '!':
        case '?':
        case '$':
          next = FALSE;
          break;
        default:
          break;
        }
      } 
    } while (next && (timeout != 0));
  if (timeout != 0) 
    {
    *(--ptr) = 0;                // Remove delimiter
    uart0_getchar_timeout(&tmp); // Wait for checksum
    }
  return(OK);
  }                    

/**************************************************************************/
/* sio0SndCommand                                              SIO0DRV.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000119                                   */
/* Revised:    000404, KEn, DEV                                           */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                               */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int sio0SndCommand(int cnt, int addr, const char *format, ...)
  {
  va_list va_ptr;
  int ret;
  char buf[40];

  va_start(va_ptr, format);
  ret = vsprintf(buf, format, va_ptr);
  va_end(va_ptr);
  ret = sio0WaitAcknowledge(addr, &buf);
  return(ret);
  }

/**************************************************************************/
/* sio0SndRequest                                                 SIO0DRV.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000119                                    */
/* Revised:    000404, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/***************************************************************************/
int sio0SndRequest(int cnt, int addr, char *result, const char *format, ...)
  {
  va_list va_ptr;
  int  ret;
  char buf[40];

  va_start(va_ptr, format);
  ret = vsprintf(buf, format, va_ptr);
  va_end(va_ptr);

  ret = sio0WaitAcknowledge(addr, buf);

  if ((ret = sio0WaitAnswer(buf)) == OK)
    strcpy(result, buf);
  else
    {
#if 0   
  sprintf(jkbuffer, "Answ=%d\n", ret);
  rs232puts(jkbuffer);
#endif
    }
  return(ret);
  }

