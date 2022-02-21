/****************************************************************************/
/* MODULE:                                                                  */
/*   TInt.c - TInt object properties and methodes.                          */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5765
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#ifndef _OBJECTID_H_
 #define _OBJECTID_H_


enum ObjID {

TThreadID=0,
TEventID,
TEventListID,
TTimerID,
TResourceID,
TMailBoxID,
TStreamID,
TListItemID,
TListID,
TListsID,
TStringsID,
TIntID,
TFloatID,
TEventLogID,
TEventLogItemID,
TEventLogEventID,
TMaskID,
TAlarmsID,
TAlarmID,
TAlarmSourceID,
TFormID,
TPageControlID,
TTabSheetID,
TButtonID,
TListBoxID,
TLabelID,
TEditID,
TValueEditID,
TProgressBarID,
TTrackBarID,
TMemoID,
TFileCBID,
TEmptyID
};

#define TEVENTLIST_COUNT 5


#define TLISTITEM_COUNT		128
#define TLIST_COUNT		64
#define TLISTS_COUNT		4
#define TINT_COUNT		16
#define TFLOAT_COUNT		16
#define TEVENTLOG_COUNT		1
#define TEVENTLOGITEM_COUNT	50



#endif