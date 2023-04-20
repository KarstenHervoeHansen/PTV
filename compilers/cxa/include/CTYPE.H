/*
 *	@(#)ctype.h	1.2
 */

/**************************************************************************
**                                                                        *
**  FILE        :  ctype.h                                                *
**                                                                        *
**  DESCRIPTION :  Include file with macros and definitions for functions *
**                 testing characters                                     *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/


#ifndef	__U
#define	__U	01
#define	__L	02
#define	__N	04
#define	__S	010
#define __P	020
#define __C	040
#define	__X	0100
#define	__W	0200
#endif

#ifndef _CTYPE_H
#define _CTYPE_H

#define _toupper(_c)	((_c)-'a'+'A')
#define _tolower(_c)	((_c)-'A'+'a')

extern	const char	_ctype_[];

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int	isalpha( int );
int	isupper( int );
int	islower( int );
int	isdigit( int );
int	isxdigit( int );
int	isspace( int );
int	ispunct( int );
int	isalnum( int );
int	isprint( int );
int	isgraph( int );
int	iscntrl( int );
int	isascii( int );
int	toascii( int );
int	toupper( int );
int	tolower( int );
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define	isalpha(_c)	((_ctype_+1)[_c]&(__U|__L))
#define	isupper(_c)	((_ctype_+1)[_c]&__U)
#define	islower(_c)	((_ctype_+1)[_c]&__L)
#define	isdigit(_c)	((_ctype_+1)[_c]&__N)
#define	isxdigit(_c)	((_ctype_+1)[_c]&(__N|__X))
#define	isspace(_c)	((_ctype_+1)[_c]&(__S|__W))
#define ispunct(_c)	((_ctype_+1)[_c]&__P)
#define isalnum(_c)	((_ctype_+1)[_c]&(__U|__L|__N))
#define isprint(_c)	((_ctype_+1)[_c]&(__P|__U|__L|__N|__S))
#define	isgraph(_c)	((_ctype_+1)[_c]&(__P|__U|__L|__N))
#define iscntrl(_c)	((_ctype_+1)[_c]&(__C|__W))
#define isascii(_c)	((unsigned)(_c)<=0177)
#define toascii(_c)	((_c)&0177)

#endif  /* _CTYPE_H */
