//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SetupPwdForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EditPasswordForm"
#pragma resource "*.dfm"
TForm_EditPassword1 *Form_EditPassword1;
//---------------------------------------------------------------------------
__fastcall TForm_EditPassword1::TForm_EditPassword1(TComponent* Owner)
        : TForm_EditPassword(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm_EditPassword1::Button_CancelClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

void __fastcall TForm_EditPassword1::Button_okClick(TObject *Sender)
{
        if(Edit_NewPassword->Text != Edit_NewPasRetry->Text)
        {
                ShowMessage("密碼填寫錯誤,請重新填寫!");
                return;
        }
        else
        {
                TQuery* pQuery = new TQuery(Form_EditPassword1);
                pQuery->DatabaseName = "DB_DCH";
                pQuery->SQL->Add("UPDATE tUSerAuthorize");
                pQuery->SQL->Add("SET LoginPwd = :LoginPwd");
                pQuery->SQL->Add("WHERE LoginID = :LoginID");
                pQuery->ParamByName("LoginPwd")->AsString = Edit_NewPassword->Text;
                pQuery->ParamByName("LoginID")->AsString = LoginID;
                try
                {
                        pQuery->ExecSQL();
                        ShowMessage("設定密碼成功!");
                }
                catch(...)
                {
                        ShowMessage("設定失敗,請檢查資料庫!");
                }
                delete pQuery;
        }
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_EditPassword1::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        Action = caFree;
        Form_EditPassword1 = NULL;        
}
//---------------------------------------------------------------------------

