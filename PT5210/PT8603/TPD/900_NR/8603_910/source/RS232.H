/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/3 SDI Digital test signal generator						*/
/*	Module:		RS232.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960614																		*/
/*	Rev. date:	980929, KEn DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	Changes:																						*/
/* 980929: Increased NoOfCommands by one.												*/
/***************************************************************************/

//#define Default_Unit_Adr	0xF0

#define NoOfCommands			21
#define NoInASCII				128

#define RxBufferLen			64
#define StringLen				16

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

extern char Cmd[4];
extern char CmdDelimiter;

extern char String[StringLen];
extern UL Parameter[3];

extern void RS232Init();
extern void CharHandler();
extern void Command_Execute();
extern bit TestInputBuffer();

extern void WriteChar( char TxChar);
extern void WriteEndOfTx( char checksum);
extern UC WriteString( char* Str);
extern UC WriteValue( UL value);
