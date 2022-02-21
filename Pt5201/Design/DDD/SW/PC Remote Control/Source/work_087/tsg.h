//---------------------------------------------------------------------------
#ifndef tsgH
#define tsgH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Mask.hpp>
//---------------------------------------------------------------------------
class TTSGForm : public TForm
{
__published:	// IDE-managed Components
	TButton *OKButton;
	TButton *HelpButton;
	TPageControl *PageControl1;
	TTabSheet *AnalogSettingsTabSheet;
	TGroupBox *SystemGroupBox;
	TComboBox *SystemComboBox;
	TGroupBox *PatternGroupBox;
	TComboBox *PatternComboBox;
	TGroupBox *TimingGroupBox;
	TLabel *Label6;
	TLabel *MinLabel;
	TLabel *MaxLabel;
	TTrackBar *OffsetTrackBar;
	TPanel *Panel2;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label1;
	TLabel *Label3;
	TEdit *FieldEdit;
	TEdit *LineEdit;
	TEdit *OffsetEdit;
	TUpDown *LineUpDown;
	TUpDown *OffsetUpDown;
	TUpDown *FieldUpDown;
	TGroupBox *ScHPhaseGroupBox;
	TEdit *ScHPhaseEdit;
	TUpDown *ScHPhaseUpDown;
	TTabSheet *SDISettingsTabSheet;
	TGroupBox *EmbAudioGroupBox;
	TComboBox *EmbAudioComboBox;
	TPanel *Panel1;
	TMemo *Memo1;
	TButton *CancelButton;
	TEdit *Field1Edit;
	TEdit *Line1Edit;
	TEdit *Offset1Edit;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SystemComboBoxChange(TObject *Sender);
	void __fastcall ScHPhaseUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall ScHPhaseEditExit(TObject *Sender);
	void __fastcall FieldUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall FieldUpDownChanging(TObject *Sender, bool &AllowChange);
	void __fastcall FieldEditExit(TObject *Sender);
	void __fastcall LineUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall LineUpDownChanging(TObject *Sender, bool &AllowChange);
	void __fastcall LineEditExit(TObject *Sender);
	void __fastcall OffsetUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall OffsetUpDownChanging(TObject *Sender, bool &AllowChange);
	void __fastcall OffsetEditExit(TObject *Sender);
	void __fastcall OffsetTrackBarChange(TObject *Sender);
	void __fastcall PatternComboBoxChange(TObject *Sender);
	void __fastcall EmbAudioComboBoxChange(TObject *Sender);
	
	
	
	
private:	// User declarations
	TTSGenerator *TSGSetup, *TSGCancel;
	int ModelessResult;
public:		// User declarations
	__fastcall TTSGForm(TComponent* Owner);
	void __fastcall LockForm( bool Lock);
	void __fastcall UpdateOutput( TTSGenerator *setup);
};
//---------------------------------------------------------------------------
extern TTSGForm *TSGForm;
//---------------------------------------------------------------------------
#endif
