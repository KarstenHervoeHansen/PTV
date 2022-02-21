/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		RS232.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960614																		*/
/*	Rev. date:	981002, KEn DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 981002: Released in SW ver. 3.0														*/
/* 980602: Released in SW ver. 2.0														*/
/***************************************************************************/
#define NoOfCommands			26
#define NoInASCII				128

#define RxBufferLen			128
#define StringLen				33

#define XORValue				0x03

#define IdleState				0x00
#define DataState				0x01
#define QuoteState			0x02		//MUST be DataState XOR 03H
#define DelState				0x03

enum CommandStates			{ State_0 = 0, State_1, State_2, State_3,\
									  State_4, State_5, State_6, State_7, State_8 }

typedef struct COMMAND_STRUCT {
	char Cmd1;
	char Cmd2;

	void ( *Exec)();
};

extern UC ParamCnt;					// Number of numeric parameters received
extern UC StrCnt;						// Number of string parameters received

extern char CmdDelimiter;

extern char String[StringLen];
extern UL Parameter[3];

extern void RS232Init();
extern void CharHandler();
extern void CommandExecute();

extern bit TestInputBuffer();

extern void WriteChar( char TxChar);
extern void WriteEndOfTx( char checksum);
extern UC WriteString( char* Str);
extern UC WriteValue( UL value);
