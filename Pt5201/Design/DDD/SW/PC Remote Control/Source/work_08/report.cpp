//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "report.h"
#include "pt52class.h"
#include "util.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TSystemReportForm *SystemReportForm;
//---------------------------------------------------------------------------
__fastcall TSystemReportForm::TSystemReportForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystemReportForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
    SaveWindowPosition( SystemReportForm, "SystemReportWindow");
}
//---------------------------------------------------------------------------
void __fastcall TSystemReportForm::PrintBitBtnClick(TObject *Sender)
{
	PrintDialog->Execute();
}
//---------------------------------------------------------------------------
