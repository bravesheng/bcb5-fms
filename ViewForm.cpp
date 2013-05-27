//2001.10.11 Memory check almost ok!
//maybe have some error in void __fastcall CCustomer::RenewList
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ViewForm.h"
#include "MainForm.h"
#include "DataMudule.h"
#include "SubForm1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_ViewInfo *Form_ViewInfo;
int g_ColumnToSort = 0;
int g_ColumnToSort_Car = 0;
//---------------------------------------------------------------------------
__fastcall TForm_ViewInfo::TForm_ViewInfo(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void  __fastcall TForm_ViewInfo::AddNewListView_Remarks(CRemarkPlace* pRemarks,TListView* pListView,int CountNum)
{
	//Add new record
        TListItem *pItem = pListView->Items->Add();
        if(CountNum <10)
    	        pItem->Caption = "0" + AnsiString(CountNum);
        else
    	        pItem->Caption = AnsiString(CountNum);                          // No SubItem
	pItem->SubItems->Add(pRemarks->m_strName);    		                //SubItems->Strings[0]
        pItem->SubItems->Add(pRemarks->GetClassName());                         //該地點的類別
	pItem->SubItems->Add(pRemarks->m_strPhone);			        //SubItems->Strings[1]
	pItem->SubItems->Add(pRemarks->m_strAddr);     		                //SubItems->Strings[2]
	pItem->SubItems->Add(pRemarks->m_strEmail);   		                //SubItems->Strings[3]
	pItem->SubItems->Add(pRemarks->m_strWebAddr);		                //SubItems->Strings[4]
	pItem->SubItems->Add(pRemarks->m_strRemarks);		                //SubItems->Strings[5]
	pItem->SubItems->Add(pRemarks->m_strImagePath);		                //SubItems->Strings[6]
        pItem->SubItems->Add(AnsiString((unsigned long)pRemarks));              //SubItems->Strings[7]
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::ListView_CarDblClick(TObject *Sender)
{
        if(ListView_Car->SelCount ==1 )
        {
    	        //in SubItem [5],[6] is Car's WGS X,Y 座標
                long CarWGS_X = atol(ListView_Car->Selected->SubItems->Strings[CAR_LON].c_str());
                long CarWGS_Y = atol(ListView_Car->Selected->SubItems->Strings[CAR_LAT].c_str());
                //Just for test to shift Taiwan to HongKong Test Lon=114175745 , Lat=22315023  , Taoyuan: Lon=121298319 , Lat=25016501
                //Move Locked Car to center of Screen (Image_Map)
		DrawImage(CallCenter_Main_Form->Image_Map,CarWGS_X,CarWGS_Y,CallCenter_Main_Form->m_dScale);
                CallCenter_Main_Form->UpdateAllObjPos();
                //set Center Line
                DrawCenterLine(CallCenter_Main_Form->Image_Map);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::ListView_RemarkColumnClick(TObject *Sender,
      TListColumn *Column)
{
	g_ColumnToSort = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::ListView_RemarkCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (g_ColumnToSort == 0)
    	        Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
                Compare = CompareText(Item1->SubItems->Strings[g_ColumnToSort-1],Item2->SubItems->Strings[g_ColumnToSort-1]);
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::ListView_RemarkDblClick(TObject *Sender)
{
        if(ListView_Remark->SelCount == 1 )
        {
    	        //delete Simu_Car_Thread
                TListItem* pItem = ListView_Remark->Selected;
                //in SubItem [8] is CRemarkPlace Pointer
                CRemarkPlace *pRemarks = (CRemarkPlace*)atol(pItem->SubItems->Strings[RMK_PTR].c_str());
                //Move Locked Car to center of Screen (Image_Map)
                DrawImage(CallCenter_Main_Form->Image_Map,pRemarks->m_X2,pRemarks->m_Y2,CallCenter_Main_Form->m_dScale);
                CallCenter_Main_Form->UpdateAllObjPos();
                //Set Center Line
                DrawCenterLine(CallCenter_Main_Form->Image_Map);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::SpeedButton_ChooseCarClick(TObject *Sender)
{
        SpeedButton_ChooseCar->Enabled = false;
        CallCenter_Main_Form->DeleteThread();
        Form_ChooseCar->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::ListView_CarColumnClick(TObject *Sender,
      TListColumn *Column)
{
	g_ColumnToSort_Car = Column->Index;
  	((TCustomListView *)Sender)->AlphaSort();        
}
//---------------------------------------------------------------------------
void __fastcall TForm_ViewInfo::ListView_CarCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
	if (g_ColumnToSort_Car == 0)
    	        Compare = CompareText(Item1->Caption,Item2->Caption);
  	else
                Compare = CompareText(Item1->SubItems->Strings[g_ColumnToSort_Car-1],Item2->SubItems->Strings[g_ColumnToSort_Car-1]);
}
//---------------------------------------------------------------------------

void __fastcall TForm_ViewInfo::ListView_CarChange(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
//避免當Close Applocation時發生存取錯誤
        if(Application->Terminated)
                return;
//找出異動的車輛,並將其更改顯示模式,Visible or invisible
        AnsiString CarID = Item->Caption;
        if(Change == ctState)
        {
                for(int j=0; j<CallCenter_Main_Form->m_pCarList->Count; j++)
                {
                        CCar* pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[j];
                        AnsiString sCarID = pCar->GetCarID();
                        if(sCarID == CarID)
                        {
                                //2002.7.5 修正原if(pCar->m_pImage_Car->Visible != Item->Checked)
                                if(pCar->Visible != Item->Checked)
                                {
                                    pCar->m_pImage_Car->Visible = Item->Checked;
                                    pCar->Visible = Item->Checked;
                                    pCar->Update(NULL);
                                }
                        }

                }
        }
}
//---------------------------------------------------------------------------
TListItem* __fastcall TForm_ViewInfo::GetCarItem(AnsiString CarID)
{
//以CarID找出車輛在ViewInfo的位置
//找不到return NULL
         TListItem* pListItem = ListView_Car->FindCaption(0,CarID,false,true,false);
         return pListItem;
}


void __fastcall TForm_ViewInfo::FormStartDock(TObject *Sender,
      TDragDockObject *&DragObject)
{
    DataModule1->DB_Check_Thread->Suspend();
    CallCenter_Main_Form->Maintain_Thread->Suspend();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ViewInfo::FormEndDock(TObject *Sender,
      TObject *Target, int X, int Y)
{
    DataModule1->DB_Check_Thread->Resume();
    CallCenter_Main_Form->Maintain_Thread->Resume();
}
//---------------------------------------------------------------------------

