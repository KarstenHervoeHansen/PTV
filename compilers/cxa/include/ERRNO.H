/*
 *	@(#)errno.h	1.2
 */

/**************************************************************************
**                                                                        *
**  FILE        :  errno.h                                                *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes and macros for errors of  *
**                 some library routines                                  *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef	_ERRNO_H
#define _ERRNO_H

extern int errno;

/* errors returned by printf/scanf */
#define	ERR_FORMAT	1	/* illegal format string for printf/scanf   */
#define	ERR_NOFLOAT	2	/* No floating point in this version of     */
				/* printf/scanf                             */
#define	ERR_NOLONG	3	/* No longs in this version of printf/scanf */
#define	ERR_NOPOINT	4	/* No pointers allowed in printf/scanf      */

/* Floating point errors */
#define	ERANGE		5	/* Value out of range                       */
#define	EDOM		6	/* Floating point domain error              */

/* errors returned by file positioning routiness */
#define ERR_POS		7	/* Positioning failure 			    */
#define _NERR		8	/* last error + 1                           */

#endif  /* _ERRNO_H */
