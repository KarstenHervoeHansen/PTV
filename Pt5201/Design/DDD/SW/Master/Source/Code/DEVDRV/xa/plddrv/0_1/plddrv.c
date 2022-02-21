/****************************************************************************/
/* MODULE:                                                                  */
/*  pldrv.c - PLD load driver                                               */
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
#include "plddrv.h"
#include "unitprg.h"
												
#define nCONFIGMask   	0x01
#define DATAMask				0x02
#define DCLKMask				0x04

#define nSTATUSMask			0x02
#define CONF_DONEMask		0x04

#define Low			0
#define High		1

#define Bit0  	0x01

code UC DataArray[40960] _at( 0x10000);

volatile UI SPGPLDPort _at( 0x6A01C);			// Defined in PLD

static UC SPGPLDBuf;

/**************************************************************************/
/* TxByte																											   PLDDRV.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000208																		*/
/* Revised:	 000321, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 CLK period max.10MHz																					*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void TxByte( UC X)
{
  UI locvar;

  for ( locvar = 8; locvar != 0; locvar--)
  {
    SPGPLDBuf &= ~DCLKMask;			// Clock line low
    SPGPLDPort = SPGPLDBuf;

    if (( X & Bit0) == 0)				// Output databit
      SPGPLDBuf &= ~DATAMask;
    else
      SPGPLDBuf |= DATAMask;

    SPGPLDPort = SPGPLDBuf;

    X /= 2;               		 	// Get next databit in position 
													
    SPGPLDBuf |= DCLKMask;	   	// Clock line high
    SPGPLDPort = SPGPLDBuf;
  }
}

/**************************************************************************/
/* pld_init																										   PLDDRV.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000208																		*/
/* Revised:	 000401, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 Set Config LOW										                				  	*/
/*           Wait min. 2us										                 						*/
/*           Set Config HIGH										               						*/
/*           Wait for nSTATUS HIGH										         						*/
/*           Ensure that CONF_DONE is LOW										   						*/
/*					 The SPG should be reset while downloading to prevent the SPG	*/	
/*						to begin executing as it uses the PLD for ports,latches etc.*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int pld_init( void)
{
  int i;
	bool result = false;
																	
	BBSPGReset( true, false, false);		// Reset the SPG ucontroller

  SPGPLDBuf &= ~DCLKMask;
  SPGPLDBuf &= ~nCONFIGMask;
  SPGPLDPort = SPGPLDBuf;
  
  for ( i = 0; i < 2000; i++)					// App. > 2us delay
	 ;

  SPGPLDBuf |= nCONFIGMask;
  SPGPLDPort = SPGPLDBuf;

  while ( !( SPGPLDPort & nSTATUSMask))
	 ;

  for ( i = 0; i < 32500; i++)
    TxByte( DataArray[i]);

  if ( SPGPLDPort & CONF_DONEMask)
		result = true;

	BBSPGReset( false, false, false);		// Release the reset for the SPG ucontroller

	if ( result)
    return( OK);

  return( FAIL);
}
