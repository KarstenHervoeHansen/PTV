/* cpart.c - RTXC Partition definitions - pt5765 rev.1         03/25/99 23:04 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cpart.h"

#define M1SZ 32 /* STR_PAR  - String_Partition     */
#define M1CT 20

#define M2SZ 4 /* LONG_PAR - Bit_Partition     
8 */
#define M2CT 256

extern const MAP near nparts = NPARTS;

extern const MAP near dnparts = DNPARTS;

static char far part1[M1CT*M1SZ];
static char far part2[M2CT*M2SZ];

PHEADER near pheader[1+NPARTS+DNPARTS];

extern const PKHEADER near pkheader[1+NPARTS] = 
{
   { (struct xmap *)0, (size_t)0, 0 },
   { (struct xmap *)&part1[0], (size_t)M1SZ, M1CT },
   { (struct xmap *)&part2[0], (size_t)M2SZ, M2CT }
};

#define NAMMAX 8

extern const char partname[1+NPARTS][NAMMAX+1] =
{
   "",
   "STR_PAR",
   "LONG_PAR"
};

/* End of file - cpart.c */
