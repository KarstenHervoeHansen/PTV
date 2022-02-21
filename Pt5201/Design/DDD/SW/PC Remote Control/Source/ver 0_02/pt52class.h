//---------------------------------------------------------------------------
#ifndef pt52classH
#define pt52classH
//---------------------------------------------------------------------------
// The enumerated types MUST match the text arrays defined in pt52class.cpp
enum GeneratorType			{ GenlockType = 0, BlackType, CBarType, AudioType };
enum GenlockSystem			{ AutoGenlock=0, PALGenlock, NTSCGenlock, f10MHzGenlock};
enum TVSystemType				{ PAL=0, PAL_ID, NTSC};
enum AudioSystemType 		{ AESEBUAudio = 0, AnalogAudio };
enum AudioToneType			{ f500Hz = 0, f1000Hz, f1000HzwClick, f8000Hz };
enum AudioClickType			{ click3s = 0, click1s };
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
		char Field;
    int Line;
    long Samples;

	private:
};
//---------------------------------------------------------------------------
class TBlackGenerator
{
	public:
		TBlackGenerator( void);
    ~TBlackGenerator( void);

		char System[3];
		char Field[3];
    int Line[3];
    long Samples[3];
		int ScHPhase[3];

	private:
};
//---------------------------------------------------------------------------
class TCBarGenerator
{
	public:
		TCBarGenerator( void);
    ~TCBarGenerator( void);

		char System;
		char Field;
    int Line;
    long Samples;
    int ScHPhase;
		char Pattern;

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
    char SamplingFreq;
    char Tone[2];
    int Level[2];
    char ClickPeriod[2];

	private:
};
//---------------------------------------------------------------------------
class TSyncGenerator : public TGenlockGenerator,\
											 public TBlackGenerator,\
											 public TCBarGenerator,\
											 public TAudioGenerator
{
	public:
		TSyncGenerator( void);
    ~TSyncGenerator( void);

		void SetGenlockSettings( TGenlockGenerator *tmp);
		void GetGenlockSettings( TGenlockGenerator *tmp);
		void SetBlackSettings( int index, TBlackGenerator *tmp);
		void GetBlackSettings( int index, TBlackGenerator *tmp);
		void SetCBarSettings( TCBarGenerator *tmp);
		void GetCBarSettings( TCBarGenerator *tmp);
		void SetAudioSettings( TAudioGenerator *tmp);
		void GetAudioSettings( TAudioGenerator *tmp);

    int TSyncGenerator::SaveToFile( AnsiString &FileName);

	private:
};
//---------------------------------------------------------------------------
#endif
