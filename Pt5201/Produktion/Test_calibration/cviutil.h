

//************** Global Variable Declarations **************
// see 'cviutil.c'
extern const char *NC12;
extern const char V24Addr[];
extern char AESLogFile[2][50];
extern char BBLogFile[2][50];
extern char PowerSupplyLogFile[2][50];
extern char SPGLogFile[2][50];
extern char SDILogFile[2][50];
extern char SDIMeasLogFile[2][50];


extern int LogDest;

extern int AppType;

extern int mainPNL;
extern int spghphPNL;
extern int aesPNL;
extern int anPNL;
extern int anmeasPNL;
extern int vccPNL;
extern int instrPNL;
extern int fanPNL;
extern int ocxoPNL;
extern int cntPNL;
extern int totPNL;
extern int spggenlPNL;
extern int selgenlPNL;
extern int spgphasePNL;
extern int configPNL;
extern int filterPNL;
extern int showfiltjustPNL;
extern int syncPNL;
extern int phasePNL;
extern int ltcPNL;
extern int clockPNL;

extern char GPSStatusStr[30];
extern int  GPSStatusOk;
extern int  GPSCheckNo;

extern int BBNo;
extern char	KUStr[10];
extern char	PT5201RevStr[10];


extern const int IOAddr;
extern const int DUTCOM;				// Device Under Test   			COM Port#
extern const int VCACOM;				// Rohde & Schwarz VCA 			COM Port#
extern const int PT5210COM;				// PT5210 sync gen i test rack  COM Port#
extern const int PM3094COM;				// PM3094 scop i test rack  	COM Port#
extern const char *COMname[];


extern double VLMGain;


extern int
			SyncLevelOk,			// boolean variables
			DCOffsetOk,
			PhaseScHGainOk,
			VoltOk,
			Volt50Ok,
			Volt33Ok,
			VoltM50Ok,
			TcxoOk,
			FanOk,
			OvenOk,
			AudioVCOOk,
			LTCTP7Ok,
			BBCalOk,
			TSGCalOk,
			SPGCalOk,
			AutoHPH_PALOk,
			AutoHPH_NTSCOk,
			GenlOk,
			GenlPALOk,
			GenlNTSCOk,
			Genl10MHzOk,
			SPGPhaseCalOk,
 			SPGGenlPhasePALOk,
 			SPGGenlPhaseNTSCOk,
			SPGResetOk,
			TPGPattOk,
 			F13Ok,
 			F27Ok,
			JusterNCOOk,
			JusterFilterOk,
			ClockOk;
			
extern int mainmenuhandle,
		   totalmenuhandle;
			

extern int	RS232Timeout;

extern int devicePM5640G;
extern int devicePM5640M;
extern int IEEEboard;
extern int ErrIEEE;
extern int Err232;
extern int hp53132;
extern int hp34970;
extern int Cont;
extern int done;
extern int PasswordUsed;

extern unsigned char PCDIOPortBStatus;	// status for PM5662 remote control
 

extern int Repeat;

extern char COMdata[100],			// data read from a COM port
				DUTStr[50],
				*dato,  		// YYMMDD
				*kustr,
				YearStr[5],
				MonthStr[5],
				DayStr[5];

extern char *strread,
				wrtbuf[100],
     			MsgStr[100],   // diverse udskrifter
     			*dd,			// MM-DD-YYYY
	  			*tt;			// HH:MM:SS
extern int  fhandle, 
	 			bytes_written;

extern int DACData[8][2];				  // [BB][syncDAC,offsetDAC]
extern unsigned int PhaseData[8][4];	  // [BB][GPhase,GScH,MPhase,MScH]
extern unsigned int SDIData[4][2];		  // [SDI][Gdata,Mdata]		(SDI=black1,2,3 + TSG)
extern unsigned int SPGCalData[4];
extern int ANLTSGDACData[2];			  // [syncDAC,offsetDAC]
extern unsigned int ANLTSGData[4];		  // [GPhase,GScH,MPhase,MScH]





/************** Global Function Declarations **************/
extern int ReadVLMGain(void);
extern int ReadTestSetup(void);
extern char *GetDate(int dateformat);
extern void EnterKUNo(char text[50], char *kuread, char *kustr);
extern void SetMenuItems(int menu);
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
extern int Set5640Vits(int VitsMode);

extern void SelectPM3094Setup(int SetupNo);

extern int WriteProtect(int OnOff, int flag);

extern int CheckFrontRearTerminal(int Term);

extern int CheckReadyVCA(void);

extern int GetGenlockStatus(void);
extern int ShowInstrumentSetup(void);
extern int CheckXLDCRef(void);
extern int CheckHP53132ExtRef(void);
