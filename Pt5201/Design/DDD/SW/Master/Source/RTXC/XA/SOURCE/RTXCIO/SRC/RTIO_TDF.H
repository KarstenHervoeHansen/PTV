/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_tdf.h - RTXCio type declarations                                  */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _RTIO_TDF_H_  /* { _RTIO_TDF_H_ */
#define  _RTIO_TDF_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#include "rtio_opt.h"

#ifndef _SIZE_T_DEF
typedef unsigned int    size_t;
#define _SIZE_T_DEF
#endif /* } _SIZE_T_DEF */

/*--------------------------------------------------------------------------*/
/* Definitions of the RTXCio argument types.                                */
/*--------------------------------------------------------------------------*/

typedef  short  IORC;                       /* this typedef MUST not change */
					    /* - signed short */

typedef  unsigned short  CHNLID;            /* this typedef MUST not change */
					    /* - unsigned short */

typedef  void *   BUFFADD;

typedef  char     CHAR;
typedef  unsigned char  UCHAR;
typedef  int      INT;

typedef  unsigned int  UINT;
typedef  size_t  BYTECNT;
typedef  unsigned int  BOOL;

typedef  long int  LONG;
typedef  unsigned long int BLKNO;

#define NO_LOST_DATA 0

#define IO_YES 1                             /* do not alter this # defines */
#define IO_NO  0                             /* do not alter this # defines */

/*--------------------------------------------------------------------------*/
/* RTXCio function identifiers.                                             */
/*--------------------------------------------------------------------------*/
typedef enum
{
	IO_READ = 0,                                       /* physical read */
	IO_WRITE,                                         /* physical write */
	IO_DEV_OPEN,                     /* physical device initilalization */
	IO_DEV_CLOSE,                 /* reset initial system configuration */
	IO_CONTROL,                    /* physical device special functions */
	IO_NONE
} IOFNCID;

/*--------------------------------------------------------------------------*/
/* Channel control block i/o types.                                         */
/*--------------------------------------------------------------------------*/
typedef enum
{
	IO_DIRECT = 6,                                        /* direct i/o */
	IO_BUFFERED                                         /* buffered i/o */
} IOTYPE;

/*--------------------------------------------------------------------------*/
/* Channel control block i/o commands.                                      */
/*--------------------------------------------------------------------------*/
typedef enum
{
	READ_IO = 8,                                        /* read channel */
	WRITE_IO,                                          /* write channel */
	RDWR_IO                                     /* read & write channel */
} IOCOMMAND;

/*--------------------------------------------------------------------------*/
/* RTXCio return codes.                                                     */
/*--------------------------------------------------------------------------*/
typedef enum
{
	IO_CHNL_CLOSED = 0,                /* Successful closing of a       */
					   /* channel.                      */

	IO_CHNLID_INVALID,                 /* channel id passed was equal   */
					   /* to or less than zero which is */
					   /* illegal.                      */

	IO_DEVICE_INVALID,                 /* Invalid device specified in   */
					   /* channel open or the channel   */
					   /* specified had an IO_COMMAND   */
					   /* that did not match the device */
					   /* capabitilies.                 */

	IO_BAD_COMMAND,                    /* An invalid channel command    */
					   /* was specified in channel open.*/

	IO_BAD_METHOD,                     /* An invalid channel method     */
					   /* was specified in channel open.*/
#ifndef ASYNCH_IO  /* { */
	IO_CMND_MISMATCH                   /* A mismatch between the        */
					   /* specified channel command and */
					   /* the device attribute was      */
					   /* detected during channel open. */
#else
	IO_CMND_MISMATCH,                  /* A mismatch between the        */
					   /* specified channel command and */
					   /* the device attribute was      */
					   /* detected during channel open. */
#endif

#ifdef ASYNCH_IO  /* { */
	IO_LOST_DATA,                      /* Asynchronous read lost data   */

	IO_DEV_BUSY,                       /* Asynchronous request could not*/
					   /* lock resource immediately     */

	IO_NEED_WAIT,                      /* Asynchronous requester needs  */
					   /* to call IO_dev_wait()         */

	IO_BAD_WAIT,                       /* Invalid call to IO_dev_wait() */

	IO_DEV_FREED,                      /* Asynchronous requester        */
					   /* released the device resource  */

	IO_WAIT_CONFLICT,                  /* semaphore in wait state       */

	IO_TIMEOUT                         /* timed out before device       */
					   /* completed request             */
#endif  /* } ASYNCH_IO */
}  IORC_TYPE;

/*--------------------------------------------------------------------------*/
/* RTXCio type of i/o requested.                                            */
/*--------------------------------------------------------------------------*/
typedef enum
{
#ifdef SYNCH_IO  /* { */
	IO_WAIT = 0,                                         /* synchronous */
#endif /* } SYNCH_IO */

#ifdef TIMEOUT_IO  /* { */
	IO_TIMED,                               /* synchronous with timeout */
#endif /* } TIMEOUT_IO */

	IO_NOWAIT                                   /* asynchronous request */

}IO_DURATION;

/*--------------------------------------------------------------------------*/
/* Class independent error set up values                                    */
/*--------------------------------------------------------------------------*/
#define SETERROR  0x8000
#define SETCLASS  0x0800
#define SETDEVICE 0x0040

/*--------------------------------------------------------------------------*/
/* RTXCio error set up values                                               */
/*--------------------------------------------------------------------------*/
#define RTXCIO_CL 0x000f
#define RTXCIO_GOOD  (SETCLASS * RTXCIO_CL)
#define RTXCIO_ERROR (SETERROR + (SETCLASS * RTXCIO_CL))

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _RTIO_TDF_H_ */

/* End of the RTIO_TDF.H module */
