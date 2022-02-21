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


/* Function Prototypes */

void RS232Init();
void CharHandler();
void CommandExecute();
bit  TestInputBuffer();
void UnitAddressCommand(void);
void WriteChar( char TxChar);

void SendChar(char c);

void JTAGMode(void);
void ManualMode(void);

void TimeCommand(void);
void DateCommand(void);
void WriteUserTextCommand(void);
void ReadUserTextCommand(void);
void ReadSWVersionCommand(void);
void ReadHWVersionCommand(void);
void ReadHWInformationCommand(void);

void GenSystemCommand(UI); 
void GenInterfaceCommand(UI);
void GenPatternCommand(UI);
void GenAttribCommand(UI);
void GenDelayCommand(UI);
void GenAudioClickTimeCommand(UI addr);
void GenAudioSignalCommand(UI addr);
void GenAudioLevelCommand(UI addr);

void GenTextPosCommand(UI);
void GenTextScaleCommand(UI);
void GenTextColorCommand(UI);
void GenTextMovCommand(UI);
void GenTextOnOffCommand(UI);
void GenTextCommand(UI);
UC   is_text(UC *);

UC   spi_transfer (UC, UC);
void send(UC, UI, UC*, int);
