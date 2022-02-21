/* cvtdate.c - date/time functions */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/


// Corrected by TK 990901 from  "  25202L; "  to  "  25203L; "  Basetime offset.


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

#define SECS_PER_MIN  (60L)
#define MINS_PER_HOUR (60L)
#define SECS_PER_HOUR (MINS_PER_HOUR * SECS_PER_MIN)
#define HOURS_PER_DAY (24L)
#define SECS_PER_DAY  (HOURS_PER_DAY * SECS_PER_HOUR)
#define DAYS_PER_YEAR (365L)

/*

     Function to compute a time_t value when given the calendar time in
     a  time structure.  The pointer to the time structure is passed as
     the argument to the function.

*/

time_t date2systime(struct time_tm *p)
{
   long j;

   j = (long)(p->tm_yr - 1901) * DAYS_PER_YEAR + (p->tm_yr - 1901) / 4 +
       ((p->tm_mon * 30L) + (p->tm_mon * 145L) / 256) + p->tm_day - 30;

   if (p->tm_mon >= 3) /* february */
   {
      j--;
      if (p->tm_yr & 0x0003)
         j--;
   }
   j -= 25203L; /* adjust for base date of 1-Jan-1970 */

   if (p->tm_isdst)
      p->tm_hr--;

   j = ((((long)j * HOURS_PER_DAY) + (long)p->tm_hr) * 60L +
           (long)p->tm_min) * 60L + (long)p->tm_sec;

   return(j);
}

/*
 
     Function  to compute  the members  of the  time structure  given a
     time_t.  The function fills in the members of the structure except
     for  the isdst  flag indicating  if the  current time  is Daylight
     Savings Time  (DST) which  must be  supplied by  the caller.   The
     calendar is accurate until Monday, 18-Jan-2038 at 3:14:07.
 
*/

void systime2date(time_t n, struct time_tm *p)
{
   time_t systime;

   if (p->tm_isdst)
      n += SECS_PER_HOUR;

   systime = n / SECS_PER_DAY;  /* divide time by 86400 to get day number */
   n -= systime * SECS_PER_DAY; /* leaves the time-of-day in number of seconds */

   p->tm_hr = (int)(n / SECS_PER_HOUR);

   n -= (long)p->tm_hr * SECS_PER_HOUR;
   p->tm_min = (int)(n / MINS_PER_HOUR);

   p->tm_sec = (int)(n - (long)p->tm_min * MINS_PER_HOUR);

   systime += 25203L; /* adjust for base date of 1-Jan-1970 */
   n = (systime * 1000L) / 365251L; /* 365.251 days/year - the hard way */

   p->tm_yr = 1901 + (int)n;
   if ((n = systime - n * 36525L / 100L + 30) >= 90)
   {
      if ((p->tm_yr & 0x0003) != 0)
         n = n + 2;
      else if (n != 90)
         n++;
   }

   p->tm_mon = (int)(n * 100L / 3057L); /* 30.57 days/mo - the hard way */

   p->tm_day = (int)(n - (p->tm_mon * 3057L) / 100L);

   if ((p->tm_wday = (int)(systime - ((systime / 7) * 7))) == 0)
      p->tm_wday = 7;

   if ((p->tm_wday = p->tm_wday + 2) > 7)
      p->tm_wday -= 7;
}
