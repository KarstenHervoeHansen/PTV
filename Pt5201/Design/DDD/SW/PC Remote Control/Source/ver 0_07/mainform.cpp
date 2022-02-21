//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "preset.h"
#include "connect.h"
#include "genlock.h"
#include "proper.h"
#include "black1.h"
#include "black2.h"
#include "black3.h"
#include "colorbar.h"
#include "audio.h"
#include "report.h"
#include "apparatu.h"
#include "system.h"
#include "about.h"
#include "util.h"
#include "pt52class.h"
#include "comm.h"
//---------------------------------------------------------------------------
#pragma link "Grids"
#pragma link "GIFImage"
#pragma resource "*.dfm"

TPT5201Form *PT5201Form;

TSyncGenerator *PT5201Setup;
TCommPort	*PT5201CommPort;

TGenlockGenerator *GenlockStatus;
TBlackGenerator *BBStatus[3];
TCBarGenerator *CBarStatus;
TAudioGenerator *AudioStatus;

const SystemStatusPanelHeight = 313;

//---------------------------------------------------------------------------
__fastcall TPT5201Form::TPT5201Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FormCreate(TObject *Sender)
{
	PT5201Setup = new TSyncGenerator;
	PT5201CommPort = new TCommPort;

  RestoreWindowPosition( PT5201Form, "");

	PT5201Form->ClientHeight = PT5201FrontPanel->Height + StatusBar->Height;

	PT5201Form->Refresh();

	DecimalSeparator = '.';			// Neccesary cause some transformations will
  														//  otherwise use the decimal separator from windows
	// Commport
	StatusBar->Panels->Items[CommPortPanel]->Text = "COM1";

	// Baudrate data-parity-stop e.g. 115000 8-N-2
	StatusBar->Panels->Items[CommSettingsPanel]->Text = "11500 8-N-2";

	// Baudrate data-parity-stop e.g. 115000 8-N-2
	StatusBar->Panels->Items[ConnectedPanel]->Text = "Disconnected";

 	PT5201CommPort->SetCommPort( "COM1");
 	PT5201CommPort->SetBaudRate( 9600);
 	PT5201CommPort->SetParity( NOPARITY);
 	PT5201CommPort->SetByteSize( 8);
 	PT5201CommPort->SetStopBits( 2);

	PT5201Setup->Connected = false;
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	switch ( Application->MessageBox( "Save changes to PT5201 PC Remote Control?", "Confirm", MB_YESNOCANCEL|MB_ICONQUESTION))
	{
		case mrYes:
			SaveWindowPosition( PT5201Form, "");

		  if ( GenlockForm->Visible)
      {
				GenlockForm->ModalResult = mrOk;
      	GenlockForm->Close();
      }

      if ( Black1Form->Visible)
      {
	      Black1Form->ModalResult = mrOk;
        Black1Form->Close();
			}

      if ( Black2Form->Visible)
      {
	      Black2Form->ModalResult = mrOk;
        Black2Form->Close();
			}

      if ( Black3Form->Visible)
      {
	      Black3Form->ModalResult = mrOk;
        Black3Form->Close();
      }

      if ( CBarForm->Visible) {
	      CBarForm->ModalResult = mrOk;
	      CBarForm->Close();
			}

      if ( AudioForm->Visible)
      {
	      AudioForm->ModalResult = mrOk;
	      AudioForm->Close();
      }

	   	Action = caFree;
			delete PT5201Setup, PT5201CommPort;
     	break;

		case mrNo:
      if ( GenlockForm->Visible || Black1Form->Visible || Black2Form->Visible ||
           Black3Form->Visible || CBarForm->Visible || AudioForm->Visible)
	    {
				if ( Application->MessageBox( "Close window(s) and loose all changes ?",\
    															    "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)

			  	Action = caNone;
					break;
      }

	   	Action = caFree;
			delete PT5201Setup, PT5201CommPort;
     	break;

  	default:
	  	Action = caNone;
      break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::RestoreFromFileClick(TObject *Sender)
{
	FileOpenDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::OpenPresetClick(TObject *Sender)
{
	PresetForm->Tag = 1;
	PresetForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::toFileAs1Click(TObject *Sender)
{
	FileSaveDialog->Execute();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::SavePresetClick(TObject *Sender)
{
	PresetForm->Tag = 0;
	PresetForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FilePrintItemClick(TObject *Sender)
{
	PrintDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FilePropertiesItemClick(TObject *Sender)
{
	PropertiesForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FileItemExitClick(TObject *Sender)
{
	PT5201Form->Close();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::EditSystemItemClick(TObject *Sender)
{
	SystemForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConnectConnectItemClick(TObject *Sender)
{
	try
  {
		PT5201CommPort->OpenCommPort();
  }
  catch( ECommError &e)
  {
		char *buffer = new char[60];

		strcpy( buffer, "Error opening ");
    strcat( buffer, PT5201CommPort->GetCommPort());
		strcat( buffer, ".");

    if ( e.Error == ECommError::OPEN_ERROR)
      strcat( buffer, " The port may already be in use!");

		Application->MessageBox( buffer, "Connect", MB_OK|MB_ICONINFORMATION);

		delete [] buffer;
		return;
  }

	PT5201CommPort->FlushCommPort();
	PT5201CommPort->WriteString( "*IDN?\r\n");

	char *buffer = new char[80];
	char *tmp, *ProductPtr, *KUNumberPtr, *SWPtr;
	bool errorState = false;

	PT5201CommPort->ReadBytes( buffer, 80);

	tmp = ProductPtr = strchr( buffer, ',');
	errorState = ( tmp == NULL);

	if ( !errorState)
  {
	  *tmp = 0;
		ProductPtr++;

		tmp = KUNumberPtr = strchr( ProductPtr, ',');
		errorState = ( tmp == NULL);
  }

	if ( !errorState)
  {
	  *tmp = 0;
		KUNumberPtr++;

		tmp = SWPtr = strchr( KUNumberPtr, ',');
		errorState = ( tmp == NULL);
  }

	if ( !errorState)
  {
	 	*tmp = 0;
		SWPtr++;

		tmp = strchr( SWPtr, '\n');
		errorState = ( tmp == NULL);
	}

	if ( !errorState)
	{
	 	*tmp = 0;

	  ProductEdit->Text = ProductPtr;
  	KUNumberEdit->Text = KUNumberPtr;
	  SWRevisionEdit->Text = SWPtr;

		delete [] buffer;
  }
  else
  {
		strcpy( buffer, "No response from the unit");

		Application->MessageBox( buffer, "Connect", MB_OK|MB_ICONINFORMATION);

		PT5201CommPort->CloseCommPort();

		delete [] buffer;
		return;
  }

	PT5201Setup->Connected = true;

  StatusBar->Panels->Items[0]->Text = "Connected";

	SystemStatusUpdate();

	PT5201LEDsUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConnectDisconnectItemClick(TObject *Sender)
{
	try
  {
		PT5201CommPort->CloseCommPort();
  }
  catch( ECommError &e)
  {
		char *buffer = new char[30];

		strcpy( buffer, "Unable to close ");
    strcat( buffer, PT5201CommPort->GetCommPort());
    strcat( buffer, ".");

		Application->MessageBox( buffer, "Connect", MB_OK|MB_ICONINFORMATION);

		delete [] buffer;
		return;
  }

	PT5201Setup->Connected = false;

  StatusBar->Panels->Items[0]->Text = "Disconnected";
//  StatusBar->Panels->Items[0]->Color = clBtnFace;

	if ( ViewSystemStatusItem->Checked)
		SystemStatusUpdate();

	PT5201LEDsUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureGenlockItemClick(TObject *Sender)
{
	GenlockForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::BlackBurst1ItemClick(TObject *Sender)
{
	Black1Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::BlackBurst2ItemClick(TObject *Sender)
{
	Black2Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::BlackBurst3ItemClick(TObject *Sender)
{
	Black3Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureColorBarItemClick(TObject *Sender)
{
	CBarForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureAudioItemClick(TObject *Sender)
{
	AudioForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ViewSystemReportItemClick(TObject *Sender)
{
	SystemReportForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ViewSystemStatusItemClick(TObject *Sender)
{
	if ( ViewSystemStatusItem->Checked)
  {
		ViewSystemStatusItem->Checked = false;
		SystemStatusPanel->Visible = false;

    SystemStatusPanel->Height = SystemStatusPanelHeight;

    PT5201Form->ClientHeight -= SystemStatusPanelHeight;
  }
  else
  {
		ViewSystemStatusItem->Checked = true;
		SystemStatusPanel->Visible = true;

    SystemStatusPanel->Height = SystemStatusPanelHeight;
    SystemStatusPanel->Align = alTop;

    PT5201Form->ClientHeight += SystemStatusPanelHeight;
  }
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::ViewStatusBarItemClick(TObject *Sender)
{
	if ( ViewStatusBarItem->Checked)
  {
		ViewStatusBarItem->Checked = false;
		StatusBar->Visible = false;

		PT5201Form->ClientHeight -= StatusBar->Height;
  }
  else
  {
		ViewStatusBarItem->Checked = true;
		StatusBar->Visible = true;

		PT5201Form->ClientHeight += StatusBar->Height;
  }
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::Contents1Click(TObject *Sender)
{
	Application->HelpCommand( HELP_FINDER, 0);
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::Frequentlyaskedquestions1Click(TObject *Sender)
{
	ShellExecute( NULL, "open", "http://www.ptv.dk", NULL, NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::OnlineSupport1Click(TObject *Sender)
{
	ShellExecute( NULL, "open", "http://www.ptv.dk", NULL, NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::PTVHomepage1Click(TObject *Sender)
{
	ShellExecute( NULL, "open", "http://www.ptv.dk", NULL, NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::HelpAboutItemClick(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::SystemStatusUpdate( void)
{
	const char* *ptr;
  TColor activeColor = clBtnFace;

/*	if ( ViewSystemStatusItem->Checked)
		SystemStatusUpdate();

	PT5201LEDsUpdate();*/
	if ( PT5201Setup->Connected)
  {
		GenlockStatus = PT5201Setup->GetGenlockSettings();

  	for ( int i=0; i<3; i++)
			BBStatus[i] = PT5201Setup->GetBlackSettings( i);

		CBarStatus = PT5201Setup->GetCBarSettings();
		AudioStatus = PT5201Setup->GetAudioSettings();

		GenlockSystemEdit->Text = GenlockSystemTxt[GenlockStatus->System];
		SamplesToFLO( GenlockStatus->System, GenlockStatus->Samples, \
    											GenlockFieldEdit, GenlockLineEdit, GenlockOffsetEdit);

		BB1SystemEdit->Text = TVSystemTxt[BBStatus[Black1Output]->System];
		SamplesToFLO( BBStatus[Black1Output]->System, BBStatus[Black1Output]->Samples, \
    											BB1FieldEdit, BB1LineEdit, BB1OffsetEdit);
		BB1ScHEdit->Text = IntToStr( BBStatus[Black1Output]->ScHPhase);

		BB2SystemEdit->Text = TVSystemTxt[BBStatus[Black2Output]->System];
		SamplesToFLO( BBStatus[Black2Output]->System, BBStatus[Black2Output]->Samples, \
    											BB2FieldEdit, BB2LineEdit, BB2OffsetEdit);
		BB2ScHEdit->Text = IntToStr( BBStatus[Black2Output]->ScHPhase);

		BB3SystemEdit->Text = TVSystemTxt[BBStatus[Black3Output]->System];
		SamplesToFLO( BBStatus[Black3Output]->System, BBStatus[Black3Output]->Samples, \
    											BB3FieldEdit, BB3LineEdit, BB3OffsetEdit);
		BB3ScHEdit->Text = IntToStr( BBStatus[Black3Output]->ScHPhase);

		CBarSystemEdit->Text = TVSystemTxt[CBarStatus->System];
		SamplesToFLO( CBarStatus->System, CBarStatus->Samples, \
    											CBarFieldEdit, CBarLineEdit, CBarOffsetEdit);
		CBarScHEdit->Text = IntToStr( CBarStatus->ScHPhase);
//	CBarPatternEdit->Color = activeColor;
		CBarEmbAudioEdit->Text = EmbAudioToneTxt[CBarStatus->EmbAudio];


		AudioOutputEdit->Text = AudioOutputTxt[AudioStatus->Output];
		AudioSystemEdit->Text = TVSystemTxt[AudioStatus->System];
		AudioTimingEdit->Color = activeColor;

    if ( AudioStatus->Output == AnalogAudio)
			ptr = AnalogLevelTxt;
    else
			ptr = AESEBULevelTxt;

		AudioToneEdit->Text = AudioToneTxt[AudioStatus->Tone[AudioStatus->Output]];
		AudioLevelEdit->Text = ptr[AudioStatus->Level[AudioStatus->Output]];
		AudioWordEdit->Text = AudioWordTxt[AudioStatus->WordClock];
		AudioClickEdit->Text = AudioClickTxt[AudioStatus->ClickPeriod[AudioStatus->Output]];
		AudioPLLEdit->Text = "Locked";
		AudioVideoEdit->Text = "Locked";

		activeColor =clLime;
  }
  else
  {
		GenlockSystemEdit->Text = "";
		GenlockFieldEdit->Text = "";
		GenlockLineEdit->Text = "";
		GenlockOffsetEdit->Text = "";

		BB1SystemEdit->Text = "";
		BB1FieldEdit->Text = "";
		BB1LineEdit->Text = "";
		BB1OffsetEdit->Text = "";
		BB1ScHEdit->Text = "";

		BB2SystemEdit->Text = "";
		BB2FieldEdit->Text = "";
		BB2LineEdit->Text = "";
		BB2OffsetEdit->Text = "";
		BB2ScHEdit->Text = "";

		BB3SystemEdit->Text = "";
		BB3FieldEdit->Text = "";
		BB3LineEdit->Text = "";
		BB3OffsetEdit->Text = "";
		BB3ScHEdit->Text = "";

		CBarSystemEdit->Text = "";
		CBarFieldEdit->Text = "";
		CBarLineEdit->Text = "";
		CBarOffsetEdit->Text = "";
		CBarScHEdit->Text = "";
		CBarPatternEdit->Text = "";
		CBarEmbAudioEdit->Text = "";

		AudioOutputEdit->Text = "";
		AudioSystemEdit->Text = "";
		AudioTimingEdit->Text = "";
		AudioToneEdit->Text = "";
		AudioLevelEdit->Text = "";
		AudioWordEdit->Text = "";
		AudioClickEdit->Text = "";
		AudioPLLEdit->Text = "";
		AudioVideoEdit->Text = "";

	  ProductEdit->Text = "";
  	KUNumberEdit->Text = "";
  	SWRevisionEdit->Text = "";

		activeColor = clBtnFace;
  }

	GenlockSystemEdit->Color = activeColor;
	GenlockFieldEdit->Color = activeColor;
	GenlockLineEdit->Color = activeColor;
	GenlockOffsetEdit->Color = activeColor;

	BB1SystemEdit->Color = activeColor;
	BB1FieldEdit->Color = activeColor;
	BB1LineEdit->Color = activeColor;
	BB1OffsetEdit->Color = activeColor;
	BB1ScHEdit->Color = activeColor;

	BB2SystemEdit->Color = activeColor;
	BB2FieldEdit->Color = activeColor;
	BB2LineEdit->Color = activeColor;
	BB2OffsetEdit->Color = activeColor;
	BB2ScHEdit->Color = activeColor;

	BB3SystemEdit->Color = activeColor;
	BB3FieldEdit->Color = activeColor;
	BB3LineEdit->Color = activeColor;
	BB3OffsetEdit->Color = activeColor;
	BB3ScHEdit->Color = activeColor;

	CBarSystemEdit->Color = activeColor;
	CBarFieldEdit->Color = activeColor;
	CBarLineEdit->Color = activeColor;
	CBarOffsetEdit->Color = activeColor;
	CBarScHEdit->Color = activeColor;
	CBarPatternEdit->Color = activeColor;
	CBarEmbAudioEdit->Color = activeColor;

	AudioOutputEdit->Color = activeColor;
	AudioSystemEdit->Color = activeColor;
	AudioTimingEdit->Color = activeColor;
	AudioToneEdit->Color = activeColor;
	AudioLevelEdit->Color = activeColor;
	AudioWordEdit->Color = activeColor;
	AudioClickEdit->Color = activeColor;
	AudioPLLEdit->Color = activeColor;
	AudioVideoEdit->Color = activeColor;

  ProductEdit->Color = activeColor;
  KUNumberEdit->Color = activeColor;
  SWRevisionEdit->Color = activeColor;
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::PT5201LEDsUpdate( void)
{
	UpdateLEDs( false, BB1PALImage);
	UpdateLEDs( false, BB1NTSCImage);

	UpdateLEDs( false, BB2PALImage);
	UpdateLEDs( false, BB2NTSCImage);

	UpdateLEDs( false, BB3PALImage);
	UpdateLEDs( false, BB3NTSCImage);

	UpdateLEDs( false, CBarPALImage);
	UpdateLEDs( false, CBarNTSCImage);

	UpdateLEDs( false, AudioAESEBUImage);
	UpdateLEDs( false, AudioAnalogImage);

	UpdateLEDs( false, Word441Image);
	UpdateLEDs( false, Word48Image);

	UpdateLEDs( false, PowerImage);
	UpdateLEDs( false, GenlockImage);

	if ( PT5201Setup->Connected)
  {
	 	for ( int i=0; i<3; i++)
			BBStatus[i] = PT5201Setup->GetBlackSettings( i);

		CBarStatus = PT5201Setup->GetCBarSettings();
		AudioStatus = PT5201Setup->GetAudioSettings();

		if ( BBStatus[Black1Output]->System == PAL)
  	{
			UpdateLEDs( true, BB1PALImage);
			UpdateLEDs( false, BB1NTSCImage);
	  }
  	else
	  {
			UpdateLEDs( false, BB1PALImage);
			UpdateLEDs( true, BB1NTSCImage);
	  }

		if ( BBStatus[Black2Output]->System == PAL)
  	{
			UpdateLEDs( true, BB2PALImage);
			UpdateLEDs( false, BB2NTSCImage);
  	}
	  else
  	{
			UpdateLEDs( false, BB2PALImage);
			UpdateLEDs( true, BB2NTSCImage);
  	}

		if ( BBStatus[Black3Output]->System == PAL)
	  {
			UpdateLEDs( true, BB3PALImage);
			UpdateLEDs( false, BB3NTSCImage);
	  }
  	else
	  {
			UpdateLEDs( false, BB3PALImage);
			UpdateLEDs( true, BB3NTSCImage);
	  }

		if ( CBarStatus->System == PAL)
  	{
			UpdateLEDs( true, CBarPALImage);
			UpdateLEDs( false, CBarNTSCImage);
  	}
	  else
  	{
			UpdateLEDs( false, CBarPALImage);
			UpdateLEDs( true, CBarNTSCImage);
  	}

		if ( AudioStatus->Output == AnalogAudio)
  	{
			UpdateLEDs( false, AudioAESEBUImage);
			UpdateLEDs( true, AudioAnalogImage);
  	}
	  else
  	{
			UpdateLEDs( true, AudioAESEBUImage);
			UpdateLEDs( false, AudioAnalogImage);
	  }

		if ( AudioStatus->WordClock == f441kHz)
  	{
			UpdateLEDs( true, Word441Image);
			UpdateLEDs( false, Word48Image);
		}
    else
    {
			UpdateLEDs( false, Word441Image);
			UpdateLEDs( true, Word48Image);
    }
  }
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::UpdateLEDs( bool LEDOn, TImage *LED)
{
	if ( LEDOn)
	 	LED->Picture = LEDOnImage->Picture;
  else
	 	LED->Picture = LEDOffImage->Picture;
}
//---------------------------------------------------------------------------
//
//
