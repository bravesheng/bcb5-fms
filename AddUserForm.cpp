//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddUserForm.h"
#include "UserManagerForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_AddUser *Form_AddUser;
//---------------------------------------------------------------------------
__fastcall TForm_AddUser::TForm_AddUser(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_AddUser::Button_CancelClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------
void __fastcall TForm_AddUser::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        Action = caFree;
        Form_AddUser = NULL;      
}
//---------------------------------------------------------------------------
void __fastcall TForm_AddUser::Button_BuildClick(TObject *Sender)
{
        if(ComboBox_AutType->ItemIndex == -1)
        {
                ShowMessage("���]�w�v��,�Э��s��J!");
                return;
        }

        if(Edit_Pwd->Text != Edit_RetryPwd->Text)
        {
                ShowMessage("�K�X�]�w���~,�Э��s��J!");
                return;
        }
        TQuery* pQuery = new TQuery(Form_AddUser);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("INSERT tUSerAuthorize");
        pQuery->SQL->Add("(LoginID, LoginName, LoginPwd, AutType, Memo)");
        pQuery->SQL->Add("VALUES (:LoginID, :LoginName, :LoginPwd, :AUtType, :Memo)");
        pQuery->ParamByName("LoginID")->AsString = Edit_LoginID->Text;
        pQuery->ParamByName("LoginName")->AsString = Edit_LoginName->Text;
        pQuery->ParamByName("Memo")->AsString = Edit_Memo->Text;
        pQuery->ParamByName("LoginPwd")->AsString = Edit_Pwd->Text;
        if(ComboBox_AutType->ItemIndex == 0)
        {
                pQuery->ParamByName("AutType")->AsString = "A";
        }
        else if(ComboBox_AutType->ItemIndex == 1)
        {
                pQuery->ParamByName("AutType")->AsString = "M";
        }
        else if(ComboBox_AutType->ItemIndex == 2)
        {
                pQuery->ParamByName("AutType")->AsString = "S";
        }
        else if(ComboBox_AutType->ItemIndex == 3)
        {
                pQuery->ParamByName("AutType")->AsString = "U";
        }
        try
        {
                pQuery->ExecSQL();
        }
        catch(...)
        {
                ShowMessage("��J����,���ˬd�b���O�_����!");
        }
        delete pQuery;
        Form_UserManager->TreeView1Click(Sender);     //��sListView
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_AddUser::Edit_LoginIDChange(TObject *Sender)
{
        Edit_Pwd->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_AddUser::Edit_PwdChange(TObject *Sender)
{
        Edit_RetryPwd->Enabled = true;        
}
//---------------------------------------------------------------------------
void __fastcall TForm_AddUser::Edit_RetryPwdChange(TObject *Sender)
{
        Button_Build->Enabled = true;        
}
//---------------------------------------------------------------------------
