/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 2000                 */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:     SW5230.H                                                   */
/*  Author:     Kim Engedahl, DEV                                          */
/*  Org. date:  980208                                                     */
/*  Rev. date:  17.03.2006, JK                                             */
/*  Status:     Version 2.8, support for TLG                               */
/*              version 3.2, HDTPG support                                 */
/*              version 4.5: DL board support,27.11.2006                   */
/*  Changes:                                                               */
/* 160408: Released as 4008 002 08157									   */
/* 080408: Released as 4008 002 08156					   				   */
/* 080208: Released as 4008 002 08155					   				   */
/* 000508: Released as 4008 002 07331                                      */
/* 991026: Released as 4008 002 06819                                      */
/* 990421: Released as 4008 002 06817                                      */
/* 990304: Released as 4008 002 06816                                      */
/* 981126: Released as 4008 002 06815                                      */
/* 9809xx: Released as 4008 002 06814                                      */
/* 980825: Released as 4008 002 06813                                      */
/* 980616: Released as 4008 002 06812                                      */
/* 980514: Released as 4008 002 06811                                      */
/* Changes:                                                                */
/* 60: Moved genlock IIC write to GenlockUnitDrv to fix preset and boot    */
/* 59: Minor corrections of menus and NTP concerned subjects               */
/* 58: Added support for NTP ethernet time and remote module			   */
/* 57: Added LTC remote support. Corrected error when reading *IDN?		   */
/* 56: GPS data not shown, when PT8616 module not installed				   */
/* 55: Added GPS status in main menu. Corrected "unlocked LED" (displayed  */
/*     unlocked for GPS even though other genlock source was selected	   */
/* 54: Support for GPS Genlock Module PT8616                               */
/* 53: DL Audio levels modified to 0 - -60 dBFS                            */
/* 52: sF names corrected.....                                             */
/*     Function add_genlock_delay() handles PAL and NTSC and SDI genlocks  */
/*     correctly.                                                          */
/* 51: Timing for HD and TLG and DL changed.Bugs in SCPI hd timing         */
/*     corrected. Audio Leves changed. SCPI support for alle HD gens.      */
/* 50: Remote routines changed for new HD and DL specs.                    */
/* 49: HD systems added, HD system is sent not as an index but as true     */
/*     value required by FPGA using a table.  HD Audio Level changed.      */
/*     Now 61 levels.  All HD timing changed to new specs.  When changing  */
/*     system, delay is NOT set to zero.				   */
/* 48: Some changes in HD timing made by Thomas 24.05.2007 *SUM=BC99)      */
/*     Released as 00208112.bin on 11.06.2007                              */
/* 47: SCPI routines for TLG, HD and DL written and tested                 */
/* 46: HD timing changed (no F), SCPI functions for HD.                    */
/* 45: DL boar support, Genlock Timing implemented, firmware download      */
/*     feature, level and timing error handling on HD/DL.                  */
/* 40: 32 K RAM and extended HD generator support                          */
/* 41: Delays before releasing Resets made much longer, otherwise HD       */
/*     boards could not be found in proper locations                       */
/* 42: Additional Patterns and Audio items in HD generator.                */
/* 43: Text implemented in HD generator (17.05.2006)                       */
/* 44: Small changes    in HD generator (23.05.2006)                       */
/* 45: Small changes defaults in HD generator (27.06.2006)                 */
/* 50: firmware upload for  HD generator (25.07.2006)                      */
/* 71: 5 extra formats in TLG,presets in DL, text edit corrected...        */
/***************************************************************************/

#define PT5230SWVersion 60          // Software version
