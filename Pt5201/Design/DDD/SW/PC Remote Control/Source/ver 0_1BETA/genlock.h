//---------------------------------------------------------------------------
#ifndef genlockH
#define genlockH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TGenlockForm : public TForm
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
	TEdit *OffsetEdit;
	TGroupBox *SystemGroupBox;
	TComboBox *SystemComboBox;
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TUpDown *LineUpDown;
	TUpDown *OffsetUpDown;
	TLabel *MinLabel;
	TLabel *MaxLabel;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall SystemComboBoxChange(TObject *Sender);
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
private:	// User declarations
	TGenlockGenerator *GenlockSetup, *GenlockCancel;
	int ModelessResult;
public:		// User declarations
	__fastcall TGenlockForm(TComponent* Owner);
	void __fastcall LockForm( bool Lock);
};
//---------------------------------------------------------------------------
extern TGenlockForm *GenlockForm;
//---------------------------------------------------------------------------
#endif
