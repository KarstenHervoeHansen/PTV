/*******************************************************/
/* file: ports.c                                       */
/* abstract:  This file contains the routines to       */
/*            output values on the JTAG ports, to read */
/*            the TDO bit, and to read a byte of data  */
/*            from the prom                            */
/*                                                     */
/*  #define WIN95PP                                    */
/*******************************************************/

#include <C8051F320.h>
#include "define.h"
#include "ports.h"
#include "serial.h"

typedef union outPortUnion
  {
  UC value;
  struct opBitsStr
    {
    UC bit0: 1;
    UC bit1: 1;
    UC bit2: 1;
    UC bit3: 1; 
    UC tdi:  1; /* This is output bit: TDI  (P2.4) */
    UC tck:  1; /* This is output bit: TCK  (P2.5) */
    UC tms:  1; /* This is output bit: TMS  (P2.6) */
    UC one:  1; /* This is input  bit: TDO  (P2.7) */
    } bits;
  } outPortType;
  
typedef union inPortUnion
  {
  UC value;
  struct ipBitsStr
    {
    UC bit0: 1;
    UC bit1: 1;
    UC bit2: 1;
    UC bit3: 1;
    UC bit4: 1;
    UC bit5: 1;
    UC bit6: 1;
    UC tdo:  1; /* This is input  bit: TDO  (P2.7) */
    } bits;
  } inpPortType;

static inpPortType in_word;
static outPortType out_word;
static int         once = 0;


#include "io.c"

/* if in debugging mode, then just set the variables */
void setPort(UC p, UC val)
  {
  if (once == 0)
    {
    out_word.bits.one  = 1;
    once = 1;
    }
  if (p == TMS)
    out_word.bits.tms = val;

  if (p == TDI)
    out_word.bits.tdi = val;

  if (p == TCK)
    {
    out_word.bits.tck = val;
    P2 =  out_word.value;
    }
  }


/* toggle tck LH */
void pulseClock(void)
  {
  setPort(TCK, 0);  /* set the TCK port to low  */
  setPort(TCK, 1);  /* set the TCK port to high */
  }

/* read in a byte of data from the Serial Interface */
/* _getkey() returns  0 - if no char was available  */
void readByte(UC *mydata)
  {
  UC stat;

  stat = getch(mydata);
  }

/* read the TDO bit from port */
UC readTDOBit()
  {
  in_word.value = P2;
  if (in_word.bits.tdo == 0x1)
    return(1);
  else
    return(0);
  }


/* Wait at least the specified number of microsec.                           */
/* Use a timer if possible; otherwise estimate the number of instructions    */
/* necessary to be run based on the microcontroller speed.  For this example */
/* we pulse the TCK port a number of times based on the processor speed.     */
void waitTime(long microsec)
  {
  long i;

  //putch(STOP);

  if (microsec == 140000000L)
    { 
    microsec = 1000000; //7000000L; //14000000L / 2;
    for ( i = 0; i < microsec; ++i )
      {
      pulseClock();
      }
    return;
    }

  if (microsec == 1000L)
    {
    return;
#if 0
    microsec = 0; //14; //25; //50; //100; //150; //1000L / 5;
    for ( i = 0; i < microsec; ++i )
      {
      pulseClock();
      }
#endif
    return;
    }


  if (microsec == 15L)
    {
    return;
#if 0
    microsec = 0; //5; //10;
    for ( i = 0; i < microsec; ++i )
      {
      pulseClock();
      }
#endif
    }

  /* For systems with TCK rates >= 1 MHz;  This implementation is fine. */
  for (i = 0; i < microsec; ++i)
    {
    pulseClock();
    }
    //putch(START);
  }
