/* atoi.c - RTXC utility functions */

/*
 *   RTXC 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

int atoi(const char *s);

/* poor man's implementation of atoi */
int atoi(const char *s)
{
   int n = 0;
   int sign = 1;

   if (*s == '-')
      sign = -1;

   while ( (*s >= '0') && (*s <= '9') )
   {
      n = 10 * n + (*s - '0');
      s++;
   }
   return(sign * n);
}

/* end of file - atoi.c */
