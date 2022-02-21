//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
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
//
//---------------------------------------------------------------------------
__fastcall TBlack3Form::TBlack3Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormCreate(TObject *Sender)
{
 	const char* *ptr;

	// Prepare TV system choices
	SystemComboBox->Items->Clear();

  ptr = SystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
//
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

  if ( !PropertiesForm->ConfigureLockItem->Checked)
		ActiveControl = SystemComboBox;

	FieldEdit->Tag = 0;
	LineEdit->Tag = 0;
	OffsetEdit->Tag = 0;

  ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete BlackCancel;

	SaveWindowPosition( Black3Form, "Black3Window");
}
//---------------------------------------------------------------------------
//
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
		{
    	PT5201Setup->SetBlackSettings( Black3Output, BlackCancel);
			UpdateOutput( BlackCancel);
		}
  }
	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OKButtonClick( TObject *Sender)
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
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::CancelButtonClick(TObject *Sender)
{
	ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::SystemComboBoxChange(TObject *Sender)
{
	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &BlackSetup->Samples, OffsetTrackBar))
  {
		BlackSetup->System = SystemComboBox->ItemIndex;

    TrackBarSetup( BlackSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

		SamplesToFLO( SystemComboBox->ItemIndex, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

		TrxBBSystem( Black3Output, BlackSetup->System);
	}
  else
		SystemComboBox->ItemIndex = BlackSetup->System;

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

  ScHPhaseAdjust( &BlackSetup->ScHPhase, ScHPhaseEdit, Button);

	TrxBBScHPhase( Black3Output, BlackSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::ScHPhaseEditExit(TObject *Sender)
{
	if ( ValidateScHPhase( Black3Form, ScHPhaseEdit, &BlackSetup->ScHPhase))
		TrxBBScHPhase( Black3Output, BlackSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

	TimingAdjust( BlackSetup->System, &BlackSetup->Samples, BlackType, FieldEdit, Button);

	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

	TimingAdjust( BlackSetup->System, &BlackSetup->Samples, BlackType, LineEdit, Button);

	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineEditExit(TObject *Sender)
{
	LineEdit->Tag = true;

	ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetUpDownClick(TObject *Sender,	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

	TimingAdjust( BlackSetup->System, &BlackSetup->Samples, BlackType, OffsetEdit, Button);

	SamplesToFLO( BlackSetup->System, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( Black3Form, BlackSetup->System, &BlackSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( BlackSetup->System, BlackSetup->Samples, OffsetTrackBar);

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &BlackSetup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, BlackSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrxBBDelay( Black3Output, FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( Black3Field);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LockForm( bool Lock)
{
	bool enable = !Lock;

	SystemComboBox->Enabled = enable;
	FieldUpDown->Enabled = enable;
	FieldEdit->Enabled = enable;
	LineUpDown->Enabled = enable;
	LineEdit->Enabled = enable;
	OffsetUpDown->Enabled = enable;
	OffsetEdit->Enabled = enable;
	OffsetTrackBar->Enabled = enable;
	ScHPhaseUpDown->Enabled = enable;
	ScHPhaseEdit->Enabled = enable;

  CancelButton->Enabled = enable;

	if ( Black3Form->Visible)
	  Black3Form->Refresh();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::UpdateOutput( TBlackGenerator *setup)
{
	TrxBBSystem( Black3Output, setup->System);

	SamplesToFLO( setup->System, setup->Samples, Field1Edit, Line1Edit, Offset1Edit);

	TrxBBDelay( Black3Output, Field1Edit->Text.ToInt(), Line1Edit->Text.ToInt(), Offset1Edit->Text.ToDouble());
	TrxBBScHPhase( Black3Output, setup->ScHPhase);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::HelpButtonClick(TObject *Sender)
{
	Application->HelpContext( IDH_EDITBB);
}
//---------------------------------------------------------------------------
