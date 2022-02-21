/* cqueue.c - RTXC Queue definitions - Default              12/28/99 08:33 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cqueue.h"

const QUEUE nqueues = NQUEUES+DNQUEUES;


QHEADER ks_tblx qheader[1+NQUEUES+DNQUEUES];

const QKHEADER qkheader[1+NQUEUES] =
{
   { (char *)0, (size_t)0, 0 }  /* not used */
};

#define NAMMAX 8

const char queuename[1+NQUEUES][NAMMAX+1] =
{
   ""
};
