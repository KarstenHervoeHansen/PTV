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
#include <vcl\Buttons.hpp>
//---------------------------------------------------------------------------
class TSystemForm : public TForm
{
__published:	// IDE-managed Components
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TPanel *Panel2;
	TMemo *PresetMemo;
	TGroupBox *GroupBox1;
	TComboBox *SystemComboBox;
	TTabSheet *TabSheet3;
	TPanel *Panel3;
	TMemo *Memo1;
	TBitBtn *ResetBitBtn;
	TBitBtn *SaveBitBtn;
	TButton *UnlockButton;
	TUpDown *CalibUpDown;
	TButton *MaxCalibButton;
	TButton *MinCalibButton;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ResetBitBtnClick(TObject *Sender);
	void __fastcall SystemComboBoxChange(TObject *Sender);
	
	
	void __fastcall UnlockButtonClick(TObject *Sender);
	void __fastcall SaveBitBtnClick(TObject *Sender);
	
	
	
	
	
	void __fastcall CalibUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall MaxCalibButtonClick(TObject *Sender);
	void __fastcall MinCalibButtonClick(TObject *Sender);
	
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall HelpButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSystemForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSystemForm *SystemForm;
//---------------------------------------------------------------------------
#endif
