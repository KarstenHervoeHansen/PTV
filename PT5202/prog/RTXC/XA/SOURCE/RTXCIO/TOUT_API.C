/****************************************************************************/
/* MODULE:                                                                  */
/*   tout_api.c - Timeout input/output application interrface.              */
/****************************************************************************/
/* FUNCTIONS:                                                               */

#ifdef TIMEOUT_IO   /* { */
/*   IORC IO_readt( CHNLID, BLKNO, BUFFADD, BYTECNT, TICKS )                */
/*   IORC IO_writet( CHNLID, BLKNO, BUFFADD, BYTECNT, TICKS )               */
/*   IORC IO_dev_opent( DEVID, void *, TICKS )                              */
/*   IORC IO_dev_closet( DEVID, void *, TICKS )                             */
/*   IORC IO_controlt( CHNLID, SPFNCID, void *, TICKS )                     */
#ifdef ASYNCH_IO  /* { */
/*   IORC IO_dev_waitt( CHNLID, TICKS )                                     */
/*   IORC IO_chnl_reset( CHNLID )                                           */
#endif /* } ASYNCH_IO */
#endif  /* } TIMEOUT_IO */

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

extern IORC  process_io( IOPACK * );

/*==========================================================================*/
/*                      RTXCio API FUNCTIONS                                */
/*==========================================================================*/
#ifdef  TIMEOUT_IO  /* { */

#ifdef  T_READ_IO  /* { */

IORC   IO_readt( CHNLID     chnlid,
		     BLKNO      startblk,
		     BUFFADD    transfer_buff,
		     BYTECNT    numbytes,
		     TICKS      waitime )
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
	iopack.io_duration = IO_TIMED;
	iopack.waitime = waitime;
	iopack.startblk = startblk;
	iopack.transfer_buff = transfer_buff;
	iopack.numbytes = numbytes;
	iopack.otherinfo = NULLIOINFO;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}
#endif   /* } T_READ_IO */

/*==========================================================================*/
#ifdef T_WRITE_IO  /* { */

IORC   IO_writet( CHNLID     chnlid,
		      BLKNO      startblk,
		      BUFFADD    transfer_buff,
		      BYTECNT    numbytes,
		      TICKS      waitime )
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
	iopack.io_duration = IO_TIMED;
	iopack.waitime = waitime;
	iopack.startblk = startblk;
	iopack.transfer_buff = transfer_buff;
	iopack.numbytes = numbytes;
	iopack.otherinfo = NULLIOINFO;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}

#endif   /* } T_WRITE_IO */

/*==========================================================================*/
#ifdef  T_DEV_OPEN_IO   /* { */

IORC   IO_dev_opent( DEVID   deviceno,
			 void *  otherinfo,
			 TICKS   waitime )
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
	iopack.io_duration = IO_TIMED;
	iopack.waitime = waitime;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}

#endif   /* } T_DEV_OPEN_IO */

/*==========================================================================*/
#ifdef  T_DEV_CLOSE_IO  /* { */

IORC   IO_dev_closet( DEVID   deviceno,
			  void *  otherinfo,
			  TICKS   waitime )
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
	iopack.io_duration = IO_TIMED;
	iopack.waitime = waitime;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}

#endif   /* } T_DEV_CLOSE_IO */

/*==========================================================================*/
#ifdef T_CONTROL_IO  /* { */

IORC  IO_controlt( CHNLID      chnlid,
		       SPFNCID     spfncid,
		       void *      otherinfo,
		       TICKS       waitime )
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
	iopack.io_duration = IO_TIMED;
	iopack.waitime = waitime;
	iopack.spfncid = spfncid;
	iopack.otherinfo = otherinfo;

	/*------------------------------------------------------------------*/
	/* Process the i/o request.                                         */
	/*------------------------------------------------------------------*/
	return( process_io( &iopack ) );
}

#endif   /* } T_CONTROL_IO */

#ifdef ASYNCH_IO     /* { */
#ifdef T_DEV_WAITT   /* { */
/*==========================================================================*/
IORC  IO_dev_waitt( CHNLID chnlid,
			TICKS wait_time  )
{
	DEVID   deviceno;
	 KSRC   ret_val;

	/*------------------------------------------------------------------*/
	/* Check for a valid chnlid # and If the channel is inactive,       */
	/* return an invalid channel error                                  */
	/*------------------------------------------------------------------*/
	IORC ret_code;

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
		/* error: The requesting task is not the owner              */
		/*----------------------------------------------------------*/
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEV_BUSY) );
	}

	if( ret_val == RC_GOOD )
	{
		/*----------------------------------------------------------*/
		/* error: The requesting task has no pending RTXCio calls   */
		/*----------------------------------------------------------*/
		KS_unlock( logical_dev_tbl[deviceno]->dev_res );
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_BAD_WAIT) );
	}

	/*------------------------------------------------------------------*/
	/* The requesting task is indeed the owner so unlock the nested lock*/
	/*------------------------------------------------------------------*/
	KS_unlock( logical_dev_tbl[deviceno]->dev_res );

	/*--------------------------------------------------------------------*/
	/* Verify completion of your request on the device.                   */
	/*--------------------------------------------------------------------*/
	ret_val = KS_waitt(logical_dev_tbl[deviceno]->dev_sema, wait_time);

	if( ret_val == RC_WAIT_CONFLICT )
	{
		/*----------------------------------------------------------*/
		/* Unlock the device's resource to free the device.         */
		/*----------------------------------------------------------*/
		KS_unlock(logical_dev_tbl[deviceno]->dev_res);
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_WAIT_CONFLICT) );
	}

	if( ret_val == RC_TIMEOUT )
		return( ((IORC)RTXCIO_GOOD + (IORC)IO_TIMEOUT) );

	/*--------------------------------------------------------------------*/
	/* return successful results.                                         */
	/*--------------------------------------------------------------------*/
	if( (logical_dev_tbl[deviceno]->last_io_oper == IO_READ) ||
	    (logical_dev_tbl[deviceno]->last_io_oper == IO_WRITE)   )
	{
		if( logical_dev_tbl[deviceno]->lost_data )
		{
		    ret_code = (IORC)SETERROR + ((IORC) SETCLASS *
			(IORC)logical_dev_tbl[deviceno]->dev_cntlr->cntlr_number) +
			((IORC) SETDEVICE * (IORC) deviceno) +
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

/*==========================================================================*/
IORC  IO_chnl_reset( CHNLID chnlid )
{
	DEVID   deviceno;

	/*------------------------------------------------------------------*/
	/* Check for a valid chnlid # and If the channel is inactive,       */
	/* return an invalid channel error                                  */
	/*------------------------------------------------------------------*/
	if( (chnlid == 0) || (chnlid > nchnls) ||
	    (channel_table[chnlid].blink == NULLCCBE) )
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	deviceno = channel_table[chnlid].io_deviceno;

	/*------------------------------------------------------------------*/
	/* Unlock the user from the resource.                               */
	/*------------------------------------------------------------------*/
	while( KS_unlock( logical_dev_tbl[deviceno]->dev_res ) != RC_GOOD );

	/*------------------------------------------------------------------*/
	/* Set the device semaphore back to pending.                        */
	/*------------------------------------------------------------------*/
	KS_pend( logical_dev_tbl[deviceno]->dev_sema );

	return( ((IORC)RTXCIO_GOOD + (IORC)IO_DEV_FREED) );
}
#endif /* } T_DEV_WAITT */
#endif  /* } ASYNCH_IO */

#endif   /* } TIMEOUT_IO */

/* End of the TOUT_API.C module */
