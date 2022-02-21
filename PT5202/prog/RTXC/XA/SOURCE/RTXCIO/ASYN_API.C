/****************************************************************************/
/* MODULE:                                                                  */
/*   asyn_api.c - Asynchronous input/output application interface.          */
/****************************************************************************/
/* FUNCTIONS:                                                               */

#ifdef ASYNCH_IO  /* { */
/*   IORC IO_read( CHNLID, BLKNO, BUFFADD, BYTECNT )                        */
/*   IORC IO_write( CHNLID, BLKNO, BUFFADD, BYTECNT )                       */
/*   IORC IO_dev_open( DEVID, void * )                                      */
/*   IORC IO_dev_close( DEVID, void * )                                     */
/*   IORC IO_control( CHNLID, SPFNCID, void * )                             */
/*   IORC IO_dev_wait( CHNLID )                                             */
#endif /* } ASYNCH_IO */
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

extern const CHNLID  nchnls;         /* RTXCio - number of channels declared */
extern const DEVID  nldevs;           /* RTXCio - number of devices declared */
extern CCB_ENTRY   channel_table[];          /* channel control block table */
extern PDU_HEADER * logical_dev_tbl[];  /* logical to physical device table */

extern IORC process_io( IOPACK * );

/*==========================================================================*/
/*                      RTXCio API FUNCTIONS                                */
/*==========================================================================*/
#ifdef ASYNCH_IO   /* { */

#ifdef A_READ_IO   /* { */

IORC   IO_read(  CHNLID     chnlid,
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
	iopack.io_duration = IO_NOWAIT;
	iopack.startblk = startblk;
	iopack.transfer_buff = transfer_buff;
	iopack.numbytes = numbytes;
	iopack.otherinfo = NULLIOINFO;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } A_READ_IO */

/*==========================================================================*/
#ifdef A_WRITE_IO   /* { */

IORC   IO_write( CHNLID     chnlid,
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
	iopack.io_duration = IO_NOWAIT;
	iopack.startblk = startblk;
	iopack.transfer_buff = transfer_buff;
	iopack.numbytes = numbytes;
	iopack.otherinfo = NULLIOINFO;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif  /* } A_WRITE_IO */

/*==========================================================================*/
#ifdef A_DEV_OPEN_IO   /* { */

IORC   IO_dev_open( DEVID  deviceno,
			void * otherinfo  )
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
	iopack.io_duration = IO_NOWAIT;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } A_DEV_OPEN_IO */

/*==========================================================================*/
#ifdef A_DEV_CLOSE_IO  /* { */

IORC   IO_dev_close( DEVID  deviceno,
			 void * otherinfo )
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
	iopack.io_duration = IO_NOWAIT;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}

#endif   /* } A_DEV_CLOSE_IO */

/*==========================================================================*/
#ifdef A_CONTROL_IO   /* { */

IORC  IO_control( CHNLID      chnlid,
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
	iopack.io_duration = IO_NOWAIT;
	iopack.spfncid = spfncid;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } A_CONTROL_IO */

/*==========================================================================*/
IORC  IO_dev_wait( CHNLID chnlid )
{
	DEVID   deviceno;
	 KSRC   ret_val;
	 IORC   ret_code;

	/*------------------------------------------------------------------*/
	/* Check for a valid chnlid # and If the channel is inactive,       */
	/* return an invalid channel error                                  */
	/*------------------------------------------------------------------*/
	if( (chnlid == 0) || (chnlid > nchnls) ||
	    (channel_table[chnlid].blink == NULLCCBE) )
		  return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	deviceno = channel_table[chnlid].io_deviceno;

	/*------------------------------------------------------------------*/
	/* Determine whether the requesting task is the device owner.       */
	/*------------------------------------------------------------------*/
	ret_val = KS_lock( logical_dev_tbl[deviceno]->dev_res );

	if( ret_val == RC_BUSY )
	{
		/*----------------------------------------------------------*/
		/* error: the requesting task is not the owner              */
		/*----------------------------------------------------------*/
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEV_BUSY) );
	}

	if( ret_val == RC_GOOD )
	{
		/*----------------------------------------------------------*/
		/* error: the requesting task has no pending RTXCio calls   */
		/*----------------------------------------------------------*/
		KS_unlock( logical_dev_tbl[deviceno]->dev_res );
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_BAD_WAIT) );
	}

	/*------------------------------------------------------------------*/
	/* The requesting task is indeed the owner so unlock                */
	/* the nested lock                                                  */
	/*------------------------------------------------------------------*/
	KS_unlock( logical_dev_tbl[deviceno]->dev_res );

	/*------------------------------------------------------------------*/
	/* Verify completion of your request on the device.                 */
	/*------------------------------------------------------------------*/
	KS_wait(logical_dev_tbl[deviceno]->dev_sema);

	/*------------------------------------------------------------------*/
	/* return successful results.                                       */
	/*------------------------------------------------------------------*/
	if( (logical_dev_tbl[deviceno]->last_io_oper == IO_READ) ||
	    (logical_dev_tbl[deviceno]->last_io_oper == IO_WRITE)    )
	{
		if( logical_dev_tbl[deviceno]->lost_data )
		{
		    ret_code = (IORC)SETERROR + ( (IORC)SETCLASS *
			 (IORC)logical_dev_tbl[deviceno]->dev_cntlr->cntlr_number) +
			 ((IORC) SETDEVICE * (IORC)deviceno) +
			 (IORC)logical_dev_tbl[deviceno]->lost_data;
		    logical_dev_tbl[deviceno]->lost_data = NO_LOST_DATA;
		}
		else
		    ret_code = (IORC)logical_dev_tbl[deviceno]->bytes_transfered;
	}
	else
		ret_code = (IORC)RTXCIO_GOOD + (IORC)IO_DEV_FREED;

	/*------------------------------------------------------------------*/
	/* Unlock the device's resource to free the device.                 */
	/*------------------------------------------------------------------*/
	KS_unlock(logical_dev_tbl[deviceno]->dev_res);
	return( ret_code );
}

#endif   /* } ASYNCH_IO */

/* End of the ASYN_API.C module */
