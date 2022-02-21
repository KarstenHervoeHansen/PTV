//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "Black.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TBlack1Form *Black1Form;
TBlackGenerator *BlackSetup;
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
	BlackSetup = new TBlackGenerator;

	RestoreWindowPosition( BlackForm, "BlackWindow");

	PT5201Setup->GetBlackSettings( 0, BlackSetup);

	SystemComboBox->ItemIndex = BlackSetup->System[0];
  //	FieldEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	LineEdit = CBarSetup->Timing = PT5201Setup->Timing;
  //	SamplesEdit = CBarSetup->Timing = PT5201Setup->Timing;
	//  ScHPhaseEdit->Text = CBarSetup->ScHPhase;

	CancelButton->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete BlackSetup;

	SaveWindowPosition( BlackForm, "BlackWindow");
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( !OKButton->Focused() && CancelButton->Enabled)
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_YESNO|MB_ICONWARNING) != mrYes)
			CanClose = false;
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::OKButtonClick(TObject *Sender)
{
	PT5201Setup->SetBlackSettings( 0, BlackSetup);

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::ApplyButtonClick(TObject *Sender)
{
	PT5201Setup->SetBlackSettings( 0, BlackSetup);

	CancelButton->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::CancelButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TBlack1Form::SystemComboBoxChange(TObject *Sender)
{
	if ( BlackSetup->System[0] != SystemComboBox->ItemIndex)
  {
		BlackSetup->System[0] = SystemComboBox->ItemIndex;

		CancelButton->Enabled = true;
	}
}
//---------------------------------------------------------------------------
