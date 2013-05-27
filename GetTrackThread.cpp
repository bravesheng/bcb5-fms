//2001.10.10 Memory Free Check OK!
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GetTrackThread.h"
#include "MainForm.h"
#include "CustRoute.h"
#include "LogoForm.h"
#include "ViewForm.h"
#include "StatManager.h"
#include <inifiles.hpp>
#include "EstablishForm.h"
#pragma package(smart_init)


//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit2::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TGetTrackThread::TGetTrackThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
        FreeOnTerminate = true;
//        Priority = tpIdle;
        bCanTerminate = false;
        Resume();
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::Execute()
{
    while(!Terminated)
	{
        while(!DataModule1->bCanAccess.Lock())
        {
            Sleep(20);
            if(Terminated) return;          //結束執行緒
        }
        if(!DataModule1->bCanAccess2.Locked)
        {
            try
            {
                DiagnoseConnect();                   //診斷連線狀況
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
//                Synchronize(CarStatDetect);                     //偵測車輛的狀態是否合法
                CarStatDetect();
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                Check_If_Update_From_DB();           //偵測是否資料庫有更動
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
//                Synchronize(GetLatestStatus);        //取得最新車輛狀態
                GetLatestStatus();
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                Determine_Car_Image_Color();         //決定顯示在螢幕上的車輛顏色
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                Check_Acknowledgement();             //處理Acknoledge狀況
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                //ProgressBar1 count
                if(CallCenter_Main_Form->ProgressBar1->Position < 40)
                {
                    CallCenter_Main_Form->ProgressBar1->Position += 10;
                }
                else
                {
                    CallCenter_Main_Form->ProgressBar1->Position = 0;
                }
            }
            catch(...)
            {}
        }
        DataModule1->bCanAccess.UnLock();
        bCanTerminate = true;
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
        int Freq = ini->ReadFloat("TRACK","FREQUENCY",9);
        Freq = Freq * 10;
        delete ini;
        for(int i=0; i < Freq; i++)
        {
            Sleep(100);
            if(Terminated) return;          //結束執行緒
        }
        bCanTerminate = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::Check_If_Update_From_DB()
{
        Form_ChooseCar->CustomerList->RenewList();
        if(Terminated) return;          //結束執行緒
        CallCenter_Main_Form->Update_CCar_List();       //檢查CCarList是否與tCarInfo同步
        if(Terminated) return;          //結束執行緒
        Sync_Route_List();              //檢查Route List是否同步
}
//---------------------------------------------------------------------------

void __fastcall TGetTrackThread::GetLatestStatus()
{
//2001.12.25 修正運作方式為只取近期有更動過之資料,以加速大量資料時的處理速度
//2002.5.6 修正運作方式為比對UpdateTime,有更動過才更新
//2002.5.6 將由Database_Local的Database component轉換至本身開啟
        sCarStatus CarStatus;
        memset(&CarStatus,0,sizeof(sCarStatus));    //RESET CarStatus 的記憶體
        TQuery* pQArea = new TQuery(NULL);      //free in last line
        pQArea->DatabaseName = "DB_DCH";
        pQArea->SQL->Add("SELECT * FROM tArea");
        TQuery* pQ_LatestStatus = new TQuery(NULL);  //free in the last line
        pQ_LatestStatus->DatabaseName = "DB_DCH";
        pQ_LatestStatus->SQL->Add("SELECT * FROM tLatestStatus");
        try
        {
                pQArea->Active = true;
                for(pQ_LatestStatus->Active = true; !pQ_LatestStatus->Eof; pQ_LatestStatus->Next())
                {
                        if(Terminated)
                        {        //突發性結束
                                delete pQArea;
                                delete pQ_LatestStatus;
                                return;
                        }
                        AnsiString LocatorNo = pQ_LatestStatus->FieldByName("LocatorNo")->AsString;
                        CCar* pCar = CallCenter_Main_Form->GetCarByLocatorNo(LocatorNo);
                        if(pCar)
                        {
                                //有更動過的資料才更新,以RecvTime來判斷是否更動過資料!
                                TDateTime NewUpdateTime = pQ_LatestStatus->FieldByName("UpdateTime")->AsDateTime;
                                if(pCar->LatestStatusUpdateTime != NewUpdateTime)
                                {
                                        pCar->LatestStatusUpdateTime = NewUpdateTime;
                                        strcpy(CarStatus.m_szLocatorNo,LocatorNo.c_str());
                                        strcpy(CarStatus.m_szRecvDate,pQ_LatestStatus->FieldByName("RecvDate")->AsString.c_str());
                                        //2002.5.10 轉換RecvTime為六位數
                                        AnsiString RecvTimeStr = pQ_LatestStatus->FieldByName("RecvTime")->AsString.SubString(1,6);
                                        strcpy(CarStatus.m_szRecvTime,RecvTimeStr.c_str());
                                        strcpy(CarStatus.m_szSendDate,pQ_LatestStatus->FieldByName("SendDate")->AsString.c_str());
                                        strcpy(CarStatus.m_szSendTime,pQ_LatestStatus->FieldByName("SendTime")->AsString.c_str());
                                        CarStatus.m_iStatus = pQ_LatestStatus->FieldByName("CarStatus")->AsInteger;
                                        CarStatus.m_iStatusHandle = pQ_LatestStatus->FieldByName("StatusHandle")->AsInteger;
                                        CarStatus.m_iLon = pQ_LatestStatus->FieldByName("Lon")->AsInteger;
                                        CarStatus.m_iLat = pQ_LatestStatus->FieldByName("Lat")->AsInteger;
                                        CarStatus.m_sSpeed = pQ_LatestStatus->FieldByName("Speed")->AsInteger;
                                        CarStatus.m_dMileage = pQ_LatestStatus->FieldByName("Mileage")->AsFloat;  //單位公里
                                        CarStatus.ReportRate = pQ_LatestStatus->FieldByName("ReportRate")->AsInteger;  //回報頻率
                                        CarStatus.m_szPlaceName = pQ_LatestStatus->FieldByName("PlaceName")->AsString;
                                        CarStatus.m_iDistID = pQ_LatestStatus->FieldByName("DistID")->AsInteger;         //行政區編號
                                        //取得行政區中文名稱
                                        CarStatus.DistName = NULL;
                                        for(pQArea->First(); !pQArea->Eof; pQArea->Next())
                                        {
                                                if(CarStatus.m_iDistID == pQArea->FieldByName("AreaID")->AsInteger)
                                                {
                                                        CarStatus.DistName = pQArea->FieldByName("CName")->AsString;
                                                        break;
                                                }
                                        }
                                        pCar->Update(&CarStatus);
                                        pCar->ReportCheckRate = pQ_LatestStatus->FieldByName("ReportChkRate")->AsFloat;       //回報檢查頻率
                                        pCar->GPSLock = pQ_LatestStatus->FieldByName("GPSLock")->AsBoolean;
                                        memset(&CarStatus,0,sizeof(sCarStatus));        //RESET CarStatus 的記憶體
                                }
                        }
                }
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        catch(...)
        {
        }
        delete pQArea;
        delete pQ_LatestStatus;
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::DiagnoseConnect()
{
//"DBCHECK","DCH","true"
        //假如目前所聯結的資料庫非主資料庫則偵測主資料庫是否已經恢復正常
        CServerInfo* pServerInfo = (CServerInfo*)Form_Establish_Connect->ServerList->Items[0];
        if(DataModule1->Database_DCH->AliasName == pServerInfo->ODBCName)
        {
                try
                {
                        DataModule1->Database_TEMP->Close();
                        DataModule1->Database_TEMP->AliasName = pServerInfo->ODBCName;
                        DataModule1->Database_TEMP->Open();
                }
                catch(...)
                {
                        DataModule1->EstablishConnect(1);
                }
                DataModule1->Database_TEMP->Close();
        }
        else
        {
                //嘗試進行切換動作
                try
                {
                        DataModule1->Database_TEMP->Close();
                        DataModule1->Database_TEMP->AliasName = pServerInfo->ODBCName;
                        DataModule1->Database_TEMP->Open();
                        if(DataModule1->Database_TEMP->Connected)
                        {
                                DataModule1->Database_TEMP->Close();
                                DataModule1->Database_DCH->Close();
                                DataModule1->Database_DCH->AliasName = pServerInfo->ODBCName;
                                DataModule1->Database_DCH->Open();
                        }
                }
                catch(...)
                {
                        DataModule1->Database_TEMP->Close();
                }
        }
        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "使用資料庫" + DataModule1->Database_DCH->AliasName;
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::OpenDBDetecotr()
{
    if(!ShowReportEXE("TForm_DBDetecter",NULL))
    {
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));        //free in next 2 line
        ini->WriteString("DBCHECK","DCH","false");
        delete ini;
    	ShellExecute(CallCenter_Main_Form, "open","DBDetector.exe", NULL, NULL, NULL);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TGetTrackThread::ShowReportEXE(char* szClassName,char* szWndName)
{
	HWND hWnd = FindWindow(szClassName,szWndName);
        if(hWnd)
        {
                return true;
        }
        return false;
}
//---------------------------------------------------------------------------
bool __fastcall TGetTrackThread::CloseReportEXE(char* szClassName,char* szWndName)
{
	HWND hWnd = FindWindow(szClassName,szWndName); //Report Generator define in Common.h
        if(hWnd)
        {
    	        PostMessage(hWnd,WM_CLOSE,0,0);
                return true;
        }
    	return false;

}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::CarStatDetect()
{
//更改為與LIS相容方式
//由資料庫讀取Event
    TQuery* pQEvent = new TQuery(Form_ManageState);         //Free before catch
    pQEvent->DatabaseName = "DB_DCH";
    pQEvent->SQL->Add("SELECT * FROM tEvent");
    try
    {
        //由資料庫中把問題找出來,然後進行問題觸發
        pQEvent->Active = false;
        pQEvent->Active = true;
        for(pQEvent->First(); !pQEvent->Eof; pQEvent->Next())
        {
            if(Terminated)
            {
                delete pQEvent;
                return;
            }
            Application->ProcessMessages();     //2002.7.15 added
            AnsiString CarID = pQEvent->FieldByName("CarID")->AsString;
            int EventType = pQEvent->FieldByName("EventType")->AsInteger;
            AnsiString Descript = pQEvent->FieldByName("Descript")->AsString;
            int CarSN = pQEvent->FieldByName("CarSN")->AsInteger;
            AnsiString Handler = pQEvent->FieldByName("Handler")->AsString;
            AnsiString SendDate = pQEvent->FieldByName("SendDate")->AsString;
            AnsiString SendTime = pQEvent->FieldByName("SendTime")->AsString;
            Form_ManageState->ImportEvent(CarSN,CarID,EventType,Descript,Handler,SendDate,SendTime);
        }
        pQEvent->Active = false;
        //將列表中的問題與資料庫中的問題拿出來比較,把已解決的問題刪除
        for(int i=0; i<Form_ManageState->ListView1->Items->Count;i++)
        {  //假如已經解決的問題就將該筆資料刪除
            if(Terminated)
            {
                delete pQEvent;
                return;
            }
            Application->ProcessMessages();     //2002.7.15 added
            TListItem* pListItem = Form_ManageState->ListView1->Items->Item[i];
            AnsiString CarID = pListItem->Caption;
            int EventType = pListItem->SubItems->Strings[0].ToInt();
            pQEvent->SQL->Clear();
            pQEvent->SQL->Add("SELECT * FROM tEvent");
            pQEvent->SQL->Add("WHERE CarID = :CarID");
            pQEvent->SQL->Add("AND EventType = :EventType");
            pQEvent->ParamByName("CarID")->AsString = CarID;
            pQEvent->ParamByName("EventType")->AsInteger = EventType;
            pQEvent->Active = false;
            pQEvent->Active = true;
            if(pQEvent->RecordCount == 0)        //此車輛已沒有進行監控,刪除
            {
                pListItem->Delete();
                i--;
            }
            pQEvent->Active = false;
        }
    }
    catch(EDBEngineError &E)
    {
        DataModule1->EstablishConnect(1);
    }
    catch(...)
    {}
    //將就資料刪除,
    delete pQEvent;
    //處理警告聲音
    ProcessSound();
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::ProcessSound()
{
//處理警告聲音
    try
    {
        static bool bIsAlertSound = false;
        if(!bIsAlertSound)
        {
            for(int i=0; i < Form_ManageState->ListView1->Items->Count; i++)
            {
                //假如正在delete thread則return
                if(Terminated)
                {
                    return;
                }
                if(Form_ManageState->ListView1->Items->Item[i]->SubItems->Strings[HANDLER] == "未處理")
                {
                    if(Form_ManageState->Visible == true)
                    {
                        bIsAlertSound = true;
                        ::sndPlaySound(ALERT_SOUND_PATH,SND_ASYNC|SND_LOOP);
                    }
                    return;
                }
            }
        }
        else
        {
            bIsAlertSound = false;
            for(int i=0; i < Form_ManageState->ListView1->Items->Count; i++)
            {
                //假如正在delete thread則return
                if(Terminated)
                {
                    return;
                }
                if(Form_ManageState->ListView1->Items->Item[i]->SubItems->Strings[HANDLER] == "未處理")
                {
                    if(Form_ManageState->Visible == true)
                    {
                        bIsAlertSound = true;
                    }
                }
            }
            if(bIsAlertSound == false)
            {
                ::sndPlaySound(ALERT_SOUND_PATH,NULL);
            }
        }
    }
    catch(...)
    {}
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::Determine_Car_Image_Color()
{
//2002.5.9 假如車子在畫面上才執行繪製動作
//2002.5.16 解決當車輛狀處理中時(黃色),主地圖車輛顏色未變的問題,去掉了車輛在畫面上才畫的程式

    try
    {
        //決定車輛顏色狀態
        for(int i=0; i<CallCenter_Main_Form->m_pCarList->Count; i++)
        {
            if(Terminated)
            {
                return;
            }
            CCar* pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
            AnsiString CarID = pCar->GetCarID();
            TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
            if(pListItem != NULL)    //假如這台車有在ListView則才做
            {
                AnsiString IO = pListItem->SubItems->Strings[IO_STAT];
                if(IO == "")
                {
                    IO = "0000";
                }
                if(IO[pCar->IO_In_Watch] != '1')                        //假如I/O OFF的時候
                {
                    pCar->ImageColor = 5;
                }
                else                                                    //假如I/O ON的時候
                {
                    pCar->ImageColor = 4;
                }
                int ACC = pCar->m_Status.m_iStatus % 4;
                if(ACC < 2) //ACC OFF                                   //假如ACC OFF的時候
                {
                    pCar->ImageColor = 3;
                }
                for(int i=0; i<Form_ManageState->ListView1->Items->Count; i++)   //找尋有狀況的車輛
                {
                    if(Terminated)
                    {
                        return;
                    }
                    TListItem* pListItem = Form_ManageState->ListView1->Items->Item[i];
                    AnsiString CarID = pListItem->Caption;
                    AnsiString Handler = pListItem->SubItems->Strings[2];
                    AnsiString pCarID = pCar->GetCarID();
                    if(CarID == pCarID)
                    {
                        if(Handler == "未處理")                         //假如狀況未處理
                        {
                            pCar->ImageColor = 1;
                        }
                        else
                        {
                            if(pCar->ImageColor != 1)//假如有發現過未處理狀態則不更動
                            {
                                pCar->ImageColor = 2;                   //假如狀況處理中
                            }
                        }
                    }
                }
            }
            else
            {
                //假如車輛沒有在ListView,給他default
                pCar->ImageColor = 5;
            }
        }
    }
    catch(...)
    {}
    try
    {
        //繪製車輛顏色
        for(int i=0; i<CallCenter_Main_Form->m_pCarList->Count; i++)
        {
            if(Terminated)
            {
                return;
            }
            CCar* pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
            if(pCar->m_pImage_Car->Tag > 0)//Unlock
            {
                int CarType = 0;
                int TmpVal = pCar->m_pImage_Car->Tag;
                while(TmpVal > 5)
                {
                    CarType++;
                    TmpVal = TmpVal - 5;
                }
                if(pCar->m_pImage_Car->Tag != CarType*5 + pCar->ImageColor)  //假如與之前的顏色不同才換色
                {
                    pCar->m_pImage_Car->Tag = CarType*5 + pCar->ImageColor;
                    CallCenter_Main_Form->ChangeCarImgByTag(pCar->m_pImage_Car,1);
                }
            }
            else    //Locked
            {
                pCar->m_pImage_Car->Tag = -pCar->m_pImage_Car->Tag;
                int CarType = 0;
                int TmpVal = pCar->m_pImage_Car->Tag;
                while(TmpVal > 5)
                {
                    CarType++;
                    TmpVal = TmpVal - 5;
                }
                pCar->m_pImage_Car->Tag = -1 * (CarType*5 + pCar->ImageColor);
                CallCenter_Main_Form->ChangeCarImgByTag(pCar->m_pImage_Car,1);
            }
        }
    }
    catch(...)
    {}
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::Check_Acknowledgement()
{
        TQuery* pQ_AckHis = new TQuery(DataModule1);       //free in the latest
        pQ_AckHis->DatabaseName = "DB_DCH";
        pQ_AckHis->SQL->Add("SELECT * FROM tAckHis");
        TQuery* pQ_WaitingAck = new TQuery(DataModule1);   //free in the latest
        pQ_WaitingAck->DatabaseName = "DB_DCH";
        try
        {
                pQ_AckHis->Active = true;
                while(pQ_AckHis->Eof == false)
                {
                        AnsiString LocatorNo = pQ_AckHis->FieldByName("LocatorNo")->AsString;
                        int CmdID = pQ_AckHis->FieldByName("CmdID")->AsInteger;
                        int Status = pQ_AckHis->FieldByName("Status")->AsInteger;
                        pQ_WaitingAck->SQL->Clear();
                        pQ_WaitingAck->SQL->Add("SELECT * FROM tWaitingAck");
                        pQ_WaitingAck->SQL->Add("WHERE (LocatorNo = :LocatorNo) AND (CmdID = :CmdID)");
                        pQ_WaitingAck->ParamByName("LocatorNo")->AsString = LocatorNo;
                        pQ_WaitingAck->ParamByName("CmdID")->AsString = CmdID;
                        pQ_WaitingAck->Active = false;
                        pQ_WaitingAck->Active = true;
                        if(pQ_WaitingAck->RecordCount == 1)     //找到該筆記錄
                        {
                                if(Status == 0)   //假如回應修改完成
                                {
                                        AnsiString Password = pQ_WaitingAck->FieldByName("Password")->AsString;
                                        TQuery* pQ_CarInfo = new TQuery(DataModule1); //delete in the loop
                                        pQ_CarInfo->DatabaseName = "DB_DCH";
                                        pQ_CarInfo->SQL->Add("UPDATE tCarInfo");
                                        pQ_CarInfo->SQL->Add("SET LocatorPwd = :Password");
                                        pQ_CarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                                        pQ_CarInfo->ParamByName("Password")->AsString = Password;
                                        pQ_CarInfo->ParamByName("LocatorNo")->AsString = LocatorNo;
                                        try
                                        {
                                                pQ_CarInfo->ExecSQL();
                                        }
                                        catch(...)
                                        {
                                        }
                                        delete pQ_CarInfo;
                                }
                                pQ_WaitingAck->Active = false;
                                pQ_WaitingAck->SQL->Clear();
                                pQ_WaitingAck->SQL->Add("DELETE FROM tWaitingAck");
                                pQ_WaitingAck->SQL->Add("WHERE (LocatorNo = :LocatorNo) AND (CmdID = :CmdID)");
                                pQ_WaitingAck->ParamByName("LocatorNo")->AsString = LocatorNo;
                                pQ_WaitingAck->ParamByName("CmdID")->AsInteger = CmdID;
                                pQ_WaitingAck->ExecSQL();
                        }
                        //處理完將該筆tAckHis刪除
                        TQuery* pQ_DelAckHis = new TQuery(DataModule1);  //free in below
                        pQ_DelAckHis->DatabaseName = "DB_DCH";
                        pQ_DelAckHis->SQL->Add("DELETE FROM tAckHis");
                        pQ_DelAckHis->SQL->Add("WHERE (LocatorNo = :LocatorNo) AND (CmdID = :CmdID)");
                        pQ_DelAckHis->ParamByName("LocatorNo")->AsString = LocatorNo;
                        pQ_DelAckHis->ParamByName("CmdID")->AsInteger = CmdID;
                        pQ_DelAckHis->ExecSQL();
                        delete pQ_DelAckHis;
                        pQ_AckHis->Active = false;
                        pQ_AckHis->Active = true;
                }
                pQ_AckHis->Active = false;
        }
        catch(...)
        {
        }
        delete pQ_WaitingAck;
        delete pQ_AckHis;
}
//---------------------------------------------------------------------------
//維護指定路線List資料,使之與Database同步
void __fastcall TGetTrackThread::Sync_Route_List()
{
        //先對List中的所有路徑進行標記為尚未檢查
        for(int i=0; i<CallCenter_Main_Form->m_pSelPathGroupList->Count; i++)
        {
                if(Terminated)
                {
                    return;
                }
                TCSelectPath* ptrPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                ptrPath->Vertify = false;
        }
        //由資料庫中找尋資料,比對Memory是否有缺少或者多餘路線
        TQuery* pQ_Route = new TQuery(DataModule1);        //free in the last line
        pQ_Route->DatabaseName = "DB_DCH";
        pQ_Route->SQL->Add("SELECT * FROM tCustAutRoute");
        pQ_Route->SQL->Add("WHERE PtSerialNo = 0");
        try
        {
                pQ_Route->Active = false;
                pQ_Route->Active = true;
                for(pQ_Route->First(); !pQ_Route->Eof; pQ_Route->Next())
                {
                        if(Terminated)
                        {
                            delete pQ_Route;
                            return;
                        }
                        int RouteNo = pQ_Route->FieldByName("RouteNo")->AsInteger;
                        TCSelectPath* pRoute = CallCenter_Main_Form->GetRoute(RouteNo);
                        if(pRoute != NULL)      //假如該Route已經存在於記憶體中,則更新該Route即可
                        {
                                pRoute->CoID = pQ_Route->FieldByName("CoID")->AsString;
                                pRoute->PathName = pQ_Route->FieldByName("Note")->AsString;
                                pRoute->Radius = pQ_Route->FieldByName("Radius")->AsInteger;
                                pRoute->Vertify = true;
                        }
                        else       //假如該Route未存在,則建立一個新的Route
                        {
                                pRoute = new TCSelectPath(CallCenter_Main_Form);
                                pRoute->QuestData(RouteNo);    //由database讀取資料
                                CallCenter_Main_Form->m_pSelPathGroupList->Add(pRoute); //加入List
                        }
                }
                pQ_Route->Active = false;
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        catch(...)
        {
        }
        delete pQ_Route;
        //將尚未標記的項目刪除
        for(int i=0; i<CallCenter_Main_Form->m_pSelPathGroupList->Count; i++)
        {
                if(Terminated)
                {
                    return;
                }
                TCSelectPath* ptrPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                if(ptrPath->Vertify == false)
                {
                        delete ptrPath;
                        CallCenter_Main_Form->m_pSelPathGroupList->Delete(i);
                        i--;
                }
        }
}
