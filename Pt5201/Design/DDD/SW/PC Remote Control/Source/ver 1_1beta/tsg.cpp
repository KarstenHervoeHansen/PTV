//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "tsg.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

int ValidPattern( int system, int pattern);			// Return validation information
int PatternToIndex( int system, int pattern);		// Return an index
int IndexToPattern( int system, int index);			// Return a pattern

TTSGForm *TSGForm;
//---------------------------------------------------------------------------
__fastcall TTSGForm::TTSGForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FormCreate(TObject *Sender)
{
	const char* *ptr;

	// Prepare TV system choices
	SystemComboBox->Items->Clear();

//	ptr = SystemTxt;
//	while ( *ptr)
//		SystemComboBox->Items->Add( *ptr++);
	SystemComboBox->Items->Add( PALTxt);
	SystemComboBox->Items->Add( NTSCTxt);

	// Prepare Embedded Audio choices
	EmbAudioComboBox->Items->Clear();

  	ptr = EmbAudioToneTxt;
   while ( *ptr)
		EmbAudioComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( TSGForm, "TSGWindow");

	// Setup pointer to actual settings
	TSGSetup = PT5201Setup->GetTSGSettings();

	TSGCancel = new TTSGenerator;

	// Copy original settings temporarily to TSGCancel
	*TSGCancel = *TSGSetup;

	// Setup TV system selection, (ONLY PAL & NTSC I TSG)
	SystemComboBox->ItemIndex = !( TSGSetup->System == PAL);

	// Setup ScHPhase selection
	ScHPhaseEdit->Text = IntToStr( TSGSetup->ScHPhase);

	// Setup Embedded Audio selection
	EmbAudioComboBox->ItemIndex = TSGSetup->EmbAudio;

	// Prepare pattern choices, (this one is system dependant)
	ptr = TSGPatternTxt;
	PatternComboBox->Items->Clear();

	int i = SMPTECBar;
	while ( *ptr)
 	{
		if ( ValidPattern( TSGSetup->System, i++))
			PatternComboBox->Items->Add( *ptr);
		ptr++;
	}

  PatternComboBox->ItemIndex = PatternToIndex( TSGSetup->System, TSGSetup->Pattern);

	// Display timing selection
	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	// Setup TrackBar markings and resolution
  TrackBarSetup( TSGSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

  LockForm( PropertiesForm->ConfigureLockItem->Checked);

										// It is not possible to foxus an invisible or disabled
										//  window, hence the statement below.
  if ( !PropertiesForm->ConfigureLockItem->Checked)
	{
		if ( AnalogSettingsTabSheet->Visible)
			ActiveControl = SystemComboBox;
		else
			ActiveControl = EmbAudioComboBox;
  }
										// Edit tags are used during validation of an entry to
										//  separate which edit field is used.
	FieldEdit->Tag = 0;
	LineEdit->Tag = 0;
	OffsetEdit->Tag = 0;

  ModelessResult = mrNone;
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete TSGCancel;

	SaveWindowPosition( TSGForm, "TSGWindow");

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if ( ModelessResult != mrOk)
	{
		if  ( Application->MessageBox( "Close window and loose all changes ?",\
    															 "Confirm", MB_OKCANCEL|MB_ICONQUESTION) != mrOk)
    {
			ModelessResult = mrNone;

			CanClose = false;
		}
   	else														// Restore original settings
		{
			PT5201Setup->SetTSGSettings( TSGCancel);
			UpdateOutput( TSGCancel);
    }
	}
	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OKButtonClick(TObject *Sender)
{
	if ( ScHPhaseEdit->Focused())
		if ( !ValidateScHPhase( TSGForm, ScHPhaseEdit, &TSGSetup->ScHPhase))
  		return;

	if ( FieldEdit->Focused())
		FieldEdit->Tag = true;
  else
		if ( LineEdit->Focused())
			LineEdit->Tag = true;
 	else
 		if ( OffsetEdit->Focused())
			OffsetEdit->Tag = true;

  	if ( FieldEdit->Tag || LineEdit->Tag || OffsetEdit->Tag)
			if ( !ValidateEntry( TSGForm, TSGSetup->System, &TSGSetup->Samples, \
  												FieldEdit, LineEdit, OffsetEdit))
	  		return;

	ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::CancelButtonClick(TObject *Sender)
{
  ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::SystemComboBoxChange(TObject *Sender)
{
	const char* *ptr;
	int OldSystem, System;

	if (( OldSystem = SystemComboBox->ItemIndex) == 1)
		System = NTSC;
  else
  	System = PAL;

	if ( ValidatePALToNTSC( System, &TSGSetup->Samples, \
  																OffsetTrackBar))
  {
		TSGSetup->System = System;

		if ( !ValidPattern( TSGSetup->System, TSGSetup->Pattern))
		{
			if ( TSGSetup->System < NTSC)
				TSGSetup->Pattern = EBUCBar;
     	else
     	{
				if ( TSGSetup->Pattern == CCIR18)
					TSGSetup->Pattern = Multiburst;
				else
					TSGSetup->Pattern = SMPTECBar;
     	}
   	}

		// Prepare pattern choices
		ptr = TSGPatternTxt;
		PatternComboBox->Items->Clear();

		int i = SMPTECBar;
		while ( *ptr)
	 	{
			if ( ValidPattern( TSGSetup->System, i++))
				PatternComboBox->Items->Add( *ptr);
			ptr++;
	 	}

		PatternComboBox->ItemIndex = PatternToIndex( TSGSetup->System, TSGSetup->Pattern);

		TrackBarSetup( TSGSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

		SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

		TrxTSGSystem( TSGSetup->System);

		TrxTSGPattern( TSGSetup->Pattern);	// Really not necessary! Illegal patterns
			                                 //  will be altered in the PT5201.
	}
  else
		SystemComboBox->ItemIndex = OldSystem;

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

	ScHPhaseAdjust( &TSGSetup->ScHPhase, ScHPhaseEdit, Button);

	TrxTSGScHPhase( TSGSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::ScHPhaseEditExit( TObject *Sender)
{
	if ( ValidateScHPhase( TSGForm, ScHPhaseEdit, &TSGSetup->ScHPhase))
		TrxTSGScHPhase( TSGSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

	TimingAdjust( TSGSetup->System, &TSGSetup->Samples, TSGType, FieldEdit, Button);

	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FieldUpDownChanging(TObject *Sender,
bool &AllowChange)
{
	FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( TSGForm, TSGSetup->System, &TSGSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

 	TimingAdjust( TSGSetup->System, &TSGSetup->Samples, TSGType,	LineEdit, Button);

	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::LineEditExit(TObject *Sender)
{
	LineEdit->Tag = true;

	ValidateEntry( TSGForm, TSGSetup->System, &TSGSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OffsetUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

 	TimingAdjust( TSGSetup->System, &TSGSetup->Samples, TSGType,	OffsetEdit, Button);

	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( TSGForm, TSGSetup->System, &TSGSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OffsetTrackBarChange(TObject *Sender)
{
 	TrackBarAdjust( TSGSetup->System, &TSGSetup->Samples, OffsetTrackBar);
	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::PatternComboBoxChange(TObject *Sender)
{
	TSGSetup->Pattern = IndexToPattern( TSGSetup->System, PatternComboBox->ItemIndex);

	TrxTSGPattern( TSGSetup->Pattern);

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::EmbAudioComboBoxChange(TObject *Sender)
{
	TSGSetup->EmbAudio = EmbAudioComboBox->ItemIndex;

	TrxTSGEmbAudioSignal( TSGSetup->EmbAudio);

	PT5201Form->SystemStatusUpdate( TSGField);
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::LockForm( bool Lock)
{
	bool enable = !Lock;

	SystemComboBox->Enabled = enable;
 	PatternComboBox->Enabled = enable;
	FieldUpDown->Enabled = enable;
	FieldEdit->Enabled = enable;
	LineUpDown->Enabled = enable;
	LineEdit->Enabled = enable;
	OffsetUpDown->Enabled = enable;
	OffsetEdit->Enabled = enable;
	OffsetTrackBar->Enabled = enable;
	ScHPhaseUpDown->Enabled = enable;
	ScHPhaseEdit->Enabled = enable;
	EmbAudioComboBox->Enabled = enable;

	CancelButton->Enabled = enable;

	if ( TSGForm->Visible)
		TSGForm->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::UpdateOutput( TTSGenerator *setup)
{
	TrxTSGSystem( setup->System);
	TrxTSGPattern( setup->Pattern);

	SamplesToFLO( setup->System, setup->Samples, Field1Edit, Line1Edit, Offset1Edit);

	TrxTSGDelay( Field1Edit->Text.ToInt(), Line1Edit->Text.ToInt(), Offset1Edit->Text.ToDouble());
	TrxTSGScHPhase( setup->ScHPhase);
	TrxTSGEmbAudioSignal( setup->EmbAudio);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int ValidPattern( int system, int pattern)
{
	switch ( pattern)
  	{
		case SMPTECBar:
		case FCCCBar:
			if ( system < NTSC)
   	   	return( 0);
			break;

		case Multiburst:				// At present Multiburst is not valid at all
   		return( 0);

		case EBUCBar:
		case CBarRed75:
		case CCIR18:
			if ( system >= NTSC)
     		return( 0);
     	break;

		case CBarGrey75:
   		return(0);
	}

	return( -1);
}
//---------------------------------------------------------------------------
int PatternToIndex( int system, int pattern)
{
	if ( system < NTSC)
 	{
		switch ( pattern)
   	{
			case SMPTECBar:
			case EBUCBar:
			case FCCCBar:
			case Multiburst:
     		return( 0);

			case EBUCBarITU:
			case CBar100:
	     	return( pattern-2);

			case CBarRed75:
			case Red75:
	     	return( pattern-3);

			default:
	     	return( pattern-4);
   	}
	}
  else
  {
		switch ( pattern)
    {
			case SMPTECBar:
			case EBUCBar:
			case CBarGrey75:
			case CBarRed75:
			case CCIR18:
			case Multiburst:
     		return( 0);

			case FCCCBar:
			case EBUCBarITU:
			case CBar100:
     		return( pattern-1);

			case Red75:
	     	return( pattern-3);

			default:
      	return( pattern-5);
 		}
 	}
}
//---------------------------------------------------------------------------
int IndexToPattern( int system, int index)
{
	if ( system < NTSC)
 	{
		switch ( index)
    {
    	case 0:
     		return( index + 1);

    	case 1:
    	case 2:
	     	return( index + 2);

    	case 3:
    	case 4:
	     	return( index + 3);

			default:
	     	return( index + 4);
     }
	}
  else
  {
		switch ( index)
    {
			case 0:
				return( index);

			case 1:
			case 2:
			case 3:
				return( index + 1);

			case 4:
	   	   return( index + 3);

			default:
	      	return( index + 5);
    }
	}
}
//---------------------------------------------------------------------------
/*
	Index to PAL pattern:         Index to NTSC pattern:
	-,	//  0 SMPTECBar				0,	//  0 SMPTECBar
	0, 	//  1 EBUCBar         -, 	//  1 EBUCBar
	-,	//  2 FCCCBar					1,	//  2 FCCCBar
	1, 	//  3 EBUCBarITU			2, 	//  3 EBUCBarITU
	2, 	//  4 CBar100					3, 	//  4 CBar100
	-, 	//  5 CBarGrey75			-, 	//  5 CBarGrey75
	3, 	//  6 CBarRed75				-, 	//  6 CBarRed75
	4, 	//  7 Red75						4, 	//  7 Red75
	-, 	//  8 Multiburst			-, 	//  8 Multiburst
	5, 	//  9 CCIR18					-, 	//  9 CCIR18
	6, 	// 10 Window10				5, 	// 10 Window10
	7, 	// 11 Window15				6, 	// 11 Window15
	8, 	// 12 Window20				7, 	// 12 Window20
 	9,  // 13 Window100 			8, 	// 13 Window100
 	10, // 14 BlWh15kHz 			9,  // 14 BlWh15kHz
	11, // 15 White100				10, // 15 White100
	12, // 16 BlackBurst			11, // 16 BlackBurst
 	13, // 17 CheckField 			12, // 17 CheckField
 	14, // 18 DigitalGrey 		13, // 18 DigitalGrey
 	15, // 19 Stair5 					14, // 19 Stair5
	16,	// 20 Stair10					15,	// 20 Stair10
	17, // 21 Crosshatch			16, // 21 Crosshatch
	18 	// 22 PLUGE						17 	// 22 PLUGE
*/
//---------------------------------------------------------------------------
void __fastcall TTSGForm::HelpButtonClick(TObject *Sender)
{
	Application->HelpContext( IDH_EDITTSG);
}
//---------------------------------------------------------------------------
