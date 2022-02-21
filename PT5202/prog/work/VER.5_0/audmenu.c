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
 
/************** *********************************
  This chooses between digital and analog audio  
***********************************************/
code struct MENU_ITEM * kind_audio_submenu[] = 
  {
  &analog_audio_submenu[0],
  &digital_audio_submenu[0]
  };




/**************************** Audio Functions ************************/
/*
    In AudioMenuShow Ptr = &audio_menu[AudioTypeNdx].
*/
void AudioMenuShow(void)
  {
  register int i;

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoSelectTxt);            // Write sub-menu headline
  ClearRestLN1();
  AudioTypeNdx = Ptr->ItemNumber;
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

/*
code struct MENU_ITEM * kind_audio_submenu[] = 
  {
  analog_audio_submenu,
  digital_audio_submenu
  };
*/
  
void AudioDown(void)
  {
  //PrevPtr = Ptr;
  Ptr = kind_audio_submenu[AudioTypeNdx]; // this chooses between analog and digital
  FuncState = 1;
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
  }

/* 
     here Ptr = analog_audio_submenu
*/
void AnalogAudioMenuShow(void)
  {
  register int i;
  char jkbuffer[30];

#if 1   
  sprintf(jkbuffer, "AMShow=%d\n", UcharVal);
  rs232puts(jkbuffer);
#endif

  
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
  
void AnalogAudioDown(void)
  {
  /* Now copy present 5201 AnalogAudio settings in AudioOrig[0] to AudioSetup[0] 
     for use in instant
     update.  If ESC is pressed, then restore original settings           */
  memcpy(&AudioSetup[AnalogAudio], &AudioOrig[AnalogAudio], sizeof(AudioSetup));
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
  //Ptr = &analog_audio_sub_submenu[AudioNdx[AudioTypeNdx]];
  Ptr = &analog_audio_sub_submenu[0];
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
      memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioOrig));
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
         the algorithm below                                            */
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
      memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioOrig));
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
      memcpy(&AudioOrig[AnalogAudio], &AudioSetup[AnalogAudio], sizeof(AudioOrig));
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
      if (UcharVal-- == 0)
        UcharVal = 30;
      }
    else 
      {                      // ..button pressed is DOWN
      if (UcharVal++ == 30)
        UcharVal = 0;
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
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
  }

/*
    Here Ptr = digital_audio_submenu;
*/
void DigitalAudioMenuShow(void)
  {
  //struct MENU_ITEM code * tmpPtr;
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
  
void DigitalAudioDown(void)
  {
  /* Now copy present 5201 DigitalAudio settings to AESEBUConfig for use in instant
     update.  If ESC is pressed, then restore original settings           */
  memcpy(&AudioSetup[AESEBUAudio], &AudioOrig[AESEBUAudio], sizeof(AudioSetup));
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
  //Ptr = &digital_audio_sub_submenu[AudioNdx[AudioTypeNdx]];
  Ptr = &digital_audio_sub_submenu[0];
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
      memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioSetup));
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioClickItem);
    Ptr = PrevPtr;
    //Ptr = &audio_menu[AudioTypeNdx];
    Ptr->DispFunct();
    FuncSave = FuncEscape = FALSE;
    return;
    }
  if (FuncTriggers & 0x03) 
    {                          // If UP/DOWN button pressed..
/* NOTE: click can have values of 1 or 3 (not 0 or 1, as usual).  Hence
         the algorithm below                                            */
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
      memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioSetup));
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
      memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioSetup));
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
      {                 // If ScHPhase has changed..
      Settings.ActivePreset = 0;            // Clear possible preset Save new BB ScH-Phase
      AudioSetup[AESEBUAudio].Timing = FloatVal;
      // update settings if Save
      memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioSetup));
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioTimingItem);
    Ptr = PrevPtr;                   // Set pointer to previous menu
    Ptr->DispFunct();                // Display previous menu
    FuncSave = FuncEscape = FALSE;
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
    if (FloatVal == AudioOrig[AESEBUAudio].Timing)
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
      memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioSetup));
      }
    do_AesebuConfig(&AudioOrig[AESEBUAudio], DigitalAudioSystemItem);
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
    WriteItemArrows(8, Mstrlen(CodePtr));  // Mark selected item
  FuncTriggers = 0;                  // Reset the button-selection
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
      memcpy(&AudioOrig[AESEBUAudio], &AudioSetup[AESEBUAudio], sizeof(AudioSetup));
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
