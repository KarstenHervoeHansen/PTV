//---------------------------------------------------------------------------
#ifndef utilH
#define utilH

extern void RestoreWindowPosition(TForm *Form, char* name);
extern void SaveWindowPosition(TForm *Form, char* name);
extern bool EnterScHPhase( TEdit *edit);
extern bool ValidateTiming( int system, char *field, int *line, long *samples);
extern void TimingAdjust( char System, long *Samples, TEdit *Edit, TTrackBar *TrackBar, int Button);
extern void TrackBarAdjust( char System, long *Samples, TTrackBar *TrackBar);
extern void SamplesToFLO( char System, long Samples, TEdit *FieldEdit, TEdit *LineEdit, TEdit *OffsetEdit);
extern void UpdateSystemLEDs( TComboBox *System, TImage *PALLED, TImage *NTSCLED);
//---------------------------------------------------------------------------
#endif
