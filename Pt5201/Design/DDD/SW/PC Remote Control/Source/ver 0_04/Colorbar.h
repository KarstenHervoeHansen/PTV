//---------------------------------------------------------------------------
#ifndef ColorbarH
#define ColorbarH
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
class TCBarForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *AnalogSettingsTabSheet;
	TGroupBox *SystemGroupBox;
	TComboBox *SystemComboBox;
	TGroupBox *PatternGroupBox;
	TComboBox *PatternComboBox;
	TGroupBox *TimingGroupBox;
	TLabel *Label6;
	TTrackBar *OffsetTrackBar;
	TPanel *Panel2;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label1;
	TLabel *Label3;
	TEdit *FieldEdit;
	TUpDown *FieldUpDown;
	TEdit *LineEdit;
	TEdit *OffsetEdit;
	TGroupBox *ScHPhaseGroupBox;
	TEdit *ScHPhaseEdit;
	TUpDown *ScHPhaseUpDown;
	TTabSheet *SDISettingsTabSheet;
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TUpDown *LineUpDown;
	TUpDown *OffsetUpDown;
	TGroupBox *EmbAudioGroupBox;
	TComboBox *EmbAudioComboBox;
	TPanel *Panel1;
	TMemo *Memo1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SystemComboBoxChange(TObject *Sender);
	void __fastcall FieldUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall LineUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall OffsetUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall OffsetTrackBarChange(TObject *Sender);
	void __fastcall ScHPhaseUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall ScHPhaseEditExit(TObject *Sender);
	void __fastcall PatternComboBoxChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TCBarForm(TComponent* Owner);
	void __fastcall LockForm( bool Lock);
	void __fastcall UpdateLEDs( void);
};
//---------------------------------------------------------------------------
extern TCBarForm *CBarForm;
//---------------------------------------------------------------------------
#endif
