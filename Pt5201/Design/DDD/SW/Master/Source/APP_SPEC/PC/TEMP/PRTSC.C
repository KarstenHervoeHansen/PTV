/* prtsc.c - print screen task */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*

    This  demonstration task performs a similar  function as the PrtSc (Print
Screen)  key under MS-DOS.   One major difference is  that multiple PrtSc key
requests are serviced  quickly  (without  waiting  on  the  printer)  due  to
buffering the screen image and printing under interrupt service.  Also, print
in  progress from another task will not be interrupted by a screen dump since
the PRN resource is protected.

    Basically, it copies  the video  ram buffer  to the  printer.   The video
controller  must be in  text mode or  else random printed  output will occur.
This is a primitive function and must be configured for the proper video type
to locate the  video ram buffer.   In order to accommodate  a wide variety of
printers,  most graphic characters (corners, tees,  etc.) are translated to a
reasonable facsimile using dashes, plus, etc.

*/

#include <dos.h>
#include <ctype.h>

/* CONFIG */
#include "prtsc.h"

#include "rtxcapi.h"

#include "csema.h"  /* PRTSCSEM */
#include "cqueue.h" /* PRNQ or COMOQ */
#include "cpart.h" /* PRTSCMAP (2000=80x25 bytes each) */
#include "cres.h"   /* PRNRES */

#define NULL     ((void *)0)
#define SELFTASK ((TASK)0)

#define NROWS    25
#define NCOLS    80

#define PRTSCINT 0x05     /* PrtSc key interrupt vector on PC */

#define EGABASE 0xb8000000L
#define MDABASE 0xb0000000L

#ifdef MDA
#define BASE MDABASE
#endif

#ifdef EGA
#define BASE EGABASE
#endif

#ifndef BASE
#define BASE MDABASE
#endif

#ifdef __cplusplus /* { */
extern "C" {

static void interrupt (*old)(...);
extern void interrupt isvcprtsc(...); /* asm level prn svc */

}
#else
static void interrupt (*old)();
extern void interrupt isvcprtsc(); /* asm level prn svc */
#endif /* } __cplusplus */


static void prtscstart(void);
void prtscstop(void);

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

FRAME *prtscc(FRAME * frame)
{
   return(KS_ISRexit(frame, PRTSCSEM)); /* signal task and return */
}

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */


/* PRTSc driver start hook */
static void prtscstart()
{
   old = getvect(PRTSCINT);      /* save  PrtSc vector */
   setvect(PRTSCINT, isvcprtsc); /* claim PrtSc vector from DOS */
}

/* PRTSc driver stop hook */
void prtscstop(void)
{
   setvect(PRTSCINT, old); /* restore vector for DOS */
}

/* TASK - print screen driver */
void prtsc(void)
{
   register int i;
   unsigned char *p; /* ptr into Video Ram */
   unsigned char *b; /* ptr into copy of Video Ram (w/o attribute) */

   prtscstart();
   if ((b = (unsigned char *)KS_alloc(PRTSCMAP)) == NULL)
      KS_terminate(SELFTASK);

   for (;;)
   {
      KS_wait(PRTSCSEM);
      p = (unsigned char *)BASE;
      for (i = 0; i < NROWS*NCOLS; i++)
      {
         *b++ = *p++;
         p++; /* ignore colors/attributes */
      }
      b -= NROWS * NCOLS;
      KS_enqueuew(PRTSCQ, (char *)&b);
      b = (unsigned char *)KS_allocw(PRTSCMAP);
   }
}

static char buffer[NCOLS];

/* TASK */
void prtdump(void)
{
   int row, col;
   unsigned char *b; /* screen buffer */

   for (;;)
   {
      KS_dequeuew(PRTSCQ, (char *)&b);
      KS_lockw(PRNRES);
      for (row = 0; row < NROWS; row++)
      {
         for (col = 0; col < NCOLS; col++, b++)
         {
            if (isprint(*b))   /* if non graphics char */
               buffer[col] = *b;
            else
               switch(*b)
               {
                  case(218): /* ulchar */
                  case(214):
                  case(213):
                  case(201):

                  case(191): /* urchar */
                  case(183):
                  case(184):
                  case(187):

                  case(217): /* lrchar */
                  case(189):
                  case(190):
                  case(188):

                  case(192): /* llchar */
                  case(211):
                  case(212):
                  case(200):

                  case(195): /* left t bar */
                  case(180): /* right t bar */
                     buffer[col] = '+';
                     break;

                  case(196): /* horizontal */
                  case(205):
                     buffer[col] = '-';
                     break;

                  case(179): /* vertical */
                  case(186):
                     buffer[col] = '|';
                     break;
               }
         }

         KS_enqueuew(OQ, "*"); /* indicate non-DOS version */
         KS_enqueuew(OQ, "");

         for (col = 0; col < NCOLS; col++)
            KS_enqueuew(OQ, &buffer[col]);
         KS_enqueuew(OQ, "\r");
         KS_enqueuew(OQ, "\n");
      }
      KS_unlock(PRNRES);
      b -= NROWS * NCOLS;
      KS_free(PRTSCMAP, b);
   }
}

/* End of file - prtsc.c */
