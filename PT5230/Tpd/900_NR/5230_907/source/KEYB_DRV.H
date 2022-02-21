/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		KEYB_DRV.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980203																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

#define LEFTButton			0xDE		// S1
#define UPButton				0xDD		// S2
#define DOWNButton			0xDB		// S3
#define RIGHTButton			0xD7		// S4
#define EXECButton			0xCF		// S5

#define CBARButton			0xBD		// S11
#define MBURSTButton			0xBB		// S12
#define WINDOWButton			0xB7		// S13
#define PRESETButton			0xAF		// S14

#define SPECIALButton		0xFD		// S15
#define LINEARITYButton		0xFB		// S16
#define PATTERNButton		0xF7		// S17
#define OUTPUTButton			0xEF		// S18

#define NOButton				0xFF
#define NOCommand				0xFF

#define MASTERButton			LEFTButton & RIGHTButton & EXECButton
#define FACTORYButton		LEFTButton & RIGHTButton

#define NormalRepeatFreq	15
#define FastRepeatFreq		4

extern UC Button;
extern UC OldButton;
extern UC ButtonCommand;
extern UC OldButtonCommand;

extern UC RepeatCount;
extern UC RepeatDelay;
extern UC Repeat1Delay;

extern bit ButtonEnable;
extern bit HorizontalRepeat;
extern bit VerticalRepeat;
extern bit RepeatOn;
extern bit CombiOn;

extern UC UpdateLEDs();
extern UC UpdateGrpLEDs( UC group);
extern bit ButtonReady();
