
extern int DACData[8][2];


extern int syncPNL;
extern int phasePNL;



extern int MakeSyncOffsetCal(int BBNo, int CalType);
extern int ReadCalData(void);
extern int InitBB(int BBNo);
extern int MakeOffsetCal(int BBNo);
extern int MakePhaseCal(int BBNo, int TVSys);
extern int CheckPowerSupply(int NomVolt);
extern int CheckFan(void);
extern int CheckOven(void);
extern int CheckAudioGenVCO(void);
extern int ShowCalFiles(void);
extern char *ReadBBKUNumber (int BB);
extern int CheckLTCVCO(int TVSystem);
