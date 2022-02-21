/****************************************************************************/
/* MODULE:                                                                  */
/*  tvp6kdrv.c - TI TVP600+ Multimedie chip driver                          */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  int pld_Init( void);                                                		*/
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "define.h"
#include "tables.h"
#include "tvp6kdrv.h"
#include "i2c_drv.h"


/**************************************************************************/
/* tvp6k_init																									 TVP6KDRV.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000401																		*/
/* Revised:	 000401, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int tvp6k_init( void)
{
	return( tvp6k_system( PAL));
}

/**************************************************************************/
/* tvp6k_system		 																						 TVP6KDRV.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000401																		*/
/* Revised:	 000401, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int tvp6k_system( int system)
{
	UC buffer[15];

	if ( system < NTSC)
	{
		buffer[0] = 0x3A;
		buffer[1] = 0x8C;
		TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV);

		buffer[0] = 0x5A;
		buffer[1] = 0x00;
		buffer[2] = 0x10;
		buffer[3] = 0x80;
		buffer[4] = 0xBE;
		buffer[5] = 0xBE;
		buffer[6] = 0x3F;
		buffer[7] = 0x20;
		buffer[8] = 0x42;
		buffer[9] = 0x41;
		buffer[10] = 0xCB;
		buffer[11] = 0x8A;
		buffer[12] = 0x09;
		buffer[13] = 0x2A;
		TransmitIIC( TVP6K_IIC_address, 14, buffer, IIC_PTV);

		buffer[0] = 0x67;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = 0x00;
		buffer[5] = 0x11;
		buffer[6] = 0xA0;
		buffer[7] = 0x00;
		buffer[8] = 0x0A;
		buffer[9] = 0x01;
		buffer[10] = 0x80;
		buffer[11] = 0x8C;
		buffer[12] = 0x4C;
		buffer[13] = 0x30;

		TransmitIIC( TVP6K_IIC_address, 14, buffer, IIC_PTV);

		buffer[0] = 0x77;
		buffer[1] = 0x8C;
		buffer[2] = 0x4C;
		buffer[3] = 0x30;
		buffer[4] = 0x70;
		buffer[5] = 0x16;
		buffer[6] = 0x35;
		buffer[7] = 0x22;
		buffer[8] = 0x0A;
		TransmitIIC( TVP6K_IIC_address, 9, buffer, IIC_PTV);

		buffer[0] = 0x90;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = 0x00;
		buffer[5] = 0x00;
		buffer[6] = 0x00;
		buffer[7] = 0x00;
		buffer[8] = 0x00;
		buffer[9] = 0x00;
		buffer[10] = 0x00;
		buffer[11] = 0x00;
		buffer[12] = 0x00;
		buffer[13] = 0x00;
		buffer[14] = 0x00;

		TransmitIIC( TVP6K_IIC_address, 15, buffer, IIC_PTV);

		buffer[0] = 0x3A;
		buffer[1] = 0x0F;
		TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV);
	}
	else
	{
		buffer[0] = 0x3A;
		buffer[1] = 0x8C;
		TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV);

		buffer[0] = 0x5A;
		buffer[1] = 0x00;
		buffer[2] = 0x01;
		buffer[3] = 0x6B;
		buffer[4] = 0xCC;
		buffer[5] = 0xB8;
		buffer[6] = 0x2E;
		buffer[7] = 0x20;
		buffer[8] = 0x41;
		buffer[9] = 0x38;
		buffer[10] = 0x1F;
		buffer[11] = 0x7C;
		buffer[12] = 0xF0;
		buffer[13] = 0x21;
		TransmitIIC( TVP6K_IIC_address, 14, buffer, IIC_PTV);

		buffer[0] = 0x67;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = 0x00;
		buffer[5] = 0x11;
		buffer[6] = 0xA0;
		buffer[7] = 0x00;
		buffer[8] = 0x0A;
		buffer[9] = 0x01;
		buffer[10] = 0xC0;
		buffer[11] = 0x82;
		buffer[12] = 0x46;
		buffer[13] = 0x30;

		TransmitIIC( TVP6K_IIC_address, 14, buffer, IIC_PTV);

		buffer[0] = 0x77;
		buffer[1] = 0x82;
		buffer[2] = 0x46;
		buffer[3] = 0x30;
		buffer[4] = 0x0C;
		buffer[5] = 0x11;
		buffer[6] = 0x03;
		buffer[7] = 0x22;
		buffer[8] = 0x0A;
		TransmitIIC( TVP6K_IIC_address, 9, buffer, IIC_PTV);

		buffer[0] = 0x90;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = 0x00;
		buffer[5] = 0x00;
		buffer[6] = 0x00;
		buffer[7] = 0x00;
		buffer[8] = 0x00;
		buffer[9] = 0x00;
		buffer[10] = 0x00;
		buffer[11] = 0x00;
		buffer[12] = 0x00;
		buffer[13] = 0x00;
		buffer[14] = 0x00;

		TransmitIIC( TVP6K_IIC_address, 15, buffer, IIC_PTV);

		buffer[0] = 0x3A;
		buffer[1] = 0x0F;
		TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV);
	}

	return( OK);
}
