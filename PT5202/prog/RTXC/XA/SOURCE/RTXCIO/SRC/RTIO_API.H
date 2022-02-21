/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_api.h - determine by the defined RTXCio classes the header        */
/*                function prototype header files to be defined.            */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _RTIO_API_H_  /* { _RTIO_API_H_ */
#define  _RTIO_API_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#include "rtio_opt.h"                              /* defined IO_ functions */
#include "rtio_inc.h"                  /* RTXCio - Header file include file */

/*------------------------*/
/* ASYNCHRONOUS I/O       */
/*------------------------*/
#ifdef ASYNCH_IO  /* { */
#include "asyn_api.h"
#endif   /* } ASYNCH_IO */

/*------------------------*/
/* SYNCHRONOUS I/O        */
/*------------------------*/

#ifdef SYNCH_IO  /* { */
#include "sync_api.h"
#endif   /* } SYNCH_IO */

/*---------------------------*/
/* SYNCHRONOUS I/O W/TIMEOUT */
/*---------------------------*/

#ifdef TIMEOUT_IO  /* { */
#include "tout_api.h"
#endif  /* { TIMEOUT_IO */

#if defined( ASYNCH_IO ) || \
    defined( SYNCH_IO ) || \
    defined( TIMEOUT_IO )  /* { */
#include "comn_api.h"
#endif /* } ASYNCH_IO | SYNCH_IO | TIMEOUT_IO */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTIO_API_H_ */

/* End of the RTIO_API.H module */
