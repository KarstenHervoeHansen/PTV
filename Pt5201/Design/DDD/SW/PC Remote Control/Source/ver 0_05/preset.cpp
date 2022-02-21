//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "mainform.h"
#include "preset.h"
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
	PresetComboBox->ItemIndex = 0;
	PresetListBox->ItemIndex = 0;

	if ( PresetForm->Tag == 0)
  {
		Caption = "Save preset As";
		Label1->Visible = true;
		PresetComboBox->Visible = true;
		PresetEdit->ReadOnly = false;
  }
  else
  {
		Caption = "Open preset";
		Label1->Visible = false;
		PresetComboBox->Visible = false;
		PresetEdit->ReadOnly = true;
  }

	PresetEdit->Text = PresetListBox->Items->Strings[PresetListBox->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::PresetComboBoxChange(TObject *Sender)
{
	PresetListBox->ItemIndex = PresetComboBox->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::PresetListBoxDblClick(TObject *Sender)
{
	PresetComboBox->ItemIndex = PresetListBox->ItemIndex;

	PresetEdit->Text = PresetListBox->Items->Strings[PresetListBox->ItemIndex];
}
//---------------------------------------------------------------------------
