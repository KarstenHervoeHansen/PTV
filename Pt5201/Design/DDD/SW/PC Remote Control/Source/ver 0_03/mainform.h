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
	TMenuItem *SavePreset1;
	TMenuItem *SavePreset2;
	TMenuItem *SavePreset3;
	TMenuItem *SavePreset4;
	TMenuItem *N6;
	TMenuItem *OpenPreset1;
	TMenuItem *OpenPreset2;
	TMenuItem *OpenPreset3;
	TMenuItem *OpenPreset4;
	TMenuItem *toFileAs1;
	TMenuItem *Print1;
	TImage *Image1;
	TPrintDialog *PrintDialog1;
	TPanel *Panel2;
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
	TGroupBox *GroupBox2;
	TPanel *Panel10;
	TGroupBox *GroupBox1;
	TPanel *Panel3;
	TTrackBar *TrackBar1;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TPanel *Panel4;
	TGroupBox *GroupBox3;
	TPanel *Panel7;
	TGroupBox *GroupBox4;
	TPanel *Panel13;
	TGroupBox *GroupBox5;
	TPanel *Panel16;
	TMemo *Memo9;
	TPanel *Panel17;
	TMemo *Memo10;
	TGroupBox *GroupBox6;
	TPanel *Panel19;
	TPanel *Panel20;
	TPanel *Panel5;
	TMemo *Memo14;
	TPanel *Panel22;
	TLabel *Label2;
	TMemo *Memo2;
	TLabel *Label6;
	TMemo *Memo11;
	TLabel *Label8;
	TPanel *Panel6;
	TLabel *Label9;
	TTrackBar *TrackBar2;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TPanel *Panel9;
	TLabel *Label10;
	TTrackBar *TrackBar3;
	TEdit *Edit7;
	TEdit *Edit8;
	TEdit *Edit9;
	TPanel *Panel12;
	TLabel *Label11;
	TTrackBar *TrackBar4;
	TEdit *Edit10;
	TEdit *Edit11;
	TEdit *Edit12;
	TPanel *Panel18;
	TLabel *Label12;
	TTrackBar *TrackBar6;
	TEdit *Edit13;
	TEdit *Edit14;
	TEdit *Edit16;
	TMemo *Memo12;
	TLabel *Label13;
	TPanel *Panel21;
	TLabel *Label17;
	TMemo *Memo13;
	TPanel *Panel15;
	TLabel *Label18;
	TTrackBar *TrackBar5;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TMemo *Memo16;
	TLabel *Label1;
	TMemo *Memo1;
	TLabel *Label3;
	TMemo *Memo3;
	TLabel *Label4;
	TMemo *Memo5;
	TLabel *Label5;
	TMemo *Memo7;
	TLabel *Label7;
	TMemo *Memo15;
	TPanel *Panel8;
	TLabel *Label14;
	TMemo *Memo4;
	TPanel *Panel11;
	TLabel *Label15;
	TMemo *Memo6;
	TPanel *Panel14;
	TLabel *Label16;
	TMemo *Memo8;
	TPanel *Panel23;
	TLabel *Label22;
	TMemo *Memo17;
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
