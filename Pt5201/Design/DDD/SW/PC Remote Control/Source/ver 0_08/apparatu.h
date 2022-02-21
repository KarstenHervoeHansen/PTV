//---------------------------------------------------------------------------
#ifndef apparatuH
#define apparatuH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
//---------------------------------------------------------------------------
class TApparatusStatusForm : public TForm
{
__published:	// IDE-managed Components
	TPrintDialog *PrintDialog;
	TMemo *Memo1;
	TButton *OKButton;
	TButton *PrintButton;
	TMemo *Memo2;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);

	void __fastcall PrintButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TApparatusStatusForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TApparatusStatusForm *ApparatusStatusForm;
//---------------------------------------------------------------------------
#endif
