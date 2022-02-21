/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef FPUTASKS_H
#define FPUTASKS_H

#define SELFTASK   ((TASK)0)

#define FPU1OQ  CONOQ
#define FPU1IQ  CONIQ

#define FPU2OQ  CONOQ
#define FPU2IQ  CONIQ

#define FPURES  CONRES

#if defined(FPU) || defined(FP_EMU)
#define FACTOR     1
#define LP1_FACTOR 3  /* 500 */
#define LP2_FACTOR 250L
#endif

/*
   Function Prototypes
*/
#if defined(FPU)
extern double dsquare(double);
extern float fsquare(float);
#endif

#endif

/* End of file - fputasks.h */
