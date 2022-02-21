/* cres.c - RTXC Resource definitions - pt5765 rev.1         03/25/99 23:04 */

#include "typedef.h"
#include "rtxstruc.h"

#include "cres.h"

extern const RESOURCE near nres = NRES+DNRES;

RHEADER near rheader[1+NRES+DNRES];

#define NAMMAX 8

extern const char resname[1+NRES][NAMMAX+1] =
{
   "",
   "SIORES",
   "MODE_RES",
   "FEND_RES",
   "ALAM_RES",
   "BER_RES",
   "LEVL_RES",
   "ELOG_RES",
   "NOIS_RES",
   "FRON_RES",
   "REMO_RES",
   "REM_IRES",
   "REM_ORES",
   "KEY_RES",
   "LED_RES",
   "HW_RES",
   "PAR_RES",
   "DIS_RES",
   "REAL_RES",
   "NV_RES",
   "FEC1_RES",
   "FEC2_RES",
   "COF_RES",
   "ALRM_RES",
   "TUNI_RES",
   "TUNF_RES"
};

/* End of file - cres.c */
