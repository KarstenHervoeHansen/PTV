//------------------------------------------------------------------------------
// netfinder.c
//------------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
//
// Description:
//    This file contains the routines which implement the netfinder protocol.
//
// Generated by TCP/IP Library Builder Version 3.23

#include "mn_userconst.h"                      // TCP/IP Library Constants
#include "mn_stackconst.h"                     // TCP/IP Library Constants
#include "mn_errs.h"                           // Library Error Codes
#include "mn_defs.h"                           // Library Type definitions
#include "mn_funcs.h"                          // Library Function Prototyes
#include "netfinder.h"                         // Netfinder User Options 
#include <string.h>


//------------------------------------------------------------------------------
// Constants and Type Definitions
//------------------------------------------------------------------------------
#define INVALID_BUFFER_ADDRESS  -1
#define REQUEST_NOT_BROADCAST   -2
#define MAC_MISMATCH            -3
#define UNRECOGNIZED_PACKET     -4

typedef struct EVENT_DATA_S {
   unsigned int days;
   unsigned char hours;
   unsigned char minutes;
   unsigned char seconds;
} EVENT_DATA;


//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
int netfinder_socketno = -1;           // Define a variable for the netfinder
                                       // socket number to be assigned by 
                                       // the mn_open() routine. 

char code STRINGA[] = STRING_A;
char code STRINGB[] = STRING_B;
char code STRINGC[] = STRING_C;
char code STRINGD[] = STRING_D;

#define NETFINDER_BUFF_SIZE   32+sizeof(STRINGA)+sizeof(STRINGB)+sizeof(STRINGC)+sizeof(STRINGD)
unsigned char netfinder_buff[NETFINDER_BUFF_SIZE];

EVENT_DATA event1;
EVENT_DATA event2;

bit need_reset_socket = 0;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
int netfinder_start(void);             // Starts the netfinder service
int netfinder_process_packet(void);
void netfinder_update_RTC(void);
void netfinder_reset_event1(void);
void netfinder_reset_event2(void);

//------------------------------------------------------------------------------
// netfinder_start
//------------------------------------------------------------------------------
//
// This routine opens a UDP socket at the NETFINDER port. If application layer
// services such as HTTP or FTP are enabled, then packets addressed to the 
// netfinder port will be placed in netfinder_buff and the callback function
// callback_app_server_process_packet() should be handled.
//
// If HTTP or FTP are not enabled, then mn_receive() should be periodically
// called on the open socket to check if any packets have arrived.
//
int netfinder_start(void)
{
   int status;

   // Open a new UDP socket.
   status = mn_open(  null_addr,           // Destination IP
                      NETFINDER_PORT,      // Source Port
                      0,                   // Destination Port
                      NO_OPEN,             // Do not wait for connection
                      PROTO_UDP,           // UDP Socket
                      STD_TYPE,            // Reqired constant
                      netfinder_buff,      // Address of receive buffer
                      NETFINDER_BUFF_SIZE  // Receive buffer size
                    );

   // Check the value returned from mn_open()
   // Possible Return Codes:
   //    0 - 126:            A valid socket number.
   //    NOT_SUPPORTED:      Unsupported Protocol
   //    NOT_ENOUGH_SOCKETS: Too many open sockets
   
   if(status < 0){
      
      // Error: Return Error Code
      return status;

   } else {
      
      // Valid socket number
      netfinder_socketno = status;
      return 1;
   }
   
}

//------------------------------------------------------------------------------
// netfinder_process_packet
//------------------------------------------------------------------------------
//
// This routine processes a netfinder packet after it has been received.
// 
// Note: This routine only processes packets received on the netfinder socket
// with the number <netfinder_socketno>.
//
// Note: This routine assumes that only a single packet has been received. It
// should be called immediately after calling mn_recv(), or directly from the 
// callback function that provided notification that a UDP packet was received.
//
// Note: The Netfinder socket must be reset after each Netfinder transaction. A
// Netfinder transaction is defined as a received packet that does not generate 
// a response, or a response to a valid packet. When an invalid packet is recieved,
// the socket is immediately reset. After a response to a valid packet is sent, this
// routine sets the <need_reset_socket> flag to trigger a socket reset from the 
// netfinder_update_RTC() routine. This is required because after any packet is sent
// or received, the Netfinder socket is bound to the address of the sender and can 
// only send or receive packets from that address until the socket is reset.
//
int netfinder_process_packet()
{
   PSOCKET_INFO socket_ptr;
   unsigned char *buffer_start = netfinder_buff;
   
   unsigned int i;
   bit mac_ok;

   static unsigned int rand = 0;                     // Random Number 

   // Obtain socket pointer
   socket_ptr = MK_SOCKET_PTR(netfinder_socketno);   

   //-----------------------------------
   // Verify and Handle Received Packet
   //-----------------------------------
   
   // Check for data placed in incorrect buffer
   if(socket_ptr->recv_ptr != buffer_start){
     
      // Reset Socket and Return
      mn_close(netfinder_socketno);
      netfinder_start();  
      return INVALID_BUFFER_ADDRESS;      
   
   } else 
   
   // Check if the packet is a Broadcast Identity Request
   if((socket_ptr->recv_len == 4) && (netfinder_buff[0] == 0x00)){
      
      //----------------------
      // Check Random Number
      //----------------------      
      // If random number matches the random number of the last
      // received request, then ignore this packet. This prevents
      // the generation of duplicate traffic. The Netfinder app
      // typically sends 3 - 8 broadcast requests using the same
      // random number for each search.
      
      if((netfinder_buff[2] == ((rand>>8) & 0xFF)) && (netfinder_buff[3] == (rand & 0xFF)) ){
               
         // Reset Socket
         mn_close(netfinder_socketno);
         netfinder_start();
         
         // Return
         return 1;

      } else {
         rand = (int) netfinder_buff[2] << 8;
         rand |=  netfinder_buff[3];
      }

      //----------------------
      // Build Identity Reply
      //----------------------

      // Set Netfinder packet type
      netfinder_buff[0] = 0x01;

      // Set the Alert Level (static = yellow(1), dhcp = green(0))
      if(dhcp_lease.infinite_lease == 1){
         netfinder_buff[1] = 0x01;
      } else {
         netfinder_buff[1] = 0x00;
      }

      // Leave the Random Sequence in the buffer ([2] & [3])

      // Set Event1 Days Hours Minutes
      netfinder_buff[4] = (event1.days >> 8);
      netfinder_buff[5] = (event1.days & 0xFF); 
      netfinder_buff[6] = (event1.hours);
      netfinder_buff[7] = (event1.minutes); 
       
      // Set Event2 Days Hours Minutes
      netfinder_buff[8] = (event2.days >> 8);
      netfinder_buff[9] = (event2.days & 0xFF); 
      netfinder_buff[10] = (event2.hours);
      netfinder_buff[11] = (event2.minutes); 
      
      // Set Event1 + Event2 seconds
      netfinder_buff[12] = (event1.seconds);
      netfinder_buff[13] = (event2.seconds);
      
      // Set MAC Address
      netfinder_buff[14] = eth_src_hw_addr[0]; 
      netfinder_buff[15] = eth_src_hw_addr[1];
      netfinder_buff[16] = eth_src_hw_addr[2];
      netfinder_buff[17] = eth_src_hw_addr[3];
      netfinder_buff[18] = eth_src_hw_addr[4];
      netfinder_buff[19] = eth_src_hw_addr[5];

      // Set IP Address
      netfinder_buff[20] = ip_src_addr[0]; 
      netfinder_buff[21] = ip_src_addr[1];
      netfinder_buff[22] = ip_src_addr[2];
      netfinder_buff[23] = ip_src_addr[3];
      
      // Set Subnet Mask
      netfinder_buff[24] = subnet_mask[0]; 
      netfinder_buff[25] = subnet_mask[1];
      netfinder_buff[26] = subnet_mask[2];
      netfinder_buff[27] = subnet_mask[3];
      
      // Set Default Gateway
      netfinder_buff[28] = gateway_ip_addr[0]; 
      netfinder_buff[29] = gateway_ip_addr[1];
      netfinder_buff[30] = gateway_ip_addr[2];
      netfinder_buff[31] = gateway_ip_addr[3];
     
      // Set String A (Name/Type Field)
      strcpy(&netfinder_buff[32], STRINGA);
      
      // Set String B (Description)
      strcpy(&netfinder_buff[32+sizeof(STRINGA)], STRINGB);       
      
      // Set String C (Event 1 Text)
      strcpy(&netfinder_buff[32+sizeof(STRINGA)+sizeof(STRINGB)], STRINGC);
    
    // Set String D (Event 2 Text)
      strcpy(&netfinder_buff[32+sizeof(STRINGA)+sizeof(STRINGB)+sizeof(STRINGC)], STRINGD);

    //--------------------------------
    // Send Identity Reply
    //--------------------------------
      
      // Place transmit data in the socket.
      // This will automatically be transmitted by the stack after this
      // routine exits.

      socket_ptr->send_ptr = netfinder_buff;
      socket_ptr->send_len = 32+sizeof(STRINGA)+sizeof(STRINGB)+sizeof(STRINGC)+sizeof(STRINGD);
                       
      need_reset_socket = 1;      

   } else 

   // Check if the packet is a Identity Assignment
   if((socket_ptr->recv_len == 24) && (netfinder_buff[0] == 0x02)){
    
      //--------------------------------
    // Verify MAC Address
    //--------------------------------
      for(i = 0; i < ETH_ADDR_LEN; i++){
         mac_ok = 1;
         if(eth_src_hw_addr[i] != netfinder_buff[i+16]){
            mac_ok = 0;
         }
      }

      //--------------------------------            
      // Update Local Identity 
      //--------------------------------
      if(mac_ok){
         
         // Update IP Address
         ip_src_addr[0] = netfinder_buff[4];
         ip_src_addr[1] = netfinder_buff[5];
         ip_src_addr[2] = netfinder_buff[6];
         ip_src_addr[3] = netfinder_buff[7];
         
         // Update Subnet Mask
         subnet_mask[0] = netfinder_buff[8];
         subnet_mask[1] = netfinder_buff[9];
         subnet_mask[2] = netfinder_buff[10];
         subnet_mask[3] = netfinder_buff[11];
         
         // Update Default Gateway
         gateway_ip_addr[0] = netfinder_buff[12];
         gateway_ip_addr[1] = netfinder_buff[13];
         gateway_ip_addr[2] = netfinder_buff[14];
         gateway_ip_addr[3] = netfinder_buff[15];
      
      }      
      
      //------------------------------------------
      // Build Identity Assignment Acknowlegement
      //------------------------------------------
     
      // Set Netfinder packet type
      netfinder_buff[0] = 0x03;
      
      // Set the response code
      if(mac_ok){
         netfinder_buff[1] = 0x01;
      } else {
         netfinder_buff[1] = 0x00;
      }         

      //------------------------------------------
      // Send Identity Assignment Acknowlegement
      //------------------------------------------
      
      // Place transmit data in the socket.
      // This will automatically be transmitted by the stack after this
      // routine exits.
      socket_ptr->send_ptr = netfinder_buff;
      socket_ptr->send_len = 4;
      
      need_reset_socket = 1;

   } else {
      
      // Reset Socket and Return
      mn_close(netfinder_socketno);
      netfinder_start(); 
      return UNRECOGNIZED_PACKET; 

   }

   return 1;
 
}

//------------------------------------------------------------------------------
// netfinder_update_RTC
//------------------------------------------------------------------------------
//
// This routine should be called once per second to update the RTC
//
// If the Netfinder socket needs to be reset, this routine waits for a one second
// to pass then resets the socket.
//
void netfinder_update_RTC(void)
{
   static bit reset_socket_delay = 0;
   
   // Update Event 1 RTC
   event1.seconds++;
   if(event1.seconds >= 60){
      event1.seconds = 0;
      event1.minutes ++;
      if(event1.minutes >= 60){
         event1.minutes = 0;
         event1.hours++;
         if(event1.hours >= 24){
            event1.hours = 0;
            event1.days++;
         }
      }
   }

   // Update Event 2 RTC
   event2.seconds++;
   if(event2.seconds >= 60){
      event2.seconds = 0;
      event2.minutes ++;
      if(event2.minutes >= 60){
         event2.minutes = 0;
         event2.hours++;
         if(event2.hours >= 24){
            event2.hours = 0;
            event2.days++;
         }
      }
   }
   
   // Check if the Netfinder socket needs to be reset.
   if(reset_socket_delay){
      reset_socket_delay = 0;

      // Reset Socket
      mn_close(netfinder_socketno);
      netfinder_start();
   }
   
   // Check if the netfinder module has flagged that a socket reset is required.
   // If so, then start the socket reset delay.
   if(need_reset_socket){
      need_reset_socket = 0;
      reset_socket_delay = 1;
   }
      

}

//------------------------------------------------------------------------------
// netfinder_reset_event1
//------------------------------------------------------------------------------
//
// This routine resets event 1 time to zero seconds
//
void netfinder_reset_event1(void)
{
   event1.days = 0;
   event1.hours = 0;
   event1.minutes = 0;
   event1.seconds = 0;

}


//------------------------------------------------------------------------------
// netfinder_reset_event2
//------------------------------------------------------------------------------
//
// This routine resets event 2 time to zero seconds
//
void netfinder_reset_event2(void)
{
   event2.days = 0;
   event2.hours = 0;
   event2.minutes = 0;
   event2.seconds = 0;

}

