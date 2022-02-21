/***************************************************************************/
/*  Copyright   ProTeleVision Technologies A/S, BRONDBY 1998               */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:     base.C                                                     */
/*  Author:     Janusz Kuzminski                                           */
/*  Org. date:  11.09.2006: JK                                             */
/*  Rev. date:                                                             */
/*  Status:     Version xx                                                 */
/*                                                                         */
/*  This was introduced because of too big size of module menu.c, which    */
/*  was increased due to new menu intems introduced.                       */
/***************************************************************************/
#include <xa.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mas.h"

#include "menutree.h"
#include "menu.h"
#include "menuhlp.h"
#include "message.h"
#include "timing.h"
#include "disp_drv.h"
#include "cont_drv.h"
#include "keyb_drv.h"
#include "text.h"
#include "tables.h"
#include "util.h"
#include "serial1.h"
#include "unit_drv.h"
#include "iic.h"
#include "rs232par.h"
#include "rs232err.h"
#include "xaexprt.h"
#include "xadrivr.h"

extern code char* near CodePtr;  // Temp. pointer to text arrays in CODE
extern char* near TxtPtr;        // Temp. pointer to text arrays in DATA

/* RAM based base menu containing installed elements only  */
typedef struct RAM_MENU_ITEM
  {
  UC ItemNumber;  /* enum BaseItems in menutree.h */

  code char* ItemTxt;
  UC ItemPos;
  UC ItemConfig;

  void ( *DispFunct)();

  void ( *LeftBut)();
  void ( *UpBut)();
  void ( *DownBut)();
  void ( *RightBut)();

  void ( *ExecBut)();
  void ( *EscapeFct)();

  struct RAM_MENU_ITEM* NextItem;
  struct RAM_MENU_ITEM* PrevItem;
  };

struct RAM_MENU_ITEM RAM_base_menu[BaseMax];

struct RAM_MENU_ITEM* near RPtr; /* pointer to RAM based base menu array */
code char SpaceTxt[]       = " ";
UC        NrInstalledItems;

static char number = 3;    /* number of items to display in RamBaseMenu */

static char tbuff[80];     /* temporary buffer used for printing messages */
                           /* for test purposes                           */
UC   ConstructMenu(void);
int  CCstrcmp(register code char * s1, register code char * s2);
UC   FindBaseNdx(UC rndx);

/***************************************************************************/
/*  BaseMenuShow                                                   MENU.C  */
/*                                                                         */
/* Author:    Janusz Kuzminski                                             */
/* Revised:   14.09.2006, JK                                               */
/*                                                                         */
/*  Function: Display the items in the BASE menu.                          */
/*  Returns:  Nothing                                                      */
/*  Updates:    TBD                                                        */
/* Remarks:  This function is entered each time base menu is to be         */
/*           displayed.  It finds out which generators are installed and   */
/*           controls contents of display second line according to number  */
/*           of generators installed                                       */
/*           This function displays base menu in form:                     */
/*           ...                                                           */
/*           MENU: <BLACK-BURST>  <AES-EBU>                                */
/*           This function has to be modified each time a new generataor   */
/*           is to be added.                                               */
/***************************************************************************/
/* this is taken from earlier version with MixSyncUnit */
void BaseMenuShow()
  {
  UC spacing;

  struct RAM_MENU_ITEM* tmpPtr;

  char  i, cursor;

#if 0
/**************************************/

  AnlTPGUnit[0].Present = 1;

  SDITSGUnit[0].Present = 1;
  SDITSGUnit[1].Present = 1;

  SDITPGUnit[0].Present = 1; // 8632
  SDITPGUnit[1].Present = 1;
  SDITPGUnit[2].Present = 1;
  DLTPGUnit[2].Present = 1;
  HDTPGUnit[0].Present = 1;
/**************************************/ItemNumber
#endif

  NrInstalledItems = ConstructMenu();

#if 0
  for (cursor = 0; cursor < NrInstalledItems; cursor++)
    {
    sprintf(tbuff, "[%d] Next = 0x%x  Prev = 0x%x Nr = %d\n", (int)cursor, 
             (int)RAM_base_menu[cursor].NextItem, (int)RAM_base_menu[cursor].PrevItem, (int)RAM_base_menu[cursor].ItemNumber);
    Udr_PutNChar(tbuff);
    }
#endif

  /* start */
  WriteCodeLN1(0, PT5230HeadlineTxt);
  ClearRestLN1();
  WriteCodeLN2(0, SpaceTxt);            // Write header for menu line

  number  = 3;
  spacing = 3;

  RAMBaseNdx = RPtr->ItemNumber;        // Save Base Menu index
  tmpPtr     = RPtr;


  if (FuncTriggers & 0x80)        // If button pressed is RIGHT..
    {
    if (!(RAMBaseNdx % number))
      FirstItem = RPtr->ItemNumber;  // if Right...
    }

  if (FuncTriggers & 0x40)        // If button pressed is LEFT..
    {
    if (!(RAMBaseNdx % number))
      {
      FirstItem = RPtr->ItemNumber;  // if Left...
      }
    else
      {
      do
        {
        RPtr = RPtr->PrevItem;
        } while (RPtr->ItemNumber % number);
      FirstItem = RPtr->ItemNumber;
      }
    }

  if ((NrInstalledItems - FirstItem) < number)
    number = NrInstalledItems - FirstItem;

  FeedTheWatchdog;

  for (cursor = 1, i = FirstItem; i < FirstItem + number; i++)
    {
    WriteCode2(RAM_base_menu[i].ItemTxt);

    WriteTxt2("   ");

    if (RAM_base_menu[i].ItemNumber < RAMBaseNdx )
      cursor += (spacing + Cstrlen(RAM_base_menu[i].ItemTxt));
    }
  /* Neither BaseNdx nor RAMBaseNdx contain right ItemNumber, which is */
  /* used to invoke right function in BaseDown().  Given RAMBaseNdx we */
  /* must find which member of BaseItems it is and put the member to   */
  /* BaseNdx.                                                          */
   BaseNdx = FindBaseNdx(RAMBaseNdx);


#if 0
  sprintf(tbuff, "RAMBaseNdx = %d  FirstItem = %d\n", (int)RAMBaseNdx, (int)FirstItem);
  Udr_PutNChar(tbuff);
  sprintf(tbuff, "BaseNdx    = %d cursor = %d number = %d\n", (int)BaseNdx, (int)cursor, (int)number);
  Udr_PutNChar(tbuff);
#endif

  RPtr = tmpPtr;            // Restore menu pointer
  i    = RPtr->ItemConfig;  // Get info on line 1&2 icons/dots

  ClearRestLN2();
  WriteArrowsField(i);
  WriteItemArrows(cursor, Cstrlen(RPtr->ItemTxt));  // Mark selected item
  FuncTriggers = 0;


}


/***************************************************************************/
/*  ConstructMenu                                                  MENU.C  */
/*                                                                         */
/* Author:    Janusz Kuzminski                                             */
/* Revised:   12.09.2006                                                   */
/*                                                                         */
/*  Function: Construct RAM_base_menu[] array, containing only elements    */
/*            that are installed, copying those members of base_menu[]     */
/*            array which are installed (plus those which are always       */
/*            present).                                                    */
/*  Returns:  Number of menu items constructed.                            */
/*                                                                         */
/*  Remarks:  Installed members DO NOT hold their original ItemNumber, i.e */
/*            RAM_base_menu[].ItemNumber is an index to RAM_base_menu[]    */
/*            array and not to base_menu[]                                 */
/*  Updates:                                                               */
/***************************************************************************/
UC ConstructMenu(void)
  {
  UC i;
  UC bitems;

  i = 0;
  Cmemcpy(&RAM_base_menu[AnlBlkItem], &base_menu[i], sizeof(base_menu[AnlBlkItem])); // AnlBlkItem
#if 0
  sprintf(tbuff, "AnlBlkItem i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif

  if (TLGUnit[0].Present   || TLGUnit[4].Present   || TLGUnit[8].Present)
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[TLGItem], sizeof(base_menu[TLGItem])); // TLGItem
    RAM_base_menu[i].ItemNumber = i;  // change ItemNumber
#if 0
  sprintf(tbuff, "TLGItem i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[HDTPGItem], sizeof(base_menu[HDTPGItem])); // HDTPGItem
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "HDTPGItem i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[DLTPGItem], sizeof(base_menu[DLTPGItem])); // DLTPGItem
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "DLTPGItem i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (AnlTPGUnit[0].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[AnlTPG2Item], sizeof(base_menu[AnlTPG2Item])); // AnlTPG2Item
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "AnlTPG2Item i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (AnlTPGUnit[1].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[AnlTPG5Item], sizeof(base_menu[AnlTPG5Item])); // AnlTPG5Item
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "AnlTPG5Item i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (SDITSGUnit[0].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[SDITSG2Item], sizeof(base_menu[SDITSG2Item])); // SDI-TSG2
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "SDI-TSG2 i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (SDITSGUnit[1].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[SDITSG3Item], sizeof(base_menu[SDITSG3Item])); // SDI-TSG3
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "SDI-TSG3 i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (SDITSGUnit[2].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[SDITSG4Item], sizeof(base_menu[SDITSG4Item])); // SDI-TSG4
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "SDI-TSG4 i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (SDITPGUnit[0].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[SDITPG1Item], sizeof(base_menu[SDITPG1Item])); // SDI-TPG1
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "SDI-TPG1 i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (SDITPGUnit[1].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[SDITPG2Item], sizeof(base_menu[SDITPG2Item])); // SDI-TPG2
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "SDI-TPG2 i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (SDITPGUnit[2].Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[SDITPG5Item], sizeof(base_menu[SDITPG5Item])); // SDI-TPG5
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "SDI-TPG5 i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  if (AESEBUUnit.Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[AESEBUItem], sizeof(base_menu[AESEBUItem])); // AESEBU
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "AESEBU i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }

  // Now Genlock, Preset, Config and Test items
  i++;
  Cmemcpy(&RAM_base_menu[i], &base_menu[GenlockItem], sizeof(base_menu[GenlockItem])); // GENLOCK
  RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "GENLOCK i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif


  //Show LTC menu, only if GPS module is present
if (GPSGenlockUnit.Present || TimeCodeUnit.Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[LTCItem], sizeof(base_menu[LTCItem])); // LTC
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "LTC i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }
    
  //Show NTP menu, only if NTP module is present
if (NTPUnit.Present)                                         
    {
    i++;
    Cmemcpy(&RAM_base_menu[i], &base_menu[NTPItem], sizeof(base_menu[NTPItem])); // LTC
    RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "NTP i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif
    }    

  i++;
  Cmemcpy(&RAM_base_menu[i], &base_menu[PresetItem], sizeof(base_menu[PresetItem])); // PRESET
  RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "PRESET i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif

  i++;
  Cmemcpy(&RAM_base_menu[i], &base_menu[ConfigItem], sizeof(base_menu[ConfigItem])); // CONFIG
  RAM_base_menu[i].ItemNumber = i;
#if 0
  sprintf(tbuff, "CONFIG i = %d\n", (int)i);
  Udr_PutNChar(tbuff);
#endif

  i++;

  // This array will never have BaseMax elements, as all possible options
  // can never be installed.
  // Now we must set MENU_ITEM* NextItem and MENU_ITEM* PrevItem members of
  // all array elements to point to proper elements.

  bitems = i;  // our array has bitems elements

  for (i = 0; i < bitems; i++)
    RAM_base_menu[i].NextItem = &RAM_base_menu[i + 1];
  RAM_base_menu[bitems - 1].NextItem   = &RAM_base_menu[0];

  for (i = 1; i < bitems; i++)
    RAM_base_menu[i].PrevItem = &RAM_base_menu[i - 1];
  RAM_base_menu[0].PrevItem   = &RAM_base_menu[bitems - 1];

  return (bitems);  
  }


/***************************************************************************/
/*  BaseItemRight                                   MENUHLP.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980122                          */
/* Revised:    980122                                       */
/*                                                  */
/*  Function:  Select next item in the BASE MENU                  */
/*  Remarks:    A special test for the existence of the TEST-MENU is      */
/*          performed                                  */
/*  Returns:    --                                        */
/*  Updates:    Ptr                                       */
/***************************************************************************/
void BaseItemRight()
  {
  RPtr = RPtr->NextItem;
  FuncTriggers = (FuncTriggers & 0x3F) | 0x80;  // right rr
  }
/***************************************************************************/
/*  BaseItemLeft                                                MENUHLP.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980125                                    */
/* Revised:    980125                                                      */
/*                                                                         */
/*  Function:  Select previous item in the BASE MENU                       */
/*  Remarks:    A special test for the existence of the TEST-MENU is       */
/*          performed                                                      */
/*  Returns:    --                                                         */
/*  Updates:    Ptr                                                        */
/***************************************************************************/
void BaseItemLeft()
  {
  RPtr = RPtr->PrevItem;
  FuncTriggers = (FuncTriggers & 0x3F) | 0x40;  // left rr
  }

void ToBase()
  {
  RPtr = &RAM_base_menu[RAMBaseNdx];        // Restore Base-Menu pointer
  FuncTriggers = 0;
  }


/***************************************************************************/
/*  UC FindBaseNdx(UC rndx)                                        base.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980125                                    */
/* Revised:    980125                                                      */
/*                                                                         */
/*  Function:  Select previous item in the BASE MENU                       */
/*  Remarks:    A special test for the existence of the TEST-MENU is       */
/*          performed                                                      */
/*  Returns:    --                                                         */
/*  Updates:    Ptr                                                        */
/***************************************************************************/
UC FindBaseNdx(UC rndx)
  {
  UC i;

  for (i = 0; i < BaseMax; i++)
    {
    if (!CCstrcmp(RAM_base_menu[rndx].ItemTxt, base_menu[i].ItemTxt))
      return (i);
    }
  return (0xFF);
  }

int CCstrcmp(register code char * s1, register code char * s2)
  {
  register signed char  r;

  while(!(r = *s1 - *s2++) && *s1++)
    continue;

  return(r);
  }

