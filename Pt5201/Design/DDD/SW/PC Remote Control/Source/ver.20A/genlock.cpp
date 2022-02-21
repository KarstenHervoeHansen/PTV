/* 100 mS delay implemented in  OffsetTrackBarChange() to avois
   errors in 5201 due to too fast transmission of Genlock delay
   11.07.2001 :JK
*/   

//--------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop
#include <dos.h>
#include "mainform.h"
#include "proper.h"
#include "genlock.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TGenlockForm *GenlockForm;

Boolean timeout2; // Global variable indicating Timer2 timeout

//---------------------------------------------------------------------------
__fastcall TGenlockForm::TGenlockForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormCreate(TObject *Sender)
{
	const char* *ptr;

	// Prepare Genlock system choice
	SystemComboBox->Items->Clear();

  ptr = GenlockSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormShow(TObject *Sender)
{
	RestoreWindowPosition( GenlockForm, "GenlockWindow");

	// Setup pointer to actual settings
	GenlockSetup = PT5201Setup->GetGenlockSettings();

	GenlockCancel = new TGenlockGenerator;

	// Copy original settings temporarily to GenlockCancel
	*GenlockCancel = *GenlockSetup;

	// Setup TV system selection
	SystemComboBox->ItemIndex = GenlockSetup->System;

	// Display timing selection
	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

	//TrackBarSetup( PAL, OffsetTrackBar, MinLabel, MaxLabel);
	TrackBarSetup( GenlockSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

  if ( !PropertiesForm->ConfigureLockItem->Checked)
		ActiveControl = SystemComboBox;

	FieldEdit->Tag = 0;
	LineEdit->Tag = 0;
	OffsetEdit->Tag = 0;

  ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete GenlockCancel;

	SaveWindowPosition( GenlockForm, "GenlockWindow");
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (( ModelessResult != mrOk) && !PT5201Setup->VerifyGenlockSettings( GenlockCancel))
  {
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
		{
			ModelessResult = mrNone;

			CanClose = false;
		}
    else														// Restore original settings
		{
    	PT5201Setup->SetGenlockSettings( GenlockCancel);
      UpdateOutput( GenlockCancel);
    }
  }

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OKButtonClick(TObject *Sender)
{
	if ( FieldEdit->Focused())
		FieldEdit->Tag = true;
  else
		if ( LineEdit->Focused())
			LineEdit->Tag = true;
  else
		if ( OffsetEdit->Focused())
			OffsetEdit->Tag = true;

	if ( FieldEdit->Tag || LineEdit->Tag || OffsetEdit->Tag)
		if ( !ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  												FieldEdit, LineEdit, OffsetEdit))
	  	return;

	ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::CancelButtonClick(TObject *Sender)
{
	ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::SystemComboBoxChange(TObject *Sender)
{
	bool EditEnable;

	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &GenlockSetup->Samples, \
														OffsetTrackBar))
	{
		GenlockSetup->System = SystemComboBox->ItemIndex;

		EditEnable = true;

		switch ( GenlockSetup->System)
    {
    	case PALGenlock:
				TrackBarSetup( PAL, OffsetTrackBar, MinLabel, MaxLabel);
				break;

    	case NTSCGenlock:
				TrackBarSetup( NTSC, OffsetTrackBar, MinLabel, MaxLabel);
				break;

			default:
				EditEnable = false;
				break;
		}

		if ( !PropertiesForm->ConfigureLockItem->Checked)
		{
			FieldUpDown->Enabled = EditEnable;
			FieldEdit->Enabled = EditEnable;
			LineUpDown->Enabled = EditEnable;
		 	LineEdit->Enabled = EditEnable;
			OffsetUpDown->Enabled = EditEnable;
		 	OffsetEdit->Enabled = EditEnable;
		 	OffsetTrackBar->Enabled = EditEnable;
		}

		SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

		TrxGenlockSystem( GenlockSetup->System);
	}
  else
		SystemComboBox->ItemIndex = GenlockSetup->System;

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( GenlockSetup->System, &GenlockSetup->Samples, GenlockType,	FieldEdit, Button);

	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( GenlockSetup->System, &GenlockSetup->Samples, GenlockType, LineEdit, Button);

	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LineEditExit(TObject *Sender)
{
	LineEdit->Tag = true;

	ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( GenlockSetup->System, &GenlockSetup->Samples, GenlockType, OffsetEdit, Button);

	SamplesToFLO( GenlockSetup->System, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( GenlockForm, GenlockSetup->System, &GenlockSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( GenlockSetup->System, GenlockSetup->Samples, OffsetTrackBar);

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( GenlockField);
}
//---------------------------------------------------------------------------

void __fastcall TGenlockForm::OffsetTrackBarChange(TObject *Sender)
  {
  //static TGenlockGenerator *GenlockSetupBuff;


 if (timeout2 == false) // if not 100 mS elapsed since last transmission, return
   {
   return;
   }
 TrackBarAdjust( SystemComboBox->ItemIndex, &GenlockSetup->Samples, OffsetTrackBar);
	SamplesToFLO( SystemComboBox->ItemIndex, GenlockSetup->Samples, FieldEdit, LineEdit, OffsetEdit);
	PT5201Form->SystemStatusUpdate( GenlockField);
	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
	timeout2 = false;

  }

//---------------------------------------------------------------------------
void __fastcall TGenlockForm::LockForm( bool Lock)
{
	bool enable = !Lock;

	SystemComboBox->Enabled = enable;

// JK addition
	// Setup pointer to actual settings
	GenlockSetup = PT5201Setup->GetGenlockSettings();

// end of addition


	CancelButton->Enabled = enable;
// below: exception class EAccess violation error! :JK
#if 1
	enable &= !(( GenlockSetup->System == InternalGenlock) ||
  	  			 ( GenlockSetup->System == f10MHzGenlock));
#endif

	FieldUpDown->Enabled = enable;
	FieldEdit->Enabled = enable;
	LineUpDown->Enabled = enable;
	LineEdit->Enabled = enable;
	OffsetUpDown->Enabled = enable;
	OffsetEdit->Enabled = enable;
	OffsetTrackBar->Enabled = enable;

  if ( GenlockForm->Visible)
  	GenlockForm->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::UpdateOutput( TGenlockGenerator *setup)
{
	TrxGenlockSystem( setup->System);

	SamplesToFLO( setup->System, setup->Samples, Field1Edit, Line1Edit, Offset1Edit);

	TrxGenlockDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
}
//---------------------------------------------------------------------------
void __fastcall TGenlockForm::HelpButtonClick(TObject *Sender)
{
	Application->HelpContext( IDH_EDITGENLOCK);
}

/* We need to implement simple delay in TGenlockForm::OffsetTrackBarChange()
   so that rapid changes in Genlock trackbar do not cause errors in PT5201.
   Apparently, when Genlock timing is transmitted too fast, 5201 cannot handle
   timing setting.  This happens only in Genlock.
   We introduce a new timer, Timer2, which ticks @100 mS.  The function below
   is executed when Timer2 times out.  It simply updates a global variable.
   In TGenlockForm::OffsetTrackBarChange(), we do not send the timing unless
   Timer2 has timed out.
   .
   .
   .
   if (!timeout2)
     return;
   send_genlock_timing();
   timeout2 = false;
   start_timer2();
   return;
*/
void __fastcall TPT5201Form::Timer2Timer(TObject *Sender)
  {
 	timeout2 = true;
  }
  

