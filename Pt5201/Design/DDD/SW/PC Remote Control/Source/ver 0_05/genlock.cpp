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
TGenlockGenerator* GenlockSetup;
//---------------------------------------------------------------------------
__fastcall TGenlockForm::TGenlockForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormCreate(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( GenlockForm, "GenlockWindow");

	GenlockSetup = new TGenlockGenerator;

	PT5201Setup->GetGenlockSettings( GenlockSetup);

	// Prepare Genlock system choice
	SystemComboBox->Items->Clear();

  ptr = GenlockSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->ItemIndex = GenlockSetup->System;

	// Prepare to display timing values
	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

	if (( GenlockSetup->System == InternalGenlock) || ( GenlockSetup->System == f10MHzGenlock))
	{
		FieldEdit->Enabled = false;
	 	LineEdit->Enabled = false;
	 	OffsetEdit->Enabled = false;
	 	OffsetTrackBar->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete GenlockSetup;

	SaveWindowPosition( GenlockForm, "GenlockWindow");
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModalResult != mrOk)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetGenlockSettings( GenlockSetup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::SystemComboBoxChange(TObject *Sender)
{
	bool EditEnable;

	if ( ValidateTiming( SystemComboBox->ItemIndex, StrToInt( FieldEdit->Text),
											 StrToInt( LineEdit->Text), &GenlockSetup->Samples, OffsetTrackBar))
	{
		GenlockSetup->System = SystemComboBox->ItemIndex;

		EditEnable = true;

		switch ( SystemComboBox->ItemIndex)
    {
    	case PALGenlock:
				OffsetTrackBar->Max = 1250;
				OffsetTrackBar->Min = -1249;
				break;

    	case NTSCGenlock:
				OffsetTrackBar->Max = 525;
				OffsetTrackBar->Min = -524;
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
	}
  else
		SystemComboBox->ItemIndex = GenlockSetup->System;

	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);
//  TrackBarAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, OffsetTrackBar);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, FieldEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, LineEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, OffsetEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);
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
