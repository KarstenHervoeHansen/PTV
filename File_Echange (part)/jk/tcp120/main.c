//------------------------------------------------------------------------------
// main.c
//------------------------------------------------------------------------------
// Copyright (C) 2005 Silicon Laboratories, Inc.
//
// Date: 12/06/07 13:23:01
// Target: C8051F12x 
//
// Description:
//    This file contains the main routine, MCU initialization code, and
//    callback functions used by the TCP/IP Library.
//
// Generated by TCP/IP Configuration Wizard Version 3.23
//

// Notes:  This program works both as Client and Server, and interacts with
//         Server and Client runing on a PC. 
//         As Client: this program opens a TCP socket to remote Server and
//                    sends a data string.  The remote Server responds with
//                    a string containing a sequence number.  This program
//                    upon reception of that string, prints it via RS232.
//                    The remote Server responds 15 times and then terminates.
//                    If everything is OK, this program prints all 15 received
//                    strings in correct sequence.  It did that on 10.12.2007.
//                    This program also indicates whether data received came
//                    from the same packet as ACK (mn_send()) or from a separate
//                    call to mn_recv().
//


#pragma WARNINGLEVEL (1) /* Lists only those warnings which may generate */
                         /* incorrect code. Default is 2.                */

#include <c8051F120.h>                         // Device-specific SFR Definitions
#include <stdio.h>
#include <string.h>
#include "mn_userconst.h"                      // TCP/IP Library Constants
#include "mn_stackconst.h"                     // TCP/IP Library Constants
#include "mn_errs.h"                           // Library Error Codes
#include "mn_defs.h"                           // Library Type definitions
#include "mn_funcs.h"                          // Library Function Prototypes
#include "netfinder.h"
#include "uart.h"

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------
sfr16 RCAP2    = 0xca;                         // Timer2 reload value
sfr16 TMR2     = 0xcc;                         // Timer2 counter
sfr16 ADC0     = 0xbe;                         // ADC0 data register
 
//------------------------------------------------------------------------------
// Global Constants
//------------------------------------------------------------------------------
#define SYSCLK                  98000000L      // System Clock Frequency in Hz
                                               // calculating UART baud rate)
#define T2_OVERFLOW_RATE        130L           // Timer 2 Overflow Rate in Hz

#define BAUDRATE                9600           // Baud rate of UART in bps
#define DATA_BUFF_LEN             40
#define SERVER_MODE                1

//-----------------------------------------------------------------------------
// Global Variables 
// code char PT5230HeadlineTxt[]    = "HD-SD Sync Generator Frame.";
//-----------------------------------------------------------------------------
char sending_buff[DATA_BUFF_LEN];
char rece_buff[DATA_BUFF_LEN];

char tbuff[50];


sbit SW2 = P3^7;                       // SW2='0' means switch pressed
sbit LED = P1^6;                       // LED='1' means ON


//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

// Initialization Routines
void PORT_Init (void);
void SYSCLK_Init (void);
void EMIF_Init(void);
int  establish_network_connection();
void UART1_Init (void);
void print(char *buff);
void TCP(void);
void Timer2_Init(void);

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
void main(void)
  {
  int retval;

  // Disable watchdog timer
  WDTCN = 0xde;
  WDTCN = 0xad;

  // Initialize the MCU
  PORT_Init();
  SYSCLK_Init();
  EMIF_Init();
  UART1_Init ();
  Timer2_Init();

  EA  = 1;
  LED = 0;

#if SERVER_MODE
  sprintf(tbuff, "SERVER running\n");
  print(tbuff);
#else
  sprintf(tbuff, "CLIENT running\n");
  print(tbuff);
#endif

   // Reset the Netfinder Event 1 counter after a Power-On Reset or Pin Reset
   // The event1 counter counts how long the device has been powered.
   if (RSTSRC & 0x03)
     {
     netfinder_reset_event1();
     }

  while(1)
    {
    // Initialize the TCP/IP stack.
    if (mn_init() < 0)
      {
      // If code execution enters this while(1) loop, the stack failed to initialize.
      // Verify that all boards are connected and powered properly.
      sprintf(tbuff, "Cannot Initialize mn_init...%d\n", (int)10);
      print(tbuff);
      while(1)
        ;
      }

    // Connect to the network
    establish_network_connection(); /* init ethernet ctrl */

    // Use DHCP to obtain an IP address. It will be written into ip_src_addr[]
    if (mn_dhcp_start(PTR_NULL, dhcp_default_lease_time) <= 0)
      {
      // DHCP Error
      // The DHCP server did not assign a valid IP address.
      sprintf(tbuff, "DHCP Error...%d\n", (int)10);
      print(tbuff);

      // Override DHCP
      dhcp_lease.infinite_lease = 1;
      dhcp_lease.dhcp_state     = DHCP_OK;

      // Specify a static IP address
      ip_src_addr[0] = 0;
      ip_src_addr[1] = 0;
      ip_src_addr[2] = 0;
      ip_src_addr[3] = 1;
      }
    else
      {
      sprintf(tbuff, "DHCP src: %d.%d.%d.%d\n", (int)ip_src_addr[0], 
                                                (int)ip_src_addr[1],
                                                (int)ip_src_addr[2],
                                                (int)ip_src_addr[3]);
      print(tbuff);
      sprintf(tbuff, "src MAC:  0x%0x.0x%0x.0x%0x.0x%0x.0x%0x.0x%0x\n", (int)eth_src_hw_addr[0], 
                                                                  (int)eth_src_hw_addr[1],
                                                                  (int)eth_src_hw_addr[2],
                                                                  (int)eth_src_hw_addr[3],
                                                                  (int)eth_src_hw_addr[4],
                                                                  (int)eth_src_hw_addr[5]);

      print(tbuff);
      
      sprintf(tbuff, "Name: %s\n", dhcp_info.sname);
      print(tbuff);
      }

      // Reset Netfinder event2 counter
      // The event2 counter counts how long the device has been on a network.
      netfinder_reset_event2();

      // Start the Netfinder Service
      if (netfinder_start() < 0)
        {
         // If code execution enters this while(1) loop, the Netfinder
         // service failed to open a UDP socket.
         //  - Verify that the current library contains UDP.
         //  - Verify that there are at least two sockets allocated.
        sprintf(tbuff, "netfinder failed ...%d\n", (int)10);
        print(tbuff);
        while(1)
          ;
        }

    // Run the TCP example
#if 1
    ip_dest_addr[0] = 192;
    ip_dest_addr[1] = 168;
    ip_dest_addr[2] = 120;
    ip_dest_addr[3] = 120;
#endif
    while (1)
      {
      sprintf(tbuff, "Silabs TCP Server waiting...\n");
      print(tbuff);
      TCP();
      }

    /***********************************************************************/
    /* for the netfinder to find our machine, we must be in mn_server()    */
    /***********************************************************************/

    sprintf(tbuff, "Enterinig mn_server.\n");
    print(tbuff);

    // Start the Application Layer Services
    // If this routine exits, check the return value for an error code.
    retval = mn_server();
    sprintf(tbuff, "mn_server terminated...%d\n", (int)10);
    print(tbuff);
    }
  }

//-----------------------------------------------------------------------------
// TCP
//-----------------------------------------------------------------------------
//
void TCP(void)
  {
  SCHAR        socket_no;
  PSOCKET_INFO socket_ptr;
  byte        *data_ptr;
  word16       data_len;
  int          status;
  int          i;
  char         abuff[4];

#if SERVER_MODE
  
  // When dest_ip is null_addr any IP address is allowed to connect
  // ECHO_PORT is sorce_port
  // 0         is desti_port: if 0 it will be filled automaticly by TCP stack
  //           Must be 0 in SERVER_MODE
  socket_no = mn_open(null_addr, ECHO_PORT, 0, PASSIVE_OPEN, PROTO_TCP,\
                      STD_TYPE, sending_buff, DATA_BUFF_LEN);
  data_ptr  = PTR_NULL;
  data_len  = 0;
  
#else

  /* NOTE: The machine @ip_dest_addr must run a server when this function (mn_open) */
  /*       is called.  Otherwise, we will get TCP_OPEN_FAILED = 0xFF84 error        */


  socket_no = mn_open(ip_dest_addr, (word16)DEFAULT_PORT, (word16)ECHO_PORT, (byte)ACTIVE_OPEN, (byte)PROTO_TCP,\
                      (byte)STD_TYPE, (byte*)sending_buff, (word16)DATA_BUFF_LEN);


#endif

  if (socket_no < 0)
    {
    sprintf(tbuff, "Fail: socket_no = 0x%x\n", (int)socket_no);
    print(tbuff);
    LED = 0;
    return;
    }
  else
    {
    sprintf(tbuff, "OK: socket_no = 0x%x\n", (int)socket_no);
    print(tbuff);
    }

  socket_ptr = MK_SOCKET_PTR(socket_no);        // get pointer to the socket

  strcpy(sending_buff, "JZ Ethernet");
  data_len  = strlen(sending_buff);

#if 0  

  sprintf(tbuff, "Echo string: %s\n", sending_buff); 
  print(tbuff);
  sprintf(tbuff, "Len of echo = %d\n", (int)data_len); 
  print(tbuff);
#endif

  i = 1;
  while(1)
    {
    status = 0;
    
    /* mn_send sends the data and waits for an ACK. Some echo servers will
     return the ACK and the data in the same packet, so we need to handle
     that case.  We receive different data each time, they are sent       
     via sercom to a terminal, so we can see if they are correct.  They
     are (10.12.2007). */
    if (data_ptr != PTR_NULL)
      {
      status = mn_send(socket_no, sending_buff, data_len); /* send data */
#if 1      
      if (status > 0 && socket_ptr->recv_len > 0)      /* case 1: if in the same packet.... */
        {
        data_ptr = socket_ptr->recv_ptr;  /* this one is always correct */
        data_len = socket_ptr->recv_len;

        data_ptr[data_len] = 0;
        sprintf(tbuff, "same: %s l=%d\n", data_ptr, data_len); 
        print(tbuff);
        continue;                                      /* if OK, keep sending */
        }
#endif
      }

    if (status < 0 || socket_ptr->tcp_state == TCP_CLOSED)
      {
#if 1
      if (status < 0)
        {
        sprintf(tbuff, "status < 0, s = %d\n", (int)status); 
        print(tbuff);
        }
      if (socket_ptr->tcp_state == TCP_CLOSED)
        {                                      /* if server on remote machine is terminated */
        sprintf(tbuff, "TCP_CLOSED\n");        /* we get here TCP_CLOSED and status < 0     */
        print(tbuff);
        }
#endif
      LED = 1;
      break;
      }

    do
      {
      status = mn_recv(socket_no, rece_buff, DATA_BUFF_LEN);  /* case 2: if NOT in the same packet.... */
      } while (status == NEED_TO_LISTEN);

      if (status < 0)
        {
        LED = 0;
        break;
        }

      // if we got something, send back what we got
      if (status > 0)
        {
        sprintf(abuff, "%d", i); 
        data_ptr = socket_ptr->recv_ptr;
        strcat(data_ptr, abuff);
        data_len = socket_ptr->recv_len;
        data_ptr[data_len] = 0;
        sprintf(tbuff, "DAT: %s l=%d\n", data_ptr, data_len); 
        print(tbuff);
        i++;
        }
     }
   mn_abort(socket_no);
  }



//-----------------------------------------------------------------------------
// establish_network_connection
//-----------------------------------------------------------------------------
//
// This function calls mn_ether_init() to initialize the CP2200 and attach to
// the network.
//
// If there is a network connection, the function returns 1.
//
// In the call to mn_ether_init(), NUM_AUTONEG_ATTEMPTS is set to 0, so the
// function will not return until it successfully auto-negotiates.
//
// mn_ether_init() will not be a blocking call if NUM_AUTONEG_ATTEMPTS is set
// to a value greater than 0.
//
int establish_network_connection()
  {
  int retval;

  do
    {
    // mn_ether_init() initializes the Ethernet controller.
    // AUTO_NEG indicates that the controller will auto-negotiate.
    retval = mn_ether_init(AUTO_NEG, 0, 0);

    // If there is no link, poll link_status until it sets or the
    // CP2200 resets and then call mn_ether_init() again.
    if (retval == LINK_FAIL)
      {
      while(!link_status && !ether_reset)
        ;
      }

    // If retval is less than zero and is not LINK_FAIL, there is a 
    // hardware error.
    else 
      if (retval < 0)
        {
        // Verify that the Ethernet controller is connected and powered properly.
        // Verity that the EMIF has been configured at a speed compatible with the
        //    Ethernet controller.
        while(1)
          ;
        }
    } while (retval < 0);
  return (1);
  }



//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Interrupts, Crossbar and GPIO ports
//
void PORT_Init (void)
  {
  char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page
  
  SFRPAGE = CONFIG_PAGE;           // set SFR page
  P0MDOUT |= 0x01;                 // Set TX1 pin to push-pull 
  P1MDOUT |= 0x40;                 // Set P1.6(TB_LED) to push-pull
  P2MDOUT |= 0x0C;                 // Set P2.2(AB4_LED1) and P2.3 (AB4_LED2)
  // all pins used by the external memory interface are in push-pull mode
  P4MDOUT =  0xDF;
  P5MDOUT =  0xFF;
  P6MDOUT =  0xFF;
  P7MDOUT =  0xFF;
  P4 = 0xC0;                       // /WR, /RD, are high, RESET is low
  P5 = 0xFF;
  P6 = 0xFF;                       // P5, P6 contain the address lines
  P7 = 0xFF;                       // P7 contains the data lines
  
  TCON &= ~0x01;                   // Make /INT0 level triggered
  
  // Enable UART1, CP0, and /INT0. This puts /INT0 on P0.3.
  XBR0 = 0x80;
  XBR1 = 0x04;
  XBR2 = 0x44;
  SFRPAGE = SFRPAGE_SAVE;       // Restore SFR page
  }


//-----------------------------------------------------------------------------
// EMIF_Init
//-----------------------------------------------------------------------------
//
// Configure the External Memory Interface for both on and off-chip access.
//
void EMIF_Init (void)
{
   char SFRPAGE_SAVE = SFRPAGE;  // Save Current SFR page

   SFRPAGE = LEGACY_PAGE;

   EMI0CF = 0xFB;             // Split-mode, non-multiplexed on P0 - P3

   EMI0TC = EMIF_TIMING;      // This constant may be modified
                              // according to SYSCLK to meet the
                              // timing requirements for the CP2200

   EMI0CN = BASE_ADDRESS;     // Page of XRAM accessed by EMIF
   SFRPAGE = SFRPAGE_SAVE;    // Restore SFR page

}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock.
//
void SYSCLK_Init (void)
{
   int i;                           // software timer

   char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;           // set SFR page

   OSCICN = 0x83;                   // set internal oscillator to run
                                    // at its maximum frequency

   CLKSEL = 0x00;                   // Select the internal osc. as
                                    // the SYSCLK source
   //Turn on the PLL and increase the system clock by a factor of M/N
   PLL0CN  = 0x00;                  // Set internal osc. as PLL source
   SFRPAGE = LEGACY_PAGE;
   FLSCL   = 0x30;                  // Set FLASH read time for 100 MHz clk
   SFRPAGE = CONFIG_PAGE;

   PLL0CN |= 0x01;                  // Enable Power to PLL

   PLL0DIV = 0x01;                  // Set Pre-divide value to N (N = 1)
   PLL0MUL = 0x04;                  // Multiply SYSCLK by M (M=4)
   PLL0FLT = 0x01;                  // Set the PLL filter register for
                                    // a reference clock from 12.2 - 19.5 MHz
                                    // and an output clock from 65 - 100 MHz
   for (i=0; i < 256; i++) ;        // Wait at least 5us
   PLL0CN  |= 0x02;                 // Enable the PLL
   while(!(PLL0CN & 0x10));         // Wait until PLL frequency is locked
   CLKSEL  = 0x02;                  // Select PLL as SYSCLK source

   SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page

}


//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
// Configure the UART1 using Timer1, for <baudrate> and 8-N-1.
//
void UART1_Init (void)
  {
  char SFRPAGE_SAVE = SFRPAGE;     // Save Current SFR page
  
  SFRPAGE = UART1_PAGE;
  SCON1   = 0x10;                  // SCON1: mode 0, 8-bit UART, enable RX
  
  SFRPAGE = TIMER01_PAGE;
  TMOD   &= ~0xF0;
  TMOD   |=  0x20;                 // TMOD: timer 1, mode 2, 8-bit reload
  
  
  // Set Timer 1 timebase.
  // Note: Since Timer 0 is used by the TCP/IP Library and forces the 
  // shared T0/T1 prescaler to SYSCLK/48, Timer 1 may only be clocked 
  // from SYSCLK or SYSCLK/48
  
  // If reload value is less than 8-bits, select SYSCLK
  // as Timer 1 baud rate generator
  #if (SYSCLK/BAUDRATE/2/256 < 1) 
    TH1 = -(SYSCLK/BAUDRATE/2);
    CKCON |= 0x10;                // T1M = 1; SCA1:0 = xx
  
  // Otherwise, select SYSCLK/48 prescaler.
  #else
    // Adjust for truncation in special case
    // Note: Additional cases may be required if the system clock is changed.
    #if ((BAUDRATE == 115200) && (SYSCLK == 98000000))
       TH1 = -((SYSCLK/BAUDRATE/2/48)+1); 
    #else
       TH1 = -(SYSCLK/BAUDRATE/2/48);
    #endif
  
    CKCON &= ~0x13;               // Clear all T1 related bits
    CKCON |=  0x02;               // T1M = 0; SCA1:0 = 10
  #endif  
  
  TL1 = TH1;                       // initialize Timer1
  TR1 = 1;                         // start Timer1  
  
  SFRPAGE = UART1_PAGE;
  serinit();
  SFRPAGE = SFRPAGE_SAVE;          // Restore SFR page
  }

//-----------------------------------------------------------------------------
// ether_reset_low
//-----------------------------------------------------------------------------
//
// This routine drives the reset pin of the ethernet controller low.
//
void ether_reset_low()
{

   unsigned char SFRPAGE_Save;

   SFRPAGE_Save = SFRPAGE;    // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;     // Switch to ports SFR page

   P4 &= ~0x20;               // Pull reset low

   SFRPAGE = SFRPAGE_Save;    // Restore SFR page

}

//-----------------------------------------------------------------------------
// ether_reset_high
//-----------------------------------------------------------------------------
//
// This routine places the reset pin in High-Z allowing it to be pulled up 
// using the external pull-up resistor.
//
// Additionally, this routine waits for the reset pin to read high before
// exiting.
//
void ether_reset_high (void)
{

   unsigned char SFRPAGE_Save;

   SFRPAGE_Save = SFRPAGE;    // Save Current SFR page
   SFRPAGE = CONFIG_PAGE;     // Switch to ports SFR page

   P4 |= 0x20;               // Allow /RST to rise
   while(!(P4 & 0x20));      // Wait for /RST to go high

   SFRPAGE = SFRPAGE_Save;    // Restore SFR page


}

void print(char *buff)
  {
  while (*buff)
    serout(*(buff++));
  }

//-----------------------------------------------------------------------------
// Timer2_ISR (T2_OVERFLOW_RATE Hz)
//-----------------------------------------------------------------------------
//
void Timer2_ISR (void) interrupt 5
{

   // Define static counters for real time clock (RTC).
   static unsigned int RTC_counter = 0;

   // Clear Timer 2 Overflow Flag
   TF2 = 0;

   // Check if one second has passed and update RTC.
   if(RTC_counter >= T2_OVERFLOW_RATE){

      // Clear counter and update real time clock
      RTC_counter = 0;
      netfinder_update_RTC();

   } else {
      // Increment interrupt count
      RTC_counter++;
   }

}

//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
// This routine initializes Timer 2 to <T2_OVERFLOW_RATE> Hz.
//
void Timer2_Init(void)
{  
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   
   SFRPAGE = TMR2_PAGE;                // Set SFR page   
   TMR2CF  = 0x00;                     // Stop Timer2; Use SYSCLK/12 as timebase
 
   // Initialize Reload Value
   RCAP2 = -(SYSCLK/12/T2_OVERFLOW_RATE);
   TMR2 = RCAP2;
 
   ET2 = 1;                            // Enable Timer 2 interrupts
   TR2 = 1;                            // Start Timer 2
 
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
 
}
