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
__fastcall TSystemForm::TSystemForm(TComponent* Owner)
	: TForm(Owner)
{
}
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

	if ( PT5201CommPort->Connected())
   {
		sprintf( TxBuffer, "FACT:MAIN:SYST?\r\n");
		if ( TrxRequest( TxBuffer, 20, RxBuffer))
	 	{
			if ( !strcmp( RxBuffer, "PAL"))
				SystemComboBox->ItemIndex = 0;
	   	else
				SystemComboBox->ItemIndex = 1;
		}
		tmpEnable = true;
	}

	ResetBitBtn->Enabled = tmpEnable;
	SystemComboBox->Enabled = tmpEnable;

	CalibTrackBar->Enabled = false;
	CalibTrackBar->Position = CalibTrackBar->Max/2;
	UnlockButton->Caption = "Unlock";
	SaveBitBtn->Enabled = false;

	UnlockButton->Enabled = tmpEnable && !PropertiesForm->LockCalibCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::ResetBitBtnClick(TObject *Sender)
{
	RSTCommand();

   // Delay

	TrxApparatusInfoRequest();
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::SystemComboBoxChange(TObject *Sender)
{
	FactSystemCommand( SystemComboBox->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::UnlockButtonClick(TObject *Sender)
{
	if ( CalibTrackBar->Enabled)
  {
		CalibTrackBar->Enabled = false;
		UnlockButton->Caption = "Unlock";

		SaveBitBtn->Enabled = false;
  }
  else
  {
		CalibTrackBar->Enabled = true;
		UnlockButton->Caption = "Lock";

		SaveBitBtn->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::SaveBitBtnClick(TObject *Sender)
{
	CalibTrackBar->Enabled = false;
	CalibTrackBar->Position = CalibTrackBar->Max/2;

	UnlockButton->Caption = "Unlock";
	SaveBitBtn->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TSystemForm::CalibUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	if ( Button == btNext)
   {
   }
   else
   {
   }
}
//---------------------------------------------------------------------------
