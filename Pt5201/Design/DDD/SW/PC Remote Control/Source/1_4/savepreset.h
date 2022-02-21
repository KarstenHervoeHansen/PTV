//---------------------------------------------------------------------------
#ifndef savepresetH
#define savepresetH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TComboBox *PresetComboBox;
	TLabel *Label1;
	TEdit *Edit1;
	TLabel *Label2;
	TButton *OKButton;
	TButton *CancelButton;
	TMemo *Memo1;
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
