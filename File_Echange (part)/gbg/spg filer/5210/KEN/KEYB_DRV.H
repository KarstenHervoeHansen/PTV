/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		KEYB_DRV.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	971211, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970115: Keyb_Scan() renamed to Button_Ready										*/
/* 970115: Keyb_Scan() now returns if new button ready							*/
/***************************************************************************/

#define LEFT_Button		0x1E
#define DOWN_Button		0x1B
#define RIGHT_Button		0x17
#define UP_Button			0x1D

#define EXEC_Button		0x0F

#define OUTPUT_Button	0x3B
#define PRESET_Button	0x3D
#define GENLOCK_Button	0x2F

#define MASTER_Button	LEFT_Button & RIGHT_Button & EXEC_Button
#define FACTORY_Button	LEFT_Button & RIGHT_Button 

#define NO_Button			0x3F
#define NO_Command		0x3F

#define Keyb_Mask			0x3F
#define LED_Mask			0x07

#define MASTER_Button	LEFT_Button & RIGHT_Button & EXEC_Button
#define FACTORY_Button	LEFT_Button & RIGHT_Button 

#define Normal_Repeat_Freq		15
#define Fast_Repeat_Freq		4

extern UC Button;
extern UC Old_Button;
extern UC Button_Command;
extern UC Old_Button_Command;

extern UC Repeat_Count;
extern UC Repeat_Delay;
extern UC Repeat1_Delay;

extern bit Button_Enable;
extern bit Horizontal_Repeat;
extern bit Vertical_Repeat;
extern bit Repeat_On;
extern bit Combi_On;

extern UC Update_LEDs();
extern bit Button_Ready();
