/***************************************************************************/
/*  Copyright DK-Audio A/S, 2005                                           */
/*  Project:  PT8642 DLTPG, using 8051F320 Cygnal                          */
/*            processor.                                                   */
/*  Module:   PT8642.C                                                     */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     22.09.2006                                                   */
/*  Status:   Version 2.0                                                  */
/*  Purpose:  Main program for PT8642 unit.                                */
/*  Note:                                                                  */
/*  Note:    F320 has 16K Flash and                                        */
/*           Internal RAM  255 bytes                                       */
/*           XARM         1024 bytes                                       */
/*           XRAM         1024 dedicated as USB buffer.                    */
/*                                                                         */
/*  Changes:  19.04.2006: new signal   LEVOK = 1; to master introduced.    */
/***************************************************************************/
/* #pragma WARNINGLEVEL (1) Lists only those warnings which may generate   */
                         /* incorrect code. Default is 2.                  */
/***************************************************************************/


#include <C8051F320.h>
#include "define.h"
#include <string.h>
#include "cmd.h"
#include "eeprom.h"
#include "serial.h"

/*
//-------------------
// Global CONSTANTS
//-------------------
*/
#include "io.c"

bit jtag_flag; /* used to switch serial interface to operate in 'transparent' */
               /* mode for FPGA programming.                                  */

/*************************/
/*  Function PROTOTYPES  */
/*************************/
void init_all(void);
void config(void);


void main (void)
  {
  config();
  LED   = 0;

  RS232Init();

  I2cInit(); /* program will hang here if no eeprom mounted! */

  LEVOK = 1; /* BB4, 6 and 8 */
  LED   = 1;

  while (1)
    {
    if (TestInputBuffer())
      CharHandler();
    }
  }

#if 0
/********************************************************/
/* erase_flash_page                                     */
/* This routine erases the FLASH page located at 0x1000 */
/********************************************************/
void erase_flash_page(void) 
  { 
  bit EA_state; 
  char xdata* data pagePointer = 0x1000; /* pointer to xdata space located        */
                                         /* in data space                         */
  EA_state = EA;                         /* holds interrupt state                 */
  EA = 0;                                /* disable interrupts                    */
  FLSCL |= 0x01;                         /* enable FLASH write/erase              */
  PSCTL  = 0x03;                         /* MOVX erases FLASH                     */
                                                                                  
  /* Erase the FLASH page at 0x1000 */  
  *pagePointer = 0;                      /* initiate the erase                    */

  PSCTL   = 0x00;                        /* MOVX writes target XRAM               */
  FLSCL  &= ~0x01;                       /* disable FLASH write/erase             */
  EA      = EA_state;                    /* restore interrupt state               */
  f_valid = FALSE;                       /* indicate that code is no longer valid */
  code_erased = TRUE;                    /* indicate that FLASH has been erased   */
  }
#endif