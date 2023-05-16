/*
 *	Version : @(#)setjmp.h	1.2
 */

/**************************************************************************
**                                                                        *
**  FILE        :  setjmp.h                                               *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes for non-local jump        *
**                 functions                                              *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef _SETJMP_H
#define _SETJMP_H

#define BUF_SIZ 3

typedef int jmp_buf[BUF_SIZ];

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int setjmp	( jmp_buf );
void longjmp	( jmp_buf, int );
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* _SETJMP_H */
