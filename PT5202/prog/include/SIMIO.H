/*
 *	Version : @(#)simio.h	1.3 96/12/16
 */

/**************************************************************************
**                                                                        *
**  FILE        :  simio.h                                                *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes and macros for            *
**                 simulated input and output for CrossView               *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef	SIMIO_INC

int _simi(unsigned stream, char *port, unsigned len);

int _simo(unsigned stream, char *port, unsigned len);


#define	SIMIO_INC
#endif



