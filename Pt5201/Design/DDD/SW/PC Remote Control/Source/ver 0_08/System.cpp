//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "System.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSystemForm *SystemForm;
//---------------------------------------------------------------------------
__fastcall TSystemForm::TSystemForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	// Prepare TV system choice
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::PasswordEditExit(TObject *Sender)
{
	ConfirmPasswordLabel->Visible = true;
	ConfirmPasswordEdit->Visible = true;

	ModalResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::EnablePasswordCheckBoxClick(TObject *Sender)
{
	PasswordEdit->Enabled = EnablePasswordCheckBox->Checked;
}
//---------------------------------------------------------------------------
