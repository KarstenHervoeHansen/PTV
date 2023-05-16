#include <stdio.h>
#include <xa.h>             /* define XA registers */

/*
Date	: 960326
Version	: 0.0
Author	: PRC
HISTORY:
*/

#define FifoLength  32


near struct
{ char Buffer[FifoLength];
  unsigned char Input;
  unsigned char Output;
} RxBuf0;


void Serial0_init(void)
{
      RxBuf0.Input = 0;
      RxBuf0.Output = FifoLength - 1;
      RxBuf0.Buffer[0] = '\0';
}


interrupt void Serial0_int(void)
{       while (RI0)
	{ RI0 = 0;
          RxBuf0.Buffer[RxBuf0.Input = ++RxBuf0.Input % FifoLength] = S0BUF;
        }

        if (TI0) { }            /* ignore transmit interrupts */

}

void put_char0 (char c)  {
    S0BUF = c;
    ERI0 = 0;         /* Disable serial interrupt  */
    while (!TI0);    /* while waiting for transmitter ready */
    TI0 = 0;
    ERI0 = 1;

}

char get_char0 (void) {
   if (RxBuf0.Input != RxBuf0.Output)
     return ( RxBuf0.Buffer [RxBuf0.Output = ++RxBuf0.Output % FifoLength]);
   else
     return (0);
}

void PutData0(char A,char B,unsigned char x) {
   put_char0 (A);
   put_char0 (B);
   put_char0 (x/100 + '0');
   put_char0 ((x/10)%10 + '0');
   put_char0 (x%10 + '0');
   put_char0 ('\n');
}

void PutData0_16(char A,char B,unsigned int x) {
   put_char0 (A);
   put_char0 (B);
   put_char0 (x/10000 + '0');
   put_char0 ((x/1000)%10 + '0');
   put_char0 ((x/100)%10 + '0');
   put_char0 ((x/10)%10 + '0');
   put_char0 (x%10 + '0');
   put_char0 ('\n');
}

void PutStr0(const char *st)
{ unsigned char i;
  i=0;
  while (st[i] != '\0')  put_char0(st[i++]);
  put_char0('\n');
}
