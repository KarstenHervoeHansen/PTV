/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "runtm.h"
#include "utilprot.h"

int ishexdigit(char digit)   /* return TRUE is char is hex digit */
{
   if(digit >= '0' && digit <= '9')
      return TRUE;

   digit |= 0x20;      /* mask letters to lowercase */
   if((digit >= 'a') && (digit <= 'f'))
      return TRUE;
   else
      return FALSE;
}

/* end of ishexdig.c */
