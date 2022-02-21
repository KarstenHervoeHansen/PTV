/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

/* atohl() - same as atoh() above, except returns a long */
unsigned long atohl(char * buf)
{
  unsigned long retval = 0;
  char * cp;
  char digit;

  cp = buf;

   /* skip past spaces and tabs and '0x' */
   while(*cp == ' ' || *cp == 9 || *cp == 'x' || *cp == '0')
      cp++;

    while(ishexdigit(digit = *cp++))
      retval = (retval << 4) + (unsigned long)hexnibble(digit);
   
   return retval;
}

/* end of atohl.c */
