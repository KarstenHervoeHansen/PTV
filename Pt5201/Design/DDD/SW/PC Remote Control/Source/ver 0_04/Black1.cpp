//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "Black1.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack1Form *Black1Form;
TBlack1Generator *Black1Setup;
//---------------------------------------------------------------------------
__fastcall TBlack1Form::TBlack1Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormCreate(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormShow(TObject *Sender)
{
 	const char* *ptr;

	RestoreWindowPosition( Black1Form, "Black1Window");

	Black1Setup = new TBlack1Generator;

	PT5201Setup->GetBlack1Settings( Black1Setup);

	// Prepare TV system choice
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->ItemIndex = Black1Setup->System;

	// Prepare to display timing values
	SamplesToFLO( Black1Setup->System, Black1Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PT5201Form->ConfigureLockItem->Checked);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete Black1Setup;

	SaveWindowPosition( Black1Form, "Black1Window");
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModalResult != mrOk)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetBlack1Settings( Black1Setup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::SystemComboBoxChange(TObject *Sender)
{
	const char* *ptr;

	if ( ValidateTiming( SystemComboBox->ItemIndex, StrToInt( FieldEdit->Text),
											 StrToInt( LineEdit->Text), &Black1Setup->Samples, OffsetTrackBar))
  {
		Black1Setup->System = SystemComboBox->ItemIndex;

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
	}
  else
		SystemComboBox->ItemIndex = Black1Setup->System;

	SamplesToFLO( SystemComboBox->ItemIndex, Black1Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FieldUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black1Setup->Samples, FieldEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black1Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  FieldEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black1Setup->Samples, LineEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black1Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LineEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( SystemComboBox->ItemIndex, &Black1Setup->Samples, OffsetEdit, OffsetTrackBar, Button);
	SamplesToFLO( SystemComboBox->ItemIndex, Black1Setup->Samples, FieldEdit, LineEdit, OffsetEdit);

  OffsetEdit->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( SystemComboBox->ItemIndex, &Black1Setup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, Black1Setup->Samples, FieldEdit, LineEdit, OffsetEdit);
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;
	ScHPhaseEdit->Enabled = Lock;

  CancelButton->Enabled = Lock;

	if ( Black1Form->Visible)
	  Black1Form->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::UpdateLEDs( void)
{
	UpdateSystemLEDs( SystemComboBox, PT5201Form->BB1PALImage, PT5201Form->BB1NTSCImage);
}
//---------------------------------------------------------------------------
