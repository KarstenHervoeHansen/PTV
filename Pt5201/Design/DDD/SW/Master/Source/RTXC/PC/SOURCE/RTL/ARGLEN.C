/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

/* stlen() - return length of string passed. space char 
   counts as end of string.
*/

int arglen(char *st1)
{
   int retval = 0;
  
   while(*st1 && *st1 != ' ')	/* find first NULL or space */
   {	
      st1++;
      retval++;
   }
   return(retval);
}

/* end of arglen.c */
