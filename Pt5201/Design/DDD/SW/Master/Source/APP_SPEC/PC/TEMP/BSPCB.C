/* bspcb.c */

/*
 *   RTXC BSP Version 1.0
 *   Copyright (c) 1994-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "ctask.h"
#include "cqueue.h"
#include "cres.h"
#include "csema.h"
#include "cclock.h"             /* changed to cproject.h for SYSgen */
#include "rtxstdio.h"


/*
 * I/O Control Blocks
*/
#define USE_PORT1 /* UART - COM[1|2]*/
#define USE_PORT2 /* DOS Console */

void *const stderr = (void *)PORT1;
void *const stdin  = (void *)PORT1;
void *const stdout = (void *)PORT1;

#ifdef USE_PORT1
/* Console port 1 init parameters */
extern const CONCB_Struct conc1def =
{
    1,                          /* Port device # */
    CONIDRV,
    CONODRV,
    DOSRES,
    (SEMA)0,
    (SEMA)0,
    CONIQ,
    CONOQ,
    (void *)0,
};
#endif

#ifdef USE_PORT2
/* Console port 2 init parameters */
extern const CONCB_Struct conc2def =
{
    2,                          /* Port device # */
    COMIDRV,                   /* Port input task */
    COMODRV,                   /* Port output task */
    COMRES,                    /* Port's mutex */
    COMISEM,                   /* input ISR signal */
    COMOSEM,                   /* output ISR signal */
    COMIQ,                     /* input que handle */
    COMOQ,                     /* out que handle */
    (void *)0,
};
#endif

extern const IOCB_Struct iocb =
{
#ifdef USE_PORT1
  (CONCB_Struct *)&conc1def,   /* Console (Port) 1 control block address */
#else
  (CONCB_Struct *)0,
#endif
#ifdef USE_PORT2
  &conc2def    /* Console (Port) 2 control block address */
#else
  (CONCB_Struct *)0
#endif
};

/*
 * Hardware Control Blocks
*/

/* Clock 1 init parameters */
extern const CLKCB_Struct CLKdcb =
{
    CLKTICK,                  /* Clock tick */
    CLKRATE                   /* Clock rate */
};

/* Print driver init parameters */
extern const PRNCB_Struct PRNdcb =
{
    PRNSEMA,                  /* PRN Driver semaphore */
    PRNQ                      /* PRN Driver queue */
};


extern const HWCB_Struct hwcb =
{
  (CLKCB_Struct *)&CLKdcb,   /* Clock 1 control block address */
  (PRNCB_Struct *)&PRNdcb,   /* Print driver control block address */
};

/* end of bspcb.c */

