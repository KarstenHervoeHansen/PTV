//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "pt52class.h"
//---------------------------------------------------------------------------
//Display names/arrays
const char InternalTxt[]				= "Internal";
const char PALBurstTxt[]				= "PAL burst";
const char NTSCBurstTxt[]				= "NTSC burst";
const char f10MHzTxt[]					= "10 MHz";
const char PALTxt[] 						= "PAL";
const char PAL_IDTxt[] 					= "PAL with ID";
const char NTSCTxt[] 					= "NTSC";
const char S500HzTxt[]					= "Stereo 500 Hz";
const char S1kHzTxt[]					= "Stereo 1 kHz";
const char SEBU1kHzTxt[]				= "Stereo EBU 1kHz";
const char S8kHzTxt[]					= "Stereo 8 kHz";

const char SilenceTxt[]					= "Silence";

const char SMPTECBarTxt[]				= "SMPTE Colorbar";
const char EBUCBarTxt[]					= "EBU Colorbar";
const char FCCCBarTxt[]					= "FCC Colorbar";
const char EBUCBarITUTxt[]				= "EBU Colorbar ITU801";
const char CBar100Txt[]					= "100% Colorbar";
const char CBar75GreyTxt[]				= "75% Colorbar + Grey";
const char CBar75RedTxt[]				= "75% Colorbar + Red";
const char Red75Txt[]					= "75% Red";
const char MultiburstTxt[]				= "Multiburst";
const char CCIR18Txt[]					= "CCIR18";
const char Window10Txt[]				= "10% Window";
const char Window15Txt[]				= "15% Window";
const char Window20Txt[]				= "20% Window";
const char Window100Txt[]				= "100% Window";
const char BlWh15kHzTxt[]				= "15 kHz Black/White";
const char White100Txt[]				= "White 100%";
const char BlackTxt[]					= "Black";
const char CheckFieldTxt[]				= "SDI Check Field";
const char DigGreyTxt[]					= "Digital Grey";
const char Staircase5Txt[]				= "5 step Staircase";
const char Staircase10Txt[]			= "10 step Staircase";
const char CrosshatchTxt[]				= "Crosshatch";
const char PLUGETxt[]					= "PLUGE";

const char *ResetSystemTxt[]			= { PALTxt, NTSCTxt, NULL };
const char *GenlockSystemTxt[]		= { InternalTxt, PALBurstTxt, NTSCBurstTxt, f10MHzTxt, NULL };
const char *BBSystemTxt[]				= { PALTxt, PAL_IDTxt, NTSCTxt, NULL };
const char *TSGSystemTxt[]				= { PALTxt, NTSCTxt, NULL };
const char *EmbAudioToneTxt[]			= { "Off", "Silence", "Stereo 1 kHz", NULL };
const char* AudioOutputTxt[]			= { "Analog", "AES/EBU", NULL };
const char* AudioSystemTxt[]			= { "PAL", "NTSC", NULL };
const char *AudioToneTxt[]				= { S500HzTxt, S1kHzTxt, SEBU1kHzTxt, S8kHzTxt, NULL };
const char *AESEBULevelTxt[]			= { "0", "-9", "-12", "-15", "-16",\
													 "-18", "-21", SilenceTxt, NULL };
const char *AnalogLevelTxt[]			= { "+10", "+8", "+7", "+6", "+5", "+4", "+3",\
													 "+2", "+1", "+0", "-1", "-2", "-3", "-4", "-5",\
													 "-6", "-7", "-8", "-9", "-10", "-11", "-12",\
													 "-13", "-15", "-18", "-21", "-24", "-27", "-30",\
                                        "-33", "-36", SilenceTxt, NULL };
const char *AudioClickTxt[]			= { "1", "3", NULL };
const char *AudioWordTxt[]				= { "44.1 kHz", "48 kHz", NULL };
const char *TSGPatternTxt[]			= { SMPTECBarTxt, EBUCBarTxt, FCCCBarTxt, EBUCBarITUTxt,\
													 CBar100Txt, CBar75GreyTxt, CBar75RedTxt, Red75Txt,
                                    	 MultiburstTxt, CCIR18Txt, Window10Txt, Window15Txt,\
                                    	 Window20Txt, Window100Txt, BlWh15kHzTxt, White100Txt,\
                                    	 BlackTxt, CheckFieldTxt, DigGreyTxt, Staircase5Txt,\
                                    	 Staircase10Txt, CrosshatchTxt, PLUGETxt, NULL };
//Remote command name/arrays
const char INTCmdTxt[]					= "INTERNAL";
const char PALBCmdTxt[]					= "PALBURST";
const char F10MHZCmdTxt[]				= "F10MHZ";
const char PALCmdTxt[] 					= "PAL";
const char PAL_IDCmdTxt[] 				= "PAL_ID";
const char NTSCCmdTxt[] 				= "NTSC";
const char NTSCBURSTCmdTxt[] 			= "NTSCBURST";

const char OFFCmdTxt[]					= "OFF";
const char SILCmdTxt[]					= "SILENCE";
const char ANALCmdTxt[]					= "ANALOG";
const char AESCmdTxt[]					= "AESEBU";

const char S500HZCmdTxt[]				= "S500HZ";
const char S1KHZCmdTxt[]				= "S1KHZ";
const char SEBU1KHZCmdTxt[]			= "SEBU1KHZ";
const char S8KHZCmdTxt[]				= "S8KHZ";

const char F441KHZCmdTxt[]				= "F441KHZ";
const char F48KHZCmdTxt[]				= "F48KHZ";

const char CBSMCmdTxt[]					= "CBSMPTE";
const char CBEBCmdTxt[]					= "CBEBU";
const char CBFCCmdTxt[]					= "CBFCC";
const char CBEB8CmdTxt[]				= "CBEBU8";
const char CB100CmdTxt[]				= "CB100";
const char CBGR75CmdTxt[]				= "CBGREY75";
const char CBR75CmdTxt[]				= "CBRED75";
const char RED75CmdTxt[]				= "RED75";
const char MULTCmdTxt[]					= "MULTIBURST";
const char CCIR18CmdTxt[]				= "CCIR18";
const char WIN10CmdTxt[]				= "WIN10";
const char WIN15CmdTxt[]				= "WIN15";
const char WIN20CmdTxt[]				= "WIN20";
const char WIN100CmdTxt[]				= "WIN100";
const char BLWH15KHZCmdTxt[]			= "BLWH15KHZ";
const char WHIT100CmdTxt[]				= "WHITE100";
const char BLACCmdTxt[]					= "BLACK";
const char SDICCmdTxt[]					= "SDICHECK";
const char DGRCmdTxt[]					= "DGREY";
const char STA5CmdTxt[]					= "STAIRCASE5";
const char STA10CmdTxt[]				= "STAIRCASE10";
const char CROSCmdTxt[]					= "CROSSHATCH";
const char PLUGCmdTxt[]					= "PLUGE";

const char *ResetSystemCmdTxt[]		= { PALCmdTxt, NTSCCmdTxt, NULL};
const char *GenlockSystemCmdTxt[]	= { INTCmdTxt, PALBCmdTxt, NTSCBURSTCmdTxt, F10MHZCmdTxt, NULL};
const char *BBSystemCmdTxt[]	 		= { PALCmdTxt, PAL_IDCmdTxt, NTSCCmdTxt, NULL };
const char *TSGSystemCmdTxt[]			= { PALCmdTxt, NTSCCmdTxt, NULL };
const char *EmbAudioToneCmdTxt[]		= { OFFCmdTxt, SILCmdTxt, S1KHZCmdTxt, NULL};
const char* AudioOutputCmdTxt[]		= { ANALCmdTxt, AESCmdTxt, NULL};
const char* AudioSystemCmdTxt[]		= { PALCmdTxt, NTSCCmdTxt, NULL};
const char *AudioToneCmdTxt[]			= { S500HZCmdTxt, S1KHZCmdTxt, SEBU1KHZCmdTxt, S8KHZCmdTxt, NULL};
const char *AudioWordCmdTxt[]			= { F441KHZCmdTxt, F48KHZCmdTxt, NULL};
const char *TSGPatternCmdTxt[]		= { CBSMCmdTxt, CBEBCmdTxt, CBFCCmdTxt, CBEB8CmdTxt,\
													 CB100CmdTxt, CBGR75CmdTxt, CBR75CmdTxt, RED75CmdTxt,
                                    	 MULTCmdTxt, CCIR18CmdTxt, WIN10CmdTxt, WIN15CmdTxt,\
                                    	 WIN20CmdTxt, WIN100CmdTxt, BLWH15KHZCmdTxt, WHIT100CmdTxt,\
                                    	 BLACCmdTxt, SDICCmdTxt, DGRCmdTxt, STA5CmdTxt,\
                                    	 STA10CmdTxt, CROSCmdTxt, PLUGCmdTxt, NULL};

//---------------------------------------------------------------------------
TGenlockGenerator::TGenlockGenerator()
{
	System = PAL;
	Samples = 0;
}
//---------------------------------------------------------------------------
TGenlockGenerator::~TGenlockGenerator()
{
}
//---------------------------------------------------------------------------
TBlackGenerator::TBlackGenerator()
{
	System = PAL;
	Samples = ScHPhase = 0;
}
//---------------------------------------------------------------------------
TBlackGenerator::~TBlackGenerator()
{
}
//---------------------------------------------------------------------------
TTSGenerator::TTSGenerator()
{
	System = PAL;
	Samples = ScHPhase = 0;
	Pattern = EBUCBar;
	EmbAudio = EmbAudioOff;
}
//---------------------------------------------------------------------------
TTSGenerator::~TTSGenerator()
{
}
//---------------------------------------------------------------------------
TAudioGenerator::TAudioGenerator()
{
	Output = AESEBUAudio;
	System = AudioPAL;
	Timing = 0;
	WordClock = f441kHz;

	for ( int i = AnalogAudio; i <= AESEBUAudio; i++)
  	{
		Tone[i] = S500Hz;
  		ClickPeriod[i] = click3s;
  	}

  	Level[AESEBUAudio] = M0dBFS;
  	Level[AnalogAudio] = P0dBm;
}
//---------------------------------------------------------------------------
TAudioGenerator::~TAudioGenerator()
{
}
//---------------------------------------------------------------------------
TSyncGenerator::TSyncGenerator()
{
}
//---------------------------------------------------------------------------
TSyncGenerator::~TSyncGenerator()
{
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetGenlockSettings( TGenlockGenerator *tmp)
{
	GenlockSetup = *tmp;
}
//---------------------------------------------------------------------------
TGenlockGenerator *TSyncGenerator::GetGenlockSettings( void)
{
	return( &GenlockSetup);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlackSettings( int index, TBlackGenerator *tmp)
{
	BlackSetup[index] = *tmp;
}
//---------------------------------------------------------------------------
TBlackGenerator *TSyncGenerator::GetBlackSettings( int index)
{
	return( &BlackSetup[index]);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetTSGSettings( TTSGenerator *tmp)
{
	TSGSetup = *tmp;
}
//---------------------------------------------------------------------------
TTSGenerator *TSyncGenerator::GetTSGSettings( void)
{
	return( &TSGSetup);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioSettings( TAudioGenerator *tmp)
{
	AudioSetup = *tmp;
}
//---------------------------------------------------------------------------
TAudioGenerator *TSyncGenerator::GetAudioSettings( void)
{
	return( &AudioSetup);
}
//---------------------------------------------------------------------------
int TSyncGenerator::SaveToFile( AnsiString &FileName)
{
	TFileStream *Stream;
	char ValidFile[7] = "rev1.0";

	try
  	{
		Stream = new TFileStream( FileName, fmCreate | fmOpenWrite);
  	}
  	catch(...)
  	{
		return( -1);
  	}

  	Stream->Write((const void*) &ValidFile, sizeof( ValidFile));

  	Stream->Write((const void*) &GenlockSetup, sizeof( GenlockSetup));

	for ( int i = Black1Output; i <= Black3Output; i++)
		Stream->Write(( const void*) &BlackSetup[i], sizeof( BlackSetup[i]));

  	Stream->Write(( const void*) &TSGSetup, sizeof( TSGSetup));
  	Stream->Write(( const void*) &AudioSetup, sizeof( AudioSetup));

	delete Stream;

	return( 0);
}
//---------------------------------------------------------------------------
int TSyncGenerator::ReadFromFile( AnsiString &FileName)
{
	TFileStream *Stream;
  	char ValidFile[7];

	try
  	{
		Stream = new TFileStream( FileName, fmOpenRead);
  	}
  	catch(...)
  	{
		return( -1);
  	}

  	Stream->Read((void*) &ValidFile, sizeof( ValidFile));

  	if ( !strcmp( ValidFile, "rev1.0"))
  	{
		Stream->Read(( void*) &GenlockSetup, sizeof( GenlockSetup));

		for ( int i = Black1Output; i <= Black3Output; i++)
			Stream->Read(( void*) &BlackSetup[i], sizeof( BlackSetup[i]));

  		Stream->Read(( void*) &TSGSetup, sizeof( TSGSetup));
	  	Stream->Read(( void*) &AudioSetup, sizeof( AudioSetup));
  	}
	else
  	{
		delete Stream;
		return( -1);
  	}

	delete Stream;

  	return( 0);
}
//---------------------------------------------------------------------------


