//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "black2.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack2Form *Black2Form;
TBlack2Generator *Black2Setup;
//---------------------------------------------------------------------------
__fastcall TBlack2Form::TBlack2Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FormCreate(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( Black2Form, "Black2Window");

	Black2Setup = new TBlack2Generator;

	PT5201Setup->GetBlack2Settings( Black2Setup);

	// Prepare TV system choice
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->ItemIndex = Black2Setup->System;

	// Prepare to display timing values
	SamplesToFLO( Black2Setup->System, Black2Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete Black2Setup;

	SaveWindowPosition( Black2Form, "Black2Window");
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModalResult != mrOk)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetBlack2Settings( Black2Setup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::SystemComboBoxChange(TObject *Sender)
{
	if ( ValidateTiming( SystemComboBox->ItemIndex, StrToInt( FieldEdit->Text),
											 StrToInt( LineEdit->Text), &Black2Setup->Samples, OffsetTrackBar))
  {
		Black2Setup->System = SystemComboBox->ItemIndex;

		if ( SystemComboBox->ItemIndex < NTSC)
    {
			OffsetTrackBar->Max = 1250;
			OffsetTrackBar->Min = -1249;
		}
	  else
    {
			OffsetTrackBar->Max = 525;
			OffsetTrackBar->Min = -524;
		}
	}
  else
		SystemComboBox->ItemIndex = Black2Setup->System;

	SamplesToFLO( SystemComboBox->ItemIndex, Black2Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FieldUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black2Setup->Samples, FieldEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black2Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black2Setup->Samples, LineEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black2Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black2Setup->Samples, OffsetEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black2Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &Black2Setup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, Black2Setup->Samples, FieldEdit, LineEdit, OffsetEdit);
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::LockForm( bool Lock)
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

	if ( Black2Form->Visible)
	  Black2Form->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::UpdateLEDs( void)
{
	UpdateSystemLEDs( SystemComboBox, PT5201Form->BB2PALImage, PT5201Form->BB2NTSCImage);
}
//---------------------------------------------------------------------------
