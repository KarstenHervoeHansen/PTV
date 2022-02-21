/*  printl.c  -  print string to queue protected by resource  */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxcapi.h"

void printl(const char *buffer, RESOURCE resource, QUEUE queue)
{
   if (resource)
      KS_lockw(resource);

   while (*buffer != '\0') /* till end of string mark */
      KS_enqueuew(queue, (void *)buffer++);

   if (resource)
      KS_unlock(resource);
}

/* End of file - printl.c */
