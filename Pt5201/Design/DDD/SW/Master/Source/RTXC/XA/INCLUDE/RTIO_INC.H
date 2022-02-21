/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_inc.h - RTXCio header include file                                */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
/*
 *  DESCRIPTION:
 *      This header include file is supplied for the sake of cleanliness.
 *      The writer of the driver functions does not need to know or worry
 *      about all the #include files.  The writer needs only to include
 *      this file.
*/
/****************************************************************************/
#ifndef _RTIO_INC_H_  /* { _RTIO_INC_H_ */
#define  _RTIO_INC_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#include "rtio_tdf.h"                         /* RTXCio - type declarations */
#include "rtcl_tdf.h" /* Controller & device independent- type declarations */

#include "rtio_str.h"                                /* RTXCio - structures */
#include "rtcl_str.h"       /* Controller & device independent - structures */

#include "rtio_nul.h"                                /* RTXCio - NULL types */
#include "rtcl_nul.h"       /* Controller & device independent - NULL types */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTIO_INC_H_ */

/* End of the RTIO_INC.H module */
