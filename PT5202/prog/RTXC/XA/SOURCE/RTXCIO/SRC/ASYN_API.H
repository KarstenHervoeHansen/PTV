/****************************************************************************/
/* MODULE:                                                                  */
/*   asyn_api.h - RTXCio Asynchronous header function prototypes.           */
/****************************************************************************/
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _ASYN_API_H_  /* { _ASYN_API_H_ */
#define  _ASYN_API_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*------------------------*/
/* ASYNCHRONOUS I/O       */
/*------------------------*/

#ifdef A_READ_IO  /* { */
IORC  IO_read( CHNLID, BLKNO, BUFFADD, BYTECNT );
#endif  /* } A_READ_IO */

#ifdef A_WRITE_IO  /* { */
IORC  IO_write( CHNLID, BLKNO, BUFFADD, BYTECNT );
#endif  /* } A_WRITE_IO */

#ifdef A_DEV_OPEN_IO  /* { */
IORC  IO_dev_open( DEVID, void * );
#endif  /* } A_DEV_OPEN_IO */

#ifdef A_DEV_CLOSE_IO  /* { */
IORC  IO_dev_close( DEVID, void * );
#endif  /* } A_DEV_CLOSE_IO */

#ifdef A_CONTROL_IO  /* { */
IORC  IO_control( CHNLID, SPFNCID, void * );
#endif  /* } A_CONTROL_IO */

IORC  IO_dev_wait( CHNLID );

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _ASYN_API_H_ */

/* End of the ASYN_API.H module */
