#include <c8051f120.h>
#include <string.h>
#include <stdio.h>
#include "mn_userconst.h"                      // TCP/IP Library Constants
#include "mn_stackconst.h"                     // TCP/IP Library Constants
#include "mn_errs.h"                           // Library Error Codes
#include "mn_defs.h"                           // Library Type definitions
#include "mn_funcs.h"                          // Library Function Prototyes
#include "netfinder.h"
#include "ntp.h"
#include "telnet.h"
#include "i2c_slave.h"
#include "main.h"

//#include "globals.h"

// States
#define SMB_MSWR				0x60
#define SMB_DATA_RECEIVED		0x80

#define SMB_MSRD				0xA8
#define SMB_DATA_TRANSMITTED	0xB8

#define SMB_STOP_RECEIVED		0xA0

enum { IP_ADDR=0, SUBNET_ADDR, GATEWAY_ADDR, DNS_ADDR, DHCP_ADDR, TELNET_ADDR, RESET_PASS_ADDR, FACTORY_RESET, READ_VERSION };


sbit	LED1	=	P2^4;
sbit	LED2	=	P2^5;
sbit	LED3	=	P2^6;
sbit	LED4	=	P2^7;

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------


unsigned char in_buffer[16];
unsigned char xdata in_index=0;

unsigned char out_buffer[32]="                                ";
unsigned char xdata out_index=0;

// These should be defined as globals somewhere
//unsigned char NetfinderName[32]="Netfinder name                  ";
//unsigned char MAC_addr[6]=		{0,1,2,3,4,5};
//unsigned char IP_addr[4]	=	{127,0,0,1};
//unsigned char subnet_addr[4]=	{127,0,0,1};
//unsigned char gateway_addr[4]=	{127,0,0,1};
unsigned char dns_addr[4]=		{0,0,0,0};
//unsigned char telnet_enable=	0;

void setup_I2C()
{
	SFRPAGE= SMB0_PAGE;

	// NTP address in PT5300; 0xA6 = NTP
	SMB0ADR=0xA6; 

	// Assert acknowledge, must be set to enable SMB bus to recognise address
	SMB0CN|=0x04;
}


// I2C Interrupt service routine
void SMBus_ISR() interrupt 7 using 1
{
//	EIE1 = 0;

	switch(SMB0STA)
	{
		case SMB_MSWR:	// start master write
			in_index=0;
			break;

		case SMB_DATA_RECEIVED:	// data received
			in_buffer[in_index++]=SMB0DAT;
			
			if(in_index==1 && SMB0DAT&0x80) // If first byte received, and "incoming" flag set (MSB)
				set_out_buffer(SMB0DAT&0x3F); // remove MSB, as this just indicates "ingoing"
			
			break;

		case SMB_MSRD:	// start master read
			SMB0DAT = out_buffer[out_index++];
			break;

		case SMB_DATA_TRANSMITTED: // one byte transmitted
			SMB0DAT = out_buffer[out_index++];
			break;

		case SMB_STOP_RECEIVED: // stop received
			interpret_msg();
			break;
	}

	// clear interrupt flag
	SMB0CN &= (~0x08);

//	EIE1 = 1;
}


// called when message was received
// in_buffer[0] = message ID
// in_buffer[1-x] = message
// in_index = message lenght + 1 byte for ID
void interpret_msg()
{
	switch(in_buffer[0])
	{
		case IP_ADDR:
			ip_src_addr[0] = in_buffer[4];
			ip_src_addr[1] = in_buffer[3];
			ip_src_addr[2] = in_buffer[2];
			ip_src_addr[3] = in_buffer[1];
			b_write_network_settings = 1;	// Tell server to exit and write IP configuration to CP2200 flash.
		break;

		case SUBNET_ADDR:
			subnet_mask[0] = in_buffer[4];
			subnet_mask[1] = in_buffer[3];
			subnet_mask[2] = in_buffer[2];
			subnet_mask[3] = in_buffer[1];
			b_write_network_settings = 1;	// Tell server to exit and write IP configuration to CP2200 flash.
		break;

		case GATEWAY_ADDR:
			gateway_ip_addr[0] = in_buffer[4];
			gateway_ip_addr[1] = in_buffer[3];
			gateway_ip_addr[2] = in_buffer[2];
			gateway_ip_addr[3] = in_buffer[1];
			b_write_network_settings = 1;	// Tell server to exit and write IP configuration to CP2200 flash.
		break;

		case DNS_ADDR:
			dns_addr[0] = in_buffer[4];
			dns_addr[1] = in_buffer[3];
			dns_addr[2] = in_buffer[2];
			dns_addr[3] = in_buffer[1];
		break;

		case DHCP_ADDR:
			DHCP_Enable = in_buffer[1];
			if (DHCP_Enable == 0)
			{
				ip_src_addr[0] = 0x00;
				ip_src_addr[1] = 0x00;
				ip_src_addr[2] = 0x00;
				ip_src_addr[3] = 0x01;
			}
			b_write_network_settings = 1;	// Tell server to exit and write IP configuration to CP2200 flash.
		break;

		case TELNET_ADDR:
			TELNET_Enable = in_buffer[1];
			b_write_network_settings = 1;	// Tell server to exit and write IP configuration to CP2200 flash.
		break;

		case RESET_PASS_ADDR:
			//LED1 = 1;
			//LED2 = 0;
			//LED3 = 1;
			b_reset_password = 1;
			b_write_network_settings = 1;	// Tell server to exit and write IP configuration to CP2200 flash.
		break;

		//unknown data
		default:
			//LED1 = 0;
			//LED2 = 0;
			//LED3 = 0;
		break;
	}
}

// Outgoing data handler
void set_out_buffer(char str_num)
{
	unsigned char j;
	out_index=0;

	switch(str_num)
	{
		case 0:
			// asking for netfindername
			//memcpy(out_buffer, "0    ", 5);
			sprintf(out_buffer, "%u    ", TELNET_PORT);
			break;

		case 1:
			// asking for MAC address
			//memcpy(out_buffer, MAC_addr, sizeof(MAC_addr));
			out_buffer[0] = eth_src_hw_addr[0];
			out_buffer[1] = eth_src_hw_addr[1];
			out_buffer[2] = eth_src_hw_addr[2];
			out_buffer[3] = eth_src_hw_addr[3];
			out_buffer[4] = eth_src_hw_addr[4];
			out_buffer[5] = eth_src_hw_addr[5];

			break;

		case 2:
			// asking for IP address
			//memcpy(out_buffer, IP_addr, sizeof(IP_addr));
			out_buffer[0] = ip_src_addr[3];
			out_buffer[1] = ip_src_addr[2];
			out_buffer[2] = ip_src_addr[1];
			out_buffer[3] = ip_src_addr[0];
			break;

		case 3:
			// asking for subnet address
			//memcpy(out_buffer, subnet_addr, sizeof(subnet_addr));
			out_buffer[0] = subnet_mask[3];
			out_buffer[1] = subnet_mask[2];
			out_buffer[2] = subnet_mask[1];
			out_buffer[3] = subnet_mask[0];
			break;

		case 4:
			// asking for gateway address
			//memcpy(out_buffer, gateway_addr, sizeof(gateway_addr));
			out_buffer[0] = gateway_ip_addr[3];
			out_buffer[1] = gateway_ip_addr[2];
			out_buffer[2] = gateway_ip_addr[1];
			out_buffer[3] = gateway_ip_addr[0];
			break;

		case 5:
			// asking for dns address
			//memcpy(out_buffer, dns_addr, sizeof(dns_addr));
			out_buffer[0] = 0;
			out_buffer[1] = 0;
			out_buffer[2] = 0;
			out_buffer[3] = 0;
			break;

		case 6:
			// asking for DHCP status
			out_buffer[0] = DHCP_Enable; // 1 = Enabled, 0 = Disabled.
			break;

		case 7:
			// asking for TELNET status
			out_buffer[0] = TELNET_Enable; // 1 = Enabled, 0 = Disabled.
			break;

		// case 8: This is the netfinder name.

		case 9:
			// asking for software version.
			out_buffer[0] = 20; //2.0
			break;

		default: // As default 32 bytes should be returned. It saves CODE space to return Case 0 data instead.
			// unknown
			// memcpy(out_buffer, "................................", 32);
		    for(j = 0; j < 32; j++)
			{
				out_buffer[j] = Netfinder_Name[j];
			}

			break;
	}
}
