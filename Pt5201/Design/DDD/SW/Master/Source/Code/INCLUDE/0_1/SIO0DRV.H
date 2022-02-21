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
#include "rtxcobj.h"

#define sio0NoError				(int)0
#define sio0NoAcknowledge	(int)1
#define sio0TxCksumError	(int)3

#define sio0RxCksumError	(int)2
#define sio0RxTimeout			(int)4

#define sio0UnknownError	(int)5

extern void sio0_init( void);
extern int sio0SndCommand( int cnt, int addr, const char *format, ...);
extern int sio0SndRequest( int cnt, int addr, char *result, const char *format, ...);

#endif


