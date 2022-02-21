//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "proper.h"
#include "Colorbar.h"
#include "util.h"
#include "pt52class.h"
#include "report.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"

int ValidPattern( int system, int pattern);			// Return validation information
int PatternToIndex( int system, int pattern);		// Return an index
int IndexToPattern( int system, int index);			// Return a pattern

TCBarForm *CBarForm;
//---------------------------------------------------------------------------
__fastcall TCBarForm::TCBarForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormCreate(TObject *Sender)
{
	const char* *ptr;

	// Prepare TV system choices
	SystemComboBox->Items->Clear();

  ptr = TVSystemTxt;
	while ( *ptr)
		SystemComboBox->Items->Add( *ptr++);

	// Prepare Embedded Audio choices
	EmbAudioComboBox->Items->Clear();

  ptr = EmbAudioToneTxt;
	while ( *ptr)
		EmbAudioComboBox->Items->Add( *ptr++);
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormShow(TObject *Sender)
{
	const char* *ptr;

	RestoreWindowPosition( CBarForm, "CBarWindow");

	// Setup pointer to actual settings
	CBarSetup = PT5201Setup->GetCBarSettings();

	CBarCancel = new TCBarGenerator;

	// Copy original settings temporarily to CBarCancel
	*CBarCancel = *CBarSetup;

	// Setup TV system selection
	SystemComboBox->ItemIndex = CBarSetup->System;

	// Setup ScHPhase selection
	ScHPhaseEdit->Text = IntToStr( CBarSetup->ScHPhase);

	// Setup Embedded Audio selection
	EmbAudioComboBox->ItemIndex = CBarSetup->EmbAudio;

	// Prepare pattern choices, (this one is system dependant)
	ptr = CBarPatternTxt;
	PatternComboBox->Items->Clear();

	int i = SMPTECBar;
	while ( *ptr)
 	{
		if ( ValidPattern( CBarSetup->System, i++))
			PatternComboBox->Items->Add( *ptr);
		ptr++;
  }

	PatternComboBox->ItemIndex = PatternToIndex( CBarSetup->System, CBarSetup->Pattern);

	// Display timing selection
	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	// Setup TrackBar markings and resolution
  TrackBarSetup( CBarSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

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
void __fastcall TCBarForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete CBarCancel;

  SaveWindowPosition( CBarForm, "CBarWindow");

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FormCloseQuery(TObject *Sender, bool &CanClose)
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
    	PT5201Setup->SetCBarSettings( CBarCancel);

			SendTVSystem( "OUTP:CBG:SYST ", CBarCancel->System);
			SendCBarPattern( CBarCancel->Pattern);
#if delaymode
			SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif
			SendScHPhase( "OUTP:CBG:SCHP ", CBarCancel->ScHPhase);
			SendCBarEmbAudioSignal( CBarCancel->EmbAudio);

			PT5201Form->SystemStatusUpdate();
    }
	}
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OKButtonClick(TObject *Sender)
{
	if ( ScHPhaseEdit->Focused())
		if ( !ValidateScHPhase( CBarForm, ScHPhaseEdit, &CBarSetup->ScHPhase))
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
		if ( !ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  												FieldEdit, LineEdit, OffsetEdit))
	  	return;

  ModelessResult = mrOk;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::CancelButtonClick(TObject *Sender)
{
  ModelessResult = mrCancel;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::SystemComboBoxChange(TObject *Sender)
{
	const char* *ptr;

	if ( ValidatePALToNTSC( SystemComboBox->ItemIndex, &CBarSetup->Samples, \
  																OffsetTrackBar))
  {
		CBarSetup->System = SystemComboBox->ItemIndex;

		if ( !ValidPattern( CBarSetup->System, CBarSetup->Pattern))
		{
		  if ( CBarSetup->System < NTSC)
				CBarSetup->Pattern = EBUCBar;
      else
      {
				if ( CBarSetup->Pattern == CCIR18)
					CBarSetup->Pattern = Multiburst;
				else
					CBarSetup->Pattern = SMPTECBar;
      }
    }

		// Prepare pattern choices
		ptr = CBarPatternTxt;
		PatternComboBox->Items->Clear();

		int i = SMPTECBar;
		while ( *ptr)
	 	{
			if ( ValidPattern( CBarSetup->System, i++))
				PatternComboBox->Items->Add( *ptr);
			ptr++;
	  }

		PatternComboBox->ItemIndex = PatternToIndex( CBarSetup->System, CBarSetup->Pattern);

    TrackBarSetup( CBarSetup->System, OffsetTrackBar, MinLabel, MaxLabel);

		SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

		TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

		SendTVSystem( "OUTP:CBG:SYST ", CBarSetup->System);

		SendCBarPattern( CBarSetup->Pattern);	// Really not necessary! Illegal patterns
			                                    //  will be altered in the PT5201.
	}
  else
		SystemComboBox->ItemIndex = CBarSetup->System;

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::ScHPhaseUpDownClick(TObject *Sender,
	TUDBtnType Button)
{
	ActiveControl = ScHPhaseEdit;

	ScHPhaseAdjust( &CBarSetup->ScHPhase, ScHPhaseEdit, Button);

	SendScHPhase( "OUTP:CBG:SCHP ", CBarSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::ScHPhaseEditExit( TObject *Sender)
{
	ValidateScHPhase( CBarForm, ScHPhaseEdit, &CBarSetup->ScHPhase);

	SendScHPhase( "OUTP:CBG:SCHP ", CBarSetup->ScHPhase);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = FieldEdit;

  TimingAdjust( CBarSetup->System, &CBarSetup->Samples, CBarType,	FieldEdit, Button);

	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

	FieldEdit->SelectAll();

#if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldUpDownChanging(TObject *Sender,
bool &AllowChange)
{
FieldUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::FieldEditExit(TObject *Sender)
{
	FieldEdit->Tag = true;

	ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

#if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = LineEdit;

  TimingAdjust( CBarSetup->System, &CBarSetup->Samples, CBarType,	LineEdit, Button);

	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

	LineEdit->SelectAll();

#if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	LineUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LineEditExit(TObject *Sender)
{
	LineEdit->Tag = true;

	ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

#if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetUpDownClick(TObject *Sender, TUDBtnType Button)
{
	ActiveControl = OffsetEdit;

  TimingAdjust( CBarSetup->System, &CBarSetup->Samples, CBarType,	OffsetEdit, Button);

	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

	OffsetEdit->SelectAll();

  #if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetUpDownChanging(TObject *Sender,
	bool &AllowChange)
{
	OffsetUpDown->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetEditExit(TObject *Sender)
{
	OffsetEdit->Tag = true;

	ValidateEntry( CBarForm, CBarSetup->System, &CBarSetup->Samples, \
  								FieldEdit, LineEdit, OffsetEdit);

	TrackBarUpdate( CBarSetup->System, CBarSetup->Samples, OffsetTrackBar);

#if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::OffsetTrackBarChange(TObject *Sender)
{
  TrackBarAdjust( CBarSetup->System, &CBarSetup->Samples, OffsetTrackBar);
	SamplesToFLO( CBarSetup->System, CBarSetup->Samples, FieldEdit, LineEdit, OffsetEdit);

#if delaymode
	SendDelay( "OUTP:CBG:DEL ", FieldEdit->Text.ToInt(), LineEdit->Text.ToInt(), OffsetEdit->Text.ToDouble());
#endif

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::PatternComboBoxChange(TObject *Sender)
{
	CBarSetup->Pattern = IndexToPattern( CBarSetup->System, PatternComboBox->ItemIndex);

	SendCBarPattern( CBarSetup->Pattern);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::EmbAudioComboBoxChange(TObject *Sender)
{
	CBarSetup->EmbAudio = EmbAudioComboBox->ItemIndex;

	SendCBarEmbAudioSignal( CBarSetup->EmbAudio);

	PT5201Form->SystemStatusUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TCBarForm::LockForm( bool Lock)
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

	if ( CBarForm->Visible)
	  CBarForm->Refresh();
}
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
	if ( system < NTSC)
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
	-,		//  0 SMPTECBar						0,		//  0 SMPTECBar
	0, 		//  1 EBUCBar             -, 		//  1 EBUCBar
	-,		//  2 FCCCBar							1,		//  2 FCCCBar
	1, 		//  3 EBUCBarITU					2, 		//  3 EBUCBarITU
	2, 		//  4 CBar100							3, 		//  4 CBar100
	-, 		//  5 CBarGrey75					-, 		//  5 CBarGrey75
	3, 		//  6 CBarRed75						-, 		//  6 CBarRed75
	4, 		//  7 Red75								4, 		//  7 Red75
	5, 		//  8 Multiburst					5, 		//  8 Multiburst
	6, 		//  9 CCIR18							-, 		//  9 CCIR18
	7, 		// 10 Window10						6, 		// 10 Window10
	8, 		// 11 Window15						7, 		// 11 Window15
	9, 		// 12 Window20						8, 		// 12 Window20
 	10, 	// 13 Window100 					9, 		// 13 Window100
 	11, 	// 14 BlWh15kHz 					10, 	// 14 BlWh15kHz
	12, 	// 15 White100						11, 	// 15 White100
	13, 	// 16 BlackBurst					12, 	// 16 BlackBurst
 	14, 	// 17 CheckField 					13, 	// 17 CheckField
 	15, 	// 18 DigitalGrey 				14, 	// 18 DigitalGrey
 	16, 	// 19 Stair5 							15, 	// 19 Stair5
	17,		// 20 Stair10							16,		// 20 Stair10
	18, 	// 21 Crosshatch					17, 	// 21 Crosshatch
	19 		// 22 PLUGE								18 		// 22 PLUGE
*/
//---------------------------------------------------------------------------

