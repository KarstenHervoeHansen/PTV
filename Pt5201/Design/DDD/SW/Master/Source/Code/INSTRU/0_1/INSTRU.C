/****************************************************************************/
/* MODULE:                                                                  */
/*  instru.c - Instrument control task                                      */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
    void instru(void);
/*                                                                          */
/* NOTES:                                                                   */
/*  Receives command messages form user control tasks, processes them and   */                               
/*  replies according to command.                                           */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "define.h"
#include "eventdef.h"
#include "rtxcobj.h"
#include "rs232.h"
#include "sio0drv.h"
#include "tim0drv.h"
#include "i2c_drv.h"
#include "tvp6kdrv.h"
#include "unitprg.h"
#include "led_hw.h"
#include "genlock.h"
#include "bb.h"
#include "cbar.h"
#include "audio.h"
#include "tables.h"
#include "keyscan.h"

#include "instru.h"
#include "interp.h"

TTimer*	 InstruTimer;
TThread* InstrumentControl;
TThread* InterpreterThread;
TThread* KeyScanThread;

/* TASK instru  */
void Instrument_Control_Task( void)
{
	/* VAR */
 	bool Test1LEDFlag = false;
	char result[5];

  /* BEGIN */
  rs232_init();				  			// init rs232 driver
  sio0_init();				 	 			// init sio0 driver
	tim0_init();								// init T0 counter for field												
  InitBus( IIC_PTV);					// init IIC bus
	tvp6k_init();								// init TI TVP6000 Multimedie chip

	GenlockInit();	 						// init genlock
	CBInit();										// init colorbar (PAL,EBU ColorBar)
	BBInit();
	AudioInit();

  KeyScanThread = newTThread( 3, keyscan);
	InterpreterThread = newTThread( 5, Interpreter);

  KeyScanThread->Execute( KeyScanThread);
  InterpreterThread->Execute( InterpreterThread);

  InstruTimer = newTTimer();
  InstruTimer->SetPeriod( InstruTimer, 250);

  for(;;)
	{
		InstruTimer->TimeOut->Wait( InstruTimer->TimeOut, 0);
			

		if (( GenlockSetup.System == BurstLockPAL) || ( GenlockSetup.System == BurstLockNTSC))
		{
			sio0SndRequest( 12, SPG_V24_address, result, "GS?");

			LEDSet( GenlockLED, ( result[0] & 0x01) > 0);
		}

		Test1LEDFlag ^= 1;
		TestLEDSet( Test1LED, Test1LEDFlag);
  }
}/* END instru */























