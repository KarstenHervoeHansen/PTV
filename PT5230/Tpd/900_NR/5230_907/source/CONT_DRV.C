/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		CONT_DRV.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		UC SendContrast( UC Contrast);													*/
/*		UC InitContrast();																	*/
/*		UC CalculateTemp( UC val);															*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980604: In SendContrast() altered CompContrast = 3*(..) to					*/
/*				CompContrast =	2*(..)														*/
/* 980514: Released as 4008 002 06811													*/
/* 980513: CalculateTemp now returns a value based on the new NTC resistor	*/
/*				and the "real" hardware on the mainboard.								*/
/***************************************************************************/

//**************************************************************************
// Include files:

#include <xa.h>

#include "mas.h"

#include "cont_drv.h"
#include "iic.h"

/***************************************************************************/
/*	SendContrast														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		980604, KEn			     													*/
/*																									*/
/*	Function:	Send value to IIC DAC-converter containing display contrast	*/
/*	Remarks:		--																				*/
/*	Returns:		IIC Error status															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC SendContrast( UC Contrast) {

	UC tmp;
	char CompContrast;
								// DAC-adjust Contrast :  C[DAC] = 2*(MAX-Contrast)
								// DAC-adjust LCD-temp :  T[ADC] = (Temp[25]-Temp)/2
								// Temperature-compensate adjustet Contrast-level :
								//   Comp_Contrast = C[DAC] + T[ADC]

	tmp = CalculateTemp( TempMeas);		// Calculate the temperature based
													//  on TempMeas which is updated every
													//  100ms in MAS.C

	CompContrast = 2*( MaxContrast - Contrast) + ( tmp - 25)/2;

	if ( CompContrast <= DACMin)
		CompContrast = DACMin;
	else
		if ( CompContrast >= DACMax)
			CompContrast = DACMax;

	WrFrontIIC2( DAC_addr, DACEnable, CompContrast);

	return( IICErrorCode);
}

/***************************************************************************/
/*	InitContrast														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		980509, KEn			     													*/
/*																									*/
/*	Function:	Initialize the IIC ADC-converter to control the display		*/
/*					contrast																		*/
/*	Remarks:		--																				*/
/*	Returns:		IIC Error status															*/
/*	Updates:		--																				*/
/***************************************************************************/
UC InitContrast(){

	if ( WrFrontIIC2( DAC_addr, DACEnable, DefContrast))
		return( SendContrast( DefContrast));

	return( IICErrorCode);
}

/***************************************************************************/
/*	CalculateTemp														 	  	 CONT_DRV.C */
/*																									*/
/* Author:		Carsten hagenb‘k, DEV, 950509	 										*/
/* Revised:		980513, PRC/KEn			 												*/
/*																									*/
/*	Function:	Calculate the measured value from the NTC-resistor				*/
/*	Remarks:		--																				*/
/*	Returns:		The calculated temperature is based on mesurement tests		*/
/*					made by PRC 980513.														*/
/*	Updates:		--																				*/
/***************************************************************************/
UC CalculateTemp( UC val) {

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
