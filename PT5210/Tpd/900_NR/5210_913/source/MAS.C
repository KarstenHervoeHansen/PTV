/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PT5210 VariTime digital sync generator, master          */
/*  Module:    MAS.C                                      */
/*  Author:    Preben Christiansen/Kim Engedahl, DEV                */
/*  Org. date:  960828                                    */
/*  Rev. date:  991110, KEn DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*    void waitms(UI time)                                */
/*    void waitus(UI time)                                */
/*    interrupt void Timer_int(void)                          */
/*    void main(void)                                    */
/*                                                  */
/*  Changes:                                            */
/* 991110 New timer variabel for display: DispTimer                */
/* 991008 Master Reset function is no longer supported              */
/* 991008 Added support for two AES-EBU generators, (PT8635)          */
/* 990614 Error in masking of bit in parallel remote corrected.        */
/* 980915 Added bit butstLockOn.                              */
/* 980616 Added support for -15dB level in PT8635                  */
/* 980616 Implemented support for PT8635                        */
/* 980609 The fan is now switched ON during internal test            */
/*       New RAMCheck routines. Added RAMCheckArea                */
/* 971127 Added parameter val to RecInt() & RecLong                */
/* 971023 All requests are replaced by calls to RecInt, RecLong or RecChar  */
/* 971013 Added initialization of RAMCheckOK_addr in FACT- & MASTER-RESET  */
/* 971010 Added parameters in call to Udr_InitializeUart()            */
/* 971007 Added bit variable RS232DiagnoseMode                    */
/* 970919 Optimized/altered keyboard/button handling.                */
/*       Added bit UpdateSignal                              */
/* 970917 Added parameter in calls to routine GetIICVersion()          */
/*       SW reset MRES1-MRES3 moved to beginning of main(). DO NOT MOVE  */
/* 970909 SPG now has it's own connector: OSCConn                  */
/* 970828 Minor changes in error handling                        */
/* 970821 Redesign of level detector variables & handling            */
/* 970723 Added NewCmdExecuted. Seriously altered the keyboard decoding    */
/*       routine. Major change in display update: Ptr->DispShow() &      */
/*       Ptr->DispUpdate has now been replaced by one function:        */
/*       Ptr->DispFunct();                                */
/* 970722 Added PrevPtr to Ptr in remote control intialization         */
/*        Added correct menu termination when initializing parallel remote  */
/*       control                                        */
/* 970717 Added variables for -5V & 12V power supply limits            */
/*      Added reading the stored limits for power supply during startup  */
/* 970703 Added monitoring of the -5V & 12V power supply              */
/* 970701 Minor changes in WINDOW 8. Added power supply monitoring      */
/* 970226 Error handling in startup/initialization altered            */
/* 970211 Reversed output of genlock status on parallel remote          */
/* 970211 Errorhandling reading the genlock status                  */
/* 970203 Added a dummy black burst unit                        */
/* 970202 Major changes especially to the error handling system        */
/* 970130 Unlocked LED was NOT updated when Internal2 genlock unit selected*/
/* 970130 ID of TimeCode unit now BEFORE GenlockUnitDrv initialization    */
/* 970124 Added ADC conversion variables                        */
/* 970115 Moved feed_dog definition to MAS.H                      */
/* 970113 Added Parallel Remote Control                        */
/* 970113 Added detection of oven (PT8610)                      */
/* 950112 Added ErrorStatus and detection of remote control type        */
/* 950108 Several small modifications made.                      */
/* 961212 Time after releasing reset to the units have been extended to    */
/*       250ms,ie. the BBs and SBs                            */
/* 961002 Recognition of options.                            */
/*      (Anal.Test + SDI Test + Multi-BB are still missing)          */
/* 960710 V24 addresses now sent to units                        */
/* 960214 Created from mas.c in PM5655 to be used in PT5210            */
/* 020627 Used in version with moving text on SDI (913) JK                 */
/***************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <xa.h>
#include <intrpt.h>

#include "mas.h"

#include "piic.h"
#include "iniic.h"
#include "serial1.h"
#include "52sync.h"
#include "sw5210.h"
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


#define xtal 29491200        // Crystal frequency for the mainboard


// *************************************************************************
// PORT CONTROL:
// RAM has address-space 0-0x1fff
// *************************************************************************
volatile UC  ind1 @ 0x2001;    // input port 1
volatile UC  ind2 @ 0x4001;    // input port 2

volatile UC  ud1 @ 0x2001;      // output port 1
volatile UC  ud2 @ 0x4001;      // output port 2
volatile UC  ud3 @ 0x6001;      // output port 3
volatile UC  ud4 @ 0x8001;      // output port 4
volatile UC  ud5 @ 0xA001;      // output port 5

near UC ud1buf;            // buffer for output port 1
near UC ud4buf;            // buffer for output port 4
near UC ud5buf;            // buffer for output port 5


// *************************************************************************
// CONTROL OF OPTIONAL UNITS:
// Resets:
// *************************************************************************
#define    MRES1_on  { ud4buf &= ~0x20; ud4 = ud4buf;}
#define    MRES1_off  { ud4buf |= 0x20; ud4 = ud4buf;}
#define    MRES2_on  { ud4buf &= ~0x40; ud4 = ud4buf;}
#define    MRES2_off  { ud4buf |= 0x40; ud4 = ud4buf;}
#define    MRES3_on  { ud4buf &= ~0x80; ud4 = ud4buf;}
#define    MRES3_off  { ud4buf |= 0x80; ud4 = ud4buf;}


// *************************************************************************
// CONTROL OF FAN (BLOWER)
// *************************************************************************
#define fan_on    { ud1buf |= 16; ud1 = ud1buf;}
#define fan_off  { ud1buf &= ~16; ud1 = ud1buf;}
 
near int temp;            // Temperature
near UI oldtemp, grense;     // Temperature stores
near UC tempcnt;          // Counter for speed change filter
bit shotdown;            // Flag for very hot apparatus
near UC tempwarn;          // Flag for hot apparatus


// *************************************************************************
// LEVEL DETECTOR
// *************************************************************************
UI LevelStatus;          // Status of level detectors 1-9, (bit 8-0)


// *************************************************************************
// A/D CONVERSION VARIABLES
// *************************************************************************
near UC TempMeas;          // Temperature read from AD converter
near UC Volt5Meas;        // 5 volt read from AD converter

near UC VoltN5Meas;        // -5 volt read from AD converter
UC VoltN5Min;            // Min. value for -5 volt
UC VoltN5Max;            // Max. value for -5 volt

near UC Volt12Meas;        // 12 volt read from AD converter
UC Volt12Min;            // Min. value for 12 volt
UC Volt12Max;            // Max. value for 12 volt

bit VoltCalib;            // 1: Power has been calibrated


// *************************************************************************
// SYSTEM VARIABLES
// *************************************************************************
near UC syncstatus;
near int RS232Char;
near UC slicer;          // Time window counter
bit Timer_flag;          // Control bit for 21 ms timing


// *************************************************************************
// FACTORY VALUES, IE. VALUES BEING DOWNLOADED DURING FACTORY PRESET
// *************************************************************************
code struct ANLBLK_STRUCTURE AnlBlkConfigFactory[NoOfAnlBlkUnits] = {
  // System, ScHPhase, Delay

  { PAL, 0, 0},          // Black Burst 1
  { PAL, 0, 0},          // Black Burst 2
  { PAL, 0, 0},          // Black Burst 3
  { PAL, 0, 0},          // Black Burst 4
  { PAL, 0, 0},          // Black Burst 5
  { PAL, 0, 0},          // Black Burst 6
  { PAL, 0, 0},          // Black Burst 7
  { PAL, 0, 0}          // Black Burst 8
};

code struct SDIBLK_STRUCTURE SDIBlkConfigFactory[NoOfSDIBlkUnits] = {
  // System, Pattern, EDH, EMBAudio, Delay

  { SDI625, SBBlack, OFF, OFF, 0},    // Serial Black 3-4
  { SDI625, SBBlack, OFF, OFF, 0},    // Serial Black 5-6
  { SDI625, SBBlack, OFF, OFF, 0}    // Serial Black 7-8
};

code struct ANALOG_STRUCTURE AnalogConfigFactory = {
  PAL,                // TV System
  EBUCBar,              // Test signal pattern
  OFF,                // Text insertion ON/OFF
  "ANALOG  ",            // Text to be inserted
  0,                  // ScH-Phase
  0                  // Delay
};

code struct SDI_STRUCTURE SDIConfigFactory = {
  SDI625,              // TV System
  EBUCBar,              // Test signal pattern
  OFF,                // Text insertion ON/OFF
  "DIGITAL ",            // Text to be inserted
  OFF,                // EDH insertion ON/OFF
  AudioSignalOff,        // Embedded audio signal
  AudioSilence,          // Embedded audio level
  0                  // Delay
};

code struct AES_EBU_STRUCTURE AES_EBUConfigFactory[NoOfAES_EBUUnits] = {
  // AES-EBU signal, AES-EBU level, AES-EBU Timing

  { AES_EBUStereo800Hz, AES_EBUSilence, AES_EBUPAL },  // AES/EBU 1
  { AES_EBUStereo800Hz, AES_EBUSilence, AES_EBUPAL }    // AES/EBU 2
};

code struct GENLOCK_STRUCTURE GenlockConfigFactory[NoOfGenlockUnits] = {
  { BurstLockPAL, 0 },      // Genlock A
  { BurstLockPAL, 0 },      // Genlock B
  { BurstLockPAL, 0 },      // Loop through
  { SDI625Lock, 0 },      // Option PT8607 SDI Genlock
  { NA, 0 },            // Internal genlock
  { NA, 0 }            // Option PT8610 Internal OCXO genlock
};

code struct RS232_STRUCTURE RS232ConfigFactory = {
  UDR_BAUD_9600,          // Baudrate
  UDR_DATA8,            // Databit
  UDR_NONE,            // Parity
  UDR_RTS_CTS            // Handshake
};


// *************************************************************************
// STATIC VARIABLES
// *************************************************************************

struct CONN_STRUCTURE Connectors[NoOfConnectors] = {
  // Present, HWType, HWVersion, KUNumber, SWVersion

  { TRUE, NA, 0, 0, 0},        // Mainboard, ALWAYS present
  { TRUE, NA, 0, 0, 0},        // OSC, (ie. SPG), ALWAYS present
  { TRUE, NA, 0, 0, 0},        // BB 12, ALWAYS present
  { FALSE, 0, 0, 0, 0},        // BB34 or SB34
  { FALSE, 0, 0, 0, 0},        // BB56 or SB78
  { FALSE, 0, 0, 0, 0},        // BB78 or SB78 or Analog test signal
  { FALSE, 0, 0, 0, 0},        // SDI Genlock unit
  { FALSE, 0, 0, 0, 0},        // AES/EBU sound unit
  { FALSE, 0, 0, 0, 0},        // SDI Test signal
  { FALSE, 0, 0, 0, 0}          // Time code unit
};

struct PNP_STRUCTURE AnlBlkUnit[NoOfAnlBlkUnits] = {
  { TRUE, bbu0_addr},
  { TRUE, bbu0_addr},
  { FALSE, bbu1_addr},
  { FALSE, bbu1_addr},
  { FALSE, bbu3_addr},
  { FALSE, bbu3_addr},
  { FALSE, bbu2_addr},
  { FALSE, bbu2_addr}
};

struct PNP_STRUCTURE SDIBlkUnit[NoOfSDIBlkUnits] = {
  { FALSE, bbu1_addr},
  { FALSE, bbu3_addr},
  { FALSE, bbu2_addr}
};

struct PNP_STRUCTURE AnalogSignalUnit = {
  FALSE, AnlSig_addr
};

struct PNP_STRUCTURE SDISignalUnit = {
  FALSE, SDISig_addr
};

struct PNP_STRUCTURE AES_EBUUnit = {
  FALSE, NA
};

struct PNP_STRUCTURE SDIGenlockUnit = {
  FALSE, NA
};

struct PNP_STRUCTURE Int2GenlockUnit = {
  FALSE, NA
};

struct PNP_STRUCTURE TimeCodeUnit = {
  FALSE, NA
};


// *************************************************************************
// DYNAMIC VARIABLES, IE. VARIABLES "CONTINUOSLY" BEING UPDATED.
// *************************************************************************

persistent struct ANLBLK_STRUCTURE AnlBlkConfig[NoOfAnlBlkUnits];
persistent struct SDIBLK_STRUCTURE SDIBlkConfig[NoOfSDIBlkUnits];
persistent struct ANALOG_STRUCTURE AnalogConfig;
persistent struct SDI_STRUCTURE SDIConfig;
persistent struct AES_EBU_STRUCTURE AES_EBUConfig[NoOfAES_EBUUnits];
persistent struct GENLOCK_STRUCTURE GenlockConfig[NoOfGenlockUnits];
persistent struct RS232_STRUCTURE RS232Config;
persistent struct PRESET_STRUCTURE Preset[NoOfPresets];
persistent struct STATUS_FLAGS Flags;

persistent UC ActivePresetNo;    // Currently active preset, (0 = NONE)

persistent UC ExternalGenlockNo;  // Last selected EXTERNAL genlock
persistent UC ActiveGenlockNo;  // Currently active genlock

persistent UC ActiveOutputNo;    // Currently active output, ie. OUTPUT BUTTON

persistent UC LCDContrast;      // Contrast on LCD-display

persistent UC PasswordState;    // OFF: Apparatus is NOT password protected

persistent UC RAMCheckArea[24];  // Area used for PowerUp RAM check

code struct MENU_ITEM* near Ptr;
code struct MENU_ITEM* near PrevPtr;

UC ErrorCode;              // Temporary error code, 8 BIT
                      // 8 BIT
                      // ErrorType+ErrorNo

UI ErrorStatus;            // Last active global error code, 16 BIT
                      // 8 MSB   8 LSB
                      // [index]  ErrorType+ErrorNo

UL ErrorQueue[NoInErrorQueue] = {// Global error code queue, 32 BIT
  0, 0, 0, 0, 0            // 16 MSB      8 BIT    8 LSB
};                      // leveldetector  [Index]  ErrorType+ErrorNo

UC ErrorQueuePtr = NoInErrorQueue-1;
bit ErrorWindowOn;          // 1: indicates display of an "error" window

bit MessageWindowOn;          // 1: indicates display of a "message" window
near UC MessageCntDwn;        // Counter for display of message

volatile near UC SecTimer;      // Seconds timer: updated every second
volatile near UC AutoESCTimer;  // Timer for auto return to status display
volatile near UC UserTimer;    // Generel user timer: updated every 20ms
volatile near UC DispTimer;    // Specific display timer: updated every 20ms
volatile near UC DisplayTimer;  // Error display timer: updated every 20ms

UC UpdateTimer;            // Timer for display animation
UC UpdateFrequency;          // Frequency for the above
bit UpdateEnable;            // 1: Display animation is active
bit UpdateSignal;            // 1: Display animatino caused the call

bit AutoESCSignal;          // 1: Return to status display

bit Do_Keyboard_Scan;        // 1:  Scan the keyboard

bit TestModeEnable;          // 1: Enable test mode

bit MenuModeOn;            // 1: Menu mode active
bit SelectModeOn;            // 1: Selection mode active

bit Warning_LED;            // 1: Illumination of the WARNING LED
bit Unlocked_LED;            // 1: Illumination of the UNLOCKED LED
bit Genlock_LED;            // 1: Illumination of the GENLOCK LED

bit burstLockOn;            // 1: if burst available in genlock
                      //  systems BurstLockPAL & BurstLockNTSC.

bit SerialRemotePresent;      // 1: Remote cable mounted in serial conn.
bit SerialRemoteActive;        // 1: Serial remote active
bit SerialRemoteLockoutOn;      // 1: Serial remote lockout (ctrl-L)

bit ParallelRemotePresent;      // 1: Remote cable mounted in parallel conn.
bit ParallelRemoteActive;      // 1: Parallel remote active
UC ParallelRemoteStatus;

bit RS232DiagnoseMode;        // 1: RS232 Loopback test in progress

/***************************************************************************/
/*  waitms                                         MAS.C  */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 960828                    */
/* Revised:    960828                                       */
/*                                                  */
/*  Function:  A <n>ms wait routine where n is user definable          */
/*  Remarks:    This is a software based loop. Altering the crystal or    */
/*          tampering with the waitstates might have serious influence  */
/*          on the timing                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void waitms(UI time) {        // Longer wait routine
  UI n;

  while (time-- > 0) {      // Wait 1 ms x time
    n = 750;
    while (n > 0)
      n--;
  }
}

/***************************************************************************/
/*  waitus                                         MAS.C  */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 960828                    */
/* Revised:    960828                                       */
/*                                                  */
/*  Function:  A <n>us wait routine where n is user definable          */
/*  Remarks:    This is a software based loop. Altering the crystal or    */
/*          tampering with the waitstates might have serious influence  */
/*          on the timing                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void waitus(UI time) {        // Short wait routine

  while (time-- > 0)
    ;                  // Wait 1.1 us x time
}

/***************************************************************************/
/*  VECTOR DEFINITIONS                                 MAS.C  */
/*                                                  */
/* Interrupts:T0 prior:     9  bank 1                          */
/*    Ser0_rec     15   -   0                                */
/*    Ser1_rec  -  15   -   0                                */
/*    Ser0_trm  -   ?   -   0                                */
/*    Ser1_trm  -   ?   -   0                                */
/***************************************************************************/
ROM_VECTOR(IV_T0, Timer_int, IV_SYSTEM+IV_PRI09+IV_BANK1)
ROM_VECTOR(IV_RI0, Udr_Uart_RX_Interrupt, IV_PSW) 
ROM_VECTOR(IV_TI0, Udr_Uart_TX_Interrupt, IV_PSW)
ROM_VECTOR(IV_EX0, Udr_CTS_Interrupt, IV_PSW)
ROM_VECTOR(IV_RI1, Serial1_int, IV_PSW)

/***************************************************************************/
/*  Timer_int                                       MAS.C  */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 960828                    */
/* Revised:    991110, KEn, DEV                                 */
/*                                                  */
/*  Function:  Interrupt routine which handles ALL timing            */
/*  Remarks:    Changing the crystal WILL affect the timing            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
interrupt void Timer_int(void) {

  static near UC ticker;

  TH0 = -xtal/256/4/3/50;        // Load timer to 50 Hz (20ms)

   if (++ticker > 2) {
     ticker = 0;
      Timer_flag = 1;

    if (!--SecTimer)  {        // This routine is executed each second.
      SecTimer = 50;          // Reload seconds timer.

      if (Flags.AutoESCEnable)            // If auto escape is ON
        if (++AutoESCTimer > AutoESCTimeOut)  // and time-out reached
          AutoESCSignal = TRUE;          // set time-out flag
    }

    if (UserTimer)          // UserTimer is decremented each 20 ms.
      UserTimer--;

    if (DispTimer)          // DispTimer is decremented each 20 ms.
      DispTimer--;

    if (DisplayTimer)        // DisplayTimer is decremented each 20 ms.
      DisplayTimer--;

    Do_Keyboard_Scan = TRUE;    // Set flag for keyboard-scan
  }
}

/***************************************************************************/
/*  FactoryPreset                                     MAS.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980609                        */
/* Revised:    991008, KEn, DEV                                 */
/*                                                  */
/*  Function:  Handle copying the variables in a factory preset.          */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryPreset() {

  UI i;
  UC j;

  for (i = 0; i < 16; i++) {
    if (i % 2 )
      RAMCheckArea[i] = 0xAA;
    else
      RAMCheckArea[i] = 0x55;
  }

  // CONFIGURE PRESET 1-8 (-2) TO PAL

  for (j = 0; j < 17; j++)
    Preset[0].Name[j] = ' ';

  Preset[0].Name[16] = 0;

  for (j=0; j < NoOfAnlBlkUnits; j++) {
    Cmemcpy(&Preset[0].AnlBlkStore[j], &AnlBlkConfigFactory[j], sizeof(AnlBlkConfig[0]));
    Cmemcpy(&AnlBlkConfig[j], &AnlBlkConfigFactory[j], sizeof(AnlBlkConfig[0]));

    if (j < NoOfSDIBlkUnits) {
      Cmemcpy(&Preset[0].SDIBlkStore[j], &SDIBlkConfigFactory[j], sizeof(SDIBlkConfig[0]));
      Cmemcpy(&SDIBlkConfig[j], &SDIBlkConfigFactory[j], sizeof(SDIBlkConfig[0]));
    }
  }
  Cmemcpy(&Preset[0].AnalogStore, &AnalogConfigFactory, sizeof(AnalogConfig));
  Cmemcpy(&AnalogConfig, &AnalogConfigFactory, sizeof(AnalogConfig));

  Cmemcpy(&Preset[0].SDIStore, &SDIConfigFactory, sizeof(SDIConfig));
  Cmemcpy(&SDIConfig,  &SDIConfigFactory, sizeof(SDIConfig));

  for (j=0; j < NoOfAES_EBUUnits; j++) {
    Cmemcpy(&Preset[0].AES_EBUStore[j], &AES_EBUConfigFactory[j], sizeof(AES_EBUConfig[0]));
    Cmemcpy(&AES_EBUConfig[j], &AES_EBUConfigFactory[j], sizeof(AES_EBUConfig[0]));
  }

  for (j=0; j < NoOfGenlockUnits; j++) {
    Cmemcpy(&Preset[0].GenlockStore[j], &GenlockConfigFactory[j], sizeof(GenlockConfig[0]));
    Cmemcpy(&GenlockConfig[j], &GenlockConfigFactory[j], sizeof(GenlockConfig[0]));
  }

  Preset[0].ActiveGenlockNo = ActiveGenlockNo = Internal;
  Preset[0].ExternalGenlockNo = ExternalGenlockNo = GenlockA;


  for (i=1; i < NoOfPresets; i++)
    memcpy(&Preset[i], &Preset[0], sizeof(Preset[0]));


  // CONFIGURE PRESET 2 TO NTSC, IE. CHANGE ONLY THE DIFFERENCES

  for (j=0; j < NoOfAnlBlkUnits; j++) {
    Preset[1].AnlBlkStore[j].System = NTSC;

    if (j < NoOfSDIBlkUnits)
      Preset[1].SDIBlkStore[j].System = SDI525;
  }

  Preset[1].AnalogStore.System = NTSC;
  Preset[1].AnalogStore.Pattern = SMPTECBar;

  Preset[1].SDIStore.System = SDI525;
  Preset[1].SDIStore.Pattern = SDISMPTECBar;

  for (j=0; j < NoOfAES_EBUUnits; j++)
    Preset[1].AES_EBUStore[j].Timing = AES_EBUNTSCPhase1;

  for (i=0; i < SDIGenlock; i++)
    Preset[1].GenlockStore[i].System = BurstLockNTSC;

  Preset[1].GenlockStore[SDIGenlock].System = SDI525Lock;

  Cmemcpy(&RS232Config, &RS232ConfigFactory, sizeof(RS232Config));

  LCDContrast = Def_Contrast;

  ActiveOutputNo = AES_EBU1Item;
  ActivePresetNo = 0;

  Flags.NormalLockOn = OFF;
  Flags.PanelLockOn = OFF;
  Flags.DownloadLockOn = OFF;
  Flags.DiagnoseLockOn = OFF;

  Flags.AutoESCEnable = ON;
  Flags.AutoESCToStatus = ON;

  PasswordState = ON;

  IIC2_Write2(mainram1_addr, RAMCheckOK_addr, TRUE);
}

/***************************************************************************/
/*  main                                           MAS.C  */
/*                                                  */
/* Author:    Preben Christiansen, DEV, 960828                    */
/* Revised:    991008, KEn, DEV                                 */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void main(void) {

  UI i;
  UC error;
  char TextBuffer[10];

// System set-up
  BCR = 0x02;    // Set 20 bit BUS, 8 bit DATA
  PSWH = 0x8F;  // Set system priority high
  BTRH = 0xAE;  // To be trimmed (was 0xef) EXT. BUS timing, Data Cycle
  BTRL = 0xCE;  // To be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
  PSWL = 0;    // Clear flags
  SCR = 0;      // Timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  PSWH &= 0xF0;  // Set system priority low

   MRES1_on;    // Reset all options NB! DO NOT MOVE!!!!!
   MRES2_on;    //       do          NB! DO NOT MOVE!!!!!
  MRES3_on;    //       do          NB! DO NOT MOVE!!!!!

// Serial set-up
  S1CON = 0xF2;  // Transmit mode 3,  9 bit UART,  Address mode, Rec.enable
  S1ADDR = master_addr;  // receive address
  S1ADEN = 0xCC;  // mask for receive address
            // receive address = 00010000 AND 11001100 = 00XX00XX (0 to 33)

// Timer set-up
  TMOD = 0x21;  // Timer 1: 8 bit autoload; Timer 0: 16 bit count
  TH0 = -255;    // Start value
  T2MOD = 0x31;  // Use T2 for V24_1, count up
  T2CAPH = -1;  // Set reload values
  T2CAPL = -6;
  TH2 = -1;    // Set for max. Baud
  TL2 = -6;
  TR2 = 1;      // Start V24-timer

// Interrupt setting
   IT0 = 1;      // Edge trig on ext0
   IT1 = 1;      // Edge trig on ext1
  EX0 = 0;      // Disable ext0
  ET0 = 1;      // Enable timer0 intrpt
  ET1 = 0;      // Disable timer1 intrpt

// NB! IPAx PRIORITETS-SETTING SKAL MATCHE SETTING I ROM_VECTOR() !!!
// (Brug kun 9-F, 0 ell. 8 er intrpt off!)
   IPA0= 0x9f;        // 0x76 prior. for Timer0/EX0 
   IPA4= 0xff;        // 0xbb serial 0 intrpt.
   IPA5= 0xff;        // 0xaa serial 1 intrpt.

  Serial1_init();    // Initialize internal serial v24 bus

  feed_dog;        // Watch-dog feed

  Rd_Front_IIC(0xF0);  // Initialize front IIC bus by reading dummy
  IIC1_Read(0xF0);    // Initialize IIC bus 1 by reading dummy
  IIC2_Read(0xF0);    // Initialize IIC bus 2 by reading dummy

                // Initialize level detector ports
  IIC1_Write(lev1det_addr, 0xFF);
  IIC1_Write(lev2det_addr, 0xFF);

  ud4buf &= 0xF8;    // Disable display
  ud4 = ud4buf;

  TR0 = 1;          // Start loop timer
  TR1= 1;          // Start V24-timer

   EA = 1 ;          // Enable all interrupts

  fan_on;          // Turn blower on while performing internal test

  oldtemp = 255;      // Initiate temperature control
  tempwarn = 0;
  slicer = 0;

  SecTimer = 50;      // Seconds timer: 50 gives a 1 second timer, ie. 5*20ms
  UpdateTimer = 0;    // Update timer used for animation. Reset to 0

  AutoESCSignal = 0;  // 1: returnSignal to escape from menu to display
  UpdateEnable = 0;    // 1: Animation enabled
  UpdateSignal = 0;    // 1: The call to the routine caused by animation

  Old_Button_Command = Button_Command = NO_Command;

  ParallelRemotePresent = FALSE;
  ParallelRemoteActive = FALSE;
  ParallelRemoteStatus = 0xFF;

  SerialRemotePresent = FALSE;
  SerialRemoteActive = FALSE;
  SerialRemoteLockoutOn = FALSE;  // Disable serial remote lockout

  RS232DiagnoseMode = FALSE;      // 

  TestModeEnable = OFF;

  feed_dog;            // Watch-dog feed

  // **********************************************************************
  // Reset all units. There are 3 resets to options, MRES1 to MRES3.
  //  In order to identify the position of the options, one reset is
  //  released at a time and an option address is send to this option.
  // If the option then answers back its address, the Master then
  //  nows the presence of this option.
  //  Do allow all units to do their own initialisation.
  // Variable 'options' (for Test purposes)  has a bit for each option.
  // **********************************************************************

  UserTimer = i = 10;      // Reset for 80-100 ms
  while (UserTimer)
    if (i == UserTimer) {
      Init_Display();    // Initialize display, this is done 4 or 5 times
      feed_dog;        // Watch-dog feed
      i--;
    }

  i = ~ind1;              // Detect type of remote connector

  if (i & SerialRemoteMask)    // RS232 remote installed
    SerialRemotePresent = TRUE;

  if (i & ParallelRemoteMask)  // Parallel remote installed
    ParallelRemotePresent = TRUE;

  Warning_LED = ON;          // Switch ON all LEDs during power-up
  Unlocked_LED = ON;
  Genlock_LED = ON;

  Update_LEDs();

  Warning_LED = OFF;        // Switch OFF all LEDs after LED testing
  Unlocked_LED = OFF;
  Genlock_LED = OFF;

  TempMeas = 128;          // Intialize TempMeasurement to dummy
  Init_Contrast();          // Initialize the display contrast

                      // Test buttons during power-on
  switch (Rd_Front_IIC(keyb_addr) & Keyb_Mask) {
    case FACTORY_Button:
      WriteCodeLN1(0, PowerUp_FactResetTxt);

      FactoryPreset();
      break;

    default:
      WriteCodeLN1(0, PowerUp_DefaultTxt);
      break;
  }
  ClearRestLN1();

  GetIICVersion(IICBus2, mainram1_addr, ProductKUNo_addr, &Connectors[MainConn].KUNumber);
  Connectors[MainConn].SWVersion = PT5210SWVersion;

  UserTimer = 20;          // Set timer to 380-400ms

  IIC2_Write(mainram1_addr, VoltCalib_addr);    // Get powersupply limits
  VoltCalib = (IIC2_Read(mainram1_addr) == ON);  // Calibration
  VoltN5Min = IIC2_Read(mainram1_addr);        // Min. -5 volt
  VoltN5Max = IIC2_Read(mainram1_addr);        // Max. -5 volt
  Volt12Min = IIC2_Read(mainram1_addr);        // Min. 12 volt
  Volt12Max = IIC2_Read(mainram1_addr);        // Max. 12 volt

  Wr_Front_IIC(ADC_addr, 0x44);            // ADC. Auto increment
  Volt12Meas = Rd_Front_IIC(ADC_addr);        // Read 12 volt A/D
  TempMeas = Rd_Front_IIC(ADC_addr);          // Read temperature A/D
  Volt5Meas = Rd_Front_IIC(ADC_addr);        // Read +5V A/D
  VoltN5Meas = Rd_Front_IIC(ADC_addr);        // Read -5V A/D
        

  while (UserTimer)        // Short delay for the eye
    feed_dog;            //  feed the watch-dog

  WriteCharLN2(0, ' ');
  ClearRestLN2();
  WriteCodeLN2(8, SelfTestTxt);// Write Selftest in progress

  if (!PowerUpRAMCheckOK())  {  // If RAM test fails..
    FactoryPreset();        //  perform a factory preset
    DisplayErrorLine(RAMMemoryErrorType, ErrorDisplayTimeout);
  }

  MRES1_off;              // Release reset 1
  UserTimer = 11;          // Set timer to 200-220ms
  feed_dog;              // Watch-dog feed

                      // Initialize the remote RS232
  Udr_InitializeUart(RS232Config.Baudrate, RS232Config.Databit,\
                 RS232Config.Parity, RS232Config.Handshake);

                      // Initialize mainboard BB's
  for (AnlBlkNdx = BB1Item; AnlBlkNdx < BB3Item; AnlBlkNdx++)
    if (ErrorCode = ConfigureAnlBlkUnit(AnlBlkNdx))
      DisplayErrorLine(ErrorCode, ErrorDisplayTimeout);

  while (UserTimer)        // While waiting for BBU_1 unit to initialize
    feed_dog;            //  feed the watch-dog

  SndChar(default_addr, "HA", bbu1_addr);
  waitms(10);            // Wait for an answer from the unit
  i = S1BUF;

  MRES2_off;              // Release reset 2
  UserTimer = 13;          // Set timer to 240-260ms

  if (i == bbu1_addr)
    {
    for (i = BB3Item; i < BB5Item; i++)
      {
      AnlBlkUnit[i].Present = TRUE;

      if (ErrorCode = ConfigureAnlBlkUnit(i))
        DisplayErrorLine(256*i + ErrorCode, ErrorDisplayTimeout);
      }
    }
  else
    if (i == bbu1_addr+64)
      {
      SDIBlkUnit[SB34Item].Present = TRUE;

      if (ErrorCode = ConfigureSDIBlkUnit(SB34Item))
        DisplayErrorLine(256*SB34Item + ErrorCode, ErrorDisplayTimeout);
      }

  while (UserTimer)        // While waiting for BBU_2 unit to initialize
    feed_dog;            //  feed the watch-dog

                      // Change V24 address on unit
  SndChar(default_addr, "HA", bbu2_addr);
  waitms(10);            // Wait for an answer from the unit
  i = S1BUF;

  MRES3_off;              // Release reset 3
  UserTimer = 13;          // Set timer to 240-260ms

  if (i == bbu2_addr) {
    for (i = BB7Item; i < BB8Item+1; i++) {
      AnlBlkUnit[i].Present = TRUE;

      if (ErrorCode = ConfigureAnlBlkUnit(i))
        DisplayErrorLine(256*i + ErrorCode, ErrorDisplayTimeout);
    }
  }
  else
    if (i == bbu2_addr+64) {
      SDIBlkUnit[SB78Item].Present = TRUE;

      if (ErrorCode = ConfigureSDIBlkUnit(SB78Item))
        DisplayErrorLine(256*SB78Item + ErrorCode, ErrorDisplayTimeout);
    }

  while (UserTimer)        // While waiting for BBU_3 unit to initialize
    feed_dog;            //  feed the watch-dog

                      // Change V24 address on unit
  SndChar(default_addr, "HA", bbu3_addr);
  waitms(10);            // Wait for an answer from the unit
  i = S1BUF;

  if (i == bbu3_addr) {
    for (i = BB5Item; i < BB7Item; i++) {
      AnlBlkUnit[i].Present = TRUE;

      if (ErrorCode = ConfigureAnlBlkUnit(i))
        DisplayErrorLine(256*i + ErrorCode, ErrorDisplayTimeout);
    }
  }
  else
    if (i == bbu3_addr+64) {
      SDIBlkUnit[SB56Item].Present = TRUE;

      if (ErrorCode = ConfigureSDIBlkUnit(SB56Item))
        DisplayErrorLine(256*SB56Item + ErrorCode, ErrorDisplayTimeout);
    }

  feed_dog;                        // Watch-dog feed
                                
  if (SndChar(AnlSig_addr, "AI", 0)==v24ok)  // PT8601 Analog test signal
    if (ErrorCode = ConfigureAnalogSignalUnit())
      DisplayErrorLine(ErrorCode, ErrorDisplayTimeout);

  feed_dog;                        // Watch-dog feed

  if (SndChar(SDISig_addr, "SI", 0)==v24ok)   // PT8602/PT8603
    if (ErrorCode = ConfigureSDISignalUnit())
      DisplayErrorLine(ErrorCode, ErrorDisplayTimeout);

  feed_dog;                        // Watch-dog feed

  if (IIC1_Write(multibb_addr, 0xff)) {    // Option PT8604 Multi BB
    Connectors[BB34Conn].Present = TRUE;
    Connectors[BB34Conn].HWType = PT8604;

    GetIICVersion(IICBus1, multibb_addr, ProductKUNo_addr, &Connectors[BB34Conn].KUNumber);
  }

  feed_dog;                        // Watch-dog feed

  if (IIC2_Write(sound1_addr, 0xff)) {      // Option PT8605/35
    AES_EBUUnit.Present = TRUE;          // AES/EBU audio generator

    Connectors[AESConn].Present = TRUE;
    Connectors[AESConn].HWType = PT8605;

    if (IIC2_Write(sound3_addr, 0xFF))     // Option PT8635
      Connectors[AESConn].HWType = PT8635;

    GetIICVersion(IICBus2, soundram_addr, ProductKUNo_addr, &Connectors[AESConn].KUNumber);

    ErrorCode = AES_EBUUnitDrv(0, AES_EBUConfig[0].Signal, AES_EBUConfig[0].Level, AES_EBUConfig[0].Timing);

    if (!ErrorCode && (Connectors[AESConn].HWType == PT8635))
      ErrorCode = AES_EBUUnitDrv(1, AES_EBUConfig[1].Signal, AES_EBUConfig[1].Level, AES_EBUConfig[1].Timing);

    if (ErrorCode)
      DisplayErrorLine(ErrorCode, ErrorDisplayTimeout);
  }

  feed_dog;                        // Watch-dog feed

  if (IIC1_Write(d1ram_addr, 0xff))  {      // Option PT8606
    SDIGenlockUnit.Present = TRUE;        // SDI Genlock unit

    Connectors[SDIGConn].Present = TRUE;
    Connectors[SDIGConn].HWType = PT8606;

    GetIICVersion(IICBus1, d1ram_addr, ProductKUNo_addr, &Connectors[SDIGConn].KUNumber);
  }
  else {
    if (ActiveGenlockNo == SDIGenlock)
      DisplayErrorLine(SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
  }

  feed_dog;                        // Watch-dog feed
                                // Oven unit PT8610
  i = RecInt(sync_addr, "GS", 0xFF, &ErrorCode);

  if (ErrorCode == 0) {
    syncstatus = (UC) i;

    if (syncstatus & OvenMask)
      Int2GenlockUnit.Present = TRUE;
    else
      if (ActiveGenlockNo == Internal2)
        DisplayErrorLine(SystemErrorType + ConfigurationError, ErrorDisplayTimeout);
  }
  else                          // Error for SPG
    DisplayErrorLine(OSCErrorType + ErrorCode, ErrorDisplayTimeout);

  feed_dog;                      // Watch-dog feed

  if (IIC1_Write(time_addr, 0xff)) {    // Option PT8607
    TimeCodeUnit.Present = TRUE;        // Time code unit, (MUST initia-
                              //  lize BEFORE GenlockUnitDrv)
    Connectors[TimeConn].Present = TRUE;
    Connectors[TimeConn].HWType = PT8607;

    GetIICVersion(IICBus1, timeram_addr, ProductKUNo_addr, &Connectors[TimeConn].KUNumber);
  }

  feed_dog;                      // Watch-dog feed

  if (ErrorCode = GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System))
    DisplayErrorLine(ErrorCode, ErrorDisplayTimeout);

  feed_dog;                      // Watch-dog feed

                              // Download calibration data to SPG
  if (ErrorCode = ConfigureSPG())
    DisplayErrorLine(OSCErrorType + ErrorCode, ErrorDisplayTimeout);

  // **********************************************************************
  // RECALL OLD SETTINGS
  // **********************************************************************
  grense = templimit - 2;        // Limit for blower (high temp.)
  feed_dog;

  Update_LEDs();

  UserTimer = 75;            // Start delay for at least 1.5 second

  if (!ROMCheckOK(&i))        // PROM Cheksum test
    DisplayErrorLine(ROMMemoryErrorType+ChecksumError, ErrorDisplayTimeout);

  if (!ErrorStatus) {
    WriteCodeLN2(8, InternalTestTxt);
    ClearRestLN2();

    while (UserTimer)          // Simulates delay for internal test
    feed_dog;
  }

  UserTimer = 100;            // Start delay 2 second

  BaseNdx = 0;              // Reset base menu
  AnlBlkNdx = 0;              // Reset last active Analog Black menu item

                        // Reset last active SDI Black menu item
  for (SDIBlkNdx = 0; SDIBlkNdx < NoOfSDIBlkUnits; SDIBlkNdx++)
    if (SDIBlkUnit[SDIBlkNdx].Present)
      break;
                        // Reset last active Analog Black items
  for (i = 0; i < NoOfAnlBlkUnits; i++)
    BBNdx[i] = 0;
                        // Reset last active SDI Black items
  for (i = 0; i < NoOfSDIBlkUnits; i++)
    SBNdx[i] = 0;

  AnlSigNdx = 0;               // Reset last active Analog-signal item
  SDISigNdx = 0;                // Reset last active SDI-signal item

  AES_EBUnNdx = 0;              // Reset last active AES-EBU out item
                        // Reset last active AES-EBU items
  for (i = 0; i < NoOfAES_EBUUnits; i++)
    AES_EBUNdx[i] = 0;

  PresetNdx = 0;                // Reset last active Preset item
  ConfigNdx = 0;                // Reset last active Config item
  RS232Ndx = 0;               // Reset last active RS232 item
  DiagnoseNdx = 0;             // Reset last active Diagnose item

  while (UserTimer)          // Delay to show internal test passed
    feed_dog;

  WriteCodeLN1(0, PT5210HeadlineTxt);
  ClearRestLN1();

  GetPT5210SWVersion(TextBuffer);
  WriteCharLN2(0, ' ');
  ClearToPosLN2(8);
  WriteCode2(VersionHdrTxt);    // Write software version
  WriteTxt2(TextBuffer);
  ClearRestLN2();

  UserTimer = 250;            // Delay 5.0 second or until key pressed
  while (UserTimer)
    if (Button_Ready())
      break;

  Send_Contrast(LCDContrast);    // Send contrast

  MenuModeOn = SelectModeOn = OFF;

  if (ErrorStatus)            // If any errors during powerup
    StatusNdx = StatusErrorItem;  //  start in Error/Warnings display
  else {
    if (ActivePresetNo)
      StatusNdx = StatusPresetItem;
    else
      StatusNdx = StatusGenlockItem;
  }

  Ptr = &status_menu[StatusNdx];  // Prepare pointer for status display
  Ptr->DispFunct();            // Display status-display

  // **********************************************************************
  // ENDLESS LOOP
  // **********************************************************************

  while(TRUE) {

    while(!Timer_flag) {      // While waiting for timer flag..

      // ****************************************************************
      // HANDLE SERIAL REMOTE INTERFACE, IF PRESENT
      // ****************************************************************
      if (SerialRemotePresent) {  // If serial remote interface enabled..

        if (!RS232DiagnoseMode)  // Do not respond if in diagnose mode
          while ((RS232Char = Udr_InChar()) != UDR_EOF) {
            CmdHandler((UC) RS232Char);

            if (SerialRemoteActive == FALSE) {
              SerialRemoteActive = TRUE;    // Set serial remote flag
              SetupRS232GotoLocalMessage();  // Prepare for "Goto.." display
            }
          }
      }
    }
    Timer_flag = 0;                  // 20 ms timer expired

    // *******************************************************************
    // HANDLE KEYBOARD SCANNING AND DECODING EVERY 20 ms
    // *******************************************************************
    if (Button_Ready()) {

      if (ParallelRemoteActive || SerialRemoteLockoutOn) {
        if (ParallelRemoteActive)
          MessageHandling(ParallelLockoutMessage);
        else
          MessageHandling(SerialLockoutMessage);
      }
      else {
        if (ErrorWindowOn)      // If error window on..
          ErrorWindowOn = FALSE;  //  pressing a button will close display
        else {
          if (SerialRemoteActive && !MenuModeOn)
            Ptr = &RS232local_menu;
          else
            switch (Button_Command) {
              case UP_Button:
                Ptr->UpBut();
                break;

              case DOWN_Button:
                Ptr->DownBut();
                break;

              case RIGHT_Button:
                Ptr->RightBut();
                break;

              case LEFT_Button:
                Ptr->LeftBut();
                break;

              case EXEC_Button:
                Ptr->ExecBut();
                break;

              case PRESET_Button:
                if (!SerialRemoteActive)
                  if (Ptr != &diagnose_submenu[DiagKeyboardItem])
                    PresetButtonSelect();
                break;

              case OUTPUT_Button:
                if (!SerialRemoteActive)
                  if (Ptr != &diagnose_submenu[DiagKeyboardItem])
                    OutputButtonSelect();
                break;

              case GENLOCK_Button:
                if (!SerialRemoteActive)
                  if (Ptr != &diagnose_submenu[DiagKeyboardItem])
                    GenlockButtonSelect();
                break;
            }
        }
        Ptr->DispFunct();
      }
    }
    feed_dog;

    // *******************************************************************
    // SLICER HANDLING
    // *******************************************************************
    if (++slicer > 10)              // Divide in 10 windows
      slicer = 1;

    // *******************************************************************
    // HANDLE AUTO ESCAPE FROM MENUMODE                    WINDOW 1
    // *******************************************************************
    if (slicer == 1) {

      if (Flags.AutoESCEnable) {
        if (MenuModeOn) {
          if (AutoESCSignal) {

            AutoESCSignal = FALSE;
            AutoESCTimer = 0;        // Reset Escape timer
      
            if (SelectModeOn) {      // If in selection mode ..
              Ptr->EscapeFct();      //  be sure to terminate NICELY!!
              Ptr->DispFunct();
            }

            if (Flags.AutoESCToStatus) {
              UpToStatus();        // Update ptr to status-display
              Ptr->DispFunct();      // Display status-display
            }
          }
        }
      }
    }

    // *******************************************************************
    // HANDLE MESSAGE WINDOWS AND RS232 WINDOWS UPDATE          WINDOW 2
    // *******************************************************************
    if (slicer == 2) {

      // Message windows:
      // Presently only two windows messages are used: Local lockout
      //  for both parallel and serial remote.

      // RS232 windows update:
      // This "routine" is ONLY used when in serial remote. If a serial
      //  command alters the content of a variable, and this variable
      //  presently is shown in the status display, the display must be
      //  updated in order to show the new value. NO update should occur
      //  if a message window is presently displayed.

      if (MessageWindowOn) {          // Message windows..
        if (MessageCntDwn-- == 0) {
          MessageWindowOn = OFF;      // Remove message window flag and
          Ptr->DispFunct();          //  update status-display
        }
      }
      else {                    // RS232 windows update..
        if (SerialRemoteActive && !MenuModeOn) {
          if (NewCmdExecuted) {      // If a new command executed..
            NewCmdExecuted = FALSE;    //  unflag the information and
            Ptr->DispFunct();        //  update the status display
          }
        }
      }
    }

    // *******************************************************************
    // HANDLE DISPLAY ANIMATION                        WINDOW 3
    // *******************************************************************
    if (slicer == 3) { 
    
      if (UpdateEnable)
        if (--UpdateTimer == 0) {
          UpdateSignal = TRUE;          // Signal update ready
          UpdateTimer = UpdateFrequency;  // Reset update timer

          Ptr->DispFunct();            // Update selected display
        }
    }

    // *******************************************************************
    // HANDLE READING THE A/D CONVERTERS                  WINDOW 4
    // *******************************************************************
    if (slicer == 4) {

      if (!Wr_Front_IIC(ADC_addr, 0x44))      // Auto increment
        GotoErrorStatus(TempADCErrorType + IICErrorCode);
      else {
        Volt12Meas = Rd_Front_IIC(ADC_addr);    // Read 12 volt A/D

        feed_dog;

        if (!IICErrorCode)
          TempMeas = Rd_Front_IIC(ADC_addr);    // Read temperature A/D

        feed_dog;

        if (!IICErrorCode)
          Volt5Meas = Rd_Front_IIC(ADC_addr);  // Read +5V A/D

        feed_dog;
  
        if (!IICErrorCode)
          VoltN5Meas = Rd_Front_IIC(ADC_addr);  // Read -5V A/D
        
        if (IICErrorCode)
          GotoErrorStatus(VoltageADCErrorType + IICErrorCode);
        else {
          if (Send_Contrast(LCDContrast))    // Update contrast
            GotoErrorStatus(LCD_ADCErrorType + IICErrorCode);
        }
      }
      }

    // *******************************************************************
    // HANDLE POWER-SUPPLY MONITORING                    WINDOW 5
    // *******************************************************************
    if (slicer == 5) { 

      if (VoltCalib) {              // If voltage has been calibrated.

        // Only the -5V and the 12V will be examined as the +5V is
        //  handled by the watch dog

                              // If -5 volt is out of range..
        if ((VoltN5Meas < VoltN5Min) || (VoltN5Meas > VoltN5Max))
          GotoErrorStatus(SystemErrorType+PowerN5Failure);
        else {                  // else if -5 volt error on..
          if (ErrorStatus == (SystemErrorType+PowerN5Failure)) {
            ErrorStatus = 0;        //  clear error

            if (!MessageWindowOn && !MenuModeOn)
              Ptr->DispFunct();
          }
        }
                              // If 12 volt is out of range..
        if ((Volt12Meas < Volt12Min) || (Volt12Meas > Volt12Max))
          GotoErrorStatus(SystemErrorType+Power12Failure);
        else {                  // else if 12 volt error on..
          if (ErrorStatus == (SystemErrorType+Power12Failure)) {
            ErrorStatus = 0;        //  clear error

            if (!MessageWindowOn && !MenuModeOn)
              Ptr->DispFunct();
          }
        }
      }
    }

    // *******************************************************************
    // HANDLE TEMPERATURE CONTROL/MONITORING                WINDOW 6
    // *******************************************************************
    if (slicer == 6) {

      temp = TempMeas;        // Set blower speed from NTC resistor temp

      if (temp == 0)
        temp = 255;

      if (temp == oldtemp)
        tempcnt = 25;
      else
        if ((--tempcnt == 0) || (oldtemp > temp+1)) {
          tempcnt = 25;
          oldtemp = temp;
                                // MANAGE TEMPERATURE WARNINGS
          if (temp > hottemp+2) {      // If colder than HOT..
            if (tempwarn != 0) {      //  warning OFF
              tempwarn = 0;
                                // If temperature error..
              if ((ErrorStatus == (SystemErrorType+TemperatureWarning)) ||\
                  (ErrorStatus == (SystemErrorType+TemperatureFailure))) {
                ErrorStatus = 0;      //  clear error

                if (!MessageWindowOn && !MenuModeOn)
                  Ptr->DispFunct();
              }
            }
          }
          else
            if (temp < panictemp) {    // If warmer than TOOHOT..
              shotdown = 1;          //  shut down
              if (tempwarn != 2) {    //  FAILURE to frontplate
                tempwarn = 2;
                GotoErrorStatus(SystemErrorType+TemperatureFailure);
              }
            }
            else
            if (temp < hottemp)  {      // If warmer than HOT..
              if (tempwarn != 1) {    //  WARNING to frontplate
                tempwarn = 1;
                GotoErrorStatus(SystemErrorType+TemperatureWarning);
              }
              }

          if (temp > panictemp+10)
              if (shotdown) {
              shotdown=0;            // Not TOOHOT, cancel shut down
            }
                                // Set blower speed
          if (temp> grense) {
            temp = 0;              // Cold, blower off
            if (ud1buf & 16)
              fan_off;
            grense = templimit-2;
          }
          else {
            grense = templimit;
            if (!(ud1buf & 16))
              fan_on;              // blower on
          }
        }
    }

    // *******************************************************************
    // HANDLE LEVEL-DETECTORS                          WINDOW 7
    // *******************************************************************
    if (slicer == 7) {

      i = IIC1_Read(lev1det_addr);          // 8 input

      if (!IIC1ErrorCode)
        i += (IIC1_Read(lev2det_addr) << 8);  // 1 input (LSB)
                                  // 1 output (MSB)
      i &= 0x1FF;                      // Mask out level bits

      error = IIC1ErrorCode;

      if (!error) {
        if (Connectors[AESConn].Present && (Connectors[AESConn].HWType == PT8635)) {
          i += ((IIC2_Read(sound2_addr) & 0x70) << 5);    // 3 input

          if (!IIC2ErrorCode)
            i += ((IIC2_Read(sound4_addr) & 0x70) << 8);  // 3 input

          if (!IIC2ErrorCode) {
            if ((i & 0x7E00) != 0x7E00)
              IIC1_Write(lev2det_addr, 0x7F);
            else
              IIC1_Write(lev2det_addr, 0xFF);
          }
                
          error = IIC2ErrorCode;
        }
        else                      // If AESEBU unit not present
          i |= 0x7E00;              //  set bit to level OK
      }

      if (error)                      // If IIC Circuit error ..
        GotoErrorStatus(LevelDetectorErrorType + error);
      else {
        if (i != LevelStatus) {
          LevelStatus = i;              // Save LevelStatus

          if (LevelStatus ^ 0x7FFF)
            GotoErrorStatus(SystemErrorType + LevelDetectorError);
          else {
            if (ErrorStatus == (SystemErrorType + LevelDetectorError)) {
              ErrorStatus = 0;

              if (!MessageWindowOn && !MenuModeOn)
                Ptr->DispFunct();
            }
          }
        }
      }
    }

    // *******************************************************************
    // HANDLE SPG GENLOCKED/UNLOCKED UPDATE                WINDOW 8
    // *******************************************************************
    if (slicer == 8) {

      if (ActiveGenlockNo != Internal) {
                                  
        i = RecInt(sync_addr, "GS", 0xFF, &ErrorCode);  // Send request

        if (ErrorCode)
          GotoErrorStatus(OSCErrorType + ErrorCode);
        else
          syncstatus = (UC) i;

        i = syncstatus & 0x08;      // Mask out burstlock flag

        if ((i && !burstLockOn) || (!i && burstLockOn)) {

          burstLockOn = !burstLockOn;

          i = 1;              // Set flag "UPDATE STATUS"
        }
        else
          i = 0;              // Clear flag "UPDATE STATUS"

        if ((syncstatus & 0x01) ^ !Unlocked_LED) {
          Unlocked_LED = !Unlocked_LED;

          Update_LEDs();

          i = 1;              // Set flag "UPDATE STATUS"
        }

        if (i && (!MessageWindowOn && !MenuModeOn))
          Ptr->DispFunct();
      }
      else {
        Unlocked_LED = OFF;
        Update_LEDs();
      }
    }

    // *******************************************************************
    // HANDLE PARALLEL REMOTE INTERFACE, IF PRESENT            WINDOW 9
    // *******************************************************************
    if (slicer == 9) {

      if (ParallelRemotePresent) {

        // 8574    Remote Connector
        // 0   ->  pin 1 (LSB)    Preset no
        // 1   ->  pin 2
        // 2   ->  pin 3 (MSB)
        // 3   ->  pin 4        0: Genlock active   1: Preset active
        // 4   ->  pin 6        0: External genlock  1: Internal genlock
        // 7   ->  pin 9        0: Remote enable    1: Remote disable

        i = IIC1_Read(ttl_addr) & 0x9F;

        if (i != ParallelRemoteStatus) {
          ParallelRemoteStatus = i;

          if (ParallelRemoteStatus & 0x80)
            ParallelRemoteActive = OFF;
          else
            ParallelRemoteActive = ON;

          if (ParallelRemoteActive) {

            if (MenuModeOn) {
              if (SelectModeOn) {    // If in selection mode..
                Ptr->EscapeFct();    //  be sure to terminate NICELY!!
                Ptr->DispFunct();
              }

              UpToStatus();      // Update ptr to status-display
              Ptr->DispFunct();    // Display status-display
            }

            if (ParallelRemoteStatus & 0x08) {    // Preset active

              ActivePresetNo = (UC) (ParallelRemoteStatus & 0x07);

              ConfigurePreset(ActivePresetNo++);

              StatusNdx = StatusPresetItem;
            }
            else {                      // Genlock active
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

        if (Warning_LED)
          i &= 0xBF;

        if (Unlocked_LED)
          i &= 0xDF;

        IIC1_Write(ttl_addr, i);
      }
    }
  }
}
