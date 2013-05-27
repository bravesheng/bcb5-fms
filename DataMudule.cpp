//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataMudule.h"
#include "MainForm.h"
#include "EstablishForm.h"
#include "ViewForm.h"
#include "ChooseCarForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
__fastcall TDataModule1::TDataModule1(TComponent* Owner)
        : TDataModule(Owner)
{
        bCanAccess.Locked = false;
        bCanAccess2.Locked = false;
        bEstablishConnect = false;
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::Update_DB_Value(AnsiString TableName,AnsiString FieldName,Variant FieldValue,AnsiString ConditionName,Variant ConditionValue)
{
/*
UPDATE :TableName
SET    :FieldName = :FieldValue
WHERE  :ConditionName = :ConditionValue
*/
        Query_Update->SQL->Clear();
        Query_Update->SQL->Add("UPDATE " + TableName);
        Query_Update->SQL->Add("SET    " + FieldName + " = :FieldValue");
        Query_Update->SQL->Add("WHERE  " + ConditionName + " = :ConditionValue");
        Query_Update->ParamByName("FieldValue")->Value = FieldValue;
        Query_Update->ParamByName("ConditionValue")->Value = ConditionValue;

        try
        {
                Query_Update->ExecSQL();
        }
        catch(...)
        {
                DataModule1->EstablishConnect(1);
        }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDataModule1::Get_DB_Value(AnsiString TableName,AnsiString FieldName,AnsiString ConditionName,AnsiString ConditionValue)
{
/*
SELECT         :FieldName
FROM           :TableName
WHERE         :ConditionName = :ConditionValue
*/

        Query_Update->Close();
        Query_Update->SQL->Clear();
        Query_Update->SQL->Add("SELECT " + FieldName);
        Query_Update->SQL->Add("FROM   " + TableName);
        Query_Update->SQL->Add("WHERE  " + ConditionName + " = '" + ConditionValue + "'");
        try
        {
                Query_Update->Open();
        }
        catch(...)
        {
                DataModule1->EstablishConnect(1);
        }
        AnsiString tmp = Query_Update->FieldByName(FieldName)->AsString;
        Query_Update->Active = false;
        return tmp;
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::Delete_DB_Value(AnsiString TableName,AnsiString ConditionName,AnsiString ConditionValue)
{
/*
DELETE  FROM :TableName
WHERE :ConditionName = :ConditionValue
*/
        Query_Update->Close();
        Query_Update->SQL->Clear();
        Query_Update->SQL->Add("DELETE FROM " + TableName);
        Query_Update->SQL->Add("WHERE  " + ConditionName + " = " + ConditionValue);
        try
        {
                Query_Update->ExecSQL();
        }
        catch(...)
        {
                DataModule1->EstablishConnect(1);
        }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::QuestRoute()
{
        int RouteNo = 0;
        int PtSerialNo = 0;
        AnsiString CoID;
        AnsiString Note;
        int Lon = 0;
        int Lat = 0;
        int Radius = 0;
        AnsiString StartPoint;
        AnsiString EndPoint;

        Query_Route->Close();
        Query_Route->Open();
        Query_Route->FindFirst();
        AnsiString FirstRouteNo = Query_Route->FieldByName("RouteNo")->AsString;
        if(FirstRouteNo == "")
        {
           Query_Route->Close();
           return;
        }
        if(CallCenter_Main_Form->m_pDefaultSelPath)
                CallCenter_Main_Form->m_pDefaultSelPath->ClearPoint();
        else
                CallCenter_Main_Form->m_pDefaultSelPath = new TCSelectPath(this);
        for(int i=0; i < Query_Route->RecordCount; Query_Route->FindNext())
        {
                i++;
                RouteNo = Query_Route->FieldByName("RouteNo")->AsInteger;
                PtSerialNo = Query_Route->FieldByName("PtSerialNo")->AsInteger;
                CoID = Query_Route->FieldByName("CoID")->AsString;
                Note = Query_Route->FieldByName("Note")->AsString;
                Lon = Query_Route->FieldByName("Lon")->AsInteger;
                Lat = Query_Route->FieldByName("Lat")->AsInteger;
                Radius = Query_Route->FieldByName("Radius")->AsInteger;
                StartPoint = Query_Route->FieldByName("StartPoint")->AsString;
                EndPoint = Query_Route->FieldByName("EndPoint")->AsString;
                if((RouteNo == FirstRouteNo)&&(PtSerialNo==0))             //第一筆資料的檔頭
                {
                        CallCenter_Main_Form->m_pDefaultSelPath->RouteNo = RouteNo;
                        CallCenter_Main_Form->m_pDefaultSelPath->CoID = CoID;
                        CallCenter_Main_Form->m_pDefaultSelPath->Radius = Radius;
                        CallCenter_Main_Form->m_pDefaultSelPath->StartPoint = StartPoint;
                        CallCenter_Main_Form->m_pDefaultSelPath->EndPoint = EndPoint;
                        CallCenter_Main_Form->m_pDefaultSelPath->PathName = Note;
                }
                if((RouteNo!=FirstRouteNo)&&(PtSerialNo==0))             //其他非第一筆料的檔頭
                {
                        CallCenter_Main_Form->m_pSelPathGroupList->Add(CallCenter_Main_Form->m_pDefaultSelPath);   //將該路線加入群組m_pSelPathGroupList中
                        CallCenter_Main_Form->m_pDefaultSelPath = NULL;
                        CallCenter_Main_Form->m_pDefaultSelPath = new TCSelectPath(this);
                        CallCenter_Main_Form->m_pDefaultSelPath->RouteNo = RouteNo;
                        CallCenter_Main_Form->m_pDefaultSelPath->CoID = CoID;
                        CallCenter_Main_Form->m_pDefaultSelPath->Radius = Radius;
                        CallCenter_Main_Form->m_pDefaultSelPath->StartPoint = StartPoint;
                        CallCenter_Main_Form->m_pDefaultSelPath->EndPoint = EndPoint;
                        CallCenter_Main_Form->m_pDefaultSelPath->PathName = Note;
                }
                CallCenter_Main_Form->m_pDefaultSelPath->AddPoint(Lon,Lat);
        }

        CallCenter_Main_Form->m_pSelPathGroupList->Add(CallCenter_Main_Form->m_pDefaultSelPath);        //將該路線加入群組m_pSelPathGroupList中
        CallCenter_Main_Form->m_pDefaultSelPath = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::SendCmdToCar(sCmdMsg& Cmd, CCar* pCar)
{//對車子下令
        try{//發送命令 -> to Database
                CCarInfo* pCarInfo = Form_ChooseCar->CustomerList->FindCarByCarID((AnsiString)pCar->GetCarID());
    	        ChkDatabase(DataModule1->Database_DCH);
    	        Table_Cmd->TableName = "tCmdQueue";
    	        Table_Cmd->Open();
    	        Table_Cmd->Append();
    	        Table_Cmd->FieldValues["LocatorNo"] = Cmd.m_szLocatorNo;
    	        Table_Cmd->FieldValues["CmdDate"] = Cmd.m_szDate;
    	        Table_Cmd->FieldValues["CmdTime"] = Cmd.m_szTime;
    	        Table_Cmd->FieldValues["Command"] = Cmd.m_iCommand;
    	        Table_Cmd->FieldValues["Options"] = Cmd.m_szOptions;
                TQuery* pQuery = new TQuery(DataModule1);
                pQuery->DatabaseName = "DB_DCH";
                pQuery->SQL->Add("SELECT LocatorPwd FROM tCarInfo");
                pQuery->SQL->Add("WHERE LocatorNo = :LocatorNo");
                pQuery->ParamByName("LocatorNo")->AsString = Cmd.m_szLocatorNo;
                pQuery->Active = false;
                pQuery->Active = true;
    	        Table_Cmd->FieldValues["Password"] = pQuery->FieldByName("LocatorPwd")->AsString;
                pQuery->Active = false;
                delete pQuery;
                Table_Cmd->FieldValues["CmdID"] = Cmd.m_iCmdID;
    	        Table_Cmd->FieldValues["Sender"] = Cmd.m_szSender;
                Table_Cmd->FieldValues["SMPP_TON"] = pCarInfo->SMPP_TON;
                Table_Cmd->FieldValues["SMPP_NPI"] = pCarInfo->SMPP_NPI;
    	        Table_Cmd->Post();
    	        Table_Cmd->Close();
        }
        catch(...)
        {}
}
//---------------------------------------------------------------------------
void TDataModule1::ChkDatabase(TDatabase* pDB)
{
    if(!pDB->Connected)
    {
        DataModule1->EstablishConnect(1);
    }
}
//---------------------------------------------------------------------------
bool TDataModule1::TryDB(TDatabase* pDB)
{
	try
        {
                pDB->Connected = false;
                pDB->Connected = true;
        }
        catch(...)
        {
                return false;
        }
        return true;
}
//---------------------------------------------------------------------------
__fastcall TDataModule1::Update_Table_Remarks(CRemarkPlace* pRemark)
{//新增備註地點至資料庫,成功時回傳1,失敗回傳0
        try
        {
                Table_Remarks->Active = false;
                Table_Remarks->Active = true;
                Table_Remarks->Insert();
                Table_Remarks->FieldValues["CoID"] = pRemark->m_szCoID;
                Table_Remarks->FieldValues["Lon"] = pRemark->m_X2;
                Table_Remarks->FieldValues["Lat"] = pRemark->m_Y2;
                Table_Remarks->FieldValues["RemarkType"] = pRemark->m_RemarkType;
                Table_Remarks->FieldValues["Name"] = pRemark->m_strName;
                Table_Remarks->FieldValues["Phone"] = pRemark->m_strPhone;
                Table_Remarks->FieldValues["Address"] = pRemark->m_strAddr;
                Table_Remarks->FieldValues["Email"] = pRemark->m_strEmail;
                Table_Remarks->FieldValues["WebAddress"] = pRemark->m_strWebAddr;
                Table_Remarks->FieldValues["ImagePath"] = pRemark->m_strImagePath;
                Table_Remarks->FieldValues["Memo"] = pRemark->m_strRemarks;
                Table_Remarks->Post();
                pRemark->i_SerialNo = Table_Remarks->FieldByName("SerialNo")->AsInteger;
                Table_Remarks->Active = false;
        }
        catch(...)
        {
                return 0;
        }
        return 1;
}

//---------------------------------------------------------------------------
void __fastcall TDataModule1::EstablishConnect(int Type)
{
//Type == 1(連線斷線時使用)
//Type == 2(User指定切換)
    if(!bEstablishConnect)
    {
        bEstablishConnect = true;
        bCanAccess.Lock();
        bCanAccess2.Lock();       // return true in Timer_Check_If_Establish_ConnectTimer
        if(Form_ChooseCar->Visible == true)
        {
            Form_ChooseCar->Close();
        }
        if(Type == 1)
        {
            Form_Establish_Connect->Label_Statu->Caption = "連線失敗,將進行資料庫切換";
            Form_Establish_Connect->ActiveMode = 1;
        }
        else if(Type == 2)
        {
            Form_Establish_Connect->Label_Statu->Caption = "請指定連線資料庫.";
            Form_Establish_Connect->ActiveMode = 2;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TDataModule1::Timer_Check_If_Establish_ConnectTimer(
      TObject *Sender)
{
//假如Terminate,將Timer停掉
        if(Application->Terminated)
        {
                Timer_Check_If_Establish_Connect->Enabled = false;
                return;
        }
        if(bEstablishConnect)
        {
                CallCenter_Main_Form->Visible = false;
                //刪除Thread以免造成錯誤
                CallCenter_Main_Form->DeleteThread();
                Form_Establish_Connect->Show();
                Form_Establish_Connect->Btn_Retry->Enabled = true;
                //假如是斷線時的自動連接的話,將進行自動連結的動作
                if(Form_Establish_Connect->ActiveMode == 1)
                {
                        //按下連線按鈕
                        Form_Establish_Connect->Btn_RetryClick(this);
                }
                Form_Establish_Connect->Close();
                //重新建立Thread
                CallCenter_Main_Form->RebuildThread();
                bCanAccess.UnLock();
                bCanAccess2.UnLock();
                CallCenter_Main_Form->Visible = true;
                bEstablishConnect = false;
        }
}
//---------------------------------------------------------------------------
CAccessControl::CAccessControl()
{
    //建構函式
    Locked = false;
}
//---------------------------------------------------------------------------
CAccessControl::~CAccessControl()
{
    //解構函式
}
//---------------------------------------------------------------------------
bool CAccessControl::Lock()
{
    //執行Lock動作
    if(Locked == false)
    {
        Locked = true;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
bool CAccessControl::UnLock()
{
    //執行UnLock動作
    if(Locked == true)
    {
        Locked = false;
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
