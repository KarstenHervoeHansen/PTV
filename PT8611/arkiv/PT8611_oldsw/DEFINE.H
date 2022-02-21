/* General declerations */
/* SAA 990325 */
/* added 'FAIL' to use as general purpose error indicator in return() */


#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

#define TRUE  -1
#define FALSE  0

#define OK    -1
#define FAIL   0

/* unions for unsigned long and unsigned int */
/* to access these data types bytewise       */
/* Note that for Kiel C compiler, charval[0] */
/* carries MSB and charval[3] carries LSB.   */
/* This is the opposite of how TC does it.   */

typedef union xx
    {
    long longval;
    int  intgval[2];
    UC   charval[4];
    }  ul;

typedef union yy
    {
    int  intval;
    UC   charv[2];
    }  ui;


typedef struct cc
  {
  UC format;
  ul delay;
  } CURR_PARAMS;


