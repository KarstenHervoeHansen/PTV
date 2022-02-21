/****************************************************************************/
/* MODULE:                                                                  */
/*  sio0drv.h - seriel channel driver using UART0 and Timer2                */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#ifndef _SIO0DRV_H
#define _SIO0DRV_H


#include "define.h"
void uart0_init(void);
char uart0_getchar( char *);
char uart0_getchar_timeout(char *);
void uart0_putchar(int);
void uart0_puts(char *);
int sio0WaitAcknowledge(int, char *);
int sio0WaitAnswer( void *);
int sio0SndCommand(int, int, const char *, ...);
int sio0SndRequest(int, int, char *, const char *, ...);

#endif


