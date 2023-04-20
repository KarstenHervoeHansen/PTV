/*
 *	Version : @(#)strtoul.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strtoul.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strtoul() routine                      *
**                 Converts a string to an unsigned long value            *
**                 Conversion is done until an unknown character is met   *
**                 or until the value doesn't fit in a long anymore       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#define	NEGATIVE	0x1	/* Negative sign read	*/
#define	READ		0x2	/* Done conversion	*/
#define	ERROR		0x4	/* Out of range		*/


unsigned long
strtoul( const char * str,  char ** ptr, int base )
{
	register long	result = 0;
	register unsigned char ch;
	unsigned char	done = 0;
	unsigned long	tstresult = 0;

	if( ptr )	/* No conversion done */
		*ptr = (char *)str;

	do
	{
		ch = *str++;
	} while( ((ch >= 9) && (ch <= 13)) || (ch == ' ' ) );	/* just read all spaces */

	if ( ch == '-' )		/* check for '-' or '+' */
		done |= NEGATIVE|READ;
	else if ( ch == '+' )
		done |= READ;
	else
		str--;

	if ( base == 0 || base == 16 )
	{
		if ( *str != '0' )
		{
			if ( base == 0 )	/* Set base only when not specified already */
				base = 10;
		}
		else	/* base should be Octal or hexadecimal */
		{
			if( str[1] == 'x' || str[1] == 'X' )	/* hexadecimal */
			{
				done |= READ;
				base = 16;
				str+=2;		/* position after '0x' */
			}
			else
				if( base == 0 )
					base = 8;		/* octal */
		}
	}

	for ( ;; )
	{
		ch = *( str++ );

		/* First check numbers, they are most used, keep the routine fast */
		ch =	((ch >= '0') && (ch < '0'+10) && (ch < '0'+base)) ? ch - '0' :
			((ch >= 'a') && (ch < 'a'-10+base)) ? ch - 'a' + 10 :
			((ch >= 'A') && (ch < 'A'-10+base)) ? ch - 'A' + 10 :
			255;

		if ( ch == 255 )				/* unknown character found */
			break;

		done |= READ;

		result = base * result + ch ;

		if ( (done & NEGATIVE) && (result<0) ||		/* out of range ? */
		     (unsigned long)result < (unsigned long)tstresult )
			done |= ERROR;

		tstresult = result;
	}

	if( !(done & READ) )	/* No conversion done */
		return( 0 );

	if ( ptr )		/* pointer to remaining characters */
		*ptr = (char *)(str - 1);

	if( done & ERROR )
	{
		errno = ERANGE;
		return( ULONG_MAX );
	}

	return( done & NEGATIVE ? (unsigned long)-result : (unsigned long)result );
}
