//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "info.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TInformationForm *InformationForm;
//---------------------------------------------------------------------------
__fastcall TInformationForm::TInformationForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInformationForm::PrintClick(TObject *Sender)
{
	PrintDialog1->Execute();
}
//---------------------------------------------------------------------------