#include <c8051F120.h>                  // Device-specific SFR Definitions
#include "mn_userconst.h"				// TCP/IP Library Constants
#include "mn_stackconst.h"				// TCP/IP Library Constants
#include "mn_errs.h"					// Library Error Codes
#include "mn_defs.h"					// Library Type definitions
#include "mn_funcs.h"					// Library Function Prototyes
#include "ublox_prot.h"
#include "ntp.h"						// Netfinder User Options 
#include <string.h>						// For use with MEMCPY.
//#include "ublox_prot.h"
//------------------------------------------------------------------------------
// Constants and Type Definitions
//------------------------------------------------------------------------------
#define INVALID_BUFFER_ADDRESS  -1
#define REQUEST_NOT_BROADCAST   -2
#define MAC_MISMATCH            -3
#define UNRECOGNIZED_PACKET     -4

//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
int ntp_socketno = -1;					// Define a variable for the netfinder
										// socket number to be assigned by 
										// the mn_open() routine. 

unsigned int ntp_socket_timeout		= 0x00;
unsigned char ntp_reset_socket_delay	= 0x00;
bit ntp_need_reset_socket				= 0;

unsigned long sec_counter				= 0x00000000; // 3445662566;
unsigned long sec_preload_counter		= 0x00000000;
unsigned long sec_frac					= 0;
unsigned char gps_lock_timeout			= 0x00;

typedef struct NTP_packet {
	unsigned char flags;
	unsigned char stratum_num;
	unsigned char peer_pool_int;
	unsigned char peer_clock_prec;
	long root_delay;
	long root_dispersion;
	unsigned char ref_clock_id[4];
	unsigned long ref_clock_update_time_int;
	unsigned long ref_clock_update_time_frac;
	unsigned long org_time_stamp_int;
	unsigned long org_time_stamp_frac;
	unsigned long rx_time_stamp_int;
	unsigned long rx_time_stamp_frac;
	unsigned long tx_time_stamp_int;
	unsigned long tx_time_stamp_frac;
};

unsigned char ntp_buff[NTP_BUFF_SIZE];

struct NTP_packet NTP_server;

sbit			LED1						= P2^4;
//------------------------------------------------------------------------------
// Function Prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ntp_start
//------------------------------------------------------------------------------
//
// This routine opens a UDP socket at the NTP port. If application layer
// services such as HTTP or FTP are enabled, then packets addressed to the 
// netfinder port will be placed in ntp_buff and the callback function
// callback_app_server_process_packet() should be handled.
//
// If HTTP or FTP are not enabled, then mn_receive() should be periodically
// called on the open socket to check if any packets have arrived.
//
int ntp_start(void)
{
	int status;

	// Open a new UDP socket.
	status = mn_open(  null_addr,				// Destination IP
						NTP_PORT,				// Source Port
						0,						// Destination Port
						NO_OPEN,				// Do not wait for connection
						PROTO_UDP,				// UDP Socket
						STD_TYPE,				// Reqired constant
						ntp_buff,				// Address of receive buffer
						NTP_BUFF_SIZE			// Receive buffer size
					);

	// Check the value returned from mn_open()
	// Possible Return Codes:
	//    0 - 126:            A valid socket number.
	//    NOT_SUPPORTED:      Unsupported Protocol
	//    NOT_ENOUGH_SOCKETS: Too many open sockets
   
	if(status < 0)
	{	// Error: Return Error Code
 		return status;

	}
	else
	{	// Valid socket number
		ntp_socketno = status;
		return 1;
	}
}

//------------------------------------------------------------------------------
// ntp_process_packet
//------------------------------------------------------------------------------
//
// This routine processes a NTP packet after it has been received.
// 
// Note: This routine only processes packets received on the NTP socket
// with the number <ntp_socketno>.
//
// Note: This routine assumes that only a single packet has been received. It
// should be called immediately after calling mn_recv(), or directly from the 
// callback function that provided notification that a UDP packet was received.
//
// Note: The NTP socket must be reset after each NTP transaction. A
// NTP transaction is defined as a received packet that does not generate 
// a response, or a response to a valid packet. When an invalid packet is recieved,
// the socket is immediately reset. After a response to a valid packet is sent, this
// routine sets the <need_reset_socket> flag to trigger a socket reset from the 
// netfinder_update_RTC() routine. This is required because after any packet is sent
// or received, the Netfinder socket is bound to the address of the sender and can 
// only send or receive packets from that address until the socket is reset.
//
int ntp_process_packet()
{
	PSOCKET_INFO socket_ptr;
	unsigned char *buffer_start = ntp_buff;

	// Obtain socket pointer
	socket_ptr = MK_SOCKET_PTR(ntp_socketno);   

	//-----------------------------------
	// Verify and Handle Received Packet
	//-----------------------------------
   
	// Check for data placed in incorrect buffer
	if(socket_ptr->recv_ptr != buffer_start)
	{	// Reset Socket and Return
		mn_close(ntp_socketno);
		ntp_start();  
		return INVALID_BUFFER_ADDRESS;      
	}
	else if((socket_ptr->recv_len == 48))// && (ntp_buff[0] == 0x00))
	{   // This is where it all happens... 
//		EA = 0;
//		ntp_need_reset_socket = 1; // Signal that the socket MUST be reset...

		memcpy(&NTP_server, ntp_buff, socket_ptr->recv_len);

		NTP_server.rx_time_stamp_int = sec_counter;
		NTP_server.rx_time_stamp_frac = sec_frac*FRACTION;

		NTP_server.root_delay=0;
		NTP_server.root_dispersion=0;

		NTP_server.peer_clock_prec = -20;
		NTP_server.ref_clock_update_time_int = NTP_server.rx_time_stamp_int; //sec_counter;
		NTP_server.ref_clock_update_time_frac = NTP_server.rx_time_stamp_frac; //sec_frac*FRACTION;
		
		NTP_server.flags = 0x24; //0x24 = No Warning, Version 4, Server mode.

		if (gps_lock_timeout >= GPS_LOCK_TIMEOUT) NTP_server.flags |= 0xC0;
		else if (UTC_leap_buffer.utc_leap_second >= 1) NTP_server.flags |= 0x40;
		else if (UTC_leap_buffer.utc_leap_second <= -1) NTP_server.flags |= 0x80;

		NTP_server.stratum_num = 1;
		
		NTP_server.org_time_stamp_int = NTP_server.tx_time_stamp_int;
		NTP_server.org_time_stamp_frac = NTP_server.tx_time_stamp_frac;

 		NTP_server.ref_clock_id[0] ='G';
		NTP_server.ref_clock_id[1] ='P';
 		NTP_server.ref_clock_id[2] ='S';
 		NTP_server.ref_clock_id[3] = 0x00;

		NTP_server.tx_time_stamp_int = sec_counter;
		NTP_server.tx_time_stamp_frac = sec_frac*FRACTION;
		
		memcpy(ntp_buff, &NTP_server, sizeof(NTP_server));

		socket_ptr->send_ptr = ntp_buff;
		socket_ptr->send_len = sizeof(NTP_server);

//		EA = 1;
		ntp_need_reset_socket = 1; // Signal that the socket MUST be reset...
	} else 
	{	// Reset Socket and Return
		mn_close(ntp_socketno);
		ntp_start(); 
		return UNRECOGNIZED_PACKET; 
	}

	return 1;
 
}

