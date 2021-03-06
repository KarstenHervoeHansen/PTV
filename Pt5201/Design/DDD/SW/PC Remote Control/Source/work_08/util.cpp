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
#include "tsg.h"
#include "comm.h"
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

  	PT5201Reg = new TRegistry;			// Allocate space and assign a pointer

                                    // Set root key inside registry
  	PT5201Reg->RootKey = HKEY_CURRENT_USER;

  	RegDirectory ="\\Software\\PT5201Control\\MainWindow\\";
	RegDirectory += Name;
								    								// Write position info
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
	int LineTst;
	int StepSize;

  	if ( Type == TSGType)
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
		if ( Edit->Name == "LineEdit")
      {
			if ( System < NTSC)					// If system is PAL..
         {
				if ( Button == btNext)
					*Samples += (long) 1728*256;
				else
					*Samples -= (long) 1728*256;
			}
			else										// otherwise system is NTSC
         {
				if ( Button == btNext)
					*Samples += (long) 1716*256;
				else
					*Samples -= (long) 1716*256;
			}
		}
   else
		if ( Edit->Name == "FieldEdit")
      {
			if ( System < NTSC)					// If system is PAL..
         {
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
			else										// otherwise system is NTSC
         {
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

		if ( System < NTSC)						// If system is PAL..
      {
			if ( *Samples < 0)
				*Samples += (long) 4*625*1728*256;
			else
				*Samples %= (long) 4*625*1728*256;
		}
		else											// otherwise system is NTSC
  		{
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

	if ( System < NTSC)							// If system is PAL..
	{
		tmp = (int)(Samples/((long)1728*256));

		if ( tmp > 1250)
	   	TrackBar->Position = tmp - 2500;
		else
	    TrackBar->Position = tmp;
	}
	else
  	{													// otherwise system is NTSC
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
								// Thew following assignment of tmp is only used for
                        //  catching any illegal input
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
		CanUpdate = FLOToSamples( System, Samples, &(Field->Text), &(Line->Text), &(Offset->Text));

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
  	else
   {
		Form->ActiveControl = Edit;

		Edit->Text = IntToStr( *ScHPhase);
   	Edit->SelectAll();
  	}

	return( CanUpdate);
}
//---------------------------------------------------------------------------
bool FLOToSamples( int System, long *Samples, \
	AnsiString *FieldEditText, AnsiString *LineEditText, AnsiString *OffsetEditText)
{
	char minus;
	int Field, Line;
  	double Offset;
  	long tmpSamples;

  	Field = (*FieldEditText).ToInt();
  	Line = (*LineEditText).ToInt();
  	Offset = (*OffsetEditText).ToDouble();

	if (( Field == 0) && ( Line == 0) && ( Offset == 0))
		return( true);

	if (( Field <= 0) && ( Line <= 0) && ( Offset <= 0))
   {
		minus = 1;
	}
	else
   {
		if (( Field >= 0) && ( Line >= 0) && ( Offset >= 0))
			minus = 0;
		else
			return( false);
	}
													// 1 sample = (1/27MHz)/256
	tmpSamples = ( long)(( Offset*6912)/1000 + 0.5);

	if ( System < NTSC)   					// If selected system is PAL..
	{
									    			// Test for illegal time var
		if ( labs( tmpSamples) > 442367)
			return( false);

		switch ( Field) 						// Test for illegal field & line
		{
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

		if ( !minus) 							// if posistive delay..
		{
			if ( Field % 2)
         {
				Line += 313;
				Field--;
			}
		}
		else 									 	// ..else delay is negative
		{
			Field += 7;

			tmpSamples += 442367;

			if ( Field % 2)
         {
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
	else											// ..else system is NTSC
	{											   // Test for illegal time var
		if ( labs( tmpSamples) > 439295)
			return( false);

		switch ( Field) 						// Test for illegal field & line
		{
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

		if ( !minus) 							// if posistive delay..
		{
			if ( Field % 2)
         {
				Line += 263;
				Field--;
			}
		}
		else					  					// ..else delay is negative
		{
			Field += 3;

			tmpSamples += 439295;

			if ( Field % 2)
         {
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

	if ( System < NTSC) 							// If system selected is PAL..
   {
		if ( Samples >= ((long) 2*625*1728*256))
      {
			Samples--;

			frame = Samples / ((long) 625*1728*256);
			line = ( Samples % ((long) 625*1728*256)) / ((long) 1728*256);
			Samples = ( Samples % ((long) 625*1728*256)) % ((long) 1728*256);

			field = 2*frame;

			if ( line > 312)
         {
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
		else
      {
			frame = Samples / ((long) 625*1728*256);
			line = ( Samples % ((long) 625*1728*256)) / ((long) 1728*256);
			Samples = ( Samples % ((long) 625*1728*256)) % ((long) 1728*256);
			field = 2*frame;

			if ( line > 312)
         {
				field++;
				line -= 313;
			}
		}
	}
	else                  						// ..system selected is NTSC
   {
		if ( Samples >=  ((long) 525*1716*256))
      {
      	Samples--;

			frame = Samples / ((long) 525*1716*256);
			line = ( Samples % ((long) 525*1716*256)) / ((long) 1716*256);
			Samples = ( Samples % ((long) 525*1716*256)) % ((long) 1716*256);

			field = 2*frame;

			if ( line > 262)
         {
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
		else
      {
			frame = Samples / ((long) 525*1716*256);
			line = ( Samples % ((long) 525*1716*256)) / ((long) 1716*256);
			Samples = ( Samples % ((long) 525*1716*256)) % ((long) 1716*256);
			field = 2*frame;

			if ( line > 262)
         {
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
//
//---------------------------------------------------------------------------
int TrxRequest( char *command, unsigned int bytes, BYTE* result)
{
	PT5201CommPort->FlushCommPort();

	PT5201CommPort->WriteString( command);

	PT5201CommPort->ReadBytes( result, bytes);

	result[strlen( result)-1] = 0;				// Clear newline character

	return( strlen( result));
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxGenlockSystem( int system)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

	strcpy( buffer, "INP:GENL:SYST ");
	strcat( buffer, GenlockSystemCmdTxt[system]);
 	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxGenlockDelay( int field, int line, float offset)
{
	if ( !PT5201CommPort->Connected())
	 	return( -1);

  	char *buffer = new char[50];
	char *buffer1 = new char[30];

  	strcpy( buffer, "INP:GENL:DEL ");

	sprintf( buffer1, "%d,%d,%f\n", field, line, offset);
  	strcat( buffer, buffer1);

	PT5201CommPort->WriteString( buffer);

	delete [] buffer,
	delete [] buffer1;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxBBSystem( int output, int system)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:BB1:SYST ");
	buffer[7] += output - Black1Output;

  	strcat( buffer, BBSystemCmdTxt[system]);
 	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxBBDelay( int output, int field, int line, float offset)
{
	if ( !PT5201CommPort->Connected())
	 	return( -1);

  	char *buffer = new char[50];
	char *buffer1 = new char[30];

  	strcpy( buffer, "OUTP:BB1:DEL ");
	buffer[7] += output - Black1Output;

	sprintf( buffer1, "%d,%d,%f\n", field, line, offset);
  	strcat( buffer, buffer1);

	PT5201CommPort->WriteString( buffer);

	delete [] buffer,
	delete [] buffer1;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxBBScHPhase( int output, int schphase)
{
	if ( !PT5201CommPort->Connected())
	 	return( -1);

  	char *buffer = new char[50];
  	char *buffer1 = new char[15];

  	strcpy( buffer, "OUTP:BB1:SCHP ");
	buffer[7] += output - Black1Output;

	sprintf( buffer1, "%d\n", schphase);
  	strcat( buffer, buffer1);

	PT5201CommPort->WriteString( buffer);

	delete [] buffer,
	delete [] buffer1;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxTSGSystem( int system)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:TSG:SYST ");
  	strcat( buffer, TSGSystemCmdTxt[system]);
	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxTSGDelay( int field, int line, float offset)
{
	if ( !PT5201CommPort->Connected())
	 	return( -1);

  	char *buffer = new char[50];
	char *buffer1 = new char[30];

  	strcpy( buffer, "OUTP:TSG:DEL ");
	sprintf( buffer1, "%d,%d,%f\n", field, line, offset);
  	strcat( buffer, buffer1);

	PT5201CommPort->WriteString( buffer);

	delete [] buffer,
	delete [] buffer1;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxTSGScHPhase( int schphase)
{
	if ( !PT5201CommPort->Connected())
	 	return( -1);

  	char *buffer = new char[50];
  	char *buffer1 = new char[15];

  	strcpy( buffer, "OUTP:TSG:SCHP ");
	sprintf( buffer1, "%d\n", schphase);
  	strcat( buffer, buffer1);

	PT5201CommPort->WriteString( buffer);

	delete [] buffer,
	delete [] buffer1;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxTSGPattern( int pattern)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:TSG:PATT ");
  	strcat( buffer, TSGPatternCmdTxt[pattern]);
	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxTSGEmbAudioSignal( int embaudio)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:TSG:EMB:SIGN ");
	strcat( buffer, EmbAudioToneCmdTxt[embaudio]);
  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioOutput( int output)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:AUD:OUTP ");
  	strcat( buffer, AudioOutputCmdTxt[output]);
  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioSystem( int system)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:AUD:AES:SYST ");
  	strcat( buffer, AudioSystemCmdTxt[system]);
  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioSignal( int output, int signal)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:AUD:");

 	if ( output == AnalogAudio)
		strcat( buffer, "ANAL:SIGN ");
 	else
  		strcat( buffer, "AES:SIGN ");

 	strcat( buffer, AudioToneCmdTxt[signal]);
  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioLevel( int output, int level)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:AUD:");

	if ( output == AnalogAudio)
  	{
	  	strcat( buffer, "ANAL:LEV ");
  		strcat( buffer, AnalogLevelTxt[level]);
  	}
  	else
  	{
		strcat( buffer, "AES:LEV ");
  		strcat( buffer, AESEBULevelTxt[level]);
  	}
  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioWordClk( int freq)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:AUD:AES:WORD ");

  	strcat( buffer, AudioWordCmdTxt[freq]);
  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioClick( int output, int click)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "OUTP:AUD:");

 	if ( output == AnalogAudio)
		strcat( buffer, "ANAL:CLIC ");
 	else
  		strcat( buffer, "AES:CLIC ");

  	strcat( buffer, AudioClickTxt[click]);
 	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxAudioTiming( int timing)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];
  	char *buffer1 = new char[20];

  	strcpy( buffer, "OUTP:AUD:AES:TIM ");
	sprintf( buffer1, "%d\n", timing);
  	strcat( buffer, buffer1);

  	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int RSTCommand( void)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

	PT5201CommPort->WriteString( "*RST\n");

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int FactSystemCommand( int system)
{
	if ( !PT5201CommPort->Connected())
  		return( -1);

  	char *buffer = new char[50];

  	strcpy( buffer, "FACT:MAIN:SYST ");
  	strcat( buffer, ResetSystemCmdTxt[system]);
 	strcat( buffer, "\n");

	PT5201CommPort->WriteString( buffer);

	delete [] buffer;

 	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int LookupCommand( const char *buffer, const char** CmdTxt)
{
	const char *ptr;
	register int i = 0;

	while ( ptr = CmdTxt[i])
  	{
  		if ( !strcmp( ptr, buffer))
    		break;
		i++;
  	}
	if ( ptr == NULL)
   	return( -1);

	return( i);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int LookupDelay( int system , long *samples, char *buffer)
{
	char *ptr1, *ptr2;

	char *LineTxt = new char[10];
	char *FieldTxt = new char[10];
	char *OffsetTxt = new char[10];

   ptr1 = ptr2 = strchr( buffer, ',');
	*ptr1 = 0;
	strcpy( FieldTxt, buffer);
	ptr2 = strchr( ++ptr1, ',');
	*ptr2 = 0;
	strcpy( LineTxt, ptr1);
	strcpy( OffsetTxt, ++ptr2);

	FLOToSamples( system, samples, &(AnsiString)FieldTxt, &(AnsiString)LineTxt, &(AnsiString)OffsetTxt);

	delete [] FieldTxt;
	delete [] LineTxt;
	delete [] OffsetTxt;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxApparatusInfoRequest( void)
{
	PT5201Form->Timer1->Enabled = false;

   TGenlockGenerator *GenlockSetup = new TGenlockGenerator ;
	TrxGenlockInfoRequest( GenlockSetup);
	PT5201Setup->SetGenlockSettings( GenlockSetup);
	delete GenlockSetup;

	TBlackGenerator *BlackSetup = new TBlackGenerator;
	for ( int i = Black1Output; i <= Black3Output; i++)
   {
		TrxBBInfoRequest( i, BlackSetup);
		PT5201Setup->SetBlackSettings( i, BlackSetup);
   }
	delete BlackSetup;

	TTSGenerator *TSGSetup = new TTSGenerator;
	TrxTSGInfoRequest( TSGSetup);
	PT5201Setup->SetTSGSettings( TSGSetup);
	delete TSGSetup;

	TAudioGenerator *AudioSetup = new TAudioGenerator;
	TrxAudioInfoRequest( AudioSetup);
	PT5201Setup->SetAudioSettings( AudioSetup);
	delete AudioSetup;

	// Update LEDs and system status
	PT5201Form->SystemStatusUpdate();

	PT5201Form->Timer1->Enabled = true;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxGenlockInfoRequest( TGenlockGenerator *setup)
{
  	char *buffer = new char[50];
	int i;

	// Get genlock system setting
	TrxRequest( "INP:GENL:SYST?\n", 40, buffer);
	if (( i = LookupCommand( buffer, GenlockSystemCmdTxt)) >= 0)
		setup->System = i;

	// Remain in GENL subsystem and get delay
	if (( setup->System == PALGenlock) || ( setup->System == NTSCGenlock))
   {
		TrxRequest( "INP:GENL:DEL?\n", 40, buffer);

		if ( setup->System == PALGenlock)
			LookupDelay( PAL, &(setup->Samples), buffer);
		else
			LookupDelay( NTSC, &(setup->Samples), buffer);
	}

	delete [] buffer;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxBBInfoRequest( int output, TBlackGenerator *setup)
{
  	char *buffer = new char[50];
	char *buffer1 = new char[20];

	int i;

	// Get BB system setting
	strcpy( buffer1, "OUTP:BB1:SYST?;");
	buffer1[7] += output -Black1Output;

	TrxRequest( buffer1, 40, buffer);
	if (( i = LookupCommand( buffer, BBSystemCmdTxt)) >= 0)
		setup->System = i;

	// Remain in BB subsystem and get delay
	TrxRequest( "DEL?;", 40, buffer);
	LookupDelay( setup->System , &(setup->Samples), buffer);

	// Remain in BB subsystem and get ScH-Phase
	TrxRequest( "SCHP?\n", 40, buffer);
	setup->ScHPhase = atoi( buffer);

	delete [] buffer;

	return( 0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int TrxTSGInfoRequest( TTSGenerator *setup)
{
  	char *buffer = new char[50];
	int i;

	// Get TSG system setting
	TrxRequest( "OUTP:TSG:SYST?;", 40, buffer);
	if (( i = LookupCommand( buffer, TSGSystemCmdTxt)) >= 0)
		setup->System = i;

	// Remain in TSG subsystem and get pattern
	TrxRequest( "PATT?;", 40, buffer);
	if (( i = LookupCommand( buffer, TSGPatternCmdTxt)) >= 0)
		setup->Pattern = i;

	// Remain in TSG subsystem and get delay
	TrxRequest( "DEL?;", 40, buffer);
	LookupDelay( setup->System , &(setup->Samples), buffer);

	// Remain in TSG subsystem and get ScH-Phase
	TrxRequest( "SCHP?;", 40, buffer);
	setup->ScHPhase = atoi( buffer);

	// Remain in TSG subsystem and get Embedded Audio Signal
	TrxRequest( "EMB:SIGN?\n", 40, buffer);
	if (( i = LookupCommand( buffer, EmbAudioToneCmdTxt)) >= 0)
		setup->EmbAudio = i;

	delete [] buffer;

	return( 0);
}
//---------------------------------------------------------------------------
int TrxAudioInfoRequest( TAudioGenerator *setup)
{
  	char *buffer = new char[50];
	const char *ptr;
	int i;

	// Get AUDIO output setting
	TrxRequest( "OUTP:AUDIO?\n", 40, buffer);
	if (( i = LookupCommand( buffer, AudioOutputCmdTxt)) >= 0)
		setup->Output = i;


	// Get ANALOG AUDIO signal
	TrxRequest( "OUTP:AUDIO:ANAL:SIGN?;", 40, buffer);
	if (( i = LookupCommand( buffer, AudioToneCmdTxt)) >= 0)
		setup->Tone[AnalogAudio] = i;

	// Remain in ANALOG AUDIO subsystem and get level
	TrxRequest( "LEV?;", 40, buffer);
	setup->Level[AnalogAudio] = 0;			//TBD

	// Remain in ANALOG AUDIO subsystem and get click
	TrxRequest( "CLIC?\n", 40, buffer);
	setup->ClickPeriod[AnalogAudio] = atoi( buffer);



	// Get AESEBU AUDIO signal
	TrxRequest( "OUTP:AUDIO:AES:SIGN?;", 40, buffer);
	if (( i = LookupCommand( buffer, AudioToneCmdTxt)) >= 0)
		setup->Tone[AESEBUAudio] = i;

	// Remain in AESEBU AUDIO subsystem and get system
	TrxRequest( "SYST?;", 40, buffer);
	if (( i = LookupCommand( buffer, AudioSystemCmdTxt)) >= 0)
		setup->System = i;

	// Remain in AESEBU AUDIO subsystem and get level
	TrxRequest( "LEV?;", 40, buffer);
	setup->Level[AESEBUAudio] = 0;  //TBD

	// Remain in AESEBU AUDIO subsystem and get timing
	TrxRequest( "TIM?;", 40, buffer);
	setup->Timing = atoi( buffer);

	// Remain in AESEBU AUDIO subsystem and get wordclock
	TrxRequest( "WORD?;", 40, buffer);
	if (( i = LookupCommand( buffer, AudioWordCmdTxt)) >= 0)
		setup->WordClock = i;

	// Remain in AESEBU AUDIO subsystem and get click
	TrxRequest( "CLIC?\n", 40, buffer);
	setup->ClickPeriod[AESEBUAudio] = atoi( buffer);

	return( 0);
}
//---------------------------------------------------------------------------


