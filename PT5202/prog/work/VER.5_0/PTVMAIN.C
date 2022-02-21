/****************************************************************************/
/* MODULE:                                                                  */
/*   ptvmain.c - ptvmain module for PTV software.                           */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*      ptvmain is the main function for PTV software called by main.       */
/*      The use of ptvmain function to start software is enabled in         */
/*      sysopts.h.                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/


#include "define.h"
#include "instru.h"
#include "rs232.h"

void ptvmain(void);

void ptvmain(void)
  {
  Instrument_Control_Task();
  }


