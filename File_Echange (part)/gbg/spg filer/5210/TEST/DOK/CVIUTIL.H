
/************** Static Function Declarations **************/

#define NO 0
#define YES 1
#define TRUE 1
#define FALSE 0
#define LF 10
#define CR 13
#define WAIT 1
#define NOWAIT 0
#define OK 0
#define NOTOK 1
#define ON 1
#define OFF 0

#define SYNC_CAL 0
#define OFFSET_CAL 1
#define PAL 0
#define NTSC 1
#define L625 625
#define L525 525


#define REARTERM34401 0
#define FRONTTERM34401 1

#define MAIN 		0
#define AUDIOGEN 	1
#define SDIGENLOCK 	2
#define DUALBB 		3
#define SDIBLACK 	4
#define SDITSG 		5
#define BBBUFFER 	6
#define SPG 		7
#define ANALOGTSG 	8
#define TIMECODE 	9

#define GENL_PAL		0
#define GENL_NTSC		1
#define GENL_443_HIGH	2
#define GENL_443_LOW	3
#define GENL_358_HIGH	4
#define GENL_358_LOW	5
#define GENL_10M_HIGH	6
#define GENL_10M_LOW	7
#define GENL_OFF		10


#define DUT_LOCKED 0x02
#define DUT_INTERN 0x04
 
extern const char *NC12[];
extern const char *V24Addr[];

extern int mainPNL;
extern int cfgPNL;
extern int dataPNL;
extern int spghphPNL;
extern int aesPNL;
extern int vccPNL;
extern int fanPNL;
extern int ocxoPNL;
extern int sditotPNL;
extern int basictotPNL;
extern int spggenlPNL;
extern int spgphasePNL;

extern const double VLMGain;
extern const double NomSyncLevel;
extern const double SyncLevelTol;
extern const double NomDCOffset;
extern const double DCOffsetTol;


/************** Global Variable Declarations **************/
extern const int IOAddr;
extern const int DUTCOM;				// Device Under Test   			COM Port#
extern const int VCACOM;				// Rohde & Schwarz VCA 			COM Port#
extern const int PT5210COM;				// PT5210 sync gen i test rack  COM Port#
extern const int PM3094COM;				// PM3094 scop i test rack  	COM Port#
extern const char *COMname[];

extern int	SyncLevelOk;			// boolean variables
extern int	DCOffsetOk;
extern int	PhaseScHOk;

extern int devicePM5640G;
extern int devicePM5640M;
extern int IEEEboard;
extern int ErrIEEE;
extern int Err232;
extern int hp34401;
extern int hp53132;
extern int Cont;
extern int done;
extern int PasswordUsed;

extern unsigned char PM5662Status;
 

extern int Repeat;

extern char COMdata[100];			// data read from a COM port

extern char DUTStr[50];

extern int DACData[8][2];				  // [BB][syncDAC,offsetDAC]
extern unsigned int PhaseData[8][4];	  // [BB][GPhase,GScH,MPhase,MScH]
extern unsigned int SDIData[4][2];		  // [SDI][Gdata,Mdata]		(SDI=black1,2,3 + TSG)
extern unsigned int SPGCalData[4];





/************** Global Function Declarations **************/
extern void SetVLM(unsigned short StartLine, unsigned short StopLine,
                   unsigned short StartPos, unsigned short StopPos);
extern void VLMdata(unsigned short xdata, unsigned short sel);
extern void SetPM5662(int TVSystem);
extern void SetGenlockSignal(int Signal);

extern char *ReadlnCOMPort(int PortNo, double Timeout);
extern unsigned char *ReadBinCOMPort(int PortNo, double Timeout, int NoOfBytes);
extern void WriteCOMPort(int PortNo, char TransmitStr[]);

extern void WaitForContinue(void);

extern int Set5640Pattern(char GM[2], int Patt);
extern int Set5640BurstLevel(char GM[2], int Level);
extern int Set5640SyncLevel(char GM[2], int Level);
extern int Set5640Hum(char GM[2], int Hum);
extern int Set5640Noise(char GM[2], int Noise);
extern int Set5640Standard(char GM[2]);
extern int ReadPM5640ScHCalData(void);
extern int Set5640ExternDataVits(void);

extern void SelectPM3094Setup(int SetupNo);

extern int WriteProtect(int OnOff, int flag);

extern int CheckFrontRearTerminal(int Term);

extern int CheckReadyVCA(void);

extern int GetGenlockStatus(void);
