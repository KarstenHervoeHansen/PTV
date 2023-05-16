/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: SERI1.C
* Version	: 1.1
* Purpose	: RS232 drivers for RX1/TX1 (port P1.4/P1.5)
* Org.date	: 960703
* Author	: PRC
********************************************************************
HISTORY:
970114 RELEASED 1.1
961111 Return_long and return_int added.
961014 Communication secured with checksum
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

// near UC c;

static bit SM2_1 @ 0x325;
static bit TB8_1 @ 0x323;

near struct
{  UC Buffer[FifoLength];	// define receive-buffer
   UC Input;
   UC Output;
}  RxBuf1;

// near UC Command[20];	// command buffer
// near UC CmdIndex;	// index to buffer
near UC rstate;		// state of receive-machine
// near UC rep_no;		// no. of resend
near UC rcksum;		// checksum of received command
near UC cksum;		// checksum of sent command

// ******************************************************************

void serial1_init(void)
{  SM2_1= 1;	// ready for address
   RxBuf1.Input = 0;
   RxBuf1.Output = FifoLength - 1;
   RxBuf1.Buffer[0] = '\0';
   rstate= 0;
   ERI1= 1;  	// enable serial interrupt
   RI1= 0;
}
// ******************************************************************
static near UC rd;	// byte just read from V24

interrupt void serial1_int(void)
{  if (RI1)
   {  RI1 = 0;
      rd= S1BUF;
      SWR |= 1;	// start SW-interrupt 1 to do rest of serial routine.
   }
   else if (TI1)  TI1= 0;
}
// ******************************************************************

void put_char1 (UC c)  {
   cksum += c;
   EA= 0;
   S1BUF = c;	// send the char
   TI1 = 0;	// correction for new AX-processor UART
   EA= 1;
   ERI1 = 0;    // Disable serial interrupt
   while (!TI1);    // while waiting for transmitter ready
   TI1 = 0;
   ERI1 = 1;
}
// ******************************************************************

interrupt void serial_end(void)
{  SWR1= 0;			//  SW1 intr request off
   if (rstate==2)		// receive checksum
   {  if (rd== (rcksum & 0x7f))
      {  put_char1(0x80); //S1BUF= 0x80;
         RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength]= 0x80;
      }
      else
      {  put_char1(0x81); //S1BUF= 0x81;	// error in checksum
         RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength]= 0x81;
      }
      SM2_1= 1;	// ready for address
      rstate= 0;
   }
   else if (rstate==1)		// receive data
   {
      rcksum += rd;
      RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength] = rd;
      if (rd==0x0A||rd==0x0D||rd==';'||rd=='?'||rd=='$'||rd=='!')
      {	 rstate= 2;		// delimiter found, ready for checksum
      }
   }
   else if (rstate==0)		// receive address
   {  if (S1BUF==myaddress)
      {  SM2_1= 0;		// ready for data
         rstate= 1;
	 rcksum= 0;
      }
   }
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

void return_char(UC x)
{  cksum= 0;
   TB8_1= 0;		// clear bit for data transmit
   pause1(500);			// wait for Master to recover
   put_char1 ((x/100)%10 + '0');
   put_char1 ((x/10)%10 + '0');
   put_char1 (x%10 + '0');
   put_char1 ('\n');
   put_char1(cksum & 0x7f);		// checksum
}
// ******************************************************************

void return_string(UC *s)
{  UC i, c;
   cksum= 0;
   TB8_1= 0;		// clear bit for data transmit
   pause1(500);			// wait for Master to recover
   put_char1(34);		// quote (")
   i= 0;
   while ((c= s[i] & 0x7f)!='\0' && i<10)
   {  put_char1(c);
      i++;
   }
/*   for (i=0;i<10;i++)
   {  c= s[i] & 0x7f;		// read string
      put_char1(c);		// send char
   }
*/
   put_char1(34);		// quote (")
   put_char1 ('\n');
   put_char1(cksum & 0x7f);		// checksum
}
// ******************************************************************

void return_long(UL x)
{  cksum= 0;
   TB8_1= 0;		// clear bit for data transmit
   pause1(500);			// wait for Master to recover
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
   put_char1(cksum & 0x7f);		// checksum
}
// ******************************************************************

void return_int(UI y)
{  cksum= 0;
   TB8_1= 0;		// clear bit for data transmit
   pause1(500);			// wait for Master to recover
   put_char1 (y/10000 + '0');
   put_char1 ((y/1000)%10 + '0');
   put_char1 ((y/100)%10 + '0');
   put_char1 ((y/10)%10 + '0');
   put_char1 (y%10 + '0');
   put_char1 ('\n');
   put_char1(cksum & 0x7f);		// checksum
}
// ******************************************************************

void return_long_int(UL x, UI y)
{  cksum= 0;
   TB8_1= 0;		// clear bit for data transmit
   pause1(500);			// wait for Master to recover
   // send the long
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
   put_char1(',');
   // send the integer
   put_char1 (y/10000 + '0');
   put_char1 ((y/1000)%10 + '0');
   put_char1 ((y/100)%10 + '0');
   put_char1 ((y/10)%10 + '0');
   put_char1 (y%10 + '0');
   put_char1 ('\n');
   put_char1(cksum & 0x7f);		// checksum
}




/*
// ******************************************************************

void sndchar(UC adr, char A, char B, UC x)
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

void sndint(UC adr, char A, char B, UI x)
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

void sndlong(UC adr, char A, char B, UL x)
{  cksum= 0;
   TB8_1= 1;		// set bit for address transmit
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

void sndvoid(UC adr, char A, char B)
{  TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   put_char1 (A);
   put_char1 (B);
   put_char1 ('\n');
}
// ******************************************************************

void sndasc(UC adr, char A, char B, const char *st)
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
*/
