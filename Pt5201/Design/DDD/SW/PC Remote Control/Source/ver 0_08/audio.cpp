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
	while ( *ptr)
  	OutputRadioGroup->Items->Add( *ptr++);

	ptr = AudioSystemTxt;

	SystemComboBox->Items->Clear();
	while ( *ptr)
  	SystemComboBox->Items->Add( *ptr++);

	ptr = AudioToneTxt;

	ToneComboBox->Items->Clear();
	while ( *ptr)
  	ToneComboBox->Items->Add( *ptr++);

	ptr = AudioClickTxt;

	ClickPeriodRadioGroup->Items->Clear();
	while ( *ptr)
  	ClickPeriodRadioGroup->Items->Add( *ptr++);

	ptr = AudioWordTxt;

	WordClockRadioGroup->Items->Clear();
	while ( *ptr)
  	WordClockRadioGroup->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormShow(TObject *Sender)
{
	const char* *ptr;

  RestoreWindowPosition( AudioForm, "AudioWindow");

	// Setup pointer to actual settings
	AudioSetup = PT5201Setup->GetAudioSettings();

	AudioCancel = new TAudioGenerator;

	// Copy original settings temporarily to AudioCancel
	*AudioCancel = *AudioSetup;

	OutputRadioGroup->ItemIndex = AudioSetup->Output;
	SystemComboBox->ItemIndex = AudioSetup->System;
  ClickPeriodRadioGroup->ItemIndex = AudioSetup->ClickPeriod[AudioSetup->Output];
	WordClockRadioGroup->ItemIndex = AudioSetup->WordClock;
//	TimingEdit = AudioSetup->Timing;
	ToneComboBox->ItemIndex = AudioSetup->Tone[AudioSetup->Output];

	if ( OutputRadioGroup->ItemIndex == AnalogAudio)
		ptr = AnalogLevelTxt;
  else
		ptr = AESEBULevelTxt;

	// Prepare for level choices
	LevelComboBox->Items->Clear();
	while ( *ptr)
		LevelComboBox->Items->Add( *ptr++);

	// Setup level selection
	LevelComboBox->ItemIndex = AudioSetup->Level[AudioSetup->Output];

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

		  SendAudioOutput( AudioCancel->Output);

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
//
// Analog output:
//   System, Timing and WordClock is not possible
// AES/EBU output:
//   Timing is not possible for WordClock = 48kHz
//
//
void __fastcall TAudioForm::OutputRadioGroupClick(TObject *Sender)
{
	const char* *ptr;
	bool enabled;

 	AudioSetup->Output = OutputRadioGroup->ItemIndex;

	if ( AudioSetup->Output == AnalogAudio)
		ptr = AnalogLevelTxt;
	else
		ptr = AESEBULevelTxt;

	enabled = ( AudioSetup->Output == AESEBUAudio);

	SystemComboBox->Enabled = enabled;
	WordClockRadioGroup->Enabled = enabled;

	enabled &= ( AudioSetup->WordClock == f441kHz);

  TimingEdit->Enabled = enabled;
  TimingTrackBar->Enabled = enabled;

	// Prepare for level choices
	LevelComboBox->Items->Clear();
	while ( *ptr)
 		LevelComboBox->Items->Add( *ptr++);

	ToneComboBox->ItemIndex = AudioSetup->Tone[AudioSetup->Output];
	LevelComboBox->ItemIndex = AudioSetup->Level[AudioSetup->Output];
  ClickPeriodRadioGroup->ItemIndex = AudioSetup->ClickPeriod[AudioSetup->Output];

  SendAudioOutput( AudioSetup->Output);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::SystemComboBoxChange(TObject *Sender)
{
	AudioSetup->System = SystemComboBox->ItemIndex;

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::ClickPeriodRadioGroupClick(TObject *Sender)
{
	AudioSetup->ClickPeriod[AudioSetup->Output] = ClickPeriodRadioGroup->ItemIndex;

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

  SendAudioWordClk( AudioSetup->WordClock);

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

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LevelComboBoxChange(TObject *Sender)
{
	AudioSetup->Level[AudioSetup->Output] = LevelComboBox->ItemIndex;

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
