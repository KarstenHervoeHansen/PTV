/* fputasks.c */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxcapi.h"

#include "cres.h"     /* CONRES  */
#include "cqueue.h"   /* CONOQ   */
#include "cclock.h"   /* CLKTICK */
//#include "math.h"
#include "fputasks.h"

extern void printl(const char *, RESOURCE, QUEUE);
extern void ltoa(long, char *, int);

#if defined(FPU) || defined(FP_EMU)
double dsquare(double);
float fsquare(float);
#endif

/* TASK */
void fputask1(void)
{
#if defined(FPU) || defined(FP_EMU)
   int i, j;
   float fi, fsum, fsumsq;
   double di, dsum, dsumsq;

#if defined(FP_EMU) /* { */
	init_FPU(SELFTASK);
#endif /* } FP_EMU */

   printl("FPU task 1: running\n", FPURES, FPU1OQ);
   KS_defslice(SELFTASK,3);
   KS_delay(SELFTASK,1000/CLKTICK);
   KS_yield();

   for(;;)
   {
      for(j = 0; j < LP1_FACTOR; j++)
      {
         for(fi  = 1.0, fsum = 0.0, fsumsq = 0.0,
             di  = 1.0, dsum = 0.0, dsumsq = 0.0,
             i  = 1;i <= LP1_FACTOR; fi += 1.0, di += 1.0, i++)
         {
            fsum   += fi;
            fsumsq += (fi * fi);

            dsum   += di;
            dsumsq += (di * di);

            if (fsquare(fi) != fi * fi)
               printl("FPU task 1: float squaring error\n", FPURES, FPU1OQ);

            if (dsquare(di) != di * di)
               printl("FPU task 1: dbl squaring error\n", FPURES, FPU1OQ);
         }

         if (fsum != (LP1_FACTOR * (LP1_FACTOR + 1) ) / 2 )
            printl("FPU task 1: float summing error\n", FPURES, FPU1OQ);
//       else
//          printl("FPU task 1: Passed float summing\n", FPURES, FPU1OQ);

         if (dsum != (LP1_FACTOR * (LP1_FACTOR + 1) ) / 2 )
            printl("FPU task 1: dbl summing error\n", FPURES, FPU1OQ);
//       else
//          printl("FPU task 1: Passed dbl summing\n", FPURES, FPU1OQ);

      }
   }
#else
   printl("FPU task 1 terminating\n", FPURES, FPU1OQ);
   KS_terminate(SELFTASK);
#endif    /* } if !FPU */
}

void fputask2(void)
{
#if defined(FPU) || defined(FP_EMU)
   int i, j;
   float fi, fj;
   double di, dj;

#if defined(FP_EMU) /* { */
	init_FPU(SELFTASK);
#endif /* } FP_EMU */

   printl("FPU task 2: running\n", FPURES, FPU2OQ);

   KS_defslice(SELFTASK,5);
   KS_delay(SELFTASK,2);
   KS_yield();

   for(;;)
   {
      for(j = 0; j < LP2_FACTOR; j++)
      {
         for(fi  = 1.0, fj  = 0.0,
             di  = 1.0, dj  = 0.0, i  = 1;
             i <= LP2_FACTOR;
             fi += 1.0, di += 1.0, i++)
         {
            fj += (fi * fi);
            dj += di;
         }

         if (dj != (LP2_FACTOR * (LP2_FACTOR + 1) ) / 2)
            printl("FPU task 2: summing error\n", FPURES, FPU2OQ);
//       else
//          printl("FPU task 2: no errors\n", FPURES, FPU2OQ);

      }
   }
#else
   printl("FPU task 2 terminating\n", FPURES, FPU2OQ);
   KS_terminate(SELFTASK);
#endif /* } if !FPU */
}

#if defined(FPU) || defined(FP_EMU)
#ifdef DOCUMENT
 1 4 9 16 25  36  49 64  81
1 3 5 7  9  11  13  15 17

(n)**2 = (n-1)**2 + (n-1)*2 + 1
#endif

/* recursive solution (slow) */
double dsquare(double d)
{
   if (d == 1.0)
      return 1.0;

   return dsquare(d - 1.0) + (( d - 1.0) * 2.0) + 1.0;
}

float fsquare(float f)
{
   if (f == 1.0f)
      return 1.0f;

   return fsquare(f - 1.0f) + ((f - 1.0f) * 2.0f) + 1.0f;
}
#endif

/*   End of file - fputasks.c */


