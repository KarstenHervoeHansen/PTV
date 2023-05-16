/*
 *	Version : @(#)sbrk.c	1.6
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _sbrk.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for sbrk()                                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#if _MODEL == 'l'
#define HUGE    _huge
#else
#define HUGE
#endif

extern	char	_lc_bh;
extern	char	_lc_eh;

static	char HUGE*	breakaddr = & _lc_bh;

char HUGE*	sbrk ( long incr )
{
	char HUGE*	cur;

	if ( ( ( incr > 0 ) && ( (unsigned long)( & _lc_eh - breakaddr ) < incr ) )   || 	/* malloc overflow */
	     ( ( incr < 0 ) && ( (unsigned long)( breakaddr - & _lc_bh ) < -incr ) )	 )	/* free underflow */
	{
	    return (char HUGE*) -1;
	}

	cur = breakaddr;	/* save current */
	
	if ( incr != 0 )
	{
	    breakaddr += incr;
	}

	return cur;
}
