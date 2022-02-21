// 19.01.2002: JK: Timing in Audio is not possible if Word Clock is 44.1 kHz
//                 Genlock LED ON/OFF tested with PT5201 Software 2.3


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

extern Boolean timeout2; // Global variable indicating Timer2 timeout

TAudioForm *AudioForm;
bool ValidateTiming( TForm *Form, TEdit *Edit, int *Timing);

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TAudioForm::TAudioForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormCreate(TObject *Sender)
{
        const char* *ptr;

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

        TimingTrackBar->Max = 13;
        TimingTrackBar->Min = -12;
        TimingTrackBar->Frequency = 1;
}
//---------------------------------------------------------------------------
//
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
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormClose(TObject *Sender, TCloseAction &Action)
{
        delete AudioCancel;

  SaveWindowPosition( AudioForm, "AudioWindow");
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        if (( ModelessResult != mrOk) && !PT5201Setup->VerifyAudioSettings( AudioCancel))
        {
                if  ( Application->MessageBox( "Close window and loose all changes ?",\
                                                                                                                         "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
                {
                        ModelessResult = mrNone;

                        CanClose = false;
                }
    else                                                                                                                // Restore original settings
                {
        PT5201Setup->SetAudioSettings( AudioCancel);

                        UpdateOutput( AudioCancel);
                }
        }
        PT5201Form->SystemStatusUpdate( AudioField);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::OKButtonClick(TObject *Sender)
{
        if ( TimingEdit->Focused())
         if ( !ValidateTiming( AudioForm, TimingEdit, &AudioSetup->Timing))
                return;

        ModelessResult = mrOk;

        Close();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::CancelButtonClick(TObject *Sender)
{
        ModelessResult = mrCancel;

        Close();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::OutputRadioGroupClick(TObject *Sender)
{
        AudioSetup->Output = OutputRadioGroup->ItemIndex;

        PrepareOutput( OutputRadioGroup->ItemIndex);
        UpdateOutput( AudioSetup);

        PT5201Form->SystemStatusUpdate( AudioField);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::SystemComboBoxChange(TObject *Sender)
{
        AudioSetup->System = SystemComboBox->ItemIndex;

  TrxAudioSystem( AudioSetup->System);

        PT5201Form->SystemStatusUpdate( AudioField);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::ClickPeriodRadioGroupClick(TObject *Sender)
{
        AudioSetup->ClickPeriod[AudioSetup->Output] = ClickPeriodRadioGroup->ItemIndex;

  TrxAudioClick( AudioSetup->Output, AudioSetup->ClickPeriod[AudioSetup->Output]);

        PT5201Form->SystemStatusUpdate( AudioField);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::WordClockRadioGroupClick(TObject *Sender)
{
        bool enable;

        AudioSetup->WordClock = WordClockRadioGroup->ItemIndex;

        enable = ( AudioSetup->WordClock == f48kHz);

        TimingUpDown->Enabled = enable;
  TimingEdit->Enabled = enable;
        TimingTrackBar->Enabled = enable;

  TrxAudioWordClk( AudioSetup->WordClock);

        PT5201Form->SystemStatusUpdate( AudioField);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::TimingUpDownClick(TObject *Sender, TUDBtnType Button)
  {
  ActiveControl = TimingEdit;

  if ( Button == btNext)
    AudioSetup->Timing += AESTimingStep;
  else
    AudioSetup->Timing -= AESTimingStep;

  if ( AudioSetup->Timing >= MaxAESTiming)
    AudioSetup->Timing = MaxAESTiming;

  if ( AudioSetup->Timing <= MinAESTiming)
    AudioSetup->Timing = MinAESTiming;

  TimingEdit->Text = FloatToStrF(((float)AudioSetup->Timing)/10., ffFixed, 5, 1);
  TimingEdit->SelectAll();

  TimingTrackBar->Position = AudioSetup->Timing/AESTimingStep;
  TrxAudioTiming( AudioSetup->Timing);

  PT5201Form->SystemStatusUpdate( AudioField); /* from settings to status */
  }
//---------------------------------------------------------------------------
void __fastcall TAudioForm::TimingEditExit(TObject *Sender)
  {
  if (ValidateTiming( AudioForm, TimingEdit, &AudioSetup->Timing))
    {
    TimingTrackBar->Position = AudioSetup->Timing/AESTimingStep;
    TrxAudioTiming( AudioSetup->Timing);
    }
  PT5201Form->SystemStatusUpdate( AudioField);
  }
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::TimingTrackBarChange(TObject *Sender)
  {
  AudioSetup->Timing = AESTimingStep*TimingTrackBar->Position;
  TimingEdit->Text = FloatToStrF(((float)AudioSetup->Timing)/10., ffFixed, 5, 1);
  TrxAudioTiming( AudioSetup->Timing);

  PT5201Form->SystemStatusUpdate( AudioField);
  }
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::ToneComboBoxChange(TObject *Sender)
  {
  AudioSetup->Tone[AudioSetup->Output] = ToneComboBox->ItemIndex;

  TrxAudioSignal( AudioSetup->Output, AudioSetup->Tone[AudioSetup->Output]);

  PT5201Form->SystemStatusUpdate( AudioField);
  }
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LevelComboBoxChange(TObject *Sender)
{
        AudioSetup->Level[AudioSetup->Output] = LevelComboBox->ItemIndex;

  TrxAudioLevel( AudioSetup->Output, AudioSetup->Level[AudioSetup->Output]);

        PT5201Form->SystemStatusUpdate( AudioField);
}
//---------------------------------------------------------------------------
// LockForm
//
// Analog output:
//   System, Timing and WordClock is not possible
// AES/EBU output:
//   Timing is not possible for WordClock = 441kHz
//---------------------------------------------------------------------------
void __fastcall TAudioForm::LockForm( bool Lock)
{
        bool enable = !Lock;

        OutputRadioGroup->Enabled = enable;
  ClickPeriodRadioGroup->Enabled = enable;
        ToneComboBox->Enabled = enable;
        LevelComboBox->Enabled = enable;
  CancelButton->Enabled = enable;

        enable &= !( OutputRadioGroup->ItemIndex == AnalogAudio);

        SystemComboBox->Enabled = enable;
        WordClockRadioGroup->Enabled = enable;

        enable &= ( WordClockRadioGroup->ItemIndex == f48kHz);

        TimingUpDown->Enabled = enable;
        TimingEdit->Enabled = enable;
        TimingTrackBar->Enabled = enable;

        if ( AudioForm->Visible)
          AudioForm->Refresh();
}
//---------------------------------------------------------------------------
// PrepareAudioOutput
//
// Analog output:
//   System, Timing and WordClock is not possible
// AES/EBU output:
//   Timing is not possible for WordClock = 441kHz
//
// Note: Setting the ItemIndex WILL call the functions
//---------------------------------------------------------------------------
void __fastcall TAudioForm::PrepareOutput( int output)
{
        const char* *ptr;
        bool enabled;

        OutputRadioGroup->ItemIndex = output;

        if ( output == AnalogAudio)
  {
                ptr = AnalogLevelTxt;
                LevelLabel->Caption = "dBu"; // JK change frm dBm
  }
        else
  {
                ptr = AESEBULevelTxt;
                LevelLabel->Caption = "dBFS";
  }

        enabled = ( output == AESEBUAudio);

        SystemComboBox->Enabled = enabled;
        WordClockRadioGroup->Enabled = enabled;

        enabled &= ( AudioSetup->WordClock == f48kHz);
        TimingUpDown->Enabled = enabled;
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

        // Setup audio timing selection   short
        TimingUpDown->Position = AudioSetup->Timing;
        TimingEdit->Text = FloatToStrF(((float)AudioSetup->Timing)/10., ffFixed, 5, 1);
        TimingTrackBar->Position = AudioSetup->Timing/AESTimingStep;
}
//---------------------------------------------------------------------------
//
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
//
//---------------------------------------------------------------------------
bool ValidateTiming( TForm *Form, TEdit *Edit, int *Timing)
  {
  double TextValue;
  int Tmp;
  bool CanUpdate = true;

  try
    {
    TextValue = Edit->Text.ToDouble();
    }
  catch(...)
    {
    Application->MessageBox( "Invalid data", "Error", MB_OK|MB_ICONERROR);
    CanUpdate = false;
    TextValue = 0;
    }

  if ((( TextValue > (float)MaxAESTiming/10.) || ( TextValue < (float)MinAESTiming/10.)) && CanUpdate)
    {
    char *buffer = new char[50];
    sprintf( buffer, "Value must be between %4.1f and %4.1f\0", (float)MinAESTiming/10., (float)MaxAESTiming/10.);
    Application->MessageBox( buffer, "Error", MB_OK|MB_ICONERROR);
    CanUpdate = false;
    delete [] buffer;
    }

  if ( CanUpdate)
    {
    Tmp = (int)( 10.0001*TextValue);;
    *Timing = 8*( Tmp/8);

  if (( abs( Tmp) % 8) >= 4)
    {
    if ( Tmp >= 0)
      *Timing += 8;
    else
      *Timing -= 8;
    }
  Edit->Text = FloatToStrF(((float)*Timing)/10., ffFixed, 5, 1);
    }
  else
    {
    Form->ActiveControl = Edit;
    Edit->Text = FloatToStrF(((float)*Timing)/10., ffFixed, 5, 1);
    Edit->SelectAll();
    }
  return( CanUpdate);
  }
//---------------------------------------------------------------------------
void __fastcall TAudioForm::HelpButtonClick(TObject *Sender)
{
        Application->HelpContext( IDH_EDITAUDIO);
}
//---------------------------------------------------------------------------
void __fastcall TAudioForm::TimingUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	TimingUpDown->Position = 0;
}

//---------------------------------------------------------------------------
