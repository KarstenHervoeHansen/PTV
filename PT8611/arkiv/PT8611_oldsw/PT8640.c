/***************************************************************************/
/*  Copyright DK-Audio A/S, 2003                                           */
/*  Project:  PT8640 TriLevel HDTV generator, using 8051F231 Cygnal        */
/*            processor.                                                   */
/*  Module:   PT8640.C                                                     */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     15.10.2003                                                   */
/*  Status:   Version 1.0                                                  */
/*  Purpose:  Main program for PT8640 unit.                                */
/*  Note:     F231 has 8K Flash and 255 bytes of RAM and no  XRAM          */
/*                                                                         */
/*            Linker args: RS(256) NOOL                                    */
/***************************************************************************/


#include "f200.h"
#include "define.h"
#include <string.h>
#include "rs232.h"
#include "spi_mod1.h"
#include "eeprom.h"

/**************************************************************************/
/* Baud Rate calculation: Figure 17.13.                                   */
/* UART operates in Mode 3, with Timer 2 used for Baud Rate generation    */
/*                                                                        */
/*   Baud Rate = Timer 2 overflow rate / 16                               */
/*                                                                        */
/*  Timer 2 overflow rate = tclk / (TIMER2_RANGE - RELOAD2)               */
/*  tclk = CRYSTAL / 2                                                    */
/*                                                                        */
/*  Baud Rate = CRYSTAL / 32 * (TIMER2_RANGE - RELOAD2)                   */
/**************************************************************************/

#define CRYSTAL                  16.666E6        /* 29.4912E6 */

#define TIMER2_RANGE (unsigned long)65536

#define BAUDRATE     (unsigned long) (76800)    /* select 57600 baud */
                                            
#define RELOAD2 (unsigned short)(TIMER2_RANGE - (CRYSTAL/(BAUDRATE*(unsigned long)32)))

#define THI2_RESET  (RELOAD2 >> 8) & 0xFF
#define TLO2_RESET  (RELOAD2 & 0xFF)


/* ==================== */
/* SCON - bit settings  */
/* -------------------- */
/* UART                 */
/* -------------------- */
#define U_SM0   (1 << 7)   /* allow for 8-bit UART  Mode 3: 9-bit UART, Variable BR */
#define U_SM1   (1 << 6)   /* allow for 8-bit UART  Mode 3: 9-bit UART, Variable BR */
#define U_SM2   (1 << 5)   /* clear to disable multiprocessor communication */
#define U_REN   (1 << 4)   /* enable serial reception */
     
#define U_TB8   (0 << 3)   /* the 9th databit transmitted in mode 2 or 3 */
#define U_RB8   (0 << 2)   /* the 9th databit received in mode 2 or 3 */

#define U_TI    (0 << 1)   /* set the transmit interrupt flag */
#define U_RI    (0 << 0)   /* set the receive interrupt flag */ 

#define xSCON (U_SM0 | U_SM1 | U_SM2 | U_REN | U_TB8 | U_RB8 | U_TI | U_RI )

/* =================== */
/* TCON - bit settings */
/* ------------------- */
/* TIMER 2             */
/* ------------------- */
#define T2_TF2    (0 << 7)   /* overflow flag - NOT cleared by hardware */
#define T2_EXF2   (0 << 6)   /* not applicable - OR in a zero (0) */
#define T2_RCLK   (1 << 5)   /* set to use timer 2 as clock source for UART,(AND select autoreload) */
#define T2_TCLK   (1 << 4)   /* set to use timer 2 as clock source for UART,(AND select autoreload) */
#define T2_EXEN2  (0 << 3)   /* not applicable - OR in a zero (0) */
#define T2_TR2    (0 << 2)   /* not applicable - OR in a zero (0) */
#define T2_CT2    (0 << 1)   /* not applicable - OR in a zero (0) */
#define T2_CPRL2  (0 << 0)   /* not applicable - OR in a zero (0) */

#define T2_CON    (T2_TF2 | T2_EXF2 | T2_RCLK| T2_TCLK | T2_EXEN2 | T2_TR2 | T2_CT2 | T2_CPRL2)

#define xTCON     T2_CON
/* =================== 

/*
//-------------------
// Global CONSTANTS
//-------------------
*/
sbit  LED = P2^4;          /* green LED */

sbit  TP = P3^2;          /* P3^2: test point */

sbit OE = P1^0;

/* CURR_PARAMS gen_params; */

CURR_PARAMS tlgs[4];
bit         changed;
UC          current_generator;
ul          tlg_samples;

/*************************/
/*  Function PROTOTYPES  */
/*************************/
void init_all(void);


void main (void)
  {
  UC error;

  changed  = 0;
  init_all();

  error = 0;

#if 0
c = 0xbb;
d = 0xaa;
PageWriteE2prom(CONROL_BYTE, 10, &c, 1);

SeqReadE2prom(CONROL_BYTE, 10, &d, 1);
#endif

  while (1)
    {
    if (TestInputBuffer())
      CharHandler();
    if (changed)
      {
      write_plds(tlgs[current_generator].format);
      changed = 0;
      }
    }
  }

/***************************************************************************/
/*  void init_all(void)                                                    */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   17.07.2002                                                   */
/*                                                                         */
/*  Function: Initialize 8051-226 Uart to 57600,n,9,1 and set Timer 2 to   */
/*            operate as BR generator                                      */
/*            Initialize processor to external clock                       */
/*                                                                         */
/*  Returns:  Nothing                                                      */
/*  Notes:    Baud rate generator mode is selected by setting RCLK and     */
/*            TCLK to logic one.  With this done, Timer 2 operates in      */
/*            auto reload mode regardless of the state of the CP/RL2 bit   */
/***************************************************************************/
void init_all(void)
  {
  UC c;
  UI n;

  /* disable watchdog timer */
  WDTCN = 0xde;
  WDTCN = 0xad;
  
  OSCXCN  = 0x67; /* XFCN = 111, f > 6.74 MHz, page 90  */

  n = 1000;
  while ( n > 0)
    n--;

  PRT2CF |= 0x10;  /* enable P2.4 (LED) as push-pull output */
  PRT2CF |= 0x3F; /* page 98, SPI, NSS_1, NSS_2, NSS_M as push-pulls */

  PRT2MX |= 0x00; /* page 95, SPI NOT routed to pins, we have software SPI */

  PRT1CF |= 0xFF; /* page 97, P1 as push-pull */

  while(1)        /* wait until ext. osc. is stable, page 91 */
   {
   c = OSCXCN;
   c &= 0x80;
   if (c)         /* if XTLVLD = 1, oscillator is stable */
     break;
   }

  OSCICN |= 0x08; /* Uses extern osc as sysclk, page 89 */

  PRT0MX  = 1;    /* UART TX and RX routed to P0.0 and P0.1, page 94 */
  PRT0CF  = 0x00; /* enable P0.0 (TX) as open drain output,   page 96 0x01 */

  P2 = 0xFF;

#if 0
  PRT3CF  = 0xFF;          /* P3.0 and p3.1 as push-pull            */
  P3MODE  = 0xFF;          /* digital inputs enabled                */
  P3      = 0xFF;
#endif

  SCON   =  xSCON; /*   Init uart  */
  
  T2CON  = xTCON;  /*   Init Timer 2 as BR generator */
/*
//
// Note: the baud rate for communication between master and units in 5230
//       is not 57600 but one which corresponds to reload values below.
//       The value of this baud rate is unknown.
//       Values below were established experimentally.
//       Probably, the baud rate is 76800
//
*/
  RCAP2H = THI2_RESET;
  RCAP2L = TLO2_RESET;

  TR2 = 1;             /* Start Timer 2 */

  RS232Init();

  TI = 0;              /* Clear possible transmit interrupt  */
  RI = 0;              /* Clear possible receive interrupt   */
  
  c = I2cInit(); /* program will hang here if no eeprom! */

  OE   = 1;  /* /OE for buffers, disable buffers */

  LED  = 0;

  ES   = 1;  /* Enable serial interrupt   */
  EA   = 1;
  }

 
