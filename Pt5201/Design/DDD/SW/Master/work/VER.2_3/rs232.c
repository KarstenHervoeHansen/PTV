/****************************************************************************/
/* MODULE:                                                                  */
/*   rs232.c - RS232 channel driver using UART1 and Timer1                  */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
    void rs232i(void);
    void rs232o(void);
/*                                                                          */
/* NOTES:                                                                   */
/*  000405: Increased input stream from 128 to 512									 */
/*  000403: Error in rs232printf(): buf was NOT deleted return with error.	 */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <periphal.h>
#include "crystal.h"
#include "rs232.h"


#define TIMER1_RANGE  (unsigned long)65536     /* UART mode 2 using Timer 1 */

#define PRESCALER       (unsigned long)4     /* selected also in SCR */

#define BAUDRATE     (unsigned long)9600     /* select  9600 baud */

#define RELOAD1 (unsigned short)(TIMER1_RANGE-(CRYSTAL/(BAUDRATE*PRESCALER*(unsigned long)16)))
#define THI1_RESET (RELOAD1 >> 8) & 0xFF
#define TLOW1_RESET (RELOAD1 & 0xFF)

/* =================== */
/* S1CON - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define U1_SM0   (0 << 7)    /* allow for 8-bit UART */
#define U1_SM1   (1 << 6)    /* allow for 8-bit UART */
#define U1_SM2   (0 << 5)    /* allow for 8-bit UART */
#define U1_REN_0 (0 << 4)    /* enable serial reception */
     
#define U1_TB8_0 (0 << 3)    /* clear to disallow 9 bit data */
#define U1_RB8_0 (0 << 2)    /* clear to disallow 9 bit data */
     
#define U1_TI_0  (0 << 1)   /* the transmit interrupt flag */
#define U1_RI_0  (0 << 0)   /* the receive interrupt flag */ 

#define xS1CON (U1_SM0 | U1_SM1 | U1_SM2 | U1_REN_0 | U1_TB8_0 | U1_RB8_0 | U1_TI_0 | U1_RI_0 )
/* =================== */

/* =================== */
/* TMOD - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define T0_GATE (0 << 3)   /* not applicable - OR in a zero (0) */
#define T0_CT   (0 << 2)   /* not applicable - OR in a zero (0) */
#define T0_M1   (0 << 1)   /* not applicable - OR in a zero (0) */
#define T0_M0   (0 << 0)   /* not applicable - OR in a zero (0) */

#define T0_TMOD  (T0_GATE | T0_CT | T0_M1 | T0_M0)

/* ------------------- */
/* TIMER 1             */
/* ------------------- */
#define T1_GATE (0 << 7)   /* disable gate control */
#define T1_CT   (0 << 6)   /* cleared for timer operation */
#define T1_M1   (0 << 5)   /* 16-bit auto-reload */
#define T1_M0   (0 << 4)   /* 16-bit auto-reload */

#define T1_TMOD  (T1_GATE | T1_CT | T1_M1 | T1_M0)

#define xTMOD  (T0_TMOD | T1_TMOD)
/* =================== */

/* =================== */
/* TCON - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define T0_TF0  (0 << 5)   /* not applicable - OR in a zero (0) */
#define T0_TR0  (0 << 4)   /* not applicable - OR in a zero (0) */
#define T0_IE0  (0 << 1)   /* not applicable - OR in a zero (0) */
#define T0_IT0  (0 << 0)   /* not applicable - OR in a zero (0) */

#define T0_CON (T0_TF0 | T0_TR0 | T0_IE0 | T0_IT0)

/* ------------------- */
/* TIMER 1             */
/* ------------------- */
#define T1_TF1  (0 << 7)   /* overflow flag - manipulated by hardware */
#define T1_TR1  (1 << 6)   /* run control bit - enabled */
#define T1_IE1  (0 << 3)   /* interrupt edge flag - enabled */
#define T1_IT1  (0 << 2)   /* interrupt type control bit - disabled */

#define T1_CON (T1_TF1 | T1_TR1 | T1_IE1 | T1_IT1)

#define xTCON (T0_CON | T1_CON)
/* =================== */


static union
{
	struct
	{
	unsigned RI:1;
	unsigned TI:1;
	unsigned RB8:1;
	unsigned TB8:1;
	signed Enable:1;
	unsigned Multi:1;
	unsigned Mode:2;
	}Bits;
	unsigned char Byte;
}_S1CON;


static union
{
	struct
	{
	unsigned EnableInt:1;
	unsigned OverFlowErr:1;
	unsigned BreakInt:1;
	unsigned FramingErr:1;
	unsigned :4;
	}Bits;
	char Byte;
}_S1STAT;


static union
{
	struct
	{
	unsigned DataBits:2;
	unsigned StopBits:1;
	unsigned Parity:2;
	unsigned HandShk:2;
	}Bits;
	char Byte;
}LCR;



static unsigned char ichar[2]; /* input char */

static TEvent			*rs232iEvent, *rs232oEvent;
static TThread		*rs232iThread, *rs232oThread;
static TResource	*rs232oRes;
static TStream		*rs232iStream, *rs232oStream;

/*#define CalcParity \
	#pragma asm \
	PUSH	R4L \
	MOV.B	R4L,_par \
	MOV.B	_par,PSW51 \
	AND.B	_par,#0FH \
	POP	R4L \
	#pragma endasm
  */
//#define EvenParity(Bte) par=(Bte); CalcParity par==1 ? 1 : 0	

//#define OddParity(Byte) par=(Byte); CalcParity (par)==0 ? 1 : 0	


//static unsigned char par;

char CheckParity(unsigned char Byte)
{
	#pragma asm_noflush 
	PUSH	R4L 
	MOV.B	R4L,R0L 
	MOV.B	R0L,PSW51 
	AND.B	R0L,#01H 
	POP	R4L 
	#pragma endasm
	return(Byte);
}	


_inline void SetOutputChar(char Ch)
{
 if(LCR.Bits.Parity)	          // if parity
 {
  TB8_1=CheckParity(Ch);          // TB8_1 = parity
  if(LCR.Bits.Parity==1)	  // if odd parity
   TB8_1=~TB8_1;	          // invert parity bit
  if (LCR.Bits.DataBits==2)       // if 7 data-bits
   if (TB8_1)		          // if parity bit
    Ch|=0x80;                     //  set parity bit
   else		         	  // else
    Ch&=0x7F;                     //  clear parity bit
 }
 else
 {
  if (LCR.Bits.DataBits==2)       // if 7 data-bits
   Ch|=0x80;                      //  set extra stop bit
 }
}	


_inline void SetInputChar(register unsigned char* Ch0,register unsigned char* Ch1)
{
 if (LCR.Bits.DataBits==3)	  // if 8 databits
  *Ch1=RB8_1;	                  //  get parity bit
 else			          // else
 {
  *((int*)Ch0) <<= 1;             //  shift parity bit to Ch+1
  *Ch0 >>= 1;			  //  shift back
//  *Ch &= 0x7F;
 }
 if(LCR.Bits.Parity==3)	          // if even parity
 {
  *Ch1 ^= CheckParity(*Ch0);	  //  Ch+1 = Ch+1 XOR Parity
 }
 else if(LCR.Bits.Parity==1)      // else if odd parity
  *Ch1 ^= (~CheckParity(*Ch0)) & 0x01;	//  Ch+1 = Ch+1 XOR NOT(Parity)
 else				  // else
  *Ch1 = 0; 	                  //  Ch+1 = 0  
}



/*
 *
 * interrupt service for transmit 0
 *
*/
FRAME ks_stk *int232o(FRAME ks_stk * frame)
{
   TI_1 = 0;
   return(KS_ISRexit(frame, rs232oEvent->ID)); /* wake out output task */
}


/*
 *
 * interrupt service for receive 0
 *
*/
FRAME ks_stk *int232i(FRAME ks_stk * frame)
{

   register unsigned char* Ch0=&ichar[0];
   register unsigned char* Ch1=&ichar[1];

   RI_1 = 0;

   *Ch0 = S1BUF;

   if (*Ch1=(S1STAT & 0x0E) <ERR_OVERFLOW)	 // if no errors
   {
    SetInputChar(Ch0,Ch1);
   }
   S1STAT = _S1STAT.Byte;         // clear status byte
   return(KS_ISRexit(frame, rs232iEvent->ID)); /* wake out input task */
}


static unsigned char junk;

void rs232_init(void)
{

  rs232iEvent=newTEvent(0);
  rs232oEvent=newTEvent(0);

  rs232oRes=newTResource(0);

  rs232iStream=newTStream(2,512);
  rs232oStream=newTStream(1,64);

  rs232iThread=newTThread(1,rs232i); 
  rs232oThread=newTThread(3,rs232o);          // CHG. 2 to 3 KEN 270100

  rs232iThread->Execute(rs232iThread);
  rs232oThread->Execute(rs232oThread);


  /* IEL - Interrupt Enable Register */
  ET1 = 0;    /* Timer 1 enable bit  - disnabled */

  /* S1CON - serial port 1 control register */
  S1CON = _S1CON.Byte = xS1CON;

  /* S1STAT - serial port 1 control/status register */
  _S1STAT.Byte=0x01;
  S1STAT = _S1STAT.Byte;
  
  /* Timer/Counter Mode Control Register */
  TMOD &= 0x0F;   /* clear timer1 settings */
  TMOD |= xTMOD;
  
  /* Timer/Counter Control Register */
  TCON &= 0x3F;   /* clear timer1 settings */
  TCON |= xTCON;

  /* Interrupt priority 5 */
  IPA5 = (TX1INTLVL<<4) + RX1INTLVL;

  rs232Setup(9600,8,'N','N');	


  junk = S1BUF; /* read input to clear */
  junk = S1BUF; /* read input to clear */

}


void rs232_stop(void)
{
}



 static bool echo=false;


/* TASK */
void rs232i(void)
{
  for (;;)
  {
    rs232iEvent->Wait(rs232iEvent,0);

    if (ichar[0]=='\r')
	   ichar[0]='\n';

    if (ichar[0]=='@')
    {
      if (echo)
      {
        echo=false;
        rs232puts("Echo OFF.\r\n");
      }
      else
      {
       echo=true;
        rs232puts("Echo ON.\n");
      }
    }
    else
    {
      if (echo)
      {
        rs232putchar(ichar[0]);
      }

      rs232iStream->Put(rs232iStream,&ichar);
    }
  }
}



/* TASK */
void rs232o(void)
{
  char ochar; /* output char */

  for (;;)
  {
    rs232oStream->Get(rs232oStream,&ochar);

    if ((ochar == '\n') && (echo))
    {
      SetOutputChar('\r');
      S1BUF = '\r'; 
   TI_1 = 0;
	   rs232oEvent->Wait(rs232oEvent,0);
    }

    SetOutputChar(ochar);

    S1BUF = ochar; /* write char to port */
   TI_1 = 0;

    rs232oEvent->Wait(rs232oEvent,0);
  }
}


int rs232Setup(unsigned long Baud, unsigned char Data, unsigned char Parity, unsigned char Hndshk)
{
  union
  {
    unsigned int  Int;
	 unsigned char Byte[2];
  }BaudRate;

  BaudRate.Int=(unsigned short)(TIMER1_RANGE-(CRYSTAL/(Baud*PRESCALER*(unsigned long)16)));

  switch ( Parity)
  {
    case 'N':
	   Parity = 0;
	   break;

	 case 'O':
	   Parity = 1;
	   break;

	 case 'E':
	   Parity = 3;
	   break;

	 default:
      return( 0);
  }

  switch ( Hndshk)
  {
    case 'N':
	   Parity = 0;
	   break;

	 case 'X':
	   Parity = 1;
	   break;

	 case 'H':
	   Parity = 2;
	   break;

	 default:
      return( 0);
  }

  if ( Data == 7)
    LCR.Bits.DataBits = 2;
  else if ( Data == 8)
    LCR.Bits.DataBits = 3;
  else
    return( 0);

  LCR.Bits.Parity = Parity;
  LCR.Bits.HandShk = Hndshk;

  ETI1 = 0;   // transmit port 1 - disabled
  ERI1 = 0;   // receive port 1 - disabled
  REN_1= 0;
  TR1  = 0;

  TL1 = RTL1 = BaudRate.Byte[0];  // Timer/Counter 1
  TH1 = RTH1 = BaudRate.Byte[1];

	if (( LCR.Bits.DataBits == 2) || ( LCR.Bits.Parity == 0))     // if databits==7 || parity==None
		_S1CON.Bits.Mode = 1;	    //  8 bit serial mode
	else					             // else
		_S1CON.Bits.Mode = 3;	    //  9 bit serial mode
	S1CON = _S1CON.Byte;	          // write S1CON register

	KS_purgequeue(rs232oStream->ID);
	KS_purgequeue(rs232iStream->ID);

	TR1  = 1;
	REN_1= 1;
	ERI1 = 1;   // receive port 1 - enabled
	ETI1 = 1;   // transmit port 1 - enabled

	return(-1);
}



int rs232putchar(unsigned char ch)
{
	rs232oRes->Lock( rs232oRes);
	rs232oStream->Put( rs232oStream, &ch);
	rs232oRes->Unlock( rs232oRes);

	return( ch);
}	 



int rs232puts( char* str)
{
  char* s = str;
 
  rs232oRes->Lock( rs232oRes);

  while( *str)
  {
    rs232oStream->Put( rs232oStream, str);
    str++;
  }

  rs232oRes->Unlock( rs232oRes);

  return( str-s);
}	

int rs232printf( int cnt, const char* format, ...)
{
  va_list ptr;
  char *buf, *tmpbuf;
  int ret;

	if ( rs232oRes && rs232oStream)
	{
  		rs232oRes->Lock( rs232oRes);

    	tmpbuf = buf = new( cnt+16);	

    	va_start( ptr, format);
    	ret = vsprintf( buf, format, ptr);
    	va_end( ptr);

   	if (( ret == EOF) || ( ret > cnt+16))	// KEn 000403
		{
			delete( tmpbuf);

			rs232oRes->Unlock( rs232oRes);

			return( EOF);
		}												  	// End KEn 000403

    	for ( ret = 0 ; ret <= cnt, *buf > 0; ret++, buf++)
      	rs232oStream->Put( rs232oStream, buf);

		delete( tmpbuf);
    	rs232oRes->Unlock( rs232oRes);

    	return( ret);
	}
  	else
  	{
   	buf = new( cnt+16);	

    	va_start( ptr, format);
    	ret = vsprintf( buf, format, ptr);
    	va_end( ptr);

    	delete( buf);

    	return( EOF);
 	}
}	 



static char LastError;


int rs232getchar(void)
{
  char dummy[2];

  if ( rs232iStream)
  {
    rs232iStream->Get( rs232iStream, &dummy);

    if ( dummy[1])
	   LastError = dummy[1];

    return( dummy[0]);
  }
  else
  {
    return( -1);
  }
}




int rs232GetLastError( void)
{
  char LE = LastError;

  LastError = 0;

  return( LE);
}
