//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\comctrls.hpp>
#include <vcl\StdCtrls.hpp>
#include <registry.hpp>
#include <math.h>
#pragma hdrstop

#include "pt52class.h"
#include "mainform.h"
#include "util.h"
#include "colorbar.h"
//---------------------------------------------------------------------------
void SaveWindowPosition(TForm *Form, char* Name)
{
	TRegistry* PT5201Reg;
	AnsiString RegDirectory;

	PT5201Reg = new TRegistry;				// Allocate space and assign a pointer

                                    // Set root key inside registry
  PT5201Reg->RootKey = HKEY_CURRENT_USER;

  RegDirectory ="\\Software\\PT5201Control\\MainWindow\\";
	RegDirectory += Name;
    								// Write posistion info
  if ( PT5201Reg->OpenKey( RegDirectory, true))
  {
		try
    {
    	PT5201Reg->WriteInteger( "Top", Form->Top);
    }
    catch(...)
    {
    	;
    }

		try
    {
    	PT5201Reg->WriteInteger( "Left", Form->Left);
    }
    catch(...)
    {
    	;
    }
  }

  PT5201Reg->Free();
}
//---------------------------------------------------------------------------
void RestoreWindowPosition(TForm *Form, char* Name)
{
	TRegistry* PT5201Reg;
	AnsiString RegDirectory;

  PT5201Reg = new TRegistry;				// Allocate space and assign a pointer

                                    // Set root key inside registry
  PT5201Reg->RootKey = HKEY_CURRENT_USER;

  RegDirectory ="\\Software\\PT5201Control\\MainWindow\\";
	RegDirectory += Name;
								    								// Write posistion info
  if ( PT5201Reg->OpenKey( RegDirectory, false))
  {
		try
  	{
    	Form->Top = PT5201Reg->ReadInteger( "Top");
    }
    catch(...)
    {
    	;
    }

		try
    {
  	 	Form->Left = PT5201Reg->ReadInteger( "Left");
    }
    catch(...)
    {
     	;
    }
  }

  PT5201Reg->Free();
}
//---------------------------------------------------------------------------
void TimingAdjust( int System, long *Samples, int Type, TEdit *Edit, int Button)
{
	int LineTst, tmp;
	int StepSize;

  if ( Type == CBarType)
  	StepSize = 256;
  else
  	StepSize = 1;

	if ( Edit->Name == "OffsetEdit") {
		if ( Button == btNext)
			*Samples += StepSize;
		else
			*Samples -= StepSize;
  }
  else
		if ( Edit->Name == "LineEdit") {
			if ( System < NTSC) {						// If system is PAL..
				if ( Button == btNext)
					*Samples += (long) 1728*256;
				else
					*Samples -= (long) 1728*256;
			}
			else {													// otherwise system is NTSC
				if ( Button == btNext)
					*Samples += (long) 1716*256;
				else
					*Samples -= (long) 1716*256;
			}
		}
	else
		if ( Edit->Name == "FieldEdit") {
			if ( System < NTSC) {						// If system is PAL..
				LineTst = ( *Samples / ((long) 1728*256)) % 625;

				if ( Button == btNext)
        {
					if ( LineTst < 313)
						*Samples += (long) 313*1728*256;
					else
						*Samples += (long) 312*1728*256;
				}
				else
        {
					if ( LineTst < 313)
						*Samples -= (long) 312*1728*256;
					else
						*Samples -= (long) 313*1728*256;
				}
			}
			else {													// otherwise system is NTSC
				LineTst = ( *Samples / ((long) 1716*256)) % 525;

				if ( Button == btNext)
        {
					if ( LineTst < 263)
						*Samples += (long) 263*1716*256;
					else
						*Samples += (long) 262*1716*256;
				}
				else
        {
					if ( LineTst < 263)
						*Samples -= (long) 262*1716*256;
					else
						*Samples -= (long) 263*1716*256;
				}
      }
		}

	if ( System < NTSC) {								// If system is PAL..

		if ( *Samples < 0)
			*Samples += (long) 4*625*1728*256;
		else
			*Samples %= (long) 4*625*1728*256;
	}
	else
  {																		// otherwise system is NTSC
		if ( *Samples < 0)
			*Samples += (long) 2*525*1716*256;
		else
			*Samples %= (long) 2*525*1716*256;
	}
}
//---------------------------------------------------------------------------
void TrackBarAdjust( int System, long *Samples, TTrackBar *TrackBar)
{
	if ( System < NTSC)
  {
		if ( TrackBar->Position < 0)
			*Samples = 1728*256*( 4*625 + TrackBar->Position);
	  else
			*Samples = 1728*256*TrackBar->Position;
  }
  else
  {
		if ( TrackBar->Position < 0)
			*Samples = 1716*256*( 2*525 + TrackBar->Position);
	  else
			*Samples = 1716*256*TrackBar->Position;
  }
}
//---------------------------------------------------------------------------
void ScHPhaseAdjust( int *ScHPhase, TEdit *Edit, int Button)
{
	if ( Button == btNext)
		*ScHPhase += 1;
	else
		*ScHPhase -= 1;

	if ( *ScHPhase > 180)
		*ScHPhase = 180;

	if ( *ScHPhase < -179)
		*ScHPhase = -179;

	Edit->Text = IntToStr( *ScHPhase);
  Edit->SelectAll();
}
//---------------------------------------------------------------------------
void TrackBarUpdate( int System, long Samples, TTrackBar *TrackBar)
{
	int tmp;

	if ( System < NTSC) {								// If system is PAL..

		tmp = (int)(Samples/((long)1728*256));

    if ( tmp > 1250)
	    TrackBar->Position = tmp - 2500;
		else
	    TrackBar->Position = tmp;
	}
	else
  {																		// otherwise system is NTSC
		tmp = (int)(Samples/((long)1716*256));

    if ( tmp > 525)
	    TrackBar->Position = tmp - 1050;
		else
	    TrackBar->Position = tmp;
	}
}
//---------------------------------------------------------------------------
void TrackBarSetup( int System, TTrackBar *TrackBar, TLabel *MinLabel, \
	TLabel *MaxLabel)
{
	if ( System < NTSC)
  {
		TrackBar->Max = 1250;
		TrackBar->Min = -1249;
		TrackBar->Frequency = 40;

		MinLabel->Caption = "-3";
		MinLabel->Left = 42;

		MaxLabel->Caption = "+4";
  }
  else
  {
		TrackBar->Max = 525;
		TrackBar->Min = -524;
		TrackBar->Frequency = 20;

		MinLabel->Caption = "-1";
		MinLabel->Left = 72;

		MaxLabel->Caption = "+2";
  }
}
//---------------------------------------------------------------------------
bool ValidatePALToNTSC( int System, long *Samples, TTrackBar *TrackBar)
{
	bool CanUpdate = true;

	if ( System >= NTSC)
		if ( *Samples > 2*525*1716*256)
			CanUpdate = false;

	if ( !CanUpdate)
		if ( Application->MessageBox( "The Offset is not valid in NTSC. If you continue, the offset will be reset!", "Warning", MB_OKCANCEL|MB_ICONWARNING) == mrOk)
   	{
	   	CanUpdate = true;

			*Samples = TrackBar->Position = 0;
    }

	return( CanUpdate);
}
//---------------------------------------------------------------------------
bool ValidateEntry( TForm *Form, int System, long *Samples, TEdit *Field, \
	TEdit *Line, TEdit *Offset)
{
  double tmp;
	bool CanUpdate = true;

	try
  {
		if ( Field->Tag)
			tmp = Field->Text.ToDouble();
    else
			if ( Line->Tag)
				tmp = Line->Text.ToDouble();
    else
			if ( Offset->Tag)
				tmp = Offset->Text.ToDouble();
  }
 	catch(...)
  {
 	 	Application->MessageBox( "Invalid data", "Error", MB_OK|MB_ICONERROR);

		CanUpdate = false;
  }

  if ( CanUpdate)
	{
		CanUpdate = FLOToSamples( System, Samples, Field, Line, Offset);

		if ( !CanUpdate)
			Application->MessageBox( "The entered value is not valid.", "Error", MB_OK|MB_ICONERROR);
	}

	SamplesToFLO( System, *Samples, Field, Line, Offset);

	if ( !CanUpdate)
  {
		if ( Field->Tag)
  	{
			Form->ActiveControl = Field;
  		Field->SelectAll();
    }
    else
			if ( Line->Tag)
  		{
				Form->ActiveControl = Line;
  			Line->SelectAll();
	    }
    else
			if ( Offset->Tag)
  		{
				Form->ActiveControl = Offset;
  			Offset->SelectAll();
	    }
  }

  Field->Tag = false;
  Line->Tag = false;
  Offset->Tag = false;

	return( CanUpdate);
}
//---------------------------------------------------------------------------
bool ValidateScHPhase( TForm *Form, TEdit *Edit, int *ScHPhase)
{
	int tmp;
	bool CanUpdate = true;

	try
  {
		tmp = Edit->Text.ToInt();
  }
  catch(...)
  {
   	Application->MessageBox( "Invalid data", "Error", MB_OK|MB_ICONERROR);

		CanUpdate = false;
  }

  if ((( tmp > 180) || ( tmp < -179)) && CanUpdate)
  {
		Application->MessageBox( "Value must be between -179 and +180", "Error", MB_OK|MB_ICONERROR);

		CanUpdate = false;
	}

	if ( CanUpdate)
		*ScHPhase = tmp;
  else{
		Form->ActiveControl = Edit;

		Edit->Text = IntToStr( *ScHPhase);
    Edit->SelectAll();
  }

	return( CanUpdate);
}
//---------------------------------------------------------------------------
bool FLOToSamples( int System, long *Samples, \
	TEdit *FieldEdit, TEdit *LineEdit, TEdit *OffsetEdit)
{
	char minus;
	int Field, Line;
  double Offset;
  long tmpSamples;

  Field = FieldEdit->Text.ToInt();
  Line = LineEdit->Text.ToInt();
  Offset = OffsetEdit->Text.ToDouble();

	if (( Field == 0) && ( Line == 0) && ( Offset == 0))
		return( true);

	if (( Field <= 0) && ( Line <= 0) && ( Offset <= 0)) {
		minus = 1;
	}
	else {
		if (( Field >= 0) && ( Line >= 0) && ( Offset >= 0))
			minus = 0;
		else
			return( false);
	}
																// 1 sample = (1/27MHz)/256
	tmpSamples = ( long)(( Offset*6912)/1000 + 0.5);

	if ( System < NTSC) {					// If selected system is PAL..

											    			// Test for illegal time var
		if ( labs( tmpSamples) > 442367)
			return( false);

		switch ( Field) {						// Test for illegal field & line
			case 0:
				if (( minus && ( Line < -311)) || ( !minus && ( Line > 312)))
					return( false);
				break;

			case -1:
			case -3:
			case 2:
				if ( abs( Line) > 312)
					return( false);
				break;

			case -2:
			case 1:
			case 3:
				if ( abs( Line) > 311)
					return( false);
				break;

			case 4:
				if (( Line > 0) || ( Offset > 0.0))
					return( false);
				break;

			default:
				return( false);
		}

		if ( !minus) {							// if posistive delay..
			if ( Field % 2) {
				Line += 313;
				Field--;
			}
		}
		else {											// ..else delay is negative
			Field += 7;

			tmpSamples += 442367;

			if ( Field % 2) {
				Line += 624;
				Field--;
			}
			else
				Line += 312;

			if ( Offset == 0)
				tmpSamples++;
		}

		tmpSamples += Field*138240000 + Line*442368;
	}
	else {												// ..else system is NTSC

											    			// Test for illegal time var
		if ( labs( tmpSamples) > 439295)
			return( false);

		switch ( Field) {						// Test for illegal field & line
			case 0:
				if (( minus && ( Line < -261)) || ( !minus && ( Line > 262)))
					return( false);
				break;

			case -1:
				if ( abs( Line) > 262)
					return( false);
				break;

			case 1:
				if ( abs( Line) > 261)
					return( false);
				break;

			case 2:
				if (( Line > 0) || ( Offset > 0.0))
					return( false);
				break;

			default:
				return( false);
		}

		if ( !minus) {							// if posistive delay..

			if ( Field % 2) {
				Line += 263;
				Field--;
			}
		}
		else {											// ..else delay is negative
			Field += 3;

			tmpSamples += 439295;

			if ( Field % 2) {
				Line += 524;
				Field--;
			}
			else
				Line += 262;

			if ( Offset == 0)
				tmpSamples++;
		}

		tmpSamples += Field*115315200 + Line*439296;
	}

	*Samples = tmpSamples;

	return( true);
}
//---------------------------------------------------------------------------
void SamplesToFLO( int System, long Samples, TEdit *FieldEdit, TEdit *LineEdit, \
	TEdit *OffsetEdit)
{
	char sign;
  int frame, field, line;
	char buf[20];

	sign = '+';

	if ( System < NTSC) {								// If system selected is PAL..

		if ( Samples >= ((long) 2*625*1728*256)) {

			Samples--;

			frame = Samples / ((long) 625*1728*256);
			line = ( Samples % ((long) 625*1728*256)) / ((long) 1728*256);
			Samples = ( Samples % ((long) 625*1728*256)) % ((long) 1728*256);

			field = 2*frame;

			if ( line > 312) {
				field++;
				line = 624-line;
			}
			else
				line = 312-line;

			Samples = ((long) 1728*256) - 1 - Samples;
			field = 7-field;

			if ( field != 4)
				sign = '-';
		}
		else {
			frame = Samples / ((long) 625*1728*256);
			line = ( Samples % ((long) 625*1728*256)) / ((long) 1728*256);
			Samples = ( Samples % ((long) 625*1728*256)) % ((long) 1728*256);
			field = 2*frame;

			if ( line > 312) {
				field++;
				line -= 313;
			}
		}
	}
	else {                  							// ..system selected is NTSC
		if ( Samples >=  ((long) 525*1716*256)) {

			Samples--;

			frame = Samples / ((long) 525*1716*256);
			line = ( Samples % ((long) 525*1716*256)) / ((long) 1716*256);
			Samples = ( Samples % ((long) 525*1716*256)) % ((long) 1716*256);

			field = 2*frame;

			if ( line > 262) {
				field++;
				line = 524-line;
			}
			else
				line = 262-line;

			Samples = ((long) 1716*256) - 1 - Samples;
			field = 3 - field;

			if ( field != 2)
				sign = '-';
		}
		else {
			frame = Samples / ((long) 525*1716*256);
			line = ( Samples % ((long) 525*1716*256)) / ((long) 1716*256);
			Samples = ( Samples % ((long) 525*1716*256)) % ((long) 1716*256);
			field = 2*frame;

			if ( line > 262) {
				field++;
				line -= 263;
			}
		}
	}

	sprintf( buf, "%c%1d", sign, abs( field));
	FieldEdit->Text = buf;

	sprintf( buf, "%c%03d", sign, abs( line));
	LineEdit->Text = buf;

	sprintf( buf, "%c%07.1f", sign, fabs((float) ( Samples*1000)/6912));
	OffsetEdit->Text = buf;
}
//---------------------------------------------------------------------------

