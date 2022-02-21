/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		FACTORY.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980121																		*/
/*	Rev. date:	991025, KEn, DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	Changes:																						*/
/* 991025: "Master reset" is no onger supported										*/
/* 980918: Added reference to TimeClockConfigFactory								*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

extern code ANLBLK_STRUCT AnlBlkConfigFactory[MaxAnlBlkUnits];
extern code ANLTPG_STRUCT AnlTPGConfigFactory[MaxAnlTPGUnits];
extern code SDITSG_STRUCT SDITSGConfigFactory[MaxSDITSGUnits];
extern code SDITPG_STRUCT SDITPGConfigFactory[MaxSDITPGUnits];
extern code AESEBU_STRUCT AESEBUConfigFactory[MaxAESEBUUnits];
extern code GENLOCK_STRUCT GenlockConfigFactory[MaxGenlockUnits];
extern code TIMECLOCK_STRUCT TimeClockConfigFactory;
extern code RS232_STRUCT RS232ConfigFactory;

extern void FactoryPreset();
