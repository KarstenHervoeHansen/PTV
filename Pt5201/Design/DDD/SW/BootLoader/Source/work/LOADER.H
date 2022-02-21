/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		LOADER.H			  															*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/* Changes:																						*/
/***************************************************************************/

#ifndef _LOADER_H
#define _LOADER_H

#define _LO_NoError			0
#define _LO_CharNotValid	1
#define _LO_RecLengthError	2
#define _LO_RecTypeError	3
#define _LO_ChecksumError	4

#define _LO_RecStart			5

extern _rom char _LO_EnteringLoaderTxt[];
extern _rom char _LO_LeavingLoaderTxt[];
extern _rom char _LO_CharNotValidTxt[];
extern _rom char _LO_RecLengthErrorTxt[];
extern _rom char _LO_RecTypeErrorTxt[];
extern _rom char _LO_ChecksumErrorTxt[];

extern byte IntelLoader( char inchr);

#endif