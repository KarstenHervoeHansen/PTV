/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:  PT5230 Digital Video Generator, Master Controller, V401      */
/*  Module:    MAS.C                                                       */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980122                                                     */
/*  Rev. date:  04.01.2006, JK                                             */
/*  Status:    Version 3.5                                                 */
/*                                                                         */
/*  This module contains the following functions:                          */
/*                                                                         */
/*    void waitms(UI time)                                                 */
/*    void waitus(UI time)                                                 */
/*    interrupt void Timer_int(void)                                       */
/*    void main(void)                                                      */
/*                                                                         */ 
/*  Changes:                                                               */
/* 07.04.2008: Corrected error. Unlocked lit, when no GPS connection, even */
/*			   when GPS genlock not selected.							   */
/* 06.02.2008: Released as 5.4, added support for PT 8616 GPS genlock and  */
/* 			   and LTC. Whole LTC menu added							   */ 
/* 04.01.2006: Comm errors tackling added on TLG and HD boards.            */
/* 16.09.2005: Modified for 32 K ram, required modification of address     */
/*             decoders, so it will no longer run on 5630 hardware.        */
/*             Complier switch -DSMALL_MEM switches between small and big  */
/*             RAM. Two link response files are provided for each RAM.     */
/* 12.08.2005: Added tackling of up to 12 HD generators (up to 3 boards)   */
/*                                                                         */
/* 08.12.2003: Modified for tackling TLG generator.  TLG generator may     */
/*             only be mounted in XC1 connector.                           */
/*                                                                         */
/* 991105: UserTimer is now an unsigned integer                            */
/* 991103: Added DispTimer to be used solely in the disp_drv.c module      */
/* 991025: "Master reset" has been removed, i.e. it is no longer supported */
/* 990420: Error in masking of bit in parallel remote corrected.           */
/*         Only 6 presets are allowed from the parallel remote.            */
/* 981005: Releases as 4008 002 06812 software ver. 2.0                    */
/* 980924: Error intializing ConfigNdx, (PT8607 error).                    */
/* 980915: Added bit butstLockOn.                                          */
/* 980827: The number of time slices in main routine has been increased    */
/* 980710: Implementation of Time Clock Interface PT8637                   */
/* 980616: Released as 4008 002 06812                                      */
/* 980611: Extended levelstatus handling with 6 audio levels for PT8635    */
/* 980609: Updated level error handling                                    */
/* 980514: Released as 4008 002 06811                                      */
/* 980514: The fan is switched ON during internal test                     */
/***************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <xa.h>
#include <intrpt.h>

#include "mas.h"

#include "iic.h"
#include "serial1.h"
#include "sw5230.h"
#include "cont_drv.h"
#include "keyb_drv.h"
#include "disp_drv.h"
#include "text.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "message.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "rs232par.h"
#include "util.h"
#include "unit_drv.h"
#include "tables.h"
#include "factory.h"


#define xtal 29491200        // Crystal  frequency for the mainboard


typedef union _ui
  {
  int  intval;
  UC   charval[2];
  }  ui;
#if 0
          charval[0] = BYTE3 = LSB
          charval[1] = BYTE2 = MSB   
#endif



static bit RxD1 @ 0x38C;
static bit TxD1 @ 0x38D;
static bit RxD0 @ 0x398;
static bit TxD0 @ 0x399;


/* Following installation of 32 K RAM, address decoders had to be */
/* redesigned.  Below are the new address assignments.            */
#ifdef SMALL_MEM
// *************************************************************************
// PORT CONTROL:
// RAM has address-space 0-0x1fff
// *************************************************************************
volatile UC ind1 @ 0x2001;      // input port 1
volatile UC ind2 @ 0x4001;      // input port 2 Not used

volatile UC ud1 @ 0x2001;      // output port 1
volatile UC ud2 @ 0x4001;      // output port 2
volatile UC ud3 @ 0x6001;      // output port 3 Display Data
volatile UC ud4 @ 0x8001;      // output port 4
volatile UC ud5 @ 0xA001;      // output port 5
#else
// *************************************************************************
// PORT CONTROL:
// RAM has address-space 0-0x7fff
// *************************************************************************
volatile UC ind1 @ 0x08001;      // input port 1
volatile UC ind2 @ 0x10001;      // input port 2 Not used

volatile     UC ud1 @ 0x08001;      // output port 1
volatile     UC ud2 @ 0x10001;      // output port 2
volatile far UC ud3 @ 0x18001;      // output port 3 Display Data
volatile far UC ud4 @ 0x20001;      // output port 4
volatile far UC ud5 @ 0x28001;      // output port 5
#endif

near UC ud1buf;            // buffer for output port 1
near UC ud4buf;            // buffer for output port 4
near UC ud5buf;            // buffer for output port 5

// *************************************************************************
// CONTROL OF OPTIONAL UNITS:
// Resets:
// *************************************************************************
#define  MRES1_on    { ud4buf &= ~0x20; ud4 = ud4buf;}
#define  MRES1_off   { ud4buf |=  0x20; ud4 = ud4buf;}
#define  MRES2_on    { ud4buf &= ~0x40; ud4 = ud4buf;}
#define  MRES2_off   { ud4buf |=  0x40; ud4 = ud4buf;}
#define  MRES3_on    { ud4buf &= ~0x80; ud4 = ud4buf;}
#define  MRES3_off   { ud4buf |=  0x80; ud4 = ud4buf;}


// *************************************************************************
// CONTROL OF FAN (BLOWER)
// *************************************************************************
#define  fan_on   { ud1buf |=  16; ud1 = ud1buf;}
#define  fan_off  { ud1buf &= ~16; ud1 = ud1buf;}

near int temp;               // Temperature
near UI oldtemp, grense;     // Temperature stores
near UC tempcnt;             // Counter for speed change filter
bit shotdown;                // Flag for very hot apparatus
near UC tempwarn;            // Flag for hot apparatus


// *************************************************************************
// A/D CONVERSION VARIABLES
// *************************************************************************
near UC TempMeas;          // Temperature read from AD converter
near UC Volt5Meas;         // 5 volt read from AD converter

near UC VoltN5Meas;        // -5 volt read from AD converter
UC VoltN5Min;              // Min. value for -5 volt
UC VoltN5Max;              // Max. value for -5 volt

near UC Volt12Meas;        // 12 volt read from AD converter
UC Volt12Min;              // Min. value for 12 volt
UC Volt12Max;              // Max. value for 12 volt

bit VoltCalib;             // 1: Power has been calibrated


// *************************************************************************
// SYSTEM VARIABLES
// *************************************************************************
near UC  syncstatus;
near UC  syncreadstat;
near int RS232Char;
near UC  slicer;           // Time window counter
bit      TimerFlag;            // Control bit for 21 ms timing
bit      SecondFlag;           // Control bit for 1 s timing


// *************************************************************************
// STATIC VARIABLES
// *************************************************************************
PNP_STRUCT MainBoard =
  {
  TRUE, master_addr, NA, 0, 0, 0, 0, PT5230SWVersion,  // MAIN-BOARD
  };

PNP_STRUCT SPGBoard =
  {
  TRUE, sync_addr, NA, 0, 0, 0, 0, 0       // SPG-BOARD
  };

/* Things get little bit confusing here, we have 12 units, each  */
/* generator is treated as a unit, we have 3 boards, which each  */
/* contains 4 units (generators).  The system is like AnlBlkUnit */
/* where each board contains 2 units (bb generators).            */
PNP_STRUCT TLGUnit[MaxTLGUnits] =
  {
  FALSE, bbu3_addr, PT8611, 0, 0, 0, 0, 0,  // TLG1 /* TLGBoard1 */
  FALSE, bbu3_addr, PT8611, 0, 0, 0, 0, 0,  // TLG2
  FALSE, bbu3_addr, PT8611, 0, 0, 0, 0, 0,  // TLG3
  FALSE, bbu3_addr, PT8611, 0, 0, 0, 0, 0,  // TLG4

  FALSE, bbu1_addr, PT8611, 0, 0, 0, 0, 0,  // TLG5 /* TLGBoard2 */
  FALSE, bbu1_addr, PT8611, 0, 0, 0, 0, 0,  // TLG6
  FALSE, bbu1_addr, PT8611, 0, 0, 0, 0, 0,  // TLG7
  FALSE, bbu1_addr, PT8611, 0, 0, 0, 0, 0,  // TLG8

  FALSE, bbu2_addr, PT8611, 0, 0, 0, 0, 0,  // TLG9 /* TLGBoard3 */
  FALSE, bbu2_addr, PT8611, 0, 0, 0, 0, 0,  // TLG10
  FALSE, bbu2_addr, PT8611, 0, 0, 0, 0, 0,  // TLG11
  FALSE, bbu2_addr, PT8611, 0, 0, 0, 0, 0   // TLG12
  };

// HDTPG may reside at bbu1_addr, bbu2_addr and bbu3_addr.
PNP_STRUCT HDTPGUnit[MaxHDTPGUnits] =  // HDTPG 
  {
  FALSE, bbu1_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG1 /* HDTPGBoard1 */
  FALSE, bbu1_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG2
  FALSE, bbu1_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG3
  FALSE, bbu1_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG4

  FALSE, bbu3_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG5 /* HDTPGBoard2 */
  FALSE, bbu3_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG6
  FALSE, bbu3_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG7
  FALSE, bbu3_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG8

  FALSE, bbu2_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG9 /* HDTPGBoard3 */
  FALSE, bbu2_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG10
  FALSE, bbu2_addr,   PT8612, 0, 0, 0, 0, 0,        // HDTPG11
  FALSE, bbu2_addr,   PT8612, 0, 0, 0, 0, 0         // HDTPG12
  };


// DLTPG may reside at bbu1_addr, bbu2_addr and bbu3_addr.
PNP_STRUCT DLTPGUnit[MaxDLTPGUnits] =  // DLTPG 
  {
  FALSE, bbu1_addr,   PT8613, 0, 0, 0, 0, 0,        // DLTPG1 /* DLTPGBoard1 */
  FALSE, bbu1_addr,   PT8613, 0, 0, 0, 0, 0,        // DLTPG2

  FALSE, bbu3_addr,   PT8613, 0, 0, 0, 0, 0,        // DLTPG3 /* DLTPGBoard2 */
  FALSE, bbu3_addr,   PT8613, 0, 0, 0, 0, 0,        // DLTPG4

  FALSE, bbu2_addr,   PT8613, 0, 0, 0, 0, 0,        // DLTPG5 /* DLTPGBoard3 */
  FALSE, bbu2_addr,   PT8613, 0, 0, 0, 0, 0,        // DLTPG6
  };

 
PNP_STRUCT SDITPGUnit[MaxSDITPGUnits] =
  {
  { FALSE, SDITPG_addr, PT8632, 0, 0, 0, 0, 0},      // SDI-TPG1
  { FALSE, bbu2_addr,   PT8633, 0, 0, 0, 0, 0},      // SDI-TPG2
  { FALSE, bbu2_addr,   PT8633, 0, 0, 0, 0, 0}       // SDI-TPG5
  };

PNP_STRUCT AnlBlkUnit[MaxAnlBlkUnits] =
  {
  {  TRUE, bbu0_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST1
  {  TRUE, bbu0_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST2
  { FALSE, bbu1_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST3
  { FALSE, bbu1_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST4
  { FALSE, bbu3_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST5
  { FALSE, bbu3_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST6
  { FALSE, bbu2_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST7
  { FALSE, bbu2_addr, PT8608, 0, 0, 0, 0, 0 },        // BLACK-BURST8
  };

PNP_STRUCT AnlTPGUnit[MaxAnlTPGUnits] =
  {
  { FALSE, bbu2_addr, PT8631, 0, 0, 0, 0, 0 },        // ANALOG-TPG2
  { FALSE, bbu2_addr, PT8631, 0, 0, 0, 0, 0 }         // ANALOG-TPG5
  };

PNP_STRUCT SDITSGUnit[MaxSDITSGUnits] =
  {
  { FALSE, bbu2_addr, PT8639, 0, 0, 0, 0, 0},        // SDI-TSG2
  { FALSE, bbu1_addr, PT8639, 0, 0, 0, 0, 0},        // SDI-TSG3
  { FALSE, bbu3_addr, PT8639, 0, 0, 0, 0, 0}         // SDI-TSG4
  };

PNP_STRUCT AESEBUUnit =
  {
  FALSE, NA, PT8635, 0, 0, 0, 0, 0,              // AES-EBU1
  };

PNP_STRUCT SDIGenlockUnit =
  {                                              // SDI Genlock
  FALSE, NA, PT8606, 0, 0, 0, 0, 0
  };

PNP_STRUCT GPSGenlockUnit =
  {                                              // GPS Genlock
  FALSE, NA, PT8616, 0, 0, 0, 0, 0
  };

  
PNP_STRUCT Int2GenlockUnit =
  {                                              // High Precision OCX
  FALSE, NA, PT8610, 0, 0, 0, 0, 0
  };

PNP_STRUCT TimeClockUnit =
  {                                              // Time code/clock
  FALSE, NA, PT8637, 0, 0, 0, 0, 0
  };
  
PNP_STRUCT NTPUnit =
  {                                              // NTP Unit
  FALSE, NA, PT8643, 0, 0, 0, 0, 0
  };  

//number of days in every month
char month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  
// *************************************************************************
// DYNAMIC VARIABLES, IE. VARIABLES "CONTINUOSLY" BEING UPDATED.
// *************************************************************************
persistent DLTPG_STRUCT     DLTPGConfig[MaxDLTPGUnits];
persistent HDTPG_STRUCT     HDTPGConfig[MaxHDTPGUnits];
persistent TLG_STRUCT       TLGConfig[MaxTLGUnits];

persistent ANLBLK_STRUCT    AnlBlkConfig[MaxAnlBlkUnits];
persistent ANLTPG_STRUCT    AnlTPGConfig[MaxAnlTPGUnits];
persistent SDITSG_STRUCT    SDITSGConfig[MaxSDITSGUnits];
persistent SDITPG_STRUCT    SDITPGConfig[MaxSDITPGUnits];
persistent AESEBU_STRUCT    AESEBUConfig[MaxAESEBUUnits];
persistent GENLOCK_STRUCT   GenlockConfig[MaxGenlockUnits];
persistent LTC_STRUCT     LTCConfig[MaxLTCUnits];
persistent NTP_STRUCT		NTPConfig;
persistent TIMECLOCK_STRUCT TimeClockConfig;

persistent RS232_STRUCT RS232Config;
persistent PRESET_STRUCT Preset[NoOfPresets];
persistent STATUS_FLAGS Flags;

persistent UC ActivePresetNo;     // Currently active preset, (0 = NONE)
persistent UC ActiveOutputNo;     // Currently active output, ie. OUTPUT BUTTON

persistent UC ExternalGenlockNo;  // Last selected EXTERNAL genlock
persistent UC ActiveGenlockNo;    // Currently active genlock

UC LTCNo;             // Selected LTC generator

persistent UC LCDContrast;        // Contrast on LCD-display
persistent UC PasswordState;      // OFF: Apparatus is NOT password protected

persistent UC RAMCheckArea[24];   // Area used for PowerUp RAM check
 
code struct MENU_ITEM* near Ptr;
code struct MENU_ITEM* near PrevPtr;

UI ErrorStatus;                 // Last active global error code, 32 BIT (?? UI is 16 bits)
                                // 8 MSB   8 LSB
                                // [index]  ErrorType+ErrorNo

UI LevelStatus;                 // Status of level detectors 1-9, (0x1FF)

UL ErrorQueue[NoInErrorQueue] =
  {                             // Global error code queue, 32 BIT
  0, 0, 0, 0, 0                 // 16 MSB      8 BIT   8 LSB
  };                            // leveldetector [Index] ErrorType+ErrorNo

UC ErrorQueuePtr = NoInErrorQueue-1;
bit ErrorWindowOn;              // 1: Displaying an "error" window

bit MessageWindowOn;            // 1: Displaying a "message" window
near UC MessageCntDwn;          // Counter for display of message

volatile near UC SecTimer;      // Seconds timer: updated every second
volatile near UC AutoESCTimer;  // Timer for auto return to status display
volatile near UC DispTimer;     // Display driver timer: updated every 20ms
volatile near UI UserTimer;     // Generel user timer: updated every 20ms
volatile near UC DisplayTimer;  // Error display timer: updated every 20ms

UC  UpdateTimer;                // Timer for display animation
UC  UpdateFrequency;            // Frequency for the above
bit UpdateEnable;               // 1: Display animation is active
bit UpdateSignal;               // 1: Display animatino caused the call
bit AutoESCSignal;              // 1: Return to status display
bit DoKeyboardScan;             // 1:  Scan the keyboard
bit TestModeEnable;             // 1: Enable test mode
bit MenuModeOn;                 // 1: Menu mode active
bit SelectModeOn;               // 1: Selection mode active
bit burstLockOn;                // 1: if burst available in genlock
  
UC LTC_request=0;       // request for confirmation or displaying, that
                // daylight saving are to be switched on/off
                
UC LTC_sync_request=0;      // request for updating 30/1,001 LTC sync
                
UC GPS_master_status=0;     //masterstatus holds bit: 0=GPS valid, 1=PLL valid, 2-7=reserved
UC GPS_warning_status=0;	//warning menu status for GPS = 1 if warning has been shown

/* Following change to 5300, with new front panel, switches and LEDs */
/* present in 5230 are no longer here.  Now we have 3 LEDs:          */
/* GenlockLED                                                        */
/* UnlockedLED                                                       */
/* WarningLED                                                        */
/*             GenlockLED is lit when genlock is not INTERNAL        */
/*             WarningLED is lit when there is some irregularity     */
/*             UnlockedLED is lit when genlock mode is enabled but   */
/*             no correct genlock signal is found on the active      */
/*             genlock input.                                        */
#if 0
bit CBarLED;                    // 1: Illumination of the C.BAR LED
bit MBurstLED;                  // 1: Illumination of the M.BURST LED
bit WindowLED;                  // 1: Illumination of the WINDOW LED
bit SpecialLED;                 // 1: Illumination of the SPECIAL LED
bit LinearityLED;               // 1: Illumination of the LINEARITY LED
bit PatternLED;                 // 1: Illumination of the PATTERN LED
bit RemoteLED;                  // 1: Illumination of the REMOTE LED
#endif
bit UnlockedLED;                // 1: Illumination of the UNLOCKED LED
bit GenlockLED;
bit WarningLED;

                                //  systems BurstLockPAL & BurstLockNTSC.
bit SerialRemotePresent;        // 1: Remote cable placed in serial conn.
bit SerialRemoteActive;         // 1: Serial remote active
bit SerialRemoteLockoutOn;      // 1: Serial remote lockout (ctrl-L)
bit ParallelRemotePresent;      // 1: Remote cable placed in parallel conn.
bit ParallelRemoteActive;       // 1: Parallel remote active
UC  ParallelRemoteStatus;
bit RS232DiagnoseMode;          // 1: RS232 Loopback test in progress

bit transparent;                // indicates transparent mode for FPGA 
                                // programming inside PT8641.

static char tbuff[80];     /* temporary buffer used for printing messages */
                           /* for test purposes                           */
                           
static char LTC_tbuff[11]; /*buffer for LTC timecode */

code char SWDownloadTxt[] = "SW Download";

void update(); 
/***************************************************************************/
/*  waitms                                                          MAS.C  */
/*                                                                         */
/* Author:    Preben Christiansen, DEV, 960828                             */
/* Revised:    960828                                                      */
/*                                                                         */
/*  Function:  A <n>ms wait routine where n is user definable.             */
/*  Remarks:    This is a software based loop. Altering the crystal or     */
/*          tampering with the waitstates might have serious influence     */
/*          on the timing.                                                 */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void waitms(UI time)       // Longer wait routine
  {
  UI n;

  while (time-- > 0)
    {                 // Wait 1 ms x time
    n = 750;
    while (n > 0)
      n--;
    }
  }

/***************************************************************************/
/*  waitus                                                          MAS.C  */
/*                                                                         */
/* Author:    Preben Christiansen, DEV, 960828                             */
/* Revised:    960828                                                      */
/*                                                                         */
/*  Function:  A <n>us wait routine where n is user definable.             */
/*  Remarks:    This is a software based loop. Altering the crystal or     */
/*          tampering with the waitstates might have serious influence     */
/*          on the timing.                                                 */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void waitus(UI time)   // Short wait routine
  {
  while (time-- > 0)
    ;                  // Wait 1.1 us x time
  }

/***************************************************************************/
/*  VECTOR DEFINITIONS                                              MAS.C  */
/*                                                                         */
/* Interrupts:T0 prior:     9  bank 1                                      */
/*    Ser0_rec     15   -   0                                              */
/*    Ser1_rec  -  15   -   0                                              */
/*    Ser0_trm  -   ?   -   0                                              */
/*    Ser1_trm  -   ?   -   0                                              */
/***************************************************************************/
ROM_VECTOR(IV_T0, Timer_int, IV_SYSTEM+IV_PRI15+IV_BANK1)

ROM_VECTOR(IV_RI0, Udr_Uart_RX_Interrupt, IV_SYSTEM+IV_PRI14+IV_BANK0)
ROM_VECTOR(IV_TI0, Udr_Uart_TX_Interrupt, IV_SYSTEM+IV_PRI14+IV_BANK0)

ROM_VECTOR(IV_EX0, Udr_CTS_Interrupt, IV_SYSTEM+IV_PRI14+IV_BANK0)

ROM_VECTOR(IV_RI1, Serial1_int, IV_SYSTEM+IV_PRI14+IV_BANK0)

/***************************************************************************/
/*  Timer_int                                                       MAS.C  */
/*                                                                         */
/* Author:    Preben Christiansen, DEV, 960828                             */
/* Revised:    991105, KEn, DEV                                            */
/*                                                                         */
/*  Function:  Interrupt routine which handles ALL timing.                 */
/*  Remarks:    Changing the crystal WILL affect the timing.               */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
interrupt void Timer_int(void)
  {
  static near UC ticker;

  TH0 = -xtal/256/4/3/50;       // Load timer to 50 Hz (20ms)
  if (++ticker > 2)
    {
    ticker = 0;
    TimerFlag = TRUE;           // Set 20ms flag
    if (!--SecTimer)
      {                                       // This routine is executed each second.
      SecTimer = 50;                          // Reload seconds timer.
      if (Flags.AutoESCEnable)                // If auto escape enabled
        if (++AutoESCTimer > AutoESCTimeOut)  // and time-out reached
          AutoESCSignal = TRUE;              //TRUE;  // set time-out flag ***** JK*******
      }
    if (DispTimer)          // DispTimer is decremented each 20 ms.
      DispTimer--;
    if (UserTimer)          // UserTimer is decremented each 20 ms.
      UserTimer--;
    if (DisplayTimer)       // DisplayTimer is decremented each 20 ms.
      DisplayTimer--;
    DoKeyboardScan = TRUE;  // Set flag for keyboard-scan
    }
  }

/***************************************************************************/
/*  main                                                            MAS.C  */
/*                                                                         */
/* Author:    Kim Engedahl DEV, 980122                                     */
/* Revised:    991025, KEn, DEV                                            */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void main(void) 
  {
  UI         i;
  UC         TCIStatus, error;
  static bit TCISecond;
  char       textBuffer[10];

  ui         loader;
  UC         M;
  UC         L;

// System set-up
  BCR   = 0x02;    // Set 20 bit BUS, 8 bit DATA
  PSWH  = 0x8F;    // Set system priority high
  BTRH  = 0xAE;    // To be trimmed (was 0xef) EXT. BUS timing, Data Cycle
  BTRL  = 0xCE;    // To be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
  PSWL  = 0;       // Clear flags
  SCR   = 0;       // Timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  PSWH &= 0xF0;    // Set system priority low

/***************************************************************************/
  WDCON  = 0;      /* Disable watchdog */
  WFEED1 = 0xA5;
  WFEED2 = 0x5A;
/***************************************************************************/

  MRES1_on;    // Reset all options NB! DO NOT MOVE!!!!!
  MRES2_on;    //       do          NB! DO NOT MOVE!!!!!
  MRES3_on;    //       do          NB! DO NOT MOVE!!!!!

// Serial set-up
  S1CON = 0xF2;          // Transmit mode 3,  9 bit UART,  Address mode, Rec.enable
  S1ADDR = master_addr;  // receive address
  S1ADEN = 0xCC;         // mask for receive address
                         // receive address = 00010000 AND 11001100 = 00XX00XX (0 to 33)

// Timer set-up
  TMOD   = 0x21;    // Timer 1: 8 bit autoload; Timer 0: 16 bit count
  TH0    = -255;    // Start value
  T2MOD  = 0x31;    // Use T2 for V24_1, count up
  T2CAPH = -1;      // Set reload values
  T2CAPL = -6;
  TH2    = -1;      // Set for max. Baud
  TL2    = -6;
  TR2    = 1;       // Start V24-timer

// Interrupt setting
  IT0 = 1;      // Edge trig on ext0
  IT1 = 1;      // Edge trig on ext1
  EX0 = 0;      // Disable ext0
  ET0 = 1;      // Enable timer0 intrpt
  ET1 = 0;      // Disable timer1 intrpt

// NB! IPAx PRIORITETS-SETTING SKAL MATCHE SETTING I ROM_VECTOR() !!!
// (Brug kun 9-F, 0 ell. 8 er intrpt off!)
  IPA0 = 0xFE;      // Priority Timer0/EX0 
  IPA4 = 0xEE;      // Priority PTI0/PRI0, (serial 0)
  IPA5 = 0x0E;      // Priority PTI1/PRI1, (serial 1)

  Serial1_init();    // Initialize internal serial v24 bus

  FeedTheWatchdog;

  RdFrontIIC(0xF0);  // Initialize front IIC bus by reading dummy
  IIC1Read(0xF0);    // Initialize IIC bus 1 by reading dummy
  IIC2Read(0xF0);    // Initialize IIC bus 2 by reading dummy

                // Initialize level detector ports
  IIC1Write(lev1det_addr, 0xFF);
  IIC1Write(lev2det_addr, 0xFF);

  ud4buf &= 0xF8;    // Disable display (@V412)
  ud4     = ud4buf;

  TR0 = 1;           // Start loop timer
  TR1 = 1;           // Start V24-timer

  transparent = 0;

  EA  = 1 ;          // Enable all interrupts

  fan_on;                  // Turn blower on while performing internal test

  oldtemp  = 255;          // Initiate temperature control
  tempwarn = 0;
  slicer   = 0;

  SecTimer      = 50;      // Seconds timer: 50 gives a 1 second timer, ie. 5*20ms
  UpdateTimer   = 0;       // Update timer used for animation. Reset to 0

  AutoESCSignal = 0;       // 1: returnSignal to escape from menu to display
  UpdateEnable  = 0;       // 1: Animation enabled
  UpdateSignal  = 0;       // 1: The call to the routine caused by animation

  OldButtonCommand = ButtonCommand = NOCommand;

  ParallelRemotePresent = FALSE;
  ParallelRemoteActive  = FALSE;
  ParallelRemoteStatus  = 0xFF;

  SerialRemotePresent   = FALSE;
  SerialRemoteActive    = FALSE;
  SerialRemoteLockoutOn = FALSE;  // Disable serial remote lockout

  RS232DiagnoseMode     = FALSE;

  TestModeEnable        = OFF;
  /* JK addition */
  Cmemcpy(&RS232Config, &RS232ConfigFactory, sizeof(RS232Config));
                            // Initialize the remote RS232
  Udr_InitializeUart(RS232Config.Baudrate, RS232Config.Databit,\
                     RS232Config.Parity, RS232Config.Handshake);

  // **********************************************************************
  //  Reset all units. There are 3 resets to options, MRES1 MRES2 MRES3.
  //  In order to identify the position of the options, one reset is
  //  released at a time and an option address is send to this option.
  //  If the option then answers back its address, the Master then
  //  knows the presence of this option.
  //  Do allow all units to do their own initialisation.
  //  Variable 'options' (for Test purposes)  has a bit for each option.
  // **********************************************************************

  UserTimer = i = 10;      // Reset for  80-100 ms

  while (UserTimer)
    {
    FeedTheWatchdog;

    if (i == UserTimer)
      {
      InitDisplay();      // Initialize display, this is done 4 or 5 times
      i--;
      }
    }

  i = ~ind1;              // Detect type of remote connector (@V407)

  if (i & SerialRemoteMask)    // RS232 remote installed
    SerialRemotePresent = TRUE;

  if (i & ParallelRemoteMask)  // Parallel remote installed
    ParallelRemotePresent = TRUE;

  UnlockedLED  = ON;
  GenlockLED   = ON;
  WarningLED   = ON;

  UpdateLEDs();

  UnlockedLED  = OFF;
  GenlockLED   = OFF;
  WarningLED   = OFF;

  TempMeas     = 128;          // Intialize TempMeasurement to dummy
  InitContrast();              // Initialize the display contrast
                               // Test buttons during power-on
  switch (RdFrontIIC(keyb_addr))
    {
    case FACTORYButton:
      WriteCodeLN1(0, PowerUpFactResetTxt);
      FactoryPreset();
      
      // send command to NTP board, to initalize reset on board
      // This is done at the very end, so that NTP board is booted
      IIC1Write2(ntp_addr, 7, 0x01);
      break;
    default:
      WriteCodeLN1(0, PowerUpDefaultTxt);
      break;
    }
  ClearRestLN1();
                      // Get information about the main-board
  GetIICVersion(IICBus2, mainram1_addr, ProductKUNo_addr,\
                &MainBoard.KUNumber);
  UserTimer = 5;          // Set timer to 380-400ms (20 orig)

  IIC2Write(mainram1_addr, VoltCalib_addr);   // Get powersupply limits
  VoltCalib = (IIC2Read(mainram1_addr) == ON);// Calibration
  VoltN5Min = IIC2Read(mainram1_addr);        // Min. -5 volt
  VoltN5Max = IIC2Read(mainram1_addr);        // Max. -5 volt
  Volt12Min = IIC2Read(mainram1_addr);        // Min. 12 volt
  Volt12Max = IIC2Read(mainram1_addr);        // Max. 12 volt

  WrFrontIIC(ADC_addr, 0x44);                 // ADC. Auto increment
  Volt12Meas = RdFrontIIC(ADC_addr);          // Read 12 volt A/D
  TempMeas   = RdFrontIIC(ADC_addr);          // Read temperature A/D
  Volt5Meas  = RdFrontIIC(ADC_addr);          // Read +5V A/D
  VoltN5Meas = RdFrontIIC(ADC_addr);          // Read -5V A/D

  while (UserTimer)        // Short delay for the eye
    FeedTheWatchdog;

  WriteCharLN2(0, ' ');
  ClearRestLN2();
  WriteCodeLN2(8, SelfTestTxt);// Write Selftest in progress


  i = 0;

/* Note: If you run this program in a ROM emulator, the routine 
         below PowerUpRAMCheckOK() will fail, as loading emulator
         RAM also corrupts Battery Powered RAM (V404).
*/
  if (!PowerUpRAMCheckOK())
    {                // If RAM test fails..
    FactoryPreset(); // ************  It is here it fails if nr of presets is too big! *********
    DisplayErrorLine(RAMMemoryErrorType, ErrorDisplayTimeout);
    }

  MRES1_off;                // Release reset 1
  UserTimer = 160;           // Set timer to 200-220ms (org 11)
  /* This delay is quite critical.  As HD board draws considerable current,        */
  /* if HD board is mounted at bbu1, before we reach FindDynamicOptions(bbu1_addr) */
  /* the volatages may not reach working values and the (HD) board mounted at      */
  /* bbu1 MAY fail to respond. (Will not remember it's settings.                   */

  while (UserTimer)          // While waiting for BBU_1 unit to
    FeedTheWatchdog;

  // **********************************************************************
  // Initialize mainboards Black Burst
  // **********************************************************************
  for (i = BB1Item; i <= BB2Item; i++)
    {
    GetV24Version(i, bbu0_addr, PT8608);
    AnlBlkUnit[i].KUNumber = MainBoard.KUNumber;
    if (error = ConfigureAnlBlkUnit(i))
      {
      DisplayErrorLine(error, ErrorDisplayTimeout);
      }
    }
  // **********************************************************************
  // Intialize options at connector TXG3Conn, TXG2Conn & TXG4Conn
  // **********************************************************************

  for (i = 0; i < MaxTLGUnits; i++)
    TLGUnit[i].Present = FALSE;

  FindDynamicOptions(bbu1_addr);

  MRES2_off;                 // Release reset 2
  UserTimer = 160;            // 13 org Set timer to 240-260ms

  while (UserTimer)          // While waiting for BBU_2 unit to
    FeedTheWatchdog;

  FindDynamicOptions(bbu2_addr);

  MRES3_off;                 // Release reset 3
  UserTimer = 160;            // 13 org Set timer to 240-260ms 13

  while (UserTimer)          // While waiting for BBU_3 unit to
    FeedTheWatchdog;

  FindDynamicOptions(bbu3_addr);


  FindStaticOptions();          //  Static options should only be intialized
                                //  AFTER all the resets have been released.
  if (error = GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System))
    {        
    /* In case of error being displayed: "No contact to OSC", we land here */
    DisplayErrorLine(error, ErrorDisplayTimeout);
    }                         
  if (error = ConfigureSPG())  // Download calibration data to SPG
    {
    DisplayErrorLine(error, ErrorDisplayTimeout);
    }
  // **********************************************************************
  // RECALL OLD SETTINGS
  // **********************************************************************
  grense = templimit - 2;     // Limit for blower (high temp.)

  UpdateLEDs();

  UserTimer = 25;             // Start delay for at least 1.5 second (75 orig)

  /* Due to unknown reasons, the ROMCheckOK() below causes (sometimes) the  */
  /* Reset signal to go active, although FeedTheWatchdog is present in this */
  /* function.  Therefore it is eliminated here.  ROM checksum still can be */
  /* verified in DIAGNOSE, MEMORY.                                          */
  /* This problem is present only when running ROM Emulator.                */
#if 1
  if (!ROMCheckOK(&i))        // PROM Cheksum test
    DisplayErrorLine(ROMMemoryErrorType + ChecksumError, ErrorDisplayTimeout);
#endif

  if (!ErrorStatus)
    {  
    WriteCodeLN2(8, InternalTestTxt);
    ClearRestLN2();
    while (UserTimer)         // Simulates delay for internal test
      FeedTheWatchdog;
    }

  UserTimer   = 5; //100;     // Start delay 2 second

  BaseNdx     = 0;            // Reset base menu
  RAMBaseNdx  = 0;     
  FirstItem   = 0;
  AnlBlkNdx   = 0;            // Reset last active Analog Black menu item
                              // Reset last active TLG menu item
  /* Find right value for TLGNoNdx, which depends on where (which bbux_addr) */
  /* TLG boards are possibly installed.                                      */

  if (DLTPGUnit[4].Present)
    DLTPGNoNdx = 4;
  if (DLTPGUnit[2].Present)
    DLTPGNoNdx = 2;
  if (DLTPGUnit[0].Present)
    DLTPGNoNdx = 0;

  if (HDTPGUnit[8].Present)
    HDTPGNoNdx = 8;
  if (HDTPGUnit[4].Present)
    HDTPGNoNdx = 4;
  if (HDTPGUnit[0].Present)
    HDTPGNoNdx = 0;

  if (TLGUnit[8].Present)
    TLGNoNdx = 8;
  if (TLGUnit[4].Present)
    TLGNoNdx = 4;
  if (TLGUnit[0].Present)
    TLGNoNdx = 0;

  for (i = 0; i < MaxTLGUnits; i++) /*__________________________*/
    TLGNdx[i] = 0;

  for (i = 0; i < MaxHDTPGUnits; i++)
    HDTPGsubmenuNdx[i] = 0;

#if 1
  for (i = 0; i < MaxDLTPGUnits; i++)
    DLTPGsubmenuNdx[i] = 0;
#endif

  for (i = 0; i < MaxAnlBlkUnits; i++)
    BBNdx[i] = 0;
                              // Reset last active Analog TPG menu item
  for (i = 0; i < MaxAnlTPGUnits; i++)
    AnlTPGNdx[i] = 0;
                        // Reset last active SDI TSG menu item
  for (i = 0; i < MaxSDITSGUnits; i++)
    SDITSGNdx[i] = 0;
                        // Reset last active SDI TPG menu item
  for (i = 0; i < MaxSDITPGUnits; i++)
    SDITPGNdx[i] = 0;
                        // Reset last active AES-EBUn menu item
  AESEBUnNdx = 0;
                        // Reset last active AES-EBU menu item
  for (i = 0; i < MaxAESEBUUnits; i++)
    AESEBUNdx[i] = 0;

  PresetNdx = 0;                // Reset last active Preset menu item

                                // Reset last active Config menu item
  ConfigNdx = (!TimeClockUnit.Present || (TimeClockUnit.HWType == PT8607));

  DateTimeNdx = 0;              // Reset last active Date-Teim menu item
  RS232Ndx = 0;                 // Reset last active RS232 menu item
  DiagnoseNdx = 0;              // Reset last active Diagnose menu item

  while (UserTimer)             // Delay to show internal test passed
    FeedTheWatchdog;

  WriteCodeLN1(0, PT5230HeadlineTxt);
  ClearRestLN1();

  // Return the SW version of the main- & oscillator-board 
  GetPT5230SWVersion(textBuffer);

  WriteCharLN2(0, ' ');
  ClearToPosLN2(8);
  WriteCode2(VersionHdrTxt);    // Write software version
  WriteTxt2(textBuffer);
  ClearRestLN2();

  UserTimer = 100; //250;              // Delay 5.0 second or until key pressed
  while (UserTimer)
    if (ButtonReady())
      break;

  SendContrast(LCDContrast);
  MenuModeOn = SelectModeOn = OFF;

  if (ErrorStatus)                // If any errors during powerup
    {
    StatusNdx = StatusErrorItem;  //  start in Error/Warnings display
    }
  else
    {
    if (ActivePresetNo)
      StatusNdx = StatusPresetItem;
    else
      StatusNdx = StatusGenlockItem;
    }
  Ptr = &status_menu[StatusNdx];  // Prepare pointer for status display
  Ptr->DispFunct();               // Display status-display


  /************* test ***************/
#if 0

        loader.intval = 5;
        sprintf(tbuff, "INT = 0x%x\n", (int)loader.intval);
        Udr_PutNChar(tbuff);
        sprintf(tbuff, "LSB = A[0] = 0x%x\n", (int)loader.charval[0]);
        Udr_PutNChar(tbuff);
        sprintf(tbuff, "MSB = A[1] = 0x%x\n", (int)loader.charval[1]);
        Udr_PutNChar(tbuff);
        L = -loader.intval & 0xff;
        M = -loader.intval / 256 - 1; 
        sprintf(tbuff, "L = 0x%x\n", (int)L);
        Udr_PutNChar(tbuff);
        sprintf(tbuff, "M = 0x%x\n", (int)M);
        Udr_PutNChar(tbuff);

        L = -loader.charval[0];
        M = -loader.charval[1] - 1; 
        sprintf(tbuff, "L = 0x%x\n", (int)L);
        Udr_PutNChar(tbuff);
        sprintf(tbuff, "M = 0x%x\n", (int)M);
        Udr_PutNChar(tbuff);


#endif

	syncreadstat = 0;


  // **********************************************************************
  // ENDLESS LOOP 
  // **********************************************************************

  while(TRUE)
    {
      
    while(!TimerFlag)        // While waiting for timer flag..
      {
      // ****************************************************************
      // HANDLE SERIAL REMOTE INTERFACE, IF PRESENT
      // ****************************************************************
      if (SerialRemotePresent)     // If serial remote interface enabled..
        {
        if (!RS232DiagnoseMode)     // Do not respond if in diagnose mode
          while ((RS232Char = Udr_InChar()) != UDR_EOF)
            {
            CmdHandler((UC) RS232Char);
            FeedTheWatchdog;
            if (SerialRemoteActive == FALSE)
              {
              SerialRemoteActive = TRUE;     // Set serial remote flag
              SetupRS232GotoLocalMessage();  // Prepare for "Goto.." display
              }
            }
        }
      }
    TimerFlag = FALSE;  // 20 ms timer expired

    // *******************************************************************
    // FOR THE PT8637 ONLY: TIME CLOCK INTERFACE READ/UPDATE EVERY 20 ms
    // *******************************************************************
    if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637))
      {
      // NOTE! The time and display will ONLY be updated if a new time
      //  update is ready from the TCI.

      TCIStatus = IIC1Read(time1_addr);  // Get TCI status information
      if (IIC1ErrorCode)
        ;
      else
        {
        if (TCISecond != (TCIStatus & 0x01))
          {
          TCISecond ^= 1;                        // Toggle seconds flag.
          TimeClockConfig.TCIStatus = TCIStatus; // Update TCI status.
          BroadcastTCITime();                    // Read & broadcast TCI time.
          if (!MenuModeOn && (Ptr == &status_menu[StatusDateTimeItem]))
            Ptr->DispFunct();
          }
        }
      }
      
    // *******************************************************************
    // HANDLE GPS MODULE
    // *******************************************************************
  if (GPSGenlockUnit.Present)
  {
    //print LTC a or b, if LTC main menu selected
    if (Ptr==&LTC_menu[LTCSelectItem])
    {
      if(IIC1RdStr(d2ram_addr, 100+LTCNo, LTC_tbuff))
      	WriteTxtLN1(27, LTC_tbuff);
    }
    
    // This differs, because only GPS boards with firmware 2.1 or higher supports status
    if(Ptr==&status_menu[StatusGenlockItem])
	{
		if(GPSGenlockUnit.SWVersion>20)
			if(IIC1RdStr(d2ram_addr, 102, LTC_tbuff))
  				WriteTxtLN1(25, LTC_tbuff);
  	}
    
      // using slicer, for distributing status checking of different parts in GPS module
      // 1: check status for connection + PLL
      if (slicer==1)
      { 
        IIC1Write(d2ram_addr, 105); //address 105 = master status for GPS board     
	    GPS_master_status=IIC1Read(d2ram_addr);
	    
	    /*if(!IIC1ErrorCode)
	    {
	    	if(GPS_master_status!=GPS_warning_status) //if error, and message has not been shown
	    	{
	    		Ptr->DispFunct();
    			GPS_warning_status=GPS_master_status;
			}
		}*/
		
		
      }
      
      //3: check daylight saving switching
      if (slicer==3)
      {
        IIC1Write(d2ram_addr, 106); //address 106 = LTC requests change of daylight saving
        LTC_request=IIC1Read(d2ram_addr);
        
        if(!IIC1ErrorCode)
        {
	        //if request for changing LTC daylight setting
	        if (Ptr!=&LTC_menu[LTCConfirmDaylightItem]) //If not already in confirm menu 
	        {
	          //if request for changing LTC A daylight savings
	          if (LTC_request&3)
	          {
	            //if changemode = confirm, goto confirm menu
	            if (LTC_request&4)
	            {
	              WriteCodeLN1(0, MenuHdrTxt);
	              WriteCode1(LTCArrTxt[0]);
	              WriteChar1('/');
	              WriteCode1(LTC_menu[LTCConfirmDaylightItem].ItemTxt);
	              ClearRestLN1(); 
	              
	              FuncState = 10;
	              Ptr = &LTC_menu[LTCConfirmDaylightItem];
	              Ptr -> DispFunct();
	            }
	            //else changemode = auto change mode
	            else
	            {
	              IIC1Write2(d2ram_addr, 50, 1);
	              LTCNo=0;
	              FuncState=10;
	              Ptr = &LTC_menu[LTCMessageItem];
	              
	              if (LTC_request&1) //if daylight ON requested
	                MessageHandling(SwitchedDaylightOn);
	              else
	                MessageHandling(SwitchedDaylightOff);
	            }
	          }
	          //if request for changing LTC B daylight savings
	          else if (LTC_request&48)
	          {
	            //if changemode = confirm, goto confirm menu
	            if (LTC_request&64)
	            {
	              WriteCodeLN1(0, MenuHdrTxt);
	              WriteCode1(LTCArrTxt[1]);
	              WriteChar1('/');
	              WriteCode1(LTC_menu[LTCConfirmDaylightItem].ItemTxt);
	              ClearRestLN1(); 
	              
	              FuncState = 10;
	              Ptr = &LTC_menu[LTCConfirmDaylightItem];
	              Ptr -> DispFunct();
	            }
	            //else changemode = auto change mode
	            else
	            {
	              IIC1Write2(d2ram_addr, 51, 1);
	              LTCNo=1;
	              FuncState=10;
	              Ptr = &LTC_menu[LTCMessageItem];
	              
	              if (LTC_request&16) //if daylight ON requested
	                MessageHandling(SwitchedDaylightOn);
	              else
	                MessageHandling(SwitchedDaylightOff);
	            }
	          }
	        }
	      }
      }
      
      //5: check for LTC resync
      if (slicer == 5)
      {
        IIC1Write(d2ram_addr, 107); //address 107 = LTC requests resync of LTC
        LTC_sync_request=IIC1Read(d2ram_addr);
        
        if(!IIC1ErrorCode)
        {
	        if ((Ptr!=&LTC_menu[LTCSyncConfirmItem]) && (Ptr!=&LTC_menu[LTCMessageItem]))  //if not already in sync confirm menu
	        { 
	          if (LTC_sync_request&1)  //if request for LTC a resync
	          {
	            if (LTC_sync_request&2) //if confirm mode, goto menu
	            {
	              LTCNo = 0; //set selected LTC to A
	              WriteCodeLN1(0, SubmnuHdrTxt);
	              WriteCode1(LTCTxt);
	              WriteChar1('/');
	              WriteCode1(LTC_menu[LTCSyncConfirmItem].ItemTxt);
	              WriteChar1(' ');
	              WriteCode1(LTCTxt);
	              WriteChar1(' ');
	              WriteCode1(LTCSyncTxt);
	              
	              WriteCode1(CoSelectTxt);
	        
	              ClearRestLN1(); 
	                  
	              FuncState = 10;
	              FuncEscape = FALSE;
	              FuncSave = FALSE;
	              Ptr = &LTC_menu[LTCSyncConfirmItem];
	              Ptr -> DispFunct();
	            }
	            else  //else if automode, do resync
	            {
	              IIC1Write2(d2ram_addr, 54, 1); //Send '1' to GPS data register 54 (confirm syncrequest) LTC a
	              LTCNo=0;
	              FuncState=10;
	              FuncEscape = FALSE;
	              FuncSave = FALSE;
	              Ptr = &LTC_menu[LTCMessageItem];
	              
	              MessageHandling(ResyncedLTC);
	            }
	          }
	          
	          else if (LTC_sync_request&16)  //if request for LTC b resync
	          {        		
	            if (LTC_sync_request&32) //if confirm mode, goto menu
	            {
	              LTCNo = 1; //set selected LTC to A
	              WriteCodeLN1(0, SubmnuHdrTxt);
	              WriteCode1(LTCTxt);
	              WriteChar1('/');
	              WriteCode1(LTC_menu[LTCSyncConfirmItem].ItemTxt);
	              WriteChar1(' ');
	              WriteCode1(LTCTxt);
	              WriteChar1(' ');
	              WriteCode1(LTCSyncTxt);
	              WriteCode1(CoSelectTxt);
	        
	              ClearRestLN1(); 
	                  
	              FuncState = 10;
	              FuncEscape = FALSE;
	              FuncSave = FALSE;
	              Ptr = &LTC_menu[LTCSyncConfirmItem];
	              Ptr -> DispFunct();
	            }
	            else  //else if automode, do resync
	            {
	              IIC1Write2(d2ram_addr, 55, 1); //Send '1' to GPS data register 55 (confirm syncrequest) LTC b
	              LTCNo=1;
	              FuncState=10;
	              FuncEscape = FALSE;
	              FuncSave = FALSE;
	              Ptr = &LTC_menu[LTCMessageItem];
	              
	              MessageHandling(ResyncedLTC);
	            }
	         }
          }
        }
      }
      
      //7: check for div. setting and states
      if (slicer == 7)
      {
        //check daylight saving states
        IIC1Write(d2ram_addr, 108); //address 108 = LTC a+b daylight states
        LTCConfig[0].DaylightState=IIC1Read(d2ram_addr)&1;
        LTCConfig[1].DaylightState=IIC1Read(d2ram_addr)>>4;
      }
  }
    
    
    // *******************************************************************
    // HANDLE KEYBOARD SCANNING AND DECODING EVERY 20 ms
    // *******************************************************************
    if (ButtonReady())
      {
      if (ParallelRemoteActive || SerialRemoteLockoutOn)
        {
        if (ParallelRemoteActive)
          MessageHandling(ParallelLockoutMessage);
        else
          MessageHandling(SerialLockoutMessage);
        }
      else
        {
        if (ErrorWindowOn)        // If error window on..
          ErrorWindowOn = FALSE;  //  pressing a button will close display
        else
          {
          if (SerialRemoteActive && !MenuModeOn)
            Ptr = &RS232local_menu;
          else
            switch (ButtonCommand)
              {
              case UPButton:
                Ptr->UpBut();
                break;
              case DOWNButton:
                Ptr->DownBut();
                break;
              case RIGHTButton:
                Ptr->RightBut();
                break;
              case LEFTButton:
                Ptr->LeftBut();
                break;
              case EXECButton:
                Ptr->ExecBut();
                break;
              case PRESETButton:
                if (!SerialRemoteActive)
                  if (Ptr != &diagnose_submenu[DiagKeyboardItem])
                    PresetButtonSelect();
                 break;

              case OUTPUTButton:
                if (!SerialRemoteActive)
                  if (Ptr != &diagnose_submenu[DiagKeyboardItem])
                    OutputButtonSelect();
                break;

              case GENLOCKButton:
                if (!SerialRemoteActive)
                  if (Ptr != &diagnose_submenu[DiagKeyboardItem])
                    GenlockButtonSelect();
                break;
              }
            }
          Ptr->DispFunct();
        }
      }

    // *******************************************************************
    // SLICER HANDLING
    // *******************************************************************
    // The number to "slices" have been increased to 20, giving each
    //  slice free every 1/400ms, ie. 2.5 times/second

    if (++slicer > 20)              // Divide in 20 windows
      slicer = 1;

    // *******************************************************************
    // HANDLE AUTO ESCAPE FROM MENUMODE                    WINDOW 1
    // *******************************************************************
    // SelectModeOn is TRUE if we are in selecting VERICALLY, f.ex. System
    // It is not TRUE if we are selecting HORIZONTALLY, f.ex. BB1 <BB2>, etc
    if (slicer == 1)
      {
	      
      if (Flags.AutoESCEnable)
        {
        if (MenuModeOn)
          {
          if (AutoESCSignal)
            {
            AutoESCSignal = FALSE;
            AutoESCTimer  = 0;        // Reset Escape timer
            while (SelectModeOn)
              {                      // If in selection mode ..
              Ptr->EscapeFct();      //  be sure to terminate NICELY!!
              Ptr->DispFunct();
              }
            if (Flags.AutoESCToStatus)
              {
              UpToStatus();          // Update ptr to status-display
              Ptr->DispFunct();      // Display status-display
              }
            }
          }
        }
      }
    // *******************************************************************
    // HANDLE MESSAGE WINDOWS AND RS232 WINDOWS UPDATE          WINDOW 3
    // *******************************************************************
    if (slicer == 3)
      {
      // Message windows:
      // Presently only two windows messages are used: Local lockout
      //  for both parallel and serial remote.
      // RS232 windows update:
      // This "routine" is ONLY used when in serial remote. If a serial
      //  command alters the content of a variable, and this variable
      //  presently is shown in the status display, the display must be
      //  updated in order to show the new value. NO update should occur
      //  if a message window is presently displayed.

      if (MessageWindowOn)
        {                             // Message windows..
        if (MessageCntDwn-- == 0)
          {
          MessageWindowOn = OFF;      // Remove message window flag and
          Ptr->DispFunct();           //  update status-display
          }
        }
      else
        {                             // RS232 windows update..
        if (SerialRemoteActive && !MenuModeOn)
          {
          if (NewCmdExecuted)
            {                         // If a new command executed..
            NewCmdExecuted = FALSE;   //  unflag the information and
            Ptr->DispFunct();         //  update the status display
            }
          }
        }
      }

    // *******************************************************************
    // HANDLE DISPLAY ANIMATION                        WINDOW 5
    // *******************************************************************
    if (slicer == 5) 
      {
      if (UpdateEnable)
        if (--UpdateTimer == 0)
          {
          UpdateSignal = TRUE;            // Signal update ready
          UpdateTimer = UpdateFrequency;  // Reset update timer
          Ptr->DispFunct();               // Update selected display
          }
      }

    // *******************************************************************
    // FOR THE PT8637 ONLY: UPDATE STATUS & TIME INFORMATION      WINDOW 7
    // *******************************************************************
    if (slicer == 7)
      {
      // NOTE! This update is performed to make sure that any changes in the
      //  status of the TCI is displayed to the user, whether a new time
      //  update is ready or not.

      if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637))
        {
        TCIStatus = IIC1Read(time1_addr);   // Get TCI status information
        if (IIC1ErrorCode)
          ;
        else
          {
          TimeClockConfig.TCIStatus = TCIStatus;    // Update TCI status.
          BroadcastTCITime();                       // Read & broadcast TCI time.
          if (!MenuModeOn && (Ptr == &status_menu[StatusDateTimeItem]))
            Ptr->DispFunct();
          }
        }
      }

    // *******************************************************************
    // HANDLE READING THE A/D CONVERTERS                  WINDOW 9
    // *******************************************************************
    if (slicer == 9)
      {
      if (!WrFrontIIC(ADC_addr, 0x44))          // Auto increment
        GotoErrorStatus(TempADCErrorType + IICErrorCode);
      else
        {
        Volt12Meas = RdFrontIIC(ADC_addr);      // Read 12 volt A/D

        FeedTheWatchdog;

        if (!IICErrorCode)
          TempMeas = RdFrontIIC(ADC_addr);      // Read temperature A/D

        FeedTheWatchdog;

        if (!IICErrorCode)
          Volt5Meas = RdFrontIIC(ADC_addr);     // Read +5V A/D

        FeedTheWatchdog;
  
        if (!IICErrorCode)
          VoltN5Meas = RdFrontIIC(ADC_addr);    // Read -5V A/D
        
        if (IICErrorCode)
          GotoErrorStatus(VoltageADCErrorType + IICErrorCode);
        else
          if (SendContrast(LCDContrast))        // Update contrast
            GotoErrorStatus(LCD_ADCErrorType + IICErrorCode);
        }
      }

    // *******************************************************************
    // HANDLE POWER-SUPPLY MONITORING                   WINDOW 11
    // *******************************************************************
    if (slicer == 11) 
      {
      if (VoltCalib)              // If voltage has been calibrated.
        {
        // Only the -5V and the 12V will be examined as the +5V is
        //  handled by the watch dog
        // If -5 volt is out of range..
        if ((VoltN5Meas < VoltN5Min) || (VoltN5Meas > VoltN5Max))
          GotoErrorStatus(SystemErrorType+PowerN5Failure);
        else
          {                  // else if -5 volt error on..
          if (ErrorStatus == (SystemErrorType+PowerN5Failure))
            {
            ErrorStatus = 0;        //  clear error
            if (!MessageWindowOn && !MenuModeOn)
              Ptr->DispFunct();
            }
          }
        // If 12 volt is out of range..
        if ((Volt12Meas < Volt12Min) || (Volt12Meas > Volt12Max))
          GotoErrorStatus(SystemErrorType+Power12Failure);
        else
          {                         // else if 12 volt error on..
          if (ErrorStatus == (SystemErrorType+Power12Failure))
            {
            ErrorStatus = 0;        //  clear error
            if (!MessageWindowOn && !MenuModeOn)
              Ptr->DispFunct();
            }
          }
        }
      }

    // *******************************************************************
    // HANDLE TEMPERATURE CONTROL/MONITORING               WINDOW 13
    // *******************************************************************
    if (slicer == 13)
      {
      temp = TempMeas;        // Set blower speed from NTC resistor temp
      if (temp == 0)
        temp = 255;
      if (temp == oldtemp)
        tempcnt = 25;
      else
        if ((--tempcnt == 0) || (oldtemp > temp+1))
          {
          tempcnt = 25;
          oldtemp = temp;
          // MANAGE TEMPERATURE WARNINGS
          if (temp > hottemp+2)
            {                        // If colder than HOT..
            if (tempwarn != 0)
              {                      //  warning OFF
              tempwarn = 0;
              // If temperature error..
              if ((ErrorStatus == (SystemErrorType+TemperatureWarning)) ||\
                 (ErrorStatus == (SystemErrorType+TemperatureFailure)))
                {
                ErrorStatus = 0;      //  clear error
                if (!MessageWindowOn && !MenuModeOn)
                  Ptr->DispFunct();
                }
              }
            }
          else
            if (temp < panictemp)
              {                      // If warmer than TOOHOT..
              shotdown = 1;          //  shut down
              if (tempwarn != 2)
                {                    //  FAILURE to frontplate
                tempwarn = 2;
                GotoErrorStatus(SystemErrorType+TemperatureFailure);
                }
              }
            else
            if (temp < hottemp)
              {                      // If warmer than HOT..
              if (tempwarn != 1)
                {                    //  WARNING to frontplate
                tempwarn = 1;
                GotoErrorStatus(SystemErrorType+TemperatureWarning);
                }
              }
          if (temp > panictemp+10)
              if (shotdown)
                shotdown = 0;         // Not TOOHOT, cancel shut down
          // Set blower speed
          if (temp> grense)
            {
            temp = 0;                 // Cold, blower off
            if (ud1buf & 16)
              fan_off;
            grense = templimit-2;
            }
          else
            {
            grense = templimit;
            if (!(ud1buf & 16))
              fan_on;              // blower on
            }
          }
        }

    // *******************************************************************
    // HANDLE LEVEL-DETECTORS                                    WINDOW 15
    // *******************************************************************
    if (slicer == 15)
      {
      i = IIC1Read(lev1det_addr);            // 8 input here

      if (!IIC1ErrorCode)
        i += (IIC1Read(lev2det_addr) << 8);  // 1 input (LSB) BB9_OK
                                             // 1 output (MSB)
      i    &= 0x1FF;                         // Mask out level bits
      error = IIC1ErrorCode;                 // IIC specific error, nothing 
                                             // to do with level error, i holds
      if (!error)                            // info about level errors
        {
        if (AESEBUUnit.Present && (AESEBUUnit.HWType == PT8635))
          {
          i += ((IIC2Read(sound2_addr) & 0x70) << 5);    // 3 input
          if (!IIC2ErrorCode)
            i += ((IIC2Read(sound4_addr) & 0x70) << 8);  // 3 input
          if (!IIC2ErrorCode)
            {
            if ((i & 0x7E00) != 0x7E00)  // V441B AND gate control (changeover)
              IIC1Write(lev2det_addr, 0x7F);  // low
            else
              IIC1Write(lev2det_addr, 0xFF);  // hi
            }
          error = IIC2ErrorCode;
          }
        else                          // If AESEBU unit not present
          i |= 0x7E00;                //  set bit to level OK for AESEBU unit (hi)
        }
      if (error)                      // If IIC specific error, nothing to do with level error
        GotoErrorStatus(LevelDetectorErrorType + error); /* #define LevelDetectorErrorType 150 */
      else                                               /* in message.c */
        {                       /* no IIC specific error... */
        if (i != LevelStatus)
          {
          LevelStatus = i;            // Save LevelStatus
          if (LevelStatus ^ 0x7FFF)
            GotoErrorStatus(SystemErrorType + LevelDetectorError); /* #define SystemErrorType 0*/
          else
            {
            if (ErrorStatus == (SystemErrorType + LevelDetectorError))
              {
              ErrorStatus = 0;
              if (!MessageWindowOn && !MenuModeOn)
                Ptr->DispFunct();
              }
            }
          }
        }
      }
  
    // *******************************************************************
    // HANDLE SPG GENLOCKED/UNLOCKED UPDATE               WINDOW 17
    // *******************************************************************
    if (slicer == 17)
      {
      if (ActiveGenlockNo != Internal)
        {
/* Function RecInt() returns:
    received int, if all goes well,
    0x5555 if not all goes well, in which case error will contain
    TxV24Timeout, TxV24Checksum, RxV24Timeout or RxV24Checksum.
     So function GotoErrorStatus(UI) gets info that this is SPG communication
     error and the kind of serial communication error.
         #define OSCErrorType 80

#define V24Ok           0
#define TxV24Timeout    1
#define TxV24Checksum   2
#define RxV24Timeout    3 gives: NoResponseFromTxt
#define RxV24Checksum   4 gives: ErrorReadingFromTxt
*/

        /* This is the only case where status is requested from SPG */
        /* (apart from the case when PT8610 is present)             */
        /* Only two bits in SPG status are used:                    */
        /* syncstatus.7: burstLockOn, and                           */
        /* syncstatus.0: Locked                                     */
        i = RecInt(sync_addr, "GS", 0xFF, &error);  // Send request
        if (error)
          {
	          syncreadstat++;
        /* it is here we end when changing delay in HD too fast!!! ??? */
#if 0
        sprintf(tbuff, "OSCErrorType  error %d\n", (int)error);
        Udr_PutNChar(tbuff);
#endif
			if(syncreadstat>3)
          		GotoErrorStatus(OSCErrorType + error);
          }
        else
        {
          syncstatus = (UC) i;
          syncreadstat=0;
        }
          
        i = syncstatus & 0x08;      // Mask out burstlock flag
        /* if burstLockOn                */
        /*   GENLOCKED                   */
        /* else                          */
        /*   SYNCLOCKED                  */
        if ((i && !burstLockOn) || (!i && burstLockOn))
          {
          burstLockOn = !burstLockOn;
          i = 1;              // Set flag "UPDATE STATUS"
          }
        else
          i = 0;              // Clear flag "UPDATE STATUS"

        //Handle GPS Lock status
        if (GPSGenlockUnit.Present && (ActiveGenlockNo == GPSGenlock))
          { //    sync                    GPS connection             PLL locked
          if (((syncstatus & 0x01) && (GPS_master_status & 1) && (GPS_master_status & 2)) ^ !UnlockedLED)
            {           //only change LED when syncstatus differs from LED status
            UnlockedLED = !UnlockedLED;
            UpdateLEDs();
            i = 1;              // Set flag "UPDATE STATUS"
            }
          }
        else
          if ((syncstatus & 0x01) ^ !UnlockedLED)  //only change LED when syncstatus differs from LED status
            {
            UnlockedLED = !UnlockedLED;
            UpdateLEDs();
            i = 1;              // Set flag "UPDATE STATUS"
            }
        if (i && (!MessageWindowOn && !MenuModeOn)) // UPDATE STATUS ??????
          Ptr->DispFunct();
        }  // if (ActiveGenlockNo != Internal)
      else
        {   
        UnlockedLED = OFF;
        UpdateLEDs();
        }
        
      if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637))
        {
        if (!UnlockedLED && (ActiveGenlockNo < SDIGenlock) &&
            (  GenlockConfig[ActiveGenlockNo].System < SDI625Lock))
          TimeClockConfig.TCIControl |= 0x01;
        else
          TimeClockConfig.TCIControl &= 0xFE;
        TrxTimeClockControl(TimeClockConfig.TCIControl);
        }
      }

    // *******************************************************************
    // HANDLE PARALLEL REMOTE INTERFACE, IF PRESENT           WINDOW 19
    // *******************************************************************
    if (slicer == 19)
      {
      if (ParallelRemotePresent)
        {
        // 8574    Remote Connector
        // 0   ->  pin 1 (LSB)    Preset no
        // 1   ->  pin 2
        // 2   ->  pin 3 (MSB)
        // 3   ->  pin 4        0: Genlock active    1: Preset active
        // 4   ->  pin 6        0: External genlock  1: Internal genlock
        // 7   ->  pin 9        0: Remote enable     1: Remote disable
        i = IIC1Read(ttl_addr) & 0x9F;
        if (i != ParallelRemoteStatus)
          {
          ParallelRemoteStatus = i;
          if (ParallelRemoteStatus & 0x80)
            ParallelRemoteActive = OFF;
          else
            ParallelRemoteActive = ON;
          if (ParallelRemoteActive)
            {
            if (MenuModeOn)
              {
              if (SelectModeOn)
                {                    // If in selection mode..
                Ptr->EscapeFct();    //  be sure to terminate NICELY!!
                Ptr->DispFunct();
                }
              UpToStatus();        // Update ptr to status-display
              Ptr->DispFunct();    // Display status-display
              }
            if (ParallelRemoteStatus & 0x08)
              {                             // Preset active
              if ((i = (UC) (ParallelRemoteStatus & 0x07)) < NoOfPresets)
                {
                ActivePresetNo = (UC) i;
                ConfigurePreset(ActivePresetNo++);
                StatusNdx = StatusPresetItem;
                }
              }
            else
              {                            // Genlock active
              if (ParallelRemoteStatus & 0x10)
                ActiveGenlockNo = Internal;
              else
                ActiveGenlockNo = ExternalGenlockNo;
              GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
              StatusNdx = StatusGenlockItem;
              }
            PrevPtr = Ptr;
            Ptr = &status_menu[StatusNdx];
            if (!MessageWindowOn)
              Ptr->DispFunct();
            }
          }
        i = 0xFF;              // Set Genlock & warning output

        if (WarningLED)
          i &= 0xBF;

        if (UnlockedLED)
          i &= 0xDF;
        IIC1Write(ttl_addr, i);
        }
      }
    }    /* endless loop */
  }

/***************************************************************************/
/* void transparent_mode(UC ident)                                  MAS.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 18.08.2006                                                     */
/*                                                                         */
/* Purpose:  To set 5300 in 'transparent mode' for downloading FPGA code   */
/*           into PT8641 which then reprograms the FPGA                    */
/* Args:     UC ident: carries combined infor on board type and number.    */
/*           Left nibble is board type:                                    */
/*            1 - HD board PT8612                                          */
/*            2 - DL board PT8613                                          */
/*           Right nibble is board number (1, 2 or 3).                     */
/* Remarks:  This function is entered in response to user commanding the   */
/*           SystemUpload via SCPI, using :SYST:UPL n;                     */
/*           where n is ident.  Previously unused function                 */
/*           SystemUpload() is used for this purpose                       */
/*                                                                         */
/* Returns:  Nothing                                                       */
/*                                                                         */
/* Notes:                                                                  */
/***************************************************************************/
void transparent_mode(UC ident)
  {
  UC board;
  UC bnumber;
  char tmp[3];

  FeedTheWatchdog;

  transparent = 1; /* signal to serial interrupt routines that we are in   */
                   /* 'transparent mode'                                   */

  WriteCharLN2(0, ' ');
  ClearRestLN2();
  WriteCodeLN2(8, HDDownloadTxt);

  board   = ident >> 4;
  bnumber = ident & 0x0F;
 
  switch (board)
    {
    case 1:
      strcpy(tmp, "UN"); /* PT8612 illegal conversion between pointer types (warning)*/
      break;
    case 2:
      strcpy(tmp, "VN"); /* PT8613  illegal conversion between pointer types (warning)*/
      break;
    }

  switch (bnumber)
    {
    case 1:
      MRES1_off;                /* Release reset 1 */
      MRES2_on;
      MRES3_on;
      SndInt(0x11, tmp, 1);    /* Set JTAG mode  illegal conversion between pointer types (warning)*/
      break;
    case 2:
      MRES2_off;                /* Release reset 2 */
      MRES1_on;
      MRES3_on;
      SndInt(0x12, tmp, 1);    /* Set JTAG mode  illegal conversion between pointer types (warning)*/
      break;
    case 3:
      MRES3_off;                /* Release reset 3 */
      MRES2_on;
      MRES1_on;
      SndInt(0x13, tmp, 1);    /* Set JTAG mode  illegal conversion between pointer types (warning)*/
      break;
    }

  EA  = 0;
  IEH = 0;                    // Disable ALL interrupts
  IEL = 0;

  TR2 = 0;                    // Stop timer 2
  T2CON &= 0xCF;              // Use timer 1 as baud rate generator for both
  T2MOD &= 0xCF;              //  serial ports.
#if 1
  S0CON = 0x50;               // UART mode 1: 1 start, 8 data, 1 stop,0x50;
  S1CON = 0x50;               // UART mode 1: 1 start, 8 data, 1 stop,0x50;
#else
  S0CON = 0xF8;               // UART mode 1: 1 start, 9 data, 1 stop,
                              //  (9th bit used as stop bit)
  S1CON = 0xF8;               // UART mode 1: 1 start, 9 data, 1 stop,
#endif

  S0STAT = 0;
  S1STAT = 0;

  TR1 = 0;                    // Stop Timer 1, just in case you know
  TMOD &= 0x0F;               // Timer 1: 16-bit auto-reload timer

  TH1 = RTH1 = 0xFF;          // 115200 baud
  TL1 = RTL1 = 0xFC;
  TR1 = 1;                    // Start Timer 1

  RI0 = 0;                    // Clear possible Interrupt flag RI_0
  TI0 = 0;                    // Clear possible Interrupt flag TI_0
  RI1 = 0;                    // Clear possible Interrupt flag RI_1
  TI1 = 0;                    // Clear possible Interrupt flag TI_1


  ET0 = 0;      // Disable timer0 intrpt
  ET1 = 0;      // Disable timer1 intrpt

  ETI0 = 0; /* changed */     // Disable SIO0 Tx-interrupt
  ETI1 = 0; /* changed */     // Disable SIO1 Tx-interrupt

  EX0  = 0;                   // Disable CTS interrupt

  ERI0 = 1;                   // Enable  SIO0 Rx-interrupt
  ERI1 = 1;                   // Enable  SIO1 Rx-interrupt
  EA   = 1;

  /* Transparent Mode: UART1 communicates with Boards        */
  /*                   UART0 communicates with outside world */
  /* Everything comming from outside goes to Boards          */
  /* Everything comming from Boards goes to outside world    */

  /*    TxD0 = RxD1;   */
  /*    TxD1 = RxD0;   */


  while (1)
    {
    FeedTheWatchdog;
    }
  }

/***************************************************************************/
/* void update_sw(void)                                             MAS.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 18.08.2006                                                     */
/*                                                                         */
/* Purpose:  To download PT5300 software (software update) and program     */
/*           Master's Flash memory with it.                                */
/* Args:     None                                                          */
/*           Right nibble is board number (1, 2 or 3).                     */
/* Remarks:  This function is entered in response to user commanding the   */
/*           SystemUpload via SCPI, using :SYST:UPL n;                     */
/*           where n is (int)0x8000.  Previously unused function           */
/*           SystemUpload() is used for this purpose, and also for         */
/*           tackling HD and DL boards update (transparent_mode).          */
/*           See: SystemUpload() in RS232CMD.c                             */
/*                                                                         */
/* Returns:  Nothing                                                       */
/*                                                                         */
/* Notes:                                                                  */
/***************************************************************************/
void update_sw()
  {
  void far (*boot)(void);

  WriteCharLN2(0, ' ');
  ClearRestLN2();
  WriteCodeLN2(8, SWDownloadTxt);// Write Selftest in progress

  UserTimer = 20;          // Set timer to 380-400ms (20 orig)
  while (UserTimer)        // Short delay for the eye
    FeedTheWatchdog;

  EA  = 0;
  IEH = 0;                    // Disable ALL interrupts
  IEL = 0;

  TR2 = 0;                    // Stop timer 2
  T2CON &= 0xCF;              // Use timer 1 as baud rate generator for both
  T2MOD &= 0xCF;              //  serial ports.

  S0CON = 0x50;               // UART mode 1: 1 start, 8 data, 1 stop,0x50;
  S1CON = 0x50;               // UART mode 1: 1 start, 8 data, 1 stop,0x50;

  S0STAT = 0;
  S1STAT = 0;

  TR1 = 0;                    // Stop Timer 1, just in case you know
  TMOD &= 0x0F;               // Timer 1: 16-bit auto-reload timer

  TH1 = RTH1 = 0xFF;          // 115200 baud
  TL1 = RTL1 = 0xFC;
  TR1 = 1;                    // Start Timer 1 

  RI0 = 0;                    // Clear possible Interrupt flag RI_0
  TI0 = 0;                    // Clear possible Interrupt flag TI_0
  RI1 = 0;                    // Clear possible Interrupt flag RI_1
  TI1 = 0;                    // Clear possible Interrupt flag TI_1


  ET0 = 0;      // Disable timer0 intrpt
  ET1 = 0;      // Disable timer1 intrpt

  ETI0 = 0; /* changed */     // Disable SIO0 Tx-interrupt
  ETI1 = 0; /* changed */     // Disable SIO1 Tx-interrupt

  EX0  = 0;                   // Disable CTS interrupt

  boot = (void far code *)0x70000;
  
#if 1
  (*boot)();
#else
  update();
#endif

  while (1)               // Loop forever (BR is 115200)
    FeedTheWatchdog;

  }

