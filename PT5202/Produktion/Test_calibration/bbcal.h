
extern int DACData[8][2];

extern int syncPNL;
extern int phasePNL;

extern int InitPT5201(void);
extern int MakeSyncOffsetCal(int CalType);
extern int ReadCalData1(void);

extern int MakeOffsetCal(int BBNo);	// BB generator
extern int MakeBBPhaseCal(int TVSys);

extern int MakeTSGPhaseCal(int TVSys);	// Color bar generator
extern int CheckTPGPattern(void);

extern int CheckPowerSupply(int NomVolt);

extern int CheckOven(void);
extern int CheckAudioGenVCO(int VCONo);

extern int SetStartupSystem5201(void);
extern int WriteFMSUpdate(void);
extern int ReadDCVolt(double *result);
extern char *ReadMasterIDStr (void);
extern char *ReadKUStr (void);
extern char *ReadBBKUNumber (int BB);
extern char *ReadBBCalDate (int BB);
extern char *ReadTSGCalDate (void);
extern char *ReadBBSWVer (void);
extern char *ReadSDISWVer (int SDI);
extern char *ReadMasterSWVer (void);
extern int TPGTotalTest(int TestNo);
