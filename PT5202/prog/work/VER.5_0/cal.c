void cal(void)
  {
  //char jkbuffer[15];

  if (FuncSave || FuncEscape)
    {
    if (FuncSave)
      {                    // If delay has changed..
      do_cal_store();
      }
    Ptr = PrevPtr;
    Ptr->DispFunct();
    do_cal_enable_off();
    return;
    }


  WriteCodeLN1(0, SecondmnuHdrTxt);
  WriteCode1(SlashDotHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);           // Write menu headline
  WriteChar1('/');
  ClearRestLN1();                 
  
  if (FuncTriggers & 0x30)      // MAX, ie. 3rd item
    {                           // If button pressed is UP/DOWN..
    FuncState = 1;              //  remove SAVE option
    if (FuncTriggers & 0x10)    // If button pressed is UP ..
      ;
    else
      ;
    }
  if (FuncTriggers & 0x0C)    // MIN, ie. second item
    {                         // If button pressed is UP/DOWN..
    FuncState = 1;              //  remove SAVE option
    if (FuncTriggers & 0x04)  // If button pressed is UP..
      ;
    else
      ;
    }
  if (FuncTriggers & 0x03)        // ADJ, ie. first item
    {
    if (FuncTriggers & 0x01)      // If button pressed is UP/DOWN..
      {                           // If button pressed is UP..
      send_up();
      }
    else 
      {
      send_down();
      }
    }

  tmpPtr = Ptr;                      // Save menu pointer temporarily
  Ptr    = &calibration_menu[0];
  WriteCodeLN2(0, SubmnuHdrTxt);     // Write header for sub-menu line
  start  = CalibrationMaxItem;
  number = 3;
  for (i = start; i < start + number; i++) 
    {
    ClearToPosLN2(Ptr->ItemPos);    // Find item column position
    WriteCode2(Ptr->ItemTxt);       // text is in code memory
    Ptr = Ptr->NextItem;
    }
  ClearRestLN2();
  Ptr = tmpPtr;


  WriteType6_SAVE_ESC(FuncState);    // Write the SAVE ESC text
  switch (FuncState)
    {
    case 1:
    case 5:
      WriteItemArrows(8, 3);        // Highlight selection
      break;
    case 2:
    case 6:
      WriteItemArrows(13, 3);      // Highlight selection
      break;
    case 3:
    case 7:
      WriteItemArrows(18, 3);      // Highlight selection
      break;
    case 10:
    case 11:
      // execute button pressed
      break;
    }
  }
