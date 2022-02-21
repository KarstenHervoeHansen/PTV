/****************************************************************************/
/* MODULE:                                                                  */
/*  sio0drv.c - seriel channel driver using UART0 and Timer2                */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  void sio0_init( void);                                                  */
/*  int sio0WaitAcknowledge( int addr, char *cmdbuf)                        */
/*  int sio0WaitAnswer( void *buf)                                          */
/*  int sio0SndCommand( int cnt, int addr, const char *format, ...)         */
/*  int sio0SndRequest( int cnt, int addr, char *result, \                  */
/*                                                 const char *format, ...) */
/*                                                                          */
/* TASKS:                                                                   */
    void sio0i( void);
    void sio0o( void);
/*                                                                          */
/* NOTES:                                                                   */
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
#include <string.h>
#include <periphal.h>

#include "reg8051.h"
#include "define.h"
#include "crystal.h"
#include "sio0drv.h"
#include "eventdef.h"

#define RxBufferLen  (unsigned char)64		//
#define RxTimeout    (unsigned char)30       // Timeout pr. character in milliseconds
#define RxRetransmit (unsigned char)3        // Number of retransmission if failure

#define TIMER2_RANGE	(unsigned long)65536

#define PRESCALER		(unsigned long)4     	// selected also in SCR. NOTE!!! This value 
                                             //  MUST be the same as deinfed in start.asm

#define BAUDRATE    	(unsigned long)57600    // select 57600 baud

#define RELOAD2      (unsigned short)(TIMER2_RANGE-(CRYSTAL/(BAUDRATE*PRESCALER*(unsigned long)16)))

#define THI2_RESET  ( RELOAD2 >> 8) & 0xFF
#define TLOW2_RESET ( RELOAD2 & 0xFF)


/* ==================== */
/* S0CON - bit settings */
/* -------------------- */
/* UART0                */
/* -------------------- */
#define U0_SM0   (1 << 7)   /* allow for 8-bit UART */
#define U0_SM1   (1 << 6)   /* allow for 8-bit UART */
#define U0_SM2   (0 << 5)   /* set to enable multiprocessor communication */
#define U0_REN   (1 << 4)   /* enable serial reception */
     
#define U0_TB8   (0 << 3)   /* the 9th databit transmitted in mode 2 or 3 */
#define U0_RB8   (0 << 2)   /* the 9th databit received in mode 2 or 3 */

#define U0_TI    (0 << 1)   /* set the transmit interrupt flag */
#define U0_RI    (0 << 0)   /* set the receive interrupt flag */ 

#define xS0CON (U0_SM0 | U0_SM1 | U0_SM2 | U0_REN | U0_TB8 | U0_RB8 | U0_TI | U0_RI )
/* =================== */

/* =================== */
/* TMOD - bit settings */
/* ------------------- */
/* TIMER 2             */
/* ------------------- */
#define T2_RCLK1  (0 << 5)   /* set to use timer 2 as clock source for UART1,(AND select autoreload) */
#define T2_TCLK1  (0 << 4)   /* set to use timer 2 as clock source for UART1,(AND select autoreload) */
#define T2_T2OE   (0 << 1)   /* not applicable - OR in a zero (0) */
#define T2_DCEN   (1 << 0)   /* set to count up for Timer 2 in auto reload mode */

#define T2_TMOD   (T2_RCLK1 | T2_TCLK1 | T2_T2OE | T2_DCEN)

#define xTMOD     T2_TMOD
/* =================== */

/* =================== */
/* TCON - bit settings */
/* ------------------- */
/* TIMER 2             */
/* ------------------- */
#define T2_TF2    (0 << 7)   /* overflow flag - NOT cleared by hardware */
#define T2_EXF2   (0 << 6)   /* not applicable - OR in a zero (0) */
#define T2_RCLK0  (1 << 5)   /* set to use timer 2 as clock source for UART0,(AND select autoreload) */
#define T2_TCLK0  (1 << 4)   /* set to use timer 2 as clock source for UART0,(AND select autoreload) */
#define T2_EXEN2  (0 << 3)   /* not applicable - OR in a zero (0) */
#define T2_TR2    (0 << 2)   /* not applicable - OR in a zero (0) */
#define T2_CT2    (0 << 1)   /* not applicable - OR in a zero (0) */
#define T2_CPRL2  (0 << 0)   /* not applicable - OR in a zero (0) */

#define T2_CON    (T2_TF2 | T2_EXF2 | T2_RCLK0| T2_TCLK0 | T2_EXEN2 | T2_TR2 | T2_CT2 | T2_CPRL2)

#define xTCON     T2_CON
/* =================== */

                               
/*
 *
 * DO NOT ALTER THE FOLLOWING VARIABLES: THEY ARE ALSO USED
 * IN ISRS.ASM
*/
volatile _near unsigned char RxInput, RxOutput; // These are defined _near to minimize
volatile unsigned char RxBuffer[RxBufferLen];   //  time used in receive 0 interrupt


static TEvent			*sio0iEvent, *sio0oEvent;
static TThread		*sio0iThread, *sio0oThread;
static TStream		*sio0iStream, *sio0oStream;
static TResource	*sio0Res;


/*
 *
 * interrupt service for transmit 0 (TI_0)
 *
*/
FRAME ks_stk *intsio0o( FRAME ks_stk *frame)
{
  TI_0 = 0;

  return( KS_ISRexit( frame, sio0oEvent->ID));  // wake up output task
}

/*
 *
 * interrupt service for receive 0 (SWR6)
 *
*/
FRAME ks_stk *intsio0i( FRAME ks_stk *frame)
{
  SWR6 = 0;

  return( KS_ISRexit( frame, sio0iEvent->ID));
}


void sio0_init( void)
{
  unsigned char tmp;

  sio0iEvent=newTEvent( 0);
  sio0oEvent=newTEvent( 0);

  sio0Res=newTResource( 0);

  sio0iStream=newTStream( 1, 128);
  sio0oStream=newTStream( 2, 64);

  sio0iThread=newTThread( 2, sio0i);
  sio0oThread=newTThread( 4, sio0o);

  sio0iThread->Execute( sio0iThread);
  sio0oThread->Execute( sio0oThread);

  // S0CON - serial port 0 control register
  S0CON = xS0CON;

  // Timer/Counter 2 Mode Control Register
  T2MOD = xTMOD;

  // Timer/Counter 2 Control Register
  T2CON = xTCON;

  // Interrupt priority 4
  IPA4 = ( TX0INTLVL << 4) + RX0INTLVL;

  T2CAPH = THI2_RESET;
  T2CAPL = TLOW2_RESET;
	
  RxInput = 0;				 // 
  RxOutput = 0;

  tmp = S0BUF;           // read input to clear
  tmp = S0BUF;           // read input to clear

  KS_purgequeue( sio0oStream->ID);
  KS_purgequeue( sio0iStream->ID);

  SWE |= 0x20;           // Enable SW interrupt 6

  TR2  = 1;              // Start Timer 2
  REN_0 = 1;             // Enable seriel reception

  TI_0 = 0;              // Clear possible transmit interrupt 
  RI_0 = 0;              // Clear possible receive interrupt 

  ERI0 = 1;              // Receive port 0 - enabled
  ETI0 = 1;              // Transmit port 0 - enabled
}


/* TASK */
void sio0i( void)
{
	for (;;)
  {
  	sio0iEvent->Wait( sio0iEvent, 0);

    EA = 0;
    RxOutput = ++RxOutput % RxBufferLen;
    	EA = 1;

    sio0iStream->Put( sio0iStream, &RxBuffer[ RxOutput]);
    
	 	if ( RxOutput - RxInput) // If more input data
      SWR6 = 1;              //  enable SW6 interrupt again
  }
}


/* TASK */
void sio0o( void)
{
  int ochar;

  for (;;)
  {
    sio0oStream->Get( sio0oStream, &ochar);

    TB8_0 = ( ochar >> 8) & 0x01;

    S0BUF = ochar & 0xFF;    // Write char to port
    TI_0 = 0;                // Clear interrupt IMMEDIATELY to disable
                             //  double buffering

    sio0oEvent->Wait( sio0oEvent, 0);
  }
}

/**************************************************************************/
/* sio0WaitAcknowledge																				  SIO0DRV.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000120																		*/
/* Revised:		000409, KEn, DEV     																				*/
/*																																				*/
/* Function:	Transmit <cmdbuf> to <addr> and wait for an acknowledge	 		*/
/* Remarks:   Resource should be locked BEFORE entering this function			*/
/* Returns:		sio0NoError, sio0TxCksumError, sio0NoAcknowledge or					*/
/*            sioUnknowError.																							*/ 
/* Updates:		--																													*/
/**************************************************************************/
int sio0WaitAcknowledge( int addr, char *cmdbuf)
{
  char *ptr;
  int tmp;
  UC checksum, retransmit = RxRetransmit;

  do
  {
    tmp  = addr + 0x100;		// Transmit unit address
    sio0oStream->Put( sio0oStream, &tmp);

    checksum = 0;
    ptr = cmdbuf;

    while ( tmp = *ptr++)
		{
      sio0oStream->Put( sio0oStream, &tmp);

      checksum += tmp;
    }

    tmp = checksum & 0x7F;		// Transmit checksum
    sio0oStream->Put( sio0oStream, &tmp);

                              // Wait for unit to acknowledge
    if ( KS_dequeuet( sio0iStream->ID, &tmp, RxTimeout/CLKTICK) == RC_GOOD)
    {
      switch ( tmp)
			{
        case 0x80:
          return( OK);

        case 0x81:
          return( EventSIO0TxCksumErr);

        default:
          return( EventSIO0Error);
			}
    }
  } while ( retransmit--);

  return( EventSIO0NoAck);
}										

/**************************************************************************/
/* sio0WaitAnswer																						  	SIO0DRV.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000119																		*/
/* Revised:		000409, KEn, DEV     																				*/
/*																																				*/
/* Function:	Wait for an answer																					*/
/* Remarks:   Resource should be locked BEFORE entering this function			*/
/* Returns:		sio0Noerror, sio0RxTimeout, sio0CksumError									*/
/* Updates:		--																													*/
/**************************************************************************/
int sio0WaitAnswer( void *buf)
{
  int  timeout;
  char tmp, next = TRUE;
  UC checksum = 0;
  char *ptr = buf;

  do
  {
    if (( timeout = KS_dequeuet( sio0iStream->ID, &tmp, RxTimeout/CLKTICK)) == RC_GOOD)
    {
      checksum += tmp;
			*ptr++ = tmp;
      
      switch ( tmp)
      {
			  case 0x0A:
			  case 0x0D:
		  	case ';':
			  case '!':
			  case '?':
			  case '$':
					next = FALSE;
			    break;

  		  default:
			    break;
			}
		} 
  } while ( next && ( timeout != RC_TIMEOUT));

  if ( timeout != RC_TIMEOUT) {

    *(--ptr) = 0;			  			// Remove delimiter

                              // Wait for checksum
    if ( KS_dequeuet( sio0iStream->ID, &tmp, RxTimeout/CLKTICK) == RC_GOOD)
    {
      if ( tmp - ( checksum & 0x7F))
			  return( EventSIO0RxCksumErr);

      return( OK);
    }
  }
                             
  return( EventSIO0RxTimeout);
}										

/**************************************************************************/
/* sio0SndCommand		 																				 	 SIO0DRV.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000119																		*/
/* Revised:		000404, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int sio0SndCommand( int cnt, int addr, const char *format, ...)
{
  va_list va_ptr;
  char *buf;
  int ret;

  sio0Res->Lock( sio0Res);

  buf = new( cnt+16);

  va_start( va_ptr, format);
  ret = vsprintf( buf, format, va_ptr);
  va_end( va_ptr);

  if (( ret == EOF) || ( ret > cnt+16))
  {
	  sio0Res->Unlock( sio0Res);

    delete( buf);

    return( EOF);				   // EOF is -1
  }

  ret = sio0WaitAcknowledge( addr, buf);

  delete( buf);

  sio0Res->Unlock( sio0Res);

  return( ret);
}

/**************************************************************************/
/* sio0SndRequest	 																					  	SIO0DRV.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000119																		*/
/* Revised:		000404, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/***************************************************************************/
int sio0SndRequest( int cnt, int addr, char *result, const char *format, ...)
{
  va_list va_ptr;
  char *buf;
  int ret;

  sio0Res->Lock( sio0Res);

  buf = new( cnt+16);

  va_start( va_ptr, format);
  ret = vsprintf( buf, format, va_ptr);
  va_end( va_ptr);

  if (( ret == EOF) || ( ret > cnt+16))
  {
	  sio0Res->Unlock( sio0Res);

    delete( buf);

    return( EOF);				   // EOF is -1
  }

  ret = sio0WaitAcknowledge( addr, buf);

  delete( buf);

  buf = new( 50);

  if (( ret = sio0WaitAnswer( buf)) == OK)
		strcpy( result, buf);

  delete( buf);

  sio0Res->Unlock( sio0Res);

  return( ret);
}




