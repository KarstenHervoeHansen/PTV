/****************************************************************************/
/* MODULE:                                                                  */
/*  pldrv.c - PLD load driver                                               */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  int pld_init( void);                                                		*/
/*  int pld_version( void);                                              		*/
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

#include "reg8051.h"
#include "define.h"
#include "config.h"
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

code UC DataArray[SizeOfPLDCode] _at( StartOfPLDCode);

/*
#define StartOfPLDCode				0x40000L
#define SizeOfPLDCode					0x0A000L
*/

volatile UI SPGPLDPort        _at(0x6A01C);   // Port defined in VIDEO PLD
volatile UI PLDHWVersionPort  _at(0x7000E);   // Port defined in AUDIO PLD
volatile UI PLDTCXOStatusPort _at(0x7000A);   // Port defined in AUDIO PLD


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
  
  for ( i = 0; i < 4000; i++)					// App. > 2us delay.  2000 ORG :JK change 
	 ;

  SPGPLDBuf |= nCONFIGMask;
  SPGPLDPort = SPGPLDBuf;

  while ( !( SPGPLDPort & nSTATUSMask))
	 ;

  for ( i = 0; i < 32500; i++)
    TxByte( DataArray[i]);

/*
#define StartOfPLDCode				0x40000L
#define SizeOfPLDCode					0x0A000L
*/

  if ( SPGPLDPort & CONF_DONEMask)
		result = true;

	BBSPGReset( false, false, false);		// Release the reset for the SPG ucontroller

	if ( result)
    return( OK);

  return( FAIL);
}

/**************************************************************************/
/* pld_version																								   PLDDRV.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000504, KEn, DEV     																				*/
/*																																				*/
/* Function: Returns the HW version of the SPG/AUDIO PLD									*/
/* Remarks:	 Requires the SPG/AUDIO PLD to be loaded											*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int pld_version( void)
{
	return( PLDHWVersionPort);
}

/**************************************************************************/
/* tcxo_status																								   PLDDRV.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000628, KEn, DEV     																				*/
/*																																				*/
/* Function: Returns the status of the TCXO																*/
/* Remarks:	 Requires the SPG/AUDIO PLD to be loaded											*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int tcxo_status( void)
{
	return( PLDTCXOStatusPort & 0xFFFC);
}
