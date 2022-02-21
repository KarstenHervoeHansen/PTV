/* br.c: program to calculate reload values for T2 for given baud rate */
/* used in blinky.c which runs on F226 Test Board                      */

#include <stdio.h>


#define CRYSTAL           16.666E6        /* 29.4912E6 */

#define TIMER2_RANGE (unsigned long)65536

#define PRESCALER    (unsigned long)1       /* see Fig 17.13 *T2 Mode 2 Block Diagram */
                                             
#define BAUDRATE     (unsigned long) (76800)    /* select 57600 baud */
                                            
#define RELOAD2      (TIMER2_RANGE - (CRYSTAL/(BAUDRATE*PRESCALER*(unsigned long)32)))


void main(void)
  {
  unsigned long relt2;

  relt2 = RELOAD2;
  

  printf("RELOAD2 = %lx\n", relt2);
  }


