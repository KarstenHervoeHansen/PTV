//---------------------------------------------------------------------------
#ifndef statusH
#define statusH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\QuickRep.hpp>
//---------------------------------------------------------------------------
class TSystemStatusForm : public TForm
{
__published:	// IDE-managed Components
	TPrintDialog *PrintDialog;
	TMemo *Memo1;
	TButton *OKButton;
	TButton *PrintButton;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	
	void __fastcall PrintButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSystemStatusForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSystemStatusForm *SystemStatusForm;
//---------------------------------------------------------------------------
#endif
