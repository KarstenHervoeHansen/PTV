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
#include "comm.h"
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
  char CommPort[5];
	bool enabled = !PT5201CommPort->Connected();

	PortComboBox->Enabled = enabled;
	BaudComboBox->Enabled = enabled;
	FlowComboBox->Enabled = enabled;

 	strcpy( CommPort, PT5201CommPort->GetCommPort());

	PortComboBox->ItemIndex = CommPort[3]- '1';

 	switch ( PT5201CommPort->GetBaudRate())
  {
		case 1200:
      BaudComboBox->ItemIndex = 0;
      break;

		case 2400:
      BaudComboBox->ItemIndex = 0;
      break;

		case 4800:
      BaudComboBox->ItemIndex = 0;
      break;

		default:
      BaudComboBox->ItemIndex = 3;
      break;
	}

	FlowComboBox->ItemIndex = 0;

	PortComboBox->Items->Strings[PortComboBox->ItemIndex].c_str();
	BaudComboBox->Items->Strings[BaudComboBox->ItemIndex].c_str();
	FlowComboBox->Items->Strings[FlowComboBox->ItemIndex].c_str();
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
    SaveWindowPosition( PropertiesForm, "PropertiesWindow");
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::PortComboBoxChange(TObject *Sender)
{
	// VERY PRIMITIV SHOULD BE REWRITTEN CORRECTLY
	char *TxtPtr;

	switch ( PortComboBox->ItemIndex)
  {
		case 0:
			TxtPtr = "COM1";
      break;

		case 1:
			TxtPtr = "COM2";
			break;

		case 2:
			TxtPtr = "COM3";
     	break;

		case 3:
			TxtPtr = "COM4";
			break;
	}

 	PT5201CommPort->SetCommPort( TxtPtr);
	PT5201Form->StatusBar->Panels->Items[1]->Text = TxtPtr;
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::BaudComboBoxChange(TObject *Sender)
{
	// Baudrate data-parity-stop e.g. 115000 8-N-2
	PT5201Form->StatusBar->Panels->Items[2]->Text = BaudComboBox->Items->Strings[BaudComboBox->ItemIndex];

 	PT5201CommPort->SetBaudRate( 9600);
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::FlowComboBoxChange(TObject *Sender)
{
	;
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
