/* ctask.c - RTXC Task definitions - test                 04/14/99 20:34 */

#include "typedef.h"
#include "rtxstruc.h"

#include "ctask.h"

#define STKSZ1 512

extern const TASK near ntasks = NTASKS;

extern const TASK near dntasks = DNTASKS;

static int ks_stk stack1[STKSZ1/sizeof(int)];

extern const TASK near startls[] =
{
   RTXCMAIN, /* rtxcmain             */
   0  /* null terminated list */
};

extern void rtxcmain(void);

#define ANY_FPU_TASKS 0
#if !defined(FPU) && ANY_FPU_TASKS
#error "Task requiring FPU support is defined, enable FPU support"
#endif


extern const KTCB near rtxktcb[1+NTASKS] =
{
  { (void(*)(void))0    , (char *)0, 0, 0 }, /* null task */
  { rtxcmain, (char *)stack1, STKSZ1, 1 } /* RTXCMAIN-rtxcmain             */
};

TCB near rtxtcb[1+NTASKS+DNTASKS];

#define NAMMAX 8

extern const char taskname[1+NTASKS][NAMMAX+1] =
{
   "",
   "RTXCMAIN"
};

/* End of file - ctask.c */
