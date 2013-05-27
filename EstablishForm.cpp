//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EstablishForm.h"
#include "DataMudule.h"
#include "MainForm.h"
#include <wininet.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Establish_Connect *Form_Establish_Connect;
//---------------------------------------------------------------------------
__fastcall TForm_Establish_Connect::TForm_Establish_Connect(TComponent* Owner)
        : TForm(Owner)
{
        ActiveMode = 1;
        ServerList = new TList();      //Free On Destory
        bConnected = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Establish_Connect::ChooseDB()
{
//由User來選擇所要聯結的資料庫類型
        TStringList *pStringList = new TStringList();
        DataModule1->Database_DCH->Session->GetDatabaseNames(pStringList);
        ComboBox_BDE->Items = pStringList;
        Show();
}
//---------------------------------------------------------------------------
bool __fastcall TForm_Establish_Connect::ConnectDB()
{
//完成連線return true else treturn false

        Label_Statu->Caption = "連接資料庫伺服器...";
        Form_Establish_Connect->Repaint();
        Sleep(500);
        if(!DataModule1->TryDB(DataModule1->Database_DCH))
        {
                Label_Statu->Caption = "資料庫伺服器連線失敗!";
                return false;
        }
        Label_Statu->Caption = "分析資料庫欄位...";
        //進行測試存取資料庫,並對ini寫入DCH_IP值
        static bool bGetSiteInfo = false;
        if(!bGetSiteInfo)
        {
                TQuery* pQ_SiteInfo = new TQuery(DataModule1);    //free in next few line
                pQ_SiteInfo->DatabaseName = "DB_DCH";
                pQ_SiteInfo->SQL->Add("SELECT *");
                pQ_SiteInfo->SQL->Add("FROM tSiteInfo");
                pQ_SiteInfo->SQL->Add("WHERE Role = :Role");
                try
                {
                        //先找出Master DB
                        pQ_SiteInfo->ParamByName("Role")->AsInteger = 0;
                        pQ_SiteInfo->Active = false;
                        pQ_SiteInfo->Active = true;
                        if(pQ_SiteInfo->RecordCount != 1)
                        {
                                Label_Statu->Caption = "資料庫格式tSiteInfo不正確,請選擇正確資料庫!";
                                return false;
                        }
                        //擷取Server的基本網路資料
                        DeleteServerList();
                        CServerInfo* MasterServerInfo = new CServerInfo();
                        MasterServerInfo->SiteName = pQ_SiteInfo->FieldByName("SiteName")->AsString;
                        MasterServerInfo->SiteIPAddr = pQ_SiteInfo->FieldByName("SiteIPAddr")->AsString;
                        MasterServerInfo->ODBCName = pQ_SiteInfo->FieldByName("ODBCName")->AsString;
                        ServerList->Add(MasterServerInfo);
                        pQ_SiteInfo->Active = false;
                        pQ_SiteInfo->ParamByName("Role")->AsInteger = 1;
                        pQ_SiteInfo->Active = true;
                        for(pQ_SiteInfo->First(); !pQ_SiteInfo->Eof; pQ_SiteInfo->Next())
                        {
                                CServerInfo* SlaverServerInfo = new CServerInfo();
                                SlaverServerInfo->SiteName = pQ_SiteInfo->FieldByName("SiteName")->AsString;
                                SlaverServerInfo->SiteIPAddr = pQ_SiteInfo->FieldByName("SiteIPAddr")->AsString;
                                SlaverServerInfo->ODBCName = pQ_SiteInfo->FieldByName("ODBCName")->AsString;
                                ServerList->Add(SlaverServerInfo);
                        }
                }
                catch(...)
                {
                        Label_Statu->Caption = "資料庫格式不正確,請選擇正確資料庫!";
                        return false;
                }
                delete pQ_SiteInfo;                             //Free TQuery
                TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));  //Free memory in function buttom
                CServerInfo* pServerInfo = (CServerInfo*)ServerList->Items[0];
                ini->WriteString("DBCHECK","DCH_IP",pServerInfo->SiteIPAddr);
                ini->WriteString("DBCHECK","ODBC_NAME",pServerInfo->ODBCName);
                delete ini;       //Free ini in normal
                bGetSiteInfo = true;
        }
        Label_Statu->Caption = "完成連接";
        Form_Establish_Connect->Repaint();
        Sleep(500);
        return true;
}
//---------------------------------------------------------------------------
bool __fastcall TForm_Establish_Connect::ConnectNet()
{
//成功return true else return false
        Label_Statu->Caption = "偵測網路連線...";
        Form_Establish_Connect->Repaint();
        if(!InternetGetConnectedState(NULL,0))
        {
                Label_Statu->Caption = "網路離線，請檢查網路狀況!";
                return false;
        }
        return true;
}
void __fastcall TForm_Establish_Connect::Btn_IgnoreClick(TObject *Sender)
{
        Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Establish_Connect::Btn_RetryClick(TObject *Sender)
{
        //決定所要連進行連線的database,找出目前暢通的連線,由master db找起,至standby db
        Btn_Retry->Enabled = false;
        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "嘗試連線...";
        Form_Establish_Connect->Repaint();
        if(ConnectNet())
        {
                if(Application->Terminated)
                {
                        Close();
                        return;
                }
                if(ConnectDB())
                {
                        DataModule1->bCanAccess.UnLock();
                        Form_Establish_Connect->bConnected = true;
                        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "使用資料庫" + DataModule1->Database_DCH->AliasName;
                        Close();
                        return;
                }
                if(ActiveMode == 1)     //網路斷線狀況,繼續測試下一個Server是否可以連線
                {
                        for(int i=1; i < ServerList->Count; i++)
                        {
                                CServerInfo* sServer = (CServerInfo*)ServerList->Items[i];
                                DataModule1->Database_DCH->AliasName = sServer->ODBCName;
                                CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "嘗試連線...";
                                if(Application->Terminated)
                                {
                                        Close();
                                        return;
                                }
                                if(ConnectDB())
                                {
                                        DataModule1->bCanAccess.UnLock();
                                        Form_Establish_Connect->bConnected = true;
                                        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "使用資料庫" + DataModule1->Database_DCH->AliasName;
                                        Close();
                                        return;
                                }
                        }
                }
        }
        Btn_Retry->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Establish_Connect::FormShow(TObject *Sender)
{
        DataModule1->bCanAccess.Lock();
        Form_Establish_Connect->bConnected = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Establish_Connect::ComboBox_BDEChange(
      TObject *Sender)
{
        DataModule1->Database_DCH->Close();
        DataModule1->Database_DCH->AliasName = ComboBox_BDE->Text;
        if(ComboBox_BDE->Text != "")
        {
                Btn_Retry->Enabled = true;
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_Establish_Connect::DeleteServerList()
{
        //清除ServerList中的objact, 並沒有將ServerList Delete掉
        for(int i=0; i < ServerList->Count; i++)
        {
                CServerInfo* pServerInfo = (CServerInfo*)ServerList->Items[i];
                delete pServerInfo;
        }
        ServerList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Establish_Connect::FormDestroy(TObject *Sender)
{
        DeleteServerList();
        delete ServerList;
}
//---------------------------------------------------------------------------

