/* cpart.c - RTXC Partition definitions - PT5765 rev.1         04/15/99 16:32 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cpart.h"

extern const MAP near nparts = NPARTS;

extern const MAP near dnparts = DNPARTS;


PHEADER near pheader[1+NPARTS+DNPARTS];

extern const PKHEADER near pkheader[1+NPARTS] = 
{
   { (struct xmap *)0, (size_t)0, 0 }
};

#define NAMMAX 8

extern const char partname[1+NPARTS][NAMMAX+1] =
{
   ""
};

/* End of file - cpart.c */
