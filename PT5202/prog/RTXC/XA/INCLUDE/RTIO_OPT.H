/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_opt.h - RTXCio function selection.                                */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
/*
 * DESCRIPTION:
 *   This header file allows you to select specifically which RTXCio packages
 *   your application requires. Leaving the unneeded packages out of your
 *   compilation process.
*/
/****************************************************************************/
#ifndef _RTIO_OPT_H_  /* { _RTIO_OPT_H_ */
#define  _RTIO_OPT_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*==========================================================================*/
/*                   DEFINE OR UNDEFINE ASYNCHRONOUS I/O                    */
/*==========================================================================*/
#define ASYNCH_IO

#ifdef ASYNCH_IO /* { */
#include "asyn_opt.h"
#endif    /* } ASYNCH_IO */

/*==========================================================================*/
/*                      DEFINE OR UNDEFINE SYNCHRONOUS I/O                  */
/*==========================================================================*/
#define SYNCH_IO

#ifdef SYNCH_IO /* { */
#include "sync_opt.h"
#endif    /* } SYNCH_IO */

/*==========================================================================*/
/*           DEFINE OR UNDEFINE SYNCHRONOUS I/O WITH TIMED WAIT             */
/*==========================================================================*/
#define TIMEOUT_IO

#ifdef TIMEOUT_IO  /* { */
#include "tout_opt.h"
#endif    /* } TIMEOUT_IO */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTIO_OPT_H_ */

/* End of the RTIO_OPT.H module */
