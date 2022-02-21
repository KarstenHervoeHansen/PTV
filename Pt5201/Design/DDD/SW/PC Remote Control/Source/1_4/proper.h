//---------------------------------------------------------------------------
#ifndef properH
#define properH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPropertiesForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox1;
	TComboBox *PortComboBox;
	TLabel *Label1;
	TComboBox *BaudComboBox;
	TComboBox *FlowComboBox;
	TLabel *Label2;
	TLabel *Label4;
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TCheckBox *ConfigureLockItem;
	TPanel *Panel1;
	TMemo *Memo1;
	TCheckBox *LockCalibCheckBox;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BaudComboBoxChange(TObject *Sender);
	void __fastcall ConfigureLockItemClick(TObject *Sender);
	void __fastcall PortComboBoxChange(TObject *Sender);
	void __fastcall HelpButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPropertiesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPropertiesForm *PropertiesForm;
extern char *CommPortTxt[];
//---------------------------------------------------------------------------
#endif
