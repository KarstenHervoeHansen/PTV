/* csema.c - RTXC Semaphore definitions - Default           12/28/99 08:32 */

#include "typedef.h"
#include "rtxstruc.h"

#include "csema.h"

#define SIGLISTSIZE 16 /* sema list size */

const SEMA nsemas = NSEMAS+DNSEMAS;

const int siglistsize = SIGLISTSIZE;

SEMA ks_datax siglist[SIGLISTSIZE];/* sema list */

SSTATE ks_tblx semat[1+NSEMAS+DNSEMAS];

#define NAMMAX 8

const char semaname[1+NSEMAS][NAMMAX+1] =
{
   ""
};
