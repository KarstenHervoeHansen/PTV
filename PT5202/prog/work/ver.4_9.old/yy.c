This is main and PLD init routine from 5201 Master Software.



void main(void)
{ 
#ifndef PTV
  int i;
#endif
  pld_init();                // initialize PLD (audio/SPG)
  ext0_init();               // initialize tick Timer source
  ticktimer_start();         // initialize tick Timer

  // kernel initialization
  rtxcinit();                // initialize RTXC

  /* set task processor priority level in global variables */
  /* and enable interrupts at task PPL */
  setintlvl(TASK_INT_LEVEL);

  /* enable interrupts */
  EA = 1;

  ptvmain();

  for (;;) /* loop forever (null task) */
  {
  }
}


void BBSPGReset( bool SPGReset, bool BBReset, bool SPGPrg)
{
  UI tmp = 3;

  if ( SPGReset)
    tmp &= 0xFE;

  if ( BBReset)
    tmp &= 0xFD;

  if ( SPGPrg)
    tmp |= 0x04;

  BBSPGResetPort = tmp;
}














/**************************************************************************/
/* pld_init                                                       PLDDRV.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000208                                    */
/* Revised:   000401, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   Set Config LOW                                                */
/*           Wait min. 2us                                                 */
/*           Set Config HIGH                                               */
/*           Wait for nSTATUS HIGH                                         */
/*           Ensure that CONF_DONE is LOW                                   */
/*           The SPG should be reset while downloading to prevent the SPG  */  
/*            to begin executing as it uses the PLD for ports,latches etc.*/
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int pld_init( void)
{
  int i;
  bool result = false;
                                  
  BBSPGReset( true, false, false);    // Reset the SPG ucontroller

  SPGPLDBuf &= ~DCLKMask;
  SPGPLDBuf &= ~nCONFIGMask;
  SPGPLDPort = SPGPLDBuf;
  
  for ( i = 0; i < 2000; i++)          // App. > 2us delay
   ;

  SPGPLDBuf |= nCONFIGMask;
  SPGPLDPort = SPGPLDBuf;

  while ( !( SPGPLDPort & nSTATUSMask))
   ;

  for ( i = 0; i < 32500; i++)
    TxByte( DataArray[i]);

  if ( SPGPLDPort & CONF_DONEMask)
    result = true;

  BBSPGReset( false, false, false);    // Release the reset for the SPG ucontroller

  if ( result)
    return( OK);

  return( FAIL);
}


