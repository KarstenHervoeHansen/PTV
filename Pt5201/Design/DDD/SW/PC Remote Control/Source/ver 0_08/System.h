//---------------------------------------------------------------------------
#ifndef SystemH
#define SystemH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSystemForm : public TForm
{
__published:	// IDE-managed Components
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TPanel *Panel1;
	TPanel *Panel2;
	TMemo *PresetMemo;
	TMemo *PasswordMemo;
	TCheckBox *EnablePasswordCheckBox;
	TEdit *PasswordEdit;
	TLabel *Label1;
	TEdit *ConfirmPasswordEdit;
	TLabel *ConfirmPasswordLabel;
	TTabSheet *TabSheet3;
	TPanel *Panel3;
	TMemo *Memo1;
	TGroupBox *GroupBox1;
	TComboBox *SystemComboBox;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PasswordEditExit(TObject *Sender);
	void __fastcall EnablePasswordCheckBoxClick(TObject *Sender);
	
	
	
private:	// User declarations
public:		// User declarations
	__fastcall TSystemForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSystemForm *SystemForm;
//---------------------------------------------------------------------------
#endif
