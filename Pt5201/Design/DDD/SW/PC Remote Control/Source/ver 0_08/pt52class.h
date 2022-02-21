//---------------------------------------------------------------------------
#ifndef pt52classH
#define pt52classH
//---------------------------------------------------------------------------
// The enumerated types MUST match the text arrays defined in pt52class.cpp
enum BlackOutputs  			{ Black1Output = 0, Black2Output, Black3Output };
enum GeneratorType			{ GenlockType = 0, BlackType, CBarType, AudioType };
enum GenlockSystem			{ InternalGenlock=0, PALGenlock, NTSCGenlock, f10MHzGenlock};
enum TVSystemType				{ PAL=0, PAL_ID, NTSC};
enum EmbAudioType				{ EmbAudioOff = 0, EmbAudioS1kHz };
enum AudioOutputType 		{ AnalogAudio = 0, AESEBUAudio };
enum AudioSystemType 		{ AudioPAL = 0, AudioNTSC };
enum AudioToneType			{ S500Hz = 0, S1kHz, SEBU1kHz, S8kHz };
enum AudioClickType			{ click1s = 0, click3s };
enum AudioFreqType			{ f441kHz = 0, f48kHz };
enum AESEBULevelType		{ M0dBFS = 0, M9dBFS, M12dBFS, M15dBFS, M16dBFS,\
													M18dBFS, M21dBFS, AESEBUSilence};
enum AnalogLevelType		{ P6dBm = 0, P4dBm, P2dBm, M0dBm, M2dBm, M4dBm,\
													M6dBm, M8dBm, M10dBm, M12dBm, M14dBm, M16dBm,\
													M18dBm, M20dBm, M22dBm, M24dBm, M26dBm, M28dBm,\
													M30dBm, M32dBm, M34dBm, M36dBm, M38dBm, M40dBm,\
													M42dBm, M44dBm, M46dBm, M48dBm, M50dBm, M52dBm,\
													M54dBm, AnalogSilence};

enum CBarPatterns				{	SMPTECBar = 0, EBUCBar,	FCCCBar, EBUCBarITU, CBar100,\
													CBarGrey75, CBarRed75, Red75, Multiburst, Window10,\
                          Window15, Window20, Window100, BlWh15kHz,\
                          White100, BlackBurst, CheckField, DigitalGrey,Stair5,\
                          Stair10,Crosshatch, PLUGE };

extern const char* TVSystemTxt[];
extern const char* GenlockSystemTxt[];
extern const char* EmbAudioToneTxt[];
extern const char* AudioOutputTxt[];
extern const char* AudioSystemTxt[];
extern const char* AudioToneTxt[];
extern const char* AESEBULevelTxt[];
extern const char* AnalogLevelTxt[];
extern const char *AudioClickTxt[];
extern const char *AudioWordTxt[];
extern const char* CBarPatternTxt[];

//---------------------------------------------------------------------------
class TGenlockGenerator
{
	public:
		TGenlockGenerator( void);
    ~TGenlockGenerator( void);

		int System;
    long Samples;

	private:
};
//---------------------------------------------------------------------------
class TBlackGenerator
{
	public:
		TBlackGenerator( void);
    ~TBlackGenerator( void);

		int System;
    long Samples;
		int ScHPhase;

	private:
};
//---------------------------------------------------------------------------
class TCBarGenerator
{
	public:
		TCBarGenerator( void);
    ~TCBarGenerator( void);

		int System;
    long Samples;
    int ScHPhase;
		int Pattern;
		int EmbAudio;

	private:
};
//---------------------------------------------------------------------------
class TAudioGenerator
{
	public:
		TAudioGenerator( void);
    ~TAudioGenerator( void);

		int Output;
		int System;
    long Timing;
    int WordClock;
    int Tone[2];
    int Level[2];
    int ClickPeriod[2];

	private:
};
//---------------------------------------------------------------------------
class TSyncGenerator
{
	public:
		TSyncGenerator( void);
    ~TSyncGenerator( void);

		void SetGenlockSettings( TGenlockGenerator *tmp);
		TGenlockGenerator *GetGenlockSettings( void);
		void SetBlackSettings( int index, TBlackGenerator *tmp);
		TBlackGenerator *GetBlackSettings( int index);
		void SetCBarSettings( TCBarGenerator *tmp);
		TCBarGenerator *GetCBarSettings( void);
		void SetAudioSettings( TAudioGenerator *tmp);
		TAudioGenerator *GetAudioSettings( void);

	  int TSyncGenerator::SaveToFile( AnsiString &FileName);

	private:
  	TGenlockGenerator GenlockSetup;
  	TBlackGenerator BlackSetup[3];
  	TCBarGenerator CBarSetup;
  	TAudioGenerator AudioSetup;
};
//---------------------------------------------------------------------------
#endif
