//---------------------------------------------------------------------------
#ifndef black3H
#define black3H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TBlack3Form : public TForm
{
__published:	// IDE-managed Components
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
	TUpDown *LineUpDown;
	TEdit *OffsetEdit;
	TUpDown *OffsetUpDown;
	TGroupBox *SystemGroupBox;
	TComboBox *SystemComboBox;
	TGroupBox *ScHPhaseGroupBox;
	TEdit *ScHPhaseEdit;
	TUpDown *ScHPhaseUpDown;
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SystemComboBoxChange(TObject *Sender);
	void __fastcall FieldUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall LineUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall OffsetUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall OffsetTrackBarChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TBlack3Form(TComponent* Owner);
	void __fastcall LockForm( bool Lock);
	void __fastcall UpdateLEDs( void);
};
//---------------------------------------------------------------------------
extern TBlack3Form *Black3Form;
//---------------------------------------------------------------------------
#endif
