/* ioinit.c */

/*
 *   RTXC BSP Version 1.0
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "typedef.h"
#include "rtxstdio.h"

extern IOCB_Struct iocb;
extern CONCB_Struct CONCdcb[];

extern void  clkstart(void);  /* initialize clock device */
extern void  clkstop(void);   /* disable clock device */

int hwinit(void);
int hwstop(void);

//int ioinit(IOCB_Struct *iocbp);
//int iostop(IOCB_Struct *iocbp);

/*--------------------------------------------------------------------------
  int bspinit(void)
  Function: Initialize the board support package include io and hardware
--------------------------------------------------------------------------*/
int bspinit()
{
	/* initialize board/chip hardware */
	if( hwinit() )
		return(1);

//	/* initialize io devices */
//	if( ioinit(&iocb) )
//		return(1);

	return(0);
}

/*--------------------------------------------------------------------------
  int bspstop(void)
  Function: Shutdown and disable the board support package
--------------------------------------------------------------------------*/
int bspstop()
{
//  iostop(&iocb);	/* stop io devices */

  hwstop(); /* stop hardware devices */

  return(0);
}

/*--------------------------------------------------------------------------
  int hwinit(void)
  Function: Initialize and enable any hardware in this routine
--------------------------------------------------------------------------*/
int hwinit()
{
  /* initialize hardware here */

  clkstart();	/* initialize and start the clock driver */

  return(0);
}

/*--------------------------------------------------------------------------
  int hwstop(void)
  Function: Shutdown and disable any hardware in this routine
--------------------------------------------------------------------------*/
int hwstop()
{
  /* disable any hardware here */

  clkstop();	/* stop the clock driver and restore state */

  return(0);
}

/*-----------------------------------------------------------------------------
   void ioinit(void)
   Function: Initialize and start any io devices

   WARNING: RTXC must be initialized before calling this function
-----------------------------------------------------------------------------*/
//int ioinit(IOCB_Struct *iocbp)
//{
//  if(iocbp->conc1cb)
//  {
//    CONCdcb[iocbp->conc1cb->devno-1] = *iocbp->conc1cb;
//  }
//
//#if USE_PORT2 /* { */
//  if(iocbp->conc2cb)
//  {
//    CONCdcb[iocbp->conc2cb->devno-1] = *iocbp->conc2cb;
//  }
//#endif /* } USE_PORT2 */
//
//  /* initialize any remaining io devices here */
//
//  return 0;
//}

/*-----------------------------------------------------------------------------
   void iostop(void)
   Function: Shutdown and disable any io devices
-----------------------------------------------------------------------------*/
//int iostop(IOCB_Struct *iocbp)
//{
//  /* shutdown and disable any io devices here */
//
//  return 0;
//}

/* End of ioinit.c */



