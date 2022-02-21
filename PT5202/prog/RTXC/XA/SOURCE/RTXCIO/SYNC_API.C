/****************************************************************************/
/* MODULE:                                                                  */
/*   sync_api.c - synchronous input/output application interrface.          */
/****************************************************************************/
/* FUNCTIONS:                                                               */

#ifdef SYNCH_IO  /* { */
/*   IORC IO_readw( CHNLID, BLKNO, BUFFADD, BYTECNT )                       */
/*   IORC IO_writew( CHNLID, BLKNO, BUFFADD, BYTECNT )                      */
/*   IORC IO_dev_openw( DEVID, void * )                                     */
/*   IORC IO_dev_closew( DEVID, void * )                                    */
/*   IORC IO_controlw( CHNLID, SPFNCID, void * )                            */
#endif  /* } SYNCH_IO */
/*                                                                          */
/****************************************************************************/
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#include "rtxcapi.h"      /* RTXC - kernel types, declarations & prototypes */

#include "rtio_inc.h"                  /* RTXCio - Header file include file */

extern const CHNLID  nchnls;        /* RTXCio - number of channels declared */
extern const DEVID  nldevs;           /* RTXCio - number of devices declared */
extern CCB_ENTRY   channel_table[];          /* channel control block table */

extern IORC  process_io( IOPACK * );

/*==========================================================================*/
/*                      RTXCio API FUNCTIONS                                */
/*==========================================================================*/
#ifdef SYNCH_IO   /* { */

#ifdef S_READ_IO   /* { */

IORC   IO_readw( CHNLID     chnlid,
		     BLKNO      startblk,
		     BUFFADD    transfer_buff,
		     BYTECNT    numbytes )
{
	/*------------------------------------------------------------------*/
	/* Declare on the task's stack a structure to send I/O request in.  */
	/*------------------------------------------------------------------*/
	struct io_package iopack;

	/*------------------------------------------------------------------*/
	/* Check for a valid chnlid # and If the channel is inactive,       */
	/* return an invalid channel error                                  */
	/*------------------------------------------------------------------*/
	if( (chnlid == 0) || (chnlid > nchnls) ||
	    (channel_table[chnlid].blink == NULLCCBE) )
		  return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	/*------------------------------------------------------------------*/
	/* Verify the requested device is setup for reading.                */
	/*------------------------------------------------------------------*/
	if( channel_table[chnlid].io_command == WRITE_IO )
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEVICE_INVALID) );

	/*------------------------------------------------------------------*/
	/* Package the I/O argument to send to the physical controller.     */
	/*------------------------------------------------------------------*/
	iopack.iofncid = IO_READ;
	iopack.chnlid = chnlid;
	iopack.deviceno = channel_table[chnlid].io_deviceno;
	iopack.io_duration = IO_WAIT;
	iopack.startblk = startblk;
	iopack.transfer_buff = transfer_buff;
	iopack.numbytes = numbytes;
	iopack.otherinfo = NULLIOINFO;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } S_READ_IO */

/*==========================================================================*/
#ifdef S_WRITE_IO  /* { */

IORC   IO_writew( CHNLID     chnlid,
		      BLKNO      startblk,
		      BUFFADD    transfer_buff,
		      BYTECNT    numbytes )
{
	/*------------------------------------------------------------------*/
	/* Declare on the task's stack a structure to send I/O request in.  */
	/*------------------------------------------------------------------*/
	struct io_package iopack;

	/*------------------------------------------------------------------*/
	/* Check for a valid chnlid # and If the channel is inactive,       */
	/* return an invalid channel error                                  */
	/*------------------------------------------------------------------*/
	if( (chnlid == 0) || (chnlid > nchnls) ||
	    (channel_table[chnlid].blink == NULLCCBE) )
		  return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	/*------------------------------------------------------------------*/
	/* Verify the requested device is setup for writing.                */
	/*------------------------------------------------------------------*/
	if( channel_table[chnlid].io_command == READ_IO )
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEVICE_INVALID) );

	/*------------------------------------------------------------------*/
	/* Package the I/O argument to send to the physical controller.     */
	/*------------------------------------------------------------------*/
	iopack.iofncid = IO_WRITE;
	iopack.chnlid = chnlid;
	iopack.deviceno = channel_table[chnlid].io_deviceno;
	iopack.io_duration = IO_WAIT;
	iopack.startblk = startblk;
	iopack.transfer_buff = transfer_buff;
	iopack.numbytes = numbytes;
	iopack.otherinfo = NULLIOINFO;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } S_WRITE_IO */

/*==========================================================================*/
#ifdef S_DEV_OPEN_IO   /* { */

IORC   IO_dev_openw( DEVID   deviceno,
			 void *  otherinfo  )
{
	/*------------------------------------------------------------------*/
	/* Declare on the task's stack a structure to send I/O request in.  */
	/*------------------------------------------------------------------*/
	struct io_package iopack;

	/*------------------------------------------------------------------*/
	/* If the device is not valid then return in error.                 */
	/*------------------------------------------------------------------*/
	if( ( deviceno <= 0 ) || ( deviceno > nldevs ) )
	     return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEVICE_INVALID) );

	/*------------------------------------------------------------------*/
	/* Package the I/O argument to send to the physical controller.     */
	/*------------------------------------------------------------------*/
	iopack.iofncid = IO_DEV_OPEN;
	iopack.deviceno = deviceno;
	iopack.io_duration = IO_WAIT;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } S_DEV_OPEN_IO */

/*==========================================================================*/
#ifdef S_DEV_CLOSE_IO  /* { */


IORC   IO_dev_closew( DEVID   deviceno,
			  void *  otherinfo )
{
	/*------------------------------------------------------------------*/
	/* Declare on the task's stack a structure to send I/O request in.  */
	/*------------------------------------------------------------------*/
	struct io_package iopack;

	/*------------------------------------------------------------------*/
	/* If the device is not valid then return in error.                 */
	/*------------------------------------------------------------------*/
	if( ( deviceno <= 0 ) || ( deviceno > nldevs ) )
	    return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEVICE_INVALID) );

	/*------------------------------------------------------------------*/
	/* Package the I/O argument to send to the physical controller.     */
	/*------------------------------------------------------------------*/
	iopack.iofncid = IO_DEV_CLOSE;
	iopack.deviceno = deviceno;
	iopack.io_duration = IO_WAIT;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}

#endif   /* } S_DEV_CLOSE_IO */

/*==========================================================================*/
#ifdef S_CONTROL_IO   /* { */

IORC  IO_controlw( CHNLID      chnlid,
		       SPFNCID     spfncid,
		       void *      otherinfo )
{
	/*------------------------------------------------------------------*/
	/* Declare on the task's stack a structure to send I/O request in.  */
	/*------------------------------------------------------------------*/
	struct io_package iopack;

	/*------------------------------------------------------------------*/
	/* Check for a valid chnlid # and If the channel is inactive,       */
	/* return an invalid channel error                                  */
	/*------------------------------------------------------------------*/
	if( (chnlid == 0) || (chnlid > nchnls) ||
	    (channel_table[chnlid].blink == NULLCCBE) )
		  return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	/*------------------------------------------------------------------*/
	/* Package the I/O argument to send to the physical controller.     */
	/*------------------------------------------------------------------*/
	iopack.iofncid = IO_CONTROL;
	iopack.chnlid = chnlid;
	iopack.deviceno = channel_table[chnlid].io_deviceno;
	iopack.io_duration = IO_WAIT;
	iopack.spfncid = spfncid;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } S_CONTROL_IO */

#endif   /* } SYNCH_IO */

/* End of the SYNC_API.C module */
