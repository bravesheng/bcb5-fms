//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StatManager.h"
#include "MainForm.h"
#include "DataMudule.h"
#include "LoginForm.h"
#include "ChooseCarForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_ManageState *Form_ManageState;
//---------------------------------------------------------------------------
__fastcall TForm_ManageState::TForm_ManageState(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::ImportEvent(int CarSN, AnsiString CarID, int EventType, AnsiString Descript, AnsiString Handler, AnsiString SendDate, AnsiString SendTime)
{
    try
    {
        int SearchIndex = 0;
        TListItem* pListItem = ListView1->FindCaption(SearchIndex,CarID,false,true,false);
        while(pListItem != NULL)
        {
            if(pListItem->SubItems->Strings[0].ToInt() == EventType)
            {
                //�o�O��ListView�w�g���o��Event,�h�u��s�i���ʪ����
                try
                {
                    pListItem->SubItems->Strings[DESCRIPT] = Descript;
                    pListItem->SubItems->Strings[HANDLER] = Handler;
                    //���y�C��,���B�z����,�B�z������
                    if(Handler == "���B�z")
                    {
                        if(pListItem->ImageIndex != 0)
                        {
                            pListItem->ImageIndex = 0;     //���y�C�⬰RED��
                        }
                    }
                    else
                    {
                        if(pListItem->ImageIndex != 2)
                        {
                            pListItem->ImageIndex = 2;     //���y�C�⬰YELLOW��
                        }
                    }
                }
                catch(...)
                {
                    ShowMessage("���p�����d�ߪ��p�O�_��ʮɵo�Ϳ��~.\n���~��������" + CarID);
                }
                return;         //�w�g�������O���ҥH���X���s�W
            }
            pListItem = ListView1->FindCaption(SearchIndex,CarID,false,true,false);
        }
        //�U���OListView�|�����o��Event,�ҥH�H�s�W���覡�B�z
        TListItem* pItem2 = ListView1->Items->Add();
        pItem2->SubItems->Add(EventType);
        pItem2->SubItems->Add(Descript);
        pItem2->SubItems->Add(Handler);
        pItem2->SubItems->Add(SendDate);
        pItem2->SubItems->Add(SendTime);
        pItem2->SubItems->Add(CarSN);
        pItem2->Caption = CarID;
        //���y�C��,���B�z����,�B�z������
        if(Handler == "���B�z")
        {
            if(pItem2->ImageIndex != 0)
            {
                pItem2->ImageIndex = 0;     //���y�C�⬰RED��
            }
            Show();                //���p�ӵ����p���Q�B�z,�~�N������ܥX��
        }
        else
        {
            if(pItem2->ImageIndex != 2)
            {
                pItem2->ImageIndex = 2;     //���y�C�⬰YELLOW��
            }
        }
    }
    catch(...)
    {}
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    CallCenter_Main_Form->N_StatManager->Checked = false;
    CallCenter_Main_Form->SpeedButton_StatManager->Down = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm_ManageState::N_HandleClick(TObject *Sender)
{
        TListItem* pListItem = ListView1->Selected;
        AnsiString CarID = pListItem->Caption;
        int EventType = pListItem->SubItems->Strings[EVENTTYPE].ToInt();
        AnsiString Handler = pListItem->SubItems->Strings[HANDLER];
        AnsiString SendDate = pListItem->SubItems->Strings[SENDDATE];
        AnsiString SendTime = pListItem->SubItems->Strings[SENDTIME];
        CCarInfo* pCarInfo = Form_ChooseCar->CustomerList->FindCarByCarID(CarID);
        if(EventType == 1)         //���p��Alarm on �ƥ�,�h�����o�e104��Locator
        {
                sCmdMsg Cmd;
                memset(&Cmd,0,sizeof(sCmdMsg));
                strcpy(Cmd.m_szLocatorNo,pCarInfo->LocatorNo.c_str());
                GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                Cmd.m_iCommand = 104;
                strcpy(Cmd.m_szOptions,"");
                strcpy(Cmd.m_szPassword,pCarInfo->LocatorPwd.c_str());
                Cmd.m_iCmdID = G_GetCmdID();
                AnsiString Sender = Form_Login->LoginID;
                strcpy(Cmd.m_szSender,Sender.c_str());
//�U�R�O
                try{//�o�e�R�O -> to Database
    	                DataModule1->Table_Cmd->TableName = "tCmdQueue";
    	                DataModule1->Table_Cmd->Open();
    	                DataModule1->Table_Cmd->Append();
    	                DataModule1->Table_Cmd->FieldValues["LocatorNo"] = Cmd.m_szLocatorNo;
    	                DataModule1->Table_Cmd->FieldValues["CmdDate"] = Cmd.m_szDate;
    	                DataModule1->Table_Cmd->FieldValues["CmdTime"] = Cmd.m_szTime;
    	                DataModule1->Table_Cmd->FieldValues["Command"] = Cmd.m_iCommand;
    	                DataModule1->Table_Cmd->FieldValues["Options"] = Cmd.m_szOptions;
                        TQuery* pQueryCarInfo = new TQuery(DataModule1);
                        pQueryCarInfo->DatabaseName = "DB_DCH";
                        pQueryCarInfo->SQL->Add("SELECT LocatorPwd FROM tCarInfo");
                        pQueryCarInfo->SQL->Add("WHERE LocatorNo = :LocatorNo");
                        pQueryCarInfo->ParamByName("LocatorNo")->AsString = Cmd.m_szLocatorNo;
                        pQueryCarInfo->Active = true;
    	                DataModule1->Table_Cmd->FieldValues["Password"] = pQueryCarInfo->FieldByName("LocatorPwd")->AsString;
                        pQueryCarInfo->Active = false;
                        delete pQueryCarInfo;
                        DataModule1->Table_Cmd->FieldValues["CmdID"] = Cmd.m_iCmdID;
    	                DataModule1->Table_Cmd->FieldValues["Sender"] = Cmd.m_szSender;
                        DataModule1->Table_Cmd->FieldValues["SMPP_TON"] = pCarInfo->SMPP_TON;
                        DataModule1->Table_Cmd->FieldValues["SMPP_NPI"] = pCarInfo->SMPP_NPI;
    	                DataModule1->Table_Cmd->Post();
    	                DataModule1->Table_Cmd->Close();
                }
                catch(...)
                {}
//
                memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
        }
        //���ListView Handler
        pListItem->SubItems->Strings[2] = Form_Login->LoginID;
        //����ƮwHandler���
        //���tEvent
        TQuery* pQuery = new TQuery(Form_ManageState);       //free in last line
        TDateTime* now = new TDateTime();                    //free in next 9 line
        *now = now->CurrentDateTime();
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("UPDATE tEvent");
        pQuery->SQL->Add("SET Handler = :Handler , UpdateTime = :UpdateTime");
        pQuery->SQL->Add("WHERE (CarID = :CarID) AND (EventType = :EventType)");
        pQuery->ParamByName("CarID")->AsString = CarID;
        pQuery->ParamByName("EventType")->AsInteger = EventType;
        pQuery->ParamByName("Handler")->AsString = Form_Login->LoginID;
        pQuery->ParamByName("UpdateTime")->AsDateTime = *now;
        delete now;
        try
        {
                pQuery->ExecSQL();
        }
        catch(...)
        {}
        //���tEventHis
        pQuery->SQL->Clear();
        pQuery->SQL->Add("UPDATE tEventHis");
        pQuery->SQL->Add("SET Handler = :Handler");
        pQuery->SQL->Add("WHERE (CarID = :CarID) AND (EventType = :EventType) AND (SendDate = :SendDate) AND (SendTime = :SendTime)");
        pQuery->ParamByName("CarID")->AsString = CarID;
        pQuery->ParamByName("EventType")->AsInteger = EventType;
        pQuery->ParamByName("Handler")->AsString = Form_Login->LoginID;
        pQuery->ParamByName("SendDate")->AsString = SendDate;
        pQuery->ParamByName("SendTime")->AsString = SendTime;
        try
        {
                pQuery->ExecSQL();
        }
        catch(...)
        {}
        delete pQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::Send_Cmd_Automatic(int EventType, AnsiString CarID)
{
        TListItem* pListItem = NULL;
        for(int i=0; i < ListView1->Items->Count; i++)
        {
                if(ListView1->Items->Item[i]->Caption == CarID)
                {
                        if(ListView1->Items->Item[i]->SubItems->Strings[EVENTTYPE].ToInt() == EventType)
                        {
                                pListItem = ListView1->Items->Item[i];
                                break;
                        }
                }
        }
        if(pListItem == NULL)
        {
                return;
        }
        AnsiString Handler = pListItem->SubItems->Strings[HANDLER];
        if(Handler != "���B�z")        //�קK���ƤU�R�O
        {
                return;
        }
        AnsiString SendDate = pListItem->SubItems->Strings[SENDDATE];
        AnsiString SendTime = pListItem->SubItems->Strings[SENDTIME];
        CCar* m_pCar = CallCenter_Main_Form->GetCarByCarID(CarID);

        if(EventType == 1)         //���p��Alarm on �ƥ�,�h�����o�e104��Locator
        {
                if(m_pCar)  //Return Alarm acknowledgement to Locator to stop the status
                {
                        sCmdMsg Cmd;
                        memset(&Cmd,0,sizeof(sCmdMsg));
                        strcpy(Cmd.m_szLocatorNo,m_pCar->GetLocatorNo());
                        GetNowDateTime(Cmd.m_szDate,Cmd.m_szTime);
                        Cmd.m_iCommand = 104;
                        strcpy(Cmd.m_szOptions,"");
                        strcpy(Cmd.m_szPassword,m_pCar->GetLocatorPwd());
                        Cmd.m_iCmdID = G_GetCmdID();
                        AnsiString Sender = Form_Login->LoginID;
                        strcpy(Cmd.m_szSender,Sender.c_str());
	     	        DataModule1->SendCmdToCar(Cmd, m_pCar);
                        //��tLatestStatus��StatusHandle��窱�A
                        DataModule1->Update_DB_Value("tLatestStatus","StatusHandle",2,"LocatorNo",(AnsiString)Cmd.m_szLocatorNo);
                        memset(&Cmd,0,sizeof(sCmdMsg));         //RESET WHEN FREE
                }
        }
        //���ListView Handler
        pListItem->SubItems->Strings[2] = Form_Login->LoginID;
        //����ƮwHandler���
        //���tEvent
        TQuery* pQuery = new TQuery(Form_ManageState);       //free in last line
        TDateTime* now = new TDateTime();                    //free in next 9 line
        *now = now->CurrentDateTime();
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("UPDATE tEvent");
        pQuery->SQL->Add("SET Handler = :Handler , UpdateTime = :UpdateTime");
        pQuery->SQL->Add("WHERE (CarID = :CarID) AND (EventType = :EventType)");
        pQuery->ParamByName("CarID")->AsString = CarID;
        pQuery->ParamByName("EventType")->AsInteger = EventType;
        pQuery->ParamByName("Handler")->AsString = "RTM Automatic Handle";
        pQuery->ParamByName("UpdateTime")->AsDateTime = *now;
        delete now;
        try
        {
                pQuery->ExecSQL();
        }
        catch(...)
        {}
        //���tEventHis
        pQuery->SQL->Clear();
        pQuery->SQL->Add("UPDATE tEventHis");
        pQuery->SQL->Add("SET Handler = :Handler");
        pQuery->SQL->Add("WHERE (CarID = :CarID) AND (EventType = :EventType) AND (SendDate = :SendDate) AND (SendTime = :SendTime)");
        pQuery->ParamByName("CarID")->AsString = CarID;
        pQuery->ParamByName("EventType")->AsInteger = EventType;
        pQuery->ParamByName("Handler")->AsString = "RTM Automatic Handle";
        pQuery->ParamByName("SendDate")->AsString = SendDate;
        pQuery->ParamByName("SendTime")->AsString = SendTime;
        try
        {
                pQuery->ExecSQL();
        }
        catch(...)
        {}
        delete pQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::N_PositionClick(TObject *Sender)
{
        long CarWGS_X = 0;
        long CarWGS_Y = 0;
        TListItem* pListItem = ListView1->Selected;
        int CarSN = pListItem->SubItems->Strings[CARSN].ToInt();
        TQuery* pQLatestStatus = new TQuery(Form_ManageState);
        pQLatestStatus->DatabaseName = "DB_DCH";
        pQLatestStatus->SQL->Add("SELECT * FROM tLatestStatus");
        pQLatestStatus->SQL->Add("WHERE CSerialNo = :CarSN");
        pQLatestStatus->ParamByName("CarSN")->AsInteger = CarSN;
        try
        {
                pQLatestStatus->Active = true;
                CarWGS_X = pQLatestStatus->FieldByName("Lon")->AsInteger;
                CarWGS_Y = pQLatestStatus->FieldByName("Lat")->AsInteger;
        }
        catch(...)
        {}
        delete pQLatestStatus;
        //Move Locked Car to center of Screen (Image_Map)
        DrawImage(CallCenter_Main_Form->Image_Map,CarWGS_X,CarWGS_Y,CallCenter_Main_Form->m_dScale);
        CallCenter_Main_Form->UpdateAllObjPos();
        //set Center Line
        DrawCenterLine(CallCenter_Main_Form->Image_Map);
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::N_ResetClick(TObject *Sender)
{
        if(MessageDlg("�����U���^�����`�ɨ����|�۰ʳ]�^���`���p.\n�]�w�^���`���p�|�N���������p�k�s,�нT�w�O�_�n�]�^���`���p?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
        {
                TListItem* pListItem = ListView1->Selected;
                AnsiString CarID = pListItem->Caption;
                int EventType = pListItem->SubItems->Strings[0].ToInt();
                //���]�Ө������̷s�˪p
                TQuery* pQ_Reset = new TQuery(Form_ManageState);
                pQ_Reset->DatabaseName = "DB_DCH";
                pQ_Reset->SQL->Add("UPDATE tLatestStatus");
                pQ_Reset->SQL->Add("SET RecvDate='00000000',RecvTime='000000000',SendDate='00000000',SendTime='000000'");
                pQ_Reset->SQL->Add("WHERE LocatorNo = (SELECT LocatorNo FROM tCarInfo WHERE CarID = :CarID)");
                pQ_Reset->ParamByName("CarID")->AsString = CarID;
                try
                {
                        pQ_Reset->ExecSQL();
                }
                catch(...)
                {}
                delete pQ_Reset;
                //������Ʈw�������
                Delete_Event_From_DB(CarID,EventType);
                //��ListView1����
                pListItem->Delete();
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_ManageState::PopupMenu1Popup(TObject *Sender)
{
        if(ListView1->Selected == NULL)
        {
                N_Handle->Visible = false;
                N_Position->Visible = false;
                N_Reset->Visible = false;
        }
        else
        {
                N_Handle->Visible = true;
                N_Position->Visible = true;
                N_Reset->Visible = true;
                if(ListView1->Selected->SubItems->Strings[0] == "1")  //Alarm On���A����ܳ]�^���`���A�\��
                {
                        N_Reset->Visible = false;
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm_ManageState::ListView1DblClick(TObject *Sender)
{
        N_PositionClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::Delete_Event_From_DB(AnsiString CarID, int EventType)
{
        TQuery* pQuery = new TQuery(Form_ManageState);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("DELETE FROM tEvent");
        pQuery->SQL->Add("WHERE (CarID = :CarID) AND (EventType = :EventType)");
        pQuery->ParamByName("CarID")->AsString = CarID;
        pQuery->ParamByName("EventType")->AsInteger = EventType;
        try
        {
                pQuery->ExecSQL();
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        catch(...)
        {}
        delete pQuery;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::PageControl1Change(TObject *Sender)
{
        if(PageControl1->ActivePage == TabSheet_HIS)
        {
                Query_EventHis->Active = false;
                Query_EventHis->Active = true;
        }
        else
        {
                Query_EventHis->Active = false;
        }
}
//---------------------------------------------------------------------------
int __fastcall TForm_ManageState::FindEvent(AnsiString CarID, int EventType)
{
//�HCarID�H��EventType��X�Y�@�Ӫ��p,�åBreturn ��m, ���p�S������return -1
        for(int i=0; i < ListView1->Items->Count; i++)
        {
                if(ListView1->Items->Item[i]->Caption == CarID)
                {
                        if(ListView1->Items->Item[i]->SubItems->Strings[EVENTTYPE] == EventType)
                        {
                                return i;
                        }
                }
        }
        return -1;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ManageState::FormShow(TObject *Sender)
{
        CallCenter_Main_Form->N_StatManager->Checked = true;
        CallCenter_Main_Form->SpeedButton_StatManager->Down = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_ManageState::FormStartDock(TObject *Sender,
      TDragDockObject *&DragObject)
{
    DataModule1->DB_Check_Thread->Suspend();
    CallCenter_Main_Form->Maintain_Thread->Suspend();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ManageState::FormEndDock(TObject *Sender,
      TObject *Target, int X, int Y)
{
    DataModule1->DB_Check_Thread->Resume();
    CallCenter_Main_Form->Maintain_Thread->Resume();    
}
//---------------------------------------------------------------------------

