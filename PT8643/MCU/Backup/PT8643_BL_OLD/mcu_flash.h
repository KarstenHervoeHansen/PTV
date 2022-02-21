//-----------------------------------------------------------------------------
//	Filename	: mcu_flash.c
//	Date		: 2010-06-01
//	Name		: BS
//	Processor	: C8051F132
//	Copyright	: DK-Technologies A/S
//-----------------------------------------------------------------------------

unsigned char read_flash_addr(unsigned int readaddress);
void write_flash_addr(unsigned char c, char xdata* data flashAddress);
void erase_flash_page(void);
unsigned char hex2char(char byteH, byteL);
unsigned char WriteHex2Flash(void);
unsigned char FillHexBuffer();
unsigned char GetHexBuffer();
unsigned int CRConFlash (bit bRun);
void SendCRC();