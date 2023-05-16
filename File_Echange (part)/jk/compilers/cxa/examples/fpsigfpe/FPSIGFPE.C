/******************************************************************************
|*
|*  VERSION CONTROL:    @(#)fpsigfpe.c	1.5 12/17/96
|*
|*  IN PACKAGE:         FPRT (Floating Point Run-Time C expression support)
|*
|*  COPYRIGHT:          1996 Copyright TASKING, Inc.
|*
|* Example program for installing a floating point exception handler
|* which causes SIGFPE to be send to the ANSI-C signal handler.
|*
 *****************************************************************************/

#include <float.h>
#include <signal.h>

/*
 * F.P. exception handler to pass any floating point exception straight
 * to the ANSI-C signal handler, ignoring any return value from it.
*/
static void	pass_fp_exception_to_signal( _fp_exception_info_t *info )
{
	info;	/* suppress parameter not used warning */

	/* cause a signal to be raised */
	raise( SIGFPE );

	/* now continue the program with the unaltered result */
}

void	install_SIGFPE_signal_raiser( void )
{
	/* turn off catching of any possible floating point exceptions */
	_fp_set_exception_mask( 0 );

	/********************************************************
	 * have floating point exceptions cause a SIGFPE signal *
	 *******************************************************/
	_fp_install_trap_handler( pass_fp_exception_to_signal );
	/* now turn on catching of all possible floating point exceptions */
	_fp_set_exception_mask( EFALL );
}

#ifndef NODEMO
/* ------------------ non reusable part starts here ------------------------ */

#include <stdio.h>
#include <setjmp.h>

static jmp_buf	context;

float	fzero( float *p )
{
	*p	= 0.0;

	return *p;
}

float	fval( float *p )
{
	*p	= 1.234;

	return *p;
}

void	local_signal_handler( int nope )
{
	nope;	/* suppress parameter not used warning */

	/* have to reinstall our signal handler */
	if ( signal( SIGFPE, local_signal_handler ) == SIG_ERR )
	{
		printf( "Installation of signal handler failed\n" );
	}

	longjmp( context, 1 );
}

void main( void )
{
	float	f	= 0.999;

	/* temporarily turn off SIGFPE while changing its handling */
	if ( signal( SIGFPE, SIG_IGN ) == SIG_ERR )
	{
		printf( "Installation of signal handler failed\n" );
		goto quit_now;
	}

	install_SIGFPE_signal_raiser( );

	if ( signal( SIGFPE, local_signal_handler ) == SIG_ERR )
	{
		printf( "Installation of signal handler failed\n" );
		goto quit_now;
	}

	/* turn back on catching of all possible floating point exceptions */
	_fp_set_exception_mask( EFALL );
	printf( "Now catch 3 fp exceptions with the ANSI-C signal handler\n" );
	printf( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
	/* EFDIVZ */
	if ( ! setjmp( context ) )
	{
		f	= fval( &f ) / fzero( &f );
		printf( "divide by zero not caught!\n" );
	}
	else
	{
		printf( "Caught divide by zero.\n" );
	}

	/* EFOVFL */
	if ( ! setjmp( context ) )
	{
		f	= fval( &f ) * FLT_MAX;
		printf( "overflow not caught!\n" );
	}
	else
	{
		printf( "Caught overflow.\n" );
	}

	/* EFOVFL when adding as exceptional exception */
	if ( ! setjmp( context ) )
	{
		f	= fval( &f ) - FLT_MAX - FLT_MAX;
		printf( "underflow not caught!\n" );
	}
	else
	{
		printf( "Caught underflow.\n" );
	}

quit_now:
	printf( "\nThats all\n" );
}
#endif /* NODEMO */
