/*
 *	@(#)c51.h	1.5
 */

/**************************************************************************
**                                                                        *
**  FILE        :  c51.h (C-51 compatible C compiler)                     *
**                                                                        *
**  DESCRIPTION :  Include file with macros to enable or disable the      *
**                 usage of Tasking C-XA language extensions.             *
**                 cxa has the following predefined preprocessor          *
**                 symbols:                                               *
**                           _CXA      value = version of compiler        *
**                                     (e.g. value 10 for v1.0)           *
**                           _MODEL    memory model used:                 *
**                                     tiny:        value is 't'          *
**                                     small:       value is 's'          *
**                                     medium:      value is 'm'          *
**                                     large:       value is 'l'          *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef C51_INC

#ifndef _CXA_INC
#include <cxa.h>        /* macro predefinitions for C-XA compiler */
#endif  /* _CXA_INC */

#ifdef _CXA
/*
 * When using the Tasking C-XA compiler, recognize language extensions.
 * For the exceptions define the memory type specifiers away.
 */
#define	plmprocedure
#define	_plmprocedure
#define	_regparm
#define	_cdecl

/*
 * The following functions are built-in functions of the CC51 compiler.
 * A macro is given here, because _getbit and _putbit become superfluous.
 * Instead of _getbit and _putbit the _atbit should be used.
 *
 *	e.g.
 *		_bitbyte bb;
 *		_bit	 b _atbit( bb, 1 );
 *		_bit 	 x;
 *		...
 *		b = 1;	// _putbit( 1, bb, 1 );
 *		x = b;	// _getbit( bb, 1 );
 */
#define _getbit( bb, bitnbr ) 		((_bit)(bb >> bitnbr ))
#define _putbit( bitval, bb, bitnbr ) 	(bb = (bb & ~(1 << bitnbr)) | (bitval << bitnbr))

#else  /* _CXA */
/* 
 * When not using the Tasking C-XA compiler, define the memory type
 * specifiers away. There is no need to remove const.
 */

/*
 * Old-style C-51 language extensions did not have a leading underscore,
 * still accepted by this generation of C compilers for compatability.
 */
#define	data
#define	idat
#define pdat
#define	xdat
#define	rom
#define	plmprocedure
#define builtin
#define	using( X )
#define	interrupt( X )

#define	_plmprocedure

#define	bit		unsigned char
#define	sfrbit		unsigned char
#define	sfrbyte		unsigned char

#define	_small
#define	_aux
#define	_large
#define	_reentrant
#define	_cdecl
#define	_regparm

/*
 * The following functions are built-in functions of the CC51 compiler.
 * A prototype is given here, so other compilers will recognize
 * the function names and types. The user should make alternatives
 * for these functions to prototype the application on another host.
 */
bit		_getbit( _bitbyte, unsigned char );
void		_putbit( bit, _bitbyte, unsigned char );

#endif

#define	C51_INC
#endif
