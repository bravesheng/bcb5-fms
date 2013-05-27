//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LoginForm.h"
#include "MainForm.h"
#include "StatManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Login *Form_Login;
//---------------------------------------------------------------------------
__fastcall TForm_Login::TForm_Login(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall TForm_Login::Execute_Login()
{
        Login_Status = 0;
        Edit_UserName->Text = "";
        Edit_Password->Text = "";
        Form_Login->Show();
        while(Login_Status == 0)
        {
                Application->ProcessMessages();
        }
        if(Login_Status == 1)
        {
                LoginID = Edit_UserName->Text;
                LoginPwd = Edit_Password->Text;
                SetAuthorize();
                Close();
                return true;
        }
        Close();
        return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Login::Button_OkClick(TObject *Sender)
{
        if((Edit_Password->Text == NULL) ||(Edit_Password->Text == ""))
        {
                ShowMessage("登錄錯誤,請檢查使用者名稱密碼是否正確!");
                return;
        }
        TQuery* pQuery = new TQuery(this);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT * FROM tUserAuthorize");
        pQuery->SQL->Add("WHERE LoginID = :Username");
        pQuery->ParamByName("Username")->AsString = Edit_UserName->Text;
        try
        {
                pQuery->Active = true;
                if(Edit_Password->Text == pQuery->FieldByName("LoginPwd")->AsString)
                {
                        //登錄成功
                        AutType = pQuery->FieldByName("AutType")->AsString;
                        Login_Status = 1;
                }
                else
                {
                        ShowMessage("登錄錯誤,請檢查使用者名稱密碼是否正確!");
                }
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        pQuery->Active = false;
        delete pQuery;
}
//---------------------------------------------------------------------------

void __fastcall TForm_Login::Button_CancelClick(TObject *Sender)
{
        Login_Status = 2;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Login::SetAuthorize()
{
//Tag == 0則顯示
//Tag == -1則不顯示
        if(AutType == "A")
        {
                CallCenter_Main_Form->N_LVCar_Cmd->Tag = 0;
                CallCenter_Main_Form->N_LVCar_Type->Tag = 0;
                CallCenter_Main_Form->N_UserManager->Enabled = true;
        }
        else if(AutType == "M")
        {
                CallCenter_Main_Form->N_LVCar_Cmd->Tag = 0;
                CallCenter_Main_Form->N_LVCar_Type->Tag = 0;
        }
        else if(AutType == "S")
        {
                CallCenter_Main_Form->N_LVCar_Cmd->Tag = 0;
                CallCenter_Main_Form->N_LVCar_Type->Tag = 0;
        }
        else if(AutType == "U")
        {
                CallCenter_Main_Form->N_LVCar_Cmd->Tag = -1;
                CallCenter_Main_Form->N_LVCar_Type->Tag = -1;
        }
}


void __fastcall TForm_Login::Edit_PasswordKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
	if(Key==VK_RETURN)
                Button_OkClick(Sender);                
}
//---------------------------------------------------------------------------

