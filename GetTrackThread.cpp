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
            if(Terminated) return;          //���������
        }
        if(!DataModule1->bCanAccess2.Locked)
        {
            try
            {
                DiagnoseConnect();                   //�E�_�s�u���p
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
//                Synchronize(CarStatDetect);                     //�������������A�O�_�X�k
                CarStatDetect();
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                Check_If_Update_From_DB();           //�����O�_��Ʈw�����
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
//                Synchronize(GetLatestStatus);        //���o�̷s�������A
                GetLatestStatus();
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                Determine_Car_Image_Color();         //�M�w��ܦb�ù��W�������C��
                if(Terminated)
                {
                    DataModule1->bCanAccess.UnLock();
                    return;
                }
                Check_Acknowledgement();             //�B�zAcknoledge���p
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
            if(Terminated) return;          //���������
        }
        bCanTerminate = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::Check_If_Update_From_DB()
{
        Form_ChooseCar->CustomerList->RenewList();
        if(Terminated) return;          //���������
        CallCenter_Main_Form->Update_CCar_List();       //�ˬdCCarList�O�_�PtCarInfo�P�B
        if(Terminated) return;          //���������
        Sync_Route_List();              //�ˬdRoute List�O�_�P�B
}
//---------------------------------------------------------------------------

void __fastcall TGetTrackThread::GetLatestStatus()
{
//2001.12.25 �ץ��B�@�覡���u���������ʹL�����,�H�[�t�j�q��Ʈɪ��B�z�t��
//2002.5.6 �ץ��B�@�覡�����UpdateTime,����ʹL�~��s
//2002.5.6 �N��Database_Local��Database component�ഫ�ܥ����}��
        sCarStatus CarStatus;
        memset(&CarStatus,0,sizeof(sCarStatus));    //RESET CarStatus ���O����
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
                        {        //��o�ʵ���
                                delete pQArea;
                                delete pQ_LatestStatus;
                                return;
                        }
                        AnsiString LocatorNo = pQ_LatestStatus->FieldByName("LocatorNo")->AsString;
                        CCar* pCar = CallCenter_Main_Form->GetCarByLocatorNo(LocatorNo);
                        if(pCar)
                        {
                                //����ʹL����Ƥ~��s,�HRecvTime�ӧP�_�O�_��ʹL���!
                                TDateTime NewUpdateTime = pQ_LatestStatus->FieldByName("UpdateTime")->AsDateTime;
                                if(pCar->LatestStatusUpdateTime != NewUpdateTime)
                                {
                                        pCar->LatestStatusUpdateTime = NewUpdateTime;
                                        strcpy(CarStatus.m_szLocatorNo,LocatorNo.c_str());
                                        strcpy(CarStatus.m_szRecvDate,pQ_LatestStatus->FieldByName("RecvDate")->AsString.c_str());
                                        //2002.5.10 �ഫRecvTime�������
                                        AnsiString RecvTimeStr = pQ_LatestStatus->FieldByName("RecvTime")->AsString.SubString(1,6);
                                        strcpy(CarStatus.m_szRecvTime,RecvTimeStr.c_str());
                                        strcpy(CarStatus.m_szSendDate,pQ_LatestStatus->FieldByName("SendDate")->AsString.c_str());
                                        strcpy(CarStatus.m_szSendTime,pQ_LatestStatus->FieldByName("SendTime")->AsString.c_str());
                                        CarStatus.m_iStatus = pQ_LatestStatus->FieldByName("CarStatus")->AsInteger;
                                        CarStatus.m_iStatusHandle = pQ_LatestStatus->FieldByName("StatusHandle")->AsInteger;
                                        CarStatus.m_iLon = pQ_LatestStatus->FieldByName("Lon")->AsInteger;
                                        CarStatus.m_iLat = pQ_LatestStatus->FieldByName("Lat")->AsInteger;
                                        CarStatus.m_sSpeed = pQ_LatestStatus->FieldByName("Speed")->AsInteger;
                                        CarStatus.m_dMileage = pQ_LatestStatus->FieldByName("Mileage")->AsFloat;  //��줽��
                                        CarStatus.ReportRate = pQ_LatestStatus->FieldByName("ReportRate")->AsInteger;  //�^���W�v
                                        CarStatus.m_szPlaceName = pQ_LatestStatus->FieldByName("PlaceName")->AsString;
                                        CarStatus.m_iDistID = pQ_LatestStatus->FieldByName("DistID")->AsInteger;         //��F�Ͻs��
                                        //���o��F�Ϥ���W��
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
                                        pCar->ReportCheckRate = pQ_LatestStatus->FieldByName("ReportChkRate")->AsFloat;       //�^���ˬd�W�v
                                        pCar->GPSLock = pQ_LatestStatus->FieldByName("GPSLock")->AsBoolean;
                                        memset(&CarStatus,0,sizeof(sCarStatus));        //RESET CarStatus ���O����
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
        //���p�ثe���p������Ʈw�D�D��Ʈw�h�����D��Ʈw�O�_�w�g��_���`
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
                //���նi������ʧ@
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
        CallCenter_Main_Form->StaticText_ConnectStatus->Caption = "�ϥθ�Ʈw" + DataModule1->Database_DCH->AliasName;
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
//��אּ�PLIS�ۮe�覡
//�Ѹ�ƮwŪ��Event
    TQuery* pQEvent = new TQuery(Form_ManageState);         //Free before catch
    pQEvent->DatabaseName = "DB_DCH";
    pQEvent->SQL->Add("SELECT * FROM tEvent");
    try
    {
        //�Ѹ�Ʈw������D��X��,�M��i����DĲ�o
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
        //�N�C�������D�P��Ʈw�������D���X�Ӥ��,��w�ѨM�����D�R��
        for(int i=0; i<Form_ManageState->ListView1->Items->Count;i++)
        {  //���p�w�g�ѨM�����D�N�N�ӵ���ƧR��
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
            if(pQEvent->RecordCount == 0)        //�������w�S���i��ʱ�,�R��
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
    //�N�N��ƧR��,
    delete pQEvent;
    //�B�zĵ�i�n��
    ProcessSound();
}
//---------------------------------------------------------------------------
void __fastcall TGetTrackThread::ProcessSound()
{
//�B�zĵ�i�n��
    try
    {
        static bool bIsAlertSound = false;
        if(!bIsAlertSound)
        {
            for(int i=0; i < Form_ManageState->ListView1->Items->Count; i++)
            {
                //���p���bdelete thread�hreturn
                if(Terminated)
                {
                    return;
                }
                if(Form_ManageState->ListView1->Items->Item[i]->SubItems->Strings[HANDLER] == "���B�z")
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
                //���p���bdelete thread�hreturn
                if(Terminated)
                {
                    return;
                }
                if(Form_ManageState->ListView1->Items->Item[i]->SubItems->Strings[HANDLER] == "���B�z")
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
//2002.5.9 ���p���l�b�e���W�~����ø�s�ʧ@
//2002.5.16 �ѨM�������B�z����(����),�D�a�Ϩ����C�⥼�ܪ����D,�h���F�����b�e���W�~�e���{��

    try
    {
        //�M�w�����C�⪬�A
        for(int i=0; i<CallCenter_Main_Form->m_pCarList->Count; i++)
        {
            if(Terminated)
            {
                return;
            }
            CCar* pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
            AnsiString CarID = pCar->GetCarID();
            TListItem* pListItem = Form_ViewInfo->GetCarItem(CarID);
            if(pListItem != NULL)    //���p�o�x�����bListView�h�~��
            {
                AnsiString IO = pListItem->SubItems->Strings[IO_STAT];
                if(IO == "")
                {
                    IO = "0000";
                }
                if(IO[pCar->IO_In_Watch] != '1')                        //���pI/O OFF���ɭ�
                {
                    pCar->ImageColor = 5;
                }
                else                                                    //���pI/O ON���ɭ�
                {
                    pCar->ImageColor = 4;
                }
                int ACC = pCar->m_Status.m_iStatus % 4;
                if(ACC < 2) //ACC OFF                                   //���pACC OFF���ɭ�
                {
                    pCar->ImageColor = 3;
                }
                for(int i=0; i<Form_ManageState->ListView1->Items->Count; i++)   //��M�����p������
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
                        if(Handler == "���B�z")                         //���p���p���B�z
                        {
                            pCar->ImageColor = 1;
                        }
                        else
                        {
                            if(pCar->ImageColor != 1)//���p���o�{�L���B�z���A�h�����
                            {
                                pCar->ImageColor = 2;                   //���p���p�B�z��
                            }
                        }
                    }
                }
            }
            else
            {
                //���p�����S���bListView,���Ldefault
                pCar->ImageColor = 5;
            }
        }
    }
    catch(...)
    {}
    try
    {
        //ø�s�����C��
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
                if(pCar->m_pImage_Car->Tag != CarType*5 + pCar->ImageColor)  //���p�P���e���C�⤣�P�~����
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
                        if(pQ_WaitingAck->RecordCount == 1)     //���ӵ��O��
                        {
                                if(Status == 0)   //���p�^���ק粒��
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
                        //�B�z���N�ӵ�tAckHis�R��
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
//���@���w���uList���,�Ϥ��PDatabase�P�B
void __fastcall TGetTrackThread::Sync_Route_List()
{
        //����List�����Ҧ����|�i��аO���|���ˬd
        for(int i=0; i<CallCenter_Main_Form->m_pSelPathGroupList->Count; i++)
        {
                if(Terminated)
                {
                    return;
                }
                TCSelectPath* ptrPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                ptrPath->Vertify = false;
        }
        //�Ѹ�Ʈw����M���,���Memory�O�_���ʤ֩Ϊ̦h�l���u
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
                        if(pRoute != NULL)      //���p��Route�w�g�s�b��O���餤,�h��s��Route�Y�i
                        {
                                pRoute->CoID = pQ_Route->FieldByName("CoID")->AsString;
                                pRoute->PathName = pQ_Route->FieldByName("Note")->AsString;
                                pRoute->Radius = pQ_Route->FieldByName("Radius")->AsInteger;
                                pRoute->Vertify = true;
                        }
                        else       //���p��Route���s�b,�h�إߤ@�ӷs��Route
                        {
                                pRoute = new TCSelectPath(CallCenter_Main_Form);
                                pRoute->QuestData(RouteNo);    //��databaseŪ�����
                                CallCenter_Main_Form->m_pSelPathGroupList->Add(pRoute); //�[�JList
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
        //�N�|���аO�����اR��
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
