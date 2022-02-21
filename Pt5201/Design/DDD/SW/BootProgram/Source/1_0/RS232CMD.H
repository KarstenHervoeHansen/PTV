/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		RS232CMD.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990917, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains no functions													*/
/*																									*/
/*	Changes:																						*/
/*																									*/
/***************************************************************************/
#ifndef _RS232CMD_H
#define _RS232CMD_H

typedef struct CMD_STRUCT {
	_rom char* LongName;
	_rom char* ShortName;
	_rom struct CMD_STRUCT* SameLevel;

	void ( *Function)( void);
};

typedef struct PAR_STRUCT {
	_rom char* LongName;
	_rom char* ShortName;

	_rom struct PAR_STRUCT* SameLevel;
};

extern _rom struct CMD_STRUCT Subsystem[14];

extern void SystemIDN( void);
extern void SystemError( void);
extern void HWInfo( void);
extern void HWTest( void);
extern void FlashReset( void);
extern void TKCalibrate( void);
extern void FlashInfo( void);
extern void FlashStatus( void);
extern void FlashChecksum( void);
extern void FlashSectorSize( void);
extern void FlashSectorProtectVerify( void);
extern void FlashSectorErase( void);
extern void FlashBankErase( void);
extern void FlashRead( void);

extern byte FindParameter( char* par, _rom struct PAR_STRUCT* TmpPtr);

#endif