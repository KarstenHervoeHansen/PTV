/****************************************************************************/
/* MODULE:                                                                  */
/*   sync_opt.h - RTXCio Synchronous function selection.                    */
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
 *   This header file allows you to select specifically which RTXCio synchronous
 *   functions your application requires. Leaving the unneeded functions
 *   out of your compilation process.
*/
/****************************************************************************/
#ifndef _SYNC_OPT_H_  /* { _SYNC_OPT_H_ */
#define  _SYNC_OPT_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/* physical read */
#define S_READ_IO

/* physical write */
#define S_WRITE_IO

/* device initialization such as setting vectors */
#define S_DEV_OPEN_IO

/* device uninitialization such as resetting system vectors */
#define S_DEV_CLOSE_IO

/* Special control functions such as HEAD_RETRACT, WRITE_DCB, READ_DCB */
#define S_CONTROL_IO

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _SYNC_OPT_H_ */

/* End of the SYNC_OPT.H module */
