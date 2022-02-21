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
	TMenuItem *BlackBurst11;
	TMenuItem *BlackBurst21;
	TMenuItem *BlackBurst31;
	TMenuItem *ViewItem;
	TOpenDialog *FileOpenDialog;
	TSaveDialog *FileSaveDialog;
	TMenuItem *N7;
	TMenuItem *ViewSystemStatusItem;
	TMenuItem *ViewApparatusStatusItem;
	TMenuItem *ViewStatusBarItem;
	TMenuItem *N2;
	TPanel *Panel1;
	TMenuItem *HelpItem;
	TMenuItem *Contents1;
	TMenuItem *SearchforHelpOn1;
	TMenuItem *HowtoUseHelp1;
	TMenuItem *About1;
	TMenuItem *ConnectItem;
	TMenuItem *ConnectConnectItem;
	TMenuItem *ConnectDisconnectItem;
	TMenuItem *N4;
	TMenuItem *ConfigureLockItem;
	TMenuItem *RestoreFromFile;
	TMenuItem *Preset1;
	TMenuItem *SaveToFile;
	TMenuItem *Preset2;
	TMenuItem *Preset11;
	TMenuItem *Preset21;
	TMenuItem *Preset31;
	TMenuItem *Preset41;
	TMenuItem *N6;
	TMenuItem *Preset12;
	TMenuItem *Preset22;
	TMenuItem *Preset32;
	TMenuItem *Preset42;
	TMenuItem *toFileAs1;
	TMenuItem *Print1;
	TImage *Image1;
	TPrintDialog *PrintDialog1;
	TPanel *Panel2;
	TGroupBox *Black2GroupBox;
	TGroupBox *AudioGroupBox;
	TGroupBox *Black3GroupBox;
	TGroupBox *Black1GroupBox;
	TGroupBox *ColorBarGroupBox;
	TGroupBox *GenlockGroupBox;
	TPanel *Panel3;
	TRadioGroup *ClickPeriodRadioGroup;
	TRadioGroup *RadioGroup1;
	TMenuItem *Properties2;
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
	void __fastcall About1Click(TObject *Sender);
	void __fastcall ConfigureGenlockItemClick(TObject *Sender);

	
	void __fastcall ConfigureAudioItemClick(TObject *Sender);
	void __fastcall ConfigureColorBarItemClick(TObject *Sender);
	void __fastcall BlackBurst11Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FileItemExitClick(TObject *Sender);
	void __fastcall ViewStatusBarItemClick(TObject *Sender);
	void __fastcall ViewSystemStatusItemClick(TObject *Sender);
	void __fastcall ViewApparatusStatusItemClick(TObject *Sender);
	
	void __fastcall Contents1Click(TObject *Sender);
	void __fastcall ConfigureLockItemClick(TObject *Sender);
	void __fastcall RestoreFromFileClick(TObject *Sender);
	
	
	void __fastcall toFileAs1Click(TObject *Sender);
	void __fastcall BlackBurst21Click(TObject *Sender);
	void __fastcall BlackBurst31Click(TObject *Sender);
	
	void __fastcall ConnectConnectItemClick(TObject *Sender);
	void __fastcall Print1Click(TObject *Sender);
	
	
	void __fastcall GenlockGroupBoxDblClick(TObject *Sender);
	
	void __fastcall Black1GroupBoxDblClick(TObject *Sender);
	void __fastcall Black2GroupBoxDblClick(TObject *Sender);
	void __fastcall Black3GroupBoxDblClick(TObject *Sender);
	void __fastcall ColorBarGroupBoxDblClick(TObject *Sender);
	void __fastcall AudioGroupBoxDblClick(TObject *Sender);
	
	
	
	
	void __fastcall Properties2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPT5201Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPT5201Form *PT5201Form;
extern TSyncGenerator *PT5201Setup;
extern const TCursor crMyCustomCursor;

//---------------------------------------------------------------------------
#endif
