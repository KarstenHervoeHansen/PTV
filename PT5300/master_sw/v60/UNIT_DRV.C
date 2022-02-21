/***************************************************************************/
/*  Copyright   ProTeleVision Technologies A/S, BRONDBY 2000               */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:     UNIT_DRV.C                                                 */
/*  Author:     Kim Engedahl, DEV                                          */
/*  Org. date:  980119                                                     */
/*  Rev. date:  12.12.2003, JK                                             */
/*  Status:     Version 1.6                                                */
/*                           illegal conversion between pointer types      */
/***************************************************************************/
/*  NOTE: Things like" Errorlevel.... UNKNOWN OPTION" are handled in       */
/*        GetErrorString() in message.c                                    */
/***************************************************************************/
/*                                                                         */
/*  Changes:                                                               */
/* 12.12.2003: Modified to tackle TLG unit at bbu3_addr.                   */
/*                                                                         */
/* 991026: Error in TestAESEBUUnit() corrected                             */
/* 990304: HWInfo has changed; see MAS.H                                   */
/* 981002: In FindStaticOptions(), PT8631: when deciding if the AnlTPG in  */
/*        bbu_2 was AnlTPG2 or AnlTPG5, it was NOT yet tested if a PT8601  */
/*        was present. The PT8631 was then accepted as AnlTPG2. When the   */
/*        PT8601 LATER on was detected it was ALSO accepted as AnlTPG2,    */
/*        as this unit is locked to this position.                         */
/* 980921: In ConfigureSDITPGUnit() & ConfigureAnlTPGUnit(): time/date     */
/*        enable is now cleared if the P8637 TCI is NOT present.           */
/* 980918: In FindStaticOptions(): PT8637 is now tested for a legal time   */
/*        and date in the TCI, AND reset if illegal.                       */
/* 980827: Added BroadcastTCITime()                                        */
/* 980825: Update Time Clock Interface                                     */
/* 980825: Added TrxTimeClockControl()                                     */
/* 980825: Released as 4008 002 06813                                      */
/*        Error in PT8635 powerup initialization. Level on output 2 was    */
/*        the same as level on output 1.                                   */
/* 980616: Released as 4008 002 06812                                      */
/* 980528: Implemented detection of 900-versions, (in GetV24Version())     */
/* 980522: Optimizations in Test functions.                                */
/* 980515: Released as 4008 002 06811                                      */
/* 980515: In FindStaticOptions() & ConfigurePreset():                     */
/*        The Hi-TECH compiler can NOT handle the construction :           */
/*          for (i = A; i <= B + (HWtype == PT8635)                        */
/***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#include <xa.h>

#include "mas.h"

#include "menutree.h"
#include "menu.h"
#include "menuhlp.h"
#include "message.h"
#include "serial1.h"
#include "unit_drv.h"
#include "tables.h"
#include "iic.h"
#include "keyb_drv.h"
#include "util.h"
#include "factory.h"
#include "xaexprt.h"
#include "disp_drv.h"
#include "timing.h"
#include "text.h"



// Temporary variables PRIVATE NOT overlayable

static char cmdBuffer[3] = { 0, 0, 0};

static char tmpBuffer[11];
static char tbuff[80];     /* temporary buffer used for printing messages */
                           /* for test purposes                           */

static UC ErrorCode;             // Temporary error code, 8 BIT
                                 //  ErrorType + ErrorNo
/* ErrorCode is probably not necessary here, as there is no need to report */
/* transmission errors from functions from this module, reports from other */
/* modules are (perhaps) sufficient.                                       */

code UC hd_sys_table[] =
  {
  0x0F, /*  0 */       /* (OFF)             */  
  0xA0, /*  1 */       /* 1920x1080/60/I    */   
  0xB0, /*  2 */       /* 1920x1080/59.94/I */
  0xA1, /*  3 */       /* 1920x1080/50/I    */   
  0x82, /*  4 */       /* 1920x1080/30/P    */   
  0x92, /*  5 */       /* 1920x1080/29.97/P */
  0x83, /*  6 */       /* 1920x1080/25/P    */   
  0x84, /*  7 */       /* 1920x1080/24/P    */   
  0x94, /*  8 */       /* 1920x1080/23.98/P */

  0xA2, /*  9  */      /*  1080sF/30        */ /* added: 25.06.2007 */
  0xB2, /* 10  */      /*  1080sF/29.97     */
  0xA3, /* 11  */      /*  1080sF/25        */
  0xA4, /* 12  */      /*  1080sF/24        */
  0xB4, /* 13  */      /*  1080sF/23.98     */

  0x86, /* 14 */       /* 1280x720/60/P     */    
  0x96, /* 15 */       /* 1280x720/59.94/P  */ 
  0x87, /* 16 */       /* 1280x720/50/P     */    
  0x88, /* 17 */       /* 1280x720/30/P     */    
  0x98, /* 18 */       /* 1280x720/29.97/P  */ 
  0x89, /* 19 */       /* 1280x720/25/P     */    
  0x8A, /* 20 */       /* 1280x720/24/P     */    
  0x9A, /* 21 */       /* 1280x720/23.98/P  */ 
  0x60, /* 22 */       /* NTSC525           */          
  0x20  /* 23 */       /* PAL625            */           
  };

void cstrcat(char * buff, code char* LineTxt);

/***************************************************************************/
/*  FindStaticOptions                                          UNIT_DRV.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980208                                    */
/* Revised:    980918                                                      */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    This function should only be called AFTER all the resets   */
/*              have been released.                                        */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void FindStaticOptions() {

  register UC i;
  UC error;
  UL tmpLong;

  FeedTheWatchdog;

  //***********************************************************************
  // Option PT8601 SDI Analog signal/Pattern Generator.
  // This unit SHOULD only be placed in upper space TXG2.
  // This unit has a dedicated address.
  //***********************************************************************
  if (SndChar(AnlSig_addr, "AI", 0) == V24Ok)
    if (GetV24Version(0, AnlSig_addr, PT8601) == 0)
      ConfigureAnlTPGUnit(0);

  //***********************************************************************
  // Option PT8602/03 SDI Test signal/Pattern Generator 1.
  // This unit can ONLY be placed in connector TXG1.
  // This unit has a dedicated address.
  //***********************************************************************
  if (SndChar(SDISig_addr, "SI", 0) == V24Ok)
    if (GetV24Version(0, SDISig_addr, PT8602) == 0)
      ConfigureSDITPGUnit(0);


  //***********************************************************************
  // Option PT8632 SDI Test signal/Pattern Generator 1.
  // This unit can ONLY be placed in connector TXG1.
  // This unit has a dedicated address.
  //***********************************************************************
  if (SndChar(SDITPG_addr, "SI", 0) == V24Ok)
    if (GetV24Version(0, SDITPG_addr, PT8632) ==0)
      ConfigureSDITPGUnit(0);


  //***********************************************************************
  // Option PT8604 Multiple Black Burst Generator.
  //***********************************************************************
  if (IIC1Write(multibb_addr, 0xff)) {

    AnlBlkUnit[BB2Item].Present = TRUE;
    AnlBlkUnit[BB2Item].HWType = PT8604;

    GetIICVersion(IICBus1, multibb_addr, ProductKUNo_addr,\
                               &AnlBlkUnit[BB2Item].KUNumber);
  }


  //***********************************************************************
  // Option PT8605/35 AES/EBU Audio Generator.
  //***********************************************************************
  if (IIC2Write(sound1_addr, 0xff))
   {
    AESEBUUnit.Present = TRUE;
    AESEBUUnit.HWType = PT8605;

    if (IIC2Write(sound3_addr, 0xff))        // Option PT8635
      AESEBUUnit.HWType = PT8635;

    GetIICVersion(IICBus2, soundram_addr, ProductKUNo_addr, &AESEBUUnit.KUNumber);

    error = AESEBUUnitDrv(0, AESEBUConfig[0].Signal, AESEBUConfig[0].Level, AESEBUConfig[0].Timing);

    if (!error && (AESEBUUnit.HWType == PT8635))
      error = AESEBUUnitDrv(1, AESEBUConfig[1].Signal, AESEBUConfig[1].Level, AESEBUConfig[1].Timing);

    if (error)
      DisplayErrorLine(error, ErrorDisplayTimeout);
  }


  //***********************************************************************
  // Option PT8606 SDI Genlock Module.
  //***********************************************************************
  if (IIC1Write(d1ram_addr, 0xff))
    {
    SDIGenlockUnit.Present = TRUE;
    SDIGenlockUnit.HWType  = PT8606;

    GetIICVersion(IICBus1, d1ram_addr, ProductKUNo_addr, &SDIGenlockUnit.KUNumber);
    }
  else
    {
    if (ActiveGenlockNo == SDIGenlock)
      {
      ActiveGenlockNo = Internal;

      DisplayErrorLine(SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
      }
    }
    

  //***********************************************************************
  // Option PT8616 GPS Genlock Module.
  //***********************************************************************
  if (IIC1Write(d2ram_addr, 0xff))
    {
#if 0
        sprintf(tbuff, "GPS Genlock Present\n");
        Udr_PutNChar(tbuff);
#endif

    GPSGenlockUnit.Present = TRUE;
    GPSGenlockUnit.HWType  = PT8616;
    //hent hardware version
    GetIICVersion(IICBus1, d2ram_addr, 202, &GPSGenlockUnit.KUNumber); //ProductKUNo_addr = 202 for GPS module
    //hent software version
    IIC1Write(d2ram_addr, 201); //address 201 = SW version ud
    GPSGenlockUnit.SWVersion=IIC1Read(d2ram_addr);
    ConfigureLTCUnit();
    }
  else
    {
    if (ActiveGenlockNo == GPSGenlock)
      {
#if 0
        sprintf(tbuff, "ActiveGenlockNo == GPSGenlock, GPS NOT present\n");
        Udr_PutNChar(tbuff);
#endif

      ActiveGenlockNo = Internal;

      DisplayErrorLine(SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
      }
    }

    
    
  //***********************************************************************
  // Option PT8607 TimeCode unit.
  //***********************************************************************
  if (IIC1Write(time_addr, 0xff))
    {
    TimeClockUnit.Present = TRUE;
    TimeClockUnit.HWType = PT8607;

    GetIICVersion(IICBus1, timeram_addr, ProductKUNo_addr, &TimeClockUnit.KUNumber);
    }

  //***********************************************************************
  // Option PT8637 TimeClock unit.
  //***********************************************************************
  if (IIC1Write(time1_addr, TimeClockConfig.TCIControl)) {

    TimeClockUnit.Present = TRUE;
    TimeClockUnit.HWType = PT8637;

    GetIICVersion(IICBus1, timeram_addr, ProductKUNo_addr,\
                                   &TimeClockUnit.KUNumber);

                          // Get TCI generel information.
    TimeClockConfig.TCIControl &= 0x1F;
    TimeClockConfig.TCIControl |= (TCIInfoRegister << 5);
    IIC1Write(time1_addr, TimeClockConfig.TCIControl);

    FeedTheWatchdog;

    IIC1ReadN(time1_addr, 4, (UC*) tmpBuffer);

    TimeClockUnit.HWType = tmpBuffer[1];
    TimeClockUnit.HWVersion = tmpBuffer[2];
    TimeClockUnit.SWVersion = tmpBuffer[3];

    FeedTheWatchdog;

                          // Set TCI time reference.
    TimeClockConfig.TCIControl &= 0x1F;
    TimeClockConfig.TCIControl |= (TCITimeRefRegister << 5);
    TrxTimeClockData(TimeClockConfig.TCIControl, 1, (UL) TimeClockConfig.Reference);

    FeedTheWatchdog;

                          // Set TCI time offset.
    TimeClockConfig.TCIControl &= 0x1F;
    TimeClockConfig.TCIControl |= (TCIOffsetRegister << 5);
    TrxTimeClockData(TimeClockConfig.TCIControl, 3, (UL) TimeClockConfig.Offset);

    FeedTheWatchdog;

                          // Read TCI time.
    TimeClockConfig.TCIControl &= 0x1F;
    TimeClockConfig.TCIControl |= (TCITimeRegister << 5);
    IIC1Write(time1_addr, TimeClockConfig.TCIControl);

    FeedTheWatchdog;

    IIC1ReadN(time1_addr, 4, (UC*) tmpBuffer);

    tmpLong = tmpBuffer[1];
    tmpLong += ((UL) tmpBuffer[2] << 8);
    tmpLong += ((UL) tmpBuffer[3] << 16);

    if (!ValidTime(tmpLong)) {    // Validate TCI time information.
      tmpLong = TimeClockConfigFactory.Time;
                          // Update TCI
      TrxTimeClockData(TimeClockConfig.TCIControl, 3, tmpLong);
    }
                          // Convert time to user format
    TimeFormat((TimeClockConfig.Format & 0x01), tmpLong, 1, tmpBuffer);
      
    SndBroadcast("BT", tmpBuffer);  // Broadcast time information

    FeedTheWatchdog;

                          // Read TCI date.
    TimeClockConfig.TCIControl &= 0x1F;
    TimeClockConfig.TCIControl |= (TCIDateRegister << 5);
    IIC1Write(time1_addr, TimeClockConfig.TCIControl);
  
    FeedTheWatchdog;

    IIC1ReadN(time1_addr, 4, (UC*) tmpBuffer);

    tmpLong = tmpBuffer[1];
    tmpLong += ((UL) tmpBuffer[2] << 8);
    tmpLong += ((UL) tmpBuffer[3] << 16);

    if (!ValidDate(tmpLong)) {    // Validate TCI date information.
      tmpLong = TimeClockConfigFactory.Date;
                          // Update TCI
      TrxTimeClockData(TimeClockConfig.TCIControl, 3, tmpLong);
    }
                          // Convert date to user format
    DateFormat((TimeClockConfig.Format >> 1), tmpLong, 1, tmpBuffer);

    SndBroadcast("BD", tmpBuffer);  // Broadcast date information
  }
  
  
  //***********************************************************************
  // Option PT8643 NTP unit.
  //***********************************************************************
  if (IIC1Write(ntp_addr, 0xff))
    {
	#if 0
        sprintf(tbuff, "NTP Present\n");
        Udr_PutNChar(tbuff);
	#endif

    NTPUnit.Present = TRUE;
    NTPUnit.HWType  = PT8643;
    //hent hardware version
    //GetIICVersion(IICBus1, ntp_addr, 202, &GPSGenlockUnit.KUNumber); //ProductKUNo_addr = 202 for GPS module
    
    }


  //***********************************************************************
  // Option PT8610 High Precision OCX oscillator.
  //***********************************************************************
  i = (UC) RecInt(sync_addr, "GS", 0xFF, &error);

  if (!error) {
    if (i & OvenMask) {
      Int2GenlockUnit.Present = TRUE;
      Int2GenlockUnit.HWType = PT8610;
    }
    else {
      if (ActiveGenlockNo == Internal2) {
        ActiveGenlockNo = Internal;

        DisplayErrorLine(SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
      }
    }
  }
}

/***************************************************************************/
/*  FindDynamicOptions                                         UNIT_DRV.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980208                                    */
/* Revised:    981002                                                      */
/*                                                                         */
/*  Function: This function examins what boards are installed at the       */
/*            sockets A, B and C (marked as XA1, XB1 and XC1 on the PCB).  */
/*            Which boards can be mounted where is described in            */
/*            5230tech.doc.  This function is now modified to tackle       */
/*            TriLevel Sync and HD-SDI Generator boards, in the following  */
/*            way:                                                         */
/*            Up to 3 TLS boards can be mounted in A, B and C              */
/*            Up to 3 HD-SDI boards can be mounted in A, B and C           */
/*            and combinations of above.                                   */
/*            Individual generators are numbered as shown on the drawing   */
/*            "TLS and HD-SDI numbers".                                    */
/*            This function now searches TLS and HD-SDI for each bbux_addr */
/*  Remarks:    This function MUST be called as bbu1_addr, bbu2_addr &     */
/*              bbu3_addr.                                                 */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/*              12.03.2004 tackling of TLG board added                     */
/***************************************************************************/
void FindDynamicOptions(UC conn_addr)
  {
  UC ndx;//, tmp;


  FeedTheWatchdog;

  switch (conn_addr)
    {
    case bbu1_addr:
      //*****************************************************************
      // Option PT8608 Black Burst & PT8609 Serial Black.
      //*****************************************************************
      SndChar(DefaultAnlBlk_addr, "HA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      switch (S1BUF)
        {
        case bbu1_addr:          // BB3&4 at bbu1_addr
          GetV24Version(BB3Item, bbu1_addr, PT8608);

          ConfigureAnlBlkUnit(BB3Item);
          ConfigureAnlBlkUnit(BB4Item);
          return;

        case bbu1_addr+0x40:        // SDI-BLK3&4 at bbu1_addr
          if (GetV24Version(1, bbu1_addr, PT8609) == 0)
            ConfigureSDITSGUnit(1);
          return;
        }

      //*****************************************************************
      // Option PT8631 Analog Test signal/Pattern Generator.
      //*****************************************************************
      SndChar(DefaultAnlTPG_addr, "AA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu1_addr)
        {                      // ANALOG-TPG2 at bbu1_addr
        if (GetV24Version(0, bbu1_addr, PT8631) == 0)
          ConfigureAnlTPGUnit(0);
        return;
        }

      //*****************************************************************
      // Option PT8633 Analog Test signal/Pattern Generator.
      //*****************************************************************
      SndChar(DefaultSDITPG_addr, "SA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu1_addr)
        {                      // SDI-TPG2 at bbu1_addr
        if (GetV24Version(1, bbu1_addr, PT8633) == 0)
          ConfigureSDITPGUnit(1);
        return;
        }

      //*****************************************************************
      // Option PT8639 SDI Test Signal Generator.
      //*****************************************************************
      SndChar(DefaultSDITSG_addr, "HA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu1_addr+0x40)
        {                           // SDI-TSG3 at bbu1_addr
        if (GetV24Version(1, bbu1_addr, PT8639) == 0)
          ConfigureSDITSGUnit(1);
        return;
        }
      //*****************************************************************
      // Option PT8641 SDI-HD TPG.
      //*****************************************************************
      /* Send 'set unit address to con_addr' command */
      SndChar(DefaultHDTPG_addr, "UA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu1_addr.        */
      if (S1BUF == conn_addr)
        {                      // HDTPG at bbu1_addr
        if (GetV24Version(HDTPG1Item, conn_addr, PT8641) == 0)
          {
          ConfigureHDTPGUnit(HDTPG1Item); // settings to HDTPG1....HDTPGUnit[HDTPG1Item].Present = TRUE;
          ConfigureHDTPGUnit(HDTPG2Item);
          ConfigureHDTPGUnit(HDTPG3Item);
          ConfigureHDTPGUnit(HDTPG4Item);
          }
        return;
        }

      // Option PT8640 Tri-Level
      //*****************************************************************
      SndChar(DefaultTLG_addr, "TA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu1_addr.        */
      if (S1BUF == conn_addr)
        {                      // TLG at bbu1_addr
        if (GetV24Version(TLG5Item, conn_addr, PT8640) == 0) // TLGUnit[TLG5Item].Present  = 1;
          {
          ConfigureTLGUnit(TLG5Item); // format and delay to TLG5....TLGUnit[TLG5Item].Present = TRUE;
          ConfigureTLGUnit(TLG6Item);
          ConfigureTLGUnit(TLG7Item);
          ConfigureTLGUnit(TLG8Item);
          }
        return;
        }

      //*****************************************************************
      // Option PT8642 DL TPG.
      // UC GetV24Version(UC ndx, UC address, UC HWtype)
      //*****************************************************************
      /* Send 'set unit address to con_addr' command */
      SndChar(DefaultDLTPG_addr, "VA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu1_addr.        */
      if (S1BUF == conn_addr)
        {                      // DLTPG at bbu1_addr
        if (GetV24Version(DLTPG1Item, conn_addr, PT8642) == 0)
          {
          ConfigureDLTPGUnit(DLTPG1Item);
          ConfigureDLTPGUnit(DLTPG2Item);
          }
        return;
        }
      break;

    case bbu2_addr:
      //*****************************************************************
      // Option PT8608 Black Burst & PT8609 Serial Black.
      //*****************************************************************
      SndChar(DefaultAnlBlk_addr, "HA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      switch (S1BUF)
        {
        case bbu2_addr:              // BB7&8 at TXG2Conn
          GetV24Version(BB7Item, bbu2_addr, PT8608);

          ConfigureAnlBlkUnit(BB7Item);
          ConfigureAnlBlkUnit(BB8Item);
          return;

        case bbu2_addr+0x40:         // SDI-BLK7&8 at TXG2Conn
          if (GetV24Version(0, bbu2_addr, PT8609) == 0)
            ConfigureSDITSGUnit(0);
          return;
        }

      //*****************************************************************
      // Option PT8631 Analog Test signal/Pattern Generator.
      // A preliminary test to see whether a PT8601 is installed MUST
      //  be performed.
      //*****************************************************************
      SndChar(DefaultAnlTPG_addr, "AA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu2_addr)
        {                          // ANALOG-TPG2/5 at TXG2
        if ((SndChar(AnlSig_addr, "AI", 0) == V24Ok) || AnlTPGUnit[0].Present || SDITPGUnit[1].Present)
          ndx = 1;              // ANALOG-TPG5
        else
          ndx = 0;              // ANALOG-TPG2

        if (GetV24Version(ndx, bbu2_addr, PT8631) == 0)
          ConfigureAnlTPGUnit(ndx);
        return;
        }
      //*****************************************************************
      // Option PT8633 Analog Test signal/Pattern Generator.
      //*****************************************************************
      SndChar(DefaultSDITPG_addr, "SA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu2_addr)
        {                       // SDI-TPG2/5 at TXG2Conn
        if (AnlTPGUnit[0].Present || SDITPGUnit[1].Present)
          ndx = 2;              // SDI-TPG5
        else
          ndx = 1;              // SDI-TPG2

        if (GetV24Version(ndx, bbu2_addr, PT8633) == 0)
          ConfigureSDITPGUnit(ndx);
        return;
        }
      //*****************************************************************
      // Option PT8639 SDI Test Signal Generator.
      //*****************************************************************
      SndChar(DefaultSDITSG_addr, "HA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu2_addr+0x40)
        {                      // SDI-TSG2 at TXG2Conn
        if (GetV24Version(0, bbu2_addr, PT8639) == 0)
          ConfigureSDITSGUnit(0);
        return;
        }
      //*****************************************************************
      // Option PT8642 DL TPG.
      /*********************************************************************/
      /* Send 'set unit address to con_addr' command, if present, the unit */
      /* will echo the con_addr.                                           */
      SndChar(DefaultDLTPG_addr, "VA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu2_addr.      */
      if (S1BUF == conn_addr)
        {
        if (GetV24Version(DLTPG5Item, conn_addr, PT8642) == 0)
          {
          ConfigureDLTPGUnit(DLTPG5Item);
          ConfigureDLTPGUnit(DLTPG6Item);
          }
        return;
        }
      //*****************************************************************
      // Option PT8641 SDI-HD TPG.
      /*********************************************************************/
      /* Send 'set unit address to con_addr' command, if present, the unit */
      /* will echo the con_addr.                                           */
      SndChar(DefaultHDTPG_addr, "UA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu2_addr.      */
      if (S1BUF == conn_addr)
        {
        if (GetV24Version(HDTPG9Item, conn_addr, PT8641) == 0)
          {
          ConfigureHDTPGUnit(HDTPG9Item); // settings to HDTPG9....HDTPGUnit[HDTPG9Item].Present = TRUE;
          ConfigureHDTPGUnit(HDTPG10Item);
          ConfigureHDTPGUnit(HDTPG11Item);
          ConfigureHDTPGUnit(HDTPG12Item);
          }
        return;
        }

      // Option PT8640 Tri-Level
      //*****************************************************************
      SndChar(DefaultTLG_addr, "TA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu2_addr.        */
      if (S1BUF == conn_addr)
        {
        if (GetV24Version(TLG9Item, conn_addr, PT8640) == 0)
          {
          ConfigureTLGUnit(TLG9Item);
          ConfigureTLGUnit(TLG10Item);
          ConfigureTLGUnit(TLG11Item);
          ConfigureTLGUnit(TLG12Item);
          }
        return;
        }
      break;

    case bbu3_addr:
      //*****************************************************************
      // Option PT8642 DL TPG.
      //*****************************************************************
      /* Send 'set unit address to con_addr' command */
      SndChar(DefaultDLTPG_addr, "VA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu3_addr.        */
      if (S1BUF == conn_addr)
        {
        if (GetV24Version(DLTPG3Item, conn_addr, PT8642) == 0)
          {
          ConfigureDLTPGUnit(DLTPG3Item);
          ConfigureDLTPGUnit(DLTPG4Item);
          }
        return;
        }

      //*****************************************************************
      // Option PT8641 SDI-HD TPG.
      //*****************************************************************
      /* Send 'set unit address to con_addr' command */
      SndChar(DefaultHDTPG_addr, "UA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu3_addr.        */
      if (S1BUF == conn_addr)
        {
        if (GetV24Version(HDTPG5Item, conn_addr, PT8641) == 0)
          { 
          ConfigureHDTPGUnit(HDTPG5Item); // settings to HDTPG5Item....HDTPGUnit[HDTPG5Item].Present = TRUE;
          ConfigureHDTPGUnit(HDTPG6Item);
          ConfigureHDTPGUnit(HDTPG7Item);
          ConfigureHDTPGUnit(HDTPG8Item);
          }
          
        return;
        }

      //*****************************************************************
      // Option PT8640 Tri-Level
      //*****************************************************************
      /*******************************************************************/
      /* When we have 3 TLG boards mounted in bbu1_addr, bbu2_addr and   */
      /* bbu3_addr, the third board here cannot be recognized.  We get   */
      /* result = TxV24Timeout and it is impossible to find out why, as  */
      /* the transmission protocols for Master and TLG are unknown.      */
      /* When we mount two TLG boards and a PTV board here at bbu3_addr, */
      /* everything works fine.                                          */
      /* This problem is overcame if we program TX output in slaves as   */
      /* Open Drain !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!          */
      /*******************************************************************/
      SndChar(DefaultTLG_addr, "TA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      /* If the board is present, it will respond with bbu1_addr.        */
      if (S1BUF == conn_addr)
        {
        if (GetV24Version(TLG1Item, conn_addr, PT8640) == 0)
          {
          ConfigureTLGUnit(TLG1Item);
          ConfigureTLGUnit(TLG2Item);
          ConfigureTLGUnit(TLG3Item);
          ConfigureTLGUnit(TLG4Item);
          }
        return;
        }
      /* If the board is not present, check for other boards             */
      //*****************************************************************
      // Option PT8608 Black Burst & PT8609 Serial Black @ bbu3_addr.
      //*****************************************************************
      SndChar(DefaultAnlBlk_addr, "HA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      switch (S1BUF)
        {
        case bbu3_addr:                // adds BB5&6 at TXG4Conn
          GetV24Version(BB5Item, bbu3_addr, PT8608);

          ConfigureAnlBlkUnit(BB5Item); // sets .Present true
          ConfigureAnlBlkUnit(BB6Item);
          return;

        case bbu3_addr+0x40:        // SDI-BLK5&6 at TXG4Conn
          if (GetV24Version(2, bbu3_addr, PT8609) == 0)
            ConfigureSDITSGUnit(2);
          return;
        }

      //*****************************************************************
      // Option PT8639 SDI Test Signal Generator @ bbu3_addr
      //*****************************************************************
      SndChar(DefaultSDITSG_addr, "HA", conn_addr);

      waitms(10);              // Wait for an answer from the unit
      FeedTheWatchdog;

      if (S1BUF == bbu3_addr+0x40)
        {                      // SDI-TSG4 at TXG4Conn
        if (GetV24Version(2, bbu3_addr, PT8639) == 0)
          ConfigureSDITSGUnit(2);
        return;
        }
    } /* switch (conn_addr) */
  }

/***************************************************************************/
/* FindUnit                                     UNIT_DRV.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980407                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Find the next/previous unit to describe/test etc.        */
/*  Remarks:    Only options detected during power-up will be found.      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
UC FindUnit(UC dir, UC unit)
  {
  UC found;

  do
    {
    if (dir == UP)
      {
      if (unit++ == 32)
        unit = 0;
      }
    else
      if (dir == DOWN)
        {
        if(unit-- == 0)
          unit = 32;
        }

    found = FALSE;

    switch (unit)
      {
      case 0:                // MainBoard
      case 1:                // OSCBoard
        found = TRUE;
        break;

      case 2:                // AnlBlkBurst
      case 4:
      case 6:
      case 8:
        if (AnlBlkUnit[unit-2].Present)
          found = TRUE;
        break;

      case 10:                // AnlTPG
      case 11:
        if (AnlTPGUnit[unit-10].Present)
          found = TRUE;
        break;

      case 12:                // SDITSG
      case 13:
      case 14:
        if (SDITSGUnit[unit-12].Present)
          found = TRUE;
        break;

      case 15:                // SDITPG
      case 16:
      case 17:
        if (SDITPGUnit[unit-15].Present)
          found = TRUE;
        break;

      case 18:                // AES-EBU
        if (AESEBUUnit.Present)
          found = TRUE;
        break;

      case 19:                // SDI Genlock
        if (SDIGenlockUnit.Present)
          found = TRUE;
        break;

      case 20:                // Internal 2
        if (Int2GenlockUnit.Present)
          found = TRUE;
        break;

      case 21:                // TimeClock
        if (TimeClockUnit.Present)
          found = TRUE;
        break;
      case 22:                // TLG Board1
        if (TLGUnit[0].Present)
          found = TRUE;
        break;
      case 23:                // TLG Board2
        if (TLGUnit[4].Present)
          found = TRUE;
        break;
      case 24:                // TLG Board3
        if (TLGUnit[8].Present)
          found = TRUE;
        break;

      case 25:                // HDTPG Board1
        if (HDTPGUnit[0].Present)
          found = TRUE;
        break;
      case 26:                // HDTPG Board2
        if (HDTPGUnit[4].Present)
          found = TRUE;
        break;
      case 27:                // HDTPG Board3
        if (HDTPGUnit[8].Present)
          found = TRUE;
        break;

      case 28:                // DLTPG Board1
        if (DLTPGUnit[0].Present)
          found = TRUE;
        break;
      case 29:                // DLTPG Board2
        if (DLTPGUnit[2].Present)
          found = TRUE;
        break;
      case 30:                // DLTPG Board3
        if (DLTPGUnit[4].Present)
          found = TRUE;
        break;
      case 31:          // GPS module
        if(GPSGenlockUnit.Present)
          found = TRUE;
        break;
	  case 32:          // NTP module
        if(NTPUnit.Present)
          found = TRUE;
        break;

      }
    } while (!found);
        
  return(unit);
  }

/***************************************************************************/
/*  FindNextTSGTPG                                             UNIT_DRV.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980204                                    */
/* Revised:    980205                                                      */
/*                                                                         */
/*  Function:  Point to the next intalled TSG/TPG.                         */
/*  Remarks:   Since entry point could be in the middle of the array, the  */
/*             test should be run twice, to ensure that the first elements */
/*             are tested, (it is actually a circular array).              */
/*  Returns:   Next unit for the OUTPUT button.                            */
/*             If NO unit is detected 255  will be returned.               */
/*  Updates:    --                                                         */
/*
enum BaseItems
  {
  AnlBlkItem = 0,
  TLGItem,                 // new TLG
  HDTPGItem,               // new HDTPG
  AnlTPG2Item, AnlTPG5Item,
  SDITSG2Item, SDITSG3Item, SDITSG4Item,
  SDITPG1Item, SDITPG2Item, SDITPG5Item,
  AESEBUItem,  GenlockItem, PresetItem, ConfigItem, TestItem
  };
*/
/***************************************************************************/
UC FindNextTSGTPG(UC unit, UC *next_tlg)
  {
  register UC i = 0;

  do
    {
    switch (unit)
      {
      case AnlBlkItem:
        if (TLGUnit[0].Present || TLGUnit[4].Present || TLGUnit[8].Present)
          return(TLGItem);

      case TLGItem:
        if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
          return(HDTPGItem);

      case HDTPGItem:
        if (AnlTPGUnit[0].Present)
          return(AnlTPG2Item);

      case AnlTPG2Item:
        if (AnlTPGUnit[1].Present)
          return(AnlTPG5Item);

      case AnlTPG5Item:
        if (SDITSGUnit[0].Present)
          return(SDITSG2Item);

      case SDITSG2Item:
        if (SDITSGUnit[1].Present)
          return(SDITSG3Item);

      case SDITSG3Item:
        if (SDITSGUnit[2].Present)
          return(SDITSG4Item);

      case SDITSG4Item:
        if (SDITPGUnit[0].Present)
          return(SDITPG1Item);

      case SDITPG1Item:
        if (SDITPGUnit[1].Present)
          return(SDITPG2Item);

      case SDITPG2Item:
        if (SDITPGUnit[2].Present)
          return(SDITPG5Item);

      case SDITPG5Item:
        if (TLGUnit[0].Present || TLGUnit[4].Present || TLGUnit[8].Present)
          return(TLGItem);

      default:
        unit = AnlBlkItem;
      }                  // If unit not found try once more,
    } while (i++ < 1);   //  starting with the "first" unit.
  return(AnlBlkItem);
  }

/***************************************************************************/
/*  ConfigureSPG                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Download calibration data to the SPG.                */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24//IIC specific error.  */
/*  Updates:    --                                        */
/***************************************************************************/
UC ConfigureSPG()
  {
  UC error, lastError = 0;
  UL tmp;

  FeedTheWatchdog;

  tmp = IIC2RdLong(spgram_addr, HPHZero_G_addr);
  if (IIC2ErrorCode)
    {
    lastError = IIC2ErrorCode;
    }
  else
    {
    if (error = SndLong(sync_addr, "GV", tmp))
      {
      lastError = error;
      }
    }

  tmp = IIC2RdInt(spgram_addr, GenlPhase_G_addr);
  if (IIC2ErrorCode)
    lastError = IIC2ErrorCode;
  else
    {
    if (error = SndInt(sync_addr, "GF", (UI) tmp))
      lastError = error;
    }

  FeedTheWatchdog;

  tmp = IIC2RdLong(spgram_addr, HPHZero_M_addr);
  if (IIC2ErrorCode)
    lastError = IIC2ErrorCode;
  else
    {
    if (error = SndLong(sync_addr, "GY", tmp))
      lastError = error;
    }

  tmp = IIC2RdInt(spgram_addr, GenlPhase_M_addr);
  if (IIC2ErrorCode)
    lastError = IIC2ErrorCode;
  else
    {
    if (error = SndInt(sync_addr, "GG", (UI) tmp))
      lastError = error;
    }

  if (error = GetIICVersion(IICBus2, spgram_addr, SPGUserText_addr+ProductKUNo_addr, &SPGBoard.KUNumber))
    lastError = error;
                                // Get information
  tmp = RecInt(sync_addr, "GI", 0xFF, &error);

  if (error)
    {
    lastError = error;
    tmp = 0;
    }

  SPGBoard.SWVersion = (UC) tmp;
  return(lastError ? (OSCErrorType + lastError) : 0);
  }




/***************************************************************************/
/* TrxTLGFormat                                                 UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 01.10.03                               */
/* Revised:                                                                */
/*                                                                         */
/* Function: Transmit current format to a TLG unit.                        */
/* Params:   UC subgen - which generator                                   */
/*           UC fmt    - format                                            */
/*           UL del    - delay                                             */
/* Remarks:  Each time format is changed for a generator, delay is set     */
/*           as well.                                                      */
/*           This will set the current_generator variable in PT8640, so    */
/*           subsequent transmissions of delay will refer to this          */
/*           (subgen) generator.                                           */
/* Returns:    System error code, i.e. type incl. V24 specific error.      */
/* Updates:    --                                                          */
/***************************************************************************/
UC TrxTLGFormat(UC subgen, UC fmt, long del)
  {
  UC error;
  error = TrxTLGDelay(subgen, fmt, del, GenlockConfig[ActiveGenlockNo].Delay);
  return(error);
  }

/***************************************************************************/
/* TrxTLGDelay                                                      Menu.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 01.10.03                               */
/* Revised:                                                                */
/*                                                                         */
/* Function: Transmit current delay to a TLG unit.                         */
/* Params:   UC subgen    - which generator, TLG1, TLG2 or TLg3            */
/*           UC form      - format                                         */
/*           long del     - TLG delay                                      */
/*           long gen_del - genlock delay                                  */
/* Remarks:  Each time format is changed for a generator, delay is set     */
/*           as well.                                                      */
/* Returns:  System error code (serial communication errror)               */
/* NOTES:    This function is invoked each time TLG delay is changed       */
/*           Before transmiting, TLG delay is recalculated to include      */
/*           genlock delay.                                                */
/*           TLG delay is argument to this function,                       */
/*           gen_del is taken from GenlockConfig[ActiveGenlockNo].Delay    */
/* NOTE:     This function must not receive negative del !!!!!!!!          */
/***************************************************************************/
UC TrxTLGDelay (UC subgen, UC form, long del, long gen_del)
  {
  UC   error;
  UC   gen;
  long tmp;

  switch (subgen)
    {
    case 0:
    case 1:
    case 2:
    case 3:
      gen = subgen;
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      gen = subgen - 4;
      break;
    case 8:
    case 9:
    case 10:
    case 11:
      gen = subgen - 8;
      break;
    }

  FeedTheWatchdog;


  /* add genlock samples  !!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!! */

  if (form == 0)
    tmp = 0;
  else
    tmp = add_genlock_delay(form, del, gen_del, tlg_fmt_table);

  tmp = adjust_hd_samples(tmp, form, tlg_fmt_table);

  /* See specification: "PT8611 Timing Behaviour" of 23.05.2007, filename unknown. */
  tmp = prepare_samples4fpga(tmp, form, tlg_fmt_table);

  error = SndULUL(TLGUnit[subgen].Address, "TL", (UL)gen, (UL)tlg_fmt_table[form].format_id);

  if (!error)
    error = SndULUL(TLGUnit[subgen].Address, "TK", (UL)gen, tmp);

  return(error ? (TRIErrorType + error) : 0);
  }


/***************************************************************************/
/* void TrxHDTPGSystem()                                       unit_drv.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.03.2005                                                     */
/*                                                                         */
/* Function: This function transmits system, delay                         */
/*           along with genrator number to PT8641 HDTPG board.             */
/* Aguments: UC ndx      - genrator number                                 */
/*           UC system   - system                                          */
/*           UL delay    - delay                                           */
/*           UL gendelay - current genlock delay                           */
/*                                                                         */
/* Returns: Error returned by serial communication with the board.         */
/* Notes:   This is the only function which transmits all parameters to    */
/*          the HD board.                                                  */
/*                                                                         */
/*          27.06.2007: when sending System, instead of simple index, we   */
/*                      send now the byte required by FPGA.  We use table  */
/*                      of course.  In this way, HD board will send data   */
/*                      directly to FPGA.  The table is the same as used   */
/*                      previously in HD board's software.                 */
/***************************************************************************/
// extern UC TrxHDTPGSystem(UC, UC, long, UL);
UC TrxHDTPGSystem(UC ndx, UC system, long delay, UL gendelay)
  {
  UC   error;
  long tmp;
  UC   gen;

  gen = find_hd_gen(ndx);

  /* add genlock samples  !!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!! */

  if (system == 0)
    tmp = 0;
  else
    tmp = add_genlock_delay(system, delay, gendelay, hdtpg_fmt_table);

  tmp = adjust_hd_samples(tmp, system, hdtpg_fmt_table);
#if 0
  tmp += hdtpg_fmt_table[system].samples_per_line *
         hdtpg_fmt_table[system].lines_per_frame;
#else
  /* See specification: "PT8611 Timing Behaviour" of 23.05.2007, filename unknown. */
  tmp = prepare_samples4fpga(tmp, system, hdtpg_fmt_table);
#endif
  /* send system */
  error = SndInt2(HDTPGUnit[ndx].Address, "UG", (UI)gen, (UI)hd_sys_table[system]);

#if 0
        sprintf(tbuff, "HD Del: tmp = %ld\n", tmp);
        Udr_PutNChar(tbuff);
#endif

  /* send delay */
  if (!error)
    error = SndULUL(HDTPGUnit[ndx].Address, "UJ", (UL)gen, (UL)tmp);
  return(error ? (HDTPGErrorType + error) : 0);
  }


/***************************************************************************/
/* UC TrxHDTPGPattern(UC ndx, UC pat)                          unit_drv.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.03.2005                                                     */
/*                                                                         */
/* Function: This function transmits pattern                               */
/*           along with genrator number to PT8641 HDTPG board.             */
/* Aguments: UC ndx      - genrator number                                 */
/*           UC pat      - pattern                                         */
/*                                                                         */
/* Returns: Error returned by serial communication with the board.         */
/* Notes:   This is the only function which transmits all parameters to    */
/*          the HD board.                                                  */
/*                                                                         */
/*          If pattern has attributes, they are taken from HDTPGConfig and */
/*          transmitted, otherwise attribute = 0 is transmitted.           */
/***************************************************************************/
UC TrxHDTPGPattern(UC ndx, UC pat)
  {
  UC gen;
  UC error;

  gen   = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UH", (UI)gen, (UI)pat);

  if ((pat >= HDColorbar) && (pat < HDCrosshatch)) /* if pat has attribs... */
    {
    TrxHDTPGAttributes(ndx, HDTPGConfig[ndx].Attrib[pat]);
    }
  else
    {
    TrxHDTPGAttributes(ndx, 0);
    }
  return(error ? (HDTPGErrorType + error) : 0);
  }

/***************************************************************************/
/* UC TrxHDTPGAttributes(UC ndx, UC attribs)                   unit_drv.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.03.2005                                                     */
/*                                                                         */
/* Function: This function transmits attirbute                             */
/*           along with genrator number to PT8641 HDTPG board.             */
/* Aguments: UC ndx      - genrator number                                 */
/*           UC pat      - attribute                                       */
/*                                                                         */
/* Returns: Error returned by serial communication with the board.         */
/* Notes:   This is the only function which transmits all parameters to    */
/*          the HD board.                                                  */
/***************************************************************************/
UC TrxHDTPGAttributes(UC ndx, UC attribs)
  {
  UC gen;
  UC error;

  gen   = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UI", (UI)gen, (UI)attribs);
  return(error ? (HDTPGErrorType + error) : 0);
  }


UC TrxHDTPGAudioSignal(UC ndx, UC signal)
  {
  UC error;
  UC gen;

  gen = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UL", (UI)gen, (UI)signal);
  return(error ? (HDTPGErrorType + error) : 0);
  }

/* Modified 25.06.2007: level is 2's complement of simple index */
UC TrxHDTPGAudioLevel(UC ndx, UC level)
  {
  UC error;
  UC gen;
  UI itmp;
  UC tmpl;

  itmp = 0x100;
  tmpl = itmp - level;
  
  gen = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UM", (UI)gen, (UI)tmpl);
  return(error ? (HDTPGErrorType + error) : 0);
  }

UC TrxHDTPGAudioClick(UC ndx, int click)
  {
  UC error;
  UC gen;
  int click_del;

  gen = find_hd_gen(ndx);

  click_del = click - 0x0800;
  click_del &= 0x0fff;

  error = SndInt2(HDTPGUnit[ndx].Address, "UK", (UI)gen, (UI)(click - 0x0800));
  return(error ? (HDTPGErrorType + error) : 0);

  return (0);
  }
  
UC TrxHDTPGJTAGMode(UC ndx)
  {
  UC error;
  UC gen;

  gen = find_hd_gen(ndx);

  error = SndInt(HDTPGUnit[ndx].Address, "UN", (UI)gen);
  return(error ? (HDTPGErrorType + error) : 0);
  }
/***************************************************************************/
/*  TrxHDTPGText                                                UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  31.10.2005                                                    */
/*                                                                         */
/* Purpose:  Transmit user text to one of four HD-SDI generator            */
/*           (UN command)                                                  */
/* Args:     UC ndx:     generator number  (0 - 11)                        */
/*           HDTPG_STRUCT* conf: config for generator                      */
/*                                                                         */
/* Remarks:  Each HD-SDI generator has 3 lines of text as defined in       */
/*           HDTPG_STRUCT in mas.h.  Each line can be up to 16 chars.      */
/*           This function sends all 3 lines of text.  If text is OFF,     */
/*           empty string is sent.                                         */
/*           This function DOES NOT send line numbers.  Line numbers are   */
/*           are identifyable by the sequence in which they are sent,      */
/*           line1, line2 and line3.  Generator number is sent with each   */
/*           line.  We cannot combine line number and generator number     */
/*           because the SndAsc() function only accepts numbers less than  */
/*           10.                                                           */
/*           Before sending all 3 lines, lines shorter than the longest    */
/*           are padded with spaces.                                       */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
//    'U', 'Z'  /* GeneratorTextCommand, */

UC TrxHDTPGText(UC ndx, HDTPG_STRUCT* conf)
  {
  UC error;
  UC gen;
  UC dim;    /* used to pad spaces */
  char txtbuff[20];

  gen = find_hd_gen(ndx);


  /* find if text is enabled or disabled */
  if (!(conf->TextEnable & 0x01))           /* Line 1 */
    if (!(conf->TextEnable & 0x02))         /* Line 2 */
      if (!(conf->TextEnable & 0x04))       /* Line 3 */
        {
        /* if no text send text OFF */
        error = SndInt2(HDTPGUnit[ndx].Address, "UU", (UI)gen, (UI)0);
        return(error ? (HDTPGErrorType + error) : 0);
        }

  /* else transmit text ON */
  error = SndInt2(HDTPGUnit[ndx].Address, "UU", (UI)gen, (UI)1);

  /* find length of longest line, do not count lines which are OFF */
  dim = 0;

  if (conf->TextEnable & 0x01) /* if Line1 is enabled... */
    dim = strlen(conf->Text[0]);

  if (conf->TextEnable & 0x02) /* if Line2 is enabled... */
    if (dim < strlen(conf->Text[1]))
      dim = strlen(conf->Text[1]);

  if (conf->TextEnable & 0x04) /* if Line3 is enabled... */
    if (dim < strlen(conf->Text[2]))
      dim = strlen(conf->Text[2]);

  /* pad line with spaces before sending */

  if (conf->TextEnable & 0x01) /* if Line1 is enabled... */
    padd_spaces(txtbuff, conf->Text[0], dim);
  else
    padd_spaces(txtbuff, "", dim);
  error = SndAsc(HDTPGUnit[ndx].Address, "UZ", gen, txtbuff);
  if (error)
    return (HDTPGErrorType + error);

  if (conf->TextEnable & 0x02) /* if Line2 is enabled... */
    padd_spaces(txtbuff, conf->Text[1], dim);
  else
    padd_spaces(txtbuff, "", dim);
  error = SndAsc(HDTPGUnit[ndx].Address, "UZ", gen, txtbuff);
  if (error)
    return (HDTPGErrorType + error);

  if (conf->TextEnable & 0x04) /* if Line3 is enabled... */
    padd_spaces(txtbuff, conf->Text[2], dim);
  else
    padd_spaces(txtbuff, "", dim);
  error = SndAsc(HDTPGUnit[ndx].Address, "UZ", gen, txtbuff);

  if (error)
    {
    return (HDTPGErrorType + error);
    }
  else
    {
    //return (HDTPGErrorType + 1);
    return (0);
    }
  }

/***************************************************************************/
/*  padd_spaces()                                               UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To copy string and padd spaces to string                      */
/* Args:     UC* dest - destination buffer                                 */
/*           UC* src  - source buffer                                      */
/*           UC  len  - desired length of destination string               */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  Nothing                                                       */
/***************************************************************************/
void padd_spaces(char* dest, char* src, UC len)
  {
  UC i;

  i = 0;
  while(src[i])
    {
    dest[i] = src[i];
    i++;
    }
  while (i < len)
    {
    dest[i] = ' ';
    i++;
    }
  dest[i]= '\0';
  }

/***************************************************************************/
/*  TrxHDTPGTextScale()                                         UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To transmit text scale.                                       */
/* Args:     UC ndx:     generator number  (0 - 11)                        */
/*           UC scale:   text scale                                        */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
UC TrxHDTPGTextScale(UC ndx, UC scale)
  {
  UC error;
  UC gen;

  gen = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UQ", (UI)gen, (UI)scale);
  return(error ? (HDTPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxHDTPGTextPos()                                           UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To transmit text position.                                    */
/* Args:     UC ndx:     generator number  (0 - 11)                        */
/*           UC Xpos:    horizontal text position                          */
/*           UC Ypos:    veritcal text position                            */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
UC TrxHDTPGTextPos(UC ndx, UC Xpos, UC Ypos)
  {
  UC error;
  UC gen;

  gen = find_hd_gen(ndx);
  error = SndInt3(HDTPGUnit[ndx].Address, "UP", (UI)gen, (UI)Xpos, (UI)Ypos);
  return(error ? (HDTPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxHDTPGTextColor()                                         UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To transmit text color.                                       */
/* Args:     UC ndx:     generator number  (0 - 11)                        */
/*           UC color:   color                                             */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
UC TrxHDTPGTextColor(UC ndx, UC color)
  {
  UC error;
  UC gen;

  gen = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UR", (UI)gen, (UI)color);
  return(error ? (HDTPGErrorType + error) : 0);
  }

UC TrxHDTPGTextMovement(UC ndx, UC mov)
  {
  UC error;
  UC gen;

  gen = find_hd_gen(ndx);

  error = SndInt2(HDTPGUnit[ndx].Address, "UT", (UI)gen, (UI)mov);
  return(error ? (HDTPGErrorType + error) : 0);
  }


/****************************  HDTPG  *************************/


/****************************  DLTPG  *************************/


/***************************************************************************/
/* void TrxDLTPGSystem()                                       unit_drv.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.03.2005                                                     */
/*                                                                         */
/* Function: This function transmits system, delay                         */
/*           along with genrator number to PT8641 DLTPG board.             */
/* Aguments: UC ndx      - genrator number                                 */
/*           UC system   - system                                          */
/*           UL delay    - delay                                           */
/*           UL gendelay - current genlock delay                           */
/*                                                                         */
/* Returns: Error returned by serial communication with the board.         */
/* Notes:   This is the only function which transmits all parameters to    */
/*          the DL board.                                                  */
/***************************************************************************/
UC TrxDLTPGSystem(UC ndx, UC system, long delay, UL gendelay)
  {
  UC   error;
  long tmp;
  UC   gen;

  gen = find_dl_gen(ndx);


  /* add genlock samples  !!!!!!!!!!!!!!!!!!!!!!!!!!!! !!!!!!!! */

  if (system == 0)
    tmp = 0;
  else
    tmp = add_genlock_delay(system, delay, gendelay, dltpg_fmt_table);

  tmp = adjust_hd_samples(tmp, system, dltpg_fmt_table);

  /* See specification: "PT8611 Timing Behaviour" of 23.05.2007, filename unknown. */
  tmp = prepare_samples4fpga(tmp, system, dltpg_fmt_table);

  error = SndInt2(DLTPGUnit[ndx].Address, "VG", (UI)gen, (UI)hd_sys_table[system]);


  if (!error)
    error = SndULUL(DLTPGUnit[ndx].Address, "VJ", (UL)gen, (UL)tmp);
  return(error ? (DLTPGErrorType + error) : 0);
  }


/***************************************************************************/
/* UC TrxDLTPGPattern(UC ndx, UC pat)                          unit_drv.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.03.2005                                                     */
/*                                                                         */
/* Function: This function transmits pattern                               */
/*           along with genrator number to PT8641 DLTPG board.             */
/* Aguments: UC ndx      - genrator number                                 */
/*           UC pat      - pattern                                         */
/*                                                                         */
/* Returns: Error returned by serial communication with the board.         */
/* Notes:   This is the only function which transmits all parameters to    */
/*          the DL board.                                                  */
/*                                                                         */
/*          If pattern has attributes, they are taken from DLTPGConfig and */
/*          transmitted, otherwise attribute = 0 is transmitted.           */
/***************************************************************************/
UC TrxDLTPGPattern(UC ndx, UC pat)
  {
  UC gen;
  UC error;

  gen   = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VH", (UI)gen, (UI)pat);

  if ((pat >= HDColorbar) && (pat < HDCrosshatch)) /* if pat has attribs... */
    {
    TrxDLTPGAttributes(ndx, DLTPGConfig[ndx].Attrib[pat]);
    }
  else
    {
    TrxDLTPGAttributes(ndx, 0);
    }
  return(error ? (DLTPGErrorType + error) : 0);
  }

/***************************************************************************/
/* UC TrxDLTPGAttributes(UC ndx, UC attribs)                   unit_drv.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.03.2005                                                     */
/*                                                                         */
/* Function: This function transmits attirbute                             */
/*           along with genrator number to PT8641 DLTPG board.             */
/* Aguments: UC ndx      - genrator number                                 */
/*           UC pat      - attribute                                       */
/*                                                                         */
/* Returns: Error returned by serial communication with the board.         */
/* Notes:   This is the only function which transmits all parameters to    */
/*          the DL board.                                                  */
/***************************************************************************/
UC TrxDLTPGAttributes(UC ndx, UC attribs)
  {
  UC gen;
  UC error;

  gen   = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VI", (UI)gen, (UI)attribs);
  return(error ? (DLTPGErrorType + error) : 0);
  }

UC TrxDLTPGInterface(UC ndx, UC inter)
  {
  UC gen;
  UC error;

  gen   = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VX", (UI)gen, (UI)inter);
  return(error ? (DLTPGErrorType + error) : 0);
  }

UC TrxDLTPGAudioSignal(UC ndx, UC signal)
  {
  UC error;
  UC gen;

  gen = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VL", (UI)gen, (UI)signal);
  return(error ? (DLTPGErrorType + error) : 0);
  }

UC TrxDLTPGAudioLevel(UC ndx, UC level)
  {
  UC error;
  UC gen;
  UI itmp;
  UC tmpl;

  itmp = 0x100;
  tmpl = itmp - level;
  
  gen = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VM", (UI)gen, (UI)tmpl);
  return(error ? (DLTPGErrorType + error) : 0);
  }

UC TrxDLTPGAudioClick(UC ndx, int click)
  {
  UC error;
  UC gen;
  int click_del;

  gen = find_dl_gen(ndx);

  click_del = click - 0x0800;
  click_del &= 0x0fff;

  error = SndInt2(DLTPGUnit[ndx].Address, "VK", (UI)gen, (UI)(click - 0x0800));
  return(error ? (DLTPGErrorType + error) : 0);

  return (0);
  }
  
UC TrxDLTPGJTAGMode(UC ndx)
  {
  UC error;
  UC gen;

  gen = find_dl_gen(ndx);

  error = SndInt(DLTPGUnit[ndx].Address, "VN", (UI)gen);
  return(error ? (DLTPGErrorType + error) : 0);
  }
/***************************************************************************/
/*  TrxDLTPGText                                                UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  31.10.2005                                                    */
/*                                                                         */
/* Purpose:  Transmit user text to one of four DL-SDI generator            */
/*           (VZ command)                                                  */
/* Args:     UC ndx:     generator number  (0 - 5)                        */
/*           DLTPG_STRUCT* conf: config for generator                      */
/*                                                                         */
/* Remarks:  Each DL-SDI generator has 3 lines of text as defined in       */
/*           DLTPG_STRUCT in mas.h.  Each line can be up to 16 chars.      */
/*           This function sends all 3 lines of text.  If text is OFF,     */
/*           empty string is sent.                                         */
/*           This function DOES NOT send line numbers.  Line numbers are   */
/*           are identifyable by the sequence in which they are sent,      */
/*           line1, line2 and line3.  Generator number is sent with each   */
/*           line.  We cannot combine line number and generator number     */
/*           because the SndAsc() function only accepts numbers less than  */
/*           10.                                                           */
/*           Before sending all 3 lines, lines shorter than the longest    */
/*           are padded with spaces.                                       */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
//    'V', 'Z'  /* GeneratorTextCommand, */

UC TrxDLTPGText(UC ndx, DLTPG_STRUCT* conf)
  {
  UC error;
  UC gen;
  UC dim;    /* used to pad spaces */
  char txtbuff[20];

  gen = find_dl_gen(ndx);


  /* find if text is enabled or disabled */
  if (!(conf->TextEnable & 0x01))           /* Line 1 */
    if (!(conf->TextEnable & 0x02))         /* Line 2 */
      if (!(conf->TextEnable & 0x04))       /* Line 3 */
        {
        /* if no text send text OFF */
        error = SndInt2(DLTPGUnit[ndx].Address, "VV", (UI)gen, (UI)0);
        return(error ? (DLTPGErrorType + error) : 0);
        }

  /* else transmit text ON */
  error = SndInt2(DLTPGUnit[ndx].Address, "VV", (UI)gen, (UI)1);

  /* find length of longest line, do not count lines which are OFF */
  dim = 0;

  if (conf->TextEnable & 0x01) /* if Line1 is enabled... */
    dim = strlen(conf->Text[0]);

  if (conf->TextEnable & 0x02) /* if Line2 is enabled... */
    if (dim < strlen(conf->Text[1]))
      dim = strlen(conf->Text[1]);

  if (conf->TextEnable & 0x04) /* if Line3 is enabled... */
    if (dim < strlen(conf->Text[2]))
      dim = strlen(conf->Text[2]);

  /* pad line with spaces before sending */

  if (conf->TextEnable & 0x01) /* if Line1 is enabled... */
    padd_spaces(txtbuff, conf->Text[0], dim);
  else
    padd_spaces(txtbuff, "", dim);
  error = SndAsc(DLTPGUnit[ndx].Address, "VZ", gen, txtbuff);
  if (error)
    return (DLTPGErrorType + error);

  if (conf->TextEnable & 0x02) /* if Line2 is enabled... */
    padd_spaces(txtbuff, conf->Text[1], dim);
  else
    padd_spaces(txtbuff, "", dim);
  error = SndAsc(DLTPGUnit[ndx].Address, "VZ", gen, txtbuff);
  if (error)
    return (DLTPGErrorType + error);

  if (conf->TextEnable & 0x04) /* if Line3 is enabled... */
    padd_spaces(txtbuff, conf->Text[2], dim);
  else
    padd_spaces(txtbuff, "", dim);
  error = SndAsc(DLTPGUnit[ndx].Address, "VZ", gen, txtbuff);

  if (error)
    {
    return (DLTPGErrorType + error);
    }
  else
    {
    //return (DLTPGErrorType + 1);
    return (0);
    }
  }

/***************************************************************************/
/*  TrxDLTPGTextScale()                                         UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To transmit text scale.                                       */
/* Args:     UC ndx:     generator number  (0 - 5)                        */
/*           UC scale:   text scale                                        */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
UC TrxDLTPGTextScale(UC ndx, UC scale)
  {
  UC error;
  UC gen;

  gen = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VQ", (UI)gen, (UI)scale);
  return(error ? (DLTPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxDLTPGTextPos()                                           UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To transmit text position.                                    */
/* Args:     UC ndx:     generator number  (0 - 5)                        */
/*           UC Xpos:    horizontal text position                          */
/*           UC Ypos:    veritcal text position                            */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
UC TrxDLTPGTextPos(UC ndx, UC Xpos, UC Ypos)
  {
  UC error;
  UC gen;

  gen = find_dl_gen(ndx);
  error = SndInt3(DLTPGUnit[ndx].Address, "VP", (UI)gen, (UI)Xpos, (UI)Ypos);
  return(error ? (DLTPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxDLTPGTextColor()                                         UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski,                                             */
/* Revised:  30.01.2006                                                    */
/*                                                                         */
/* Purpose:  To transmit text color.                                       */
/* Args:     UC ndx:     generator number  (0 - 5)                        */
/*           UC color:   color                                             */
/*                                                                         */
/* Remarks:                                                                */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/***************************************************************************/
UC TrxDLTPGTextColor(UC ndx, UC color)
  {
  UC error;
  UC gen;

  gen = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VR", (UI)gen, (UI)color);
  return(error ? (DLTPGErrorType + error) : 0);
  }

UC TrxDLTPGTextMovement(UC ndx, UC mov)
  {
  UC error;
  UC gen;

  gen = find_dl_gen(ndx);

  error = SndInt2(DLTPGUnit[ndx].Address, "VT", (UI)gen, (UI)mov);
  return(error ? (DLTPGErrorType + error) : 0);
  }
/****************************  DLTPG  *************************/





/***************************************************************************/
/*  ConfigureAnlBlkUnit                                         UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980526                                                      */
/*                                                                         */
/*  Function:  Download all parameters to a Black Burst unit.              */
/*  Remarks:      --                                                       */
/*  Returns:    System error code, i.e. type incl. V24 specific error.     */
/*  Updates:    --                                                         */
/***************************************************************************/
UC ConfigureAnlBlkUnit(UC ndx)
  {
  UC error;

  AnlBlkUnit[ndx].Present = TRUE;
                              // Just validate delay
  AnlValidateSetting(AnlBlkUnit[ndx].HWType, ndx);

                              // Transmit system/delay/ScHPhase
  error = TrxAnlBlkSystem(ndx, AnlBlkConfig[ndx].System,\
                 AnlBlkConfig[ndx].Delay, AnlBlkConfig[ndx].ScHPhase);

  return(error);
  }


/***************************************************************************/
/*  ConfigureAnlTPGUnit                             UNIT_DRV.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Download all parameters to an Analog-TPG.              */
/*  Remarks:      The sequence of initialization should NOT be changed 'cause  */
/*          the system update in the unit is VERY time consuming.      */
/*          NOTE! When entering this function it is necessary to vali-  */
/*          date the configuration, due to the possibilty of the user  */
/*          having exchanged the unit e.g. a PT8631 with a PT8601,    */
/*          which might make some of the configurations invalid.      */
/*          NOTE! When validating the TIME/DATE information, the PT8637  */
/*          has NOT yet been intialized, hence it is NOT possible to    */
/*          test the TimeClockUnit.nn variables.                */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC ConfigureAnlTPGUnit(UC ndx) {

  UC i, error, lastError = 0, pattern;

  AnlTPGUnit[ndx].Present = TRUE;
                                // Validate the configuration
  AnlValidateSetting(AnlTPGUnit[ndx].HWType, ndx);

  pattern = AnlTPGConfig[ndx].Pattern;      // Get validated pattern

                                 // If TCI (PT8637) NOT present..
  if (!IIC1Write(time1_addr, TimeClockConfig.TCIControl))
    for (i = 0; i < 2; i++)            // Clear possible TCI settings
      AnlTPGConfig[ndx].TextStyle[i] &= 0x0F;

                                // Transmit system/delay
  if (error = TrxAnlTPGDelay(ndx, AnlTPGConfig[ndx].System, AnlTPGConfig[ndx].Delay))
    lastError = error;

  waitms(5);                        //
                                // Transmit pattern/attribute
  if (error = TrxAnlTPGPattern(ndx, pattern, AnlTPGAttrib(ndx, pattern)))
    lastError = error;

  waitms(5);                        //
                                // Transmit ScHPhase
  if (error = TrxAnlTPGScHPhase(ndx, AnlTPGConfig[ndx].ScHPhase))
    lastError = error;

  waitms(5);                        //
                                // Configure text setting
  if (error = ConfigureAnlTPGText(ndx, pattern))
    lastError = error;

  return(lastError);
}

/***************************************************************************/
/*  ConfigureAnlTPGText                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980304                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Download all text parameters to an Analog-TPG.          */
/*  Remarks:      The sequence of initialization should NOT be changed 'cause  */
/*          the system update in the unit is VERY time consuming.      */
/*          The function will differ between the PT8601 & PT8631 units.  */
/*          The function will stop transmitting immediately an error  is */
/*          found.                                    */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC ConfigureAnlTPGText(UC ndx, UC pattern) {

  register UC i;
  UC error, tmp, type = AnlFindPatternType(pattern);

  if (AnlTPGUnit[ndx].HWType == PT8601) {
    error = TrxAnlTPGTextLineEnable(ndx, 1,
                    (AnlTPGConfig[ndx].TextEnable & 0x01));

    waitms(5);                    //

    if (!error)
      error = TrxAnlTPGText(ndx, 1, AnlTPGConfig[ndx].Text[0]);
  }
  else {
                              // Transmit text style
    error = TrxAnlTPGTextStyle(ndx, pattern, AnlTPGConfig[ndx].TextStyle[type]);

    for (i = 0; i < (3 - type); i++) {  // Transmit text information
      tmp = 3*type + i;

      waitms(10);                //

      if (!error)                       // Transmit text line enable
        error = TrxAnlTPGTextLineEnable(ndx, i+1,\
                (AnlTPGConfig[ndx].TextEnable >> tmp) & 0x01);

      waitms(5);                  //

      if (!error)                 // Transmit text line string
        error = TrxAnlTPGText(ndx, i+1, AnlTPGConfig[ndx].Text[tmp]);
    }
  }

  return(error);
}

/***************************************************************************/
/*  ConfigureSDITSGUnit                             UNIT_DRV.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980526                                       */
/*                                                  */
/*  Function:  Download all parameters to a SDI-TSG, (former B/CB).      */
/*  Remarks:      The sequence of initialization should NOT be changed 'cause  */
/*          the system update in the unit is VERY time consuming.      */
/*          The function will differ between the PT8609 & PT8639 units.  */
/*          The function will stop transmitting immediately an error  is */
/*          found.                                    */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    -                                        */
/***************************************************************************/
UC ConfigureSDITSGUnit(UC ndx) {

  UC error, lastError = 0;

  SDITSGUnit[ndx].Present = TRUE;
                                // Validate the configuration
  SDIValidateSetting(SDITSGUnit[ndx].HWType, ndx);

                                // Transmit system/delay
  if (error = TrxSDITSGDelay(ndx, SDITSGConfig[ndx].System, SDITSGConfig[ndx].Delay))
    lastError = error;

  waitms(5);
                                // Transmit pattern
  if (error = TrxSDITSGPattern(ndx, SDITSGConfig[ndx].Pattern))
    lastError = error;

  waitms(5);
                                // Transmit EDH insertion
  if (error = TrxSDITSGEDH(ndx, SDITSGConfig[ndx].EDH))
    lastError = error;

  waitms(5);
                                // Transmit audio signal
  if (error = TrxSDITSGAudioSignal(ndx, SDITSGConfig[ndx].AudioSignal))
    lastError = error;


  if (SDITSGUnit[ndx].HWType == PT8639)  {    // If PT8639..
    waitms(5);
                                // Transmit audio level
    if (error = TrxSDITSGAudioLevel(ndx, SDITSGConfig[ndx].AudioLevel))
      lastError = error;
  }

  return(lastError);
}

/***************************************************************************/
/*  ConfigureSDITPGUnit                                         UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Download all parameters to a SDI-TPG.                       */
/*  Remarks:   The sequence of initialization should NOT be changed 'cause */
/*             the system update in the unit is VERY time consuming.       */
/*             NOTE! When entering this function it is necessary to vali-  */
/*             date the configuration, due to the possibilty of the user   */
/*             having exchanged the unit e.g. a PT8633 with a PT8602,      */
/*             which might make some of the configurations invalid.        */
/*             The function will stop transmitting immediately an error    */
/*             occurs.                                                     */
/*             NOTE! When validating the TIME/DATE information, the PT8637 */
/*             has NOT yet been intialized, hence it is NOT possible to    */
/*             test the TimeClockUnit.nn variables.                        */
/*  Returns:   System error code, i.e. type incl. V24 specific error.      */
/*  Updates:    --                                                         */
/***************************************************************************/
UC ConfigureSDITPGUnit(UC ndx)
  {
  UC i, error, lastError = 0, pattern;

  SDITPGUnit[ndx].Present = TRUE;
                                // Validate the configuration
  SDIValidateSetting(SDITPGUnit[ndx].HWType, ndx);

  pattern = SDITPGConfig[ndx].Pattern;      // Get validated pattern

                                 // If TCI (PT8637) NOT present..
  if (!IIC1Write(time1_addr, TimeClockConfig.TCIControl))
    for (i = 0; i < 2; i++)            // Clear possible TCI settings
      SDITPGConfig[ndx].TextStyle[i] &= 0x0F;

                                // Transmit system/delay
  if (error = TrxSDITPGDelay(ndx, SDITPGConfig[ndx].System, SDITPGConfig[ndx].Delay))
    lastError = error;

  waitms(5);
                                // Transmit pattern/attribute
  if (error = TrxSDITPGPattern(ndx, pattern, SDITPGAttrib(ndx, pattern)))
    lastError = error;

  waitms(5);
                                // Configure text setting
  if (error = ConfigureSDITPGText(ndx, pattern))
    lastError = error;

  waitms(5);
                                // Transmit EDH insertion
  if (error = TrxSDITPGEDH(ndx, SDITPGConfig[ndx].EDH))
    lastError = error;

  waitms(5);
                                // Transmit audio signal
  if (error = TrxSDITPGAudioSignal(ndx, SDITPGConfig[ndx].AudioSignal))
    lastError = error;

  waitms(5);
                                // Transmit audio level
  if (error = TrxSDITPGAudioLevel(ndx, SDITPGConfig[ndx].AudioLevel))
    lastError = error;

  if (SDITPGUnit[ndx].HWType == PT8633)
    {                           // If PT8633..
    waitms(5);
                                // Transmit audio group
    if (error = TrxSDITPGAudioGroup(ndx, SDITPGConfig[ndx].AudioGroup))
      lastError = error;
    }
  return(lastError);
  }


/***************************************************************************/
/* UC ConfigureTLGUnit(UC item)                                    MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 24.06.2004                                                     */
/*          20.07.2005                                                     */
/*                                                                         */
/* Function: To configure given TLG generator                              */
/*                                                                         */
/* Args:     UC item - which TLG                                           */
/*                                                                         */
/* Returns:  System error code, i.e. type incl. V24 specific error.        */
/* Notes:                                                                  */
/*                                                                         */
/*        There is a fail in this function.  It really is not necessary    */
/*        as it is used only after finding units which is followed by      */
/*        genlock delay broadcast, which configures TLG units anyway.      */
/*        This function is, however, used in configuring presets.          */
/*        This function is entered after a board has been detected at      */
/*        socket.                                                          */
/***************************************************************************/
UC ConfigureTLGUnit(UC item)
  {
  UC error;

    TLGUnit[item].Present = TRUE;
    error = TrxTLGFormat(item, TLGConfig[item].Format, TLGConfig[item].Delay);

  return (error);
  }
/***************************************************************************/
/*  ConfigureSDITPGText                                         UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980304                                    */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Download all text parameters to a SDI-TPG.                  */
/*  Remarks:   The sequence of initialization should NOT be changed 'cause */
/*             the system update in the unit is VERY time consuming.       */
/*             The function will differ between the PT860X & PT863X units. */
/*             The function will stop transmitting immediately an error    */
/*             is found.                                                   */
/*  Returns:   System error code, i.e. type incl. IIC specific error.      */
/*  Updates:    --                                                         */
/***************************************************************************/
UC ConfigureSDITPGText(UC ndx, UC pattern)
  {
  register UC i;
  UC error=0, tmp, type = SDIFindPatternType(pattern);
  UC Xpos, Ypos;

  Xpos = SDITPGConfig[ndx].TextPos[0][0];
  Ypos = SDITPGConfig[ndx].TextPos[0][1];

  switch (SDITPGUnit[ndx].HWType)
    {
    case PT8602:
    //case PT8603:
      error = TrxSDITPGTextLineEnable(ndx, 1,
                      (SDITPGConfig[ndx].TextEnable & 0x01));
      waitms(5);                  //
      if (!error)
        error = TrxSDITPGText(ndx, 1, SDITPGConfig[ndx].Text[0]);
      break;
    // new case PT8603: behaviour like in PT5210 :JK
    case PT8603:
      if (SDITPGConfig[ndx].System == SDI625)
        tmp = 22*Ypos+8;
      else
        tmp = 18*Ypos+6;

      error = SndInt3(SDITPGUnit[ndx].Address, "SJ", (UI)1, (UI)(4 + Xpos * 2), (UI)tmp);

      if (error)
        break;
    
      for (i = 0; i < 3; i++)
        {
        waitms(5);
        error = TrxSDITPGText(ndx, i+1, SDITPGConfig[ndx].Text[i]);
        }

      if (error)
        break;

      waitms(5);
      error = TrxSDITPGTextLineEnable(ndx, 0, SDITPGConfig[ndx].TextEnable);
    
      if (error)
        break;
      waitms(5);
        // new for 8603: text move
      error = SndInt(SDITPGUnit[ndx].Address, "SH", SDITPGConfig[ndx].TextMovement);
      break;

    default:
      if (!error)      // Transmit text style
        error = TrxSDITPGTextStyle(ndx, pattern, SDITPGConfig[ndx].TextStyle[type]);
      for (i = 0; i < (3 - type); i++)  // Transmit text information
        {
        tmp = 3*type + i;
        waitms(10);              //
        if (!error)                     // Transmit text line enable
          error = TrxSDITPGTextLineEnable(ndx, i+1,\
                  (SDITPGConfig[ndx].TextEnable >> tmp) & 0x01);
        waitms(5);                //
        if (!error)               // Transmit text line string
          error = TrxSDITPGText(ndx, i+1, SDITPGConfig[ndx].Text[tmp]);
        }
      break;
    }
  return(error);
  }



/***************************************************************************/
/* UC ConfigureHDTPGUnit(UC ndx)                                UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 23.02.2005                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: Send configuration to HDTPG board.                            */
/* Params:   UC ndx - which HDTPG generator (not board)                    */
/* Remarks:                                                                */
/* Returns:  0                                                             */
/* Updates:    --                                                          */
/***************************************************************************/
UC ConfigureHDTPGUnit(UC ndx)
  {
  UC error;
  UC pat;


  FeedTheWatchdog;


  HDTPGUnit[ndx].Present = TRUE;


  error = TrxHDTPGSystem(ndx, 
                         HDTPGConfig[ndx].System,
                         HDTPGConfig[ndx].Delay,
                         GenlockConfig[ActiveGenlockNo].Delay);

  pat = HDTPGConfig[ndx].Pattern;
  //waitms(5);
  TrxHDTPGPattern(ndx, pat); /* this transmits also attributes */
  //waitms(5);
  TrxHDTPGAudioClick(ndx, HDTPGConfig[ndx].AudioClick);
  //waitms(5);
  TrxHDTPGAudioSignal(ndx, HDTPGConfig[ndx].AudioSignal);
  //waitms(5);
  TrxHDTPGAudioLevel(ndx, HDTPGConfig[ndx].AudioLevel);
  //waitms(5);
  TrxHDTPGText(ndx, &HDTPGConfig[ndx]); /* trans text and ON/Off info */
  //waitms(5);
  TrxHDTPGTextColor(ndx, HDTPGConfig[ndx].TextColor); /* trans color and background info */
  TrxHDTPGTextScale(ndx, HDTPGConfig[ndx].TextScale);
  TrxHDTPGTextPos(  ndx, HDTPGConfig[ndx].TextPos[0], HDTPGConfig[ndx].TextPos[1]);
  TrxHDTPGTextMovement(ndx, HDTPGConfig[ndx].TextMovement);
  return(0);
  }
  
  UC ConfigureLTCUnit()
  {
	  UC UcharVal1;
    FeedTheWatchdog;
    
    //configure LTC A
    //7-10: offset 3-0
    IIC1Write2(d2ram_addr, 0x07, (unsigned char)(LTCConfig[0].Offset));
    IIC1Write2(d2ram_addr, 0x08, (unsigned char)(LTCConfig[0].Offset>>8));
    IIC1Write2(d2ram_addr, 0x09, (unsigned char)(LTCConfig[0].Offset>>16));
    IIC1Write2(d2ram_addr, 0x0A, (unsigned char)(LTCConfig[0].Offset>>24));
    
    //11: LTC a format
    IIC1Write2(d2ram_addr, 0x0B, LTCConfig[0].Format);
    //12: dropframe setup
    IIC1Write2(d2ram_addr, 0x0C, LTCConfig[0].DFMode);
    //13+14: sync hour+min
    IIC1Write2(d2ram_addr, 0x0D, LTCConfig[0].DFHour);
    IIC1Write2(d2ram_addr, 0x0E, LTCConfig[0].DFMin);
    //15+16: timezone hour+min
    IIC1Write2(d2ram_addr, 0x0F, LTCConfig[0].TimezoneHour);
    IIC1Write2(d2ram_addr, 0x10, LTCConfig[0].TimezoneMin);
    //17: daylight switch mode
    IIC1Write2(d2ram_addr, 0x11, LTCConfig[0].DaylightMode);
    //18-20: daylight on month+day+hour
    IIC1Write2(d2ram_addr, 0x12, LTCConfig[0].DaylightStartMonth);
    IIC1Write2(d2ram_addr, 0x13, LTCConfig[0].DaylightStartDay);
    IIC1Write2(d2ram_addr, 0x14, LTCConfig[0].DaylightStartHour);
    //21-23: daylight off month+day+hour
    IIC1Write2(d2ram_addr, 0x15, LTCConfig[0].DaylightEndMonth);
    IIC1Write2(d2ram_addr, 0x16, LTCConfig[0].DaylightEndDay);
    IIC1Write2(d2ram_addr, 0x17, LTCConfig[0].DaylightEndHour);
    
    FeedTheWatchdog;
    
    //configure LTC B
    //7-10: offset 3-0
    IIC1Write2(d2ram_addr, 0x1B, (unsigned char)(LTCConfig[1].Offset));
    IIC1Write2(d2ram_addr, 0x1C, (unsigned char)(LTCConfig[1].Offset>>8));
    IIC1Write2(d2ram_addr, 0x1D, (unsigned char)(LTCConfig[1].Offset>>16));
    IIC1Write2(d2ram_addr, 0x1E, (unsigned char)(LTCConfig[1].Offset>>24));
    
    //11: LTC a format
    IIC1Write2(d2ram_addr, 0x1F, LTCConfig[1].Format);
    //12: dropframe setup
    IIC1Write2(d2ram_addr, 0x20, LTCConfig[1].DFMode);
    //13+14: sync hour+min
    IIC1Write2(d2ram_addr, 0x21, LTCConfig[1].DFHour);
    IIC1Write2(d2ram_addr, 0x22, LTCConfig[1].DFMin);
    //15+16: timezone hour+min
    IIC1Write2(d2ram_addr, 0x23, LTCConfig[1].TimezoneHour);
    IIC1Write2(d2ram_addr, 0x24, LTCConfig[1].TimezoneMin);
    //17: daylight switch mode
    IIC1Write2(d2ram_addr, 0x25, LTCConfig[1].DaylightMode);
    
    //18-20: daylight on month+day+hour
    IIC1Write2(d2ram_addr, 0x26, LTCConfig[1].DaylightStartMonth);
    IIC1Write2(d2ram_addr, 0x27, LTCConfig[1].DaylightStartDay);
    IIC1Write2(d2ram_addr, 0x28, LTCConfig[1].DaylightStartHour);
    //21-23: daylight off month+day+hour
    IIC1Write2(d2ram_addr, 0x29, LTCConfig[1].DaylightEndMonth);
    IIC1Write2(d2ram_addr, 0x2A, LTCConfig[1].DaylightEndDay);
    IIC1Write2(d2ram_addr, 0x2B, LTCConfig[1].DaylightEndHour);
    
    FeedTheWatchdog;
    
    //send daylight savings mode
	UcharVal1=0;
	
	if(LTCConfig[0].DaylightState==0)
		UcharVal1|=2;
	else
		UcharVal1|=1;
	
	if(LTCConfig[1].DaylightState==0)
		UcharVal1|=32;
	else
		UcharVal1|=16;
	
	IIC1Write2(d2ram_addr, 0x38, UcharVal1);
 }
    
    


/***************************************************************************/
/* UC FindHDTPGBoard(UC ndx)                                    UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 23.02.2005                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: Given HD generator nr find board to which it belongs.         */
/* Params:   UC ndx - which HDTPG generator (not board)                    */
/* Remarks:                                                                */
/* Returns:  Board number                                                  */
/* Updates:    --                                                          */
/***************************************************************************/
UC FindHDTPGBoard(UC ndx)
  {
  switch (ndx)
    {
    case 0:
    case 1:
    case 2:
    case 3:
      return (0);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      return (1);
      break;
    case 8:
    case 9:
    case 10:
    case 11:
      return (2);
      break;
    }
  return(0xFF);
  }

/***************************************************************************/
/* UC FindDLTPGBoard(UC ndx)                                    UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 23.02.2005                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: Given DL generator nr find board to which it belongs.         */
/* Params:   UC ndx - which DLTPG generator (not board)                    */
/* Remarks:                                                                */
/* Returns:  Board number                                                  */
/* Updates:    --                                                          */
/***************************************************************************/
UC FindDLTPGBoard(UC ndx)
  {
  switch (ndx)
    {
    case 0:
    case 1:
      return (0);;
      break;
    case 2:
    case 3:
      return (1);
      break;
    case 4:
    case 5:
      return (2);
      break;
    }
  return(0xFF);
  }

/***************************************************************************/
/* UC ConfigureDLTPGUnit(UC ndx)                                UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 23.02.2005                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: Send configuration to DLTPG board.                            */
/* Params:   UC ndx - which DLTPG generator (not board)                    */
/* Remarks:                                                                */
/* Returns:  0                                                             */
/* Updates:    --                                                          */
/***************************************************************************/
UC ConfigureDLTPGUnit(UC ndx)
  {
  UC error;
  UC pat;

  FeedTheWatchdog;

  DLTPGUnit[ndx].Present = TRUE;

  error = TrxDLTPGSystem(ndx, 
                         DLTPGConfig[ndx].System,
                         DLTPGConfig[ndx].Delay,
                         GenlockConfig[ActiveGenlockNo].Delay);

  pat = DLTPGConfig[ndx].Pattern;
  //waitms(5);
  TrxDLTPGPattern(ndx, pat); /* this transmits also attributes */
  //waitms(5);
  TrxDLTPGAudioClick(ndx, DLTPGConfig[ndx].AudioClick);
  //waitms(5);
  TrxDLTPGAudioSignal(ndx, DLTPGConfig[ndx].AudioSignal);
  //waitms(5);
  TrxDLTPGAudioLevel(ndx, DLTPGConfig[ndx].AudioLevel);
  //waitms(5);
  TrxDLTPGText(ndx, &DLTPGConfig[ndx]); /* trans text and ON/Off  info */
  //waitms(5);
  TrxDLTPGTextColor(ndx, DLTPGConfig[ndx].TextColor); /* trans color and background info */
  TrxDLTPGTextScale(ndx, DLTPGConfig[ndx].TextScale);
  TrxDLTPGTextPos(  ndx, DLTPGConfig[ndx].TextPos[0], DLTPGConfig[ndx].TextPos[1]);
  TrxDLTPGTextMovement(ndx, DLTPGConfig[ndx].TextMovement);
  return(0);
  }


/***************************************************************************/
/* UC FindTLGBoard(UC ndx)                                      UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 23.02.2005                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: Given TLG generator nr find board to which it belongs.        */
/* Params:   UC ndx - which TLG generator (not board)                      */
/* Remarks:                                                                */
/* Returns:  Board number                                                  */
/* Updates:    --                                                          */
/***************************************************************************/
UC FindTLGBoard(UC ndx)
  {
  switch (ndx)
    {
    case 0:
    case 1:
    case 2:
    case 3:
      return (0);;
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      return (1);
      break;
    case 8:
    case 9:
    case 10:
    case 11:
      return (2);
      break;
    }
  return (0xFF);
  }

/***************************************************************************/
/*  AESEBUUnitDrv                                               UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980216                                    */
/* Revised:    980511                                                      */
/*                                                                         */
/*  Function:  Configure the AES/EBU generator.                            */
/*  Remarks:    AESEBU1:                                                   */
/*          Level     : 3;             (o)                                 */
/*          Signal    : 4;             (o)                                 */
/*          Lock27MHz : 1;             (i): 1: PLL locked                  */
/*          Timing    : 3;             (o):                                */
/*          Vidlock   : 1;             (i): 1: Video locked to audio       */
/*          Level BNC Generator1 : 1;  (i): 1: Level OK                    */
/*          Level BNC Generator2 : 1;  (i): 1: Level OK                    */
/*          Level XLR Generator1 : 1;  (i): 1: Level OK                    */
/*          Not used : 1;              (i):                                */
/*                                                                         */
/*          AESEBU2:                                                       */
/*          Level    : 3;              (o)                                 */
/*          Signal   : 4;              (o)                                 */
/*          Not used : 1;              (i):                                */
/*          Timing   : 3;              (o):                                */
/*          Vidlock  : 1;              (i): 1: Video locked to audio       */
/*          Level XLR Generator2 : 1;  (i): 1: Level OK                    */
/*          Level BNC Wordclock1 : 1;  (i): 1: Level OK                    */
/*          Level BNC Wordclock2 : 1;  (i): 1: Level OK                    */
/*          Signal X2 : 1;             (o): 0: Board is in PT5230          */
/*                                                                         */
/*          This function always attempts to update BOTH IIC ports,        */
/*          even though the first initialized fails.                       */
/*  Returns:    System error code, i.e. type incl. IIC specific error.     */
/*  Updates:    --                                                         */
/***************************************************************************/
UC AESEBUUnitDrv(UC ndx, UC signal, UC level, UC timing) {

  UI port;
  UC error = 0, addr1, addr2;

  FeedTheWatchdog;

  port = AESEBUSignalTable[signal] + 0xF880;

  if (signal != AESEBUWordClock)
    port |= AESEBULevelTable[level] + AESEBUTimingTable[timing];

  if (ndx == AESEBU1Item) {
    addr1 = sound1_addr;
    addr2 = sound2_addr;
  }
  else {
    addr1 = sound3_addr;
    addr2 = sound4_addr;

    port &= 0x7FFF;                  // Clear PT5230 bit
  }

  if (!IIC2Write(addr1, (UC) port))
    error = IIC2ErrorCode;

  if(!IIC2Write(addr2, (UC)(port >> 8)))
    error = IIC2ErrorCode;

  return(error ? (AESEBUErrorType + error) : 0);
}

/***************************************************************************/
/*  GenlockUnitDrv                                              UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 961010                                    */
/* Revised:    980907                                                      */
/*                                                                         */
/*  Purpose:  Given GenlockInput and GenlockSystem, compute mode and send  */
/*            it to SPG, compute ud5 value, send genlock delay to SPG      */
/*  Remarks:    Only a possible LAST detected error will be reported.      */
/*  Returns:    System error code, i.e. type incl. V24/IIC specific error. */
/*  Updates:    --                                                         */
/*

code char* GenlockInputArrTxt[] =
  {
  GenlockATxt,
  GenlockBTxt,
  LoopThroughTxt,
  SDITxt,
  GPSTxt,
  InternalGenlockTxt,
  InternalOCXOGenlockTxt
  };

code char* GenlockSignalArrTxt[]  =
  {
  PALBurstTxt,
  NTSCBurstTxt,
  Sync625Txt,
  Sync525Txt,
  SDI625Txt,
  SDI525Txt,
  MHz358Txt,
  MHz443Txt,
  MHz5Txt,
  MHz10Txt
  };

code char GenlockATxt[]             = "A";
code char GenlockBTxt[]             = "B";
code char LoopThroughTxt[]          = "A-B";
code char SDITxt[]                  = "SDI";
code char GPSTxt[]                  = "GPS";
code char InternalGenlockTxt[]      = "Internal";
code char InternalOCXOGenlockTxt[]  = "Internal 2";

code char PALBurstTxt[]        = "PAL Burst";
code char NTSCBurstTxt[]       = "NTSC Burst";
code char Sync625Txt[]         = "625 Sync";
code char Sync525Txt[]         = "525 Sync";
code char SDI625Txt[]          = "625/50";
code char SDI525Txt[]          = "525/59.94";
code char MHz358Txt[]          = "3.58 MHz";
code char MHz443Txt[]          = "4.43 MHz";
code char MHz5Txt[]            = "5 MHz";
code char MHz10Txt[]           = "10 MHz";

code UC GenlockInputTable[14] =
  {
  0x40, 0x00,      // A
  0xC0, 0x00,      // B
  0xA0, 0x00,      // LOOP
  0x10, 0x00,      // SDI Genlock
  0x10, 0x80,      // GPS Genlock    NEW (formerly 0x00)
  0x01, 0x00,      // Internal
  0x0F, 0x01       // Internal2

code UC GenlockSystemTable[20] =
  {
  0x01, 0x08,      // BurstLockPAL
  0x00, 0x0A,      // BurstLockNTSC
  0x01, 0x07,      // SyncLock625
  0x00, 0x0B,      // SyncLock525
  0x11, 0x06,      // SDI625Lock
  0x10, 0x09,      // SDI525Lock
  0x04, 0x05,      // MHzLock358
  0x05, 0x04,      // MHzLock443
  0x05, 0x03,      // MHzLock5
  0x0D, 0x02       // MHzLock10
    |    |
    |    |
   port  |
        mode
  };
enum GenlockInput
  {                       //gps genlock
  GenlockA = 0, GenlockB, LoopThrough, SDIGenlock, GPSGenlock, Internal, Internal2
  };
enum GenlockSystem
  {
  BurstLockPAL = 0, BurstLockNTSC, SyncLock625, SyncLock525, SDI625Lock,
  SDI525Lock,       MHzLock358,    MHzLock443,  MHzLock5,    MHzLock10
  };
We can choose GPSGenlock in systems: SDI625Lock and SDI525Lock, like on 
SDI Genlock module.
So, mode is (in case of GPS) as it would be in case of SDIGenlock.  That's not
good enough.  We must have a means to convey to SPG that we have GPS Genlock
module.

We will use bit 2 of Genlock port ud5.  This signal is marked FREQ_SEL and 
goes to SPG microprocessor as p1.7.  It was unused by original SPG software.
But, it is fed to some PLD, so it might be used for something.

We do the following: 
in GenlockInputTable[], the entry for GPS is now:
                0x10, 0x80,      // GPS Genlock    NEW

The 0x80 goes first into mode, where bit 7 indicates presence of GPS module
(see  (**** A ****)).
After (**** B ****), the mode variable's bit 7 will indicate presence of GPS
module.  In SPG software this will be used to indicate GPS presence
*/
/***************************************************************************/
UC GenlockUnitDrv(UC GenlockInput, UC GenlockSystem)  // 4, 4 if GPS and SDI625Lock
  {
  UC error, lastError = 0, errorCnt = 0, mode, port, tmp;

  FeedTheWatchdog;

  port = GenlockInputTable[tmp = 2 * GenlockInput]; // 0x10 if GPS (D1_SELECT)
  mode = GenlockInputTable[++tmp];                  // 0x80 if GPS (**** A ****)

  if (GenlockInput < Internal)
    {
    port |= GenlockSystemTable[tmp = 2 * GenlockSystem]; // | 0x11 if GPS and SDI625Lock
    mode |= GenlockSystemTable[++tmp];                   //   0x86 (**** B ****)
    }
    
    //If GPS module selected, send setup info to this device
      if (GenlockInput == GPSGenlock)
        IIC1Write2(d2ram_addr, 0x00, GenlockSystem);

  ud5 = port;

  if (TimeClockUnit.Present)                // If timeclock unit present..
    switch (TimeClockUnit.HWType)
      {
      case PT8607:                          // Timecode unit..
        if (GenlockInput < SDIGenlock)
          {                                 // If external analog genlocked..
          if (GenlockSystem == BurstLockPAL)
            tmp = 0xFF;
          else
            tmp = 0xFE;

          if (!IIC1Write(time_addr, tmp))
            {
            lastError = TimeClockErrorType + IIC1ErrorCode;
            errorCnt++;
            }
          }
        break;

      case PT8637:                // Time Clock Interface
        TimeClockConfig.TCIControl &= 0xFC;   // Force unlocked status AND PAL

        if ((GenlockSystem == BurstLockNTSC) || (GenlockSystem == SyncLock525))
          TimeClockConfig.TCIControl |= 0x02;
      
        TrxTimeClockControl(TimeClockConfig.TCIControl);
        break;
      }
                             
  UnlockedLED = !(ActiveGenlockNo == Internal); // Update UNLOCKED LED status
  GenlockLED  = !(ActiveGenlockNo == Internal); // from 5210

  if (error = UpdateLEDs())
    {          // Update front plate LEDs
    lastError = error;
    errorCnt++;
    }

  FeedTheWatchdog;

  // Update the OSC board, #define sync_addr 0x15
 #if 0
  strcpy(tbuff, "SPG:input = ");
  cstrcat(tbuff, GenlockInputArrTxt[GenlockInput]);
  strcat(tbuff, ", system = ");
  cstrcat(tbuff, GenlockSignalArrTxt[GenlockSystem]);
  Udr_PutNChar(tbuff);

  sprintf(tbuff, " mode = 0x%x, port = 0x%x\n", (int)mode, (int)port);
  Udr_PutNChar(tbuff);
#endif
 if (error = SndChar(sync_addr, "GM", mode)) 
    {
    lastError = OSCErrorType + error;
    errorCnt++;
    }

  if (error = TrxGenlockDelay(GenlockConfig[ExternalGenlockNo].Delay))
    {
    lastError = error;
    errorCnt++;
    }
  if (errorCnt > 1)                 // Multiple Errors detected
    return(SystemErrorType + MultipleError);
  return(lastError);
  }


/***************************************************************************/
/* void cstrcat(char * buff, code char* LineTxt)                           */
/*                                                                         */
/* Author:    Janusz Kuzminski, 18.01.2007                                 */
/* Revised:   18.01.2007                                                   */
/*                                                                         */
/*  Function:  Concatenate a string from code memory to a string in RAM    */
/*             buffer.                                                     */
/*  Args:      char      *buff    - RAM buffer containig a string.         */
/*             code char *LineTxt - text string in ROM memeory             */
/*                                                                         */
/*  Returns:   Nothing                                                     */
/*  Notes:                                                                 */
/***************************************************************************/
void cstrcat(char * buff, code char* LineTxt)
  {
  char tmp;

  while (*buff)
    buff++;

  while (tmp = *LineTxt++)
    *(buff++) = tmp;
  *buff = 0;
  }


/***************************************************************************/
/*  BroadcastTCITime                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980827                          */
/* Revised:    980827                                       */
/*                                                  */
/*  Function:  Read and broadcast time/date from a PT8637 Time Clock     */
/*          Interface.                                  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
UC BroadcastTCITime() {
                        // Get TCI time information
  TimeClockConfig.TCIControl &= 0x1F;
  TimeClockConfig.TCIControl |= (TCITimeRegister << 5);
  IIC1Write(time1_addr, TimeClockConfig.TCIControl);

  FeedTheWatchdog;

  IIC1ReadN(time1_addr, 4, (UC*) tmpBuffer);

  TimeClockConfig.Time = tmpBuffer[1];
  TimeClockConfig.Time += ((UL) tmpBuffer[2] << 8);
  TimeClockConfig.Time += ((UL) tmpBuffer[3] << 16);

                        // Convert time to user format
  TimeFormat((TimeClockConfig.Format & 0x01),\
                TimeClockConfig.Time, 1, tmpBuffer);
      
                        // Broadcast time information
  SndBroadcast("BT", tmpBuffer);

  FeedTheWatchdog;

                        // Get TCI date information
  TimeClockConfig.TCIControl &= 0x1F;
  TimeClockConfig.TCIControl |= (TCIDateRegister << 5);
  IIC1Write(time1_addr, TimeClockConfig.TCIControl);

  FeedTheWatchdog;

  IIC1ReadN(time1_addr, 4, (UC*) tmpBuffer);

  TimeClockConfig.Date = tmpBuffer[1];
  TimeClockConfig.Date += ((UL) tmpBuffer[2] << 8);
  TimeClockConfig.Date += ((UL) tmpBuffer[3] << 16);

                        // Convert date to user format
  DateFormat((TimeClockConfig.Format >> 1),\
                TimeClockConfig.Date, 1, tmpBuffer);
    
                        // Broadcast date information
  SndBroadcast("BD", tmpBuffer);

  return(0);
}


/***************************************************************************/
/*  TrxAnlBlkSystem                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980428                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function: Transmit a system setup to a Black Burst unit.          */
/*  Remarks:  Although the ScHPhase are not dependant of the system, it   */
/*            MUST be transmitted after a system change, due to the      */
/*            nature of the Black Burst unit.                    */
/*  Returns:  System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlBlkSystem(UC ndx, UC system, UL delay, int ScHPhase) {

  UC error;

  error = TrxAnlBlkDelay(ndx, system, delay);  // Transmit system/delay

  waitms(5);

  if (!error)                      // Transmit ScH-Phase
    error = TrxAnlBlkScHPhase(ndx, ScHPhase);

  return(error);
}

/***************************************************************************/
/*  TrxAnlBlkDelay                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit system/delay to a Black Burst unit.            */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlBlkDelay(UC ndx, UC system, UL delay) {

  UC error;
  char V24char;

  FeedTheWatchdog;

  (ndx & 0x01) ? (V24char = 'J') : (V24char = 'H');

  cmdBuffer[0] = V24char;

  switch (system) {
    case PAL_PALID:
      V24char = 'K';
      break;

    case NTSC:
      V24char = 'M';
      break;

    case NTSCwoSetup:
      V24char = 'L';
      break;

    default:                // PAL et.al
      V24char = 'G';
      break;
  }

  cmdBuffer[1] = V24char;

  error = SndLong(AnlBlkUnit[ndx].Address, cmdBuffer, delay);

  return(error ? (AnlBlkErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlBlkScHPhase                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Convert and transmit a ScH-Phase to a Black Burst unit.    */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlBlkScHPhase(UC ndx, int ScHPhase) {

  UC error;
  char V24char;
  long tmp = ScHPhase;

  FeedTheWatchdog;

  (ndx & 0x01) ? (V24char = 'J') : (V24char = 'H');

  cmdBuffer[0] = V24char;
  cmdBuffer[1] = 'H';

  if (ScHPhase < 0)        // -1 deg equ. 359 deg a.s.f.
    tmp += 360;

  tmp *= 2048;
  tmp /= 360;              // tmp = (ScHPhase*2048)/360;

  error = SndInt(AnlBlkUnit[ndx].Address, cmdBuffer, (UI) tmp);

  return(error ? (AnlBlkErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGSystem                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980428                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Transmit a system setup to an Analog-TPG.              */
/*  Remarks:      Only parameters influenced by the system are transmitted,  */
/*          i.e. delay and the pattern. It is for the calling function   */
/*          to ensure that the pattern is valid in the selected system.  */
/*          Pattern should be transmitted first in case the current    */
/*          pattern is invalid in the new system.                */
/*          NOTE! It is not necessary to transmit the ScHPhase.      */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGSystem(UC ndx, UC system, UL delay, UC pattern, UC attrib) {

  UC error;
                                 // Transmit system/delay
  error = TrxAnlTPGDelay(ndx, system, delay);

  waitms(5);

  if (!error)                      // Transmit pattern/attribute
    error = TrxAnlTPGPattern(ndx, pattern, attrib);

  waitms(5);

  if (!error)                       // Transmit text setting
    error = ConfigureAnlTPGText(ndx, pattern);

  return(error);
}

/***************************************************************************/
/*  TrxAnlTPGDelay                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit system and delay to an Analog-TPG.            */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGDelay(UC ndx, UC system, UL delay) {

  UC error;
  char V24char;

  FeedTheWatchdog;

  switch (system) {
    case PAL_PALID:
      V24char = 'K';
      break;

    case NTSC:
      V24char = 'M';
      break;

    case NTSCwoSetup:
      V24char = 'L';
      break;

    default:                // PAL et.al
      V24char = 'G';
      break;
  }

  cmdBuffer[0] = 'A';
  cmdBuffer[1] = V24char;

  error = SndLong(AnlTPGUnit[ndx].Address, cmdBuffer, delay);

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGPattern                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit pattern and attribute to an Analog-TPG.        */
/*  Remarks:      The argument <attrib> is not specified for PT8601.        */
/*          The pattern MUST be valid when calling this function.      */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGPattern(UC ndx, UC pattern, UC attrib) {

  UC error, HWtype = AnlTPGUnit[ndx].HWType;

  FeedTheWatchdog;

                              // Lookup analog pattern code
  pattern = FindPatternTable(HWtype)[pattern];

  if (HWtype == PT8601)
    error = SndInt(AnlTPGUnit[ndx].Address, "AP", pattern);
  else
    error = SndInt2(AnlTPGUnit[ndx].Address, "AP", pattern, attrib);

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGScHPhase                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Convert and transmit a ScH-Phase to an Analog-TPG.        */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGScHPhase(UC ndx, int ScHPhase) {

  UC error;
  long tmp = ScHPhase;

  FeedTheWatchdog;

  if (ScHPhase < 0)        // -1 deg equ. 359 deg a.s.f.
    tmp += 360;

  tmp *= 2048;
  tmp /= 360;              // tmp = (ScHPhase*2048)/360;

  error = SndInt(AnlTPGUnit[ndx].Address, "AH", (UI) tmp);

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGTextStyle                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980301                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Transmit user text style to an Analog-TPG.            */
/*  Remarks:      The function is not specified for PT8601.              */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGTextStyle(UC ndx, UC pattern, UC style) {

  UC error, type = AnlFindPatternType(pattern);

  FeedTheWatchdog;

  error = SndChar(AnlTPGUnit[ndx].Address, "AU", (style & 0xF0) + TPGStandardTextStyle);

  waitms(5);

  error = SndChar(AnlTPGUnit[ndx].Address, "AU", style);

  if ((style & 0x0F)== TPGFreeTextStyle)
  {
    error = TrxAnlTPGTextPos(ndx, style,
                      AnlTPGConfig[ndx].TextPos[type][0],
                      AnlTPGConfig[ndx].TextPos[type][1]);
  }

  if (style & 0xF0)
  {
    switch (pattern)
    {
      case AnlPhilips4x3:
      case AnlPhilips16x9:
        if (AnlTPGConfig[ndx].System < NTSC)
          error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 3, 0, 159);
        else
          error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 3, 0, 135);
        break;

      case AnlFuBK4x3:
      case AnlFuBK16x9:
        error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 3, 0, 268);
        break;

      default:
        if (AnlTPGConfig[ndx].System < NTSC)
          error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 3, 0, 287);
        else
          error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 3, 0, 241);
        break;
    }
  }
  else
  {
    if (type == ComplexPattern)

      // If NO TIME/CLOCK information, ALWAYS disable text line 3.

      if (!(style & 0xF0))
        error = SndInt2(AnlTPGUnit[ndx].Address, "AF", 3, 0);
  }

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGTextPos                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 000509                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Transmit user text position to a Analog-TPG.            */
/*  Remarks:      This function is not specified for PT8601.            */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGTextPos(UC ndx, UC style, UC Xpos, UC Ypos)
{
  UC error, tmp;

  FeedTheWatchdog;

  if (AnlTPGConfig[ndx].System < NTSC)
    tmp = 22*Ypos+9;
  else
    tmp = 18*Ypos+7;

  error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 1, 15+Xpos*2, tmp);

  if (!error)
    error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 2, 15+Xpos*2, tmp+18);

  FeedTheWatchdog;

  if (!error && !(style & 0xF0))
    error = SndInt3(AnlTPGUnit[ndx].Address, "AJ", 3, 15+Xpos*2, tmp+36);

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGTextLineEnable                           UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980301                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit text line enable to an Analog-TPG.            */
/*  Remarks:      The argument <no> is not specified for PT8601, i.e. there  */
/*          is only one text line to enable in PT8601.            */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGTextLineEnable(UC ndx, UC no, UC state) {

  UC error;

  FeedTheWatchdog;

  if (AnlTPGUnit[ndx].HWType == PT8601)
    error = SndInt(AnlTPGUnit[ndx].Address, "AF", state);
  else
    error = SndInt2(AnlTPGUnit[ndx].Address, "AF", no, state);

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxAnlTPGText                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980301                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit user text to a Analog-TPG.                  */
/*  Remarks:      The argument <no> is not specified for PT8601, i.e. there  */
/*          is only one text line in PT8601.  For the PT8601 the text-  */
/*          line will be converted to fixed 8 character width.        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxAnlTPGText(UC ndx, UC no, char* text) {

  register UC i;
  UC error;
  char tmp;

  FeedTheWatchdog;

  if (AnlTPGUnit[ndx].HWType == PT8601) {
    i = 0;
    while ((tmp = text[i]) && (i < 8))
      tmpBuffer[i++] = tmp;

    while (i < 8)
      tmpBuffer[i++] = ' ';

    tmpBuffer[8] = '\0';

    error = SndAsc(AnlTPGUnit[ndx].Address, "AN", 0xFF, tmpBuffer);
  }
  else
    error = SndAsc(AnlTPGUnit[ndx].Address, "AN", no, text);

  return(error ? (AnlTPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITSGSystem                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980504                          */
/* Revised:    980514                                       */
/*                                                  */
/*  Function:  Transmit a system setup to a SDI-TSG.                */
/*  Remarks:      Only parameters influenced by the system are transmitted,  */
/*          i.e. delay and the pattern. It is for the calling function   */
/*          to ensure that the pattern is valid in the selected system.  */
/*          Then pattern is transmitted LAST to be compatible with the  */
/*          PT8609.                                    */
/*  Returns:    System error code, i.e. type incl. V24 specific error      */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITSGSystem(UC ndx, UC system, UL delay, UC pattern) {

  UC error;

  error = TrxSDITSGDelay(ndx, system, delay);  // Transmit system/delay

  waitms(5);

  if (!error)                       // Transmit pattern
    error = TrxSDITSGPattern(ndx, pattern);

  return (error);
}

/***************************************************************************/
/*  TrxSDITSGDelay                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit system and delay to a SDI-TSG.              */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITSGDelay(UC ndx, UC system, UL delay) {

  UC error, V24char;

  FeedTheWatchdog;

  (system == SDI625) ? (V24char = 'G') : (V24char = 'M');

  cmdBuffer[0] = 'H';
  cmdBuffer[1] = V24char;

  error = SndLong(SDITSGUnit[ndx].Address, cmdBuffer, delay);

  return(error ? (SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITSGPattern                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit a pattern to a SDI-TSG.                    */
/*  Remarks:    The pattern MUST be valid when calling this function.      */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITSGPattern(UC ndx, UC pattern) {

  UC error;

  FeedTheWatchdog;
                              // Lookup pattern code
  pattern = FindPatternTable(SDITSGUnit[ndx].HWType)[pattern];

  error = SndChar(SDITSGUnit[ndx].Address, "HP", pattern);

  return(error ? (SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITSGEDH                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit  EDH on/off to a SDI-TSG.                  */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITSGEDH(UC ndx, UC state) {

  UC error;

  FeedTheWatchdog;

  error = SndChar(SDITSGUnit[ndx].Address, "HE", state);

  return(error ? (SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITSGAudioSignal                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit embedded audio signal to a SDI-TSG.            */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITSGAudioSignal(UC ndx, UC signal) {

  UC error;

  FeedTheWatchdog;

  error = SndChar(SDITSGUnit[ndx].Address, "HU", signal);

  return(error ? (SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITSGAudioLevel                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit embedded audio level to a SDI-TSG.            */
/*  Remarks:      This function is not specified for PT8609.            */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITSGAudioLevel(UC ndx, UC level) {

  UC error;

  FeedTheWatchdog;

  error = SndChar(SDITSGUnit[ndx].Address, "HL", level);

  return(error ? (SDITSGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITPGSystem                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980429                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Transmit a system setup to a SDI-TPG.                */
/*  Remarks:      Only parameters influenced by the system are transmitted,  */
/*          i.e. delay and the pattern. It is for the calling function   */
/*          to ensure that the pattern is valid in the selected system.  */
/*          Pattern should be transmitted first in case the current    */
/*          pattern is invalid in the new system.                */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                             */
/***************************************************************************/
UC TrxSDITPGSystem(UC ndx, UC system, UL delay, UC pattern, UC attrib) {

  UC error;

  error = TrxSDITPGDelay(ndx, system, delay);  // Transmit system/delay

  waitms(5);

  if (!error)                       // Transmit pattern/attribute
    error = TrxSDITPGPattern(ndx, pattern, attrib);

  waitms(5);

  if (!error)                       // Transmit text setting
    error = ConfigureSDITPGText(ndx, pattern);
  return (error);
}

/***************************************************************************/
/*  TrxSDITPGDelay                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit delay to a SDI-TPG.                      */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGDelay(UC ndx, UC system, UL delay)
  {
  UC error, V24char;

  FeedTheWatchdog;

  (system == SDI625) ? (V24char = 'G') : (V24char = 'M');

  cmdBuffer[0] = 'S';
  cmdBuffer[1] = V24char;

  error = SndLong(SDITPGUnit[ndx].Address, cmdBuffer, delay);
#if 0
 /* JK *************************************/
  error = TxV24Timeout;
/* This gives "No contact to ..... error! (Not always though) */
/*******************************************/
#endif

  return(error ? (SDITPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxSDITPGPattern                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit pattern and attribute to a SDI-TPG.            */
/*  Remarks:      The argument <attrib> is not specified for PT8602/03.      */
/*          The pattern MUST be valid when calling this function.      */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGPattern(UC ndx, UC pattern, UC attrib) {

  UC  error, HWtype = SDITPGUnit[ndx].HWType;

  FeedTheWatchdog;
                              // Lookup digital pattern code
  pattern = FindPatternTable(HWtype)[pattern];

  switch (HWtype) {
    case PT8602:
    case PT8603:
      error = SndInt(SDITPGUnit[ndx].Address, "SP", pattern);
      break;

    default:
      error = SndInt2(SDITPGUnit[ndx].Address, "SP", pattern, attrib);
      break;
  }

  return(error ? (SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITPGEDH                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit EDH on/off to a SDI-TPG.                  */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGEDH(UC ndx, UC state) {

  UC error;

  FeedTheWatchdog;

  error = SndChar(SDITPGUnit[ndx].Address, "SE", state);

  return(error ? (SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITPGTextStyle                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980224                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Transmit user text style to a SDI-TPG.                */
/*  Remarks:      This function is not specified for PT8602/03.          */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGTextStyle(UC ndx, UC pattern, UC style) {

  UC error, type = SDIFindPatternType(pattern);

  FeedTheWatchdog;

  error = SndChar(SDITPGUnit[ndx].Address, "SD", (style & 0xF0) + TPGStandardTextStyle);

  waitms(5);

  error = SndChar(SDITPGUnit[ndx].Address, "SD", style);

  if ((style & 0x0F)== TPGFreeTextStyle)
  {
    error = TrxSDITPGTextPos(ndx, style, 
                      SDITPGConfig[ndx].TextPos[type][0],
                      SDITPGConfig[ndx].TextPos[type][1]);
  }

  if (style & 0xF0)
  {
    switch (pattern)
    {
      case SDIPhilips4x3:
      case SDIPhilips16x9:
        if (SDITPGConfig[ndx].System == SDI525)
          error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 3, 0, 134);
        else
          error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 3, 0, 158);
        break;

      case SDIFuBK4x3:
      case SDIFuBK16x9:
        error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 3, 0, 268);
        break;

      default:
        if (SDITPGConfig[ndx].System == SDI525)
          error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 3, 0, 240);
        else
          error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 3, 0, 286);
        break;
    }
  }
  else
  {
    if (type == ComplexPattern)

      // If NO TIME/CLOCK information, ALWAYS disable text line 3.

      if (!(style & 0xF0))
        error = SndInt2(SDITPGUnit[ndx].Address, "SF", 3, 0);
  }

  return(error ? (SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITPGTextPos                                            UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 000508                                    */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Transmit user text position to a SDI-TPG.                   */
/*  Remarks:   This function is not specified for PT8602/03.               */
/*  Returns:   System error code, i.e. type incl. V24 specific error.      */
/*  Updates:   This function IS NOW specified for PT8603. (18.11.2006)     */
/***************************************************************************/
UC TrxSDITPGTextPos(UC ndx, UC style, UC Xpos, UC Ypos)
  {
  UC error, tmp;

  FeedTheWatchdog;

  if (SDITPGConfig[ndx].System == SDI625)
    tmp = 22*Ypos+8;
  else
    tmp = 18*Ypos+6;

  error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 1, 4+Xpos*2, tmp);

  if (!error)
    error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 2, 4+Xpos*2, tmp+18);

  if (!error && !(style & 0xF0))
    error = SndInt3(SDITPGUnit[ndx].Address, "SJ", 3, 4+Xpos*2, tmp+36);

  return(error ? (SDITPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxSDITPGTextLineEnable                           UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980224                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit text line enable to a SDI-TPG.              */
/*  Remarks:      The argument <no> is not specified in PT8602/03, i.e. there  */
/*          is only one text line to enable in PT8602/03.          */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGTextLineEnable(UC ndx, UC no, UC state) 
  {
  UC error;

  FeedTheWatchdog;

  switch (SDITPGUnit[ndx].HWType)
    {
    case PT8602:
      error = SndInt(SDITPGUnit[ndx].Address, "SF", state);
      break;

    case PT8603:
      error = SndInt2(SDITPGUnit[ndx].Address, "SF", 0xFF, state); // ??????**********
      break;

    default:
      error = SndInt2(SDITPGUnit[ndx].Address, "SF", no, state);
      break;
    }
  return(error ? (SDITPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxSDITPGText                                               UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980524                                                      */
/*                                                                         */
/*  Function:  Transmit user text to a SDI-TPG.                            */
/*  Remarks:   The argument <no> is not used in PT8602/03, i.e. there      */
/*             is only one text line in PT8602/03.  For the PT8602/03 the  */
/*             text-line will be converted to fixed 8 character width.     */
/*  Returns:    System error code, i.e. type incl. V24 specific error.     */
/*                                                                         */
/*  Updates:   18.11.2006: PT8603 was made compatible with PT5210, ie it   */
/*             has 3 lines of movable text.                                */
/***************************************************************************/
UC TrxSDITPGText(UC ndx, UC no, char* text)
  {
  register UC i;
  UC       error;
  char     tmp;

  FeedTheWatchdog;

  switch (SDITPGUnit[ndx].HWType)
    {
    case PT8602:
    //case PT8603:
      i = 0;
      while ((tmp = text[i]) && (i < 8))
        tmpBuffer[i++] = tmp;

      while (i < 8)
        tmpBuffer[i++] = ' ';

      tmpBuffer[8] = '\0';

      error = SndAsc(SDITPGUnit[ndx].Address, "SN", 0xFF, tmpBuffer);
      break;

    default: /* no is text line number here */
      error = SndAsc(SDITPGUnit[ndx].Address, "SN", no, text);
      break;
    }
  return(error ? (SDITPGErrorType + error) : 0);
  }

/***************************************************************************/
/*  TrxSDITPGAudioSignal                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit embedded audio signal to a SDI-TPG.            */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGAudioSignal(UC ndx, UC signal) {

  UC error;

  FeedTheWatchdog;

  error = SndChar(SDITPGUnit[ndx].Address, "SU", signal);

  return(error ? (SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITPGAudioLevel                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit embedded audio level to a SDI-TPG.             */
/*  Remarks:    The audio level MUST be valid when calling this function.  */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGAudioLevel(UC ndx, UC level) {

  UC error, HWtype = SDITPGUnit[ndx].HWType;

  FeedTheWatchdog;

  level = FindAudioLevelTable(HWtype)[level];

  error = SndChar(SDITPGUnit[ndx].Address, "SL", level);

  return(error ? (SDITPGErrorType + error) : 0);
}

/***************************************************************************/
/*  TrxSDITPGAudioGroup                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Transmit embedded audio group to a SDI-TPG.             */
/*  Remarks:      This function is only used for PT8633.                */
/*  Returns:    System error code, i.e. type incl. V24 specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxSDITPGAudioGroup(UC ndx, UC group) {

  UC error;

  FeedTheWatchdog;

  error = SndChar(SDITPGUnit[ndx].Address, "SC", group);

  return(error ? (SDITPGErrorType + error) : 0);
}

/****************************         HDTPG        *************************/




/***************************************************************************/
/*  TrxTimeClockControl                             UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980708                          */
/* Revised:    980722                                       */
/*                                                  */
/*  Function:  Transmit control information to the Time Clock Interface.  */
/*  Remarks:      --                                        */
/*  Returns:    System error code, i.e. type incl. IIC specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxTimeClockControl(UC control) {

  FeedTheWatchdog;

  IIC1Write(time1_addr, control);

  return(IIC1ErrorCode ? (TimeClockErrorType + IIC1ErrorCode) : 0);
}

/***************************************************************************/
/*  TrxTimeClockData                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980708                          */
/* Revised:    980722                                       */
/*                                                  */
/*  Function:  Transmit data information to the Time Clock Interface.    */
/*  Remarks:      No of bytes should be restricted to 4.                */
/*  Returns:    System error code, i.e. type incl. IIC specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC TrxTimeClockData(UC TCIControl, UC noOfBytes, UL val) {

  UC i, tmp[5];

  FeedTheWatchdog;

  tmp[0] = TCIControl;

  for (i = 1; i <= noOfBytes; i++) {
    tmp[i] = val & 0x000000FF;
    val >>= 8;
  }

  IIC1WriteN(time1_addr, noOfBytes+1, tmp);

  FeedTheWatchdog;

  return(IIC1ErrorCode ? (TimeClockErrorType + IIC1ErrorCode) : 0);
}

/***************************************************************************/
/*  TrxGenlockDelay                                             UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980524                                                      */
/*                                                                         */
/*  Function:  Transmit genlock offset to ALL of the installed units.      */
/*  Remarks:   This function will try to transmit the offset to ALL units  */
/*             even if one or more units fails.                            */
/*  Returns:    System error code, i.e. type incl. V24 specific error.     */
/*  Updates:    --                                                         */
/*             12.03.2004: Sending genlock delay to TLG board added,       */
/*             using SendTLGDelays() function                              */
/*             12.11.2006: Sending genlock delay to HD and DL boards added *.
/***************************************************************************/
UC TrxGenlockDelay(UL delay)
  {
  register UC i;
  UC error, errorCnt = 0, lastError = 0;

  FeedTheWatchdog;

  for (i = 0; i < MaxAnlBlkUnits; i++)
    if (AnlBlkUnit[i].Present)
      {
      if (i % 2)
        error = SndLong(AnlBlkUnit[i].Address, "JO", delay);
      else
        error = SndLong(AnlBlkUnit[i].Address, "HO", delay);

      if (error)
        {
        lastError = AnlBlkErrorType + error;
        errorCnt++;
        }
      }
  FeedTheWatchdog;

/*********************************************************************/
/*   Recalc and send new delays to all present TLG, HD and DL boards */
/*   These boards DO NOT accept genlock delay like other boards.     */
/*   Therefore, on genlock timing change, we add genlock delay to    */
/*   TLG, HD and DL delays and transmit resulting delays to these    */
/*   boards.                                                         */
/*********************************************************************/

  for (i = 0; i < MaxTLGUnits; i++)
    {
    if (TLGUnit[i].Present)
      {
      FeedTheWatchdog;
      error = TrxTLGDelay(i, TLGConfig[i].Format,
                             TLGConfig[i].Delay,
                             delay);
      if (error)
        {
        lastError = DLTPGErrorType + error;
        errorCnt++;
        }
      }
    }

  FeedTheWatchdog;

  for (i = 0; i < MaxHDTPGUnits; i++)
    if (HDTPGUnit[i].Present) 
      {
      FeedTheWatchdog;
      error = TrxHDTPGSystem(i, 
                             HDTPGConfig[i].System,
                             HDTPGConfig[i].Delay,
                             delay);
      if (error)
        {
        lastError = HDTPGErrorType + error;
        errorCnt++;
        }
      }

  FeedTheWatchdog;

  for (i = 0; i < MaxDLTPGUnits; i++)
    if (DLTPGUnit[i].Present) 
      {
      FeedTheWatchdog;
      error = TrxDLTPGSystem(i, 
                             DLTPGConfig[i].System,
                             DLTPGConfig[i].Delay,
                             delay);
      if (error)
        {
        lastError = DLTPGErrorType + error;
        errorCnt++;
        }
      }

/*********************************************************************/
  FeedTheWatchdog;

  for (i = 0; i < MaxAnlTPGUnits; i++)
    if (AnlTPGUnit[i].Present) 
      {
      if (error = SndLong(AnlTPGUnit[i].Address, "AO", delay))
        {
        lastError = AnlTPGErrorType + error;
        errorCnt++;
        }
      }

  FeedTheWatchdog;

  for (i = 0; i < MaxSDITSGUnits; i++)
    if (SDITSGUnit[i].Present)
      {
      if (error = SndLong(SDITSGUnit[i].Address, "HO", delay))
        {
        lastError = SDITSGErrorType + error;
        errorCnt++;
        }
      }

  FeedTheWatchdog;

  for (i = 0; i < MaxSDITPGUnits; i++)
    if (SDITPGUnit[i].Present)
      {
      if (error = SndLong(SDITPGUnit[i].Address, "SO", delay))
        {
        lastError = SDITPGErrorType + error;
        errorCnt++;
        }
      }

  FeedTheWatchdog;

  if (errorCnt > 1)                   // Multiple Errors detected
    return(SystemErrorType + MultipleError);

  return(lastError);
  }

/***************************************************************************/
/* ConfigurePreset                                              UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980524                                                      */
/*                                                                         */
/*  Function:  Configure the PT5230 with a complete preset setup.          */
/*  Remarks:   Part of PresetRecallSelect() function                       */
/*  Returns:    TBD                                                        */
/*  Updates:    --                                                         */
/***************************************************************************/
UC ConfigurePreset(UC no)
  {
  register UC i;
  UC last, error, errorCnt = 0, lastError = 0;

  /******  Tackle DLTPGs   ******************/
  for (i = 0; i < MaxDLTPGUnits; i++)
    memcpy(&DLTPGConfig[i], &Preset[no].DLTPGStore[i], sizeof(DLTPGConfig[0]));
  
  if (DLTPGUnit[0].Present)
    {
    ConfigureDLTPGUnit(DLTPG1Item);
    ConfigureDLTPGUnit(DLTPG2Item);
    }
  if (HDTPGUnit[2].Present)
    {
    ConfigureDLTPGUnit(DLTPG3Item);
    ConfigureDLTPGUnit(DLTPG4Item);
    }
  if (HDTPGUnit[4].Present)
    {
    ConfigureDLTPGUnit(DLTPG5Item);
    ConfigureDLTPGUnit(DLTPG6Item);
    }



  /******  Tackle HDTPGs   ******************/
  for (i = 0; i < MaxHDTPGUnits; i++)
    memcpy(&HDTPGConfig[i], &Preset[no].HDTPGStore[i], sizeof(HDTPGConfig[0]));
  
  if (HDTPGUnit[0].Present)
    {
    ConfigureHDTPGUnit(HDTPG1Item); // settings to HDTPG1....HDTPGUnit[HDTPG1Item].Present = TRUE;
    ConfigureHDTPGUnit(HDTPG2Item);
    ConfigureHDTPGUnit(HDTPG3Item);
    ConfigureHDTPGUnit(HDTPG4Item);
    }
  if (HDTPGUnit[4].Present)
    {
    ConfigureHDTPGUnit(HDTPG5Item);
    ConfigureHDTPGUnit(HDTPG6Item);
    ConfigureHDTPGUnit(HDTPG7Item);
    ConfigureHDTPGUnit(HDTPG8Item);
    }

  if (HDTPGUnit[8].Present)
    {
    ConfigureHDTPGUnit(HDTPG9Item);
    ConfigureHDTPGUnit(HDTPG10Item);
    ConfigureHDTPGUnit(HDTPG11Item);
    ConfigureHDTPGUnit(HDTPG12Item);
    }


  /******  Tackle TLGs   ******************/
  for (i = 0; i < MaxTLGUnits; i++)
    memcpy(&TLGConfig[i], &Preset[no].TLGStore[i], sizeof(TLGConfig[0]));
  
  if (TLGUnit[0].Present)
    {
    ConfigureTLGUnit(TLG1Item);
    ConfigureTLGUnit(TLG2Item);
    ConfigureTLGUnit(TLG3Item);
    ConfigureTLGUnit(TLG4Item);
    }

  if (TLGUnit[4].Present)
    {
    ConfigureTLGUnit(TLG5Item);
    ConfigureTLGUnit(TLG6Item);
    ConfigureTLGUnit(TLG7Item);
    ConfigureTLGUnit(TLG8Item);
    }

  if (TLGUnit[8].Present)
    {
    ConfigureTLGUnit(TLG9Item);
    ConfigureTLGUnit(TLG10Item);
    ConfigureTLGUnit(TLG11Item);
    ConfigureTLGUnit(TLG12Item);
    }

  for (i = 0; i < MaxAnlBlkUnits; i++)
    {
    memcpy(&AnlBlkConfig[i], &Preset[no].AnlBlkStore[i], sizeof(AnlBlkConfig[0]));

    if (AnlBlkUnit[i].Present)
      if (error = ConfigureAnlBlkUnit(i))
        {
        lastError = error;
        errorCnt++;
        }
    }

  FeedTheWatchdog;

  for (i = 0; i < MaxAnlTPGUnits; i++) {
    memcpy(&AnlTPGConfig[i], &Preset[no].AnlTPGStore[i], sizeof(AnlTPGConfig[0]));

    if (AnlTPGUnit[i].Present)
      if (error = ConfigureAnlTPGUnit(i)) {
        lastError = error;
        errorCnt++;
      }
  }

  FeedTheWatchdog;

  for (i = 0; i < MaxSDITSGUnits; i++) {
    memcpy(&SDITSGConfig[i], &Preset[no].SDITSGStore[i], sizeof(SDITSGConfig[0]));

    if (SDITSGUnit[i].Present)
      if (error = ConfigureSDITSGUnit(i)) {
        lastError = error;
        errorCnt++;
      }
  }

  FeedTheWatchdog;

  for (i = 0; i < MaxSDITPGUnits; i++) {
    memcpy(&SDITPGConfig[i], &Preset[no].SDITPGStore[i], sizeof(SDITPGConfig[0]));

    if (SDITPGUnit[i].Present)
      if (error = ConfigureSDITPGUnit(i)) {
        lastError = error;
        errorCnt++;
      }
  }

  FeedTheWatchdog;

  for (i = 0; i < MaxAESEBUUnits; i++)
    memcpy(&AESEBUConfig[i], &Preset[no].AESEBUStore[i], sizeof(AESEBUConfig[0]));

  if (AESEBUUnit.HWType == PT8635)
    last = AESEBU2Item;
  else
    last = AESEBU1Item;

  if (AESEBUUnit.Present) {
    for (i = AESEBU1Item; i <= last; i++)
      if (error = AESEBUUnitDrv(i, AESEBUConfig[i].Signal, AESEBUConfig[i].Level, AESEBUConfig[i].Timing)) {
        lastError = error;
        errorCnt++;
      }
  }

  FeedTheWatchdog;

  for (i = 0; i < MaxGenlockUnits; i++)
    memcpy(&GenlockConfig[i], &Preset[no].GenlockStore[i], sizeof(GenlockConfig[0]));

  ActiveGenlockNo = Preset[no].ActiveGenlockNo;
  ExternalGenlockNo = Preset[no].ExternalGenlockNo;

  if (error = GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System)) {
    lastError = error;
    errorCnt++;
  }  
  FeedTheWatchdog;
  
  for (i = 0; i < MaxLTCUnits; i++)
    memcpy(&LTCConfig[i], &Preset[no].LTCStore[i], sizeof(LTCConfig[0]));
  
  ConfigureLTCUnit();

  if (errorCnt > 1)                 // Multiple Errors detected
    return(SystemErrorType + MultipleError);

  return(lastError);
}

/***************************************************************************/
/*  GetV24Version                                               UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    990304                                                      */
/*                                                                         */
/*  Function:  Get the SW version and KU number from a RS232 unit.         */
/*  Remarks:    Not all of the PT86XX units returns the HW type & version. */
/*          900-versions is NOT recognized UNLESS bit 6&7 in the           */
/*          hardware information is set, indicating that customized        */
/*          patterns are available.                                        */
/*  Returns:    TRUE: The unit exists and is valid in this master.         */
/*  Updates:    --                                                         */
/*              12.03.2004: Tackling of TLG board added.                   */
/*      KU number is read by master as string as follows:                  */
/*      ascPtr = RecStr(address, "HR", ProductKUNo_addr/10, &error);       */
/*      where ProductKUNo_addr/10 = 2.                                     */
/*      KU number is stored as user string nr. 2.                          */
/*                                                                         */
/*  Args:   UC ndx -                                                       */
/*                                                                         */
/*  Notes:  TLGUnit[0].Present means that one TLG board is installed and   */
/*          configured by ConfigureTLGUnit()                               */
/*          TLGUnit[0].Present and TLGUnit[4].Present: two TLG boards are  */
/*          already installed... and configured by ConfigureTLGUnit() etc. */
/*       if (GetV24Version(DLTPG3Item, conn_addr, PT8642) == 0)            */
/***************************************************************************/
UC GetV24Version(UC ndx, UC address, UC HWtype)
  {
  char* ascPtr;
  UL KUNumber;
  UI tmp;
  UC error;

  FeedTheWatchdog;

  switch (HWtype) /* ProductKUNo_addr = 20 */
    {
    case PT8642:
      DLTPGUnit[ndx].Address = address; // address = bbux_addr
      tmp = RecInt(address, "VE", 0xFF, &error); // Get the hardware version (fast coded)
      DLTPGUnit[ndx].HWVersion = tmp & 0xFF;
      tmp = RecInt(address, "VD", 0xFF, &error); // Get the software version (fast coded)
      DLTPGUnit[ndx].SWVersion = (UC) tmp;
      tmp = RecInt(address, "VF", 0xFF, &error); // Get the hardware information (fast coded)
      DLTPGUnit[ndx].HWType = tmp & 0xFF;
      ascPtr = RecStr(address, "VC", ProductKUNo_addr / 10, &error);
      if (!error)
        KUNumber = atol(&ascPtr[0]);
      else
        KUNumber = 0;
      DLTPGUnit[ndx].KUNumber = KUNumber;
      DLTPGUnit[ndx].Present  = 1;
      break;

    case PT8641:
      HDTPGUnit[ndx].Address = address; // address = bbux_addr
      tmp = RecInt(address, "UE", 0xFF, &error); // Get the hardware version (fast coded)
      HDTPGUnit[ndx].HWVersion = tmp & 0xFF;
      tmp = RecInt(address, "UD", 0xFF, &error); // Get the software version (fast coded)
      HDTPGUnit[ndx].SWVersion = (UC) tmp;
      tmp = RecInt(address, "UF", 0xFF, &error); // Get the hardware information (fast coded)
      HDTPGUnit[ndx].HWType = tmp & 0xFF;
      
      // This is what causes temperature error, and is not needed
      /*ascPtr = RecStr(address, "UC", ProductKUNo_addr / 10, &error);
      if (!error)
        KUNumber = atol(&ascPtr[0]);
      else*/
      
      KUNumber = 0;
      HDTPGUnit[ndx].KUNumber = KUNumber;
      HDTPGUnit[ndx].Present  = 1;
      break;
    case PT8640:
      TLGUnit[ndx].Address = address; // address = bbux_addr
      tmp = RecInt(address, "TH", 0xFF, &error); // Get the hardware version (fast coded)
      TLGUnit[ndx].HWVersion = tmp & 0xFF;
      tmp = RecInt(address, "TI", 0xFF, &error); // Get the software version (fast coded)
      TLGUnit[ndx].SWVersion = (UC) tmp;
      tmp = RecInt(address, "TT", 0xFF, &error); // Get the hardware information (fast coded)
      TLGUnit[ndx].HWType = tmp & 0xFF;
      ascPtr = RecStr(address, "TR", ProductKUNo_addr / 10, &error);
      if (!error)
        KUNumber = atol(&ascPtr[0]);
      else
        KUNumber = 0;
      TLGUnit[ndx].KUNumber = KUNumber;
      TLGUnit[ndx].Present  = 1;
      break;

    case PT8608:                // Black Burst generator
      //***************************************************************
      //                   PT8608
      // HW type:             -
      // HW version:          -
      // HW user pattern:     -
      // HW information :     -
      // KU Number:           +
      // SW version:          +
      //***************************************************************
      AnlBlkUnit[ndx].Address = address;

      AnlBlkUnit[ndx].HWType = HWtype;  // Set the hardware type
      AnlBlkUnit[ndx].HWVersion = 0;  // Reset the hardware version
                            // Get the software version
      tmp = RecInt(address, "HI", 0xFF, &error);
      if (error)
        tmp = 0;
      AnlBlkUnit[ndx].SWVersion = (UC) tmp;
      AnlBlkUnit[ndx].HWCustomPattern = 0;  // Reset the hardware information
      AnlBlkUnit[ndx].HWInformation = 0;
                            // Get the KU number
      ascPtr = RecStr(address, "HR", ProductKUNo_addr/10, &error);
      if (!error && (strlen(ascPtr) == 8))
        KUNumber = atol(&ascPtr[2]);
      else
        KUNumber = 0;
      AnlBlkUnit[ndx].KUNumber = KUNumber;
      error = 0;
      break;
    case PT8601:                // Analog-Signal Generator (PT5210)
    case PT8631:                // Analog-TPG
      //***************************************************************
      //                    PT8601        PT8631
      // HW type/version:     +              +
      // HW user pattern:     0 (>=2.5)      0
      // HW information:      0 (>=2.5)      0
      // KU Number:           +              +
      // SW version:          +              +
      //***************************************************************
      AnlTPGUnit[ndx].Address = address;
                            // Get the hardware type/version
      tmp = RecInt(address, "AV", 0xFF, &error);
      if (error)
        tmp = HWtype;
      AnlTPGUnit[ndx].HWType = tmp & 0xFF;
      AnlTPGUnit[ndx].HWVersion = tmp >> 8;
                            // Get the software version
      tmp = RecInt(address, "AI", 0xFF, &error);
      if (error)
        tmp = 0;
      AnlTPGUnit[ndx].SWVersion = (UC) tmp;
                            // Get the hardware information
      if ((AnlTPGUnit[ndx].HWType == PT8601) && (AnlTPGUnit[ndx].SWVersion < 30))
        tmp = 0;
      else
        {
        tmp = RecInt(address, "AT", 0xFF, &error);
        if (error)
          tmp = 0;
        }
      AnlTPGUnit[ndx].HWCustomPattern = (UC) tmp;
      AnlTPGUnit[ndx].HWInformation = 0;
                            // Get the KU number
      ascPtr = RecStr(address, "AR", ProductKUNo_addr/10, &error);
      if (!error && (strlen(ascPtr) == 8))
        KUNumber = atol(&ascPtr[2]);
      else
        KUNumber = 0;
      AnlTPGUnit[ndx].KUNumber = KUNumber;
      error = 0;
                          // If /900 version & NO custom patterns
      if (AnlTPGUnit[ndx].HWVersion)
        if (!(AnlTPGUnit[ndx].HWCustomPattern & 0x88))
          error = SystemErrorType + ConfigurationError;
      break;
    case PT8602:                // SDI Signal Generator 1 (PT5210)
    case PT8603:                // SDI Signal Generator 2 (PT5210)
    case PT8632:                // SDI-TPG 1
    case PT8633:                // SDI-TPG 2
      //***************************************************************
      //                      PT8602/3      PT8632/3
      // HW type/version:        +              +
      // HW user patterm:     0 (>=4.0)      0 (>1.0)
      // HW information:      0 (>=4.0)      0 (>1.0)
      // KU Number:              +              +
      // SW version:             +              +
      //***************************************************************
      SDITPGUnit[ndx].Address = address;
                            // Get the hardware type/version
      tmp = RecInt(address, "SV", 0xFF, &error);
      if (error)
        tmp = HWtype;
      SDITPGUnit[ndx].HWType = tmp & 0xFF;
      SDITPGUnit[ndx].HWVersion = tmp >> 8;
                            // Get the software version
      tmp = RecInt(address, "SI", 0xFF, &error);
      if (error)
        tmp = 0;
      SDITPGUnit[ndx].SWVersion = (UC) tmp;
                            // Get the hardware information
      if ((SDITPGUnit[ndx].HWType < PT8632) && (SDITPGUnit[ndx].SWVersion < 40))
        tmp = 0;
      else
        {
        if (SDITPGUnit[ndx].SWVersion <= 10)
          tmp = 0;
        else
          {
          tmp = RecInt(address, "ST", 0xFF, &error);
          if (error)
            tmp = 0;
          }
        }
      SDITPGUnit[ndx].HWCustomPattern = (UC) tmp;
      if (SDITPGUnit[ndx].HWType == PT8632)
        tmp >>= 8;
      else
        tmp = 0;
      SDITPGUnit[ndx].HWInformation = (UC) tmp;
                            // Get the KU number
      ascPtr = RecStr(address, "SR", ProductKUNo_addr/10, &error);
      if (!error && (strlen(ascPtr) == 8))
        KUNumber = atol(&ascPtr[2]);
      else
        KUNumber = 0;
      SDITPGUnit[ndx].KUNumber = KUNumber;
      error = 0;
                          // If /900 version & NO custom patterns
      if (SDITPGUnit[ndx].HWVersion)
        if (!(SDITPGUnit[ndx].HWCustomPattern & 0x88))
          error = SystemErrorType + ConfigurationError;
      break;
    case PT8609:                // SDI-Black/Colorbar (PT5210)
    case PT8639:                // SDI-TSG
      //***************************************************************
      //                     PT8609       PT8639
      // HW type/version:      -             +
      // HW information 0:     0 (>=3.0)     0
      // HW information 1:     0 (>=3.0)     0
      // KU Number:            +             +
      // SW version:           +             +
      //***************************************************************
      SDITSGUnit[ndx].Address = address;
                            // Get the hardware type/version
      if (HWtype == PT8609)
        tmp = HWtype;
      else
        {
        tmp = RecInt(address, "HV", 0xFF, &error);
        if (error)
          tmp = HWtype;
        }
      SDITSGUnit[ndx].HWType = tmp & 0xFF;
      SDITSGUnit[ndx].HWVersion = tmp >> 8;
                            // Get the software version
      tmp = RecInt(address, "HI", 0xFF, &error);
      if (error)
        tmp = 0;
      SDITSGUnit[ndx].SWVersion = (UC) tmp;
                            // Get the hardware information
      if ((SDITSGUnit[ndx].HWType == PT8609)  && (SDITSGUnit[ndx].SWVersion < 30))
        tmp = 0;
      else
        {
        tmp = RecInt(address, "HT", 0xFF, &error);
        if (error)
          tmp = 0;
        }
      SDITSGUnit[ndx].HWCustomPattern = (UC) tmp;
      SDITSGUnit[ndx].HWInformation = 0;
                            // Get the KU number
      ascPtr = RecStr(address, "HR", ProductKUNo_addr/10, &error);
      if (!error && (strlen(ascPtr) == 8))
        KUNumber = atol(&ascPtr[2]);
      else
        KUNumber = 0;
      SDITSGUnit[ndx].KUNumber = KUNumber;
      error = 0;
                          // If /900 version & NO custom patterns
      if (SDITSGUnit[ndx].HWVersion)
        if (!(SDITSGUnit[ndx].HWCustomPattern & 0x88))
          error = SystemErrorType + ConfigurationError;
      break;
    } // switch HWType

  if (error)
    {
    DisplayErrorLine(error, ErrorDisplayTimeout);
    }
  return(error);
  }

/***************************************************************************/
/*  GetIICVersion                                               UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980524                                                      */
/*                                                                         */
/*  Function:  Get the KU number from an IIC unit.                         */
/*  Remarks:    IIC2RdStr ALWAYS reads 10+1 characters, hence the string   */
/*              to hold the characters MUST be at least 11 characters      */
/*  Returns:    An IIC error code.  KU is filled with KU number.           */
/*  Updates:    --                                                         */
// GetIICVersion(IICBus1, d1ram_addr, ProductKUNo_addr, &SDIGenlockUnit.KUNumber);
/***************************************************************************/
UC GetIICVersion(UC bus, UC address, UC KU_address, UL* KU)
  {
  UC error;

  if (bus == IICBus2)
    {
    IIC2RdStr(address, KU_address, tmpBuffer);
    error = IIC2ErrorCode;
    }
  else
    {
    IIC1RdStr(address, KU_address, tmpBuffer);
    error = IIC1ErrorCode;
    }

    
  if (error)
    *KU = 0;
  else
    {
    tmpBuffer[8] = 0;
    *KU = atol(&tmpBuffer[2]);
    }
  return(error);
  }

/***************************************************************************/
/*  TestMainboard                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the main board.                            */
/*  Remarks:    This is mainly a test of the mainboard EEPROM.          */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestMainboard(void) {

  FeedTheWatchdog;
                              // Test power calibration values
  if(!IIC2Write(mainram1_addr, VoltCalib_addr))
    return(1);

  if ((UC) VoltCalib - IIC2Read(mainram1_addr))
    return(1);

  if (!IIC2ErrorCode)
    if (VoltN5Min - IIC2Read(mainram1_addr))
      return(1);

  if (!IIC2ErrorCode)
    if (VoltN5Max - IIC2Read(mainram1_addr))
      return(1);

  if (!IIC2ErrorCode)
    if (Volt12Min - IIC2Read(mainram1_addr))
      return(1);

  if (!IIC2ErrorCode)
    if (Volt12Max - IIC2Read(mainram1_addr))
      return(1);

  IIC1Read(ttl_addr);            // Read the parallel remote IIC port

  if (IIC1ErrorCode || IIC2ErrorCode)
    return(1);

  return(0);
}

/***************************************************************************/
/*  TestSPGUnit                                   UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the SPG unit.                            */
/*  Remarks:    --                                        */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestSPGUnit(void)
  {
  UC error, tmp;

  FeedTheWatchdog;

  tmp = (UC) (IIC2RdLong(spgram_addr, HPHZero_G_addr) - RecLong(sync_addr, "GA", 0xFF, &error));
  if (error || tmp)
    return(1);

  tmp = (UC) (IIC2RdInt(spgram_addr, GenlPhase_G_addr) - RecInt(sync_addr, "GF", 0xFF, &error));
  if (error || tmp)
    return(1);

  tmp = (UC) (IIC2RdLong(spgram_addr, HPHZero_M_addr) - RecLong(sync_addr, "GU", 0xFF, &error));
  if (error || tmp)
    return(1);

  tmp = (UC) (IIC2RdInt(spgram_addr, GenlPhase_M_addr) - RecInt(sync_addr, "GG", 0xFF, &error));
  if (error || tmp)
    return(1);

  return(0);
  }

/***************************************************************************/
/*  TestAESEBUUnit                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    991026, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the PT8605/35 AES/EBU Audio generator            */
/*  Remarks:    --                                        */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestAESEBUUnit() {

  register UC i;
  UC tmp;
  UI port[2];

  FeedTheWatchdog;

  for (i = 0; i < 2; i++) {
    tmp = AESEBUConfig[i].Signal;

    port[i] = AESEBUSignalTable[tmp] + 0xF880;

    if (tmp != AESEBUWordClock)
      port[i] |= AESEBULevelTable[AESEBUConfig[i].Level] +
            AESEBUTimingTable[AESEBUConfig[i].Timing];
  }
                      // Compare level/signal for AES_EBU1
  if (((port[0] & 0x00FF) - IIC2Read(sound1_addr)) & 0x7F)
    return(1);

                      // Compare timing for AES_EBU1
  if (!IIC2ErrorCode)
    if (((port[0] >> 8) - IIC2Read(sound2_addr)) & 0x07)
      return(1);

  if (!IIC2ErrorCode)
    if (AESEBUUnit.HWType == PT8635) {

                      // Compare level/signal for AES_EBU2
      if (((port[1] & 0x00FF) - IIC2Read(sound3_addr)) & 0x7F)
        return(1);

                      // Compare timing for AES_EBU2
      if (!IIC2ErrorCode)
        if (((port[1] >> 8) - IIC2Read(sound4_addr)) & 0x07)
          return(1);
    }

  return(IIC2ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*  TestAnlBlkUnit                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 971023                          */
/* Revised:    980524, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the PT8608 Black Burst generator.                */
/*  Remarks:    --                                        */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestAnlBlkUnit(UC ndx) {

  UC tmp, error, addr;
  float ScHPhase;

  FeedTheWatchdog;

  addr = AnlBlkUnit[ndx].Address;

                        // Examine status byte
  tmp = (UC) RecInt(addr, "HS", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test ScHPhase in BBn
  ScHPhase = (RecInt(addr, "HH", 0xFF, &error) * 360.)/2048.;

  (ScHPhase > 180.) ? (ScHPhase -= 360.) : 0;

  if ((fabs(ScHPhase - (float) AnlBlkConfig[ndx].ScHPhase) > 0.5) || error)
    return(1);

                        // Test ScHPhase in BBn+1
  ScHPhase = (RecInt(addr, "JH", 0xFF, &error) * 360.)/2048.;

  (ScHPhase > 180.) ? (ScHPhase -= 360.) : 0;

  if ((fabs(ScHPhase - (float) AnlBlkConfig[ndx+1].ScHPhase) > 0.5) || error)
    return(1);
                        // Test system in BBn
                        // Not implemented in ver 2.2 and earlier
  tmp = AnlBlkConfig[ndx].System - (UC) RecInt(addr, "HQ", 0xFF, &error);
  if (tmp || error)
    if (error != RxV24Timeout)
      return(1);
                        // Test system in BBn+1
                        // Not implemented in ver 2.2 and earlier
  tmp = AnlBlkConfig[ndx+1].System - (UC) RecInt(addr, "JQ", 0xFF, &error);
  if (tmp || error)
    if (error != RxV24Timeout)
      return(1);
                        // Test delay in BBn
                        // Not implemented in ver 2.2 and earlier
  tmp = (UC) (AnlBlkConfig[ndx].Delay - RecLong(addr, "HB", 0xFF, &error));
  if (tmp || error)
    if (error != RxV24Timeout)
      return(1);
                        // Test delay in BBn+1
                        // Not implemented in ver 2.2 and earlier
  tmp = (UC) (AnlBlkConfig[ndx+1].Delay - RecLong(addr, "JB", 0xFF, &error));
  if (tmp || error)
    if (error != RxV24Timeout)
      return(1);

  return(0);
}

/***************************************************************************/
/*  TestAnlTPGUnit                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Test the PT8601/PT8631 Analog-TPGs.                  */
/*  Remarks:    Will test both PT8601 and PT8631                    */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestAnlTPGUnit(UC ndx) {

  UC error, tmp, HWtype, addr;
  float ScHPhase;

  FeedTheWatchdog;

  HWtype = AnlTPGUnit[ndx].HWType;
  addr = AnlTPGUnit[ndx].Address;

                        // Examine status byte
  tmp = (UC) RecInt(addr, "AS", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test system
  tmp = AnlTPGConfig[ndx].System - (UC) RecInt(addr, "AQ", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Lookup analog pattern code
  tmp = FindPatternTable(HWtype)[AnlTPGConfig[ndx].Pattern];

                        // Test pattern
  tmp -= (UC) RecInt(addr, "AP", 0xFF, &error);
  if (tmp || error)
    return(1);
                        // Test text enable
//  if (AnalogConfig.TextOn - (UC) RecInt(AnlSig_addr, "AF", 0xFF, &error))
//    return(7);

                        // Test text information
//  if (strcmp(AnalogConfig.TextInfo, RecStr(AnlSig_addr, "AN", 0xFF, &error)))
//    return(8);

                        // Test ScHPhase
  ScHPhase = (RecInt(addr, "AH", 0xFF, &error) * 360.)/2048.;

  (ScHPhase > 180.) ? (ScHPhase -= 360.) : 0;

 // unit_drv.c: 3474: implicit conversion of float to integer (warning)
 // JK change: fabs((float)ScHPhase.....
  if ((fabs((float)ScHPhase - (float) AnlTPGConfig[ndx].ScHPhase) > 0.5) || error)
    return(1);

                        // Test delay
  tmp = (UC) (AnlTPGConfig[ndx].Delay - RecLong(addr, "AB", 0xFF, &error));
  if (tmp || error)
    return(1);

  return(0);
}

/***************************************************************************/
/*  TestSDITSGUnit                                 UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Test the PT8609/39 SDI-TSGs.                      */
/*  Remarks:    --                                        */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestSDITSGUnit(UC ndx) {

  UC error, tmp, HWtype, addr;

  FeedTheWatchdog;

  HWtype = SDITSGUnit[ndx].HWType;
  addr = SDITSGUnit[ndx].Address;

                        // Examine status byte
  tmp = (UC) RecInt(addr, "HS", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test system, (only for PT8639)
  if (HWtype == PT8639) {
    tmp = SDITSGConfig[ndx].System - (UC) RecInt(addr, "HQ", 0xFF, &error);
    if (tmp || error)
      return(1);
  }
                        // Lookup digital pattern code
  tmp = FindPatternTable(HWtype)[SDITSGConfig[ndx].Pattern];

                        // Test pattern
  tmp -= (UC) RecInt(addr, "HP", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test EDH insertion
  tmp = SDITSGConfig[ndx].EDH - (UC) RecInt(addr, "HE", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test audio signal
  tmp = SDITSGConfig[ndx].AudioSignal - (UC) RecInt(addr, "HU", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test audio level, (only for PT8639)
  if (HWtype == PT8639) {
    tmp = SDITSGConfig[ndx].AudioLevel - (UC) RecInt(addr, "HL", 0xFF, &error);
    if (tmp || error)
      return(1);
  }

                        // Test delay
  tmp = (UC) (SDITSGConfig[ndx].Delay - RecLong(addr, "HB", 0xFF, &error));
  if (tmp || error)
    return(1);

  return(0);
}

/***************************************************************************/
/*  TestSDITPGUnit                                              UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980524                                                      */
/*                                                                         */
/*  Function:  Test the  PT8602/03/32/33 SDI-TPGs.                         */
/*  Remarks:    Will test both PT8602/03 and PT8632/33                     */
/*  Returns:    0: OK, 1: FAIL                                             */
/*  Updates:    --                                                         */
/***************************************************************************/
UC TestSDITPGUnit(UC ndx)
  {
  UC error, tmp, HWtype, addr;

  FeedTheWatchdog;

  HWtype = SDITPGUnit[ndx].HWType;
  addr = SDITPGUnit[ndx].Address;

                        // Examine status byte (SS = read status)
  tmp = (UC) RecInt(addr, "SS", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test system         (SQ = read system)
  tmp = SDITPGConfig[ndx].System - (UC) RecInt(addr, "SQ", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Lookup digital pattern code
  tmp = FindPatternTable(HWtype)[SDITPGConfig[ndx].Pattern];

                        // Test pattern        (SP = read pattern)
  tmp -= (UC) RecInt(addr, "SP", 0xFF, &error);
  if (tmp || error)
    return(1);

#if 0
                        // Test text enable
//  tmp = SDITPGConfig[ndx].TextOn - (UC) RecInt(addr, "SF", 0xFF, &error);
//  if (tmp || error)
//    return(1);

                        // Test text information
//  tmp = strcmp(SDITPGConfig[ndx].TextInfo, RecStr(addr, "SN", 0xFF, &error)))
//  if (tmp || error)
//    return(1);
#endif
                        // Test EDH insertion  (SE = read EDH)
  tmp = SDITPGConfig[ndx].EDH - (UC) RecInt(addr, "SE", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test audio signal
  tmp = SDITPGConfig[ndx].AudioSignal - (UC) RecInt(addr, "SU", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Lookup audio level code
  tmp = FindAudioLevelTable(HWtype)[SDITPGConfig[ndx].AudioLevel];

                        // Test audio level
  tmp -= (UC) RecInt(addr, "SL", 0xFF, &error);
  if (tmp || error)
    return(1);

                        // Test audio group, (only for PT8633)
  if (HWtype == PT8633)
    {
    tmp = SDITPGConfig[ndx].AudioGroup - (UC) RecInt(addr, "SC", 0xFF, &error);
    if (tmp || error)
      return(1);
    }

                        // Test delay
  tmp = (UC) (SDITPGConfig[ndx].Delay - RecLong(addr, "SB", 0xFF, &error));
  if (tmp || error)
    return(1);
  return(0);
  }

/***************************************************************************/
/*  TestBBMulUnit                                  UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the PT8604 Multiple Parallel Black Burst Option      */
/*  Remarks:    --                                        */
/*  Returns:    0: OK, 1: FAIL                                */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestBBMulUnit(void) {

  FeedTheWatchdog;

  IIC1RdStr(multibb_addr, ProductKUNo_addr, tmpBuffer);

  return(IIC1ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*  TestSDIGenlockUnit                                          UNIT_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980122                                    */
/* Revised:    980524, KEn, DEV                                            */
/*                                                                         */
/*  Function:  Test the PT8606 SDI Digital Genlock                         */
/*  Remarks:    --                                                         */
/*  Returns:    errorcode                                                  */
/*  Updates:    --                                                         */
/***************************************************************************/
UC TestSDIGenlockUnit(void) {

  FeedTheWatchdog;

  IIC1RdStr(d1ram_addr, ProductKUNo_addr, tmpBuffer);

  return(IIC1ErrorCode ? 1 : 0);
}


/***************************************************************************/
/*  TestGPSGenlockUnit                                          UNIT_DRV.C */
/*                                                                         */
/* Author:    Jens K Hanse, DEV, 071129                                    */
/*                                                                         */
/*  Function:  Test the PT8616 GPS Digital Genlock                         */
/*  Remarks:    --                                                         */
/*  Returns:    errorcode                                                  */
/*  Updates:    --                                                         */
/***************************************************************************/
UC TestGPSGenlockUnit(void) {

  FeedTheWatchdog;

  IIC1RdStr(d2ram_addr, 200, tmpBuffer);  //200 = GPS module subaddress for KU str

  return(IIC1ErrorCode ? 1 : 0);
}

/***************************************************************************/
/*  TestTimeClockUnit                               UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980730, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the PT8607/37 Time Code-/Time Clock Module          */
/*  Remarks:    --                                        */
/*  Returns:    0: OK, 1: ERROR                              */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestTimeClockUnit(void) {

  FeedTheWatchdog;

  if (TimeClockUnit.HWType == PT8637) {
                          // Get TCI error information.
    TimeClockConfig.TCIControl &= 0x1F;
    TimeClockConfig.TCIControl |= (TCIErrorRegister << 5);
    IIC1Write(time1_addr, TimeClockConfig.TCIControl);

    FeedTheWatchdog;

    IIC1ReadN(time1_addr, 2, (UC*) tmpBuffer);

    if (tmpBuffer[1] || IIC1ErrorCode)
      return(1);              // FAIL
  }

  IIC1RdStr(timeram_addr, ProductKUNo_addr, tmpBuffer);

  return(IIC1ErrorCode ? 1 : 0);
}


/***************************************************************************/
/* Function: UC TestTLGUnit(void)                               UNIT_DRV.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 10.06.2004                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: To perform test of TLG unit as part of 'Diagnose/Options'     */
/* Args:     None                                                          */
/* Returns:  0: OK, 1: ERROR                                               */
/* NOTES  :                                                                */
/***************************************************************************/
UC TestTLGUnit(void)
  {
  return (0);
  }

UC TestHDTPGUnit(void)
  {
  return (0);
  }

UC TestDLTPGUnit(void)
  {
  return (0);
  }

/***************************************************************************/
/*  TestHighPrecisionOCXUnit                         UNIT_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980524, KEn, DEV                                 */
/*                                                  */
/*  Function:  Test the PT8610 High Precision OCXO                  */
/*  Remarks:    Presently NOT tested                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
UC TestHighPrecisionOCXUnit(void) {

  FeedTheWatchdog;

  return(0);
}


/* given generator nr (i) find which generator */
/* onboard it is.                              */
/* Returns 1 if it is gen 0                    */
/* Returns 2 if it is gen 1                    */
/* Returns 4 if it is gen 2                    */
/* Returns 8 if it is gen 3                    */
UC find_hd_gen(UC i)
  {
  switch (i)
    {
    case 0:
      return (1);
    case 1:
      return (2);
    case 2:
      return (4);
    case 3:
      return (8);
    case 4:
      return (1);
    case 5:
      return (2);
    case 6:
      return (4);
    case 7:
      return (8);
    case 8:
      return (1);
    case 9:
      return (2);
    case 10:
      return (4);
    case 11:
      return (8);
      break;
    }
  return (0xFF);
  }

/* given generator nr (i) find which generator */
/* onboard it is.                              */
/* Returns 1 if it is gen 0                    */
/* Returns 2 if it is gen 1                    */
UC find_dl_gen(UC i)
  {
  switch (i)
    {
    case 0:
      return (1);
    case 1:
      return (2);
    case 2:
      return (1);
    case 3:
      return (2);
    case 4:
      return (1);
    case 5:
      return (2);
    }
  return (0xFF);
  }
