//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "System.h"
#include "mainform.h"
#include "proper.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

TSystemForm *SystemForm;
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TSystemForm::TSystemForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::FormShow(TObject *Sender)
{
	bool tmpEnable = false;
	const char* *ptr;
	char TxBuffer[25], RxBuffer[25];

	// Prepare TV system choice
	SystemComboBox->Items->Clear();

 	ptr = ResetSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);

	SystemComboBox->ItemIndex = 0;

	if ( PT5201CommPort->Connected() || TestModeConnected)
	{
		if ( !TestModeConnected)
    {
			sprintf( TxBuffer, "FACT:MAIN:SYST?\r\n");
			if ( TrxRequest( TxBuffer, 20, RxBuffer))
	 		{
				if ( !strcmp( RxBuffer, "PAL"))
					SystemComboBox->ItemIndex = 0;
		   	else
					SystemComboBox->ItemIndex = 1;
			}
    }
    else
			SystemComboBox->ItemIndex = 0;

		tmpEnable = true;
	}

	ResetBitBtn->Enabled = tmpEnable;
	SystemComboBox->Enabled = tmpEnable;

	UnlockButton->Caption = "Unlock";
	CalibUpDown->Enabled = false;
	MinCalibButton->Enabled = false;
	MaxCalibButton->Enabled = false;
	SaveBitBtn->Enabled = false;

	UnlockButton->Enabled = tmpEnable && !PropertiesForm->LockCalibCheckBox->Checked;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	TrxGenlockCalibEnable( false);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::ResetBitBtnClick(TObject *Sender)
{
	if ( !TestModeConnected)
  {
		RSTCommand();

  	 // Delay

		TrxApparatusInfoRequest();
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::SystemComboBoxChange(TObject *Sender)
{
	FactSystemCommand( SystemComboBox->ItemIndex);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::UnlockButtonClick(TObject *Sender)
{
	if ( UnlockButton->Caption == "Lock")
  {
		UnlockButton->Caption = "Unlock";

		CalibUpDown->Enabled = false;
		MinCalibButton->Enabled = false;
		MaxCalibButton->Enabled = false;
		SaveBitBtn->Enabled = false;
  }
  else
  {
		UnlockButton->Caption = "Lock";

		CalibUpDown->Enabled = true;
		MinCalibButton->Enabled = true;
		MaxCalibButton->Enabled = true;
		SaveBitBtn->Enabled = true;
  }

	TrxGenlockCalibEnable( CalibUpDown->Enabled);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::SaveBitBtnClick(TObject *Sender)
{
	UnlockButton->Caption = "Unlock";

	CalibUpDown->Enabled = false;
	MinCalibButton->Enabled = false;
	MaxCalibButton->Enabled = false;
	SaveBitBtn->Enabled = false;

	TrxGenlockCalibStore();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TSystemForm::CalibUpDownClick(TObject *Sender, TUDBtnType Button)
{
	if ( Button == btNext)
		TrxGenlockCalibAdjust( UP);
  else
		TrxGenlockCalibAdjust( DOWN);
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::MaxCalibButtonClick(TObject *Sender)
{
	TrxGenlockCalibAdjust( MAXValue);
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::MinCalibButtonClick(TObject *Sender)
{
	TrxGenlockCalibAdjust( MINValue);
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::HelpButtonClick(TObject *Sender)
{
	Application->HelpContext( IDH_EDITSYSTEM);
}
//---------------------------------------------------------------------------
