//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "audio.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

TAudioForm *AudioForm;
//---------------------------------------------------------------------------
__fastcall TAudioForm::TAudioForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormCreate(TObject *Sender)
{
	const char* *ptr = AudioOutputTxt;

	OutputRadioGroup->Items->Clear();
	ptr = AudioOutputTxt;
	while ( *ptr)
  	OutputRadioGroup->Items->Add( *ptr++);

	SystemComboBox->Items->Clear();
	ptr = AudioSystemTxt;
	while ( *ptr)
  	SystemComboBox->Items->Add( *ptr++);

	ToneComboBox->Items->Clear();
	ptr = AudioToneTxt;
	while ( *ptr)
  	ToneComboBox->Items->Add( *ptr++);

	ClickPeriodRadioGroup->Items->Clear();
	ptr = AudioClickTxt;
	while ( *ptr)
  	ClickPeriodRadioGroup->Items->Add( *ptr++ + AnsiString( " second"));

	WordClockRadioGroup->Items->Clear();
	ptr = AudioWordTxt;
	while ( *ptr)
  	WordClockRadioGroup->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormShow(TObject *Sender)
{
  RestoreWindowPosition( AudioForm, "AudioWindow");

	// Setup pointer to actual settings
	AudioSetup = PT5201Setup->GetAudioSettings();

	AudioCancel = new TAudioGenerator;

	// Copy original settings temporarily to AudioCancel
	*AudioCancel = *AudioSetup;

	PrepareOutput( AudioSetup->Output);

	LockForm( PropertiesForm->ConfigureLockItem->Checked);

  if ( !PropertiesForm->ConfigureLockItem->Checked)
		ActiveControl = OutputRadioGroup;

	ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete AudioCancel;

  SaveWindowPosition( AudioForm, "AudioWindow");
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModelessResult != mrOk)
	{
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
		{
			ModelessResult = mrNone;

			CanClose = false;
		}
    else														// Restore original settings
		{
    	PT5201Setup->SetAudioSettings( AudioCancel);
			UpdateOutput( AudioCancel);

			PT5201Form->SystemStatusUpdate();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::OKButtonClick(TObject *Sender)
{
	ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::CancelButtonClick(TObject *Sender)
{
	ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
// AudioOutput:
void __fastcall TAudioForm::OutputRadioGroupClick(TObject *Sender)
{
 	AudioSetup->Output = OutputRadioGroup->ItemIndex;

	PrepareOutput( OutputRadioGroup->ItemIndex);
	UpdateOutput( AudioSetup);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::SystemComboBoxChange(TObject *Sender)
{
	AudioSetup->System = SystemComboBox->ItemIndex;

  TrxAudioSystem( AudioSetup->System);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::ClickPeriodRadioGroupClick(TObject *Sender)
{
	AudioSetup->ClickPeriod[AudioSetup->Output] = ClickPeriodRadioGroup->ItemIndex;

  TrxAudioClick( AudioSetup->Output, AudioSetup->ClickPeriod[AudioSetup->Output]);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::WordClockRadioGroupClick(TObject *Sender)
{
	bool tmp;

	AudioSetup->WordClock = WordClockRadioGroup->ItemIndex;

	tmp = ( AudioSetup->WordClock == f441kHz);

  TimingEdit->Enabled = tmp;
 	TimingTrackBar->Enabled = tmp;

  TrxAudioWordClk( AudioSetup->WordClock);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::TimingUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::ToneComboBoxChange(TObject *Sender)
{
	AudioSetup->Tone[AudioSetup->Output] = ToneComboBox->ItemIndex;

  TrxAudioSignal( AudioSetup->Output, AudioSetup->Tone[AudioSetup->Output]);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LevelComboBoxChange(TObject *Sender)
{
	AudioSetup->Level[AudioSetup->Output] = LevelComboBox->ItemIndex;

  TrxAudioLevel( AudioSetup->Output, AudioSetup->Level[AudioSetup->Output]);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LockForm( bool Lock)
{
	Lock = !Lock;

	OutputRadioGroup->Enabled = Lock;
	SystemComboBox->Enabled = Lock;
  ClickPeriodRadioGroup->Enabled = Lock;
	WordClockRadioGroup->Enabled = Lock;
	TimingUpDown->Enabled = Lock;
	TimingEdit->Enabled = Lock;
	TimingTrackBar->Enabled = Lock;
	ToneComboBox->Enabled = Lock;
	LevelComboBox->Enabled = Lock;

  CancelButton->Enabled = Lock;

	if ( AudioForm->Visible)
	  AudioForm->Refresh();
}
//---------------------------------------------------------------------------
// PrepareAudioOutput
//
// Analog output:
//   System, Timing and WordClock is not possible
// AES/EBU output:
//   Timing is not possible for WordClock = 48kHz
//
// Note: Setting the ItemIndex WILL call the functions
void __fastcall TAudioForm::PrepareOutput( int output)
{
	const char* *ptr;
	bool enabled;

	OutputRadioGroup->ItemIndex = output;

	if ( output == AnalogAudio)
  {
		ptr = AnalogLevelTxt;
		LevelLabel->Caption = "dBm";
  }
	else
  {
		ptr = AESEBULevelTxt;
		LevelLabel->Caption = "dBFS";
  }

	enabled = ( output == AESEBUAudio);

	SystemComboBox->Enabled = enabled;
	WordClockRadioGroup->Enabled = enabled;

	enabled &= ( AudioSetup->WordClock == f441kHz);

  TimingEdit->Enabled = enabled;
  TimingTrackBar->Enabled = enabled;

	// Prepare for level choices
	LevelComboBox->Items->Clear();
	while ( *ptr)
 		LevelComboBox->Items->Add( *ptr++);

	SystemComboBox->ItemIndex = AudioSetup->System;
	WordClockRadioGroup->ItemIndex = AudioSetup->WordClock;

	ToneComboBox->ItemIndex = AudioSetup->Tone[output];
	LevelComboBox->ItemIndex = AudioSetup->Level[output];
  ClickPeriodRadioGroup->ItemIndex = AudioSetup->ClickPeriod[output];

//  TrxAudioOutput( output);
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::UpdateOutput( TAudioGenerator *setup)
{
  TrxAudioOutput( setup->Output);
  TrxAudioSystem( setup->System);
  TrxAudioClick( setup->Output, setup->ClickPeriod[setup->Output]);
  TrxAudioWordClk( setup->WordClock);
  TrxAudioTiming( setup->Timing);
  TrxAudioSignal( setup->Output, setup->Tone[setup->Output]);
  TrxAudioLevel( setup->Output, setup->Level[setup->Output]);
}
//---------------------------------------------------------------------------

