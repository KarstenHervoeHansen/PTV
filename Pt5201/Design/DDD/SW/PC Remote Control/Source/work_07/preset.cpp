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
	bool readonly;

	if ( PresetForm->Tag == 0)
	{
		Caption = "Save as";
		readonly = false;
	}
	else
  	{
		Caption = "Open";
		readonly = true;
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
	char TxBuffer[25], RxBuffer[25];

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

	sprintf( TxBuffer, "DATE? %d\r\n", PresetNo);
	if ( TrxRequest( TxBuffer, 20, RxBuffer))
  	{
		sscanf( RxBuffer, "%d,%d,%d", &year, &month, &day);
		sprintf( RxBuffer, "%02d-%02d-%02d", day, month, year);

  		DateMaskEdit->Text = RxBuffer;
  	}
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

		  	strcpy( buffer, "SYST:PRES:NAME ");
			buffer[15] = PresetNo;
			buffer[16] = 0;
		  	strcat( buffer, ",\"");
	  		strcat( buffer, NameEdit->Text.c_str());
		  	strcat( buffer, "\";");

			PT5201CommPort->WriteString( buffer);

		  	strcpy( buffer, "AUTH ");
			buffer[5] = PresetNo;
			buffer[6] = 0;
		  	strcat( buffer, ",\"");
	  		strcat( buffer, AuthorEdit->Text.c_str());
		  	strcat( buffer, "\"\r\n");

			PT5201CommPort->WriteString( buffer);

/*		  	strcpy( buffer, "SYST:PRES:DATE ");
			buffer[15] = PresetNo;
			buffer[16] = 0;
		  	strcat( buffer, ",\"");
	  		strcat( buffer, AuthorEdit->Text.c_str());
		  	strcat( buffer, "\";");

			PT5201CommPort->WriteString( buffer);
*/
		  	strcpy( buffer, "STOR ");
			buffer[5] = PresetNo;
			buffer[6] = 0;
		  	strcat( buffer, "\r\n");

//			PT5201CommPort->WriteString( buffer);

			delete [] buffer;
    	}
  	}
	else
	{
		if ( PT5201CommPort->Connected())
		{
			char *buffer = new char[50];
			TTSGenerator TSGSetup;

			strcpy( buffer, "SYST:PRES n\r\n");
			buffer[10] = '1' + PresetComboBox->ItemIndex;

			PT5201CommPort->WriteString( buffer);

			TrxTSGInfoRequest( &TSGSetup);

			delete [] buffer;
		}
	}
}
//---------------------------------------------------------------------------
