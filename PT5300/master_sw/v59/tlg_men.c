/***************************************************************************/
/* TLGMenuShow                                                      menu.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 29.06.2004                             */
/* Revised:                                                                */
/*                                                                         */
/* Function: To display TLG menu, according to number of generators        */
/*           present in PT5230.                                            */
/* Params:   None                                                          */
/* Remarks:  This function finds out how many generators are present       */
/*           and displays menu in form:                                    */
/*           <TLG1>  <TLG2>  <TLG3>  <TLG4> ... etc                        */
/* Returns:  Nothing                                                       */
/* Updates:    --                                                          */
/***************************************************************************/
void TLGMenuShow()
  {
  register int i, j, last, NoOfTLGs;

  WriteCodeLN1(0, MenuHdrTxt);
  WriteCode1(base_menu[BaseNdx].ItemTxt);
  WriteCode1(CoConfigureTxt);           // Write menu headline
  ClearRestLN1();

  TLGNoNdx = Ptr->ItemNumber;          // Update TLG unit index, ie. no.

  WriteCodeLN2(0, SubmnuHdrTxt);
                            
  for (NoOfTLGs = 0, i = TLG1Item; i < TLG12Item + 1; i++)
    if (TLGUnit[i].Present)
      NoOfTLGs++;              // Calculate no. of TLG units


  switch (NoOfTLGs)
    {
    case 4:
      i = 0;
      last = TLG3Item+1;
      break;
    case 8:
      i = last = TLG12Item;   // two screens are required
      (TLGNoNdx > TLG1Item) ? (last = TLG7Item+1) : (i = TLG1Item);
      break;
    case 12:
      break;
    default:
      break;
    }

  for (j = 0;  i < last; i++)
    {                              // Display active screen
    if (TLGUnit[i].Present)
      {
      WriteCode2(tlg_menu[i].ItemTxt);// Write item text
      WriteTxt2("  ");                // Jump to next position

      if (i < TLGNoNdx)               // Calculate item marker
        j += (Cstrlen(tlg_menu[i].ItemTxt) + 2);
      }
    }
  ClearRestLN2();             // Clear reset of line 2

  i = Ptr->ItemConfig;        // Get line1&2 icons/dots info

  if (NoOfTLGs > 4 )          // If 4 or more TLG units..
    i += MORE_;               //  add 'more' dots to line 2

  WriteArrowsField(i);        // Write line 1&2 icons/dots
  i = Cstrlen(Ptr->ItemTxt);  // Get length of text to mark
  WriteItemArrows(j + 8, (UC) i);  // Mark selected item
  }
