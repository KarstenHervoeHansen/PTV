//-----------------------------------------------------------------------------
//	Filename	: mcu_flash.c
//	Date		: 2010-06-01
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "mcu_flash.h"
#include "main.h"
#include <c8051F120.h>                      // Device-specific SFR Definitions
#include "textstr.h"
#include "UART_0.h"
#include <ctype.h>                     // tolower() and toint()
#include <stdio.h> 
#include "i2c_bus.h"

//-----------------------------------------------------------------------------
// Constant
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------
extern unsigned char idata flash_secure_1;		// Should be set to 0x44 to enable Flash Fonctions.
extern unsigned char idata flash_secure_2;		// Should be set to 0x85 to enable Flash Writes.
extern unsigned char idata flash_secure_3; 		// Should be set to 0x02 to enable Flash Erase.
extern unsigned char idata flash_secure_4; 		// Should be set to 0x55 to enable MOVX Redirect.
extern bit code_erased;

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
unsigned char idata HexBuffer[64];		// Array to hold incomming Intel Hex data.
unsigned char HexCount;					// Number of bytes in the HexArray.
unsigned char HexIndex;					// Read Index in the HexArray.

unsigned char code Hex2Char[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//-----------------------------------------------------------------------------
// MCU_FLASH Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// read_flash_addr	: Read a byte from MCU flash memory. (Code memory.)
//-----------------------------------------------------------------------------
unsigned char read_flash_addr(unsigned int readaddress)
{
	char code* data pread;                 // pointer used for reading FLASH
	pread = readaddress;
	return *pread;
}

//-----------------------------------------------------------------------------
// erase_flash_page	: Erases 54 sectors of 1024 bytes starting from 0x2400.
//-----------------------------------------------------------------------------
void erase_flash_page(void)
{
    int i;
    char xdata* data pagePointer = FLASH_START_ADDR;// pointer to xdata space located 
                                           			// in data space

    bit	EA_state = EA;								// Holds interrupt state
 	char SFRPAGE_SAVE = SFRPAGE;     				// Save Current SFR page

	EA = 0;
	SFRPAGE = LEGACY_PAGE;

	for (i=0; i<55; i++)
	{	
		if (flash_secure_1 == 0x44)
		{
			if (flash_secure_2 == 0x85){FLSCL = BitOn(FLSCL, 0);}	// Enable Flash Writes. (Bit FLWE)
				else {PCON = 2;}					// Something is wrong, STOP the MCU.

			if (flash_secure_3 == 0x02){PSCTL = BitOn(PSCTL, 1);}	// Enable Flash Erase. (Bit PSEE)
				else {PCON = 2;}					// Something is wrong, STOP the MCU.

			if (flash_secure_4 == 0x55){PSCTL = BitOn(PSCTL, 0);}	// Redirect MOVX to flash. (Bit PSWE)
				else {PCON = 2;}					// Something is wrong, STOP the MCU.
		}
		else
		{
			SFRPAGE = CONFIG_PAGE;
			P2 = 0x00;
			PCON = 2;
		}											// Something is wrong, STOP the MCU.

	    *pagePointer = 0;                   		// Initiate the erase by writing to a byte in the selected sector.
		pagePointer = pagePointer + 1024;			// Skip to next sector.

		FLSCL = BitOff(FLSCL, 0);					// Disable Flash Erase.
		PSCTL = BitOff(PSCTL, 1);					// Redirect MOVX to Ram.
		PSCTL = BitOff(PSCTL, 0);					// Disable Flash Writes.
	
	}
	
	flash_secure_1	=0x00;
	flash_secure_2	=0x00;
	flash_secure_3	=0x00;
	flash_secure_4	=0x00;

    code_erased = 1;	              				// indicate that FLASH has been erased

	SFRPAGE = SFRPAGE_SAVE;       					// Restore SFR page
    EA = EA_state;                  				// restore interrupt state
}


//-----------------------------------------------------------------------------
// hex2char	: Convert a two-byte ASCII representation of a char and convert to a single 8 bit result
//-----------------------------------------------------------------------------
unsigned char hex2char(char byteH, byteL)
{ 
   unsigned char retval;
   retval = (char) toint(byteH) * 16;
   retval += (char) toint(byteL);
   return retval;
}

//-----------------------------------------------------------------------------
// write_flash_addr	: Write a single byte to MCU Flash.
//-----------------------------------------------------------------------------
void write_flash_addr(unsigned char c, char xdata* data flashAddress)
{
    char xdata* data WritePointer = flashAddress;// Pointer to xdata space.

    bit	EA_state = EA;								// Holds interrupt state
 	char SFRPAGE_SAVE = SFRPAGE;     				// Save Current SFR page

	EA = 0;
	SFRPAGE = LEGACY_PAGE;

	if (flash_secure_1 == 0x44)
	{
		if (flash_secure_2 == 0x85){FLSCL = BitOn(FLSCL, 0);}	// Enable Flash Writes. (Bit FLWE)
			else {PCON = 2;}					// Something is wrong, STOP the MCU.

		if (flash_secure_3 == 0x02){}			// Check Flash Secure byte 3.
			else {PCON = 2;}					// Something is wrong, STOP the MCU.

		if (flash_secure_4 == 0x55){PSCTL = BitOn(PSCTL, 0);}	// Redirect MOVX to flash. (Bit PSWE)
			else {PCON = 2;}					// Something is wrong, STOP the MCU.
	}
	else
	{
		SFRPAGE = CONFIG_PAGE;
		P2 = 0x00;
		PCON = 2;
	}											// Something is wrong, STOP the MCU.

	if(WritePointer < 0x2400)
	{
		SFRPAGE = CONFIG_PAGE;
		P2 = 0x00;
		PCON = 2;
	}
/*
	if(WritePointer > 0xFFEF)
	{
		SFRPAGE = CONFIG_PAGE;
		P2 = 0x00;
		PCON = 2;
	}
*/
	
    *WritePointer = c;		                   	// Write data to flash.

	FLSCL = BitOff(FLSCL, 0);					// Disable Flash Erase.
	PSCTL = BitOff(PSCTL, 0);					// Disable Flash Writes.
	PSCTL = BitOff(PSCTL, 1);					// Redirect MOVX to Ram.

	SFRPAGE = CONFIG_PAGE;
	while(FLBUSY);
	
	SFRPAGE = SFRPAGE_SAVE;     				// Restore SFR page
    EA = EA_state;                 				// restore interrupt state
}

unsigned int CRConFlash (bit bRun)
{
	data unsigned int k, CRC, FlashCount;
	data unsigned char j;
	data unsigned char code * FlashPtr;

	if (bRun == 0)
	{
		FlashPtr = BL_FLASH_START_ADDR;
		FlashCount = BL_FLASH_CRC_COUNT;
	}
	else
	{
		FlashPtr = FLASH_START_ADDR;
		FlashCount = FLASH_CRC_COUNT;
	}
	CRC = 0x0000;

	// Process each byte in the page into the running CRC
	for (k = 0; k < FlashCount; k++)
	{
		// Read the next Flash byte and XOR it with the upper 8 bits 
		// of the running CRC.
		CRC ^= (*FlashPtr++ << 8);

		// For each bit in the upper byte of the CRC, shift CRC 
		// left, test the MSB, and if set, XOR the CRC with the
		// polynomial coefficients (0x1021)
		for (j = 0; j < 8; j++)
		{
			CRC = CRC << 1;
			if (CRC & 0x8000 ) CRC ^= 0x1021;	
		}
	}  
	return CRC;
}

//-----------------------------------------------------------------------------
// WriteHex2Flash	: Program Intel Hex file.
//-----------------------------------------------------------------------------
unsigned char WriteHex2Flash(void)
{
	unsigned char dt;
	unsigned char HighByte;
	unsigned char LowByte;
	unsigned char IntelState;
	unsigned int Intel_State = 0x00;		

    char xdata* data pwrite;               	// pointer used for writing FLASH
    char code* data pread;                 	// pointer used for reading FLASH
    unsigned int  len;                     	// holds the HEX record length field
    char record_type;                      	// holds the HEX record type field
    unsigned int offset;                   	// holds the HEX record offset field
                                           	// this is the starting address of
                                           	// the code image contained in the 
                                           	// record

    char checksum;                         	// holds the HEX record checksum field
    char flash_checksum;                   	// holds the checksum calculated after
                                           	// the FLASH has been programmed

	int i;                                 	// temporary int
	char dat;
	bit odd = 0x00;	
    unsigned int lencount;                 	// holds the HEX record length field

	bit sec_run = 0x00;						// This is needed to avoid re-reading the RX Buffer	when starting.

	while (CmdState == CmdState_IntelHex)
	{
		dt = 0x00;
		FillHexBuffer();

			while(dt != 0x0D)
			{
				
				dt = GetHexBuffer();

				if (dt == ':') {IntelState = 0x01;}
					
				else if (IntelState == 0x01) {HighByte = dt; IntelState++;}
				else if (IntelState == 0x02)
				{
					LowByte = dt;
					len = hex2char(HighByte, LowByte);
					lencount = len;
					IntelState++;
				}
				else if (IntelState == 0x03) {HighByte = dt; IntelState++;}
				else if (IntelState == 0x04)
				{
					LowByte = dt;
					offset = hex2char(HighByte, LowByte);
				    offset <<= 8;
					if(offset > 0x0000 && offset < FLASH_START_ADDR)
					{
  			 		   str2COM0(&str_erradr, 0x00);
					   str2COM0(&str_lfcr, 0x00);
					   sprintf(CmdArray, "Address 0x%x", offset);
  			 		   str2COM0(&str_lfcr, 0x00);
					   str2COM0(&CmdArray, 0x00);
					   Transmit_COM0();
					   CmdState = CmdState_CMD;
					   return(0);
					} 
					IntelState++;
				}
				else if (IntelState == 0x05) {HighByte = dt; IntelState++;}
				else if (IntelState == 0x06)
				{
					LowByte = dt;
					offset |= hex2char(HighByte, LowByte);
					pwrite = (char xdata*) offset;      // initialize the write pointer
					IntelState++;
				}
				else if (IntelState == 0x07) {HighByte = dt; IntelState++;}
				else if (IntelState == 0x08)
				{
					LowByte = dt;
					record_type = hex2char(HighByte, LowByte);
				    if( record_type != 0 && record_type != 1 )
					{
						str2COM0(&str_errhex, 0x00);
						str2COM0(&str_lfcr, 0x00);
						Transmit_COM0();
						IntelState = 0x00;
					    CmdState = CmdState_CMD;
	  				    return(0);
					} else {IntelState++;}
					if (record_type == 1) {IntelState = 0x0A;}
				}
				else if (IntelState == 0x09)
				{
					if (lencount != 0x00 && odd == 0)
					{
						odd = 1;
						HighByte = dt;					
					}
					else if (lencount != 0x00 && odd == 1)
					{
						odd = 0;
						LowByte = dt;					
						dat = hex2char(HighByte, LowByte);
						lencount--;

   						code_erased = 0;                // clear the code_erased flag
					    
						write_flash_addr(dat, pwrite);	// write the record into flash
				        pwrite++;                       // increment FLASH write pointer
					}
					if (lencount == 0x00)
					{
						odd = 0;
						IntelState++;
					}
				}
				else if (IntelState == 0x0A) {HighByte = dt; IntelState++;}
				else if (IntelState == 0x0B)
				{
					LowByte = dt;
					checksum = hex2char(HighByte, LowByte);
					IntelState = 0x00;

   					pread =  (char code*) offset;       // initialize the read pointer

			        flash_checksum = 0;                 // set the flash_checksum to zero
      
				    // add the data field stored in FLASH to the checksum
				    for( i = 0; i < len; i++)
				    {
				       flash_checksum += *pread++;
				    } 
      
				    // add the remaining fields
				    flash_checksum += len;
				    flash_checksum += (char) (offset >> 8);
				    flash_checksum += (char) (offset & 0x00FF);
				    flash_checksum += record_type;
				    flash_checksum += checksum;
      
				    // verify the checksum (the flash_checksum should equal zero)
				    if(flash_checksum != 0)
					{
  			 			str2COM0(&str_errcrc, 0x00);
						str2COM0(&str_lfcr, 0x00);
						str2COM0(&str_lfcr, 0x00);
						Transmit_COM0();
					    CmdState = CmdState_CMD;
   				    	return(0);
					} 
					else if (flash_checksum == 0 && record_type == 0x01)
					{				    
						
						// Finish Flash programming and return to command mode...	
						flash_secure_1	=0x00;
						flash_secure_2	=0x00;
						flash_secure_3	=0x00;
						flash_secure_4	=0x00;

						CmdState = CmdState_CMD;
						IntelState = 0x00;
						
						SendCRC();
						return(1);
					}//End CRC

					//else  {StrToUser(&str_ok); StrToUser(&str_lfcr);}

					str2COM0(&str_ok, 0x00);
					str2COM0(&str_lfcr, 0x00);									
					Transmit_COM0();
 						
				}//End IntelState 0x0B
			}//while dt != 0x0D
	}//End CMD_State == 0x09
}

unsigned char FillHexBuffer()
{
	unsigned char dt;

	dt = 0x00;

	while (dt != 0x0D)
	{
		if (RXcount > 0)
		{
			dt = COM0GetByte();
			if (dt == ':')
			{
				HexCount = 0x00;
				HexIndex = 0x00;
			}
			
			HexBuffer[HexCount] = dt;
			HexCount++;
			if (HexCount > 63) return(0xff);
		}
	}
	
	return(HexCount);
}

unsigned char GetHexBuffer()
{
	unsigned char dt;
	
	dt = HexBuffer[HexIndex];
	if (HexIndex < 63) HexIndex++;
	
	return(dt);	
}

void SendCRC()
{
    unsigned int xdata CRCCalcFlash, CRCReadFlash;// Checksum for whole flash.	
	unsigned char xdata CRClo, CRChi;

	sprintf(CmdArray, "CRCc: ---- CRCr: ---- OK\r\n"); 

	CRCCalcFlash = CRConFlash(1);
	CRCReadFlash = read_flash_addr(FLASH_RUNCRC_ADDR);
	CRCReadFlash <<= 8;
	CRCReadFlash += read_flash_addr(FLASH_RUNCRC_ADDR + 1);
										    				
	CRClo = CRCCalcFlash;
	CRChi = CRCCalcFlash >> 8;			
	CmdArray[6] = Hex2Char[CRChi >> 4];
	CmdArray[7] = Hex2Char[CRChi & 0x0F];
	CmdArray[8] = Hex2Char[CRClo >> 4];
	CmdArray[9] = Hex2Char[CRClo & 0x0F];

	CRClo = CRCReadFlash;
	CRChi = CRCReadFlash >> 8;
	CmdArray[17] = Hex2Char[CRChi >> 4];
	CmdArray[18] = Hex2Char[CRChi & 0x0F];
	CmdArray[19] = Hex2Char[CRClo >> 4];
	CmdArray[20] = Hex2Char[CRClo & 0x0F];

	if (CRCCalcFlash != CRCReadFlash)
	{
		CmdArray[22] = '-'; // CRC is not OK.
		CmdArray[23] = '-';	// Firmware loader expects "RightStr('OK',2) := 'OK'" if CRC is OK.
	}

	str2COM0(&CmdArray, 0x00);
	Transmit_COM0();
}
