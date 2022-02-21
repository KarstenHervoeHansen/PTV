//---------------------------------------------------------------------------
#ifndef presetH
#define presetH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPresetForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TComboBox *PresetComboBox;
	TLabel *Label1;
	TEdit *PresetEdit;
	TLabel *Label2;
	TButton *OKButton;
	TButton *CancelButton;
	TListBox *PresetListBox;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PresetComboBoxChange(TObject *Sender);
	void __fastcall PresetListBoxDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TPresetForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPresetForm *PresetForm;
//---------------------------------------------------------------------------
#endif
