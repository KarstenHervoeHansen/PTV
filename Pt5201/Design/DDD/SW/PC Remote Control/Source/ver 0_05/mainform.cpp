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
//---------------------------------------------------------------------------
#pragma link "Grids"
#pragma link "GIFImage"
#pragma resource "*.dfm"

TPT5201Form *PT5201Form;
TSyncGenerator *PT5201Setup;

const TCursor crMyCustomCursor = 1;    // ID for custom cursor

//---------------------------------------------------------------------------
__fastcall TPT5201Form::TPT5201Form(TComponent* Owner)
	: TForm(Owner)
{
	Screen->Cursors[crMyCustomCursor] = LoadCursor((void*)HInstance, "handpnt");
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FormCreate(TObject *Sender)
{
	PT5201Setup = new TSyncGenerator;

  RestoreWindowPosition( PT5201Form, "");

	PT5201Form->ClientHeight = PT5201FrontPanel->Height + StatusBar->Height;

	PT5201Form->Refresh();
	// Baudrate data-parity-stop e.g. 115000 8-N-2
	PT5201Form->StatusBar->Panels->Items[1]->Text = "11500 8-N-2";

	// Baudrate data-parity-stop e.g. 115000 8-N-2
	PT5201Form->StatusBar->Panels->Items[0]->Text = "Disconnected";
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
			delete PT5201Setup;
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
			delete PT5201Setup;
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
void __fastcall TPT5201Form::toFileAs1Click(TObject *Sender)
{
	FileSaveDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FilePrintItemClick(TObject *Sender)
{
	PrintDialog1->Execute();
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
void __fastcall TPT5201Form::ConnectConnectItemClick(TObject *Sender)
{
	ConnectForm->ShowModal();
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
void __fastcall TPT5201Form::ViewPT5201StatusItemClick(TObject *Sender)
{
	if ( ViewPT5201StatusItem->Checked)
  {
		ViewPT5201StatusItem->Checked = false;
		ApparatusStatusPanel->Visible = false;
    ApparatusStatusPanel->Height = 326;

    PT5201Form->ClientHeight -= ApparatusStatusPanel->Height;
  }
  else
  {
		ViewPT5201StatusItem->Checked = true;
		ApparatusStatusPanel->Visible = true;
    ApparatusStatusPanel->Height = 326;
    ApparatusStatusPanel->Align = alTop;

    PT5201Form->ClientHeight += ApparatusStatusPanel->Height;
  }
}
//---------------------------------------------------------------------------
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
void __fastcall TPT5201Form::Contents1Click(TObject *Sender)
{
	Application->HelpCommand( HELP_FINDER, 0);
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::HelpAboutItemClick(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall ApparatusStatusUpdate(TObject *Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Image2DblClick(TObject *Sender)
{
	if ( MooseGIFImage->Visible)
		MooseGIFImage->Visible = false;
	else
		MooseGIFImage->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::OpenPresetClick(TObject *Sender)
{
	PresetForm->Tag = 1;
	PresetForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::SavePresetClick(TObject *Sender)
{
	PresetForm->Tag = 0;
	PresetForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::EditSystemItemClick(TObject *Sender)
{
	SystemForm->ShowModal();
}
//---------------------------------------------------------------------------
