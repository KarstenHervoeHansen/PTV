//---------------------------------------------------------------------------
#ifndef presetH
#define presetH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Mask.hpp>
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
	TEdit *Edit1;
	TMaskEdit *MaskEdit1;
	TLabel *Label3;
	TLabel *Label4;
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
