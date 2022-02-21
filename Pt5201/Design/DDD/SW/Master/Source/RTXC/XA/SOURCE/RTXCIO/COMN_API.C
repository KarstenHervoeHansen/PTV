/****************************************************************************/
/* MODULE:                                                                  */
/*   comn_api.c - common input/output application interface.                */
/****************************************************************************/
/* FUNCTIONS:                                                               */

#if defined( ASYNCH_IO ) || \
    defined( SYNCH_IO ) || \
    defined( TIMEOUT_IO )  /* { */
/*   CHNLID IO_chnl_open( DEVID, IOTYPE, IOCOMMAND, INITCHNL * )            */
/*     IORC IO_chnl_close( CHNLID )                                         */
/*--------------------------------------------------------------------------*/
/* SUPPORT FUNCTION:                                                        */
/*   IORC  process_io( IOPACK *iopack );                                    */
/*   const char *  IO_version(void);                                    */
#endif /* } ASYNCH_IO | SYNCH_IO | TIMEOUT_IO */
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
extern CCB_ENTRY * chnls_free;              /* pointer to free channel list */
extern CCB_ENTRY * chnls_active;          /* pointer to active channel list */
extern CCB_ENTRY   channel_table[];          /* channel control block table */
extern PDU_HEADER * logical_dev_tbl[];  /* logical to physical device table */

/*==========================================================================*/
/*                      COMMON RTXCio API FUNCTIONS                         */
/*==========================================================================*/

#if defined( ASYNCH_IO ) || \
    defined( SYNCH_IO ) || \
    defined( TIMEOUT_IO )  /* { */
/*==========================================================================*/
/*              THESE RTXCio API FUNCTIONS ARE ALWAYS INCLUDED!             */
/*==========================================================================*/
const char *  IO_version(void)
{
   return("RTXCio v1.4a XA-G3/Tasking v2.0 Mar-26-98");
}

/*==========================================================================*/
CHNLID   IO_chnl_open( DEVID       deviceno,
			   IOTYPE      io_method,
			   IOCOMMAND   io_command,
			   INITCHNL *  otherinfo )
{
	PDU_HEADER * pduh;
	CHNLID chnlid;

	/*------------------------------------------------------------------*/
	/* If the IO_COMMAND does not equal any of the                      */
	/* three command types then return in error.                        */
	/*------------------------------------------------------------------*/
	if( ( io_command != READ_IO  ) &&
	    ( io_command != WRITE_IO ) &&
	    ( io_command != RDWR_IO  ) )
		   return( (CHNLID)((IORC)RTXCIO_ERROR + (IORC)IO_BAD_COMMAND) );

	/*------------------------------------------------------------------*/
	/* If the IO_METHOD does not equal any of the                       */
	/* two method types then return in error.                           */
	/*------------------------------------------------------------------*/
	if( ( io_method != IO_DIRECT  ) && ( io_method != IO_BUFFERED ) )
		   return( (CHNLID)((IORC)RTXCIO_ERROR + (IORC)IO_BAD_METHOD) );

	/*------------------------------------------------------------------*/
	/* If the device is not valid then return in error.                 */
	/*------------------------------------------------------------------*/
	if( ( deviceno <= 0 ) || ( deviceno > nldevs ) )
	       return( (CHNLID)((IORC)RTXCIO_ERROR + (IORC)IO_DEVICE_INVALID) );

	/*------------------------------------------------------------------*/
	/* Get the pysical device header of the device.                     */
	/*------------------------------------------------------------------*/
	pduh = logical_dev_tbl[deviceno];

	/*------------------------------------------------------------------*/
	/* If the device attribute is to update, then read or write command */
	/* is okay, or if the channel command is the equivalent to the      */
	/* device attribute then okay. Else return in error.                */
	/*------------------------------------------------------------------*/
	if( (pduh->dev_attribute == ATTB_RDWR) ||
	    ( io_command == (IOCOMMAND)pduh->dev_attribute ) )
	{
	    /*--------------------------------------------------------------*/
	    /* If channels are available then assign one, else return ZERO  */
	    /* indicating no channels are available.                        */
	    /*--------------------------------------------------------------*/
	    if( chnls_free != NULLCCBE )
	    {
		if( chnls_active != NULLCCBE )
		{
			chnls_active->blink = chnls_free;
			chnls_free = chnls_free->flink;
			chnls_active->blink->flink = chnls_active;
			chnls_active = chnls_active->blink;
		}
		else
		{
			chnls_active = chnls_free;
			chnls_free = chnls_free->flink;
			chnls_active->flink = NULLCCBE;
		}
		chnls_active->blink = ( CCB_ENTRY *)&chnls_active;
		chnlid = chnls_active->chnl_number;
	    }
	    else
		return( (CHNLID)0 );    /* NO FREE CHANNELS */
	}
	else
	      return( (CHNLID)((IORC)RTXCIO_ERROR + (IORC)IO_CMND_MISMATCH) );

	/*------------------------------------------------------------------*/
	/* Passed the tests, now setup the channel control block.           */
	/*------------------------------------------------------------------*/
	channel_table[chnlid].io_deviceno = deviceno;
	channel_table[chnlid].io_method = io_method;
	channel_table[chnlid].io_command = io_command;

	/*------------------------------------------------------------------*/
	/* If the channel is being setup for block I/O, then assign the     */
	/* other information.                                               */
	/*------------------------------------------------------------------*/
	if(otherinfo != NULLIOINFO )
	{
	  channel_table[chnlid].io_bytespblk = otherinfo->bytes_per_block;
	  channel_table[chnlid].io_blks2transfer= otherinfo->blocks_to_transfer;
	}
	return( chnlid );
}

/*==========================================================================*/
IORC   IO_chnl_close( CHNLID  chnlid )
{
	CCB_ENTRY *pccbe;

	/*------------------------------------------------------------------*/
	/* Is the channel id valid ?                                        */
	/*------------------------------------------------------------------*/
	if( (chnlid == 0) || (chnlid > nchnls) )
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	/*------------------------------------------------------------------*/
	/* Take the channel out of the active list of channels and insert   */
	/* it into the list of available channels.                          */
	/*------------------------------------------------------------------*/
	pccbe = &channel_table[chnlid];

	/*------------------------------------------------------------------*/
	/* If the channel is inactive, return an invalid channel error      */
	/*------------------------------------------------------------------*/
	if( pccbe->blink == NULLCCBE )
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_CHNLID_INVALID) );

	/*------------------------------------------------------------------*/
	/* first in the active channel list test.                           */
	/*------------------------------------------------------------------*/
	if( pccbe->blink != (CCB_ENTRY *)&chnls_active )
		pccbe->blink->flink = pccbe->flink;
	else
		chnls_active = chnls_active->flink;

	/*------------------------------------------------------------------*/
	/* Not end of the active channel list test.                         */
	/*------------------------------------------------------------------*/
	if( pccbe->flink != NULLCCBE )
		pccbe->flink->blink = pccbe->blink;

	pccbe->flink = chnls_free;
	pccbe->blink = NULLCCBE;
	chnls_free = pccbe;

	/* valid close of channel */
	return( ((IORC)RTXCIO_GOOD + (IORC)IO_CHNL_CLOSED) );
}

/*==========================================================================*/
/*                      PROCESSING FUNCTION                                 */
/*==========================================================================*/
IORC process_io( IOPACK *iopack )
{

	IORC phys_result;      /* The results of the physical driver's call */
	/*------------------------------------------------------------------*/
	/* Lock the physical drivers resource.                              */
	/*------------------------------------------------------------------*/
#ifdef ASYNCH_IO     /* { */
	KSRC ret_val;

	if( iopack->io_duration == IO_NOWAIT )
	{
	    ret_val = KS_lock( logical_dev_tbl[iopack->deviceno]->dev_res );

	    if( ret_val == RC_NESTED )
	    {
		/*----------------------------------------------------------*/
		/* return an error stating the device is busy               */
		/*----------------------------------------------------------*/
		KS_unlock( logical_dev_tbl[iopack->deviceno]->dev_res );
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_NEED_WAIT) );
	    }

	    if( ret_val == RC_BUSY )
	    {
		/*----------------------------------------------------------*/
		/* return an error stating the device is busy               */
		/*----------------------------------------------------------*/
		return( ((IORC)RTXCIO_ERROR + (IORC)IO_DEV_BUSY) );
	    }
	}
	else
	{
#endif    /* } ASYNCH_IO */

#if defined( SYNCH_IO ) || \
    defined( TIMEOUT_IO )  /* { */
	KS_lockw( logical_dev_tbl[iopack->deviceno]->dev_res );
#endif /* } SYNCH_IO | TIMEOUT_IO */

#ifdef ASYNCH_IO     /* { */
	}
#endif    /* } ASYNCH_IO */
	/*------------------------------------------------------------------*/
	/* Reset the following PDU's fields.                                */
	/*------------------------------------------------------------------*/
	logical_dev_tbl[iopack->deviceno]->last_io_oper = iopack->iofncid;
	logical_dev_tbl[iopack->deviceno]->lost_data = NO_LOST_DATA;
	logical_dev_tbl[iopack->deviceno]->bytes_transfered = 0;


	/*------------------------------------------------------------------*/
	/* Physical driver's resource has been locked.                      */
	/* Call the physical driver for the device specified.               */
	/* IO_WAIT & IO_TIMED act the same.                                 */
	/*------------------------------------------------------------------*/
	phys_result = (IORC)logical_dev_tbl[iopack->deviceno]->dev_cntlr->io_funcs[iopack->iofncid]( iopack );

	/*------------------------------------------------------------------*/
	/* Distinquish whether Asynchronous or synchronous/timeout and take */
	/* the appropriate actions.                                         */
	/*------------------------------------------------------------------*/
	if( iopack->io_duration != IO_NOWAIT )
	{
		/*----------------------------------------------------------*/
		/* Unlock the physical driver's resource after the physical */
		/* device has completed the request.                        */
		/*----------------------------------------------------------*/
		KS_unlock( logical_dev_tbl[iopack->deviceno]->dev_res );
	}
	else
		if( (iopack->iofncid == IO_DEV_OPEN) || (iopack->iofncid == IO_DEV_CLOSE) )
			KS_unlock( logical_dev_tbl[iopack->deviceno]->dev_res );

	return( phys_result );
}

#endif /* } ASYNCH_IO | SYNCH_IO | TIMEOUT_IO */

/* End of the COMN_API.C module */
