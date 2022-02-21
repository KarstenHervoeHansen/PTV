#include<c8051f320.h>

sbit LED1 = P2^0;

#define WRITE 0x00 			// SMBus WRITE command
#define READ 0x01 			// SMBus READ command
#define SLAVE_ADDR 0xA0 	// Device addresses (7 bits)
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

bit DATA_READY = 0; // Set to ‘1’ by the SMBus ISR

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
		switch (SMB0CN & 0xF0) // Decode the SMBus status vector
		{
			// Slave Receiver: Start+Address received
			case SMB_SRADD:
				STA = 0; // Clear STA bit
				if((SMB0DAT&0xFE) == (SLAVE_ADDR&0xFE)) // Decode address
				{ 										// If the received address matches,
					ACK = 1; 							// ACK the received slave address
					if((SMB0DAT&0x01) == READ) 			// If the transfer is a master READ,
						SMB0DAT = SMB_DATA; 			// Prepare outgoing byte
				}
				else 					// If received slave address does not match,
					ACK = 0; 			// NACK received address
			break;

			// Slave Receiver: Data received
			case SMB_SRDB:
				SMB_DATA = SMB0DAT; 	// Store incoming data
				DATA_READY = 1;		 	// Indicate new data received
				ACK = 1;				// ACK received data
			break;

			// Slave Receiver: Stop received while either a Slave Receiver or
			// Slave Transmitter
			case SMB_SRSTO:
				STO = 0;				// STO must be cleared by software when
										// a STOP is detected as a slave
			break;

			// Slave Transmitter: Data byte transmitted
			case SMB_STDB:
			// No action required;
			// one-byte transfers
			// only for this example
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

void setup_I2C()
{
	//I2C setup
	SMB0CF|=0x89;				//SMBus enabled, Slave mode, SCL low timeout, Bus free detect, timer 1 bitrate
	EIE1|=0x01;					//enable SMBus interrupt
}