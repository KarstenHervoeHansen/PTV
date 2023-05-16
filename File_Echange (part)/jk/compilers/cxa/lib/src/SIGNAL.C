/*
 *	Version : @(#)signal.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  signal.c						  *
**                                                                        *
**  DESCRIPTION :  Install signal handler in table.			  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <signal.h>

extern signalfunction *signaltable[_NSIG];

signalfunction *(signal)(int signal, signalfunction *function)
{
	signalfunction *s;
	
	if (signal <= 0 || _NSIG <= signal || function == SIG_ERR) 
		return (SIG_ERR);
		
	s = signaltable[signal], signaltable[signal] = function;
	
	return (s);
}
