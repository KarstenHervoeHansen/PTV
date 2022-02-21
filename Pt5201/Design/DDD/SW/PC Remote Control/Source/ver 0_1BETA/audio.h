//---------------------------------------------------------------------------
#ifndef audioH
#define audioH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ComCtrls.hpp>
//---------------------------------------------------------------------------
class TAudioForm : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *WordClockRadioGroup;
	TRadioGroup *ClickPeriodRadioGroup;
	TGroupBox *TimingGroupBox;
	TPanel *Panel3;
	TEdit *TimingEdit;
	TTrackBar *TimingTrackBar;
	TGroupBox *GroupBox1;
	TComboBox *ToneComboBox;
	TComboBox *LevelComboBox;
	TLabel *Label1;
	TLabel *Label2;
	TUpDown *TimingUpDown;
	TLabel *Label4;
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TGroupBox *SystemGroupBox;
	TComboBox *SystemComboBox;
	TRadioGroup *OutputRadioGroup;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	
	void __fastcall SystemComboBoxChange(TObject *Sender);
	void __fastcall ClickPeriodRadioGroupClick(TObject *Sender);
	void __fastcall WordClockRadioGroupClick(TObject *Sender);
	void __fastcall TimingUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall ToneComboBoxChange(TObject *Sender);
	void __fastcall LevelComboBoxChange(TObject *Sender);
	void __fastcall OutputRadioGroupClick(TObject *Sender);
private:	// User declarations
	TAudioGenerator *AudioSetup, *AudioCancel;
	int ModelessResult;
public:		// User declarations
	__fastcall TAudioForm(TComponent* Owner);
	void __fastcall LockForm( bool Lock);
};
//---------------------------------------------------------------------------
extern TAudioForm *AudioForm;
//---------------------------------------------------------------------------
#endif
