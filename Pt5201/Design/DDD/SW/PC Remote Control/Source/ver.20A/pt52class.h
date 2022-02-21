 // 31.05.2001 modified for JNTS: JK


//---------------------------------------------------------------------------
#ifndef pt52classH
#define pt52classH
//---------------------------------------------------------------------------
// The enumerated types MUST match the text/cmd arrays defined in pt52class.cpp

enum directions					{ UP = 0, DOWN, MINValue, MAXValue };

enum StatusFields 			{ GenlockField = 0, Black1Field, Black2Field, Black3Field,\
													TSGField, AudioField, AllFields };

enum BlackOutputs  			{ Black1Output = 0, Black2Output, Black3Output };
enum GeneratorType			{ GenlockType = 0, BlackType, TSGType, AudioType, AllGenerators };
enum GenlockSystem			{ InternalGenlock=0, PALGenlock, NTSCGenlock, f10MHzGenlock};
enum BBSystemType				{ PAL = 0, PAL_ID, NTSC, JNTS};
enum EmbAudioType				{ EmbAudioOff = 0, EmbAudioSilence, EmbAudioS1kHz };
enum AudioOutputType 		{ AnalogAudio = 0, AESEBUAudio };
enum AudioSystemType 		{ AudioPAL = 0, AudioNTSC };
enum AudioToneType			{ S500Hz = 0, S1kHz, SEBU1kHz, S8kHz };
enum AudioClickType			{ click1s = 0, click3s };
enum AudioFreqType			{ f441kHz = 0, f48kHz };
enum AESEBULevelType		{ M0dBFS = 0, M9dBFS, M12dBFS, M15dBFS, M16dBFS,\
									  			M18dBFS, M20dBFS, AESEBUSilence};
enum AnalogLevelType		{ P10dBm = 0, P8dBm, P7dBm, P6dBm, P5dBm,\
									  			P4dBm, P3dBm, P2dBm, P1dBm, P0dBm, M1dBm,\
									  			M2dBm, M3dBm, M4dBm, M5dBm, M6dBm, M7dBm,\
									  			M8dBm, M9dBm, M10dBm, M11dBm, M12dBm,\
									  			M15dBm, M18dBm, M20dBm, M24dBm, M27dBm, M30dBm,\
									  			M33dBm, M36dBm, AnalogSilence};

enum TSGPatterns				{ SMPTECBar = 0, EBUCBar,	FCCCBar, EBUCBarITU, CBar75ITU, CBar100,\
									  			CBarGrey75, CBarRed75, Red75, Multiburst, CCIR18,\
                          Window10, Window15, Window20, Window100, BlWh15kHz,\
                          White100, Black, CheckField, DigitalGrey,Stair5,\
                          Stair10,Crosshatch, PLUGE };

extern const char PALTxt[];
extern const char PAL_IDTxt[];
extern const char NTSCTxt[];
extern const char JNTSTxt[]; // JK change

extern const char* ResetSystemTxt[];
extern const char* GenlockSystemTxt[];
extern const char* SystemTxt[];

extern const char* TSGSystemTxt[];   //JK change

extern const char* EmbAudioToneTxt[];
extern const char* AudioOutputTxt[];
extern const char* AudioSystemTxt[];
extern const char* AudioToneTxt[];
extern const char* AESEBULevelTxt[];
extern const char* AnalogLevelTxt[];
extern const char* AudioClickTxt[];
extern const char* AudioWordTxt[];
extern const char* TSGPatternTxt[];

extern const char* ResetSystemCmdTxt[];
extern const char* GenlockSystemCmdTxt[];
extern const char* SystemCmdTxt[];

extern const char* TSGSystemCmdTxt[];  // JK change

extern const char* EmbAudioToneCmdTxt[];
extern const char* AudioOutputCmdTxt[];
extern const char* AudioSystemCmdTxt[];
extern const char* AudioToneCmdTxt[];
extern const char* AudioWordCmdTxt[];
extern const char* TSGPatternCmdTxt[];

extern const char *OffOnCmdTxt[];
extern const char *DirectionCmdTxt[];
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
class TTSGenerator
{
	public:
		TTSGenerator( void);
    ~TTSGenerator( void);

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
		int Timing;
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
		int VerifyGenlockSettings( TGenlockGenerator *tmp);
		void SetBlackSettings( int index, TBlackGenerator *tmp);
		TBlackGenerator *GetBlackSettings( int index);
		int VerifyBlackSettings( int index, TBlackGenerator *tmp);
		void SetTSGSettings( TTSGenerator *tmp);
		TTSGenerator *GetTSGSettings( void);
		int VerifyTSGSettings( TTSGenerator *tmp);
		void SetAudioSettings( TAudioGenerator *tmp);
		TAudioGenerator *GetAudioSettings( void);
		int VerifyAudioSettings( TAudioGenerator *tmp);
		int SaveToFile( AnsiString &FileName);
	  int ReadFromFile( AnsiString &FileName);

	private:
  		TGenlockGenerator GenlockSetup;
  		TBlackGenerator BlackSetup[3];
  		TTSGenerator TSGSetup;
  		TAudioGenerator AudioSetup;
};
//---------------------------------------------------------------------------
#endif
