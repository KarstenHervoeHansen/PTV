/*
 *	Version : @(#)limits.h	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  limits.h                                               *
**                                                                        *
**  DESCRIPTION :  Definitions of constants of the sizes of integral      *
**                 types                                                  *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef	_LIMITS_H
#define	_LIMITS_H

#define	CHAR_BIT	(8)
#define	SCHAR_MAX	(127)
#define	SCHAR_MIN	(-SCHAR_MAX-1)
#define	UCHAR_MAX	(0xFFU)
#define	CHAR_MIN	(SCHAR_MIN)
#define	CHAR_MAX	(SCHAR_MAX)
#define	SHRT_MAX	(32767)
#define	SHRT_MIN	(-SHRT_MAX-1)
#define	USHRT_MAX	(0xFFFFU)
#define	INT_MAX		(32767)
#define	INT_MIN		(-INT_MAX-1)
#define	UINT_MAX	(0xFFFFU)
#define	LONG_MAX	(2147483647)
#define	LONG_MIN	(-LONG_MAX-1)
#define	ULONG_MAX	(0xFFFFFFFFUL)
#define	MB_LEN_MAX	(1)

#endif  /* _LIMITS_H */
