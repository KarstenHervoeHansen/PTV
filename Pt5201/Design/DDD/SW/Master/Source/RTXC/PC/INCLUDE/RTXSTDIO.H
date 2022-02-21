/* rtxstdio.h */

/*
 *   RTXC Example Drivers Version 1.0a
 *   Copyright (c) 1994-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXSTDIO_H
#define _RTXSTDIO_H

#include "rtxcapi.h"
#include "stdarg.h"

/* BSP prototypes */
int bspinit();	
int bspstop();

#if __cplusplus
extern "C" {
#endif

typedef void FILE;

#define PRINT_BUF_SIZE 128
#define UNGET_BUF_SIZE   8
#undef  USE_PORT2

#define EOF  -1

typedef struct
{
  int            in;
  unsigned char  buf[UNGET_BUF_SIZE];
} UNGETB_Struct;

typedef struct
{
  int           devno;     /* logical device number */ 
  TASK          rxtask;    /* receive driver task */
  TASK          txtask;    /* transmit driver task */
  RESOURCE      resource;  /* device resource */
  SEMA          isema;     /* input semaphore */
  SEMA          osema;     /* output semaphore */ 
  QUEUE         inq;       /* input queue */ 
  QUEUE         outq;      /* output queue */
  void          *idcbp;    /* init device control block pointer */
} CONCB_Struct;

typedef struct
{
  CONCB_Struct   *conc1cb;   /* uart 1 control block */
  CONCB_Struct   *conc2cb;   /* uart 2 control block */
} IOCB_Struct;

typedef struct
{
  short   ctick;   /* CLKTICK - Clock tick */
  short   crate;   /* CLKRATE - Clock rate */
} CLKCB_Struct;

typedef struct
{
  SEMA    sema;   /* PRN Semaphore */
  QUEUE   queue;  /* PRN Queue */
} PRNCB_Struct;

typedef struct
{
  CLKCB_Struct   *clkcb;     /* timer 1 control block */
  PRNCB_Struct   *prncb;     /* print driver control block */
} HWCB_Struct;

extern void *const stdin;
extern void *const stdout;
extern void *const stderr;

extern int vsprintf(char *, const char *, va_list);
extern int sprintf(char *, const char *, ...);
extern int sscanf(const char *, const char *, ...);

extern int fprintf(FILE *, const char *msg, ...);
extern int printf(const char *msg, ...);

extern int defconc(FILE *, CONCB_Struct *);

extern int feditln(FILE *, char *, int);
extern int fckinput(FILE *);
extern int fgetc(FILE *);
extern int fgetc_t(FILE *, TICKS);
extern int ungetc(int c, FILE *file);

extern int fputc(int , FILE *);
extern int fputs(char *, FILE *);

extern fflushport(FILE *, int);

extern KSRC flockport(FILE *);
extern KSRC funlockport(FILE *);

#define PORT1    ((FILE *)0)      /* Maps to DOS Console */
#define PORT2    ((FILE *)1)      /* Maps to COM x */

#undef  USE_PORT2

#define BACKSP_KEY 0x08
#define RETURN_KEY 0x0D
#define DELETE_KEY 0x7F
#define BELL       0x07

#define FLUSH_IN     0
#define FLUSH_OUT    1
#define FLUSH_IO     2

#define editln(a,b)  feditln(stdout, a, b)
#define ckinput()    fckinput(stdout)
#define getc_w()     fgetc(stdout)
#define getc_t(a)    fgetc_t(stdout, a)
#define lockport()   flockport(stdout)
#define unlockport() funlockport(stdout)
#define flushport(a)  fflushport(stdout, a)

#ifndef USE_STDIO
#define getc()       fgetc(stdout)
#define putc(c)      fputc(c, stdout)
#define puts(b)      fputs(b, stdout)
#endif


#if __cplusplus
}
#endif

#endif

/* end of rtxstdio.h */

