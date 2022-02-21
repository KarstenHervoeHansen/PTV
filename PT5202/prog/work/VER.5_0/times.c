#if 1
/***************************************************************************/
/* AnlTimeAdjust                                    MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960621                          */
/* Revised:    980507                                       */
/*                                                  */
/*  Function:  Update the delay in the analog units.                */
/*  Remarks:    The parameter <item> is based the items: AnlBlkItem,      */
/*          AnlTPG2Item-AnlTPG5Item & GenlockItem                */
/*  Returns:    --                                        */
/*  Updates:    TBD                                      */
/***************************************************************************/
void AnlTimeAdjust(UC system, long delay)
  {
  char  sign;
  int   field;
  int   line;
  float time;
  int   lineTst;
                                // Time, ie. T, adjustement..
  if (FuncTriggers & 0x30)
    {                           // If button pressed is UP/DOWN..
    if (FuncTriggers & 0x10)   // If button pressed is UP ..
      samples += stepVal;
    else
      samples -= stepVal;
    }
  if (system < NTSC_US) // ***************If system is PAL.. ***********
    {                               // Line, ie. H, adjstement..
    if (FuncTriggers & 0x0C)
      {                               // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1728L * 256L;
      else
        samples -= (long) 1728L * 256L;
      }
                                    // Field, ie. V, adjustement ..
    if (FuncTriggers & 0x03)       // If button pressed is UP/DOWN..
      {
      lineTst = (samples / ((long) 1728L *256L)) % 625L;
      if (FuncTriggers & 0x01)
        {                           // If button pressed is UP..
        if (lineTst < 313)
          samples += (long) 313L * 1728L * 256L;
        else
          samples += (long) 312L * 1728L * 256L;
        }
      else 
        {
        if (lineTst < 313)
          samples -= (long) 312L * 1728L * 256L;
        else
          samples -= (long) 313L * 1728L * 256L;
        }
      }
    if (samples < 0)
      samples += (long) 4L * 625L *  172L * 256L;
    else
      samples %= (long) 4L * 625L * 1728L * 256L;
    }
  else 
    {                      // ********* otherwise system is NTSC ***********
    if (FuncTriggers & 0x0C)         // Line, ie. H, adjustement..
      {                               // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1716L*256L;
      else
        samples -= (long) 1716L*256L;
      }
                                     // Field, ie. V, adjustement..
    if (FuncTriggers & 0x03)
      {                              // If button pressed is UP/DOWN..
      lineTst = (samples / ((long) 1716L*256L)) % 525L;
      if (FuncTriggers & 0x01)
        {                            // If button pressed is UP..
        if (lineTst < 263)
          samples += (long) 263L*1716L*256L;
        else
          samples += (long) 262L*1716L*256L;
        }
      else 
        {
        if (lineTst < 263)
          samples -= (long) 262L*1716L*256L;
        else
          samples -= (long) 263L*1716L*256L;
        }
      }
    if (samples < 0)
      samples += (long) 2L*525L*1716L*256L;
    else
      samples %= (long) 2L*525L*1716L*256L;
    }
                                 // If time ie T, is being updated..
  if (FuncTriggers & 0xC0)
    {                            // If button pressed is RIGHT/LEFT..
    if (FuncTriggers & 0x40)
      {                          // If button pressed is LEFT..
      if (stepNdx++ == stepNdxAnalogMax)
        stepNdx = stepNdxAnalogMin;
      }
    else 
      {                          // ..button pressed is RIGHT
      if (stepNdx-- == stepNdxAnalogMin)
        stepNdx = stepNdxAnalogMax;
      }
    stepVal = stepsizeAnalogTable[stepNdx];  // Get step size from index
    }
  if (FuncTriggers & 0x3F)
    {
    if (delay == samples)
      {
      if (FuncState == 11)
        FuncState--;
      else
        FuncState -= 4;
      }
    }
  AnlSamplesToFLT(system, samples, &sign, &field, &line, &time);
  WriteTimingFLT(sign, field, line, time);
  WriteType6_SAVE_ESC(FuncState);    // Write the SAVE ESC text
  switch (FuncState)
    {
    case 1:
    case 5:
      WriteItemArrows(3, 2);        // Highlight selection
      break;
    case 2:
    case 6:
      WriteItemArrows(10, 4);      // Highlight selection
      break;
    case 3:
    case 7:
      WriteItemArrows(19, 8);      // Highlight selection
      CharBlinkLN2(0, OFF);        // Switch on blinking at
      stepVal = stepsizeAnalogTable[stepNdx = stepNdxAnalogMin]; // Reset step index and stepVal
      break;
    case 10:
    case 11:
      WriteItemBrackets(19, 8);        // Highlight selection
      if (stepNdx == stepNdxAnalogMin)
        CharBlinkLN2(26-stepNdx, ON);  // Switch on blinking
      else
        CharBlinkLN2(25-stepNdx, ON);
      break;
    }
  }
#endif


void SDITimeAdjust(UC system, long delay)
  {
  //char jkbuffer[15];
  
  char  sign;
  int   field;
  int   line;
  float time;
  int   lineTst;
                                // Time, ie. T, adjustement..
  if (FuncTriggers & 0x30)
    {                           // If button pressed is UP/DOWN..
    if (FuncTriggers & 0x10)   // If button pressed is UP ..
      samples += stepVal;
    else
      samples -= stepVal;
    }
  if (system < NTSC_US) // ***************If system is PAL.. ***********
    {                               // Line, ie. H, adjstement..
    if (FuncTriggers & 0x0C)
      {                               // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1728L * 256L;
      else
        samples -= (long) 1728L * 256L;
      }
                                    // Field, ie. V, adjustement ..
    if (FuncTriggers & 0x03)       // If button pressed is UP/DOWN..
      {
      lineTst = (samples / ((long) 1728L *256L)) % 625L;
      if (FuncTriggers & 0x01)
        {                           // If button pressed is UP..
        if (lineTst < 312)
          samples += (long) 312L * 1728L * 256L;
        else
          samples += (long) 313L * 1728L * 256L;
        }
      else 
        {
        if (lineTst < 312)
          samples -= (long) 313L * 1728L * 256L;
        else
          samples -= (long) 312L * 1728L * 256L;
        }
      }
    if (samples < 0)
      samples += (long) 625L * 1728L * 256L;
    else
      samples %= (long) 625L * 1728L * 256L;
    }
  else 
    {                      // ********* otherwise system is NTSC ***********
    if (FuncTriggers & 0x0C)         // Line, ie. H, adjustement..
      {                               // If button pressed is UP/DOWN..
      if (FuncTriggers & 0x04)       // If button pressed is UP..
        samples += (long) 1716L*256L;
      else
        samples -= (long) 1716L*256L;
      }
                                     // Field, ie. V, adjustement..
    if (FuncTriggers & 0x03)
      {                              // If button pressed is UP/DOWN..
      lineTst = (samples / ((long) 1716L*256L)) % 525L;
      if (FuncTriggers & 0x01)
        {                            // If button pressed is UP..
        if (lineTst < 262)
          samples += (long) 262L*1716L*256L;
        else
          samples += (long) 263L*1716L*256L;
        }
      else 
        {
        if (lineTst < 262)
          samples -= (long) 263L*1716L*256L;
        else
          samples -= (long) 263L*1716L*256L;
        }
      }
    if (samples < 0)
      samples += (long) 525L * 1716L * 256L;
    else
      samples %= (long) 525L * 1716L * 256L;
    }
                                 // If time ie T, is being updated..
  if (FuncTriggers & 0xC0)
    {                            // If button pressed is RIGHT/LEFT..
    if (FuncTriggers & 0x40)
      {                          // If button pressed is LEFT..
      if (stepNdx++ == stepNdxSDIMax)
        stepNdx = stepNdxSDIMin;
      }
    else 
      {                          // ..button pressed is RIGHT
      if (stepNdx-- == stepNdxSDIMin)
        stepNdx = stepNdxSDIMax;
      }
    stepVal = stepsizeSDITable[stepNdx];  // Get step size from index
    }
  if (FuncTriggers & 0x3F)
    {
    if (delay == samples)
      {
      if (FuncState == 11)
        FuncState--;
      else
        FuncState -= 4;
      }
    }
  AnlSamplesToFLT(system, samples, &sign, &field, &line, &time);
  WriteTimingFLT(sign, field, line, time);
  WriteType6_SAVE_ESC(FuncState);    // Write the SAVE ESC text
  switch ( FuncState)
    {
    case 1:
    case 5:
      WriteItemArrows( 3, 2);        // Highlight selection
      break;
    case 2:
    case 6:
      WriteItemArrows( 10, 4);      // Highlight selection
      break;
    case 3:
    case 7:
      WriteItemArrows( 19, 8);      // Highlight selection
      CharBlinkLN2( 0, OFF);        // Switch on blinking at
      stepVal = stepsizeSDITable[stepNdx = stepNdxSDIMin];
      break;
    case 10:
    case 11:
      WriteItemBrackets( 19, 8);        // Highlight selection
      CharBlinkLN2( 23-stepNdx, ON);
      break;
    }
  }
