/****************************************************************************/
/* MODULE:                                                                  */
/*   asyn_opt.h - RTXCio Asynchronous function selection.                   */
/****************************************************************************/
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
/*
 * DESCRIPTION:
 *   This header file allows you to select specifically which RTXCio asynchronous
 *   functions your application requires. Leaving the unneeded functions
 *   out of your compilation process.
*/
/****************************************************************************/
#ifndef _ASYN_OPT_H_  /* { _ASYN_OPT_H_ */
#define  _ASYN_OPT_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/* physical read */
#define A_READ_IO

/* physical write */
#define A_WRITE_IO

/* device initialization such as setting vectors */
#define A_DEV_OPEN_IO

/* device uninitialization such as resetting system vectors */
#define A_DEV_CLOSE_IO

/* Special control functions such as HEAD_RETRACT, WRITE_DCB, READ_DCB */
#define A_CONTROL_IO

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _ASYN_OPT_H_ */

/* End of the ASYN_OPT.H module */
