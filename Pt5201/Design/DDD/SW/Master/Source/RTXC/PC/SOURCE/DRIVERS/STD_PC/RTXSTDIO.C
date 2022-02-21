/* rtxstdio.c */

/*
 *   RTXC STDIO Version 1.0a
 *   Copyright (c) 1994-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxstdio.h"

extern int getch(void);

CONCB_Struct CONCdcb[1];

UNGETB_Struct ungetbuf[2];

/*---------------------------------------------------------------------------
      Note: Some demos have more than one serial port. For compatibilty
            reasons the variable PORTNO is passed even though it may not
            be required. 
---------------------------------------------------------------------------*/

int defconc (FILE *portno, CONCB_Struct *concbp)
{
   CONCdcb[(int)portno] = *concbp;
   return 0;
}

/*---------------------------------------------------------------------------
  Function: Input Line, limited to length (len)

  In:
     portno:  Device handle

  Out:

---------------------------------------------------------------------------*/

int feditln(FILE *portno, char *buff, int len)
{
  char *cp;
  int  data, count;

  data = 0;
  count = 0;
  cp = buff;

  flockport(portno);

  do
  {
    data = fgetc(portno);

    switch(data)
    {
      case RETURN_KEY:
         if(count < len)
	 {
	   *cp = '\0';
	   fputc('\n', portno);
	 }
	 else
	   fputc(BELL, portno);
	   break;

      case BACKSP_KEY:
      case DELETE_KEY:

         if(count)
	 {
	   count--;
	   *(--cp) = '\0';
	   fputs("\b \b", portno);
	 }
	 else
	   fputc('\a',portno);
	 break;

      default:

	 if( data > 0x1F && data < 0x7F && count < len)
	 {
	   *cp++ = (char)data;
	   count++;
	   fputc(data, portno);
	 }
	 else
	   fputc(BELL, portno);
    }

  } while(data != RETURN_KEY);

  funlockport(portno);

  return (count);
}

/*---------------------------------------------------------------------------
  Function: Input Character

  In:
     portno:  Device handle

  Out:

---------------------------------------------------------------------------*/

int fgetc(FILE *portno)
{
  unsigned char data;
  int           i;

  i = (int)portno;
  if(i > 1)
    return -1;

  if(ungetbuf[i].in)
  {
    data = ungetbuf[i].buf[UNGET_BUF_SIZE - ungetbuf[i].in];
    ungetbuf[i].in--;
  }
  else
#ifndef USE_STDIO
    KS_dequeuew(CONCdcb[i].inq, &data);   /* get next char from queue */
#else
   {
    if(kbhit())
    {
      data = getch();
    }
   }
#endif

  return((int)data);
}

/*---------------------------------------------------------------------------
  Function: Get character within given time period

  In:
     portno:  Device handle

  Out:

---------------------------------------------------------------------------*/

int fgetc_t(FILE *portno, TICKS timeout)
{

  unsigned char data;
  KSRC          ksrc;
  int           i;

  i = (int)portno;
  if(i > 1)
    return -1;

  if(ungetbuf[i].in)
  {
    data = ungetbuf[i].buf[UNGET_BUF_SIZE - ungetbuf[i].in];
    ungetbuf[i].in--;
  }
  else
#ifndef USE_STDIO
    /* get next char from que */
    ksrc = KS_dequeuet(CONCdcb[i].inq, &data, timeout);
#else
   {
    if(kbhit())
    {
      data = getch();
      ksrc = RC_GOOD;
    }
   }
#endif

  if(ksrc == RC_GOOD)
    return((int)data);
  else
    return(EOF);
}

/*---------------------------------------------------------------------------
  Function: Get character within given time period
  In:
     portno:  Device handle

  Out:

---------------------------------------------------------------------------*/

int fckinput(FILE *portno)
{
  unsigned char  data;
  KSRC           ksrc;
  int           i;

  i = (int)portno;
  if(i > 1)
    return(EOF);

  if(ungetbuf[i].in)
  {
    data = ungetbuf[i].buf[UNGET_BUF_SIZE - ungetbuf[i].in];
    ungetbuf[i].in--;
  }
  /* get next char from queue */
  else
#ifndef USE_STDIO
    ksrc = KS_dequeue(CONCdcb[i].inq, &data);
#else
   {
    if(kbhit())
    {
      data = getch();
      ksrc = RC_GOOD;
    }
   }
#endif

  if(ksrc == RC_GOOD)
    return((int)data);
  else
    return(EOF);
}

/*---------------------------------------------------------------------------
  Function: Puts the character read from que back to the ports unget buffer.
            Since the queue entry can not be jammed back onto top of queue,
            the unget buffer is used. All get() functions first check the
            unget buffer before getting an entry from the input queue. If
            data is in the unget buffer, it is read first.
  In:
     c:    :  character to be written to the unget buffer.
     portno:  Device handle

  Out:
     returns:
          character (c) = succes
          EOF = buffer is full

---------------------------------------------------------------------------*/

int ungetc(int c, FILE *portno)
{
  int           i;

  i = (int)portno;

  if(i > 1)
    return EOF;

  if(ungetbuf[i].in >= UNGET_BUF_SIZE)
    return EOF;

  ungetbuf[i].in++;

  ungetbuf[i].buf[UNGET_BUF_SIZE - ungetbuf[i].in] = (char)c;

  return c;
}

/*-----------------------------------------------------------------------------
   fflushport(): Flush input and/or output ques

  In:
     portno:  Device handle
     iotype:  What to flush input, output or both

  Out:
     returns:
            0 = succes
          EOF = invalid port number
-----------------------------------------------------------------------------*/

int fflushport(FILE *portno, int iotype)
{
  if((int)portno > 1)
    return(EOF);

  /* flush input que */
  if(iotype == FLUSH_IN || iotype == FLUSH_IO)
  {
    KS_purgequeue(CONCdcb[(int)portno].inq);
    ungetbuf[(int)portno].in = 0;
  }

  /* flush output que */
  if(iotype == FLUSH_OUT || iotype == FLUSH_IO)
  {
    if(CONCdcb[(int)portno].resource)
      KS_lockw(CONCdcb[(int)portno].resource);

    KS_purgequeue(CONCdcb[(int)portno].outq);

    if(CONCdcb[(int)portno].resource)
      KS_unlock(CONCdcb[(int)portno].resource);
  }

  return 0;
}
/*-----------------------------------------------------------------------------
   printf(char *msg, ...):  Print to STDOUT device.

  In:
     msg   :  pointer to vararg buffer
     ...   :  other passed parameters

  Out:
     returns the number of bytes output

-----------------------------------------------------------------------------*/

int printf (const char *msg, ...)
{
  int cnt;
  char buffer[PRINT_BUF_SIZE];

  va_list arg_ptr;

  va_start(arg_ptr, msg);
  cnt = vsprintf(&buffer[0], msg, arg_ptr);
  va_end(arg_ptr);

  fputs((char *)&buffer[0], stdout);

  return cnt;

}

/*-----------------------------------------------------------------------------
   fprintf(char *msg, ...): Print routine called from Stack.

  In:
     portno:  Device handle
     msg   :  pointer to vararg buffer
     ...   :  other passed parameters
  Out:
     returns the number of bytes output

-----------------------------------------------------------------------------*/

int fprintf (FILE *portno, const char *msg, ...)
{
  int cnt;
  char buffer[PRINT_BUF_SIZE];

  va_list arg_ptr;

  va_start(arg_ptr, msg);
  cnt = vsprintf(&buffer[0], msg, arg_ptr);
  va_end(arg_ptr);

  fputs((char *)&buffer[0], portno);

  return cnt;

}

/*---------------------------------------------------------------------------
  Function: Output Character Port driver.

  In:
     c     :  character to output
     portno:  Device handle

  Out:

---------------------------------------------------------------------------*/

int fputc(int c, FILE *portno)
{
  char           chr;
  int            i;

  i = (int)portno;
  if(i > 1)
    return(EOF);

  if(CONCdcb[i].resource)
    KS_lockw(CONCdcb[i].resource);

  chr = (char)c;

#ifndef USE_STDIO
  KS_enqueuew(CONCdcb[i].outq, &chr); /* queue char for later processing */
#else
  putchar(chr);
#endif

  if(CONCdcb[i].resource)
    KS_unlock(CONCdcb[i].resource);

  return c;
}

/*---------------------------------------------------------------------------

  Function: Output Character Port driver.

  In:
     buf   :  pointer to buffer to be output
     portno:  Device handle

  Out:

---------------------------------------------------------------------------*/

int fputs(char *buf, FILE *portno)
{
  char   *cp;
  int    i;

  i = (int)portno;

  if(i > 1)
    return(EOF);

  if(CONCdcb[i].resource)
    KS_lockw(CONCdcb[i].resource);

#ifndef USE_STDIO
  for(cp = buf; *cp != 0; cp++)
    KS_enqueuew(CONCdcb[i].outq, cp); /* queue char for later processing */
#else
   puts(buf);
#endif

  if(CONCdcb[i].resource)
    KS_unlock(CONCdcb[i].resource);

  return(0);
}

/*---------------------------------------------------------------------------
  Function: Lock port for output

  In:
     portno:  Device handle

  Out:
     return code
         RC_GOOD: success
        !RC_GOOD: One of the RTXC return codes for unlock

---------------------------------------------------------------------------*/

KSRC flockport(FILE *portno)
{

  if((int)portno > 1)
    return(EOF);

  if(CONCdcb[(int)portno].resource)
    return(KS_lockw(CONCdcb[(int)portno].resource));
  return(RC_GOOD);
}

/*---------------------------------------------------------------------------
  Function: UnLock port for output

  In:
     portno:  Device handle

  Out:
     return code
         RC_GOOD: success
        !RC_GOOD: One of the RTXC return codes for unlock

---------------------------------------------------------------------------*/

KSRC funlockport(FILE *portno)
{
  if((int)portno > 1)
    return(EOF);

  if(CONCdcb[(int)portno].resource)
    return(KS_unlock(CONCdcb[(int)portno].resource));
  return(RC_GOOD);
}

/* end of of source */

 /*--------------------------------------------------------------------------*\
 *                                                                            *
 *  History:                                                                  *
 *                                                                            *
 *        DATE    IN    VER   Comments                                        *
 *      --------  ---   ----  --------------------------------------------    *
 *      09/11/97  DDR   1.1   Initial release                                 *
 *                                                                            *
 *                                                                            *
 \*--------------------------------------------------------------------------*/



/* end of stdio.c */



