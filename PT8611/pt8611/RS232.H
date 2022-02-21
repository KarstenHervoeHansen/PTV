/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT8632/33 SDI Test Pattern Generator 1/2                   */
/*  Module:    RS232.H                                                     */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  960614                                                     */
/*  Rev. date:  981002, KEn DEV                                            */
/*  Status:    Version 2.0                                                 */
/*                                                                         */
/*  Changes:                                                               */
/* 981002: Released in SW ver. 3.0                                         */
/* 980602: Released in SW ver. 2.0                                         */
/***************************************************************************/
#define UC        unsigned char
#define UI        unsigned int
#define UL        unsigned long

#define NoError      0x00
#define Buffer_OV    0x01
#define Illegal_Del  0x02
#define Illegal_Cmd  0x04
#define Illegal_Par  0x08
#define IIC_NoAck    0x10
#define IIC_CheckSum 0x20
#define IIC_DataErr  0x40
#define Error_7      0x80

#define NoInASCII     128
#define XORValue     0x03

enum CommandStates
  { 
  State_0 = 0, State_1, State_2, State_3,\
  State_4,     State_5, State_6, State_7, State_8
  };

typedef struct COMMAND_STRUCT
  {
  char Cmd1;
  char Cmd2;
  void ( *Exec)();
  };


/* Function Prototypes */

void RS232Init();
void CharHandler();
void CommandExecute();
bit  TestInputBuffer();
void UnitAddressCommand(void);
void WriteChar( char TxChar);
void WriteString(UC idata *Str);
void WriteVal( UL value);

void TimeCommand(void);
void DateCommand(void);
void WriteUserTextCommand(void);
void ReadUserTextCommand(void);
void ReadSWVersionCommand(void);
void ReadHWVersionCommand(void);
void ReadHWInformationCommand(void);

void SetFormatGen0(void);
void SetFormatGen1(void);
void SetFormatGen2(void);
void Gen0SetDelayCommand(void);
void Gen1SetDelayCommand(void);
void Gen2SetDelayCommand(void);
void SetGenlockDelay(void);
void ReadTLGSamplesCommand(void);
void GeneratorDelayCommand(void);
void GeneratorFormatCommand(void);
