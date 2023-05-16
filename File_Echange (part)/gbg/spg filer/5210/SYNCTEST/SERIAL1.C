/* SERIAL1.C 	RS232 drivers for addressed communication on
		XA serial bus RX1/TX1 (port P1.4/P1.5)

Date	: 960326
Version	: 0.0
Author	: PRC
HISTORY:
960703 Routine names changed to Snd.. (Char, Int, Long, Asc, Void)
*/

#include <stdio.h>
#include <xa.h>             /* define 8051 registers */
#include <intrpt.h>
#include "equ.c"

// Set address for receiving
#define FifoLength  32
#define UC unsigned char
#define UI unsigned int
#define UL unsigned long
extern near UC myaddress;
extern near UC sersave;

static bit SM2_1 @ 0x325;
static bit TB8_1 @ 0x323;

near struct
{  char Buffer[FifoLength];	// define receive-buffer
   unsigned char Input;
   unsigned char Output;
}  RxBuf1;
// ******************************************************************

void Serial1_init(void)
{
//   SCON = 0x90;       // 10010000   Serial port mode 2 for 375/2 kbit/sec
//   PCON = 0x00 ;      // SMOD = 0; Not Bit-addressable
//   PS = 1;            // High prio. on serial interrupt

   SM2_1= 1;	// ready for address
      RxBuf1.Input = 0;
      RxBuf1.Output = FifoLength - 1;
      RxBuf1.Buffer[0] = '\0';
//    ERI1= 1;  	// enable serial interrupt
//    ETI1= 1;		// enable transmit
}
// ******************************************************************

banked interrupt void Serial1_int(void)
{       while (RI1)
        {  RI1 = 0;
	   sersave= S1BUF;
	}
   SWR |= 1;	// start SW-interrupt 1 for rest of serial routine.
}
// ******************************************************************

interrupt void serial_end(void)
{  SWR &= ~ser1req;
   if (S1BUF==myaddress)  SM2_1= 0;	// ready for data
   else if (SM2_1==0)
   { if (S1BUF==0x0a)  SM2_1= 1;	// ready for address
     RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength] = S1BUF;
   }
//        if (TI1) {               /* ignore transmit interrupts */
//        }

}

// ******************************************************************

void put_char1 (UC c)  {
    S1BUF = c;
    ERI1 = 0;         /* Disable serial interrupt  */
    while (!TI1);    /* while waiting for transmitter ready */
    TI1 = 0;
    ERI1 = 1;
}
// ******************************************************************

char get_char1 (void) {
   if (RxBuf1.Input != RxBuf1.Output)
     return ( RxBuf1.Buffer [RxBuf1.Output = ++RxBuf1.Output % FifoLength]);
   else
     return (0);
}
// ******************************************************************

void pause1(UI tid)
{  while(tid>0)  tid--;
}
// ******************************************************************

void SndChar(UC adr, char A, char B, UC x)
{  TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   put_char1 (A);
   put_char1 (B);
   put_char1 (x/100 + '0');
   put_char1 ((x/10)%10 + '0');
   put_char1 (x%10 + '0');
   put_char1 ('\n');
}
// ******************************************************************

void SndInt(UC adr, char A, char B, UI x)
{  TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   put_char1 (A);
   put_char1 (B);
   put_char1 (x/10000 + '0');
   put_char1 ((x/1000)%10 + '0');
   put_char1 ((x/100)%10 + '0');
   put_char1 ((x/10)%10 + '0');
   put_char1 (x%10 + '0');
   put_char1 ('\n');
}
// ******************************************************************

void SndLong(UC adr, char A, char B, UL x)
{  TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   put_char1 (A);
   put_char1 (B);
   put_char1 (x/1000000000 + '0');
   put_char1 ((x/100000000)%10 + '0');
   put_char1 ((x/10000000)%10 + '0');
   put_char1 ((x/1000000)%10 + '0');
   put_char1 ((x/100000)%10 + '0');
   put_char1 ((x/10000)%10 + '0');
   put_char1 ((x/1000)%10 + '0');
   put_char1 ((x/100)%10 + '0');
   put_char1 ((x/10)%10 + '0');
   put_char1 (x%10 + '0');
   put_char1 ('\n');
}
// ******************************************************************

void SndVoid(UC adr, char A, char B)
{  TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   put_char1 (A);
   put_char1 (B);
   put_char1 ('\n');
}
// ******************************************************************

void SndAsc(UC adr, char A, char B, const char *st)
{  unsigned char i;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   put_char1 (A);
   put_char1 (B);
   i=0;
   while (st[i] != '\0' && i<8)  put_char1(st[i++]);
   put_char1('\n');
}
// ******************************************************************
