/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef RUNTM_H
#define RUNTM_H

/* CONFIG */
#define RADIX  /* indicates non-decimal radix support */
               /* some versions don't support non-decimal radix */

#undef NEED_REVERSE

#ifndef TRUE
#define TRUE   -1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef   NULL
#define   NULL ((void*)0)
#endif   /* NULL */


/* Cross Code C specific size_t handling */
#ifndef __SIZE_T
#define __SIZE_T
#ifdef _INTBITS
    typedef _size_t size_t;
#else
    typedef unsigned int size_t;
#endif
#endif

#define NEED_ITOA
#define NEED_UITOA
#define NEED_REVERSE

#ifdef NEED_UITOA
#define LONG
#endif

#endif
