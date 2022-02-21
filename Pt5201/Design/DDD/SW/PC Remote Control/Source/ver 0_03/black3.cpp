//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "black3.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack3Form *Black3Form;
TBlackGenerator *Black3Setup;
//---------------------------------------------------------------------------
__fastcall TBlack3Form::TBlack3Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormCreate(TObject *Sender)
{
  const char* *ptr = TVSystemTxt;

	SystemComboBox->Items->Clear();
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TBlack3Form::FormShow(TObject *Sender)
{
	Black3Setup = new TBlackGenerator;

	RestoreWindowPosition( Black3Form, "Black3Window");

	PT5201Setup->GetBlackSettings( 2, Black3Setup);

	SystemComboBox->ItemIndex = Black3Setup->System[0];
  //	FieldEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	LineEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	SamplesEdit = CBarSetup->Timing = PT5201Setup->Timing;
	//  ScHPhaseEdit->Text = CBarSetup->ScHPhase;

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
	PT5201Setup->SetBlackSettings( 2, Black3Setup);

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
	UpdateLEDs();
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
