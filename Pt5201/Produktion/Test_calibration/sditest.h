
extern int	jitPNL,
			sdiPNL,
			amspPNL,
			pattPNL,
			rlossPNL,
			sdigenlPNL;
			
extern int XX1_Stik;

extern const float xd;  // remote genlock error

extern int SDIJitterTest(void);
extern int SDIAmplSpecTest(void);
extern int SDIReturnLossTest(void);
extern int CheckSDIEmbAudio_EDH(void);
extern int SDITotalTest(int Item,int Sel);


extern int SDIReturnLossMeasurement(void);
extern int ResetSDI(int SDINo);
