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

#define ERR_NONE 0x00
#define ERR_PARITY 0x01
#define ERR_OVERFLOW 0x02
#define ERR_BREAK 0x04
#define ERR_FRAMING 0x08

void uart1_init(void);
char uart1_getchar( char *);
void uart1_putchar(unsigned char);
void uart1_puts(char *);
int rs232printf( int, const char*, ...);
int rs232putchar(unsigned char);
int rs232puts( char*);
int rs232getchar(void);
int rs232GetLastError( void);

#endif


