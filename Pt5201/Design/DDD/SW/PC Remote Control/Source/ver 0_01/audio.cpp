//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "audio.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TAudioForm *AudioForm;
TAudioGenerator *AudioSetup;
//---------------------------------------------------------------------------
__fastcall TAudioForm::TAudioForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormCreate(TObject *Sender)
{
	const char* *ptr = AudioToneTxt;

	ToneComboBox->Items->Clear();
	while ( *ptr)
  	ToneComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	AudioSetup = new TAudioGenerator;

  RestoreWindowPosition( AudioForm, "AudioWindow");

	PT5201Setup->GetAudioSettings( AudioSetup);

	OutputRadioGroup->ItemIndex = AudioSetup->Output;
	SystemComboBox->ItemIndex = AudioSetup->System;
  ClickPeriodRadioGroup->ItemIndex = AudioSetup->ClickPeriod[AudioSetup->Output];
	SamplingFreqRadioGroup->ItemIndex = AudioSetup->SamplingFreq;
//	TimingEdit = AudioSetup->Timing;
	ToneComboBox->ItemIndex = AudioSetup->Tone[AudioSetup->Output];

	if ( OutputRadioGroup->ItemIndex == AnalogAudio)
		ptr = AnalogLevelTxt;
  else
		ptr = AESEBULevelTxt;

	LevelComboBox->Items->Clear();
	while ( *ptr)
		LevelComboBox->Items->Add( *ptr++);

	LevelComboBox->ItemIndex = AudioSetup->Level[AudioSetup->Output];

	LockForm( PT5201Form->ConfigureLockItem->Checked);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete AudioSetup;

  SaveWindowPosition( AudioForm, "AudioWindow");
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModalResult != mrOk)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetAudioSettings( AudioSetup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::SystemComboBoxChange(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::OutputRadioGroupClick(TObject *Sender)
{
	const char* *ptr;
	bool tmp;

 	AudioSetup->Output = OutputRadioGroup->ItemIndex;

	if ( tmp = ( AudioSetup->Output == AnalogAudio))
		ptr = AnalogLevelTxt;
	else
		ptr = AESEBULevelTxt;

	tmp = !tmp;

	SystemComboBox->Enabled = tmp;
	SamplingFreqRadioGroup->Enabled = tmp;
  TimingEdit->Enabled = tmp;
  TimingTrackBar->Enabled = tmp;

	LevelComboBox->Items->Clear();
	while ( *ptr)
 		LevelComboBox->Items->Add( *ptr++);

	ToneComboBox->ItemIndex = AudioSetup->Tone[AudioSetup->Output];
	LevelComboBox->ItemIndex = AudioSetup->Level[AudioSetup->Output];
  ClickPeriodRadioGroup->ItemIndex = AudioSetup->ClickPeriod[AudioSetup->Output];

  UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::ClickPeriodRadioGroupClick(TObject *Sender)
{
	AudioSetup->ClickPeriod[AudioSetup->Output] = ClickPeriodRadioGroup->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::SamplingFreqRadioGroupClick(TObject *Sender)
{
	AudioSetup->SamplingFreq = SamplingFreqRadioGroup->ItemIndex;

  UpdateLEDs();
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
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LevelComboBoxChange(TObject *Sender)
{
	AudioSetup->Level[AudioSetup->Output] = LevelComboBox->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LockForm( bool Lock)
{
	Lock = !Lock;

	OutputRadioGroup->Enabled = Lock;
	SystemComboBox->Enabled = Lock;
  ClickPeriodRadioGroup->Enabled = Lock;
	SamplingFreqRadioGroup->Enabled = Lock;
	TimingEdit->Enabled = Lock;
	TimingTrackBar->Enabled = Lock;
	ToneComboBox->Enabled = Lock;
	LevelComboBox->Enabled = Lock;

  CancelButton->Enabled = Lock;

	if ( AudioForm->Visible)
	  AudioForm->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::UpdateLEDs( void)
{
	if ( OutputRadioGroup->ItemIndex == AnalogAudio)
  {
	 	PT5201Form->AudioAnalogImage->Picture = PT5201Form->LEDOnImage->Picture;
   	PT5201Form->AudioAESEBUImage->Picture = PT5201Form->LEDOffImage->Picture;
  }
  else
  {
	 	PT5201Form->AudioAnalogImage->Picture = PT5201Form->LEDOffImage->Picture;
   	PT5201Form->AudioAESEBUImage->Picture = PT5201Form->LEDOnImage->Picture;
	}

	if ( SamplingFreqRadioGroup->ItemIndex == f441kHz)
  {
	 	PT5201Form->Word48Image->Picture = PT5201Form->LEDOffImage->Picture;
	 	PT5201Form->Word441Image->Picture = PT5201Form->LEDOnImage->Picture;
  }
  else
  {
	 	PT5201Form->Word48Image->Picture = PT5201Form->LEDOnImage->Picture;
   	PT5201Form->Word441Image->Picture = PT5201Form->LEDOffImage->Picture;
	}
}
//---------------------------------------------------------------------------
