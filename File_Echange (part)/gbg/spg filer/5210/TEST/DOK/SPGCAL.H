extern int 
	AutoHPH_PALOk,
	AutoHPH_NTSCOk,
	GenlSystemOk,
	GenlInputOk;
	
extern int TV;	

extern int MakeSPGGenlockCal(int TVSys);
extern int MakeSPGGenlockSystem(void);
extern int MakeSPGGenlockInput(void);
extern int MakeSPGHPHCal(void);
extern int MakeOCXOCal(int Freq);
extern int MakeSPGReset(void);


extern char *ReadSPGKUNumber (void);
extern char *ReadSPGProdDate (void);
extern char *ReadSPGCalDate (void);

extern int BasicTotalTest(int Sel);
