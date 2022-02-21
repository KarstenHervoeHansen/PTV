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
TBlackGenerator *Black1Setup;
//---------------------------------------------------------------------------
__fastcall TBlack1Form::TBlack1Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormCreate(TObject *Sender)
{
  const char* *ptr = TVSystemTxt;

	SystemComboBox->Items->Clear();
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormShow(TObject *Sender)
{
	Black1Setup = new TBlackGenerator;

	RestoreWindowPosition( Black1Form, "Black1Window");

	PT5201Setup->GetBlackSettings( 0, Black1Setup);

	SystemComboBox->ItemIndex = Black1Setup->System[0];
  //	FieldEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	LineEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	SamplesEdit = CBarSetup->Timing = PT5201Setup->Timing;
	//  ScHPhaseEdit->Text = CBarSetup->ScHPhase;

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
	PT5201Setup->SetBlackSettings( 0, Black1Setup);

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
  UpdateLEDs();
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
