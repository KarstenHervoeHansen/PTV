/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		COMM.H																		*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960703																		*/
/*	Rev. date:	990319, KEn, DEV															*/
/*	Status:		Version 2.2																	*/
/*																									*/
/*	Changes:																						*/
/* 990319: Removed reference to PT_HWUserPattern & PT_HWInformation			*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/***************************************************************************/

#define EEPROM_IIC_Adr		0xA0

#define PT_HWType				0x00		// Type

#define IIC_G_Address		0x01		// Long var + checksum
#define IIC_M_Address		0x06		// Long var + checksum

#define IIC_UserTxt			0x0B		// 10 blocks of 11 byte ( 10 data + checksum)

#define PT_HWVersion			0x79		// Generator 900 version, eg. 901, 902 etc.

extern near UC IICErrorCode;

extern UC IIC_Read( UC Address);
extern bit IIC_Write( UC Address, UC Data);
extern bit IIC_Write2( UC Address,UC Data, UC Data2);
extern bit IIC_WrStr( UC Address, UC subadr, char* s);
extern bit IIC_RdStr( UC Address, UC subadr, char *s);
extern bit IIC_WrOffset( UC Address, UC subadr, UL a);
extern UL IIC_RdOffset( UC Address, UC subadr);

