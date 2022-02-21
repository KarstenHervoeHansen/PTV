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

	ptr = TSGSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);

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

	// Setup TV system selection
	SystemComboBox->ItemIndex = TSGSetup->System;

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

	PT5201Form->SystemStatusUpdate();
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
			PT5201Form->SystemStatusUpdate();
    	}
	}
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

	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &TSGSetup->Samples, \
  																OffsetTrackBar))
  	{
		TSGSetup->System = SystemComboBox->ItemIndex;

		if ( !ValidPattern( TSGSetup->System, TSGSetup->Pattern))
		{
			if ( TSGSetup->System == TSG_PAL)
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
		SystemComboBox->ItemIndex = TSGSetup->System;

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

	ScHPhaseAdjust( &TSGSetup->ScHPhase, ScHPhaseEdit, Button);

	TrxTSGScHPhase( TSGSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::ScHPhaseEditExit( TObject *Sender)
{
	ValidateScHPhase( TSGForm, ScHPhaseEdit, &TSGSetup->ScHPhase);

	TrxTSGScHPhase( TSGSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  	TimingAdjust( TSGSetup->System, &TSGSetup->Samples, TSGType, FieldEdit, Button);

	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
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

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  	TimingAdjust( TSGSetup->System, &TSGSetup->Samples, TSGType,	LineEdit, Button);

	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
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

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OffsetUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  	TimingAdjust( TSGSetup->System, &TSGSetup->Samples, TSGType,	OffsetEdit, Button);

	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( TSGSetup->System, TSGSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
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

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::OffsetTrackBarChange(TObject *Sender)
{
  	TrackBarAdjust( TSGSetup->System, &TSGSetup->Samples, OffsetTrackBar);
	SamplesToFLO( TSGSetup->System, TSGSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

#if delaymode
	TrxTSGDelay( FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::PatternComboBoxChange(TObject *Sender)
{
	TSGSetup->Pattern = IndexToPattern( TSGSetup->System, PatternComboBox->ItemIndex);

	TrxTSGPattern( TSGSetup->Pattern);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::EmbAudioComboBoxChange(TObject *Sender)
{
	TSGSetup->EmbAudio = EmbAudioComboBox->ItemIndex;

	TrxTSGEmbAudioSignal( TSGSetup->EmbAudio);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::LockForm( bool Lock)
{
	Lock = !Lock;

	SystemComboBox->Enabled = Lock;
  	PatternComboBox->Enabled = Lock;
	FieldUpDown->Enabled = Lock;
	FieldEdit->Enabled = Lock;
	LineUpDown->Enabled = Lock;
	LineEdit->Enabled = Lock;
	OffsetUpDown->Enabled = Lock;
	OffsetEdit->Enabled = Lock;
	OffsetTrackBar->Enabled = Lock;
	ScHPhaseUpDown->Enabled = Lock;
	ScHPhaseEdit->Enabled = Lock;
	EmbAudioComboBox->Enabled = Lock;

	CancelButton->Enabled = Lock;

	if ( TSGForm->Visible)
		TSGForm->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TTSGForm::UpdateOutput( TTSGenerator *setup)
{
	TrxTSGSystem( setup->System);
	TrxTSGPattern( setup->Pattern);

#if delaymode
	SamplesToFLO( setup->System, setup->Samples, Field1Edit, Line1Edit, Offset1Edit);
	TrxTSGDelay( Field1Edit->Text.ToInt(), Line1Edit->Text.ToInt(), Offset1Edit->Text.ToDouble());
#endif

	TrxTSGScHPhase( setup->ScHPhase);
	TrxTSGEmbAudioSignal( setup->EmbAudio);
}
//---------------------------------------------------------------------------
int ValidPattern( int system, int pattern)
{
	switch ( pattern)
  	{
		case SMPTECBar:
		case FCCCBar:
			if ( system == TSG_PAL)
   	   	return( 0);
			break;

		case EBUCBar:
		case CBarRed75:
		case CCIR18:
			if ( system == TSG_NTSC)
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
	if ( system == TSG_PAL)
  	{
		switch ( pattern)
    	{
			case SMPTECBar:
			case EBUCBar:
			case FCCCBar:
      		return( 0);

			case EBUCBarITU:
			case CBar100:
	      	return( pattern-2);

			default:
	      	return( pattern-3);
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
      		return( 0);

			case FCCCBar:
			case EBUCBarITU:
			case CBar100:
      		return( pattern-1);

			case Red75:
      	case Multiburst:
	      	return( pattern-3);

			default:
	      	return( pattern-4);
    	}
  	}
}
//---------------------------------------------------------------------------
int IndexToPattern( int system, int index)
{
	if ( system == TSG_PAL)
  	{
		switch ( index)
    	{
    		case 0:
       		return( index + 1);

    		case 1:
    		case 2:
	      	return( index + 2);

			default:
	      	return( index + 3);
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
			case 5:
	   	   return( index + 3);

			default:
	      	return( index + 4);
    	}
	}
}
//---------------------------------------------------------------------------
/*
	Index to PAL pattern:           Index to NTSC pattern:
	-,		//  0 SMPTECBar				0,		//  0 SMPTECBar
	0, 	//  1 EBUCBar             	-, 	//  1 EBUCBar
	-,		//  2 FCCCBar					1,		//  2 FCCCBar
	1, 	//  3 EBUCBarITU				2, 	//  3 EBUCBarITU
	2, 	//  4 CBar100					3, 	//  4 CBar100
	-, 	//  5 CBarGrey75				-, 	//  5 CBarGrey75
	3, 	//  6 CBarRed75				-, 	//  6 CBarRed75
	4, 	//  7 Red75						4, 	//  7 Red75
	5, 	//  8 Multiburst				5, 	//  8 Multiburst
	6, 	//  9 CCIR18					-, 	//  9 CCIR18
	7, 	// 10 Window10					6, 	// 10 Window10
	8, 	// 11 Window15					7, 	// 11 Window15
	9, 	// 12 Window20					8, 	// 12 Window20
 	10, 	// 13 Window100 				9, 	// 13 Window100
 	11, 	// 14 BlWh15kHz 				10, 	// 14 BlWh15kHz
	12, 	// 15 White100					11, 	// 15 White100
	13, 	// 16 BlackBurst				12, 	// 16 BlackBurst
 	14, 	// 17 CheckField 				13, 	// 17 CheckField
 	15, 	// 18 DigitalGrey 			14, 	// 18 DigitalGrey
 	16, 	// 19 Stair5 					15, 	// 19 Stair5
	17,	// 20 Stair10					16,	// 20 Stair10
	18, 	// 21 Crosshatch				17, 	// 21 Crosshatch
	19 	// 22 PLUGE						18 	// 22 PLUGE
*/
//---------------------------------------------------------------------------
