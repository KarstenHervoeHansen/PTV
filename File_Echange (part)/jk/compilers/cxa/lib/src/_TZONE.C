/*
 *	Version : @(#)_tzone.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _tzone.c                                               *
**                                                                        *
**  DESCRIPTION : The _tzset function converts the widely-used time zone  * 
**		  format string pointed by s to tzone format. That string *
**		  takes the form EST05EDT, where the number in the middle *
**		  counts hours West of UTC.                               *
**		  The _tzget functions returns one of the time zone       *
**		  fields which are used in localtime(), mktime() and      *
**		  strftime().                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <time.h>
#include <ctype.h>
#include <string.h>

static char tzone[3][5]  = { "UTC", "UTC", "0" }; /* default zone */

int _tzset( const char * s )
{
	int	i;
	int 	val;
	char 	tztmp[3][5];
	
	if( s )
	{
		/* standard time zone name */
		for ( i = 0; i < 3 ; i++ )
		{
			if ( isalpha( *s ) )
				tztmp[0][i] = *s++;
			else
				return( 0 );
		}
		tztmp[0][3] = '\0';

		tztmp[2][0] = *s == '-' || *s == '+' ? *s++ : '+';

		if ( !isdigit( *s ) )
			return( 0 );

		val = *s++ - '0';

		if( isdigit( *s ) )
			val = 10 * val + *s++ - '0';

		val *= 60;
		
		for ( i = 3; i > 0; i-- )
		{
			tztmp[2][i] = val % 10 + '0';
			val /= 10;
		}
		tztmp[2][4] = '\0';

		/* day light saving time zone name */
		for ( i = 0; i < 3 ; i++ )
		{
			if ( isalpha( *s ) )
				tztmp[1][i] = *s++;
			else
				return( 0 );
		}
		tztmp[1][3] = '\0';
				
		if ( *s == '\0' )
			memcpy( tzone, tztmp, sizeof( tzone ) );
		else
			return( 0 );	
	} else
		return( 0 );

	return( 1 );	
}

char * _tzget( int i )
{
	if ( i == 0 || i == 1 || i == 2 )
		return( tzone[i] );
	else
		return( NULL );
}
