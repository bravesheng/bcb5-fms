//---------------------------------------------------------------------------

#ifndef LoginFormH
#define LoginFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TForm_Login : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit_UserName;
        TLabel *Label1;
        TLabel *Label2;
        TButton *Button_Ok;
        TButton *Button_Cancel;
        TEdit *Edit_Password;
        TLabel *Label3;
        void __fastcall Button_OkClick(TObject *Sender);
        void __fastcall Button_CancelClick(TObject *Sender);
        void __fastcall Edit_PasswordKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_Login(TComponent* Owner);
        bool __fastcall Execute_Login();
        void __fastcall SetAuthorize();
        int Login_Status;       //目前登錄狀況 0.正在登錄 1.登錄成功 2.取消登錄
        AnsiString LoginID;     //登入者ID
        AnsiString LoginPwd;    //登入者密碼
        AnsiString AutType;     //登入者權限
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_Login *Form_Login;
//---------------------------------------------------------------------------
#endif
