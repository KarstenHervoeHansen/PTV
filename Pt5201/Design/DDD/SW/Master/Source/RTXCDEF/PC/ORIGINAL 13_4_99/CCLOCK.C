/* cclock.c - RTXC Clock definitions - Default              03/25/99 23:05 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cclock.h"

extern const int near ntmrs = NTMRS;
extern const int near clktick = CLKTICK;
extern const int near clkrate = CLKRATE;

CLKBLK near clkq[NTMRS];

/* End of file - cclock.c */
