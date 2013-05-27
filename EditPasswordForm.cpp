//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditPasswordForm.h"
#include "LoginForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_EditPassword *Form_EditPassword;
//---------------------------------------------------------------------------
__fastcall TForm_EditPassword::TForm_EditPassword(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_EditPassword::Button_okClick(TObject *Sender)
{
        if(Edit_NewPassword->Text != Edit_NewPasRetry->Text)
        {
                ShowMessage("�s�K�X��J���~!���ˬd!!");
                return;
        }
        TQuery* pQuery = new TQuery(this);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT * FROM tUserAuthorize");
        pQuery->SQL->Add("WHERE LoginID = :UserName");
        pQuery->ParamByName("UserName")->AsString = Form_Login->LoginID;
        try
        {
                pQuery->Active = true;
                AnsiString RealPas = pQuery->FieldByName("LoginPwd")->AsString;
                pQuery->Active = false;
                if(RealPas != Edit_OldPassword->Text)
                {
                        ShowMessage("�±K�X��J���~,���ˬd!!");
                        delete pQuery;
                        return;
                }
                pQuery->SQL->Clear();
                pQuery->SQL->Add("UPDATE tUserAuthorize");
                pQuery->SQL->Add("SET LoginPwd = :PASSWORD");
                pQuery->SQL->Add("WHERE LoginID = :UserName");
                pQuery->ParamByName("UserName")->AsString = Form_Login->LoginID;
                pQuery->ParamByName("PASSWORD")->AsString = Edit_NewPassword->Text;
                pQuery->ExecSQL();
                Form_Login->LoginPwd = Edit_NewPassword->Text;  //�g�J�s�K�X,�ץ���I�sReport�|�o�ͪ����D
                ShowMessage("��s�K�X���\!");
                Close();
        }
        catch(...)
        {
                ShowMessage("��s�K�X����!");
        }

}
//---------------------------------------------------------------------------
void __fastcall TForm_EditPassword::Button_CancelClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------
void __fastcall TForm_EditPassword::FormShow(TObject *Sender)
{
        Edit_OldPassword->Text = "";
        Edit_NewPassword->Text = "";
        Edit_NewPasRetry->Text = "";
}
//---------------------------------------------------------------------------
