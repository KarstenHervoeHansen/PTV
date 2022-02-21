/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

/* stcmp() - special string compare routine. returns 0 if strings match,
   else returns char where 1st mismatch occured. ie: 
   stcmp("help", "helper") returns a 5.
   stcmp("help", "nohelp")	returns a 1.

Also: treats space as end of string.
*/

int stcmp(char * s1, char * s2)
{
  int	ct = 1;		/* byte being checked */
  
  while(*s1 && *s1 != ' ')	/* read to 1st space or end */
  {
    if(*s1++ != *s2++) return(ct);
    ct++;
  }
  if(*s2) return(ct);	/* s2 was longer than s1 */
  return(0);
}

/* end of strcmp.c */
