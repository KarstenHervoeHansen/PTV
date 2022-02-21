/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:    UTIL.C                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  960628                                    */
/*  Rev. date:  991025, KEn, DEV                              */
/*  Status:    Version 1.2                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*    void NONE()                                        */
/*    UC HexToByte(char s)                                */
/*    UC htol(char* s, UL* result)                            */
/*    UC otol(char* s, UL* result)                            */
/*    UC btol(char* s, UL* result)                            */
/*    void _ultoa(UL val, char* result)                        */
/*    int Cstrlen(code char * s)                                         */
/*    char* Cstrcat(register char* to, register code char* from)        */
/*    void* Cmemcpy(void* d1, code void* s1, register size_t n)        */
/*    char* Cstrcpy(char* to, register code char * from)            */
/*    int Cstrcmp(register const char * s1, register code char * s2)    */
/*    int Cstrncmp(register const char * s1, register code char * s2)    */
/*    char GetNextTextASCII(UC HWtype, UC direction, char ASCIIChar)    */
/*    char GetNextPresetASCII(UC direction, char ASCIIChar)          */
/*    void TimeFormat(UC format, UL TimeInf, UC TCIFormat, char* TimeTxt)  */
/*    UC ValidTime(UL TimeInf)                              */
/*    UL AdjustTime(UC ndx, UC dir, UC format, UL TimeInf)            */
/*    void DateFormat(UC format, UL DateInf, UC TCIFormat, char* DateTxt)  */
/*    UC ValidDate(UL DateInf)                              */
/*    UL AdjustDate(UC ndx, UC step, UC format, UL DateInf)          */
/*    void TimeOffsetFormat(UL time, char* TimeTxt)                */
/*    UL AdjustTimeOffset(UC ndx, UC dir, UL TimeInf)              */
/*    bit ROMCheckOK()                                    */
/*    bit PowerUpRAMCheckOK()                                */
/*    bit DiagnoseRAMCheckOK()                              */
/*    bit RS232CheckOK(UC Baudrate)                          */
/*                                                                         */
/* Changes:                                                                */
/* 991025: In _ultoa() i&j was defined as UC, should have been char      */
/* 980724: Added routines for TimeOffset                        */
/* 980721: Updated TimeFormat() & DateFormat() for Time Clock Interface    */
/* 980616: Released as 4008 002 06812                          */
/* 980514: Released as 4008 002 06811                          */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                        */

#include <stdio.h>
#include <string.h>
#include <xa.h>

#include <ctype.h>

#include "mas.h"
#include "util.h"
#include "iic.h"
#include "tables.h"
#include "text.h"
#include "xaexprt.h"

#define SizeOfPROMCode 0x50000  // No of bytest used in PROM
code char CSumAddr @   0x4FFFE; // Start of 16-bit PROM checksum

UC StartOfRAM @ 0x200;          // First available external RAM location

code char Page0 @ 0x00000;      // Code space page 0
code char Page1 @ 0x10000;      // Code space page 1
code char Page2 @ 0x20000;      // Code space page 2
code char Page3 @ 0x30000;      // Code space page 3
code char Page4 @ 0x40000;      // Code space page 4
code char Page5 @ 0x50000;      // Code space page 5
code char Page6 @ 0x60000;      // Code space page 6
code char Page7 @ 0x70000;      // Code space page 7

static char tbuff[60];

/***************************************************************************/
/*  NONE                                                           UTIL.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 960828                                    */
/* Revised:    970105                                                      */
/*                                                                         */
/*  Function:  DOES ABSOLUTELY NOTHING!!!                                  */
/*  Remarks:    This one is primarily used in menu tables                  */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void NONE()
  {
  }

/***************************************************************************/
/*  HexToByte                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Converts a char defined in hex to binary              */
/*  Remarks:    --                                        */
/*  Returns:    A byte from 0-15                              */
/*  Updates:    --                                        */
/***************************************************************************/
UC HexToByte(char s) {

  toupper(s);

  if ((s >= 'A') && (s <= 'F'))
    return(s - 'A' + 10);

  return(s - '0');
}

/***************************************************************************/
/*  htol                                           UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Converts a string containg a hex number to an unsigned long  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
UC htol(char* s, UL* result) {

  *result = 0;

  while (*s)
    *result = 16*(*result) + HexToByte(*s++);

  return(0);
}

/***************************************************************************/
/*  otol                                           UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Converts a string containg an octal number to unsinged long  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
UC otol(char* s, UL* result) {

  *result = 0;

  while (*s)
    *result = 8*(*result) + (*s++ - '0');

  return(0);
}

/***************************************************************************/
/*  btol                                           UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Converts a string containg a binary number to unsigned long  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
UC btol(char* s, UL* result) {

  *result = 0;

  while (*s)
    *result = 2*(*result) + (*s++ - '0');

  return(0);
}

/***************************************************************************/
/*  _ultoa                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    991025                                       */
/*                                                  */
/*  Function:  Converts an unsigned long to a string                */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void _ultoa(UL val, char* result) {

  UL tmp = val;
  char i=0, j=0;
  UC TmpChar;

  if (val == 0)
    strcpy(result, "0");
  else {
    while (val) {
      tmp = val / 10;
      result[i++] = (val % 10) + '0';
      val = tmp;
    }
    result[i--] = '\0';

    while (i >= j) {              // Reverse ASCII string
      TmpChar = result[i];
      result[i] = result[j];
      result[j] = TmpChar;
      i--;
      j++;
    }
  }
}

/***************************************************************************/
/*  Cstrlen                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Calculates the length of a string from code memory        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
size_t Cstrlen(register code char* s) {

  register code char *  cp;

  cp = s;
  while(*cp++)
    continue;

  return(cp-s-1);
}

/***************************************************************************/
/*  Cstrcat                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Concatenate a string from code memory to data memory      */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
char* Cstrcat(register char* to, register code char* from) {

  register char* cp;

  cp = to;
  while(*cp)
    cp++;

  while(*cp++ = *from++)
    continue;

  return(to);
}

/***************************************************************************/
/*  Cmemcpy                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Copy a memory-area from code-memory to RAM-memory        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void* Cmemcpy(void* d1, code void* s1, register size_t n) {

  register char* d;
  register code char* s;

  s = s1;
  d = d1;

  while(n--)
    *d++ = *s++;

  return(d1);
}

/***************************************************************************/
/*  Cstrcpy                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 971003                          */
/* Revised:    971003                                       */
/*                                                  */
/*  Function:  Copy a string in code-memory to a string in RAM-memory    */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
char* Cstrcpy(char* to, register code char * from) {

  register char*  cp;

  cp = to;
  while(*cp++ = *from++)
    continue;

  return(to);
}

/***************************************************************************/
/*  Cstrcmp                                                        UTIL.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 960828                                    */
/* Revised:    970105                                                      */
/*                                                                         */
/*  Function:  Compare a string in code-memory to a string in RAM-memory   */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
int Cstrcmp(register const char * s1, register code char * s2)
  {
  register signed char  r;

  while(!(r = *s1 - *s2++) && *s1++)
    continue;

  return(r);
  }

/***************************************************************************/
/*  Cstrncmp                                         UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:  Compare a string in code-memory to a string in RAM-memory  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
int Cstrncmp(register const char * s1, register code char * s2, size_t len) {

  while(len--) {
    if (*s1 == 0 || *s1 != *s2)
      return(*s1 - *s2);

    s1++;
    s2++;
  }

  return(0);
}

/***************************************************************************/
/*  GetNextTextASCII                                   UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960828                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Find the next valid character to name a TPG text with.    */
/*  Remarks:    --                                        */
/*  Returns:    Next/previous legal character.                    */
/*  Updates:    --                                        */
/***************************************************************************/
char GetNextTextASCII(UC HWtype, UC direction, UC ASCIIChar)
  {
  code UC* CharPtr;
  UC MaxCharNdx;

  switch (HWtype)
    {
    case PT8601:
    case PT8602:
    case PT8603:
      CharPtr = StdTextCharSet;
      MaxCharNdx = 99;          // Number of characters in CharSet
      break;

    default:
      CharPtr = ExtTextCharSet;
      MaxCharNdx = 235;           // Number of characters in CharSet
      break;
    }

  FeedTheWatchdog;

  if (direction)
    {            // Down: 0 otherwise UP
    do
      {
      if (ASCIIChar++ == MaxCharNdx)
        ASCIIChar = 0;
      } while (!CharPtr[ASCIIChar]);
    }
  else
    {
    do
      {
      if (ASCIIChar-- == 0)
        ASCIIChar = MaxCharNdx;
      } while (!CharPtr[ASCIIChar]);
    }
  return(ASCIIChar);
  }


/***************************************************************************/
/* char GetNext(UC direction, UC ASCIIChar)                        UTIL.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  24.10.2006                                                    */
/*                                                                         */
/* Function: This function is used under Text Edit for HD and DL generators*/
/*           It uses standard ASCII values, as character generator onboard */
/*           only supports these.                                          */
/*                                                                         */
/* Args:     UC direction - Up or Down (increment or decrement current     */
/*                          char)                                          */
/*           UC ASCIIChar - current char under cursor                      */
/* Remarks:  None                                                          */
/*                                                                         */
/* Returns:  Next ASCII char according to direction                        */
/***************************************************************************/
char GetNext(UC direction, UC ASCIIChar)
  {
  code UC* CharPtr;
  UC MaxCharNdx;

  FeedTheWatchdog;

  if (direction)
    {            // Down: 0 otherwise UP
    if (++ASCIIChar == 0x7F)
      ASCIIChar = 0x20;
    }
  else
    {
    if (--ASCIIChar == 0x1F)
      ASCIIChar = 0x7E;
    }
  return(ASCIIChar);
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
char GetNextPresetASCII(UC direction, UC ASCIIChar) {

  if (direction ) {            // Down: 0 otherwise UP
    do {
      if (ASCIIChar++ >= 127)
        ASCIIChar = 0;
    } while (!PresetCharSet[ASCIIChar]);
  }
  else {
    do {
      if (ASCIIChar-- <= 0)
        ASCIIChar = 127;
    } while (!PresetCharSet[ASCIIChar]);
  }

  return(ASCIIChar);
}

/***************************************************************************/
/*  TimeFormat                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980412                          */
/* Revised:    980721                                       */
/*                                                  */
/*  Function:  Convert BCD coded time to a formated textstring          */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeFormat(UC timeFormat, UL time, UC TCIFormat, char* TimeTxt) {

  UC hour, minute, second;

  hour = time >> 16;
  minute = (time >> 8) & 0xFF;
  second = time & 0xFF;

  if (timeFormat == Hour12Format) {
    if (hour > 0x12) {
      hour = 10*(hour/16) + hour%16 - 12;
      hour = 0x10*(hour/10) + hour%10;
    }
  }

  TimeTxt[0] = (hour >> 4) + '0';
  TimeTxt[1] = (hour & 0x0F) + '0';

  if (TCIFormat) {
    TimeTxt[2] = (minute >> 4) + '0';
    TimeTxt[3] = (minute & 0x0F) + '0';

    TimeTxt[4] = (second >> 4) + '0';
    TimeTxt[5] = (second & 0x0F) + '0';

    TimeTxt[6] = 0;
  }
  else {
    TimeTxt[2] = TimeTxt[5] = ':';

    TimeTxt[3] = (minute >> 4) + '0';
    TimeTxt[4] = (minute & 0x0F) + '0';

    TimeTxt[6] = (second >> 4) + '0';
    TimeTxt[7] = (second & 0x0F) + '0';

    TimeTxt[8] = 0;
  }

}

/***************************************************************************/
/*  ValidTime                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980413                                       */
/*                                                  */
/*  Function:  Validate a specific time                        */
/*  Remarks:    Note that TimeInf must be in legal BCD format          */
/*  Returns:    TRUE: if date is valid, otherwise FALSE              */
/*  Updates:    --                                        */
/***************************************************************************/
UC ValidTime(UL TimeInf) {

  UC hour, minute, second;

  hour = (TimeInf >> 16) & 0xFF;
  minute = (TimeInf >> 8) & 0xFF;
  second = TimeInf & 0xFF;

  if ((hour > 0x23) || (minute > 0x59) || (second > 0x59))
    return(FALSE);

  return(TRUE);
}

/***************************************************************************/
/*  AdjustTime                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980413                                       */
/*                                                  */
/*  Function:  Increment/decrement time coded in BCD format            */
/*  Remarks:    ndx is 0,1,3,4,6,7                            */
/*          Format is Hour12Format=0 or Hour24Format=1            */
/*  Returns:    adjusted time                                 */
/*  Updates:    --                                        */
/***************************************************************************/
UL AdjustTime(UC ndx, UC dir, UC format, UL TimeInf) {

  UC hour, minute, second;

  hour = (TimeInf >> 16) & 0xFF;
  minute = (TimeInf >> 8) & 0xFF;
  second = TimeInf & 0xFF;

  switch (ndx) {
    case 0:              // Hour in 10th
      if (format == Hour12Format) {
        if ((hour & 0x0F) < 2) {
          if (hour & 0xF0)
            hour &= 0x0F;
          else
            hour |= 0x10;
        }
        else
          if ((hour > 0x11) && (hour < 0x14))
            hour += 0x10;
        else
          if ((hour > 0x21) && (hour < 0x24))
            hour -= 0x10;
      }
      else {
        if (dir) {
          hour += 0x10;
          if ((hour & 0xF0) > 0x20)
            hour &= 0x0F;
        }
        else {
          if (hour & 0xF0)
            hour -= 0x10;
          else
            hour = 0x20 + (hour & 0x0F);
        }
      }
      break;

    case 1:              // Hour in 1th
      if (dir) {
        if ((hour++ & 0x0F) == 0x09)
          hour &= 0xF0;
      }
      else {
        if (hour & 0x0F)
          hour--;
        else
          hour = 0x09 + (hour & 0xF0);
      }
      break;

    case 3:              // Minutes in 10th
      if (dir) {
        minute += 0x10;
        if ((minute & 0xF0) > 0x50)
          minute &= 0x0F;
      }
      else {
        if (minute & 0xF0)
          minute -= 0x10;
        else
          minute = 0x50 + (minute & 0x0F);
      }
      break;

    case 4:              // Minute in 1th
      if (dir) {
        if ((minute++ & 0x0F) == 0x09)
          minute &= 0xF0;
      }
      else {
        if (minute & 0x0F)
          minute--;
        else
          minute = 0x09 + (minute & 0xF0);
      }
      break;

    case 6:              // Second in 10th
      if (dir) {
        second += 0x10;
        if ((second & 0xF0) > 0x50)
          second &= 0x0F;
      }
      else {
        if (second & 0xF0)
          second -= 0x10;
        else
          second = 0x50 + (second & 0x0F);
      }
      break;

    case 7:              // Second in 1th
      if (dir) {
        if ((second++ & 0x0F) == 0x09)
          second &= 0xF0;
      }
      else {
        if (second & 0x0F)
          second--;
        else
          second = 0x09 + (second & 0xF0);
      }
      break;
  }

  return(hour*0x10000 + minute*0x100 + second);
}

/***************************************************************************/
/*  DateFormat                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980412                          */
/* Revised:    980721                                       */
/*                                                  */
/*  Function:  Convert BCD coded date to a formated textstring.        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void DateFormat(UC format, UL DateInf, UC TCIFormat, char* DateTxt) {

  UC date, month, year, Yndx, Mndx, Dndx;

  year = DateInf >> 16;
  month = (DateInf >> 8) & 0xFF;
  date = DateInf & 0xFF;

  switch (format) {
    case YMDFormat:
      Yndx = 0;

      if (TCIFormat) {
        Mndx = 2;
        Dndx = 4;
      }
      else {
        Mndx = 3;
        Dndx = 6;
      }
      break;

    case DMYFormat:
      Dndx = 0;

      if (TCIFormat) {
        Mndx = 2;
        Yndx = 4;
      }
      else {
        Mndx = 3;
        Yndx = 6;
      }
      break;

    default:              // MMDDYYFormat & others
      Mndx = 0;

      if (TCIFormat) {
        Dndx = 2;
        Yndx = 4;
      }
      else {
        Dndx = 3;
        Yndx = 6;
      }
      break;
  }

  DateTxt[Yndx++] = (year >> 4) + '0';
  DateTxt[Yndx] = (year & 0x0F) + '0';

  DateTxt[Mndx++] = (month >> 4) + '0';
  DateTxt[Mndx] = (month & 0x0F) + '0';

  DateTxt[Dndx++] = (date >> 4) + '0';
  DateTxt[Dndx] = (date & 0x0F) + '0';

  if (TCIFormat)
    DateTxt[6] = 0;
  else {
    DateTxt[2] = DateTxt[5] = '-';
    DateTxt[8] = 0;
  }
}

/***************************************************************************/
/*  ValidDate                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980505                                       */
/*                                                  */
/*  Function:  Validate a specific date.                        */
/*  Remarks:    Note that DateInf must be in legal BCD format.          */
/*  Returns:    TRUE: if date is valid, otherwise FALSE.              */
/*  Updates:    --                                        */
/***************************************************************************/
UC ValidDate(UL DateInf) {

  // Year and millenium must be dividable by 4, e.g year 2000 is a leap year
  // while year 1900 was not

  UC date, month, valid = FALSE;
  UI tmp;

  month = (DateInf >> 8) & 0xFF;
  date = DateInf & 0xFF;

  if (date != 0x00)            // Date 00 is NOT legal
    switch (month) {
      case 0x02:              // February
        tmp = DateInf >> 16;
        tmp = 10*(tmp/16) + tmp%16;

        if (tmp % 4) {
          if (date > 0x28)
            break;
        }
        else {              // Leap year
          if (date > 0x29)
            break;
        }
        valid = TRUE;
        break;

      case 0x04:              // 30 days in month
      case 0x06:
      case 0x09:
      case 0x11:
        if (date > 0x30)
          break;
        valid = TRUE;
        break;

      case 0x01:              // 31 days in month
      case 0x03:
      case 0x05:
      case 0x07:
      case 0x08:
      case 0x10:
      case 0x12:
        if (date > 0x31)
          break;
        valid = TRUE;
        break;

      default:                    // Illegal month
        break;
    }

  return(valid);
}

/***************************************************************************/
/*  AdjustDate                                       UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980505                                       */
/*                                                  */
/*  Function:  Increment/decrement date coded in BCD format.          */
/*  Remarks:    ndx is 0,1,3,4,6,7.                            */
/*  Returns:    adjusted date.                               */
/*  Updates:    --                                        */
/***************************************************************************/
UL AdjustDate(UC ndx, UC dir, UC format, UL DateInf) {

  UC date, month, year;

  year = (DateInf >> 16) & 0xFF;
  month = (DateInf >> 8) & 0xFF;
  date = DateInf & 0xFF;

  switch (format) {
    case YMDFormat:
      switch (ndx) {
        case 0:
        case 1:
          ndx += 4;      // Adjust year
          break;
        case 3:
        case 4:
          ndx--;        // Adjust month
          break;
        default:
          ndx -= 6;      // Adjust date
          break;
      }
      break;

    case MDYFormat:
      switch (ndx) {
        case 0:
        case 1:
          ndx += 2;      // Adjust month
          break;
        case 3:
        case 4:
          ndx -= 3;      // Adjust date
          break;
        default:
          ndx -= 2;      // Adjust year
          break;
      }
      break;

    default:              // DDMMYYFormat & others
      switch (ndx) {
        case 0:
        case 1:
          break;        // Adjust date
        case 3:
        case 4:
          ndx--;        // Adjust month
          break;
        default:
          ndx -= 2;      // Adjust year
          break;
      }
      break;
  }

  switch (ndx) {
    case 0:              // Date in 10th
      if (dir) {
        date += 0x10;
        if ((date & 0xF0) > 0x30)
          date &= 0x0F;
      }
      else {
        if (date & 0xF0)
          date -= 0x10;
        else
          date = 0x30 + (date & 0x0F);
      }
      break;

    case 1:              // Date in 1th
      if (dir) {
        if ((date++ & 0x0F) == 0x09)
          date &= 0xF0;
      }
      else {
        if (date & 0x0F)
          date--;
        else
          date = 0x09 + (date & 0xF0);
      }
      break;

    case 2:              // Month in 10th
      if (month & 0xF0)
        month &= 0x0F;
      else
        month |= 0x10;
      break;

    case 3:              // Month in 1th
      if (dir) {
        if ((month++ & 0x0F) == 0x09)
          month &= 0xF0;
      }
      else {
        if (month & 0x0F)
          month--;
        else
          month = 0x09 + (month & 0xF0);
      }
      break;

    case 4:              // Year in 10th
      if (dir) {
        year += 0x10;
        if ((year & 0xF0) > 0x90)
          year &= 0x0F;
      }
      else {
        if (year & 0xF0)
          year -= 0x10;
        else
          year = 0x90 + (year & 0x0F);
      }
      break;

    case 5:              // Year in 1th
      if (dir) {
        if ((year++ & 0x0F) == 0x09)
          year &= 0xF0;
      }
      else {
        if (year & 0x0F)
          year--;
        else
          year = 0x09 + (year & 0xF0);
      }
      break;
  }

  return(year*0x10000 + month*0x100 + date);
}

/***************************************************************************/
/*  TimeOffsetFormat                                   UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980724                          */
/* Revised:    980727                                       */
/*                                                  */
/*  Function:  Convert BCD coded time offset a formated textstring      */
/*  Remarks:    A sign will be included.                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeOffsetFormat(UL time, char* TimeTxt) {

  UC sign, seconds, milliseconds;

  sign = time >> 16;
  seconds = (time >> 8) & 0xFF;
  milliseconds = time & 0xFF;

  if (sign)
    TimeTxt[0] = '-';
  else
    TimeTxt[0] = '+';

  TimeTxt[1] = (seconds >> 4) + '0';
  TimeTxt[2] = (seconds & 0x0F) + '0';

  TimeTxt[3] = ':';

  TimeTxt[4] = (milliseconds >> 4) + '0';

  TimeTxt[5] = 0;
}

/***************************************************************************/
/*  AdjustTimeOffset                                   UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980727                                       */
/*                                                  */
/*  Function:  Increment/decrement time offset coded in BCD format      */
/*  Remarks:    ndx is 0,1,3,4,6,7                            */
/*  Returns:    adjusted time offset                            */
/*  Updates:    --                                        */
/***************************************************************************/
UL AdjustTimeOffset(UC ndx, UC dir, UL TimeInf) {

  UC sign, seconds, milliseconds;
  long value;

  TimeInf >>= 4;
  value = (TimeInf & 0x0F);

  TimeInf >>= 4;
  value += 10*((10*(TimeInf >> 4) & 0x0F) + (TimeInf & 0x0F));

  sign = ((TimeInf & 0x100) > 0);

  if (dir) {
    if (sign) {
      if (--value == 0)
        sign = 0;
    }
    else {
      if (value++ == 100)
        value = 100;
    }
  }
  else {
    if (sign) {
      if (value++ == 100)
        value = 100;
    }
    else {
      if (--value == -1) {
        sign = 1;
        value = 1;
      }
    }
  }

  milliseconds = value % 10;
  seconds = value / 10;

  return(sign*0x10000 + ((seconds / 10) << 12) + ((seconds % 10) << 8)+ (milliseconds << 4));
}

/***************************************************************************/
/*  ROMCheckOK                                                     UTIL.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970724                                    */
/* Revised:    980524                                                      */
/*                                                                         */
/*  Function:  Test if checksum stored in EEPROM is correct                */
/*  Remarks:   Checksum is a simple 16 bit checksum                        */
/*             This routine requires a LOT of attention.                   */
/*  Returns:    1: ROM check was OK,                                       */
/*              0: The calculated checksum does not                        */
/*                 match the stored checksum                               */
/*  Updates:    --                                                         */
/***************************************************************************/
int ROMCheckOK(UI* Checksum)
  {
  register code char* s;
  register UI i;
  UC j;
  UI CalcChecksum = 0;

  for (j = 0; j < (SizeOfPROMCode / 0x10000); j++)
    {
    FeedTheWatchdog;

    switch (j)
      {
      case 0:
        s = &Page0;
        break;
      case 1:
        s = &Page1;
        break;
      case 2:
        s = &Page2;
        break;
      case 3:
        s = &Page3;
        break;
      case 4:
        s = &Page4;
        break;
      case 5:
        s = &Page5;
        break;
      case 6:
        s = &Page6;
        break;
      case 7:
        s = &Page7;
        break;
      }

    i = 0;
    do
      {
      CalcChecksum += (UC) *s++;
      } while (++i);
    }
  s = &CSumAddr; // code char CSumAddr @ 0x3FFFE;

  *Checksum  = (256 * (UC) *s++);
  *Checksum += (UC) *s;
#if 0
   sprintf(tbuff, "CalcChecksum= %x Checksum= %x\n%", CalcChecksum, *Checksum);
   Udr_PutNChar(tbuff);
#endif

  if (CalcChecksum == *Checksum)
    {
    return (1);
    }
  else
    {
    return (0);
    }
  }

/***************************************************************************/
/*  PowerUpRAMCheckOK                                              UTIL.C  */
/*                                                                         */
/* Author:     Kim Engedahl, DEV, 980505                                   */
/* Revised:    980509                                                      */
/*                                                                         */
/*  Function:  Test if RAM is OK after power up.                           */
/*  Remarks:   This function is a NON destructive test as compared to      */
/*             DiagnoseRAMCheckOK. Note that the status of the diagnose    */
/*             RAM check is examined in this routine too, AND reset if     */
/*             this bit is set.                                            */
/*  Returns:    1: RAM check was OK,                                       */
/*              0: RAM failed at some point OR the                         */
/*                 diagnose RAM check has failed at some point.            */
/*  Updates:    --                                                         */
/*  Note:      This routine check status of Battery Powered RAM V404.      */
/*             A smal portion of the RAM (RAMCheckArea) is reserved for    */
/*             read and write check.  If this check is positive, then      */
/*             this routine writes into NOVRAM V415, purpose and details   */
/*             of this are unknown.                                        */
/***************************************************************************/
bit PowerUpRAMCheckOK() 
  {
  register int i;
  UC tmp;

  for (i = 0; i < 16; i++) /* This checks data retention      */
    {                      /* FactoryPreset() fills AA and 55 */
    tmp = RAMCheckArea[i];
    if (i % 2 )
      tmp -= 0xAA;
    else
      tmp -= 0x55;
    if (tmp)
      return(FALSE);
    }

  for (i = 16; i < 24; i++) /* This checks rw operations */
    {
    RAMCheckArea[i] = 0x55;
    if (RAMCheckArea[i] != 0x55)
      return(FALSE);
    RAMCheckArea[i] = 0xAA;
    if (RAMCheckArea[i] != 0xAA)
      return(FALSE);
    }
  /* bit IIC2Write(UC Address, UC Data) */
  /* #define RAMCheckOK_addr (UC) 85    */
  IIC2Write(mainram1_addr, RAMCheckOK_addr);
  if (!IIC2Read(mainram1_addr))
    return(FALSE);
  return(TRUE);
  }
/***************************************************************************/
/*  DiagnoseRAMCheckOK                                 UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970918                          */
/* Revised:    980505                                       */
/*                                                  */
/*  Function:  Test if RAM is OK.                            */
/*  Remarks:    This routine requires a LOT of attention.              */
/*  Returns:    1: RAM check was OK, 0: The RAM failed at some point      */
/*  Updates:    --                                        */
/***************************************************************************/
bit DiagnoseRAMCheckOK()
  {
  UC  tmp;
  UC  OKFlag = TRUE;
  UC* RAMPage;
  int  i;

  IIC2Write(mainram1_addr, RAMCheckOK_addr);
  if (IIC2Read(mainram1_addr) != TRUE)
    return(FALSE);

  IIC2Write2(mainram1_addr, RAMCheckOK_addr, FALSE);

  
  RAMPage = &StartOfRAM;

  for (i = 0; i < (8*1024 - (int) &StartOfRAM); i++)
    {
    tmp = *RAMPage;

    *RAMPage = 0x55;

    if (*RAMPage != 0x55)
      {
      *RAMPage = tmp;
      OKFlag = FALSE;
      break;
      }
    else
      {
      *RAMPage = 0xAA;
      if (*RAMPage != 0xAA)
        {
        *RAMPage = tmp;
        OKFlag = FALSE;
        break;
        }
      }
    *RAMPage++ = tmp;
    }
  /* Write two bytes to IIC address                */
  /* bit IIC2Write2(UC Address, UC Data, UC Data2) */
  /* returns  1:OK, 0:ERROR                        */
  /* #define RAMCheckOK_addr (UC) 85    */
  IIC2Write2(mainram1_addr, RAMCheckOK_addr, TRUE);
  return(OKFlag);
  }

/***************************************************************************/
/*  RS232CheckOK                                     UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 971014                          */
/* Revised:    980524                                       */
/*                                                  */
/*  Function:  Test if serial communication at a given baudrate is OK    */
/*  Remarks:    Requires that a loopback connector is mounted          */
/*  Returns:    1: RS232 check was OK, 0: An error occurred            */
/*  Updates:    --                                        */
/***************************************************************************/
bit RS232CheckOK(UC Baudrate) {

  UC i, j, idx;
  char ch;

  FeedTheWatchdog;

  for (i = UDR_DATA7; i <= UDR_DATA8; i++) {
    for (j = UDR_NONE; j <= UDR_EVEN; j++) {
      waitms(2);
                            // Re-initialize UART
      Udr_InitializeUart(Baudrate, i, j, UDR_XON_XOFF);

      waitms(2);
  
      idx = 0;
      while (ch = RS232TestTxt[idx]) {
        Udr_PutChar(ch);

        FeedTheWatchdog;

        UserTimer = 40;
        while (((ch = Udr_InChar()) == UDR_EOF) && UserTimer)
          FeedTheWatchdog;

        if ((ch != RS232TestTxt[idx++]) || (UserTimer == 0))
          return(FALSE);
      }  
    }
  }
  return(TRUE);
}

/***************************************************************************/
/*  FindNextASCII                                                  UTIL.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 960828                                    */
/* Revised:    970105                                                      */
/*                                                                         */
/*  Function:  Handle the display containing the                           */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
char FindNextASCII(UC Direction, UC LimitedType, char ASCIIChar)
  {
  if (Direction)
    {                         // Down: 0 otherwise UP
    switch (ASCIIChar)
      {
      case ' ':
        return('-');
      case '-':
        return('0');
      case '9':
        return('A');
      case 'Z':
        if (LimitedType)
          return('_');
        else
          return('a');
      case 'z':
        return('_');
      case '_':
        return(' ');
      default:
        return(++ASCIIChar);
      }
    }
  else
    {
    switch (ASCIIChar)
      {
      case ' ':
        return('_');
      case '-':
        return(' ');
      case '0':
        return('-');
      case 'A':
        return('9');
      case 'a':
        return('Z');
      case '_':
        if (LimitedType)
          return('Z');
        else
          return('z');
      default:
        return(--ASCIIChar);
      }
    }
  }
