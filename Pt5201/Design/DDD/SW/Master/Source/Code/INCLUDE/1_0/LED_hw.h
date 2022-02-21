/****************************************************************************/
/* MODULE:                                                                  */
/*   LED_hw.h - LED driver																	 */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*  NONE                                                                    */
/*                                                                          */
/* TASKS:                                                                   */
/*  NONE                                                                    */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#ifndef _LED_HW_H
#define _LED_HW_H

#define BB1PALLED					0x0001
#define BB1NTSCLED				0x0002
#define BB2PALLED					0x0004
#define BB2NTSCLED				0x0008
#define BB3PALLED					0x0010
#define BB3NTSCLED				0x0020
#define CBPALLED					0x0040
#define CBNTSCLED					0x0080

#define AnalogAudioLED	 	0x0100
#define AESEBUAudioLED	 	0x0200
#define D25LED						0x0400
#define D27LED						0x0800
#define WordClk441LED			0x1000
#define WordClk48LED		 	0x2000
#define GenlockLED				0x4000
#define D37LED						0x8000

#define Test1LED					0x01
#define Test2LED					0x02

enum LEDstates						{ NormLEDs = 0, PresetLEDs, ResetLEDs, ErrorLEDs };

extern void LEDInit( void);
extern int LEDSet( UI LED, bool value);
extern int TestLEDSet( UI LED, bool value);
extern int SystemsLEDSet( UC state, UC column);

#endif