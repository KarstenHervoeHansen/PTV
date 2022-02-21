/* prndrv.h - device specification file */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#define PRN1 /* valid choices are PRN1, PRN2 or PRNx */

#define NO_INTS /* use iff card does not support interrupts */

#ifdef PRN1
#define BASE    0x378  /* 0x378 for IBM LPT1 */
#define PRNINT  0x0F   /* LPT1 interrupt */
#define PRN_IRQ 7      /* IRQ7 */
#endif

#ifdef PRN2
#define BASE    0x278  /* 0x278 for IBM LPT2 */
#define PRNINT  0x0D   /* LPT2 interrupt */
#define PRN_IRQ 5      /* IRQ5 */
#endif

#ifdef PRNx
#define BASE    0x3bc  /* 0x3bc=MDA */
#define PRNINT  0x0D   /* LPT2 interrupt (usually N/A) */
#define PRN_IRQ 5      /* IRQ (N/A) */
#endif

#define PRN_INT_DISABLE (1<<PRN_IRQ)
#define PRN_INT_ENABLE  (~PRN_INT_DISABLE)

#define SELFTASK        ((TASK)0)

#define DATA_OUT        BASE+0 /* data output register */
#define STATUS          BASE+1 /* status register */
#define CONTROL         BASE+2 /* control register */

#define EOI             0x20 /* End of Interrupt command to 8259 */
#define PIC_EOI_ADDR    0x20
#define PIC_MASK_ADDR   0x21

#define INIT_RTXC       0x1c    /* interrupt enable */
#define INIT_DOS        0x0c    /* DOS std value, no interrupts */

/* these may vary from one LPT board to the next */
#define PRN_READY       0xc0
#define NO_PAPER        0x20
#define PRN_OFF         0x10

#define PAPER_TMOUT     500
#define OFF_TMOUT       100
#define CHAR_TMOUT      10

#ifdef NO_INTS
#define NORM_MODE       0x0c
#define SEND_CHAR       0x0d
#else
#define NORM_MODE       0x1c
#define SEND_CHAR       0x1d
#endif

/* End of file - prndrv.h */
