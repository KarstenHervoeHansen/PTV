/***************************************************************************/
/*  Copyright DK-Audio A/S, 2003                                           */
/*  Project:  PT8640 Trilevel HDTV generator, using 8051F231 Cygnal        */
/*            processor.                                                   */
/*  Module:   SPI_MOD1.c                                                   */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     25.02.2004                                                   */
/*  Purpose:  This module implements SPI transfer function to transfer     */
/*            data to SPARTAN FPGA on PT8640 board.                        */
/***************************************************************************/


#include "f200.h"
#include "define.h"
#include "spi_mod1.h"

#if 0
sbit SCL =  P1^2; /* PT8640 defs (P1^2) for onboard eeprom */
sbit SDA =  P1^1;
#endif

sbit LED   = P2^4;
sbit SCK   = P2^0;
sbit MISO  = P2^1;
sbit MOSI  = P2^2;
sbit NSS_0 = P2^3;
sbit NSS_1 = P2^5;
sbit NSS_2 = P1^7;
sbit NSS_3 = P1^6;
sbit NSS_4 = P1^5;
sbit NSS_5 = P1^4;
sbit NSS_6 = P1^3;
sbit NSS_7 = P0^2;

typedef struct aa
  {
  long   samples_per_line;
  long   lines_per_frame;
  UC   sys_clk;
  UC   scan;
  UC   sync;
  }  TLG_FORMATS;

/*      spl      lpf   clk  scan  sync      This is version 1.0  */
code TLG_FORMATS tlg_fmt_table[22] =
  {
    {   1,      1125,   0,   0,    0}, /*  0 */  
    {2200 / 11, 1125,   0,   0,    0}, /*  1 */  
    {2200 / 11, 1125,   1,   0,    3}, /*  2 */
    {2640 / 11, 1125,   0,   0,    2}, /*  3 */
    {4400 / 11, 1125,   0,   1,    0}, /*  4 */
    {4400 / 11, 1125,   1,   1,    3}, /*  5 */
    {5280 / 11, 1125,   0,   1,    2}, /*  6 */
    {4400 / 11, 1125,   0,   0,    0}, /*  7 */
    {4400 / 11, 1125,   1,   0,    3}, /*  8 */
    {5280 / 11, 1125,   0,   0,    2}, /*  9 */
    {5500 / 11, 1125,   0,   0,    0}, /* 10 */
    {5500 / 11, 1125,   1,   0,    0}, /* 11 */
    {4400 / 11, 1125,   0,   1,    0}, /* 12 */ /* will be removed */
    {4400 / 11, 1125,   1,   1,    3}, /* 13 */ /* will be removed */
    {3300 / 10,  750,   0,   0,    0}, /* 14 */
    {3300 / 10,  750,   1,   0,    3}, /* 15 */
    {3960 / 10,  750,   0,   0,    2}, /* 16 */
    {6600 / 10,  750,   0,   0,    0}, /* 17 */
    {6600 / 10,  750,   1,   0,    3}, /* 18 */
    {7920 / 10,  750,   0,   0,    2}, /* 19 */
    {8250 / 10,  750,   0,   0,    0}, /* 20 */
    {8250 / 10,  750,   1,   0,    0}  /* 21 */ /* 22 elements */
  };

/* gen_params: */

extern CURR_PARAMS tlgs[4];
extern bit         changed;
extern bit         genlock_changed;
extern UC          current_generator;
extern ul          tlg_samples;

/******************************************************************
 * NAME       UC spi_transfer (UC spi_byte, UC cnt)
 * PURPOSE    To transfer data bits using SPI protocol
 * 
 * ARGUMENTS  UC spi_byte - data byte  to be transferred
 *            UC cnt      - nr of bits to be transferred
 *
 * RETURNS    byte received simultaneously 
 *
 * EXTERNS    None
 *
 * NOTES:     This function wil serialize cnt bits of spi_byte
 *            and send them via MOSI, MSB bit first.
 *            If cnt is less than 8, make sure at the bits to 
 *            send are already shifted to the left before 
 *            calling this function.
 *            Slave select signals are not handled by this function.
 ******************************************************************/
UC spi_transfer (UC spi_byte, UC cnt)
  {
  UC i;

  for (i = 0; i < cnt; i++)
    {
    SCK = 0x01;

    if (spi_byte & 0x80)
      MOSI = 1;
    else
      MOSI = 0;
    spi_byte = spi_byte << 1;
    SCK = 0x00;
    spi_byte |= MISO;
    }
  return (spi_byte);
  }

/******************************************************************
 * NAME       void write_plds(UC ndx)
 * PURPOSE    To construct and send data packets for Spartan PLD.
 *
 * ARGUMENTS  UC ndx - index into tlg_fmt_table[], i.e. format.
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     
 *            This function is called each time new delay value is
 *            received from PT5230 Master.  It uses ndx to extract
 *            values from tlg_fmt_table[] 
 *
 *            The transfer protocol is as follows:
 *
 *            Particular generator is selected with its CS (NSS_x)
 *            - 26 bits of delay (as 4 bytes)
 *            -  1 bit  of system clock
 *            -  2 bits of sync mode
 *            - 10 bits of samples_per_line - 1 (2 bytes)
 *            and then:
 *            -  2 bits of generator number // no longer need for, 
 *               as we use CS for selecting one of 4 generators.
 *            -  1 bit  of scan method.
 *            - 12 bits of total lines per frame - 1 (2 bytes)
 *            Particular generator is deselected with its CS
 *
 *            This function is called from main loop as follows:
 *
 *            write_plds(tlgs[current_generator].format);
 *
 *            See "Kommunikations protokol PT8611 - Tri Level Sync
 *                 Generator (CPU - FPGA), PCB layout= tsg_rev2.pcb
 * 
 *******************************************************************
 *     Next time you see debugger skipping some of your code......
 *******************************************************************
 *  This is due to optimization!  Resulting code does not correspond
 *  to the code you have written, and there is nothing you can do
 *  about it!
 ******************************************************************/
void write_plds(UC ndx)
  {
  UC tmp;
  ui itmp;

  tlg_samples.longval = tlgs[current_generator].delay.longval;

#if 1
  if (tlg_fmt_table[ndx].samples_per_line == 1L)
    LED = 1;
  else
    LED = 0;
#endif

  /* set chip selects */
  switch (current_generator)
    {
    case 0:
      NSS_0 = 0;
      break;
    case 1:
      NSS_1 = 0;
      break;
    case 2:
      NSS_2 = 0;
      break;
    case 3:
      NSS_3 = 0;
      break;

    }

  spi_transfer(tlg_samples.charval[0], 8); /* 1 */
  spi_transfer(tlg_samples.charval[1], 8); /* 2 */
  spi_transfer(tlg_samples.charval[2], 8); /* 3 */
  spi_transfer(tlg_samples.charval[3], 8); /* 4 */

  tmp = tlg_fmt_table[ndx].sync;
  tmp <<= 4;  /* sync bits go to bits 5 and 5 of tmp */

  if (tlg_fmt_table[ndx].sys_clk)     /* bit 7 is sysclk */
    tmp |= 0x40;
  else
    tmp &= 0xBF;

  itmp.intval = tlg_fmt_table[ndx].samples_per_line;
  itmp.intval--;                      /* we send spl - 1 */

  tmp &= 0xFC;                        /* bits 2 and 3 are 0     */
  tmp |= itmp.charv[0];               /* two MSbits of SPL - 1  */

  spi_transfer(tmp, 8); /* 5 */

  tmp = itmp.charv[1];                /* rest of SPL - 1 */
  spi_transfer(tmp, 8);               /* 6 */

/***/
  /* 'Master' 2 bytes */
  tmp = 0;

  if (tlg_fmt_table[ndx].scan) /* Scanmethod is bit 4      */
    tmp |= 0x10;
  else
    tmp &= 0xEF;

  itmp.intval = tlg_fmt_table[ndx].lines_per_frame;
  itmp.intval--;

  tmp &= 0xF8;                   /* zero bits 0, 1 and 2 of tmp  */
  tmp |= itmp.charv[0];          /* 3 MSbits of LPF - 1 */
  spi_transfer(tmp, 8); /* 7 */

  tmp = itmp.charv[1];           /* rest of LPF - 1     */
  spi_transfer(tmp, 8); /* 8 */

/***/

  switch (current_generator)
    {
    case 0:
      NSS_0 = 1;
      break;
    case 1:
      NSS_1 = 1;
      break;
    case 2:
      NSS_2 = 1;
      break;
    case 3:
      NSS_3 = 1;
      break;
    }
  }


