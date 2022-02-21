/****************************************************************************/
/* MODULE:                                                                  */
/*  cbar.c - ColorBar generator                                             */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  int WriteCBGain( UC gain)                                               */
/*  int WriteCBChromaGain( UC gain)                                          */
/*  int SRAMVideoLoad( UC signal, UC bank)                                  */
/*  int SRAMSoundLoad( UC signal)                                           */
/*  void CBLineInterrupt( void)                                             */
/*  void CBDefaults( void)                                                  */
/*  int CBInit( void)                                                        */
/*  int CBUpdate( void)                                                      */
/*  int SetCBPattern( UI pattern)                                            */
/*  int SetCBSystem( UC system)                                              */
/*  int SetCBDelay( UL delay)                                                */
/*  int SetCBScHPhase( int schphase)                                        */
/*  int SetCBEmbAudioSignal( UC signal)                                     */
/*  int ValidateCBPattern( UC system, UI *pattern)                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES: CB Tables defined at absolute address of 0x2000: 14.03.02         */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#define MOVE_TEXT

#include "reg8051.h"
#include "define.h"        // Standard PTV defines
#include "rtxcobj.h"
#include "config.h"
#include "audio.h"
#include "cbar.h"
#include "tables.h"
#include "led_hw.h"
#include "tvp6kdrv.h"
#include "i2c_drv.h"
#include "instru.h"

#define SRAMBank0      0x00
#define SRAMBank1      0x10
#define SRAMBank2      0x14
#define SRAMBank3      0x18
#define SRAMBank4      0x1C
#define SRAMBank5      0x20
#define SRAMBank6      0x24
#define SRAMBank7      0x28
#define SRAMBank8      0x2C
#define SRAMBank9      0x30
#define SRAMBank10    0x34
#define SRAMBank11    0x38
#define SRAMBank12    0x3C

#define dEBUCBar_8bit_625           0
#define dEBUCBar_625              1
#define dSMPTECBar                2
#define dBlueBars                  3
#define dIWhiteQ                  4
#define dCBar100_625              5
#define dGrey75_625                6
#define dRed75_625                7
#define dRed75_525                8
#define dChroma_Multiburst        9
#define d_CCIR18_625              9
#define dFCC_Multiburst_525        10
#define dChroma_Multiburst_525    11
#define dWhiteLine_625            11
#define dCrossHatch19_625          12
#define dWhiteLine_525            13
#define dCrossHatch17_525          14
#define dPlugeWhite_625            15
#define dPlugeGreyC_625            16
#define dPlugeGreyD_625            17
#define dPlugeGreyE_625            18
#define dPlugeWhite_525            19
#define dPlugeGreyC_525            20
#define dPlugeGreyD_525            21
#define dPlugeGreyE_525            22
#define dGreyScale5_625            23
#define dGreyScale5_525            24
#define dGreyScale10_625          25
#define dGreyScale10_525          26
#define dWindow50Marker            27
#define dMarkerLine              28
#define dWindow15                29
#define dMarker15                30
#define dWindow20                31
#define dMarker20                32
#define dWindow100              33
#define dMarker100              34
#define dDigitalGrey_625          35
#define dDigitalGrey_525          36
#define dSDI_test_line1            37
#define dSDI_test_line2            38
#define dSDI_test_line3            39
#define dCBar75_STD_625             40
#define dFCC_CBar_525              41
#define dFCC_CBar100_525          42
#define dWindow10                  43
#define dMarker10                  44
#define dkHz15BlWh_625            45
#define dkHz15BlWh_525            46
#define dCCIR18_625                47

/****************************************************************************/
/*
The statusport is defined in the PLD as:

  bit 0:  S0        Background system
  bit 1:  S1

  bit 2:  S2        Linetype
  bit 3:  S3
  bit 4:  S4
  bit 5:  S5

  bit 6:  SYSSEL    System select ( 0:G, 1:M)
  bit 7:  SOUND      Sound ( 0:off, 1:on)

  bit 8:  3OR4      Samples in sound ( 0:3 samples, 1:4 samples)
  bit 9:  FIELD1    Field one bit ( 1 in Field 1/line 1 (out of 8/4) otherwise 0)
  bit 10: CB  
  bit 11: EDHON      EDH bit for future use ( 0:off, 1:on)

  bit 12: PS        PS ( 0:disable writing in SRAM, 1: enable writing in SRAM)
  bit 13: Line7      Line 7 pulse in PAL w/PAL_ID
  bit 14: PhaseReg  Phase offset register select
  bit 15: -          Not used
*/
/****************************************************************************/
                    
#define S1S0        0x0003
#define S5S4S3S2    0x003C
#define SYSSEL      0x0040
#define SOUNDON      0x0080
#define S3OR4        0x0100
#define FIELD1      0x0200
#define CB          0x0400
#define EDHON        0x0800
#define PS          0x1000
#define LINE7PULSE  0x2000
#define PHASEREGSEL  0x4000

#define SOUND        (int)(SOUNDON + S3OR4)

#define SyncLineA    1
#define SyncLineB   0
#define SyncLineC   2
#define SyncLineD   3

/****************************************************************************/
// Prototypes internal functions

int ValidateCBPattern( UC, UI *);
int Update_StatusPort(UI);

/* The tables used for control of test patterns are in ctable.c.
   These tables are defined at absolute addresses, starting at
   0x2000.  It was found that address line A15 is sometimes
   asserted incorrectly, probably due to RTXC.  If the tables 
   lay at such position that A15 is involved in reading them,
   a reading failure occured and pictures were displayed incorrectly.
   By moving the tables to a lower address where A15 is not needed 
   under read operation, this problem no longer exists.
   14.03.02
*/   
#include "ctable.c"

                                              // Signal array defined in ROM
                                              //  (size/start is specified in BYTES
                                              //  in config.h)
_ROM_ int VideoSignalArray[SizeOfCBVideoCode/2] _at( StartOfCBVideoCode);
_ROM_ int SoundSignalArray[SizeOfEmbAudioCode/2] _at( StartOfEmbAudioCode);

volatile int SRAMVideoPort _at( 0x44000);       // Port defined in PLD
volatile int SRAMSoundPort _at( 0x40000);       // Port defined in PLD
//volatile int StatusPort _at( 0x6a000);         // Port defined in PLD
volatile int PLD_StatusPort _at( 0x6a000);         // Port defined in PLD
volatile int HOffsetPort _at( 0x6a004);       // Port defined in PLD
volatile int ChromaGainPort _at( 0x6a008);    // Port defined in PLD

/***/
volatile UI LEDPort _at( 0x6a00C);        // Port defined in PLD
volatile UI TestLEDPort _at( 0x6a014);  // Port defined in PLD


volatile _near UI TimeTickCounter;
volatile _near int LineOffset;

volatile int PhaseOffset;

CBObject CBSetup;

#ifdef MOVE_TEXT

enum { TEXTLINES_HIGHT = 14,	// Total number of lines for Text BAR
       TEXTREPEAT_HIGH = 1    // Number of times the text lines shut be repeated
     };

  static _ROM_  int *MainSignalPtr;

  static UC DispStr [] = "DK-Audio A/S : PT502";


  static UC Font57[] =
{
//      <sp>
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//      !
        0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x04,
//      "
        0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00,
//      #
        0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A,
//      $
        0x04, 0x0F, 0x14, 0x0E, 0x05, 0x1E, 0x04,
//      %
        0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03,
//      &
        0x0C, 0x12, 0x14, 0x08, 0x15, 0x12, 0x0D,
//      '
        0x0C, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,
//      (
        0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02,
//      )
        0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08,
//      *
        0x00, 0x04, 0x15, 0x0E, 0x15, 0x04, 0x00,
//      +
        0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00,
//      ,
        0x00, 0x00, 0x00, 0x00, 0x0C, 0x04, 0x08,
//      -
        0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
//      .
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C,
//      /
        0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00,
//      0
        0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E,
//      1
        0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E,
//      2
        0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F,
//      3
        0x1F, 0x02, 0x04, 0x02, 0x01, 0x01, 0x0E,
//      4
        0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02,
//      5
        0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E,
//      6
        0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x1E,
//      7
        0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08,
//      8
        0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E,
//      9
        0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C,
//      :
        0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00,
//      ;
        0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x04, 0x08,
//      <
        0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02,
//      =
        0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00,
//      >
        0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08,
//      ?
        0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04,


//      @
        0x0E, 0x11, 0x01, 0x0D, 0x15, 0x15, 0x0E,
//      A
        0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11,
//      B
        0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E,
//      C
        0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E,
//      D
        0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C,
//      E
        0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F,
//      F
        0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10,
//      G
        0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F,
//      H
        0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11,
//      I
        0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,
//      J
        0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C,
//      K
        0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11,
//      L
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F,
//      M
        0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11,
//      N
        0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11,
//      O
        0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,
//      P
        0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10,
//      Q
        0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D,
//      R
        0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11,
//      S
        0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E,
//      T
        0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
//      U
        0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E,
//      V
        0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04,
//      W
        0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A,
//      X
        0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11,
//      Y
        0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04,
//      Z
        0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F,
//      [
        0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0E,
//      yen
        0x11, 0x0A, 0x1F, 0x04, 0x1F, 0x04, 0x04,
//      ]
        0x0E, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0E,
//      ^
        0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00,
//      _
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,


//      `
        0x08, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00,
//      a
        0x00, 0x00, 0x0E, 0x01, 0x0F, 0x11, 0x0F,
//      b
        0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x1E,
//      c
        0x00, 0x00, 0x0E, 0x10, 0x10, 0x11, 0x0E,
//      d
        0x01, 0x01, 0x0D, 0x13, 0x11, 0x11, 0x0F,
//      e
        0x00, 0x00, 0x0E, 0x11, 0x1F, 0x10, 0x0E,
//      f
        0x06, 0x09, 0x08, 0x1C, 0x08, 0x08, 0x08,
//      g
        0x00, 0x0F, 0x11, 0x11, 0x0F, 0x01, 0x0E,
//      h
        0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x11,
//      i
        0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E,
//      j
        0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0C,
//      k
        0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12,
//      l
        0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E,
//      m
        0x00, 0x00, 0x1A, 0x15, 0x15, 0x11, 0x11,
//      n
        0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11,
//      o
        0x00, 0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E,
//      p
        0x00, 0x00, 0x1E, 0x11, 0x1E, 0x10, 0x10,
//      q
        0x00, 0x00, 0x1D, 0x13, 0x1E, 0x01, 0x01,
//      r
        0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10,
//      s
        0x00, 0x00, 0x0E, 0x10, 0x0E, 0x01, 0x1E,
//      t
        0x08, 0x08, 0x1C, 0x08, 0x08, 0x09, 0x06,
//      u
        0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0D,
//      v
        0x00, 0x00, 0x11, 0x11, 0x11, 0x0A, 0x04,
//      w
        0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0A,
//      x
        0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11,
//      y
        0x00, 0x00, 0x11, 0x11, 0x0F, 0x01, 0x0E,
//      z
        0x00, 0x00, 0x1F, 0x02, 0x04, 0x08, 0x1F,
//      {
        0x02, 0x04, 0x04, 0x08, 0x04, 0x04, 0x02,
//      |
        0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
//      }
        0x08, 0x04, 0x04, 0x02, 0x04, 0x04, 0x08,
//      ==>
        0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00,
//      <==
        0x00, 0x04, 0x08, 0x1F, 0x08, 0x04, 0x00

};


//Emulates the 8 lines avalible for text.
//Would be implemented ase a seperat table.

#define TA17 (TA16 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineC[] _at(TA17) =		//Interlaced
  	{
	1*256	+	SRAMBank5 + SyncLineC,		//Line 0
	1*256	+	SRAMBank6 + SyncLineC,
	1*256	+	SRAMBank7 + SyncLineC,
	1*256	+	SRAMBank8 + SyncLineC,
	1*256	+	SRAMBank9 + SyncLineC,
	1*256	+	SRAMBank10 + SyncLineC,
	1*256	+	SRAMBank11 + SyncLineC,
	1*256	+	SRAMBank12 + SyncLineC,
	};


#define TA18 (TA17 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineD[] _at(TA18) =		//De-Interlaced 
	{
	1*256	+	SRAMBank5 + SyncLineD,		//Line 0
	1*256	+	SRAMBank6 + SyncLineD,
	1*256	+	SRAMBank7 + SyncLineD,
	1*256	+	SRAMBank8 + SyncLineD,
	1*256	+	SRAMBank9 + SyncLineD,
	1*256	+	SRAMBank10 + SyncLineD,
	1*256	+	SRAMBank11 + SyncLineD,
	1*256	+	SRAMBank12 + SyncLineD,
	};

   
#endif   // MOVE_TEXT


static _near UI Status _at( 0x120);
#ifdef MOVE_TEXT
   static UI Normal_Status;
#endif   // MOVE_TEXT

static _ROM_  UI * _near TablePtr _at( 0x122);
static _ROM_  UI *newTablePtr;

static _near UI LineCnt _at( 0x126);
static _near UC FrameCnt _at( 0x128);

static _near UC SyncLineCnt _at( 0x129);
static _near UC SyncLinePtr _at( 0x12A);
                                 
static _near UC SoundFrameCnt _at( 0x12B);
static _near UC SoundCnt _at( 0x12C);
static _near UC SoundPtr _at( 0x12D);
static _near UC SoundPtrMax _at( 0x12E);

static char Blanking, NewPattern, SoundEnable;
extern       _bit JKflag;
_sfrbit    JKpin   _atbit(P1,5);    // P1^5
_sfrbit    SDApin  _atbit(P1,6);    // P1^6

extern UI LEDMirror;
extern UI TestLEDMirror;
//UI temp_status;			//KS. To force compiler to use a R-register when writing out 
				//to the Status port.

/**************************************************************************/
/* WriteCBGain                                                       CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000504                                    */
/* Revised:   000606, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int WriteCBGain( UC gain)
{
  return( Write_AD8402( CBPotMeter, 0, gain));
}

/**************************************************************************/
/* WriteCBChromaGain                                                 CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000504                                    */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   The resistor at V118 has been swapped ie. bit 0 is bit 3 etc.*/
/*           The conversion is done in SW instead of correcting the parts  */
/*           list.                                                        */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int WriteCBChromaGain( UC gain)
{
  UC tmp = 0;

  // gain bit 1 = bit4
  // gain bit 2 = bit3
  // gain bit 3 = bit2
  // gain bit 4 = bit1

  if ( gain & 0x01)
    tmp += 0x08;

  if ( gain & 0x02)
    tmp += 0x04;

  if ( gain & 0x04)
    tmp += 0x02;

  if ( gain & 0x08)
    tmp += 0x01;

  ChromaGainPort = tmp;

  return( OK);
}



/**************************************************************************/
/* SRAMVideoLoad                                                   CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000324                                    */
/* Revised:   000603, KEn, DEV                                            */
/*                                                                        */
/* Function: Copy video data to video RAM                                 */
/* Remarks:   S5,S4,S3 and S2 should be kept at ZERO while loading AND the */
/*           interrupt system should NOT clear the PS bit.                */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int SRAMVideoLoad( UC signal, UC bank)
{
  volatile int *VideoDstPtr;
  _ROM_  int *VideoSrcPtr;
  int i;


  Status |= PS;                    // Set PS bit while loading
  //PLD_StatusPort = Status;
 #ifdef MOVE_TEXT
  Normal_Status = Status;
 #endif
  Update_StatusPort(Status);

  VideoDstPtr = &SRAMVideoPort + (( bank/4)-4) * 0x0800;
  VideoSrcPtr = &VideoSignalArray[ 0x800 * (long)signal];
#ifdef MOVE_TEXT
   if (bank == SRAMBank1)
     MainSignalPtr = VideoSrcPtr;
#endif

  for ( i = 0; i < 2048; i++)
    *VideoDstPtr++ = *VideoSrcPtr++;

  Status &= ~PS;                  // Clear PS bit again
 #ifdef MOVE_TEXT
  Normal_Status = Status;
 #endif

  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  return( OK);
}

#ifdef MOVE_TEXT
/**************************************************************************/
/* SRAMVideoTextLoad                                               CBAR.C */
/*                                                                        */
/* Author:   Poul-Erik Hansen                                             */
/* Revised:   ??                                                          */
/*                                                                        */
/* Function: Copy textlines video data to video RAM                       */
/* Remarks:   S5,S4,S3 and S2 should be kept at ZERO while loading AND the */
/*           interrupt system should NOT clear the PS bit.                */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/

//  static UC DispStr [] = "DK-Audio A/S : PT502";
//  static UC Font57[] =

int SRAMVideoTextLoad(void)
{
  volatile int *VideoDstPtr;
  _ROM_  int *VideoSrcPtr;
  int i,	// Sample index
      n, // hos font index (0..5)
      m, // Repeat sample
      l; // Line number (0..6)
  UC *Str;
  static const UC TextBanks[] = { SRAMBank5,  SRAMBank6,  SRAMBank7, SRAMBank8,
  											 SRAMBank9,  SRAMBank10, SRAMBank11, SRAMBank12 };

  Status |= PS;                    // Set PS bit while loading
  Normal_Status = Status;

  for (l = 0; l < 7; l++)
    {
    VideoDstPtr = &SRAMVideoPort + (( TextBanks[l] /4)-4) * 0x0800;

    VideoSrcPtr = MainSignalPtr;

    for ( i = 0; i < 48; i++)		// 24 Y sample, 6 U sample & 6 V sample
      {
      *VideoDstPtr++ = *VideoSrcPtr++;
      }
    Str = DispStr;

    while(*Str)
      {
      UC StrValue = *Str++;
      UC Font     = Font57[ l + (7 * (StrValue - ' '))];

      for (n = 0; n < 6; n++)
         {
         int Yvalue = (0x20 & (Font << n)) ? 895 : 96;
         for (m = 0; m < 3; m++)
            {
            	*VideoDstPtr++ = Yvalue;
               VideoSrcPtr++;
            	*VideoDstPtr++ = 512;	// U / V = 0;
               VideoSrcPtr++;
               i += 2;
            }
         }
      }

    for ( ; i < 2048; i++)	
      {
      *VideoDstPtr++ = *VideoSrcPtr++;
      }

    }


  Status &= ~PS;                  // Clear PS bit again
  Normal_Status = Status;
  Update_StatusPort(Status);
  return( OK);
}

#ifdef TEST1
  for (b = 0; b < 7; b++)
    {
    int Load_value = (b & 1) ? 64 : 895 ;
    VideoDstPtr = &SRAMVideoPort + (( TextBanks[b] /4)-4) * 0x0800;
    //VideoSrcPtr = &VideoSignalArray[ 0x800 * (long)dEBUCBar_625];
    VideoSrcPtr = MainSignalPtr;

    for ( i = 0; i < 24; i++)
      {
      *VideoDstPtr++ = *VideoSrcPtr++;
      }

    // 16*4 .. 235*4
    for ( ; i < 1024; i += 2)
      {
      // 939 / 512  ==> white 100%
      // 895 / 512  ==> gray   95%
      // 750 / 512  ==> gray  78%
      // 128 / 512  ==> gray  8%
      //  96 / 512  ==> gray  4%
      //  64 / 512  ==> black 0%

      *VideoDstPtr++ =  Load_value;
      VideoSrcPtr++;
      *VideoDstPtr++ = 512;	// U & V = 0
      VideoSrcPtr++;
      }
/*
    for ( ; i < 1280; i += 2)
      {
      *VideoDstPtr++ = Load_value;
      *VideoDstPtr++ = 250;
      VideoSrcPtr++;
      VideoSrcPtr++;
      }
*/
    for (; i < 2048; i++)
      {
      *VideoDstPtr++ = *VideoSrcPtr++;
      }
    }
#endif


#endif

/**************************************************************************/
/* SRAMSoundLoad                                    CBAR.C */
/*                                                  */
/* Author:   Kim Engedahl, DEV, 000410                          */
/* Revised:   000603, KEn, DEV                                 */
/*                                                  */
/* Function: Copy sound data to sound RAM                        */
/* Remarks:                                             */
/* Returns:                                            */
/* Updates:                                            */
/**************************************************************************/
int SRAMSoundLoad( UC signal)
{
  volatile int *SoundDstPtr;
  _ROM_  int *SoundSrcPtr;
  int i;

  signal = ( signal == EmbAudioStereo1kHz);

  Status |= PS;                  // Set PS bit while loading
 #ifdef MOVE_TEXT
  Normal_Status = Status;
 #endif
  
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);
                                             
  SoundDstPtr = &SRAMSoundPort;
  SoundSrcPtr = &SoundSignalArray[2304*signal];

  for ( i = 0; i < 2304; i++)
    *SoundDstPtr++ = *SoundSrcPtr++;

  Status &= ~PS;                  // Clear PS bit again
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  return( OK);
}


/**************************************************************************/
/* CBLineInterrupt                                                 CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000322                                    */
/* Revised:   000603, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:   Delayed field interrupt is synchronized to line ??/??       */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
void CBLineInterrupt( void)
  {
  static UI tmp;
  static UI tmp_t;

#ifdef MOVE_TEXT

   static UC StatusPort_Flag;
   static UC TextLineBegin = 75;  //Indicated from which line the Text lines have to be inserted !
   static UC TextLineDir;  //Indicates direction of the movement of the text line !
   static UC TextLineCnt = 0;	// Text-line nr to display
   static char TextRepeat  = 0;	// Counter : display text line


   static UI LineNum = 262;  //Used to prevent Interlaving flicker !

   StatusPort_Flag = FALSE;

//KS./PEH. Controls position of the text lines in the test pattern.

    if (LineCnt == 1)                  
      {
      if (TextLineDir==0)
        {
          if (TextLineBegin++ >= 100)
            TextLineDir = 1;

          if ( CBSetup.System < NTSC_US)
            LineNum =	313;
          else
			   LineNum =	263;
        }
        else		//Up movement.
        {
          if (TextLineBegin-- <= 50)
            TextLineDir = 0;

          if ( CBSetup.System < NTSC_US)
            LineNum =	312;
          else
	         LineNum =	262;
        }
      }
#endif  // MOVE_TEXT

  if ( CBSetup.System < NTSC_US)
    {
    if ( TF0)                  // If delayed field interrupt...
      {
      TF0 = 0;                  // Clear delayed field interrupt

      SyncLineCnt = 19;            // Next interrupt in 19 lines
      SyncLinePtr = 0;

      LineCnt = 3;

      SoundCnt = 2;
      SoundPtr = 2;

      SoundPtrMax = 64;

      Status |= FIELD1;            // This is field 1 of 8
      Status &= ~SYSSEL;
      //PLD_StatusPort = Status;
      Update_StatusPort(Status);

      return;
      }
    else
      {
      if ( Status & FIELD1)
        {
        Status &= ~FIELD1;
        //PLD_StatusPort = Status;
  	     Update_StatusPort(Status);

        }

      if (++LineCnt >= 625)
        LineCnt = 0;

      if ( !--SoundCnt)
        {
        if ( SoundPtr > SoundPtrMax)
          {
          SoundPtr = 0;

          if ( SoundPtrMax == 63)
            SoundPtrMax = 64;
          else
            SoundPtrMax = 63;
          }
        tmp = Sound625[SoundPtr++];  // Get sound information

        Status &= ~SOUND;            // Clear previous sound sample

        if ( SoundEnable)           // If embedded audio on..
           Status |= ( tmp & SOUND);  //  set sound samples

        SoundCnt = tmp & 0x0F;      // Lines to next sound update
        //PLD_StatusPort = Status;        // Update status port
  	     Update_StatusPort(Status);

        }
      }
    }
  else    // NTSC
    {
    if ( TF0)                        // If delayed field interrupt...
      {
      TF0 = 0;                      // Clear delayed field interrupt

      if ( FrameCnt > 9)
        {
        FrameCnt = 1;
        SoundFrameCnt = 1;
        }

      SyncLineCnt = 12;              // Next interrupt in 12 lines
      SyncLinePtr = 1;

      LineCnt = 6;

      SoundCnt = 2;
      SoundPtr = 3;
      SoundPtrMax = 47;

      Status |= FIELD1;                // This is field 1 of 4
      Status |= SYSSEL;
      //PLD_StatusPort = Status;
      Update_StatusPort(Status);

      return;
      }
    else
      {
      if ( Status & FIELD1)
        {
        Status &= ~FIELD1;
        //PLD_StatusPort = Status;
  	     Update_StatusPort(Status);

        }

      if ( ++LineCnt >= 525)
        {
        FrameCnt++;
        LineCnt = 0;

        if ( ++SoundFrameCnt > 5)
          SoundFrameCnt = 1;
        }

      if ( !--SoundCnt)
        {
        if ( SoundPtr > SoundPtrMax)
          {
          SoundPtr = 0;

          if ( SoundPtrMax == 46)
            SoundPtrMax = 47;
          else
            SoundPtrMax = 46;
          }
        tmp = Sound525[SoundPtr];      // Get sound information

        if ( SoundPtr++ == 47)        // Prepare for line 263
          if ( SoundFrameCnt & 0x01)  // Sound frames 1,3,5 are special
            tmp |= S3OR4;

        Status &= ~SOUND;              // Clear previous sound sample

        if ( SoundEnable)
          Status |= ( tmp & SOUND);    // Sound samples

        SoundCnt = tmp & 0x0F;         // Lines to next sound update
        //PLD_StatusPort = Status;          // Update status port
  	     Update_StatusPort(Status);
        }
      }
    } // if NTSC
#if 0 
  if ( CBSetup.System < NTSC_US)
    {   
    switch (LineCnt)  //<-PAL
      {
      case 311:       // Context switching via SWI7 happens only after
      case 624:       // line 311 or line 624 are done in case of PAL
        JKflag = 1;
        break;
      }
    }
  else
    {
    switch (LineCnt)  //<- NTSC
      {
      case 262:       // Context switching via SWI7 happens only after
      case 524:       // line 265 or line 524 are done in case of NTSC
        JKflag = 1;
        break;
      }
    }
#endif
  if ( --SyncLineCnt == 0)
    {
     tmp = TablePtr[++SyncLinePtr];
    if ( tmp == 0)
      {
      if ( NewPattern)
        {
        NewPattern = Blanking = FALSE;

        TablePtr = newTablePtr;
        }
      SyncLinePtr = 0;
      tmp = TablePtr[0];
      }


    Normal_Status = Status;
    SyncLineCnt = tmp / 256; // extract line cnt to SyncLineCnt
    tmp %= 256;              // extract SramBankX + SyncLineX tmp
    tmp &= 0x3F;
    Normal_Status &= ~( S5S4S3S2 | S1S0);
    Normal_Status |= tmp;

    if ( Blanking)                    // If blanking do NOT enable any
      Normal_Status &= ~S5S4S3S2;    //  bank

    //PLD_StatusPort = temp_status;
    #ifdef MOVE_TEXT
       StatusPort_Flag = TRUE;
    #else
       Update_StatusPort(Normal_Status);
    #endif
   }


#ifdef MOVE_TEXT

//KS./PEH. Test of text line moving in the test pattern.

//Field #1.

  if ((LineCnt >= TextLineBegin)	&&	//Start of text lines !
      (LineCnt <= (TextLineBegin + TEXTLINES_HIGHT)))
    {
    StatusPort_Flag = FALSE;	// Dont update "normal" patton

	 if (LineCnt < (TextLineBegin + TEXTLINES_HIGHT))
       {
       if (--TextRepeat < 0)
          {
          TextRepeat = TEXTREPEAT_HIGH;
          tmp_t = TextLines_SyncLineC[TextLineCnt++];
          tmp_t %= 256;              // extract SramBankX + SyncLineX tmp
          tmp_t &= 0x3F;
          Status &= ~( S5S4S3S2 | S1S0);
          Status |= tmp_t;
          StatusPort_Flag = TRUE;	// Update Status in hardware
          }
	    }
    else	  //Restore original pattern
       {
       TextRepeat  = 0;				// prepare for next text view
       TextLineCnt = 0;				// prepare for next text view
    	 Status = Normal_Status;	// Restore original pattern
       StatusPort_Flag = TRUE;	// Update Status in hardware
       }

    }
  else

//Field #2.
//To prevent Interlacing "flicker" Down and Up movements is handled differently.


  if ((LineCnt >= (TextLineBegin + LineNum)) &&   //263 = number of lines in a feeld #1. !
      (LineCnt <= (TextLineBegin + TEXTLINES_HIGHT + LineNum)))
    {
    StatusPort_Flag = FALSE;	// Dont update "normal" patton

	 if (LineCnt < (TextLineBegin + TEXTLINES_HIGHT + LineNum))
       {
       if (--TextRepeat < 0)
          {
          TextRepeat = TEXTREPEAT_HIGH;
		    tmp_t = TextLines_SyncLineD[TextLineCnt++];
    	    tmp_t %= 256;              // extract SramBankX + SyncLineX tmp
    	    tmp_t &= 0x3F;
    	    Status &= ~( S5S4S3S2 | S1S0);
    	    Status |= tmp_t;
          StatusPort_Flag = TRUE;	// Update Status in hardware
          }
		 }
    else	  //Restore original pattern
       {
       TextRepeat  = 0;				// prepare for next text view
       TextLineCnt = 0;				// prepare for next text view
    	 Status = Normal_Status;	// Restore original pattern
       StatusPort_Flag = TRUE;	// Update Status in hardware
       }
    }
  else
    {
    	 if (StatusPort_Flag)
          Status = Normal_Status;
    }

  if(StatusPort_Flag)
    Update_StatusPort(Status);

#endif

  LEDPort     = LEDMirror;
  TestLEDPort = TestLEDMirror;
  }


/**************************************************************************/
/* CBDefaults                                                       CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000526                                    */
/* Revised:   000617, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void CBDefaults( void)
{
  Settings.CBSetup.System = CBSetup.System = Calibration.ResetSystem;

  if ( CBSetup.System < NTSC_US)
    Settings.CBSetup.Pattern = CBSetup.Pattern = EBUCBar;
  else
    Settings.CBSetup.Pattern = CBSetup.Pattern = SMPTECBar;

  Settings.CBSetup.Delay = CBSetup.Delay = 0;
  Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = 0;
  Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = EmbAudioOff;

  NV_Store( &Settings, SettingsPtr, sizeof( Settings));
}

/**************************************************************************/
/* CBInit                                                           CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000322                                    */
/* Revised:   0009145, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int CBInit( void)
{
  SyncLinePtr = 0;
  TablePtr = &NTSCFullFieldTable[0];

  NewPattern = FALSE;
  Blanking = TRUE;
  SoundEnable = FALSE;

  Status |= SYSSEL;
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  SyncLineCnt = 1;                  // Interrupt in 1 line

  CBSetup.System = Settings.CBSetup.System;
  CBSetup.Pattern = Settings.CBSetup.Pattern;
  CBSetup.Delay = Settings.CBSetup.Delay;
  CBSetup.ScHPhase = Settings.CBSetup.ScHPhase;
  CBSetup.EmbAudioSignal = Settings.CBSetup.EmbAudioSignal;

  return( 0);
}

/**************************************************************************/
/* CBUpdate                                                          CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000413                                    */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int CBUpdate( void)
{
  SetCBSystem( CBSetup.System);      // Also updates pattern

  SetCBDelay( CBSetup.Delay);
  SetCBScHPhase( CBSetup.ScHPhase);
  SetCBEmbAudioSignal( CBSetup.EmbAudioSignal);

  return( OK);
}

/**************************************************************************/
/* SetCBPattern                                                     CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBPattern( UI pattern)
{
  int system = CBSetup.System;

  if ( ValidateCBPattern( system , &pattern) == FAIL)
    return( FAIL);

  Settings.ActivePreset = 0;

  Settings.CBSetup.Pattern = CBSetup.Pattern = pattern;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  Blanking = TRUE;

  Status &= ~S5S4S3S2;                         // Blank line immediately
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  switch ( pattern)
  {
    case SMPTECBar:
      SRAMVideoLoad( dSMPTECBar, SRAMBank1);
      SRAMVideoLoad( dBlueBars, SRAMBank2);
      SRAMVideoLoad( dIWhiteQ, SRAMBank3);

      newTablePtr = &SMPTECBarTable[0];
      break;

    case EBUCBar:
      SRAMVideoLoad( dEBUCBar_625, SRAMBank1);

      newTablePtr = &PALFullFieldTable[0];
      break;

    case FCCCBar:
      SRAMVideoLoad( dFCC_CBar_525, SRAMBank1);

      newTablePtr = &NTSCFullFieldTable[0];
      break;

    case EBUCBarITU:
      SRAMVideoLoad( dEBUCBar_8bit_625, SRAMBank1);

      if ( system < NTSC_US)
        newTablePtr = &PALFullFieldTable[0];
      else
        newTablePtr = &NTSCFullFieldTable[0];
      break;

    case CBar100:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dCBar100_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dFCC_CBar100_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case CBarGrey75:
      SRAMVideoLoad( dCBar75_STD_625, SRAMBank1);
      SRAMVideoLoad( dGrey75_625, SRAMBank2);
  
      newTablePtr = &PALSplitFieldTable[0];
      break;

    case CBarRed75:
      SRAMVideoLoad( dEBUCBar_625, SRAMBank1);
      SRAMVideoLoad( dRed75_625, SRAMBank2);

      newTablePtr = &PALSplitFieldTable[0];
      break;
      
    case Red75:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dRed75_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dRed75_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Multiburst:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dChroma_Multiburst, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dFCC_Multiburst_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case CCIR18:
      SRAMVideoLoad( dCCIR18_625, SRAMBank1);

      newTablePtr = &PALFullFieldTable[0];
      break;

    case Window10:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker10, SRAMBank3);
      SRAMVideoLoad( dWindow10, SRAMBank4);

      if ( system < NTSC_US)
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case Window15:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker15, SRAMBank3);
      SRAMVideoLoad( dWindow15, SRAMBank4);

      if ( system < NTSC_US)                        
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case Window20:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker20, SRAMBank3);
      SRAMVideoLoad( dWindow20, SRAMBank4);

      if ( system < NTSC_US)
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case Window100:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker100, SRAMBank3);
      SRAMVideoLoad( dWindow100, SRAMBank4);

      if ( system < NTSC_US)
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case BlWh15kHz:
      if ( system < NTSC_US)                        
      {
        SRAMVideoLoad( dkHz15BlWh_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dkHz15BlWh_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case White100:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dWhiteLine_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dWhiteLine_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Black:
      if ( system < NTSC_US)
        newTablePtr = &PALBlackTable[0];
      else
        newTablePtr = &NTSCBlackTable[0];
      break;

    case CheckField:
      SRAMVideoLoad( dSDI_test_line1, SRAMBank1);
      SRAMVideoLoad( dSDI_test_line2, SRAMBank2);
      SRAMVideoLoad( dSDI_test_line3, SRAMBank3);

      if ( system < NTSC_US)
        newTablePtr = &PALCheckfieldTable[0];
      else
        newTablePtr = &NTSCCheckfieldTable[0];
      break;

    case DigitalGrey:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dDigitalGrey_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dDigitalGrey_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Stair5:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dGreyScale5_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dGreyScale5_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Stair10:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dGreyScale10_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dGreyScale10_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Crosshatch:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dCrossHatch19_625, SRAMBank1);
        SRAMVideoLoad( dWhiteLine_625, SRAMBank2);

        newTablePtr = &PALCrosshatchTable[0];
      }
      else
      {
        SRAMVideoLoad( dCrossHatch17_525, SRAMBank1);
        SRAMVideoLoad( dWhiteLine_525, SRAMBank2);

        newTablePtr = &NTSCCrosshatchTable[0];
      }
      break;

    case PLUGE:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dPlugeWhite_625, SRAMBank1);
        SRAMVideoLoad( dPlugeGreyC_625, SRAMBank2);
        SRAMVideoLoad( dPlugeGreyD_625, SRAMBank3);
        SRAMVideoLoad( dPlugeGreyE_625, SRAMBank4);

        newTablePtr = &PALPLUGETable[0];
      }
      else
      {
        SRAMVideoLoad( dPlugeWhite_525, SRAMBank1);
        SRAMVideoLoad( dPlugeGreyC_525, SRAMBank2);
        SRAMVideoLoad( dPlugeGreyD_525, SRAMBank3);
        SRAMVideoLoad( dPlugeGreyE_525, SRAMBank4);

        newTablePtr = &NTSCPLUGETable[0];
      }
      break;
  }
#ifdef MOVE_TEXT
  SRAMVideoTextLoad();
#endif  

  NewPattern = TRUE;

  return( OK);
}

/**************************************************************************/
/* SetCBSystem                                     CBAR.C  */
/*                                                  */
/* Author:   Kim Engedahl, DEV, 000331                          */
/* Revised:   000705, KEn, DEV                                 */
/*                                                  */
/* Function:                                           */
/* Remarks:                                            */
/* Returns:                                            */
/* Updates:                                              */
/**************************************************************************/
int SetCBSystem( UC system)
{
  Settings.ActivePreset = 0;

  Settings.CBSetup.System = CBSetup.System = system;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  if ( system < NTSC_US)
  {
    Status &= ~SYSSEL;
    //PLD_StatusPort = Status;    // Select new (SDI) system
    Update_StatusPort(Status);

    WriteCBGain( Calibration.CBCalibration.GainG);
    WriteCBChromaGain( Calibration.CBCalibration.ChromaGainG);
  }
  else
  {
    Status |= SYSSEL;
    //PLD_StatusPort = Status;    // Select new (SDI) system
    Update_StatusPort(Status);

    WriteCBGain( Calibration.CBCalibration.GainM);
    WriteCBChromaGain( Calibration.CBCalibration.ChromaGainM);
  }


  tvp6k_system( system);  // Initialise the composite output

                          // Validate pattern and change if invalid in new system                                    
  ValidateCBPattern( system , &CBSetup.Pattern);

                          // This one initialises TablePtr;
  SetCBPattern( CBSetup.Pattern);

  SetCBScHPhase( CBSetup.ScHPhase);

  LEDSet( CBPALLED, system < NTSC_US);
  LEDSet( CBNTSCLED, system >= NTSC_US);

  return( OK);
}

/**************************************************************************/
/* SetCBDelay                                                       CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   001009, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   CB Delay resolution is 37ns                                  */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBDelay( UL delay)
  {
  UL tmpOffset;

  Settings.ActivePreset = 0;

  Settings.CBSetup.Delay = CBSetup.Delay = delay;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  // Divide GenlockSetup.Delay with 256 as the resulotion is 27ns/256 whereas the
  //  resolution of the TSG is 37ns.
  tmpOffset = delay + GenlockSetup.Delay/256;

  if ( CBSetup.System < NTSC_US)
    {
    tmpOffset += Calibration.CBCalibration.PhaseG;

    tmpOffset %= 4320000L;            // 4*625*1728

    LineOffset = 0xFFFF - ( tmpOffset / 1728L);
    PhaseOffset = 1727 - ( tmpOffset % 1728L);
    }
  else 
    {
    tmpOffset += Calibration.CBCalibration.PhaseM;

    tmpOffset %= 1801800L;            // 2*525*1716

    LineOffset = 0xFFFF - ( tmpOffset / 1716L);
    PhaseOffset = 1715 - ( tmpOffset % 1716L);

    if ( PhaseOffset > 1687)
      PhaseOffset += 12;
    }

  if ( PhaseOffset >= 1443)
    Status |= PHASEREGSEL;        // Set Phase Register Select
  else    
    Status &= ~PHASEREGSEL;        // Clear Phase Register Select
    
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  HOffsetPort = PhaseOffset;

  return( OK);
}

/**************************************************************************/
/* SetCBScHPhase                                                   CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   Note that the resolution is different from BlackBurst, (1.4).*/
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBScHPhase( int schphase)
{
  long tmp;
  UC buffer[2];

  Settings.ActivePreset = 0;

  Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = schphase;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));
  
  if ( CBSetup.System < NTSC_US)
    tmp = (long) Calibration.CBCalibration.ScHPhaseG;
  else 
    tmp = (long) Calibration.CBCalibration.ScHPhaseM;

  tmp += (long) schphase;

  if ( tmp < 0)                      // -1 deg equ. 359 deg a.s.f.
    tmp += 360;

  tmp *= 256;
  tmp /= 360;                        // tmp = ( ScHPhase*256)/360;

  buffer[0] = C_PHASE_address;
  buffer[1] = tmp & 0xFF;
  
  return( TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV));
}

/**************************************************************************/
/* SetCBEmbAudioSignal                                             CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBEmbAudioSignal( UC signal)
{
  Settings.ActivePreset = 0;

  Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = signal;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  if ( signal == EmbAudioOff)
    SoundEnable = FALSE;
  else
  {
    SoundEnable = TRUE;

    Blanking = TRUE;                  // Blank line while loading SRAM

    Status &= ~S5S4S3S2;             // Blank the line immediately
    //PLD_StatusPort = Status;
    Update_StatusPort(Status);

    SRAMSoundLoad( signal);           // Download sound information

    Blanking = FALSE;
  }

  return( OK);
}

/**************************************************************************/
/* ValidateCBPattern                                                CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000405                                    */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int ValidateCBPattern( UC system, UI *pattern)
{
  if ( system < NTSC_US)
  {
    switch ( *pattern)
    {
      case SMPTECBar:
      case FCCCBar:
        *pattern = EBUCBar;
        break;

      default:
        return( OK);
    }
  }
  else
  {
    switch ( *pattern)
    {
      case EBUCBar:
      case CBarGrey75:
      case CBarRed75:
        *pattern = SMPTECBar;
        break;

      case CCIR18:
        *pattern = Multiburst;
        break;

      default:
        return( OK);
    }
  }

  return( FAIL);
}/**************************************************************************/
/* Update_StatusPort                                              CBAR.C  */
/*                                                                        */
/* Author:   Kim Stefensen                                                */
/* Revised:   000617, KEn, DEV                                             */
/*                                                                        */
/* Function:  	It have been determend that under some conditions          */
/* 		when writing to contens of status to the StatusPort 	   */	
/*		a addressing error i introduced !         		  */
/*		To awoid this problem a the following routine forces the  */
/*		C-compiler to use a R register as the source.		  */         
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int Update_StatusPort(UI TempStatus)
{
UI temp_status;

   temp_status = TempStatus;
   temp_status &= 0x7FFF; 	// Forsing the compiler to write from a register instead from a
				// internal memory lacation.
   //SDApin =1;
   PLD_StatusPort = (int)temp_status; // Update status port
   //StatusPort = Status; 	// Update status port
   //SDApin =0;
   return( OK);
}

