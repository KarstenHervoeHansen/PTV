/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/03 SDI Digital test signal generator						*/
/*	Module:		COMM.H																		*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960703																		*/
/*	Rev. date:	990308, KEn	DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	Changes:																						*/
/* 990308: Renamed some variables														*/
/* 980929: Added GeneratorVers &GeneratorInfo										*/
/***************************************************************************/

#define EEPROM_IIC_Adr	0xA0

#define PT_HWType			0x00		// Type
#define PT_HWVersion		0x79		// Generator 900 version, eg. 901, 902 etc.

#define IIC_G_Address	0x01		// Long var + checksum
#define IIC_M_Address	0x06		// Long var + checksum

#define IIC_UserTxt		0x0B		// 10 blocks of 11 byte ( 10 data + checksum)

extern near UC IICErrorCode;

extern UC IIC_Read( UC Address);
extern bit IIC_Write( UC Address, UC Data);
extern bit IIC_Write2( UC Address,UC Data, UC Data2);
extern bit IIC_WrStr( UC Address, UC subadr, char* s);
extern bit IIC_RdStr( UC Address, UC subadr, char *s);
extern bit IIC_WrOffset( UC Address, UC subadr, UL a);
extern UL IIC_RdOffset( UC Address, UC subadr);
