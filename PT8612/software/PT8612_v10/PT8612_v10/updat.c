/***************************************************************************/
/*  Copyright DK-Audio A/S, 2005                                           */
/*  Project:  Programming firmware on HD and DL boards via 5300 Master.    */
/*                                                                         */
/*  Module:   updat.C                                                      */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     03.10.2006                                                   */
/*  Status:   Version 1.0                                                  */
/*  Purpose:  This module contains function update(), which receives F320  */
/*            program in form of a bin file and programs the processor by  */
/*            writing program data into flash program memory.              */
/*            After programming, FLASH memory is read to calculate checksum*/
/*            which is then compared to checksum of received data.         */
/*  Notes:                                                                 */
/*            The F320 processor has 16 kB of flash memory, organized as   */
/*            32 pages @ 512 bytes. Before programming, the memory must be */
/*            erased.  It is possible to only erase 1 page at the time.    */
/*            Function update() is to reside @0x3C00 (see linker args) and */
/*            ocupies less than one page.  To load software                */
/*            including this function, combine program hex file with       */
/*            update.hex, using a text editor.                             */
/***************************************************************************/
#pragma WARNINGLEVEL (1) /* Lists only those warnings which may generate   */
                         /* incorrect code. Default is 2.                  */
/***************************************************************************/
#include <C8051F320.h>
#include <stdio.h>                     /* printf() and getchar()           */
#include <stdlib.h>
#include <ctype.h>                     /* tolower() and toint()            */
/***************************************************************************/

#define START   (UC) 0x31
#define STOP    (UC) 0x32
#define ERR     (UC) 0x20
#define SUCCESS (UC) 0x1B

#define UC unsigned char
#define UI unsigned int

/* unions for unsigned long and unsigned int */
/* to access these data types bytewise       */
/* Note that for Kiel C compiler, charval[0] */
/* carries MSB and charval[3] carries LSB.   */
/* This is the opposite of how TC does it.   */

typedef union yy
    {
    UI   intval;
    UC   bytes[2];
    }  ui;

sbit LED   = P1^2;  /* Test LED on 8612 */

void  update(void);
void  erase_flash(void);
UC    receive_code(void);
char  fetch (void);
void  put (UC);

/******************************************************************
* NAME       void update(void)
* PURPOSE    To receive program data via UART and program processor
*            FLASH program memory.
* 
* ARGUMENTS  None
*
* RETURNS    Nothing
*
* EXTERNS    None
*
* NOTES:     This function first erases all FLASH memory where the
*            program is to be.  Then it receives all bytes and 
*            writes them into FLASH.
*            It then transmits result code via UART.  The result
*            code may be SUCCESS or checksum error.
******************************************************************/
void update(void)
  {
  UC input;


  EA = 0;             /* Disable interrupts (precautionary) */
  erase_flash();

  LED = 1;

  input = receive_code();
  put(input);
  LED = 0;
  }

/***************************************************************************/
/* erase_flash                                                             */
/*                                                                         */
/* This routine erases the first 30 pages of FLASH (0x0000 to 0x3BFF).     */
/* Last page erased is from 0x3B00 to 0x3BFF                               */
/***************************************************************************/
void erase_flash(void)
  {
  UC xdata* data pagePointer = 0;/* a pointer to xdata located in data space */
                                 /* points to the first FLASH page that      */
                                 /* will be erased                           */
  UC i;
  
  PSCTL = 3;                       /* MOVX erases FLASH   */
   
  /* Erase the first 30 FLASH pages  */
  for (i = 0; i < 30; i++)
    {
    FLKEY = 0xA5;                 /* FLASH lock and key sequence 1  */
    FLKEY = 0xF1;                 /* FLASH lock and key sequence 2  */
    
    *pagePointer = 0;             /* initiate the erase  */
    
    pagePointer += 512;
    }   

  PSCTL = 0;                      /* MOVX writes target XRAM */
  }

/********************************************************************************/
/* receive_code                                                                 */
/* This routine receives the new firmware through the UART as binary data       */
/*                                                                              */
/* Returns: 0 - on Success                                                      */
/*          3 - on checksum fail                                                */
/********************************************************************************/
UC receive_code(void)
  {
  UC xdata* data pwrite;              /* pointer used for writing FLASH       */
  UC code* data pread;                /* pointer used for reading FLASH       */
  ui  len;                            /* holds length of data                 */
  UC checksum;                        /* holds checksum of received data      */
  UC flash_checksum;                  /* holds the checksum calculated after  */
                                      /* the FLASH has been programmed        */
  UC c;
  UI i;

  /* receive data length */
  len.bytes[1] = fetch(); /* LSB */
  len.bytes[0] = fetch(); /* MSB */


  PSCTL    = 1;                       /* MOVX writes to FLASH                 */
  pwrite   = (char xdata*) 0;         /* initialize the write pointer         */
  checksum = 0;

    /* write the data into FLASH */
    for(i = 0; i < len.intval; i++)
      {
      if (pwrite < 0x3D00)            /* check for valid pointer          */
        {                             /* updater starts at 0x3D00         */
        FLKEY     = 0xA5;             /* FLASH lock and key sequence 1    */
        FLKEY     = 0xF1;             /* FLASH lock and key sequence 2    */
        c         = fetch();
        checksum += c;
        *pwrite   = c;                /* write one byte to FLASH          */
        pwrite++;                     /* increment FLASH write pointer    */
        } 
      }
      
    PSCTL = 0;                             /* MOVX writes target XRAM           */
          
    /* verify the checksum  */
    pread          =  (char code*) 0;      /* initialize the read pointer       */
    flash_checksum = 0;                    /* set the flash_checksum to zero    */

    /* add the data field stored in FLASH to the checksum */
    for(i = 0; i < len.intval; i++)
      {
      flash_checksum += *pread++;
      } 
    
    /* verify the checksum (the flash_checksum should equal zero) */
    if (flash_checksum != checksum)
      {
      /*printf("*** checksum failed, try again");  */
      return (3);
      }
  return (0);                   /* signal SUCCESS */
  }

/******************************************************************
* NAME       char fetch(void)
* PURPOSE    to fetch one char from UART
* 
* ARGUMENTS  None
*
* RETURNS    Received char
*
* EXTERNS    None
*
* NOTES:     This function first signals to the sender that it is
*            ready to receive by sending START.  It then waits 
*            for character and when received, returns it.
******************************************************************/
char fetch(void)
  {
  char c;

  put(START);

  while (!RI0)
    ;
  c   = SBUF0;
  RI0 = 0;
  return (c);
  }

#if 0
void put (UC c) /* This one is supplied by Silabs. It does not work */
  {
  while (!TI0)
    ;
  TI0   = 0;
  SBUF0 = c;
  }
#else
/******************************************************************
* NAME       void put(UC c)
* PURPOSE    To send one character via UART
* 
* ARGUMENTS  None
*
* RETURNS    Nothing
*
* EXTERNS    None
*
* NOTES:     
******************************************************************/
void put(UC c) /* This one is supplied by JK. It DOES work */
  {
  SBUF0 = c;
  while (!TI0)
    ;
  TI0   = 0;
  }
#endif

