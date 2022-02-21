/**************************************************************************
 ; FILE NAME        xcom.c
 ; PURPOSE          Module implementing serial communication
 ;                  library for MS-DOS.
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:       18.09.2003
 ;                  16.11.2005: Added ctrl-break functions in Xcom_Init()
 ;
 ; NOTES:
 ;                  This module provides the following functions:
 ;
 ;                    int   Xcom_Init(int, long, int, int, int, long);
 ;                    void  Xcom_Close(int);
 ;                    int   Xcom_Putchar(uchar, int);
 ;                    int   Xcom_Putdata(uchar *, int, int);
 ;                    void  Xcom_Flush(int);
 ;                    int   Xcom_DataReady(int);
 ;                    int   Xcom_Getchar(uchar *, int);
 ;                    int   Xcom_Getchar_wtimeout(uchar *, int, long);
 ;                  and CtrlBrk functions:
 ;                    void  INstallNewCC(int);
 ;                    void  INstallOldCC(void);

 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <sys/types.h> 
#include <sys/timeb.h>
#include <time.h>
#include "xcom.h"

#define COM1 0
#define COM2 1
#define COM3 2
#define COM4 3

#define TRUE  1
#define FALSE 0

enum { NONE, ODD, filler, EVEN };

typedef struct
   {
   long delay;           /* time to wait */
   struct timeb start;   /* start time   */
   } chrono_t;



/* 8250 UART port address offsets */
#define DATA    0   /* Send or Receive Data */
#define BAUDDIV 0   /* Baud Rate Divisor (DLAB set to 1) */
#define ENINT   1   /* Enable Interrupts */
#define INTID   2   /* Interrupt Identification */
#define LNCTRL  3   /* Line Control */
#define MDMCTRL 4   /* MODEM Control */
#define LNSTAT  5   /* Line Status */
#define MDMSTAT 6   /* MODEM Status */

#define DLAB    0x80   /* Divisor Latch Access Bit (in Line Control Register) */
#define DTR     0x01   /* Data Terminal Ready (in MODEM Control Register) */
#define RTS     0x02   /* Request to Send (in MODEM Control Register) */
#define OUT2    0x08   /* UART OUT2 enables Interrupts on IBM-PC */
#define RXINT   0x01   /* Enable Receive Interrupts for UART */
#define RXREADY 0x01   /* Receive Ready Bit (in Line Status Register) */
#define TXREADY 0x60   /* Transmit Holding & Shift Register Empty Bits */
#define INTPEND 0x01   /* This bit will be Zero if an interrupt is pending */
#define DCTS    0x01   /* Delta (change in) Clear to Send */
#define DDSR    0x02   /* Delta (change in) Data Set Ready */
#define DRI     0x04   /* Delta (change in) Ring Indicator */
#define DDCD    0x08   /* Delta (change in) Data Carrier Detect */
#define CTS     0x10   /* Clear to Send */
#define DSR     0x20   /* Data Set Ready */
#define RI      0x40   /* Ring Indicator */
#define DCD     0x80   /* Data Carrier Detect */

#define CLOCK 0x1C200L /* Baud Rate Clock (1,843,200 / 16) */

#define PICMR   0x21   /* Priority Interrupt Controller Mask Address */
#define PICCR   0x20   /* Priority Interrupt Controller Control Reg. Address */
#define EOI     0x20   /* End Of Interrupt signal to PIC */
 
#define BUFSIZE 1024   /* size of circular receive buffer */

static char Buffer[4][BUFSIZE];   /* circular buffer for COM1 - COM4 */
static int PortAdr[4] = {0x03F8, 0x02F8, 0x03E8, 0x02E8};
static int PtrB[4]    = {0, 0, 0, 0};   /* pointer (index) to start of buffer */
static int PtrE[4]    = {0, 0, 0, 0};   /* pointer (index) to end of buffer */
static int InUse[4]   = {FALSE, FALSE, FALSE, FALSE};   /* port still in use? */

static void (interrupt far *OldFunc[2])();   /* original ISR vectors */

static void interrupt (*Old0x1BHandler)();   /* Control-Break: BIOS */
static void interrupt (*Old0x23Handler)();   /* Control-C: MS-DOS */

static int Vect[2]      = {0x0C, 0x0B};   /* serial port vector numbers */
static int PICMask[2]   = {0x10, 0x08};   /* priority interrupt controller mask */
static int Installed[2] = {FALSE, FALSE};   /* ISR installed yet? */

static long rx_timeout;

static int Cbrk_Installed = FALSE;   /* TRUE if our handler installed */
static int CBrk;                     /* stores original status of Ctrl-C/Brk processing */
int CCFlag;                          /* Set to TRUE if handler invoked */


/* protos */
static void start_chrono (chrono_t *, long);
static int end_chrono (chrono_t);
/* initialize baud rate, data bits, parity, and stop bits */
void initcom (int port, long baud, int data, int parity, int stop);

/* start receiving -- install interrupt service routine */
void start (int port);

/* stop receiving -- remove ISR, disable interrupts */
void stop (int port);

/* determine if UART is able to accept another character */
int txstat (int port);

/* send one character to the UART */
void tx (int port, char c);

/* determine if there is a character waiting in the receive buffer */
int rxstat (int port);

/* get the next character from the receive buffer */
char rx (int port);

/* determine status of Clear To Send */
int ctsstat (int port);

/* determine status of Ring Indicator */
int ristat (int port);

/* determine status of Data Carrier Detect */
int dcdstat (int port);

/* determine status of Data Set Ready */
int dsrstat (int port);

/**********************************************************************
 * NAME       int Xcom_Init(int port, long baud, int data, 
 *                          int parity, int stop, long time_out)
 *            
 * PURPOSE    To open COM port.
 *
 * ARGUMENTS  int  port     - port number
 *            long baud     - baud rate
 *            int  data     - data len
 *            int  parity   - parity
 *            int  stop     - stop bits
 *            long time_out - receive timeout in mS
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES:     
 *
 **********************************************************************/
int Xcom_Init(int port, long baud, int data, int parity, int stop, long time_out)
  {
  initcom(port, baud, data, parity, stop);
  rx_timeout = time_out;
  start(port);
  CCFlag = FALSE;
  INstallNewCC(TRUE);
  return (1);
  }

/**********************************************************************
 * NAME       void Xcom_Close(int port)
 *            
 * PURPOSE    To close COM port.
 *
 * ARGUMENTS  int  port - comport
 *
 * RETURNS    Nothing.
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
void Xcom_Close(int port)
  {
  INstallOldCC();
  stop(port);
  }

/**********************************************************************
 * NAME       void Xcom_Flush(int port)
 *            
 * PURPOSE    To remove all receoved chars from buffer.
 *
 * ARGUMENTS  int  port - comport
 *
 * RETURNS    Nothing.
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
void Xcom_Flush(int port)
  {
  PtrB[port] = PtrE[port] = 0;   /* initialize receive buffer */
  }

/**********************************************************************
 * NAME       int Xcom_DataReady(int port)
 *            
 * PURPOSE    To check if a character was received.
 *
 * ARGUMENTS  int  port - comport
 *
 * RETURNS    0 - if no character is available
 *            1 - if a character is available
 *
 * EXTERNS    None.
 *
 * NOTES:     
 **********************************************************************/
int Xcom_DataReady(int port)
  {
  return rxstat(port);
  }

/**********************************************************************
 * NAME       int Xcom_Putchar(uchar c, int port)
 *
 * PURPOSE    To send one character.
 *
 * ARGUMENTS  uchar c   - character to send.
 *            int port  - comport.
 *
 * RETURNS    1
 *
 * EXTERNS    None.
 *
 * NOTES      
 **********************************************************************/
int Xcom_Putchar(uchar c, int port)
  {
  while(!txstat(port))
    ;
  tx(port, c);
  return (1);
  }



/**************************************************************************
 * NAME       int Xcom_Getchar(uchar * c, int port)
 * PURPOSE    To fetch a character from serial receive buffer with timeout.
 * ARGUMENTS  uchar * c - poiter to result.
 *            int port  - comport
 *
 * RETURNS    0 if a character does not arrive during RXTIMEOUT,
 *            1 otherwise, a received character is placed at c
 *            
 *
 * EXTERNS    None.
 * NOTES
 **************************************************************************/

int Xcom_Getchar(uchar *c, int port)
  {
  chrono_t timeout;

  start_chrono (&timeout, rx_timeout);
  while(!rxstat(port))
    {
    if (CCFlag)
      {
      printf("Ctrl-Brk pressed.\n");
      return (0);
      }
    if (end_chrono(timeout))
      return (0);
    }
  *c = rx(port);
  return (1);
  }

/**********************************************************************
 * NAME       int Xcom_Getchar_wtimeout(uchar *c, int port, long mytimeout)
 *
 * PURPOSE    To fetch a character from serial receive buffer with timeout.
 *
 * ARGUMENTS  uchar * c - poiter to result.
 *            int port  - comport
 *            long mytimeout - timeout in mS
 *
 * RETURNS    0 if a character does not arrive during RXTIMEOUT,
 *            1 otherwise, a received character is placed at c
 *
 * EXTERNS    None.
 * NOTES
 **********************************************************************/
int Xcom_Getchar_wtimeout(uchar *c, int port, long mytimeout)
  {
  chrono_t timeout;

  start_chrono (&timeout, mytimeout);
  while(!rxstat(port))
    {
    if (CCFlag)
      {
      printf("Ctrl-Brk pressed.\n");
      return (0);
      }
    if (end_chrono(timeout))
      return (0);
    }
  *c = rx(port);
  return (1);
  }


/* initialize chronometer */
static void start_chrono (chrono_t *c, long t)
  {
  ftime (&(c->start));
  c->delay = t;
  }

/* return TRUE if time has expired */
static int end_chrono (chrono_t c)
  {
  struct timeb temp;
  long t;

  ftime (&temp);
  /* convert difference to milliseconds */ 
  t = (temp.time - c.start.time) * 1000 + ((long)temp.millitm - (long)c.start.millitm);

  return (t >= c.delay);
  }

/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/


/* local function for handling COM1 - COM4 serial port interrupts */
static void interrupt far rxisr (void)
  {
   char c;     /* character to read from port */
   int adr;    /* address of port */
   int port;   /* port number: COM1 - COM4 */
   int pos;    /* position in buffer */
   
   for (port = COM1; port <= COM4; port++)
     {                                           /* poll each port */
     adr = PortAdr[port];                        /* get port address */
     if ((INTPEND & inport (adr + INTID)) == 0)
       {                                         /* Interrupt Pending? */
       c = inportb (adr + DATA);                 /* get character */
       pos = PtrE[port];                         /* get position of buffer end */
       Buffer[port][pos++] = c;                  /* add character to end of buffer */
       PtrE[port] = (pos == BUFSIZE) ? 0 : pos;  /* update buffer position */
       }
     }
   outportb (PICCR, EOI); /* tell priority interrupt controller that interrupt is over */
  }


/* initialize baud rate, data bits, parity, and stop bits */
static void initcom (int port, long baud, int data, int parity, int stop)
  {
  long line;
  int  adr = PortAdr[port];
   
  /* Access Divisor Latch; set baud rate */
  outportb (adr + LNCTRL, DLAB);
  line = CLOCK / baud;
  outport (adr + BAUDDIV, (int)line);
#if 0 
  printf("Baud rate is %ld\n", baud);
  printf("Divisor is %d\n", (int) line);
  printf("COMPort is %d at %3X\n", (int) port+1, adr);
#endif

  /* combine data, parity, and stop bits; set port accordingly */  
  line = (data - 5) | (parity << 3) | ((stop - 1) << 2);
  outportb (adr + LNCTRL, (char)line);
  }


/* start receiving -- install interrupt service routine */
static void start (int port)
  {
   char pic;
   int adr = PortAdr[port];
   int ip  = port & 0x0001;   /* COM3/4 uses ISR for COM1/2 */
   
   PtrB[port] = PtrE[port] = 0;   /* initialize receive buffer */
   
   if (!Installed[ip])
     {
      /* save original value from vector table */
      OldFunc[ip] = getvect(Vect[ip]);
   
      /* set vector table with address of our serial port ISR */
      setvect(Vect[ip], rxisr);
      
      /* unmask the priority interrupt controller */
      pic  = inportb (PICMR);
      pic &= ~PICMask[ip];
      outportb (PICMR, pic);
   
      /* mark isr for this "group" as installed */
      Installed[ip] = TRUE;
   }
   
   if (rxstat (port))
      rx (port);   /* remove any character "stuck" in UART */
      
   /* enable Data Terminal Ready, Request to Send; allow interrupts through */
   outportb (adr + MDMCTRL, DTR | RTS | OUT2);
   
   /* enable receive interrupts in UART */
   outportb (adr + ENINT, RXINT);
   
   /* mark this port as in use */
   InUse[port] = TRUE;
}


/* stop receiving -- remove ISR, disable interrupts */
static void stop (int port)
  {
  char pic;   /* PIC Mask */
  int adr = PortAdr[port];   /* port address */
  int ip = port & 0x0001;   /* COM3/4 uses ISR for COM1/2 */
  
  /* mark this port as no longer in use */
  InUse[port] = FALSE;
  
  /* disable all interrupts in UART */
  outportb (adr + ENINT, 0);
  
  /* disable Data Terminal Ready, Request to Send; block interrupts */
  outportb (adr + MDMCTRL, 0);
  
  /* disable interrupt only if other port not using it */
  if (!InUse[ip] && !InUse[ip | 0x0002])
    {   /* neither port using ISR */
    /* mask the priority interrupt controller */
    pic = inportb (PICMR);
    pic |= PICMask[ip];
    outportb (PICMR, pic);
  
    /* restore original contents of vector table */
    setvect(Vect[ip], OldFunc[ip]);
    
    /* mark ISR as no longer installed */
    Installed[ip] = FALSE;
    }
  }


/* determine if UART is able to accept another character */
static int txstat (int port)
  {
  unsigned char stat;   /* UART status */
  
  stat = inportb (PortAdr[port] + LNSTAT);
  return (stat & TXREADY) == TXREADY;   /* Holding and Shift Empty */
  }


/* send one character to the UART */
static void tx (int port, char c)
  {
  outportb (PortAdr[port] + DATA, c);
  }


/* determine if there is a character waiting in the receive buffer */
static int rxstat (int port)
  {
  if (PtrB[port] == PtrE[port])
    return (0);
  else
    return (1);
  }


/* get the next character from the receive buffer */
static char rx (int port)
  {
  char c;   /* character to get from buffer */
  int pos = PtrB[port];   /* position within buffer */
  
  c = Buffer[port][pos++];   /* get the character from the buffer */
  PtrB[port] = (pos == BUFSIZE) ? 0 : pos;   /* update buffer position */
    
  return c;
  }


/* determine status of Clear To Send */
static int ctsstat (int port)
  {
  unsigned char stat;   /* MODEM status */
  
  stat = inportb (PortAdr[port] + MDMSTAT);
  return (stat & CTS) == CTS;
  }


/* determine status of Ring Indicator */
static int ristat (int port)
  {
  unsigned char stat;   /* MODEM status */
  
  stat = inportb (PortAdr[port] + MDMSTAT);
  return (stat & RI) == RI;
  }


/* determine status of Data Carrier Detect */
static int dcdstat (int port)
  {
  unsigned char stat;   /* MODEM status */
  
  stat = inportb (PortAdr[port] + MDMSTAT);
  return (stat & DCD) == DCD;
  }


/* determine status of Data Set Ready */
static int dsrstat (int port)
  {
  unsigned char stat;   /* MODEM status */
  
  stat = inportb (PortAdr[port] + MDMSTAT);
  return (stat & DSR) == DSR;
  }


/*
 * Disable Control-C & Control-Break abort.
 * Redirects the interrupt to our own routine which sets a
 * flag and then returns control to the program.  Flag allows
 * application program to determine if Control-C/Break
 * has occured.  Many applications may simply ignore the flag.
 *
 * Programmer: Brian R. Anderson
 * Date: January 4, 1991
 *
 */
static void interrupt handler (void)
  {
  CCFlag = TRUE;
  }

    
/* disable Control-C & Control-Break so that they may not accidently  */
/* terminate our application; handler sets flag to TRUE when invoked. */
void INstallNewCC(int flag)
  {
  if (!Cbrk_Installed)
    {
    CBrk = getcbrk();
    setcbrk (1);   /* check only during traditional I/O */
    Old0x1BHandler = getvect (0x1B);
    setvect (0x1B, handler);
    Old0x23Handler = getvect (0x23);
    setvect (0x23, handler);
    Cbrk_Installed = TRUE;
    }
  } 


/* re-enabled normal MS-DOS Control-C & Control- Break processing. */
void INstallOldCC(void)
  {
  if (Cbrk_Installed)
    {
    setvect (0x1B, Old0x1BHandler);
    setvect (0x23, Old0x23Handler);
    setcbrk (CBrk);
    Cbrk_Installed = FALSE;
    }
  }
  
     

#if 0
/*
ftime() 
get date and time


SYNOPSIS 
#include <sys/timeb.h> 

int ftime(struct timeb *tp); 

DESCRIPTION 
The ftime() function sets the time and millitm members
of the timeb structure pointed to by tp to contain
the seconds and milliseconds portions, respectively, 
of the current time in seconds
since 00:00:00 UTC (Coordinated Universal Time), January 1, 1970. 
The contents of the timezone and dstflag members of tp 
after a call to ftime() are unspecified. 

PARAMETERS 

tp  
Points to the timeb structure that is
to receive the current time information. 

RETURN VALUES 
The ftime() function always returns 0. 
*/
#endif
/* end of xcom.c */


