/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_str.h - RTXCio structure declarations                             */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _RTIO_STR_H_  /* { _RTIO_STR_H_ */
#define  _RTIO_STR_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*--------------------------------------------------------------------------*/
/* RTXCio request package built in API sent to device driver.               */
/*--------------------------------------------------------------------------*/
typedef struct io_package
{
	  DEVID   deviceno;                   /* Physical device identifier */
	 CHNLID   chnlid;                  /* channel identification number */
	IOFNCID   iofncid;                       /* I/O function identifier */
	SPFNCID   spfncid;                   /* Special function identifier */
    IO_DURATION   io_duration;    /* How to process i/o wait, nowait, timed */
	  BLKNO   startblk;          /* Block number to start read or write */
	BUFFADD   transfer_buff;                  /* The io transfer buffer */
	BYTECNT   numbytes;         /* The number of bytes to read or write */
	   void * otherinfo;                       /* A user defined object */
#ifdef TIMEOUT_IO  /* { */
	  TICKS   waitime;        /* Time to wait in TIMEOUT_IO requests */
#endif  /* } TIMEOUT_IO */
} IOPACK;

/*--------------------------------------------------------------------------*/
/* RTXCio channel initialization strucure.                                  */
/*--------------------------------------------------------------------------*/
typedef struct init_channel
{
	BYTECNT      bytes_per_block;          /* number of bytes per block */
	  BLKNO      blocks_to_transfer;    /* number of blocks to transfer */
} INITCHNL;


/*--------------------------------------------------------------------------*/
/* RTXCio channel entry                                                     */
/*--------------------------------------------------------------------------*/
typedef struct ccb_entry
{
  struct ccb_entry * flink;                   /* next entry in the CCB list */
  struct ccb_entry * blink;               /* previous entry in the CCB list */
	    CHNLID   chnl_number;                      /* channel id number */
	     DEVID   io_deviceno;             /* Physical device identifier */
	 IOCOMMAND   io_command;                           /* read or write */
	    IOTYPE   io_method;                         /* direct, buffered */
	   BYTECNT   io_bytespblk;             /* number of bytes per block */
	     BLKNO   io_blks2transfer;      /* number of blocks to transfer */
} CCB_ENTRY;

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTIO_STR_H_ */

/* End of the RTIO_STR.H module */
