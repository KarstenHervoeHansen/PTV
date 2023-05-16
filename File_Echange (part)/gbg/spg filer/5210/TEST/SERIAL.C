#include <stdio.h>
#include <xa.h>             /* define 8051 registers */

/*
Date	: 960326
Version	: 0.0
Author	: PRC
HISTORY:
*/

#include "vars.c"

#define FifoLength  64


struct
{ char Buffer[FifoLength];
  unsigned char Input;
  unsigned char Output;
} RxBuf0;

struct
{ char Buffer[FifoLength];
  unsigned char Input;
  unsigned char Output;
} RxBuf1;

void Serial_init(void)
{
//      SCON = 0x90;       /* 10010000   Serial port mode 2 for 375/2 kbit/sec */
//      PCON = 0x00 ;      /* SMOD = 0; Not Bit-addressable */
//      PS = 1;            /* High prio. on serial interrupt  */

      RxBuf0.Input = 0;
      RxBuf0.Output = FifoLength - 1;
      RxBuf0.Buffer[0] = '\0';
      ERI0= 1;  // enables serial interrupt */
      ETI0= 1;	// enable transmit

      RxBuf1.Input = 0;
      RxBuf1.Output = FifoLength - 1;
      RxBuf1.Buffer[0] = '\0';
      ERI1= 1;  // enables serial interrupt */
      ETI1= 1;	// enable transmit
}


interrupt void Serial0_int(void)
{       while (RI0) {
          RI0 = 0;
          RxBuf0.Buffer[RxBuf0.Input = ++RxBuf0.Input % FifoLength] = S0BUF;
        }

        if (TI0) {               /* ignore transmit interrupts */
        }

}


interrupt void Serial1_int(void)
{       while (RI1) {
          RI1 = 0;
          RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength] = S1BUF;
        }

        if (TI1) {               /* ignore transmit interrupts */
        }

}
extern void write(UC *adr);
extern UC modtaget[];

put_char0 (char c)  {
    S0BUF = c;
    ERI0 = 0;         /* Disable serial interrupt  */
    while (!TI0);    /* while waiting for transmitter ready */
    TI0 = 0;
    ERI0 = 1;

}

put_char1 (char c)  {
    S1BUF = c;
    ERI1 = 0;         /* Disable serial interrupt  */
    while (!TI1);    /* while waiting for transmitter ready */
    TI1 = 0;
    ERI1 = 1;

}

char get_char1 (void) {
   if (RxBuf1.Input != RxBuf1.Output)
     return ( RxBuf1.Buffer [RxBuf1.Output = ++RxBuf1.Output % FifoLength]);
   else
     return (0);
}

void PutData(char A,char B,unsigned char x) {
   put_char0 (A);
   put_char0 (B);
   put_char0 (x/100 + '0');
   put_char0 ((x/10)%10 + '0');
   put_char0 (x%10 + '0');
   put_char0 ('\n');
}

void PutData1(char A,char B,unsigned char x) {
   put_char1 (A);
   put_char1 (B);
   put_char1 (x/100 + '0');
   put_char1 ((x/10)%10 + '0');
   put_char1 (x%10 + '0');
   put_char1 ('\n');
}

void PutData16(char A,char B,unsigned int x) {
   put_char0 (A);
   put_char0 (B);
   put_char0 (x/10000 + '0');
   put_char0 ((x/1000)%10 + '0');
   put_char0 ((x/100)%10 + '0');
   put_char0 ((x/10)%10 + '0');
   put_char0 (x%10 + '0');
   put_char0 ('\n');
}

// PutCmd(char A,char B) {
//    put_char (A);
//    put_char (B);
//    put_char ('\n');
// }

void PutStr(const char *st)
{ unsigned char i;
  i=0;
  while (st[i] != '\0')  put_char0(st[i++]);
  put_char0('\n');
}
