//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "pt52classold.h"
//---------------------------------------------------------------------------
const char AutoTxt[]						= "Auto";
const char f10MHzTxt[]					= "10 MHz";
const char PALTxt[] 						= "PAL";
const char PAL_IDTxt[] 					= "PAL with ID";
const char NTSCTxt[] 						= "NTSC";
const char f500HzTxt[]					= "500 Hz";
const char f1000HzTxt[]					= "1000 Hz";
const char f1000HzwClickTxt[]		= "1000 Hz w/click in left channel";
const char f8000HzTxt[]					= "8000 Hz";

const char* TVSystemTxt[]				= { PALTxt, PAL_IDTxt, NTSCTxt, NULL };
const char* GenlockSystemTxt[]	= { AutoTxt, PALTxt, NTSCTxt, f10MHzTxt, NULL };
const char* AudioToneTxt[]			= { f500HzTxt, f1000HzTxt, f1000HzwClickTxt, f8000HzTxt, NULL };
//const char* AESEBULevelTxt[]		= { f500HzTxt, f1000HzTxt, f1000HzwClickTxt, f8000HzTxt, NULL };
//const char* AnalogLevelTxt[]		= { f500HzTxt, f1000HzTxt, f1000HzwClickTxt, f8000HzTxt, NULL };

/*
enum AudioSystem 		{ AESEBUAudio = 0, AnalogAudio };
enum Pattern				{ CBar = 0, CBar100, SMPTECBar };
enum AudioFreq			{ f441kHz = 0, f48kHz };*/
//---------------------------------------------------------------------------
TSyncGenerator::TSyncGenerator()
{
}
//---------------------------------------------------------------------------
TSyncGenerator::~TSyncGenerator()
{
}
//---------------------------------------------------------------------------
/*void TSyncGenerator::SetGenlockSystem( char System)
{
	GenlockSystem = System;
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetGenlockSystem( void)
{
	return( GenlockSystem);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetGenlockFieldTiming( int Field)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetGenlockFieldTiming( void)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetGenlockLineTiming( int Line)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetGenlockLineTiming( void)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetGenlockSamplesTiming( long Samples)
{
}
//---------------------------------------------------------------------------
long TSyncGenerator::GetGenlockSamplesTiming( void)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlackSystem( int index, char System)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetBlackSystem( int index)
{
	if ( index < 3)
		return( BlackSystem[index]);

  return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlackFieldTiming( int index, int Field)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetBlackFieldTiming( int index)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlackLineTiming( int index, int Line)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetBlackLineTiming( int index)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlackSamplesTiming( int index, long Samples)
{
}
//---------------------------------------------------------------------------
long TSyncGenerator::GetBlackSamplesTiming( int index)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlackScHPhase( int index, int ScHPhase)
{
	BlackScHPhase[index] = ScHPhase;
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetBlackScHPhase( int index)
{
	return( BlackScHPhase[index]);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarSystem( char System)
{
	CBarSystem = System;
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetCBarystem( void)
{
	return( CBarSystem);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarFieldTiming( int Field)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetCBarFieldTiming( void)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarLineTiming( int Line)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetCBarLineTiming( void)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarSamplesTiming( long Samples)
{
}
//---------------------------------------------------------------------------
long TSyncGenerator::GetCBarSamplesTiming( void)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarScHPhase( int ScHPhase)
{
	CBarScHPhase = ScHPhase;
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetCBarScHPhase( void)
{
	return( CBarSchPhase);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarPattern( int Pattern)
{
	CBarPattern = Pattern;
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetCBarPattern( void)
{
	return( CBarPattern);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioOutput( int Output)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetAudioOutput( void)
{
	return( AudioOutput);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioTiming( int index, int Timing)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetAudioTiming( int index)
{
	return( 0);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioTone( int index, int Tone)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetAudioTone( int index)
{
	return( AudioTone[index]);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioLevel( int index, int Level)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetAudioLevel( int index)
{
	return( AudioLevel[index]);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioClickPeriod( int index,int ClickPeriod)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetAudioClickPeriod( int index)
{
	return( AudioClickPeriod[index]);
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioSamplingFreq( int Freq)
{
}
//---------------------------------------------------------------------------
int TSyncGenerator::GetAudioSamplingFreq( void)
{
	return( 0);
}
*/
//---------------------------------------------------------------------------

