 /*********************************************************************
 ; DK-Technologies A/S
 ;
 ; FILE NAME        usb_main.c
 ; PURPOSE          To receive data via USB interface and send them
 ;                  via RS232 interface.
 ;                  To receive data via RS232 interface and send them
 ;                  via USB interface.
 ;
 ; SPEC             Xia Wu
 ; DESIGN           Xia Wu
 ; CODING           Xia Wu
 ; CODE INSP.       Janusz Kuzminski
 ;
 ; TEST
 ; LAST UPDT:       14.12.2004
 ;
 ; NOTES:           This program runs on C8051F320 processor from 
 ;                  Silicon Laboratories.  This processor has:
 ;                  256 Bytes of Internal RAM
 ;                  1   KB of XRAM (internal)
 ;                  16  KB of Flash program memory
 ;                  The program must be linked with uart.obj module
 ;                  (uart.c)
************************************************************************/
#include <c8051f320.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "USB_API.h"
#include "defins.h"
#include "uart.h"


sbit  SW1    = P2^1;
sbit  LED1   = P2^2;
sbit  LED2   = P2^3;
sbit  SRESET = P0^2;
sbit  SSCL   = P0^1;
sbit  SSDA   = P0^0; 

/* global variables */

xdata BYTE IN_Packet[MAX_WRITE_BYTE];
xdata BYTE OUT_Packet[64];

uchar bdata comm_flags;
sbit  serial_data_ready  = comm_flags^0; /* signifies that one line of data     */
                                         /* sent from sensor has been collected */

code  BYTE Serial[]     = {"1234"};
code  BYTE MaxPower     = 200;       //max current
code  BYTE PwAttributes = 0x80;
code  UINT bcdDevice    = 0x100;

/* Prototypes */
void Port_Init(void);
void Suspend_Device(void);
void Uart_Init(void);
void SYSCLK_Init (void);
void set_XY_mode(void);
bit collect_sesor_data(char xdata*);

void main(void)
  {
  int  i;

  PCA0MD &= ~0x40;  //disable watchdog timer
  SYSCLK_Init();  
  Port_Init();

  for(i = 0; i < MAX_WRITE_BYTE; i++) //initialize I/O buffer
    {
    IN_Packet[i] = 0xaa;
    }

  serinit();
  serial_data_ready = 0;

#if 1
  USB_Init(0, 0xea61, NULL, NULL, Serial, MaxPower, PwAttributes, bcdDevice);
//  CLKSEL |= 0x02;
  USB_Int_Enable();
#endif

  EA   = 1;   //global interrupt enable
  LED1 = 0;

  while (1)          // JK test here
    set_XY_mode();

  serflush();

  /* main loop */
  while(1)
    {
    if (!serial_data_ready)
      {
      if (collect_sesor_data(IN_Packet)) /* see if sensor has sent some data... */
        {
        LED1 = ~LED1;
        serial_data_ready = 1;
        }
      }
    }
  }

void Port_Init(void)
  {
  P0MDIN  = 0xFF;   
  P0MDOUT = 0x00;   //define P0 to be open-drain port
  P1MDIN  = 0xFF;   
  P1MDOUT = 0xFF;
  P2MDIN  = 0xFF;   
  P2MDOUT = 0x3f;   //define P2.7,P2.6 to be open-drain bits
  
  P1SKIP  = 0xFF; 
  XBR0    = 0x01;   //enable UART bit
  XBR1    = 0x40;   //enable cross bar
  }


void Suspend_Device(void)
  {
  P0MDIN  = 0x00;
  P1MDIN  = 0x00;
  P2MDIN  = 0x00;
  P3MDIN  = 0x00;
  
  USB_Suspend();
  P0MDIN  = 0xFF;
  P1MDIN  = 0xFF;
  P2MDIN  = 0xFF;
  P3MDIN  = 0x01;
  }


void SYSCLK_Init (void)
  {
  OSCICN = 0x83; // configure internal oscillator for 12M / 1 (page 119)
  RSTSRC = 0x04; // enable missing clock detector

  //  CLKMUL |= 0xC0;              // enable clock multiplier
  //  while (! (CLKMUL & 0x20) );  // wait until stable
  //  CLKSEL |= 0x02;              // system clock  = 12M * 2
  }

/**********************************************************************
 * NAME       void USB_API_TEST_ISR(void) interrupt 16
 * PURPOSE    Interrupt routine tackling USB reception.
 *
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:   
 *          Is entered when a packet of data arrived via USB.
 *          The number of characters arrived is variable and
 *          is returned by Block_Read() function.
 *          This function copies characters received via USB
 *          into tx_serbuf[] using serout().
 *          This function sends out characters received from sensor
 *          via USB interface using Block_Write().
 **********************************************************************/
void USB_API_TEST_ISR(void) interrupt 16
  {
  BYTE  INTVAL;
  BYTE  i;
  BYTE  bytes_read_from_USB;
  BYTE  junk;

  junk   = 0;
  INTVAL = Get_Interrupt_Source();

  if (INTVAL & RX_COMPLETE)
    {
    /* unload data from USB buffer and send to sensor */
    bytes_read_from_USB = Block_Read(OUT_Packet, 64);

    if (OUT_Packet[0] == 1)    /* if no data coming from USB... */
      {                    
      if (serial_data_ready)   /* see if sensor has sent some data... */
        {
        LED2 = ~LED2;
        Block_Write(IN_Packet, strlen(IN_Packet));  /* if it has, send them via USB */
        serial_data_ready = 0;
        }
      else                                          /* if not...           */
        Block_Write(&junk, 1);                      /* send junk buffer    */
      }
    else
      {                        /* if data coming from USB... */
      for (i = 0; i < bytes_read_from_USB; i++)
         {
         serout(OUT_Packet[i]);
         }
      }
    }
  }

/**********************************************************************
 * NAME       bit collect_sesor_data(void)
 * PURPOSE    To collect data sent by sensor.
 *
 * ARGUMENTS  char * destin - buffer to put data into 
 *
 * RETURNS    0 - if there is no data available.
 *            1 - otherwise
 *
 * EXTERNS    None.
 *
 * NOTES:    This function collects all characters from serial 
 *           connection into separate buffer until a CR is found.
 *           When received characters are terminated by CR it means
 *           that sensor has finished transmitting data and contents
 *           of the buffer can be used to use this data.
 *           The data is returned as string.
 **********************************************************************/
bit collect_sesor_data(char xdata* destin)
  {
  static unsigned char ndx;
  char c;

  if (!serhit())
    return (0);
  c = serinp();
  destin[ndx++] = c;

  if (ndx == MAX_WRITE_BYTE)
    {
    ndx = 0;
    return (0);                /* if buffer overflow.... */
    }

  if (destin[ndx - 1] == 0x0D) /* if we got LF */
    {
    destin[ndx] = '\0';        /* add NULL terminator */
    ndx = 0;
    return (1);
    }
  else
    return (0);  
  }

void set_XY_mode(void)
  {
  serout('M');
  serout('S');
  serout(';');

  serout('X');
  serout('Y');
  serout(';');

  serout('M');
  serout('C');
  serout(';');
  }

