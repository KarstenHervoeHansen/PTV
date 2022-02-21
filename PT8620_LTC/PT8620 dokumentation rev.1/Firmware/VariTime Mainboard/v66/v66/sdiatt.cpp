code struct CMD_STRUCT SDITPGLevel1[7] =
  {
    { PATTERNCmdTxt,   PATTCmdTxt, &SDITPGLevel1[1], &SDITPGLevel2[0], NONE, SDITPGPattern},
    { TEXTCmdTxt,      TEXTCmdTxt, &SDITPGLevel1[2], &SDITPGLevel4[0], NONE, NONE},
    { SYSTEMCmdTxt,    SYSTCmdTxt, &SDITPGLevel1[3], NULL,             NONE, SDITPGSystem},
    { EDHINSERTCmdTxt, EDHCmdTxt,  &SDITPGLevel1[4], NULL,             NONE, SDITPGEDHInsert},
    { EMBAUDIOCmdTxt,  EMBCmdTxt,  &SDITPGLevel1[5], &SDITPGLevel5[0], NONE, NONE},
    { DELAYCmdTxt,     DELCmdTxt,  &SDITPGLevel1[6], NULL,             NONE, SDITPGDelay},
    { VERSIONCmdTxt,   VERSCmdTxt, NULL,             NULL,             NONE, SDITPGVersion}
  };

code struct CMD_STRUCT SDITPGLevel2[1] = {
  { MODIFYCmdTxt, MODCmdTxt, NULL, &SDITPGLevel3[0], NONE, NONE},
};

code struct CMD_STRUCT SDITPGLevel3[6] = {
  { APALCmdTxt,      APALCmdTxt, &SDITPGLevel3[1], NULL, NONE,               SDITPGAttribAPAL},
  { PLUGEParTxt,     PLUGParTxt, &SDITPGLevel3[2], NULL, NONE,               SDITPGAttribPLUGE},
  { STAIRCASECmdTxt, STACmdTxt,  &SDITPGLevel3[3], NULL, SDITPGAttribSuffix, SDITPGAttribStair10},
  { MOTIONCmdTxt,    MOTCmdTxt,  &SDITPGLevel3[4], NULL, NONE,               SDITPGAttribMotion},
  { CIRCLESCmdTxt,   CIRCCmdTxt, &SDITPGLevel3[5], NULL, NONE,               SDITPGAttribCircles},
  { LOGOCmdTxt,      LOGCmdTxt,  NULL,             NULL, NONE,               SDITPGAttribLogo},
};


/***************************************************************************/
/*  SDITPGAttribAPAL                                           RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    990305                                                      */
/*                                                                         */
/*  Function:  Handle the SDI-TPG pattern-attribute Anti-PAL.              */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDITPGAttribAPAL()
  {
  if (SDITPGUnit[Suffix].Present)
    {
    if (AttribAntiPAL == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
        SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribAntiPAL)) 
      SDITPGAttribCommon(AttribAntiPAL);
    else
      SCPIError = ExecErr;
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGAttribCommon                                         RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    980625                                                      */
/*                                                                         */
/*  Function:  Handle the SDI-TPG pattern attributes.                      */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDITPGAttribCommon(UC Attribute)
  {
  UC tmp, pattern;

  pattern = SDITPGConfig[Suffix].Pattern;

  switch (ParameterType)
    {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0)
        {
        ParState++;

        ucharval = FindParameter(Cmd, OffOnPar);

        if (ucharval > ON)
          SCPIError = IllegalParValue;
        }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
    }

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParState)
        SCPIError = ParNotAllowed;
      else
        {
        tmp = ((SDITPGAttrib(Suffix, pattern) & (1 << Attribute)) > 0);

        Udr_CPutNChar(OffOnPar[tmp].LongName);
        Udr_PutNewline();
        }
      }
    else
      {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        tmp = pattern - SDIPhilips4x3;

        if (((SDITPGConfig[Suffix].Attrib[tmp] & (1 << Attribute)) ^ (ucharval << Attribute))) {
          ActivePresetNo = 0;      // Clear possible preset

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          SDITPGConfig[Suffix].Attrib[tmp] &= ~(1 << Attribute);
          SDITPGConfig[Suffix].Attrib[tmp] |= (ucharval << Attribute);

          TrxSDITPGPattern(Suffix, SDITPGConfig[Suffix].Pattern,\
                            SDITPGConfig[Suffix].Attrib[tmp]);
          }
        }
      }
    }
  }


