//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "black3.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack3Form *Black3Form;
TBlack3Generator *Black3Setup;
//---------------------------------------------------------------------------
__fastcall TBlack3Form::TBlack3Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormCreate(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( Black3Form, "Black3Window");

	Black3Setup = new TBlack3Generator;

	PT5201Setup->GetBlack3Settings( Black3Setup);

	// Prepare TV system choice
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->ItemIndex = Black3Setup->System;

	// Prepare to display timing values
	SamplesToFLO( Black3Setup->System, Black3Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PT5201Form->ConfigureLockItem->Checked);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete Black3Setup;

	SaveWindowPosition( Black3Form, "Black3Window");
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModalResult != mrOk)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetBlack3Settings( Black3Setup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::SystemComboBoxChange(TObject *Sender)
{
	if ( ValidateTiming( SystemComboBox->ItemIndex, StrToInt( FieldEdit->Text),
											 StrToInt( LineEdit->Text), &Black3Setup->Samples, OffsetTrackBar))
  {
		Black3Setup->System = SystemComboBox->ItemIndex;

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
		SystemComboBox->ItemIndex = Black3Setup->System;

	SamplesToFLO( SystemComboBox->ItemIndex, Black3Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FieldUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black3Setup->Samples, FieldEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black3Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black3Setup->Samples, LineEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black3Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black3Setup->Samples, OffsetEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black3Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &Black3Setup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, Black3Setup->Samples, FieldEdit, LineEdit, OffsetEdit);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;
	ScHPhaseEdit->Enabled = Lock;

  CancelButton->Enabled = Lock;

	if ( Black3Form->Visible)
	  Black3Form->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::UpdateLEDs( void)
{
	UpdateSystemLEDs( SystemComboBox, PT5201Form->BB3PALImage, PT5201Form->BB3NTSCImage);
}
//---------------------------------------------------------------------------
