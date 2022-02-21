//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "pt52class.h"
//---------------------------------------------------------------------------
const char InternalTxt[]				= "Internal";
const char f10MHzTxt[]					= "10 MHz";
const char PALTxt[] 						= "PAL";
const char PAL_IDTxt[] 					= "PAL with ID";
const char NTSCTxt[] 						= "NTSC";
const char S500HzTxt[]					= "Stereo 500 Hz";
const char S1kHzTxt[]						= "Stereo 1 kHz";
const char SEBU1kHzTxt[]				= "Stereo EBU 1kHz";
const char S8kHzTxt[]						= "Stereo 8 kHz";

const char SMPTECBarTxt[]				= "SMPTE Colorbar";
const char EBUCBarTxt[]					= "EBU Colorbar";
const char FCCCBarTxt[]					= "FCC Colorbar";
const char EBUCBarITUTxt[]			= "EBU Colorbar ITU801";
const char CBar100Txt[]					= "100% Colorbar";
const char CBar75GreyTxt[]			= "75% Colorbar + Grey";
const char CBar75RedTxt[]				= "75% Colorbar + Red";
const char Red75Txt[]						= "75% Red";
const char MultiburstTxt[]			= "Multiburst";
const char Window10Txt[]				= "10% Window";
const char Window15Txt[]				= "15% Window";
const char Window20Txt[]				= "20% Window";
const char Window100Txt[]				= "100% Window";
const char BlWh15kHzTxt[]				= "15 kHz Black/White";
const char White100Txt[]				= "White 100%";
const char BlackBurstTxt[]			= "Black";
const char CheckFieldTxt[]			= "SDI Check Field";
const char DigGreyTxt[]					= "Digital Grey";
const char Staircase5Txt[]			= "5 step Staircase";
const char Staircase10Txt[]			= "10 step Staircase";
const char CrosshatchTxt[]			= "Crosshatch";
const char PLUGETxt[]						= "PLUGE";

const char *TVSystemTxt[]				= {	PALTxt, PAL_IDTxt, NTSCTxt, NULL };
const char *GenlockSystemTxt[]	= {	InternalTxt, PALTxt, NTSCTxt, f10MHzTxt, NULL };
const char *EmbAudioToneTxt[]		= { "Off", "Stereo 1 kHz", NULL };
const char* AudioOutputTxt[]		= { "Analog", "AES/EBU", NULL };
const char* AudioSystemTxt[]		= { "PAL", "NTSC", NULL };
const char *AudioToneTxt[]			= { S500HzTxt, S1kHzTxt, SEBU1kHzTxt, S8kHzTxt, NULL };
const char *AESEBULevelTxt[]		= { "0 dBFS", "-9 dBFS", "-12 dBFS", "-15 dBFS", "-16 dBFS",\
																	 	"-18 dBFS", "-21 dBFS", "Silence", NULL };
const char *AnalogLevelTxt[]		= { "+6 dBm", "+4dBm", "+2 dBm", "0 dBm", "-2 dBm", "-4 dBm",\
																		"-6 dBm", "-8 dBm", "-10 dBm", "-12 dBm", "-14 dBm", "-16 dBm",\
																		"-18 dBm", "-20 dBm", "-22 dBm", "-24 dBm", "-26 dBm", "-28 dBm",\
																		"-30 dBm", "-32 dBm", "-34 dBm", "-36 dBm", "-38 dBm", "-40 dBm",\
																		"-42 dBm", "-44 dBm", "-46 dBm", "-48 dBm", "-50 dBm", "-52 dBm",\
																		"-54 dBm", "Silence", NULL };
const char *AudioClickTxt[]			= { "3 seconds", "1 second", NULL };
const char *AudioWordTxt[]			= { "44.1 kHz", "48 kHz", NULL };
const char *CBarPatternTxt[]		= { SMPTECBarTxt, EBUCBarTxt, FCCCBarTxt, EBUCBarITUTxt,\
																	  CBar100Txt, CBar75GreyTxt, CBar75RedTxt, Red75Txt,
                                    MultiburstTxt, Window10Txt, Window15Txt, Window20Txt,\
                                    Window100Txt, BlWh15kHzTxt, White100Txt,\
                                    BlackBurstTxt, CheckFieldTxt, DigGreyTxt, Staircase5Txt,\
                                    Staircase10Txt, CrosshatchTxt, PLUGETxt, NULL };

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
TCBarGenerator::TCBarGenerator()
{
	System = NTSC;
	Samples = ScHPhase = 0;
	Pattern = SMPTECBar;
  EmbAudio = EmbAudioOff;
}
//---------------------------------------------------------------------------
TCBarGenerator::~TCBarGenerator()
{
}
//---------------------------------------------------------------------------
TAudioGenerator::TAudioGenerator()
{
	Output = AESEBUAudio;
	System = AudioPAL;
  Timing = 0;
  WordClock = f441kHz;

	for ( int i = 0; i < 2; i++)
  {
		Tone[i] = S500Hz;
    ClickPeriod[i] = click3s;
  }

  Level[AESEBUAudio] = M0dBFS;
  Level[AnalogAudio] = M0dBm;
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
void TSyncGenerator::SetCBarSettings( TCBarGenerator *tmp)
{
	CBarSetup = *tmp;
}
//---------------------------------------------------------------------------
TCBarGenerator *TSyncGenerator::GetCBarSettings( void)
{
	return( &CBarSetup);
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
/*	TFileStream *Stream;
	int tmp;
  char buf[100];

	try
  {
		Stream = new TFileStream( FileName, fmCreate | fmOpenWrite);
  }
  catch(...)
  {
		Application->MessageBox( "The specified file could not be opened/created",\
   													 "Error", MB_OK|MB_ICONERROR);
		return( -1);
  }

  memcpy( buf, (const void* ) (TGenlockGenerator::&System), sizeof( TGenlockGenerator::System));

	tmp = TGenlockGenerator::System;
  Stream->Write( (const void*)&tmp, sizeof( tmp));
//	Stream->Write( TGenlockGenerator::Field, sizeof( TGenlockGenerator::Field));
//	Stream->Write( TGenlockGenerator::Line, sizeof( TGenlockGenerator::Line));
//	Stream->Write( TGenlockGenerator::Samples, sizeof( TGenlockGenerator::Samples));

	Stream->Write( TCBarGenerator::System, sizeof( TCBarGenerator::System));
	Stream->Write( TCBarGenerator::Field, sizeof( TCBarGenerator::Field));
	Stream->Write( TCBarGenerator::Line, sizeof( TCBarGenerator::Line));
	Stream->Write( TCBarGenerator::Samples, sizeof( TCBarGenerator::Samples));
	Stream->Write( TCBarGenerator::ScHPhase, sizeof( TCBarGenerator::ScHPhase));
	Stream->Write( TCBarGenerator::Pattern, sizeof( TCBarGenerator::Pattern));

	delete Stream;
*/
  return( 0);
}
//---------------------------------------------------------------------------

