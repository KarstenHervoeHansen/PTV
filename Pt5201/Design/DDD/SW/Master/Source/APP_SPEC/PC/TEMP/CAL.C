/* calendar task - cal.c */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxcapi.h"

#include "cclock.h" /* CLKTICK */
#include "csema.h"  /* CALSEMA */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/* time support routines */
extern long time(long *);
extern struct tm * localtime(long *);

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

struct tm
{
   int tm_sec;
   int tm_min;
   int tm_hour;
   int tm_mday;
   int tm_mon;
   int tm_year;
   int tm_wday;
   int tm_yday;
   int tm_isdst;
};

struct tod
{
   int hour;   /* hour - 0-23                    */
   int min;    /* minute - 0-59                  */
   int sec;    /* second - 0-59                  */
   int mon;    /* month - 1-12                   */
   int day;    /* day of month - 1-31            */
   int year;   /* year (current year minus 1900) */
};

struct tod dattim;

static int maxday[] = /* # days in months 1-12 */
{
   0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

#define FREQ 18.206481    /* DOS clock frequency in Hz */
#define TPM (int)(60.0*FREQ) /* DOS ticks per minute */

static int t0sec;     /* time at startup */

/* TASK */
void cal(void)
{
   long ltime;
   int hour, min, sec, mon, day, year;
   struct tm *t;            /* structure tm defined in <time.h>    */
   unsigned long far *timer_low = (unsigned long far *)0x046c; /* BIOS */
   int roundoff = 0;
   CLKBLK ks_clk *pclkblk = KS_alloc_timer();

   time(&ltime);            /* get system time in long word format */

   t = localtime(&ltime);        /* convert to separate fields in  */
                                  /* structure pointed to by t      */

   dattim.hour = t->tm_hour;     /* move into global structure     */
   dattim.min  = t->tm_min;
   dattim.sec  = t->tm_sec;
   dattim.mon  = t->tm_mon + 1;  /* localtime() uses 0-11 for month */
   dattim.day  = t->tm_mday;
   dattim.year = t->tm_year;

   t0sec = t->tm_sec; /* save second of startup for stop correction */

   /* start 1 sec cyclic timer */
   KS_start_timer(pclkblk, 1000/CLKTICK, 1000/CLKTICK, CALSEMA);

   for(;;)
   {
      KS_wait(CALSEMA);      /* wait for timer semaphore */

      sec = dattim.sec;
      if ((dattim.sec = (sec < 59) ? ++sec : 0) == 0)
      {
         min = dattim.min;

         /* BIOS std tick is approx 18.2/sec or 1092.4/minute   */
         /* for speed purposes, we keep roundoff factor and add */
         /* in 2 more ticks every 5 minutes to compensate       */

         if (++roundoff < 5)
            *timer_low += TPM; /* adjust BIOS/DOS ticks ctr by 1 min worth */
         else
         {
            roundoff = 0;
            *timer_low += TPM + 2; /* adjust for roundoff every 4 minutes */
         }

         if ((dattim.min = (min < 59) ? ++min : 0) == 0)
         {
            hour = dattim.hour;
            if ((dattim.hour = (hour < 23) ? ++hour : 0) == 0)
            {
               day  = dattim.day;
               mon  = dattim.mon;
               year = dattim.year + 1900;
               if (mon == 2 && year % 4 == 0 &&
                   year % 100 != 0 || year % 400 == 0)
                  maxday[2] = 29;
               else
                  maxday[2] = 28;
               if ((dattim.day = (day < maxday[mon]) ? ++day : 1) == 1)
                  if ((dattim.mon = (mon < 12) ? ++mon : 1) == 1)
                     dattim.year++;
            }
         }
      }
   }
}

/* to adjust BIOS time to be correct +/- 1/18.2 second when RTXC stops */
void calstop(void)
{
   unsigned long far *timer_low = (unsigned long far *)0x046c; /* BIOS */
   int diff;

   if (dattim.sec > t0sec)
      diff = dattim.sec - t0sec;
   else
      diff = (60 - t0sec) + dattim.sec;
   diff = (diff * (int)FREQ) / 60;

   *timer_low += diff; /* adjust BIOS/DOS tick ctr as needed */
}

/* End of file - cal.c */
