/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PT8602/03 SDI Digital test signal generator            */
/*  Module:    D1UTIL.C                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  960703                                    */
/*  Rev. date:  970512, KEn  DEV                              */
/*  Status:    Version 1.0                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*    void Copy_TextRAM()                                  */
/*    void Copy_SoundRAM()                                  */
/*    void Wait10ms()                                    */
/*                                                  */
/* Changes:                                            */
/* 970429: New: clicks in Copy_SoundRAM                        */
/* 970428: New: Wait10ms() function                            */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                        */

#include <string.h>
#include <xa.h>

#include "d1gener.h"
#include "d1util.h"
#include "d1tables.h"
#include "comm.h"

/***************************************************************************/
/*  Copy_TextRAM                                                 D1UTIL.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970325                                    */
/* Revised:    970325                                                      */
/*                                                                         */
/*  Function:  Load a new string to video text RAM                         */
/*  Remarks:                                                               */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void Copy_TextRAM()
  {
  static bit tmp;
  UC i, j, k, no, addr, index;

  tmp = TextEnable;
  TextEnable = 0;

  Status &= 0xFE; // S1S0 = X0 // Remove text insertion immediately
  StatusPort = Status;         // Update status port

  for ( i = 0; i < 26; i++)    // Clear 26 bytes of video RAM
    TextPort[0xC0 + i] = 0;

  TextPort[0xC0] = 94;         // Write half a space

  for ( k = i = 0; i < 8; i++)
    {
    index = ( OSDText[i] - ' ') << 1;

    no   = TextTable[index];      // Get no of samples for character
    addr = TextTable[index + 1];  // Get 1st character address

    for ( j = 0; j < no; j++)
      TextPort[0xC1 + k++] = addr++;
    }
  TextEnable = tmp;
  }

/***************************************************************************/
/*  Copy_SoundRAM                                   D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970325                          */
/* Revised:    970429                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void Copy_SoundRAM()
  {
  static bit TmpSoundEnable;

  code far UC* SilentAddr;
  code far UC* SignalAddr;
  UI i, j;

  TmpSoundEnable = SoundEnable;

  SoundEnable = OFF;
  while ( Status & 0xC0)      // Wait for sound OFF
    ;

  SilentAddr = SoundPageTable[AudioSilence];  // Silence STEREO

  if ( AudioSignal > AudioStereoBBC1kHz)
    {
    if ( AudioSignal < AudioDual)
      SilentAddr += 0x1200;            // Silence MONO
    else 
      SilentAddr += 0x2400;            // Silence DUAL SOUND
    }

  TextStatus &= 0xF3;                // Reset click information

  if ( AudioLevel == AudioSilence)
    {                                // If audiolevel == SILENCE..
    for ( j = 0; j < 11520; j += 2304)
      for ( i = 0; i < 2304; i++)
        {
        EA = 0;
        PageSelect = 1;              // Select downloading sound RAM

        LowSoundRAM[j+i] = SilentAddr[i];
        HighSoundRAM[j+i] = SilentAddr[i+2304];

        PageSelect = 0;
        EA = 1;
        }
    }
  else
    {
    SignalAddr = SoundPageTable[AudioLevel] + SoundOffsetTable[AudioSignal];
    
    switch( AudioSignal)
      {                          // TextStatus bit 2,3 used for CLICK
      case AudioStereoEBU1kHz:
      case AudioMonoEBU1kHz:
        TextStatus |= 0x04;
        break;
      case AudioStereoBBC1kHz:
        TextStatus |= 0x08;
        break;
      case AudioMonoBBC1kHz:        // No click bit in AudioMonoEBU1kHz
        break;
      }

    TextStatusPort = TextStatus;

    switch( AudioSignal)
      {
      case AudioStereo1kHz:         // Alt. sound/silence in click area
      case AudioStereoEBU1kHz:      // Alt. sound/silence in click area
      case AudioStereoBBC1kHz:      // Silence in ALL click area
      case AudioMonoEBU1kHz:        // Silence in ALL click area
      case AudioMonoBBC1kHz:        // There's no click, ie. don't care
        for ( j = 0; j < 11520; j += 2304)
          for ( i = 0; i < 2304; i++)
            {
            EA = 0;
            PageSelect = 1;      // Select downloading sound RAM

            LowSoundRAM[j+i]  = SignalAddr[i];
            HighSoundRAM[j+i] = SignalAddr[i+2304];

            PageSelect = 0;
            EA = 1;            // Allow possible interrupt

            PageSelect = 0;      // Dummy statement

            EA = 0;
            PageSelect = 1;      // Select downloading sound RAM

            if ((( i % 6) < 3) ||
               ( AudioSignal == AudioStereoBBC1kHz) ||
               ( AudioSignal == AudioMonoEBU1kHz))
              {
              LowClickSoundRAM[j+i] = SilentAddr[i];
              HighClickSoundRAM[j+i] = SilentAddr[i+2304];
              }
            else
              {
              LowClickSoundRAM[j+i] = SignalAddr[i];
              HighClickSoundRAM[j+i] = SignalAddr[i+2304];
              }
            PageSelect = 0;
            EA = 1;            // Allow possible interrupt
            }
        break;
      case AudioStereo800Hz:
      case AudioDual:
        for ( i = 0; i < 11520; i++)
          {
          EA = 0;
          PageSelect = 1;          // Select downloading sound RAM

          LowSoundRAM[i] = SignalAddr[i];
          HighSoundRAM[i] = SignalAddr[i+11520];

          PageSelect = 0;
          EA = 1;
          }
        break;
      }
    }
  SoundEnable = TmpSoundEnable;
  }

/***************************************************************************/
/*  Wait10ms                                       D1UTIL.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970428                          */
/* Revised:    970429                                       */
/*                                                  */
/*  Function:  Wait 10 ms                                  */
/*  Remarks:    Line interrupt is used for counting -> system dependant    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void Wait10ms()
  {
  register UI j;
  register UC cnt;

  j = LineCnt;          // Delay for 10ms, while NVRAM store bytes
  cnt = 0;

  while ( cnt < 160)      
    if ( j - LineCnt)
      {
      j = LineCnt;
      cnt++;
      }
  }
