/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MENU.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980115																		*/
/*	Rev. date:	000508, KEn, DEV															*/
/*	Status:		Version 1.2																	*/
/*																									*/
/*	Changes:																						*/
/* 000508: Support for positioning in TPG text										*/
/* 980825: Status??Update changed to Status??Show									*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

// Ndx indicates the last active item in menu:

extern near UC StatusNdx;								// Display item
extern near UC BaseNdx;									// Base-menu item

extern near UC AnlBlkNdx;								// Black-burst submenu item
extern near UC BBNdx[MaxAnlBlkUnits];				// Black-burst 2ndmenu item

extern near UC AnlTPGNdx[MaxAnlTPGUnits];			// Analog-TPG submenu item

extern near UC SDITSGNdx[MaxSDITSGUnits];			// SDI-TSG submenu item
extern near UC SDITSGAudioNdx[MaxSDITSGUnits];	// SDI-TSG audio 2ndmenu item

extern near UC SDITPGNdx[MaxSDITPGUnits];			// SDI-TPG submenu item
extern near UC SDITPGTextNdx[MaxSDITPGUnits];	// SDI-TPG text 2ndmenu item
extern near UC SDITPGAudioNdx[MaxSDITPGUnits];	// SDI-TPG audio 2ndmenu item

extern near UC AESEBUnNdx;	  							// AES-EBU submenu item
extern near UC AESEBUNdx[MaxAESEBUUnits];			// AES-EBU 2ndmenu item

extern near UC PresetNdx;	  							// Preset submenu item
extern near UC ConfigNdx;	  							// Config submenu item
extern near UC DateTimeNdx;							// Date/Time 2ndmenu item
extern near UC RS232Ndx;	 							// RS232 2ndmenu item
extern near UC DownloadNdx;  							// Download select item
extern near UC DiagnoseNdx; 							// Diagnose select item

extern UL ResponseKUNo;									// TBD
extern UC ResponseSWVersion;							// TBD

extern void GroupButtonSelect( UC group);
extern void PresetButtonSelect();
extern void OutputButtonSelect();

extern void StatusPresetShow();
extern void StatusGenlockShow();
extern void StatusAnlTPGShow();
extern void StatusSDITSGShow();
extern void StatusSDITPGShow();
extern void StatusAESEBUShow();
extern void StatusDateTimeShow();
extern void StatusErrorShow();
extern void StatusDown();
extern void UpToStatus();

extern void BaseMenuShow();
extern void BaseDown();
extern void UpToBase();

extern void AnlBlkMenuShow();
extern void AnlBlkDown();
extern void UpToAnlBlk();

extern void BBMenuShow();
extern void BBDown();
extern void BBSystemSelect();
extern void BBTimingSelect();
extern void BBScHPhaseSelect();

extern void AnlTPGMenuShow();
extern void AnlTPGDown();
extern void AnlTPGPatternSelect();
extern void AnlTPGPatternModify();
extern void AnlTPGSystemSelect();
extern void AnlTPGTimingSelect();
extern void AnlTPGScHPhaseSelect();

extern void UpToAnlTPG();

extern void AnlTPGTextMenuShow();
extern void AnlTPGTextDown();
extern void AnlTPGTextEditSelect();
extern void AnlTPGTextStyleSelect();
extern void AnlTPGTextPosSelect();
extern void AnlTPGTextClockSelect();

extern void SDITSGMenuShow();
extern void SDITSGDown();
extern void SDITSGPatternSelect();
extern void SDITSGSystemSelect();
extern void SDITSGEDHSelect();
extern void SDITSGTimingSelect();

extern void SDITSGAudioMenuShow();
extern void SDITSGAudioDown();
extern void UpToSDITSG();
extern void SDITSGAudioSignalSelect();
extern void SDITSGAudioLevelSelect();

extern void SDITPGMenuShow();
extern void SDITPGDown();
extern void SDITPGPatternSelect();
extern void SDITPGPatternModify();
extern void SDITPGSystemSelect();
extern void SDITPGEDHSelect();
extern void SDITPGTimingSelect();

extern void UpToSDITPG();

extern void SDITPGTextMenuShow();
extern void SDITPGTextDown();
extern void SDITPGTextEditSelect();
extern void SDITPGTextStyleSelect();
extern void SDITPGTextPosSelect();
extern void SDITPGTextClockSelect();

extern void SDITPGAudioMenuShow();
extern void SDITPGAudioDown();
extern void SDITPGAudioGroupSelect();
extern void SDITPGAudioSignalSelect();
extern void SDITPGAudioLevelSelect();

extern void AESEBUnMenuShow();
extern void AESEBUnDown();
extern void UpToAESEBUn();

extern void AESEBUMenuShow();
extern void AESEBUDown();
extern void AESEBUSignalSelect();
extern void AESEBULevelSelect();
extern void AESEBUTimingSelect();

extern void GenlockInputShow();
extern void GenlockInputSelect();
extern void GenlockSystemSelect();
extern void GenlockTimingSelect();

extern void PresetMenuShow();
extern void PresetDown();
extern void PresetRecallSelect();
extern void PresetStoreSelect();
extern void PresetNameSelect();

extern void ConfigMenuShow();
extern void ConfigDown();
extern void UpToConfig();
extern void ConfigLockSelect();
extern void ConfigAutoESCSelect();
extern void ConfigLCDContSelect();

extern void DateTimeMenuShow();
extern void DateTimeDown();
extern void DateTimeDateSelect();
extern void DateTimeTimeSelect();
extern void DateTimeRefSelect();
extern void DateTimeOffsetSelect();

extern void DownloadTypeSelect();
extern void UpToDownload();
extern void DownloadTestSPG();
extern void DownloadFromSPG();

extern void RS232MenuShow();
extern void RS232Down();
extern void RS232BaudrateSelect();
extern void RS232DatabitSelect();
extern void RS232ParitySelect();
extern void RS232HandshakeSelect();

extern void DiagnoseMenuShow();
extern void DiagnoseDown();

extern void DiagnoseMainTest();
extern void DiagnoseOptionsTest();
extern void DiagnoseRS232Test();
extern void DiagnoseMemoryTest();
extern void DiagnoseDisplayTest();
extern void DiagnoseKeyboardTest();
extern void DiagnoseConfigTest();
extern void DiagnoseErrorQueueTest();
extern void DiagnoseErrorQueueReset();

extern void AnlTimeAdjust( UC unit, UC System, long Delay);
extern void SDITimeAdjust( UC unit, UC System, long Delay);
