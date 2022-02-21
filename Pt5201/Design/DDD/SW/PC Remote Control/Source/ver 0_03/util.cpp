//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#include <vcl\comctrls.hpp>
#include <registry.hpp>
#include <math.h>
#pragma hdrstop

#include "colorbar.h"
#include "mainform.h"
#include "util.h"
#include "pt52class.h"
//---------------------------------------------------------------------------
void SaveWindowPosition(TForm *Form, char* name)
{
	TRegistry* PT5201Reg;
	AnsiString RegDirectory;

	PT5201Reg = new TRegistry;				// Allocate space and assign a pointer

                                    // Set root key inside registry
  PT5201Reg->RootKey = HKEY_CURRENT_USER;

  RegDirectory ="\\Software\\PT5201Control\\MainWindow\\";
	RegDirectory += name;
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
void RestoreWindowPosition(TForm *Form, char* name)
{
	TRegistry* PT5201Reg;
	AnsiString RegDirectory;

  PT5201Reg = new TRegistry;				// Allocate space and assign a pointer

                                    // Set root key inside registry
  PT5201Reg->RootKey = HKEY_CURRENT_USER;

  RegDirectory ="\\Software\\PT5201Control\\MainWindow\\";
	RegDirectory += name;
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
bool EnterScHPhase( TEdit *edit)
{
	int tmp;

	try
  {
		tmp = edit->Text.ToInt();
  }
  catch(...)
  {
   	Application->MessageBox( "Invalid data", "Error", MB_OK|MB_ICONERROR);

		return( false);
  }

  if (( tmp > 180) || ( tmp <-179))
  {
		Application->MessageBox( "Value must be between -179 and +180", "Error", MB_OK|MB_ICONERROR);

		return( false);
	}

	return( true);
}
//---------------------------------------------------------------------------
bool ValidateTiming( int system, char *field, int *line, long *samples)
{
	bool CanUpdate = true;

	if ( system >= NTSC)
	  switch ( *field)
  	{
			case -1:
				if (( *line < -262) || ( *line > 0))
					CanUpdate = false;
  	    break;

			case 0:
				if (( *line < -261) || ( *line > 262))
					CanUpdate = false;
	      break;

			case 1:
				if (( *line < 0) || ( *line > 261))
					CanUpdate = false;
	      break;

			case 2:
				if ( *line != 0)
					CanUpdate = false;
	      break;

			default:
				CanUpdate = false;
  	}

	if ( !CanUpdate)
		if ( Application->MessageBox( "Offset is invalid in the chosen system. If you do continue, the offset will be reset!", "Warning", MB_OKCANCEL|MB_ICONWARNING) == mrOk)
   	{
	   	CanUpdate = true;

      *field = *line = *samples = 0;
    }

	return( CanUpdate);
}
//---------------------------------------------------------------------------
void TimingAdjust( char System, long *Samples, TEdit *Edit, TTrackBar *TrackBar, int Button)
{
	int LineTst, tmp;

	if ( Edit->Name == "OffsetEdit") {
		if ( Button == btNext)
			*Samples += 1;
		else
			*Samples -= 1;
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

		tmp = (int)(*Samples/((long)1728*256));

    if ( tmp > 1250)
	    TrackBar->Position = tmp - 2500;
		else
	    TrackBar->Position = tmp;
	}
	else
  {																		// otherwise system is NTSC
		if ( *Samples < 0)
			*Samples += (long) 2*525*1716*256;
		else
			*Samples %= (long) 2*525*1716*256;

		tmp = (int)(*Samples/((long)1716*256));

    if ( tmp > 525)
	    TrackBar->Position = tmp - 1050;
		else
	    TrackBar->Position = tmp;
	}
}
//---------------------------------------------------------------------------
void TrackBarAdjust( char System, long *Samples, TTrackBar *TrackBar)
{
	if ( System < NTSC)
  {
		if ( TrackBar->Position < 0)
			*Samples = 1728*256*( 2500 + TrackBar->Position);
	  else
			*Samples = 1728*256*TrackBar->Position;
  }
  else
  {
		if ( TrackBar->Position < 0)
			*Samples = 1716*256*( 1050 + TrackBar->Position);
	  else
			*Samples = 1716*256*TrackBar->Position;
  }
}
//---------------------------------------------------------------------------
void SamplesToFLO( char System, long Samples, TEdit *FieldEdit, TEdit *LineEdit, TEdit *OffsetEdit)
{
	char frame, field, sign;
	char buf[20];
	int line;

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
void UpdateSystemLEDs( TComboBox *System, TImage *PALLED, TImage *NTSCLED)
{
	if ( System->ItemIndex < NTSC)
  {
	 	PALLED->Picture = PT5201Form->LEDOnImage->Picture;
   	NTSCLED->Picture = PT5201Form->LEDOffImage->Picture;
  }
  else
  {
	 	PALLED->Picture = PT5201Form->LEDOffImage->Picture;
   	NTSCLED->Picture = PT5201Form->LEDOnImage->Picture;
	}
}
//---------------------------------------------------------------------------

