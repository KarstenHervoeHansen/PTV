//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "preset.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

TPresetForm *PresetForm;
//---------------------------------------------------------------------------
__fastcall TPresetForm::TPresetForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::FormShow(TObject *Sender)
{
	if ( PresetForm->Tag == 0)
		Caption = "Save as";
  else
		Caption = "Open";

	PresetComboBox->ItemIndex = 0;

	GetPresetInformation( PresetComboBox->ItemIndex + 1);
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::PresetComboBoxChange(TObject *Sender)
{
	GetPresetInformation( PresetComboBox->ItemIndex + 1);
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::GetPresetInformation( char PresetNumber)
{
	char TxBuffer[25], RxBuffer[25];

	if ( PT5201CommPort->Connected() && !TestMode)
  {
		sprintf( TxBuffer, "SYST:PRES:NAME? %d;", PresetNumber);

		if ( SendRequest( TxBuffer, 20, RxBuffer))
    {
  		NameEdit->Text = RxBuffer;
    }

		sprintf( TxBuffer, "AUTH? %d\r\n", PresetNumber);
		if ( SendRequest( TxBuffer, 20, RxBuffer))
    {
  		AuthorEdit->Text = RxBuffer;
    }
  }
}
//---------------------------------------------------------------------------
