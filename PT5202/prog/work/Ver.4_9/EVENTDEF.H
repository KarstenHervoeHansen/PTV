/* Definition of all errors / events */

#ifndef _EVENTDEF_H
#define _EVENTDEF_H


#ifdef _EVLOG_C

#define EventName(num,txt)  {num,txt},

typedef struct 
{
   int   ID;
   char  *RomTxt;
}EventRom;

EventRom RomTable[]=
{
#else 
#define EventName(num,txt)
#endif

//	Range:			Global	Group	Specif.	Specif.
//				Type	Type	Group	Event

#define EventInstrument		1000
EventName(EventInstrument,"Instrument change")

#define EventPowerOn					1001
EventName(EventPowerOn,"Power ON")

#define EventPowerOff					1002
#define EventTemperatureFail				1003
#define EventPowerFail					1004

#define	EventSettingChanged		1100
EventName(EventSettingChanged,"Setting changed")

#define	EventRemoteChanged			1110
#define	EventBaudRateChanged				1111
#define EventParityChanged				1112
#define EventDataBitsChanged				1113
#define EventHandShakeChanged				1114
#define EventRemoteLocalState			1120
#define EventLocalLockOut				1121
#define EventLocalEnabled				1122
#define EventRemoteLockOut				1123
#define EventRemoteEnabled				1124
#define EventInstrumentClock			1130
#define EventClockSet					1131
#define EventAutoEscChanged				1132
#define EventAutoSwitchChanged				1133
#define	EventDisplayChanged			1140
#define	EventContrastChanged				1141
#define EventPreset			1200
EventName(EventPreset,"Preset")

#define EventPresetRecalled				1211
#define EventPresetSaved				1212
#define EventAlarms			1300
#define EventAlarm1				1310
#define EventAlarm1Set					1311
#define EventAlarm1Cleared				1312
#define EventAlarm2				1320
#define EventAlarm2Set					1321
#define EventAlarm2Cleared				1322
#define EventLogEvent		1400

#define	EventLogCleared					1411
EventName(EventLogCleared,"Eventlog cleared")
				
#define	EventLogAutoClear				1412
EventName(EventLogAutoClear,"Eventlog auto cleared")
				
/* Project specific instrument events */
#define EventSpecificInstrument	2000
EventName(EventSpecificInstrument,"Setting changed")

#define EventSpecificChanged		2100				

#define EventFrontEndChanged			2110				
#define EventInputChanged				2111
#define EventUserFreqChanged				2112
#define EventModeChanged			2120
#define EventFFTChanged					2121
#define EventPolarityChanged				2122
#define EventConstChanged				2123
#define EventGuardChanged				2124
#define EventCodeRateChanged				2125
#define EventAutoModeChanged				2129

#define EventNoiseChanged				2130
#define EventNoiseOn				2131
#define EventNoiseOff				2132
#define EventNoiseValChanged		2133

/* General Instrument Loggin */


/*Internal communication */
#define EventIntCommError				3000
EventName(EventIntCommError,"Internal comm error")
/*IIC*/

#define EventIICError		        3300	
#define EventIICNoAck	        	3301
#define EventIICTimeout	        3302
#define EventIICBusError        3303
#define EventIICNoAdrAck				3304
#define EventIICNoBus						3309

// Specific bus errors can be calculated from general above :
// Fx : EventIIC5BusError = EventIICBusError + 5 * 10;


#define EventIIC1Error	        3310	
#define EventIIC1NoAck	        3311
#define EventIIC1Timeout        3312
#define EventIIC1BusError       3313

#define EventIIC2Error					3320
#define EventIIC2NoAck		 			3321
#define EventIIC2Timeout				3322
#define EventIIC2BusError				3323

#define EventIIC3Error					3330
#define EventIIC3NoAck					3331
#define EventIIC3Timeout				3332
#define EventIIC3BusError				3333

//Reserved up to 3399 for IIC events

/*V24*/
#define EventSIO0Error		      3400	
EventName(EventSIO0Error,"Internal V24 comm error")

#define EventSIO0NoAck					3401
#define EventSIO0TxCksumErr	 		3402
#define EventSIO0RxCksumErr	    3403
#define EventSIO0RxTimeout			3404



/* Codes for software debugging */

#define EventSWbug							6000
EventName(EventSWbug,"SW info")

#define EventNotInitialised			6101	/* Function called before required initialisation */
#define EventRepeatedInit				6102	/* 2nd (or more) call to same init */

#define EventNoMoreDynamic			6110	
#define EventNoMoreResources	  6111
#define EventNoMoreEvents				6112
#define EventNoMoreTimers				6113
#define EventNoMoreThreads      6114
#define EventNoMoreMailboxes    6115
#define EventNoMoreStreams			6116
#define EventOutOfMemmory				6117



#define EventParameterError			6200

#define EventSwitchDefaults			6221

#define EventTracing				   	6300
#define EventTracingTask		    6310
#define EventTracingFunction		6320


#ifdef _EVLOG_C
};
#endif

#endif