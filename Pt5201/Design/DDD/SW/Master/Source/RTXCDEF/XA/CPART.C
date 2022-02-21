/* cpart.c - RTXC Partition definitions - Default           12/28/99 08:34 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cpart.h"

const MAP nparts = NPARTS;
const MAP dnparts = DNPARTS;


PHEADER ks_tblx pheader[1+NPARTS+DNPARTS];

const PKHEADER pkheader[1+NPARTS] = 
{
   { (struct xmap *)0, (size_t)0, 0 }
};

#define NAMMAX 8

const char partname[1+NPARTS][NAMMAX+1] =
{
   ""
};
