/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT8632/33 SDI Test Pattern Generator 1/2                   */
/*  Module:    D1GENER.C                                                   */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980119                                                     */
/*  Rev. date:  990319 KEn, DEV                                            */
/*  Status:    Version 2.2                                                 */
/*                                                                         */
/*  This module contains the following functions:                          */
/*                                                                         */
/*    banked interrupt void LineIntr(void)                                 */
/*    interrupt void FieldIntr(void)                                       */
/*    interrupt void FieldCleanup(void)                                    */
/*    main()                                                               */
/*                                                                         */
/*  Important:                                                             */
/*  These files will compile to both the PT8632 and PT8633. In the file:   */
/*  VERSION.H a compiler definition bit MUST be cleared or set according   */
/*  to the version to be compiled                                          */
/*                                                                         */
/*  Changes:                                                               */
/* 99xxxx: Released in SW ver. 3.1                                         */
/* 990319: New YPosxxLogo for all complex PAL patterns in PT8633.         */
/* 990304: Update according to make 2 versions of the PT8632, namely  one  */
/*         with a Philips pattern and one with a FuBK pattern.             */
/* 981109: Increased array of PatternType & DynamicPatternType from 25 to  */
/*         29. Start of OSDText[3][21] moved to 0xB23A. Length of LogoCnt */
/*         in Philips4:3 PAL is now different in PT8632 and PT8633.        */
/* 981002: Released in SW ver. 3.0                                         */
/* 980603: Released in SW ver. 2.0                                         */
/* 021031: Modified for moving TDC logo                                    */
/*         Modification for Moving Logo and Click synchronized together    */
/*         Logo data is now moved to extra 32k x 2 memory                  */

/***************************************************************************/
/*
    Moving Logo and Click synchronized together in PT8633 for TDC.
    
    1. Movement (Moving Logo) is only possible when Philips 4x3
       picture in PAL is selected.  Movement is disabled for 
       other pictures in UpdatePattern().
    
    2. "Stereo BBC 1 kHz" embedded audio signal no longer exists. BBC bit
       in TextStatusPort is now used to signal synchro-click or 
       ordinary click.  Synchro-click is only on Philips 4x3 PAl.  In v63
       PLD BBC bit switches between synchro-click (new) and old click.
       BBC bit = 1: synchro-click
       BBC bit = 0: ordinary click
    
    3. "-15 dBFS" signal level no longer exists.  Logo data occupies
       program PROM space previously allocated to hold "-15 dBFS"
       level sound data
       code UC AudioPage4[65535] @ 0x40000;
       The combine.c program loads logo data @ 0x40000.
    
    4. When Philips 4x3 in PAL is selected together with embedded
       audio signal of which has a click, there is a click in 
       left audio channel occuring when left edge of moving logo
       passes left edge of vertical red color bar.
*/
/***************************************************************************/

#include <xa.h>
#include <intrpt.h>
#include <string.h>

#include "version.h"

#include "d1gener.h"
#include "d1patt.h"
#include "d1tables.h"
#include "d1util.h"
#include "rs232.h"
#include "comm.h"
#include "d1cmd.h"

// ******************************************************************
// DEFINITION OF HARDWARE PORTS AND DUAL PORT RAM
// ******************************************************************
volatile UC StatusPort               @ 0x8000;  // LD.STATUS

volatile UC LinePortL                @ 0xA000;  // LNR_LSB
volatile UC LinePortH                @ 0xA800;  // LNR_MSB

volatile UC OffsetPortL              @ 0x8800;  // LD.LSBPH
volatile UC OffsetPortH              @ 0x9000;  // LD.MSBPH

volatile UC TextStatusPort           @ 0xC000;

volatile UC TextPort[512]            @ 0xB000;  // LDTXTRAM

volatile UC LowAudioRAM[11520]       @ 0x0200;  // cs_sound_ram_lo
volatile UC LowClickAudioRAM[11520]  @ 0x4200;

volatile UC HighAudioRAM[11520]      @ 0x8200;  // cs_sound_ram_hi
volatile UC HighClickAudioRAM[11520] @ 0xC200;

/********************************************************************/
/*           Modification for Moving Logo and Click                 */
/*       Logo data is now moved to extra 32k x 2 memory             */
/********************************************************************/

far volatile UC LowLogoRAM[22528]   @ 0x12800; // cs_logo_ram_lo 0x10000 - 0x17FFF
far volatile UC HighLogoRAM[22528]  @ 0x1A800; // cs_logo_ram_hi 0x18000 - 0x1FFFF

bit PageSelect                       @ 0x38C;   // P1.4


// *****************************************************************
// DEFINITION OF VARIABLES
// ******************************************************************
near UC MyAddress;          // 

near UL CalOffset;          // Calibration offset
near UL UserOffset;         // User offset
near UL GenlockOffset;      // Genlock offset

near UI PhaseOffset;        // Calculated timing offset, ie. < 64us
near UI LineOffset = -31;   // Calculated line offset

near UC Status;             // Status byte

near UC PatternNo = SDIBlack;  // Pattern number
near UC PatternAttr = 0;       // Pattern attribute, (ONLY Philips & FuBK)

bit UpdatingPatternType;       // 1: Pattern type is being updated

UI PatternType[29] @ 0xB200;   // Reserve 29 integers for temp Dynamic Patterns
near UI DynamicPattern[29];    // Reserve 29 integers for Dynamic Patterns

bit EDHInsertion;              // 1: EDH insertion ON
bit TVSystem;                  // 0: SDI525,  1: SDI625

near UC AudioSignal;           // Embedded audio signal selection
near UC AudioLevel;            // Embedded audio level selection
near UC AudioGroup;            // Embedded audio group selection

near UC TextStyle;             // Text style information
near UC ClockStyle;            // Clock style information

near UC ErrorStatus;           // Global error status

char OSDText[3][21] @ 0xB23A;  // Reserve 63 Bytes for each OSD text line
char DateText[10] @ 0xB2B0;    // Reserve 10 Bytes for date information
char TimeText[10] @ 0xB2BA;    // Reserve 10 Bytes for clock information

                               // NOTE! The Dual port RAM is also used in
                               //  RS232.C for the RxBuffer[];

bit GeneratorRunning;          // Security bit

near UC NFrameCnt;             // Field/8 or field/4 counter for 0.1Hz BL/WH
near UC FrameCnt = 1;          // Frame counter
near int LineCnt = 2;          // Line counter
near UI LineAddrCnt;           // Line address counter

near UC TextStatus;            // Variable to hold group-/click-/line-offset 

bit AudioEnable;               // 1: Audio ON

bit TextLN1Enable;          // 1: Text line 1 ON
bit TextLN2Enable;          // 1: Text line 2 ON
bit TextLN3Enable;          // 1: Text line 3 ON

bit ClockEnable;            // 1: Clok information ON
bit LogoEnable;            // 1: moving logo is on, 0: no moving logo

near UC XPosLine1;          // Line 1 X-position
near int YPosLine1 = 20;    // Line 1 Y-position. Actual start is +1
near UC XPosLine2;          // Line 2 X-position
near int YPosLine2 = 40;    // Line 2 Y-position. Actual start is +1
near UC XPosLine3;          // Line 3 X-position.
near int YPosLine3 = 60;    // Line 3 Y-position. Actual start is +1
                            // Start of logo. Actual start is +1
near int YPosLogo = YPosPhil4x3GLogo;

near UC XPosTime;            // Start of time position, horizontal
near UC XPosDate;            // Start of date position, horizontal

near int AudioPtrMax = 64;   // Maximum number of entries in audio table
near int AudioPtr = 2;       // Pointer to entry in audio table
near UC AudioFrameCnt = 1;   // Audio sequence frame counter, (SDI525 only)
near UC AudioCnt = 2;        // Counter to next change in audio pattern

near int Line1Cnt = 17;      // Counter used for text line 1 insertion
near int Line2Cnt = 37;      // Counter used for text line 2 insertion
near int Line3Cnt = 57;      // Counter used for text line 3 insertion
                             // Counter used for logo line insertion
near int LogoCnt = YPosPhil4x3GLogo-3; // YPosPhil4x3GLogo = 240

bit Line1Enable;          // 1: Display line 1
bit Line2Enable;          // 1: Display line 2
bit Line3Enable;          // 1: Display line 3

bit Line1On;              // Set high in text line 1
bit Line2On;              // Set high in text line 2
bit Line3On;              // Set high in text line 3
bit LogoOn;              // Set high in when line contains moving logo

code struct PATTERN_ITEM* near ptr = &G_Black[1];

/***************************************************************************/

ROM_VECTOR(IV_EX1, FieldIntr, IV_SYSTEM+IV_PRI14)
ROM_VECTOR(IV_EX0, LineIntr,  IV_SYSTEM+IV_PRI13+IV_BANK1)

ROM_VECTOR(IV_RI0, RS232Intr, IV_SYSTEM+IV_PRI12+IV_BANK2)
ROM_VECTOR(IV_TI0, RS232Intr, IV_SYSTEM+IV_PRI12+IV_BANK2)

ROM_VECTOR(IV_SWI4, FieldCleanup, IV_SYSTEM+IV_PRI04)

/***************************************************************************/
/*  LineIntr                                                     D1GENER.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980119                                    */
/* Revised:    990319, KEn                                                 */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY        */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
banked interrupt void LineIntr(void)
  {
  UI temp;

  if (TVSystem == SDI625)
    {                             // If TV system is SDI625..
    if (TF1)
      {                           // If delayed field interrupt..
      TF1 = 0;                    //  setup for line 3, (app. 12us)
      TF0 = 0;                    // Clear possible LINE CNT intr.
      if (PhaseOffset >= 1443)    // Readout BEFORE PhaseOffset to
        TextStatus |= 0x02;       //  correct 1 line offset
      else
        TextStatus &= 0xFD;
      TextStatusPort = TextStatus;
      OffsetPortL = (UC)  PhaseOffset;
      OffsetPortH = (UC) (PhaseOffset >> 8);

      asm("mov.b  0451H, #HIGH(-19)");
      asm("mov.b  0450H, #LOW(-19)");

      LineCnt     = 2;
      AudioCnt    = 2;
      AudioPtr    = 2;
      AudioPtrMax = 64;
      Line1Cnt    = YPosLine1 - 3;
      Line2Cnt    = YPosLine2 - 3;
      Line3Cnt    = YPosLine3 - 3;
      LogoCnt    = YPosLogo - 3; 
      Line1On     = 0;
      Line2On     = 0;
      Line3On     = 0;
      LogoOn     = 0;
      Status     &= ~TVSystemMask;   // syssel = 0, i.e. PAL
      Status     &= ~Field2Mask;     // This is start of field 1
      StatusPort  = Status;

#if PT8633Version
      if (NFrameCnt++ > 62)          // 62*f/8 = 9.92
        NFrameCnt = 1;
      if (PatternNo == SDIAltBlWh)
        {
        if (NFrameCnt > 31) 
          ptr = G_PatternTable[SDIFlat100];
        else
          ptr = G_PatternTable[SDIBlack];
        }
      else
        ptr = G_PatternTable[PatternNo];  // Select pattern to display
#else
      ptr = G_PatternTable[PatternNo];    // Select pattern to display
#endif
      SWR4 = TRUE;                        // Generate SW1 interrupt
      }
    else
      {
      if (++LineCnt >= 625)
        {
        LineCnt = 0;
        Line1Cnt = YPosLine1;
        Line2Cnt = YPosLine2;
        Line3Cnt = YPosLine3;
        LogoCnt = YPosLogo; 
        }
      if (!--Line1Cnt)
        {            // Text line 1, app. 5-6 us
        if (Line1On)
          {            // Ramadr. 0x000-0x0FF
          Line1Cnt = 297;
          Status &= 0xFE;
          }
        else
          {
          Line1Cnt = 16;
          if (Line1Enable)          // If text on selected..  
            Status |= 0x01;        //  update status byte
          }
        Line1On ^= 1;              // Toggle line bit
        StatusPort = Status;          // Update status port
        }
      if (!--Line2Cnt)
        {            // Text line 2, app. 5-6 us
        if (Line2On)
          {            // Ramadr. 0x100-0x17F
          Line2Cnt = 297;
          Status &= 0xFD;
          }
        else
          {
          Line2Cnt = 16;
          if (Line2Enable)          // If text on selected..  
            Status |= 0x02;        //  update status byte
          }
        Line2On ^= 1;              // Toggle line bit
        StatusPort = Status;          // Update status port
        }
      if (!--Line3Cnt)
        {            // Text line 3, app. 5-6 us
        if (Line3On)
          {            // Ramadr. 0x180-0x1FF
          Line3Cnt = 297;
          Status &= 0xFC;
          }
        else
          {
          Line3Cnt = 16;
          if (Line3Enable)        // If text on selected..  
            Status |= 0x03;        //  update status byte
          }
        Line3On ^= 1;              // Toggle line bit
        StatusPort = Status;       // Update status port
        }
      if (!--LogoCnt)
         {            // App. 5-6 us
        if (LogoOn)
          {
#if PT8633Version
          LogoCnt = 293; // 294 orig JK change
#else
          LogoCnt = 292;
#endif
          if (PatternNo > SDIPhilips16x9)
            LogoCnt += 2;
          Status &= 0xFB;
          }
        else
          {       // if Logo is OFF
#if PT8633Version
          LogoCnt = 20;  // orig 19 JK (MOVEBIT is hi for 20 line syncs)
#else
          LogoCnt = 21;
#endif
          if (PatternNo > SDIPhilips16x9)
            LogoCnt -= 2;
          if (LogoEnable)         // If logo on selected..  
            Status |= 0x04;        //  update status byte
          }
        LogoOn ^= 1;              // Toggle logo bit
        StatusPort = Status;       // Update status port
        }
      if (!--AudioCnt)
        {            // App. 11us
        if (AudioPtr > AudioPtrMax)
          {
          AudioPtr = 0;
          if (AudioPtrMax == 63)
            {
            AudioPtrMax = 64;
            Status &= ~Field2Mask;    // This is start of field 1
            }
          else
            {
            AudioPtrMax = 63;
            Status |= Field2Mask;    // This is start of field 2
            }
          }
        AudioCnt = Audio625[AudioPtr++];  // Get audio information
        Status &= 0x3F;            // Clear previous audio sample
        if (AudioEnable)           // If embedded audio on..
          Status |= (AudioCnt & 0xC0);  //  set audio samples
        AudioCnt &= 0x0F;            // Lines to next audio update
        StatusPort = Status;          // Update status port
        }
      }
    }
  else
    {                        // ..TV System is SDI525
    if (TF1)
      {                    // If delayed field interrupt..
      TF1 = 0;                    //  setup for line 7, (app. 12us)
      TF0 = 0;                    // Clear possible LINE CNT intr.
      if (FrameCnt > 9)
        {
        FrameCnt = 1;
        AudioFrameCnt = 1;
        }
      if (PhaseOffset >= 1443)
        TextStatus |= 0x02;          //  correct 1 line offset
      else
        TextStatus &= 0xFD;
      TextStatusPort = TextStatus;
      OffsetPortL = (UC) PhaseOffset;
      OffsetPortH = (UC) (PhaseOffset >> 8);

      asm("mov.b  0451H, #HIGH(-12)");
      asm("mov.b  0450H, #LOW(-12)");

      LineCnt = 6;
      AudioCnt = 2;
      AudioPtr = 3;
      AudioPtrMax = 47;
      Line1Cnt = YPosLine1 - 7;
      Line2Cnt = YPosLine2 - 7;
      Line3Cnt = YPosLine3 - 7;
      LogoCnt = YPosLogo - 7; 
      Line1On = Line2On = Line3On = LogoOn = 0;
      Status |= TVSystemMask;         // syslel = 1, i.e. NTSC
      Status &= ~Field2Mask;          // This is start of field 1
      StatusPort = Status;            // Update status port

#if PT8633Version
      if (NFrameCnt++ > 150)          // 150*f/4 = 10.0000000
        NFrameCnt = 1;
      if (PatternNo == SDIAltBlWh)
        { 
        if (NFrameCnt > 75)
          ptr = M_PatternTable[SDIFlat100];
        else
          ptr = M_PatternTable[SDIBlack];
        }
      else
        ptr = M_PatternTable[PatternNo];  // Select pattern to display
#else
      ptr = M_PatternTable[PatternNo];    // Select pattern to display
#endif
      SWR4 = TRUE;                // Generate SW1 interrupt
      }                           // if (TF1)
    else 
      {
      if (++LineCnt >= 525)
        {
        FrameCnt++;
        if (++AudioFrameCnt > 5)
          AudioFrameCnt = 1;
        LineCnt = 0;
        Line1Cnt = YPosLine1;
        Line2Cnt = YPosLine2;
        Line3Cnt = YPosLine3;
        LogoCnt = YPosLogo;
        }
      if (!--Line1Cnt)
        {            // Text line 1, app. 5-6 us
        if (Line1On)
          {            // Ramadr. 0x000-0x0FF
          Line1Cnt = 246;
          Status &= 0xFE;
          }
        else
          {
          Line1Cnt = 16;
          if (Line1Enable)          // If text on selected..  
            Status |= 0x01;        //  update status byte
          }
        Line1On ^= 1;              // Toggle line bit
        StatusPort = Status;          // Update status port
        }
      if (!--Line2Cnt)
        {            // Text line 2, app. 5-6 us
        if (Line2On)
          {            // Ramadr. 0x100-0x17F
          Line2Cnt = 246;
          Status &= 0xFD;
          }
        else
          {
          Line2Cnt = 16;
          if (Line2Enable)          // If text on selected..  
            Status |= 0x02;        //  update status byte
          }
        Line2On ^= 1;              // Toggle line bit
        StatusPort = Status;          // Update status port
        }
      if (!--Line3Cnt)
        {            // Text line 3, app. 5-6 us
        if (Line3On)
          {            // Ramadr. 0x180-0x1FF
          Line3Cnt = 246;
          Status &= 0xFC;
          }
        else
          {
          Line3Cnt = 16;
          if (Line3Enable)          // If text on selected..  
            Status |= 0x03;        //  update status byte
          }
        Line3On ^= 1;              // Toggle line bit
        StatusPort = Status;          // Update status port
        }
      if (!--LogoCnt)
        {            // App. 5-6 us
        if (LogoOn)
          {
          LogoCnt = 244;
          Status &= 0xFB;
          }
        else
          {
          LogoCnt = 18;
          if (LogoEnable)          // If logo on selected..  
            Status |= 0x04;        //  update status byte
          }
        LogoOn ^= 1;              // Toggle logo bit
        StatusPort = Status;          // Update status port
        }
      if (!--AudioCnt)
        {            // App. 11us
        if (AudioPtr > AudioPtrMax)
          {
          AudioPtr = 0;
          if (AudioPtrMax == 46)
            {
            AudioPtrMax = 47;
            Status &= ~Field2Mask;    // This is start of field 1
            }
          else
            {
            AudioPtrMax = 46;
            Status |= Field2Mask;    // This is start of field 2
            }
          }
        AudioCnt = Audio525[AudioPtr];  // Get audio information
        if (AudioPtr++ == 47)        // Prepare for line 263
          if (AudioFrameCnt & 0x01)    // Audio frames 1,3,5 are special
            AudioCnt |= 0x80;
        Status &= 0x3F;            // Clear previousa audio sample
        if (AudioEnable)
          Status |= (AudioCnt & 0xC0);  // Audio samples
        AudioCnt &= 0x0F;            // Lines to next audio update
        StatusPort = Status;          // Update status port
        }
      }
    }       // ..TV System is SDI525
  if (TF0)
    {                      // Update in pattern, wc. 25us
    TF0 = 0;
    if (temp = ptr->LineAddrStart) // If countertype pattern..
      {
      if (temp > 30)
        LineAddrCnt = temp;
      else
        LineAddrCnt = DynamicPattern[temp];
      LinePortL = LineAddrCnt;
      LinePortH = LineAddrCnt >> 8;      // Write Linetype
      }
    else
      {
      LineAddrCnt = 0;
      temp = ptr->LineType;          // Get next linetype
      LinePortL = temp;
      LinePortH = temp >> 8;          // Write Linetype
      }
    temp = ptr->NextIntrIn;            // Get no of lines to next intr.
    TL0 = temp;
    TH0 = temp >> 8;                // Load counter with no of lines
    ptr = ptr->NextPattern;            // Pointer to next address
    }
  else
    {
    if (LineAddrCnt)
      {
      LineAddrCnt += 2;
      LinePortL = LineAddrCnt;
      LinePortH = LineAddrCnt >> 8;      // Write Linetype
      }
    }
  }

/***************************************************************************/
/*  FieldIntr                                     D1GENER.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
interrupt void FieldIntr(void) {

  asm("mov.b  0453H, _LineOffset+1");    // TH1 = LineOffset >> 8;
  asm("mov.b  0452H, _LineOffset");      // TL1 = LineOffset;
}

/***************************************************************************/
/*  FieldCleanup                                   D1GENER.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980319                         */
/* Revised:    981109, KEn                                     */
/*                                                  */
/*  Function:  Download selected Philips/FuBK pattern to memory.        */
/*  Remarks:    DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY.      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
interrupt void FieldCleanup(void)
  {
  register int i;

  if (!UpdatingPatternType)
    if ((PatternNo >= SDIPhilips4x3) || (PatternNo <= SDIFuBK16x9))
      for (i = 0; i < 29; i++)
        DynamicPattern[i] = PatternType[i];
  SWR4 = FALSE;                           // Clear SW interrupt 4
  }

/***************************************************************************/
/*  main                                         D1GENER.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                         */
/* Revised:    990304, KEn                                     */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void main(void) 
  {
  UI i;
                // System set-up
  BCR  = 0x02;        // Set 20 bit BUS, 8 bit DATA
  PSWH = 0x8F;      // Set system priority high

  BTRH = 0x54;      // EXT. BUS timing, Data Cycle, (was 0xae) 
  BTRL = 0x45;      // EXT. BUS timing, Wr+ALE-pulses+Code, (was 0xce) 
                // NOTE!! When altering these parameters, the timing
                //  for the IIC routines are NO LONGER VALID

  PSWL = 0;        // Clear flags
  SCR  = 0;          // Timer speed prescaler = 4 (0 gives 4)
  PSWH &= 0xF0;       // Set system priority low

                // Serial set-up
  S0CON = 0xF2;      // Transmit mode 3,  9 bit UART,  Address mode, Rec.enable
                //
  MyAddress = DefAddr;

  S0ADDR = MyAddress;    // Receive address
  S0ADEN = DefAddrEnable;  // Mask for receive address.

  S0M2 = 1;        // Prepare for receiving My Address
  RI0  = TI0 = 0;      // Clear possible hardware interrupts

  T2MOD  = 0x01;      // Timer 2 count up only
  T2CAPH = -1;      // Set reload values
  T2CAPL = -3;
  T2CON |= 0x30;      // Use T2 for V24_0
  TH2  = -1;        // Set for max. Baud
  TL2  = -3;        //
  TR2  = 1;          // Start V24-timer
  ERI0 = ETI0 = 1;    // Enable both receive and transmit interrupt

  SWR4 = 0;        // Clear possible SW interrupt 4
  SWE |= 0x08;      // Enable SW interrupt 4

  TMOD = 0x55;      // Timer 0 & 1 as 16-bit counters with external trigger
  ET0  = 0;          // Disable timer0 intrpt
  TR0  = 1;          // Start counter 0

  ET1 = 0;          // Disable timer1 intrpt
  TR1 = 1;          // Start counter 1

                // Interrupt settings
  IT0 = 1;          // Edge trig on ext0
  IT1 = 1;          // Edge trig on ext1

  EX0 = 1;          // Enable ext0, ie. line interrupt
  EX1 = 1;          // Enable ext1, ie. field interrupt

// NOTE! IPAx PRIORITY-SETTINGS MUST MATCH THE SETTING IN ROM_VECTOR() !!!
// (Only use 9-F, 0 or 8 are intrpt off!)
  IPA0 = 0x0D;          // Priority Timer0/EX0 
  IPA1 = 0x0E;          // Priority Timer1/EX1 
  IPA4 = 0xCC;          // Priority PTI0/PRI0

  RS232Init();          // Initialize RS232 variables

  PageSelect = 0;

  TextStatusPort = 1;      // Text status = 1, resetting LOWER video RAM

  for (i = 0; i < 256; i++)
    TextPort[i] = 0;      // Initialize video RAM memory

  TextStatusPort = 0;      // Text status = 0, resetting UPPER video RAM

  for (i = 256; i < 512; i++)
    TextPort[i] = 0;      // Initialize video RAM memory

  TextStatus = 0;
  TextStatusPort = TextStatus;

  Status = 0;
  StatusPort = Status;

  TVSystem = SDI625;
  
  OffsetPortL = 0;
  OffsetPortH = 0;

  asm("mov.b  0451H, #HIGH(-19)");
  asm("mov.b  0450H, #LOW(-19)");

  LinePortL = ptr->LineType;
  LinePortH = (ptr->LineType) >> 8;

  OSDText[0][0] = 0;              // Reset text line variables
  OSDText[1][0] = 0;
  OSDText[2][0] = 0;

  strcpy(TimeText, "00#00#00");      // # equals short colon
  strcpy(DateText, "00$00$00");      // $ equals short hyphen

  EA = 1;                    // Enable all interrupts

  IIC_Read(EEPROM_IIC_Adr);        // Dummy IIC read to initialize

                          // Get calibration for PAL-G
  CalOffset = IIC_RdOffset(EEPROM_IIC_Adr, IIC_G_Address);

  if (IICErrorCode)
    {            // If any errors in IIC..
    ErrorStatus |= IICErrorCode;
    CalOffset = 0;              //  reset calibration
    }

  GeneratorRunning = FALSE;        // Default generator status

  IIC_Write(EEPROM_IIC_Adr, PT_HWType);
  if (IIC_Read(EEPROM_IIC_Adr) == HWType)
    {
    IIC_Write(EEPROM_IIC_Adr, PT_HWVersion);
    if (IIC_Read(EEPROM_IIC_Adr) == HWVersion)
      GeneratorRunning = TRUE;
    }
  //CopyLogoRAM(); // we have separate RAM for logo now, so we copy 
                  // only once (this never worked OK)
  while(TRUE)
    {
    if (TestInputBuffer())
      CharHandler();
#if 0
    TextStatus    |= 0x08;       // BBC = 1 JK
    TextStatusPort = TextStatus; // JK addition, to update BBC bit
#endif
    }
  }
