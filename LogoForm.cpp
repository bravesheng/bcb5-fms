//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LogoForm.h"
#include "MainForm.h"
//for Test Displsy Dll
#include "DisplayDll.h"
#include "Common.h"
#include "CustRoute.h"
#include "SettingForm.h"
#include "ViewForm.h"
#include "LoginForm.h"
#include "EstablishForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Logo *Form_Logo;

//---------------------------------------------------------------------------
__fastcall TForm_Logo::TForm_Logo(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TForm_Logo::InitApp()
{
        AnsiString str;
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
        str = ini->ReadString("MAP","Layer",(AnsiString)g_DrawFlag);
        delete ini;
        if(str != "")
                strcpy(g_DrawFlag, str.c_str());
        for(int i=0; i<9; i++)
        {
                if(g_DrawFlag[i] == '1')
                {
                        Form_Setting->LayerBox->State[i] = cbChecked;
                }
        }
        if(Application->Terminated) return false;          //�{��������
        //�ϥΪ̿�ܸ�Ʈw
        Form_Establish_Connect->ChooseDB();
        while(Form_Establish_Connect->bConnected == false)
        {
                Application->ProcessMessages();
                if(Application->Terminated)
                {
                        return false;
                }
        }
        //���pUser���n��,�h�}�ҵn������
        if(Form_Login->LoginID == "")
        {
                if(!Form_Login->Execute_Login())
                        Application->Terminate();
        }
        CallCenter_Main_Form->Visible = true;
        //�إ߸�T����
        if(!Form_ViewInfo)
                Form_ViewInfo = new TForm_ViewInfo(this);
        Form_ViewInfo->Visible = true;
        Form_ViewInfo->ManualDock(CallCenter_Main_Form->Panel_DynBar,Form_ViewInfo,alTop);
        CallCenter_Main_Form->Panel_Street->Visible = true;
       	CallCenter_Main_Form->Panel_Street->ManualDock(CallCenter_Main_Form->Panel_DynBar,CallCenter_Main_Form->Panel_Street,alBottom);
        Application->ProcessMessages();
        //�إ�CustomerList
        if(!Form_ChooseCar->CustomerList)
        {
                Form_ChooseCar->CustomerList = new CCustomerList();
        }
        Form_ChooseCar->CustomerList->RenewList();
        //�إ߰����
        CallCenter_Main_Form->Maintain_Thread = new CTMaintain(true);
        DataModule1->DB_Check_Thread = new TGetTrackThread(true);      // free in MainForm
        //�}��Timer
        CallCenter_Main_Form->Timer1->Enabled = true;        //2002.6.26 added
        Application->ProcessMessages();
        DataModule1->QuestRoute();
        CallCenter_Main_Form->m_bInitialed = true;                
        return(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm_Logo::Image_LogoClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Logo::Memo_VersionInfoClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------



