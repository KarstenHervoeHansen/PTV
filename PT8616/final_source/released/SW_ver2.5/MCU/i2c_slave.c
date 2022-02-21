#include <c8051f320.h>
#include <string.h>
#include "i2c_slave.h"
#include "pt5300_handler.h"
#include "LTC_handler.h"
#include "globals.h"

#define WRITE 0x00 			// SMBus WRITE command
#define READ 0x01 			// SMBus READ command
#define SLAVE_ADDR 0xA4 	// Device addresses (7 bits) //0xA4 = GPS
#define SMB_SRADD 0x20 		// (SR) slave address received
							// (also could be a lost
							// arbitration)
#define SMB_SRSTO 0x10	 	// (SR) STOP detected while SR or ST,
							// or lost arbitration
#define SMB_SRDB 0x00		// (SR) data byte received, or
							// lost arbitration
#define SMB_STDB 0x40 		// (ST) data byte transmitted
#define SMB_STSTO 0x50 		// (ST) STOP detected during a
							// transaction; bus error
							// End status vector definition

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------
unsigned char SMB_DATA; // Global holder for SMBus data.

//in buffer
unsigned char xdata in_buffer[16];
unsigned char in_index;

unsigned char out_buffer_select;
unsigned char out_index;



sbit LED2 = P2^1;

bit i2c_msgupdate = 0; // Set to ‘1’ by the SMBus ISR


//interrupt ved overflow af lav SCL
void timer3_ISR() interrupt 14
{
	SMB0CF &= ~0x80; // Disable SMBus
	SMB0CF |= 0x80;  // Re-enable SMBus
	TMR3CN &= ~0x80; // Clear Timer3 interrupt-pending flag
}

void SMBus_ISR() interrupt 7
{
	if (ARBLOST == 0)
	{		
		// ACK received data

		switch (SMB0CN & 0xF0) // Decode the SMBus status vector
		{
			// Slave Receiver: Start+Address received
			case SMB_SRADD:
				STA = 0; // Clear STA bit
				if((SMB0DAT&0xFE) == (SLAVE_ADDR&0xFE)) // Decode address
				{ 										// If the received address matches,
					if((SMB0DAT&0x01) == READ) 			// If the transfer is a master READ,
						SMB0DAT = i2c_putch(); 			// Prepare outgoing byte
					else
						in_index=0;						// Else reset buffer in_index

					ACK = 1; 							// ACK the received slave address
				}
				else 					// If received slave address does not match,
					ACK = 0; 			// NACK received address
			break;

			// Slave Receiver: Data received
			case SMB_SRDB:
				SMB_DATA = SMB0DAT; 	// Store incoming data

				in_buffer[in_index++]=SMB_DATA;
				
				//hvis register over 100, skal der læses fra GPS modul
				if(in_index==1 && SMB_DATA>=100)
				{	
					out_index=0;
					pt5300_out_handle();
				}

				//in_index=in_index&15;

				ACK = 1;
			break;

			// Slave Receiver: Stop received while either a Slave Receiver or
			// Slave Transmitter
			case SMB_SRSTO:
				i2c_msgupdate = 1;		 	// Indicate new data received
				STO = 0;				// STO must be cleared by software when
										// a STOP is detected as a slave
			break;

			// Slave Transmitter: Data byte transmitted
			case SMB_STDB:
				
			break;
			// Slave Transmitter: Arbitration lost, Stop detected.
			// This state will only be entered on a bus error condition.
			// In normal operation, the slave is no longer sending data or has
			// data pending when a STOP is received from the master, so the TXMODE
			// bit is cleared and the slave goes to the SRSTO state.
			case SMB_STSTO:
				STO = 0; 		// STO must be cleared by software when
								// a STOP is detected as a slave
			
			break;
			// Default: all other cases undefined
			default:
				SMB0CF &= ~0x80; // Reset communication
				SMB0CF |= 0x80;
				STA = 0;
				STO = 0;
				ACK = 0;
			break;
		}
	}	

	// ARBLOST = 1, Abort failed transfer
	else
	{
		STA = 0;
		STO = 0;
		ACK = 0;
	}
	SI = 0; // Clear SMBus interrupt flag
}

char i2c_putch()
{	
	char out;

	//håndter output strenge
	switch (out_buffer_select)
	{
		case 0:
			out=LTC_a_str[out_index++];
			break;
		case 1:
			out=LTC_b_str[out_index++];
			break;
		case 2:
			out=PT5300_str[out_index++];
			break;

		case 5:
			out=master_status_byte;
			break;
		case 6:
			out=LTC_request;
			break;
		case 7:
			out=LTC_sync_request;
			break;
		case 8:
			out=0;
			if(LTC_a_daylight_flag)
				out+=1;
			if(LTC_b_daylight_flag)
				out+=16;
			break;

		case 9:
			out=phase_str[out_index++];
			break;
		case 10:
			out=counter_str[out_index++];
			break;
		case 11:
			out=quality_str[out_index++];
			break;
		case 12:
			out=debug_str[out_index++];
			break;

		case 101:
			out=current_MCU_sw_version;
			break;
		case 102:
			out=KU_str[out_index++];
			break;


	}

	return (out);
}