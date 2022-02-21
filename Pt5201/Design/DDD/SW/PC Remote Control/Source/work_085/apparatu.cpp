//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "apparatu.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TApparatusStatusForm *ApparatusStatusForm;
//---------------------------------------------------------------------------
__fastcall TApparatusStatusForm::TApparatusStatusForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TApparatusStatusForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
    SaveWindowPosition( ApparatusStatusForm, "ApparatusStatusWindow");
}
//---------------------------------------------------------------------------
void __fastcall TApparatusStatusForm::FormShow(TObject *Sender)
{
	RestoreWindowPosition( ApparatusStatusForm, "ApparatusStatusWindow");

  Memo1->Lines->Clear();
 	Memo1->Lines->Add( "KU number: 12231212");
  Memo1->Lines->Add("");
}
//---------------------------------------------------------------------------
void __fastcall TApparatusStatusForm::PrintButtonClick(TObject *Sender)
{
	PrintDialog->Execute();
}
//---------------------------------------------------------------------------
