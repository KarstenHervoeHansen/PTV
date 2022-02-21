//---------------------------------------------------------------------------
#ifndef pt52classH
#define pt52classH
//---------------------------------------------------------------------------
// The enumerated types MUST match the text arrays defined in pt52class.cpp
enum GeneratorType			{ GenlockType = 0, BlackType, CBarType, AudioType };
enum GenlockSystem			{ InternalGenlock=0, PALGenlock, NTSCGenlock, f10MHzGenlock};
enum TVSystemType				{ PAL=0, PAL_ID, NTSC};
enum EmbAudioType				{ EmbAudioSilence = 0, EmbAudiof1kHz };
enum AudioOutputType 		{ AnalogAudio = 0, AESEBUAudio };
enum AudioToneType			{ f500Hz = 0, f1000Hz, f1000HzwClick, f8000Hz };
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

extern const char* TVSystemTxt[];
extern const char* GenlockSystemTxt[];
extern const char* EmbAudioToneTxt[];
extern const char* AudioToneTxt[];
extern const char* AESEBULevelTxt[];
extern const char* AnalogLevelTxt[];
extern const char* PatternTxt[];
extern const char* PALPatternTxt[];
extern const char* NTSCPatternTxt[];

//---------------------------------------------------------------------------
class TGenlockGenerator
{
	public:
		TGenlockGenerator( void);
    ~TGenlockGenerator( void);

		char System;
    long Samples;

	private:
};
//---------------------------------------------------------------------------
class TBlack1Generator
{
	public:
		TBlack1Generator( void);
    ~TBlack1Generator( void);

		char System;
    long Samples;
		int ScHPhase;

	private:
};
//---------------------------------------------------------------------------
class TBlack2Generator
{
	public:
		TBlack2Generator( void);
    ~TBlack2Generator( void);

		char System;
    long Samples;
		int ScHPhase;

	private:
};
//---------------------------------------------------------------------------
class TBlack3Generator
{
	public:
		TBlack3Generator( void);
    ~TBlack3Generator( void);

		char System;
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

		char System;
    long Samples;
    int ScHPhase;
		char Pattern;
		char EmbAudio;

	private:
};
//---------------------------------------------------------------------------
class TAudioGenerator
{
	public:
		TAudioGenerator( void);
    ~TAudioGenerator( void);

		char Output;
		char System;
    long Timing;
    char WordClock;
    char Tone[2];
    int Level[2];
    char ClickPeriod[2];

	private:
};
//---------------------------------------------------------------------------
class TSyncGenerator : public TGenlockGenerator,\
											 public TBlack1Generator,\
											 public TBlack2Generator,\
											 public TBlack3Generator,\
											 public TCBarGenerator,\
											 public TAudioGenerator
{
	public:
		TSyncGenerator( void);
    ~TSyncGenerator( void);

		void SetGenlockSettings( TGenlockGenerator *tmp);
		void GetGenlockSettings( TGenlockGenerator *tmp);
		void SetBlack1Settings( TBlack1Generator *tmp);
		void GetBlack1Settings( TBlack1Generator *tmp);
		void SetBlack2Settings( TBlack2Generator *tmp);
		void GetBlack2Settings( TBlack2Generator *tmp);
		void SetBlack3Settings( TBlack3Generator *tmp);
		void GetBlack3Settings( TBlack3Generator *tmp);
		void SetCBarSettings( TCBarGenerator *tmp);
		void GetCBarSettings( TCBarGenerator *tmp);
		void SetAudioSettings( TAudioGenerator *tmp);
		void GetAudioSettings( TAudioGenerator *tmp);

    int TSyncGenerator::SaveToFile( AnsiString &FileName);

	private:
};
//---------------------------------------------------------------------------
#endif
