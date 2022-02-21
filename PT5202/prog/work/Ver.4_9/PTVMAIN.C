/****************************************************************************/
/* MODULE:                                                                  */
/*   ptvmain.c - ptvmain module for PTV software.                           */
/****************************************************************************/
/* FUNCTIONS:                                                               */
	void ptvmain(void);
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
#include "rtxcobj.h"
#include "instru.h"
#include "rs232.h"


void ptvmain(void)
{
/* VAR */

/*BEGIN */
 InstrumentControl = newTThread( 15, Instrument_Control_Task);
 InstrumentControl->Execute( InstrumentControl);
}


void ptvend(void)
{
/* VAR */

/*BEGIN */

 deleteTThread( InstrumentControl);

 rs232_stop();
}