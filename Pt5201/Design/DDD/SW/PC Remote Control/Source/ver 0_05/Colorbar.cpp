//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "Colorbar.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TCBarForm *CBarForm;
TCBarGenerator *CBarSetup;
//---------------------------------------------------------------------------
__fastcall TCBarForm::TCBarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormCreate(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( CBarForm, "CBarWindow");

	CBarSetup = new TCBarGenerator;

	PT5201Setup->GetCBarSettings( CBarSetup);

	// Prepare TV system choice
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->ItemIndex = CBarSetup->System;

	// Prepare embedded audio choice
	EmbAudioComboBox->Items->Clear();

  ptr = EmbAudioToneTxt;
	while ( *ptr)
		EmbAudioComboBox->Items->Add( *ptr++);
	EmbAudioComboBox->ItemIndex = CBarSetup->EmbAudio;

	// Prepare pattern choice, (this is system dependant)
	PatternComboBox->Items->Clear();

	if ( CBarSetup->System < NTSC)
		ptr = PALPatternTxt;
  else
		ptr = NTSCPatternTxt;

	while ( *ptr)
		PatternComboBox->Items->Add( *ptr++);
	PatternComboBox->ItemIndex = CBarSetup->Pattern;

	// Prepare to display timing values
	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete CBarSetup;

  SaveWindowPosition( CBarForm, "CBarWindow");
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModalResult != mrOk)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetCBarSettings( CBarSetup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::SystemComboBoxChange(TObject *Sender)
{
	const char* *ptr;

	if ( ValidateTiming( SystemComboBox->ItemIndex, StrToInt( FieldEdit->Text),
											 StrToInt( LineEdit->Text), &CBarSetup->Samples, OffsetTrackBar))
  {
		CBarSetup->System = SystemComboBox->ItemIndex;

		if ( SystemComboBox->ItemIndex < NTSC)
    {
			OffsetTrackBar->Max = 1250;
			OffsetTrackBar->Min = -1249;

			ptr = PALPatternTxt;
		}
	  else
    {
			OffsetTrackBar->Max = 525;
			OffsetTrackBar->Min = -524;

			ptr = NTSCPatternTxt;
		}

		PatternComboBox->Items->Clear();
		while ( *ptr)
			PatternComboBox->Items->Add( *ptr++);

		PatternComboBox->ItemIndex = CBarSetup->Pattern;
	}
  else
		SystemComboBox->ItemIndex = CBarSetup->System;

	SamplesToFLO( SystemComboBox->ItemIndex, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &CBarSetup->Samples, FieldEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &CBarSetup->Samples, LineEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &CBarSetup->Samples, OffsetEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &CBarSetup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

	ScHPhaseEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::ScHPhaseEditExit(TObject *Sender)
{
	int error;
															// Restore old value
	if ( !CancelButton->Focused())
  {
		error = EnterScHPhase( ScHPhaseEdit);

		if ( OKButton->Focused() && error)
    	ActiveControl = ScHPhaseEdit;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::PatternComboBoxChange(TObject *Sender)
{
	CBarSetup->Pattern = PatternComboBox->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
  PatternComboBox->Enabled = Lock;
	FieldUpDown->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineUpDown->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetUpDown->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;
	ScHPhaseUpDown->Enabled = Lock;
	ScHPhaseEdit->Enabled = Lock;
	EmbAudioComboBox->Enabled = Lock;

	CancelButton->Enabled = Lock;

	if ( CBarForm->Visible)
	  CBarForm->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::UpdateLEDs( void)
{
	UpdateSystemLEDs( SystemComboBox, PT5201Form->CBarPALImage, PT5201Form->CBarNTSCImage);
}
//---------------------------------------------------------------------------
