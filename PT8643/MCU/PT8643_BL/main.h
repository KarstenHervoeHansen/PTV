//-----------------------------------------------------------------------------
//	Filename	: main.h
//	Date		: 2010-05-31
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------
#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

extern void Init_Device(void);
extern void Oscillator_Init(void);
extern void ResetCPFlash(void);

extern unsigned char CmdIndex;					// Defined in main.c
extern unsigned char idata CmdArray[32];		// Defined in main.c

extern unsigned char CmdState;					// Defined in main.c
extern unsigned char idata flash_secure_1;		// Defined in main.c;
extern unsigned char idata flash_secure_2;		// Defined in main.c;
extern unsigned char idata flash_secure_3;		// Defined in main.c;
extern unsigned char idata flash_secure_4;		// Defined in main.c;
extern unsigned int xdata BL_CRC;				// Flash CRC.
extern unsigned int xdata RUN_CRC;				// Flash CRC.


#define T2_OVERFLOW_RATE 100            // Timer 2 Overflow Rate in Hz


#define DISABLED    		0xFF		// LED State Definitions
#define SOLID_OFF  			0x00				
#define SOLID_ON   			0x01
#define BLINK_SLOW  		0x02
#define BLINK_FAST  		0x03
#define BLINK_TWICE 		0x04

										// Command State Definitions
#define CmdState_Normal		0x00		// Normal transparent mode.
#define CmdState_CMD		0x01		// Interpret commands.
#define CmdState_IntelHex	0x02		// Program MCU Flash.

#define BL_FLASH_START_ADDR 0x0000		// Starting address of erasable area
#define FLASH_START_ADDR 	0x2400		// Starting address of erasable area
										// Note: Startup.a51 must also be modified.

#define FLASH_PAGE_SIZE  	512			// Number of bytes in a Flash page

#define FLASH_BLInfo_ADDR 	0x23FD		// Bootloader version.
										// 0x23FD = Major.
										// 0x23FE = Minor.
										// 0x23FF = SoftwareID.

#define FLASH_CRCOWR_ADDR 	0xFFFA		// CRC OVERRIDE = A2
#define FLASH_RUNCRC_ADDR 	0xFFFB		// Run Code CRC Value 0xF9FB + 0xF9FC	
#define FLASH_RUNInfo_ADDR	0xFFFD		// Run Code version.
										// 0xF9FD = Major.
										// 0xF9FE = Minor.
										// 0xF9FF = SoftwareID.

#define FLASH_BLCRC_ADDR 	0x23FB		// Run Code CRC Value 0xF9FB + 0xF9FC	

#define BL_FLASH_LAST_ADDR	0x23FF		// LAST USABLE FLASH ADDRESS!!!
#define FLASH_LAST_ADDR		0xFFFF		// LAST USABLE FLASH ADDRESS!!!

#define FLASH_CRC_COUNT	(FLASH_LAST_ADDR - FLASH_START_ADDR  - 10) // Number of bytes in FLASH CRC check. 
										// The last 10 bytes is not included in the check.
#define BL_FLASH_CRC_COUNT	(BL_FLASH_LAST_ADDR - BL_FLASH_START_ADDR  - 10) // Number of bytes in FLASH CRC check. 

unsigned char BitOn(unsigned char TheChar, unsigned char TheBit);
unsigned char BitOff(unsigned char TheChar, unsigned char TheBit);