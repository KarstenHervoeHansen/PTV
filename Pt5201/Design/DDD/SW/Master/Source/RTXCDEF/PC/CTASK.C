/* ctask.c - RTXC Task definitions - PT5765 rev.1         04/15/99 10:32 */

#include "typedef.h"
#include "rtxstruc.h"

#include "ctask.h"


extern const TASK near ntasks = NTASKS;

extern const TASK near dntasks = DNTASKS;


extern const TASK near startls[] =
{
   0  /* null terminated list */
};


#define ANY_FPU_TASKS 0
#if !defined(FPU) && ANY_FPU_TASKS
#error "Task requiring FPU support is defined, enable FPU support"
#endif


extern const KTCB near rtxktcb[1+NTASKS] =
{
  { (void(*)(void))0    , (char *)0, 0, 0 }, /* null task */
};

TCB near rtxtcb[1+NTASKS+DNTASKS];

#define NAMMAX 8

extern const char taskname[1+NTASKS][NAMMAX+1] =
{
   ""
};

/* End of file - ctask.c */
