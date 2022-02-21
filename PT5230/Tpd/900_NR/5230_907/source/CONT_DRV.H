/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		CONT_DRV.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

#define StdTemperature	25				// Standard Temperature [ca. 25 Deg
#define DefTemperature	25				//	default	    - " -

#define MaxContrast		20				// Max Contrast-level
#define StdContrast		17				// HW-Reset Contrast-level
#define DefContrast		17				// Default	-
#define MinContrast		0				// Min Contrast-level

#define DACMin				0				// Min Contrast DAC-data
#define DACMax				(20-3)*3		// Max 	   - " -

#define DACEnable			0x40			// PCF8591 Control-status = DAC ok

extern UC SendContrast( UC Contrast);
extern UC InitContrast();
extern UC CalculateTemp( UC val);
