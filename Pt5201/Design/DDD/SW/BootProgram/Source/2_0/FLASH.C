/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		FLASH.C	 																	*/
/*	Author:		AMD, Kim Engedahl, DEV		 											*/
/*	Org. date:	990521																		*/
/*	Rev. date:	000711, KEn, DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*		void flash_init( byte  flash_type);												*/
/*		void flash_reset( void);															*/
/*		byte flash_status( word _far *address);										*/
/*		void flash_get_device_info( byte sector, byte *manufacturer,\			*/
/*																					 word *ID);	*/
/*		byte flash_get_status( byte sector);											*/
/*		byte flash_sector_protect_verify( byte sector);								*/
/*		byte flash_sector_erase( byte sector);											*/
/*		byte flash_sector_erase_int( byte sector);									*/
/*		word flash_read_word( dword offset);											*/
/*		byte flash_write_word( dword offset, word data);							*/
/*		word flash_get_sector_size( byte sector);										*/
/*		byte flash_get_numsectors( void);												*/
/*																									*/
/* Changes:																					 	*/
/* 000710: Support for program retry													*/
/* 000228: Added support for Fujitsu MBM29F400 and alternative address		*/
/* 		  for programming.																*/
/***************************************************************************/
#include "system.h"
#include "string.h"
#include "flash.h"

_rom char _FL_STATUSREADYTxt[] 	 	= "READY\r\n";
_rom char _FL_STATUSBUSYTxt[]	 	 	= "BUSY\r\n";
_rom char _FL_STATUSERSUSPTxt[]	 	= "ERASE SUSPENDED\r\n";
_rom char _FL_STATUSTIMEOUTTxt[]	 	= "TIME-OUT\r\n";
_rom char _FL_STATUSRETRYTxt[]	 	= "RETRY\r\n";
_rom char _FL_STATUSRETRYERRTxt[]	= "RETRY ERROR\r\n";
_rom char _FL_STATUSERRORTxt[]	 	= "ERROR\r\n";
_rom char _FL_WRITEERRORTxt[]	 		= "WRITE ERROR\r\n";

_rom char _FL_ONTxt[]				 	= "ON\r\n";
_rom char _FL_OFFTxt[]				 	= "OFF\r\n";

_rom char*_FL_STATUS[] = {	_FL_STATUSREADYTxt,
									_FL_STATUSBUSYTxt,
									_FL_STATUSERSUSPTxt,
									_FL_STATUSTIMEOUTTxt,
									_FL_STATUSRETRYTxt,
									_FL_STATUSRETRYERRTxt,
									_FL_STATUSERRORTxt
								};
					 
_rom struct flashinfo memdesc[8] = {
	{ "29DL800T", 22, 14, 0, {
		{ 65536,	0x00000, 2},
		{ 65536,	0x10000, 2},
		{ 65536,	0x20000, 2},
		{ 65536,	0x30000, 2},
		{ 65536,	0x40000, 2},
		{ 65536,	0x50000, 2},
		{ 65536,	0x60000, 2},
		{ 65536,	0x70000, 2},
		{ 65536,	0x80000, 2},
		{ 65536,	0x90000, 2},
		{ 65536,	0xA0000, 2},
		{ 65536,	0xB0000, 2},
		{ 65536,	0xC0000, 2},
		{ 65536,	0xD0000, 2},
		{ 16384,	0xE0000, 1},
		{ 32768,	0xE4000, 1},
		{ 8192,	0xEC000, 1},
		{ 8192,	0xEE000, 1},
		{ 8192,	0xF0000, 1},
		{ 8192,	0xF2000, 1},
		{ 32768,	0xF4000, 1},
		{ 16384,	0xFC000, 1},
		{ 0,		0x100000, 1}
		}
	},
	{ "29DL800B", 22, 0, 8, {
		{ 16384,	0x00000, 1},
		{ 32768,	0x04000, 1},
		{ 8192,	0x0C000, 1},
		{ 8192,	0x0E000, 1},
		{ 8192,	0x10000, 1},
		{ 8192,	0x12000, 1},
		{ 32768,	0x14000, 1},
		{ 16384,	0x1C000, 1},
		{ 65536,	0x20000, 2},
		{ 65536,	0x30000, 2},
		{ 65536,	0x40000, 2},
		{ 65536,	0x50000, 2},
		{ 65536,	0x60000, 2},
		{ 65536,	0x70000, 2},
		{ 65536,	0x80000, 2},
		{ 65536,	0x90000, 2},
		{ 65536,	0xA0000, 2},
		{ 65536,	0xB0000, 2},
		{ 65536,	0xC0000, 2},
		{ 65536,	0xD0000, 2},
		{ 65536,	0xE0000, 2},
		{ 65536,	0xF0000, 2},
		{ 0,		0x100000, 2}
		}
	},
	{ "29LV800T", 19, 0, 0, {
		{ 65536,	0x00000,	1},
		{ 65536, 0x10000, 1},
		{ 65536, 0x20000, 1},
		{ 65536, 0x30000, 1},
		{ 65536, 0x40000, 1},
		{ 65536, 0x50000, 1},
		{ 65536, 0x60000, 1},
		{ 65536, 0x70000, 1},
		{ 65536, 0x80000, 1},
		{ 65536, 0x90000, 1},
		{ 65536, 0xA0000, 1},
		{ 65536, 0xB0000, 1},
		{ 65536, 0xC0000, 1},
		{ 65536, 0xD0000, 1},
		{ 65536, 0xE0000, 1},
		{ 32768, 0xF0000, 1},
		{ 8192,  0xF8000, 1},
		{ 8192,  0xFA000, 1},
		{ 16384, 0xFC000, 1},
		{ 0,		0x100000, 1}
		}
	},
	{ "29LV800B", 19, 0, 0, {
		{ 16384, 0x00000, 1},
		{ 8192,  0x04000, 1},
		{ 8192,  0x06000, 1},
		{ 32768, 0x08000, 1},
		{ 65536, 0x10000, 1},
		{ 65536, 0x20000, 1},
		{ 65536, 0x30000, 1},
		{ 65536, 0x40000, 1},
		{ 65536, 0x50000, 1},
		{ 65536, 0x60000, 1},
		{ 65536, 0x70000, 1},
		{ 65536, 0x90000, 1},
		{ 65536, 0xA0000, 1},
		{ 65536, 0xB0000, 1},
		{ 65536, 0xC0000, 1},
		{ 65536, 0xD0000, 1},
		{ 65536, 0xE0000, 1},
		{ 65536, 0xF0000, 1},
		{ 0,		0x100000, 1}
		}
	},
	{ "29FLV400T", 11, 0, 0 ,{
		{ 65536,	0x00000, 1},
		{ 65536,	0x10000, 1},
		{ 65536,	0x20000, 1},
		{ 65536,	0x30000, 1},
		{ 65536,	0x40000, 1},
		{ 65536,	0x50000, 1},
		{ 65536,	0x60000, 1},
		{ 32768,	0x70000, 1},
		{ 8192,	0x78000, 1},
		{ 8192,	0x7A000, 1},
		{ 16384,	0x7C000, 1},
		{ 0,		0x80000, 1}
		}
	},
	{ "29FLV400B", 11, 0, 0 ,{
		{ 16384,	0x00000, 1},
		{ 8192,	0x04000, 1},
		{ 8192,	0x06000, 1},
		{ 32768,	0x08000, 1},
		{ 65536,	0x10000, 1},
		{ 65536,	0x20000, 1},
		{ 65536,	0x30000, 1},
		{ 65536,	0x40000, 1},
		{ 65536,	0x50000, 1},
		{ 65536,	0x60000, 1},
		{ 65536,	0x70000, 1},
		{ 0,		0x80000, 1}
		}
	},
	{ "29F4096T", 4, 0, 0 ,{
		{ 491520, 0x00000, 1},
		{ 8192,	 0x78000, 1},
		{ 8192,	 0x7A000, 1},
		{ 16384,	 0x7C000, 1},
		{ 0,		 0x80000, 1}
		}
	},
	{ "29F4096", 4, 0, 0 ,{
		{ 16384,	 0x00000, 1},
		{ 8192,	 0x04000, 1},
		{ 8192,	 0x06000, 1},
		{ 491520, 0x08000, 1},
		{ 0,		 0x80000, 1}
		}
	},
};

_rom struct flashinfo* _near meminfo;	// A pointer

/***************************************************************************/
/*	flash_init																 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000228, KEn, DEV	     													*/
/*																									*/
/*	Function:	Initialise and reset the flash device for reading.			  	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void flash_init( byte flash_type) {

	word _far *address;

	meminfo = &memdesc[flash_type];

	#if BootProgram
		address = (word _far *) ( XORaddress);
	#else
		address = (word _far *) ( 0x00000);
	#endif

	address[0] = 0xF0;
}

/***************************************************************************/
/*	flash_reset																 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000228, KEn, DEV	     													*/
/*																									*/
/*	Function:	Reset the flash device for reading.									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void flash_reset( void) {

	word _far *address;

	#if BootProgram
		address = (word _far *) ( XORaddress);
	#else
		address = (word _far *) ( 0x00000);
	#endif

	address[0] = 0xF0;
}

/***************************************************************************/
/*	flash_status															 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		990902, KEn, DEV	     													*/
/*																									*/
/*	Function:	Return the status based on DQ6, DQ5 and DQ2 polling			*/
/*					algorithms.																	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte flash_status( word _far *address) {

	word val, toggle;
	byte retry = 1;

	do {
		val = *address;					// Read data
		toggle = val ^ *address;		// Read it again to examine what toggled

		if ( toggle == 0) 				// No toggles, nothing's happening
			return( STATUS_READY);
		else {
			if ( toggle == 0x04) { 		// DQ2: Erase-suspend  fo
				if ( retry == 0)
					return( STATUS_ERSUSP);
			}
			else {
				if ( toggle & 0x40) {	// DQ6:
					if ( val & 0x20)		// DQ5: Timeout
						return( STATUS_TIMEOUT);

				  return( STATUS_BUSY);
				}
			}
		}
	} while( retry--);					// May have been write completion

	return( STATUS_ERROR);
}

/***************************************************************************/
/*	flash_get_device_info												 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000712, KEn, DEV	     													*/
/*																									*/
/*	Function:	Return the manufacturer code and device ID of the flash		*/
/*	 				device.																		*/
/*	Remarks:		Function automatically resets to read mode.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void flash_get_device_info( byte *manufacturer, word *ID)
{
	word _far *address;

	#if BootProgram
		address = (word _far *) ( XORaddress);
	#else
		address = (word _far *) ( 0);
	#endif

	address[0x0555] = 0xAA;				// Unlock 1
	address[0x02AA] = 0x55;				// Unlock 2
	address[0x0555] = 0x90;

	*manufacturer = (byte) ( address[0] & 0x00FF);
	*ID = address[1];

	address[0] = 0xF0;					// Reset flash device to read mode
}

/***************************************************************************/
/*	flash_get_status	 													 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000228, KEn, DEV	     													*/
/*																									*/
/*	Function:	Return the status of the flash device								*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte flash_get_status( byte sector) {

	#if BootProgram
		return( flash_status((word _far *) ( meminfo->sec[sector].base ^ XORaddress)));
	#else
		return( flash_status((word _far *) meminfo->sec[sector].base));
	#endif
}

/***************************************************************************/
/*	flash_sector_protect_verify										 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000712, KEn, DEV	     													*/
/*																									*/
/*	Function:	Checks the status of the sector protect CAM to check if the */
/*					particular sector is protected.										*/
/*	Remarks:		--																				*/
/*	Returns:		0 if the sector is unprotected otherwise 1						*/
/*	Updates:		--																				*/
/***************************************************************************/
byte flash_sector_protect_verify( byte sector) {

	word _far *address;
	byte answer;

	#if BootProgram
		address = (word _far *) ( meminfo->sec[sector].base ^ XORaddress);
	#else
		address = (word _far *) meminfo->sec[sector].base;
	#endif

	address[0x0555] = 0xAA;				// Unlock 1
	address[0x02AA] = 0x55;				// Unlock 2
	address[0x0555] = 0x90;

	answer = (byte) ( address[0] & 0x0001);	// Only need DQ0 to check

	address[0] = 0xF0;  			 					// Reset flash device to read mode

	return( answer);
}

/***************************************************************************/
/*	flash_sector_erase_int												 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000712, KEn, DEV	     													*/
/*																									*/
/*	Function:	Commence erasing a single sector and wait until erased.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte flash_sector_erase_int( byte sector) {

	word _far *address;
	byte stat, retry = 2;

	#if BootProgram
		address = (word _far *) ( meminfo->sec[sector].base ^ XORaddress);
	#else
		address = (word _far *) meminfo->sec[sector].base;
	#endif

	do
	{
		address[0x555] = 0xAA;				// Unlock 1
		address[0x2AA] = 0x55;				// Unlock 2
		address[0x555] = 0x80;
		address[0x555] = 0xAA;
		address[0x2AA] = 0x55;
										
		address[0] = 0x30;		
										
	} while ( retry-- && ( flash_status( address) == STATUS_READY));
		
	if ( retry >= 0)
	{
		while (( stat = flash_status( address)) == STATUS_BUSY)
			;
	}
	else
		stat = STATUS_RETRYERR;

	address[0] = 0xF0;						// Reset flash device to read mode

	return( stat);
}

/***************************************************************************/
/*	flash_read_word														 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000228, KEn, DEV	     													*/
/*																									*/
/*	Function:	Read single word of data from the specified offset.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
word flash_read_word( dword offset) {

	word _far *address;

	#if BootProgram
		address = (word _far *) ( offset ^ XORaddress);
	#else
		address = (word _far *) offset;
	#endif

	return( address[0]);
}

/***************************************************************************/
/*	flash_write_word														 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		000712, KEn, DEV	     													*/
/*																									*/
/*	Function:	Program a single word of data at the specified offset.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte flash_write_word( dword offset, word data) {

	word _far *address, *dst;
	byte stat, retry = 2;

	#if BootProgram
		dst = (word _far *) ( offset ^ XORaddress);
		address = (word _far *) (( offset ^ XORaddress) & 0xF0000);
	#else
		dst = (word _far *) offset;
		address = (word _far *) ( offset & 0xF0000);
	#endif

	do
	{
		address[0x555] = 0xAA;			// Unlock 1
		address[0x2AA] = 0x55;			// Unlock 2
		address[0x555] = 0xA0;

		*dst = data;

	} while ( retry-- && ( flash_status( address) == STATUS_READY));
		
	if ( retry >= 0)
	{
		while (( stat = flash_status( address)) == STATUS_BUSY)
			;
	}
	else
		stat = STATUS_RETRYERR;

	address[0] = 0xF0;				// Reset flash device to read mode

	return( stat);
}

/***************************************************************************/
/*	flash_get_sector_size												 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		990910, KEn, DEV	     													*/
/*																									*/
/*	Function:	Return the size of a sector in the flash EPROM device.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
dword flash_get_sector_size( byte sector) {

	return( meminfo->sec[sector].size);
}

/***************************************************************************/
/*	flash_get_numsectors													 		 FLASH.C	*/
/*																									*/
/* Author:		AMD																			*/
/* Revised:		990902, KEn, DEV	     													*/
/*																									*/
/*	Function:	Return the number of sectors in the flash EPROM device.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
byte flash_get_numsectors( void) {

	return( meminfo->nsect);
}
