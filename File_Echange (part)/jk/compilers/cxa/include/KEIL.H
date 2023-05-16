/*
 *	@(#)keil.h	1.4
 */

/**************************************************************************
**                                                                        *
**  FILE        :  keil.h                                                 *
**                                                                        *
**  DESCRIPTION :  Include file with macros for migration from            *
**                 Keil, Franklin or Archimedes 8051 C-compiler to        *
**                 the Tasking C-XA compiler                              *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef KEIL_INC

#ifndef _CXA_INC
#include <cxa.h>        /* macro predefinitions for C-XA compiler */
#endif  /* _CXA_INC */

	/* memory type keywords 	*/
#define data		_data
#define bdata		_bdat
#define idata		_idat
#define pdata		_pdat
#define xdata		_xdat
#define code		_rom

	/* function qualifier keyword 	*/
#define alien		_asmfunc

	/* variable type keyword 	*/
#define bit		_bit

	/* names inline functions 	*/
#define _crol_		_rol
#define _cror_		_ror
#define _testbit_	_testclear
#define _nop_		_nop

#define	KEIL_INC
#endif
