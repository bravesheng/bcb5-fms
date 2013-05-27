//2001.10.10 Memory Free Check OK!
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MaintainThread.h"
#include "MainForm.h"
#include "SubForm1.h"
#include "ViewForm.h"
#include "TraceForm.h"
#include "SmartQryForm.h"
#include "common.h"
#include "StatManager.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall CTMaintain::CTMaintain(bool CreateSuspended)
        : TThread(CreateSuspended)
{
        FreeOnTerminate = true;
        Priority = tpIdle;
        bCanTerminate = false;
        Resume();
}
//---------------------------------------------------------------------------
void __fastcall CTMaintain::Execute()
{
    while(!Terminated)
    {
        //        Synchronize(PanelStat);       //改寫該功能至MainForm的Timer上
                if(DataModule1->bCanAccess.Lock())
                {
                        //檢查bCanAccess2是否為true,代表並未重新連線
                        if(!DataModule1->bCanAccess2.Locked)
                                UpdateCarStatusOnForm();
                        if(!DataModule1->bCanAccess2.Locked)
                                UpdateCarStatusOnSubForm();
                        //progressbar2 count
                        if(CallCenter_Main_Form->ProgressBar2->Position < 40)
                        {
                            CallCenter_Main_Form->ProgressBar2->Position++;
                        }
                        else
                        {
                            CallCenter_Main_Form->ProgressBar2->Position = 0;
                        }
                        //UnLock
                        DataModule1->bCanAccess.UnLock();
                }
                bCanTerminate = true;
                for(int i=0; i<30; i++)
                {
                        Sleep(10);
                        if(Terminated) return;          //結束執行緒
                }
                bCanTerminate = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall CTMaintain::PanelStat()
{
//2002.5.7 修正最小化Panel後,要回復育設大小無法回復的問題
        CallCenter_Main_Form->SpeedButton_ShowSetting->Down = Form_ViewInfo->Visible;
        CallCenter_Main_Form->SpeedButton_ShowStreet->Down = CallCenter_Main_Form->Panel_Street->Visible;
        CallCenter_Main_Form->Menu_Wnd_Setting->Checked = Form_ViewInfo->Visible;
        CallCenter_Main_Form->Menu_Wnd_Street->Checked = CallCenter_Main_Form->Panel_Street->Visible;
        bool CanMinimize = true;               //是否可以最小化
        for(int i=0; i < CallCenter_Main_Form->Panel_DynBar->DockClientCount; i++)
        {
                TControl* pControl = CallCenter_Main_Form->Panel_DynBar->DockClients[i];
                if(pControl->Visible == true)
                {
                        CanMinimize = false;
                        break;
                }
        }
        if(CanMinimize)
        {
                if(CallCenter_Main_Form->Panel_DynBar->Width != 2)
                {
                        CallCenter_Main_Form->Panel_DynBar->Width = 2;
                        CallCenter_Main_Form->UpdateAllObjPos();
                        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
                }
        }
        else if((CallCenter_Main_Form->Panel_DynBar->Width != CallCenter_Main_Form->DockSiteWidth) && (CanMinimize == false))
        {
                CallCenter_Main_Form->Panel_DynBar->Width = CallCenter_Main_Form->DockSiteWidth;
	        CallCenter_Main_Form->UpdateAllObjPos();
                ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
        }
        //同步視窗啟動按鈕狀態
        if(Form_ViewInfo->PageControl1->ActivePage == Form_ViewInfo->TabSheet_Car)
        {
                CallCenter_Main_Form->Menu_Car->Checked = true;
                CallCenter_Main_Form->Menu_Remark->Checked = false;
        }
        else if(Form_ViewInfo->PageControl1->ActivePage == Form_ViewInfo->TabSheet_Remark)
        {
                CallCenter_Main_Form->Menu_Car->Checked = false;
                CallCenter_Main_Form->Menu_Remark->Checked = true;
        }
        if(!Form_SmartQry)
        {
                CallCenter_Main_Form->SpeedButton_Query->Down = false;
                CallCenter_Main_Form->N_Query->Checked = false;
        }
        else
        {
                CallCenter_Main_Form->SpeedButton_Query->Down = true;
                CallCenter_Main_Form->N_Query->Checked = true;
        }
        static bool InitialMap = false;
        if(!InitialMap)
        {
                CallCenter_Main_Form->InitialDraw();
                InitialMap = true;
        }
}
//---------------------------------------------------------------------------
void  __fastcall CTMaintain::UpdateCarStatusOnForm()
{
    ////  Update Cars Status in other Components in Main Form
    try
    {
	    int iActiveCarCount = 0;
        CCar* pCar;
        bool bIsEvent = false;
        int TmpVal = 0;
        for(int i=0;i<CallCenter_Main_Form->m_pCarList->Count;i++)
        {
            if(TmpVal == 2)
            {
                Sleep(1);
                TmpVal = 0;
            }
            TmpVal++;
            Sleep(1);     //2002.6.26 added
            if(Terminated)
            {
                return;
            }
            pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
            if( !bIsEvent )	//只要有1輛車顯示異常即在 Form 上顯示異常
            {
                if(Form_ManageState->ListView1->Items->Count > 0)
                {
                    bIsEvent = true;
                }
            }
            // if record is exist Update Cars Info in ListView_CarInfo ,or Add new record
            TListItem* pListItem = Form_ViewInfo->ListView_Car->FindCaption(0,pCar->GetCarID(),false,true,false);
            if(pListItem != NULL)
            {
                Update_ListView_CarInfo(pCar,pListItem);    //找到對應之後更新車輛列表
            }
            int ACC = pCar->m_Status.m_iStatus % 4;
            if(ACC >= 2)
            {
                iActiveCarCount++; 	//this also the index of ListView
            }
        }
        //Update Cars Count in StaticText_CarsStatus
        if(bIsEvent)
        {
            CallCenter_Main_Form->StaticText_CarsStatus->Caption = "異常";
            CallCenter_Main_Form->StaticText_CarsStatus->Font->Color = clRed;
            CallCenter_Main_Form->StaticText_CarsStatus->Cursor = crHandPoint;
        }
        else
        {
            CallCenter_Main_Form->StaticText_CarsStatus->Caption = "正常";
            CallCenter_Main_Form->StaticText_CarsStatus->Font->Color = clGreen;
            CallCenter_Main_Form->StaticText_CarsStatus->Cursor = crDefault;
        }
        //Update Cars Count in StaticText_ActiveCarCount
        CallCenter_Main_Form->StaticText_ActiveCarCount->Caption = AnsiString(iActiveCarCount);
        //Update Cars Total Counts
        CallCenter_Main_Form->StaticText_TotalCarCount->Caption = AnsiString(CallCenter_Main_Form->m_pCarList->Count);
        //Update 異常車數
        CallCenter_Main_Form->StaticText_ExceptCars->Caption = AnsiString(Form_ManageState->ListView1->Items->Count);
    }
    catch(...)
    {}
}
//---------------------------------------------------------------------------
void  __fastcall CTMaintain::UpdateCarStatusOnSubForm()
{
    try
    {
	    TCallCenter_Sub_Form* pSubForm;
	    for(int i=0;i<CallCenter_Main_Form->m_pSubFormList->Count;i++)
        {
            Sleep(1);//2002.6.26 added
                if(Terminated)
                {
                    return;
                }
    	        pSubForm = (TCallCenter_Sub_Form*)CallCenter_Main_Form->m_pSubFormList->Items[i];
		    pSubForm->IsCarMove_UpdateCarImg();
        }
    }
    catch(...)
    {}
}
//---------------------------------------------------------------------------
void  __fastcall CTMaintain::Update_ListView_CarInfo(CCar* pCar,TListItem* pListItem)
{
    try
    {
	//Update CarInfo
        if(strcmp(pListItem->SubItems->Strings[CAR_RECVTIME].c_str(),pCar->GetDateTime())!=0 )
        {       //只有時間更動時才進行更新的動作!!
	        pListItem->SubItems->Strings[CAR_RECVTIME] = AnsiString(pCar->GetDateTime());
        }
  	        if(pListItem->ImageIndex != GetImgIndexFromTag(pCar->m_pImage_Car->Tag) )
                        pListItem->ImageIndex = GetImgIndexFromTag(pCar->m_pImage_Car->Tag);
                AnsiString LocatorNo =  pCar->GetLocatorNo();
                if(pListItem->SubItems->Strings[CAR_LOCATOR_NO] !=  LocatorNo)
                        pListItem->SubItems->Strings[CAR_LOCATOR_NO] = LocatorNo;
                //中文字=>必需分小寫
                if(strcmp(pListItem->Caption.c_str(),pCar->GetCarID())!=0)
    	                pListItem->Caption=AnsiString(pCar->GetCarID());
                if(strcmp(pListItem->SubItems->Strings[CAR_LOCATOR_NO].c_str(),pCar->GetLocatorNo())!=0)
    	                pListItem->SubItems->Strings[CAR_LOCATOR_NO]=AnsiString(pCar->GetLocatorNo());
                if(strcmp(pListItem->SubItems->Strings[CAR_SENDTIME].c_str(),pCar->SendDateTime())!=0 )
	                pListItem->SubItems->Strings[CAR_SENDTIME]=AnsiString(pCar->SendDateTime());
                if(pListItem->SubItems->Strings[CAR_PLACE] != pCar->m_Status.DistName)   //DistName為AnsiString
    	                pListItem->SubItems->Strings[CAR_PLACE] = pCar->m_Status.DistName;
                if(strcmp(pListItem->SubItems->Strings[ROAD_NAME].c_str(),pCar->GetPlaceInfo())!=0)
    	                pListItem->SubItems->Strings[ROAD_NAME]=AnsiString(pCar->GetPlaceInfo());
                if(pListItem->SubItems->Strings[CAR_SPEED] != AnsiString(pCar->GetCarSpeed()) )
    	                pListItem->SubItems->Strings[CAR_SPEED]=AnsiString(pCar->GetCarSpeed());
                if(atol(pListItem->SubItems->Strings[CAR_LON].c_str()) != pCar->GetCar_X2() )
    	                pListItem->SubItems->Strings[CAR_LON]=AnsiString(pCar->GetCar_X2());
                if(atol(pListItem->SubItems->Strings[CAR_LAT].c_str()) != pCar->GetCar_Y2() )
    	                pListItem->SubItems->Strings[CAR_LAT]=AnsiString(pCar->GetCar_Y2());
                if(pListItem->SubItems->Strings[CAR_MILEAGE] != (AnsiString)pCar->m_Status.m_dMileage)
                        pListItem->SubItems->Strings[CAR_MILEAGE]=AnsiString(pCar->m_Status.m_dMileage);
                AnsiString Status;
                AnsiString StatusOld = pListItem->SubItems->Strings[1];
                Status.SetLength(20);
                AnsiString IOStr = IntToBinary(pCar->m_Status.m_iStatus);
                if(IOStr != pListItem->SubItems->Strings[IO_STAT])
                        pListItem->SubItems->Strings[IO_STAT] = IOStr;
                if(pListItem->SubItems->Strings[CAR_TYPE] != pCar->CarType)
                        pListItem->SubItems->Strings[CAR_TYPE] = pCar->CarType;
                if(pListItem->SubItems->Strings[CAR_GROUP] != pCar->CarGroup)
                        pListItem->SubItems->Strings[CAR_GROUP] = pCar->CarGroup;
                AnsiString ReportRate = IntToStr(pCar->m_Status.ReportRate) + "sec";
                if(pListItem->SubItems->Strings[CAR_REPORTRATE] != ReportRate)
                        pListItem->SubItems->Strings[CAR_REPORTRATE] = ReportRate;
                if(pListItem->SubItems->Strings[REPORT_CHECK_RATE] != pCar->ReportCheckRate)
                        pListItem->SubItems->Strings[REPORT_CHECK_RATE] = pCar->ReportCheckRate;
                if(pListItem->SubItems->Strings[CAR_IO_IN_WATCH] != pCar->IO_In_Watch)
                        pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = pCar->IO_In_Watch;
                if(pListItem->SubItems->Strings[GPSLOCK] != pCar->GPSLock)
                        pListItem->SubItems->Strings[GPSLOCK] = pCar->GPSLock;
    }
    catch(...)
    {}

}
//---------------------------------------------------------------------------

