/* itoa.c - RTXC int to ASCII function */

/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/* itoa.c */

/*
 * itoa is used by format()
*/

#include "runtm.h"
#include "utilprot.h"

/* integer to ascii */
void itoa(int n, char *s, int radix)
{
    int i = 0;
    int sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */

    do
        { /* generate digits in reverse order */
#ifdef RADIX
            s[i++] = RTXCRadixCTable[n % radix]; /* get next digit */
#else
            s[i++] = n % 10 + '0'; /* get next digit */
#endif
        }
#ifdef RADIX
    while ((n /= radix) > 0); /* delete it */
#else
    while ((n /= 10) > 0); /* delete it */
#endif
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    reverse(s);
}

/* end of itoa.c */
