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

#define RDY      (UC) 0x33
#define START    (UC) 0x31
#define STOP     (UC) 0x32
#define ERR      (UC) 0x20
#define SUCCESS  (UC) 0x1B
#define ENQ      (UC) 0xAA
#define CSUMFAIL (UC) 0xF0

#define CRC8CONST     0xB8
#define STARTCRCVALUE 0x2A

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
    UC   bytes[2];
    }  ui;

