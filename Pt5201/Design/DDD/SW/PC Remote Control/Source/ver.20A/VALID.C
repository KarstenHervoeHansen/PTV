//---------------------------------------------------------------------------
bool ValidateScHPhase(TForm *Form, TEdit *Edit, int *ScHPhase)
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


bool ValidateEntry(TForm *Form, int System, long *Samples, TEdit *Field, \
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
bool FLOToSamples( int System, long *Samples, \
        AnsiString *FieldEditText, AnsiString *LineEditText,\
        AnsiString *OffsetEditText)
  {
  char minus;
  int Field, Line;
  double Offset;
  long tmpSamples;

  Field = (*FieldEditText).ToInt();
  Line = (*LineEditText).ToInt();
  Offset = (*OffsetEditText).ToDouble();

  if (( Field == 0) && ( Line == 0) && ( Offset == 0))
    {
    *Samples = 0;
    return( true);
    }

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

  if ( System < NTSC)     // If selected system is PAL..
    {
    // Test for illegal time var
    if ( labs( tmpSamples) > 442367)
      return( false);

    switch ( Field)       // Test for illegal field & line
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
    if ( !minus)     // if posistive delay..
      {
      if ( Field % 2)
        {
        Line += 313;
        Field--;
        }
      }
    else   // ..else delay is negative
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
  else   // ..else system is NTSC
    {                                                                                          // Test for illegal time var
    if ( labs( tmpSamples) > 439295)
      return( false);
    switch ( Field)   // Test for illegal field & line
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
    if ( !minus)     // if posistive delay..
      {
      if ( Field % 2)
        {
        Line += 263;
        Field--;
        }
      }
    else                                                                            // ..else delay is negative
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
void SamplesToFLO( int System, long Samples, TEdit *FieldEdit,\
                   TEdit *LineEdit, TEdit *OffsetEdit)
  {
  char sign;
  int frame, field, line;
  char buf[20];

  sign = '+';

  if ( System < NTSC) // If system selected is PAL..
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
  else         // ..system selected is NTSC
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

void TimingAdjust(Char System, long *Samples, Char Type, Char which, Char Button)
{
	int LineTst;
	int StepSize;

  if ( Type == TSGType)
  	StepSize = 256;
	else
  	StepSize = 1;

	if (which == OFFSET)
  {
		if (Button == INCR)
			*Samples += StepSize;
		else
			*Samples -= StepSize;
	}
  else
		if (which == LINE)
  	{
			if ( System < NTSC)					// If system is PAL..
    	{
				if (Button == INCR)
					*Samples += (long) 1728*256;
      	else
					*Samples -= (long) 1728*256;
    	}
			else										// otherwise system is NTSC
    	{
				if (Button == INCR)
					*Samples += (long) 1716*256;
				else
					*Samples -= (long) 1716*256;
			}
		}
	else
		if (which == FIELD)
    {
			if ( System < NTSC)					// If system is PAL..
      {
				LineTst = ( *Samples / ((long) 1728*256)) % 625;

				if (Button == INCR)
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

				if (Button == INCR)
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
