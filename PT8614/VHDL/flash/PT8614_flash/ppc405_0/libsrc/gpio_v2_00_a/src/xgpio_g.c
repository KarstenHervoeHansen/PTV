
/*******************************************************************
*
* CAUTION: This file is automatically generated by libgen.
* Version: Xilinx EDK 8.1 EDK_I.18.7
* DO NOT EDIT.
*
* Copyright (c) 2005 Xilinx, Inc.  All rights reserved. 
* 
* Description: Driver configuration
*
*******************************************************************/

#include "xparameters.h"
#include "xgpio.h"

/*
* The configuration table for devices
*/

XGpio_Config XGpio_ConfigTable[] =
{
	{
		XPAR_GENERIC_GPIO_DEVICE_ID,
		XPAR_GENERIC_GPIO_BASEADDR,
		XPAR_GENERIC_GPIO_INTERRUPT_PRESENT,
		XPAR_GENERIC_GPIO_IS_DUAL
	},
	{
		XPAR_LEDS_DEVICE_ID,
		XPAR_LEDS_BASEADDR,
		XPAR_LEDS_INTERRUPT_PRESENT,
		XPAR_LEDS_IS_DUAL
	}
};


