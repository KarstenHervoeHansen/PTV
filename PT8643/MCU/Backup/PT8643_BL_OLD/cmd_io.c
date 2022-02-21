//-----------------------------------------------------------------------------
//	Filename	: cmd_io.c
//	Date		: 2010-06-01
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "cmd_io.h"
#include "textstr.h"
#include "UART_0.h"
#include "main.h"
#include "mcu_flash.h"
#include "i2c_slave.h"
#include <string.h>
#include <stdio.h>
#include <c8051F120.h>                      	// Device-specific SFR Definitions

//-----------------------------------------------------------------------------
// Constant
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Externs
//-----------------------------------------------------------------------------
extern unsigned char xdata LED1_STATE;
extern unsigned int boot_timeout;
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMD_IO Subroutines
//-----------------------------------------------------------------------------

void decode_cmd(char dt)
{
	unsigned char i;
	unsigned int ver1, ver2;
	
	if (dt == 0x40) {CmdIndex = 0;} // @ will reset the array. (Start of CMD)
	
	CmdArray[CmdIndex] = dt;
	if (CmdIndex > 32) {CmdIndex = 0;}
	CmdIndex++;

	if (dt == 0x0D) // CR = End of CMD.
	{			
		if (CmdIndex > 0) {CmdIndex--;}
		CmdArray[CmdIndex] = 0x00; // Make sure the CMD is ended with a 0x00 (CR replaced with 0x00)
				
		if (strcmp(CmdArray,cmd_ver) == 0) //Show version number.
		{
			str2COM0(&str_ver, 0x00);					

			ver1 = read_flash_addr(FLASH_BLInfo_ADDR);
			ver2 = read_flash_addr(FLASH_BLInfo_ADDR + 1);

			sprintf(CmdArray, "%d.%d", ver1, ver2); 

			str2COM0(&CmdArray, 0x00);
			str2COM0(&str_lfcr, 0x00);
			Transmit_COM0();							
		}

		else if (strcmp(CmdArray, cmd_getstat) == 0)//Return buffer until 0x00...
		{
			CmdArray[0x00] = 0x20;		  		// Data count.
			CmdArray[0x01] = 0x00;				// 0x00 = BootLoader Mode. 0x01 = RUN Mode
			CmdArray[0x02] = read_flash_addr(FLASH_BLInfo_ADDR);		// Bootloader Version Major
			CmdArray[0x03] = read_flash_addr(FLASH_BLInfo_ADDR + 1);	// Bootloader Version Minor
			CmdArray[0x04] = read_flash_addr(FLASH_BLInfo_ADDR + 2);	// Program ID
			CmdArray[0x05] = read_flash_addr(FLASH_RUNInfo_ADDR);		// Run code Version Major
			CmdArray[0x06] = read_flash_addr(FLASH_RUNInfo_ADDR + 1);	// Run code Version Minor
			CmdArray[0x07] = read_flash_addr(FLASH_RUNInfo_ADDR + 2);	// Run code Program ID;
			CmdArray[0x08] = MAC_addr[0x00];	// MAC Address.
			CmdArray[0x09] = MAC_addr[0x01];	// MAC Address.
			CmdArray[0x0A] = MAC_addr[0x02];	// MAC Address.
			CmdArray[0x0B] = MAC_addr[0x03];	// MAC Address.
			CmdArray[0x0C] = MAC_addr[0x04];	// MAC Address.
			CmdArray[0x0D] = MAC_addr[0x05];	// MAC Address.
			CmdArray[0x0E] = read_flash_addr(FLASH_BLCRC_ADDR);			// BL CRC Read.
			CmdArray[0x0F] = read_flash_addr(FLASH_BLCRC_ADDR + 1);		// BL CRC Read.
			CmdArray[0x10] = HIGHBYTE(BL_CRC);	// BL CRC Calc.
			CmdArray[0x11] = LOWBYTE(BL_CRC);	// BL CRC Calc.
			CmdArray[0x12] = read_flash_addr(FLASH_RUNCRC_ADDR);		// RUN CRC Read.
			CmdArray[0x13] = read_flash_addr(FLASH_RUNCRC_ADDR + 1);	// RUN CRC Read.
			CmdArray[0x14] = HIGHBYTE(RUN_CRC);	// RUN CRC Calc.
			CmdArray[0x15] = LOWBYTE(RUN_CRC);	// RUN CRC Calc.
			CmdArray[0x16] = 0x00;				// Unused.
			CmdArray[0x17] = 0x00;				// Unused.
			CmdArray[0x18] = 0x00;				// Unused.
			CmdArray[0x19] = 0x00;				// Unused.
			CmdArray[0x1A] = 0x00;				// Unused.
			CmdArray[0x1B] = 0x00;				// Unused.
			CmdArray[0x1C] = 0x00;				// Unused.
			CmdArray[0x1D] = 0x00;				// Unused.
			CmdArray[0x1E] = 0x00;				// Unused.
	    	CmdArray[0x1F] = 0x00;

			for(i=0; i<0x1F; i++) //100
			{
				CmdArray[0x1F] += CmdArray[i];// CRC.
    		}
			
			CmdArray[0x1F] ^= 0x55;			// CRC.
			
			str2COM0(&CmdArray, 0x20);		// String length. 0x00 = Auto;							
			str2COM0(&str_lfcr, 0x00);		// String length. 0x00 = Auto;							
			Transmit_COM0();
		}

		else if (strcmp(CmdArray,cmd_erase) == 0)//Erase FLASH.
		{
			erase_flash_page();

			str2COM0(&str_ok, 0x00);
			str2COM0(&str_lfcr, 0x00);
			Transmit_COM0();
		}

		else if (strcmp(CmdArray,cmd_armflash) == 0)//Erase FLASH.
		{
			while(RXcount < 8);
			flash_secure_1 = hex2char(COM0GetByte(), COM0GetByte());
			flash_secure_2 = hex2char(COM0GetByte(), COM0GetByte());
			flash_secure_3 = hex2char(COM0GetByte(), COM0GetByte());
			flash_secure_4 = hex2char(COM0GetByte(), COM0GetByte());

			str2COM0(&str_ok, 0x00);
			str2COM0(&str_lfcr, 0x00);
			Transmit_COM0();
		}

		else if (strcmp(CmdArray,cmd_prgflash) == 0)// Program FLASH Memory.
		{		
			str2COM0(&str_ok, 0x00);
			str2COM0(&str_lfcr, 0x00);									
			Transmit_COM0();
			CmdState = CmdState_IntelHex;
//			CmdArray[31] = FillHexBuffer();
			WriteHex2Flash();

			boot_timeout = 2500;
		}

		else if (strcmp(CmdArray,cmd_restart) == 0)// Execute code from 0x2400
		{		
			str2COM0(&str_ok, 0x00);
			str2COM0(&str_lfcr, 0x00);
			Transmit_COM0();
		    RSTSRC = 0xFF; //Reset Device
		}

		else if (strcmp(CmdArray,cmd_esc) == 0)//End Command mode.
		{
			str2COM0(&str_ok, 0x00);
			str2COM0(&str_lfcr, 0x00);							
			Transmit_COM0();
			LED1_STATE	= BLINK_SLOW;
			CmdState = CmdState_Normal;
		}

		else if (strcmp(CmdArray,cmd_enable) == 0)//Return OK.
		{		
			str2COM0(&str_ok, 0x00);
			str2COM0(&str_lfcr, 0x00);
			Transmit_COM0();
		}

		else //If no command then return Not OK 
		{
			str2COM0(&str_nok, 0x00);
			str2COM0(&str_lfcr, 0x00);							
			Transmit_COM0();
		}
	CmdArray[0] = 0x00; //Prevent a CR to repeat the command via USB.
	}// CR = End of CMD.
}
