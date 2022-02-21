//---------------------------------------------------------------------------
#ifndef utilH
#define utilH

extern void RestoreWindowPosition(TForm *Form, char* Name);
extern void SaveWindowPosition(TForm *Form, char* Name);

extern void TimingAdjust( int System, long *Samples, int Type, TEdit *Edit, int Button);
extern void TrackBarAdjust( int System, long *Samples, TTrackBar *TrackBar);
extern void ScHPhaseAdjust( int *ScHPhase, TEdit *Edit, int Button);

extern void TrackBarUpdate( int System, long Samples, TTrackBar *TrackBar);
extern void TrackBarSetup( int System, TTrackBar *TrackBar, TLabel *MinLabel, TLabel *MaxLabel);

extern bool ValidatePALToNTSC( int System, long *Samples, TTrackBar *TrackBar);
extern bool ValidateEntry( TForm *Form, int System, long *Samples, TEdit *Field, \
	TEdit *Line, TEdit *Offset);
extern bool	ValidateScHPhase( TForm *Form, TEdit *Edit, int *ScHPhase);

extern bool FLOToSamples( int System, long* Samples, TEdit *FieldEdit, TEdit *LineEdit, TEdit *OffsetEdit);
extern void SamplesToFLO( int System, long Samples, TEdit *FieldEdit, TEdit *LineEdit, TEdit *OffsetEdit);

extern int SendRequest( char *command, int bytes, char* result);

extern int SendGenlockSystem( int system);
extern int SendTVSystem( char *command, int system);
extern int SendDelay( char *command, int field, int line, float offset);
extern int SendScHPhase( char *command, int schphase);

extern int SendCBarPattern( int pattern);
extern int SendCBarEmbAudioSignal( int embaudio);

extern int SendAudioOutput( int output);
extern int SendAudioSystem( int system);
extern int SendAudioSignal( int signal);
extern int SendAudioLevel( int level);
extern int SendAudioWordClk( int freq);
extern int SendAudioClick( int click);

//---------------------------------------------------------------------------
#endif
