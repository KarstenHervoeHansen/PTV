/****************************************************************************
* MODULE:
*    rtxcbug.h - define rtxcbugs peripherals, queues and resources
*****************************************************************************
* RTXCio    
* Copyright(c) 
* Embedded System Products, Inc.
* ALL RIGHTS RESERVED
*****************************************************************************/
#include "sysopts.h"
#include "rtio_api.h"

#include "cchnls.h"                           /* USER - channel definitions */
#include "ccntlr.h"                        /* USER - controller definitions */
#include "cdevice.h"                           /* USER - device definitions */

CHNLID cbug_in_chnlid;
CHNLID cbug_out_chnlid;

#define CBUG_BLOCK NULLBLOCK

#define CBUG_IN_PORT  SIO_0_I
#define CBUG_OUT_PORT  SIO_0_O

/*--------------------------------------------------------------------------*/
/* #define only one of the following below and #undef the others            */
/*--------------------------------------------------------------------------*/
#undef CBUG_ASYNCH_IO
#define CBUG_SYNCH_IO
#undef CBUG_TIMEOUT_IO

