/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TABLES.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960908																		*/
/*	Rev. date:	971211, KEn DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains NO functions:													*/
/*	All the tables in this file contains the values send to all the units	*/
/*	and options. PLEASE handle with care!												*/
/* ALL tables should correspond with actual hardware AND enumerated types	*/
/* as defined in the file TABLES.H														*/
/*																									*/
/*	Changes:																						*/
/* 971203: Minor changes																	*/
/* 971028: Added GenlockInputTable & GenlockSystemTable							*/
/* 970127: New definition of AES_EBUSignalTable & AES_EBULevelTable			*/
/*			  to support extended levels													*/
/* 970112: Added new audio levels														*/
/* 970106: Added V24CRAMAddressTable													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>

#include "mas.h"

#include "52sync.h"
#include "tables.h"
#include "xaexprt.h"

code UC GenlockInputTable[12] = {
	0x40, 0,						// A
	0xC0, 0,						// B
	0xA0, 0,						// LOOP
	0x10, 0,						// SDI Genlock
	0x01, 0,						// Internal
	0x0F, 1						// Internal2
};

code UC GenlockSystemTable[20] = {
	0x01, 8,						// BurstLockPAL
	0x00, 10,					// BurstLockNTSC
	0x01, 7,						// SyncLock625
	0x00, 11,					// SyncLock525
	0x11, 6,						// SDI625Lock
	0x10, 9,						// SDI525Lock
	0x04, 5,						// MHzLock358
	0x05, 4,						// MHzLock443
	0x05, 3,						// MHzLock5
	0x0D, 2						// MHzLock10
};

code UC V24CRAMAddressTable[7] = {
	bbu0_addr,
	bbu1_addr,
	bbu2_addr,
	bbu3_addr,
	sync_addr,
	AnlSig_addr,
	SDISig_addr
};

code UI AES_EBULevelTable[8] = {
	0x0007,			// AES_EBUSilence
	0x0000,			// AES_EBULevel0dB
	0x0001,			// AES_EBULevel9dB
	0x0002,			// AES_EBULevel12dB
	0x0003,			// AES_EBULevel14dB
	0x0004,			// AES_EBULevel16dB
	0x0005,			// AES_EBULevel18dB
	0x0006			// AES_EBULevel20dB
};

code UI AES_EBUSignalTable[9] = {
	0,					// NOT USED
	0x0048,			// AES_EBUStereo800Hz
	0x0000,			// AES_EBUStereo1kHz
	0x0008,			// AES_EBUStereoEBU1kHz
	0x0010,			// AES_EBUStereoBBC1kHz
	0x0018,			// AES_EBUMonoEBU1kHz
	0x0020,			// AES_EBUMonoBBC1kHz
	0x0040,			// AES_EBUDual
	0x0078			// AES_EBU48kHz
};

code UI AES_EBUTimingTable[6] = {
	0x0000,			// AES_EBUPAL
	0x0100,			// AES_EBUNTSCPhase1
	0x0200,			// AES_EBUNTSCPhase2
	0x0300,			// AES_EBUNTSCPhase3
	0x0400,			// AES_EBUNTSCPhase4
	0x0500			// AES_EBUNTSCPhase5
};

