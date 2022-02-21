/* cqueue.c - RTXC Queue definitions - pt5765 rev.1         03/25/99 23:04 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cqueue.h"

#define Q1WD 1 /* REM_IQ   - RemoteIn_Queue       */
#define Q1DP 1

#define Q2WD 2 /* STDI_Q   - StandartInput_Queue  */
#define Q2DP 10

extern const QUEUE near nqueues = NQUEUES;

static char far q1[Q1DP][Q1WD];
static char far q2[Q2DP][Q2WD];

QHEADER near qheader[1+NQUEUES];

extern const QKHEADER near qkheader[1+NQUEUES] =
{
   { (char *)0, (size_t)0, 0 }, /* not used */
   { &q1[0][0], (size_t)Q1WD, Q1DP },
   { &q2[0][0], (size_t)Q2WD, Q2DP }
};

#define NAMMAX 8

extern const char queuename[1+NQUEUES][NAMMAX+1] =
{
   "",
   "REM_IQ",
   "STDI_Q"
};

/* End of file - cqueue.c */
