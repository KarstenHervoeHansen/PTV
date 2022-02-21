/****************************************************************************/
/* MODULE:                                                                  */
/*   tout_opt.h - RTXCio Timeout function selection.                        */
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
 *   This header file allows you to select specifically which RTXCio
 *   timed synchronous and asynchronous functions your application requires.
 *   Leaving the unneeded functions out of your compilation process.
*/
/****************************************************************************/
#ifndef _TOUT_OPT_H_  /* { _TOUT_OPT_H_ */
#define  _TOUT_OPT_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/* physical read */
#define T_READ_IO

/* physical write */
#define T_WRITE_IO

/* device initialization such as setting vectors */
#define T_DEV_OPEN_IO

/* device uninitialization such as resetting system vectors */
#define T_DEV_CLOSE_IO

/* Special control functions such as HEAD_RETRACT, WRITE_DCB, READ_DCB */
#define T_CONTROL_IO

/* The asynchronous call to resolve a i/o request with a timeout */
#define T_DEV_WAITT

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _TOUT_OPT_H_ */

/* End of the TOUT_OPT.H module */
