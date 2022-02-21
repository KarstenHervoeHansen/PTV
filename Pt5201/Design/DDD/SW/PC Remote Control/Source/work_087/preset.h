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
	TButton *OKButton;
	TButton *CancelButton;
	TButton *HelpButton;
	TPanel *Panel1;
	TLabel *PresetNoLabel;
	TLabel *NameLabel;
	TLabel *AuthorLabel;
	TLabel *DateLabel;
	TComboBox *PresetComboBox;
	TEdit *NameEdit;
	TEdit *AuthorEdit;
	TMaskEdit *DateMaskEdit;
	void __fastcall FormShow(TObject *Sender);


	void __fastcall PresetComboBoxChange(TObject *Sender);
	
	void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations
	void __fastcall GetPresetInformation( char PresetNumber);
public:		// User declarations
	__fastcall TPresetForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern TPresetForm *PresetForm;
//---------------------------------------------------------------------------
#endif
