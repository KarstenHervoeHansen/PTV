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

const int PALPatternTable[22][2] = {
	{ 0, EBUCBar},			// SMPTECBar
	{ 1, EBUCBarITU},		// EBUCBar
	{ 0, CBar100},			// FCCCBar
	{ 1, CBarGrey75},		// EBUCBarITU
	{ 1, CBarRed75 },		// CBar100
	{ 1, Red75},				// CBarGrey75
	{ 1, Multiburst},		// CBarRed75
	{ 1, Window10},			// Red75
	{ 1, Window15},			// Multiburst
	{ 1, Window20},			// Window10
	{ 1, Window100},		// Window15
	{ 1, BlWh15kHz},		// Window20
 	{ 1, White100},			// Window100
 	{ 1, BlackBurst},		// BlWh15kHz,
	{ 1, CheckField},		// White100
	{ 1, DigitalGrey},	// BlackBurst
 	{ 1, Stair5},				// CheckField
 	{ 1, Stair10},			// DigitalGrey
 	{ 1, Crosshatch},		// Stair5,
	{ 1, PLUGE},				// Stair10
	{ 1, BlackBurst},		// Crosshatch
	{ 1, BlackBurst}		// PLUGE
};

const int NTSCPatternTable[22][2] = {
	{ 1, SMPTECBar},		// SMPTECBar
	{ 0, FCCCBar},			// EBUCBar
	{ 1, EBUCBarITU},		// FCCCBar
	{ 1, CBar100},			// EBUCBarITU
	{ 1, Red75 },				// CBar100
	{ 0, Multiburst},		// CBarGrey75
	{ 0, Window10},			// CBarRed75
	{ 1, Window15},			// Red75
	{ 1, Window20},			// Multiburst
	{ 1, Window100},		// Window10
	{ 1, BlWh15kHz},		// Window15
	{ 1, White100},			// Window20
 	{ 1, BlackBurst},		// Window100
 	{ 1, CheckField},		// BlWh15kHz,
	{ 1, DigitalGrey},	// White100
	{ 1, Stair5},				// BlackBurst
 	{ 1, Stair10},			// CheckField
 	{ 1, Crosshatch},		// DigitalGrey
 	{ 1, PLUGE},				// Stair5,
	{ 1, BlackBurst},		// Stair10
	{ 1, BlackBurst},		// Crosshatch
	{ 1, BlackBurst},		// PLUGE
};

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
	for ( int i= 0; i< 3; i++)
	  Stream->Write(( const void*) &BlackSetup[i], sizeof( BlackSetup[i]));
  Stream->Write(( const void*) &CBarSetup, sizeof( CBarSetup));
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

/*
	ByteSize = sizeof( ValidFile);
	ByteSize += sizeof( GenlockSetup);
  ByteSize += 3*sizeof( BlackSetup[0]);
  ByteSize += sizeof( CBarSetup);
	ByteSize += sizeof( AudioSetup)+1;
*/
  Stream->Read((void*) &ValidFile, sizeof( ValidFile));

  if ( !strcmp( ValidFile, "rev1.0"))
  {
	  Stream->Read(( void*) &GenlockSetup, sizeof( GenlockSetup));
		for ( int i= 0; i< 3; i++)
		  Stream->Read(( void*) &BlackSetup[i], sizeof( BlackSetup[i]));
  	Stream->Read(( void*) &CBarSetup, sizeof( CBarSetup));
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

