/* cres.c - RTXC Resource definitions - Default             12/28/99 08:33 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cres.h"

const RESOURCE nres = NRES+DNRES;

RHEADER ks_tblx rheader[1+NRES+DNRES];

#define NAMMAX 8

const char resname[1+NRES][NAMMAX+1] =
{
   ""
};
