/*
 *	Version : @(#)_simio.c	1.5
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _simio.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for _simi and _simo() routine              *
**                 Simulated input and output support for CrossView.      *
**                 This module must be compiled with debug on (-g option) *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <simio.h>

int _simi(unsigned stream, char *port, unsigned len)
{
	return len + stream + *port;	/* names used by CrossView */
}

int _simo(unsigned stream, char *port, unsigned len)
{
	return len + stream + *port;	/* names used by CrossView */
}




