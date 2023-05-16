/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MENU.H																		*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	971112, KEn VDE															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970924: removed UpToDiagnose()														*/
/* 970201: Added diagnose test functions												*/
/***************************************************************************/

// ...Ndx indicates the Item last active in menu:

extern near UC StatusNdx;					// Display item
extern near UC BaseNdx;						// Base-menu item
extern near UC AnlBlkNdx;					// Analog black menu item
extern near UC BBNdx[NoOfAnlBlkUnits];	// Black-burst menu items
extern near UC SDIBlkNdx;					// SDI black menu item
extern near UC SBNdx[NoOfSDIBlkUnits];	// Serial-black menu items
extern near UC AnlSigNdx;					// Analog-signal menu item
extern near UC SDISigNdx;					// Digital-signal menu item
extern near UC AES_EBUNdx;	  				// AES-EBU out item
extern near UC PresetNdx;	  				// Preset item
extern near UC ConfigNdx;	  				// Config item
extern near UC RS232Ndx;	 				// RS232 item
extern near UC DownloadNdx;  				// Download item
extern near UC DiagnoseNdx; 				// Diagnose item

extern UL ResponseKUNo;
extern UC ResponseSWVersion;

extern void PresetButtonSelect();
extern void OutputButtonSelect();
extern void GenlockButtonSelect();

extern void StatusPresetUpdate();
extern void StatusGenlockUpdate();
extern void StatusAnalogSignalUpdate();
extern void StatusSDISignalUpdate();
extern void StatusAudioUpdate();
extern void StatusErrorUpdate();
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
extern void BBTimeSelect();
extern void BBScHPhaseSelect();
extern void BBCopySelect();

extern void SDIBlkMenuShow();
extern void SDIBlkDown();
extern void UpToSDIBlk();

extern void SBMenuShow();
extern void SBDown();
extern void SBPatternSelect();
extern void SBSystemSelect();
extern void SBEDHSelect();
extern void SBAudioSelect();
extern void SBTimeSelect();
extern void SBCopySelect();

extern void AnalogMenuShow();
extern void AnalogDown();
extern void AnalogPatternSelect();
extern void AnalogTextSelect();
extern void AnalogSystemSelect();
extern void AnalogTimeSelect();
extern void AnalogScHPhaseSelect();

extern void SDIMenuShow();
extern void SDIDown();
extern void SDIPatternSelect();
extern void SDITextSelect();
extern void SDISystemSelect();
extern void SDIEDHSelect();
extern void SDIAudioSignalSelect();
extern void SDIAudioLevelSelect();
extern void SDITimeSelect();

extern void AES_EBUMenuShow();
extern void AES_EBUDown();
extern void AES_EBUSignalSelect();
extern void AES_EBULevelSelect();
extern void AES_EBUTimingSelect();

extern void GenlockInputShow();
extern void GenlockInputSelect();
extern void GenlockSystemSelect();
extern void GenlockTimeSelect();

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

extern void DownloadTypeSelect();
extern void UpToDownload();
extern void DownloadTestSPG();
extern void DownloadFromSPG();

extern void RS232MenuShow();
extern void RS232Down();
extern void UpToRS232();
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

extern void AnalogTimeAdjust( UC unit, UC System, long Delay);
extern void SDITimeAdjust( UC unit, UC System, long Delay);

