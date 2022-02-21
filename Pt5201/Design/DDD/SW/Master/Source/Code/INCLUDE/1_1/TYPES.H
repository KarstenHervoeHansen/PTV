/****************************************************************************/
/* MODULE:                                                                  */
/*   Types.h - settings and measure object defenitions.                     */
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

#ifndef _TYPES_

 #define _TYPES_

 /* SettingsItem are used as pointer to an alphabetical text in constant lists of settings */
 /*      ex. Auto escape */
 typedef const struct SI{
	const char *Text;
	const int Index;
	const struct SI *Next;
	const struct SI *Prev;
 } SettingsItem;


 /* MeasureItem are used as pointer to an alpahbetical text of actual measured value */
 /*      ex.  */
 typedef const struct MI{
	const char *Text;
	const int Index;
 } MeasureItem;


 typedef SettingsItem ListItem;


 /* SettingsIntRange are used as pointer to range for any setting of integer type */
 /*      ex. Display contrast */
 typedef const struct SIR{
	const int Min;
	const int Max;
	const int Step;
 } SettingsIntRange;


 /* SettingsDoubleRange are used as pointer to range for any setting of float and double type */
 /*      ex. Tuner frequency */
 typedef const struct DIR{
	const double Min;
	const double Max;
	const double Step;
 } SettingsDoubleRange;


#endif

