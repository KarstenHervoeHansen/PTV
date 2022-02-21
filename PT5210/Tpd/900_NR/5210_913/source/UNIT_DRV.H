/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		UNIT_DRV.H																	*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	960904																		*/
/*	Rev. date:	991007, KEn VDE															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	Changes:																						*/
/* 991007: Added support for two AES-EBU generators, (PT8635)					*//* 990614: Corrected header text in ConfigLockSelect								*/
/* 971118: Altered parameters in GetV24Version()									*/
/* 971027: Added TestMainboard(), TestSPGUnit()										*/
/* 971024: Added TestAnlBlkUnit(), TestSDIBlkUnit(), TestAnalogSignalUnit()*/
/*			  TestSDISignalUnit(), TestBBMulUnit(), TestSDIGenlockUnit(),		*/
/*			  TestTimeCodeUnit(), TstHighPrecisionOCX()								*/
/* 970917: Added parameter offset to GetIICVersion()								*/
/* 970225: Added GetIIC- and GetV24version											*/
/* 970113: Added ConfigurePreset( UC no)												*/
/***************************************************************************/

extern UC ConfigureSPG();

extern UC ConfigureAnlBlkUnit( UC no);
extern UC ConfigureSDIBlkUnit( UC no);

extern UC ConfigureAnalogSignalUnit();
extern UC ConfigureSDISignalUnit();

extern UC AES_EBUUnitDrv( UC ndx, UC Signal, UC Level, UC Timing);
extern UC GenlockUnitDrv( UC GenlockInput, UC GenlockSystem);

extern UC TrxAnlBlkSystem( UC index, UC system);
extern UC TrxAnlBlkDelay( UC index, UL delay);
extern UC TrxAnlBlkScHPhase( UC index, int ScHPhase);

extern UC TrxSDIBlkSystem( UC index, UC system);
extern UC TrxSDIBlkPattern( UC index, UC pattern);
extern UC TrxSDIBlkDelay( UC index, UL delay);
extern UC TrxSDIBlkEDH( UC index, UC state);
extern UC TrxSDIBlkAudio( UC index, UC audio);

extern UC TrxAnalogPattern( UC pattern);
extern UC TrxAnalogText( UC state, char* text);
extern UC TrxAnalogSystem( UC system);
extern UC TrxAnalogDelay( UL delay);
extern UC TrxAnalogScHPhase( int ScHPhase);

extern UC TrxSDIPattern( UC pattern);
extern UC TrxSDIText( UC state, char* text);
extern UC TrxSDISystem( UC system);
extern UC TrxSDIDelay( UL delay);
extern UC TrxSDIEDH( UC state);
extern UC TrxSDIAudioSignal( UC audiosignal);
extern UC TrxSDIAudioLevel( UC audiolevel);

extern UC TrxGenlockDelay( UL delay);

extern UC ConfigurePreset( UC No);
extern void StorePreset( UC No);

extern void GetV24Version( UC address, UC type, struct CONN_STRUCTURE* Connector);
extern void GetIICVersion( UC bus, UC address, UC KU_address, UL* KU);

extern UC TestMainboard( void);
extern UC TestSPGUnit( void);
extern UC TestAES_EBUUnit( void);
extern UC TestAnlBlkUnit( UC conn);
extern UC TestSDIBlkUnit( UC conn);
extern UC TestAnalogSignalUnit( void);
extern UC TestSDISignalUnit( void);
extern UC TestBBMulUnit( void);
extern UC TestSDIGenlockUnit( void);
extern UC TestTimecodeUnit( void);
extern UC TestHighPrecisionOCXUnit( void);
