//---------------------------------------------------------------------------

#ifndef EditPasswordFormH
#define EditPasswordFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm_EditPassword : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *Edit_OldPassword;
        TEdit *Edit_NewPassword;
        TEdit *Edit_NewPasRetry;
        TButton *Button_ok;
        TButton *Button_Cancel;
        void __fastcall Button_okClick(TObject *Sender);
        void __fastcall Button_CancelClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_EditPassword(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_EditPassword *Form_EditPassword;
//---------------------------------------------------------------------------
#endif
