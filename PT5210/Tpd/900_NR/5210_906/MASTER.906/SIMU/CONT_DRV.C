/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		CONT_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	971112, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		UC Send_Contrast( UC Contrast);													*/
/*		UC Init_Contrast();																	*/
/*		UC Calculate_Temp( UC val);														*/
/*																									*/
/*	Changes:																						*/
/* 970701: SendContrast now uses the variable TempMeas instead of reading	*/
/*			   the variable itself															*/ 
/* 970112: Send_ & Init_Contrast now returns IICErrorCode						*/
/***************************************************************************/

//**************************************************************************
// Include files:

#include <xa.h>

#include "mas.h"

#include "52sync.h"
#include "cont_drv.h"
#include "iniic.h"

code const UC TempTable[60] = {
	27, 27, 27, 28, 28, 29, 29, 30, 30, 30, 31, 31, 32, 32, 33, 33, 33,
	34, 34, 35, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 39, 40, 40, 41,
	41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 49, 50, 51,
	52, 53, 54, 55, 56, 57, 58, 59, 60
};

/***************************************************************************/
/*	Send_Contrast														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		970701, KEn			     													*/
/*																									*/
/*	Function:	Send value to IIC DAC-converter containing display contrast	*/
/*	Remarks:		--																				*/
/*	Returns:		ErrorStatus																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC Send_Contrast( UC Contrast) {

	UC tmp;
	char Comp_Contrast;
								// DAC-adjust Contrast :  C[DAC] = 3*(MAX-Contrast)
								// DAC-adjust LCD-temp :  T[ADC] = (Temp[25]-Temp)/2
								// Temperature-compensate adjustet Contrast-level :
								//   Comp_Contrast = C[DAC] + T[ADC]

	tmp = Calculate_Temp( TempMeas);		// Calculate the temperature based
													//  on TempMeas which is updated every
													//  100ms in MAS.C

	Comp_Contrast = 3*( Max_Contrast - Contrast) + ( tmp - 25)/2;

	if ( Comp_Contrast <= DAC_Min)
		Comp_Contrast = DAC_Min;
	else
		if ( Comp_Contrast >= DAC_Max)
			Comp_Contrast = DAC_Max;

	Wr_Front_IIC2( DAC_addr, DAC_Enable, Comp_Contrast);

	return( IICErrorCode);
}

/***************************************************************************/
/*	Init_Contrast														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		970112, KEn			     													*/
/*																									*/
/*	Function:	Initialize the IIC ADC-converter to control the display		*/
/*					contrast																		*/
/*	Remarks:		--																				*/
/*	Returns:		ErrorCode																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC Init_Contrast(){

	if ( Wr_Front_IIC2( DAC_addr, DAC_Enable, Def_Contrast))
		return( Send_Contrast( Def_Contrast));

	return( IICErrorCode);
}

/***************************************************************************/
/*	Calculate_Temp														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		970108, KEn			     													*/
/*																									*/
/*	Function:	Calculate the measured value from the NTC-resistor				*/
/*	Remarks:		--																				*/
/*	Returns:		The calculated temperature based on the parameter val and	*/
/*					the TempTable is returned												*/
/*	Updates:		--																				*/
/***************************************************************************/
UC Calculate_Temp( UC val) {

	if ( val < 40) 						// DAC-data: Vtemp [0-39]
		return( 60);						// Temperature = MAX [60]

	if ( val < 100)						// DAC-data: Vtemp [40-99]
		return( TempTable[100-val]);

	if ( val < 180)						// DAC-data: Vtemp [100-179]
		return(( 180-val)/3);			// Temperature = [DAC(Vtemp):3]

												// DAC-data: Vtemp [180-255]
	return( 0);								// Temperature = MIN [0]
}
