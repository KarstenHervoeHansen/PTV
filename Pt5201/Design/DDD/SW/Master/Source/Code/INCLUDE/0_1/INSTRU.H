/****************************************************************************/
/* MODULE:                                                                  */
/*   instru.h - structures and enums for instrument control task            */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/


#ifndef _INSTRU_H
#define _INSTRU_H


extern void Instrument_Control_Task( void);
extern TThread *InstrumentControl;

#endif