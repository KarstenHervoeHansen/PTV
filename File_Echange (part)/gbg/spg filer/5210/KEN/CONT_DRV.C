/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		CONT_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	980609, KEn	DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		UC Send_Contrast( UC Contrast);													*/
/*		UC Init_Contrast();																	*/
/*		UC Calculate_Temp( UC val);														*/
/*																									*/
/*	Changes:																						*/
/* 980609: In SendContrast() altered CompContrast = 3*(..) to					*/
/*				CompContrast =	2*(..)														*/
/* 980513: CalculateTemp now returns a value based on the new NTC resistor	*/
/*				and the "real" hardware on the mainboard.								*/
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

/***************************************************************************/
/*	Send_Contrast														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		980609, KEn			     													*/
/*																									*/
/*	Function:	Send value to IIC DAC-converter containing display contrast	*/
/*	Remarks:		--																				*/
/*	Returns:		ErrorStatus																	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC Send_Contrast( UC Contrast) {

	UC tmp;
	char Comp_Contrast;
								// DAC-adjust Contrast :  C[DAC] = 2*(MAX-Contrast)
								// DAC-adjust LCD-temp :  T[ADC] = (Temp[25]-Temp)/2
								// Temperature-compensate adjustet Contrast-level :
								//   Comp_Contrast = C[DAC] + T[ADC]

	tmp = Calculate_Temp( TempMeas);		// Calculate the temperature based
													//  on TempMeas which is updated every
													//  100ms in MAS.C

	Comp_Contrast = 2*( Max_Contrast - Contrast) + ( tmp - 25)/2;

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
/* Revised:		980609, PRC/KEn															*/
/*																									*/
/*	Function:	Calculate the measured value from the NTC-resistor				*/
/*	Remarks:		--																				*/
/*	Returns:		The calculated temperature is based on mesurement tests		*/
/*					made by PRC 980513.														*/
/*	Updates:		--																				*/
/***************************************************************************/
UC Calculate_Temp( UC val) {

	if ( val <= 78)
		return( 60 + ((78-val) * 5) / 8);			// over 60 gr.
	else
		if ( val <= 128)
			return( 35 + ((128-val) >> 1));			// 35 to 60 gr.
	else
		if ( val <= 211)
			return( 35-((val-128) * 3) / 7);			// 0 to 35 gr.
   else
		return( 0);
}
