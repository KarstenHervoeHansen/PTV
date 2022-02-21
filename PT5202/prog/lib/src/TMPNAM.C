/*
 *	@(#)tmpnam.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  tmpnam.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for tmpnam() routine                       *
**                 Generates a 'unique' file name. In this version        *
**                 not the process-id is taken but a random number.       *
**                 This could be dangerous !                              *
**                                                                        *
**                 This routine should be customised.                     *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <string.h>

int	rand( void );

char *
tmpnam( char * s )
{
	/* 's' buffer for temporary file name */
	int	i;
	char	*p;
	unsigned	int	j;
	unsigned	int	r;
	static	char	namebuf[ L_tmpnam ];

	if( s == NULL )
		s = namebuf;
	
	r = rand();	/* should be something with getpid()... */
	strcpy( s, P_tmpdir );
	i = 5;
	p = s + strlen( s ) + i;
	*p = '\0';
	for( j = r; 0 <= --i; j>>=3 )
		*--p = '0' + (j & 07);
	return( s );
}
