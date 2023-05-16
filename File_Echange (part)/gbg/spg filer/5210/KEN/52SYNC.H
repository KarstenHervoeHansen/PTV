/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		52SYNC.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960614																		*/
/*	Rev. date:	980915, KEn DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	Changes:																						*/
/* 980915: Added burstLockOn																*/
/* 980616: Added PT8635																		*/
/* 980609: Added RAMCheckArea																*/
/* 971010: RS232_STRUCT: Baudrate is now an int										*/
/* 971007: Added bit variable RS232DiagnoseMode										*/
/* 971002: Added comments																	*/
/* 970919; Added bit UpdateSignal														*/
/* 970911: In CONN_STRUCTURE: Added HWVersion, changed Version-> SWVersion	*/
/* 970909: SPG now has its own connector: OSCConn									*/
/* 970828: Removed SWVersion to it's own include file: SW5210.H				*/
/* 970825: Added variable SystemError													*/
/* 970828: Minor changes																	*/
/* 970723: Major change: Ptr->DispShow() & Ptr->DispUpdate has now been		*/
/*			  now been replaced by one function: Ptr->DispFunct();				*/
/* 970717: New SW version																	*/
/* 970225: Major changes 																	*/
/* 970220: RS232 setup has been removed from preset								*/
/*	970203: Added a dummy black burst to NoOfAnlBlkUnit							*/
/*	970112: Added ErrorStatus																*/
/***************************************************************************/

#define UC						unsigned char
#define UI						unsigned int
#define UL						unsigned long

#define NA 						0

#define IICBus1				0
#define IICBus2				1

#define ScHPhaseLow			-179
#define ScHPhaseHigh			180

#define AutoESCTimeOut		60					// 60 seconds auto escape timeout

#define NoOfAnlBlkUnits		8
#define NoOfSDIBlkUnits		3
#define NoOfGenlockUnits	6
#define NoOfPresets			8
#define NoInErrorQueue		5

#define NoOfConnectors		3+7				// 1 main + SPG + BB12 + 7 connectors

#define SerialRemoteMask	0x10
#define ParallelRemoteMask	0x20
#define OvenMask				0x40

enum boolean	 		 	   { FALSE = 0, TRUE };
enum Logics						{ LOW = 0, HIGH };
enum OffOnModes			   { OFF = 0, ON };
enum MinMax						{ MIN = 0, MAX };

enum LockMode					{ NormalLock = 0, PanelLock, DownloadLock,\
									  DiagnoseLock };

enum ConnType					{ MainConn = 0, OSCConn, BB12Conn, BB34Conn,\
									  BB56Conn, BB78Conn, SDISConn, SDIGConn, AESConn,\
									  TimeConn};

enum Options					{ PT8601 = 1, PT8602, PT8603, PT8604, PT8605,\
									  PT8606, PT8607, PT8608, PT8609, PT8610, PT8611,\
									  PT8612, PT8613, PT8635 = 35};

typedef struct MENU_ITEM {
	UC ItemNumber;

	code char* ItemTxt;
	UC ItemPos;
	UC ItemConfig;

	void ( *DispFunct)();

	void ( *LeftBut)();
	void ( *UpBut)();
	void ( *DownBut)();
	void ( *RightBut)();

	void ( *ExecBut)();
	void ( *EscapeFct)();

	code struct MENU_ITEM* NextItem;
	code struct MENU_ITEM* PrevItem;
};

typedef struct CONN_STRUCTURE {
	UC Present;
	UC HWType;								// PT86xx option
	UC HWVersion;							// > 0: PT86xx/900 version
	UL KUNumber;							// KU Number
	UC SWVersion;							// Software version
};

typedef struct PNP_STRUCTURE {
	UC Present;
	UC Address;
};

typedef struct ANLBLK_STRUCTURE {	// Option PT8608
	UC System;
	int ScHPhase;
	UL Delay;
};

typedef struct SDIBLK_STRUCTURE {	// Option PT8609
	UC System;
	UC Pattern;
	UC EDH;
	UC EMBAudio;
	UL Delay;
};

typedef struct ANALOG_STRUCTURE {	// Option PT8601
	UC System;
	UC Pattern;
	UC TextOn;
	char TextInfo[9];						// 8 characters + \0
	int ScHPhase;
	UL Delay;
};

typedef struct SDI_STRUCTURE {		// Option PT8602 or -03
	UC System;
	UC Pattern;
	UC TextOn;
	char TextInfo[9];						// 8 characters + \0
	UC EDH;
	UC AudioSignal;
	UC AudioLevel;
	UL Delay;
};

typedef struct AES_EBU_STRUCTURE {	// Option PT8605 hardware
	UC Signal;
	UC	Level;
	UC Timing;
};

typedef struct GENLOCK_STRUCTURE {
	UC System;
	UL Delay;
};

typedef struct RS232_STRUCTURE {
	int Baudrate;
	UC Databit;
	UC Parity;
	UC Handshake;
};

typedef struct STATUS_FLAGS {			// ONLY static variables should go here
	unsigned NormalLockOn : 1;
	unsigned PanelLockOn : 1;
	unsigned DownloadLockOn : 1;
	unsigned DiagnoseLockOn : 1;

	unsigned AutoESCEnable : 1;		// 1: Timeout in menus active
	unsigned AutoESCToStatus : 1;		// 1: At timeout return to status

	unsigned : 11;							// Unused for now
};

typedef struct PRESET_STRUCTURE {

	char Name[17];							// 16 characters + \0

	UC ActiveGenlockNo;
	UC ExternalGenlockNo;

	struct ANLBLK_STRUCTURE AnlBlkStore[NoOfAnlBlkUnits];
	struct SDIBLK_STRUCTURE SDIBlkStore[NoOfSDIBlkUnits];
	struct ANALOG_STRUCTURE AnalogStore;
	struct SDI_STRUCTURE SDIStore;
	struct AES_EBU_STRUCTURE AES_EBUStore;
	struct GENLOCK_STRUCTURE GenlockStore[NoOfGenlockUnits];
	struct STATUS_FLAGS FlagsStore;
};


extern code struct ANLBLK_STRUCTURE AnlBlkConfigFactory[NoOfAnlBlkUnits];
extern code struct SDIBLK_STRUCTURE SDIBlkConfigFactory[NoOfSDIBlkUnits];
extern code struct ANALOG_STRUCTURE AnalogConfigFactory;
extern code struct SDI_STRUCTURE SDIConfigFactory;
extern code struct AES_EBU_STRUCTURE AES_EBUConfigFactory;
extern code struct GENLOCK_STRUCTURE GenlockConfigFactory[NoOfGenlockUnits];
extern code struct RS232_STRUCTURE RS232ConfigFactory;

extern struct CONN_STRUCTURE Connectors[NoOfConnectors];

extern struct PNP_STRUCTURE AnlBlkUnit[NoOfAnlBlkUnits];
extern struct PNP_STRUCTURE SDIBlkUnit[NoOfSDIBlkUnits];
extern struct PNP_STRUCTURE AnalogSignalUnit;
extern struct PNP_STRUCTURE SDISignalUnit;
extern struct PNP_STRUCTURE AES_EBUUnit;
extern struct PNP_STRUCTURE SDIGenlockUnit;
extern struct PNP_STRUCTURE Int2GenlockUnit;
extern struct PNP_STRUCTURE TimeCodeUnit;

extern persistent struct ANLBLK_STRUCTURE AnlBlkConfig[NoOfAnlBlkUnits];
extern persistent struct SDIBLK_STRUCTURE SDIBlkConfig[NoOfSDIBlkUnits];
extern persistent struct ANALOG_STRUCTURE AnalogConfig;
extern persistent struct SDI_STRUCTURE SDIConfig;
extern persistent struct AES_EBU_STRUCTURE AES_EBUConfig;
extern persistent struct GENLOCK_STRUCTURE GenlockConfig[NoOfGenlockUnits];
extern persistent struct RS232_STRUCTURE RS232Config;
extern persistent struct PRESET_STRUCTURE Preset[NoOfPresets];
extern persistent struct STATUS_FLAGS Flags;

extern persistent UC ActivePresetNo;
extern persistent UC ActiveOutputNo;

extern persistent UC ActiveGenlockNo;
extern persistent UC ExternalGenlockNo;

extern persistent UC LCDContrast;
extern persistent UC PasswordState;		// OFF: Apparatus is NOT password
													//  protected

extern persistent UC RAMCheckArea[24];

extern code struct MENU_ITEM* near Ptr;
extern code struct MENU_ITEM* near PrevPtr;

extern UC ErrorCode;
extern UI ErrorStatus;
extern UL ErrorQueue[NoInErrorQueue];
extern UC ErrorQueuePtr;
extern bit ErrorWindowOn;

extern bit MessageWindowOn;		// 1: indicates display of a "message" window
extern near UC MessageCntDwn;		// Counter for display of a message

extern volatile near UC AutoESCTimer;
extern volatile near UC UserTimer;
extern volatile near UC DisplayTimer;

extern UC UpdateTimer;				// Timer for diagnose display animation
extern UC UpdateFrequency;			// Frequency for the above
extern bit UpdateEnable;			// 1: Display animation has been enabled
extern bit UpdateSignal;			// 1: Display animatino caused the call

extern bit AutoESCSignal;			// 1: Return to status display

extern bit Do_Keyboard_Scan;

extern bit TestModeEnable;			// 1: Enable test mode

extern bit MenuModeOn;				// 1: Menu mode, ie. selecting between menus
extern bit SelectModeOn;			// 1: Selection mode, ie. altering something

extern bit Warning_LED;				// 1: Illumination of the WARNING LED
extern bit Unlocked_LED;			// 1: Illumination of the UNLOCKED LED
extern bit Genlock_LED;				// 1: Illumination of the GENLOCK LED

extern bit burstLockOn;					// 1: if burst is available in genlock
												//  systems BurstLockPAL & BurstLockNTSC.

extern bit SerialRemotePresent;
extern bit SerialRemoteActive;
extern bit SerialRemoteLockoutOn;

extern bit ParallelRemotePresent;
extern bit ParallelRemoteActive;

extern bit RS232DiagnoseMode;
