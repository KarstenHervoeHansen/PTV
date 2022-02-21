/***************************************************************************/
/*      Copyright ProTeleVision Technologies A/S, BRONDBY 1998             */
/*      Project:                PT5230 Digital Video Generator             */
/*      Module:         INIIC.C                                            */
/*      Author:         Preben Christiansen, Kim Engedahl, DEV             */
/*      Org. date:      960703                                             */
/*      Rev. date:      980616, KEn, DEV                                   */
/*      Status:         Version 1.0                                        */
/***************************************************************************/

#include "define.h"
#include "tvp6kdrv.h"
#include "iic.h"

#if 1
#define PAUS    pau=0;
#define PAUS1   pau=0;
#else // original
#define PAUS    pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#define PAUS1   pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0;pau=0
#endif

#define SCL_3L  SCL=0;PAUS1;PAUS1;
#define SCL_3H  SCL=1;PAUS1;PAUS1;

#define STARTCond  SDA=0;PAUS1;PAUS1;SCL=0
#define STOPCond   SDA=0;SCL_3L;SCL=1;while(SCL==0);SCL_3H;SDA=1;PAUS;PAUS

static UC pau;
bit Acknowledge;

/* Define direct pins used for IIC */
_sfrbit    SCL  _atbit(P1,7);   // P1^7
_sfrbit    SDA  _atbit(P1,6);   // P1^6

_sfrbit	   CY _atbit(PSWL, 7);

/***************************************************************************/
/*      ByteTx2                                                    INIIC.C */
/*                                                                         */
/* Author:              Preben Christiansen, DEV, 941124                   */
/* Revised:             970107, KEn                                        */
/*                                                                         */
/*      Function:       Write one byte to IIC bus                          */
/*      Remarks:                --                                         */
/*      Returns:                Acknowledge bit, 1:OK, 0:ERROR             */
/*      Updates:                --                                         */
/***************************************************************************/
bit ByteTx2(UC to_send)
  {
  register UC nr_ofbits = 8;

  do 
    {                         // Coded to almost fastest execute time
    SCL       = 0;
    to_send <<= 1;
    SDA       = CY; PAUS1;
    SCL       = 1;  PAUS1; PAUS;
    while(SCL == 0)
            ;
    SCL = 0; PAUS;
    } while(--nr_ofbits);

  SDA = 1; SDA = 1; SDA = 1; PAUS1;
                                    // Ready for reading Acknowledge bit
  SCL = 1; PAUS; PAUS;
  while( SCL == 0)
    ;
  Acknowledge = !SDA;               // Read here ack-bit
  SCL = 0;
  return(Acknowledge);
  }

/***************************************************************************/
/*      RdFrontIIC                                                 INIIC.C */
/*                                                                         */
/* Author:   Preben Christiansen, DEV, 941124                              */
/* Revised:  980509, KEn                                                   */
/*                                                                         */
/* Function: Read one byte from the IIC bus                                */
/* Remarks:  --                                                            */
/* Returns:  Acknowledge bit, 1:OK, 0:ERROR                                */
/* Updates:  Nothing                                                       */
/*              int ReceiveIIC(UC Address ,UC num ,UC *data)               */
/***************************************************************************/
UC RdFrontIIC(UC Address) 
  {
#if 0
  UC dat;

  ReceiveIIC(Address ,1 ,&dat);
  return(dat);

#else
  register UC result, nr_ofbits;

  STARTCond;  // see Fig. 11 (data sheet)
  Acknowledge = ByteTx2(Address + 1);
  PAUS1; PAUS;
                             // Read byte into result
  result    = 0;                                                                                       
  nr_ofbits = 8;
  do 
    {
    SCL = 1; PAUS; PAUS;
    while( SCL == 0)
      ;
    result += (result + (char) SDA);
    SCL = 0; SCL = 0; PAUS1; PAUS1;
    } while (--nr_ofbits);
                           // Write Acknowledge bit  (`1` for last byte to read) 
  SDA = 1;                 // Ready for reading Acknowledge bit
  SCL_3H;
  while(SCL == 0)
    ;
  SCL = 0; SCL = 0;
  STOPCond;
  return(result);
#endif
  }

/***************************************************************************/
/*      WrFrontIIC                                                 INIIC.C */
/*                                                                         */
/* Author:   Preben Christiansen, DEV, 941124                              */
/* Revised:  980509, KEn                                                   */
/*                                                                         */
/* Function: Write one byte to IIC bus                                     */
/* Remarks:  --                                                            */
/* Returns:  Acknowledge bit, 1:OK, 0:ERROR                                */
/* Updates:  Nothing                                                       */
/*                                                                         */
/***************************************************************************/
bit WrFrontIIC(UC Address, UC Data)
  {
#if 0

  TransmitIIC(Address, 1 ,&Data);
  return 1;

#else
  STARTCond;

  Acknowledge = ByteTx2(Address);
  Acknowledge = ByteTx2(Data);
  STOPCond;
  return(Acknowledge);
#endif
  }
/* end of iniic.c */