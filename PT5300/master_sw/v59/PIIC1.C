/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   PIIC1.C                                   */
/*  Author:   Preben Christiansen, DEV                        */
/*  Org. date:  960713                                    */
/*  Rev. date:  980720, KEn DEV                             */
/*  Status:   Version 1.1                                 */
/* Purpose:   IIC driver for IIC1 on ext. port                    */
/*                                                  */
/*  This module contains the following functions:                 */
/*                                                  */
/*    bit ByteTx1( UC XS)                                 */
/*    UC IIC1Read( UC Address)                              */
/*    bit IIC1ReadN( UC Address, UC n, UC* s)                   */
/*    bit IIC1Write( UC Address, UC Data)                       */
/*    bit IIC1Write2( UC Address, UC Data, UC Data2)                */
/*    bit IIC1WriteN( UC Address, UC noOfBytes, UC* Data)           */
/*    bit IIC1WrStr( UC Address, UC *Data)                      */
/*    bit IIC1RdStr( UC Address, UC subadr, char *s)                */
/*                                                  */
/*  Changes:                                            */
/* 980720: Added IIC1WriteN()                               */
/* 980703: Added IIC1ReadN()                                */
/* 980616: Released as 4008 002 06812                         */
/* 980514: Released as 4008 002 06811                         */
/***************************************************************************/

#include <stdio.h>
#include <xa.h>                      /* define 8051 registers */

#include "mas.h"
#include "util.h"
#include "iic.h"

#define SDA     (( ind1 & 2) >> 1)
#define SCL     ( ind1 & 1)
#define SDA_0   { ud1buf &= ~2; ud1= ud1buf;}
#define SCL_0   { ud1buf &= ~1; ud1= ud1buf;}
#define SDA_1   { ud1buf |= 2; ud1= ud1buf;}
#define SCL_1   { ud1buf |= 1; ud1= ud1buf;}

// Each "pau=0" delays xx us. (0.15?)
#define PAUS0   pau=0;
#define PAUS    PAUS0;PAUS0
#define PAUS1   PAUS0;PAUS0;PAUS0
#define SCL_3L    SCL_0;PAUS1;PAUS;PAUS
#define SCL_3H    SCL_1;PAUS1;PAUS1
#define STARTCond SDA_0;PAUS1;PAUS1;PAUS;SCL_0
#define STOPCond  SDA_0;SCL_3L;SCL_1;while((SCL)==0); SCL_3H;SDA_1;PAUS1;PAUS

#define Retry 3

UC IIC1ErrorCode;

static near UC pau;

sbit  CY @ 0x207;   // Carry
sbit  Acknowledge;

/***************************************************************************/
/*  ByteTx1                                        PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   970107, KEn                                   */
/*                                                  */
/*  Function: Send one byte to IIC bus 1                        */
/*  Remarks:                                            */
/*  Returns:    Acknowledge                                 */
/*  Updates:    -                                       */
/***************************************************************************/
bit ByteTx1( UC XS) {

   static bit Ack;
   register UC NoOfBit = 8;

   do {                       // Coded to almost fastest execute time
      SCL_0;
      XS <<= 1;
      if ( CY)
         SDA_1
      else
         SDA_0;
      PAUS1;
      SCL_1; PAUS1; PAUS;
      while( SCL == 0)
       ;
      SCL_0; PAUS;
   } while( --NoOfBit);

   SDA_1; SDA_1; SDA_1; PAUS1;
                              // Ready for reading Acknowledge bit
   SCL_1; PAUS; PAUS;
   while( SCL == 0)
      ;
   Ack = !(SDA);              // Read here ack-bit
   SCL_0;
      
  return( Ack);
}

/***************************************************************************/
/*  IIC1Read                                         PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   980509, KEn                                   */
/*                                                  */
/*  Function:                                         */
/*  Remarks:                                            */
/*  Returns:    Byte freqad from IIC                            */
/*  Updates:    IIC1ErrorCode                               */
/***************************************************************************/
UC IIC1Read( UC Address) {

  register UC B;

  IIC1ErrorCode = RxIICTimeout;

  STARTCond;

  for ( B=0; B < Retry; B++)
    if ( Acknowledge = ByteTx1(Address+1)) {
      IIC1ErrorCode = NoIICError;

      PAUS1; PAUS;

      B = 0;              // Read byte into B
      Address = 8;          // Use Address as loop - counter

      do {
        PAUS0;
        SCL_1; PAUS1;
        while((SCL) == 0)
          ;
        B += B + SDA;       // B += B + (char)SDA;
        SCL_0; PAUS1; PAUS; PAUS;
      } while ( --Address != 0);

      // Write Acknowledge bit  (`1` for last byte to read )

      SDA_1;
      SCL_1; PAUS1; PAUS;pau=0;
      while((SCL)==0)
        ;
      SCL_0;
      break;
    }

   STOPCond;

   return( B);
}

/***************************************************************************/
/*  IIC1ReadN                                      PIIC1.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980703                       */
/* Revised:   980703, KEn                                   */
/*                                                  */
/*  Function: Read <n> bytes from the IIC port.                 */
/*  Remarks:    --                                        */
/*  Returns:    Acknowledge                                 */
/*  Updates:    IIC1ErrorCode                               */
/***************************************************************************/
bit IIC1ReadN( UC Address, UC n, UC* s) {

  register UC B, i;

  IIC1ErrorCode = RxIICTimeout;

  STARTCond;

  for ( B = 0; B < Retry; B++)
    if ( Acknowledge = ByteTx1( Address+1)) {

      for ( i = 0; i < n; i++) {
        IIC1ErrorCode = NoIICError;

        PAUS1; PAUS;

        B = 0;              // Read byte into B
        Address = 8;          // Use Address as loop - counter

        do {
          PAUS0;
          SCL_1; PAUS1;
          while((SCL) == 0)
            ;
          B += B + SDA;       // B += B + (char)SDA;
          SCL_0; PAUS1; PAUS; PAUS;

        } while ( --Address != 0);

        s[i] = B;

        // Write Acknowledge bit  (`1` for last byte to read )

        if ( i < (n-1))
          SDA_0
        else
          SDA_1;

        SCL_1; PAUS1; PAUS;pau=0;
        while(( SCL)==0)
          ;
        SCL_0;
        if ( i < (n-1))
          SDA_1;
      }
      break;
  }

   STOPCond;

   return( Acknowledge);
}

/***************************************************************************/
/*  IIC1Write                                      PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   980509, KEn                                   */
/*                                                  */
/*  Function: Write one byte to IIC address                     */
/*  Remarks:    --                                        */
/*  Returns:    Acknowledge bit, 1:OK, 0:ERROR                    */
/*  Updates:    IIc1ErrorCode                               */
/***************************************************************************/
bit IIC1Write( UC Address, UC Data) {

  register UC i;

  IIC1ErrorCode = TxIICTimeout;

  STARTCond;

  for ( i=0; i < Retry; i++)
    if ( Acknowledge = ByteTx1( Address)) {
      IIC1ErrorCode = TxIICDataError;

      if ( Acknowledge = ByteTx1( Data))
        IIC1ErrorCode = NoIICError;

      break;
    }

  STOPCond;

  return( Acknowledge);
}

/***************************************************************************/
/*  IIC1Write2                                       PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   980509, KEn                                   */
/*                                                  */
/*  Function: Write two bytes to IIC address                    */
/*  Remarks:    --                                        */
/*  Returns:    Acknowledge bit, 1:OK, 0:ERROR                    */
/*  Updates:    IIC1ErrorCode                               */
/***************************************************************************/
bit IIC1Write2( UC Address, UC Data, UC Data2) {

  register UC i;

  IIC1ErrorCode = TxIICTimeout;

  STARTCond;

  for ( i=0; i < Retry; i++)
    if ( Acknowledge = ByteTx1( Address)) {
      IIC1ErrorCode = TxIICDataError;

      if ( Acknowledge = ByteTx1( Data))
        if ( Acknowledge = ByteTx1( Data2))
          IIC1ErrorCode = NoIICError;

      break;
    }

  STOPCond;

  return( Acknowledge);
}

/***************************************************************************/
/*  IIC1WriteN                                       PIIC1.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980720                       */
/* Revised:   980720, KEn                                   */
/*                                                  */
/*  Function: Write <n> bytes to an IIC address                 */
/*  Remarks:    --                                        */
/*  Returns:    Acknowledge bit, 1:OK, 0:ERROR                    */
/*  Updates:    IIC1ErrorCode                               */
/***************************************************************************/
bit IIC1WriteN( UC Address, UC noOfBytes, UC* Data) {

  register UC i;

  IIC1ErrorCode = TxIICTimeout;

  STARTCond;

  for ( i=0; i < Retry; i++)
    if ( Acknowledge = ByteTx1( Address)) {
      IIC1ErrorCode = TxIICDataError;

      for ( i = 0; i < noOfBytes; i++)
        if ( !( Acknowledge = ByteTx1( *Data++)))
          break;

      IIC1ErrorCode = NoIICError;
      break;
    }

  STOPCond;

  return( Acknowledge);
}

/***************************************************************************/
/*  IIC1WrStr                                      PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   980509, KEn                                   */
/*                                                  */
/*  Function: EEPROM string write routine from IIC-bus 1            */
/*  Remarks:                                            */
/*  Returns:    Acknowledge bit, 1:OK, 0:ERROR                    */
/*  Updates:    IIC1ErrorCode                               */
/***************************************************************************/
bit IIC1WrStr( UC Address, UC subadr, char* s) {

  register UC i;            

  IIC1ErrorCode = NoIICError;

  STARTCond;

  for ( i=0; i< Retry; i++)
    if ( Acknowledge = ByteTx1( Address))
      break;

  if ( Acknowledge) {
    Acknowledge = ByteTx1( subadr);

    for ( i=0; i<10; i++) {
      if ( Acknowledge)
        Acknowledge = ByteTx1( s[i]);

      if (( s[i] == '\0') || ( !Acknowledge)) {
        if (( !Acknowledge) && ( IIC1ErrorCode = NoIICError))
          IIC1ErrorCode = TxIICDataError;
        break;
      }

      if (((( i+1) & 3) == 0) || (( subadr+i+1) % 16) == 0) {
        STOPCond;     // packet write, enables use of alternative EEPROM
        waitms(40);     // wait while NVRAM store bytes

        STARTCond;      // store next (1-4) bytes

        if ( Acknowledge = ByteTx1( Address))
          Acknowledge = ByteTx1( subadr+i+1);
        else
          IIC1ErrorCode = TxIICTimeout;
        }
    }
  }
  else
    IIC1ErrorCode = TxIICTimeout;

   STOPCond;
   waitms(20);            // Wait while NVRAM store bytes

   return( Acknowledge);    // NB! allow 20ms before next write
}

/***************************************************************************/
/*  IIC1RdStr                                      PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   980509, KEn                                   */
/*                                                  */
/*  Function: Read string[10] from IIC ( address, subaddress)         */
/*  Remarks:    --                                        */
/*  Returns:    Acknowledge bit, 1:OK, 0:ERROR                    */
/*  Updates:    IIC1ErrorCode                               */
/***************************************************************************/
bit IIC1RdStr( UC Address, UC subadr, char *s) {

  register UC i;
                            // Transmit IIC address
  if ( Acknowledge = IIC1Write( Address, subadr)) {

    for ( i=0; i<10; i++) {
      s[i] = IIC1Read( Address);      // Read char from IIC circuit

      if ( IIC1ErrorCode)
        return( 0);
    }
    s[10]= '\0';
  }
  
  return( Acknowledge);
}

/***************************************************************************/
/*  IIC1RdLong                                       PIIC1.C */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 941124                    */
/* Revised:   980703, KEn                                   */
/*                                                  */
/*  Function: Read unsigned long from IIC ( address, subaddres)       */
/*  Remarks:    --                                        */
/*  Returns:    Unsigned long                               */
/*  Updates:    IIC2ErrorCode                               */
/***************************************************************************/
UL IIC1RdLong( UC Address, UC subadr) {

  register UC i;
  UL y;
                          // Transmit RAM address
  if ( Acknowledge = IIC1Write( Address, subadr)) {

    for ( y=i=0; i<4 ; i++)
      y = ( UL) IIC1Read( Address) + ( y << 8);
  }

  return( y);
}

