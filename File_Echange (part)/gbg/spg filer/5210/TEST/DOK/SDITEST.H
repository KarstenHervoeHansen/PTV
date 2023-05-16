
extern int	jitPNL,
			sdiPNL,
			amspPNL,
			sdipattPNL,
			rlossPNL,
			sdigenlPNL;

extern int SDICal(int TVLines, int SDINo);
extern int CheckSDIPattern(int SDINo);
extern int SDIJitterTest(int SDINo);
extern int SDIAmplSpecTest(int SDINo);
extern int SDIReturnLossTest(int SDINo);
extern int SDIGenlockTest(void);
extern int CheckSDIEmbAudio_EDH(int SDINo);
extern int SDITotalTest(int Item,int Sel);


extern char *ReadSDIKUNumber (int SDINo);
extern char *ReadSDIProdDate (int SDINo);
extern char *ReadSDICalDate (int SDINo);
extern char *ReadRLossInEEPROM (int SDINo);
extern char *ReadJitterInEEPROM (int SDINo);
extern char *ReadSDIGenlKUNumber (void);
extern char *ReadSDIGenlProdDate (void);
extern char *ReadSDIGenlCalDate (void);
