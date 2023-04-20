/******************************************************************************
|*
|*  VERSION CONTROL:    @(#)fptrap.c	1.7 96/12/18
|*
|*  IN PACKAGE:         FPRT (Floating Point Run-Time C expression support)
|*
|*  COPYRIGHT:          1996 Copyright TASKING, Inc.
|*
|* Example program for installing a floating point trap handler
|* for performing specific actions.
|*
 *****************************************************************************/

/*
 * Note: Be aware that any floating point operation done inside the exception
 *       handler will be done recursively, i.e. using the floating point
 *       run-time library again.
 *       Therefore either temporarily turn off floating point exception
 *       or ensure no floating point exception can arise.
*/

#include <float.h>

/*
 * As example a table with all preferred result values
 * (a full 3-dimensional matrix may be necessary instead).
 * An operation value of -1 acts as wildcard.
 * Note that multiple exceptions may arise simultaneously.
*/
typedef struct new_results_t
{
	_fp_exception_type_t	exception;
	_fp_operation_type_t	operation;
	long			signed_integer;
	unsigned long		unsigned_integer;
	double			floating_point;
}
new_results_t;

static new_results_t	preferred_result[] =
{
    /* invalid exception operation new result value(s) */
    { EFINVOP,	127,		0x1eadbeefL,	0xdeadbeefL,	+0.12	},

    /* divide by zero exception new result value(s) */
    { EFDIVZ,	127,		0x1eadbeefL,	0xdeadbeefL,	+4.15	},

    /* overflow exception new result value(s) */
    { EFOVFL,	_OP_ADDITION,	0x01010101L,	0xdeadbeefL,	+3.14	},
    { EFOVFL,	127,		0x1eadbeefL,	0xdeadbeefL,	+100.0	},

    /* underflow exception new result value(s) */
    { EFUNFL,	127,		0x1eadbeefL,	0xdeadbeefL,	-2.72	},

    /* inexact exception new result value(s) */
    { EFINEXCT,	127,		0x1eadbeefL,	0xdeadbeefL,	+4.56	}
};

static void	assign_new_result( _fp_exception_info_t *info,
						new_results_t *result )
{
	switch ( info->destination_format )
	{
	case _TYPE_SIGNED_CHARACTER:
		info->result.c	= result->signed_integer;
		break;

	case _TYPE_UNSIGNED_CHARACTER:
		info->result.uc	= result->unsigned_integer;
		break;

	case _TYPE_SIGNED_SHORT_INTEGER:
		info->result.s	= result->signed_integer;
		break;

	case _TYPE_UNSIGNED_SHORT_INTEGER:
		info->result.us	= result->unsigned_integer;
		break;

	case _TYPE_SIGNED_INTEGER:
		info->result.i	= result->signed_integer;
		break;

	case _TYPE_UNSIGNED_INTEGER:
		info->result.ui	= result->unsigned_integer;
		break;

	case _TYPE_SIGNED_LONG_INTEGER:
		info->result.l	= result->signed_integer;
		break;

	case _TYPE_UNSIGNED_LONG_INTEGER:
		info->result.ul	= result->unsigned_integer;
		break;

	case _TYPE_FLOAT:
		info->result.f	= result->floating_point;
		break;

#if ! _SINGLE_FP
	case _TYPE_DOUBLE:
		info->result.d	= result->floating_point;
		break;
#endif
	}
}

/*
 * Exception handler to deal with IEEE-754 specified type exceptions in
 * detail.
 *
 * Only a few cases are examined in detail as examples.
*/
static void	IEEE_fp_trap_handler( _fp_exception_info_t *info )
{
	int		i;
	int		max_nr;
	new_results_t	*rp;

	/* As example a table with all preferred result values */
	/* (a full 3-dimensional matrix may be necessary instead). */
	/* note that multiple exceptions may arise simultaneously */
	/* search first match in new result value table though */
	max_nr	= sizeof(preferred_result) / sizeof(new_results_t);
	for ( i = 0, rp = preferred_result; i < max_nr; ++i, ++rp )
	{
		if ( info->exception & rp->exception )
		{
			if ( info->operation == rp->operation
				||
				rp->operation == 127
			)
			{
				/* found matching entry or wildcard match */
				assign_new_result( info, rp );
				break;
			}
		}
	}
	/* now continue the program with the perhaps new result */
}

void	install_fp_trap_handler( void )
{
	/* turn off catching of all possible floating point exceptions */
	_fp_set_exception_mask( 0 );

	_fp_install_trap_handler( IEEE_fp_trap_handler );

	/* turn back on catching of all possible floating point exceptions */
	_fp_set_exception_mask( EFALL );
}

#ifndef NODEMO
/* ------------------ non reusable part starts here ------------------------ */

#include <stdio.h>

#define	MAXIMUM		DBL_MAX
#define	VERY_LARGE	(DBL_MAX/10)
#define	VERY_SMALL	(DBL_MIN*10)

double	dzero( double *p )
{
	*p	= 0.0;

	return *p;
}

double	d_large_val( double *p )
{
	*p	= VERY_LARGE;

	return *p;
}

void main( void )
{
	double	d	= 0.999;
	int	old_mask;

	old_mask	= _fp_get_exception_mask();
	install_fp_trap_handler( );

	/* examine full IEEE-754 trap handler */
	printf( "\nNow 3 fp exceptions are processed by the IEEE-754 type trap handler\n" );
	printf(   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );

	/* EFDIVZ */
	d	= d_large_val( &d ) / dzero( &d );
	printf( "divide by zero delivered (4.15): %g\n", d );

	/* EFOVFL */
	d	= d_large_val( &d ) * MAXIMUM;
	printf( "overflow when *not* adding delivered (100.0): %g\n", d );

	/* EFOVFL when adding as exceptional exception */
	d	= d_large_val( &d ) * 10 + MAXIMUM;
	printf( "overflow when adding delivered (3.14): %g\n", d );

	/* EFUNFL when adding as exceptional exception */
	d	= VERY_SMALL / d_large_val( &d );
	printf( "underflow delivered (-2.72): %g\n", d );

	_fp_set_exception_mask( old_mask );
	printf( "\nThats all\n" );
}
#endif /* NODEMO */
