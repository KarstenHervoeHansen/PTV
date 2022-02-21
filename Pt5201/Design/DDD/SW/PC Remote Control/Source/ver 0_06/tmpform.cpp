//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\registry.hpp>
#pragma hdrstop

#include "tmpform.h"
#include "connect.h"
#include "genlock.h"
#include "proper.h"
#include "black1.h"
#include "black2.h"
#include "black3.h"
#include "colorbar.h"
#include "audio.h"
#include "status.h"
#include "apparatu.h"
#include "about.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma link "Grids"
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

	PT5201Form->ClientHeight = Panel1->Height + StatusBar->Height;

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
void __fastcall TPT5201Form::About1Click(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureGenlockItemClick(TObject *Sender)
{
	GenlockForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureAudioItemClick(TObject *Sender)
{
	AudioForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureColorBarItemClick(TObject *Sender)
{
	CBarForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::BlackBurst11Click(TObject *Sender)
{
	Black1Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::BlackBurst21Click(TObject *Sender)
{
	Black2Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::BlackBurst31Click(TObject *Sender)
{
	Black3Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::FileItemExitClick(TObject *Sender)
{
	PT5201Form->Close();
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
void __fastcall TPT5201Form::ViewSystemStatusItemClick(TObject *Sender)
{
	SystemStatusForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ViewApparatusStatusItemClick(TObject *Sender)
{
	if ( ViewApparatusStatusItem->Checked)
  {
		ViewApparatusStatusItem->Checked = false;
		Panel2->Visible = false;
    Panel2->Height = 200;

    PT5201Form->ClientHeight -= Panel2->Height;
  }
  else
  {
		ViewApparatusStatusItem->Checked = true;
		Panel2->Visible = true;
    Panel2->Height = 200;
    Panel2->Align = alTop;

    PT5201Form->ClientHeight += Panel2->Height;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Contents1Click(TObject *Sender)
{
	Application->HelpCommand( HELP_FINDER, 0);
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ConfigureLockItemClick(TObject *Sender)
{
	if ( ConfigureLockItem->Checked)
		ConfigureLockItem->Checked = false;
	else
		ConfigureLockItem->Checked = true;

	GenlockForm->LockForm( ConfigureLockItem->Checked);
	Black1Form->LockForm( ConfigureLockItem->Checked);
	Black2Form->LockForm( ConfigureLockItem->Checked);
	Black3Form->LockForm( ConfigureLockItem->Checked);
	CBarForm->LockForm( ConfigureLockItem->Checked);
	AudioForm->LockForm( ConfigureLockItem->Checked);
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
void __fastcall TPT5201Form::ConnectConnectItemClick(TObject *Sender)
{
	ConnectForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Print1Click(TObject *Sender)
{
	PrintDialog1->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::GenlockGroupBoxDblClick(TObject *Sender)
{
	GenlockForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Black1GroupBoxDblClick(TObject *Sender)
{
	Black1Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Black2GroupBoxDblClick(TObject *Sender)
{
	Black2Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Black3GroupBoxDblClick(TObject *Sender)
{
	Black3Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::ColorBarGroupBoxDblClick(TObject *Sender)
{
	CBarForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::AudioGroupBoxDblClick(TObject *Sender)
{
	AudioForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TPT5201Form::Properties2Click(TObject *Sender)
{
	PropertiesForm->ShowModal();
}
//---------------------------------------------------------------------------
