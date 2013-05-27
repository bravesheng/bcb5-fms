//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TripForm.h"
#include "HisTracer.h"
#include "MainForm.h"
#include "ChooseCarForm.h"
#include "ViewForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TraceForm"
#pragma resource "*.dfm"
TForm_Trip *Form_Trip;
//---------------------------------------------------------------------------
__fastcall TForm_Trip::TForm_Trip(TComponent* Owner)
        : TForm_Trace(Owner)
{
        LocatorNoBoxDropDown(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm_Trip::FormShow(TObject *Sender)
{
        CallCenter_Main_Form->SpeedButton_Trip->Down = true;
        CallCenter_Main_Form->N_Trip->Checked = true;
        CallCenter_Main_Form->Menu_HisTrip->Checked = true;
        //�]�w�w�]���ʱ��ɶ�
        StartDatePicker->DateTime = StartDatePicker->DateTime.CurrentDateTime();
        StartTimePicker->DateTime = StartTimePicker->DateTime.CurrentDate();
        EndDatePicker->DateTime = EndDatePicker->DateTime.CurrentDateTime();
        EndTimePicker->DateTime = EndTimePicker->DateTime.CurrentDateTime();
}
//---------------------------------------------------------------------------
void __fastcall TForm_Trip::TraceListViewDblClick(TObject *Sender)
{
	if(TraceListView->SelCount == 1)
        {
                TListItem* pListItem;
                pListItem = TraceListView->Selected;
                CCarInfo* pCarInfo = Form_ChooseCar->CustomerList->FindCarByCarID(LocatorNoBox->Items->Strings[LocatorNoBox->ItemIndex]);
                CHisTracer->LocatorNo = pCarInfo->LocatorNo;
                CHisTracer->StartDate = pListItem->Caption;
                CHisTracer->StartTime = pListItem->SubItems->Strings[START_TIME];
                CHisTracer->EndDate = pListItem->SubItems->Strings[END_DATE];
                CHisTracer->EndTime = pListItem->SubItems->Strings[END_TIME];
                CHisTracer->Execute();
                CCar *ptrCar = NULL;   //���j�M��Car class
                //�M���Ҧ����l�����v���|
                ClearAllTrace_InMemory();
                //���wptrCar�ҫ������l
                for(int i=0; i < CallCenter_Main_Form->m_pCarList->Count; i++)
                {
                        ptrCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
                        AnsiString LocNo = ptrCar->GetLocatorNo();
                        if(LocNo == CHisTracer->LocatorNo)
                        break;
                }
                if(!ptrCar)
                        return;
                //if History Track exist->free List , else ->create new list
                if(ptrCar->m_pHisCarTraceList==NULL)
                {  //Create New History Track List
                        ptrCar->m_pHisCarTraceList = new TList();
                        //Add sSelectPoint Point in CCar::Update()
                        //Draw Car Trace in DrawImage()
                }
                else
                {
                        //Free �¾��v�y����
                        for(int i=0;i<ptrCar->m_pHisCarTraceList->Count;i++)
                        delete ((sSelectPoint*)ptrCar->m_pHisCarTraceList->Items[i]);
                        ptrCar->m_pHisCarTraceList->Clear();
                }
                sSelectPoint* pTrack;
                //�N���|�I��JCCar
                for(vector<TCHisTracer::sCarStat>::iterator it = CHisTracer->VectorCarStat.begin();it != CHisTracer->VectorCarStat.end(); ++it)
                {
                        AnsiString tmpstr;
                        pTrack = new sSelectPoint;
                        pTrack->mX2 = it->m_iLon;
                        pTrack->mY2 = it->m_iLat;
                        ptrCar->m_pHisCarTraceList->Add(pTrack);   //Add new to TList
                        //�N�a�ϲ��ܸӸ��u�Ĥ@�I��m
                        if(it == CHisTracer->VectorCarStat.begin())
                        {
                                DrawImage(CallCenter_Main_Form->Image_Map,it->m_iLon,it->m_iLat,CallCenter_Main_Form->m_dScale);
                                CallCenter_Main_Form->UpdateAllObjPos();
                        }
                }
                ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_Trip::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        CallCenter_Main_Form->SpeedButton_Trip->Down = false;
        CallCenter_Main_Form->N_Trip->Checked = false;
        CallCenter_Main_Form->Menu_HisTrip->Checked = false;
        ClearAllTrace_InMemory();
        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
        Action = caFree;
        Form_Trip = NULL;        
}
//---------------------------------------------------------------------------
void __fastcall TForm_Trip::LocatorNoBoxDropDown(TObject *Sender)
{
        //��JLocatorNoBox�ثe�ʱ�������
        LocatorNoBox->Items->Clear();
        for(int i=0; i < Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                LocatorNoBox->Items->Add(Form_ViewInfo->ListView_Car->Items->Item[i]->Caption);
        }
}
//---------------------------------------------------------------------------
AnsiString TForm_Trip::GetPeriod(AnsiString TimeA, AnsiString TimeB)
{
//����ⵧ�ɶ��r��,��X��ɶ��t�Z
        AnsiString BigStr, SmallStr;
        if(TimeA > TimeB)
        {
                BigStr = TimeA;
                SmallStr = TimeB;
        }
        else
        {
                SmallStr = TimeA;
                BigStr = TimeB;
        }
        AnsiString DateTimeStr = BigStr.SubString(5,2);
        DateTimeStr = DateTimeStr + "/";
        DateTimeStr = DateTimeStr + BigStr.SubString(7,2);
        DateTimeStr = DateTimeStr + "/";
        DateTimeStr = DateTimeStr + BigStr.SubString(1,4);
        DateTimeStr = DateTimeStr + " ";
        DateTimeStr = DateTimeStr + BigStr.SubString(9,2);
        DateTimeStr = DateTimeStr + ":";
        DateTimeStr = DateTimeStr + BigStr.SubString(11,2);
        DateTimeStr = DateTimeStr + ":";
        DateTimeStr = DateTimeStr + BigStr.SubString(13,2);
        ShortDateFormat = "m/d/yyyy";
        TDateTime BigTime = StrToDateTime(DateTimeStr);
        DateTimeStr = SmallStr.SubString(5,2);;
        DateTimeStr = DateTimeStr + "/";
        DateTimeStr = DateTimeStr + SmallStr.SubString(7,2);
        DateTimeStr = DateTimeStr + "/";
        DateTimeStr = DateTimeStr + SmallStr.SubString(1,4);
        DateTimeStr = DateTimeStr + " ";
        DateTimeStr = DateTimeStr + SmallStr.SubString(9,2);
        DateTimeStr = DateTimeStr + ":";
        DateTimeStr = DateTimeStr + SmallStr.SubString(11,2);
        DateTimeStr = DateTimeStr + ":";
        DateTimeStr = DateTimeStr + SmallStr.SubString(13,2);
        TDateTime SmallTime = StrToDateTime(DateTimeStr);
        BigTime = BigTime - SmallTime;
        SmallTime = 1;
        int day = BigTime;
        if( BigTime > SmallTime)
        {
                float tmp = day;
                BigTime = BigTime - tmp;
        }
        if(day > 0)
        {
                return (IntToStr(day) + "day " + FormatDateTime("hh:mm",BigTime));
        }
        return (FormatDateTime("hh:mm",BigTime));

}
//---------------------------------------------------------------------------
void __fastcall TForm_Trip::SpeedButton1Click(TObject *Sender)
{
//�N��ܤ��������y��C�X
        if(LocatorNoBox->ItemIndex == -1)
        {
                ShowMessage("����ܨ���!!");
                return;
        }
        TQuery *pQuery = new TQuery(DataModule1);
        TQuery *pQuery2 = new TQuery(DataModule1);
        pQuery->DatabaseName = "DB_DCH";
        pQuery2->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT * FROM tStatusHis");
        pQuery->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery->SQL->Add("AND SendDate >= :StartDate");
        pQuery->SQL->Add("AND SendDate <= :EndDate");
        pQuery->SQL->Add("AND Cancel <> 1");       //Cancel==1���Ȭ��R������
        pQuery->SQL->Add("ORDER BY SendDate, SendTime");     //�Ƨ�
        CCarInfo* pCarInfo = Form_ChooseCar->CustomerList->FindCarByCarID(LocatorNoBox->Items->Strings[LocatorNoBox->ItemIndex]);
        pQuery->ParamByName("LocatorNo")->AsString = pCarInfo->LocatorNo;
        pQuery->ParamByName("StartDate")->AsString = StartDatePicker->Date.FormatString("yyyymmdd");
        pQuery->ParamByName("EndDate")->AsString = EndDatePicker->Date.FormatString("yyyymmdd");
        TraceListView->Items->Clear();
        try
        {
                bool OnTrip = false;
                bool IO1;
                pQuery->Active = true;
                AnsiString StartDate, StartTime, StartPlaceName, EndDate, EndTime, EndPlaceName;
                float StartMileage, EndMileage;
                for(int i=1;!pQuery->Eof;i++)
                {
                        AnsiString DATE = pQuery->FieldByName("SendDate")->AsString;   //�ӵ���ƪ��o�e���
                        AnsiString TIME = pQuery->FieldByName("SendTime")->AsString;   //�ӵ���ƪ��o�e�ɶ�
                        AnsiString StartD = StartDatePicker->Date.FormatString("yyyymmdd");
                        AnsiString StartT = StartTimePicker->Time.FormatString("hhnnss");
                        AnsiString EndD = EndDatePicker->Date.FormatString("yyyymmdd");
                        AnsiString EndT = EndTimePicker->Time.FormatString("hhnnss");
                        if((DATE == StartD)&&(TIME < StartT))           //�|���}�l�i�J�d��
                        {
                                pQuery->Next();
                        }
                        else if((DATE == EndD)&&(TIME > EndT))          //�w�g�W�X�d��
                        {
                                if(OnTrip == true)     //���p�̫�@�I���٬O�B��Trip On�h����I�⦨�̫�@�I
                                {
                                        pQuery->Prior();
                                        int CarStatus = pQuery->FieldByName("CarStatus")->AsInteger;
                                        int temp = CarStatus % 8;
                                        int DistID = pQuery->FieldByName("DistID")->AsInteger;
                                        AnsiString DistName;
                                        //���o��F�Ϥ���W��
                                        pQuery2->SQL->Clear();
                                        pQuery2->SQL->Add("SELECT Cname FROM tArea");
                                        pQuery2->SQL->Add("WHERE AreaID = :DistID");
                                        pQuery2->ParamByName("DistID")->AsInteger = DistID;
                                        pQuery2->Active = true;
                                        DistName = pQuery2->FieldByName("Cname")->AsString;
                                        pQuery2->Active = false;
                                        if(temp >= 4)
                                        {
                                                IO1 = true;
                                        }
                                        else
                                        {
                                                IO1 = false;
                                        }
                                        OnTrip = false;
                                        EndDate  = pQuery->FieldByName("SendDate")->AsString;
                                        EndTime = pQuery->FieldByName("SendTime")->AsString;
                                        EndPlaceName = DistName + " " + pQuery->FieldByName("PlaceName")->AsString;
                                        EndMileage = pQuery->FieldByName("Mileage")->AsFloat;
                                        //��X�̫�@�I����,�N��Trip�g�JListView
                                        TListItem* pListItem = TraceListView->Items->Add();
                                        pListItem->Caption =  StartDate;
                                        pListItem->SubItems->Add(StartTime);
                                        pListItem->SubItems->Add(EndDate);
                                        pListItem->SubItems->Add(EndTime);
                                        pListItem->SubItems->Add(StartPlaceName);
                                        pListItem->SubItems->Add(EndPlaceName);
                                        pListItem->SubItems->Add(GetPeriod((StartDate+StartTime),(EndDate+EndTime)));
                                        EndMileage = EndMileage - StartMileage;
                                        AnsiString Mileage = Mileage.FormatFloat("0.00",(EndMileage));
                                        pListItem->SubItems->Add(Mileage);
                                        pQuery->Next();
                                }
                                pQuery->Next();
                        }
                        else
                        {
                                int CarStatus = pQuery->FieldByName("CarStatus")->AsInteger;
                                int temp = CarStatus % 8;
                                int DistID = pQuery->FieldByName("DistID")->AsInteger;
                                AnsiString DistName;
                                //���o��F�Ϥ���W��
                                pQuery2->SQL->Clear();
                                pQuery2->SQL->Add("SELECT Cname FROM tArea");
                                pQuery2->SQL->Add("WHERE AreaID = :DistID");
                                pQuery2->ParamByName("DistID")->AsInteger = DistID;
                                pQuery2->Active = true;
                                DistName = pQuery2->FieldByName("Cname")->AsString;
                                pQuery2->Active = false;
                                if(temp >= 4)
                                {
                                        IO1 = true;
                                }
                                else
                                {
                                        IO1 = false;
                                }
                                if((IO1 == true) && (OnTrip == false)) //IO1�OOn�ӥB�٨S�i�JTrip,�Y�OTrip���Ĥ@�I
                                {
                                        OnTrip = true;
                                        StartDate  = pQuery->FieldByName("SendDate")->AsString;
                                        StartTime = pQuery->FieldByName("SendTime")->AsString;
                                        StartPlaceName = DistName + " " + pQuery->FieldByName("PlaceName")->AsString;
                                        StartMileage = pQuery->FieldByName("Mileage")->AsFloat;
                                }
                                if((IO1 == false) && (OnTrip == true))  //IO1�OOff�ӥB�w�g�i�JTrip,�ӵ��Y�OTrip���̫�@�I
                                {
                                        //pQuery->Prior();
                                        OnTrip = false;
                                        EndDate  = pQuery->FieldByName("SendDate")->AsString;
                                        EndTime = pQuery->FieldByName("SendTime")->AsString;
                                        EndPlaceName = DistName + " " + pQuery->FieldByName("PlaceName")->AsString;
                                        EndMileage = pQuery->FieldByName("Mileage")->AsFloat;
                                        //pQuery->Next();
                                        //��X�̫�@�I����,�N��Trip�g�JListView
                                        TListItem* pListItem = TraceListView->Items->Add();
                                        pListItem->Caption =  StartDate;
                                        pListItem->SubItems->Add(StartTime);
                                        pListItem->SubItems->Add(EndDate);
                                        pListItem->SubItems->Add(EndTime);
                                        pListItem->SubItems->Add(StartPlaceName);
                                        pListItem->SubItems->Add(EndPlaceName);
                                        pListItem->SubItems->Add(GetPeriod((StartDate+StartTime),(EndDate+EndTime)));
                                        AnsiString Mileage = Mileage.FormatFloat("0.00",(EndMileage - StartMileage));
                                        pListItem->SubItems->Add(Mileage);
                                }
                                pQuery->Next();
                                if(pQuery->Eof == true)
                                {
                                        pQuery->Prior();
                                        if(OnTrip == true)     //���p�̫�@�I���٬O�B��Trip On�h����I�⦨�̫�@�I
                                        {
                                                OnTrip = false;
                                                EndDate  = pQuery->FieldByName("SendDate")->AsString;
                                                EndTime = pQuery->FieldByName("SendTime")->AsString;
                                                EndPlaceName = DistName + " " + pQuery->FieldByName("PlaceName")->AsString;
                                                EndMileage = pQuery->FieldByName("Mileage")->AsFloat;
                                                //��X�̫�@�I����,�N��Trip�g�JListView
                                                TListItem* pListItem = TraceListView->Items->Add();
                                                pListItem->Caption =  StartDate;
                                                pListItem->SubItems->Add(StartTime);
                                                pListItem->SubItems->Add(EndDate);
                                                pListItem->SubItems->Add(EndTime);
                                                pListItem->SubItems->Add(StartPlaceName);
                                                pListItem->SubItems->Add(EndPlaceName);
                                                pListItem->SubItems->Add(GetPeriod((StartDate+StartTime),(EndDate+EndTime)));
                                                EndMileage = EndMileage - StartMileage;
                                                AnsiString Mileage = Mileage.FormatFloat("0.00",(EndMileage));
                                                pListItem->SubItems->Add(Mileage);
                                        }
                                        pQuery->Next();
                                        pQuery->Next();
                                }
                        }
                        StatusBar1->Panels->Clear();
                        StatusBar1->Panels->Add();
                        StatusBar1->Panels->Items[0]->Text = AnsiString("�ثe�w�B�z ")+IntToStr(i)+AnsiString(" �����");
                        Application->ProcessMessages();
                }
        }
        catch(EDBEngineError &E)
        {
                if(pQuery)
                        delete pQuery;
                if(pQuery2)
                        delete pQuery2;
                DataModule1->EstablishConnect(1);
        }
        if(pQuery)
                delete pQuery;
        if(pQuery2)
                delete pQuery2;
        StatusBar1->Panels->Clear();
        StatusBar1->Panels->Add();
        StatusBar1->Panels->Items[0]->Text = AnsiString("�`�@ ")+AnsiString(TraceListView->Items->Count)+AnsiString(" �����");
}
//---------------------------------------------------------------------------
