/*
 *	Version : @(#)_doflt.c	1.9	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _doflt.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for '_doflt()' routine                     *
**                 Which is the formatter for floating point printing     *
**                 This file is only linked when floating point is used   *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <float.h>
#include <math.h>

#ifndef	_SINGLE_FP
#define	NDIG	(DBL_DIG+3)
#define EXP	0x7FF
#else
#define	NDIG	(FLT_DIG+3)
#define EXP	0xFF
#endif

/* Return the number of characters written */
void _doflt( int, int, unsigned char, unsigned char, double *, FILE *, unsigned int *characters );

/* Flags in 'format' */
#define	ALTERNATE	0x10
#define	LEFT		0x40

/* min		-> minimum output width
 * precision	-> number of characters after the '.'
 * type		-> Conversion character ('e', 'E', 'f', 'g', 'G')
 * format	->	b6 - Left alignment
 *			b5 - zero fill
 *			b4 - Alternate printing
 *			b3,b2 - 0x04 --> always sign
 *			      - 0x08 --> sign or space
 *			Other bits are not used by floating point printing
 * number	-> Floating point value to print
 * fout		-> File to print to
 */
void
_doflt( int min, int precision, unsigned char type, unsigned char format,
	double * number, FILE *fout, unsigned int *characters )
{
	unsigned char		vsign;			/* sign of the value	*/
	int			decpt;
	register char * 	q;
	register int		i;
	int			j;
	unsigned int		length = 0;		/* To determine the length of the string to print */
	double			fi, tmp_fi;		/* Integral part of 'arg' */
	double			fj;
	double			multiplier = 1.0;
	register char *		p;
	unsigned char		first_nonzero = 0;
	char			buf[ NDIG ];		/* Room to print ascii representation into */
	double			arg = *number;
	
	/*
	 *
	 *	The conversion
	 *	code in the floating point formatter cannot handle these two operand
	 *	types properly, so they must be tested before entering the formatter.
	 *	Testing the operand must be done without using floating point instructions
	 *	( they might raise exceptions ).
	*/
	/* print infinity in 4 chars, as native hosts and CrossView do */
#if _SINGLE_FP
	if ( isinff( arg ) )
	{
		if ( arg > 0 )
		{
			fputs( "+Inf", fout );
		}
		else
		{
			fputs( "-Inf", fout );
		}
		return;
	}
	if ( isnanf( arg ) )
	{
		fputs( "NaN", fout );
		return;
	}
#else
	if ( isinf( arg ) )
	{
		if ( arg > 0 )
		{
			fputs( "+Inf", fout );
		}
		else
		{
			fputs( "-Inf", fout );
		}
		return;
	}
	if ( isnan( arg ) )
	{
		fputs( "NaN", fout );
		return;
	}
#endif

	/* ANSI specification */
	if(    !precision
	    && (type & 0xDF) == 'G'
	  )
		precision = 1;

	/* After conversion 'decpt' is :
	 *	< 0	- Only fractional part present
	 *	== 0	- 0.0 or 1.0
	 *	> 0	- Integer part present
	 */
	decpt = 0;

	vsign = 0;	/* 0 - positive value, otherwise negative value */	

	/* Check the sign, make sure the value gets positive */
	if ( arg < 0.0 )
	{
		vsign = '-';
		arg = -arg;
	}

	/* Get integer and fractional part */
	arg = modf( arg, &fi );
	/* In 'arg', the fractional part remains */

	p = buf;
	/*
	 * Do integer part
	 */
	if ( fi != 0.0 )
	{
		q = &buf[ NDIG ];
		tmp_fi = fi;
		while ( fi != 0.0 )
		{
			/* When buffer is full, shift the complete buffer */
			/* one position to the right */
			if ( q == buf )
			{
				p  = &buf[ NDIG-1 ];
				q = &buf[ NDIG ];
				while ( p != buf ) *--q = *--p;
				/* Now 'q' points to 'buf+1', the left most */
				/* position of 'buf' can be used again */
			}
			fj = modf( fi * 0.1, &fi );	/* 'fj' gets fractional value (divide by 10 == Multiply with 1/10 */
			if( !first_nonzero )
			{
				multiplier *= 10.0;
				if( (char)( (fj + 0.03) * 10.0 ) )
				{
					first_nonzero = 1;
/* In this special case it is more precise to divide tmp_fi with a multiplier
 * than to multiply tmp_fi with a divisor. So, this is less precise.
 * divisor *= 0.1;
 * ..
 * fj = modf( tmp_fi * divisor, &fi );
 * ..
 */
					fj = modf( tmp_fi / multiplier, &fi );
				}
			}
			*--q = (char)( (fj + 0.03) * 10.0 ) + '0'; /* Convert fraction to a character */
			decpt++;		/* Dot one position to the right */
		}
		while ( q < &buf[ NDIG ] )	/* Move all converted characters to */
			*p++ = *q++;		/* the left side of the buffer */
	}
	else if ( arg > 0.0 )
	{	/* make 'arg' a value between 1 and 10 */
		while ( (fj = arg*10.0) < 1.0 )
		{
			arg = fj;
			decpt--;		/* Dot one position to the left */
		}
	}

	/* Rounding starts one position after the precision we can print */
	i = precision;
	if( type == 'f' )
		i += decpt;
	if( (type & 0xDF) == 'E' )	/* Always one digit in front of the '.', which is not counted */
		i += 1;
	q = &buf[ (i < 0) ? 0 : (i >= NDIG-2) ? NDIG-2 : i ];

	/* Now convert fractional part */
	/* q points to the last position in 'buf' where a character may be written */
	while ( p <= q && p < &buf[ NDIG ] )
	{
		arg *= 10.0;
		arg = modf( arg, &fj );
		*p++ = (char)fj + '0';
	}

	/* Rounding up at last position */
	p = q;
	/* Rounding up, real precision starts at position 15 for double	*/
	/* precision and at position 6 for single precision.		*/
	/* Rounding up starts at position DBL_DIG+2, if number of 	*/
	/* digits > DBL_DIG+1.						*/

	*q += 5;	/* Add 5 to last position */
	if( *q > '9' )
	{
		while ( *q > '9' )	/* Carry ? */
		{
			if ( q > buf )	/* Add carry to previous digit */
			{
				p = q;	/* round to '0', and remove 0 digits at the end of the string */
				*--q += 1;
			}
			else
			{	
				/* When leftmost digit, it is rounded to '1' */
				*q = '1';
				/* Decimal point one position to the right */
				decpt++;
			}
		}
	}
	*p-- = '\0';	/* Terminate string and point to last character in buffer */

	/* Remove '0' characters at the end of the string */
	for( ; p>=buf && *p=='0'; p-- )
		*p = 0;

	if( !vsign )
	{
		if( (format & 0x0C) == 0x04 )	/* sign always	*/
			vsign = '+';
		else if( (format & 0x0C) == 0x08 ) /* sign or space */
			vsign = ' ';
	}	

	/* To do the correct alignment, we need to detect the length of the string we are going to print */
	if( vsign )
		length++;
	
	if ( ((type & 0xDF) == 'E') || (type != 'f' && ((decpt >= precision) || decpt < -4)) )
	{	/* E-style */
		q = buf;
		length += 1;	/* Digit before the '.' */

		if( (type & 0xDF) == 'E' )
		{
			if( *q )
				q++;
			if( *q || (format & ALTERNATE) || precision )
				length++;	/* '.' */
			length += precision;
		}
		else	/* 'g' or 'G' */
		{
			j = precision;
			if( *q )
			{
				j--;
				q++;
			}
			if( *q || (format & ALTERNATE) )
				length++;	/* '.' */
			for( i=0; i<j; i++ )
			{
				if( *q || (format & ALTERNATE) )
					length++;
			}
		}
#ifndef	_SINGLE_FP
		length += 5;	/* Exponent double prec. */
#else
		length += 4;	/* Exponent single prec. */
#endif
	}
	else	/* F-style */
	{
		if( type == 'f' )
		{
			if( decpt <= 0 )
				length++;		/* '0' before '.' */
			else
				length+=decpt;		/* nr of digits before '.' */
			if( precision || (format & ALTERNATE) )
			{
				length++;		/* '.' */
				length += precision;	/* nr of digits after '.' */
			}
		}
		else	/* 'g' or 'G' */
		{
			q = buf;
				i = precision;

			if( decpt <= 0 )
				length++;		/* '0' before '.' */
			else
			{
				length+= decpt;		/* nr of digits before '.' */
				j = decpt;
				do
				{
					if( *q )
						q++;
					if( i ) i--;
				} while( --j );
			}
			if( (*q && i) || (format & ALTERNATE) )
			{
				length++;		/* '.' */
				j = -decpt;
				/* no detect nr of digits after '.' */
				while( i && j-- > 0 )
				{
					length++;
					i--;
				}
				while( i && *q )
				{
					length++;
					q++;
					i--;
				}
				if( format & ALTERNATE )
					length+= i;
			}
		}
	}

	if( vsign && (format & 0x60) )	/* ( (format & 0x20) || */
	{				/* (format & 0x40) )	*/
		(*characters)++;
		fputc( vsign, fout );
		vsign = 0;
	}	

	if( !(format & LEFT) )	/* Right alignment ? */
	{
		for( min-=length; min>0; min-- )
		{
			(*characters)++;
			fputc( (format & 0x20) ? '0' : ' ', fout );
		}
	}
	
	if( vsign )
	{	
		(*characters)++;
		fputc( vsign, fout );
	}

	q = buf;

	/* ANSI : Use 'e' notation when (exponent < -4) || (exponent >= precision) */
	/* ASCII 'e' -> 0x65, 'E' -> 0x45 --> AND with 0xDF -> 'e' becomes 'E' */
	if ( ((type & 0xDF) == 'E') || (type != 'f' && ((decpt >= precision) || decpt < -4)) )
	{	/* use E-style */
		if( !*q )			/* Value 0.0, no digits present */
			decpt++;		/* Add one to the exponent */

		/* For 'g' or 'G', the digit before the '.' is also a significant digit */
		if( *q && ((type & 0xDF) == 'G') )
			precision--;

		(*characters)++;
		fputc( *q ? *q++ : '0', fout );

		if(    *q					/* Digits remain */
		    || (format & ALTERNATE)			/* or Alternate printing */
		    || (precision && ((type & 0xDF) == 'E'))	/* Or not the complete precision is printed yet */
		  )
		{
			(*characters)++;
			fputc( '.', fout );
		}

		for ( i=0; i<precision; i++ )
		{
			if( *q || (format & ALTERNATE) || ((type & 0xDF) == 'E') )
			{
				(*characters)++;
				fputc( *q ? *q++ : '0', fout );
			}
		}

		/* Watch this, in ASCII, the codes for
		 * 'e'	-> 0x65		'g'	-> 0x67
		 * 'E'	-> 0x45		'G'	-> 0x47
		 * To convert 'g' to 'e' and 'G' to 'E', we only have to AND the character
		 * with 0xFD (clear bit '1')
		 */
		(*characters)++;
		fputc( type & 0xFD, fout );	/* Position the 'exponent' */

		if ( --decpt >= 0 )		/* Now print exponent '+xxx' or '-xxx' */
		{
			(*characters)++;
			fputc( '+', fout );		/* First print sign ... */
		}
		else
		{
			decpt = -decpt;
			(*characters)++;
			fputc( '-', fout );
		}

		/* Value of decpt now always >= 0 */
		/* Next casts are neccessary to get unsigned division */
#ifndef	_SINGLE_FP	/* exponent always < 100 */
		(*characters)++;
		fputc( (unsigned int)decpt/100U + '0', fout );	/* Print a three digit exponent */
		decpt = (unsigned int)decpt % 100U;
#endif
		/* Now decpt always < 100, we may do character division ! */
		(*characters)++;
		fputc( (unsigned char)decpt / 10U + '0', fout );	/* We only print a two digit exponent */
		(*characters)++;
		fputc( (unsigned char)decpt % 10U + '0', fout );
	}
	else
	{
		i = decpt;
		if( decpt <= 0 )	/* Make sure at least one digit is printed before the '.' is placed */
		{
			(*characters)++;
			fputc( '0', fout );
		}
		else		/* Print all digits before the '.' */
		{
			do
			{
				(*characters)++;
				fputc( *q ? *q++ : '0', fout );
				if( (type != 'f') && precision )		/* Count all significant digits */
					precision--;
			} while( --i > 0 );
		}
		/* Now we are at the '.' position */
		if( (((type == 'f') || *q) && precision) || (format & ALTERNATE) )
		{	/* '*q' != 0 when some digit != '0' is not printed yet */
			(*characters)++;
			fputc( '.', fout );
			i = -decpt;
			while( precision && (i-- > 0) )
			{
				(*characters)++;
				fputc( '0', fout );
				precision--;
			}
			while( precision && *q )	/* Now copy the rest of the digits */
			{
				(*characters)++;
				fputc( *q++, fout );
				precision--;
			}
			if( (type == 'f') || (format & ALTERNATE) )
			{
				while( precision )		/* And trailing zeros */
				{
					(*characters)++;
					fputc( '0', fout );
					precision--;
				}
			}
		}
	}

	/* For left alignment, we still have to print some spaces */
	for( min-=length; min>0; min-- )
	{
		(*characters)++;
		fputc( ' ', fout );
	}
}
