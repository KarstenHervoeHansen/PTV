//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "genlock.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TGenlockForm *GenlockForm;
//---------------------------------------------------------------------------
__fastcall TGenlockForm::TGenlockForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormCreate(TObject *Sender)
{
	const char* *ptr;

	// Prepare Genlock system choice
	SystemComboBox->Items->Clear();

  ptr = GenlockSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormShow(TObject *Sender)
{
	RestoreWindowPosition( GenlockForm, "GenlockWindow");

	// Setup pointer to actual settings
	GenlockSetup = PT5201Setup->GetGenlockSettings();

	GenlockCancel = new TGenlockGenerator;

	// Copy original settings temporarily to GenlockCancel
	*GenlockCancel = *GenlockSetup;

	// Setup TV system selection
	SystemComboBox->ItemIndex = GenlockSetup->System;

	// Display timing selection
	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

	if (( GenlockSetup->System == InternalGenlock) || ( GenlockSetup->System == f10MHzGenlock))
	{
		FieldEdit->Enabled = false;
	 	LineEdit->Enabled = false;
	 	OffsetEdit->Enabled = false;
	 	OffsetTrackBar->Enabled = false;
	}

	TrackBarSetup( PAL, OffsetTrackBar, MinLabel, MaxLabel);

  if ( !PropertiesForm->ConfigureLockItem->Checked)
		ActiveControl = SystemComboBox;

	FieldEdit->Tag = 0;
	LineEdit->Tag = 0;
	OffsetEdit->Tag = 0;

  ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete GenlockCancel;

	SaveWindowPosition( GenlockForm, "GenlockWindow");
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormCloseQuery(TObject *Sender, bool &CanClose)
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
    	PT5201Setup->SetGenlockSettings( GenlockCancel);

			SendGenlockSystem( GenlockCancel->System);

			PT5201Form->SystemStatusUpdate();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OKButtonClick(TObject *Sender)
{
	if ( FieldEdit->Focused())
		FieldEdit->Tag = true;
  else
		if ( LineEdit->Focused())
			LineEdit->Tag = true;
  else
		if ( OffsetEdit->Focused())
			OffsetEdit->Tag = true;

	if ( FieldEdit->Tag || LineEdit->Tag || OffsetEdit->Tag)
		if ( !ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  												FieldEdit, LineEdit, OffsetEdit))
	  	return;

	ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::CancelButtonClick(TObject *Sender)
{
	ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::SystemComboBoxChange(TObject *Sender)
{
	bool EditEnable;

	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &GenlockSetup->Samples, \
														OffsetTrackBar))
	{
		GenlockSetup->System = SystemComboBox->ItemIndex;

		EditEnable = true;

		switch ( GenlockSetup->System)
    {
    	case PALGenlock:
				TrackBarSetup( PAL, OffsetTrackBar, MinLabel, MaxLabel);
				break;

    	case NTSCGenlock:
				TrackBarSetup( NTSC, OffsetTrackBar, MinLabel, MaxLabel);
				break;

			default:
				EditEnable = false;
				break;
		}

		if ( !PropertiesForm->ConfigureLockItem->Checked)
		{
			FieldUpDown->Enabled = EditEnable;
			FieldEdit->Enabled = EditEnable;
			LineUpDown->Enabled = EditEnable;
		 	LineEdit->Enabled = EditEnable;
			OffsetUpDown->Enabled = EditEnable;
		 	OffsetEdit->Enabled = EditEnable;
		 	OffsetTrackBar->Enabled = EditEnable;
		}

		SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

		SendGenlockSystem( GenlockSetup->System);
	}
  else
		SystemComboBox->ItemIndex = GenlockSetup->System;

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( GenlockSetup->System, &GenlockSetup->Samples, GenlockType,	FieldEdit, Button);

	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( GenlockSetup->System, &GenlockSetup->Samples, GenlockType, LineEdit, Button);

	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineEditExit(TObject *Sender)
{
	LineEdit->Tag = true;

	ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( GenlockSetup->System, &GenlockSetup->Samples, GenlockType, OffsetEdit, Button);

	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LockForm( bool Lock)
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

	CancelButton->Enabled = Lock;

  if ( GenlockForm->Visible)
  	GenlockForm->Refresh();
}
//---------------------------------------------------------------------------
