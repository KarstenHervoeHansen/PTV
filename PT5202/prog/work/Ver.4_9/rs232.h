/****************************************************************************/
/* MODULE:                                                                  */
/*   rs232.h - RS232 driver module.                                         */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5765
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#ifndef _RS232_H
#define _RS232_H


#include "define.h"
#include "rtxcobj.h"


#define ERR_NONE 0x00
#define ERR_PARITY 0x01
#define ERR_OVERFLOW 0x02
#define ERR_BREAK 0x04
#define ERR_FRAMING 0x08

void rs232_init(void);
void rs232_stop(void);

int rs232Setup(unsigned long Baud, unsigned char Data, unsigned char Parity, unsigned char Hndshk);

int rs232putchar(unsigned char ch);

int rs232puts(char* str);

int rs232printf(int cnt, const char* format, ...);

int rs232getchar(void);

int rs232GetLastError(void);

#endif


