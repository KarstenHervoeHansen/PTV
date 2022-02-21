//---------------------------------------------------------------------------
#ifndef connectH
#define connectH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
//---------------------------------------------------------------------------
class TConnectForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TButton *OKButton;
private:	// User declarations
public:		// User declarations
	__fastcall TConnectForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TConnectForm *ConnectForm;
//---------------------------------------------------------------------------
#endif
