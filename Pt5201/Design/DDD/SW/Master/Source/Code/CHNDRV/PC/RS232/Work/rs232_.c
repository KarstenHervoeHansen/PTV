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
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5765    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/


#include "rs232.h"
#include <stdarg.h>
#include <stdio.h>
#include <dos.h>



#define COM2 /* valid choices are COM1, COM2, COM3, or COM4 */

#ifdef COM1
#define BASE    0x3f8 /* std COM1 */
#define COM_IRQ 4
#endif

#ifdef COM2
#define BASE    0x2f8 /* std COM2 */
#define COM_IRQ 3
#endif

#ifdef COM3
#define BASE    0x3e8 /* std COM3 */
#define COM_IRQ 5     /* 4 or 5 */
#endif

#ifdef COM4
#define BASE    0x2e8 /* std COM4 */
#define COM_IRQ 3     /* 3 or 7 */
#endif

#define UART_DLAB	 	0x80		/* devisor latch access bit  */

#define COMINT  8+COM_IRQ            /* interrupt # */
#define COM_INT_DISABLE (1<<COM_IRQ) /* PIC mask */
#define COM_INT_ENABLE  (~COM_INT_DISABLE)

#define UART_TBR        BASE+0 /* transmit buffer register */
#define UART_RBR        BASE+0 /* receive buffer register */
#define UART_DLR_LSB    BASE+0 /* divisor latch register LSB */
#define UART_DLR_MSB    BASE+1 /* divisor latch register MSB */
#define UART_INT_EN_REG BASE+1 /* interrupt enable register */
#define UART_INT_ID_REG BASE+2 /* interrupt id register */
#define UART_LCR        BASE+3 /* line control register */
#define UART_MCR        BASE+4 /* modem control register */
#define UART_LSR        BASE+5 /* line status register */
#define UART_MSR        BASE+6 /* modem status register */

#define TX_BUFF_EMPTY 0x02
#define RX_DATA_READY 0x04

#define RX_INT_ENABLE    0x01
#define TX_INT_ENABLE    0x02
#define ERR_INT_ENABLE   0x04
#define MSR_INT_ENABLE   0x08

#define INT_DISABLE      0x00

#define COM_MCR 0x0b /* DTR, RTS */

#define EOI           0x20
#define PIC_EOI_ADDR  0x20
#define PIC_MASK_ADDR 0x21


typedef enum
{
	Err, Rx, Tx, Status
}InterruptID;

static union
{
	struct
	{
	InterruptID ID:3;
	unsigned char :5;
	}Bits;
	char Byte;
}IIR;

static union
{
	struct
	{
	unsigned char DataReady:1;
	unsigned char THR_Empty:1;
	unsigned char Error:1;
	unsigned char MSR:1;
	unsigned char :5;
	}Bits;
	char Byte;
}IER;

static union
{
	struct
	{
	unsigned char DataBits:2;
	unsigned char StopBits:1;
	unsigned char Parity:2;
	unsigned char :2;
	unsigned char DLAB:1;
	}Bits;
	char Byte;
}LCR;

static union
{
	struct
	{
	unsigned char DTR:1;
	unsigned char RTS:1;
	unsigned char Out:2;
	unsigned char Loop:1;
	unsigned char :3;
	}Bits;
	char Byte;
}MCR;

static union
{
	struct
	{
	unsigned char DataReady:1;
	unsigned char OverflowErr:1;
	unsigned char ParityErr:1;
	unsigned char PackageErr:1;
	unsigned char BreakInt:1;
	unsigned char THR_Empty:1;
	unsigned char TSR_Empty:1;
	unsigned char :1;
	}Bits;
	char Byte;
}LSR;	

static union
{
	struct
	{
	unsigned char DeltaDTS:1;
	unsigned char DeltaDSR:1;
	unsigned char TrailingEdgeRing:1;
	unsigned char DeltaDCD:1;
	unsigned char CTS:1;
	unsigned char DSR:1;
	unsigned char Ring:1;
	unsigned char DCD:1;
	}Bits;
	char Byte;
}MSR;	


/* CONFIG */
#ifdef __cplusplus /* { */
extern "C" {

static void interrupt (*old)(...);
extern void interrupt isvccom(...); /* assembly level COM svc */

}
#else
static void interrupt (*old)();
extern void interrupt isvccom(); /* assembly level COM svc */
#endif /* } __cplusplus */



static TThread*	rs232iThread;
static TThread*	rs232oThread;
static TResource* rs232oRes;
static TEvent*	rs232iEvent;
static TEvent*	rs232oEvent;
static TStream*	rs232iStream;
static TStream*	rs232oStream;

static char ichar; /* last char read for input */
static char iErr; /* last char read for input */
static unsigned char oldregs[3];

/*
/* interrupt service for COM ports on IBM PC serial card
/*
/* Note, more than one interrupt may be pending (TX done, RX ready) at a
/* given time, so you must service "both" interrupts before exiting this
/* routine.  The USART will not re-request an interrupt if you only clear
/* one of them.  Thus the nasty logic for checking which one caused the
/* interrupt and then another check for duplicate interrupts.  Note also
/* that it may require two semaphores to be signaled (COMISEM and COMOSEM).
/* The KS_ISRsignal() call performs this secondary semaphore signal.
*/

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

FRAME *int232(FRAME *frame)
{
   InterruptID status;

   iErr=0;
   IIR.Byte = inportb(UART_INT_ID_REG); /* read hw status */
   if (IIR.Bits.ID == Rx) /* RX is higher priority w/in USART */
   {
      ichar = inportb(UART_RBR); /* read char, clear USART interrupt */
      IIR.Byte = inportb(UART_INT_ID_REG); /* re-read hw status */
      if (IIR.Bits.ID == Tx)
         KS_ISRsignal(rs232oEvent->ID); /* signal char output semaphore */
      outportb(PIC_EOI_ADDR, EOI); /* clear PIC interrupt */
      return(KS_ISRexit(frame, rs232iEvent->ID)); /* signal char input semaphore */
   }

   else if (IIR.Bits.ID == Tx)
   {
      IIR.Byte = inportb(UART_INT_ID_REG); /* re-read hw status */
      if (IIR.Bits.ID == Rx)
      {
         ichar = inportb(UART_RBR); /* read char, clear USART interrupt */
         KS_ISRsignal(rs232iEvent->ID); /* signal char input semaphore */
      }
      outportb(PIC_EOI_ADDR, EOI); /* clear PIC interrupt */
      return(KS_ISRexit(frame, rs232oEvent->ID)); /* exit signalling char output sema */
   }

   else if (IIR.Bits.ID == Err)
   {
      LSR.Byte=inportb(UART_LSR);
      if (LSR.Bits.OwerflowErr=1)
         iErr=ERR_OWERFLOW;
      if (LSR.Bits.ParityErr=1)
         iErr=ERR_PARITY;
      if (LSR.Bits.PackageErr=1)
         iErr=ERR_FRAMING;
      if (LSR.Bits.BreakInt=1)
         iErr=ERR_BREAK;
      ichar |= 0xFF; 
      IIR.Byte = inportb(UART_INT_ID_REG); /* re-read hw status */
      if (IIR.Bits.ID == Tx)
         KS_ISRsignal(rs232oEvent->ID); /* signal char output semaphore */
      outportb(PIC_EOI_ADDR, EOI); /* clear PIC interrupt */
      return(KS_ISRexit(frame, rs232iEvent->ID)); /* signal char input semaphore */
   }

   outportb(PIC_EOI_ADDR, EOI); /* clear interrupt */
   return(KS_ISRexit(frame, (SEMA)0));
}
#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */



/* COM driver start hook */
void rs232_init(void)
{
   unsigned char mask;

//   DeviceAdd(COM_Open,COM_Close,COM_IOControl,COM_Read,COM_Write,COM_Seek);
	rs232oRes=newTResource(0);
	rs232iEvent=newTEvent(0);
	rs232oEvent=newTEvent(0);
 
	rs232iStream=newTStream(1,128);
	rs232oStream=newTStream(1,64);
	
	rs232iThread=newTThread(1,rs232i);
	rs232oThread=newTThread(2,rs232o);
	rs232iThread->Execute(rs232iThread);
	rs232oThread->Execute(rs232oThread);

	old = getvect(COMINT);    /* save COM vector */
	setvect(COMINT, isvccom); /* claim COM vector */

	outportb(UART_MCR, COM_MCR);

	oldregs[2]=inportb(UART_LCR);
	outportb(UART_LCR, 0x83);  /* Setup port after writing DLAB=1 */
	oldregs[0]=inportb(UART_DLR_LSB); /* write to DLR */
	oldregs[1]=inportb(UART_DLR_MSB);
	outportb(UART_LCR, 0x03);  /*  DLAB=0 */
	rs232Setup(9600,8,'N','N');	
//   outportb(UART_DLR_LSB,12);
//   outportb(UART_DLR_MSB,0);
//   outportb(UART_LCR, inportb(UART_LCR) & ~UART_DLAB);  /* restore to DLAB=0 for normal operation */

	inportb(UART_RBR); /* dummy read to clear any TX interrupt */
	inportb(UART_INT_ID_REG); /* clear any RX interrupt */

	IER.Bits.DataReady=1;
	IER.Bits.THR_Empty=1;
	IER.Bits.Error=0;
	IER.Bits.MSR=0;
	outportb(UART_INT_EN_REG, IER.Byte); /* Enable TX/RX int */

	mask = inportb(PIC_MASK_ADDR);
	outportb(PIC_MASK_ADDR, mask & COM_INT_ENABLE); /* enable com interrupts */
}

/* COM driver stop hook - called before returning to DOS */
void rs232_stop(void)
{
   unsigned char mask;

   outportb(UART_INT_EN_REG, INT_DISABLE); /* disable TX/RX int */

   mask = inportb(PIC_MASK_ADDR);
   outportb(PIC_MASK_ADDR, mask | COM_INT_DISABLE); /* disable com interrupts */

   outportb(UART_LCR, inportb(UART_LCR) | UART_DLAB);  /* restore baudrate after writing DLAB=1 */
   outportb(UART_DLR_LSB,oldregs[0]);
   outportb(UART_DLR_MSB,oldregs[1]);
   outportb(UART_LCR,oldregs[2]);

   setvect(COMINT, old); /* restore COM vector */
}


/* TASK */
void rs232i(void) /* input driver for COM channel */
{
   for (;;)
   {
      rs232iEvent->Wait(rs232iEvent,0);
      if (ichar=='\r')
	ichar='\n';
      rs232iStream->Put(rs232iStream,&ichar);
   }
}


/* TASK */
void rs232o(void) /* output driver for COM channel */
{
   char ochar; /* output char */

   rs232oEvent->Wait(rs232oEvent,0);    /* for initialization caused INT */

   for (;;)
   {
      rs232oStream->Get(rs232oStream,&ochar);
      if (ochar == '\n')
      {
//	outportb(UART_TBR, '\r');    /* output to usart */
//	rs232oEvent->Wait(rs232oEvent,0);

      }
      outportb(UART_TBR, ochar);    /* output to usart */
      rs232oEvent->Wait(rs232oEvent,0);
   }
}



int rs232Setup(unsigned long Baud, unsigned char Data, unsigned char Parity, unsigned char Hndshk)
{
	union
	{
		unsigned long Long;
		unsigned char Byte[4];
	}BaudRate;

	Data-=5;
	BaudRate.Long=115200/Baud;
	if (Parity=='N')
		Parity=0;
	else if (Parity=='O')
		Parity=1;
	else if (Parity=='E')
		Parity=3;
	else
		return(0);
	if (Hndshk=='N')
		Hndshk=0;
	else if (Hndshk=='X')
		Hndshk=1;
	else if (Hndshk=='H')
		Hndshk=3;
	else
		return(0);

	IER.Byte=inportb(UART_INT_EN_REG);
	outportb(UART_INT_EN_REG, 0); /* disable TX/RX int */

	LCR.Byte=inportb(UART_LCR);
	LCR.Bits.DLAB=1;
	outportb(UART_LCR, LCR.Byte);  /* Setup port after writing DLAB=1 */

	LCR.Bits.DLAB=0;
	LCR.Bits.DataBits=Data;
	LCR.Bits.Parity=Parity;

	outportb(UART_DLR_LSB,BaudRate.Byte[0]);
	outportb(UART_DLR_MSB,BaudRate.Byte[1]);


	outportb(UART_LCR, LCR.Byte);  /* Setup port after writing DLAB=1 */
	
	outportb(UART_INT_EN_REG,IER.Byte); /* enable TX/RX int */
	return(-1);
}	


int rs232putchar(unsigned char ch)
{
 unsigned char cha=ch;

 rs232oRes->Lock(rs232oRes);
 rs232oStream->Put(rs232oStream,&cha);
 rs232oRes->Unlock(rs232oRes);
 return(cha);
}	 


int rs232puts(char* str)
{
 char* s=str;
 
 rs232oRes->Lock(rs232oRes);
 while(*str)
 {
  rs232oStream->Put(rs232oStream,str);
  str++;
 }
 rs232oRes->Unlock(rs232oRes);
 return(str-s);
}	


int rs232printf(int cnt, const char* format, ...)
{
 va_list ptr;
 char* buf;
 int ret;

 if (rs232oRes && rs232oStream)
 {
  rs232oRes->Lock(rs232oRes);
  buf=new(cnt+16);	
  va_start(ptr,format);
  ret=vsprintf(buf,format,ptr);
  va_end(ptr);
  if (ret==EOF)
   return(ret);
  if (ret>cnt+16)
   return(EOF);
  for (ret=0 ;ret<=cnt, *buf>0 ;ret++, buf++ )
  {
   rs232oStream->Put(rs232oStream,buf);
  }
  delete(buf);
  rs232oRes->Unlock(rs232oRes);
  return(ret);
 }
 else
 {
  buf=new(cnt+16);	
  va_start(ptr,format);
  ret=vsprintf(buf,format,ptr);
  va_end(ptr);
  delete(buf);
  return(EOF);
 }
}	 



int rs232getchar(void)
{
 char dummy;

 if (rs232iStream)
 {
  rs232iStream->Get(rs232iStream,&dummy);
  return(dummy);
 }
 else
 {
  return(-1);
 }
}


