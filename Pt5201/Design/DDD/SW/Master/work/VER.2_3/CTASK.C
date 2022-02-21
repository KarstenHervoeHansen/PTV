/* ctask.c - RTXC Task definitions - Default                12/28/99 08:32 */

#include "typedef.h"
#include "rtxstruc.h"

#include "ctask.h"

#define NULL 0


const TASK ntasks = NTASKS;
const TASK dntasks = DNTASKS;


const TASK startls[] =
{
   0  /* null terminated list */
};



const KTCB rtxktcb[1+NTASKS] =
{
  { NULL    , NULL, 0, 0 }, /* null task */
};

TCB ks_tblx rtxtcb[1+NTASKS+DNTASKS];

#define NAMMAX 8

const char taskname[1+NTASKS][NAMMAX+1] =
{
   ""
};
