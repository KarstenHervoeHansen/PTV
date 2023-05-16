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

// Set address for receiving
#define FifoLength  64
#define UC unsigned char
#define UI unsigned int
#define UL unsigned long
extern near UC myaddress;

static bit SM2_1 @ 0x325;
static bit TB8_1 @ 0x323;

near struct
{  char Buffer[FifoLength];	// define receive-buffer
   unsigned char Input;
   unsigned char Output;
}  RxBuf1;

near UC Command[20];	// command buffer
near UC CmdIndex;	// index to buffer
near UC rstate;		// state of receive-machine
near UC rep_no;		// no. of resend
near UC cksum;		// checksum of sent command
// ******************************************************************

void Serial1_init(void)
{  SM2_1= 1;	// ready for address
   RxBuf1.Input = 0;
   RxBuf1.Output = FifoLength - 1;
   RxBuf1.Buffer[0] = '\0';
   rstate= 0;
   SM2_1= 1;	// ready for address input
//  ERI1= 1;  	// enable serial interrupt
//  ETI1= 1;		// enable transmit
}
// ******************************************************************

banked interrupt void Serial1_int(void)
{  UC c;
        while (RI1)
        { c= S1BUF; RI1 = 0;
	  if (rstate==2)		// receive checksum
	  {  if (S1BUF== (cksum & 0x7f))
	     {  S1BUF= 0x80;
		SWR1= 1;		// request SW-interrupt
	     }
	     else S1BUF= 0x81;	// error in checksum
	     SM2_1= 1;	// ready for address
	     rstate= 0;
	  }
	  else if (rstate==1)		// receive data
	  {  Command[CmdIndex++]= c; cksum += c;
	     if (c==0x0A)
	     {	rstate= 2; Command[CmdIndex]= 0;
	     }
	  }
	  else if (rstate==0)		// receive address
	  {  if (S1BUF==myaddress)
	     { SM2_1= 0; rstate= 1;
	       CmdIndex= 0; cksum= myaddress;
	     } // ready for data
	  }
        }

//        if (TI1) {               /* ignore transmit interrupts */
//        }
}
// ******************************************************************

interrupt void cmdmove(void)
{  UC c,i;
   SWR1= 0;
   i= 0;		// move command to cyclic buffer
   while ((c= Command[i++])!=0)
     RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength] = c;
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
