	
extern int TV;	

extern int MakeSPGGenlockCal(int TVSys);
extern int TestSPGGenlockSystem(void);
extern int MakeSPGHPHCal(void);
extern int MakeRefCal(int Freq);
extern int MakeSPGReset(void);
extern int MeasWithHP53132(void);


extern char *ReadSPGSWVer (void);
extern char *ReadSPGCalDate (void);

extern int BasicTotalTest(int Sel);
