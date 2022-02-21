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
//---------------------------------------------------------------------------
__fastcall TCBarForm::TCBarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormCreate(TObject *Sender)
{
	const char* *ptr;

	// Prepare TV system choices
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);

	// Prepare Embedded Audio choices
	EmbAudioComboBox->Items->Clear();

  ptr = EmbAudioToneTxt;
	while ( *ptr)
		EmbAudioComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( CBarForm, "CBarWindow");

	// Setup pointer to actual settings
	CBarSetup = PT5201Setup->GetCBarSettings();

	CBarCancel = new TCBarGenerator;

	// Copy original settings temporarily to CBarCancel
	*CBarCancel = *CBarSetup;

	// Setup TV system selection
	SystemComboBox->ItemIndex = CBarSetup->System;

	// Setup ScHPhase selection
	ScHPhaseEdit->Text = IntToStr( CBarSetup->ScHPhase);

	// Setup Embedded Audio selection
	EmbAudioComboBox->ItemIndex = CBarSetup->EmbAudio;

	// Prepare pattern choices, (this one is system dependant)
	if ( CBarSetup->System < NTSC)
		ptr = PALPatternTxt;
  else
		ptr = NTSCPatternTxt;

	PatternComboBox->Items->Clear();
	while ( *ptr)
		PatternComboBox->Items->Add( *ptr++);

	// Setup pattern selection
	PatternComboBox->ItemIndex = CBarSetup->Pattern;

	// Display timing selection
	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	// Setup TrackBar markings and resolution
  TrackBarSetup( CBarSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

										// It is not possible to foxus an invisible or disabled
										//  window, hence the statement below.
	if ( AnalogSettingsTabSheet->Visible)
		ActiveControl = SystemComboBox;
	else
		ActiveControl = EmbAudioComboBox;

										// Edit tags are used during validation of an entry to
										//  separate which edit field is used.
	FieldEdit->Tag = 0;
	LineEdit->Tag = 0;
	OffsetEdit->Tag = 0;

  ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete CBarCancel;

  SaveWindowPosition( CBarForm, "CBarWindow");
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormCloseQuery(TObject *Sender, bool &CanClose)
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
    	PT5201Setup->SetCBarSettings( CBarCancel);
	}
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OKButtonClick(TObject *Sender)
{
	if ( ScHPhaseEdit->Focused())
		if ( !ValidateScHPhase( CBarForm, ScHPhaseEdit, &CBarSetup->ScHPhase))
  		return;

	if ( FieldEdit->Focused())
		FieldEdit->Tag = true;
  else
		if ( LineEdit->Focused())
			LineEdit->Tag = true;
  else
		if ( OffsetEdit->Focused())
			OffsetEdit->Tag = true;

	if ( FieldEdit->Tag || LineEdit->Tag || OffsetEdit->Tag)
		if ( !ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  												FieldEdit, LineEdit, OffsetEdit))
	  	return;

  ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::CancelButtonClick(TObject *Sender)
{
  ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::SystemComboBoxChange(TObject *Sender)
{
	const char* *ptr;

	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &CBarSetup->Samples, \
  																OffsetTrackBar))
  {
		CBarSetup->System = SystemComboBox->ItemIndex;

		if ( CBarSetup->System < NTSC)
			ptr = PALPatternTxt;
	  else
			ptr = NTSCPatternTxt;

	// Prepare pattern choices, (this one is system dependant)
		PatternComboBox->Items->Clear();
		while ( *ptr)
			PatternComboBox->Items->Add( *ptr++);

		PatternComboBox->ItemIndex = CBarSetup->Pattern;

    TrackBarSetup( CBarSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

		SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);
	}
  else
		SystemComboBox->ItemIndex = CBarSetup->System;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

  ScHPhaseAdjust( &CBarSetup->ScHPhase, ScHPhaseEdit, Button);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::ScHPhaseEditExit( TObject *Sender)
{
	ValidateScHPhase( CBarForm, ScHPhaseEdit, &CBarSetup->ScHPhase);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( CBarSetup->System, &CBarSetup->Samples, CBarType,	FieldEdit, Button);

	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( CBarSetup->System, &CBarSetup->Samples, CBarType,	LineEdit, Button);

	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineEditExit(TObject *Sender)
{
	LineEdit->Tag = true;

	ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( CBarSetup->System, &CBarSetup->Samples, CBarType,	OffsetEdit, Button);

	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( CBarSetup->System, &CBarSetup->Samples, OffsetTrackBar);
	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::PatternComboBoxChange(TObject *Sender)
{
	CBarSetup->Pattern = PatternComboBox->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::EmbAudioComboBoxChange(TObject *Sender)
{
	CBarSetup->EmbAudio = EmbAudioComboBox->ItemIndex;
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
