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



#define EventMeasureChanged		2200
EventName(EventMeasureChanged,"Measuring event")

#define EventLevelChanged			2210
#define EventLevelBelowChanged				2212
#define EventLevelAboveChanged				2213

#define EventSyncUnlocked			2230
EventName(EventSyncUnlocked,"Lock change")

#define EventTIMUnlocked				2231
#define EventAFCUnlocked				2232
#define EventTPSUnlocked				2233
#define EventFrameUnlocked				2234
#define EventFECUnlocked				2235

#define EventSyncLocked				2240
EventName(EventSyncLocked,"Lock change")

#define EventTIMlocked					2241
#define EventAFClocked					2242
#define EventTPSlocked					2243
#define EventFramelocked				2244
#define EventFEClocked					2245
#define EventTPSChanged				2220
#define EventTPSFFTChanged				2221
#define EventTPSConstChanged				2223
#define EventTPSGuardChanged				2224
#define EventTPSCodeRateHPChanged			2225
#define EventTPSCodeRateLPChanged			2226
#define EventTPSHierarchyChanged			2227



/* General Instrument Loggin */


/*Internal communication */
#define EventIntCommError	3000
EventName(EventIntCommError,"Internal comm error")
/*IIC*/

#define EventIICError		        3300	
#define EventIICNoAck	        			3301
#define EventIICTimeout	        			3302
#define EventIICBusError        			3303
#define EventIICNoAdrAck				3304
#define EventIICNoBus					3309

// Specific bus errors can be calculated from general above :
// Fx : EventIIC5BusError = EventIICBusError + 5 * 10;


#define EventIIC1Error	        		3310	
#define EventIIC1NoAck	        			3311
#define EventIIC1Timeout        			3312
#define EventIIC1BusError       			3313

#define EventIIC2Error				3320
#define EventIIC2NoAck		 			3321
#define EventIIC2Timeout				3322
#define EventIIC2BusError				3323

#define EventIIC3Error				3330
#define EventIIC3NoAck					3331
#define EventIIC3Timeout				3332
#define EventIIC3BusError				3333





//Reserved up to 3399 for IIC events



/* Device specific events */
#define EventDeviceError	4000
EventName(EventDeviceError,"Internal device error")

#define EventChipError			4100
#define EventDemodError				4110
#define EventDemodNoGen					4111
#define EventDemodNoAck					4112

#define EventFECError				4120
#define EventFECNoGen					4121
#define EventFECNoAck					4122

#define EventDemodNoExp				4130		/* IIC expander on Demod board*/

#define EventNoiseNoAck				4142		/* IIC expander on Demod board*/



/* Codes for software debugging */

#define EventSWbug		6000
EventName(EventSWbug,"SW info")

#define EventNotInitialised				6101	/* Function called before required initialisation */
#define EventRepeatedInit				6102	/* 2nd (or more) call to same init */

#define EventNoMoreDynamic			6110	
#define EventNoMoreResources	  		6111
#define EventNoMoreEvents				6112
#define EventNoMoreTimers				6113
#define EventNoMoreThreads          6114
#define EventNoMoreMailboxes        6115
#define EventNoMoreStreams				6116
#define EventOutOfMemmory				6117



#define EventParameterError		6200

#define EventSwitchDefaults				6221

#define EventTracing				   6300
#define EventTracingTask		     6310
#define EventTracingFunction		  6320


#ifdef _EVLOG_C
};
#endif

#endif