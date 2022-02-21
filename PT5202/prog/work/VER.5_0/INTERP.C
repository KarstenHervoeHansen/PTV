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
/****************************************************************************/


//#include "interp.h"
#include "rs232.h"

extern UC rs232_touched;

extern void CmdHandler( char c);


/*  Task  */
void Interpreter(void)
  {
  static char ch;
  int result;

  result = rs232getchar();
  if (result == -1)
    return;
  ch = (char) result;
  CmdHandler(ch);
  if (!rs232_touched)
    rs232_touched = 1;     // This signals that remote interface was used
  }                        // and the display must return to status.
  