/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MENUHLP.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	971211, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970923: Added Type 11 state machine													*/
/* 961229: Added StatusItemLeft & StatusItemRight									*/
/***************************************************************************/

#define NONE_	0x00
#define L_		0x01
#define D_     0x02
#define R_		0x04
#define U_     0x08
#define E_		0x10
#define P_		0x20
#define MORE_	0x40

#define F_		0x80				// Free to use


extern UC FuncState;				// Current state in state machine

extern UC FuncTriggers;			// The value <yy>, <xx>, <zz> or <rr> has
										//  been altered
extern bit FuncSave;				// 1: Activate Save function
extern bit FuncEscape;			// 1: Activate Escape function


// Help functions for writing on the display

extern void WriteTestArrows( UC NoOfArrows);
extern void WriteItemArrows( UC start, UC length);
extern void WriteItemBrackets( UC start, UC length);
extern void WriteArrowsField( UC No);

extern void StatusItemLeft();
extern void StatusItemRight();

extern void BaseItemLeft();
extern void BaseItemRight();

extern void AnlBlkItemLeft();
extern void AnlBlkItemRight();
extern void SDIBlkItemLeft();
extern void SDIBlkItemRight();

extern void ItemLeft();
extern void ItemRight();

extern void EscFunction();

// Functions and state machines for <SAVE ESC> display handling

extern void WriteType1_SAVE_ESC( char FuncState);
extern void UpButType1();
extern void DownButType1();
extern void LeftButType1();
extern void RightButType1();
extern void ExecButType1();


// Functions and state machines for <XY SAVE ESC> display handling

extern void WriteType2_SAVE_ESC( char FuncState);
extern void UpButType2();
extern void DownButType2();
extern void LeftButType2();
extern void RightButType2();
extern void ExecButType2();


// Functions and state machines for <STORE SAVE ESC> display handling

extern void WriteType3_OK_ESC( char FuncState);
extern void UpButType3();
extern void DownButType3();
extern void LeftButType3();
extern void RightButType3();
extern void ExecButType3();


// Functions and state machines for <INSTANT ADJUST LEVEL> display handling

extern void WriteType4_SAVE_ESC( char FuncState);
extern void UpButType4();
extern void DownButType4();
extern void LeftButType4();
extern void RightButType4();


// Functions and state machines for <EXECUTE TEXT ESC> display handling

extern void WriteType5_SAVE_ESC( char FuncState);
extern void UpButType5();
extern void DownButType5();
extern void LeftButType5();
extern void RightButType5();
extern void ExecButType5();

// Functions and state machines for <XY EDIT SAVE ESC> display handling

extern void WriteType6_SAVE_ESC( char FuncState);
extern void UpButType6();
extern void DownButType6();
extern void LeftButType6();
extern void RightButType6();
extern void ExecButType6();

// Functions and state machines for <USER TEXT SAVE ESC> display handling

extern void WriteType7_SAVE_ESC( char FuncState);
extern void UpButType7();
extern void DownButType7();
extern void LeftButType7();
extern void RightButType7();
extern void ExecButType7();

// Functions and state machines for <USER TEXT SAVE ESC> display handling

extern void WriteType8_SAVE_ESC( char FuncState);
extern void UpButType8();
extern void DownButType8();
extern void LeftButType8();
extern void RightButType8();
extern void ExecButType8();

// Functions and state machines for <EDIT SAVE ESC> display handling

extern void WriteType9_SAVE_ESC( char FuncState);
extern void UpButType9();
extern void DownButType9();
extern void LeftButType9();
extern void RightButType9();
extern void ExecButType9();

// Functions and state machines for <OK ESC> display handling

extern void WriteType10_OK_ESC( char FuncState);
extern void UpButType10();
extern void LeftButType10();
extern void RightButType10();
extern void ExecButType10();

// Functions and state machines for <OK FAIL> display handling

extern void WriteType11_OK_FAIL( char FuncState);
extern void UpButType11();
extern void ExecButType11();
