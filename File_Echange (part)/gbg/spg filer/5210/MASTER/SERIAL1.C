/*
********************************************************************
* Project	: PT5210  MASTER
* Filename	: SERIAL1.C
* Version	: 0.0	961209
* Purpose	: RS232 drivers for RX1/TX1 (port P1.4/P1.5)
* Org.date	: 960903
* Author	: PRC
********************************************************************
HISTORY:
961209 AnswerCheck now returns an UC. Transmission to units terminates
	with an ';' instead of \0A
961129 Termination (0) added to AnswerBuf. No save of delimiter.
961111 Added SndRequest_n(), SndSpecial(), SndAsc_n()
961101 Added SndRequest() routine.
961031 Added request answering handling and more delimiters.
961014 Added Re-send. A checksum is sent after each command. If answer
	is not 0x80 within 2 ms, the command is repeated (3 times).
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

#define MaxTransmit 3
//#define TIME_OUT	10000
#define TIME_OUT	20000
// timeout: each 1000 is appr. 2 ms

extern near UC myaddress;

static bit SM2_1 @ 0x325;
static bit TB8_1 @ 0x323;
static bit request;

near struct
{  char Buffer[FifoLength];	// define receive-buffer
   unsigned char Input;
   unsigned char Output;
}  RxBuf1;

near UC CmdToSend[20];	// command buffer
near UC CmdIndex;	// index to buffer
near UC rstate;		// state of receive-machine
near UC rep_no;		// no. of resend
near UC cksum;		// checksum of sent command
near UC v24stat;	// status for transmission
// *****************************************************************

void Serial1_init(void)
{
   RxBuf1.Input = 0;
   RxBuf1.Output = FifoLength - 1;
   RxBuf1.Buffer[0] = '\0';
   SWE |= 2;		// enable 'wait_for_answer' interrupt
   ERI1= 1;  	// enable serial interrupt
   SM2_1= 0;
}
// *****************************************************************

near char AnswerBuf[30]; // global buffer to save answer and checksum
bit AnswerError;	// global flag for checksum error in answer
static UI answertout;	// local time-out counter

#define False 0
#define True 1
// *****************************************************************


interrupt void Serial1_int(void)
{       while (RI1) {
          RI1 = 0;
          RxBuf1.Buffer[RxBuf1.Input = ++RxBuf1.Input % FifoLength] = S1BUF;
        }

        if (TI1) {               /* ignore transmit interrupts */
        }

}
/*
interrupt void Serial1_int(void)
{  UC c;
   static near UC bufindex;

   while (RI1)
   {  c= S1BUF; RI1 = 0;	// save received byte
      if (rstate==1)		// awaits answer
      {  if (c==0x80)
	 {  if (request)  rstate= 4;
            else  rstate= 0;
	    rep_no= 0;		// reset repetition counter
	    cksum= 0;		// prepare check of answer (if any)
	    bufindex= 0;	// prepare answer buffer
	    v24stat= 0;		// OK-flag for transmit
	 }
	 else if (c==0x81)	// error in checksum
	 {  v24stat= 1;		// set checksum error flag
	    rstate= 2;		// enter re-send state
	 }
      }

      else if (rstate==5)		// receive checksum
      {  AnswerBuf[bufindex]= '\0';	// terminate
         if (c== (cksum & 0x7f))
            AnswerError= False;		// checksum OK
         else AnswerError= True;	// error in checksum
         rstate= 0;			// back to initial state
	 request= 0;			// request flag off
      }

      else if (rstate==4)		// receive data
      {  answertout= TIME_OUT;		// refresh time-out counter
         cksum += c;
         if (c==0x0A || c==0x0D || c==';' || c=='!' || c== '?' || c=='$')
            rstate= 5;			// wait for checksum
         else AnswerBuf[bufindex++]= c;	// save byte
      }
   }
   if (TI1)   TI1= 0;
}
*/
// *****************************************************************

UC AnswerCheck(void)
{  answertout= TIME_OUT;	// set time-out delay (is refreshed
   while(answertout>0 && rstate>0)	//    for each byte received)
      answertout--;

   if (answertout==0)
      return(2);		// Time-out gives error
   else 
      return ( (UC) AnswerError) ;
}
// *****************************************************************

void pause1(UI tid)
{  while(tid>0)  tid--;		// appr. 110 us for each 100
}
// *****************************************************************

void waitanswer(void)
// (former Low priority (SW)interrupt,) called from Serial1_int.
// Purpose is to wait for command-confirm code from units and re-send
// command in case of checksum error or time-out.

{  UI tel;
   while (rstate>0 && rstate<3)
   {  if (rstate==1)
      {  tel= TIME_OUT;	// wait up to appr. 2 ms
         while (rstate==1 && --tel>0) ; // wait until answer or timeout
         if (tel==0)
         {  rstate= 2;
	    v24stat= 2;	// time-out
         }
      }
      if (rstate==2)
      {  CmdIndex= 0;	// resend command
         rep_no += 1;	// count re-sends
         if (rep_no>=MaxTransmit)// stop after 2 times resend
         {  rstate= 0;	// Sorry! receiver would not respond
	    rep_no= 0;	// this is a communication error!
         }
         else // if (CmdToSend[0]!=0)
         {  TB8_1= 1;	// set bit for address transmit
   	    S1BUF= CmdToSend[CmdIndex++];	// send address
   	    ERI1= 0;       // Disable serial interrupt
    	    while (!TI1);  // while waiting for transmitter ready
   	    TI1= 0;
   	    ERI1= 1;
            TB8_1= 0;	// clear bit for data transmit
            pause1(100);	// wait for slaves to evaluate address
     	    while (CmdToSend[CmdIndex]!=0)
	    {  S1BUF= CmdToSend[CmdIndex++];	// send next byte
   	       ERI1 = 0;   // Disable serial interrupt 
   	       while (!TI1);	// while waiting for transmitter ready
   	       TI1 = 0;
   	       ERI1 = 1;
            }
	    rstate= 1;	// wait for status answer
         }
      }
   }
}
// *****************************************************************

void put_char1 (UC c)  {
   CmdToSend[CmdIndex++]= c;
   cksum += c;
   S1BUF = c;
   ERI1 = 0;         /* Disable serial interrupt  */
   while (!TI1);    /* while waiting for transmitter ready */
   TI1 = 0;
   ERI1 = 1;
}
// *****************************************************************

char get_char1 (void) {
   if (RxBuf1.Input != RxBuf1.Output)
     return ( RxBuf1.Buffer [RxBuf1.Output = ++RxBuf1.Output % FifoLength]);
   else
     return (0);
}
// *****************************************************************

UC SndChar(UC adr, char *A, UC x)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait (app.180us) for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   pause1(50);		// wait (app.180us)
   put_char1 (*A);
   pause1(50);		// wait (app.180us)
   put_char1 (x/100 + '0');
   pause1(50);		// wait (app.180us)
   put_char1 ((x/10)%10 + '0');
   pause1(50);		// wait (app.180us)
   put_char1 (x%10 + '0');
   pause1(50);		// wait (app.180us)
   put_char1 (';');
   pause1(50);		// wait (app.180us)
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndInt(UC adr, char *A, UI x)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   put_char1 (x/10000 + '0');
   put_char1 ((x/1000)%10 + '0');
   put_char1 ((x/100)%10 + '0');
   put_char1 ((x/10)%10 + '0');
   put_char1 (x%10 + '0');
   put_char1 (';');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndLong(UC adr, char *A, UL x)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
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
   put_char1 (';');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndVoid(UC adr, char *A)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   put_char1 (';');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndAsc(UC adr, char *A, const char *st)
{  unsigned char i;
   cksum= 0;
   CmdIndex= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   i=0;

   put_char1 ('"');		// quote the string
   while (st[i] != '\0' && i<8)  put_char1(st[i++]);
   put_char1 ('"');		// unquote the string

   put_char1 (';');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************
UC SndRequest(UC adr, char *A)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   put_char1 ('?');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 1;			// set flag for request wanted
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndRequest_n(UC adr, char *A, UC n)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   put_char1 (n+'0');
   put_char1 ('?');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 1;			// set flag for request wanted
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndSpecial(UC adr, char *A, char C)
{  CmdIndex= 0;
   cksum= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   put_char1 (C);
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}
// *****************************************************************

UC SndAsc_n(UC adr, char *A, UC C, const char *st)
{  unsigned char i;
   cksum= 0;
   CmdIndex= 0;
   TB8_1= 1;		// set bit for address transmit
   put_char1(adr);	// send address
   TB8_1= 0;		// clear bit for data transmit
   pause1(100);		// wait for slaves to evaluate address
   cksum= 0;
   put_char1 (*A++);
   put_char1 (*A);
   put_char1 (C+'0');
   put_char1 (',');

   put_char1 ('"');		// quote the string
   i=0;
   while (st[i] != '\0' && i<10)  put_char1(st[i++]);

   put_char1 ('"');		// unquote the string
   put_char1 (';');
   put_char1 (cksum & 0x7f);
   rstate= 1;			// state for 'wait for answer'
   request= 0;			// request flag off
   CmdToSend[CmdIndex]= 0;	// end the command string
   waitanswer();		// wait for answer (with re-send)
   return(v24stat);   
}

