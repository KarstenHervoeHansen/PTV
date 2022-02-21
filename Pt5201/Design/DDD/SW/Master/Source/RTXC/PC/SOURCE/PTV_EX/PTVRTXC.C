/* ptvrtxc.c - PTV's RTXC application program interface to kernel directives */

/*
 *   Added to RTXC    Version 3.2
 *   Copyright (c) 1999.
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/

#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcarg.h"
#include "enable.h"
#include "rtxcapi.h"

#define NULL        ((void *)0)
#define NULLCLK     ((CLKBLK ks_clk *)0)
#define NULLTCB     ((TCB near *)0)
 
#define SELFTASK    ((TASK)0)

extern long Sems[];

#ifdef HAS_RESOURCES /* { */
extern RHEADER near rheader[];
#endif /* } HAS_RESOURCES */

#ifdef HAS_QUEUES /* { */
//extern QHEADER near qheader[];
#endif /* } HAS_QUEUES */

/*32******************************************************/
#ifdef HAS_ALLOC /* { */
#ifdef HAS_INQMAP /* { */
SEMA KSAPI KS_alloc_semafore(void)
{
   extern MAP semafores;
   SEMA sem;
   long *ptr;

   ptr=KS_alloc(semafores);
   if(ptr==NULL)
      return(0);
   sem=1;
   while(ptr!=&Sems[0])
   {
      sem++;
      ptr--;
   }
   return(sem);
}
#endif /* } HAS_INQMAP */
#endif /* } HAS_ALLOC */

/*******************************************************/
#ifdef HAS_FREE /* { */
#ifdef HAS_INQMAP /* { */
void KSAPI KS_free_semafore(SEMA sem)
{
   extern MAP semafores;
   long *ptr;

   ptr=&Sems[0];
   ptr=ptr+(sem-1);
   KS_free(semafores,ptr);
}
#endif /* } HAS_INQMAP */
#endif /* } HAS_FREE */

/*******************************************************/



/*******************************************************/
#ifdef HAS_RESOURCES /* { */
#ifdef HAS_ALLOC /* { */
#ifdef HAS_INQMAP /* { */
RESOURCE KSAPI KS_alloc_resource(void)
{
   extern MAP resources;
   RESOURCE res;
   RHEADER *ptr;

   ptr=KS_alloc(resources);
   if(ptr==NULL)
      return(0);
   res=1;
   while(ptr!=&rheader[0])
   {
      res++;
      ptr--;
   }
   return(res);
}
#endif /* } HAS_INQMAP */
#endif /* } HAS_ALLOC */
#endif /* } HAS_RESOURCES */

/*******************************************************/
#ifdef HAS_RESOURCES /* { */
#ifdef HAS_FREE /* { */
#ifdef HAS_INQMAP /* { */
void KSAPI KS_free_resource(RESOURCE res)
{
   extern MAP resources;
   RHEADER *ptr;

   ptr=&rheader[0];
   ptr=ptr+(res-1);
   KS_free(resources,ptr);
}
#endif /* } HAS_INQMAP */
#endif /* } HAS_FREE */
#endif /* } HAS_RESOURCES */

/*******************************************************/

/* End of file - ptvrtxc.c */
