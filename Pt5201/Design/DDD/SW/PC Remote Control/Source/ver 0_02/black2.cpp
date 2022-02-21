//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "black2.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack2Form *Black2Form;
TBlackGenerator *Black2Setup;
//---------------------------------------------------------------------------
__fastcall TBlack2Form::TBlack2Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FormCreate(TObject *Sender)
{
  const char* *ptr = TVSystemTxt;

	SystemComboBox->Items->Clear();
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::FormShow(TObject *Sender)
{
	Black2Setup = new TBlackGenerator;

	RestoreWindowPosition( Black2Form, "Black2Window");

	PT5201Setup->GetBlackSettings( 1, Black2Setup);

	SystemComboBox->ItemIndex = Black2Setup->System[0];
  //	FieldEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	LineEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	SamplesEdit = CBarSetup->Timing = PT5201Setup->Timing;
	//  ScHPhaseEdit->Text = CBarSetup->ScHPhase;

	LockForm( PT5201Form->ConfigureLockItem->Checked);

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
	PT5201Setup->SetBlackSettings( 1, Black2Setup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;
	ScHPhaseEdit->Enabled = Lock;

  CancelButton->Enabled = Lock;

	if ( Black2Form->Visible)
	  Black2Form->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::SystemComboBoxChange(TObject *Sender)
{
	UpdateLEDs();
}
//---------------------------------------------------------------------------
void __fastcall TBlack2Form::UpdateLEDs( void)
{
	UpdateSystemLEDs( SystemComboBox, PT5201Form->BB2PALImage, PT5201Form->BB2NTSCImage);
}
//---------------------------------------------------------------------------
