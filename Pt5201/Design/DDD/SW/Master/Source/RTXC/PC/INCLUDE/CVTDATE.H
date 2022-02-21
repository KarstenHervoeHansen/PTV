/* cvtdate.h */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1994.
 *   Embedded System Products, Inc.
 *   11501 Chimney Rock
 *   Houston, TX 77035-2905
 *   (713) 728-9688
 *   ALL RIGHTS RESERVED
*/

#include "typedef.h"

struct time_tm
{
   int tm_yr;
   int tm_mon;
   int tm_day;
   int tm_wday;
   int tm_sec;
   int tm_min;
   int tm_hr;
   int tm_isdst;
};

time_t date2systime(struct time_tm *);
void systime2date(time_t, struct time_tm *);
