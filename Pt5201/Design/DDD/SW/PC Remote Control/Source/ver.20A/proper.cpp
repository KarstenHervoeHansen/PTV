//--------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "Genlock.h"
#include "Black1.h"
#include "Black2.h"
#include "Black3.h"
#include "tsg.h"
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
	bool enabled = !PT5201CommPort->Connected();

	PortComboBox->Enabled = enabled;

	// Baudrate has been disabled as the command to change the baudrate has not
   //  been implemented in the generator itself.
	BaudComboBox->Enabled = false;

	// Flowcontrol has been disabled as the PLD in the PT5201 does NOT route
  //  the handshake signals to the 9-pin subD port.
	FlowComboBox->Enabled = false;

	PortComboBox->ItemIndex = ( PT5201CommPort->GetCommPort())[3]- '1';
	PortComboBox->Items->Strings[PortComboBox->ItemIndex].c_str();

	switch ( PT5201CommPort->GetBaudRate())
  	{
		case 1200:
      	BaudComboBox->ItemIndex = 0;
      	break;

		case 2400:
      	BaudComboBox->ItemIndex = 1;
      	break;

		case 4800:
      	BaudComboBox->ItemIndex = 2;
      	break;

		default:
      	BaudComboBox->ItemIndex = 3;
      	break;
	}

	BaudComboBox->Items->Strings[BaudComboBox->ItemIndex].c_str();

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
void __fastcall TPropertiesForm::PortComboBoxChange(TObject *Sender)
{
 	PT5201CommPort->SetCommPort( CommPortTxt[PortComboBox->ItemIndex]);

	PT5201Form->StatusBar->Panels->Items[1]->Text = CommPortTxt[PortComboBox->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::BaudComboBoxChange(TObject *Sender)
{
	// Baudrate data-parity-stop e.g. 115000 8-N-2
	PT5201Form->StatusBar->Panels->Items[2]->Text = BaudComboBox->Items->Strings[BaudComboBox->ItemIndex];

 	PT5201CommPort->SetBaudRate( 9600);
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::ConfigureLockItemClick(TObject *Sender)
{
	GenlockForm->LockForm( ConfigureLockItem->Checked);
	Black1Form->LockForm( ConfigureLockItem->Checked);
	Black2Form->LockForm( ConfigureLockItem->Checked);
	Black3Form->LockForm( ConfigureLockItem->Checked);
	TSGForm->LockForm( ConfigureLockItem->Checked);
	AudioForm->LockForm( ConfigureLockItem->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::HelpButtonClick(TObject *Sender)
{
	Application->HelpContext( IDH_FILESPROPERTIES);
}
//---------------------------------------------------------------------------
