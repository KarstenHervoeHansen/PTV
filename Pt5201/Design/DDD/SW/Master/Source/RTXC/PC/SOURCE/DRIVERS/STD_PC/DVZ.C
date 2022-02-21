/* dvz.c - divide by zero exception handler */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxcapi.h"

extern TCB near * near hipritsk;
extern TASK near brktask;
extern char near brkflag;

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

FRAME *dvzc(FRAME *frame)
{
   unsigned char *pc;
   unsigned char opcode;
   int mod, rm;
   int pcadjust = 0; /* adjustment to skip over bad instr */

   pc = (unsigned char *)(frame->pc);

   opcode = *pc;

   switch(opcode) /* check for segment override prefix */
   {
       case 0x2e: /* CS: */
       case 0x3e: /* DS: */
       case 0x26: /* ES: */
       case 0x36: /* SS: */
          pcadjust++;
          pc++;
          break;
   }

   pc++; /* skip opcode */

   mod = (*pc & 0xc0) >> 6;

   pcadjust++; /* for opcode */

   switch(opcode)
   {
       case 0xf6: /* hit IDIV or DIV instr */
       case 0xf7:
          switch(mod)
          {
             case 0: /* no DISP unless r/m = 6 */
                rm = *pc & 0x7;
                if (rm == 6)
                   pcadjust += 2;
                break;

             case 1: /* 1 byte DISP extended to 16 bits */
                pcadjust += 2;
                break;

             case 2: /* 2 byte DISP */
                pcadjust += 3;
                break;

             case 3: /* REG field only */
                pcadjust++;
                break;
          }
          break;

       default: /* major bug */
          break;
   }

   brktask = hipritsk->task; /* log errant task # */

   brkflag = 2; /* indicate div by zero exception */

   (char *)(frame->pc) += pcadjust; /* skip over instruction */

   return(KS_ISRexit(frame, 0)); /* no sema to post, just return */
}

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

/* End of file - dvz.c */
