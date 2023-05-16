 /*
 ; DK-Technologies A/S
 ;
 ; FILE NAME        defins.h
 ; PURPOSE          definitions for pr94 project
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:
 ;
 ; NOTES:
 ;
 ;
*/

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long


#define MAX_WRITE_BYTE 256  /* max 939 */

typedef union xx
    {
    ulong longval;
    uint  intgval[2];
    uchar charval[4];
    }  ul;

typedef union yy
    {
    uint intval;
    uchar charv[2];
    }  ui;

