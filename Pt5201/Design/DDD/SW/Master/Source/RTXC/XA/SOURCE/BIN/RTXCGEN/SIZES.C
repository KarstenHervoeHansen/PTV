/* sizes.c - program to display RTXC typedef and structure sizes */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <stdio.h>

#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcopts.h"
#include "rtio_inc.h"

void main(void);

void main(void)
{
   printf("RTXC Typedef and Structure sizes\n\n");

#ifdef CBUG
   printf("CBUG is defined\n");
#else
   printf("CBUG is NOT defined\n");
#endif

#ifdef FPU
   printf("FPU is defined\n");
#else
   printf("FPU is NOT defined\n");
#endif

   printf("\n");

   printf("TASK     %d\n",sizeof(TASK));
   printf("PRIORITY %d\n",sizeof(PRIORITY));
   printf("SEMA     %d\n",sizeof(SEMA));

   printf("TCB      %d\n",sizeof(TCB));
   printf("KTCB     %d\n",sizeof(KTCB));
#ifdef HAS_MAILBOXES
   printf("MBOX     %d\n",sizeof(MBOX));
   printf("RTXCMSG  %d\n",sizeof(RTXCMSG));
   printf("MHEADER  %d\n",sizeof(MHEADER));
#endif
#ifdef HAS_PARTITIONS
   printf("MAP      %d\n",sizeof(MAP));
   printf("PHEADER  %d\n",sizeof(PHEADER));
   printf("PKHEADER %d\n",sizeof(PKHEADER));
#endif
#ifdef HAS_QUEUES
   printf("QUEUE    %d\n",sizeof(QUEUE));
   printf("QHEADER  %d\n",sizeof(QHEADER));
   printf("QKHEADER %d\n",sizeof(QKHEADER));
#endif
#ifdef HAS_RESOURCES
   printf("RESOURCE %d\n",sizeof(RESOURCE));
   printf("RHEADER  %d\n",sizeof(RHEADER));
#endif
   printf("TICKS    %d\n",sizeof(TICKS));
   printf("CLKBLK   %d\n",sizeof(CLKBLK));
   printf("KSNUM    %d\n",sizeof(enum KSNUM));
   printf("FRAME    %d\n",sizeof(FRAME));
#ifdef FPU
   printf("FPREGS   %d\n",sizeof(FPREGS));
#endif

printf("CCB_ENTRY   %d\n",sizeof(CCB_ENTRY));
printf("CNTLR_UNIT  %d\n",sizeof(CNTLR_UNIT));
printf("PDU_HEADER  %d\n",sizeof(PDU_HEADER));

}
