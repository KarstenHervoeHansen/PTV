/****************************************************************************/
/* MODULE:                                                                  */
/*   tout_api.h - RTXCio Timeout header function prototypes.                */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _TOUT_API_H_  /* { _TOUT_API_H_ */
#define  _TOUT_API_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*---------------------------*/
/* SYNCHRONOUS I/O W/TIMEOUT */
/*---------------------------*/

#ifdef T_READ_IO  /* { */
IORC  IO_readt( CHNLID, BLKNO, BUFFADD, BYTECNT, TICKS );
#endif  /* } T_READ_IO */

#ifdef T_WRITE_IO  /* { */
IORC  IO_writet( CHNLID, BLKNO, BUFFADD, BYTECNT, TICKS );
#endif  /* } T_WRITE_IO */

#ifdef T_DEV_OPEN_IO  /* { */
IORC  IO_dev_opent( DEVID, void *, TICKS );
#endif  /* } T_DEV_OPEN_IO */

#ifdef T_DEV_CLOSE_IO  /* { */
IORC  IO_dev_closet( DEVID, void *, TICKS );
#endif   /* } T_DEV_CLOSE_IO */

#ifdef T_CONTROL_IO  /* { */
IORC  IO_controlt( CHNLID, SPFNCID, void *, TICKS );
#endif  /* } T_CONTROL_IO */

/* The asynchronous call to resolve a i/o request with a timeout */
#ifdef ASYNCH_IO  /* { */
#ifdef T_DEV_WAITT
IORC  IO_dev_waitt( CHNLID, TICKS );
IORC  IO_chnl_reset( CHNLID );
#endif   /* } TIME_SYNCH_IO */
#endif   /* } ASYNCH_IO */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _TOUT_API_H_ */

/* End of the TOUT_API.H module */
