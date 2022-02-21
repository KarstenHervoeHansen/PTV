/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PT8602/03 SDI Digital test signal generator            */
/*  Module:    D1CMD.C                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  970217                                    */
/*  Rev. date:  990614, KEn  DEV                              */
/*  Status:    Version 1.2                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*  Changes:                                            */
/* 990614: Support for textpositioning                          */
/* 990318 renamed some variables                              */
/* 980929 Added Read_HW_Information_Command(). Update Reset_Command()    */
/* 960215 Adapted to XA processor.                            */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                        */

#include "string.h"
#include <xa.h>

#include "d1gener.h"
#include "d1cmd.h"
#include "d1tables.h"
#include "d1util.h"
#include "rs232.h"
#include "comm.h"
#include "version.h"

/***************************************************************************/
/*  Unit_Address_Command                                   D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Unit_Address_Command()   {

  switch ( CmdDelimiter)   {
    case ';':
      break;

    case '?':
//      WriteEndOfTx( WriteValue(( UL) MyAddress));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  G_Offset_Command                                      D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void G_Offset_Command()   {

  if ( CmdDelimiter == ';')   {

    if ( TVSystem != PALG)   {
      TVSystem = PALG;
      CalOffset = IIC_RdOffset( EEPROM_IIC_Adr, IIC_G_Address);

      if ( IICErrorCode)   {          // If any errors in IIC..
        ErrorStatus |= IICErrorCode;
        CalOffset = 0;            //  reset timing
        }
      }
    UserOffset = Parameter[0];
    Calculate_Offset();
    }
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  M_Offset_Command                                      D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void M_Offset_Command()   {

  if ( CmdDelimiter == ';')   {

    if ( TVSystem != NTSC)   {
      TVSystem = NTSC;
      CalOffset = IIC_RdOffset( EEPROM_IIC_Adr, IIC_M_Address);

      if ( IICErrorCode)   {          // If any errors in IIC..
        ErrorStatus |= IICErrorCode;
        CalOffset = 0;            //  reset timing
        }
      }
    UserOffset = Parameter[0];
    Calculate_Offset();
    }
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Read_System_Command                                    D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_System_Command()   {

  if ( CmdDelimiter == '?')
    WriteEndOfTx( WriteValue((UL) TVSystem));
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Read_Offset_Command                                    D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_Offset_Command()   {

  if ( CmdDelimiter == '?') 
    WriteEndOfTx( WriteValue( UserOffset));
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Genlock_Offset_Command                                  D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970401, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Genlock_Offset_Command()   {

  switch ( CmdDelimiter)   {
    case ';':
      GenlockOffset = Parameter[0];
      Calculate_Offset();
      break;

    case '?':
      WriteEndOfTx( WriteValue( GenlockOffset));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Pattern_Command                                      D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Pattern_Command()   {

  switch ( CmdDelimiter)   {
    case ';':
      PatternNo = (UC) Parameter[0];
      break;

    case '?':
      WriteEndOfTx( WriteValue((UL) PatternNo));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  EDH_Command                                          D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void EDH_Command()   {

  switch ( CmdDelimiter)   {
    case ';':
      if ( EDHInsertion = (bit) Parameter[0])
        Status &= ~EDHMask;
      else
        Status |= EDHMask;
      break;

    case '?':
      WriteEndOfTx( WriteValue((UL) EDHInsertion));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Audio_Signal_Command                                    D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Audio_Signal_Command()   {

  switch ( CmdDelimiter)   {
    case ';':
      AudioSignal = (UC) Parameter[0];

      if ( AudioSignal == AudioSignalOff)
        SoundEnable = OFF;
      else   {
        Copy_SoundRAM();
        SoundEnable = ON;
        }
      break;

    case '?':
      WriteEndOfTx( WriteValue((UL) AudioSignal));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Audio_Level_Command                                 D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Audio_Level_Command()   {

  switch ( CmdDelimiter)   {
    case ';':
      AudioLevel = (UC) Parameter[0];
      Copy_SoundRAM();
      break;

    case '?':
      WriteEndOfTx( WriteValue((UL) AudioLevel));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Text_ON_Command                                                D1CMD.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    990613, KEn                                                 */
/*                                                                         */
/*  Function:                                                              */
/*  Remarks:                                                               */
/*  Returns:                                                               */
/*  Updates:    Text can only be ON or OFF: JK                             */
/***************************************************************************/
void Text_ON_Command()
  {
  switch ( CmdDelimiter)
    {
    case ';':
      TextEnable = (bit) Parameter[0];
#if 0
      if ((bit) Parameter[1])
        first_line_w_text = 38;
      else
        first_line_w_text = 24;
#endif
      break;

    case '?':
      WriteEndOfTx( WriteValue((UL) TextEnable));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Text_Command                                        D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Text_Command()
  {
  switch ( CmdDelimiter)
    {
    case ';':
      strcpy( OSDText, String);
      Copy_TextRAM();
      break;

    case '?':
      WriteEndOfTx( WriteString( OSDText));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Read_Status_Command                                    D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_Status_Command()   {

  if ( CmdDelimiter == '?')   {
    WriteEndOfTx( WriteValue( ErrorStatus));
    ErrorStatus = 0;
    }
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  G_Phase_Command                                      D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970429, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void G_Phase_Command()   {

  UL tmp;

  switch ( CmdDelimiter)   {
    case '$':
      if ( IIC_WrOffset( EEPROM_IIC_Adr, IIC_G_Address, UserOffset))   {
        CalOffset = UserOffset;
        UserOffset = 0;
        }
      else
        ErrorStatus |= IICErrorCode;
      break;

    case '?':
      tmp = IIC_RdOffset( EEPROM_IIC_Adr, IIC_G_Address);
      if ( IICErrorCode)   {
        ErrorStatus |= IICErrorCode;
        tmp = 0;
        }

      WriteEndOfTx( WriteValue( tmp));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  M_Phase_Command                                      D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970429, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void M_Phase_Command()   {

  UL tmp;
  
  switch ( CmdDelimiter)   {
    case '$':
      if ( IIC_WrOffset( EEPROM_IIC_Adr, IIC_M_Address, UserOffset))   {
        CalOffset = UserOffset;
        UserOffset = 0;
        }
      else
        ErrorStatus |= IICErrorCode;
      break;

    case '?':
      tmp = IIC_RdOffset( EEPROM_IIC_Adr, IIC_M_Address);
      if ( IICErrorCode)   {
        ErrorStatus |= IICErrorCode;
        tmp = 0;
        }

      WriteEndOfTx( WriteValue( tmp));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Reset_Command                                        D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    990308, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Reset_Command()   {

  UC i;

  if ( CmdDelimiter == ';')   {
    GeneratorRunning = TRUE;

    UserOffset = GenlockOffset = CalOffset = 0;

    IIC_Write2( EEPROM_IIC_Adr, PT_HWType, HWType);
    IIC_Write2( EEPROM_IIC_Adr, PT_HWVersion, HWVersion);

    if ( !IIC_WrOffset( EEPROM_IIC_Adr, IIC_G_Address, UserOffset))
      ErrorStatus |= IICErrorCode;

    if ( !IIC_WrOffset( EEPROM_IIC_Adr, IIC_M_Address, UserOffset))
      ErrorStatus |= IICErrorCode;
    
    for ( i = 0; i < 10; i++) 
      IIC_WrStr(  EEPROM_IIC_Adr, IIC_UserTxt+( 11*i), "\0\0\0\0\0\0\0\0\0\0\0");
    }
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Write_User_Text_Command                                 D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Write_User_Text_Command()   {

  UC no;

  if ( CmdDelimiter == ';')   {
    if (( no = (UC) Parameter[0]) < 10)
      if ( !IIC_WrStr( EEPROM_IIC_Adr, IIC_UserTxt+( 11*no), String))
        ErrorStatus |= IICErrorCode;
    }
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Read_User_Text_Command                                 D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/* Function:  Read m, (<11), bytes of user text from IIC area n, (n: 0-9)  */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_User_Text_Command()   {

  UC no;
  char str[11];

  if ( CmdDelimiter == '?')   {
    if (( no = (UC) Parameter[0]) < 10)   {
      if ( IIC_RdStr( EEPROM_IIC_Adr, IIC_UserTxt+( 11*no), str))
        WriteEndOfTx( WriteString( str));
      else
        ErrorStatus |= IICErrorCode;
      }
    }
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Read_SW_Version_Command                                 D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_SW_Version_Command()   {

  switch ( CmdDelimiter)   {
    case ';':            // Dummy command
      break;

    case '?':            // Request
      WriteEndOfTx( WriteValue( SWVersion));
      break;

    default:
      ErrorStatus |= Illegal_Del;
      break;
    }
  }

/***************************************************************************/
/*  Read_HW_Version_Command                                 D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    990318, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_HW_Version_Command()   {
  
  if ( CmdDelimiter == '?')
    WriteEndOfTx( WriteValue((UL) 256*HWVersion + HWType));
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Read_HW_Information_Command                             D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980929                         */
/* Revised:    980929, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Read_HW_Information_Command()   {
  
  if ( CmdDelimiter == '?')
    WriteEndOfTx( WriteValue((UL) HWInformation));
  else
    ErrorStatus |= Illegal_Del;
  }

/***************************************************************************/
/*  Calculate_Offset                                     D1CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970401, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Calculate_Offset()   {

  UL tmpOffset;

  tmpOffset = UserOffset + GenlockOffset + CalOffset;

  if ( TVSystem == PALG)   {

    tmpOffset %= 276480000;
    EA = 0;                // Disable interrupt while updating offset

                        // Lineoffset must be converted to counter
    LineOffset = 0xFFFF - ( tmpOffset / 442368);
    PhaseOffset = 1727 - (( tmpOffset % 442368) / 256);
    }
  else   {
    tmpOffset %= 230630400;

    EA = 0;                // Disable interrupt while updating offset
                        // Lineoffset must be converted to counter
    LineOffset = 0xFFFF - ( tmpOffset / 439296);
    PhaseOffset = 1715 - (( tmpOffset % 439296) / 256);

    if ( PhaseOffset > 1687)
      PhaseOffset += 12;
    }

  EA = 1;
  }
