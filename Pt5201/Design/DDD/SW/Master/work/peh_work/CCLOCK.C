/* cclock.c - RTXC Clock definitions - Default              02/25/99 09:51 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cclock.h"

const int ntmrs = NTMRS;
const int clktick = CLKTICK;
const int clkrate = CLKRATE;

CLKBLK ks_clkx clkq[NTMRS];
