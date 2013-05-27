//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TraceForm.h"
#include "HisTracer.h"
#include "MainForm.h"
#include "ChooseCarForm.h"
#include "ViewForm.h"
#include "TripForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CCALENDR"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm_Trace *Form_Trace;
//---------------------------------------------------------------------------
__fastcall TForm_Trace::TForm_Trace(TComponent* Owner)
        : TForm(Owner)
{
        LocatorNoBoxDropDown(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm_Trace::FormShow(TObject *Sender)
{
        CallCenter_Main_Form->SpeedButton_History->Down = true;
        CallCenter_Main_Form->N_History->Checked = true;
        CallCenter_Main_Form->Menu_HisTrace->Checked = true;
        //設定預設的監控時間
        StartDatePicker->DateTime = StartDatePicker->DateTime.CurrentDateTime();
        StartTimePicker->DateTime = StartTimePicker->DateTime.CurrentDate();
        EndDatePicker->DateTime = EndDatePicker->DateTime.CurrentDateTime();
        EndTimePicker->DateTime = EndTimePicker->DateTime.CurrentDateTime();
}
//---------------------------------------------------------------------------

void __fastcall TForm_Trace::TraceListViewDblClick(TObject *Sender)
{
	if(TraceListView->SelCount ==1 )
    {
    	//in SubItem [5],[6] is Car's WGS X,Y 座標
        long CarWGS_X = atol(TraceListView->Selected->SubItems->Strings[3].c_str());
        long CarWGS_Y = atol(TraceListView->Selected->SubItems->Strings[4].c_str());

		//Just for test to shift Taiwan to HongKong Test Lon=114175745 , Lat=22315023  , Taoyuan: Lon=121298319 , Lat=25016501
        DrawImage(CallCenter_Main_Form->Image_Map,CarWGS_X,CarWGS_Y,CallCenter_Main_Form->m_dScale);
        CallCenter_Main_Form->UpdateAllObjPos();
        //set Center Line
        DrawCenterLine(CallCenter_Main_Form->Image_Map);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_Trace::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        CallCenter_Main_Form->SpeedButton_History->Down = false;
        CallCenter_Main_Form->N_History->Checked = false;
        CallCenter_Main_Form->Menu_HisTrace->Checked = false;
        ClearAllTrace_InMemory();
        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
        Action = caFree;
        Form_Trace = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm_Trace::ClearAllTrace_InMemory()
{
   CCar *ptrCar = NULL;   //欲搜尋之Car class
   //清除所有車子的歷史路徑
   for(int i=0; i < CallCenter_Main_Form->m_pCarList->Count; i++)
   {
      ptrCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
      //Free 舊歷史軌跡資料
      if(ptrCar->m_pHisCarTraceList!=NULL)
      {
         for(;ptrCar->m_pHisCarTraceList->Count != 0;)
         {
            if(ptrCar->m_pHisCarTraceList->Items[0])
            {
               delete ((sSelectPoint*)ptrCar->m_pHisCarTraceList->Items[0]);
               ptrCar->m_pHisCarTraceList->Delete(0);
            }
         }
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm_Trace::LocatorNoBoxDropDown(TObject *Sender)
{
        //填入LocatorNoBox目前監控中車輛
        LocatorNoBox->Items->Clear();
        for(int i=0; i < Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                LocatorNoBox->Items->Add(Form_ViewInfo->ListView_Car->Items->Item[i]->Caption);
        }        
}
//---------------------------------------------------------------------------


void __fastcall TForm_Trace::SpeedButton1Click(TObject *Sender)
{
//將選擇之車輛的軌跡列出
        if(LocatorNoBox->ItemIndex == -1)
        {
                ShowMessage("未選擇車輛!!");
                return;
        }
        CCarInfo* pCarInfo = Form_ChooseCar->CustomerList->FindCarByCarID(LocatorNoBox->Items->Strings[LocatorNoBox->ItemIndex]);
        CHisTracer->LocatorNo = pCarInfo->LocatorNo;
        CHisTracer->StartDate = StartDatePicker->Date.FormatString("yyyymmdd");
        CHisTracer->StartTime = StartTimePicker->Time.FormatString("hhnnss");
        CHisTracer->EndDate = EndDatePicker->Date.FormatString("yyyymmdd");
        CHisTracer->EndTime = EndTimePicker->Time.FormatString("hhmmss");
        CHisTracer->Execute();

        CCar *ptrCar = NULL;   //欲搜尋之Car class
        //清除所有車子的歷史路徑
        ClearAllTrace_InMemory();
        TraceListView->Items->Clear();    //清除舊List
        //指定ptrCar所指之車子
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
                //Free 舊歷史軌跡資料
                for(int i=0;i<ptrCar->m_pHisCarTraceList->Count;i++)
                delete ((sSelectPoint*)ptrCar->m_pHisCarTraceList->Items[i]);
                ptrCar->m_pHisCarTraceList->Clear();
        }
        sSelectPoint* pTrack;

        //將值填入Item並將路徑點填入CCar
        TListItem *ptr = NULL;
        for(vector<TCHisTracer::sCarStat>::iterator it = CHisTracer->VectorCarStat.begin();it != CHisTracer->VectorCarStat.end(); ++it)
        {
                AnsiString tmpstr;
                ptr = TraceListView->Items->Add();
                tmpstr = it->m_szSendDate;
                tmpstr.Insert("/",5);
                tmpstr.Insert("/",8);
                ptr->Caption = tmpstr;
                tmpstr = it->m_szSendTime;
                tmpstr.Insert(":",3);
                tmpstr.Insert(":",6);
                ptr->SubItems->Add(tmpstr);
                ptr->SubItems->Add(it->DistName);
                ptr->SubItems->Add(it->m_szPlaceName);
                ptr->SubItems->Add(it->m_iLon);
                ptr->SubItems->Add(it->m_iLat);
                ptr->SubItems->Add(it->m_sSpeed);
                AnsiString Mileage = Mileage.FormatFloat("0.00",it->m_dMileage);
                ptr->SubItems->Add(Mileage);
                ptr->SubItems->Add(IntToBinary(it->m_iStatus));
                ptr->SubItems->Add(it->GPSLock);
                pTrack = new sSelectPoint;
                pTrack->mX2 = it->m_iLon;
                pTrack->mY2 = it->m_iLat;
                ptrCar->m_pHisCarTraceList->Add(pTrack);   //Add new to TList
                Application->ProcessMessages();
        }
        StatusBar1->Panels->Items[0]->Text = AnsiString("總共 ")+AnsiString(TraceListView->Items->Count)+AnsiString(" 筆資料");
        if(TraceListView->Items->Count > 0)
        {
                StatusBar1->Panels->Items[1]->Text = "Trip:" + TraceListView->Items->Item[0]->SubItems->Strings[1]+" "+TraceListView->Items->Item[0]->SubItems->Strings[2]+"->" + TraceListView->Items->Item[TraceListView->Items->Count-1]->SubItems->Strings[1]+" "+TraceListView->Items->Item[TraceListView->Items->Count-1]->SubItems->Strings[2];
                float intevel = TraceListView->Items->Item[TraceListView->Items->Count-1]->SubItems->Strings[6].ToDouble() - TraceListView->Items->Item[0]->SubItems->Strings[6].ToDouble();
                StatusBar1->Panels->Items[2]->Text = "里數:" + StatusBar1->Panels->Items[2]->Text.FormatFloat("0.00",intevel);
        }
        else
        {
                StatusBar1->Panels->Items[1]->Text = "";
                StatusBar1->Panels->Items[2]->Text = "";
        }
        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);        
}
//---------------------------------------------------------------------------

