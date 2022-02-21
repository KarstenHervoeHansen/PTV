/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		FLASH.H	 																	*/
/*	Author:		AMD, Kim Engedahl, DEV		 											*/
/*	Org. date:	990521																		*/
/*	Rev. date:	000307, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions										*/
/*																									*/
/* Changes:																					 	*/
/*																									*/
/***************************************************************************/
#ifndef _FLASH_H
#define _FLASH_H

#define MAXSECTORS 		23		// Maximum number of sectors supported

#define XORaddress		(long) 0xE0000  // XA Controller Board
//#define XORaddress		(long) 0x80000	 // PT5201

// A structure for identifying a flash part.  There is one for each
// of the flash part definitions.  We need to keep track of the
// sector organization, the address register used, and the size
// of the sectors.
struct flashinfo {
	char *name;						// "Am29DL800T", etc.
	int nsect;						// # of sectors -- 19 in LV, 22 in DL
	int bank1start;				// First sector # in bank 1
	int bank2start;				// First sector # in bank 2, if DL part

	struct {
		unsigned long size; 		// # of bytes in this sector
		unsigned long base; 		// Offset from beginning of device
		int bank;					// 1 or 2 for DL; 1 for LV
	} sec[MAXSECTORS];			// Per-sector info
};

// Return codes from flash_status
#define STATUS_READY		0		// Ready for action
#define STATUS_BUSY		1		// Operation in progress
#define STATUS_ERSUSP	2		// Erase suspended
#define STATUS_TIMEOUT	3		// Operation timed out
#define STATUS_ERROR		4		// Unclassified but unhappy status

// Flash manufacturer ID
#define AMDPART			0x01
#define ATMELPART			0xFE
#define FUJITSUPART	 	0xFF

// A list of flash device ID's
#define ID_AM29DL800T	0x224A
#define ID_AM29DL800B	0x22CB
#define ID_MBM29F400T	0x2223
#define ID_MBM29F400B	0x22AB

// An index into the memdesc organization array. Is set by init_flash
// so the proper sector tables are used in the correct device
// Add others as needed, and as added to the device ID section
#define AM29DL800T		0
#define AM29DL800B		1
#define MBM29F400T	   2
#define MBM29F400B		3

// Prototypes
extern _rom char _FL_STATUSREADYTxt[];
extern _rom char _FL_STATUSBUSYTxt[];
extern _rom char _FL_STATUSERSUSPTxt[];
extern _rom char _FL_STATUSTIMEOUTTxt[];
extern _rom char _FL_STATUSERRORTxt[];			 

extern _rom char _FL_ONTxt[];
extern _rom char _FL_OFFTxt[];

extern _rom struct flashinfo* _near meminfo;

extern void flash_init( byte flash_type);
extern void flash_reset( void);
extern byte flash_status( word _far *address);
extern void flash_get_device_info( byte sector, byte *manufacturer, word *ID);
extern byte flash_get_status( byte sector);
extern byte flash_sector_protect_verify( byte sector);
extern void flash_sector_erase( byte sector);
extern byte flash_sector_erase_int( byte sector);
extern word flash_read_word( dword offset);
extern byte flash_write_word( dword offset, word data);
extern dword flash_get_sector_size( byte sector);
extern byte flash_get_numsectors( void);

#endif
