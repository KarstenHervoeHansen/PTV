/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		UNIT_DRV.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980122																		*/
/*	Rev. date:	000608, KEn, DEV															*/
/*	Status:		Version 1.2																	*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added suppot for TPG text positioning									*/
/* 980828: Added BroadcastTCITime()														*/
/* 980708: Added TrxTimeClockControl()													*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

extern void FindStaticOptions();
extern void FindDynamicOptions( UC conn_addr);

extern UC FindUnit( UC dir, UC unit);
extern UC FindNextTSGTPG( UC unit);

extern UC ConfigureSPG();
extern UC ConfigureAnlBlkUnit( UC ndx);
extern UC ConfigureAnlTPGUnit( UC ndx);
extern UC ConfigureAnlTPGText( UC ndx, UC pattern);
extern UC ConfigureSDITSGUnit( UC ndx);
extern UC ConfigureSDITPGUnit( UC ndx);
extern UC ConfigureSDITPGText( UC ndx, UC pattern);

extern UC AESEBUUnitDrv( UC ndx, UC Signal, UC Level, UC Timing);
extern UC GenlockUnitDrv( UC GenlockInput, UC GenlockSystem);

extern UC BroadcastTCITime();

extern UC TrxAnlBlkSystem( UC ndx, UC system, UL delay, int ScHPhase);
extern UC TrxAnlBlkDelay( UC ndx, UC system, UL delay);
extern UC TrxAnlBlkScHPhase( UC ndx, int ScHPhase);

extern UC TrxAnlTPGSystem( UC ndx, UC system, UL delay, UC pattern, UC attrib);
extern UC TrxAnlTPGDelay( UC ndx, UC system, UL delay);
extern UC TrxAnlTPGPattern( UC ndx, UC pattern, UC attrib);
extern UC TrxAnlTPGScHPhase( UC ndx, int ScHPhase);
extern UC TrxAnlTPGTextStyle( UC ndx, UC pattern, UC style);
extern UC TrxAnlTPGTextPos( UC ndx, UC style, UC Xpos, UC Ypos);
extern UC TrxAnlTPGTextLineEnable( UC ndx, UC no, UC state);
extern UC TrxAnlTPGText( UC ndx, UC no, char* text);

extern UC TrxSDITSGSystem( UC ndx, UC system, UL delay, UC pattern);
extern UC TrxSDITSGDelay( UC ndx, UC system, UL delay);
extern UC TrxSDITSGPattern( UC ndx, UC pattern);
extern UC TrxSDITSGEDH( UC ndx, UC state);
extern UC TrxSDITSGAudioSignal( UC ndx, UC level);
extern UC TrxSDITSGAudioLevel( UC ndx, UC signal);

extern UC TrxSDITPGSystem( UC ndx, UC system, UL delay, UC pattern, UC attrib);
extern UC TrxSDITPGDelay( UC ndx, UC system, UL delay);
extern UC TrxSDITPGPattern( UC ndx, UC pattern, UC attrib);
extern UC TrxSDITPGEDH( UC ndx, UC state);
extern UC TrxSDITPGTextStyle( UC ndx, UC pattern, UC style);
extern UC TrxSDITPGTextPos( UC ndx, UC style, UC Xpos, UC Ypos);
extern UC TrxSDITPGTextLineEnable( UC ndx, UC no, UC state);
extern UC TrxSDITPGText( UC ndx, UC no, char* text);
extern UC TrxSDITPGAudioSignal( UC ndx, UC signal);
extern UC TrxSDITPGAudioLevel( UC ndx, UC level);
extern UC TrxSDITPGAudioGroup( UC ndx, UC group);

extern UC TrxTimeClockControl( UC control);
extern UC TrxTimeClockData( UC TCIControl, UC noOfBytes, UL val);

extern UC TrxGenlockDelay( UL delay);

extern UC ConfigurePreset( UC No);

extern UC GetV24Version( UC ndx, UC address, UC HWtype);
extern UC GetIICVersion( UC bus, UC address, UC KU_address, UL* KU);

extern UC TestMainboard( void);
extern UC TestSPGUnit( void);
extern UC TestAESEBUUnit();
extern UC TestAnlBlkUnit( UC ndx);
extern UC TestAnlTPGUnit( UC ndx);
extern UC TestSDITSGUnit( UC ndx);
extern UC TestSDITPGUnit( UC ndx);
extern UC TestBBMulUnit( void);
extern UC TestSDIGenlockUnit( void);
extern UC TestTimeClockUnit( void);
extern UC TestHighPrecisionOCXUnit( void);
