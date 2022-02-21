/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

/* hex2bin() - convert hexadecimal text into binary.

Returns 0 if OK else error code in invalid hex char encountered
in input string.
*/

int hex2bin(char * inbuf, unsigned char * outbuf, int bcount)
{
  int i;
  char * cp = inbuf;

   for(i = 0; i < bcount; i++)
   {
       if(ishexdigit(*cp))
         outbuf[i] = (unsigned char)(hexnibble(*cp) << 4);
      else
         return -1;
      cp++;
       if(ishexdigit(*cp))
         outbuf[i] += (unsigned char)hexnibble(*cp);
      else
         return -1;
      cp++;
   }
   return 0;
}

/* end of hex2bin.c */
