/* condrv.c - PC CONsole keyboard driver/character oriented output */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*
 * note: task CONODRV must be higher priority than task CONIDRV if CON
 *       is to be Cbug port since PC CONsole is polled (not event driven)
*/

#include "rtxcapi.h"

#include "rtxstdio.h"

/* Control blocks */
extern CONCB_Struct CONCdcb[];
extern const CLKCB_Struct CLKdcb;
#define PORT 0 /* DOS console */

/* CONFIG */
#define POLLTMO 250 /* poll keyboard at 4 Hz */

#define SELFTASK ((TASK)0)

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

extern int getch(void);
extern int kbhit(void);
extern int printf(const char *, ...);

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#define GETCH getch()
#define KBHIT kbhit()
#define PUTCH(x) putch(x)
//#define PUTCH(x) printf("%c",x)

#ifdef CBUG
extern void sgnlcbug(void);
extern char near cbugflag;
#endif

/*
 * driver task to read PC keyboard and queue characters to CONIQ
 *
 * note: carriage returns are ignored
 *
*/

/* TASK */
void conidrv(void) /* input driver for PC CONsole */
{
   extern char near brkflag;
   char c;

   for(;;)
   {
      KS_lockw(CONCdcb[PORT].resource);
      while (KBHIT)     /* see if keyboard hit */
      {
         c = (char)GETCH;
         KS_unlock(CONCdcb[PORT].resource); /* unlock in case Cbug needs resource or q full */

         if (brkflag) /* ^C pseudo handler when compute bound */
            KS_terminate(SELFTASK);

#ifdef CBUG
         if (c == '!')
            sgnlcbug();     /* wake up Cbug */
         else
            KS_enqueuew(CONCdcb[PORT].inq, &c);
#else
         KS_enqueuew(CONCdcb[PORT].inq, &c);
#endif
         KS_lockw(CONCdcb[PORT].resource); /* re-lock for next poll */
      }
      KS_unlock(CONCdcb[PORT].resource);

#ifdef CBUG
      if (!cbugflag) /* don't delay if in Cbug since clock is not ticking! */
      {
         KS_delay(SELFTASK, POLLTMO/CLKdcb.ctick);
      }
#else
      KS_delay(SELFTASK, POLLTMO/CLKdcb.ctick);
#endif
   }
}

/*
 * driver task for PC CONsole output
 *
 * note: carriage returns (\r) are automatically inserted before
 *       linefeeds (\n)
*/

/* TASK */
void conodrv(void) /* output driver for PC CONsole */
{
   char ochar;

   for (;;)
   {
      KS_dequeuew(CONCdcb[PORT].outq, &ochar);

      KS_lockw(CONCdcb[PORT].resource);

      if (ochar == '\n')
      {
         PUTCH('\r');
         PUTCH('\n');
      }
      else
         PUTCH(ochar);

      KS_unlock(CONCdcb[PORT].resource);
   }
}

/* End of file - condrv.c */
