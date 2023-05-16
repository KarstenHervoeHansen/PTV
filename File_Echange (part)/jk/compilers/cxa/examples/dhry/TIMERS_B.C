/*****************************************************/
/* Various timer routines.                           */
/* Al Aburto, aburto@marlin.nosc.mil, 20 Dec 1992    */
/*                                                   */
/* t = dtime() outputs the current time in seconds.  */
/* Use CAUTION as some of these routines will mess   */
/* up when timing across the hour mark!!!            */
/*                                                   */
/* For timing I use the 'user' time whenever         */
/* possible. Using 'user+sys' time is a separate     */
/* issue.                                            */
/*                                                   */
/* Example Usage:                                    */
/* [timer options added here]                        */
/* main()                                            */
/* {                                                 */
/* double starttime,benchtime,dtime();               */
/*                                                   */
/* starttime = dtime();                              */ 
/* [routine to time]                                 */
/* benchtime = dtime() - starttime;                  */
/* }                                                 */
/*                                                   */
/* [timer code below added here]                     */
/*****************************************************/

/***************************************************************/
/* Timer options. You MUST uncomment one of the options below  */
/* or compile, for example, with the '-DUNIX' option.          */
/***************************************************************/
/* #define Amiga       */
/* #define UNIX        */
/* #define UNIX_Old    */
/* #define VMS         */
/* #define BORLAND_C   */
/* #define MSC         */
/* #define MAC         */
/* #define IPSC        */
/* #define FORTRAN_SEC */
/* #define GTODay      */
/* #define CTimer      */
/* #define UXPM        */

/*********************************/
/* Timer code.                   */
/*********************************/
/*******************/
/*  Amiga dtime()  */
/*******************/
#ifdef Amiga
#include <ctype.h>
#define HZ 50

double dtime()
{
   double q;

   struct   tt {
      long  days;
      long  minutes;
      long  ticks;
   } tt;

   DateStamp(&tt);

   q = ((double)(tt.ticks + (tt.minutes * 60L * 50L))) / (double)HZ;

   return q;
}
#endif

/*****************************************************/
/*  UNIX dtime(). This is the preferred UNIX timer.  */
/*  Provided by: Markku Kolkka, mk59200@cc.tut.fi    */
/*  HP-UX Addition by: Bo Thide', bt@irfu.se         */
/*****************************************************/
#ifdef UNIX
#include <sys/time.h>
#include <sys/resource.h>

#ifdef __hpux
#include <sys/syscall.h>
#define getrusage(a,b) syscall(SYS_getrusage,a,b)
#endif

struct rusage rusage;

double dtime()
{
   double q;

   getrusage(RUSAGE_SELF,&rusage);

   q = (double)(rusage.ru_utime.tv_sec);
   q = q + (double)(rusage.ru_utime.tv_usec) * 1.0e-06;
   
   return q;
}
#endif

/***************************************************/
/*  UNIX_Old dtime(). This is the old UNIX timer.  */
/*  Use only if absolutely necessary as HZ may be  */
/*  ill defined on your system.                    */
/***************************************************/
#ifdef UNIX_Old
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>

#ifndef HZ
#define HZ 60
#endif

struct tms tms;

double dtime()
{
   double q;

   times(&tms);

   q = (double)(tms.tms_utime) / (double)HZ;
   
   return q;
}
#endif

/*********************************************************/
/*  VMS dtime() for VMS systems.                         */
/*  Provided by: RAMO@uvphys.phys.UVic.CA                */
/*  Some people have run into problems with this timer.  */
/*********************************************************/
#ifdef VMS
#include time

#ifndef HZ
#define HZ 100
#endif

struct tbuffer_t
       {
	int proc_user_time;
	int proc_system_time;
	int child_user_time;
	int child_system_time;
       };
struct tbuffer_t tms;

double dtime()
{
   double q;

   times(&tms);

   q = (double)(tms.proc_user_time) / (double)HZ;
   
   return q;
}
#endif

/******************************/
/*  BORLAND C dtime() for DOS */
/******************************/
#ifdef BORLAND_C
#include <ctype.h>
#include <dos.h>
#include <time.h>

#define HZ 100
struct time tnow;

double dtime()
{
   double q;

   gettime(&tnow);

   q = 60.0 * (double)(tnow.ti_min);
   q = q + (double)(tnow.ti_sec);
   q = q + (double)(tnow.ti_hund)/(double)HZ;
   
   return q;
}
#endif

/**************************************/
/*  Microsoft C (MSC) dtime() for DOS */
/**************************************/
#ifdef MSC
#include <time.h>
#include <ctype.h>

#define HZ CLK_TCK
clock_t tnow;

double dtime()
{
   double q;

   tnow = clock();

   q = (double)tnow / (double)HZ;
   
   return q;
}
#endif

/*************************************/
/*  Macintosh (MAC) Think C dtime()  */
/*************************************/
#ifdef MAC
#include <time.h>

#define HZ 60

double dtime()
{
   double q;

   q = (double)clock() / (double)HZ;
   
   return q;
}
#endif

/************************************************************/
/*  iPSC/860 (IPSC) dtime() for i860.                       */
/*  Provided by: Dan Yergeau, yergeau@gloworm.Stanford.EDU  */
/************************************************************/
#ifdef IPSC
extern double dclock();

double dtime()
{
   double q;

   q = dclock();
   
   return q;
}
#endif

/**************************************************/
/*  FORTRAN dtime() for Cray type systems.        */
/*  This is the preferred timer for Cray systems. */
/**************************************************/
#ifdef FORTRAN_SEC

fortran double second();

double dtime()
{
   double q;

   second(&q);
   
   return q;
}
#endif

/***********************************************************/
/*  UNICOS C dtime() for Cray UNICOS systems.  Don't use   */
/*  unless absolutely necessary as returned time includes  */
/*  'user+system' time.  Provided by: R. Mike Dority,      */
/*  dority@craysea.cray.com                                */
/***********************************************************/
#ifdef CTimer
#include <time.h>

double dtime()
{
   double    q;
   clock_t   t;

       t = clock();

       q = (double)t / (double)CLOCKS_PER_SEC;

       return q;
}
#endif

/********************************************/
/* Another UNIX timer using gettimeofday(). */
/* However, getrusage() is preferred.       */
/********************************************/
#ifdef GTODay
#include <sys/time.h>

struct timeval tnow;

double dtime()
{
   double q;

   gettimeofday(&tnow,NULL);
   q = (double)tnow.tv_sec + (double)tnow.tv_usec * 1.0e-6;

   return q;
}
#endif

/*****************************************************/
/*  Fujitsu UXP/M timer.                             */
/*  Provided by: Mathew Lim, ANUSF, M.Lim@anu.edu.au */
/*****************************************************/
#ifdef UXPM
#include <sys/types.h>
#include <sys/timesu.h>
struct tmsu rusage;

double dtime()
{
   double q;

   timesu(&rusage);

   q = (double)(rusage.tms_utime) * 1.0e-06;
   
   return q;
}
#endif
