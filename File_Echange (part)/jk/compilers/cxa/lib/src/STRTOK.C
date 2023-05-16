/*
 *	Version : @(#)strtok.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strtok.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for strtok() routine                       *
**                 splits the first string into tokens separated by       *
**                 characters from the second string.                     *
**                 uses : strpbrk(), strspn()                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>
#include <string.h>

/*
 * uses strpbrk and strspn to break string into tokens on
 * sequentially subsequent calls.  returns NULL at end.
 */
char *
strtok( register char * string, register const char * sepset )
{
	register char *q, *r;
	static	char *savept;

	q = string ? string : savept;

	if( !q )			/* No string to search in ?	  */
		return((char *)NULL);

	q += strspn( q, sepset);	/* point to first character which */
					/* is not a separator		  */

	if( *q == '\0' )		/* empty string left, no tokens   */
	{				/* anymore			  */
		savept = NULL;		/* be sure a next call will also  */
		return((char *)NULL);	/* return NULL			  */
	}

	if((r = strpbrk(q, sepset)) == NULL) /* r will point after the token */
		savept = NULL;
	else
	{
		*r = '\0';	/* terminate the token with a NULL character */
		savept = ++r;
	}

	return(q);			/* return a pointer to the token */
}
