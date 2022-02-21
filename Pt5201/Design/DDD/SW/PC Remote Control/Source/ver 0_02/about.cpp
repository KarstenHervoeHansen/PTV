//---------------------------------------------------------------------
#include <vcl.h>
#include <shellapi.h>
#pragma hdrstop

#include "about.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TAboutBox::WebLabelClick(TObject *Sender)
{
	ShellExecute( NULL, "open", "http://www.ptv.dk", NULL, NULL, SW_RESTORE);

	WebLabel->Font->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::emailLabelClick(TObject *Sender)
{
	ShellExecute( NULL, "open", "mailto:helpdesk@ptv.dk", NULL, NULL, SW_RESTORE);

	emailLabel->Font->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	Close();	
}
//---------------------------------------------------------------------------
