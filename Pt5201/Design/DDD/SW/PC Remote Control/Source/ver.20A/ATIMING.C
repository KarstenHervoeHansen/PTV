//---------------------------------------------------------------------------
bool ValidateTiming( TForm *Form, TEdit *Edit, int *Timing)
  {
  double TextValue;
  int Tmp;
  bool CanUpdate = true;

  try
    {
    TextValue = Edit->Text.ToDouble();
    }
  catch(...)
    {
    Application->MessageBox( "Invalid data", "Error", MB_OK|MB_ICONERROR);
    CanUpdate = false;
    TextValue = 0;
    }

  if ((( TextValue > (float)MaxAESTiming/10.) || ( TextValue < (float)MinAESTiming/10.)) && CanUpdate)
    {
    char *buffer = new char[50];
    sprintf( buffer, "Value must be between %4.1f and %4.1f\0", (float)MinAESTiming/10., (float)MaxAESTiming/10.);
    Application->MessageBox( buffer, "Error", MB_OK|MB_ICONERROR);
    CanUpdate = false;
    delete [] buffer;
    }

  if ( CanUpdate)
    {
    Tmp = (int)( 10.0001*TextValue);;
    *Timing = 8*( Tmp/8);

  if (( abs( Tmp) % 8) >= 4)
    {
    if ( Tmp >= 0)
      *Timing += 8;
    else
      *Timing -= 8;
    }
  Edit->Text = FloatToStrF(((float)*Timing)/10., ffFixed, 5, 1);
    }
  else
    {
    Form->ActiveControl = Edit;
    Edit->Text = FloatToStrF(((float)*Timing)/10., ffFixed, 5, 1);
    Edit->SelectAll();
    }
  return( CanUpdate);
  }
