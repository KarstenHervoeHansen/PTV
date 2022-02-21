//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "Genlock.h"
#include "Black1.h"
#include "Black2.h"
#include "Black3.h"
#include "Colorbar.h"
#include "Audio.h"
#include "util.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TPropertiesForm *PropertiesForm;
//---------------------------------------------------------------------------
__fastcall TPropertiesForm::TPropertiesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::FormShow(TObject *Sender)
{
	PortComboBox->ItemIndex = 0;
	PortComboBox->Items->Strings[PortComboBox->ItemIndex].c_str();

	BaudComboBox->ItemIndex = 1;
	BaudComboBox->Items->Strings[BaudComboBox->ItemIndex].c_str();

	DataComboBox->ItemIndex = 0;
	DataComboBox->Items->Strings[DataComboBox->ItemIndex].c_str();

	FlowComboBox->ItemIndex = 0;
	FlowComboBox->Items->Strings[FlowComboBox->ItemIndex].c_str();
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
    SaveWindowPosition( PropertiesForm, "PropertiesWindow");
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::BaudComboBoxChange(TObject *Sender)
{
	// Baudrate data-parity-stop e.g. 115000 8-N-2
	PT5201Form->StatusBar->Panels->Items[1]->Text = BaudComboBox->Items->Strings[BaudComboBox->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::ConfigureLockItemClick(TObject *Sender)
{
	GenlockForm->LockForm( ConfigureLockItem->Checked);
	Black1Form->LockForm( ConfigureLockItem->Checked);
	Black2Form->LockForm( ConfigureLockItem->Checked);
	Black3Form->LockForm( ConfigureLockItem->Checked);
	CBarForm->LockForm( ConfigureLockItem->Checked);
	AudioForm->LockForm( ConfigureLockItem->Checked);
}
//---------------------------------------------------------------------------
