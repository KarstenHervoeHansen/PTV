//---------------------------------------------------------------------------
#ifndef BlackH
#define BlackH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ComCtrls.hpp>
//---------------------------------------------------------------------------
class TBlack1Form : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *SystemGroupBox;
	TComboBox *SystemComboBox;
	TGroupBox *StatusGroupBox;
	TMemo *Memo1;
	TGroupBox *TimingGroupBox;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label2;
	TEdit *LineEdit;
	TEdit *FieldEdit;
	TUpDown *FieldUpDown;
	TUpDown *LineUpDown;
	TEdit *SamplesEdit;
	TUpDown *SamplesUpDown;
	TGroupBox *ScHPhaseGroupBox;
	TEdit *ScHPhaseEdit;
	TUpDown *ScHPhaseUpDown;
	TButton *OKButton;
	TButton *ApplyButton;
	TButton *CancelButton;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall ApplyButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SystemComboBoxChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TBlack1Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TBlack1Form *Black1Form;
//---------------------------------------------------------------------------
#endif
