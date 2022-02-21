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
#include "Grids.hpp"
#include "GIFImage.hpp"
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
	TMenuItem *ConfigureItem;
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
	TMenuItem *ViewApparatusStatusItem;
	TMenuItem *ViewStatusBarItem;
	TMenuItem *N2;
	TPanel *PT5201FrontPanel;
	TMenuItem *HelpItem;
	TMenuItem *Contents1;
	TMenuItem *SearchforHelpOn1;
	TMenuItem *HowtoUseHelp1;
	TMenuItem *HelpAboutItem;
	TMenuItem *ConnectItem;
	TMenuItem *ConnectConnectItem;
	TMenuItem *ConnectDisconnectItem;
	TMenuItem *N4;
	TMenuItem *ConfigureLockItem;
	TMenuItem *RestoreFromFile;
	TMenuItem *OpenPreset;
	TMenuItem *SaveToFile;
	TMenuItem *SavePreset;
	TMenuItem *N6;
	TMenuItem *toFileAs1;
	TMenuItem *FilePrintItem;
	TPrintDialog *PrintDialog1;
	TPanel *ApparatusStatusPanel;
	TMenuItem *FilePropertiesItem;
	TMenuItem *N1;
	TImage *LEDOnImage;
	TImage *LEDOffImage;
	TImage *BB3PALImage;
	TImage *CBarPALImage;
	TImage *AudioAnalogImage;
	TImage *AudioAESEBUImage;
	TImage *Word441Image;
	TImage *Word48Image;
	TImage *Image14;
	TImage *Image15;
	TImage *Image16;
	TImage *BB1PALImage;
	TImage *BB2PALImage;
	TImage *BB1NTSCImage;
	TImage *BB2NTSCImage;
	TImage *BB3NTSCImage;
	TImage *CBarNTSCImage;
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
	TTrackBar *CBarOffsetTrackBar;
	TEdit *CBarFieldEdit;
	TEdit *CBarLineEdit;
	TEdit *CBarOffsetEdit;
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
	TImage *Image2;
	TLabel *Label23;
	TLabel *Label24;
	TEdit *BB2SystemEdit;
	TEdit *BB1SystemEdit;
	TEdit *GenlockSystemEdit;
	TEdit *BB3SystemEdit;
	TEdit *CBarSystemEdit;
	TEdit *AudioSystemEdit;
	TEdit *BB1ScHEdit;
	TEdit *BB2ScHEdit;
	TEdit *BB3ScHEdit;
	TEdit *CBarScHEdit;
	TEdit *AudioTimingEdit;
	TEdit *CBarPatternEdit;
	TEdit *AudioToneEdit;
	TEdit *AudioPLLEdit;
	TEdit *AudioVideoEdit;
	TEdit *AudioClickEdit;
	TEdit *Edit1;
	TEdit *AudioLevelEdit;
	TEdit *AudioWordEdit;
	TPanel *Panel1;
	TLabel *Label25;
	TEdit *EmbAudioEdit;
	TGroupBox *GroupBox1;
	TGIFImage *MooseGIFImage;
	TPanel *Panel2;
	TLabel *Label26;
	TEdit *Edit2;
	TPanel *Panel24;
	TLabel *Label27;
	TEdit *Edit3;
	TLabel *Label28;
	TPanel *Panel26;
	TLabel *Label29;
	TLabel *Label30;
	TEdit *Edit4;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RestoreFromFileClick(TObject *Sender);
	void __fastcall toFileAs1Click(TObject *Sender);
	void __fastcall FilePrintItemClick(TObject *Sender);
	void __fastcall FilePropertiesItemClick(TObject *Sender);
	void __fastcall FileItemExitClick(TObject *Sender);
	void __fastcall ConfigureGenlockItemClick(TObject *Sender);
	void __fastcall ConfigureAudioItemClick(TObject *Sender);
	void __fastcall ConfigureColorBarItemClick(TObject *Sender);
	void __fastcall BlackBurst1ItemClick(TObject *Sender);
	void __fastcall BlackBurst2ItemClick(TObject *Sender);
	void __fastcall BlackBurst3ItemClick(TObject *Sender);
	void __fastcall ConfigureLockItemClick(TObject *Sender);
	void __fastcall ViewSystemStatusItemClick(TObject *Sender);
	void __fastcall ViewApparatusStatusItemClick(TObject *Sender);
	void __fastcall ViewStatusBarItemClick(TObject *Sender);
	void __fastcall Contents1Click(TObject *Sender);
	void __fastcall ConnectConnectItemClick(TObject *Sender);
	void __fastcall HelpAboutItemClick(TObject *Sender);
	void __fastcall Image2DblClick(TObject *Sender);
	
	void __fastcall OpenPresetClick(TObject *Sender);
	void __fastcall SavePresetClick(TObject *Sender);
	
	
	
private:	// User declarations
public:		// User declarations
	__fastcall TPT5201Form(TComponent* Owner);
	void __fastcall ApparatusStatusUpdate(TObject *Sender);
};
//---------------------------------------------------------------------------
extern TPT5201Form *PT5201Form;
extern TSyncGenerator *PT5201Setup;
extern const TCursor crMyCustomCursor;

//---------------------------------------------------------------------------
#endif
