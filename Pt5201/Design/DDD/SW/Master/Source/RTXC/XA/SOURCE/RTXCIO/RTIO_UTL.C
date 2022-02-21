/****************************************************************************/
/* MODULE:                                                                  */
/*   rtio_utl.c - utility functions                                         */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*     void UL_inq_error( IORC, UL_ERROR_PACK * )                           */
/*     INT  UL_create_FIFO(FIFO *, CHAR *, UCHAR, INT, INT, UCHAR, UCHAR);  */
/*     INT  UL_put_FIFO (FIFO *, CHAR *);                                   */
/*     INT  UL_get_FIFO (FIFO *, CHAR *);                                   */
/*     INT  UL_put_LIFO (FIFO *, CHAR *);                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#include "rtio_tdf.h"        /* RTXCio - Type definitions                   */
#include "rtio_utl.h"        /* RTXCio - Utilities Header file include file */

/*==========================================================================*/
/*                              Error utility                               */
/*==========================================================================*/
void UL_inq_error(IORC ret_code, UL_ERROR_PACK *error_pack )
{
    /* get result of bits 11-14 inclusive */
    error_pack->controller = ((ret_code >> 11) & ~(~0 << 4));

    if( error_pack->controller >= 13 )
    {
	/* there is no device return zero */
	error_pack->device = 0;
	/* get result of bits 0-10 inclusive */
	error_pack->error = ((ret_code >> 0) & ~(~0 << 11));
    }
    else
    {
	/* get result of bits 6-10 inclusive */
	error_pack->device = ((ret_code >> 6) & ~(~0 << 5));
	/* get result of bits 0-5 inclusive */
	error_pack->error = ((ret_code >> 0) & ~(~0 << 6));
    }

    /* if the device will equal to zero than when RTXCio error is returned */
    /* RTXCio error range from bits 0-10 */
}

/*==========================================================================*/
/*                              FIFO utilities                              */
/*==========================================================================*/
INT UL_create_FIFO( FIFO *pf,
		    CHAR *body,
		    UCHAR width,
		    INT blksize,
		    INT cursize,
		    UCHAR highwater,
		    UCHAR lowwater )
{
   INT m;

   if (body != NULLPTR)
   {
      if (blksize > 256)
	 return (BAD_FIFO_SIZE);
      pf->body = body;
   }
   else
   {
      if ( (blksize > 256) || (blksize < sizeof(FIFO)) )
	  return (BAD_FIFO_SIZE);
      blksize -= sizeof(FIFO);
      pf->body = (CHAR *)pf+sizeof(FIFO);
   };
   m = 256;
   blksize /= width;
   while ((m & blksize) == 0)
      m /= 2;

   pf->depth = (unsigned char)(m - 1);
   pf->width = width;
   pf->cursize = cursize;
   pf->highT = highwater;
   pf->lowT = lowwater;
   pf->putindex = cursize;
   pf->getindex = 0;

   return (FIFO_OKAY);
}

/*==========================================================================*/
INT UL_put_FIFO( FIFO *pf,
		 CHAR *src )
{
   INT i;
   CHAR *put;

   if (pf->cursize == pf->depth + 1)
      return (FIFO_FULL);

   put = pf->body + (pf->width * pf->putindex);

   for (i = 0; i < pf->width; i++)
      *put++ = *src++;

   pf->putindex = (pf->putindex + 1) & pf->depth;
   pf->cursize++;

   if ( (pf->highT > 0) && (pf->highT == pf->cursize) )
      return (HIGH_THRESHOLD);

   if (pf->cursize == 1)
	 return (FIFO_NOT_EMPTY);

   return (PUT_OKAY);
}

/*==========================================================================*/
INT UL_get_FIFO( FIFO *pf,
		 CHAR *dst)
{
   INT i;
   CHAR *get;

   if (pf->cursize == 0)
      return (FIFO_EMPTY);

   get = pf->body + (pf->width * pf->getindex);

   for (i = 0; i < pf->width; i++)
      *dst++ = *get++;

   pf->getindex = (pf->getindex + 1) & pf->depth;
   pf->cursize--;

   if ( (pf->lowT > 0) && (pf->lowT == pf->cursize) )
      return (LOW_THRESHOLD);

      if (pf->cursize == pf->depth)
	 return (FIFO_NOT_FULL);

      return (GET_OKAY);
}

/*==========================================================================*/
INT UL_put_LIFO( FIFO *pf,
		 CHAR *src )
{
   INT i;
   CHAR *put;

   if (pf->cursize == pf->depth + 1)
      return (FIFO_FULL);

   pf->getindex = (pf->getindex - 1) & pf->depth;
   put = pf->body + (pf->width * pf->getindex);

   for (i = 0; i < pf->width; i++)
      *put++ = *src++;

   pf->cursize++;

   if (pf->cursize == 1)
	 return (FIFO_NOT_EMPTY);

   if ( (pf->highT > 0) && (pf->highT == pf->cursize) )
      return (HIGH_THRESHOLD);

   return (PUT_OKAY);
}

/* End of the RTIO_UTL.C module */
