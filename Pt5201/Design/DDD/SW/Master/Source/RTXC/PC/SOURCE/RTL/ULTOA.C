/* ultoa.c - RTXC unsigned long to ASCII functions */

/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

/* unsigned long to ascii */
void ultoa(unsigned long n, char *s, int radix)
{
    int i = 0;

    do
        { /* generate digits in reverse order */
#ifdef RADIX
            s[i++] = RTXCRadixCTable[n % radix]; /* get next digit */
#else
            s[i++] = n % 10L + '0'; /* get next digit */
#endif
        }
#ifdef RADIX
    while ((n /= radix) > 0L); /* delete it */
#else
    while ((n /= 10L) > 0); /* delete it */
#endif
    s[i] = '\0';

    reverse(s);
}

