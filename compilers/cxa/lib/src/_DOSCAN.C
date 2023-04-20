/*
 *	Version : @(#)_doscan.c	1.9	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _doscan.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for _doscan() routines                     *
**									  *
**		   Define :                                               *
**                 SMALL  - No floats                                     *
**                 LARGE  - Full ANSI scanner				  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#define	READ	0x1

#if !defined SMALL && !defined LARGE
#define	LARGE		/* Default scanner */
#endif

/* Function prototypes */
#ifndef	SMALL
extern int  _getfloat	( unsigned char, unsigned int, void *, FILE *,
			  unsigned int *characters);
#endif
static char _getnumber	( unsigned char, unsigned char, unsigned int, void *, 
			  FILE *, unsigned int *characters );
static char _getstring	( const char *, unsigned char, unsigned int, void *, 
			  FILE *, unsigned int *characters );
int	    _doscan	( const char *, va_list, FILE * );

typedef char *	CHARP;

/* When ap == NULL, no assignment should be made */
static char
_getnumber(	register unsigned char ch, register unsigned char type,
		unsigned int length, void * ap, FILE *fin, unsigned int *characters )
{
	register int c;
	register unsigned long value = 0;
	unsigned long tst_value;
	unsigned int hold_char;
	unsigned char neg = 0;

	if( !length )		/* no length specified ?	*/
		length = (unsigned int)(-1);

	while( isspace( ((*characters)++, c = fgetc( fin )) ) )		/* read whitespace */
		;
	/* now 'c' is the first non whitespace character */

	hold_char = *characters - 1;	/* Whitespace characters are not 'converted' */

	if( c == '-'  )		/* negative decimal */
	{
		neg = 1;
		(*characters)++;
		c = fgetc( fin );
		length--;
	}
	else if( c == '+' )	/* Positive decimal */
	{
		(*characters)++;
		c = fgetc( fin );
		length--;
	}

	if( ((ch == 'i') || (ch == 'x')) && (c == '0') )	/* Check for '0x' */
	{
		if( ch == 'i' )
			ch = 'o';	/* Maybe octal, or later we may find it is hexadecimal */
		(*characters)++;
		c = fgetc( fin );
		if( length )
			length--;
		if( c == 'x' || c == 'X' )	/* hexadecimal */
		{
			(*characters)++;
			c = fgetc( fin );
			if( length )
				length--;
			ch = 'x';		/* Make sure hexadecimal values will be read */
		}
	}
	else if( ch == 'i' )
		ch = 'd';		/* Decimal value to read */

	/* now type = 'd' for decimal values	*/
	/* now type = 'u' for decimal values	*/
	/* type = 'o' for octal values		*/
	/* type = 'x' for hexadecimal values	*/
	switch( ch )
	{
	case 'd':
	case 'u':
		while( length-- && isdigit(c) )
		{
			tst_value = value;
			value = value * 10 + c - '0';
			if( value < tst_value )
				value = ULONG_MAX;
			(*characters)++;
			c = fgetc( fin );
		}
		break;
		/* octal */
	case 'o':
		while( length-- && (c >= '0' && c <= '7') )
		{
			tst_value = value;
			value = value * 8 + c - '0';
			if( value < tst_value )
				value = ULONG_MAX;
			(*characters)++;
			c = fgetc( fin );
		}
		break;
		/* hexadecimal */
	case 'x':
		while( length-- && isxdigit( c ) )
		{
			c = toupper( c );
			if( c > '9' )
				c = c + 10 - 'A';
			else
				c = c - '0';
			tst_value = value;
			value = value * 16 + c;
			if( value < tst_value )
				value = ULONG_MAX;
			(*characters)++;
			c = fgetc( fin );
		}
		break;
	}

	ungetc( c, fin );
	(*characters)--;

	if( ch == 'd' || neg )	/* Read a signed value ? */
	{
		if( neg )
			value = (value > LONG_MAX) ? LONG_MIN : -value;
		else
			value = (value > LONG_MAX) ? LONG_MAX : value;

		switch(type)
		{	case 'l':
			case 'L':  break;
			case 'h':  value = ((signed long)value > SHRT_MAX ) ? SHRT_MAX :
					   ((signed long)value < SHRT_MIN ) ? SHRT_MIN : value;
				   break;
			default :  value = ((signed long)value > INT_MAX ) ? INT_MAX :
					   ((signed long)value < INT_MIN ) ? INT_MIN : value;
				   break;
		}
	}
	else		/* Read an unsigned value ? */
	{
		switch(type)
		{	case 'l':
			case 'L':  break;
			case 'h':  if(value > USHRT_MAX)  value = USHRT_MAX;  break;
			default :  if(value > UINT_MAX )  value = UINT_MAX;   break;
		}
	}

	if( *characters != hold_char )	/* conversion is done */
	{
		if( ap )	/* Do assignment AND a real value is detected */
		{
			switch( type )
			{
			case 'l':	/* long				*/
				*(long *)ap = (long)value;
				break;
			case 'h':	/* short, is the same as int	*/
			default:	/* default is also int		*/
				*(int *)ap = (int)( value & UINT_MAX );
			}
			return( 1 );
		}
		else			/* No assignment done -> No conversion */
			return( 0 );
	}
	else	/* No conversion is done, found EOF, return EOF */
	{
		if( c == EOF )
			return( EOF );
		else
			return( 0 );
	}
}

static char
_getstring(	const char * match, unsigned char type,
		unsigned int length, void * ap, FILE * fin, unsigned int *characters )
/* 'match' points to matching string, not including '['	*/
/* when 'ap'==0, no assignment is done, 0 is returned	*/
{
	register unsigned char notsearch = 0;
		/* when '0', no matching string				  */
		/* when '1', search for characters not in matching string */
		/* when '2', search for characters in matching string	  */
	unsigned char converted = 0;
	register const char *search;		/* just a pointer	  */
	register int character = 0;		/* keep read character in */

	if( !length )		/* no length specified ?	*/
		length = 30000;

	if( type == '[' )
	{
		if( *match == '^' )
		{	/* matching characters NOT in matching string ? */
			notsearch = 1;
			++match;
		}
		else
			notsearch = 2;
		(*characters)++;
		character = fgetc( fin );
	}
	else
	{
		while( isspace( ((*characters)++, character = fgetc( fin )) ) )
			;
	}

	if( character == EOF )
	{
		ungetc( character, fin );
		(*characters)--;
		return( EOF );	/* EOF reached before conversion is done */
	}

	while( length-- != 0 )
	{
		if( character == EOF )
			break;

		if( !notsearch )
		{
			if( isspace( character ) )
			{	/* End of string found on whitespace */
				ungetc( character, fin );
				(*characters)--;
				length = 0; /* quit the while loop */
				character = '\n';
				break;
			}
			if( ap )
				*(char *)ap = character;
		}
		else if( notsearch == 1 )
		{		/* match characters not in matching string */
			for( search = match; ; )
			{
				if( character == *search )
				{	/* read character is in string ? */
					if( character != '\n' )
					{
						ungetc( character, fin );
						(*characters)--;
					}
					length = 0; /* quit the while loop */
					character = '\n';
					break;	    /* end the for loop */
				}
				if( *++search == ']' )
				{	/* all matching characters checked ? */
					if( ap )
						*(char *)ap = character;
					break;		/* end the for loop */
				}
			}
		}
		else			/* match characters in string */
		{
			for( search = match; ; )
			{
				if( character == *search )
				{	/* read character is in string ? */
					if( ap )
						*(char *)ap = character;
					break;	/* end the for loop	*/
				}
				if( *++search == ']' )
				{	/* no matching character found ? */
					if( character != '\n' )
					{
						ungetc( character, fin );
						(*characters)--;
					}
					length = 0; /* stop the while loop */
					character = '\n';
					break;	/* end the for loop */
				}
			}
		}

		if( character == '\n' )
			break;
		else
			if( ap )
				ap = (char *)ap + 1;

		converted = 1;	/* Some conversion had success */
		if( length != 0 )	/* When not ready converting, read next character */
		{
			(*characters)++;
			character = fgetc( fin );
		}
	}

	if( ap )
	{
		*(char *)ap = '\0';
		return( converted );
	}
	else
		return( 0 );	/* No assignment -> No conversion done */
}

/* Return EOF on error */
int
_gethex( FILE * fin, int c, unsigned int *characters )
{
	unsigned char value;

	if( !isxdigit( c ) )
	{
		ungetc( c, fin );
		return( EOF );
	}
	value = (c<'9') ? (c-'0') : (c<'G') ? (c-'A'+10) : (c-'a'+10);
	value <<= 4;
	(*characters)++;
	c = fgetc( fin );
	if( !isxdigit( c ) )
	{
		ungetc( c, fin );
		return( EOF );
	}
	value |= (c<'9') ? (c-'0') : (c<'G') ? (c-'A'+10) : (c-'a'+10);
	return( value );
}

/* does the actual scanning */
int
_doscan( const char *fmt, va_list ap, FILE *fin )
{
	register unsigned char ch;	/* character in format string	 */
	register unsigned int length;	/* maximum length of input field */
	register unsigned char type;	/* type of input field (long, short) */
	register const char * match;
	register char * search;
	register unsigned int	count = 0;	/* count the read number of items */
	register int		ch1;		/* character from input	*/
	unsigned char		skipinput = 0;
	unsigned char		noconvert = 1;
	unsigned int		characters = 0;	/* no characters read yet */

	match = fmt;		/* prevent superfluous compiler warning */
	for(;;)
	{
		switch( ch = *fmt++ )
		{
		case '\0':			/* end of scan reached */
			return( count );
		case ' ':	/* blanks or tabs in the format string */
		case '\t':	/* are ignored */
			while( isspace( (characters++, ch1 = fgetc( fin )) ) )
				; /* remove all whitespace from the input */
			ungetc( ch1, fin );	/* We did not read the non-whitespace character yet */
			characters--;
			break;
		case '%':
			ch = *fmt++;		/* get next character */
			if( ch == '%' )
				goto _do_ws;

			/* Formatter expected */
			skipinput = (ch == '*'); /* no assignment ? */
			if( skipinput )
				ch = *fmt++;

			length = 0;
			while( isdigit( ch ) )
			{	/* read length specifier */
				length = length * 10 + ch - '0';
				/* decimal length */
				ch = *fmt++;
			}	/* now if length == 0, no length is specified */

			do
			{
				switch( ch )
				{
				case 'l':
				case 'h':
				case 'L':
					type = ch;
					break;
				default:
					type = 0;
					continue;
				}
				ch = *fmt++;
			} while( 0 );

			switch( ch )
			{
				/* integers, longs, shorts */
			case 'X':
				ch = 'x';
			case 'd':
			case 'i':
			case 'o':
			case 'u':
			case 'x':
				ch1 = _getnumber( ch, type, length,
						  skipinput ? NULL : (CHARP)va_arg( ap, CHARP ),
					    	  fin, &characters );
				if( ch1 == EOF )
					return( noconvert ? EOF : count );
				else
				{
					noconvert = 0;
					count += ch1;
				}
				break;
				/* characters */
			case 'c':
				if( length == 0 )
					length = 1;
				if( !skipinput )
				{
					search = (CHARP)va_arg( ap, CHARP );
					if( (characters++, ch1 = fgetc( fin )) != EOF )
					{
						count++;	/* one more item converted */
						noconvert = 0;
					}

					while( length-- )
					{
						if( ch1 != EOF )
						{
							*(char *)search++ = ch1;
							characters++;
							ch1 = fgetc( fin );
						}
						else
							return( noconvert ? EOF : count );
					}
					ungetc( ch1, fin );
					characters--;					
				}
				else
				{
					while( length-- != 0 )
					{
						if( (characters++, fgetc( fin )) == EOF )
							return( noconvert ? EOF : count );
					}
				}
				break;
				/* matching or non matching string */
			case '[':
				match = fmt;
				/* keep pointer to matching string, */
				/* not including '[' */
				if( *fmt == '^' )
					fmt++;	/* no empty list allowed */
				while( *fmt++ != ']' )
					; /* search for the matching ']' */
				type = ch;
				/* fall through ...	*/
				/* string */
			case 's':
				/* Note : 'ch' is misused */
				ch1 = _getstring( match, type, length,
						  skipinput ? NULL : (CHARP)va_arg( ap, CHARP ),
						  fin, &characters );
				if( ch1 == EOF )
					return( noconvert ? EOF : count );
				else
				{
					count += ch1;
					noconvert = 0;
				}
				break;
			case 'E':
				ch = 'e';
				goto _g_float;
			case 'G':
				ch = 'g';
				goto _g_float;
			case 'e':
			case 'f':
			case 'g':
			_g_float:
#ifndef SMALL
				if( ! _getfloat( type, length,
						 (CHARP)va_arg( ap, CHARP ), fin,
						 &characters ) )
				{
					count++;	/* one more item converted */
					noconvert = 0;
				}
				break;
#else
				errno = ERR_NOFLOAT;	/* no floating point */
				return( count );
#endif
				/* pointer */
			case 'p':
				while( isspace( (characters++, ch1 = fgetc( fin )) ) )		/* read whitespace */
					;
#if _MODEL == 't'		/* Default = '_near' pointer */
				if( ! type )
					type = 'h';
#else	/* _MODEL == 's' || _MODEL == 'm' || _MODEL == 'l', default = '_far' pointer */
				if( ! type )
					type = 'l';
#endif
#if _MODEL == 't' || _MODEL == 's'	/* no huge pointers */
				search = (type == 'h') ? "<_near>" : "<_far>";
#else /* _MODEL == 'm' || _MODEL == 'l' */
				search = (type == 'h') ? "<_near>" : (type == 'l') ? "<_far>" : "<_huge>";
#endif
				while( * search )
				{
					if( ch1 != *search )
					{
						ungetc( ch1, fin );
						return( count );	/* Conversion error */
					}
					search++;
					characters++;
					ch1 = fgetc( fin );
				}
#if _MODEL == 'm' || _MODEL == 'l'
#if _MODEL == 'm'
				if( type == 'L' )	/* Huge pointers */
#else
				if( type != 'h' )	/* Far or Huge pointers */
#endif
				{
				    ((char *)(*(*(CHARP **)ap)))[3] = 0;	/* Huge pointers, fill high byte with '0' */
				    ch1 = _gethex( fin, ch1, &characters );
				    if( ch1 == EOF )
					return( count ); /* Conversion error */
				    ((char *)(*(*(CHARP **)ap)))[2] = ch1;
				    characters++;
				    ch1 = fgetc( fin );
				    if( ch1 != ':' )	/* ':' expected */
				    {
					ungetc( ch1, fin );
					return( count ); /* Conversion error */
				    }
				    characters++;
				    ch1 = fgetc( fin );
				}
#endif
				/* expect 4 hex digits */
				for( length=2; length-- > 0; )
				{	/* length=1, length=0 */
					ch1 = _gethex( fin, ch1, &characters );
					if( ch1 == EOF )
						return( count );	/* Conversion error */
					((char *)(*(*(CHARP **)ap)))[length] = ch1;
					characters++;
					ch1 = fgetc( fin );
				}
#if _MODEL == 't' || _MODEL == 's'
				va_arg( ap, void * );
#else
				if( type == 'h' )
					va_arg( ap, _near void * );
				else if( type == 'l' )
					va_arg( ap, _far void * );
				else
					va_arg( ap, _huge void * );
#endif
				count++;		/* one more item converted */
				break;
			case 'n':
				*(int *)(va_arg( ap, CHARP ))= characters;	
				/* conversion count will not be incremented */
				break;
			default:
				/* error, invalid format string */
				errno = ERR_FORMAT; /* illegal format string */
				return( count );
			}
			break;

		default:	/* must match non-whitespace input character */
		_do_ws:
			characters++;
			ch1 = fgetc( fin );
			if( ch1 != ch )	/* non matching character found */
			{
				ungetc( ch1, fin );
				return( count );
			}
			break;
		}
	}
}
