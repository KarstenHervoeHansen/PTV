//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
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
	const char* *ptr = GenlockSystemTxt;

	SystemComboBox->Items->Clear();
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormShow(TObject *Sender)
{
	GenlockSetup = new TGenlockGenerator;

	RestoreWindowPosition( GenlockForm, "GenlockWindow");

	PT5201Setup->GetGenlockSettings( GenlockSetup);

	SystemComboBox->ItemIndex = GenlockSetup->System;

	LockForm( PT5201Form->ConfigureLockItem->Checked);
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
	if ( GenlockSetup->System != SystemComboBox->ItemIndex)
  {
		GenlockSetup->System = SystemComboBox->ItemIndex;
	}
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	// btNext, btPrev
	// char *itoa(int value, char *string, int radix);
	// wchar_t *_itow(int value, wchar_t *string, int radix);

	if ( Button == btNext)
  {
  }
  else
  {
  }
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;

	CancelButton->Enabled = Lock;

  if ( GenlockForm->Visible)
  	GenlockForm->Refresh();
}
//---------------------------------------------------------------------------
