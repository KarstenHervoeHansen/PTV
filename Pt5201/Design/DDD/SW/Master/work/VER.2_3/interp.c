/****************************************************************************/
/* MODULE:                                                                  */
/*   interp.c - Interpreter module RS232 remote.                            */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
	void Interpreter(void);
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   TK-PTV software for PT5765    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/


//#include "interp.h"
#include "rs232.h"


extern void CmdHandler( char c);


/*  Task  */
void Interpreter(void)
{
 static char ch;

 for(;;)
 {
  ch=rs232getchar();

  CmdHandler(ch);
 }
}
  