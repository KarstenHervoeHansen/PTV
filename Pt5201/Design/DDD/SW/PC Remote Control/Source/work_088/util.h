//---------------------------------------------------------------------------
#ifndef utilH
#define utilH

extern void SaveCommPort( void);
extern char *RestoreCommPort( void);

extern void SaveWindowPosition(TForm *Form, char* Name);
extern void RestoreWindowPosition(TForm *Form, char* Name);

extern void TimingAdjust( int System, long *Samples, int Type, TEdit *Edit, int Button);
extern void TrackBarAdjust( int System, long *Samples, TTrackBar *TrackBar);
extern void ScHPhaseAdjust( int *ScHPhase, TEdit *Edit, int Button);

extern void TrackBarUpdate( int System, long Samples, TTrackBar *TrackBar);
extern void TrackBarSetup( int System, TTrackBar *TrackBar, TLabel *MinLabel, TLabel *MaxLabel);

extern bool ValidatePALToNTSC( int System, long *Samples, TTrackBar *TrackBar);
extern bool ValidateEntry( TForm *Form, int System, long *Samples, TEdit *Field, \
											TEdit *Line, TEdit *Offset);
extern bool	ValidateScHPhase( TForm *Form, TEdit *Edit, int *ScHPhase);

extern bool FLOToSamples( int System, long *Samples, \
							AnsiString *FieldEditText, AnsiString *LineEditText, AnsiString *OffsetEditText);
extern void SamplesToFLO( int System, long Samples, TEdit *FieldEdit, TEdit *LineEdit, TEdit *OffsetEdit);

extern int TrxRequest( char *command, unsigned int bytes, BYTE* result);

extern int TrxGenlockSystem( int system);
extern int TrxGenlockDelay( int field, int line, float offset);

extern int TrxBBSystem( int output, int system);
extern int TrxBBDelay( int output, int field, int line, float offset);
extern int TrxBBScHPhase( int output, int schphase);

extern int TrxTSGSystem( int system);
extern int TrxTSGDelay( int field, int line, float offset);
extern int TrxTSGScHPhase( int schphase);
extern int TrxTSGPattern( int pattern);
extern int TrxTSGEmbAudioSignal( int embaudio);

extern int TrxAudioOutput( int output);
extern int TrxAudioSystem( int system);
extern int TrxAudioSignal( int output, int signal);
extern int TrxAudioLevel( int output, int level);
extern int TrxAudioWordClk( int freq);
extern int TrxAudioClick( int output, int click);
extern int TrxAudioTiming( int timting);

extern int RSTCommand( void);
extern int FactSystemCommand( int system);

extern int TrxApparatusInfoRequest( void);
extern int TrxGenlockInfoRequest( TGenlockGenerator *setup);
extern int TrxBBInfoRequest( int output, TBlackGenerator *setup);
extern int TrxTSGInfoRequest( TTSGenerator *setup);
extern int TrxAudioInfoRequest( TAudioGenerator *setup);

//---------------------------------------------------------------------------
#endif
