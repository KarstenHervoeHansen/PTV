/****************************************************************************
* MODULE:
*    printl.c  -  print string to queue protected by resource
*****************************************************************************
* FUNCTIONS:
*    void printl(char *buffer, RESOURCE resource, QUEUE queue);
*****************************************************************************
* RTXCio    
* Copyright (c) 
* Embedded System Products, Inc.
* ALL RIGHTS RESERVED
*****************************************************************************/

#include "rtxcapi.h"

void printl(char *buffer, RESOURCE resource, QUEUE queue)
{
   if (resource)
      KS_lockw(resource);

   while (*buffer != '\0') /* till end of string mark */
      KS_enqueuew(queue, buffer++);

   if (resource)
      KS_unlock(resource);
}
