/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997                  */
/*  Project:    PT8602/03 SDI Digital test signal generator                */
/*  Module:    D1GENER.C                                                   */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  960703                                                     */
/*  Rev. date:  990308, KEn  DEV                                           */
/*  Status:    Version 1.1                                                 */
/*                                                                         */
/*  This module contains the following functions:                          */
/*    banked interrupt void LineIntr(void)                                 */
/*    interrupt void FieldIntr(void)                                       */
/*    main()                                                               */
/*                                                                         */
/* Changes:                                                                */
/* 990308: Renamed some variables                                          */
/* 980929: Added support for HWInformation/SW900Version                    */
/* 020626: Modified for moving text, see LineIntr()                        */
/***************************************************************************/


#include <xa.h>
#include <intrpt.h>

#include "d1gener.h"
#include "d1patt.h"
#include "d1tables.h"
#include "d1util.h"
#include "rs232.h"
#include "comm.h"
#include "d1cmd.h"
#include "version.h"

#define  MyAddress    (UC) 0x17

#define  EVEN  1
#define  ODD   0
#define  DOWN  0
#define  UP    1
#define  ODD_FIELD_OFFSET_G  313
#define  ODD_FIELD_OFFSET_M  263
// ******************************************************************
// DEFINITION OF HARDWARE PORTS
// ******************************************************************
volatile UC StatusPort     @ 0x8000;  // LD.STATUS signal

volatile UC LinePortL      @ 0xA000;  // 
volatile UC LinePortH      @ 0xA800;

volatile UC OffsetPortL    @ 0x8800;  // LD.LSBPH signal
volatile UC OffsetPortH    @ 0x9000;  // LD.MSBPH signal

volatile UC TextStatusPort @ 0xC000;
volatile UC TextPort[256]  @ 0xB000;  // LDTXTRAM

/* Apparentlly, below are sound data tables which lays in ROM from
   0x200 to 0xEF00
*/
volatile UC LowSoundRAM[11520]       @ 0x0200;
volatile UC LowClickSoundRAM[11520]  @ 0x4200;
volatile UC HighSoundRAM[11520]      @ 0x8200;
volatile UC HighClickSoundRAM[11520] @ 0xC200;

bit PageSelect                       @ 0x38C;

// ******************************************************************
// DEFINITION OF VARIABLES
// ******************************************************************
near UL  CalOffset;          // Calibration offset
near UL  UserOffset;         // User offset
near UL  GenlockOffset;      // Genlock offset

near UI  PhaseOffset;        // Calculated timing offset, ie. < 64us
near UI  LineOffset;         // Calculated line offset

near UC  Status;             // Status byte

near UC  PatternNo;          // Pattern number

bit      EDHInsertion;       // 1: EDH insertion ON
bit      TVSystem;           // 1: PAL-G,  0: NTSC

near UC  AudioSignal;        // Embedded audio signal selection
near UC  AudioLevel;         // Embedded audio level selection

near UC  ErrorStatus;        // Global error status

near UC  FrameCnt;          // Frame counter
near int LineCnt;           // Line counter

near int SoundPtrMax;       // Maximum number of entries in sound table
near int SoundPtr;          // Pointer to entry in sound table
near UC  SoundFrameCnt;     // Sound sequence frame counter, (NTSC only)
near UC  SoundCnt;          // Counter used to determine next change in sound
                            //  pattern
bit      SoundEnable;       // 1: Sound ON

near char OSDText[9];       // Buffer for text to display
near UC   first_line_w_text = 48; // Start of text insertion. Actual start is +1 
                            // is 24 or 38
                            // This is number of first line which contains text

near int  TextCnt;          // Counter used for text insertion
bit       TextEnable;       // 1: Display text ON
bit       line_has_text;    // 1: Line contains text
UC        TextMovementDir;  //Indicates direction of the movement of the text lines !

near UC TextStatus;

bit GeneratorRunning;

code struct PATTERN_ITEM* near ptr;

/***************************************************************************/

ROM_VECTOR(IV_EX1, FieldIntr, IV_SYSTEM+IV_PRI14)
ROM_VECTOR(IV_EX0, LineIntr,  IV_SYSTEM+IV_PRI13+IV_BANK1)

ROM_VECTOR(IV_RI0, RS232Intr, IV_SYSTEM+IV_PRI12+IV_BANK2)
ROM_VECTOR(IV_TI0, RS232Intr, IV_SYSTEM+IV_PRI12+IV_BANK2)

/***************************************************************************/
/*  LineIntr                                                     D1GENER.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    970217, KEn                                                 */
/*                                                                         */
/*  Function:                                                              */
/*  Remarks:    DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY        */
/*  Returns:                                                               */
/*  Updates:    26.06.2002: Moving text feature added, JK                  */
/***************************************************************************/
banked interrupt void LineIntr(void)
 {  
 UI temp;


  if (TVSystem == PALG)
    {                                // If TV system is PAL-G..
  /* BEGIN */
    if ((LineCnt == 10) || (LineCnt == (10 + ODD_FIELD_OFFSET_G)))  // once per field....
      {                                                  // line 10 and 323..
      if (TextMovementDir == DOWN)
        {
        first_line_w_text++;
        TextCnt++;
        if (first_line_w_text>= 100)
          TextMovementDir = UP;
        }
      else    //Up movement.
        {
        first_line_w_text--;
        TextCnt--;
        if (first_line_w_text <= 48)
          TextMovementDir = DOWN;
        }
      }
  /* END */
    if (TF1)
      {                              // If delayed field interrupt..
      TF1 = 0;                       //  setup for line 3, (app. 12us)
      TF0 = 0;                       // Clear possible LINE CNT intr.

      if (PhaseOffset >= 1443)       // Readout BEFORE PhaseOffset to
        TextStatus |= 0x02;          //  correct 1 line offset
      else
        TextStatus &= 0xFD;

      TextStatusPort = TextStatus;

      OffsetPortL = (UC) PhaseOffset;
      OffsetPortH = (UC) (PhaseOffset >> 8);

      asm("mov.b  0451H, #HIGH(-19)"); // load Timer0
      asm("mov.b  0450H, #LOW(-19)");

      LineCnt  = 2;
      SoundCnt = 2;
      SoundPtr = 2;

      SoundPtrMax = 64;

      TextCnt = first_line_w_text - 3;
      line_has_text  = 0;

      ptr = G_PatternTable[PatternNo];

      Status    &= ~TVSystemMask;
      Status    &= ~Field2Mask;       // This is start of field 1
      StatusPort = Status;
      }
    else
      {  
      if (++LineCnt >= 625)   // new field...
        {  
        LineCnt = 0;
        TextCnt = first_line_w_text; // so we have to count down lines down to one with text   
        }
      if (!--TextCnt)         // this counts down lines with or without text
        {                     // App. 5-6 us
        if (line_has_text)    // if this is the last line with text...
          {  
          line_has_text  = 0;  
          TextCnt        = 297;  // number of lines without text
          Status        &= 0xFE; // S1S0 = X0: S0 = 0;
          }
        else
          {                      // ...last line without text...
          line_has_text  = 1;
          TextCnt        = 16;   // number of lines with text
          if (TextEnable)        // If text on selected..  
            Status |= 0x01;      // S1S0 = X1: S0 = 1;
          }
        StatusPort = Status;
        }
      if (!--SoundCnt)
        {                           // App. 11us
        if (SoundPtr > SoundPtrMax)
          {  
          SoundPtr = 0;
          if (SoundPtrMax == 63)
            {  
            SoundPtrMax = 64;
            Status &= ~Field2Mask;  // This is start of field 1
            }
          else
            {  
            SoundPtrMax = 63;
            Status     |= Field2Mask;   // This is start of field 2
            }
          }
        SoundCnt = Sound625[SoundPtr++];// Get sound information
        Status &= 0x3F;                 // Clear previous sound sample
        if (SoundEnable)                // If embedded audio on..
          Status  |= (SoundCnt & 0xC0); //  set sound samples
        SoundCnt  &= 0x0F;              // Lines to next sound update
        StatusPort = Status;            // Update status port
        }
      }
    }
  else
    {                          // ..TV System is NTSC
  /* BEGIN */
    if ((LineCnt == 10) || (LineCnt == (10 + ODD_FIELD_OFFSET_M)))  // once per field....
      {                                                  // line 10 and 273..
      if (TextMovementDir == DOWN)
        {
        first_line_w_text++;
        TextCnt++;
        if (first_line_w_text>= 100)
          TextMovementDir = UP;
        }
      else    //Up movement.
        {
        first_line_w_text--;
        TextCnt--;
        if (first_line_w_text <= 48)
          TextMovementDir = DOWN;
        }
      }
  /* END */
    if (TF1)
      {                        // If delayed field interrupt..
      TF1 = 0;                 //  setup for line 7, (app. 12us)
      TF0 = 0;                 // Clear possible LINE CNT intr.
      if (FrameCnt > 9)
        {  
        FrameCnt      = 1;
        SoundFrameCnt = 1;
        }
      if (PhaseOffset >= 1443)
        TextStatus |= 0x02;            //  correct 1 line offset
      else
        TextStatus  &= 0xFD;
      TextStatusPort = TextStatus;
      OffsetPortL    = (UC) PhaseOffset;
      OffsetPortH    = (UC) (PhaseOffset >> 8);

      asm("mov.b  0451H, #HIGH(-12)"); // load Timer0
      asm("mov.b  0450H, #LOW(-12)");

      LineCnt     = 6;
      SoundCnt    = 2;
      SoundPtr    = 3;
      SoundPtrMax = 47;
      TextCnt     = first_line_w_text - 7;
      line_has_text      = 0;

      ptr = M_PatternTable[PatternNo];

      Status    |= TVSystemMask;
      Status    &= ~Field2Mask;          // This is start of field 1
      StatusPort = Status;
      }
    else
      {  
      if (++LineCnt >= 525)
        {  
        FrameCnt++;
        LineCnt = 0;
        TextCnt = first_line_w_text;
        if (++SoundFrameCnt > 5)
          SoundFrameCnt = 1;
        }
      if (!--TextCnt)
        {                              // App. 5-6 us
        if (line_has_text)
          {  
          line_has_text  = 0;  
          TextCnt        = 247;
          Status        &= 0xFE;
          }
        else
          {  
          line_has_text  = 1;
          TextCnt        = 16;
          if (TextEnable)              // If text on selected..  
            Status |= 0x01;            //  update status byte
          }
        StatusPort = Status;           // Update status port
        }
      if (!--SoundCnt)
        {              // App. 11us
        if (SoundPtr > SoundPtrMax)
          {  
          SoundPtr = 0;
          if (SoundPtrMax == 46)
            {  
            SoundPtrMax = 47;
            Status &= ~Field2Mask;      // This is start of field 1
            }
          else
            {  
            SoundPtrMax = 46;
            Status |= Field2Mask;       // This is start of field 2
            }
          }
        SoundCnt = Sound525[SoundPtr];  // Get sound information
        if (SoundPtr++ == 47)           // Prepare for line 263
          if (SoundFrameCnt & 0x01)     // Sound frames 1,3,5 are special
            SoundCnt |= 0x80;
        Status &= 0x3F;                 // Clear previous sound sample
        if (SoundEnable)
          Status  |= (SoundCnt & 0xC0); // Sound samples
        SoundCnt  &= 0x0F;              // Lines to next sound update
        StatusPort = Status;            // Update status port
        }
      }
    }  // NTSC
  if (TF0)
    {                                   // Update in pattern, app. 20us
    TF0       = 0;
    temp      = ptr->LineType;
    LinePortL = temp;
    LinePortH = temp >> 8;
    temp      = ptr->NextIntrIn;
    TL0       = temp;
    TH0       = temp >> 8;
    ptr       = ptr->NextPattern;
    }
  }

/***************************************************************************/
/*  FieldIntr                                                    D1GENER.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    970217, KEn                                                 */
/*                                                                         */
/*  Function:                                                              */
/*  Remarks:    DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY        */
/*  Returns:                                                               */
/*  Updates:    --                                                         */
/***************************************************************************/
interrupt void FieldIntr(void)
  {  
  asm("mov.b  0453H, _LineOffset+1");    // TH1 = LineOffset >> 8;
  asm("mov.b  0452H, _LineOffset");      // TL1 = LineOffset;
  }

/***************************************************************************/
/*  main                                                         D1GENER.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    990308, KEn                                                 */
/*                                                                         */
/*  Function:                                                              */
/*  Remarks:                                                               */
/*  Returns:                                                               */
/*  Updates:    --                                                         */
/***************************************************************************/
void main(void)
  {  
  
  UI i;
  UC TmpVers, TmpInfo;
                     // System set-up
  BCR = 0x02;        // Set 20 bit BUS, 8 bit DATA
  PSWH = 0x8F;       // Set system priority high

  BTRH = 0x54;       // EXT. BUS timing, Data Cycle, (was 0xae) 
  BTRL = 0x45;       // EXT. BUS timing, Wr+ALE-pulses+Code, (was 0xce) 
                     // NOTE!! When altering these parameters, the timing
                     //  for the IIC routines are NO LONGER VALID

  PSWL = 0;          // Clear flags
  SCR = 0;           // Timer speed prescaler = 4 (0 gives 4)
  PSWH &= 0xF0;      // Set system priority low

                     // Serial set-up
  S0CON = 0xF2;      // Transmit mode 3,  9 bit UART,  Address mode, Rec.enable
  S0ADDR = MyAddress;// Receive address
  S0ADEN = 0xFF;     // Mask for receive address ie. only MyAddress is allowed

  S0M2 = 1;          // Prepare for receiving My Address
  RI0 = TI0 = 0;     // Clear possible hardware interrupts

  T2MOD = 0x01;      // Timer 2 count up only
  T2CAPH = -1;       // Set reload values
  T2CAPL = -3;
  T2CON |= 0x30;     // Use T2 for V24_0
  TH2 = -1;          // Set for max. Baud
  TL2 = -3;          //
  TR2 = 1;           // Start V24-timer
  ERI0 = ETI0 = 1;   // Enable both receive and transmit interrupt

  TMOD = 0x55;       // Timer 0 & 1 as 16-bit counters with external trigger
  ET0 = 0;           // Disable timer0 intrpt
  TR0 = 1;           // Start counter 0

  ET1 = 0;           // Disable timer1 intrpt
  TR1 = 1;           // Start counter 1

                // Interrupt settings
  IT0 = 1;           // Edge trig on ext0
  IT1 = 1;           // Edge trig on ext1

  EX0 = 1;           // Enable ext0, ie. line interrupt
  EX1 = 1;           // Enable ext1, ie. field interrupt

// NOTE! IPAx PRIORITY-SETTINGS MUST MATCH THE SETTING IN ROM_VECTOR() !!!
// (Only use 9-F, 0 or 8 are intrpt off!)
  IPA0 = 0x0D;       // Priority Timer0/EX0 
  IPA1 = 0x0E;       // Priority Timer1/EX1 
  IPA4 = 0xCC;       // Priority PTI0/PRI0

  RS232Init();       // Initialize RS232 variables

  LineOffset = -31;

  PageSelect = 0;

  TextStatusPort = 1;      // Text status = 1, when resetting video RAM

  for (i = 0; i < 256; i++)
    TextPort[i] = 0;      // Initialize video RAM memory

  TextStatus     = 0;
  TextStatusPort = TextStatus;

  OffsetPortL = 0;
  OffsetPortH = 0;

  EDHInsertion = OFF;

  ErrorStatus = 0;              // Reset error status

  TVSystem  = PALG;             // Set PAL-G as default
  CalOffset = 0;                // Set calibration timing to 0

  TextEnable = SoundEnable = OFF;

  asm("mov.b  0451H, #HIGH(-19)"); // load Timer0
  asm("mov.b  0450H, #LOW(-19)");

  LineCnt = 2;
  TextCnt = first_line_w_text - 3;
  line_has_text  = 0;

  SoundCnt = 2;
  SoundPtr = 2;

  SoundPtrMax = 64;

  PatternNo = SDIBlack;
  ptr       = &G_Black[1];

  LinePortL =  ptr->LineType;
  LinePortH = (ptr->LineType) >> 8;

  Status     = 0;
  StatusPort = Status;

  FrameCnt = SoundFrameCnt = 1;

  EA = 1;                       // Enable all interrupts

  IIC_Read(EEPROM_IIC_Adr);     // Dummy IIC read to initialize

                                // Get calibration for PAL-G
  CalOffset = IIC_RdOffset(EEPROM_IIC_Adr, IIC_G_Address);

  if (IICErrorCode)
    {                           // If any errors in IIC..
    ErrorStatus |= IICErrorCode;
    CalOffset = 0;              //  reset calibration
    }

  GeneratorRunning = FALSE;     // Default generator status

#if 0 // This condition is to allow operation of uncalibrated unit
//#define PT_HWType    0x00 // Type                                      - in comm.h
//#define PT_HWVersion 0x79 // Generator 900 version, eg. 901, 902 etc.  - in comm.h
//#define HWType       03   // Hardware version: 03=PT8603               - in vers.h
//#define HWVersion    10   // 900 version: 0=Normal, 1,2,.. = 901,902,..- in vers.h

//WriteEndOfTx( WriteValue((UL) 256*HWVersion + HWType));

  IIC_Write(EEPROM_IIC_Adr, PT_HWType);          //0x00
  if (IIC_Read(EEPROM_IIC_Adr) == HWType)        //03
    {  
    IIC_Write(EEPROM_IIC_Adr, PT_HWVersion);     // 0x79
    if (IIC_Read(EEPROM_IIC_Adr) == HWVersion)   // 10
      GeneratorRunning = TRUE;
    }
#else
  GeneratorRunning = TRUE;
#endif

  while(TRUE)
    if (TestInputBuffer())
      CharHandler();
  }
