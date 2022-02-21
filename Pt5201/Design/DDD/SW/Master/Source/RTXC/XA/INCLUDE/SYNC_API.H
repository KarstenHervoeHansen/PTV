/****************************************************************************/
/* MODULE:                                                                  */
/*   sync_api.h - RTXCio Synchronous header function prototypes.            */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _SYNC_API_H_  /* { _SYNC_API_H_ */
#define  _SYNC_API_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*------------------------*/
/* SYNCHRONOUS I/O        */
/*------------------------*/

#ifdef S_READ_IO  /* { */
IORC  IO_readw( CHNLID, BLKNO, BUFFADD, BYTECNT );
#endif  /* } S_READ_IO */

#ifdef S_WRITE_IO  /* { */
IORC  IO_writew( CHNLID, BLKNO, BUFFADD, BYTECNT );
#endif  /* } S_WRITE_IO */

#ifdef S_DEV_OPEN_IO  /* { */
IORC  IO_dev_openw( DEVID, void * );
#endif  /* } S_DEV_OPEN_IO */

#ifdef S_DEV_CLOSE_IO  /* { */
IORC  IO_dev_closew( DEVID, void * );
#endif  /* } S_DEV_CLOSE_IO */

#ifdef S_CONTROL_IO  /* { */
IORC  IO_controlw( CHNLID, SPFNCID, void * );
#endif  /* } S_CONTROL_IO */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _SYNC_API_H_ */

/* End of the SYNC_API.H module */
