//---------------------------------------------------------------------------
#ifndef infoH
#define infoH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\ComCtrls.hpp>
//---------------------------------------------------------------------------
class TInformationForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn3;
	TPrintDialog *PrintDialog1;
	TBitBtn *Print;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	void __fastcall PrintClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TInformationForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TInformationForm *InformationForm;
//---------------------------------------------------------------------------
#endif
