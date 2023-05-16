/*
 *	Version : @(#)_ctype.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _ctype.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for definition of the array needed for all *
**                 character checking routines/macros                     *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <ctype.h>

const char _ctype_[257] = {
	0,	/* EOF character */
	__C,	__C,	__C,	__C,	__C,	__C,	__C,	__C,
	__C,	__W,	__W,	__W,	__W,	__W,	__C,	__C,
	__C,	__C,	__C,	__C,	__C,	__C,	__C,	__C,
	__C,	__C,	__C,	__C,	__C,	__C,	__C,	__C,
	__S,	__P,	__P,	__P,	__P,	__P,	__P,	__P,
	__P,	__P,	__P,	__P,	__P,	__P,	__P,	__P,
	__N,	__N,	__N,	__N,	__N,	__N,	__N,	__N,
	__N,	__N,	__P,	__P,	__P,	__P,	__P,	__P,
	__P,	__U|__X,__U|__X,__U|__X,__U|__X,__U|__X,__U|__X,__U,
	__U,	__U,	__U,	__U,	__U,	__U,	__U,	__U,
	__U,	__U,	__U,	__U,	__U,	__U,	__U,	__U,
	__U,	__U,	__U,	__P,	__P,	__P,	__P,	__P,
	__P,	__L|__X,__L|__X,__L|__X,__L|__X,__L|__X,__L|__X,__L,
	__L,	__L,	__L,	__L,	__L,	__L,	__L,	__L,
	__L,	__L,	__L,	__L,	__L,	__L,	__L,	__L,
	__L,	__L,	__L,	__P,	__P,	__P,	__P,	__C
};
