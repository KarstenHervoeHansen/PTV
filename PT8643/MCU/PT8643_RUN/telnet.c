//------------------------------------------------------------------------------
// telnet.c
//------------------------------------------------------------------------------
//
// Description:
//    This file contains the routines which implement the TCP Echo Server.
//
#include <c8051F120.h>                         // Device-specific SFR Definitions
#include <stdio.h>							// SPRINTF
#include "mn_userconst.h"					// TCP/IP Library Constants
#include "mn_stackconst.h"					// TCP/IP Library Constants
#include "mn_errs.h"						// Library Error Codes
#include "mn_defs.h"						// Library Type definitions
#include "mn_funcs.h"						// Library Function Prototyes
#include <ctype.h>							// TOLOWER
#include <string.h>							// STRCMP
#include "UART_0.h"
#include "ublox_prot.h"
#include "telnet.h"							// Telnet Server Header File
#include "I2C_Bus.h"						// Telnet Server Header File
#include "Main.h"
#include "ntp.h"

#define CP2201_Flash_Pass 		0x1C31; //0x1FDD;

//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
unsigned int TELNET_PORT = 23;				// Telnet Port Number.

int telnet_socketno = -1;					// Holds the the Echo Server socket 
											// number. This number is assigned 
											// by the mn_open() routine. 

unsigned char telnet_socketstate = DISCONNECTED;  
											   
// Echo Server Transmit Buffer.
unsigned char telnet_TX_BUFF[telnet_TX_BUFF_SIZE];      

// Echo Server Receive Buffer.
unsigned char telnet_RX_BUFF[telnet_RX_BUFF_SIZE];      

unsigned char telnet_CMD_BUFF[telnet_CMD_BUFF_SIZE];      
unsigned char telnet_CMD_BUFF_PTR;      

unsigned int telnet_send_length;			// The size of the data in the transmit buffer.							

bit telnet_send_now = 0;					// Flag to indicate that the transmit 
											// buffer has been filled and is ready to be sent.
										
char telnet_SendTXBUFF(unsigned int length);

unsigned long telnet_InactivityTimer = 0x0000;

sbit	LED2	=	P2^5;

//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------
int telnet_start(void);             
void telnet_process_packet(void);
SCHAR telnet_server_idle(void);
void telnet_socket_closed(void);
void telnet_CMD_BUFF_PUTBYTE (unsigned char c);

//------------------------------------------------------------------------------
// telnet_start
//------------------------------------------------------------------------------
// 
// This routine starts the Telnet Server by opening a TCP socket and placing it
// in the IDLE state. 
//
int telnet_start(void)
{
	int i;

	for(i = 0; i < telnet_RX_BUFF_SIZE; i++) telnet_RX_BUFF[i] = 0x00; // Clear the buffer... Debug...
	telnet_InactivityTimer = TELNETTIMEOUT;

	// Open a new TCP socket.
	i = mn_open(  null_addr,				// Destination IP -- allow any IP
											// address to connect
						TELNET_PORT,		// Source Port -- specify the local port number										
						0,					// Destination Port -- allow any remote port to connect                                          
						NO_OPEN,			// Do not wait for connection to be established (i.e. non-blocking)                                          
						PROTO_TCP,			// TCP Socket
						STD_TYPE,			// Reqired constant
						telnet_RX_BUFF,		// Address of receive buffer
						telnet_RX_BUFF_SIZE	// Receive buffer size
					);
   // Check the value returned from mn_open()
   // Possible Return Codes:
   //    0 - 126:            A valid socket number.
   //    NOT_SUPPORTED:      Unsupported Protocol
   //    NOT_ENOUGH_SOCKETS: Too many open sockets
   
   if(i < 0){
      
      // Set the socket state to DISCONNECTED
      telnet_socketstate = DISCONNECTED;
	  return 0;

   } else {
      
      // Assign the return value of mn_open (a valid socket number) to the 
      // current socket number and set the socket state to IDLE
      telnet_socketno = i;
      telnet_socketstate = IDLE;
	  LED2 = 1;
	  return 1;
   }
   
}

//------------------------------------------------------------------------------
// telnet_process_packet
//------------------------------------------------------------------------------
//
// This routine processes a packet after it has been received.
// 
//
void telnet_process_packet()
{
	int i, j;
	unsigned int BootVer1, BootVer2, BootID, RunVer1, RunVer2, RunID;

	PSOCKET_INFO socket_ptr;
	unsigned char IAC = 0;

	// Obtain socket pointer
	socket_ptr = MK_SOCKET_PTR(telnet_socketno);   
   
	//----------------------------------------------
	// Handle incoming packet based on socket state
	//----------------------------------------------
	EIE2 &= (~0x04);
    telnet_InactivityTimer = TELNETTIMEOUT;
	EIE2 |= 4;

	// IDLE State: This indicates that a connection has been established
	if(telnet_socketstate == IDLE)
	{  
		telnet_CMD_BUFF_PTR	= 0;
		telnet_socketstate = LOGIN_Name;
		for(j = 0; j < 32; j++) { telnet_CMD_BUFF[j] = 0x00;} //Prepare the buffer to receive a password...

		telnet_send_length = sprintf(telnet_TX_BUFF, "Login: ");
		telnet_send_now = 1;
		return;
	}
	else if(telnet_socketstate == LOGIN_Name)
	{
		for(i = 0; i < socket_ptr->recv_len; i++)
    	{      			
		   	//WDTCN = 0xA5; // Reset WDT.
			if (telnet_RX_BUFF[i] == 0xFF) IAC = 2; 		// Skip IAC commands...
			else if (IAC > 0) IAC--;
			else if ( (telnet_RX_BUFF[i] >= 0x20) && (telnet_RX_BUFF[i] <= 0x7E))
			{
				telnet_CMD_BUFF_PUTBYTE(telnet_RX_BUFF[i]);
			}		
			else if ((telnet_RX_BUFF[i] == 0x08) && (telnet_CMD_BUFF_PTR > 0) ){telnet_CMD_BUFF_PTR--;}
			else if (telnet_RX_BUFF[i] == 0x0D)
			{
				// Username correct.
				telnet_CMD_BUFF_PTR = 16;			
				telnet_socketstate = LOGIN; // Password correct.
				telnet_send_length = sprintf(telnet_TX_BUFF, "Password: %c%c%c%c%c%c", 0xFF, 0xFB, 0x01, 0xFF, 0xFE, 0x01);    //FF FD 01
				telnet_send_now = 1;			
			}
			else if ( (telnet_socketstate == LOGIN_Name) && (telnet_CMD_BUFF_PTR == 16)) {telnet_CMD_BUFF_PTR = 0;}
		}

	}
	else if(telnet_socketstate == LOGIN)
	{
		for(i = 0; i < socket_ptr->recv_len; i++)
    	{      	
			if  (telnet_CMD_BUFF_PTR > 32) {telnet_CMD_BUFF_PTR = 16;}
			
			if ( (telnet_RX_BUFF[i] >= 0x20) && (telnet_RX_BUFF[i] <= 0x7E))
			{
				telnet_CMD_BUFF_PUTBYTE(telnet_RX_BUFF[i]);
			}		
			else if ((telnet_RX_BUFF[i] == 0x08) && (telnet_CMD_BUFF_PTR > 0) ){telnet_CMD_BUFF_PTR--;}
			else if (telnet_RX_BUFF[i] == 0x0D)
			{

				for(j = 0; j < 31; j++)
				if (telnet_CMD_BUFF[j] != CPFLASH_ByteRead(j + 0x1C21))
				{	// Password incorrect.
					mn_close(telnet_socketno);
					return;
				}

				// Password correct.
				telnet_CMD_BUFF_PTR = 0;			
				telnet_socketstate = CONNECTED; // Password correct.

				telnet_send_length = sprintf(telnet_TX_BUFF, "%c%c%c%c%c%c\n\rWelcome to the PT5300 Telnet interface.\n\rUnit Name.: ", 0xFF, 0xFC, 0x01, 0x40, 0x40, 0x40);//, 0xFF, 0xFD, 0x01);  // IAC WONT ECHO  IAC DO ECHO

				for(j = 0; j < 32; j++)
				{	
					telnet_TX_BUFF[j + telnet_send_length] = CPFLASH_ByteRead(j + 0x1C01);
					if (telnet_TX_BUFF[j + telnet_send_length] == 0x00)
					{
						telnet_send_length	+= j;
						break;
					}
				}

				telnet_TX_BUFF[0 + telnet_send_length] = 0x0A;
				telnet_TX_BUFF[1 + telnet_send_length] = 0x0D;
				telnet_send_length += 2;
				telnet_send_now = 1;			
				
				i2c_set_rs232dest(RS232_MCU_MF); //RS232_DSUB_MCU; RS232_MCU_MF RS232_DSUB_MF
			}
		}

	}
    else if(telnet_socketstate == CONNECTED)	// CONNECTED State: This indicates a data packet has been received
	{
		for(i = 0; i < socket_ptr->recv_len; i++)
    	{      	
			if ( (telnet_RX_BUFF[i] >= 0x20) && (telnet_RX_BUFF[i] <= 0x7E))
			{
				telnet_CMD_BUFF_PUTBYTE(telnet_RX_BUFF[i]);
							
			}		
			else if (telnet_RX_BUFF[i] == 0x0D) // Process command...
			{
				telnet_CMD_BUFF_PUTBYTE(0x00); // Make sure the CMD is ended with a 0x00 (CR replaced with 0x00)
				telnet_CMD_BUFF_PTR = 0x00;
				
				//-------------------------< Show help >-----------------------		
				if (strcmp(telnet_CMD_BUFF, "@AT+++") == 0) 
				{
					telnet_socketstate = COMMAND;
					telnet_send_length = sprintf(telnet_TX_BUFF, "OK\n\r");    
					telnet_send_now = 1;				
				}

			}

			COM0PutByte(telnet_RX_BUFF[i]);
		}
		
		Transmit_COM0();
		LED2 = !LED2;
	} 

	else if(telnet_socketstate == COMMAND)
	{
		for(i = 0; i < socket_ptr->recv_len; i++)
    	{      	
			if ( (telnet_RX_BUFF[i] >= 0x20) && (telnet_RX_BUFF[i] <= 0x7E))
			{
				telnet_CMD_BUFF_PUTBYTE(telnet_RX_BUFF[i]);
							
			}		
			else if (telnet_RX_BUFF[i] == 0x0D) // Process command...
			{
				telnet_CMD_BUFF_PUTBYTE(0x00); // Make sure the CMD is ended with a 0x00 (CR replaced with 0x00)
				telnet_CMD_BUFF_PTR = 0x00;
				
				//----------------- ----< Exit Command Mode >------ ---------------		
				if (strcmp(telnet_CMD_BUFF, "@AT---") == 0) 
				{
					telnet_socketstate = CONNECTED;
					telnet_send_length = sprintf(telnet_TX_BUFF, "OK\n\r");    
					telnet_send_now = 1;				
				}

				//-------------------< Get module information >--------------------		
				else if (strcmp(telnet_CMD_BUFF, "STATUS") == 0) 
				{
					BootVer1 = read_flash_addr(FLASH_BLInfo_ADDR);
					BootVer2 = read_flash_addr(FLASH_BLInfo_ADDR + 1);
					BootID	 = read_flash_addr(FLASH_BLInfo_ADDR + 2);
					RunVer1  = read_flash_addr(FLASH_RUNInfo_ADDR);
					RunVer2  = read_flash_addr(FLASH_RUNInfo_ADDR + 1);
					RunID	 = read_flash_addr(FLASH_RUNInfo_ADDR + 2);
					telnet_send_length = sprintf(telnet_TX_BUFF, "%d.%d.%d,%d.%d.%d,%02X-%02X-%02X-%02X-%02X-%02X,%d\n\r", BootVer1, BootVer2, BootID, RunVer1, RunVer2, RunID,(int) eth_src_hw_addr[0],(int) eth_src_hw_addr[1],(int) eth_src_hw_addr[2],(int) eth_src_hw_addr[3],(int) eth_src_hw_addr[4],(int) eth_src_hw_addr[5],(int) SNTP_Enable);    
					telnet_send_now = 1;				
				}

				//----------------------< Show GPS Signal >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "GPSSTATUS") == 0) 
				{
					telnet_send_length = sprintf(telnet_TX_BUFF,									
						 "\n\rGPS Channel Number: 01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16\n\rSignal Quality    : %02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i\n\r",
						 (unsigned int) signal_quality[ 0], (int) signal_quality[ 1], (int) signal_quality[ 2],(int) signal_quality[ 3],
						 (unsigned int) signal_quality[ 4], (int) signal_quality[ 5], (int) signal_quality[ 6],(int) signal_quality[ 7],
						 (unsigned int) signal_quality[ 8], (int) signal_quality[ 9], (int) signal_quality[10],(int) signal_quality[11],
						 (unsigned int) signal_quality[12], (int) signal_quality[13], (int) signal_quality[14],(int) signal_quality[15] );    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length,									
						 "Signal Strength   : %02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u,%02u\n\r",
						 (unsigned int) signal_strength[ 0], (unsigned int) signal_strength[ 1], (unsigned int) signal_strength[ 2],(unsigned int) signal_strength[ 3],
						 (unsigned int) signal_strength[ 4], (unsigned int) signal_strength[ 5], (unsigned int) signal_strength[ 6],(unsigned int) signal_strength[ 7],
						 (unsigned int) signal_strength[ 8], (unsigned int) signal_strength[ 9], (unsigned int) signal_strength[10],(unsigned int) signal_strength[11],
						 (unsigned int) signal_strength[12], (unsigned int) signal_strength[13], (unsigned int) signal_strength[14],(unsigned int) signal_strength[15] );    

					telnet_send_now = 1;				
				}


				//----------------------< Show DHCP Status >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "DHCP") == 0) 
				{ 
					telnet_send_length = sprintf(telnet_TX_BUFF,                       "\n\rDHCP Lease  : %lu\n\r", (unsigned long) dhcp_lease.org_lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Expire : %lu\n\r", (unsigned long) dhcp_lease.lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T1     : %lu\n\r", (unsigned long) dhcp_lease.t1_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T2     : %lu\n\r", (unsigned long) dhcp_lease.t2_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP State  : %u\n\r", (unsigned int) dhcp_lease.dhcp_state);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Server : %u.%u.%u.%u\n\r", (unsigned int) dhcp_lease.server_id[0], (unsigned int) dhcp_lease.server_id[1], (unsigned int) dhcp_lease.server_id[2], (unsigned int) dhcp_lease.server_id[3]);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Renew  : %i\n\r", (int) iDHCPRenew);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Count  : %u\n\r", (unsigned int) DHCPRenewCount);    
					telnet_send_now = 1;				
				}

				//----------------------< Show BOOT Timestamp >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "SYSTEMTIME") == 0) 
				{ 
					telnet_send_length = sprintf(telnet_TX_BUFF,                   "\n\rCurrent Time : %04u-%02u-%02u %02u:%02u:%02u UTC\n\r", (int)UTC_time_buffer.year, (int)UTC_time_buffer.month, (int)UTC_time_buffer.day, (int)UTC_time_buffer.hour, (int)UTC_time_buffer.min, (int)UTC_time_buffer.sec);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "System BOOT  : %04u-%02u-%02u %02u:%02u:%02u UTC\n\r", (int)UTC_Boot_Time.year, (int)UTC_Boot_Time.month, (int)UTC_Boot_Time.day, (int)UTC_Boot_Time.hour, (int)UTC_Boot_Time.min, (int)UTC_Boot_Time.sec);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "Server Start : %04u-%02u-%02u %02u:%02u:%02u UTC\n\r", (int)UTC_ServerStart_Time.year, (int)UTC_ServerStart_Time.month, (int)UTC_ServerStart_Time.day, (int)UTC_ServerStart_Time.hour, (int)UTC_ServerStart_Time.min, (int)UTC_ServerStart_Time.sec);    

					telnet_send_now = 1;				
				}
				/*
				//----------------------< FORCE DHCP ERROR >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "DHCPERR1") == 0) 
				{ 
					iDHCPRenew = -100;
					telnet_send_length = sprintf(telnet_TX_BUFF,                       "\n\rDHCP Lease  : %lu\n\r", (unsigned long) dhcp_lease.org_lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Expire : %lu\n\r", (unsigned long) dhcp_lease.lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T1     : %lu\n\r", (unsigned long) dhcp_lease.t1_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T2     : %lu\n\r", (unsigned long) dhcp_lease.t2_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP State  : %u\n\r", (unsigned int) dhcp_lease.dhcp_state);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Server : %u.%u.%u.%u\n\r", (unsigned int) dhcp_lease.server_id[0], (unsigned int) dhcp_lease.server_id[1], (unsigned int) dhcp_lease.server_id[2], (unsigned int) dhcp_lease.server_id[3]);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Renew  : %i\n\r", (int) iDHCPRenew);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Count  : %u\n\r", (unsigned int) DHCPRenewCount);    
					telnet_send_now = 1;				
				}

				//----------------------< FORCE DHCP ERROR >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "DHCPERR2") == 0) 
				{ 
					dhcp_lease.lease_time = 0xFFFFFFFE;
					telnet_send_length = sprintf(telnet_TX_BUFF,                       "\n\rDHCP Lease  : %lu\n\r", (unsigned long) dhcp_lease.org_lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Expire : %lu\n\r", (unsigned long) dhcp_lease.lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T1     : %lu\n\r", (unsigned long) dhcp_lease.t1_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T2     : %lu\n\r", (unsigned long) dhcp_lease.t2_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP State  : %u\n\r", (unsigned int) dhcp_lease.dhcp_state);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Server : %u.%u.%u.%u\n\r", (unsigned int) dhcp_lease.server_id[0], (unsigned int) dhcp_lease.server_id[1], (unsigned int) dhcp_lease.server_id[2], (unsigned int) dhcp_lease.server_id[3]);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Renew  : %i\n\r", (int) iDHCPRenew);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Count  : %u\n\r", (unsigned int) DHCPRenewCount);    
					telnet_send_now = 1;				
				}
				
				//----------------------< FORCE DHCP ERROR >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "DHCPRENEW") == 0) 
				{ 
					iDHCPRenew = mn_dhcp_renew(dhcp_default_lease_time);
					telnet_send_length = sprintf(telnet_TX_BUFF,                       "\n\rDHCP Lease  : %lu\n\r", (unsigned long) dhcp_lease.org_lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Expire : %lu\n\r", (unsigned long) dhcp_lease.lease_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T1     : %lu\n\r", (unsigned long) dhcp_lease.t1_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP T2     : %lu\n\r", (unsigned long) dhcp_lease.t2_renew_time);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP State  : %u\n\r", (unsigned int) dhcp_lease.dhcp_state);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Server : %u.%u.%u.%u\n\r", (unsigned int) dhcp_lease.server_id[0], (unsigned int) dhcp_lease.server_id[1], (unsigned int) dhcp_lease.server_id[2], (unsigned int) dhcp_lease.server_id[3]);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Renew  : %i\n\r", (int) iDHCPRenew);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "DHCP Count  : %u\n\r", (unsigned int) DHCPRenewCount);    
					telnet_send_now = 1;				
				}
				*/
				//----------------------< Show Lock Status >-----------------------		
				else if (strcmp(telnet_CMD_BUFF, "GPSLOCK") == 0) 
				{ 
					telnet_send_length = sprintf(telnet_TX_BUFF,                       "\n\rGPS Lock Threshold : %u\n\r", (unsigned) GPS_LOCK_TIMEOUT);    
					telnet_send_length += sprintf(telnet_TX_BUFF + telnet_send_length, "GPS Lock Count     : %u\n\r", (unsigned) gps_lock_timeout);    
					telnet_send_now = 1;				
				}
				
				//-------------------------< Show help >--------------------------		
				else if ((strcmp(telnet_CMD_BUFF, "HELP") == 0) || (strcmp(telnet_CMD_BUFF, "help") == 0))
				{
					telnet_send_length = sprintf(telnet_TX_BUFF, "\n\rCommands.\n\rSTATUS    : Show software versions.\n\rGPSSTATUS : Show GPS Information.\n\rGPSLOCK   : Show GPS Lock status.\n\rDHCP      : Show DHCP status.\n\r");    
					telnet_send_now = 1;				
				}

				else if (strcmp(telnet_CMD_BUFF, "EXIT") == 0) 
				{
					//mn_close(telnet_socketno); // Close the telnet connection...
					telnet_socketstate = DISCONNECTED;
				}

				else
				{
					telnet_send_length = sprintf(telnet_TX_BUFF, "Not OK\n\r");    
					telnet_send_now = 1;				
				}



			}
			else if (telnet_RX_BUFF[i] == 0x18) // Ctrl + X
			{
				telnet_socketstate = DISCONNECTED;
				//mn_close(telnet_socketno); // Close the telnet connection...
			}

		}

	}

}

//------------------------------------------------------------------------------
// telnet_server_idle
//------------------------------------------------------------------------------
//
// This routine is called when the TCP/IP Library is idle.
// 
// Return Values:
//    NEED_TO_SEND <-114> - The library will immediately send the data stored in
//    the socket.
//
//    NEED_TO_EXIT <-102> - The mn_server() routine will exit immediately, returning
//    control to the main() routine.
//
//    FALSE <0> - The mn_server() routine will continue to function normally.
//
SCHAR telnet_server_idle(void)
{
	unsigned int i;

	SCHAR retval = 0;

	PSOCKET_INFO socket_ptr;

	// Obtain socket pointer
	socket_ptr = MK_SOCKET_PTR(telnet_socketno);   
         
	//---------------------------------
	// Task #1 
	// Send Packet (if neccessary)
	//----------------------------------

	// Check if a packet should be sent.

	if( (telnet_socketstate == CONNECTED) && (RX0Ready == 0) && (RX0count > 0) ) // DEBUG ENABLED 20120305
	{
		if( (RX0Ready == 0) && (RX0count > 0) )
		{
			i = 0;
			while (RX0count > 0)
			{
				telnet_TX_BUFF[i] = COM0GetByte();
				i++;
			}
			telnet_send_length = i;
			telnet_send_now = 1;
		}
	}

	if( ((telnet_socketstate == CONNECTED) || (telnet_socketstate == LOGIN_Name) || (telnet_socketstate == LOGIN) || (telnet_socketstate == COMMAND)) && telnet_send_now)
	{
      
		// Clear the telnet_send_now flag to prevent the same packet
		// from being transmitted twice
		telnet_send_now = 0;
     
		// Send a packet
		socket_ptr->send_ptr = telnet_TX_BUFF;     // Pointer to Transmit Buffer
		socket_ptr->send_len = telnet_send_length; // Number of bytes to send
		retval = NEED_TO_SEND;  
	} else 
   
	//---------------------------------
	// Task #2 
	// Check if socket needs to be opened
	//----------------------------------
   
	if(telnet_socketstate == DISCONNECTED)    
	{
		// Start or Re-Start the server
	    mn_close(telnet_socketno);
		telnet_start();
	}      
   
   	if (telnet_socketstate == DISABLED) 
	{
		mn_close(telnet_socketno);

	}

	//LED2 = !LED2;
	return retval;
}

//------------------------------------------------------------------------------
// telnet_socket_closed
//------------------------------------------------------------------------------
//
// This routine is called when the TCP socket is closed
//
void telnet_socket_closed(void)
{
	// Update the socket state variable
	if (telnet_socketstate != DISABLED) telnet_socketstate = DISCONNECTED;
	i2c_set_rs232dest(RS232_DSUB_MF); //RS232_DSUB_MCU; RS232_MCU_MF RS232_DSUB_MF
}

void telnet_CMD_BUFF_PUTBYTE(unsigned char c)
{
	if (telnet_CMD_BUFF_PTR >= telnet_CMD_BUFF_SIZE) { telnet_CMD_BUFF_PTR = 0x00;}
	telnet_CMD_BUFF[telnet_CMD_BUFF_PTR] = c;
	telnet_CMD_BUFF_PTR++;
}

char telnet_SendTXBUFF(unsigned int length)
{
	PSOCKET_INFO socket_ptr = MK_SOCKET_PTR(telnet_socketno);   
	// Define and Obtain socket pointer

	EIE2 &= (~0x04);
    telnet_InactivityTimer = TELNETTIMEOUT; // The instrument can stream data by it self, and if it does, keep the connection open.
	EIE2 |= 4;

	socket_ptr->send_len = length; // Number of bytes to send
	socket_ptr->send_ptr = RX0Buffer; //telnet_TX_BUFF;     // Pointer to Transmit Buffer

	RX0count = 0;
	return (mn_send(telnet_socketno, socket_ptr->send_ptr, socket_ptr->send_len));		
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          