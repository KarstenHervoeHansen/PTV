/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MAS.H																			*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	971211																		*/
/*	Rev. date:	000508, KEn, DEV															*/
/*	Status:		Version 1.4																	*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added text positioning in ANLTPG-,SDITPG_STRUCT						*/
/* 991103: Added DispTimer, (to be used in disp_drv.c ONLY!!!)					*/
/* 990304: In PNP_STRUCT: HWInfo hac changed to 2 variables, HWUserPattern	*/
/*				and HWInformation																*/
/* 980917: increased no of attributes in ANLTPG_STRUCT							*/
/* 980915: Added burstLockOn																*/
/* 980826: Added several comments														*/
/* 980708: Implemented Time Clock Interface											*/
/* 980616: Released as 4008 002 06812													*/
/* 980524: Optimizations throughout the whole file. Added HWInformation in	*/
/*				PNP_STRUCT.																		*/
/* 980514: Released as 4008 002 06811													*/
/* 980507: New values for temperature variables, (blower).						*/
/***************************************************************************/

// **************************************************************************
// GLOBAL DEFINITIONS
// **************************************************************************
#define sbit static bit

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

#define NA 						0

#define IICBus1				0
#define IICBus2				1

#define ScHPhaseLow			-179
#define ScHPhaseHigh			180

#define AutoESCTimeOut		60			// 60 seconds auto escape timeout

#define MaxAnlBlkUnits		8			// Max. number of black burst units
#define MaxAnlTPGUnits		2			// Max. number of Analog TPG units
#define MaxSDITSGUnits		3			// Max. number of SDI TSG units
#define MaxSDITPGUnits		3			// Max. number of SDI TPG units
#define MaxAESEBUUnits		2			// Max. number of AES/EBU units
#define MaxGenlockUnits		6			// Max. Number of genlock "units"

#define NoOfPresets			6			// Number of preset
#define NoInErrorQueue		5			// Depth of error queue

#define SerialRemoteMask	0x10		//
#define ParallelRemoteMask	0x20     //
#define OvenMask				0x40     //

// **************************************************************************
// GLOBAL ENUMERATIONS
// **************************************************************************
enum boolean	 		 	   { FALSE = 0, TRUE };
enum Logics						{ LOW = 0, HIGH };
enum OffOnModes			   { OFF = 0, ON };
enum MinMax						{ MIN = 0, MAX };
enum directions				{ STAY = 0, UP, DOWN };

enum LockMode					{ NormalLock = 0, PanelLock, DateTimeLock,
									  DownloadLock, DiagnoseLock };

enum Options					{ PT8601 = 1, PT8602, PT8603, PT8604, PT8605,
									  PT8606, PT8607, PT8608, PT8609, PT8610, PT8611,
									  PT8612, PT8613,
									  PT8631 = 31, PT8632, PT8633, PT8634, PT8635,
									  PT8636, PT8637, PT8638, PT8639 };

enum DownloadTypes 			{ DwnPresetNo = 0, DwnAllPreset = NoOfPresets,\
									  DwnCompleteSPG };

// **************************************************************************
// GLOBAL TYPE DEFINITIONS
// **************************************************************************
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

typedef struct {						// Unit PLUG & PLAY information
	UC Present;							// TRUE if a unit is installed
	UC Address;							// Address used to communicate with the unit
	UC HWType;							// PT86xx option
	UC HWVersion;						// > 0: PT86xx/900 version
	UC HWCustomPattern;				// Custom patterns
	UC HWInformation;					// 
	UL KUNumber;						// KU Number
	UC SWVersion;						// Software version
} PNP_STRUCT;

typedef struct {						// Option PT8608
	UC System;     					// TV system
	int ScHPhase;                 // ScHPhase
	UL Delay;							// Delay
} ANLBLK_STRUCT;

typedef struct {						// Option PT8601/31
	UC System;							// TV system
	UC Pattern;							// Active pattern
	UC Attrib[4];						// Pattern attributes for test patterns
											//  Philips4:3, Philips16:9, FuBK4:3 and
											//	 FuBK16:9. Only one attribute per pattern.
											// Max. 8 attributes are possible
	UC Memory[6];						// Last selected pattern in each group
	UC TextEnable;						//	Text line Off/On
	UC TextStyle[2];					// Text style for standard/complex pattern
											//  0: Standard, 1: Complex
											// +16: Time On
											//	+32: Date On
	UC TextPos[2][2];					// Text positioning for standard/complex pattern
	char Text[5][17];					// 5 lines of 16 characters + \0
	int ScHPhase;                 // ScHPhase
	UL Delay;                     // Delay
} ANLTPG_STRUCT;						// Total of 120 bytes

typedef struct {						// Option PT8609/39
	UC System;							// TV system
	UC Pattern;							// Active pattern
	UC Memory[6];						// Last selected pattern in each group
	UC EDH;								// EDH insertion On/Off
	UC AudioSignal;					// Embedded audio signal
	UC AudioLevel;						// Embedded audio level
	UL Delay;							// Delay
} SDITSG_STRUCT;						//

typedef struct {						// Option PT8602/03/32/33
	UC System;							// TV system
	UC Pattern;							// Active pattern
	UC Attrib[4];						// Pattern attributes for test patterns
											//  Philips4:3, Philips16:9, FuBK4:3 and
											//	 FuBK16:9. Only one attribute per pattern,
											//  i.e. 625 and 525 are common
											// Max. 8 attributes are possible
	UC Memory[6];						// Last selected pattern in each group
	UC TextEnable;						//	Text line Off/On
	UC TextStyle[2];					// Text style for standard/complex pattern
											//  0: Standard, 1: Complex
											// +16: Time On
											//	+32: Time+Date On
	UC TextPos[2][2];					// Text positioning for standard/complex pattern
	char Text[5][17];					// 5 lines of 16 characters + \0
	UC EDH;                       // EDH insertion On/Off
	UC AudioSignal;   	         // Embedded audio signal
	UC AudioLevel;    	   	   // Embedded audio level
	UC AudioGroup;       	   	// Embedded audio group
	UL Delay;                     // Delay
} SDITPG_STRUCT;						// Total of 125 bytes

typedef struct {						// Option PT8635 AES/EBU unit
	UC Signal;                    // AES/EBU audio signal
	UC	Level;							// AES/EBU audio level
	UC Timing;                    // AES/EBU audio timing
} AESEBU_STRUCT;

typedef struct {					  	// Genlock spcification
	UC System;                    //
	UL Delay;                     //


} GENLOCK_STRUCT;

typedef struct { 						// Option PT8637 TimeCode unit
	UC TCIControl;						// Time Clock Interfacets control byte
											// Bit 0: 1: PT5230 genlocked to external
											//			    analog signal.
											// Bit 1: 0: Genlock system is PAL.
											//	Bit 1: 1: Genlock system is NTSC.
											// Bit 2: 1: Reserved
											// Bit 3: 1: Reserved
											// Bit 4: 1: Reserved
											// Bit7-5: TCI registers

	UC TCIStatus;						// Time Clock Interfacets status byte
											// Bit 0: 1: New time update ready
											// Bit 1: 1: An error has occurred
											// Bit 2: 1: External reference is present
											// Bit 3: 1: External reference is valid
											// Bit 4: 0: VITC/LTC time reference is PAL
											// Bit 4: 1: VITC/LTC time reference is NTSC
											// Bit7-5: TCI registers

	UC Reference;						// Selected time reference.
	UL Date;								// Date, ( always organized as YY/MM/DD)
	UL Time;								// Time, ( always organized as HH/MM/SS)
	UC Format;							// Bit 0: Time format, bit 2-1: Date format
	UL Offset;                    // Time offset
} TIMECLOCK_STRUCT;

typedef struct {					   //
	int Baudrate;                 //
	UC Databit;                   //
	UC Parity;                    //
	UC Handshake;                 //
} RS232_STRUCT;

typedef struct {						// ONLY static variables should go here
	unsigned NormalLockOn : 1;
	unsigned PanelLockOn : 1;
	unsigned DateTimeLockOn : 1;
	unsigned DownloadLockOn : 1;
	unsigned DiagnoseLockOn : 1;

	unsigned AutoESCEnable : 1;	// 1: Timeout in menus active
	unsigned AutoESCToStatus : 1;	// 1: At timeout return to status

	unsigned : 11;						// Unused for now
} STATUS_FLAGS;

typedef struct {
	char Name[17];						// 16 characters + terminating NULL

	UC ActiveGenlockNo;
	UC ExternalGenlockNo;

	ANLBLK_STRUCT AnlBlkStore[MaxAnlBlkUnits];
	ANLTPG_STRUCT AnlTPGStore[MaxAnlTPGUnits];
	SDITSG_STRUCT SDITSGStore[MaxSDITSGUnits];
	SDITPG_STRUCT SDITPGStore[MaxSDITPGUnits];
	AESEBU_STRUCT AESEBUStore[MaxAESEBUUnits];

	GENLOCK_STRUCT GenlockStore[MaxGenlockUnits];

	TIMECLOCK_STRUCT TimeClockStore;

	STATUS_FLAGS FlagsStore;
} PRESET_STRUCT;

// **************************************************************************
// PORT AND MEMORY CONTROL:
// RAM has address-space 0-0x1fff
// **************************************************************************
extern volatile UC ind1;
extern volatile UC ind2;

extern volatile UC ud1;
extern volatile UC ud2;
extern volatile UC ud3;
extern volatile UC ud4;
extern volatile UC ud5;

extern near UC ud1buf;
extern near UC ud4buf;
extern near UC ud5buf;

// **************************************************************************
// INTERNAL V24 CONTROL:
// default V24 address for units:
// **************************************************************************
#define DefaultAnlBlk_addr		0xF0		// PT8608/09 default address
#define DefaultAnlTPG_addr		0xF1		// PT8631 default address
#define DefaultSDITSG_addr		0xF2		// PT8639 default address
#define DefaultSDITPG_addr		0xF3		// PT8632/33 default address

#define master_addr				0x10
#define bbu0_addr					0x00		// Internal BB 1&2 default address
#define bbu1_addr					0x11
#define bbu2_addr					0x12
#define bbu3_addr					0x13

#define sync_addr					0x15		// SPG, (OSC), default address

#define AnlSig_addr				0x16     // PT8601 default address
#define SDISig_addr				0x17		// PT8602/03 default address
#define SDITPG_addr				0x18		// PT8632 default address

// **************************************************************************
// IIC-addresses, bus 1:
// **************************************************************************
#define lev1det_addr				0x44		// PCF8574 level detector 1
#define lev2det_addr				0x46		// PCF8574 level detector 2
#define ttl_addr					0x4A		// Ground closure parallel port
#define time_addr					0x4E		// Time code unit
#define time1_addr				0x50		// Time clock interface
#define d1ram_addr				0xA0		// SDI Genlock
#define multibb_addr				0xA2		// Multiple black burst RAM
#define timeram_addr				0xA8		// Time clock unit RAM

// **************************************************************************
// IIC-addresses, bus 2:
// **************************************************************************
#define sound1_addr				0x40		// PT8605/35 Audio generator
#define sound2_addr				0x42		// do.
#define sound3_addr				0x44		// PT8635 Audio generator
#define sound4_addr				0x46		// do.
#define soundram_addr			0xA0
#define mainram1_addr			0xA2
#define spgram_addr				0xA8		// SPG, (OSC), RAM

// **************************************************************************
// IIC-addresses, bus front:
// **************************************************************************
#define keyb_addr					0x40
#define led_addr					0x42
#define ADC_addr					0x90
#define DAC_addr					0x90

// **************************************************************************
// IIC-addresses/offsets for textstrings in MAINBOARD and UNITS EEPROM
// **************************************************************************
#define CompanyName_addr		(UC) 00	// PTV
#define Product12NC_addr		(UC) 10	// 108 12345, ie. Mounted board
#define ProductKUNo_addr		(UC) 20	// KU123456
#define ProductionDate_addr	(UC) 30	//
#define ProductName1_addr		(UC) 60	// PT5230
#define ProductName2_addr		(UC) 70	//

// **************************************************************************
// IIC-addresses for constants in MAINBOARD EEPROM
// **************************************************************************
#define VoltCalib_addr			(UC) 80
#define VoltN5Min_addr			VoltCalib_addr+1
#define VoltN5Max_addr			VoltCalib_addr+2
#define Volt12Min_addr			VoltCalib_addr+3
#define Volt12Max_addr			VoltCalib_addr+4

// **************************************************************************
// IIC-addresses for user-variables in MAINBOARD EEPROM
// **************************************************************************
#define RAMCheckOK_addr			(UC) 85
#define RAMCheckVal_addr		RAMCheckOK_addr+1

// **************************************************************************
// IIC-addresses for constants in SPG EEPROM
// **************************************************************************
#define HPHZero_G_addr			(UC) 0	// Unsigned long
#define GenlPhase_G_addr		(UC) 4	// Unsigned integer
#define HPHZero_M_addr			(UC) 6	// Unsigned long
#define GenlPhase_M_addr		(UC) 10	// Unsigned integer
#define SPGUserText_addr		(UC) 12	// Start of usertext area, 100 bytes

// **************************************************************************
// A/D CONVERSION VARIABLES
// **************************************************************************
extern near UC TempMeas;
extern near UC Volt5Meas;

extern near UC VoltN5Meas;
extern UC VoltN5Min;
extern UC VoltN5Max;

extern near UC Volt12Meas;
extern UC Volt12Min;
extern UC Volt12Max;

extern bit VoltCalib;					// 1: Power has been calibrated

// **************************************************************************
// WATCHDOG
// **************************************************************************
#define FeedTheWatchdog		{ ud4buf ^= 16; ud4= ud4buf;}

// **************************************************************************
// TEMPERATURE VARIABLES, corrected 980507/PRC
// **************************************************************************
#define templimit  	106				// appr.45gr. sensor - 41 gr. air
#define panictemp		55					// appr.75gr. sensor - 71 gr. air
#define hottemp		62					// appr.69gr. sensor - 66 gr. air

// **************************************************************************
// VARIABLES
// **************************************************************************
extern PNP_STRUCT MainBoard;
extern PNP_STRUCT SPGBoard;
extern PNP_STRUCT AnlBlkUnit[MaxAnlBlkUnits];
extern PNP_STRUCT AnlTPGUnit[MaxAnlTPGUnits];
extern PNP_STRUCT SDITSGUnit[MaxSDITSGUnits];
extern PNP_STRUCT SDITPGUnit[MaxSDITPGUnits];
extern PNP_STRUCT AESEBUUnit;
extern PNP_STRUCT SDIGenlockUnit;
extern PNP_STRUCT Int2GenlockUnit;
extern PNP_STRUCT TimeClockUnit;

extern persistent ANLBLK_STRUCT AnlBlkConfig[MaxAnlBlkUnits];
extern persistent ANLTPG_STRUCT AnlTPGConfig[MaxAnlTPGUnits];
extern persistent SDITSG_STRUCT SDITSGConfig[MaxSDITSGUnits];
extern persistent SDITPG_STRUCT SDITPGConfig[MaxSDITPGUnits];
extern persistent AESEBU_STRUCT AESEBUConfig[MaxAESEBUUnits];
extern persistent GENLOCK_STRUCT GenlockConfig[MaxGenlockUnits];
extern persistent TIMECLOCK_STRUCT TimeClockConfig;

extern persistent RS232_STRUCT RS232Config;
extern persistent PRESET_STRUCT Preset[NoOfPresets];
extern persistent STATUS_FLAGS Flags;

extern persistent UC ActivePresetNo;
extern persistent UC ActiveOutputNo;

extern persistent UC ActiveGenlockNo;
extern persistent UC ExternalGenlockNo;

extern persistent UC LCDContrast;
extern persistent UC PasswordState;	// OFF: NOT password protected

extern persistent UC RAMCheckArea[24];

extern code struct MENU_ITEM* near Ptr;
extern code struct MENU_ITEM* near PrevPtr;

extern UI ErrorStatus;
extern UI LevelStatus;
extern UL ErrorQueue[NoInErrorQueue];
extern UC ErrorQueuePtr;
extern bit ErrorWindowOn;

extern bit MessageWindowOn;			// 1: displaying a "message" window
extern near UC MessageCntDwn;			// Counter for displaying a message

extern volatile near UC AutoESCTimer;
extern volatile near UC DispTimer;
extern volatile near UI UserTimer;
extern volatile near UC DisplayTimer;

extern UC UpdateTimer;					// Timer for diagnose display animation
extern UC UpdateFrequency;				// Frequency for the above
extern bit UpdateEnable;				// 1: Display animation has been enabled
extern bit UpdateSignal;				// 1: Display animatino caused the call

extern bit AutoESCSignal;				// 1: Return to status display

extern bit DoKeyboardScan;				// 1: Time to scan the keyboard

extern bit TestModeEnable;				// 1: Enable test mode

extern bit MenuModeOn;					// 1: Menu mode
extern bit SelectModeOn;				// 1: Selection mode

extern bit CBarLED;						// 1: Illumination of the C.BAR LED
extern bit MBurstLED;					// 1: Illumination of the M.BURST LED
extern bit WindowLED;					// 1: Illumination of the WINDOW LED
extern bit SpecialLED;					// 1: Illumination of the SPECIAL LED
extern bit LinearityLED;				// 1: Illumination of the LINEARITY LED
extern bit PatternLED;					// 1: Illumination of the PATTERN LED
extern bit RemoteLED;					// 1: Illumination of the REMOTE LED
extern bit UnlockedLED;					// 1: Illumination of the UNLOCKED LED

extern bit burstLockOn;					// 1: if burst is available in genlock
												//  systems BurstLockPAL & BurstLockNTSC.

extern bit SerialRemotePresent;
extern bit SerialRemoteActive;
extern bit SerialRemoteLockoutOn;

extern bit ParallelRemotePresent;
extern bit ParallelRemoteActive;

extern bit RS232DiagnoseMode;

// **************************************************************************
// FUNCTIONS
// **************************************************************************
extern void waitms( UI time);
extern void waitus( UI time);
