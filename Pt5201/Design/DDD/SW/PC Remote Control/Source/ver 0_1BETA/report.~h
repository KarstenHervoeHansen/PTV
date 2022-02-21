//---------------------------------------------------------------------------
#ifndef reportH
#define reportH
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
class TSystemReportForm : public TForm
{
__published:	// IDE-managed Components
	TPrintDialog *PrintDialog;
	TMemo *SystemReportMemo;
	TButton *OKButton;
	TBitBtn *PrintBitBtn;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	
	
	
	void __fastcall PrintBitBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSystemReportForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TSystemReportForm *SystemReportForm;
//---------------------------------------------------------------------------
#endif
