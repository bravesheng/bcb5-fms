//---------------------------------------------------------------------------

#ifndef SetupPwdFormH
#define SetupPwdFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EditPasswordForm.h"
//---------------------------------------------------------------------------
class TForm_EditPassword1 : public TForm_EditPassword
{
__published:	// IDE-managed Components
        void __fastcall Button_CancelClick(TObject *Sender);
        void __fastcall Button_okClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_EditPassword1(TComponent* Owner);
        AnsiString LoginID;     //紀錄目前設定的使用者為誰
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_EditPassword1 *Form_EditPassword1;
//---------------------------------------------------------------------------
#endif
