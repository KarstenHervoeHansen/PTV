//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "black3.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack3Form *Black3Form;
//---------------------------------------------------------------------------
__fastcall TBlack3Form::TBlack3Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormCreate(TObject *Sender)
{
 	const char* *ptr;

	// Prepare TV system choices
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormShow(TObject *Sender)
{
	RestoreWindowPosition( Black3Form, "Black3Window");

	// Setup pointer to actual settings
	BlackSetup = PT5201Setup->GetBlackSettings( Black3Output);

	BlackCancel = new TBlackGenerator;

	// Copy original settings temporarily to BlackCancel
	*BlackCancel = *BlackSetup;

	// Setup TV system selection
	SystemComboBox->ItemIndex = BlackSetup->System;

	// Setup ScHPhase selection
	ScHPhaseEdit->Text = IntToStr( BlackSetup->ScHPhase);

	// Display timing selection
	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  TrackBarSetup( BlackSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

	ActiveControl = SystemComboBox;

	FieldEdit->Tag = 0;
	LineEdit->Tag = 0;
	OffsetEdit->Tag = 0;

  ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete BlackCancel;

	SaveWindowPosition( Black3Form, "Black3Window");
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormCloseQuery(TObject *Sender, bool &CanClose)
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
    	PT5201Setup->SetBlackSettings( Black3Output, BlackCancel);
  }
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OKButtonClick(TObject *Sender)
{
	if ( ScHPhaseEdit->Focused())
		if ( !ValidateScHPhase( Black3Form, ScHPhaseEdit, &BlackSetup->ScHPhase))
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
		if ( !ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  												FieldEdit, LineEdit, OffsetEdit))
	  	return;

  ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::CancelButtonClick(TObject *Sender)
{
	ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::SystemComboBoxChange(TObject *Sender)
{
	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &BlackSetup->Samples, OffsetTrackBar))
  {
		BlackSetup->System = SystemComboBox->ItemIndex;

    TrackBarSetup( BlackSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

		SamplesToFLO( SystemComboBox->ItemIndex, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);
	}
  else
		SystemComboBox->ItemIndex = BlackSetup->System;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

  ScHPhaseAdjust( &BlackSetup->ScHPhase, ScHPhaseEdit, Button);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::ScHPhaseEditExit(TObject *Sender)
{
	ValidateScHPhase( Black3Form, ScHPhaseEdit, &BlackSetup->ScHPhase);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( BlackSetup->System, &BlackSetup->Samples, CBarType,	FieldEdit, Button);

	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( BlackSetup->System, &BlackSetup->Samples, CBarType,	LineEdit, Button);

	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineEditExit(TObject *Sender)
{
	LineEdit->Tag =true;

	ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( BlackSetup->System, &BlackSetup->Samples, CBarType,	OffsetEdit, Button);

	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &BlackSetup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
	FieldUpDown->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineUpDown->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetUpDown->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;
	ScHPhaseUpDown->Enabled = Lock;
	ScHPhaseEdit->Enabled = Lock;

  CancelButton->Enabled = Lock;

	if ( Black3Form->Visible)
	  Black3Form->Refresh();
}
//---------------------------------------------------------------------------
