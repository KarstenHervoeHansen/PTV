/****************************************************************************/
/* MODULE:                                                                  */
/*  cbar.c - ColorBar generator                                             */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  int WriteCBGain(UC gain)                                                */
/*  int WriteCBChromaGain(UC gain)                                          */
/*  int SRAMVideoLoad(UC signal, UC bank)                                   */
/*  int SRAMSoundLoad(UC signal)                                            */
/*  void CBLineInterrupt(void)                                              */
/*  void CBDefaults(void)                                                   */
/*  int CBInit(void)                                                        */
/*  int CBUpdate(void)                                                      */
/*  int SetCBPattern(UI pattern)                                            */
/*  int SetCBSystem(UC system)                                              */
/*  int SetCBDelay(UL delay)                                                */
/*  int SetCBScHPhase(int schphase)                                         */
/*  int SetCBEmbAudioSignal(UC signal)                                      */
/*  int ValidateCBPattern(UC system, UI *pattern)                           */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES: CB Tables defined at absolute address of 0x2000: 14.03.02         */
/*        On Screen Text added: 20.06.2002                                  */
/*        02.01.2003: Two levels of embedded audio introduced.  Sound data  */
/*        now has 1kHz stereo with level of -18dB, which is to be set for   */
/*        NTSC.                                                             */
/*        31.01.2003: Algorithm for line interrupt changed in order to      */
/*        avoid periodic embedded sound error.                              */
/*        CB Interrupt disabled in SetCBSystem                              */
/*        5.03.2003: SoundEnableMirror introduced to synchronize sound      */
/*        change with interrupt routine.                                    */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "reg8051.h"
#include "define.h"        // Standard PTV  defines
#include <stdio.h>
#include "config.h"
#include "audio.h"
#include "cbar.h"
#include "tables.h"
#include "led_hw.h"
#include "tvp6kdrv.h"
#include "iic.h"
#include "instru.h"
#include <string.h>
#include "rs232.h"
#include "priori.h"


#define TEXT_X_POS       0   /* 320 orig: gives distortion on left text side */
#define TEXT_Y_POS      75

#define SRAMBank0      0x00
#define SRAMBank1      0x10
#define SRAMBank2      0x14
#define SRAMBank3      0x18
#define SRAMBank4      0x1C
#define SRAMBank5      0x20 /****/
#define SRAMBank6      0x24
#define SRAMBank7      0x28
#define SRAMBank8      0x2C
#define SRAMBank9      0x30
#define SRAMBank10     0x34
#define SRAMBank11     0x38
#define SRAMBank12     0x3C /****/

#define dEBUCBar_8bit_625           0
#define dEBUCBar_625                1
#define dSMPTECBar                  2
#define dBlueBars                   3
#define dIWhiteQ                    4
#define dCBar100_625                5
#define dGrey75_625                 6
#define dRed75_625                  7
#define dRed75_525                  8
#define dChroma_Multiburst          9
#define d_CCIR18_625                9
#define dFCC_Multiburst_525        10
#define dChroma_Multiburst_525     11
#define dWhiteLine_625             11
#define dCrossHatch19_625          12
#define dWhiteLine_525             13
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
#define dGreyScale10_625           25
#define dGreyScale10_525           26
#define dWindow50Marker            27
#define dMarkerLine                28
#define dWindow15                  29
#define dMarker15                  30
#define dWindow20                  31
#define dMarker20                  32
#define dWindow100                 33
#define dMarker100                 34
#define dDigitalGrey_625           35
#define dDigitalGrey_525           36
#define dSDI_test_line1            37
#define dSDI_test_line2            38
#define dSDI_test_line3            39
#define dCBar75_STD_625            40
#define dFCC_CBar_525              41
#define dFCC_CBar100_525           42
#define dWindow10                  43
#define dMarker10                  44
#define dkHz15BlWh_625             45
#define dkHz15BlWh_525             46
#define dCCIR18_625                47

/*************************** *************************************************/
/*
The statusport is defined in the PLD as:

  bit 0:  S0         Background system
  bit 1:  S1

  bit 2:  S2         Linetype
  bit 3:  S3
  bit 4:  S4
  bit 5:  S5

  bit 6:  SYSSEL     System select (0:G, 1:M)
  bit 7:  SOUND      Sound (0:off, 1:on)

  bit 8:  3OR4       Samples in sound (0:3 samples, 1:4 samples)
  bit 9:  FIELD1     Field one bit (1 in Field 1/line 1 (out of 8/4) otherwise 0)
  bit 10: CB  
  bit 11: EDHON      EDH bit for future use (0:off, 1:on)

  bit 12: PS         PS (0:disable writing in SRAM, 1: enable writing in SRAM)
  bit 13: Line7      Line 7 pulse in PAL w/PAL_ID
  bit 14: PhaseReg   Phase offset register select
  bit 15: -          Not used
*/
/****************************************************************************/
                    
#define S1S0         0x0003
#define S5S4S3S2     0x003C
#define SYSSEL       0x0040
#define SOUNDON      0x0080
#define S3OR4        0x0100
#define FIELD1       0x0200
#define CB           0x0400
#define EDHON        0x0800
#define PS           0x1000
#define LINE7PULSE   0x2000
#define PHASEREGSEL  0x4000

#define SOUND        (int)(SOUNDON + S3OR4)

#define SyncLineA    1
#define SyncLineB    0
#define SyncLineC    2
#define SyncLineD    3

typedef struct FiltStruct
  {
  UC           InputIndex;
  unsigned int DelayLine[4];
  } FiltStruct;

/****************************************************************************/
// Prototypes internal functions

int ValidateCBPattern(UC, UI *);
int Update_StatusPort(UI);
UI  VideoFilter(unsigned int, FiltStruct *);

extern char* Mstrcpy(register char *, register code char *);

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
//  (size/start is specified in BYTES in config.h)
/*
#define StartOfPROMCode				0x00000L
#define SizeOfPROMCode 				0x40000L

#define StartOfPLDCode				0x40000L
#define SizeOfPLDCode					0x0A000L

#define StartOfEmbAudioCode		0x4A000L
#define SizeOfEmbAudioCode		0x02400L

#define StartOfCBVideoCode		0x50000L
#define SizeOfCBVideoCode			0x30000L
*/
_ROM_ int VideoSignalArray[SizeOfCBVideoCode/2]  _at(StartOfCBVideoCode);
_ROM_ int SoundSignalArray[SizeOfEmbAudioCode/2] _at(StartOfEmbAudioCode);

volatile int SRAMVideoPort  _at(0x44000);       // Port defined in PLD
volatile int SRAMSoundPort  _at(0x40000);       // Port defined in PLD
//volatile int StatusPort   _at(0x6a000);       // Port defined in PLD
volatile int PLD_StatusPort _at(0x6a000);       // Port defined in PLD
volatile int HOffsetPort    _at(0x6a004);       // Port defined in PLD
volatile int ChromaGainPort _at(0x6a008);       // Port defined in PLD

volatile _near int LineOffset;
volatile _near int rec_timeout;
volatile int PhaseOffset;

CBObject CBSetup;

char OSDTextBuffer[16]; // = "DK-Audio A/S : PT502";
code char def_text[] = "PT5202";

char text_on; // shows if text should be displayed or not
static char TextMove = 1; // shows if text moves or not
  //Indicates from which line Text lines have to be inserted:
static UC TextLineBegin = TEXT_Y_POS;

static UC   TextLineDir;       //Indicates direction of the movement of the text line !

enum 
  {
  TEXTLINES_HIGHT = 18,  // Total number of lines for Text BAR
  TEXTREPEAT_HIGH = 1    // Number of times the text lines shut be repeated
  };
static _ROM_  int *MainSignalPtr;

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

#if 1
//Emulates the 8 lines avalible for text.
//Would be implemented ase a seperat table.

#define TA17 (TA16 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineC[] _at(TA17) =    //Interlaced
  {
  1*256  +  SRAMBank12 + SyncLineC,
  1*256  +  SRAMBank5  + SyncLineC,    //Line 0
  1*256  +  SRAMBank6  + SyncLineC,
  1*256  +  SRAMBank7  + SyncLineC,
  1*256  +  SRAMBank8  + SyncLineC,
  1*256  +  SRAMBank9  + SyncLineC,
  1*256  +  SRAMBank10 + SyncLineC,
  1*256  +  SRAMBank11 + SyncLineC,
  1*256  +  SRAMBank12 + SyncLineC,
  };


#define TA18 (TA17 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineD[] _at(TA18) =    //De-Interlaced
  {
  1*256  +  SRAMBank12 + SyncLineD,
  1*256  +  SRAMBank5  + SyncLineD,    //Line 0
  1*256  +  SRAMBank6  + SyncLineD,
  1*256  +  SRAMBank7  + SyncLineD,
  1*256  +  SRAMBank8  + SyncLineD,
  1*256  +  SRAMBank9  + SyncLineD,
  1*256  +  SRAMBank10 + SyncLineD,
  1*256  +  SRAMBank11 + SyncLineD,
  1*256  +  SRAMBank12 + SyncLineD,
  };
#else
//Emulates the 8 lines avalible for text.
//Would be implemented ase a seperat table.

#define TA17 (TA16 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineC[] _at(TA17) =    //Interlaced
  {
  (SRAMBank12 + SyncLineC) & 0x3F,
  (SRAMBank5  + SyncLineC) & 0x3F,    //Line 0
  (SRAMBank6  + SyncLineC) & 0x3F,
  (SRAMBank7  + SyncLineC) & 0x3F,
  (SRAMBank8  + SyncLineC) & 0x3F,
  (SRAMBank9  + SyncLineC) & 0x3F,
  (SRAMBank10 + SyncLineC) & 0x3F,
  (SRAMBank11 + SyncLineC) & 0x3F,
  (SRAMBank12 + SyncLineC) & 0x3F
  };


#define TA18 (TA17 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineD[] _at(TA18) =    //De-Interlaced
  {
  (SRAMBank12 + SyncLineD) & 0x3F,
  (SRAMBank5  + SyncLineD) & 0x3F,    //Line 0
  (SRAMBank6  + SyncLineD) & 0x3F,
  (SRAMBank7  + SyncLineD) & 0x3F,
  (SRAMBank8  + SyncLineD) & 0x3F,
  (SRAMBank9  + SyncLineD) & 0x3F,
  (SRAMBank10 + SyncLineD) & 0x3F,
  (SRAMBank11 + SyncLineD) & 0x3F,
  (SRAMBank12 + SyncLineD) & 0x3F
  };
#endif


static _near UI Status _at(0x120);
static UI Normal_Status;

static _ROM_  UI * _near TablePtr _at(0x122);
static _ROM_  UI *       newTablePtr;

static _near UI LineCnt _at(0x126);
static _near UC FrameCnt _at(0x128);

static _near UC SyncLineCnt _at(0x129);
static _near UC SyncLinePtr _at(0x12A);
                                 
static _near UC SoundFrameCnt _at(0x12B);
static _near UC SoundCnt _at(0x12C);
static _near UC SoundPtr _at(0x12D);
static _near UC SoundPtrMax _at(0x12E);

static char Blanking, NewPattern, SoundEnable;
static char SoundEnableMirror;

extern        _bit JKflag;

_sfrbit       JKpin   _atbit(P1,5);    // P1^5
_sfrbit       SDApin  _atbit(P1,6);    // P1^6

extern CBText TextInfo;


//UI temp_status;      //KS. To force compiler to use a R-register when writing out 
                       //to the Status port.


/**************************************************************************/
/* CBLineInterrupt                                                  CBAR.C */
/*                                                                         */
/* Author:   JK, 24.07.2002                                                */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                         */
/* Function:  INT0 interrupt routine                                       */
/* Remarks:   Delayed field interrupt is synchronized to line ??/??        */
/* Returns:                                                                */
/* Updates:                                                                */
/*  Note:   This function is now rewritten for 'all in C' version of       */
/*          PT5201.  Due to addition of movable text, which takes time,    */
/*          it was necessary to code reloading of Timer0 in assembly.      */
/*          Otherwise, this function took too much time (> 64 uS) and      */
/*          picture was unstable with text on.                             */
/*                                                                         */
/*  Note:      The argument to _using() is PSW (PSWH and PSWL)             */
/*             which this routine will be using. The PSWH is as follows:   */
/*             SM = 1                                                      */
/*             TM = 0                                                      */
/*             RS0 - RS1 = 01, i.e reg bank 1 is used                      */
/*             IM0 - IM3 = 15, i.e this interrupt priority is 15           */
/*             The argument to _interrupt() is vector number.              */
/*             It is obtained as follows:                                  */
/*             vector_number = vector address / 4                          */
/*             In this case: 0x80 / 4 = 32                                 */
/* _interrupt(32) _using(0xAF00) void CBLineInterrupt(void)                */
/***************************************************************************/
#define ARG (SYSMODE | (EAREGBANK << 4) | EAINTLVL) << 8

_interrupt(32) _using(0x9F00) void CBLineInterrupt(void)
  {
  static UI   tmp;
  static UI   tmp_t;

  static UC   TextLineCnt =  0;  // Text-line nr to display
  static char TextRepeat  =  0;  // Counter : display text line 
  //static UC   TextLineDir;       //Indicates direction of the movement of the text line !
  static UC   StatusPort_Flag;

  static UI   LineNum = 262;  //Used to prevent Interlaving flicker !
  
#pragma asm
  push.w  R0
  ADDS.W  _rec_timeout,#0FH
	JBC		  IE1, LoadFieldTimer
  JMP     cblin
LoadFieldTimer:
  mov     R0, _LineOffset

  mov.b   TH0, R0H
  mov.b   TL0, R0L
  
cblin:
  pop.w   R0
#pragma endasm

  if (text_on) // text_on (moved here from end of routine to avoid errors in embedded)
    {
    //KS./PEH. Test of text line moving in the test pattern.
    //Field #1.
    if ((LineCnt >= TextLineBegin)  &&  //Start of text lines !
        (LineCnt <= (TextLineBegin + TEXTLINES_HIGHT)))
      {
      StatusPort_Flag = FALSE;  // Dont update "normal" pattern
      if (LineCnt < (TextLineBegin + TEXTLINES_HIGHT))
        {
        if (--TextRepeat < 0)
          {
          TextRepeat = TEXTREPEAT_HIGH;
          tmp_t = TextLines_SyncLineC[TextLineCnt++];
          tmp_t %= 256;              // extract SramBankX + SyncLineX tmp
          tmp_t &= 0x3F;
          Status &= ~(S5S4S3S2 | S1S0);
          Status |= tmp_t;
          StatusPort_Flag = TRUE;  // Update Status in hardware
          }
        }
      else    //Restore original pattern
         {
         TextRepeat  = 0;         // prepare for next text view
         TextLineCnt = 0;         // prepare for next text view
         Status = Normal_Status;  // Restore original pattern
         StatusPort_Flag = TRUE;  // Update Status in hardware
         }
      }
    else
      {
    //Field #2.
    //To prevent Interlacing "flicker" Down and Up movements are handled differently.
      if ((LineCnt >= (TextLineBegin + LineNum)) &&   //263 = number of lines in a field #1. !
          (LineCnt <= (TextLineBegin + TEXTLINES_HIGHT + LineNum)))
        {
        StatusPort_Flag = FALSE;  // Dont update "normal" patton
        if (LineCnt < (TextLineBegin + TEXTLINES_HIGHT + LineNum))
          {
          if (--TextRepeat < 0)
            {
            TextRepeat = TEXTREPEAT_HIGH;
            tmp_t = TextLines_SyncLineD[TextLineCnt++];
            tmp_t %= 256;              // extract SramBankX + SyncLineX tmp
            tmp_t &= 0x3F;
            Status &= ~(S5S4S3S2 | S1S0);
            Status |= tmp_t;
            StatusPort_Flag = TRUE;  // Update Status in hardware
            }
          }
        else    //Restore original pattern
          {
          TextRepeat  = 0;        // prepare for next text view
          TextLineCnt = 0;        // prepare for next text view
          Status = Normal_Status;  // Restore original pattern
          StatusPort_Flag = TRUE;  // Update Status in hardware
          }
        }
      else
        {
        if (StatusPort_Flag)
          Status = Normal_Status;
        }
      }
    } // if (text_on)
  if (StatusPort_Flag)
    //PLD_StatusPort = Status;
    Update_StatusPort(Status);

  if (text_on)
    { 
    StatusPort_Flag = FALSE;
    //KS./PEH. Controls position of the text lines in the test pattern.
    if (LineCnt == 1)                  
      {
      if (TextLineDir == 0)
        {
        if (TextMove)
          {  
          if (TextLineBegin++ >= 100)
            TextLineDir = 1;
          } 
        if (CBSetup.System < NTSC_US)
          LineNum =  313;
        else
          LineNum =  263;
        }
      else    //Up movement.
        {
        if (TextMove)
          {  
          if (TextLineBegin-- <= 50)
            TextLineDir = 0;
          }    
        if (CBSetup.System < NTSC_US)
          LineNum =  312;
        else
          LineNum =  262;
        }
      }
    } // if (text_on)

  if (CBSetup.System < NTSC_US)
    {
    if (TF0)                  // If delayed field interrupt...
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
      if (Status & FIELD1)
        {
        Status &= ~FIELD1;
        //PLD_StatusPort = Status;
         Update_StatusPort(Status);

        }

      if (++LineCnt >= 625)
        LineCnt = 0;

      if (!--SoundCnt)
        {
        if (SoundPtr > SoundPtrMax)
          {
          SoundPtr = 0;

          if (SoundPtrMax == 63)
            SoundPtrMax = 64;
          else
            SoundPtrMax = 63;
          }
        tmp = Sound625[SoundPtr++];  // Get sound information

        Status &= ~SOUND;            // Clear previous sound sample

        if (SoundEnable)           // If embedded audio on..
           Status |= (tmp & SOUND);  //  set sound samples

        SoundCnt = tmp & 0x0F;      // Lines to next sound update
        //PLD_StatusPort = Status;        
        Update_StatusPort(Status);

        }
      }
    }
  else    // NTSC
    {
    if (TF0)                        // If delayed field interrupt...
      {
      TF0 = 0;                      // Clear delayed field interrupt

      if (FrameCnt > 9)
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
      if (Status & FIELD1)
        {
        Status &= ~FIELD1;
        //PLD_StatusPort = Status;
        Update_StatusPort(Status);
        }
      if (++LineCnt >= 525)
        {
        FrameCnt++;
        LineCnt = 0;

        if (++SoundFrameCnt > 5)
          SoundFrameCnt = 1;
        }
      if (!--SoundCnt)
        {
        if (SoundPtr > SoundPtrMax)
          {
          SoundPtr = 0;
          if (SoundPtrMax == 46)
            SoundPtrMax = 47;
          else
            SoundPtrMax = 46;
          }
        tmp = Sound525[SoundPtr];     // Get sound information
        if (SoundPtr++ == 47)         // Prepare for line 263
          if (SoundFrameCnt & 0x01)   // Sound frames 1,3,5 are special
            tmp |= S3OR4;
        Status &= ~SOUND;             // Clear previous sound sample
        if (SoundEnable)
          Status |= (tmp & SOUND);    // Sound samples
        SoundCnt = tmp & 0x0F;        // Lines to next sound update
        //PLD_StatusPort = Status;
        Update_StatusPort(Status);
        }
      }
    } // if NTSC
  if (--SyncLineCnt == 0)
    {
     tmp = TablePtr[++SyncLinePtr];
    if (tmp == 0)
      {
      if (NewPattern)
        {
        NewPattern = Blanking = FALSE;

        TablePtr = newTablePtr;
        }
      SyncLinePtr = 0;
      tmp = TablePtr[0];
      }
    if (text_on)
      {
      Normal_Status = Status;
      SyncLineCnt = tmp / 256; // extract line cnt to SyncLineCnt
      tmp %= 256;              // extract SramBankX + SyncLineX tmp
      tmp &= 0x3F;
      Normal_Status &= ~(S5S4S3S2 | S1S0);
      Normal_Status |= tmp;
      if (Blanking)                    // If blanking do NOT enable any
        Normal_Status &= ~S5S4S3S2;    //  bank
      //PLD_StatusPort = temp_status;
      StatusPort_Flag = TRUE;
      //PLD_StatusPort = Normal_Status;
      Update_StatusPort(Normal_Status);
      }
    else
      {
      SyncLineCnt = tmp / 256; // extract line cnt to SyncLineCnt
      tmp %= 256;              // extract SramBankX + SyncLineX tmp
      tmp &= 0x3F;
      Status &= ~(S5S4S3S2 | S1S0);
      Status |= tmp;
      if (Blanking)                    // If blanking do NOT enable any
        Status &= ~S5S4S3S2;            //  bank
      //PLD_StatusPort = Status;
      Update_StatusPort(Status);
      }
    } // if (--SyncLineCnt == 0)
  }


/**************************************************************************/
/* WriteCBGain                                                     CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000504                                    */
/* Revised:  000606, KEn, DEV                                             */
/*                                                                        */
/* Function: This function writes calibration values into a port          */
/*           in PLD (V8), from where they are transmitted to the V116A    */
/*           AD8402 DAC (digitally controlled pot) via a local IIC bus,   */
/*           thus altering voltage at pin 44 of V117                      */
/*           (Gain Adjust by V116B).                                      */
/* Remarks:                                                               */
/* Returns:  OK                                                           */
/* Updates:                                                               */
/**************************************************************************/
int WriteCBGain(UC gain)
  {
  return(Write_AD8402(CBPotMeter, 0, gain));
  }

/****************************************************************************/
/* WriteCBChromaGain                                                 CBAR.C */
/*                                                                          */
/* Author:   Kim Engedahl, DEV, 000504                                      */
/* Revised:  000603, KEn, DEV                                               */
/*                                                                          */
/* Function: This function writes calibration vaues to V113,                */
/*           from where a set of DG611 Analog Switches V118 is controlled,  */
/*           altering chroma input signal to amplifier V115A.               */ 
/*                                                                          */
/* Remarks:  The resistor at V118 has been swapped ie. bit 0 is bit 3 etc.  */
/*           The conversion is done in SW instead of correcting the parts   */
/*           list.                                                          */
/* Returns:  OK                                                             */
/* Updates:                                                                 */
/****************************************************************************/
int WriteCBChromaGain(UC gain)
  {
  UC tmp = 0;

  // gain bit 1 = bit4
  // gain bit 2 = bit3
  // gain bit 3 = bit2
  // gain bit 4 = bit1

  if (gain & 0x01)
    tmp += 0x08;
  if (gain & 0x02)
    tmp += 0x04;
  if (gain & 0x04)
    tmp += 0x02;
  if (gain & 0x08)
    tmp += 0x01;
  ChromaGainPort = tmp;
  return(OK);
  }

/***************************************************************************/
/* SRAMVideoLoad                                                   CBAR.C  */
/*                                                                         */
/* Author:   Kim Engedahl, DEV, 000324                                     */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                         */
/* Function: Copy video data to video RAM                                  */
/* Remarks:   S5,S4,S3 and S2 should be kept at ZERO while loading AND the */
/*           interrupt system should NOT clear the PS bit.                 */
/* Returns:                                                                */
/* Updates:                                                                */
/***************************************************************************/
int SRAMVideoLoad(UC signal, UC bank)
  {
  volatile int *VideoDstPtr;
  _ROM_  int *VideoSrcPtr;
  int i;


  Status |= PS;
  Normal_Status = Status;
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  VideoDstPtr = &SRAMVideoPort + ((bank / 4) - 4) * 0x0800;
  VideoSrcPtr = &VideoSignalArray[0x800 * (long)signal];
   if (bank == SRAMBank1)
     MainSignalPtr = VideoSrcPtr;

  for (i = 0; i < 2048; i++)
    *VideoDstPtr++ = *VideoSrcPtr++;

  Status &= ~PS;                  
  Normal_Status = Status;

  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  return(OK);
  }
/**************************************************************************/
/* VideoFilter                                                     CBAR.C */
/*                                                                        */
/* Author:   Poul-Erik Hansen                                             */
/*                                                                        */
/*                                                                        */
/* Returns: Filter sampled                                                */
/* Updates:                                                               */
/**************************************************************************/
unsigned int VideoFilter(unsigned int InputSample, FiltStruct * FiltDelayLine)
  {
  unsigned int FileSum = 0, Y;
  unsigned int FiltCorf[4] = { 3, 13, 13, 3 }; 
  unsigned int *FCptr      = FiltCorf;
  UC           ReadIndex;
  unsigned int *Dptr;

  Dptr = &FiltDelayLine->DelayLine[(FiltDelayLine->InputIndex)++];
  *Dptr++ = InputSample;
  if ((ReadIndex = FiltDelayLine->InputIndex) > 3)
    {
    FiltDelayLine->InputIndex = ReadIndex = 0;
    Dptr                      = FiltDelayLine->DelayLine;
    }

  FileSum  = (Y = *Dptr++) << 1; // 2
  FileSum += Y; // 1
  if (++ReadIndex > 3)
    {
    ReadIndex = 0;
    Dptr      = FiltDelayLine->DelayLine;
    }

  FileSum += (Y = *Dptr++) << 3; // 8
  FileSum += Y << 2; // 4
  FileSum += Y; // 1
  if (++ReadIndex > 3)
    {
    ReadIndex = 0;
    Dptr      = FiltDelayLine->DelayLine;
    }

  FileSum += (Y = *Dptr++) << 3; // 8
  FileSum += Y << 2; // 4
  FileSum += Y; // 1
  if (++ReadIndex > 3)
    {
    ReadIndex = 0;
    Dptr      = FiltDelayLine->DelayLine;
    }

  FileSum += (Y = *Dptr) << 1; // 2
  FileSum += Y; // 1

  return ((unsigned int)16 + FileSum) >> 5;
 // return InputSample;
  }

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

int SRAMVideoTextLoad(void)
  {
  const int text_offset = TEXT_X_POS;
  volatile int *VideoDstPtr;
  _ROM_    int *VideoSrcPtr;
  int i,       // Sample index
      n,       // hos font index (0..5)
      m,       // Repeat sample
      line_nr; // Line number (0..6)
  char *Str;
  unsigned int Sample, FiltValue;
  static const UC TextBanks[] = 
    {
    SRAMBank5,  SRAMBank6,  SRAMBank7,  SRAMBank8,
    SRAMBank9,  SRAMBank10, SRAMBank11, SRAMBank12
    };

  FiltStruct Yfilt = { 0,   0,   0,   0,   0 };
  FiltStruct Ufilt = { 0, 512, 512, 512, 512 };
  FiltStruct Vfilt = { 0, 512, 512, 512, 512 };

  Status |= PS;
  Normal_Status  = Status;
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  for (line_nr = 0; line_nr < 8; line_nr++)
    {
    VideoDstPtr = &SRAMVideoPort + ((TextBanks[line_nr] / 4) - 4) * 0x0800;

    VideoSrcPtr = MainSignalPtr;

    for (m = 0, i = 0; m < (56 + text_offset); m++, i += 2) // 56 Y sample, 14 U sample & 14 V sample
      {
      Sample = *VideoSrcPtr++;    // Y Value
      if (m >= (48 + text_offset))
        Sample = 96;

      FiltValue = VideoFilter(Sample, &Yfilt);
      *VideoDstPtr++ = (m > (24 + text_offset)) ? FiltValue : Sample;

      Sample = *VideoSrcPtr++;    // U/V Value
      if (m >= (48 + text_offset))
        Sample = 512;

      FiltValue = VideoFilter(Sample, (i & 0x02) ?  &Vfilt : &Ufilt);
      *VideoDstPtr++ = (m > (24 + text_offset)) ? FiltValue : Sample;
      }

    Str = OSDTextBuffer;

    while(*Str)
      {
      UC StrValue = *Str++;
      UC Font     = Font57[line_nr + (7 * (StrValue - ' '))];
      if (line_nr == 7)
        Font = 0;    

      for (n = 0; n < 6; n++)
        {
        int Yvalue = (0x20 & (Font << n)) ? 895 : 96;
        for (m = 0; m < 3; m++)
          {
          *VideoDstPtr++ = VideoFilter(Yvalue, &Yfilt);
          if (i & 0x02)
            *VideoDstPtr++ = VideoFilter(512, &Vfilt);
          else
            *VideoDstPtr++ = VideoFilter(512, &Ufilt);
          VideoSrcPtr += 2;
          i += 2;
          }
        }
      }

    for (m = 0; m < 16; m++, i += 2) // 16 Y sample, 4 U sample & 4 V sample
      {
      Sample = *VideoSrcPtr++;    // Y Value
      if (m < 8)
        Sample = 96;

      *VideoDstPtr++ = VideoFilter(Sample, &Yfilt);

      Sample = *VideoSrcPtr++;    // U/V Value
      if (m < 8)
        Sample = 512;

      *VideoDstPtr++ = VideoFilter(Sample, (i & 0x02) ?  &Vfilt : &Ufilt);
      }

    for (; i < 2048; i++)
      {
      *VideoDstPtr++ = *VideoSrcPtr++;
      }
    }

  Status &= ~PS;                  
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);
  return(OK);
  }

/**************************************************************************/
/* SRAMSoundLoad                                                   CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000410                                    */
/* Revised:   000603, KEn, DEV                                            */
/*                                                                        */
/* Function: Copy sound data to sound RAM                                 */
/* Remarks:  If signal is EmbAudioOff, this is not called                 */
/*           If system is PAL, we load data for stereo -20dB              */
/*           otherwise we load data for stereo -18dB                      */
/*           signal is Off  - 0: not aplicable here                       */
/*                     Sil  - 1                                           */
/*                     1kHz - 2                                           */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int SRAMSoundLoad(UC signal)
  {
  volatile int *SoundDstPtr;
  _ROM_  int *SoundSrcPtr;
  int i;

  signal = (signal == EmbAudioStereo1kHz) ? 1 : 0;

  Status |= PS;                 // Set PS bit while loading
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  SoundDstPtr = &SRAMSoundPort;
  // Change: we now have -18 dB in PAL and -20 dB in NTSC (ver 23)
  if (signal) // if it is 1kHz
    {
    if (CBSetup.System < NTSC_US)
      SoundSrcPtr = &SoundSignalArray[2304 * (signal + 1)]; // point @ -18 dB data
    else
      SoundSrcPtr = &SoundSignalArray[2304 * signal];
    }
  else
    SoundSrcPtr = &SoundSignalArray[2304 * signal];

  for (i = 0; i < 2304; i++)
    {
    *SoundDstPtr++ = *SoundSrcPtr++;
    }

  Status &= ~PS;                
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);
  return(OK);
  }



/**************************************************************************/
/* CBDefaults                                                     CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000526                                    */
/* Revised:  26.02.2003: JK                                               */
/*                                                                        */
/* Function: To initialize Settings.CBSetup and CBSetup with default      */
/*           values.                                                      */
/* Remarks:  Settings are stored in NVRAM                                 */
/*           This function is only used on instrument reset               */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
void CBDefaults(void)
  {
  Settings.CBSetup.System = CBSetup.System = Calibration.ResetSystem;

  if (CBSetup.System < NTSC_US)
    Settings.CBSetup.Pattern = CBSetup.Pattern = EBUCBar;
  else
    Settings.CBSetup.Pattern = CBSetup.Pattern = SMPTECBar;

  Settings.CBSetup.Delay          = CBSetup.Delay = 0;
  Settings.CBSetup.ScHPhase       = CBSetup.ScHPhase = 0;
  Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = EmbAudioOff;

  Mstrcpy(Settings.CBSetup.TextData.Text, def_text);
  Mstrcpy(CBSetup.TextData.Text, def_text);
  Settings.CBSetup.TextData.TextEnable   = (char) OFF;
  CBSetup.TextData.TextEnable            = (char) OFF;
  Settings.CBSetup.TextData.TextMovement = (char) OFF;
  CBSetup.TextData.TextMovement          = (char) OFF;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));
  }

/**************************************************************************/
/* CBInit                                                         CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000322                                    */
/* Revised:  26.02.2003: JK                                               */
/*                                                                        */
/* Function: To copy Settings.CBSetu to CBSetup and initialize certain    */
/*           variables.                                                   */
/*           This function is only used on instrument reset and on        */
/*           instrument startup                                           */
/* Returns:  0                                                            */
/* Updates:  SyncLinePtr                                                  */
/*           SyncLineCnt                                                  */
/*           TablePtr                                                     */
/*           NewPattern                                                   */
/*           Blanking                                                     */
/*           SoundEnable                                                  */
/*           Status                                                       */
/*           TextMove                                                     */
/*           text_on                                                      */
/**************************************************************************/
int CBInit(void)
  {
  SyncLinePtr = 0;
  TablePtr    = &NTSCFullFieldTable[0];

  NewPattern  = FALSE;
  Blanking    = TRUE;
  SoundEnable = FALSE;

  Status |= SYSSEL;
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  SyncLineCnt = 1;                  // Interrupt in 1 line

  CBSetup.System         = Settings.CBSetup.System;
  CBSetup.Pattern        = Settings.CBSetup.Pattern;
  CBSetup.Delay          = Settings.CBSetup.Delay;
  CBSetup.ScHPhase       = Settings.CBSetup.ScHPhase;
  CBSetup.EmbAudioSignal = Settings.CBSetup.EmbAudioSignal;

  strcpy(CBSetup.TextData.Text, Settings.CBSetup.TextData.Text);
  CBSetup.TextData.TextEnable   = Settings.CBSetup.TextData.TextEnable;
  CBSetup.TextData.TextMovement = Settings.CBSetup.TextData.TextMovement;
  TextMove = 0;
  text_on  = 0;
  return(0);
  }

/**************************************************************************/
/* CBUpdate                                                       CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000413                                    */
/* Revised:  26.02.2003: JK                                               */
/*                                                                        */
/* Function: To set all parameters in CBSetup using SetCBXX() functions   */
/*           This function is only used on instrument reset and on        */
/*           instrument startup                                           */
/* Remarks:  Settings are stored in NVRAM in each of SetCBXX() function   */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int CBUpdate(void)
  {
  SetCBSystem(CBSetup.System);      // Also sets pattern

  SetCBDelay(CBSetup.Delay);
  SetCBScHPhase(CBSetup.ScHPhase);
  SetCBEmbAudioSignal(CBSetup.EmbAudioSignal);

  SetCBText(CBSetup.TextData.Text);  // Also sets pattern if text is on
  SetCBTextOnoff(CBSetup.TextData.TextEnable);
  SetCBMovement(CBSetup.TextData.TextMovement);
  
  return(OK);
  }

/**************************************************************************/
/* SetCBPattern                                                   CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                               */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int SetCBPattern(UI pattern)
  {
  int system = CBSetup.System;

  if (ValidateCBPattern(system , &pattern) == FAIL)
    return(FAIL);

  Settings.ActivePreset = 0;

  Settings.CBSetup.Pattern = CBSetup.Pattern = pattern;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));

  // 4 lines below are to prevent embedded sound disappear fail
  SoundEnableMirror = SoundEnable;
  SoundEnable       = FALSE;

  while (Status & 0x080) // while SOUND bit is hi...
    ;

  EX0 = 0;   // Disable Color Bar Interrupt

  Status &= ~S5S4S3S2;                     // Blank line immediately
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);

  TextLineBegin = TEXT_Y_POS;
  SyncLinePtr   = 0;
  SyncLineCnt   = 1;                  // Interrupt in 1 line
  NewPattern    = FALSE;
  TextLineDir   = 0;

  Blanking      = TRUE;

  switch (pattern)
    {
    case SMPTECBar:                        // This may have text
      SRAMVideoLoad(dSMPTECBar, SRAMBank1);
      SRAMVideoLoad(dBlueBars, SRAMBank2);
      SRAMVideoLoad(dIWhiteQ, SRAMBank3);
      newTablePtr = &SMPTECBarTable[0];
      break;

    case EBUCBar:                          // This may have text
      SRAMVideoLoad(dEBUCBar_625, SRAMBank1);
      newTablePtr = &PALFullFieldTable[0];
      break;

    case FCCCBar:
      SRAMVideoLoad(dFCC_CBar_525, SRAMBank1);
      newTablePtr = &NTSCFullFieldTable[0];
      break;

    case EBUCBarITU:
      SRAMVideoLoad(dEBUCBar_8bit_625, SRAMBank1);

      if (system < NTSC_US)
        newTablePtr = &PALFullFieldTable[0];
      else
        newTablePtr = &NTSCFullFieldTable[0];
      break;

    case CBar100:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dCBar100_625, SRAMBank1);
        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dFCC_CBar100_525, SRAMBank1);
        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case CBarGrey75:
      SRAMVideoLoad(dCBar75_STD_625, SRAMBank1);
      SRAMVideoLoad(dGrey75_625, SRAMBank2);
      newTablePtr = &PALSplitFieldTable[0];
      break;

    case CBarRed75:
      SRAMVideoLoad(dEBUCBar_625, SRAMBank1);
      SRAMVideoLoad(dRed75_625, SRAMBank2);
      newTablePtr = &PALSplitFieldTable[0];
      break;
      
    case Red75:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dRed75_625, SRAMBank1);
        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dRed75_525, SRAMBank1);
        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case Multiburst:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dChroma_Multiburst, SRAMBank1);
        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dFCC_Multiburst_525, SRAMBank1);
        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case CCIR18:
      SRAMVideoLoad(dCCIR18_625, SRAMBank1);
      newTablePtr = &PALFullFieldTable[0];
      break;

    case Window10:
      SRAMVideoLoad(dWindow50Marker, SRAMBank1);
      SRAMVideoLoad(dMarkerLine, SRAMBank2);
      SRAMVideoLoad(dMarker10, SRAMBank3);
      SRAMVideoLoad(dWindow10, SRAMBank4);

      if (system < NTSC_US)
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case Window15:
      SRAMVideoLoad(dWindow50Marker, SRAMBank1);
      SRAMVideoLoad(dMarkerLine, SRAMBank2);
      SRAMVideoLoad(dMarker15, SRAMBank3);
      SRAMVideoLoad(dWindow15, SRAMBank4);

      if (system < NTSC_US)                        
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case Window20:
      SRAMVideoLoad(dWindow50Marker, SRAMBank1);
      SRAMVideoLoad(dMarkerLine, SRAMBank2);
      SRAMVideoLoad(dMarker20, SRAMBank3);
      SRAMVideoLoad(dWindow20, SRAMBank4);

      if (system < NTSC_US)
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case Window100:
      SRAMVideoLoad(dWindow50Marker, SRAMBank1);
      SRAMVideoLoad(dMarkerLine, SRAMBank2);
      SRAMVideoLoad(dMarker100, SRAMBank3);
      SRAMVideoLoad(dWindow100, SRAMBank4);

      if (system < NTSC_US)
        newTablePtr = &PALWindowTable[0];
      else
        newTablePtr = &NTSCWindowTable[0];
      break;

    case BlWh15kHz:
      if (system < NTSC_US)                        
        {
        SRAMVideoLoad(dkHz15BlWh_625, SRAMBank1);
        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dkHz15BlWh_525, SRAMBank1);
        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case White100:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dWhiteLine_625, SRAMBank1);
        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dWhiteLine_525, SRAMBank1);
        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case Black:
      if (system < NTSC_US)
        newTablePtr = &PALBlackTable[0];
      else
        newTablePtr = &NTSCBlackTable[0];
      break;

    case CheckField:
      SRAMVideoLoad(dSDI_test_line1, SRAMBank1);
      SRAMVideoLoad(dSDI_test_line2, SRAMBank2);
      SRAMVideoLoad(dSDI_test_line3, SRAMBank3);

      if (system < NTSC_US)
        newTablePtr = &PALCheckfieldTable[0];
      else
        newTablePtr = &NTSCCheckfieldTable[0];
      break;

    case DigitalGrey:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dDigitalGrey_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dDigitalGrey_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case Stair5:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dGreyScale5_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dGreyScale5_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case Stair10:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dGreyScale10_625, SRAMBank1);

        newTablePtr = &PALFullFieldTable[0];
        }
      else
        {
        SRAMVideoLoad(dGreyScale10_525, SRAMBank1);

        newTablePtr = &NTSCFullFieldTable[0];
        }
      break;

    case Crosshatch:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dCrossHatch19_625, SRAMBank1);
        SRAMVideoLoad(dWhiteLine_625, SRAMBank2);

        newTablePtr = &PALCrosshatchTable[0];
        }
      else
        {
        SRAMVideoLoad(dCrossHatch17_525, SRAMBank1);
        SRAMVideoLoad(dWhiteLine_525, SRAMBank2);

        newTablePtr = &NTSCCrosshatchTable[0];
        }
      break;

    case PLUGE:
      if (system < NTSC_US)
        {
        SRAMVideoLoad(dPlugeWhite_625, SRAMBank1);
        SRAMVideoLoad(dPlugeGreyC_625, SRAMBank2);
        SRAMVideoLoad(dPlugeGreyD_625, SRAMBank3);
        SRAMVideoLoad(dPlugeGreyE_625, SRAMBank4);

        newTablePtr = &PALPLUGETable[0];
        }
      else
        {
        SRAMVideoLoad(dPlugeWhite_525, SRAMBank1);
        SRAMVideoLoad(dPlugeGreyC_525, SRAMBank2);
        SRAMVideoLoad(dPlugeGreyD_525, SRAMBank3);
        SRAMVideoLoad(dPlugeGreyE_525, SRAMBank4);

        newTablePtr = &NTSCPLUGETable[0];
        }
      break;
    }
  SRAMVideoTextLoad();

  // Text, moving or stationary, is only possible with 
  // SMPTECBar and EBUCBar

  if ((pattern == SMPTECBar) || (pattern == EBUCBar))
    {
    if (CBSetup.TextData.TextEnable == ON)
      text_on = ON;
    else
      text_on = OFF;
    }
  else
    text_on = OFF;

  NewPattern = TRUE;

  // Lines below are to prevent embedded sound disappear fail
  if (SoundEnableMirror)
    {
    SoundEnable = TRUE;
    Blanking = TRUE;                 // Blank line while loading SRAM
    Status &= ~S5S4S3S2;             // Blank the line immediately
    //PLD_StatusPort = Status;
    Update_StatusPort(Status);
    Blanking = FALSE;
    }

  EX0 = 1;   // Enable Color Bar Interrupt

  return(OK);
  }

/**************************************************************************/
/* SetCBSystem                                                    CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000705, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                               */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int SetCBSystem(UC system)
  {
  Settings.ActivePreset = 0;

  Settings.CBSetup.System = CBSetup.System = system;
  // NV_store below is not necessary, as SetCBPattern() will perform it
  //NV_Store(&Settings, SettingsPtr, sizeof(Settings));

  // 4 lines below are to prevent embedded sound disappear fail
  SoundEnableMirror = SoundEnable;
  SoundEnable       = FALSE;

  while (Status & 0x080) // while SOUND bit is hi...
    ;


  EX0 = 0;   // Disable Color Bar Interrupt

  if (system < NTSC_US)
    {
    Status &= ~SYSSEL;
    //PLD_StatusPort = Status;    // Select new (SDI) system
    Update_StatusPort(Status);
    WriteCBGain(Calibration.CBCalibration.GainG);
    WriteCBChromaGain(Calibration.CBCalibration.ChromaGainG);
    }
  else
    {
    Status |= SYSSEL;
    //PLD_StatusPort = Status;    // Select new (SDI) system
    Update_StatusPort(Status);
    WriteCBGain(Calibration.CBCalibration.GainM);
    WriteCBChromaGain(Calibration.CBCalibration.ChromaGainM);
    }
  tvp6k_system(system);   // Initialise the composite output
                          // Validate pattern and change if invalid in new system                                    
  ValidateCBPattern(system, &CBSetup.Pattern);
                          // This one initialises TablePtr;
  SetCBPattern(CBSetup.Pattern);
  SetCBScHPhase(CBSetup.ScHPhase);

/* We have 2 embedded audio levels now, -20 for PAL and -18 for NTSC
   When system changes, we have to call SRAMSoundLoad() to cange level
   if necessary 
*/
  //SRAMSoundLoad(CBSetup.EmbAudioSignal);
  SetCBEmbAudioSignal(CBSetup.EmbAudioSignal);
  LEDSet(CBPALLED, system  < NTSC_US);
  LEDSet(CBNTSCLED, system >= NTSC_US);

#if 0
#pragma asm
LoadFieldTimer_Again:
  mov     R5, _LineOffset

  mov.b   TH0, R5H
  mov.b   TL0, R5L
#pragma endasm
#endif

  // Lines below are to prevent embedded sound disappear fail
  if (SoundEnableMirror)
    {
    SoundEnable = TRUE;
    Blanking = TRUE;                 // Blank line while loading SRAM
    Status &= ~S5S4S3S2;             // Blank the line immediately
    //PLD_StatusPort = Status;
    Update_StatusPort(Status);
    Blanking = FALSE;
    }
  EX0 = 1;   // Enable Color Bar Interrupt

  return(OK);
}

/**************************************************************************/
/* SetCBDelay                                                     CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   001009, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:   CB Delay resolution is 37ns                                 */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int SetCBDelay(UL delay)
  {
  UL tmpOffset;

  Settings.ActivePreset = 0;

  Settings.CBSetup.Delay = CBSetup.Delay = delay;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));
  // Divide GenlockSetup.Delay with 256 as the resulotion is 27ns/256 whereas the
  //  resolution of the TSG is 37ns.
  tmpOffset = delay + GenlockSetup.Delay/256;

  if (CBSetup.System < NTSC_US)
    {
    tmpOffset += Calibration.CBCalibration.PhaseG;
    tmpOffset %= 4320000L;            // 4*625*1728
    LineOffset = 0xFFFF - (tmpOffset / 1728L);
    PhaseOffset = 1727 - (tmpOffset % 1728L);
    }
  else 
    {
    tmpOffset += Calibration.CBCalibration.PhaseM;
    tmpOffset %= 1801800L;            // 2*525*1716
    LineOffset = 0xFFFF - (tmpOffset / 1716L);
    PhaseOffset = 1715 - (tmpOffset % 1716L);
    if (PhaseOffset > 1687)
      PhaseOffset += 12;
    }
  if (PhaseOffset >= 1443)
    Status |= PHASEREGSEL;        // Set Phase Register Select
  else    
    Status &= ~PHASEREGSEL;        // Clear Phase Register Select
  //PLD_StatusPort = Status;
  Update_StatusPort(Status);
  HOffsetPort = PhaseOffset;
  return(OK);
  }

/**************************************************************************/
/* SetCBScHPhase                                                  CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:  Note that the resolution is different from BlackBurst, (1.4).*/
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int SetCBScHPhase(int schphase)
  {
  long tmp;
  UC buffer[2];

  Settings.ActivePreset = 0;
  Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = schphase;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));
  
  if (CBSetup.System < NTSC_US)
    tmp = (long) Calibration.CBCalibration.ScHPhaseG;
  else 
    tmp = (long) Calibration.CBCalibration.ScHPhaseM;

  tmp += (long) schphase;

  if (tmp < 0)                      // -1 deg equ. 359 deg a.s.f.
    tmp += 360;

  tmp *= 256;
  tmp /= 360;                        // tmp = (ScHPhase*256)/360;

  buffer[0] = C_PHASE_address;
  buffer[1] = tmp & 0xFF;
  
  return(TransmitIIC(TVP6K_IIC_address, 2, buffer));
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
int SetCBEmbAudioSignal(UC signal)
  {
  Settings.ActivePreset = 0;

  Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = signal;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));

  // 4 lines below are to prevent embedded sound disappear fail
  SoundEnableMirror = SoundEnable;
  SoundEnable       = FALSE;

  while (Status & 0x080) // while SOUND bit is hi...
    ;

  EX0 = 0;   // Disable Color Bar Interrupt

  if (signal == EmbAudioOff)
    SoundEnable = FALSE;
  else
    {
    Blanking    = TRUE;             // Blank line while loading SRAM
    Status     &= ~S5S4S3S2;        // Blank the line immediately
    //PLD_StatusPort = Status;
    Update_StatusPort(Status);
    SRAMSoundLoad(signal);
    Blanking = FALSE;
    SoundEnable = TRUE;
    }
  EX0 = 1;   // Enable Color Bar Interrupt
  return(OK);
  }

void SetCBText(char *tptr)
  {
  strcpy((char*)OSDTextBuffer, tptr);
  strcpy(Settings.CBSetup.TextData.Text, tptr);
  strcpy(CBSetup.TextData.Text, tptr);
  if (text_on)
    SetCBPattern(Settings.CBSetup.Pattern); // has TextLineBegin = TEXT_Y_POS;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));
  }

void SetCBTextOnoff(char onoff)
  {
  // Text, moving or stationary, is only possible with 
  // SMPTECBar and EBUCBar

  CBSetup.TextData.TextEnable          = onoff;
  Settings.CBSetup.TextData.TextEnable = onoff;
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));

  TextLineBegin = TEXT_Y_POS;

  if ((Settings.CBSetup.Pattern == SMPTECBar) || (Settings.CBSetup.Pattern == EBUCBar))
    {
    if (CBSetup.TextData.TextEnable == ON)
      text_on = ON;
    else
      text_on = OFF;
    }
  else
    text_on = OFF;
  }

void SetCBMovement(char move)
  {
  TextMove = move;
  if (move == OFF)
    TextLineBegin = TEXT_Y_POS;

  CBSetup.TextData.TextMovement = move;
  Settings.CBSetup.TextData.TextMovement = move;
  //SetCBPattern(Settings.CBSetup.Pattern);
  NV_Store(&Settings, SettingsPtr, sizeof(Settings));
  }
/**************************************************************************/
/* ValidateCBPattern                                              CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000405                                    */
/* Revised:   000603, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                               */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
#if 1
int ValidateCBPattern(UC system, UI *pattern)
  {
  if (system < NTSC_US)
    {
    switch (*pattern)     // PAL
      {
      case SMPTECBar:
      case FCCCBar:
        *pattern = EBUCBar;
        break;
      default:
        return(OK);
      }
    }
  else
    {
    switch (*pattern)     // NTSC
      {
      case EBUCBar:
      case CCIR18:
      case CBarRed75:
        *pattern = SMPTECBar;
        break;
      default:
        return(OK);
      }
    }
  return(FAIL);
  }
#else
int ValidateCBPattern(UC system, UI *pattern)
  {
  switch (pattern)
    {
    case SMPTECBar:
    case FCCCBar:
    case CBar75ITU:  // JK addition
      if (system < NTSC_US)
        return( 0);
      break;

    case Multiburst:  // At present Multiburst is not valid at all
       return(0);

    case EBUCBar:
    case CBarRed75:
    case CCIR18:
    case EBUCBarITU: // JK addition
      if (system >= NTSC_US)
        return( 0);
      break;

    case CBarGrey75:
       return(0);
    }
  return(-1);
  }
#endif
/**************************************************************************/
/* Update_StatusPort                                              CBAR.C  */
/*                                                                        */
/* Author:   Kim Stefensen                                                */
/* Revised:   000617, KEn, DEV                                            */
/*                                                                        */
/* Functio   It have been determend that under some conditions            */
/*           when writing to contens of status to the StatusPort          */  
/*           a addressing error i introduced !                            */
/*           To awoid this problem a the following routine forces the     */
/*           C-compiler to use a R register as the source.                */         
/* Remarks:                                                               */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
int Update_StatusPort(UI TempStatus)
  {
  UI temp_status;

  temp_status = TempStatus;
  temp_status &= 0x7FFF;   // Forsing the compiler to write from a register instead from a
                           // internal memory lacation.


  PLD_StatusPort = (int)temp_status; 
  return(OK);
  }

/* end of cbar.c */
