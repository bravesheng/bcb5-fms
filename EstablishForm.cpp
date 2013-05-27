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
//��User�ӿ�ܩҭn�p������Ʈw����
        TStringList *pStringList = new TStringList();
        DataModule1->Database_DCH->Session->GetDatabaseNames(pStringList);
        ComboBox_BDE->Items = pStringList;
        Show();
}
//---------------------------------------------------------------------------
bool __fastcall TForm_Establish_Connect::ConnectDB()
{
//�����s�ureturn true else treturn false

        Label_Statu->Caption = "�s����Ʈw���A��...";
        Form_Establish_Connect->Repaint();
        Sleep(500);
        if(!DataModule1->TryDB(DataModule1->Database_DCH))
        {
                Label_Statu->Caption = "��Ʈw���A���s�u����!";
                return false;
        }
        Label_Statu->Caption = "���R��Ʈw���...";
        //�i����զs����Ʈw,�ù�ini�g�JDCH_IP��
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
                        //����XMaster DB
                        pQ_SiteInfo->ParamByName("Role")->AsInteger = 0;
                        pQ_SiteInfo->Active = false;
                        pQ_SiteInfo->Active = true;
                        if(pQ_SiteInfo->RecordCount != 1)
                        {
                                Label_Statu->Caption = "��Ʈw�榡tSiteInfo�����T,�п�ܥ��T��Ʈw!";
                                return false;
                        }
                        //�^��Server���򥻺������
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
                        Label_Statu->Caption = "��Ʈw�榡�����T,�п�ܥ��T��Ʈw!";
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
        Label_Statu->Caption = "�����s��";
        Form_Establish_Connect->Repaint();
        Sleep(500);
        return true;
}
//---------------------------------------------------------------------------
bool __fastcall TForm_Establish_Connect::ConnectNet()
{
//���\return true else return false
        Label_Statu->Caption = "���������s�u...";
        Form_Establish_Connect->Repaint();
        if(!InternetGetConnectedState(NULL,0))
        {
                Label_Statu->Caption = "�������u�A���ˬd�������p!";
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
        //�M�w�ҭn�s�i��s�u��database,��X�ثe�Z�q���s�u,��master db��_,��standby db
        Btn_Retry->Enabled = false;
        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "���ճs�u...";
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
                        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "�ϥθ�Ʈw" + DataModule1->Database_DCH->AliasName;
                        Close();
                        return;
                }
                if(ActiveMode == 1)     //�����_�u���p,�~����դU�@��Server�O�_�i�H�s�u
                {
                        for(int i=1; i < ServerList->Count; i++)
                        {
                                CServerInfo* sServer = (CServerInfo*)ServerList->Items[i];
                                DataModule1->Database_DCH->AliasName = sServer->ODBCName;
                                CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "���ճs�u...";
                                if(Application->Terminated)
                                {
                                        Close();
                                        return;
                                }
                                if(ConnectDB())
                                {
                                        DataModule1->bCanAccess.UnLock();
                                        Form_Establish_Connect->bConnected = true;
                                        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "�ϥθ�Ʈw" + DataModule1->Database_DCH->AliasName;
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
        //�M��ServerList����objact, �èS���NServerList Delete��
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

