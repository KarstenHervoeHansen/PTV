/* ltoa.c - RTXC long to ASCII functions */

/*
 *   RTXC 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

#ifdef RADIX

/* long to ascii */
void ltoa(long n, char *s, int radix)
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

#else
/* does not use RTXCRadixCTable */
char *ltoa(char *p, unsigned long num, int radix)
{
   unsigned int i;
   char *q;
   char *hold;

   hold = p;
   q = p + 12;

   *--q = '\0'; /* be sure to terminate the string with a NULL character */

   do
   {
      i = (num % radix) + '0';
      if ( i > '9' )
         i+='A' - '0' - 10;
      *--q = i;
   } while( num /= radix );

   /* move the string to the left, including the NULL character	*/
   for ( ; (*p++ = *q++); )
      ;

   return( hold );
}
#endif

/* End of ltoa.c */
