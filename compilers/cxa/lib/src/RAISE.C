/*
 *	Version : @(#)raise.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  raise.c						  *
**                                                                        *
**  DESCRIPTION :  Executes signal handler sig.				  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

signalfunction *signaltable[_NSIG] = {0};

int (raise)(int signal)
{
	signalfunction *s;
	
	if (signal <= 0 || _NSIG <= signal) return(-1);	/* not a legal signal */
		
	if ( (s = signaltable[signal]) != SIG_IGN && s != SIG_DFL )
	{
		signaltable[signal] = SIG_DFL;	/* remove signal handler */
		(*s)(signal); 			/* call signal handler */
	}
	else if (s == SIG_DFL)
		{
			switch (signal)
			{
				case SIGINT:
					fputs("INTerrupt\n", stderr);
					break;
				case SIGILL:
					fputs("ILLegal instruction\n", stderr);
					break;
				case SIGFPE:
					fputs("Floating Point Error\n", stderr);
					break;
				case SIGSEGV:
					fputs("SEGmentation Violation\n", stderr);
					break;
				case SIGTERM:
					fputs("software TERMination\n", stderr);
					break;
				case SIGABRT:
					fputs("ABoRT\n", stderr); break;
				default:
					fputs("Unexpected signal !\n", stderr);
			}
			fputs("## ABORT PROGRAM EXECUTION IMMEDIATE ##\n", stderr);
			exit(EXIT_FAILURE);
		}
	return(0);
}
