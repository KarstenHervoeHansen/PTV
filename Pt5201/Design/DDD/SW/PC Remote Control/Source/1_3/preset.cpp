//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <dos.h>
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
	bool readonly;

	if ( PresetForm->Tag == 0)
   {
		readonly = false;
		Caption = "Save as";
	}
	else
   {
		readonly = true;
		Caption = "Open";
   }

	NameEdit->ReadOnly = readonly;
	AuthorEdit->ReadOnly = readonly;
	DateMaskEdit->ReadOnly = readonly;

	PresetComboBox->ItemIndex = 0;

	GetPresetInformation( PresetComboBox->ItemIndex + 1);
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::PresetComboBoxChange(TObject *Sender)
{
	GetPresetInformation( PresetComboBox->ItemIndex + 1);
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::GetPresetInformation( char PresetNo)
{
	int day, month, year;
  char *TxBuffer = new char[30];
  char *RxBuffer = new char[30];

	sprintf( TxBuffer, "SYST:PRES:NAME? %d;", PresetNo);
	if ( TrxRequest( TxBuffer, 20, RxBuffer))
 		NameEdit->Text = RxBuffer;
	else
 		NameEdit->Text = "------";

	sprintf( TxBuffer, "AUTH? %d;", PresetNo);
	if ( TrxRequest( TxBuffer, 20, RxBuffer))
 		AuthorEdit->Text = RxBuffer;
	else
 		AuthorEdit->Text = "------";

	sprintf( TxBuffer, "DATE? %d\n", PresetNo);
	if ( TrxRequest( TxBuffer, 20, RxBuffer))
  {
		sscanf( RxBuffer, "%d,%d,%d", &year, &month, &day);
		sprintf( RxBuffer, "%02d-%02d-%02d", year, month, day);

  	DateMaskEdit->Text = RxBuffer;
  }

	delete [] TxBuffer;
	delete [] RxBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::OKButtonClick(TObject *Sender)
{
	char PresetNo;

	if ( PresetForm->Tag == 0)
	{
		if ( PT5201CommPort->Connected())
		{
			char *buffer = new char[50];

			PresetNo = '1' + PresetComboBox->ItemIndex;

	  	strcpy( buffer, "SYST:PRES:STOR X;");
			buffer[15] = PresetNo;

			PT5201CommPort->WriteString( buffer);

			sleep( 2);	// This delay is necessaray in order for the instrument to
         					//  store ALL the variables,

		  strcpy( buffer, "NAME X,'");
			buffer[5] = PresetNo;
	  	strcat( buffer, NameEdit->Text.c_str());
		  strcat( buffer, "';");

			PT5201CommPort->WriteString( buffer);

		  strcpy( buffer, "AUTH X,'");
			buffer[5] = PresetNo;
	  	strcat( buffer, AuthorEdit->Text.c_str());
		  strcat( buffer, "';");

			PT5201CommPort->WriteString( buffer);

		  strcpy( buffer, "DATE X,");
			buffer[5] = PresetNo;
  		strcat( buffer, DateMaskEdit->Text.c_str());
			buffer[9] = ',';
			buffer[12] = ',';
		  strcat( buffer, "\n");

			PT5201CommPort->WriteString( buffer);

			delete [] buffer;
    	}
  	}
	else
	{
		if ( PT5201CommPort->Connected())
		{
			char *buffer = new char[50];

			strcpy( buffer, "SYST:PRES n\n");
			buffer[10] = '1' + PresetComboBox->ItemIndex;

			PT5201CommPort->WriteString( buffer);

			TrxApparatusInfoRequest();

			delete [] buffer;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TPresetForm::HelpButtonClick(TObject *Sender)
{
	if ( PresetForm->Tag == 0)
		Application->HelpContext( IDH_FILESSAVEPRESET);
  else
		Application->HelpContext( IDH_FILESOPENPRESET);
}
//---------------------------------------------------------------------------
