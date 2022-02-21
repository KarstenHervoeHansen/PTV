//---------------------------------------------------------------------------
#ifndef mainformH
#define mainformH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Menus.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\Buttons.hpp>
#include "pt52class.h"
#include "comm.h"
#include "Grids.hpp"

#define IDH_ABOUT													1
#define IDH_FILESOPENFILE                 2
#define IDH_FILESOPENPRESET               3
#define IDH_FILESSAVEFILE                 4
#define IDH_FILESSAVEPRESET               5
#define IDH_FILESPRINT                    6
#define IDH_FILESPROPERTIES               7
#define IDH_CONNECTGENERATORSETTINGS      8
#define IDH_CONNECTPCSETTINGS             9
#define IDH_CONNECTDISCONNECT            10
#define IDH_CONNECTDEMOMODE              11
#define IDH_EDITGENLOCK                  12
#define IDH_EDITBB                       13
#define IDH_EDITTSG                      14
#define IDH_EDITAUDIO                    15
#define IDH_EDITSYSTEM                   16
#define IDH_VIEWSYSTEMSTATUS             17
#define IDH_VIEWSTATUSBAR                18

enum StatusPanels
{
	ConnectedPanel = 0,
	CommPortPanel,
  CommSettingsPanel,
  DemoModePanel,
  HintPanel
};

//---------------------------------------------------------------------------
class TPT5201Form : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TStatusBar *StatusBar;
	TMenuItem *FileItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileSaveItem;
	TMenuItem *FileItemExit;
	TMenuItem *EditItem;
	TMenuItem *ConfigureGenlockItem;
	TMenuItem *ConfigureBlackBurstItem;
	TMenuItem *ConfigureColorBarItem;
	TMenuItem *ConfigureAudioItem;
	TMenuItem *BlackBurst1Item;
	TMenuItem *BlackBurst2Item;
	TMenuItem *BlackBurst3Item;
	TMenuItem *ViewItem;
	TOpenDialog *FileOpenDialog;
	TSaveDialog *FileSaveDialog;
	TMenuItem *N7;
	TMenuItem *ViewSystemStatusItem;
	TMenuItem *ViewStatusBarItem;
	TPanel *PT5201FrontPanel;
	TMenuItem *HelpItem;
	TMenuItem *Contents1;
	TMenuItem *SearchforHelpOn1;
	TMenuItem *HelpAboutItem;
	TMenuItem *ConnectItem;
	TMenuItem *ConnectConnectItem;
	TMenuItem *ConnectDisconnectItem;
	TMenuItem *RestoreFromFile;
	TMenuItem *OpenPreset;
	TMenuItem *SavePreset;
	TMenuItem *N6;
	TMenuItem *toFileAs1;
	TMenuItem *FilePrintItem;
	TPrintDialog *PrintDialog;
	TPanel *SystemStatusPanel;
	TMenuItem *FilePropertiesItem;
	TMenuItem *N1;
	TImage *LEDOnImage;
	TImage *LEDOffImage;
	TImage *BB3PALImage;
	TImage *TSGPALImage;
	TImage *AudioAnalogImage;
	TImage *AudioAESEBUImage;
	TImage *Word441Image;
	TImage *Word48Image;
	TImage *PowerImage;
	TImage *GenlockImage;
	TImage *Image16;
	TImage *BB1PALImage;
	TImage *BB2PALImage;
	TImage *BB1NTSCImage;
	TImage *BB2NTSCImage;
	TImage *BB3NTSCImage;
	TImage *TSGNTSCImage;
	TGroupBox *BB2GroupBox;
	TPanel *Panel10;
	TGroupBox *GenlockGroupBox;
	TPanel *Panel3;
	TTrackBar *GenlockOffsetTrackBar;
	TEdit *GenlockFieldEdit;
	TEdit *GenlockLineEdit;
	TEdit *GenlockOffsetEdit;
	TPanel *Panel4;
	TGroupBox *BB3GroupBox;
	TPanel *Panel7;
	TGroupBox *CBarGroupBox;
	TPanel *Panel13;
	TGroupBox *AudioGroupBox;
	TPanel *Panel16;
	TPanel *Panel17;
	TGroupBox *BB1GroupBox;
	TPanel *Panel19;
	TPanel *Panel20;
	TPanel *Panel5;
	TPanel *Panel22;
	TLabel *Label2;
	TLabel *Label6;
	TLabel *Label8;
	TPanel *Panel6;
	TLabel *Label9;
	TTrackBar *BB1OffsetTrackBar;
	TEdit *BB1FieldEdit;
	TEdit *BB1LineEdit;
	TEdit *BB1OffsetEdit;
	TPanel *Panel9;
	TLabel *Label10;
	TTrackBar *BB2OffsetTrackBar;
	TEdit *BB2FieldEdit;
	TEdit *BB2LineEdit;
	TEdit *BB2OffsetEdit;
	TPanel *Panel12;
	TLabel *Label11;
	TTrackBar *BB3OffsetTrackBar;
	TEdit *BB3FieldEdit;
	TEdit *BB3LineEdit;
	TEdit *BB3OffsetEdit;
	TPanel *Panel18;
	TLabel *Label12;
	TTrackBar *TSGOffsetTrackBar;
	TEdit *TSGFieldEdit;
	TEdit *TSGLineEdit;
	TEdit *TSGOffsetEdit;
	TLabel *Label13;
	TPanel *Panel21;
	TLabel *Label17;
	TPanel *Panel15;
	TLabel *Label18;
	TTrackBar *AudioTrackBar;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label7;
	TPanel *Panel8;
	TLabel *Label14;
	TPanel *Panel11;
	TLabel *Label15;
	TPanel *Panel14;
	TLabel *Label16;
	TPanel *Panel23;
	TLabel *Label22;
	TPanel *Panel25;
	TImage *PT5201FrontImage;
	TLabel *Label23;
	TLabel *Label24;
	TEdit *BB2SystemEdit;
	TEdit *BB1SystemEdit;
	TEdit *GenlockSystemEdit;
	TEdit *BB3SystemEdit;
	TEdit *TSGSystemEdit;
	TEdit *AudioSystemEdit;
	TEdit *BB1ScHEdit;
	TEdit *BB2ScHEdit;
	TEdit *BB3ScHEdit;
	TEdit *TSGScHEdit;
	TEdit *AudioTimingEdit;
	TEdit *TSGPatternEdit;
	TEdit *AudioToneEdit;
	TEdit *AudioPLLEdit;
	TEdit *AudioVideoEdit;
	TEdit *AudioClickEdit;
	TEdit *AudioOutputEdit;
	TEdit *AudioLevelEdit;
	TEdit *AudioWordEdit;
	TPanel *Panel1;
	TLabel *Label25;
	TEdit *TSGEmbAudioEdit;
	TMenuItem *N3;
	TMenuItem *N5;
	TMenuItem *EditSystemItem;
	TMenuItem *N4;
	TMenuItem *PTVonttheWeb1;
	TMenuItem *OnlineSupport1;
	TMenuItem *N8;
	TMenuItem *PTVHomepage1;
	TMenuItem *Frequentlyaskedquestions1;
	TGroupBox *GroupBox1;
	TPanel *Panel2;
	TPanel *Panel24;
	TLabel *Label26;
	TEdit *KUNumberEdit;
	TPanel *Panel26;
	TEdit *SWRevisionEdit;
	TPanel *Panel27;
	TEdit *ErrorStatusEdit;
	TLabel *Label27;
	TLabel *Label28;
	TPanel *Panel28;
	TLabel *Label29;
	TEdit *PresetStatusEdit;
	TLabel *Label30;
	TTimer *Timer1;
	TMenuItem *N9;
	TMenuItem *ConnectDemoModeItem;
	TMenuItem *ConnectUsingGeneratorSettings;
	TMenuItem *ConnectUsingPCSettings;
	TPanel *InternalPanel;
	TMemo *GeneratorInfoMemo;
	TLabel *Label31;
	TImage *Image1;
	TLabel *AudioLevelLabel;
	TLabel *Label32;
	TLabel *Label33;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RestoreFromFileClick(TObject *Sender);
	void __fastcall toFileAs1Click(TObject *Sender);
	void __fastcall FilePrintItemClick(TObject *Sender);
	void __fastcall FilePropertiesItemClick(TObject *Sender);
	void __fastcall FileItemExitClick(TObject *Sender);
	void __fastcall ConnectUsingGeneratorSettingsClick(TObject *Sender);
	void __fastcall ConnectUsingPCSettingsClick(TObject *Sender);
	void __fastcall ConnectDisconnectItemClick(TObject *Sender);
	void __fastcall ConnectDemoModeItemClick(TObject *Sender);
	void __fastcall ConfigureGenlockItemClick(TObject *Sender);
	void __fastcall ConfigureAudioItemClick(TObject *Sender);
	void __fastcall ConfigureColorBarItemClick(TObject *Sender);
	void __fastcall BlackBurst1ItemClick(TObject *Sender);
	void __fastcall BlackBurst2ItemClick(TObject *Sender);
	void __fastcall BlackBurst3ItemClick(TObject *Sender);
	void __fastcall ViewSystemStatusItemClick(TObject *Sender);
	void __fastcall ViewStatusBarItemClick(TObject *Sender);
	void __fastcall Contents1Click(TObject *Sender);
	void __fastcall Frequentlyaskedquestions1Click(TObject *Sender);
	void __fastcall OnlineSupport1Click(TObject *Sender);
	void __fastcall PTVHomepage1Click(TObject *Sender);
	void __fastcall HelpAboutItemClick(TObject *Sender);
	void __fastcall OpenPresetClick(TObject *Sender);
	void __fastcall SavePresetClick(TObject *Sender);
	void __fastcall EditSystemItemClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall ConnectItemClick(TObject *Sender);
	void __fastcall Image1DblClick(TObject *Sender);
	
private:	// User declarations
public:		// User declarations
	__fastcall TPT5201Form(TComponent* Owner);
	void __fastcall SystemStatusUpdate( int StausField);
	void __fastcall PT5201LEDsUpdate( void);
	void __fastcall UpdateLEDs( bool LEDOn, TImage *LED);
	bool __fastcall TPT5201Form::ConnectToGenerator( void);
};
//---------------------------------------------------------------------------
extern TPT5201Form *PT5201Form;
extern TSyncGenerator *PT5201Setup;
extern TCommPort *PT5201CommPort;

extern bool TestMode, TestModeConnected;
//---------------------------------------------------------------------------
#endif
