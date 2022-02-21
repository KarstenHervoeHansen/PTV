//#define DEBUG_MCU

#define FLASH_BLInfo_ADDR 	0x23FD		// Bootloader version.
										// 0x23FD = Major.
										// 0x23FE = Minor.
										// 0x23FF = SoftwareID.

#define FLASH_RUNInfo_ADDR 	0xFFFD		// Bootloader version.
										// 0xFFFD = Major.
										// 0xFFFE = Minor.
										// 0xFFFF = SoftwareID.

extern unsigned char SNTP_Enable;

extern unsigned char read_flash_addr(unsigned int readaddress);
extern xdata unsigned char Netfinder_Name[32];
extern int iDHCPRenew;
extern unsigned char Software_WD;

#ifdef DEBUG_MCU		
	extern unsigned char DebugString[128];
#endif