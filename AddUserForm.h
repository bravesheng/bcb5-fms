//---------------------------------------------------------------------------

#ifndef AddUserFormH
#define AddUserFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_AddUser : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit_LoginID;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *Edit_LoginName;
        TEdit *Edit_Memo;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *Edit_Pwd;
        TEdit *Edit_RetryPwd;
        TBevel *Bevel1;
        TButton *Button_Build;
        TButton *Button_Cancel;
        TBevel *Bevel2;
        TLabel *Label6;
        TComboBox *ComboBox_AutType;
        void __fastcall Button_CancelClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button_BuildClick(TObject *Sender);
        void __fastcall Edit_LoginIDChange(TObject *Sender);
        void __fastcall Edit_PwdChange(TObject *Sender);
        void __fastcall Edit_RetryPwdChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_AddUser(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_AddUser *Form_AddUser;
//---------------------------------------------------------------------------
#endif
