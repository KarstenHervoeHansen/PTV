/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_utl.h - utility function headers                                  */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*     void UL_inq_error( IORC, UL_ERROR_PACK * );                          */
/*     INT  UL_create_FIFO(FIFO *, CHAR *, UCHAR, INT, INT, UCHAR, UCHAR);  */
/*     INT  UL_put_FIFO (FIFO *, CHAR *);                                   */
/*     INT  UL_get_FIFO (FIFO *, CHAR *);                                   */
/*     INT  UL_put_LIFO (FIFO *, CHAR *);                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _RTIO_UTL_H_  /* { _RTIO_UTL_H_ */
#define  _RTIO_UTL_H_

#include "rtcl_tdf.h"

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/*--------------------------------------------------------------------------*/
/* System return code break down for user viewing.                          */
/*--------------------------------------------------------------------------*/
typedef struct ul_error_pack
{
	CNTLRID controller;
	DEVID device;
	INT error;
} UL_ERROR_PACK;

typedef struct ul_fifo
{
	CHAR * body;
	UCHAR width;
	UCHAR depth;
	UCHAR highT;
	UCHAR lowT;
	UCHAR putindex;
	UCHAR getindex;
	UINT  cursize;
} FIFO;

#define NULLPTR (void *)0
#define BAD_FIFO_SIZE -1
#define FIFO_OKAY 0
#define FIFO_FULL -1
#define PUT_OKAY 0
#define FIFO_NOT_EMPTY 1
#define HIGH_THRESHOLD 2
#define FIFO_EMPTY -1
#define GET_OKAY 0
#define FIFO_NOT_FULL 1
#define LOW_THRESHOLD 2

void UL_inq_error(IORC, UL_ERROR_PACK * );
INT  UL_create_FIFO(FIFO *, CHAR *, UCHAR, INT, INT, UCHAR, UCHAR);
INT  UL_put_FIFO (FIFO *, CHAR *);
INT  UL_get_FIFO (FIFO *, CHAR *);
INT  UL_put_LIFO (FIFO *, CHAR *);


#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTIO_UTL_H_ */

/* End of the RTIO_UTL.H module */
