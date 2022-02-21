#include <c8051f120.h>
#include <string.h>
#include <stdio.h>
#include "i2c_slave.h"
#include "main.h"
#include "mcu_flash.h"
//#include "globals.h"

// States
#define SMB_MSWR				0x60
#define SMB_DATA_RECEIVED		0x80

#define SMB_MSRD				0xA8
#define SMB_DATA_TRANSMITTED	0xB8

#define SMB_STOP_RECEIVED		0xA0

enum { IP_ADDR=0, SUBNET_ADDR, GATEWAY_ADDR, DNS_ADDR, DHCP_ADDR, TELNET_ADDR, RESET_PASS_ADDR, FACTORY_RESET };


sbit	LED1	=	P2^4;
sbit	LED2	=	P2^5;
sbit	LED3	=	P2^6;
sbit	LED4	=	P2^7;

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------


char in_buffer[16];
unsigned char xdata in_index=0;

char out_buffer[32]="                                ";
unsigned char xdata out_index=0;

char netfinder_name[32];

extern unsigned char bdata BOOT;				// Used for Interrupt redirection.

// These should be defined as globals somewhere
//unsigned char NetfinderName[32]="PT8643 Bootloader Mode...       ";
unsigned char MAC_addr[6]=		{0,0,0,0,0,0};
unsigned char IP_addr[4]	=	{0,0,0,0};
unsigned char subnet_addr[4]=	{0,0,0,0};
unsigned char gateway_addr[4]=	{0,0,0,0};
unsigned char dns_addr[4]=		{0,0,0,0};
unsigned char DHCP_enable=		1;
unsigned char telnet_enable=	0;



void setup_I2C()
{
	SFRPAGE= SMB0_PAGE;

	// NTP address in PT5300; 0xA6 = NTP
	SMB0ADR=0xA6; 

	// Assert acknowledge, must be set to enable SMB bus to recognise address
	SMB0CN|=0x04;
}


// I2C Interrupt service routine
void SMBus_ISR() interrupt 7
{
	if (BOOT == 0)							// Check if bootloader is running.
	{
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

	} else {((void (code *) (void)) 0x243B)();} //Jump to interrupt in run code.
}


// called when message was received
// in_buffer[0] = message ID
// in_buffer[1-x] = message
// in_index = message lenght + 1 byte for ID
void interpret_msg()
{

	switch(in_buffer[0])
	{
		/*
		case IP_ADDR:
			IP_addr[0]=in_buffer[1];
			IP_addr[1]=in_buffer[2];
			IP_addr[2]=in_buffer[3];
			IP_addr[3]=in_buffer[4];
		break;

		case SUBNET_ADDR:
			subnet_addr[0]=in_buffer[1];
			subnet_addr[1]=in_buffer[2];
			subnet_addr[2]=in_buffer[3];
			subnet_addr[3]=in_buffer[4];
		break;

		case GATEWAY_ADDR:
			gateway_addr[0]=in_buffer[1];
			gateway_addr[1]=in_buffer[2];
			gateway_addr[2]=in_buffer[3];
			gateway_addr[3]=in_buffer[4];
		break;

		case DNS_ADDR:
			dns_addr[0]=in_buffer[1];
			dns_addr[1]=in_buffer[2];
			dns_addr[2]=in_buffer[3];
			dns_addr[3]=in_buffer[4];
		break;

		case DHCP_ADDR:
			DHCP_enable=in_buffer[1];
		break;

		case TELNET_ADDR:
			telnet_enable=in_buffer[1];
		break;

		case RESET_PASS_ADDR:
			LED1=1;
			LED2=0;
			LED3=0;
		break;
		*/
		case FACTORY_RESET:
			ResetCPFlash();
			LED1=0;
			LED2=1;
			LED3=0;
			LED4=1;
		break;
		/*
		//unknown data
		default:
			LED1=0;
			LED2=0;
			LED3=0;
		break;
		*/
	}
}

// Outgoing data handler
void set_out_buffer(char str_num)
{
	out_index=0;

	switch(str_num)
	{
		case 0:
			// asking for netfindername
			memcpy(out_buffer, "None.", 5);
			break;

		case 1:
			// asking for MAC address
			memcpy(out_buffer, MAC_addr, sizeof(MAC_addr));
			break;

		case 2:
			// asking for IP address
			memcpy(out_buffer, IP_addr, sizeof(IP_addr));
			break;

		case 3:
			// asking for subnet address
			memcpy(out_buffer, subnet_addr, sizeof(subnet_addr));
			break;

		case 4:
			// asking for gateway address
			memcpy(out_buffer, gateway_addr, sizeof(gateway_addr));
			break;

		case 5:
			// asking for dns address
			memcpy(out_buffer, dns_addr, sizeof(dns_addr));
			break;

		default:
			// unknown
			memcpy(out_buffer, netfinder_name, 32);
			break;
	}
}
