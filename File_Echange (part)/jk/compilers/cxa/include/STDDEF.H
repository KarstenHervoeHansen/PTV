/*
 *	@(#)stddef.h	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  stddef.h                                               *
**                                                                        *
**  DESCRIPTION :  Include file with macros for common use                *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef	_SIZE_T_DEF
typedef unsigned int    size_t;
#define	_SIZE_T_DEF
#endif

#ifndef _WCHAR_T
typedef unsigned short wchar_t;
#define _WCHAR_T
#endif

typedef	int		ptrdiff_t;

#define	NULL	0
#define offsetof( _type, _member )	((size_t)(&((_type *)0)->_member))

#endif  /* _STDDEF_H */
