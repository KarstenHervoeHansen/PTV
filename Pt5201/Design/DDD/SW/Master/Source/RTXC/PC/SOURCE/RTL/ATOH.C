/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"


/* atoh() - convert hex ascii string to unsigned. number may be preceeded
by whitspace or "0x". Halts at first non hex digit character. */

long atoh(char * buf)
{
  char * cp;
  char digit;
  long retval = 0;

  cp = buf;

  /* skip past spaces and tabs and '0x' */
  while(*cp == ' ' || *cp == 9 || *cp == 'x' || *cp == '0')
    cp++;
  
  while(ishexdigit(digit = *cp++))
    retval = (retval << 4) + hexnibble(digit);
  
  return retval;
}

/* end of atoh.c */
