/* setvect.c - set/get vector */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*
 * note, Exception Vector Table may be at a fixed or variable location.
 *       #define EVT below as needed for your system
*/

/* CONFIG */
#define EVT 0 /* exception vector table address */

/* direct exception vector table manipulation */
long far getvect(int vector)
{
  long far *p = (long far *)EVT; /* "long *" is used for convenience */
                                 /* isrs are really "void (*)(void)" */

  p += vector; /* calculate address of proper vector */

  return(*p); /* return old isr */
}

/* direct exception vector table manipulation */
void far setvect(int vector, void interrupt (far *newVector)(void))
{
  long far *p = (long far *)EVT; /* "long *" is used for convenience */
                                  /* isrs are really "void (*)(void)" */

  p += vector; /* calculate address of proper vector */

  *p = (long)newVector; /* establish new isr */
}

/* End of file - setvect.c */
