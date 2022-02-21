/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_int.c - RTXCio initialization.                                    */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*   void rtxcio_init( void )                                               */
/*   void dev_init( void )                                                  */
/****************************************************************************/
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#include "typedef.h"                            /* RTXC - type declarations */

#include "rtio_inc.h"                  /* RTXCio - Header file include file */

extern const DEV_INIT_PACK  dev_init_tbl[];

extern CCB_ENTRY * chnls_free;                    /* List of free channel   */
						  /* control blocks.        */
extern CCB_ENTRY * chnls_active;                  /* List of active channel */
						  /* control blocks.        */
extern CCB_ENTRY   channel_table[];

extern PDU_HEADER * logical_dev_tbl[];               /* logical to physical */
							   /* device tablee */

extern CNTLR_UNIT * cntlr_loc_table[];          /* Controller locator table */

extern const DEVID  nldevs;      /* number of devices defined in application */
extern const CHNLID  nchnls;    /* number of channels defined in application */
extern const CNTLRID  ncntlrs; /* number of controllers defined in application */

void dev_init( void );

/*==========================================================================*/
void rtxcio_init( void )
{
	CHNLID chnlnum;
	CCB_ENTRY *link;

	/*------------------------------------------------------------------*/
	/* Initialize the Channel control block array.                      */
	/*------------------------------------------------------------------*/
	chnls_active = NULLCCBE;
	link = chnls_free = &channel_table[1];

	/*------------------------------------------------------------------*/
	/* Initialize the zero CCB_ENTRY fields of the channel table to some*/
	/* arbitrary number. This entry is not allocated to any application */
	/* tasks! This initialization distinquishes the zeroeth entry.      */
	/*------------------------------------------------------------------*/
          channel_table[0].chnl_number = 0;
          channel_table[0].io_deviceno = 0;
          channel_table[0].io_command = (IOCOMMAND) 0;
          channel_table[0].io_method = (IOTYPE) 0;
          channel_table[0].io_bytespblk = 0;
          channel_table[0].io_blks2transfer = 0;

	for(chnlnum=1;chnlnum<nchnls;chnlnum++, link++)
	{
		link->chnl_number = chnlnum;
		link->flink = link + 1;
		link->blink = NULLCCBE;
	}
	link->chnl_number = chnlnum;
	link->flink = NULLCCBE;
	link->blink = NULLCCBE;

	dev_init( );
}

/*==========================================================================*/
void dev_init( void )
{
	INT i;

	/*------------------------------------------------------------------*/
	/* Initialize the Controller Locator Table                          */
	/*------------------------------------------------------------------*/
	for(i=0;i<ncntlrs+1;i++)
		cntlr_loc_table[i] = NULLCNTLR;

	/*------------------------------------------------------------------*/
	/* Initialize the logical to physical device table                  */
	/*------------------------------------------------------------------*/
	for(i=0;i<nldevs+1;i++)
		logical_dev_tbl[i] = NULLUHEADER;

	/*------------------------------------------------------------------*/
	/* Initialize the PDUs, Controller table, logical device table ...  */
	/*------------------------------------------------------------------*/
	for(i=1;i<=nldevs;i++)
	     dev_init_tbl[i].devinit_func( (DEV_INIT_PACK* )&dev_init_tbl[i] );
}

/* End of the RTIO_INT.C module */
