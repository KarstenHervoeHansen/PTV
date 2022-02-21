/* csema.c - RTXC Semaphore definitions - pt5765 rev.1         03/25/99 23:04 */

#include "typedef.h"
#include "rtxstruc.h"

#include "csema.h"

#define SIGLISTSIZE 16 /* sema list size */

extern const SEMA near nsemas = NSEMAS;

extern const int near siglistsize = SIGLISTSIZE;

SEMA near siglist[SIGLISTSIZE];/* sema list */

SSTATE near semat[1+NSEMAS];

#define NAMMAX 8

extern const char semaname[1+NSEMAS][NAMMAX+1] =
{
   "",
   "KEY_T",
   "DISP_T",
   "STDI_QNE",
   "CBUGSEMA",
   "FRONT_T",
   "MODE_C",
   "LOCK_C",
   "ALLOCK_C",
   "SCAN_T",
   "TPS_T",
   "AGC_T",
   "INPUT_C",
   "FREQ_C",
   "ALARM1_C",
   "ALARM2_C",
   "ALARM3_C",
   "ALARM_T",
   "BERLIM_C",
   "LIMBER_C",
   "BER_T",
   "LEVLIM_C",
   "LIMLEV_C",
   "LEV_T",
   "EVLOG_F",
   "NOILIM_C",
   "LIMNOI_C",
   "NOISE_T",
   "WDOG_T",
   "REM_ISEM",
   "REM_OSEM",
   "KEY_SEM",
   "LED_SEM",
   "HW_SEM",
   "PAR_SEM",
   "DIS_SEM",
   "REAL_SEM",
   "NV_SEM",
   "FEC1_SEM",
   "FEC2_SEM",
   "COF_SEM",
   "ALRM_SEM",
   "TUNI_SEM",
   "TUNF_SEM"
};

/* End of file - csema.c */
