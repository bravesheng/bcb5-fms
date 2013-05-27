//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UserManagerForm.h"
#include "SetupPwdForm.h"
#include "AddUserForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_UserManager *Form_UserManager;
//---------------------------------------------------------------------------
__fastcall TForm_UserManager::TForm_UserManager(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_UserManager::TreeView1Click(TObject *Sender)
{
        ListView1->Items->Clear();
        AnsiString AutType = TreeView1->Selected->Text[1];
        TQuery* pQuery = new TQuery(Form_UserManager);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT * FROM tUserAuthorize");
        pQuery->SQL->Add("WHERE AutType = :AutType");
        pQuery->ParamByName("AutType")->AsString = AutType;
        try
        {
                pQuery->Active = true;
                while(!pQuery->Eof)
                {
                        TListItem* pListItem = ListView1->Items->Add();
                        pListItem->Caption = pQuery->FieldByName("LoginID")->AsString;
                        pListItem->SubItems->Add(pQuery->FieldByName("LoginName")->AsString);
                        pListItem->SubItems->Add(pQuery->FieldByName("Memo")->AsString);
                        pQuery->Next();
                }
                pQuery->Active = false;
        }
        catch(...)
        {
        }
        delete pQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm_UserManager::SetPasswdClick(TObject *Sender)
{
        if(!Form_EditPassword1)
        {
                Form_EditPassword1 = new TForm_EditPassword1(this);
                Form_EditPassword1->LoginID = ListView1->Selected->Caption;
                Form_EditPassword1->Show();
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_UserManager::AddUserClick(TObject *Sender)
{
        if(!Form_AddUser)
        {
                Form_AddUser = new TForm_AddUser(this);
        }
        if(TreeView1->Selected->Text == "Administrator")
        {
                Form_AddUser->ComboBox_AutType->ItemIndex = 0;
        }
        else if(TreeView1->Selected->Text == "Manager")
        {
                Form_AddUser->ComboBox_AutType->ItemIndex = 1;
        }
        else if(TreeView1->Selected->Text == "Supervisor")
        {
                Form_AddUser->ComboBox_AutType->ItemIndex = 2;
        }
        else if(TreeView1->Selected->Text == "User")
        {
                Form_AddUser->ComboBox_AutType->ItemIndex = 3;
        }
        Form_AddUser->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm_UserManager::N1Click(TObject *Sender)
{
        TQuery* pQuery = new TQuery(Form_UserManager);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("DELETE FROM tUserAuthorize");
        pQuery->SQL->Add("WHERE LoginID = :LoginID");
        pQuery->ParamByName("LoginID")->AsString = ListView1->Selected->Caption;
        try
        {
                pQuery->ExecSQL();
                TreeView1Click(this);
        }
        catch(...)
        {
                ShowMessage("§R°£¥¢±Ñ!");
        }
        delete pQuery;
}
//---------------------------------------------------------------------------

void __fastcall TForm_UserManager::Popup_UserSetupPopup(TObject *isibleSender)
{
        if(ListView1->SelCount == 0)
        {
                SetPasswd->Visible = false;
                N1->Visible = false;
        }
        else
        {
                SetPasswd->Visible = true;
                N1->Visible = true;
        }
}
//---------------------------------------------------------------------------

