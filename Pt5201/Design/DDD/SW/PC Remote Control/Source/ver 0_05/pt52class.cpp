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
const char f500HzTxt[]					= "500 Hz";
const char f1000HzTxt[]					= "1000 Hz";
const char f1000HzwClickTxt[]		= "1000 Hz w/click in left channel";
const char f8000HzTxt[]					= "8000 Hz";

const char EBUCBarTxt[]					= "EBU Colorbar";
const char SMPTECBarTxt[]				= "SMPTE Colorbar";
const char FCCCBarTxt[]					= "FCC Colorbar";
const char EBUCBarITU801Txt[]		= "EBU Colorbar ITU801";
const char CBar100Txt[]					= "100% Colorbar";
const char CBar75RedTxt[]				= "75% Colorbar + Red";
const char CBar75GreyTxt[]			= "75% Colorbar + Grey";
const char Red75Txt[]						= "75% Red";
const char MultiburstTxt[]			= "Multiburst";
const char Window10Txt[]				= "10% Window";
const char Window15Txt[]				= "15% Window";
const char Window20Txt[]				= "20% Window";
const char Window100Txt[]				= "100% Window";
const char BlWh15kHzTxt[]				= "15kHz Black/White";
const char BlackBurstTxt[]			= "Black burst";
const char Staircase5Txt[]			= "5 step Staircase";
const char Staircase10Txt[]			= "10 step Staircase";
const char CrossHatchTxt[]			= "Cross Hatch";
const char PlugeTxt[]						= "Pluge";

const char* TVSystemTxt[]				= { PALTxt, PAL_IDTxt, NTSCTxt, NULL };
const char* GenlockSystemTxt[]	= { InternalTxt, PALTxt, NTSCTxt, f10MHzTxt, NULL };
const char* EmbAudioToneTxt[]		= { "Silence", "1 kHz", NULL };
const char* AudioToneTxt[]			= { f500HzTxt, f1000HzTxt, f1000HzwClickTxt, f8000HzTxt, NULL };
const char* AESEBULevelTxt[]		= { "0 dBFS", "-9 dBFS", "-12 dBFS", "-15 dBFS", "-16 dBFS",\
																	 	"-18 dBFS", "-21 dBFS", "Silence", NULL };
const char* AnalogLevelTxt[]		= { "+6 dBm", "+4dBm", "+2 dBm", "0 dBm", "-2 dBm", "-4 dBm",\
																		"-6 dBm", "-8 dBm", "-10 dBm", "-12 dBm", "-14 dBm", "-16 dBm",\
																		"-18 dBm", "-20 dBm", "-22 dBm", "-24 dBm", "-26 dBm", "-28 dBm",\
																		"-30 dBm", "-32 dBm", "-34 dBm", "-36 dBm", "-38 dBm", "-40 dBm",\
																		"-42 dBm", "-44 dBm", "-46 dBm", "-48 dBm", "-50 dBm", "-52 dBm",\
																		"-54 dBm", "Silence", NULL };
const char* PALPatternTxt[]			= { EBUCBarTxt, EBUCBarITU801Txt, CBar100Txt, CBar75RedTxt, \
																		CBar75GreyTxt, Red75Txt, MultiburstTxt, Window10Txt, \
                                    Window15Txt, Window20Txt, Window100Txt,BlWh15kHzTxt, \
                                    BlackBurstTxt, Staircase5Txt, Staircase10Txt, \
                                    CrossHatchTxt, PlugeTxt, NULL };
const char* NTSCPatternTxt[]		= { SMPTECBarTxt, EBUCBarITU801Txt, FCCCBarTxt, CBar100Txt,\
																		Red75Txt, MultiburstTxt, Window10Txt, Window15Txt, \
                                    Window20Txt, Window100Txt,BlWh15kHzTxt, BlackBurstTxt, \
                                    Staircase5Txt, Staircase10Txt, CrossHatchTxt, \
                                    PlugeTxt, NULL };
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
TBlack1Generator::TBlack1Generator()
{
	System = PAL;
	Samples = ScHPhase = 0;
}
//---------------------------------------------------------------------------
TBlack1Generator::~TBlack1Generator()
{
}
//---------------------------------------------------------------------------
TBlack2Generator::TBlack2Generator()
{
	System = PAL;
	Samples = ScHPhase = 0;
}
//---------------------------------------------------------------------------
TBlack2Generator::~TBlack2Generator()
{
}
//---------------------------------------------------------------------------
TBlack3Generator::TBlack3Generator()
{
	System = PAL;
	Samples = ScHPhase = 0;
}
//---------------------------------------------------------------------------
TBlack3Generator::~TBlack3Generator()
{
}
//---------------------------------------------------------------------------
TCBarGenerator::TCBarGenerator()
{
	System = PAL;
	Samples = ScHPhase = 0;
	Pattern = 0;
  EmbAudio = EmbAudioSilence;
}
//---------------------------------------------------------------------------
TCBarGenerator::~TCBarGenerator()
{
}
//---------------------------------------------------------------------------
TAudioGenerator::TAudioGenerator()
{
	Output = AESEBUAudio;
	System = PAL;
  Timing = 0;
  WordClock = f441kHz;

	for ( int i = 0; i < 2; i++)
  {
		Tone[i] = f500Hz;
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
	TGenlockGenerator::System = tmp->System;
	TGenlockGenerator::Samples = tmp->Samples;
}
//---------------------------------------------------------------------------
void TSyncGenerator::GetGenlockSettings( TGenlockGenerator* tmp)
{
	tmp->System = TGenlockGenerator::System;
	tmp->Samples = TGenlockGenerator::Samples;
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlack1Settings( TBlack1Generator *tmp)
{
	TBlack1Generator::System = tmp->System;
	TBlack1Generator::Samples = tmp->Samples;
	TBlack1Generator::ScHPhase = tmp->ScHPhase;
}
//---------------------------------------------------------------------------
void TSyncGenerator::GetBlack1Settings( TBlack1Generator* tmp)
{
	tmp->System = TBlack1Generator::System;
	tmp->Samples = TBlack1Generator::Samples;
	tmp->ScHPhase = TBlack1Generator::ScHPhase;
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlack2Settings( TBlack2Generator *tmp)
{
	TBlack2Generator::System = tmp->System;
	TBlack2Generator::Samples = tmp->Samples;
	TBlack2Generator::ScHPhase = tmp->ScHPhase;
}
//---------------------------------------------------------------------------
void TSyncGenerator::GetBlack2Settings( TBlack2Generator* tmp)
{
	tmp->System = TBlack2Generator::System;
	tmp->Samples = TBlack2Generator::Samples;
	tmp->ScHPhase = TBlack2Generator::ScHPhase;
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetBlack3Settings( TBlack3Generator *tmp)
{
	TBlack3Generator::System = tmp->System;
	TBlack3Generator::Samples = tmp->Samples;
	TBlack3Generator::ScHPhase = tmp->ScHPhase;
}
//---------------------------------------------------------------------------
void TSyncGenerator::GetBlack3Settings( TBlack3Generator* tmp)
{
	tmp->System = TBlack3Generator::System;
	tmp->Samples = TBlack3Generator::Samples;
	tmp->ScHPhase = TBlack3Generator::ScHPhase;
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetCBarSettings( TCBarGenerator *tmp)
{
	TCBarGenerator::System = tmp->System;
	TCBarGenerator::Samples = tmp->Samples;
  TCBarGenerator::ScHPhase = tmp->ScHPhase;
	TCBarGenerator::Pattern = tmp->Pattern;
}
//---------------------------------------------------------------------------
void TSyncGenerator::GetCBarSettings( TCBarGenerator *tmp)
{
	tmp->System = TCBarGenerator::System;
	tmp->Samples = TCBarGenerator::Samples;
  tmp->ScHPhase = TCBarGenerator::ScHPhase;
	tmp->Pattern = TCBarGenerator::Pattern;
}
//---------------------------------------------------------------------------
void TSyncGenerator::SetAudioSettings( TAudioGenerator *tmp)
{
	TAudioGenerator::Output = tmp->Output;
	TAudioGenerator::Timing = tmp->Timing;
	TAudioGenerator::WordClock = tmp->WordClock;

	for ( int i = 0; i < 2; i++)
  {
		TAudioGenerator::Tone[i] = tmp->Tone[i];
		TAudioGenerator::Level[i] = tmp->Level[i];
		TAudioGenerator::ClickPeriod[i] = tmp->ClickPeriod[i];
  }
}
//---------------------------------------------------------------------------
void TSyncGenerator::GetAudioSettings( TAudioGenerator *tmp)
{
	tmp->Output = TAudioGenerator::Output;
	tmp->Timing = TAudioGenerator::Timing;
	tmp->WordClock =TAudioGenerator::WordClock;

	for ( int i = 0; i < 2; i++)
  {
		tmp->Tone[i] = TAudioGenerator::Tone[i];
		tmp->Level[i] = TAudioGenerator::Level[i];
		tmp->ClickPeriod[i] = TAudioGenerator::ClickPeriod[i];
  }
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

