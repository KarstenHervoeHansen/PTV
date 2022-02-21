/***************************************************************************/
/*  Copyright   DK-Audio A/S, 2002                                         */
/*  Project:    PT5202 Digital Video Generator                             */
/*  Module:     MENUTREE.C                                                 */
/*  Purpse:     To implement LCD display and keyboard into PT5201          */
/*  Author:     JK                                                         */
/*  Org. date:  19.03.2002 End on 15.05.02 (serial operation, tested)      */
/*  Rev. date:  Tested and running on 5201: 10.06.2002                     */
/*  Notes:      This module implements menu interface for 5201 using 5230  */
/*              display/keyboard hardware.  Other than it uses 2x40 chars  */
/*              display, it is hardware independent                        */
/*              and interfaces to LCD and keyboard via                     */
/*              keyb_drv.c and                                             */
/*              disp_drv.c modules                                         */
/*              To use this module, you must                               */
/*              first call the menu_init() function and then place the     */
/*              check_keys() function in your program's main loop.         */
/*              You must also provide the following functions:             */
/*              void do_BBSetup(BBObject *, UC, UC);
/*              void do_GenlockSetup(GenlockObject *, UC);
/*              void do_CBSetup(CBObject *, UC);
/*              void do_AnalogConfig(AudioObject *, UC);
/*              void do_AesebuConfig(AudioObject *, UC);
/*              void do_RecallPreset(UC);
/*              void do_StorePreset(UC);
/*              void do_NamePreset(UC, char *);
/*              void do_Calibration(UC);
/*              void do_calibration_save(void);
/*              void do_Audio_Kind(UC);
/*              void do_FactoryReset(UC);
/*              void init_configs(UC *);
/*              which implement updating of generator parameters, either   */
/*              via serial, IIC or other hardware.                         */
/***************************************************************************/
/*

Note on Genlock Calibration function.

This function allows you to calibrate SPG frequency.  Divided master clock
appears at BB1 output, where its frequency can be measured.  A set of commands 
allows you to adjust this frequency by outputting a value which alters the
SPG master clock DAC reference (V74), thus altering SPG master clock frequency.
After a 5202 leaves factory, master clock frequency is calibrated and corresponding
DAC value is stored in Calibration.GenlockCalibration.DACUserValue in NOVRAM.
Master clock frequency may be checked / recalibrated by user using the following
commands.

1. The command:

   INPUT:GENLOCK:CALIBRATE:ENABLE:ON;

   sets 5202 into calibration mode, SPG master clock divided by 6 appears on 
   BB1 output as 4.5 MHz sinusoidal signal.

2. The command:

   INPUT:GENLOCK:CALIBRATE:UP | DOWN;
   
   will change the frequency by 1 unit of Calibration.GenlockCalibration.DACUserValue.

3. The command:


   INPUT:GENLOCK:CALIBRATE:MAX | MIN;

   by increasing / decreasing the Calibration.GenlockCalibration.DACUserValue
   by 150.

4. The command:

   INPUT:GENLOCK:CALIBRATE:STORE;

   will store new value of Calibration.GenlockCalibration.DACUserValue in NOVRAM,
   thus altering master clock frequency permanently.

5. The command:

   INPUT:GENLOCK:CALIBRATE:ENABLE:OFF;

   will terminate calibration mode and black burst signal will appear on BB1 
   output.


Note:
   Master clock frequency is calibrated properly if output of BB1 has frequency
   of 45000000 Hz.

   When setting calibration enable off (5), the new value will not be remebered
   unless calibration store command (4) was issued previously.  This is useful
   for implementing a cancel function.

   Password setting does not affect the above functions.

   Genlock Calibration object is defined as follows:

        typedef struct 
          {
          UL Date;
         
          UL DACFactDate;
          UL DACUserDate;
        
          UI DACFactValue;  // factory value
          UI DACUserValue;  // value set by user
        
          UL HPHZeroG;
          UL PhaseG;
        
          UL HPHZeroM;
          UL PhaseM;
          int DACOffset; // JK
          } GenlockCalibrationObject;

  There is DACFactValue and DACUserValue.  When 5202 is tested at the factory, both
  these variables contain the same value (written by; the test program), which 
  correspond to calibrated SPG master clock frequency.  When user recalibrates master
  clock, user DAC74 value is stored in DACUserValue, DACFactValue is unchanged.
  In this way we can always find out what value was set by the test program in 
  the factory.

  These two values are retrieved by:

     :FACT:GENL:FREF:DAC?; and
     :FACT:GENL:UREF:DAC?;

  When performing calibrationl, values allowed are:

      DACFactValue +- 150
  
  i.e. user can change frequency within range of +- 150  off DACFactValue.


  When initializing genlock (SPG), DACUserValue is retirieved from NOVRAM and sent
  to the SPG.  
  Current value in SPG can be retirieved by:

     :FACT:V24C:COMM 21,'GX?';

  (password must be OFF).

The calibration menu has following elements:

                 ADJ   MIN   MAX   SAVE   ESC
FuncState =       1     2     3            4   <-- no change
FuncState =       5     6     7      8     9   <-- if ADJ used (changed)

If MIN or MAX has been issued, the SAVE option must be removed.  SAVE option
is only available if ADJ has been used (either up or down).

The following functions implement state machine and function triggers for
the calibrate menu:

void ExecButTypeCal(void);
void RightButTypeCal(void);
void LeftButTypeCal(void);
void DownButTypeCal(void);
void UpButTypeCal(void);
void WriteTypeCal_SAVE_ESC(char);



*/

#include "define.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "util.h"
#include "menutree.h"
#include "disp_02.h"
#include "keyb_drv.h"
#include "bb.h"
#include "cbar.h"
#include "audio.h"
#include "genlock.h"
#include "instru.h"
#include "mnuinter.h"
#include "rs232.h"

#define SAVE                0
#define RESTORE             1

#define stepNdxSDIMax       3
#define stepNdxSDIMin       0
#define stepNdxAnalogMax    5
#define stepNdxAnalogMin    0

#define PAL_MAX            19
#define NTSC_MAX           18

#define NoOfPresets         4

struct PATTERNS
  {
  char code * patt_text;
  UC   code   patt_no;
  } ;


/* extern config structures to hold user input parameters, defined in mnuinter.c */
extern BBObject            BBSetup[3];
extern CBObject            CBSetup;
extern AudioObject         AudioSetup[2];
extern GenlockObject       GenlockSetup;

extern _near int rec_timeout;

extern char text_on; // shows if text should be displayed or not

/* temporary object to hold original settings in order to restore them in case
   ESC is commanded                                                             */
BBObject       BBOrig[3];
CBObject       CBOrig;
AudioObject    AudioOrig[2];
GenlockObject  GenlockOrig;

// Arrays below store names of presets
char           OrgPresetNames[4][MaxPresetNameLength];
char           PresetNames[4][MaxPresetNameLength];

UC status_is_displayed; // this shows that display is displaying status
                        // and not other menus and is used to update
                        // parameters used by status display

UC rs232_touched;     // This signals that remote interface was used and 
                      // display must return to status.


code long stepsizeAnalogTable[] = 
  {
  1, 7, 69, 691, 6910, 69100
  };

code long stepsizeSDITable[] =  
  {
  256, 768, 6912, 69120
  };

// Variables for the state machine(s):

UC FuncState;    // Current state in state machine
UC FuncTriggers; // Bit 1,0:  01-> X++, 10 -> X--
                 // Bit 3,2:  01-> Y++, 10 -> Y--
                 // Bit 5,4:  01-> Z++, 10 -> Z--
                 // Bit 7,6:  01-> R++, 10 -> R--

bit FuncSave;    // if 1, do a (controlled) save
bit FuncEscape;  // if 1, do a (controlled) escape

// Indexes

static UC StatusNdx;    // Last active display item
static UC BaseNdx;      //     -"-     base-menu item
static UC AnlBlkNdx;    //     -"-     analog black menu item
static UC GenlockNdx;
static UC SdiNdx;       //     -"-     SDI-TSG menu item

static UC PresetNdx;

static UC BBNdx[3];
static UC AudioNdx[2]; 
static UC AudioTypeNdx;




/*******/

//code char PT5230HeadlineTxt[]    = "PT 5202 Dig.Vid Gen. NR";
code char PT5230HeadlineTxt[]    = "PT 5202 Compact Varitime SPG";


code char StatusHdrTxt[]          = "STATUS: ";
code char GENLOCKEDTxt[]          = "GENLOCKED";
code char UNLOCKEDTxt[]           = "UNLOCKED";
code char HyphenTxt[]             = "------";
code char SignalHdrTxt[]          = "SIGNAL: ";
code char StatusPresetTxt[]       = "PT 5202 Preset Status:";
code char PresetHdrTxt[]          = "PRESET : ";
code char NoPresetActiveTxt[]     = "No preset active";


code char CoSelectFunctionTxt[]  = ", select function";
code char CoEditNameTxt[]        = ", edit name";
code char RecallHdrTxt[]         = "RECALL : ";
code char StoreHdrTxt[]          = "STORE : ";

code char MaxTxt[]               = "MAX";
code char MinTxt[]               = "MIN";
code char UpTxt[]                = "UP";
code char DownTxt[]              = "DOWN";
code char AdjTxt[]               = "ADJ";

code char GenlockTxt[]       = "GENLOCK";
code char BlkBurstTxt[]      = "BLACK-BURST";
code char SdiTxt[]           = "SDI";
code char BB1Txt[]           = "BB1";
code char BB2Txt[]           = "BB2";
code char BB3Txt[]           = "BB3";
code char AUDIOTxt[]         = "AUDIO";
code char AnalogAudioTxt[]   = "ANALOG";
code char DigitalAudioTxt[]  = "AES/EBU";
code char PRESETTxt[]        = "PRESETS";
code char SYSTEMTxt[]        = "SYSTEM";
code char PatternTxt[]       = "PATTERN";
code char ScHPhaseTxt[]      = "ScH-PHASE";
code char TimingTxt[]        = "TIMING";
code char StoreTxt[]         = "STORE";
code char RecallTxt[]        = "RECALL";
code char NameTxt[]          = "NAME";
code char RestoreTxt[]       = "RESTORE";
code char Presets1Txt[]      = "PRESET1";
code char Presets2Txt[]      = "PRESET2";
code char Presets3Txt[]      = "PRESET3";
code char Presets4Txt[]      = "PRESET4";
code char EMBAudioTxt[]      = "EMB.AUDIO";
code char AnalogTxt[]        = "ANL.SETTINGS";
code char SignalTxt[]        = "SIGNAL";
code char FACTORY_RESETTxt[] = "RESET";
code char CALIBRATIONTxt[]   = "CALIBRATION";

code char LevelTxt[]              = "LEVEL";
code char CoModifyTxt[]           = ", modify";
code char CoConfigureTxt[]        = ", configure";
code char CoSelectTxt[]           = ", select";
code char CoEditDelayTxt[]        = ", edit delay";
code char CoEditTxt[]             = ", edit";
code char ItemTxt[]               = "Item: ";
code char MenuHdrTxt[]            = "MENU  : ";
code char AudioTxt[]              = "AUDIO : ";
code char colon_spaceTxt[]        = ": ";

code char SystemHdrTxt[]          = "SYSTEM: ";
code char ScrollHdrTxt[]          = "SCROLL: ";
code char FunctionHdrTxt[]        = "FUNC. : ";
code char PATTERNTxt[]            = "PATT. : ";
code char CLICKTxt[]              = "CLICK : ";
code char TONETxt[]               = "TONE  : ";
code char LEVELTxt[]              = "LEVEL : ";
code char WCLKTxt[]               = "WCLK  : ";
code char TimingHdrTxt[]          = "TIMING: ";
code char ScHPhaseHdrTxt[]        = "ScH-PHASE: ";
code char SubmnuHdrTxt[]          = "SUBMNU: ";
code char SecondmnuHdrTxt[]       = "2NDMNU:";
code char ThirdmnuHdrTxt[]        = "3RDMNU:";
code char TimeVHdrTxt[]           = "V: ";
code char TimeHHdrTxt[]           = "  H: ";
code char TimeTHdrTxt[]           = "  T: ";
code char NameHdrTxt[]            = "NAME : ";

code char BLACKBURSTTxt[]         = "BLACK BURST : ";
code char SBB1Txt[]               = "1:";
code char SBB2Txt[]               = "2:";
code char SBB3Txt[]               = "3:";

code char SlashDotHdrTxt[]        = "../";
code char ThreeDotsTxt[]          = "...";
code char degTxt[]                = " deg";
code char SAVETxt[]               = "SAVE";
code char ESCTxt[]                = "ESC";
code char OKTxt[]                 = "OK";
code char SPACETxt[]              = "  ";
code char pal_txt[]               =  "PAL";
code char palid_txt[]             =  "PAL w.ID";
code char ntsc7_txt[]             =  "NTSC 7.5%";
code char ntscj_txt[]             =  "NTSC 0%";
code char internal_txt[]          =  "Internal";
code char mhz_txt[]               =  "10 MHz";
code char ntsc_txt[]              =  "NTSC";

code char  off_txt[]              = "OFF";
code char  on_txt[]               = "ON";

code char  silence_txt[]          = "SILENCE";
code char  stereo1_txt[]          = "STEREO 1 kHz";

code char AnalogAudioClickTxt[]      = "CLICK";
code char AnalogAudioSignalTxt[]     = "TONE";
code char AnalogAudioLevelTxt[]      = "LEVEL";

code char DigitalAudioClickTxt[]     = "CLICK";
code char DigitalAudioSignalTxt[]    = "TONE";
code char DigitalAudioLevelTxt[]     = "LEVEL";
code char DigitalAudioTimingTxt[]    = "TIMING";
code char DigitalAudioFrequencyTxt[] = "WCLK";
code char DigitalAudioSystemTxt[]    = "SYSTEM";

code char EDITTxt[]                  = "EDIT";
code char MOVEMENTTxt[]              = "MOVEMENT";
code char StyleTxt[]                 = "STYLE";
code char PositionTxt[]              = "POS";
code char ClockTxt[]                 = "CLOCK";
code char TextTxt[]                  = "TEXT";

code char* code OffOnArrTxt[] = 
  {
  off_txt,
  on_txt 
  };

code char  stereo500_txt[]           = "STEREO 500 Hz";
code char  stereoebu_text[]          = "STEREO EBU 1kHz";
code char  stereo8_txt[]             = "STEREO 8 kHz";
code char  onesec_txt[]              = " 1 SEC";
code char  threesec_txt[]            = " 3 SEC";
code char  four4_txt[]               = "44.1 kHz";
code char  four8_txt[]               = "48 kHz";


code char  twulf[]    = "+12 dBu";
code char  ten[]      = "+10 dBu";
code char  eight[]    = "+8 dBu";
code char  seven[]    = "+7 dBu";
code char  six[]      = "+6 dBu";
code char  five[]     = "+5 dBu";
code char  four[]     = "+4 dBu";
code char  three[]    = "+3 dBu";
code char  two[]      = "+2 dBu";
code char  one[]      = "+1 dBu";
code char  zero[]     = "+0 dBu";
code char  mone[]     = "-1 dBu";
code char  mtwo[]     = "-2 dBu";
code char  mthree[]   = "-3 dBu";
code char  mfour[]    = "-4 dBu";
code char  mfive[]    = "-5 dBu";
code char  msix[]     = "-6 dBu";
code char  mseven[]   = "-7 dBu";
code char  meight[]   = "-8 dBu";
code char  mnine[]    = "-9 dBu";
code char  mten[]     = "-10 dBu";
code char  meleven[]  = "-11 dBu";
code char  twelv[]    = "-12 dBu";
code char  mfivet[]   = "-15 dBu";
code char  might[]    = "-18 dBu";
code char  mtwe[]     = "-20 dBu";
code char  mtwef[]    = "-24 dBu";
code char  mtwes[]    = "-27 dBu";
code char  mthirty[]  = "-30 dBu";
code char  mthirtyt[] = "-33 dBu";
code char  mthirtys[] = "-36 dBu";
code char  silec[]    = "SILENCE";

code char dzero[]     = "0 dBFS";
code char m9[]        = "-9 dBFS";
code char m12[]       = "-12 dBFS";
code char m15[]       = "-15 dBFS";
code char m16[]       = "-16 dBFS";
code char m18[]       = "-18 dBFS";
code char m20[]       = "-20 dBFS";
code char sil[]       = "SILENCE";
code char dummy_txt[] = "dummy";

code char MinusTxt[]  = "-";
code char PlusTxt[]   = "+";

code char SDIHdrTxt[] = "SDI: ";
code char EDHTxt[]    = "EDH";

/***************************************************************************/
/*      PRESET TEXT CHARACTER TABLE     
/***************************************************************************/
code UC PresetCharSet[128] = 
  {
// 0-24
// ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 25-49
//                      , !, ", #, $, %, &, ï, (, ), *, +, ,, -, ., /, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1,
// 50-74
//2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?, @, A, B, C, D, E, F, G, H, I, J,
  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// 75-99
//K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \, ], ^, _, `, a, b, c,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1,
// 100-124
//d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, {, |,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
// 125-127
//}, ~,
  0, 0, 0
  };


code char* code AnlSystemArrTxt[] = 
  { 
  pal_txt,
  palid_txt,
  ntsc7_txt,
  ntscj_txt 
  };

code char* code AESEBUSystemArrTxt[] = 
  { 
  pal_txt,
  ntsc_txt,
  };


code char* code GenlockSystemArrTxt[] = 
  { 
  internal_txt,
  pal_txt,
  ntsc_txt,
  mhz_txt 
  };

code char* code SdiSystemArrTxt[] = 
  { 
  pal_txt,
  ntsc7_txt,
  ntscj_txt 
  };

code char* code SdiEmbeddedArrTxt[] =
  {
  off_txt,
  silence_txt,
  stereo1_txt
  };

code char* code AnalogAudioSignalArrTxt[] =
  {
  stereo500_txt,
  stereo1_txt,
  stereoebu_text,
  stereo8_txt
  };

code char* code AnalogAudioClickArrTxt[] =
  {
  dummy_txt,
  onesec_txt,
  dummy_txt,
  threesec_txt
  };

code char* code DigitalAudioFrequencyArrTxt[] =
  {
  four4_txt,
  four8_txt
  };

#if 0
enum AnalogLevelType  // 32 
	{
	P12dBm = 0, P10dBm,	P8dBm,  P7dBm,  P6dBm,  P5dBm,  P4dBm,\
	P3dBm       P2dBm,  P1dBm,  M0dBm,  M1dBm,  M2dBm,  M3dBm,\
  M4dBm,      M5dBm,  M6dBm,  M7dBm,  M8dBm,  M9dBm,  M10dBm,\
	M12dBm,     M15dBm, M18dBm, M20dBm, M24dBm, M27dBm, M30dBm,\
	M11dBm,     M33dBm, M36dBm, AnalogSilence
  };
#endif

code char* code AnalogAudioLevelArrTxt[] = // 32
  {
  twulf,    // 0
  ten,      // 1
  eight,    // 2
  seven,    // 3
  six,      // 4
  five,     // 5
  four,     // 6
  three,    // 7
  two,      // 8
  one,      // 9
  zero,     // 10
  mone,     // 11
  mtwo,     // 12
  mthree,   // 13
  mfour,    // 14
  mfive,    // 15
  msix,     // 16
  mseven,   // 17
  meight,   // 18
  mnine,    // 19
  mten,     // 20
  meleven,  // 21
  twelv,    // 22
  mfivet,   // 23
  might,    // 24
  mtwe,     // 25
  mtwef,    // 26
  mtwes,    // 27
  mthirty,  // 28
  mthirtyt, // 29
  mthirtys, // 30
  silec     // 31
  };

code char* code DigitalAudioLevelArrTxt[] =
  {
  dzero,
  m9,
  m12,
  m15,
  m16,
  m18,
  m20,
  sil
  };

code char* code CalibrationArrTxt[] = 
  { 
  MaxTxt,
  MinTxt,
  UpTxt,
  DownTxt
  };

code char* code MinusPlusArrTxt[] =
  {
  MinusTxt,
  PlusTxt
  };



/****/
code char EBUCBarTxt[]        = "EBU Colorbar";
code char EBUCBarITUTxt[]     = "EBU Colorbar ITU801";
code char CBar100Txt[]        = "100% Colorbar";
code char CBar75RedTxt[]      = "75% Colorbar + Red";
code char Red75Txt[]          = "75% Red";
code char CCIR18Txt[]         = "CCIR18";
code char Window10Txt[]       = "10% Window";
code char Window15Txt[]       = "15% Window";
code char Window20Txt[]       = "20% Window";
code char Window100Txt[]      = "100% Window";
code char BlWh15kHzTxt[]      = "15 kHz Black/White";
code char White100Txt[]       = "White 100%";
code char BlackTxt[]          = "Black";
code char CheckFieldTxt[]     = "SDI Check Field";
code char DigGreyTxt[]        = "Digital Grey";
code char Staircase5Txt[]     = "5 step Staircase";
code char Staircase10Txt[]    = "10 step Staircase";
code char CrosshatchTxt[]     = "Crosshatch";
code char PLUGETxt[]          = "PLUGE";

/* NTSC patterns */

code char SmpteCBTxt[]        = "SMPTE Colorbar";
code char FccCBTxt[]          = "FCC Colorbar";  // this one is missing in PAL
code char EBUituTxt[]         = "EBU Colorbar ITU801";

#if 0
// Below is the table from Windows program:
code char *TSGPatternTxt[]	=
  {
  SMPTECBarTxt,
  EBUCBarTxt,
  FCCCBarTxt,
  EBUCBarITUTxt,
	CBar75ITUTxt,
  CBar100Txt,
  CBar75GreyTxt,
  CBar75RedTxt,
  Red75Txt,
  MultiburstTxt,
  CCIR18Txt,
  Window10Txt,
  Window15Txt,
  Window20Txt,
  Window100Txt,
  BlWh15kHzTxt,
  White100Txt,
  BlackTxt,
  CheckFieldTxt,
  DigGreyTxt,
  Staircase5Txt,
  Staircase10Txt,
  CrosshatchTxt,
  PLUGETxt,
  NULL
  };
#endif



struct PATTERNS code pal_patterns[19] =
  {
    {EBUCBarTxt,      1}, // 0  "EBU Colorbar"
    {EBUCBarITUTxt,   3}, // 1  "EBU Colorbar ITU801";
    {CBar100Txt,      4}, // 2  "100% Colorbar";
    {CBar75RedTxt,    6}, // 3  "75% Colorbar + Red";
    {Red75Txt,        7}, // 4  "75% Red";
    {CCIR18Txt,       9}, // 5  "CCIR18";
    {Window10Txt,    10}, // 6  "10% Window";
    {Window15Txt,    11}, // 7  "15% Window";
    {Window20Txt,    12}, // 8  "20% Window";
    {Window100Txt,   13}, // 9  "100% Window";
    {BlWh15kHzTxt,   14}, //10  "15 kHz Black/White";
    {White100Txt,    15}, //11  "White 100%";
    {BlackTxt,       16}, //12  "Black";
    {CheckFieldTxt,  17}, //13  "SDI Check Field";
    {DigGreyTxt,     18}, //14  "Digital Grey";
    {Staircase5Txt,  19}, //15  "5 step Staircase";
    {Staircase10Txt, 20}, //16  "10 step Staircase";
    {CrosshatchTxt,  21}, //17  "Crosshatch";
    {PLUGETxt,       22}  //18  "PLUGE";
  };

struct PATTERNS code ntsc_patterns[18] =
  {
    {SmpteCBTxt,      0},  // 0  "SMPTE Colorbar";
    {FccCBTxt,        2},  // 1  "FCC Colorbar";  // this one is missing in PAL
    {EBUituTxt,       3},  // 2  "EBU Colorbar ITU801";
    {Red75Txt,        7},  // 3  "75% Red";
    {CBar100Txt,      4},  // 4  "100% Colorbar";
    {Window10Txt,    10},  // 5  "10% Window";
    {Window15Txt,    11},  // 6  "15% Window";
    {Window20Txt,    12},  // 7  "20% Window";
    {Window100Txt,   13},  // 8  "100% Window";
    {BlWh15kHzTxt,   14},  // 9  "15 kHz Black/White";
    {White100Txt,    15},  //10  "White 100%";
    {BlackTxt,       16},  //11  "Black";
    {CheckFieldTxt,  17},  //12  "SDI Check Field";
    {DigGreyTxt,     18},  //13  "Digital Grey";
    {Staircase5Txt,  19},  //14  "5 step Staircase";
    {Staircase10Txt, 20},  //15  "10 step Staircase";
    {CrosshatchTxt,  21},  //16  "Crosshatch";
    {PLUGETxt,       22}   //17  "PLUGE";
  };


#if 0
Char *tsg_palm_patterns[19] =
  {
  "EBU Colorbar",
  "EBU ITU801",
  "100%Colorbar",
  "75%Colorbar+Red", // this one is missing in NTSC
  "75%Red",
  "CCIR118", // this one is missing in NTSC
  "10%Window",
  "15%Window",
  "20%Window",
  "100%Window",
  "15 kHz Black/White",
  "White 100%",
  "Black",
  "SDI Check Field",
  "Digital Gray",
  "5 step Staircase",
  "10 step Staircase",
  "Crosshatch",
  "PLUGE"
  };            // 19 strings ok
Char *tsg_palm_ntsc_patterns[18] =
  {
  "SMPTE Colorbar",
  "FCC Colorbar",  // this one is missing in PAL
  "EBU Colorbar ITU801",
  "100%Colorbar",
  "75%Red",
  "10%Window",
  "15%Window",
  "20%Window",
  "100%Window",
  "15 kHz Black/White",
  "White 100%",
  "Black",
  "SDI Check Field",
  "Digital Gray",
  "5 step Staircase",
  "10 step Staircase",
  "Crosshatch",
  "PLUGE"
  };  
#endif

static struct MENU_ITEM code * Ptr;
static struct MENU_ITEM code * PrevPtr;
static struct MENU_ITEM code * tmpPtr; // this must be made global, if it is local,
                                // the Compiler does not generate any code 
                                // for tmpPtr = Ptr;
//extern bit VerticalRepeat;

/**********************/
code char*  CodePtr;    // Temp. pointer to text arrays in CODE

static char  GlobalBuffer[30];

static UC    enum_ndx;       // used to hold pattern number as defined by enums
static int   IntVal;         // Temp. integer value
static UI    UintVal;
static float FloatVal;

static long  samples;
static long  stepVal;
static UC    stepNdx;
static UC    UcharVal;       // Temp. unsigned char value

/**********************/
/*  Protos            */
extern int SRAMVideoTextLoad(void);

#if 1
code struct MENU_ITEM status_menu[5] =
  {
    {
    StatusGenlockItem, NULL, 0, (L_+D_+R_),
    StatusGenlockShow,
    ItemLeft, empty, StatusDown, ItemRight, empty, empty,
    &status_menu[1], &status_menu[4],
    },
    {
    StatusSDIItem, NULL, 0, (L_+D_+R_),
    StatusSDIShow,
    ItemLeft, empty, StatusDown, ItemRight, empty, empty,
    &status_menu[2], &status_menu[0],
    },
    {
    StatusAudioItem, NULL, 0, (L_+D_+R_),
    StatusAudioShow,
    ItemLeft, empty, StatusDown, ItemRight, empty, empty,
    &status_menu[3], &status_menu[1],
    },
    {
    StatusPresetItem, NULL, 0, (L_+D_+R_),
    StatusPresetShow,
    ItemLeft, empty, StatusDown, ItemRight, empty, empty,
    &status_menu[4], &status_menu[2],
    },
    {
    StatusBBItem, NULL, 0, (L_+D_+R_),
    StatusBBShow,
    ItemLeft, empty, StatusDown, ItemRight, empty, empty,
    &status_menu[0], &status_menu[3],
    }
  };
#endif

code struct MENU_ITEM base_menu[7] = 
  {
    {
    GenlockItem, GenlockTxt, 8, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[1], &base_menu[6]
    },
    {
    AnlBlkItem, BlkBurstTxt, 18, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[2], &base_menu[0]
    },
    {
    SdiItem, SdiTxt, 32, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[3], &base_menu[1]
    },
    {
    AUDIOItem, AUDIOTxt, 8, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[4], &base_menu[2]
    },
    {  
    PRESETItem, PRESETTxt, 16, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[5], &base_menu[3]
    },
    {  
    CALIBRATIONItem, CALIBRATIONTxt, 26, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[6], &base_menu[4]
    },
    {  
    SYSTEMItem, FACTORY_RESETTxt, 8, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    ItemLeft, UpToStatus, BaseDown, ItemRight, empty, empty,
    &base_menu[0], &base_menu[5]
    }
  };

code struct MENU_ITEM anlblk_menu[] = 
  {
    {
    BB1Item, BB1Txt, 8, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    ItemLeft, UpToBase, AnlBlkDown, ItemRight, empty, empty,
    &anlblk_menu[1], &anlblk_menu[2]
    },
    {
    BB2Item, BB2Txt, 13, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    ItemLeft, UpToBase, AnlBlkDown, ItemRight, empty, empty,
    &anlblk_menu[2], &anlblk_menu[0]
    },
    {
    BB3Item, BB3Txt, 18, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    ItemLeft, UpToBase, AnlBlkDown, ItemRight, empty, empty,
    &anlblk_menu[0], &anlblk_menu[1]
    }
  };

code struct MENU_ITEM BB_menu[] = 
  {
    {
    BBSystemItem, SYSTEMTxt, 8, (L_+D_+U_+R_),
    BBMenuShow,
    ItemLeft, UpToAnlBlk, BBDown, ItemRight, empty, empty,
    &BB_menu[1], &BB_menu[2]
    },
    {
    BBTimingItem, TimingTxt, 17, (L_+D_+U_+R_),
    BBMenuShow,
    ItemLeft, UpToAnlBlk, BBDown, ItemRight, empty, empty,
    &BB_menu[2], &BB_menu[0]
    },
    {
    BBScHPhaseItem, ScHPhaseTxt, 26, (L_+D_+U_+R_),
    BBMenuShow,
    ItemLeft, UpToAnlBlk, BBDown, ItemRight, empty, empty,
    &BB_menu[0], &BB_menu[1]
    }
  };

code struct MENU_ITEM BB_submenu[] = 
  {
    {
    BBSystemItem, SYSTEMTxt, 0, NA,
    BBSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {
    BBTimingItem, TimingTxt, 0, NA,
    BBTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL
    },
    {
    BBScHPhaseItem, ScHPhaseTxt, 0, NA,
    BBScHPhaseSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };

code struct MENU_ITEM genlock_menu[] = 
  {
    {  
    GenlockSystemItem, SYSTEMTxt, 8, (L_+D_+U_+R_),
    GenlockMenuShow,
    ItemLeft, UpToBase, GenlockDown, ItemRight, empty, empty,
    &genlock_menu[1], &genlock_menu[1]
    },
    {  
    GenlockTimingItem, TimingTxt, 17, (L_+D_+U_+R_),
    GenlockMenuShow,
    ItemLeft, UpToBase, GenlockDown, ItemRight, empty, empty,
    &genlock_menu[0], &genlock_menu[0]
    }
  };

code struct MENU_ITEM genlock_submenu[] = 
  {
    {  
    GenlockSystemItem, SYSTEMTxt, 0, NA,
    GenlockSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    GenlockTimingItem, TimingTxt, 0, NA,
    GenlockTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL,
    }
  };

code struct MENU_ITEM sdi_menu[] = 
  {
    { 
    SdiSystemItem, SYSTEMTxt, 8, (L_+D_+U_+R_+MORE_),
    SdiMenuShow,
    ItemLeft, UpToBase, SdiDown, ItemRight, empty, empty,
    &sdi_menu[1], &sdi_menu[5]
    },
    { 
    SdiTimingItem, TimingTxt, 17, (L_+D_+U_+R_+MORE_),
    SdiMenuShow,
    ItemLeft, UpToBase, SdiDown, ItemRight, empty, empty,
    &sdi_menu[2], &sdi_menu[0]
    },
    { 
    SdiPatternItem, PatternTxt, 26, (L_+D_+U_+R_+MORE_),
    SdiMenuShow,
    ItemLeft, UpToBase, SdiDown, ItemRight, empty, empty,
    &sdi_menu[3], &sdi_menu[1]
    },
    {  
    SdiEmbeddedItem, EMBAudioTxt, 8, (L_+D_+U_+R_+MORE_),
    SdiMenuShow,
    ItemLeft, UpToBase, SdiDown, ItemRight, empty, empty,
    &sdi_menu[4], &sdi_menu[2]
    },
    { 
    SdiAnalogItem, AnalogTxt, 20, (L_+D_+U_+R_+MORE_),
    SdiMenuShow,
    ItemLeft, UpToBase, SdiDown, ItemRight, empty, empty,
    &sdi_menu[5], &sdi_menu[3]
    },
    {  
    SdiTextItem, TextTxt, 8, (L_+D_+U_+R_+MORE_),
    SdiMenuShow,
    ItemLeft, UpToBase, SdiDown, ItemRight, empty, empty,
    &sdi_menu[0], &sdi_menu[4]
    }
  };

code struct MENU_ITEM sdi_submenu[] = 
  {
    {  
    SdiSystemItem, SYSTEMTxt, 0, NA,
    SdiSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    SdiTimingItem, TimingTxt, 0, NA,
    SdiTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL
    },
    { 
    SdiPatternItem, PatternTxt, 0, NA,
    SdiPatternSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {
    SdiEmbeddedItem, EMBAudioTxt, 0, NA,
    SdiEmbeddedSelect, 
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {
    SdiAnalogItem, ScHPhaseTxt, 0, NA,
    SdiAnalogEdit,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
#if 1
    {
    SdiTextItem, TextTxt, 8, (L_+D_+U_+R_),
    SdiTextMenuShow,
    ItemLeft, UpButType1,
    DownButType1, ItemRight, empty, empty,
    NULL, NULL,
    }
#endif
  };

code struct MENU_ITEM text_menu[] = 
  {
    {  
    SDITextEditItem, EDITTxt, 8, (L_+D_+U_+R_),
    SdiTextMenuShow,
    ItemLeft, UpToSdi, SdiTextDown, ItemRight, empty, empty,
    &text_menu[1], &text_menu[1]
    },
    {  
    SDITextMovementItem, MOVEMENTTxt, 17, (L_+D_+U_+R_),
    SdiTextMenuShow,
    ItemLeft, UpToSdi, SdiTextDown, ItemRight, empty, empty,
    &text_menu[0], &text_menu[0]
    }
  };


code struct MENU_ITEM text_submenu[] = 
  {
    {  
    SDITextEditItem, EDITTxt,  0, NA,
    SdiTextEdit,
    LeftButType12, UpButType12,
    DownButType12, RightButType12, ExecButType12, EscFunction,
    NULL, NULL,
    },
    {  
    SDITextMovementItem, MOVEMENTTxt, 0, NA,
    SdiMovementSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };

/******************       Audio Menus **************************/

code struct MENU_ITEM audio_menu[] = 
  {
    {  
    AnalogAudioItem, AnalogAudioTxt, 8, (L_+D_+U_+R_),
    AudioMenuShow,
    AudioItemLeft, UpToBase, AudioDown, AudioItemRight, empty, empty,
    &audio_menu[1], &audio_menu[1]
    },
    {  
    DigitalAudioItem, DigitalAudioTxt, 17, (L_+D_+U_+R_),
    AudioMenuShow,
    AudioItemLeft, UpToBase, AudioDown, AudioItemRight, empty, empty,
    &audio_menu[0], &audio_menu[0]
    }
  };

code struct MENU_ITEM analog_audio_submenu[] = 
  {
    {  
    AnalogAudioSignalItem, AnalogAudioSignalTxt, 8, (L_+D_+U_+R_),
    AnalogAudioMenuShow,
    ItemLeft, UpToAudio, AnalogAudioDown, ItemRight, empty, empty,
    &analog_audio_submenu[1], &analog_audio_submenu[2]
    },
    {  
    AnalogAudioLevelItem, AnalogAudioLevelTxt, 17, (L_+D_+U_+R_),
    AnalogAudioMenuShow,
    ItemLeft, UpToAudio, AnalogAudioDown, ItemRight, empty, empty,
    &analog_audio_submenu[2], &analog_audio_submenu[0]
    },
    {
    AnalogAudioClickItem, AnalogAudioClickTxt, 26, (L_+D_+U_+R_),
    AnalogAudioMenuShow,
    ItemLeft, UpToAudio, AnalogAudioDown, ItemRight, empty, empty,
    &analog_audio_submenu[0], &analog_audio_submenu[1]
    }
  };

code struct MENU_ITEM digital_audio_submenu[] = 
  {
    {  
    DigitalAudioSignalItem, DigitalAudioSignalTxt, 8, (L_+D_+U_+R_+MORE_),
    DigitalAudioMenuShow,
    ItemLeft, UpToAudio, DigitalAudioDown, ItemRight, empty, empty,
    &digital_audio_submenu[1], &digital_audio_submenu[5]
    },
    {  
    DigitalAudioLevelItem, DigitalAudioLevelTxt, 17, (L_+D_+U_+R_+MORE_),
    DigitalAudioMenuShow,
    ItemLeft, UpToAudio, DigitalAudioDown, ItemRight, empty, empty,
    &digital_audio_submenu[2], &digital_audio_submenu[0]
    },
    {  
    DigitalAudioTimingItem, DigitalAudioTimingTxt, 26, (L_+D_+U_+R_+MORE_),
    DigitalAudioMenuShow,
    ItemLeft, UpToAudio, DigitalAudioDown, ItemRight, empty, empty,
    &digital_audio_submenu[3], &digital_audio_submenu[1]
    },
    {  
    DigitalAudioSystemItem, DigitalAudioSystemTxt, 8, (L_+D_+U_+R_+MORE_),
    DigitalAudioMenuShow,
    ItemLeft, UpToAudio, DigitalAudioDown, ItemRight, empty, empty,
    &digital_audio_submenu[4], &digital_audio_submenu[2]
    },
    {  
    DigitalAudioFrequencyItem, DigitalAudioFrequencyTxt, 17, (L_+D_+U_+R_+MORE_),
    DigitalAudioMenuShow,
    ItemLeft, UpToAudio, DigitalAudioDown, ItemRight, empty, empty,
    &digital_audio_submenu[5], &digital_audio_submenu[3]
    },
    {
    DigitalAudioClickItem, DigitalAudioClickTxt, 26, (L_+D_+U_+R_+MORE_),
    DigitalAudioMenuShow,
    ItemLeft, UpToAudio, DigitalAudioDown, ItemRight, empty, empty,
    &digital_audio_submenu[0], &digital_audio_submenu[4]
    }
  };

code struct MENU_ITEM analog_audio_sub_submenu[] = 
  {
    {  
    AnalogAudioSignalItem, AnalogAudioSignalTxt, 0, NA,
    AnalogAudioSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    AnalogAudioLevelItem, AnalogAudioLevelTxt, 0, NA,
    AnalogAudioLevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    AnalogAudioClickItem, AnalogAudioClickTxt, 0, NA,
    AnalogAudioClickSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    }
  };
  
code struct MENU_ITEM digital_audio_sub_submenu[] = 
  {
    {  
    DigitalAudioSignalItem, DigitalAudioSignalTxt, 8, NA,
    DigitalAudioSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DigitalAudioLevelItem, DigitalAudioLevelTxt, 17, NA,
    DigitalAudioLevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DigitalAudioTimingItem, DigitalAudioTimingTxt, 26, NA,
    DigitalAudioTimingSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DigitalAudioSystemItem, DigitalAudioSystemTxt, 8, NA,
    DigitalAudioSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DigitalAudioFrequencyItem, DigitalAudioFrequencyTxt, 17, NA,
    DigitalAudioFrequencySelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DigitalAudioClickItem, DigitalAudioClickTxt, 26, NA,
    DigitalAudioClickSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };
 
code struct MENU_ITEM preset_menu[] = {
  {  PresetRecallItem, RecallTxt, 8, (L_+D_+U_+R_),
    PresetMenuShow,
    ItemLeft, UpToBase, PresetDown, ItemRight, empty, empty,
    &preset_menu[1], &preset_menu[2]
  },
  {  PresetStoreItem, StoreTxt, 17, (L_+D_+U_+R_),
    PresetMenuShow,
    ItemLeft, UpToBase, PresetDown, ItemRight, empty, empty,
    &preset_menu[2], &preset_menu[0]
  },
  {  PresetNameItem, NameTxt, 25, (L_+D_+U_+R_),
    PresetMenuShow,
    ItemLeft, UpToBase, PresetDown, ItemRight, empty, empty,
    &preset_menu[0], &preset_menu[1]
  }
};

code struct MENU_ITEM preset_submenu[] = {
  {  PresetRecallItem, RecallTxt, 8, NA,
    PresetRecallSelect,
    LeftButType3, UpButType3,
    DownButType3, RightButType3, ExecButType3, EscFunction,
    NULL, NULL,
  },
  {  PresetStoreItem, StoreTxt, 8, NA,
    PresetStoreSelect,
    LeftButType3, UpButType3,
    DownButType3, RightButType3, ExecButType3, EscFunction,
    NULL, NULL,
  },
  {  PresetNameItem, NameTxt, 8, NA,
    PresetNameSelect,
    LeftButType8, UpButType8,
    DownButType8, RightButType8, ExecButType8, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM system_menu = 
  {  
  0, SYSTEMTxt, 0, NA,
  FactoryResetSystemSelect,
  LeftButType3, UpButType3,
  DownButType3, RightButType3, ExecButType3, EscFunction,
  NULL, NULL
  };

code struct MENU_ITEM calibration_menu[] =
  {
    {
    CalibrationAdjItem, AdjTxt, 8, (L_+D_+U_+R_),
    CalibrationMenuShow,
    LeftButTypeCal, UpButTypeCal,
    DownButTypeCal, RightButTypeCal, ExecButTypeCal, EscFunction,
    &calibration_menu[1], &calibration_menu[2]
    },
    {
    CalibrationMinItem, MinTxt, 13, (L_+R_),
    CalibrationMenuShow,
    LeftButTypeCal, UpButTypeCal,
    DownButTypeCal, RightButTypeCal, ExecButTypeCal, EscFunction,
    &calibration_menu[2], &calibration_menu[0]
    },
    {
    CalibrationMaxItem, MaxTxt, 18, (L_+R_),
    CalibrationMenuShow,
    LeftButTypeCal, UpButTypeCal,
    DownButTypeCal, RightButTypeCal, ExecButTypeCal, EscFunction,
    &calibration_menu[0], &calibration_menu[1]
    }
  };

void StatusDown()
  {
  StatusNdx = Ptr->ItemNumber;  // Save active Status-Display pointer
  Ptr = &base_menu[BaseNdx];        // Restore Base-Menu pointer
  status_is_displayed = 0;
  rs232_touched = 0;
  }


void StatusGenlockShow(void)
  {
  WriteCodeLN1(0, GenlockTxt);
  //WriteCode1(GenlockInputArrTxt[ActiveGenlockNo]);
  ClearRestLN1();

  WriteCodeLN2(0, SignalHdrTxt);

  if (GenlockSetup.System == INTERNAL)
    {                                  // If internal genlock..
    WriteCode2(HyphenTxt);            // No signal information
    ClearToPosLN2(20);
    WriteCode2(StatusHdrTxt);
    WriteCode2(HyphenTxt);            // No status information
    }
  else
    {                                  // ..external genlock active
    WriteCode2(GenlockSystemArrTxt[GenlockSetup.System]);
    ClearToPosLN2(20);
    WriteCode2(StatusHdrTxt);

    if (GenlockStatus)
      WriteCode2(GENLOCKEDTxt);
    else 
      WriteCode2(UNLOCKEDTxt);
    }
  ClearRestLN2();                  // Clear rest of line 2
  WriteArrowsField(Ptr->ItemConfig);      // Write icons/dots on line 1&2
  }


void StatusSDIShow(void)
  {
  UC ndx;
  UC i;

  WriteCodeLN1(0, SDIHdrTxt);

  ndx = CBSetup.Pattern;         // ndx holds pattern no..
  if (CBSetup.System == SPAL)
    {
    for (i = 0; i < PAL_MAX; i++)
      if (ndx == pal_patterns[i].patt_no)
        {
        ndx = i;
        break;
        }
    WriteCode1(pal_patterns[ndx].patt_text);
    }
  else
    {
    for (i = 0; i < NTSC_MAX; i++)
      if (ndx == ntsc_patterns[i].patt_no)
        {
        ndx = i;
        break;
        }
    WriteCode1(ntsc_patterns[ndx].patt_text);
    }

  ClearRestLN1();

  WriteCodeLN2(0, SystemHdrTxt);

  WriteCode2(AnlSystemArrTxt[CBSetup.System]);

  ClearToPosLN2(20);              // Write text information

  WriteCode2(MinusPlusArrTxt[(UC) (text_on)]);
  WriteCode2(TextTxt);

  ClearToPosLN2(27);                // Write audio information
  WriteCode2(MinusPlusArrTxt[(UC) (CBSetup.EmbAudioSignal > 0)]);
  WriteCode2(AUDIOTxt);

  ClearRestLN2();

  WriteArrowsField(Ptr->ItemConfig);      // Write icons/dots on line 1&2
  }



void StatusAudioShow(void)
  {

  WriteCodeLN1(0, AudioTxt);
  WriteCode1(audio_menu[AudioTypeNdx].ItemTxt);
  WriteCode1(colon_spaceTxt);
  WriteCode1(AnalogAudioSignalArrTxt[AudioSetup[AudioTypeNdx].Signal]);

  ClearRestLN1();

/* AudioTypeNdx is 0 for Analog and 1 for Digital */
  WriteCodeLN2(0, LEVELTxt);

  if (AudioTypeNdx) // if Digital
    {
    WriteCode2(DigitalAudioLevelArrTxt[AudioSetup[AESEBUAudio].Level]);
    ClearToPosLN2(17);
    WriteCode2(WCLKTxt);
    WriteCode2(DigitalAudioFrequencyArrTxt[AudioSetup[AESEBUAudio].Frequency]);
    }
  else
    {
    WriteCode2(AnalogAudioLevelArrTxt[AudioSetup[AnalogAudio].Level]);
    ClearToPosLN2(17);
    }

  ClearRestLN2();

  WriteArrowsField(Ptr->ItemConfig);      // Write icons/dots on line 1&2
  }


void StatusPresetShow(void)
  {
  WriteCodeLN1(0, StatusPresetTxt);   // Write line 1 text
  ClearRestLN1();

  if (Settings.ActivePreset)
    {
    WriteCodeLN2(0, PresetHdrTxt);
    WriteCharLN2(6, Settings.ActivePreset + '0');

    WriteTxtLN2(9, PresetNames[Settings.ActivePreset - 1]);
    }
  else
    WriteCodeLN2(0, NoPresetActiveTxt);

  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);  // Write icons/dots on line 1&2
  }


void StatusBBShow(void)
  {
  WriteCodeLN1(0, BLACKBURSTTxt);
  ClearRestLN1();

  WriteCodeLN2(0, SBB1Txt);
  WriteCodeLN2(2, AnlSystemArrTxt[BBSetup[BB1Item].System]);
  ClearToPosLN2(12);

  WriteCodeLN2(12, SBB2Txt);
  WriteCodeLN2(14, AnlSystemArrTxt[BBSetup[BB2Item].System]);
  ClearToPosLN2(24);

  WriteCodeLN2(24, SBB3Txt);
  WriteCodeLN2(26, AnlSystemArrTxt[BBSetup[BB3Item].System]);

  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);  // Write icons/dots on line 1&2

  }


#if 1
/***************************************************************************/
/* AnlTimeAdjust                                    MENU.C                 */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 960621                                    */
/* Revised:    980507                                                      */
/*                                                                         */
/*  Function:  Update the delay in the analog units.                       */
/*  Remarks:                                                               */
/*                                                                         */
/*  Returns:    --                                                         */
/*  Updates:    TBD                                                        */
/***************************************************************************/
void AnlTimeAdjust(UC system, long delay)
  {
  
  char  sign;
  int   field;
  int   line;
  float time;
  int   lineTst;
                                // Time, ie. T, adjustement..
  if (FuncTriggers & 0x30)
    {                           // If button pressed is UP/DOWN..
    if (FuncTriggers & 0x10)   // If button pressed is UP ..
      samples += stepVal;
    else
      samples -= stepVal;
    }
  if (system < NTSC_US) // ***************If system is PAL.. ***********
    {                               // Line, ie. H, adjstement..
    if (FuncTriggers & 0x0C)
      {                               // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1728L * 256L;
      else
        samples -= (long) 1728L * 256L;
      }
                                    // Field, ie. V, adjustement ..
    if (FuncTriggers & 0x03)       // If button pressed is UP/DOWN..
      {
      lineTst = (samples / ((long) 1728L *256L)) % 625L;
      if (FuncTriggers & 0x01)
        {                           // If button pressed is UP..
        if (lineTst < 313)
          samples += (long) 313L * 1728L * 256L;
        else
          samples += (long) 312L * 1728L * 256L;
        }
      else 
        {
        if (lineTst < 313)
          samples -= (long) 312L * 1728L * 256L;
        else
          samples -= (long) 313L * 1728L * 256L;
        }
      }
    if (samples < 0)
      samples += (long) 4L * 625L*1728L*256L;
    else
      samples %= (long) 4L*625L*1728L*256L;
    }
  else 
    {                      // ********* otherwise system is NTSC ***********
    if (FuncTriggers & 0x0C)         // Line, ie. H, adjustement..
      {                               // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1716L*256L;
      else
        samples -= (long) 1716L*256L;
      }
                                     // Field, ie. V, adjustement..
    if (FuncTriggers & 0x03)
      {                              // If button pressed is UP/DOWN..
      lineTst = (samples / ((long) 1716L*256L)) % 525L;
      if (FuncTriggers & 0x01)
        {                            // If button pressed is UP..
        if (lineTst < 263)
          samples += (long) 263L*1716L*256L;
        else
          samples += (long) 262L*1716L*256L;
        }
      else 
        {
        if (lineTst < 263)
          samples -= (long) 262L*1716L*256L;
        else
          samples -= (long) 263L*1716L*256L;
        }
      }
    if (samples < 0)
      samples += (long) 2L*525L*1716L*256L;
    else
      samples %= (long) 2L*525L*1716L*256L;
    }
                                 // If time ie T, is being updated..
  if (FuncTriggers & 0xC0)
    {                            // If button pressed is RIGHT/LEFT..
    if (FuncTriggers & 0x40)
      {                          // If button pressed is LEFT..
      if (stepNdx++ == stepNdxAnalogMax)
        stepNdx = stepNdxAnalogMin;
      }
    else 
      {                          // ..button pressed is RIGHT
      if (stepNdx-- == stepNdxAnalogMin)
        stepNdx = stepNdxAnalogMax;
      }
    stepVal = stepsizeAnalogTable[stepNdx];  // Get step size from index
    }
  if (FuncTriggers & 0x3F)
    {
    if (delay == samples)
      {
      if (FuncState == 11)
        FuncState--;
      else
        FuncState -= 4;
      }
    }
  AnlSamplesToFLT(system, samples, &sign, &field, &line, &time);
  WriteTimingFLT(sign, field, line, time);
  WriteType6_SAVE_ESC(FuncState);    // Write the SAVE ESC text
  switch (FuncState)
    {
    case 1:
    case 5:
      WriteItemArrows(3, 2);        // Highlight selection
      break;
    case 2:
    case 6:
      WriteItemArrows(10, 4);      // Highlight selection
      break;
    case 3:
    case 7:
      WriteItemArrows(19, 8);      // Highlight selection
      CharBlinkLN2(0, OFF);        // Switch on blinking at
      stepVal = stepsizeAnalogTable[stepNdx = stepNdxAnalogMin]; // Reset step index and stepVal
      break;
    case 10:
    case 11:
      WriteItemBrackets(19, 8);        // Highlight selection
      if (stepNdx == stepNdxAnalogMin)
        CharBlinkLN2(26-stepNdx, ON);  // Switch on blinking
      else
        CharBlinkLN2(25-stepNdx, ON);
      break;
    }
  }
#endif


/* Function below is taken from 5230 software */
void SDITimeAdjust(UC system, long delay)
  {
  int   line;
  int   field;
  char  sign;
  float time;
  int   lineTst;

                            // Time, ie. T, adjustement..
  if (FuncTriggers & 0x30)
    {        // If button pressed is UP/DOWN..
    if (FuncTriggers & 0x10)         // If button pressed is UP ..
      samples += stepVal;
    else
      samples -= stepVal;
    }
  if (system < NTSC_US)
    {          // If system is SDI625..// Line, ie. H, adjstement..
    if (FuncTriggers & 0x0C)
      {                              // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)      // If button pressed is UP..
        samples += (long) 1728L*256L;
      else
        samples -= (long) 1728L*256L;
      }
    if (FuncTriggers & 0x03)  // Field, ie. V, adjustement ..
      {                        // If button pressed is UP/DOWN..
      lineTst = (samples / ((long) 1728L*256L)) % 625L;
      if (FuncTriggers & 0x01)
        {     // If button pressed is UP..
        if (lineTst < 312)
          samples += (long) 312L*1728L*256L;
        else
          samples += (long) 313L*1728L*256L;
        }
      else
        {
        if (lineTst < 312)
          samples -= (long) 313L*1728L*256L;
        else
          samples -= (long) 312L*1728L*256L;
        }
      }
    if (samples < 0)
      samples += (long) 625L*1728L*256L;
    else
      samples %= (long) 625L*1728L*256L;
    }
  else
    {                           // otherwise system is SDI525
    if (FuncTriggers & 0x0C)
      {                         // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1716L*256L;
      else
        samples -= (long) 1716L*256L;
      }
    if (FuncTriggers & 0x03)     // Field, ie. V, adjustement..
      {                           // If button pressed is UP/DOWN..
      lineTst = (samples / ((long) 1716L*256L)) % 525L;
      if (FuncTriggers & 0x01)
        {                         // If button pressed is UP..
        if (lineTst < 262)
          samples += (long) 262L*1716L*256L;
        else
          samples += (long) 263L*1716L*256L;
        }
      else
        {
        if (lineTst < 262)
          samples -= (long) 263L*1716L*256L;
        else
          samples -= (long) 262L*1716L*256L;
        }
      }
    if (samples < 0)
      samples += (long) 525L*1716L*256L;
    else
      samples %= (long) 525L*1716L*256L;
    }
                                 // If time ie T, is being updated..
  if (FuncTriggers & 0xC0)
    {                            // If button pressed is RIGHT/LEFT..
    if (FuncTriggers & 0x40)
      {                          // If button pressed is LEFT..
      if (stepNdx++ == stepNdxSDIMax)
        stepNdx = stepNdxSDIMin;
      }
    else
      {                          // ..button pressed is RIGHT
      if (stepNdx-- == stepNdxSDIMin)
        stepNdx = stepNdxSDIMax;
      }
    stepVal = stepsizeSDITable[stepNdx];    // Get step size from index
    }
  if (FuncTriggers & 0x3F)
    {
    if (delay == samples)
      {
      if (FuncState == 11)
        FuncState--;
      else
        FuncState -= 4;
      }
    }
  SDISamplesToFLT(system, samples, &sign, &field, &line, &time);
  WriteTimingFLT(sign, field, line, time);

  WriteType6_SAVE_ESC(FuncState);      // Write the SAVE ESC text

  switch (FuncState)
    {
    case 1:
    case 5:
      WriteItemArrows(3, 2);        // Highlight selection
      break;
    case 2:
    case 6:
      WriteItemArrows(10, 4);      // Highlight selection
      break;
    case 3:
    case 7:
      WriteItemArrows(19, 8);      // Highlight selection
      CharBlinkLN2(0, OFF);        // Switch on blinking at
      stepVal = stepsizeSDITable[stepNdx = stepNdxSDIMin];
      break;
    case 10:
    case 11:
      WriteItemBrackets(19, 8);        // Highlight selection
      CharBlinkLN2(23-stepNdx, ON);
      break;
    }
  }



/***************************************************************************/
/* AnlSamplesToFLT                                               TIMING.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void AnlSamplesToFLT(UC system, long samples, char* sign, int* F, int* L, float* T)
  {
  //long k;
  int frame, field;
  int line;

  *sign = '+';

  if (system < NTSC_US)     // If system selected is PAL..
    {
    if (samples >= 552960000L)
      {
      samples--;
      frame = samples / 276480000L;
      line = (samples % 276480000L) /  442368L;
      samples = (samples % 276480000L) %  442368L;
      field = 2*frame;
      if (line > 312)
        {
        field++;
        line = 624-line;
        }
      else
        line = 312-line;
      samples = 442368L - 1 - samples;
      field = 7-field;
      if (field != 4)
        *sign = '-';
      }
    else
      {
      frame = samples / 276480000L;
      line = (samples % 276480000L) / 442368L;
      samples = (samples % 276480000L) % 442368L;
      field = 2*frame;
      if (line > 312)
        {
        field++;
        line -= 313;
        }
      }
    }
  else 
    {                      // ..system selected is NTSC
    if (samples >= 230630400L)
      {
      // 230630400 = 525*1716*256
      // 439296    = 1716*256
      samples--;
      frame = samples / 230630400L;
      line = (samples % 230630400L) / 439296L;
      samples = (samples % 230630400L) % 439296L;
      field = 2*frame;
      if (line > 262)
        {
        field++;
        line = 524-line;
        }
      else
        line  = 262 - line;
      samples = 439296L - 1 - samples;
      field   = 3 - field;
      if (field != 2)
        *sign = '-';
      }
    else
      {
      frame   = samples / 230630400L;
      line    = (samples % 230630400L) / 439296L;
      samples = (samples % 230630400L) % 439296L;
      field   = 2 * frame;
      if (line > 262)
        {
        field++;
        line -= 263;
        }
      }
    }
  *L = abs(line);
  *F = abs(field);
  *T = fabs((float) (samples * 1000L) / 6912L);
  }
  
/* Function below is copied directly from 5230 software */
void SDISamplesToFLT(UC system, long samples, char* sign, int* F, int* L, float* T)
  {
  char field;
  int line;

  *sign = '+';

  if (system < NTSC_US)
    {            // If system selected is SDI625..
    if (samples >=  ((long) 312L*1728L*256L))
      {
      samples--;
      line = (samples % ((long) 625L*1728L*256L)) / ((long) 1728L*256L);
      samples = (samples % ((long) 625L*1728L*256L)) % ((long) 1728L*256L);
      field = 0;
      if (line > 311)
        {
        field++;
        line = 624-line;
        }
      else
        line = 311-line;
      samples = ((long) 1728L*256L) - 1 - samples;
      field   = 1-field;
      if (field != 1)
        *sign = '-';
      }
    else
      {
      line    = (samples % ((long) 625L*1728L*256L)) / ((long) 1728L*256L);
      samples = (samples % ((long) 625L*1728L*256L)) % ((long) 1728L*256L);
      field   = 0;
      if (line > 311)
        {
        field++;
        line -= 312;
        }
      }
    }
  else
    {                                // ..system selected is SI525
    if (samples >= ((long) 262L*1716L*256L))
      {
      samples--;
      line    = (samples % ((long) 525L*1716L*256L)) / ((long) 1716L*256L);
      samples = (samples % ((long) 525L*1716L*256L)) % ((long) 1716L*256L);
      field = 0;
      if (line > 261)
        {
        field++;
        line = 524-line;
        }
      else
        line = 261-line;
      samples = ((long) 1716L*256L) - 1 - samples;
      field   = 1 - field;
      if (field != 1)
        *sign = '-';
      }
    else
      {
      line    = (samples % ((long) 525L*1716L*256L)) / ((long) 1716L*256L);
      samples = (samples % ((long) 525L*1716L*256L)) % ((long) 1716L*256L);
      field   = 0;
      if (line > 261)
        {
        field++;
        line -= 262;
        }
      }
    }
  *L = abs(line);
  *F = abs(field);
  *T = fabs((float) (samples*1000L)/6912L);
  }


void UpToStatus()
  {
  Ptr = &status_menu[StatusNdx];    // Restore Display-Status pointer
  status_is_displayed = 1;
  }


/***************************************************************************/
/*  BaseMenuShow                                                           */
/*  Function:  Display the items in the BASE menu.                         */
/*  Remarks:    Cursor position on line 1 & 2 are updated.                 */
/*  Returns:    --                                                         */
/*  Updates:    TBD                                                        */
/***************************************************************************/
void BaseMenuShow(void)
  {
  int i;
  int number;       // number of items to display in line 2
  int start;

  WriteCodeLN1(0, PT5230HeadlineTxt);
  ClearRestLN1();
  BaseNdx = Ptr->ItemNumber;  // Save Base Menu index
  WriteCodeLN2(0, MenuHdrTxt);
  tmpPtr = Ptr;
  number = 0;
  start  = 0;
  switch (BaseNdx) 
    {
    case 0:
    case 1:
    case 2:
      Ptr = &base_menu[GenlockItem];
      start = GenlockItem;
      number = 3;
      break;
    case 3:
    case 4:
    case 5:
      Ptr = &base_menu[AUDIOItem];
      start = AUDIOItem;
      number = 3;
      break;
    case 6:
      Ptr = &base_menu[SYSTEMItem];
      start = SYSTEMItem;
      number = 1;
      break;
    }
  for (i = start; i < start + number; i++) 
    {
    ClearToPosLN2(Ptr->ItemPos);    // Find item column position
    WriteCode2(Ptr->ItemTxt);       // text is in code memory
    Ptr = Ptr->NextItem;
    }
  ClearRestLN2();
  Ptr = tmpPtr;
  i = Ptr->ItemConfig;     // Get info on line 1&2 icons/dots
  WriteArrowsField(i);     // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));  // Mark selected item (start, length)
  }


/**********/  
/***************************************************************************/
/*  BaseDown                                        MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                          */
/* Revised:    980507                                       */
/*                                                  */
/*  Function:  Go to the selected item in the BASE MENU.              */
/*  Remarks:    BASE MENU function.                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void BaseDown()
  {
  switch (BaseNdx) 
    {
    case SYSTEMItem:
      PrevPtr = Ptr;
      Ptr = &system_menu;
      UcharVal = 0;
      FuncState = 1;
      break;
    case CALIBRATIONItem:
      PrevPtr = Ptr;
      Ptr = &calibration_menu[0];
      FuncState = 1;    // Prepare for item state-machine
      do_cal_enable_on();
      break;
    case GenlockItem:
      PrevPtr = Ptr;
      Ptr = &genlock_menu[GenlockNdx];
      break;
    case AnlBlkItem:
      PrevPtr = Ptr;
      Ptr = &anlblk_menu[BBNdx[AnlBlkNdx]];
      break;
    case SdiItem:
      PrevPtr = Ptr;
      Ptr = &sdi_menu[SdiNdx];
      break;
    case AUDIOItem:
      /* Ptr must reflect audio type the same as set in 5201, so the    */
      /* right type will be selected when we display analog/aesebu menu */
      /* AudioTypeNdx must reflect type set inside 5201                 */
      PrevPtr = Ptr;
      Ptr = &audio_menu[AudioTypeNdx];
      break;
    case PRESETItem:
      PrevPtr = Ptr;
      Ptr = &preset_menu[PresetNdx];
      break;
    default:
      PrevPtr = Ptr;
      Ptr = &system_menu;
      UcharVal = 0;
      FuncState = 1; // Prepare for item state-machine
      break;
    }
  FuncSave = FuncEscape = FALSE;
  }

/***************************************************************************/
/*  UpToBase                                        MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                          */
/* Revised:    980126                                       */
/*                                                  */
/*  Function:  Go to the BASE MENU and restore last selected item        */
/*  Remarks:    BASE MENU function                            */
/*  Returns:    --                                        */
/*  Updates:    Ptr                                      */
/***************************************************************************/
void UpToBase(void)
  {
  Ptr = &base_menu[BaseNdx];          // Restore Base menu item
  }

/***************************************************************************/
/*  AnlBlkMenuShow                                    MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                          */
/* Revised:    980508                                       */
/*                                                  */
/*  Function:  Display the items in the BLACK-BURST menu.            */
/*  Remarks:    Cursor position on line 1 & 2 are updated.            */
/*  Returns:    --                                        */
/*  Updates:    AnlBlkNdx                                  */
/***************************************************************************/
void AnlBlkMenuShow(void)
  {
  register int i, start, number;

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoConfigureTxt);           // Write menu headline
  ClearRestLN1();                 
  AnlBlkNdx = Ptr->ItemNumber;        // Update BB unit index, ie. no.
 
  tmpPtr = Ptr;     // Save menu pointer temporarily
  Ptr = &anlblk_menu[0];
  WriteCodeLN2(0, SubmnuHdrTxt);     // Write header for sub-menu line
  start  = BB1Item;
  number = 3;
  for (i = start; i < start + number; i++) 
    {
    ClearToPosLN2(Ptr->ItemPos);    // Find item column position
    WriteCode2(Ptr->ItemTxt);       // text is in code memory
    Ptr = Ptr->NextItem;
    }
  ClearRestLN2();
  Ptr = tmpPtr;
  i = Ptr->ItemConfig;     // Get info on line 1&2 icons/dots
  WriteArrowsField(i);     // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));  // Mark selected item (start, length)
  }

/***************************************************************************/
/*  AnlBlkDown                                      MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                          */
/* Revised:    980508                                       */
/*                                                  */
/*  Function:  Go to the selected item in the BLACK-BURST menu.        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Ptr                                      */
/***************************************************************************/
void AnlBlkDown(void)
  {
  Ptr = &BB_menu[BBNdx[AnlBlkNdx]];  // Restore unit BBn's last item
  }

/***************************************************************************/
/* UpToAnlBlk                                      MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                          */
/* Revised:    980508                                       */
/*                                                  */
/*  Function:  Go to the BLACK-BURST menu and restore last selected item.  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Ptr                                      */
/***************************************************************************/
void UpToAnlBlk(void)
  {
  Ptr = &anlblk_menu[AnlBlkNdx];    // Restore selected BLACK-BURST item.
  }

/***************************************************************************/
/*  ItemLeft                                       MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980122                                       */
/*                                                  */
/*  Function:  Select previous item in the general menu-/submenu-system    */
/*  Remarks:    This function does NOT apply to the BASE-menu          */
/*  Returns:    --                                        */
/*  Updates:    Ptr                                      */
/***************************************************************************/
void ItemLeft() 
  {
  Ptr = Ptr->PrevItem;
  }

void AudioItemLeft(void) 
  {
  Ptr = Ptr->PrevItem;
  AudioTypeNdx = Ptr->ItemNumber;
  do_Audio_Kind(AudioTypeNdx);
  }

/***************************************************************************/
/*  ItemRight                                     MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980122                                       */
/*                                                  */
/*  Function:  Select next item in the general menu-/submenu-system      */
/*  Remarks:    This function does NOT apply to the BASE-menu          */
/*  Returns:    --                                        */
/*  Updates:    Ptr                                      */
/***************************************************************************/
void ItemRight(void) 
  {
  Ptr = Ptr->NextItem;
  }


void AudioItemRight(void) 
  {
  Ptr = Ptr->NextItem;
  AudioTypeNdx = Ptr->ItemNumber;
  do_Audio_Kind(AudioTypeNdx);
  }

/***************************************************************************/
/*  BBMenuShow                                                 MENUTREE.C  */
/*                                                                         */
/* Author:     Kim Engedahl, DEV, 980119                                   */
/* Revised:    980508                                                      */
/*                                                                         */
/*  Function:  Display the items in the BLACK-BURST/BB submenu (BB_menu[]):*/
/*             <SYSTEM>  TIMING  ScH-PHASE                                 */
/*  Remarks:   Ptr = &BB_menu[BBNdx[AnlBlkNdx]] on entry                   */
/*  Returns:   Nothing                                                     */
/*  Updates:   BBNdx[AnlBlkNdx]                                            */
/***************************************************************************/
void BBMenuShow(void)
  {
  register int i;
  
  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  CodePtr = anlblk_menu[AnlBlkNdx].ItemTxt;
  WriteCode1(anlblk_menu[AnlBlkNdx].ItemTxt);
  WriteCode1(CoSelectTxt);
  ClearRestLN1();
  BBNdx[AnlBlkNdx] = Ptr->ItemNumber;
  WriteCodeLN2(0, SecondmnuHdrTxt);
  for (i = BBSystemItem; i <= BBScHPhaseItem; i++)
    {
    ClearToPosLN2(BB_menu[i].ItemPos);   // Find item column position
    WriteCode2(BB_menu[i].ItemTxt);      // Write item texts
    }
  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);      // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));// Mark selected item
  }


/***************************************************************************/
/* BBDown                                                      MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  Go to the selected item in the BLACK-BURST/BBx submenu.     */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:   None                                                        */
/*  Note:      We copy BBSetup to BBOrig on entry here.  Normally, these   */
/*             two are kept synchronized.  However, when BBSetup  are      */
/*             altered via remote, BBOrig is not updated.  On entry here   */
/*             BBOrig will always reflect BBSetup                          */
/***************************************************************************/
void BBDown(void)
  {
  memcpy(&BBOrig[AnlBlkNdx], &BBSetup[AnlBlkNdx], sizeof(BBObject));
  WriteCodeLN1(0, SecondmnuHdrTxt);
  WriteCode1(SlashDotHdrTxt);
  WriteCode1(anlblk_menu[AnlBlkNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(BB_menu[BBNdx[AnlBlkNdx]].ItemTxt);
  switch (BBNdx[AnlBlkNdx])
    {
    case BBSystemItem:
      WriteCode1(CoSelectTxt);
      UcharVal = BBSetup[AnlBlkNdx].System;
      break;
    case BBTimingItem:
      WriteCode1(CoEditDelayTxt);
      UcharVal = BBSetup[AnlBlkNdx].System;
      samples  = BBSetup[AnlBlkNdx].Delay;
      stepNdx  = stepNdxAnalogMin;
      stepVal  = stepsizeAnalogTable[stepNdx];
      break;
    case BBScHPhaseItem:
      WriteCode1(CoEditTxt);
      IntVal = BBSetup[AnlBlkNdx].ScHPhase;
      break;
    }
  ClearRestLN1();
  PrevPtr      = Ptr;
  Ptr          = &BB_submenu[BBNdx[AnlBlkNdx]];  // Point to selected item
  FuncState    = 1;
  FuncTriggers = 0;
  FuncSave     = FuncEscape = FALSE;
  }

/***************************************************************************/
/*  BBSystemSelect                                                 MENU.C  */
/*                                                                         */
/* Author:      Kim Engedahl, DEV, 960621                                  */
/* Revised:     980428                                                     */
/*                                                                         */
/*  Function:   Sets the TV system in a black burst generator.             */
/*  Remarks:    BLACK-BURST SUBMENU function                               */
/*              Cursor position on line 2 is updated                       */
/*  Returns:    Nothing                                                    */
/*  Updates:    Ptr, CodePtr, UcharVal, FuncSave, FuncEscape, FuncState,   */
/*              FunState, FuncTriggers, Settings.ActivePreset              */
/***************************************************************************/
void BBSystemSelect(void)
  {
  
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      BBSetup[AnlBlkNdx].System = UcharVal;
      // update settings if Save
      memcpy(&BBOrig[AnlBlkNdx], &BBSetup[AnlBlkNdx], sizeof(BBObject));
      }
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if Escape
    do_AnlBlkConfig(&BBOrig[AnlBlkNdx], AnlBlkNdx, BBSystemItem);
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == PAL)
        UcharVal = NTSC_J;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == NTSC_J)
        UcharVal = PAL;
      }
    if (UcharVal == BBOrig[AnlBlkNdx].System)
      FuncState = 1;                                //  remove SAVE option
    BBSetup[AnlBlkNdx].System = UcharVal;      // Save new BB system
    do_AnlBlkConfig(&BBSetup[AnlBlkNdx], AnlBlkNdx, BBSystemItem);
    }
  CodePtr = AnlSystemArrTxt[UcharVal];
  WriteCodeLN2(0, SystemHdrTxt);
  WriteCode2(CodePtr); // write actual system
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;
  }

/***************************************************************************/
/*  BBTimingSelect                                     MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960621                          */
/* Revised:   980507                                       */
/*                                                  */
/*  Function: Set the delay in a Black Burst Generator.              */
/*  Remarks:  BLACK-BURST SUBMENU function.                      */
/*            Cursor position on line 2 is updated.                */
/*            Instant update implemented.                      */
/*  Returns:    --                                        */
/*  Updates:    TBD                                      */
/***************************************************************************/
void BBTimingSelect(void)
  {
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {                    // If delay has changed..
      Settings.ActivePreset = 0;
      BBSetup[AnlBlkNdx].Delay = samples;
      // if SAVE, udate BBOrig with new settings in BBSetup
      memcpy(&BBOrig[AnlBlkNdx], &BBSetup[AnlBlkNdx], sizeof(BBObject));
      }
    CharBlinkLN2(0, OFF);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // if SAVE, then BBOrig = BBOrig
    // if ESC,  then BBOrig conatins original settings
    do_AnlBlkConfig(&BBOrig[AnlBlkNdx], AnlBlkNdx, BBTimingItem);
    return;
    }
  AnlTimeAdjust(UcharVal, BBOrig[AnlBlkNdx].Delay);
  BBSetup[AnlBlkNdx].Delay = samples;  // Save new BBn delay
  do_AnlBlkConfig(&BBSetup[AnlBlkNdx], AnlBlkNdx, BBTimingItem);
  FuncTriggers = 0;                  // Reset button selectors
  }

/***************************************************************************/
/*  BBScHPhaseSelect                                   MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960621                          */
/* Revised:    980208                                       */
/*                                                  */
/*  Function:  Setting the ScH-phase value in a black burst generator.    */
/*  Remarks:    BLACK-BURST SUBMENU function                      */
/*          Instant update of ScH-Phase. The ScH-Phase is not updated  */
/*          if the menu is left without altering the value.          */
/*  Returns:    --                                        */
/*  Updates:    Ptr, FloatVal, IntVal, TextBuffer, FuncSave,  FuncState    */
/*          FuncTriggers, Settings.ActivePreset            */
/***************************************************************************/
void BBScHPhaseSelect(void)
  {
  char TextBuffer[20];
  
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      BBSetup[AnlBlkNdx].ScHPhase = IntVal;
      // update settings if Save
      memcpy(&BBOrig[AnlBlkNdx], &BBSetup[AnlBlkNdx], sizeof(BBObject));
      }
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if Escape
    do_AnlBlkConfig(&BBOrig[AnlBlkNdx], AnlBlkNdx, BBScHPhaseItem);
    return;
    }
  if (FuncTriggers & 0x03)
    {                               // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01)
      {                             // If button pressed is UP..
      if (++IntVal > ScHPhaseHigh)      // If max. ScH-Phase reached..
        IntVal = ScHPhaseLow;        //  .. roll-over to min. value
      }
    else 
      {                              // ..button pressed is DOWN
      if (--IntVal < ScHPhaseLow)   // If min. ScH-Phase reached..
        IntVal = ScHPhaseHigh;       //  .. roll-over to max. value
      }
    if (IntVal == BBOrig[AnlBlkNdx].ScHPhase)
      FuncState = 1;                 //  remove SAVE option
    BBSetup[AnlBlkNdx].ScHPhase = IntVal;
    do_AnlBlkConfig(&BBSetup[AnlBlkNdx], AnlBlkNdx, BBScHPhaseItem);
    }
  WriteCodeLN2(0, ScHPhaseHdrTxt);      // Write line 2 header
  sprintf(TextBuffer, "%+4d", IntVal);
  Mstrcat(TextBuffer, degTxt);          // Add prefix deg
  WriteTxt2(TextBuffer);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(11, strlen(TextBuffer));  // Mark selected item
  FuncTriggers = 0;                  // Reset button selectors
  }

/*******************/
/*******************/
void GenlockMenuShow(void)
  {
  register int i;

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  //WriteChar1('/');
  //CodePtr = genlock_menu[GenlockNdx].ItemTxt;
  //WriteCode1(genlock_menu[GenlockNdx].ItemTxt);
  WriteCode1(CoSelectTxt);            // Write sub-menu headline
  ClearRestLN1();
  GenlockNdx = Ptr->ItemNumber;
  WriteCodeLN2(0, SubmnuHdrTxt);      // Write line 2 header
  for (i = GenlockSystemItem; i <= GenlockTimingItem; i++)
    {
    ClearToPosLN2(genlock_menu[i].ItemPos);    // Find item column position
    WriteCode2(genlock_menu[i].ItemTxt);      // Write item texts
    }
  ClearRestLN2();                  // Clear rest of line 2
  WriteArrowsField(Ptr->ItemConfig);      // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));
  }

void GenlockDown(void)
  {
  memcpy(&GenlockOrig, &GenlockSetup, sizeof(GenlockObject));
  WriteCodeLN1(0, SecondmnuHdrTxt);
  WriteCode1(SlashDotHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(genlock_menu[GenlockNdx].ItemTxt);
  switch (GenlockNdx) 
    {
    case GenlockSystemItem:
      WriteCode1(CoSelectTxt);
      UcharVal = GenlockSetup.System;
      break;
    case GenlockTimingItem:
      WriteCode1(CoEditDelayTxt);
      UcharVal = GenlockSetup.System;
      samples  = GenlockSetup.Delay;
      stepNdx  = stepNdxAnalogMin;
      stepVal  = stepsizeAnalogTable[stepNdx];    // Get step size
      break;
    }
  ClearRestLN1();
  PrevPtr = Ptr;
  Ptr = &genlock_submenu[GenlockNdx];  // Point to selected item
  FuncState = 1;
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
  }



void GenlockSystemSelect(void)
  {
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      GenlockSetup.System = UcharVal;
      // update settings if Save
      memcpy(&GenlockOrig, &GenlockSetup, sizeof(GenlockObject));
      }
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if escape
    do_GenlockConfig(&GenlockOrig, GenlockSystemItem);
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == INTERNAL)
        UcharVal = GMHZ;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == GMHZ)
        UcharVal = INTERNAL;
      }
    if (UcharVal == GenlockOrig.System) // If actual system..
      FuncState = 1;                      //  remove SAVE option
    GenlockSetup.System = UcharVal;
    do_GenlockConfig(&GenlockSetup, GenlockSystemItem);
    }
  CodePtr = GenlockSystemArrTxt[UcharVal];
  WriteCodeLN2(0, SystemHdrTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }

void GenlockTimingSelect(void)
  {
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {                    // If delay has changed..
      Settings.ActivePreset = 0;
      GenlockSetup.Delay = samples;
      // update settings if Save
      memcpy(&GenlockOrig, &GenlockSetup, sizeof(GenlockObject));
      }
    CharBlinkLN2(0, OFF);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if escape
    do_GenlockConfig(&GenlockOrig, GenlockTimingItem);
    FuncSave = FuncEscape = FALSE;
    return;
    }
  AnlTimeAdjust(UcharVal, GenlockSetup.Delay);
  GenlockSetup.Delay = samples;  // Save new BBn delay
  do_GenlockConfig(&GenlockSetup, GenlockTimingItem);
  FuncTriggers = 0;                  // Reset button selectors
  }

void SdiMenuShow(void)
  {
  //struct MENU_ITEM code * tmpPtr;
  register int i;
  int number;       // number of items to display in line 2
  int start;
  
  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoSelectTxt);
  ClearRestLN1();
  SdiNdx = Ptr->ItemNumber;
  WriteCodeLN2(0, SubmnuHdrTxt);
  tmpPtr = Ptr;
  start  = 0;
  number = 0;
  switch (SdiNdx) 
    {
    case 0:
    case 1:
    case 2:
      Ptr    = &sdi_menu[SdiSystemItem];
      start  = SdiSystemItem;  // 0
      number = 3;
      break;
    case 3:
    case 4:
      Ptr    = &sdi_menu[SdiEmbeddedItem];
      start  = SdiEmbeddedItem;  // 1
      number = 2;
      break;
    case 5:     // Text
      Ptr    = &sdi_menu[SdiTextItem];
      start  = SdiTextItem;
      number = 1;
      break;
    }
  for (i = start; i < start + number; i++) 
    {
    ClearToPosLN2(Ptr->ItemPos);    // Find item column position
    WriteCode2(Ptr->ItemTxt);  // text is in code memory
    Ptr = Ptr->NextItem;
    }
  ClearRestLN2();
  Ptr = tmpPtr;
  i = Ptr->ItemConfig;     // Get info on line 1&2 icons/dots
  WriteArrowsField(i);     // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));  // Mark selected item (start, length)
  }
  

/***************************************************************************/
/* SdiDown                                                     MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  Go to the selected item in the SDI submenu.                 */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:   None                                                        */
/*  Note:      We copy CBSetup to CBOrig on entry here.  Normally, these   */
/*             two are kept synchronized.  However, when CBSetup  are      */
/*             altered via remote, CBOrig is not updated.  On entry here   */
/*             CBOrig will always reflect CBSetup for correct menu         */
/*             operation.                                                  */
/***************************************************************************/
void SdiDown(void)
  {
  int i;

  memcpy(&CBOrig, &CBSetup, sizeof(CBObject));

  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(SlashDotHdrTxt);             // = "../";
  //WriteChar1('/');
  WriteCode1(sdi_menu[SdiNdx].ItemTxt);
  switch (SdiNdx) 
    {
    case SdiSystemItem:
      WriteCode1(CoSelectTxt);
      UcharVal = CBSetup.System;
      break;
    case SdiTimingItem:
      WriteCode1(CoEditDelayTxt);
      UcharVal = CBSetup.System;
      samples  = CBSetup.Delay * 256; // kurt
      stepNdx  = stepNdxSDIMin;
      stepVal  = stepsizeSDITable[stepNdx];  // Get step size
      break;
    case SdiPatternItem: //  ndx = CBSetup.Pattern;
      WriteCode1(CoSelectTxt);
      enum_ndx = CBSetup.Pattern;      // enum_ndx holds pattern no..
      if (CBSetup.System == SPAL)
        {
        for (i = 0; i < PAL_MAX; i++)
          if (pal_patterns[i].patt_no == enum_ndx)
            {
            UcharVal = i;                // and UcharVal holds index to 
            break;                       // table with names.
            }
        }
      else
        {
        for (i = 0; i < NTSC_MAX; i++)
          if (ntsc_patterns[i].patt_no == enum_ndx)
            {
            UcharVal = i;
            break;
            }
        }    
      break;
    case SdiEmbeddedItem:
      WriteCode1(CoSelectTxt);
      UcharVal = CBSetup.EmbAudioSignal;
      break;
    case SdiAnalogItem:
      WriteCode1(CoSelectTxt);
      IntVal = CBSetup.ScHPhase;
      break;
    case SdiTextItem:
      ClearRestLN1();
      PrevPtr = Ptr;
      Ptr = &text_menu[0];
      FuncState = 1;
      FuncTriggers = 0;
      FuncSave = FuncEscape = FALSE;
      return;
    }
  ClearRestLN1();
  PrevPtr = Ptr;
  Ptr = &sdi_submenu[SdiNdx];
  FuncState = 1;
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
  }

void UpToSdi(void)
  {
  Ptr = &sdi_menu[SdiTextItem];  // point to TEXT on return
  }

void SdiTextMenuShow(void)
  {
  register int i;

  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(sdi_menu[SdiNdx].ItemTxt);
  WriteCode1(CoSelectTxt);
  ClearRestLN1();
  WriteCodeLN2(0, SecondmnuHdrTxt);
  for (i = SDITextEditItem ; i <= SDITextMovementItem; i++)
    {
    ClearToPosLN2(text_menu[i].ItemPos);
    WriteCode2(text_menu[i].ItemTxt);
    }
  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));
  }


void SdiTextDown(void)
  {

  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(sdi_menu[SdiNdx].ItemTxt);
  WriteChar1('/');

  switch (Ptr->ItemNumber) 
    {
    case SDITextEditItem:
      WriteCode1(CoEditTxt);
      ClearRestLN1();
      UcharVal = CBSetup.TextData.TextEnable;
      break;
    case SDITextMovementItem:
      WriteCode1(MOVEMENTTxt);
      WriteCode1(CoSelectTxt);
      ClearRestLN1();
      UcharVal = CBSetup.TextData.TextMovement;
      break;
    }
  PrevPtr      = Ptr;
  Ptr          = &text_submenu[Ptr->ItemNumber];  // Point to selected item
  FuncState    = 1;
  FuncTriggers = 0;
  FuncSave     = FuncEscape = FALSE;
  }

/***************************************************************************/
/*  SdiTextEdit                                                            */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:    12.06.2002                                                  */
/*                                                                         */
/*  Function:  Edit a text in SDI.                                         */
/*  Remarks:    SDI/TEXT MENU function.                                    */
/*          Cursor position on line 2 is updated.                          */
/*          UcharVal     LineNumber 0-4  // Text Enable ON/OFF             */
/*          UcharVal1    Min. LineNumber // not used                       */
/*          UcharVal2    Max. LineNumber // not used                       */
/*          UcharVal3    Text Enable ON/OFF  // not used                   */
/*          UintVal      Position in text buffer                           */
/*  Returns:    --                                                         */
/*  Updates:    TBD                                                        */
/*          Text is edited in CBSetup.TextData.Text                      */
/***************************************************************************/
void SdiTextEdit(void)
  {
  UC tmp;

  if (FuncEscape) 
    {
    CharBlinkLN2(0, OFF);
    // if ESC, copy Orig to Setup and do configs
    strcpy(CBSetup.TextData.Text, CBOrig.TextData.Text);
    CBSetup.TextData.TextEnable = CBOrig.TextData.TextEnable;
    do_SdiText(CBOrig.TextData.TextEnable);
    do_SdiConfig(&CBOrig, SdiTextItem);
    FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  if (FuncSave) 
    {                    // If textinfo has changed
    FuncSave = FALSE;
    Settings.ActivePreset = 0;
    // if SAVE, copy Setup to Orig and do configs
    strcpy(CBOrig.TextData.Text, CBSetup.TextData.Text);
    CBOrig.TextData.TextEnable = UcharVal;  // Save text enable information
    do_SdiText(CBOrig.TextData.TextEnable);
    do_SdiConfig(&CBOrig, SdiTextItem);
    FuncSave = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  if (FuncTriggers & 0x0C)        // Select textline ON/OFF
    {                              // If UP/DOWN button pressed..
    UcharVal ^= ON;                // Toggle selection
    CBSetup.TextData.TextEnable = UcharVal;
    do_SdiText(CBSetup.TextData.TextEnable);
    if (!strcmp(CBSetup.TextData.Text, CBOrig.TextData.Text))
      if (CBOrig.TextData.TextEnable == UcharVal) //CBSetup.TextData.TextEnable)
        FuncState = 2; // orig: 2;
    }
  if (FuncTriggers & 0xC0)    // Select position in CBSetup.TextData.Text 
    {                         // If LEFT/RIGHT button pressed..
    if (FuncTriggers & 0x40) 
      {                       // If button pressed is LEFT..
      CBSetup.TextData.Text[UintVal] = 0;// Delete character
      if (UintVal)            // Decrement pointer to last
        UintVal--;            //  character
      }
    else 
      {                       // ..button pressed is RIGHT
      tmp = 15;               // Max. 16 characters in 5202
      if ((UintVal < tmp) && (UintVal < strlen((char *)CBSetup.TextData.Text)))
        UintVal++;
      }
    }
  if (FuncTriggers & 0x30)  // Select character in CBSetup.TextData.Text
    {                       // If UP/DOWN button pressed..
    if (CBSetup.TextData.Text[UintVal] == 0) 
      {                     // If text insert.. set new character = previous
      CBSetup.TextData.Text[UintVal]     = CBSetup.TextData.Text[UintVal - 1];
      CBSetup.TextData.Text[UintVal + 1] = 0;
      }
    CBSetup.TextData.Text[UintVal] = GetNextPresetASCII((FuncTriggers & 0x10), (char)CBSetup.TextData.Text[UintVal]);
    if (!strcmp(CBSetup.TextData.Text, CBOrig.TextData.Text))
      if (CBOrig.TextData.TextEnable == UcharVal)
        FuncState = 8;  // this will cause SAVE to be displayed - NO!!
    }
  CodePtr = TextTxt;    /// check this out !!!!!!!!!! JK
  WriteCodeLN2(0, CodePtr);
  ClearRestLN2();
  WriteTxtLN2(5, ": ");
  WriteTxtLN2(7, (char *)CBSetup.TextData.Text);
  WriteCodeLN2(25, OffOnArrTxt[UcharVal]);  // Write on/off on line 2 (is now in UcharVal)
  WriteType12_SAVE_ESC(FuncState);          // Write the SAVE ESC text

  switch (FuncState)
    {
    case 1:
    case 4:                          // Mark selected item
      WriteItemArrows(7, strlen((char *)CBSetup.TextData.Text));
      CharBlinkLN2(0, OFF);          // Switch OFF blinking
      break;
    case 2:
    case 5:
    //case 6:                        // Mark selected item JK
      WriteItemArrows(25, Mstrlen(OffOnArrTxt[UcharVal]));
      CharBlinkLN2(0, OFF);          // Switch OFF blinking
      break;
    case 8:
    case 9:                          // Mark selected item
      WriteItemBrackets(7, strlen((char *)CBSetup.TextData.Text));
      CharBlinkLN2(7 + UintVal, ON);    // Switch OFF blinking at
      break;                            //  editing character
    }
  FuncTriggers = 0;
  }

void SdiMovementSelect(void)
  {
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      CBSetup.TextData.TextMovement = UcharVal;
      CBOrig.TextData.TextMovement  = UcharVal;
      //memcpy(&CBOrig, &CBSetup, sizeof(CBObject));
      }
    // restore originals if escape
    FuncSave = FuncEscape = FALSE;
    do_SdiConfig(&CBOrig, SdiMovementItem);   // Item error here !!!! (corrected) SDITextMovementItem
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal++ == 1)
        UcharVal = 0;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal-- == 0)
        UcharVal = 1;
      }
    if (UcharVal == CBOrig.TextData.TextMovement) // If actual move..--- !!!!!!!
      FuncState = 1;                  //  remove SAVE option
    CBSetup.TextData.TextMovement = UcharVal;
    do_SdiConfig(&CBSetup, SdiMovementItem);
    }
  CodePtr = OffOnArrTxt[UcharVal];  // Get text for selection
  WriteCodeLN2(0, ScrollHdrTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;
  }

void SdiSystemSelect(void)
  {

  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      CBSetup.System = UcharVal;
      // update settings if Save
      memcpy(&CBOrig, &CBSetup, sizeof(CBObject));
      }
    FuncSave = FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    do_SdiConfig(&CBOrig, SdiSystemItem);
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal++ == SNTSC0)
        UcharVal = SPAL;
      if (UcharVal == SDUMMY)
        UcharVal = SNTSC7;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal == SPAL)
        UcharVal = SNTSC0;
      else
        if (--UcharVal == SDUMMY)
          UcharVal = SPAL;
      }
    if (UcharVal == CBOrig.System) // If actual system..
      FuncState = 1;                  //  remove SAVE option

    CodePtr = AnlSystemArrTxt[UcharVal];  // Get text for selection
    WriteCodeLN2(0, SystemHdrTxt);
    WriteCode2(CodePtr);
    ClearRestLN2();
    WriteType1_SAVE_ESC(FuncState);
    if ((FuncState == 1) || (FuncState == 3))
      WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
    FuncTriggers = 0;                        // Reset the button-selection
  
    CBSetup.System = UcharVal;          // Save new SDI system
    do_SdiConfig(&CBSetup, SdiSystemItem);
    return;
    }
  CodePtr = AnlSystemArrTxt[UcharVal];  // Get text for selection
  WriteCodeLN2(0, SystemHdrTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;
  }
  
void SdiTimingSelect(void)
  {
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {                    // If delay has changed..
      Settings.ActivePreset = 0;
      CBSetup.Delay = samples / 256; // kurt
      // update settings if Save
      memcpy(&CBOrig, &CBSetup, sizeof(CBObject));
      }
    CharBlinkLN2(0, OFF);
    FuncSave = FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if escape
    do_SdiConfig(&CBOrig, SdiTimingItem);
    return;
    }
  SDITimeAdjust(UcharVal, CBOrig.Delay);
  CBSetup.Delay = samples / 256; // kurt
  do_SdiConfig(&CBSetup, SdiTimingItem);
  FuncTriggers = 0;                  // Reset button selectors
  }
    
/***************************************************************************/
/*  SdiPatternSelect                                                       */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   980222                                                      */
/*                                                                         */
/*  Function:  Select pattern in a SDI-TSG.                                */
/*  Remarks:    Cursor position on line 2 is updated.                      */
/*          Instant update implemented.                                    */
/*  Returns:    --                                                         */
/*  Updates:    TBD                                                        */

// There are two sets of patterns, one for NTSC and one for PAL
// They are not the same and index to them does not represent 
// particular pattern. Patterns are defined in cbar.h and are
// common for both systems.  Inside this function, UcharVal holds
// index to pattern names, whereas enum_ndx holds actual pattern
// number, corresponding to pattern name.

/***************************************************************************/
void SdiPatternSelect()
  {

  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      CBSetup.Pattern = enum_ndx;
      // update settings if Save
      memcpy(&CBOrig, &CBSetup, sizeof(CBObject));
      }
    FuncSave = FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if escape
    do_SdiConfig(&CBOrig, SdiPatternItem);
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 0)
        if (CBSetup.System == SPAL) // Tables with patterns are of different
          UcharVal = 18;              // sizes
        else
          UcharVal = 17;
      }
    else 
      {                      // ..button pressed is DOWN
      if (CBSetup.System == SPAL) // Tables with patterns are of different
        {                           // sizes
        if (UcharVal++ == 18)
          UcharVal = 0;
        }
      else
        if (UcharVal++ == 17)
          UcharVal = 0;
      }
/***  NOTE:  ****/
// There are two sets of patterns, one for NTSC and one for PAL
// They are not the same and index to them does not represent 
// particular pattern. Patterns are defined in cbar.h and are
// common for both systems. 

    if (CBSetup.System == SPAL)
      enum_ndx = pal_patterns[UcharVal].patt_no;
    else
      enum_ndx = ntsc_patterns[UcharVal].patt_no;

    if (enum_ndx == CBOrig.Pattern) //  If actual system..
      FuncState = 1;                //  remove SAVE option
    CBSetup.Pattern = enum_ndx;   //  Save new CB pattern
    do_SdiConfig(&CBSetup, SdiPatternItem);
    }
/* At this point UcharVal contains pattern number as defined by
   enum CBPatterns.  We must find what text corresponds to this
   pattern number.
*/

  if (CBSetup.System == SPAL)
    CodePtr = pal_patterns[UcharVal].patt_text;
  else
    CodePtr = ntsc_patterns[UcharVal].patt_text;

  WriteCodeLN2(0, PATTERNTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }


void SdiEmbeddedSelect(void)
  {
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      CBSetup.EmbAudioSignal = UcharVal;
      // update settings if Save
      memcpy(&CBOrig, &CBSetup, sizeof(CBObject));
      }
    FuncSave = FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if escape
    do_SdiConfig(&CBOrig, SdiEmbeddedItem);
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is DOWN..
      if (UcharVal++ == EmbAudioStereo1kHz)
        UcharVal = EmbAudioOff;
      }
    else 
      {                      // ..button  pressed is UP
      if (UcharVal-- == EmbAudioOff)
        UcharVal = EmbAudioStereo1kHz;
      }
    if (UcharVal == CBOrig.EmbAudioSignal)
      FuncState = 1;                      //  remove SAVE option
    CBSetup.EmbAudioSignal = UcharVal;
    do_SdiConfig(&CBSetup, SdiEmbeddedItem);
    }
  CodePtr = SdiEmbeddedArrTxt[UcharVal];
  WriteCodeLN2(0, AudioTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }
  
void SdiAnalogEdit(void)
  {
  char TextBuffer[20];
  
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      CBSetup.ScHPhase = IntVal;
      // update settings if Save
      memcpy(&CBOrig, &CBSetup, sizeof(CBObject));
      }
    FuncSave = FuncEscape = FALSE;
    Ptr = PrevPtr;                   // Set pointer to previous menu
    Ptr->DispFunct();                // Display previous menu
    // restore originals if escape
    do_SdiConfig(&CBOrig, SdiAnalogItem);
    return;
    }
  if (FuncTriggers & 0x03)
    {                               // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01)
      {                             // If button pressed is UP..
      if (++IntVal > ScHPhaseHigh)      // If max. ScH-Phase reached..
        IntVal = ScHPhaseLow;        //  .. roll-over to min. value
      }
    else 
      {                              // ..button pressed is DOWN
      if (--IntVal < ScHPhaseLow)   // If min. ScH-Phase reached..
        IntVal = ScHPhaseHigh;       //  .. roll-over to max. value
      }
    if (IntVal == CBOrig.ScHPhase)
      FuncState = 1;                 //  remove SAVE option
    CBSetup.ScHPhase = IntVal;
    do_SdiConfig(&CBSetup, SdiAnalogItem);
    }
  WriteCodeLN2(0, ScHPhaseHdrTxt);
  sprintf(TextBuffer, "%+4d", IntVal);
  Mstrcat(TextBuffer, degTxt);          // Add prefix deg
  WriteTxt2(TextBuffer);              // Write phase value
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);        // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(11, strlen(TextBuffer));  // Mark selected item
  FuncTriggers = 0;   // Reset button selectors               
  }

/**************************** Audio Functions ************************/
/*
    In AudioMenuShow Ptr = &audio_menu[AudioTypeNdx].
                     Ptr = &audio_menu[AudioTypeNdx];
*/
void AudioMenuShow(void)
  {
  register int i;

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoSelectTxt);            // Write sub-menu headline
  ClearRestLN1();
  //AudioNdx[AudioTypeNdx] = Ptr->ItemNumber;
  WriteCodeLN2(0, SubmnuHdrTxt);
  for (i = AnalogAudioItem ; i <= DigitalAudioItem; i++)
    {
    ClearToPosLN2(audio_menu[i].ItemPos);
    WriteCode2(audio_menu[i].ItemTxt);
    }
  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));
 }

 
void AudioDown(void)
  {
  switch (AudioTypeNdx)                // JK here!
    {
    case 0:
      Ptr = &analog_audio_submenu[AudioNdx[AudioTypeNdx]]; // this chooses analog or digital
      break;
    case 1:
      Ptr = &digital_audio_submenu[AudioNdx[AudioTypeNdx]]; // this chooses analog or digital
      break;
    default:
      break;
    }
  FuncState    = 1;
  FuncTriggers = 0;
  FuncSave     = FuncEscape = FALSE;
  }

/* 
     here Ptr = analog_audio_submenu
*/
void AnalogAudioMenuShow(void)
  {
  register int i;
  
  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  CodePtr = audio_menu[AudioTypeNdx].ItemTxt;
  WriteCode1(audio_menu[AudioTypeNdx].ItemTxt);
  WriteCode1(CoSelectTxt);            // Write sub-menu headline
  ClearRestLN1();
  AudioNdx[AudioTypeNdx] = Ptr->ItemNumber;
  WriteCodeLN2(0, SecondmnuHdrTxt);
  for (i = 0; i <= 2; i++)
    {
    ClearToPosLN2(analog_audio_submenu[i].ItemPos);    // Find item column position
    WriteCode2(analog_audio_submenu[i].ItemTxt);      // Write item texts
    }
  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);      // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));
  }
  
/***************************************************************************/
/* AnalogAudioDown                                             MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  Go to the selected item in the Analog Audio submenu.        */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:   None                                                        */
/*                                                                         */
/*  Note: We copy AudioSetup to AudioOrig on entry here.  Normally, these  */
/*        two are kept synchronized.  However, when AudioSetup  are        */
/*        altered via remote, AudioOrig is not updated.  On entry here     */
/*        AudioOrig will always reflect AudioSetup for correct menu        */
/*        operation.                                                       */
/***************************************************************************/
void AnalogAudioDown(void)
  { 
  memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioObject));
  WriteCodeLN1(0, SecondmnuHdrTxt);
  WriteCode1(SlashDotHdrTxt);
  WriteCode1(audio_menu[AudioTypeNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(analog_audio_submenu[AudioNdx[AudioTypeNdx]].ItemTxt);
  switch (AudioNdx[AudioTypeNdx])
    {
    case AnalogAudioClickItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AnalogAudio].Click;
      break;
    case AnalogAudioSignalItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AnalogAudio].Signal;
      break;
    case AnalogAudioLevelItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AnalogAudio].Level;
      break;
    }
  ClearRestLN1();
  PrevPtr = Ptr;
  Ptr = &analog_audio_sub_submenu[AudioNdx[AudioTypeNdx]];
  FuncState = 1;
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
  }

void AnalogAudioClickSelect(void)
  {

   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AnalogAudio].Click = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioObject));
      AudioOrig[AnalogAudio].Click = AudioSetup[AnalogAudio].Click;
      }
    do_AnalogConfig(&AudioOrig[AnalogAudio], AnalogAudioClickItem);
    Ptr = PrevPtr;         // Set pointer to previous menu
    Ptr->DispFunct();      // Display previous menu
    // restore originals if Esc    
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    /* NOTE: click can have values of 1 or 3 (not 0 or 1, as usual).  Hence
         the algorithm below                                                */
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (++UcharVal == 2)     // was 1
        UcharVal = 3;
      if (UcharVal == 4)       // was 3
        UcharVal = 1;
      }
    else 
      {                        // ..button pressed is DOWN
      if (UcharVal == 1)
        UcharVal = 3;
      else
        if (--UcharVal == 2)
          UcharVal = 1;
      }
    if (UcharVal == AudioOrig[AnalogAudio].Click)
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AnalogAudio].Click = UcharVal;
    do_AnalogConfig(&AudioSetup[AnalogAudio], AnalogAudioClickItem);
    }
  CodePtr = AnalogAudioClickArrTxt[UcharVal];
  WriteCodeLN2(0, CLICKTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }
  
void AnalogAudioSignalSelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AnalogAudio].Signal = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioObject));
      AudioOrig[AnalogAudio].Signal = AudioSetup[AnalogAudio].Signal;
      }
    do_AnalogConfig(&AudioOrig[AnalogAudio], AnalogAudioSignalItem);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if Esc    
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 0)
        UcharVal = 3;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 3)
        UcharVal = 0;
      }
    if (UcharVal == AudioOrig[AnalogAudio].Signal)
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AnalogAudio].Signal = UcharVal;
    do_AnalogConfig(&AudioSetup[AnalogAudio], AnalogAudioSignalItem);
   }
  CodePtr = AnalogAudioSignalArrTxt[UcharVal];
  WriteCodeLN2(0, TONETxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
 }
  
void AnalogAudioLevelSelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AnalogAudio].Level = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioObject));
      AudioOrig[AnalogAudio].Level = AudioSetup[AnalogAudio].Level;
      }
    do_AnalogConfig(&AudioOrig[AnalogAudio], AnalogAudioLevelItem);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    // restore originals if Esc    
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 1)
        UcharVal = 31;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 31)
        UcharVal = 1;
      }
    if (UcharVal == AudioOrig[AnalogAudio].Level)
      FuncState = 1;
    AudioSetup[AnalogAudio].Level = UcharVal;
    do_AnalogConfig(&AudioSetup[AnalogAudio], AnalogAudioLevelItem);
    }
  CodePtr = AnalogAudioLevelArrTxt[UcharVal];
  WriteCodeLN2(0, LEVELTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark  selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }

/*
    Here Ptr = digital_audio_submenu;
*/
void DigitalAudioMenuShow(void)
  {
  register int i;
  int number;       // number of items to display in line 2
  int start;
  
  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  CodePtr = audio_menu[AudioTypeNdx].ItemTxt;
  WriteCode1(audio_menu[AudioTypeNdx].ItemTxt);
  WriteCode1(CoSelectTxt);            // Write sub-menu headline
  ClearRestLN1();
  AudioNdx[AudioTypeNdx] = Ptr->ItemNumber;
  WriteCodeLN2(0, SecondmnuHdrTxt);
  tmpPtr = Ptr;
  switch (AudioNdx[AudioTypeNdx]) 
    {
    case 0:
    case 1:
    case 2:
      Ptr    = &digital_audio_submenu[DigitalAudioSignalItem];
      start  = DigitalAudioSignalItem;
      number = 3;
      break;
    case 3:
    case 4:
    case 5:
      Ptr    = &digital_audio_submenu[DigitalAudioSystemItem];
      start  = DigitalAudioSystemItem;
      number = 3;
      break;
    default:
      Ptr    = &digital_audio_submenu[DigitalAudioSignalItem];
      start  = DigitalAudioSignalItem;
      number = 3;
      break;
    }
  for (i = start; i < start + number; i++) 
    {
    ClearToPosLN2(Ptr->ItemPos);    // Find item column position
    WriteCode2(Ptr->ItemTxt);       // text is in code memory
    Ptr = Ptr->NextItem;
    }
  ClearRestLN2();
  Ptr = tmpPtr;
  i = Ptr->ItemConfig;     // Get info on line 1&2 icons/dots
  WriteArrowsField(i);     // Write line 1&2 icons/dots
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));  // Mark selected item (start, length)
  }
  
/***************************************************************************/
/* DigitalAudioDown                                            MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  Go to the selected item in the Digital Audio submenu.       */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:   None                                                        */
/*                                                                         */
/*  Note: We copy AudioSetup to AudioOrig on entry here.  Normally, these  */
/*        two are kept synchronized.  However, when AudioSetup  are        */
/*        altered via remote, AudioOrig is not updated.  On entry here     */
/*        AudioOrig will always reflect AudioSetup for correct menu        */
/*        operation.                                                       */
/***************************************************************************/
void DigitalAudioDown(void)
  {
  memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
  WriteCodeLN1(0, SecondmnuHdrTxt);
  WriteCode1(SlashDotHdrTxt);
  WriteCode1(audio_menu[AudioTypeNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(digital_audio_submenu[AudioNdx[AudioTypeNdx]].ItemTxt);
  switch (AudioNdx[AudioTypeNdx])
    {
    case DigitalAudioClickItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AESEBUAudio].Click;
      break;
    case DigitalAudioSignalItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AESEBUAudio].Signal;
      break;
    case DigitalAudioLevelItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AESEBUAudio].Level;
      break;
    case DigitalAudioTimingItem:
      WriteCode1(CoEditTxt);
      FloatVal = AudioSetup[AESEBUAudio].Timing;
      break;
    case DigitalAudioFrequencyItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AESEBUAudio].Frequency;
      break;    
    case DigitalAudioSystemItem:
      WriteCode1(CoSelectTxt);
      UcharVal = AudioSetup[AESEBUAudio].System;
      break;    
    }
  ClearRestLN1();
  PrevPtr = Ptr;
  Ptr = &digital_audio_sub_submenu[AudioNdx[AudioTypeNdx]];
  FuncState = 1;
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
  }

void DigitalAudioClickSelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AESEBUAudio].Click = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
      AudioOrig[AESEBUAudio].Click = AudioSetup[AESEBUAudio].Click;
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioClickItem);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    /* NOTE: click can have values of 1 or 3 (not 0 or 1, as usual).  Hence
         the algorithm below                                                */
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (++UcharVal == 2) // was 1
        UcharVal = 3;
      if (UcharVal == 4)   // was 3
        UcharVal = 1;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal == 1)
        UcharVal = 3;
      else
        if (--UcharVal == 2)
          UcharVal = 1;
      }
    if (UcharVal == AudioOrig[AESEBUAudio].Click) // If actual system..
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AESEBUAudio].Click = UcharVal;
    do_AesebuConfig(&AudioSetup[AESEBUAudio], DigitalAudioClickItem);
    }
  CodePtr = AnalogAudioClickArrTxt[UcharVal];
  WriteCodeLN2(0, CLICKTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }
  
void DigitalAudioSignalSelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AESEBUAudio].Signal = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
      AudioOrig[AESEBUAudio].Signal = AudioSetup[AESEBUAudio].Signal;
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioSignalItem);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 0)
        UcharVal = 3;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 3)
        UcharVal = 0;
      }
    if (UcharVal == AudioOrig[AESEBUAudio].Signal)
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AESEBUAudio].Signal = UcharVal;
    do_AesebuConfig(&AudioSetup[AESEBUAudio], DigitalAudioSignalItem);
    }
  CodePtr = AnalogAudioSignalArrTxt[UcharVal];
  WriteCodeLN2(0, TONETxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;
  }
  
void DigitalAudioLevelSelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AESEBUAudio].Level = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
      AudioOrig[AESEBUAudio].Level = AudioSetup[AESEBUAudio].Level;
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioLevelItem);
    Ptr = PrevPtr;         // Set pointer to previous menu
    Ptr->DispFunct();      // Display previous menu
    FuncSave = FuncEscape = FALSE;
    return;
    }

  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 0)
        UcharVal = 7;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 7)
        UcharVal = 0;
      }
    if (UcharVal == AudioOrig[AESEBUAudio].Level) // If actual system..
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AESEBUAudio].Level = UcharVal; // Save new BB system
    do_AesebuConfig(&AudioSetup[AESEBUAudio], DigitalAudioLevelItem);
    }
  CodePtr = DigitalAudioLevelArrTxt[UcharVal];  // Get text for selection !!-- error
  WriteCodeLN2(0, LEVELTxt);        // Write header on line 2
  WriteCode2(CodePtr);                  // Write selected text on line 2
  ClearRestLN2();                       // Clear rest of line 2
  WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }


void DigitalAudioTimingSelect(void)
  {
  char TextBuffer[20];
  // Timing only if word clock = 48 kHz
  if (AudioSetup[AESEBUAudio].Frequency ==f441kHz)
    {
    Ptr = PrevPtr;                   // Set pointer to previous menu
    Ptr->DispFunct();                // Display previous menu
    return;
    }
  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {                 // If timing has changed..
      AudioSetup[AESEBUAudio].Timing = FloatVal; // --------- JK
      // update settings if Save
      //memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
      AudioOrig[AESEBUAudio].Timing = AudioSetup[AESEBUAudio].Timing;
      }
    Ptr = PrevPtr;                   // Set pointer to previous menu
    Ptr->DispFunct();                // Display previous menu
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioTimingItem);
    return;
    }
  if (FuncTriggers & 0x03)
    {                               // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01)
      {  
      FloatVal += 0.8;
      if (FloatVal > 10.5) 
        FloatVal = -9.6;
      }
    else                       // ..button pressed is DOWN
      {    
      FloatVal -= 0.8;                         
      if (FloatVal < -9.7)
        FloatVal = 10.4;       //  .. roll-over to max. value
      }
/*  NOTE: Below we must compare two float values.  It does not perform very well,
          probably due to compiler implementation of float compare.  We multiply
          timings by 10, conver them to int and then compare.
*/
    if ((int)(FloatVal * 10) == (int)(AudioOrig[AESEBUAudio].Timing * 10))
      FuncState = 1;                 //  remove SAVE option
    AudioSetup[AESEBUAudio].Timing = FloatVal;
    do_AesebuConfig(&AudioSetup[AESEBUAudio], DigitalAudioTimingItem);
    }
  WriteCodeLN2(0, TimingHdrTxt);      // Write line 2 header
  sprintf(TextBuffer, "%+03.1f", FloatVal); // Convert timing to ascii
  WriteTxt2(TextBuffer);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);        // Write the SAVE ESC text
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, strlen(TextBuffer));  // Mark selected item
  FuncTriggers = 0;                  // Reset button selectors
  }


void DigitalAudioSystemSelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AESEBUAudio].System = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
      AudioOrig[AESEBUAudio].System = AudioSetup[AESEBUAudio].System;
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioSystemItem);
    Ptr = PrevPtr;
    Ptr->DispFunct();
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 0)
        UcharVal = 1;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 1)
        UcharVal = 0;
      }
    if (UcharVal == AudioOrig[AESEBUAudio].System) // If actual system..
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AESEBUAudio].System = UcharVal; // Save new system
    do_AesebuConfig(&AudioSetup[AESEBUAudio], DigitalAudioSystemItem);
    }
  CodePtr = AESEBUSystemArrTxt[UcharVal];  // Get text for selection !!-- error : corrected
  WriteCodeLN2(0, SystemHdrTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);
  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));
  FuncTriggers = 0;                      // Reset the button-selection
  }
  
void DigitalAudioFrequencySelect(void)
  {
   if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      AudioSetup[AESEBUAudio].Frequency = UcharVal;
      // update settings if Save
      //memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioObject));
      AudioOrig[AESEBUAudio].Frequency = AudioSetup[AESEBUAudio].Frequency;
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioFrequencyItem);
    Ptr = PrevPtr;         // Set pointer to previous menu
    Ptr->DispFunct();      // Display previous menu
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal-- == 0)
        UcharVal = 1;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 1)
        UcharVal = 0;
      }
    if (UcharVal == AudioOrig[AESEBUAudio].Frequency) // If actual system..
      FuncState = 1;                      //  remove SAVE option
    AudioSetup[AESEBUAudio].Frequency = UcharVal; // Save new BB system
    do_AesebuConfig(&AudioSetup[AESEBUAudio], DigitalAudioFrequencyItem);
    }

  CodePtr = DigitalAudioFrequencyArrTxt[UcharVal];
  WriteCodeLN2(0, WCLKTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType1_SAVE_ESC(FuncState);

  if ((FuncState == 1) || (FuncState == 3))
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }


void UpToAudio(void)
  {
  Ptr = &audio_menu[AudioTypeNdx]; // this chooses between analog and digital
  }

/**************************** Audio Functions ************************/


/***************************************************************************/
/* PresetMenuShow                                              MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                  */
/*  Function:  Display the items in the PRESET menu.                */
/*  Remarks:    Cursor position on line 1 & 2 are updated.            */
/*  Returns:    --                                        */
/*  Updates:    PresetNdx                                  */
/***************************************************************************/
void PresetMenuShow()
  {
  register int i;

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoSelectFunctionTxt);
  ClearRestLN1();
  PresetNdx = Ptr->ItemNumber;
  WriteCodeLN2(0, SubmnuHdrTxt);

  for (i = PresetRecallItem; i <= PresetNameItem; i++)
    {
    ClearToPosLN2(preset_menu[i].ItemPos);
    WriteCode2(preset_menu[i].ItemTxt);
    }
  ClearRestLN2();
  WriteArrowsField(Ptr->ItemConfig);
  WriteItemArrows(Ptr->ItemPos, Mstrlen(Ptr->ItemTxt));
}

/***************************************************************************/
/* PresetDown                                                  MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  Go to the selected item in the PRESET menu.                 */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:                                                               */
/***************************************************************************/
void PresetDown()
  {

  // copy all [4] names into org names.
  memcpy(&OrgPresetNames, &PresetNames, sizeof(PresetNames));

  WriteCodeLN1(0, SubmnuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(preset_menu[PresetNdx].ItemTxt);  // Write headline

  if (Settings.ActivePreset)              // If preset is active..
    UcharVal = Settings.ActivePreset - 1; //  select currently active preset
  else
    UcharVal = 0;                         // ..otherwise select preset ONE

  switch (PresetNdx)
    {
    case PresetRecallItem:
    case PresetStoreItem:
      WriteCode1(CoSelectTxt);
      break;
    case PresetNameItem:
      WriteCode1(CoEditNameTxt);
      //strcpy(GlobalBuffer, Preset[UcharVal].Name);
      strcpy(GlobalBuffer, PresetNames[UcharVal]);
      if (UintVal = strlen(GlobalBuffer)) // Position in Text string
        UintVal--;
      break;
    }
  ClearRestLN1();
  PrevPtr = Ptr;
  Ptr = &preset_submenu[PresetNdx]; // Point to selected item
  FuncState = 1;                    // Prepare for item state-machine
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
  }

/***************************************************************************/
/* PresetRecallSelect                                          MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  To recall selected preset                                   */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:                                                               */
/***************************************************************************/
void PresetRecallSelect()
  {
  char * TxtPtr;
  
  if (FuncEscape)
    {
    FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  if (FuncSave)
    {
    do_RecallPreset(UcharVal + 1, &AudioTypeNdx);
    FuncSave = FALSE;
    Ptr = PrevPtr;        // Set pointer to previous menu
    Ptr->DispFunct();     // Display previous menu
    return;
    }
  if (FuncTriggers & 0x03)
    {                        // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01)
      {                      // If button pressed is UP..
      if (UcharVal++ == 3)
        UcharVal = 0;
      }
    else                     // ..button pressed is DOWN
      if (UcharVal-- == 0)
        UcharVal = 3;
    }
  TxtPtr = PresetNames[UcharVal];    // Get text for selection
  WriteCodeLN2(0, RecallHdrTxt);     // Write header on line 2
  WriteCharLN2(6, '1' + UcharVal);   // This DESTROYS line 2 position
  WriteTxtLN2(9, TxtPtr);            // Write selected text on line 2
  ClearRestLN2();
  WriteType3_OK_ESC(FuncState);
  if (FuncState == 1)
    WriteItemArrows(9, strlen(TxtPtr));  // Mark selected item
  FuncTriggers = 0;                      // Reset the button-selection
  }

/***************************************************************************/
/* PresetStoreSelect                                           MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  To store settings into selected preset                      */
/*  Remarks:   Cursor position on line 1 is updated.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:                                                               */
/***************************************************************************/
void PresetStoreSelect()
  {
  char * TxtPtr;
  
  if (FuncEscape)
    {
    FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  if (FuncSave)
    {
    do_StorePreset(UcharVal + 1);

    do_NamePreset(UcharVal, PresetNames[UcharVal]);

    FuncSave = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }

  if (FuncTriggers & 0x03)
    {          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01)
      {          // If button pressed is UP..
      if (UcharVal++ == 3)
        UcharVal = 0;
      }
    else                        // ..button pressed is DOWN
      if (UcharVal-- == 0)
        UcharVal = 3;
    }
  TxtPtr = PresetNames[UcharVal];        // Get text for selection
  WriteCodeLN2(0, StoreHdrTxt);        // Write header on line 2
  WriteCharLN2(5, '1' + UcharVal);      // This DESTROYS line 2 position
  WriteTxtLN2(8, TxtPtr);            // Write selected text on line 2
  ClearRestLN2();                  // Clear rest of line 2
  WriteType3_OK_ESC(FuncState);        // Write the SAVE ESC text
  if (FuncState == 1)
    WriteItemArrows(8, strlen(TxtPtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }

/***************************************************************************/
/* PresetNameSelect                                            MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 12.02.2003                                               */
/* Revised:   12.02.2003                                                   */
/*                                                                         */
/*  Function:  To edit name of a selected preset                           */
/*  Remarks:                                                               */
/*  Returns:   Nothing                                                     */
/*  Updates:                                                               */
/***************************************************************************/
void PresetNameSelect()
  {
  if (FuncSave)
    {
    strcpy(PresetNames[UcharVal], GlobalBuffer);
    strcpy(OrgPresetNames[UcharVal], GlobalBuffer);
    do_NamePreset(UcharVal, GlobalBuffer);
    CharBlinkLN2(0, OFF);
    FuncSave = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  if (FuncEscape)
    {
    strcpy(PresetNames[UcharVal], OrgPresetNames[UcharVal]);
    CharBlinkLN2(0, OFF);
    FuncEscape = FALSE;
    Ptr = PrevPtr;
    Ptr->DispFunct();
    return;
    }
  // Select preset to name
  if (FuncTriggers & 0x03)
    {                                  // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01)
      {                                // If button  pressed is UP..
      if (UcharVal++ == NoOfPresets - 1)
        UcharVal = 0;
      }                               // ..button pressed is DOWN
    else 
      {
      if (UcharVal-- == 0)
        UcharVal = NoOfPresets - 1;
      }                               // Copy selected preset name to global buffer
    strcpy(GlobalBuffer, PresetNames[UcharVal]);
    if (UintVal = strlen(GlobalBuffer))    // Find position in string
      UintVal--;
    FuncState = 1;
    }
                               // Select GlobalBuffer position
  if (FuncTriggers & 0xC0)
    {                          // If LEFT/RIGHT button pressed..
    if (FuncTriggers & 0x40)
      {                        // If button pressed is LEFT..
      GlobalBuffer[UintVal] = 0;
      if (UintVal)
        UintVal--;
      }
    else
      {                        // ..button pressed is RIGHT
      if ((UintVal < 15) && (UintVal < strlen(GlobalBuffer)))
        UintVal++;
      }
    }
                                    // Select character for GlobalBuffer
  if (FuncTriggers & 0x0C)
    {                               // If UP/DOWN button pressed..
    if (GlobalBuffer[UintVal] == 0)
      {                             // If text insert..set new character = previous
      GlobalBuffer[UintVal] = GlobalBuffer[UintVal-1];
      GlobalBuffer[UintVal+1] = 0;
      }
    GlobalBuffer[UintVal] = GetNextPresetASCII((FuncTriggers & 0x04), GlobalBuffer[UintVal]);
    if (!strcmp(PresetNames[UcharVal], GlobalBuffer))
      FuncState = 6;
    }
  WriteCodeLN2(0, NameHdrTxt);
  WriteCharLN2(4, '1' + UcharVal);        // This DESTROYS line 2 position
  WriteTxtLN2(7, GlobalBuffer);
  ClearRestLN2();
  WriteType8_SAVE_ESC(FuncState);
  switch (FuncState)
    {
    case 1:
    case 3:                      // Mark selected item
      WriteItemArrows(7, strlen(GlobalBuffer));
      CharBlinkLN2(0, OFF);
      break;
    case 6:
    case 7:                      // Mark selected item
      WriteItemBrackets(7, strlen(GlobalBuffer));
      CharBlinkLN2(7+UintVal, ON); // Switch OFF blinking at
      break;                       //  editing character
    }
  FuncTriggers = 0; // Reset the button-selection
  }


void FactoryResetSystemSelect(void)
  {
  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteChar1('/');
  WriteCode1(system_menu.ItemTxt);
  WriteCode1(CoSelectTxt);
  if (FuncSave || FuncEscape) 
    {
    if (FuncSave)
      {
      Settings.ActivePreset = 0;
      do_FactoryReset(UcharVal);
      init_configs(&AudioTypeNdx); // update configs after factory reset
      FuncSave = FALSE;
      FuncTriggers = 0;
      Ptr = &status_menu[0];
      Ptr->DispFunct();
      return;
      }
    Ptr = PrevPtr;         // Set pointer to previous menu
    Ptr->DispFunct();      // Display previous menu
    FuncTriggers = 0;
    FuncSave = FuncEscape = FALSE;
    return;
    }

  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
    if (FuncTriggers & 0x01) 
      {                        // If button pressed is UP..
      if (UcharVal++ == SNTSC0)
        UcharVal = SPAL;
      if (UcharVal == SDUMMY)
        UcharVal = SNTSC7;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal == SPAL)
        UcharVal = SNTSC0;
      else
        if (--UcharVal == SDUMMY)
          UcharVal = SPAL;
      }
    }
  CodePtr = AnlSystemArrTxt[UcharVal];  // systems the same as for SDI
  WriteCodeLN2(0, SystemHdrTxt);
  WriteCode2(CodePtr);
  ClearRestLN2();
  WriteType3_OK_ESC_JK(FuncState);
  if (FuncState == 1)
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;
  }

void CalibrationMenuShow(void)
  {
  register int i, start, number;

  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {
      do_cal_store();
      }
    do_cal_enable_off();
    Ptr = PrevPtr;
    Ptr->DispFunct();
    FuncEscape = FALSE;
    FuncSave   = FALSE;
    return;
    }

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoSelectTxt);
  ClearRestLN1();                 
  
  if (FuncTriggers & 0x30)      // MAX, ie. 3rd item
    {                           // If button pressed is UP/DOWN..
    if (FuncTriggers & 0x10)    // If button pressed is UP .. (i.e EXEC)
      send_min();
    }
  if (FuncTriggers & 0x0C)    // MIN, ie. second item
    {                         // If button pressed is UP/DOWN..
    if (FuncTriggers & 0x04)  // If button pressed is UP.. (i.e EXEC)
      send_max();
    }
  if (FuncTriggers & 0x03)        // ADJ, ie. first item
    {
    if (FuncTriggers & 0x01)      // If button pressed is UP/DOWN..
      {                           // If button pressed is UP..
      send_down();
      }
    else 
      {
      send_up();
      }
    }

  tmpPtr = Ptr;                      // Save menu pointer temporarily
  Ptr    = &calibration_menu[0];
  WriteCodeLN2(0, SubmnuHdrTxt);     // Write header for sub-menu line
  start  = CalibrationMaxItem;
  number = 3;
  for (i = start; i < start + number; i++) 
    {
    ClearToPosLN2(Ptr->ItemPos);    // Find item column position
    WriteCode2(Ptr->ItemTxt);       // text is in code memory
    Ptr = Ptr->NextItem;
    }
  ClearRestLN2();
  Ptr = tmpPtr;


  WriteTypeCal_SAVE_ESC(FuncState);    // Write the SAVE ESC text
  switch (FuncState)
    {
    case 1:
    case 5:
      WriteItemArrows(8, 3);        // Highlight selection
      break;
    case 2:
    case 6:
      WriteItemArrows(13, 3);      // Highlight selection
      break;
    case 3:
    case 7:
      WriteItemArrows(18, 3);      // Highlight selection
      break;
    case 10:
    case 11:
      // execute button pressed
      break;
    }
  FuncTriggers = 0; // Reset button selectors
  }

/***************************************************************************/
/*  EscFunction                                                 MENUHLP.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 940401                                    */
/* Revised:    970725                                                      */
/*                                                                         */
/*  Function:  Function to terminate a menu/selection without using the    */
/*           ESC-item in the menu                                          */
/*  Remarks:    FuncSave MUST be set to FALSE, as the EscFunction may be   */
/*           called from within a save function, see MENU.C51              */
/*  Returns:    --                                                         */
/*  Updates:    Ptr                                                        */
/***************************************************************************/
void EscFunction(void)
  {
  FuncSave = FALSE;
  FuncEscape = TRUE;
  }

/***************************************************************************/
/*  WriteType1_SAVE_ESC                              MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    980827                                       */
/*                                                  */
/*  Function:  Handle the SAVE and ESC text in the <SAVE ESC> display     */
/*          handling                                    */
/*  Remarks:    Part of <SAVE ESC> display handling                  */
/*          Reference: PM5689 NICAM Monitor, Menu structure document    */
/*          Cursor postion on line 2 is updated                  */
/*  Returns:    --                                        */
/* Updates:    Vertical_Repeat                              */
/***************************************************************************/
void WriteType1_SAVE_ESC(char FuncState)
  {
  WriteAddressLN2(30);
  if (FuncState > 2)
    WriteCode2(SAVETxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();
  switch (FuncState)
    {
    case 1:
    case 3:
      WriteArrowsField(L_+D_+U_+R_);
      //VerticalRepeat = TRUE;
      break;
    case 4:
      WriteItemArrows(30, Mstrlen(SAVETxt));
      WriteArrowsField(L_+R_+E_);
      //VerticalRepeat = FALSE;
      break;
    default:
      WriteItemArrows(36, Mstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);
      //VerticalRepeat = FALSE;
      break;
    }
  }


/***************************************************************************/
/*  UpButType1                                    MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the UP-button in the <SAVE ESC> state machine      */
/*  Remarks:    Part of <SAVE ESC> display handling                  */
/*          Reference: PM5689 NICAM Monitor, Menu structure document    */
/*  Returns:    --                                        */
/* Updates:    Ptr, FuncState and FuncTriggers                    */
/***************************************************************************/
void UpButType1(void)
  {
  switch (FuncState)
    {
    case 1:
      FuncState = 3;                      // Break IS missing
    case 3:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;
      break;
    case 2:
    case 5:
      FuncEscape = TRUE;
      return;
    }
  }

/***************************************************************************/
/*  DownButType1                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the DOWN-button in the <SAVE ESC> state machine    */
/*  Remarks:    Part of <SAVE ESC> display handling                  */
/*          Reference: PM5689 NICAM Monitor, Menu structure document    */
/*  Returns:    --                                        */
/* Updates:    FuncState and FuncTriggers                        */
/***************************************************************************/
void DownButType1(void)
  {
  switch (FuncState)
    {
    case 1:
      FuncState = 3;                    // Break IS missing
    case 3:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x02;
      break;
    }
  }

/***************************************************************************/
/*  LeftButType1                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the LEFT-button in the <SAVE ESC> state machine    */
/*  Remarks:    Part of <SAVE ESC> display handling                  */
/*          Reference: PM5689 NICAM Monitor, Menu structure document    */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void LeftButType1(void)
  {
  switch (FuncState) 
    {
    case 3:
      FuncState++;                    // Break IS missing
    case 1:
      FuncState++;
      break;
    default:
      FuncState--;
      break;
    }
  }

/***************************************************************************/
/*  RightButType1                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the RIGHT-button in the <SAVE ESC> state machine    */
/*  Remarks:    Part of <SAVE ESC> display handling                  */
/*          Reference: PM5689 NICAM Monitor, Menu structure document    */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void RightButType1(void)
  {
  switch (FuncState) 
    {
    case 5:
      FuncState--;   // Break IS missing
    case 2:
      FuncState--;
      break;

    default:
      FuncState++;
      break;
    }
  }

/***************************************************************************/
/*  ExecButType1                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the EXECUTE-button in the <SAVE ESC> state machine  */
/*  Remarks:    Part of <SAVE ESC> display handling                  */
/*          Reference: PM5689 NICAM Monitor, Menu structure document    */
/*  Returns:    --                                        */
/* Updates:    FuncSave                                    */
/***************************************************************************/
void ExecButType1(void)
  {
  if (FuncState == 4)
    FuncSave = TRUE;
  }
/***************************************************************************/
/*  WriteType3_OK_ESC                                MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    980827                                       */
/*                                                  */
/*  Function:  Handle the OK and ESC text in the <OK ESC> display  handling */
/*  Remarks:    Part of <OK ESC> display handling                  */
/*  Returns:    --                                        */
/* Updates:    --                                        */
/***************************************************************************/
void WriteType3_OK_ESC(char FuncState)
  {
  WriteCodeLN2(31, OKTxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();
  switch (FuncState) 
    {
    case 1:
      WriteArrowsField(L_+U_+R_+D_); // ???
      break;
    case 2:
      WriteItemArrows(31, Mstrlen(OKTxt));
      WriteArrowsField(L_+R_+E_);
      break;
    case 3:
      WriteItemArrows(36, Mstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);
      break;
    }
  } 


void WriteType3_OK_ESC_JK(char FuncState)
  {
  WriteCodeLN2(31, SAVETxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();
  switch (FuncState) 
    {
    case 1:
      WriteArrowsField(L_+U_+R_+D_); // ???
      break;
    case 2:
      WriteItemArrows(31, Mstrlen(SAVETxt));
      WriteArrowsField(L_+R_+E_);
      break;
    case 3:
      WriteItemArrows(36, Mstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);
      break;
    }
  } 

#if 1
/* These two functions manage only 1 and 3 (not 2, which is normally OK) */
void LeftButTypeJK() 
  {
  if (FuncState == 3)
    FuncState = 1;
  else
    FuncState = 3;
  }

void RightButTypeJK() 
  {
  if (FuncState == 1)
    FuncState = 3;
  else
    FuncState = 1;
  }
#endif

void WriteTypeJK_ESC(char FuncState)
  {
  WriteCodeLN2(31, OKTxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();

  switch (FuncState) 
    {
    case 1:
      WriteArrowsField(L_+D_+U_+R_+E_); // ???
      break;
#if 1      
    case 2:
      WriteItemArrows(31, Mstrlen(OKTxt));
      WriteArrowsField(L_+R_+E_);
      break;
#endif      
    case 3:
      WriteItemArrows(36, Mstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);
      break;
    }
  }
  
/***************************************************************************/
/*  UpButType3                                    MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the UP-button in the <OK ESC> state machine        */
/*  Remarks:    Part of <OK ESC> display handling                  */
/*  Returns:    --                                        */
/* Updates:    Ptr                                      */
/***************************************************************************/
void UpButType3() 
  {
  switch (FuncState)  
    {
    case 1:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;
      break;
    case 3:
      FuncEscape = TRUE;
      break;
    }
  }

/***************************************************************************/
/*  DownButType3                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the DOWN-button in the <OK ESC> state machine      */
/*  Remarks:    Part of <OK ESC> display handling                  */
/*  Returns:    --                                        */
/* Updates:    Ptr                                      */
/***************************************************************************/
void DownButType3()
  {
  if (FuncState == 1)
    FuncTriggers = (FuncTriggers & 0xFC) | 0x02;
  }

/***************************************************************************/
/* LeftButType3                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the LEFT-button in the <OK ESC> state machine      */
/*  Remarks:    Part of <OK ESC> display handling                  */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void LeftButType3() 
  {
  if (FuncState == 1)
    FuncState = 3;
  else
    FuncState--;
  }


/***************************************************************************/
/*  RightButType3                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the RIGHT-button in the <OK ESC> state          */
/*          machine                                    */
/*  Remarks:    Part of <OK ESC> display handling                  */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void RightButType3() 
  {
  if (FuncState == 3)
    FuncState = 1;
  else
    FuncState++;
  }

/***************************************************************************/
/*  ExecButType3                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970401                          */
/* Revised:    970908                                       */
/*                                                  */
/*  Function:  Handle the EXECUTE-button in the <OK ESC> state  machine    */
/*  Remarks:    Part of <OK ESC> display handling                  */
/*  Returns:    --                                        */
/* Updates:    FuncSave                                    */
/***************************************************************************/
void ExecButType3() 
  {
  if (FuncState == 2)
    FuncSave = TRUE;
  }

void ExecButTypeJK() 
  {
  if ((FuncState == 1)  || (FuncState == 2))
    FuncSave = TRUE;
  }

/***************************************************************************/
/*  WriteType6_SAVE_ESC                                         MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    980827                                       */
/*                                                  */
/*  Function:  Handle the SAVE and ESC text in the <SAVE ESC> display     */
/*          handling                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*              Cursor postion on line 2 is updated                        */
/*  Returns:    --                                        */
/* Updates:    VerticalRepeat                                */
/***************************************************************************/
void WriteType6_SAVE_ESC(char FuncState)
  {
  WriteAddressLN2(30);
  if ((FuncState > 4) && (FuncState < 10) || (FuncState == 11))
    WriteCode2(SAVETxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();
  switch (FuncState)
    {
    case 1:
    case 2:
    case 5:
    case 6:
      WriteArrowsField(L_+D_+U_+R_);
      //VerticalRepeat = TRUE;
      break;
    case 3:
    case 7:
    case 10:
    case 11:
      WriteArrowsField(L_+D_+U_+R_+E_);
      //VerticalRepeat = TRUE;
      break;
    case 8:
      WriteItemArrows(30, Mstrlen(SAVETxt));
      WriteArrowsField(L_+R_+E_);
      //VerticalRepeat = FALSE;
      break;
    default:
      WriteItemArrows(36, Mstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);
      //VerticalRepeat = FALSE;
      break;
    }
  }

/***************************************************************************/
/*  UpButType6                                    MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the UP-button in the <SAVE ESC> state machine      */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    Ptr, FuncState and FuncTriggers                    */
/***************************************************************************/
void UpButType6(void)
  {
  switch (FuncState) 
    {
    case 1:
      FuncState = 5;                    // Break IS missing
    case 5:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;  // inc xx
      break;
    case 2:
      FuncState = 6;                    // Break IS missing
    case 6:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x04;  // inc yy
      break;
    case 3:
      FuncState = 7;                    // Break IS missing
    case 7:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x10;  // inc zz
      break;
    case 4:
    case 9:
      FuncEscape = TRUE;
      break;
    case 10:
      FuncState = 11;                  // Break IS missing
    case 11:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x10;  // inc zz
      break;
    }
  }

/***************************************************************************/
/*  DownButType6                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the DOWN-button in the <XY EDIT SAVE ESC> state machine*/
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState and FuncTriggers                        */
/***************************************************************************/
void DownButType6(void)
  {
  switch (FuncState)
    {
    case 1:
      FuncState = 5;                    // Break IS missing
    case 5:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x02;  // dec xx
      break;
    case 2:
      FuncState = 6;                    // Break IS missing
    case 6:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x08;  // dec yy
      break;
    case 3:
      FuncState = 7;                    // Break IS missing
    case 7:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x20;  // dec zz
      break;
    case 10:
      FuncState = 11;                  // Break IS missing
    case 11:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x20;  // dec zz
      break;
    }
  }

/***************************************************************************/
/*  LeftButType6                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the LEFT-button in the <XY EDIT SAVE ESC> state machine*/
/*  Syntax:     void LeftButType6();                            */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void LeftButType6(void)
  {
  switch (FuncState)
    {
    case 1:
      FuncState = 4;
      break;
    case 5:
      FuncState = 9;
      break;
    case 10:
    case 11:
      FuncTriggers = (FuncTriggers & 0x3F) | 0x40;  // left rr
      break;
    default:
      FuncState--;
      break;
    }
  }

/***************************************************************************/
/*  RightButType6                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the RIGHT-button in the <XY EDIT SAVE ESC> state    */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void RightButType6(void)
  {
  switch (FuncState)
    {
    case 4:
      FuncState = 1;
      break;
    case 9:
      FuncState = 5;
      break;
    case 10:
    case 11:
      FuncTriggers = (FuncTriggers & 0x3F) | 0x80;  // right rr
      break;
    default:
      FuncState++;
      break;
    }
  }

/***************************************************************************/
/*  ExecButType6                                                 MENUHLP.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970623                                    */
/* Revised:    970623                                                      */
/*                                                                         */
/*  Function:   Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state  */
/*              machine                                                    */
/*  Syntax:     void ExecButType1();                                       */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling                */
/*  Returns:    Nothing                                                    */
/*  Updates:    FuncSave                                                   */
/***************************************************************************/
void ExecButType6(void)
  {
  switch (FuncState)
    {
    case 3:
      FuncState = 10;
      break;
    case 7:
      FuncState = 11;
      break;
    case 8:
      FuncSave = TRUE;
      break;
    case 10:
      FuncState = 3;
      break;
    case 11:
      FuncState = 7;
      break;
    }
  }


/**************  CAL BEG *******************/
void ExecButTypeCal(void)
  {
  switch (FuncState)
    {
    case 2:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x04;  // inc yy (signal that MIN executed)
      break;
    case 3:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x10;  // inc zz (signal that MAX executed)
      break;
    case 8:
      FuncSave = TRUE;
      break;
    case 6:
      FuncState = 2; // if MAX or MIN executed, remove SAVE
      FuncTriggers = (FuncTriggers & 0xF3) | 0x04;  // inc yy (signal that MIN executed)
      break;
    case 7:
      FuncState = 3;
      FuncTriggers = (FuncTriggers & 0xCF) | 0x10;  // inc zz (signal that MAX executed)
      break;
    }
  }

void RightButTypeCal(void)
  {
  switch (FuncState)
    {
    case 4:
      FuncState = 1;
      break;
    case 9:
      FuncState = 5;
      break;
    default:
      FuncState++;
      break;
    }
  }

void LeftButTypeCal(void)
  {
  switch (FuncState)
    {
    case 1:
      FuncState = 4;
      break;
    case 5:
      FuncState = 9;
      break;
    default:
      FuncState--;
      break;
    }
  }

void DownButTypeCal(void)
  {
  switch (FuncState)
    {
    case 1:
      FuncState = 5;
      FuncTriggers = (FuncTriggers & 0xFC) | 0x02;  // dec xx
      break;
    case 5:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x02;  // dec xx
      break;
    }
  }

void UpButTypeCal(void)
  {
  switch (FuncState) 
    {
    case 1:
      FuncState = 5;
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;  // inc xx
      break;
    case 4:
    case 9:
      FuncEscape = TRUE;
      break;
    case 5:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;  // inc xx
      break;
    }
  }

void WriteTypeCal_SAVE_ESC(char FuncState)
  {
  WriteAddressLN2(30);
  switch (FuncState)
    {
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      WriteCode2(SAVETxt);
    break;
    }

  //if ((FuncState== 5) || (FuncState == 8) || (FuncState == 9))
    //WriteCode2(SAVETxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();
  switch (FuncState)
    {
    case 1:  // ADJ, ADJchange
    case 5:
      WriteArrowsField(L_+D_+U_+R_);
      break;
    case 2:
    case 3:
    case 6:
    case 7:
      WriteArrowsField(L_+R_+E_);
      break;
    case 8:  // SAVE
      WriteItemArrows(30, Mstrlen(SAVETxt)); // highlight SAVE
      WriteArrowsField(L_+R_+E_);
      break;
    default:
      WriteItemArrows(36, Mstrlen(ESCTxt));  // highlight ESC
      WriteArrowsField(L_+U_+R_);
      break;
    }
  }
/**************  CAL END *******************/


/***************************************************************************/
/*  WriteType8_SAVE_ESC                              MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970624                          */
/* Revised:    980827                                       */
/*                                                  */
/*  Function:  Handle the SAVE and ESC text in the <USER TEXT SAVE ESC>    */
/*          display handling                              */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*          Cursor postion on line 2 is updated                  */
/*  Returns:    --                                        */
/* Updates:    VerticalRepeat                                */
/***************************************************************************/
void WriteType8_SAVE_ESC(char FuncState) {

  WriteAddressLN2(30);

  if ((FuncState >= 3) && (FuncState <= 5) || (FuncState == 7))
    WriteCode2(SAVETxt);

  ClearToPosLN2(36);
  WriteCode2(ESCTxt);

  ClearRestLN2();

  switch (FuncState) {
    case 1:
    case 3:
    case 6:
    case 7:
      WriteArrowsField(L_+D_+U_+R_+E_);

      //VerticalRepeat = TRUE;
      break;

    case 4:
      WriteItemArrows(30, Cstrlen(SAVETxt));
      WriteArrowsField(L_+R_+E_);

      //VerticalRepeat = FALSE;
      break;

    default:
      WriteItemArrows(36, Cstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);

      //VerticalRepeat = FALSE;
      break;
  }
}

/***************************************************************************/
/*  UpButType8                                    MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the UP-button in the <SAVE ESC> state machine      */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    Ptr, FuncState and FuncTriggers                    */
/***************************************************************************/
void UpButType8() {

  switch (FuncState) {
    case 1:
    case 3:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;  // inc xx
      break;

    case 2:
    case 5:
      FuncEscape = TRUE;
      break;

    case 6:
      FuncState++;
    case 7:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x04;  // inc yy
      break;
  }
}

/***************************************************************************/
/*  DownButType8                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the DOWN-button in the <XY EDIT SAVE ESC> state     */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState and FuncTriggers                        */
/***************************************************************************/
void DownButType8() {

  switch (FuncState) {
    case 1:
    case 3:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x02;  /* dec xx */
      break;

    case 6:
      FuncState++;
    case 7:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x08;  // dec yy
      break;
  }
}

/***************************************************************************/
/*  LeftButType8                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the LEFT-button in the <XY EDIT SAVE ESC> state     */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void LeftButType8() {
  switch (FuncState) {
    case 1:
      FuncState++;
      break;

    case 3:
      FuncState = 5;
      break;

    case 6:
      FuncState++;
    case 7:
      FuncTriggers = (FuncTriggers & 0x3F) | 0x40;  // right rr
      break;

    default:
      FuncState--;
      break;
  }
}

/***************************************************************************/
/*  RightButType8                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    970623                                       */
/*                                                  */
/*  Function:  Handle the RIGHT-button in the <XY EDIT SAVE ESC> state     */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void RightButType8() {
  switch (FuncState) {
    case 2:
      FuncState--;
      break;

    case 5:
      FuncState = 3;
      break;

    case 6:
    case 7:
      FuncTriggers = (FuncTriggers & 0x3F) | 0x80;  // left rr
      break;

    default:
      FuncState++;
      break;
  }
}

/***************************************************************************/
/*  ExecButType8                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970623                          */
/* Revised:    971215                                       */
/*                                                  */
/*  Function:  Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state  */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncSave                                    */
/***************************************************************************/
void ExecButType8() {
  switch (FuncState){
    case 1:
      FuncState = 6;
      break;

    case 3:
      FuncState = 7;
      break;

    case 4:
      FuncSave = TRUE;
      break;

    case 6:
      FuncState = 1;
      break;

    case 7:
      FuncState = 3;
      break;
  }
}

/***************************************************************************/
/*  UpButType12                                    MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980302                          */
/* Revised:    980302                                       */
/*                                                  */
/*  Function:  Handle the UP-button in the <SAVE ESC> state machine      */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    Ptr, FuncState and FuncTriggers                    */
/***************************************************************************/
void UpButType12(void) 
  {
  switch (FuncState) 
    {
    case 1:
      FuncState = 4;
    case 4:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x01;  // inc xx
      break;
    case 2:
      FuncState = 5;
    case 5:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x04;  // inc yy
      break;
    case 8:
      FuncState = 9;
    case 9:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x10;  // inc zz
      break;
    case 3:
    case 7:
      FuncEscape = TRUE;
      break;
    }
  }

/***************************************************************************/
/*  DownButType12                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980302                          */
/* Revised:    980302                                       */
/*                                                  */
/*  Function:  Handle the DOWN-button in the <XY EDIT SAVE ESC> state     */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState and FuncTriggers                        */
/***************************************************************************/
void DownButType12(void) 
  {

  switch (FuncState) 
    {
    case 1:
      FuncState = 4;
    case 4:
      FuncTriggers = (FuncTriggers & 0xFC) | 0x02;  // dec xx
      break;
    case 2:
      FuncState = 5;
    case 5:
      FuncTriggers = (FuncTriggers & 0xF3) | 0x08;  // dec yy
      break;
    case 8:
      FuncState = 9;
    case 9:
      FuncTriggers = (FuncTriggers & 0xCF) | 0x20;  // dec zz
      break;
    }
  }

/***************************************************************************/
/*  LeftButType12                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980302                          */
/* Revised:    980302                                       */
/*                                                  */
/*  Function:  Handle the LEFT-button in the <XY EDIT SAVE ESC> state     */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void LeftButType12(void) 
  {

  switch (FuncState) 
    {
    case 1:
      FuncState = 3;
      break;
    case 4:
      FuncState = 7;
      break;
    case 8:
      FuncState = 9;
    case 9:
      FuncTriggers = (FuncTriggers & 0x3F) | 0x40;  // left rr
      break;
    default:
      FuncState--;
      break;
    }
  }

/***************************************************************************/
/*  RightButType12                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980302                          */
/* Revised:    980302                                       */
/*                                                  */
/*  Function:  Handle the RIGHT-button in the <XY EDIT SAVE ESC> state     */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncState                                  */
/***************************************************************************/
void RightButType12(void) 
{

  switch (FuncState) {
    case 3:
      FuncState = 1;
      break;

    case 7:
      FuncState = 4;
      break;

    case 8:
      FuncState = 9;
    case 9:
      FuncTriggers = (FuncTriggers & 0x3F) | 0x80;  // right rr
      break;

    default:
      FuncState++;
      break;
  }
}

/***************************************************************************/
/*  ExecButType12                                  MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980302                          */
/* Revised:    980302                                       */
/*                                                  */
/*  Function:  Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state  */
/*          machine                                    */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*  Returns:    --                                        */
/* Updates:    FuncSave                                    */
/***************************************************************************/
void ExecButType12(void) 
  {

  switch (FuncState){
    case 1:
      FuncState = 8;
      break;

    case 4:
      FuncState = 9;
      break;

    case 6:
      FuncState = 1;
      FuncSave = TRUE;
      break;

    case 8:
      FuncState = 1;
      break;

    case 9:
      FuncState = 4;
      break;
  }
}


/***************************************************************************/
/*  WriteType12_SAVE_ESC                              MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980302                          */
/* Revised:    980827                                       */
/*                                                  */
/*  Function:  Handle the SAVE and ESC text in the <USER TEXT SAVE ESC>    */
/*          display handling                              */
/*  Remarks:    Part of <XY EDIT SAVE ESC> display handling            */
/*          Cursor postion on line 2 is updated                  */
/*  Returns:    --                                        */
/* Updates:    VerticalRepeat                                */
/***************************************************************************/
void WriteType12_SAVE_ESC(char FuncState) 
  {
  WriteAddressLN2(30);
  if ((FuncState >= 4) && (FuncState <= 7) || (FuncState == 9))
    WriteCode2(SAVETxt);
  ClearToPosLN2(36);
  WriteCode2(ESCTxt);
  ClearRestLN2();
  switch (FuncState)
    {
    case 1:
    case 4:
    case 8:
    case 9:
      WriteArrowsField(L_+D_+U_+R_+E_);
      //VerticalRepeat = TRUE;
      break;
    case 6:
      WriteItemArrows(30, Mstrlen(SAVETxt));
      WriteArrowsField(L_+R_+E_);
      //VerticalRepeat = FALSE;
      break;
    case 3:
    case 7:
      WriteItemArrows(36, Mstrlen(ESCTxt));
      WriteArrowsField(L_+U_+R_);
      //VerticalRepeat = FALSE;
      break;
    default:
      WriteArrowsField(L_+D_+U_+R_);
      //VerticalRepeat = FALSE;
      break;
    }
  }



/***************************************************************************/
/*  WriteItemArrows                                 MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 940401                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Hightlight selected item , ie. write arrows, (<txt>),      */
/*          around the item.                              */
/*  Remarks:    Cursor position on line 2 is updated                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void WriteItemArrows(UC start, UC length)
  {
  WriteCharLN2(start - 1, '<');
  WriteCharLN2(start + length, '>');
  }

/***************************************************************************/
/*  WriteArrowsField                                 MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 940401                          */
/* Revised:    980826                                       */
/*                                                  */
/*  Function:  Indicates the keyboard-selection, (ie. keys that can be    */
/*          used), the upper right corner of the display, (ie. line 1  */
/*          pos 36-39). Also displays a possible padlock            */
/*  Remarks:    Special characters, which are not included in the standard  */
/*          character set, are written to the eight user definable     */
/*          characters in the display-controller                */
/*          Cursor position on line 1 is updated                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void WriteArrowsField(UC No)
  {
  char tmpChar;

  // Array Definition:
    // Error =inverse '!'   0
    // Arrow LEFT         1
    // Arrow UP/DOWN       2
    // Arrow RIGHT       3
    // Execute icon       4
    // Lock icon         5
    // Arrow UP         6
    // Arrow DOWN         7
    // Not Equal icon     8

  // Character defintion as respect to addresses
    // Adr 0: Arrow LEFT
    // Adr 1: Arrow UP/DOWN or arrow UP or arrow DOWN
    // Adr 2: Arrow RIGHT
    // Adr 3: Execute icon
    // Adr 4: Lock icon
    // Adr 5: Not Equal icon
    // Adr 6: Used in local routines
    //       ConfigLCDContrastSelect Bar10/10 Adr. 26
    // Adr 7: Used in local routines

  if (No & L_) 
    {
    WriteCGChar(0, 0x01);        // LEFT
    tmpChar = 0;
    }
  else
    tmpChar = ' ';
  WriteCharLN1(36, tmpChar);
  switch (No & 0x0A) 
    {
    case D_:
      WriteCGChar(1, 0x07);
      tmpChar = 1;            // DOWN
      break;
    case U_:
      WriteCGChar(1, 0x06);
      tmpChar = 1;            // UP
      break;
    case D_+U_:
      WriteCGChar(1, 0x02);
      tmpChar = 1;            // UP/DOWN
      break;
    default:
      tmpChar = ' ';
      break;
    }
  WriteCharLN1(37, tmpChar);
  if (No & R_) 
    {
    WriteCGChar(2, 0x03);        // RIGHT
    tmpChar = 2;
    }
  else
    tmpChar = ' ';
  WriteCharLN1(38, tmpChar);
  if (No & P_) 
    {
    WriteCGChar(4, 0x05);        // Lock Icon
    tmpChar = 4;
    }
  if (No & E_) 
    {
    WriteCGChar(3, 0x04);        // Execute Icon
    tmpChar = 3;
    }
  else
    tmpChar = ' ';
  WriteCharLN1(39, tmpChar);
  if (No & MORE_)
    WriteCodeLN2(37, ThreeDotsTxt);
  }


/***************************************************************************/
/*  WriteItemBrackets                                 MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 940401                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Hightlight selected item , ie. write brackets, ([txt]),    */
/*          around the item.                              */
/*  Remarks:    Cursor position on line 2 is updated                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void WriteItemBrackets(UC start, UC length)
  {
  WriteCharLN2(start - 1, '[');
  WriteCharLN2(start + length, ']');
  }

#if 1
/***************************************************************************/
/*  Mstrcat                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                      */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Concatenate a string from code memory to data memory      */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
char* Mstrcat(char* to, code char* from) 
  {
  register char* cp;

  cp = to;
  while(*cp)
    cp++;
  while(*cp++ = *from++)
    continue;
  return(to);
  }
/***************************************************************************/
/*  Mstrcpy                                                                */
/*                                                                         */
/* Author:     JK                                                          */
/* Revised:    18.04.02                                                    */
/*                                                                         */
/*  Function:  Copy a string from code memory to data memory               */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
char* Mstrcpy(char* to, code char* from) 
  {
  register char* tp;

  tp = to;
  while(*tp++ = *from++)
    continue;
  return(to);
  }

size_t Mstrlen(code char* s) 
  {
  register code char * cp;

  cp = s;
  while(*cp++)
    continue;
  return(cp - s - 1);
  }

#endif


/***************************************************************************/
/* WriteTimingFLT                                                TIMING.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void WriteTimingFLT(char sign, int field, int line, float time)
  {
  char TextBuffer[20];
  
  WriteCodeLN2(0, TimeVHdrTxt);         // Write V: (ie. field) header
  sprintf(TextBuffer, "%2d", (int)field);    // Copy field number to buffer
  TextBuffer[0] = sign;                 // Add sign to field number
  WriteTxt2(TextBuffer);                // Write field number

  WriteCode2(TimeHHdrTxt);              // Write H: (ie. line) header
  sprintf(TextBuffer, "%04d", (int)line);    // Copy line number to buffer
  TextBuffer[0] = sign;                 // Add sign to line no
  WriteTxt2(TextBuffer);                // Write line number

  WriteCode2(TimeTHdrTxt);              // Write T: (ie. ns) header
                                        // Copy samples (in ns) to buffer
  sprintf(TextBuffer, "%08.1f", time);
  TextBuffer[0] = sign;                 // Add sign to time value
  spaces2zeros(TextBuffer);             // sprintf does not pad zeroes!!!!!!
  WriteTxt2(TextBuffer);                // Write time value
  ClearRestLN2();                       // Clear rest of line 2
  }



void spaces2zeros(char * buff)
  {

  while(*buff)
    {
    if (*buff == ' ')
      *buff = '0';
    buff++;
    }
  }

/***************************************************************************/
/* menu_init                                                   MENUTREE.C  */
/*                                                                         */
/* Author:     JK, 11.02.2003                                              */
/* Revised:    12.02.2003                                                  */
/*                                                                         */
/*  Function:  Display status.  This function is called after auto-escape  */

/*  Remarks:   The indexes are not altered, i.e. when going back to menu   */
/*             we end in the place we were before auto-escape timeout.     */
/*  Returns:   Nothing                                                     */
/*  Updates:   Ptr                                                         */
/***************************************************************************/
/* in menu_init() the state of variables must reflect settings of 5201 */
/* function init_configs() in mnuinter.c must do that                  */
void menu_init(void)
  {
  UC i;

  keyb_init();

  StatusNdx                  = 0;
  BaseNdx                    = 0;
  SdiNdx                     = 0;
  AnlBlkNdx                  = 0;
  GenlockNdx                 = 0;
  AudioTypeNdx               = 0;
  PresetNdx                  = 0;
  for (i = 0; i < 3; i++)
    BBNdx[i] = 0;
  samples                    = 0;
  status_is_displayed        = 1;
  rs232_touched              = 0;

  init_configs(&AudioTypeNdx);    // AudioTypeNdx must be updated to
                                  // reflect actual setting in 5201
  Ptr = &status_menu[StatusNdx];  // Prepare pointer for top menu display
  Ptr->DispFunct();               // Display top menu
  }
 

/***************************************************************************/
/*  void check_keys(void)                                                  */
/*  Function:  Check if keyboard key pressed and act accordingly           */
/*  Remarks:   Place this function in main loop of your program to         */
/*             implement complete 5202 menu system                         */
/*  Returns:   Nothing                                                     */
/*  Updates:   Nothing                                                     */
/***************************************************************************/
void check_keys(void)
  {
  static UC c;

  if (ButtonReady(&c))
    {
    switch (c) 
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
// now shortcut buttons....
      case GENLOCKButton:   //0xBF    // S11
        CharBlinkLN2(0, OFF);
        BaseNdx = GenlockItem;
        Ptr = &genlock_menu;
        status_is_displayed = 0;
        break;
      case BBButton:        //0xFE    // S2
        CharBlinkLN2(0, OFF);
        BaseNdx = AnlBlkItem;
        Ptr = &anlblk_menu;
        status_is_displayed = 0;
        break;
      case SDIButton:       //0xFD    // S4
        CharBlinkLN2(0, OFF);
        BaseNdx = SdiItem;
        Ptr = &sdi_menu;
        status_is_displayed = 0;
        break;
      case AUDIOButton:     //0xFB    // S6
        CharBlinkLN2(0, OFF);
        BaseNdx = AUDIOItem;
        Ptr = &audio_menu[AudioTypeNdx];
        status_is_displayed = 0;
        break;
      case PRESETButton:    //0xF7    // S8
        CharBlinkLN2(0, OFF);
        BaseNdx = PRESETItem;
        Ptr = &preset_menu[AudioTypeNdx];
        status_is_displayed = 0;
        break;
      }
    Ptr->DispFunct();
    }
  }

void empty(void)
  {
  return;
  }  

/***************************************************************************/
/*  GetNextPresetASCII                                 UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    980505                                       */
/*                                                  */
/*  Function:  Find the next valid character to name a preset with.      */
/*  Remarks:    --                                        */
/*  Returns:    Next/previous legal character.                    */
/*  Updates:    --                                        */
/***************************************************************************/
char GetNextPresetASCII(UC direction, UC ASCIIChar)
  {
  if (direction )
    {            // Down: 0 otherwise UP
    do 
      {
      if (ASCIIChar++ >= 127)
        ASCIIChar = 0;
      } while (!PresetCharSet[ASCIIChar]);
    }
  else 
    {
    do 
      {
      if (ASCIIChar-- <= 0)
        ASCIIChar = 127;
      } while (!PresetCharSet[ASCIIChar]);
    }
  return(ASCIIChar);
  }


/***************************************************************************/
/* ret2status                                                  MENUTREE.C  */
/*                                                                         */
/* Author:     JK, 11.02.2003                                              */
/* Revised:    12.02.2003                                                  */
/*                                                                         */
/*  Function:  Display status.  This function is called after auto-escape  */
/*             timeout has occured.  From main loop in instru.c            */
/*             1-st status display is entered, the one showing Genlock     */
/*             status  (not the last status display).                      */
/*  Remarks:   The indexes are not altered, i.e. when going back to menu   */
/*             we end in the place we were before auto-escape timeout.     */
/*  Returns:   Nothing                                                     */
/*  Updates:   Ptr                                                         */
/***************************************************************************/
void ret2status(void)
  {
  Ptr = &status_menu[0];  // Restore Display-Status pointer, point to 0-th element.
  status_is_displayed = 1;
  CharBlinkLN2(0, OFF);
  Ptr->DispFunct();    // Display  status-display
  }

/***************************************************************************/
/* update_status                                               MENUTREE.C  */
/*                                                                         */
/* Author:     JK, 11.02.2003                                              */
/* Revised:    12.02.2003                                                  */
/*                                                                         */
/*  Function:  Refresh status display.  This function is called from main  */
/*             loop in instru.c.  If status is being displayed, the display*/
/*             is refreshed to reflect settings which might be changed by  */
/*             remote interface.                                           */
/*  Remarks:   Display is refreshed only if status is displayed.  Variable */
/*             status_is_displayed is used for that.                       */
/*  Returns:   Nothing                                                     */
/*  Updates:   Ptr                                                         */
/***************************************************************************/
void update_status(void)
  {
  if (status_is_displayed)
    Ptr->DispFunct();    // Display  status-display
  }



/***************************************************************************/
/* copy_presets                                                MENUTREE.C  */
/*                                                                         */
/* Author:    JK, 17.02.2003                                               */
/* Revised:   17.02.2003                                                   */
/*                                                                         */
/*  Function:  To copy existing preset names into preset name buffers      */
/*             so they can be used for editing using menu system.          */
/*  Remarks:                                                               */
/*  Returns:   Nothing                                                     */
/*  Updates:   None                                                        */
/*  Note:      We copy CBSetup to CBOrig on entry here.  Normally, these   */
/***************************************************************************/

void  copy_presets(void)
  {
  UC i;
  PresetObject prst;

  for (i = 0; i < 4; i++)
    {
    NV_Recall(&prst, PresetPtr[i], sizeof(PresetObject));
    strcpy(OrgPresetNames[i], &prst.Name);
    strcpy(PresetNames[i], &prst.Name);
    }
  }

#if 0
/* This funcion copies one config structure/union into another */
void MMemcpy(void *dest, void *source, UC hmany)
  {
  UC i;
  UC * de;
  UC * so;
  
  de = (UC *) dest;
  so = (UC *) source;

  for (i = 0; i < hmany; i++)
    *(de + i) = *(so + i);
  }
#endif

#if 0   
  sprintf(jkbuffer, "clock=%d\n", UcharVal);
  rs232puts(jkbuffer);
#endif

