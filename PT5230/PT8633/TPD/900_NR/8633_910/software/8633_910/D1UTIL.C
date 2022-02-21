/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT8632/33 SDI Test Pattern Generator 1/2                   */
/*  Module:    D1UTIL.C                                                    */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980119                                                     */
/*  Rev. date:  990323, KEn, DEV                                           */
/*  Status:    Version 2.3                                                 */
/*                                                                         */
/*  This module contains the following functions:                          */
/*                                                                         */
/*    UC stringlen(char* textptr)                                          */
/*    UC CenterTextString(UC center, char* string)                         */
/*    void ClearOSDLine(UC lineno)                                         */
/*    void InsertOSDText(UC lineno, UC xpos, char* textptr)                */
/*    void InsertOSDTime()                                                 */
/*    void InsertOSDDate()                                                 */
/*    void WriteOSDText(UC lineno)                                         */
/*    void UpdateTextEnable()                                              */
/*    void UpdateTextStyle()                                               */
/*    void UpdatePattern()                                                 */
/*    void CopyLogoRAM()                                                  */
/*    void CopyAudioRAM()                                                  */
/*    void Wait10ms()                                                      */
/*                                                                         */
/* Changes:                                                                */
/* 990323: PT8633: New motion marker insertion for Philips/FuBK patterns   */
/* 990304: FuBK4x3 is now available in PT8632                              */
/* 981109: PT8633: New insertion in Philips4x3(PAL) to support new audio.  */
/* 981002: Released as SW ver. 3.0                                         */
/* 980929: Parking of lines in UpdatetextStyle() implemented.              */
/* 980603: Released as SW ver. 2.0                                         */
/* 021031: Modified for moving TDC logo.  The BBC signal, which was used   */
/*         to implement the BBC click is now used to indicate that click   */
/*         has to be synchronized with logo movement.  It is used in PLD   */
/*         V63 code.                                                       */
/* 29.04.03: function InsertOSDText() modified to avoid loading text data  */
/*           outside active video area.                                    */
/***************************************************************************/

#include <string.h>
#include <xa.h>

#include "version.h"

#include "d1gener.h"
#include "d1util.h"
#include "d1tables.h"

//***************************************************************************
//  VARIABLES USED IN InsertOSDText(), InsertOSDTime() & InsertOSDDate()
//***************************************************************************
static char* textPtr;

static near UC tmpChar;
static near UC noSamples;
static near UC charAddr;
static near UI RAMaddr;
static near UI ndx;

/***************************************************************************/
/*  stringlen                                          D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980309                         */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Return the length of a string in samples.              */
/*  Remarks:    --                                        */
/*  Returns:    The string length in samples.                    */
/*  Updates:    --                                        */
/***************************************************************************/
UC stringlen(char* textptr)
  {
  UC no = 0;
  UC tmp;

  while (tmp = *textptr++)      // Get no of samples for each character
    no += TextTable[ 2 * (tmp - 32)];

  return(no);
  }

/***************************************************************************/
/*  CenterTextString                                      D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 971120                         */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Center a textstring around a user specified X coordinate.  */
/*  Remarks:    --                                        */
/*  Returns:    X coordinate to start the centered text string.          */
/*  Updates:    --                                        */
/***************************************************************************/
UC CenterTextString(UC center, char* TextPtr)
  {
  return(center - (stringlen(TextPtr) >> 1));
  }

/***************************************************************************/
/*  ClearOSDLine                                                 D1UTIL.C  */
/*                                                                         */
/* Author:     Kim Engedahl, DEV, 980309                                   */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  Clear complete line in video RAM.                           */
/*  Remarks:   If the line to clear is currently being displayed OR if     */
/*             there is LESS than 40 lines until BEGINNING displaying the  */
/*             line, WAIT until the line has been displayed, before        */
/*             clearing.                                                   */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void ClearOSDLine(UC lineno)
  {
  register UI i;
    
  switch (lineno)
    {
    case 1:
      if (Line1On)
        {                      // If line 1 is being displayed..
        while (Line1On)        //  wait to clear the line until AFTER
          ;                    //  the line has been displayed.
        }
      else
        {
        if (Line1Cnt < 40)
          {                    // If LT 40 lines to displaing line 1..
          while (!Line1On)     //  wait until the line is BEING displayed
            ;
          while (Line1On)      //  and wait to clear the line until
            ;                  //  AFTER the line has been displayed.
          }
        }
      break;
    case 2:
      if (Line2On)
        {                      // If line 2 is being displayed..
        while (Line2On)        //  wait to clear the line until AFTER
          ;                    //  the line has been displayed.
        }
      else
        {
        if (Line2Cnt < 40)
          {                    // If LT 40 lines to displaing line 2..
          while (!Line2On)     //  wait until the line is BEING displayed
            ;
          while (Line2On)      //  and wait to clear the line until
            ;                  //  AFTER the line has been displayed.
          }
        }
      break;
    case 3:
      if (Line3On)
        {                      // If line 3 is being displayed..
        while (Line3On)        //  wait to clear the line until AFTER
          ;                    //  the line has been displayed.
        }
      else
        {
        if (Line3Cnt < 40)
          {                    // If LT 40 lines to displaing line 3..
          while (!Line3On)     //  wait until the line is BEING displayed
            ;          
          while (Line3On)      //  and wait to clear the line until
            ;                  //  AFTER the line has been displayed.
          }
        }
    break;
    } // switch()

  i = 0x80 * lineno;           // Start position in video RAM to delete 
  noSamples = 128;

  while(noSamples--)
    TextPort[i++] = 0;
  }

/***************************************************************************/
/*  InsertOSDText                                                D1UTIL.C  */
/*                                                                         */
/* Author:     Kim Engedahl, DEV, 980309                                   */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  Insert string into video RAM.                               */
/*  Remarks:   This function should ONLY be called RIGHT after a call to   */
/*             ClearOSDLine.                                               */
/*  Returns:    --                                                         */
/*  Updates:   29.04.2003: Boundaries in video RAM (V59) introduced, in    */
/*             order to prevent text data being placed in inactive video   */
/*             part of SDI signal, where it disturbed sound data and EAV   */
/*             codes.  The idea is to stop loading RAM if text data fall   */
/*             into outside active video area.  This happened when text    */
/*             was positioned too much to the right.  Two boundary values  */
/*             are introduced, for line 1 and 2 respectively, they were    */
/*             determined by trial and error.                              */
/***************************************************************************/
#define MAX_LINE_1 220
#define MAX_LINE_2 348

void InsertOSDText(UC lineno, UC xpos, char* textptr)
  {
  UI RAMaddrMax;

  RAMaddr = 0x80 * lineno + xpos;   // Start position in video RAM to insert
  if (lineno == 1)
    RAMaddrMax = MAX_LINE_1; // 218
  else
    RAMaddrMax = MAX_LINE_2; // 346

  while (tmpChar = *textptr++)
    {
    ndx = 2 * (tmpChar - 32);

    noSamples = TextTable[ndx++];   // Get no of samples for character (is 1 or 2)
    charAddr  = TextTable[ndx];     // Get 1st character address

    while (noSamples--)
      {
      TextPort[RAMaddr++] = charAddr++;
      if (RAMaddr > RAMaddrMax) // 172
        return;
      }
    }
  }

/***************************************************************************/
/*  InsertOSDTime                                   D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980309                          */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Update clock insertion in video text RAM.              */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void InsertOSDTime() {

  RAMaddr = 0x80*3 + XPosTime;    // Start position in video RAM to insert

  textPtr = TimeText;          // Point to start of time text

  if (Line3On) {            // If line 3 is being displayed..
    while (Line3On)          //  wait to clear the line until AFTER
      ;                  //  the line has been displayed.
  }
  else {
    if (Line3Cnt < 20) {      // If LT 20 lines to displaing line 3..

      while (!Line3On)        //  wait until the line is BEING displayed
        ;          
      while (Line3On)        //  and wait to clear the line until
        ;                //  AFTER the line has been displayed.
    }
  }

  while (tmpChar = *textPtr++) {

    if (tmpChar == '#')          //  Get no of samples for character
      noSamples = 1;
    else
      noSamples = 2;
                          // Get 1st character address
    charAddr = TextTable[(UC)2*tmpChar-63];

    while (noSamples--)
      TextPort[RAMaddr++] = charAddr++;
  }
}

/***************************************************************************/
/*  InsertOSDDate                                   D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980519                          */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Update clock insertion in video text RAM.              */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void InsertOSDDate() {

  RAMaddr = 0x80*3 + XPosDate;    // Start position in video RAM to insert

  textPtr = DateText;          // Point to start of date text

  if (Line3On) {            // If line 3 is being displayed..
    while (Line3On)          //  wait to clear the line until AFTER
      ;                  //  the line has been displayed.
  }
  else {
    if (Line3Cnt < 20) {      // If LT 20 lines to displaing line 3..

      while (!Line3On)        //  wait until the line is BEING displayed
        ;          
      while (Line3On)        //  and wait to clear the line until
        ;                //  AFTER the line has been displayed.
    }
  }

  while (tmpChar = *textPtr++) {

    if (tmpChar == '$')        //  Get no of samples for character
      noSamples = 1;
    else
      noSamples = 2;
                        // Get 1st character address
    charAddr = TextTable[(UC) 2*tmpChar-63];

    while (noSamples--)
      TextPort[RAMaddr++] = charAddr++;
  }
}

/***************************************************************************/
/*  WriteOSDText                                   D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980309                          */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Update text insertion in video text RAM.              */
/*  Remarks:    FuBK patterns in complex text style is special.          */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void WriteOSDText(UC lineno)
  {
  if ((TextStyle == ComplexTextStyle) &&
     ((PatternNo == SDIFuBK4x3) || (PatternNo == SDIFuBK16x9)))
    {
    if (PatternNo == SDIFuBK4x3)
      {
      XPosLine1 = CenterTextString(42, OSDText[0]);
      XPosLine2 = CenterTextString(58, OSDText[1]);
      }
    else
      {
      XPosLine1 = CenterTextString(44, OSDText[0]);
      XPosLine2 = CenterTextString(56, OSDText[1]);
      }
    ClearOSDLine(1);                    // Only line 1 is used

    if (TextLN1Enable)
      InsertOSDText(1, XPosLine1, OSDText[0]);

    if (TextLN2Enable)
      InsertOSDText(1, XPosLine2, OSDText[1]);
    }
  else
    {
    switch (lineno)
      {
      case 1:
        if (TextStyle == ComplexTextStyle)
          XPosLine1 = CenterTextString(50, OSDText[0]);

        ClearOSDLine(1);
        InsertOSDText(1, XPosLine1, OSDText[0]);
        break;
      case 2:
        if (TextStyle == ComplexTextStyle)
          XPosLine2 = CenterTextString(50, OSDText[1]);

        ClearOSDLine(2);
        InsertOSDText(2, XPosLine2, OSDText[1]);
        break;
      case 3:
        if (!ClockEnable)
          {
          ClearOSDLine(3);
          InsertOSDText(3, XPosLine3, OSDText[2]);
          }
        break;
      }
    }
  }

/***************************************************************************/
/*  UpdateTextEnable                                     D1UTIL.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980227                         */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Enable/disable the textlines in the pattern            */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void UpdateTextEnable() {

  switch (TextStyle) {
    case FreeTextStyle:
      Line1Enable = TextLN1Enable;
      Line2Enable = TextLN2Enable;
      Line3Enable = TextLN3Enable | ClockEnable;
      break;

    case StandardTextStyle:
      switch (PatternNo) {
        case SDIPhilips4x3:
        case SDIPhilips16x9:
          Line1Enable = TextLN1Enable;
          Line2Enable = TextLN2Enable;
          Line3Enable = ClockEnable;
          break;

        case SDIFuBK4x3:
        case SDIFuBK16x9:
          Line1Enable = TextLN1Enable & !ClockEnable;
          Line2Enable = TextLN2Enable;
          Line3Enable = ClockEnable;
          break;

        default:
          Line1Enable = TextLN1Enable;
          Line2Enable = TextLN2Enable;
          Line3Enable = TextLN3Enable | ClockEnable;
          break;
      }
      break;

    case ComplexTextStyle:
      switch (PatternNo) {
        case SDIPhilips4x3:
        case SDIPhilips16x9:
          Line1Enable = TextLN1Enable;
                              // Text in lower text field is NOT
                              //  possible with motion enabled
          Line2Enable = TextLN2Enable & (LogoEnable == OFF);
          Line3Enable = ClockEnable;
          break;

        case SDIFuBK4x3:
        case SDIFuBK16x9:
          WriteOSDText(1);
                              // Text in text fields is NOT
                              //  possible with motion enabled
          Line1Enable = (TextLN1Enable | TextLN2Enable) & (LogoEnable == OFF);
          Line2Enable = 0;
          Line3Enable = ClockEnable;
          break;
      }
      break;
  }
}

/***************************************************************************/
/*  UpdateTextStyle                                     D1UTIL.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980227                         */
/* Revised:    980929, KEn                                     */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void UpdateTextStyle() {

  switch (TextStyle) {
    case StandardTextStyle:
      switch (PatternNo) {
        case SDIPhilips4x3:
        case SDIPhilips16x9:
          if (TVSystem == SDI625) {
            YPosLine1 = 268;
            YPosLine2 = 286;
          }
          else {
            YPosLine1 = 222;
            YPosLine2 = 240;
          }

          XPosLine1 = 92 - stringlen(OSDText[0]);
          XPosLine2 = 92 - stringlen(OSDText[1]);

          WriteOSDText(1);
          WriteOSDText(2);
          break;

        case SDIFuBK4x3:
        case SDIFuBK16x9:
          YPosLine1 = 268;
          YPosLine2 = 286;

          XPosLine1 = 92 - stringlen(OSDText[0]);
          XPosLine2 = 92 - stringlen(OSDText[1]);

          if (!ClockEnable)
            WriteOSDText(1);

          WriteOSDText(2);
          break;

        default:
          if (TVSystem == SDI625) {
            YPosLine1 = 250;
            YPosLine2 = 268;
            YPosLine3 = 286;
          }
          else {
            YPosLine1 = 204;
            YPosLine2 = 222;
            YPosLine3 = 240;
          }

          XPosLine1 = 92 - stringlen(OSDText[0]);
          XPosLine2 = 92 - stringlen(OSDText[1]);
          XPosLine3 = 92 - stringlen(OSDText[2]);

          WriteOSDText(1);
          WriteOSDText(2);

          if (!ClockEnable)
            WriteOSDText(3);
          break;
      }
      break;
  
    case ComplexTextStyle:
      switch (PatternNo) {
        case SDIPhilips4x3:
        case SDIPhilips16x9:
          if (TVSystem == SDI625) {
            YPosLine1 = 53;
            YPosLine2 = 242;
          }
          else {
            YPosLine1 = 49;
            YPosLine2 = 202;
          }

          WriteOSDText(1);
          WriteOSDText(2);
          break;

        case SDIFuBK4x3:
        case SDIFuBK16x9:          // ONLY available in SDI625
          YPosLine1 = 168;
          YPosLine2 = 30;        // Parking of line 2

          WriteOSDText(1);
          break;
      }
      break;
  }

  switch (PatternNo) {
    case SDIPhilips4x3:
      if (TVSystem == SDI625) {
        XPosDate = 25;
        XPosTime = 59;

        YPosLine3 = 158;
      }
      else {
        XPosDate = 26;
        XPosTime = 58;

        YPosLine3 = 134;
      }

      DateText[2] = DateText[5] = '-';
      TimeText[2] = TimeText[5] = ':';
      break;

    case SDIPhilips16x9:
      if (TVSystem == SDI625) {
        XPosDate = 31;
        XPosTime = 54;

        YPosLine3 = 158;
      }
      else {
        XPosDate = 32;
        XPosTime = 54;

        YPosLine3 = 134;
      }
      DateText[2] = DateText[5] = '$';
      TimeText[2] = TimeText[5] = '#';
      break;

    case SDIFuBK4x3:
      XPosDate = 15;
      XPosTime = 70;

      YPosLine3 = 268;

      DateText[2] = DateText[5] = '$';
      TimeText[2] = TimeText[5] = '#';
      break;

    case SDIFuBK16x9:
      XPosDate = 20;
      XPosTime = 65;

      YPosLine3 = 268;

      DateText[2] = DateText[5] = '$';
      TimeText[2] = TimeText[5] = '#';
      break;

    default:
      DateText[2] = DateText[5] = '-';
      TimeText[2] = TimeText[5] = ':';

      XPosDate = 8;
      XPosTime = 92 - stringlen(TimeText);
      break;
  }

  if (ClockEnable) {

    ClearOSDLine(3);

    if ((ClockStyle == TextClockTime) ||
       (ClockStyle == TextClockTimeDate))
      InsertOSDTime();

    if ((ClockStyle == TextClockDate) ||
       (ClockStyle == TextClockTimeDate))
      InsertOSDDate();
  }

  UpdateTextEnable();
}

/***************************************************************************/
/*  UpdatePattern                                       D1UTIL.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980313                         */
/* Revised:    990324, KEn                                     */
/*                                                  */
/*  Function:  Update pattern output.                          */
/*  Remarks:    For Philips/FuBK: download pattern according to attributes  */
/*          to temporary memory.                            */
/*          Update OSD text                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void UpdatePattern(UC pattern)
  {
  UpdatingPatternType = TRUE;

#if PT8633Version
  //************************************************************************
  // ADVANCED PATTERN DEFINITIONS FOR THE PT8633 GENERATOR
  //************************************************************************
  switch (pattern)
    {
    case SDIPhilips4x3:
      if (TVSystem == SDI625)
        {
        if (!(PatternAttr & 0x01))
          {    // If Anti-PAL  OFF...
          PatternType[1] = PatternType[13] = 1382;
          PatternType[4] = PatternType[16] = 1732;
          PatternType[6] = PatternType[18] = 1858;

          PatternType[7] = PatternType[19] = 1383;
          PatternType[10] = PatternType[22] = 1733;
          PatternType[12] = PatternType[24] = 1859;

          if (!(PatternAttr & 0x08))
            {                                 // If motion is OFF...
            PatternType[25] = PatternType[27] = 1816;
            PatternType[26] = PatternType[28] = 1817;
            }
          else
            {
            PatternType[25] = PatternType[27] = 1816;    // background same as in
            PatternType[26] = PatternType[28] = 1817;    // motion OFF : JK
//          PatternType[25] = PatternType[27] = 6796;  //
//          PatternType[26] = PatternType[28] = 6797;
            }

          if (!(PatternAttr & 0x02)) {  // If PLUGE OFF..
            PatternType[3] = PatternType[15] = 6268;
            PatternType[9] = PatternType[21] = 6269;
          }
          else {                  // ..else PLUGE ON
            PatternType[3] = PatternType[15] = 1690;
            PatternType[9] = PatternType[21] = 1691;
          }

          if (!(PatternAttr & 0x04)) {  // If GreyScale10 OFF..
            PatternType[5] = PatternType[17] = 1774;
            PatternType[11] = PatternType[23] = 1775;
          }
          else {                  // ..else Greyscale10 ON
            PatternType[5] = PatternType[17] = 4678;
            PatternType[11] = PatternType[23] = 4679;
          }

          switch (ClockStyle) {
            case TextClockNone:
              PatternType[2] = PatternType[14] = 1650;
              PatternType[8] = PatternType[20] = 1651;
              break;

            case TextClockTime:
              PatternType[2] = PatternType[14] = 6500;
              PatternType[8] = PatternType[20] = 6501;
              break;

            case TextClockTimeDate:
              PatternType[2] = PatternType[14] = 4636;
              PatternType[8] = PatternType[20] = 4637;
              break;
          }
        }
        else
          {                          // ..else Anti-PAL ON
          PatternType[1] = 230;
          PatternType[4] = 580;
          PatternType[6] = 706;

          PatternType[7] = 231;
          PatternType[10] = 581;
          PatternType[12] = 707;

          PatternType[13] = 806;
          PatternType[16] = 1156;
          PatternType[18] = 1282;

          PatternType[19] = 807;
          PatternType[22] = 1157;
          PatternType[24] = 1283;

          if (!(PatternAttr & 0x08))
            {                          // If motion is OFF...
            PatternType[25] = 664;
            PatternType[26] = 665;

            PatternType[27] = 1240;
            PatternType[28] = 1241;
            }
          else
            {
            PatternType[25] = 664;
            PatternType[26] = 665;

            PatternType[27] = 1240;
            PatternType[28] = 1241;


//            PatternType[25] = 6838;  // JK same pattern as for OFF
//            PatternType[26] = 6839;

//            PatternType[27] = 6880;
//            PatternType[28] = 6881;
            }

          if (!(PatternAttr & 0x04)) {  // If GreyScale10 OFF..
            PatternType[5] = 622;
            PatternType[11] = 623;
            PatternType[17] = 1198;
            PatternType[23] = 1199;
          }
          else {                  // ..else Greyscale10 ON
            PatternType[5] = 4720;
            PatternType[11] = 4721;
            PatternType[17] = 4762;
            PatternType[23] = 4763;
          }

          if (!(PatternAttr & 0x02)) {  // If PLUGE OFF..
            PatternType[3] = 6184;
            PatternType[9] = 6185;
            PatternType[15] = 6226;
            PatternType[21] = 6227;
          }
          else {                  // ..else PLUGE ON
            PatternType[3] = 538;
            PatternType[9] = 539;
            PatternType[15] = 1114;
            PatternType[21] = 1115;
          }

          switch (ClockStyle) {
            case TextClockNone:
              PatternType[2] = 498;
              PatternType[8] = 499;
              PatternType[14] = 1074;
              PatternType[20] = 1075;
              break;

            case TextClockTime:
              PatternType[2] = 6420;
              PatternType[8] = 6421;
              PatternType[14] = 6460;
              PatternType[20] = 6461;
              break;

            case TextClockTimeDate:
              PatternType[2] = 4556;
              PatternType[8] = 4557;
              PatternType[14] = 4596;
              PatternType[20] = 4597;
              break;
          }
        }

        YPosLogo = YPosPhil4x3GLogo;
        if (!(PatternAttr & 0x08))     // If motion is OFF...
          LogoEnable = OFF;
        else
          {                            // ..else motion is ON
          if (!LogoEnable)
            CopyLogoRAM();
          TextStatus &= 0xBF;          // Clear 16x9 size of motion window
          LogoEnable = ON;
          }
        }
      else
        {                            // ..else TV system is 525
        if (!(PatternAttr & 0x02))
          {                          // If PLUGE OFF..
          PatternType[2] = 6352;
          PatternType[5] = 6353;
          }
        else
          {                          // ..else PLUGE is ON
          PatternType[2] = 5066;
          PatternType[5] = 5065;
          }

        if (!(PatternAttr & 0x04))
          {                          // If GreyScale10 is OFF..
          PatternType[3] = 5134;
          PatternType[6] = 5133;
          }
        else
          {                          // ..else GreyScale10 is ON
          PatternType[3] = 5812;
          PatternType[6] = 5813;
          }

        switch (ClockStyle)
          {
          case TextClockNone:
            PatternType[1] = 5034;
            PatternType[4] = 5033;
            break;

          case TextClockTime:
            PatternType[1] = 6580;
            PatternType[4] = 6581;
            break;

          case TextClockTimeDate:
            PatternType[1] = 5780;
            PatternType[4] = 5781;
            break;
          }
        YPosLogo = YPosPhilMLogo;
        // In 525, there is no logo...........................................
        LogoEnable  = OFF;
        TextStatus &= 0xBF;          // Clear 16x9 size of motion window
        }                            // ...TV system is 525
      break;

    case SDIPhilips16x9:
      if (TVSystem == SDI625)
        {
        if (!(PatternAttr & 0x01))
          {                         // If Anti-PAL is OFF
          PatternType[2] = 3632;
          PatternType[8] = 3633;
          PatternType[13] = 3632;
          PatternType[14] = 3633;
          }
        else
          {                         // ..else Anti-PAL is ON
          PatternType[2] = 4006;
          PatternType[8] = 4007;
          PatternType[13] = 4070;
          PatternType[14] = 4071;
          }

        if (!(PatternAttr & 0x02))
          {                         // If PLUGE is OFF
          PatternType[4] = 6310;
          PatternType[10] = 6311;
          }
        else
          {                         // ..else PLUGE is ON
          PatternType[4] = 3738;
          PatternType[10] = 3739;
          }

        if (!(PatternAttr & 0x10))
          {    // If Circles is OFF..
          PatternType[1] = 4134;        // Upper circle
          PatternType[7] = 4135;        // Upper circle +1
  
          if (!(PatternAttr & 0x04))
            {  // If GreyScale10 is OFF..
            PatternType[5] = 4390;      // GreyScale
            PatternType[11] = 4391;      // GreyScale+1
            }
          else
            {
            PatternType[5] = 4432;      // GreyScale
            PatternType[11] = 4433;      // GreyScale+1
            }

          PatternType[6] = 4320;        // Lower circle
          PatternType[12] = 4321;        // Lower circle+1
          }
        else
          {                    // ..else Circles is ON
          PatternType[1] = 3458;        // Upper circle 
          PatternType[7] = 3459;        // Upper circle +1

          if (!(PatternAttr & 0x04))
            {  // If GreyScale10 is OFF..
            PatternType[5] = 3822;      // GreyScale
            PatternType[11] = 3823;      // GreyScale+1
            }
          else
            {
            PatternType[5] = 4474;      // GreyScale
            PatternType[11] = 4475;      // GreyScale+1
            }

          PatternType[6] = 3906;        // Lower circle
          PatternType[12] = 3907;        // Lower circle+1
          }

        switch (ClockStyle)
          {
          case TextClockNone:
            PatternType[3] = 3698;        // Date/Time
            PatternType[9] = 3699;        // Date/Time+1
            break;

          case TextClockTime:
            PatternType[3] = 6540;        // Date/Time
            PatternType[9] = 6541;        // Date/Time+1
            break;

          case TextClockTimeDate:
            PatternType[3] = 4516;        // Date/Time
            PatternType[9] = 4517;        // Date/Time+1
            break;
          }

        YPosLogo = YPosPhil16x9GLogo;
        // No motion possible.....
        if (!(PatternAttr & 0x10))
          {    // If Circles is OFF..
          PatternType[15] = 4278;        // Lower circle
          PatternType[16] = 4279;        // Lower circle+1
          }
        else
          {
          PatternType[15] = 3864;        // Lower circle
          PatternType[16] = 3865;        // Lower circle+1
          }
        LogoEnable = OFF;
        }
      else
        {                              // ..else TVSystem 525
        if (!(PatternAttr & 0x02))
          {    // If PLUGE is OFF
          PatternType[3] = 6386;
          PatternType[8] = 6387;
          }
        else
          {                    // ..else PLUGE is ON
          PatternType[3] = 5554;
          PatternType[8] = 5553;
          }

        if (!(PatternAttr & 0x10))
          {    // If Circles is OFF..
          PatternType[1] = 5878;        // Upper circle
          PatternType[6] = 5879;        // Upper circle +1

          if (!(PatternAttr & 0x04))
            {  // If GreyScale10 is OFF..
            PatternType[4] = 5992;      // GreyScale
            PatternType[9] = 5993;      // GreyScale+1
            }
          else
            {
            PatternType[4] = 6026;      // GreyScale
            PatternType[9] = 6027;      // GreyScale+1
            }

          PatternType[5] = 6094;        // Lower circle
          PatternType[10] = 6095;        // Lower circle+1
          }
        else
          {                    // ..else Circles is ON
          PatternType[1] = 5328;        // Upper circle 
          PatternType[6] = 5327;        // Upper circle +1

          if (!(PatternAttr & 0x04))
            {  // If GreyScale10 is OFF..
            PatternType[4] = 5622;      // GreyScale
            PatternType[9] = 5621;      // GreyScale+1
            }
          else
            {
            PatternType[4] = 6060;      // GreyScale
            PatternType[9] = 6061;      // GreyScale+1
            }

          PatternType[5] = 5656;        // Lower circle
          PatternType[10] = 5655;        // Lower circle+1
          }

        switch (ClockStyle)
          {
          case TextClockNone:
            PatternType[2] = 5522;        // Date/Time
            PatternType[7] = 5521;        // Date/Time+1
            break;

          case TextClockTime:
            PatternType[2] = 6612;        // Date/Time
            PatternType[7] = 6613;        // Date/Time+1
            break;

          case TextClockTimeDate:
            PatternType[2] = 5846;        // Date/Time
            PatternType[7] = 5847;        // Date/Time+1
            break;
          }
        YPosLogo = YPosPhilMLogo;
        // No motion possible.......
        LogoEnable = OFF;
        }
      break;

    case SDIFuBK4x3:
      if (!(PatternAttr & 0x01)) {  // Anti-PAL ON/OFF
        PatternType[1] = 2364;
        PatternType[2] = 2365;
        PatternType[3] = 2364;
        PatternType[4] = 2365;
      }
      else {
        PatternType[1] = 2534;
        PatternType[2] = 2535;
        PatternType[3] = 2614;
        PatternType[4] = 2615;
      }

      switch (ClockStyle) {
        case TextClockNone:
          PatternType[5] = 2446;        // Date/Time
          PatternType[6] = 2447;        // Date/Time+1
          break;

        case TextClockTime:
          PatternType[5] = 6720;        // Date/Time
          PatternType[6] = 6721;        // Date/Time+1
          break;

        case TextClockTimeDate:
          PatternType[5] = 6644;        // Date/Time
          PatternType[6] = 6645;        // Date/Time+1
          break;
      }
      YPosLogo = YPosFuBKGLogo;
      // No motion.................
      PatternType[7] = 2246;      // Motion
      PatternType[8] = 2247;      // Motion+1
      LogoEnable = OFF;
      break;

    case SDIFuBK16x9:
      if (!(PatternAttr & 0x01)) {  // Anti-PAL ON/OFF
        PatternType[1] = 3100;
        PatternType[2] = 3101;
        PatternType[3] = 3100;
        PatternType[4] = 3101;
      }
      else {
        PatternType[1] = 3270;
        PatternType[2] = 3271;
        PatternType[3] = 3350;
        PatternType[4] = 3351;
      }

      switch (ClockStyle) {
        case TextClockNone:
          PatternType[5] = 3182;        // Date/Time
          PatternType[6] = 3183;        // Date/Time+1
          break;

        case TextClockTime:
          PatternType[5] = 6758;        // Date/Time
          PatternType[6] = 6759;        // Date/Time+1
          break;

        case TextClockTimeDate:
          PatternType[5] = 6682;        // Date/Time
          PatternType[6] = 6683;        // Date/Time+1
          break;
      }

      YPosLogo = YPosFuBKGLogo;
      // No motion.........
      PatternType[7] = 2982;      // Motion
      PatternType[8] = 2983;      // Motion+1
      LogoEnable = OFF;
      break;

    default:
      LogoEnable = OFF;
      break;
  }
#else
  //************************************************************************
  // ADVANCED PATTERN DEFINITIONS FOR THE PT8632 GENERATOR
  //************************************************************************
  switch (pattern) {
    case SDIPhilips4x3:
      if (TVSystem == SDI625) {
        if (!(PatternAttr & 0x01)) {    // If Anti-PAL OFF...
          PatternType[1] = PatternType[13] = 1382;
          PatternType[4] = PatternType[16] = 1732;
          PatternType[6] = PatternType[18] = 1816;

          PatternType[7] = PatternType[19] = 1383;
          PatternType[10] = PatternType[22] = 1733;
          PatternType[12] = PatternType[24] = 1817;

          if (!(PatternAttr & 0x02)) {  // If PLUGE OFF..
            PatternType[3] = PatternType[15] = 2282;
            PatternType[9] = PatternType[21] = 2283;
          }
          else {                  // ..else PLUGE ON
            PatternType[3] = PatternType[15] = 1690;
            PatternType[9] = PatternType[21] = 1691;
          }

          if (!(PatternAttr & 0x04)) {  // If GreyScale10 OFF..
            PatternType[5] = PatternType[17] = 1774;
            PatternType[11] = PatternType[23] = 1775;
          }
          else {                  // ..else Greyscale10 ON
            PatternType[5] = PatternType[17] = 2324;
            PatternType[11] = PatternType[23] = 2325;
          }

          switch (ClockStyle) {
            case TextClockNone:
              PatternType[2] = PatternType[14] = 1650;
              PatternType[8] = PatternType[20] = 1651;
              break;

            case TextClockTime:
              PatternType[2] = PatternType[14] = 2158;
              PatternType[8] = PatternType[20] = 2159;
              break;

            case TextClockTimeDate:
              PatternType[2] = PatternType[14] = 2038;
              PatternType[8] = PatternType[20] = 2039;
              break;
          }
        }
        else {                    // ..else Anti-PAL ON
          PatternType[1] = 230;
          PatternType[4] = 580;
          PatternType[6] = 664;

          PatternType[7] = 231;
          PatternType[10] = 581;
          PatternType[12] = 665;

          PatternType[13] = 806;
          PatternType[16] = 1156;
          PatternType[18] = 1240;

          PatternType[19] = 807;
          PatternType[22] = 1157;
          PatternType[24] = 1241;

          if (!(PatternAttr & 0x04)) {  // If GreyScale10 OFF..
            PatternType[5] = 622;
            PatternType[11] = 623;
            PatternType[17] = 1198;
            PatternType[23] = 1199;
          }
          else {                  // ..else Greyscale10 ON
            PatternType[5] = 2366;
            PatternType[11] = 2367;
            PatternType[17] = 2408;
            PatternType[23] = 2409;
          }

          if (!(PatternAttr & 0x02)) {  // If PLUGE OFF..
            PatternType[3] = 2198;
            PatternType[9] = 2199;
            PatternType[15] = 2240;
            PatternType[21] = 2241;
          }
          else {                  // ..else PLUGE ON
            PatternType[3] = 538;
            PatternType[9] = 539;
            PatternType[15] = 1114;
            PatternType[21] = 1115;
          }

          switch (ClockStyle) {
            case TextClockNone:
              PatternType[2] = 498;
              PatternType[8] = 499;
              PatternType[14] = 1074;
              PatternType[20] = 1075;
              break;

            case TextClockTime:
              PatternType[2] = 2078;
              PatternType[8] = 2079;
              PatternType[14] = 2118;
              PatternType[20] = 2119;
              break;

            case TextClockTimeDate:
              PatternType[2] = 1958;
              PatternType[8] = 1959;
              PatternType[14] = 1998;
              PatternType[20] = 1999;
              break;
          }
        }

        YPosLogo = YPosPhil4x3GLogo;

        if (!(PatternAttr & 0x08))   // If motion is OFF...
          LogoEnable = OFF;
        else
          {                // ..else motion is ON
          if (!LogoEnable)
            CopyLogoRAM();

          TextStatus &= 0xBF;      // Clear 16x9 size of motion window
          LogoEnable = ON;
          }
        }
      break;

    case SDIFuBK4x3:
      if (!(PatternAttr & 0x01)) {  // Anti-PAL ON/OFF
        PatternType[1] = 636;
        PatternType[2] = 637;
        PatternType[3] = 636;
        PatternType[4] = 637;
      }
      else {
        PatternType[1] = 806;
        PatternType[2] = 807;
        PatternType[3] = 886;
        PatternType[4] = 887;
      }

      switch (ClockStyle) {
        case TextClockNone:
          PatternType[5] = 718;        // Date/Time
          PatternType[6] = 719;        // Date/Time+1
          break;

        case TextClockTime:
          PatternType[5] = 1004;        // Date/Time
          PatternType[6] = 1005;        // Date/Time+1
          break;

        case TextClockTimeDate:
          PatternType[5] = 966;        // Date/Time
          PatternType[6] = 967;        // Date/Time+1
          break;
      }

      YPosLogo = YPosFuBKGLogo;

      if (!(PatternAttr & 0x08))     // If motion is OFF...
        LogoEnable = OFF;
      else
        {                  // ..else motion is ON
        if (!LogoEnable)
          CopyLogoRAM();

        TextStatus |= 0x40;        // Set 16x9 size of motion window.
                                   // This is done to synchronize some
                                   //  audio.
        LogoEnable = ON;
        }
      break;

    default:
      LogoEnable = OFF;
      break;
  }
#endif
  UpdatingPatternType = FALSE;
  PatternNo = pattern;
  UpdateTextStyle();              // Function calls UpdateTextEnable()
  //CopyAudioRAM();
  //control_synchro();
  }
#if 0
/* this function controls the BBC signal used to 
   choose synchro-click/ordinary click.
*/
void control_synchro(void)
  {
  switch (PatternNo)
    {
    case SDIPhilips4x3:
    case SDIPhilips4x3woAPAL:
      if (TVSystem == SDI625)
        {
        switch(AudioSignal)
          {
          case AudioStereoEBU1kHz:  // only these 2 have click
          case AudioMonoEBU1kHz:
            if (LogoEnable == ON)
              TextStatus |= 0x0C;   // BBC = 1, EBU = 1
            else
              {
              TextStatus |= 0x04;   // EBU = 1
              TextStatus &= 0xF7;   // BBC = 0;
              }
            break;
          default:
            TextStatus &= 0xF3;   // BBC = 0, EBU = 0
          }
        }
      else
        {
        LogoEnable = OFF;        // no motion in ntsc
        switch(AudioSignal)
          {
          case AudioStereoEBU1kHz:  // only these 2 have click
          case AudioMonoEBU1kHz:
            TextStatus |= 0x04;   // EBU = 1
            TextStatus &= 0xF7;   // BBC = 0
            break;
          default:
            TextStatus &= 0xF3;   // BBC = 0, EBU = 0
          }
        }
      break;
    default:
        LogoEnable = OFF;        // no motion in other patterns
        switch(AudioSignal)
          {
          case AudioStereoEBU1kHz:  // only these 2 have click
          case AudioMonoEBU1kHz:
            TextStatus |= 0x04;   // EBU = 1
            TextStatus &= 0xF7;   // BBC = 0
            break;
          default:
            TextStatus &= 0xF3;   // BBC = 0, EBU = 0
          }
    }
  TextStatusPort = TextStatus; // JK addition, to update BBC bit
  }
#endif
/***************************************************************************/
/*  CopyLogoRAM                                   D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980226                          */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Load pulse information to video text RAM.              */
/*  Remarks:    0xB000 in PROM.                               */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void CopyLogoRAM()
  {
  UI i, j;
	UC lowTmp, highTmp;

	while(!LogoOn)		// Wait with download until right AFTER the 
		;				  				//  logo has been displayed.
	while(LogoOn)
    ;
  if (AudioEnable == ON) // Disable audio so sound is not garbled when
    {                    // loading logo ram
    AudioEnable = OFF;

    for (j = 0; j < 22528; j += 512) // 19456 22528
      for (i = 0; i < 512; i++)
        {
        lowTmp  = LogoDataPageTable[2*j+i];
        highTmp = LogoDataPageTable[2*j+i+512];
  
        EA         = 0;
        PageSelect = 1;              // Select downloading logo RAM
  
        LowLogoRAM[j+i]  = lowTmp;
        HighLogoRAM[j+i] = highTmp;
  
        PageSelect = 0;
        EA         = 1;
        }
    AudioEnable = ON;
    }
  else
    {
    for (j = 0; j < 22528; j += 512)
      for (i = 0; i < 512; i++)
        {
        lowTmp  = LogoDataPageTable[2*j+i];
        highTmp = LogoDataPageTable[2*j+i+512];
  
        EA         = 0;
        PageSelect = 1;              // Select downloading logo RAM
  
        LowLogoRAM[j+i]  = lowTmp;
        HighLogoRAM[j+i] = highTmp;
  
        PageSelect = 0;
        EA         = 1;
        }
    }
  }

#if 0
/* original function */
void CopyLogoRAM() {

  register int i;
  //UC lowTmp, highTmp;
  code far UC* SignalAddr;

  SignalAddr = LogoPageTable;

  while(!LogoOn)     // Wait with download until right AFTER the 
    ;                  //  pulse has been displayed.
  while(LogoOn)
    ;

  for (i = 0; i < 22528; i++)
    {
    //lowTmp  = LogoPageTable[i];
    //highTmp = LogoPageTable[i+2048];

    EA         = 0;              // Disable interrupt
    PageSelect = 1;              // Select downloading audio RAM

    LowLogoRAM[i]  = SignalAddr[i];  
    HighLogoRAM[i] = SignalAddr[i + 22528];

    PageSelect = 0;
    EA         = 1;              // Allow possible interrupt
    }
  }
#endif

/***************************************************************************/
/*  CopyAudioRAM                                   D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970325                          */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Download audio information to audio RAM.              */
/*  Remarks:    Setting audio group is now allowed in PT8632.          */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void CopyAudioRAM()
  {
  static bit TmpAudioEnable;
  static bit TmpLogoEnable;

  code far UC* SilentAddr;
  code far UC* SignalAddr;

  register UI i, j, ndx;
  UC lowTmp, highTmp;

  TmpAudioEnable = AudioEnable;
  TmpLogoEnable = LogoEnable;

  AudioEnable = OFF;
  LogoEnable = OFF;
  while (Status & 0xC4)            // Wait for audio OFF/pulse OFF
    ;
                            // Silence STEREO
  SilentAddr = AudioPageTable[AudioSilence];

  if (AudioSignal > AudioStereoEBU1kHz) // JK: BBC was here
    {
    if (AudioSignal < AudioDual)
      SilentAddr += 0x1200;        // Silence MONO
    else 
      SilentAddr += 0x2400;        // Silence DUAL SOUND
    }

  TextStatus &= 0xC3;              // Reset group/click bits

#if PT8633Version
  TextStatus |= (AudioGroup << 4);    // Insert audio group
#endif

  if (AudioLevel == AudioSilence)
    {
    for (ndx = j = 0; j < 11520; j += 2304)
      for (i = 0; i < 2304; i++, ndx++)
        {
        lowTmp = SilentAddr[i];      // Get audio data
        highTmp = SilentAddr[i+2304];

        EA = 0;                // Disable interrupt
        PageSelect = 1;          // Select downloading audio RAM

        LowAudioRAM[ndx] = lowTmp;    // Download audio data
        HighAudioRAM[ndx] = highTmp;

        PageSelect = 0;
        EA = 1;
        }
    }
   else
    {
    SignalAddr = AudioPageTable[AudioLevel] + AudioOffsetTable[AudioSignal];

		switch( AudioSignal)
      {					// TextStatus bit 2,3 used for CLICK
			case AudioStereoEBU1kHz:
			case AudioMonoEBU1kHz:
				TextStatus |= 0x04;
				break;
#if 0
			case AudioStereoBBC1kHz:
				TextStatus |= 0x08;
				break;
#endif
			case AudioMonoBBC1kHz:				// No click bit in AudioMonoEBU1kHz
				break;
		  }

		TextStatusPort = TextStatus;

    switch(AudioSignal)
      {
      case AudioStereo1kHz:        // Alt. audio/silence in click area
      case AudioStereoEBU1kHz:      // Alt. audio/silence in click area
//      case AudioStereoBBC1kHz:      // Silence in ALL click area
      case AudioMonoEBU1kHz:        // Silence in ALL click area
      case AudioMonoBBC1kHz:        // There's no click, ie. don't care
        for (ndx = j = 0; j < 11520; j += 2304)
          for (i = 0; i < 2304; i++, ndx++)
            {
            lowTmp = SignalAddr[i];
            highTmp = SignalAddr[i+2304];
            
            EA = 0;
            PageSelect = 1;      // Select downloading sound RAM

            LowAudioRAM[ndx] = lowTmp;
            HighAudioRAM[ndx] = highTmp;

            PageSelect = 0;
            EA = 1;            // Allow possible interrupt

            if (((i % 6) < 3) /*|| (AudioSignal == AudioStereoBBC1kHz)*/ || // JK
                (AudioSignal == AudioMonoEBU1kHz))
              {
              lowTmp = SilentAddr[i];
              highTmp = SilentAddr[i+2304];
              }
            else
              {
              lowTmp = SignalAddr[i];
              highTmp = SignalAddr[i+2304];
              }

            EA = 0;
            PageSelect = 1;      // Select downloading sound RAM

            LowClickAudioRAM[ndx] = lowTmp;
            HighClickAudioRAM[ndx] = highTmp;

            PageSelect = 0;
            EA = 1;            // Allow possible interrupt
          }
        break;

      case AudioStereo800Hz:
      case AudioDual:
        for (i = 0; i < 11520; i++)
          {
          lowTmp = SignalAddr[i];
          highTmp = SignalAddr[i+11520];

          EA = 0;
          PageSelect = 1;          // Select downloading sound RAM

          LowAudioRAM[i] = lowTmp;
          HighAudioRAM[i] = highTmp;

          PageSelect = 0;
          EA = 1;
          }
        break;
      }
    }

  AudioEnable = TmpAudioEnable;
  LogoEnable = TmpLogoEnable;
  //control_synchro(); // set BBC signal accordingly
  }

/***************************************************************************/
/*  Wait10ms                                       D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970428                          */
/* Revised:    980601, KEn                                     */
/*                                                  */
/*  Function:  Wait 10 ms                                  */
/*  Remarks:    Line interrupt is used for counting -> system dependant    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void Wait10ms() {

  register UI j;
  register UC cnt;

  j = LineCnt;          // Delay for 10ms, while NVRAM store bytes
  cnt = 0;

  while (cnt < 160)      
    if (j - LineCnt) {
      j = LineCnt;
      cnt++;
    }
}
