/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_nul.h - RTXCio null types                                         */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _RTIO_NUL_H_  /* { _RTIO_NUL_H_ */
#define  _RTIO_NUL_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#define NULLCCBE    ((CCB_ENTRY *)0)                  /* null Channel entry */
#define NULLFUNC    (void (*)(void))0              /* NULL FUNCTION pointer */
#define NULLIOINFO  ((void *)0)        /* null pointer for iopack.otherinfo */
#define NULLBLOCK   ((BLKNO)0)    /* used when no block parameter is needed */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTIO_NUL_H_ */

/* End of the RTIO_NUL.H module */
