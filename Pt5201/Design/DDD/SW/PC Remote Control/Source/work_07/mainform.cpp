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
#include "tsg.h"
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
#pragma resource "*.dfm"

TPT5201Form *PT5201Form;

TSyncGenerator *PT5201Setup;
TCommPort	*PT5201CommPort;

TGenlockGenerator *GenlockStatus;
TBlackGenerator *BBStatus[3];
TTSGenerator *TSGStatus;
TAudioGenerator *AudioStatus;

bool TestMode;

const SystemStatusPanelHeight = 313;

//---------------------------------------------------------------------------
//
//
__fastcall TPT5201Form::TPT5201Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::FormCreate(TObject *Sender)
{
	PT5201Setup = new TSyncGenerator;
	PT5201CommPort = new TCommPort;

  	TestMode = false;

  	RestoreWindowPosition( PT5201Form, "");

	PT5201Form->ClientHeight = PT5201FrontPanel->Height + StatusBar->Height;

	PT5201Form->Refresh();

	if ( PT5201Setup->ReadFromFile( "PT5201.INI") == -1)
  	{
		Application->MessageBox( "The INI file could not be opened. Default settings will be used.",\
   													 "Information", MB_OK|MB_ICONINFORMATION);
		PT5201Setup->SaveToFile( "PT5201.INI");
  	}

	DecimalSeparator = '.';		// Neccesary cause some transformations will
  										//  otherwise use the decimal separator from windows
	// Commport
	StatusBar->Panels->Items[CommPortPanel]->Text = "COM1";
	StatusBar->Panels->Items[CommSettingsPanel]->Text = "9600";
	StatusBar->Panels->Items[ConnectedPanel]->Text = "Disconnected";

	// COM port may be changed by the user
 	PT5201CommPort->SetCommPort( "COM1");

	// Baudrate, Data-parity-stop i.e. 8-N-2 may NOT be changed by the user
 	PT5201CommPort->SetBaudRate( 9600);
 	PT5201CommPort->SetByteSize( 8);
 	PT5201CommPort->SetParity( NOPARITY);
 	PT5201CommPort->SetStopBits( 2);
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	switch ( Application->MessageBox( "Save changes to PT5201 PC Remote Control?", "Confirm", MB_YESNOCANCEL|MB_ICONQUESTION))
	{
		case mrYes:
			SaveWindowPosition( PT5201Form, "");
			PT5201Setup->SaveToFile( "PT5201.INI");

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

      	if ( TSGForm->Visible) {
	      	TSGForm->ModalResult = mrOk;
	      	TSGForm->Close();
			}

      	if ( AudioForm->Visible)
      	{
	      	AudioForm->ModalResult = mrOk;
	      	AudioForm->Close();
      	}

//			PT5201CommPort->FlushCommPort();
			PT5201CommPort->CloseCommPort();

     		while( PT5201CommPort->Connected())
				;

	   	Action = caFree;
			delete PT5201Setup, PT5201CommPort;
     		break;

		case mrNo:
      	if ( GenlockForm->Visible || Black1Form->Visible || Black2Form->Visible ||
           		Black3Form->Visible || TSGForm->Visible || AudioForm->Visible)
	    	{
				if ( Application->MessageBox( "Close window(s) and loose all changes ?",\
    															    "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)

			  		Action = caNone;
				break;
      	}

			SaveWindowPosition( PT5201Form, "");

//			PT5201CommPort->FlushCommPort();
			PT5201CommPort->CloseCommPort();

   	  	while( PT5201CommPort->Connected())
				;

	   	Action = caFree;
			delete PT5201Setup, PT5201CommPort;
     		break;

  		default:
	  		Action = caNone;
      	break;
	}
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::RestoreFromFileClick(TObject *Sender)
{
	if ( FileOpenDialog->Execute())
  	{
  		if ( PT5201Setup->ReadFromFile( FileOpenDialog->FileName) == -1)
   	{
			Application->MessageBox( "The chosen file is not a valid PT5201 configuration file.",\
   													 "Error", MB_OK|MB_ICONERROR);
    	}

		if ( PT5201CommPort->Connected())
		{
			GenlockForm->UpdateOutput( PT5201Setup->GetGenlockSettings());

			Black1Form->UpdateOutput( PT5201Setup->GetBlackSettings( 0));
			Black2Form->UpdateOutput( PT5201Setup->GetBlackSettings( 1));
			Black3Form->UpdateOutput( PT5201Setup->GetBlackSettings( 2));

			TSGForm->UpdateOutput( PT5201Setup->GetTSGSettings());

			AudioForm->UpdateOutput( PT5201Setup->GetAudioSettings());

			SystemStatusUpdate();

			PT5201LEDsUpdate();
		}
	}
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::OpenPresetClick(TObject *Sender)
{
	if ( PT5201CommPort->Connected() && !TestMode)
	{
		PresetForm->Tag = 1;
		PresetForm->ShowModal();
	}
  	else
  	{
		Application->MessageBox( "No connection to the unit. Please connect and try again.",\
   													 "Information", MB_OK|MB_ICONINFORMATION);
  	}
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::toFileAs1Click(TObject *Sender)
{
	if ( FileSaveDialog->Execute())
  		PT5201Setup->SaveToFile( FileSaveDialog->FileName);
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::SavePresetClick(TObject *Sender)
{
	if ( PT5201CommPort->Connected() && !TestMode)
	{
		PresetForm->Tag = 0;
		PresetForm->ShowModal();
  	}
  	else
  	{
		Application->MessageBox( "No connection to the unit. Please connect and try again.",\
   													 "Information", MB_OK|MB_ICONINFORMATION);
  	}
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FilePrintItemClick(TObject *Sender)
{
	PrintDialog->Execute();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::FilePropertiesItemClick(TObject *Sender)
{
	PropertiesForm->ShowModal();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::FileItemExitClick(TObject *Sender)
{
	PT5201Form->Close();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::EditSystemItemClick(TObject *Sender)
{
	SystemForm->ShowModal();
}
//---------------------------------------------------------------------------
//
//
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

	if ( !TestMode)
  	{
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

	  		KUNumberEdit->Text = KUNumberPtr;
		  	SWRevisionEdit->Text = SWPtr;

			delete [] buffer;
	  	}
  		else
	  	{
			strcpy( buffer, "No response from the unit.");

			Application->MessageBox( buffer, "Information", MB_OK|MB_ICONINFORMATION);

			PT5201CommPort->CloseCommPort();

			delete [] buffer;
			return;
	  	}
	}

  	StatusBar->Panels->Items[0]->Text = "Connected";

	GenlockForm->UpdateOutput( PT5201Setup->GetGenlockSettings());

	Black1Form->UpdateOutput( PT5201Setup->GetBlackSettings( 0));
	Black2Form->UpdateOutput( PT5201Setup->GetBlackSettings( 1));
	Black3Form->UpdateOutput( PT5201Setup->GetBlackSettings( 2));

	TSGForm->UpdateOutput( PT5201Setup->GetTSGSettings());

	AudioForm->UpdateOutput( PT5201Setup->GetAudioSettings());

	SystemStatusUpdate();

	PT5201LEDsUpdate();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::ConnectDisconnectItemClick(TObject *Sender)
{
	try
  	{
		PT5201CommPort->CloseCommPort();
  	}
  	catch( ECommError &e)
  	{
		char *buffer = new char[40];

		strcpy( buffer, "Unable to close ");
   	strcat( buffer, PT5201CommPort->GetCommPort());
    	strcat( buffer, ".");

		Application->MessageBox( buffer, "Connect", MB_OK|MB_ICONINFORMATION);

		delete [] buffer;
		return;
  	}

  	StatusBar->Panels->Items[0]->Text = "Disconnected";

	if ( ViewSystemStatusItem->Checked)
		SystemStatusUpdate();

	PT5201LEDsUpdate();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::ConfigureGenlockItemClick(TObject *Sender)
{
	GenlockForm->Show();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::BlackBurst1ItemClick(TObject *Sender)
{
	Black1Form->Show();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::BlackBurst2ItemClick(TObject *Sender)
{
	Black2Form->Show();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::BlackBurst3ItemClick(TObject *Sender)
{
	Black3Form->Show();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::ConfigureColorBarItemClick(TObject *Sender)
{
	TSGForm->Show();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::ConfigureAudioItemClick(TObject *Sender)
{
	AudioForm->Show();
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::ViewSystemReportItemClick(TObject *Sender)
{
	SystemReportForm->Show();//Modal();
}
//---------------------------------------------------------------------------
//
//
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
		SystemStatusUpdate();

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
//	Application->HelpCommand( HELP_FINDER, 0);
}
//---------------------------------------------------------------------------
//
//
void __fastcall TPT5201Form::Frequentlyaskedquestions1Click(TObject *Sender)
{
	ShellExecute( NULL, "open", "http://www.ptv.dk", NULL, NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------
//
//
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

	PT5201LEDsUpdate();

	if ( PT5201CommPort->Connected())
  	{
		GenlockStatus = PT5201Setup->GetGenlockSettings();

  		for ( int i=0; i<3; i++)
			BBStatus[i] = PT5201Setup->GetBlackSettings( i);

		TSGStatus = PT5201Setup->GetTSGSettings();
		AudioStatus = PT5201Setup->GetAudioSettings();

		GenlockSystemEdit->Text = GenlockSystemTxt[GenlockStatus->System];
		SamplesToFLO( GenlockStatus->System, GenlockStatus->Samples, \
    											GenlockFieldEdit, GenlockLineEdit, GenlockOffsetEdit);

		BB1SystemEdit->Text = BBSystemTxt[BBStatus[Black1Output]->System];
		SamplesToFLO( BBStatus[Black1Output]->System, BBStatus[Black1Output]->Samples, \
    											BB1FieldEdit, BB1LineEdit, BB1OffsetEdit);
		BB1ScHEdit->Text = IntToStr( BBStatus[Black1Output]->ScHPhase);

		BB2SystemEdit->Text = BBSystemTxt[BBStatus[Black2Output]->System];
		SamplesToFLO( BBStatus[Black2Output]->System, BBStatus[Black2Output]->Samples, \
    											BB2FieldEdit, BB2LineEdit, BB2OffsetEdit);
		BB2ScHEdit->Text = IntToStr( BBStatus[Black2Output]->ScHPhase);

		BB3SystemEdit->Text = BBSystemTxt[BBStatus[Black3Output]->System];
		SamplesToFLO( BBStatus[Black3Output]->System, BBStatus[Black3Output]->Samples, \
    											BB3FieldEdit, BB3LineEdit, BB3OffsetEdit);
		BB3ScHEdit->Text = IntToStr( BBStatus[Black3Output]->ScHPhase);

		TSGSystemEdit->Text = TSGSystemTxt[TSGStatus->System];
		SamplesToFLO( TSGStatus->System, TSGStatus->Samples, \
    											TSGFieldEdit, TSGLineEdit, TSGOffsetEdit);
		TSGScHEdit->Text = IntToStr( TSGStatus->ScHPhase);
		TSGPatternEdit->Text = TSGPatternTxt[TSGStatus->Pattern];
		TSGEmbAudioEdit->Text = EmbAudioToneTxt[TSGStatus->EmbAudio];


		AudioOutputEdit->Text = AudioOutputTxt[AudioStatus->Output];
		AudioSystemEdit->Text = AudioSystemTxt[AudioStatus->System];
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

		TSGSystemEdit->Text = "";
		TSGFieldEdit->Text = "";
		TSGLineEdit->Text = "";
		TSGOffsetEdit->Text = "";
		TSGScHEdit->Text = "";
		TSGPatternEdit->Text = "";
		TSGEmbAudioEdit->Text = "";

		AudioOutputEdit->Text = "";
		AudioSystemEdit->Text = "";
		AudioTimingEdit->Text = "";
		AudioToneEdit->Text = "";
		AudioLevelEdit->Text = "";
		AudioWordEdit->Text = "";
		AudioClickEdit->Text = "";
		AudioPLLEdit->Text = "";
		AudioVideoEdit->Text = "";

	  	ErrorStatusEdit->Text = "";
  		KUNumberEdit->Text = "";
  		SWRevisionEdit->Text = "";
	  	SayWhatEdit->Text = "";

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

	TSGSystemEdit->Color = activeColor;
	TSGFieldEdit->Color = activeColor;
	TSGLineEdit->Color = activeColor;
	TSGOffsetEdit->Color = activeColor;
	TSGScHEdit->Color = activeColor;
	TSGPatternEdit->Color = activeColor;
	TSGEmbAudioEdit->Color = activeColor;

	AudioOutputEdit->Color = activeColor;
	AudioSystemEdit->Color = activeColor;
	AudioTimingEdit->Color = activeColor;
	AudioToneEdit->Color = activeColor;
	AudioLevelEdit->Color = activeColor;
	AudioWordEdit->Color = activeColor;
	AudioClickEdit->Color = activeColor;
	AudioPLLEdit->Color = activeColor;
	AudioVideoEdit->Color = activeColor;

  	ErrorStatusEdit->Color = activeColor;
  	KUNumberEdit->Color = activeColor;
  	SWRevisionEdit->Color = activeColor;
  	SayWhatEdit->Color = activeColor;
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

	UpdateLEDs( false, TSGPALImage);
	UpdateLEDs( false, TSGNTSCImage);

	UpdateLEDs( false, AudioAESEBUImage);
	UpdateLEDs( false, AudioAnalogImage);

	UpdateLEDs( false, Word441Image);
	UpdateLEDs( false, Word48Image);

	UpdateLEDs( false, PowerImage);
	UpdateLEDs( false, GenlockImage);

	if ( PT5201CommPort->Connected())
  	{
		for ( int i=0; i<3; i++)
			BBStatus[i] = PT5201Setup->GetBlackSettings( i);

		TSGStatus = PT5201Setup->GetTSGSettings();
		AudioStatus = PT5201Setup->GetAudioSettings();

		if ( BBStatus[Black1Output]->System < NTSC)
  		{
			UpdateLEDs( true, BB1PALImage);
			UpdateLEDs( false, BB1NTSCImage);
	  	}
  		else
	  	{
			UpdateLEDs( false, BB1PALImage);
			UpdateLEDs( true, BB1NTSCImage);
	  	}

		if ( BBStatus[Black2Output]->System < NTSC)
  		{
			UpdateLEDs( true, BB2PALImage);
			UpdateLEDs( false, BB2NTSCImage);
  		}
	  	else
  		{
			UpdateLEDs( false, BB2PALImage);
			UpdateLEDs( true, BB2NTSCImage);
  		}

		if ( BBStatus[Black3Output]->System < NTSC)
	  	{
			UpdateLEDs( true, BB3PALImage);
			UpdateLEDs( false, BB3NTSCImage);
	  	}
  		else
	  	{
			UpdateLEDs( false, BB3PALImage);
			UpdateLEDs( true, BB3NTSCImage);
	  	}

						// This exception has been included because the colorbar
      				//  at present DO NOT support PAL w/PAL_ID. See also UTIL.CPP
		if ( TSGStatus->System == PAL)
  		{
			UpdateLEDs( true, TSGPALImage);
			UpdateLEDs( false, TSGNTSCImage);
  		}
	  	else
  		{
			UpdateLEDs( false, TSGPALImage);
			UpdateLEDs( true, TSGNTSCImage);
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

		UpdateLEDs( true, PowerImage);
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
void __fastcall TPT5201Form::Timer1Timer(TObject *Sender)
{
	if ( PT5201CommPort->Connected() && !TestMode)
  	{
		char *buffer = new char[60];

		if ( TrxRequest( "SYST:ERR?\r\n", 60, buffer))
	   	ErrorStatusEdit->Text = buffer;
   	else
    	{
	   	ErrorStatusEdit->Text = "Error connecting to unit";
			ConnectDisconnectItemClick( Sender);
    	}

		delete [] buffer;
  	}
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConnectDemoModeItemClick(TObject *Sender)
{
	if ( ConnectDemoModeItem->Checked)
  	{
		ConnectDemoModeItem->Checked = false;
  		TestMode = false;
		StatusBar->Panels->Items[DemoModePanel]->Text = "";

		ConnectDisconnectItemClick( Sender);
	}
  	else
  	{
		ConnectDemoModeItem->Checked = true;
  		TestMode = true;
		StatusBar->Panels->Items[DemoModePanel]->Text = "Demo Mode";
  	}
}
//---------------------------------------------------------------------------
