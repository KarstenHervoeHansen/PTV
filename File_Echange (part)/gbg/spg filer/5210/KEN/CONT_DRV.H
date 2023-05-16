/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		CONT_DRV.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	971112, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

#define Std_Temperature	25				// Standard Temperature [ca. 25 Deg
#define Def_Temperature	25				//	default	    - " -

#define Max_Contrast		20				// Max Contrast-level
#define Std_Contrast		17				// HW-Reset Contrast-level
#define Def_Contrast		17				// Default	-
#define Min_Contrast		0				// Min Contrast-level

#define DAC_Min			0				// Min Contrast DAC-data
#define DAC_Max			(20-3)*3		// Max 	   - " -		

#define DAC_Enable		0x40			// PCF8591 Control-status = DAC ok

extern UC Send_Contrast( UC Contrast);
extern UC Init_Contrast();
extern UC Calculate_Temp( UC val);
