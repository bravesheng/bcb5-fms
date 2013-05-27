//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChooseCarForm.h"
#include "DataMudule.h"
#include "ViewForm.h"
#include "MainForm.h"
#include "SubForm1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_ChooseCar *Form_ChooseCar;
//---------------------------------------------------------------------------
__fastcall TForm_ChooseCar::TForm_ChooseCar(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
CCustomerList::CCustomerList()
{
        List = new TList;       //free on destory function
}
//---------------------------------------------------------------------------
CCustomerList::~CCustomerList()
{
        while(List->Count>0)
        {
                delete (CCustomer*)List->Items[0];
                List->Delete(0);
        }
        delete List;
}

//---------------------------------------------------------------------------
void __fastcall CCustomerList::RenewList()
{
        TList* List_new = new TList();                  //free using List_old instead
        TList* List_old;
        TQuery* Q_CustInfo = new TQuery(DataModule1);       //free in the function button
        Q_CustInfo->DatabaseName = "DB_DCH";
        Q_CustInfo->SQL->Add("SELECT    SerialNo, CoID, FullName");
        Q_CustInfo->SQL->Add("FROM      tCustInfo");
        try
        {
                Q_CustInfo->Active = false;
                Q_CustInfo->Active = true;
                for(int i=0; i < Q_CustInfo->RecordCount; i++)
                {
                        CCustomer* pCustomer = new CCustomer(this);
                        pCustomer->SerialNo = Q_CustInfo->FieldByName("SerialNo")->AsInteger;
                        pCustomer->CoID = Q_CustInfo->FieldByName("CoID")->AsString;
                        pCustomer->FullName = Q_CustInfo->FieldByName("FullName")->AsString;
                        pCustomer->RenewList();
                        List_new->Add(pCustomer);
                        Q_CustInfo->Next();
                }
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        catch(...)
        {
        }
        Q_CustInfo->Active = false;
        delete Q_CustInfo;                        //free Q_CustInfo
        List_old = List;
        List = List_new;
        List_new = NULL;
        while(List_old->Count>0)                //free memory
        {
                delete (CCustomer*)List_old->Items[0];
                List_old->Delete(0);
        }
        delete List_old;
}
//---------------------------------------------------------------------------
CCarInfo* CCustomerList::FindCarByCarID(AnsiString CarID)
{
        for(int i=0; i < List->Count; i++)
        {
                CCustomer* pCustomer = (CCustomer*)List->Items[i];
                CCarInfo* pCarInfo = pCustomer->FindCarByCarID(CarID);
                if(pCarInfo != NULL)
                {
                        return pCarInfo;
                }
        }
        return NULL;
}
//---------------------------------------------------------------------------
CCustomer::CCustomer(void* Parent_Point)
{
        pParent = Parent_Point;
        GroupList = new TList;            //Free on destory function
        GroupCodes = new CCodeSet();
        TypeCodes = new CCodeSet();
}
//---------------------------------------------------------------------------
CCustomer::~CCustomer()
{
        delete GroupCodes;
        delete TypeCodes;
        while(GroupList->Count>0)
        {
                delete (CGroup*)GroupList->Items[0];
                GroupList->Delete(0);
        }
        delete GroupList;
}
//---------------------------------------------------------------------------
void __fastcall CCustomer::RenewList()
{
//2002.5.7 新增 RenewTypeGroupCode
        try
        {
                RenewTypeGroupSet();
                TList* List_new = new TList();                          //free list_old instead
                TList* List_old;
                TQuery* Q_CustGroup = new TQuery(DataModule1);
                Q_CustGroup->DatabaseName = "DB_DCH";
                Q_CustGroup->SQL->Add("SELECT    SerialNo, GrID, FullName");
                Q_CustGroup->SQL->Add("FROM      tCustGroup");
                Q_CustGroup->SQL->Add("WHERE     CSerialNo = " + IntToStr(SerialNo));
                Q_CustGroup->Active = false;
                Q_CustGroup->Active = true;
                for(int i=0; i < Q_CustGroup->RecordCount; i++)
                {
                        CGroup* pGroup = new CGroup(this);
                        pGroup->SerialNo = Q_CustGroup->FieldByName("SerialNo")->AsInteger;
                        pGroup->GrID = Q_CustGroup->FieldByName("GrID")->AsString;
                        pGroup->FullName = Q_CustGroup->FieldByName("FullName")->AsString;
                        pGroup->RenewList();
                        List_new->Add(pGroup);
                        Q_CustGroup->Next();
                }
                Q_CustGroup->Active = false;
                delete Q_CustGroup;
                List_old = GroupList;
                GroupList = List_new;
                while(List_old->Count>0)                     //free memory
                {
                        delete (CCustomer*)List_old->Items[0];
                        List_old->Delete(0);
                }
                delete List_old;
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
}
//---------------------------------------------------------------------------
bool __fastcall CCustomer::RenewTypeGroupSet()
{
//更新包含的TypeCode 以及 GroupCode,成功return true else return false
        TQuery* pQCarType = new TQuery(NULL);
        pQCarType->DatabaseName = "DB_DCH";
        pQCarType->SQL->Add("SELECT * FROM tCarType");
        pQCarType->SQL->Add("WHERE CSerialNo = :ComSN");
        pQCarType->ParamByName("ComSN")->AsInteger = SerialNo;
        TQuery* pQCarGroup = new TQuery(NULL);
        pQCarGroup->DatabaseName = "DB_DCH";
        pQCarGroup->SQL->Add("SELECT * FROM tCarGroup");
        pQCarGroup->SQL->Add("WHERE CSerialNo = :ComSN");
        pQCarGroup->ParamByName("ComSN")->AsInteger = SerialNo;
        try
        {
                //填入TyoeCode
                TypeCodes->Clear();
                for(pQCarType->Open(); !pQCarType->Eof; pQCarType->Next())
                {
                        CCodeUnit* pCodeUnit = new CCodeUnit();
                        pCodeUnit->SerialNo = pQCarType->FieldByName("SerialNo")->AsInteger;
                        pCodeUnit->Code = pQCarType->FieldByName("Code")->AsString;
                        pCodeUnit->Explanation = pQCarType->FieldByName("Explanation")->AsString;
                        TypeCodes->Add(pCodeUnit);
                }
                pQCarType->Close();
                //填入GroupCode
                GroupCodes->Clear();
                for(pQCarGroup->Open(); !pQCarGroup->Eof; pQCarGroup->Next())
                {
                        CCodeUnit* pCodeUnit = new CCodeUnit();
                        pCodeUnit->SerialNo = pQCarGroup->FieldByName("SerialNo")->AsInteger;
                        pCodeUnit->Code = pQCarGroup->FieldByName("Code")->AsString;
                        pCodeUnit->Explanation = pQCarGroup->FieldByName("Explanation")->AsString;
                        GroupCodes->Add(pCodeUnit);
                }
                pQCarGroup->Close();
        }
        catch(...)
        {
                delete pQCarType;
                delete pQCarGroup;
                return false;
        }
        delete pQCarType;
        delete pQCarGroup;
        return true;
}
//---------------------------------------------------------------------------
CCarInfo* CCustomer::FindCarByCarID(AnsiString CarID)
{
        for(int i=0; i < GroupList->Count; i++)
        {
                CGroup* pGroup = (CGroup*)GroupList->Items[i];
                CCarInfo* pCarInfo = pGroup->FindCarByCarID(CarID);
                if(pCarInfo != NULL)
                {
                        return pCarInfo;
                }
        }
        return NULL;
}
//---------------------------------------------------------------------------
CGroup::CGroup(void* Parent_Point)
{
        pParent = Parent_Point;
        CarInfoList = new TList;

}
//---------------------------------------------------------------------------
CGroup::~CGroup()
{
        while(CarInfoList->Count>0)
        {
                delete (CCarInfo*)CarInfoList->Items[0];
                CarInfoList->Delete(0);
        }
        delete CarInfoList;
}
//---------------------------------------------------------------------------
CCarInfo::CCarInfo(void* Parent_Point)
{
        pParent = Parent_Point;
}
//---------------------------------------------------------------------------
void __fastcall CGroup::RenewList()
{
        try
        {
                TList* List_new = new TList();                          //free list_old instead
                TList* List_old;
                TQuery* Q_CarInfo = new TQuery(DataModule1);
                Q_CarInfo->DatabaseName = "DB_DCH";
                Q_CarInfo->SQL->Add("SELECT    UpdateTime,SerialNo, CarID, LocatorNo, SMPP_TON, SMPP_NPI, LocatorPwd, TypeCode, GroupCode, RouteNo, Image, DefaultIO");
                Q_CarInfo->SQL->Add("FROM      tCarInfo");
                Q_CarInfo->SQL->Add("WHERE     CSerialNo = " + IntToStr(SerialNo));
                Q_CarInfo->Active = true;
                for(int i=0; i < Q_CarInfo->RecordCount; i++)
                {
                        if(CallCenter_Main_Form->ThreadDeleteing)
                        {
                                while(List_new->Count>0)                     //free memory
                                {
                                        delete (CCarInfo*)List_new->Items[0];
                                        List_new->Delete(0);
                                }
                                delete List_new;
                                delete Q_CarInfo;
                                return;
                        }
                        CCarInfo* pCarInfo = new CCarInfo(this);
                        pCarInfo->UpdateTime = Q_CarInfo->FieldByName("UpdateTime")->AsDateTime;
                        pCarInfo->SerialNo = Q_CarInfo->FieldByName("SerialNo")->AsInteger;
                        pCarInfo->CarID = Q_CarInfo->FieldByName("CarID")->AsString;
                        pCarInfo->LocatorNo = Q_CarInfo->FieldByName("LocatorNo")->AsString;
                        pCarInfo->SMPP_TON = Q_CarInfo->FieldByName("SMPP_TON")->AsInteger;
                        pCarInfo->SMPP_NPI = Q_CarInfo->FieldByName("SMPP_NPI")->AsInteger;
                        pCarInfo->LocatorPwd = Q_CarInfo->FieldByName("LocatorPwd")->AsString;
                        pCarInfo->TypeCode = Q_CarInfo->FieldByName("TypeCode")->AsInteger;
                        pCarInfo->GroupCode = Q_CarInfo->FieldByName("GroupCode")->AsInteger;
                        pCarInfo->RouteNo = Q_CarInfo->FieldByName("RouteNo")->AsInteger;
                        pCarInfo->Image = Q_CarInfo->FieldByName("Image")->AsInteger;
                        pCarInfo->DefaultIO = Q_CarInfo->FieldByName("DefaultIO")->AsInteger;
                        List_new->Add(pCarInfo);
                        Q_CarInfo->Next();
                }
                Q_CarInfo->Active = false;
                delete Q_CarInfo;
                List_old = CarInfoList;
                CarInfoList = List_new;
                while(List_old->Count>0)                     //free memory
                {
                        delete (CCarInfo*)List_old->Items[0];
                        List_old->Delete(0);
                }
                delete List_old;
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
}
//---------------------------------------------------------------------------
CCarInfo* CGroup::FindCarByCarID(AnsiString CarID)
{
        for(int i=0; i < CarInfoList->Count; i++)
        {
                CCarInfo* pCarInfo = (CCarInfo*)CarInfoList->Items[i];
                if(pCarInfo->CarID == CarID)
                {
                        return pCarInfo;
                }
        }
        return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ChooseCar::ComboBox_CompanyDropDown(TObject *Sender)
{
        CustomerList->RenewList();//重新整理
        CCustomer* pCustomer;
        ComboBox_Company->Items->Clear();
        ComboBox_Company->Items->Add(sViewAll);
        for(int i=0; i < CustomerList->List->Count; i++)
        {
                pCustomer = (CCustomer*)CustomerList->List->Items[i];
                ComboBox_Company->Items->Add(pCustomer->FullName);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_ChooseCar::FormShow(TObject *Sender)
{
        delete CustomerList;
        CustomerList = NULL;
        sViewAll = "全部列出";
        if(!CustomerList)
        {
                CustomerList = new CCustomerList();
        }
        CustomerList->RenewList();
        ComboBox_Company->Items->Clear();
        ComboBox_Company->Items->Add(sViewAll);
        ComboBox_Company->ItemIndex = 0;
        //列出監控車輛
        ListView_Selected->Items->Clear();
        ListView_Group->Items->Clear();
        ListView_Type->Items->Clear();
        ListView_NoSelect->Items->Clear();
        for(int i=0; i < Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                //先判斷該車輛是否存在資料庫,避免當我們加入memory後,該車輛剛好被刪除!
                if(CustomerList->FindCarByCarID(Form_ViewInfo->ListView_Car->Items->Item[i]->Caption) != NULL)
                {
                        TListItem* pListItem = ListView_Selected->Items->Add();
                        pListItem->Caption = Form_ViewInfo->ListView_Car->Items->Item[i]->Caption;
                        pListItem->SubItems->Add(Form_ViewInfo->ListView_Car->Items->Item[i]->SubItems->Strings[CAR_LOCATOR_NO]);
                }
        }
        UpdateNonSelect();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ChooseCar::UpdateNonSelect()      //篩選掉不要的車輛
{


        CCustomer* pCustomer;
        ListView_NoSelect->Items->Clear();
        if(ComboBox_Company->ItemIndex > 0)      //單一公司顯示(Group and Type)
        {
                pCustomer = (CCustomer*)CustomerList->List->Items[ComboBox_Company->ItemIndex-1];
                //列出該公司車輛
                for(int j=0; j<pCustomer->GroupList->Count; j++)
                {
                        CGroup* pGroup = (CGroup*)pCustomer->GroupList->Items[j];
                        for(int k=0; k<pGroup->CarInfoList->Count; k++)
                        {
                                CCarInfo* pCarInfo = (CCarInfo*)pGroup->CarInfoList->Items[k];
                                TListItem* pListItem = ListView_NoSelect->Items->Add();
                                pListItem->Caption = pCarInfo->CarID;
                                pListItem->SubItems->Add(pCarInfo->LocatorNo);
                                pListItem->SubItems->Add(pCarInfo->GroupCode);
                                pListItem->SubItems->Add(pCarInfo->TypeCode);
                        }
                }
        }
        else
        {
                //沒有選擇Company所以不顯示Type and Group,列出所有車輛
                for(int i=0; i<CustomerList->List->Count; i++)
                {
                        CCustomer* pCustomer = (CCustomer*)CustomerList->List->Items[i];
                        for(int j=0; j<pCustomer->GroupList->Count; j++)
                        {
                                CGroup* pGroup = (CGroup*)pCustomer->GroupList->Items[j];
                                for(int k=0; k<pGroup->CarInfoList->Count; k++)
                                {
                                        CCarInfo* pCarInfo = (CCarInfo*)pGroup->CarInfoList->Items[k];
                                        TListItem* pListItem = ListView_NoSelect->Items->Add();
                                        pListItem->Caption = pCarInfo->CarID;
                                        pListItem->SubItems->Add(pCarInfo->LocatorNo);
                                        pListItem->SubItems->Add(pCarInfo->GroupCode);
                                        pListItem->SubItems->Add(pCarInfo->TypeCode);
                                }
                        }
                }
        }
        for(int i=0; i<ListView_NoSelect->Items->Count;)
        {
                bool bSelected = false;         //是否已經被選了
                bool bConform_Group = false;           //使否符合資格
                bool bConform_Type = false;
                bool bHaveCheckedBox_Group = false;
                bool bHaveCheckedBox_Type = false;
                //檢查有沒有選擇Group and Type
                if(ComboBox_Company->ItemIndex > 0)
                {
                        for(int j=0; (j<ListView_Group->Items->Count)&&(bConform_Group == false); j++)
                        {
                                if(ListView_Group->Items->Item[j]->Checked)
                                {
                                        bHaveCheckedBox_Group = true;
                                        if(ListView_Group->Items->Item[j]->SubItems->Strings[1] == ListView_NoSelect->Items->Item[i]->SubItems->Strings[1])
                                        {
                                                bConform_Group = true;
                                        }

                                }
                        }
                        for(int j=0; (j<ListView_Type->Items->Count)&&(bConform_Type == false); j++)
                        {
                                if(ListView_Type->Items->Item[j]->Checked)
                                {
                                        bHaveCheckedBox_Type = true;
                                        if(ListView_Type->Items->Item[j]->SubItems->Strings[1] == ListView_NoSelect->Items->Item[i]->SubItems->Strings[2])
                                        {
                                                bConform_Type = true;
                                        }
                                }
                        }
                }
                if((ComboBox_Company->ItemIndex <= 0) || (bHaveCheckedBox_Group == false))  //假如沒有Group可以篩選則全部過
                {
                        bConform_Group = true;
                }
                if((ComboBox_Company->ItemIndex <= 0) || (bHaveCheckedBox_Type == false))  //假如沒有Type可以篩選則全部過
                {
                        bConform_Type = true;
                }
                for(int j=0; (j<ListView_Selected->Items->Count)&&(bSelected == false); j++)       //決定是否已經被選擇
                {
                        if(ListView_Selected->Items->Item[j]->Caption == ListView_NoSelect->Items->Item[i]->Caption)
                        {
                                bSelected = true;
                        }
                }
                if((bSelected == true) || (bConform_Group == false) || (bConform_Type == false))           //對不要的項目進行刪除
                {
                        ListView_NoSelect->Items->Delete(i);
                }
                else
                {
                        i++;
                }
        }
}

void __fastcall TForm_ChooseCar::ListView_GroupMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        UpdateNonSelect();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::ListView_TypeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        UpdateNonSelect();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::SpeedButton_SingleGoClick(TObject *Sender)
{
        for(int i=0; i < ListView_NoSelect->Items->Count; i++)
        {
                if(ListView_NoSelect->Items->Item[i]->Selected)
                {
                        if(ListView_Selected->Items->Count == 500)
                        {
                                ShowMessage("已經超過最多選取車輛500台,其餘車輛將不會加入!");
                                break;
                        }
                        TListItem* pListItem = ListView_Selected->Items->Add();
                        pListItem->Caption = ListView_NoSelect->Items->Item[i]->Caption;
                        pListItem->SubItems->Add(ListView_NoSelect->Items->Item[i]->SubItems->Strings[0]);
                        pListItem->SubItems->Add(ListView_NoSelect->Items->Item[i]->SubItems->Strings[1]);
                        pListItem->SubItems->Add(ListView_NoSelect->Items->Item[i]->SubItems->Strings[2]);
                }
        }
        UpdateNonSelect();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::SpeedButton_AllGoClick(TObject *Sender)
{
        for(int i=0; i < ListView_NoSelect->Items->Count; i++)
        {
                if(ListView_Selected->Items->Count == 500)
                {
                        ShowMessage("已經超過最多選取車輛500台,其餘車輛將不會加入!");
                        break;
                }
                TListItem* pListItem = ListView_Selected->Items->Add();
                pListItem->Caption = ListView_NoSelect->Items->Item[i]->Caption;
                pListItem->SubItems->Add(ListView_NoSelect->Items->Item[i]->SubItems->Strings[0]);
                pListItem->SubItems->Add(ListView_NoSelect->Items->Item[i]->SubItems->Strings[1]);
                pListItem->SubItems->Add(ListView_NoSelect->Items->Item[i]->SubItems->Strings[2]);
        }
        UpdateNonSelect();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::SpeedButton_SingleBackClick(
      TObject *Sender)
{
        for(int i=0; i < ListView_Selected->Items->Count;)
        {
                if(ListView_Selected->Items->Item[i]->Selected)
                {
                        ListView_Selected->Items->Delete(i);
                }
                else
                {
                        i++;
                }
        }
        UpdateNonSelect();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::SpeedButton_AllBackClick(TObject *Sender)
{
        ListView_Selected->Items->Clear();
        UpdateNonSelect();
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::SpeedButton_OKClick(TObject *Sender)
{
//2002.5.9 新增清除Sub Form程序
        //清除開啟的SubForm
        while(CallCenter_Main_Form->m_pSubFormList->Count > 0)
        {
                TCallCenter_Sub_Form *pSubForm = (TCallCenter_Sub_Form*)CallCenter_Main_Form->m_pSubFormList->Items[0];
                pSubForm->Close();
        }
        //清除CCarList
        CallCenter_Main_Form->CCarList_SortByLocatorNo->Clear();
        CCar* pCar=NULL;
        for (int i = 0; i < CallCenter_Main_Form->m_pCarList->Count; i++)
        {
                pCar = (CCar*) CallCenter_Main_Form->m_pCarList->Items[i];
                delete pCar;
        }
        CallCenter_Main_Form->m_pCarList->Clear();
        //更新ListView_Car
        Form_ViewInfo->ListView_Car->Items->Clear();
        for(int i=0; i<ListView_Selected->Items->Count; i++)
        {
                //加入ListView_Car
                TListItem* pListItem = Form_ViewInfo->ListView_Car->Items->Add();
                pListItem->Caption = ListView_Selected->Items->Item[i]->Caption;
                pListItem->Checked = true;
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add("");
                pListItem->SubItems->Add(""); //15
                pListItem->SubItems->Strings[CAR_LOCATOR_NO] = ListView_Selected->Items->Item[i]->SubItems->Strings[0];
                pListItem->SubItems->Strings[CAR_IO_IN_WATCH] = "1";
        }
        Get_Remarks_From_DB();       //更新ListView Remarks
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_ChooseCar::FormDestroy(TObject *Sender)
{
        delete CustomerList;
        CustomerList = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ChooseCar::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        if(DataModule1->bEstablishConnect == false)
        {
                CallCenter_Main_Form->RebuildThread();
        }
        Form_ViewInfo->SpeedButton_ChooseCar->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_ChooseCar::Change_Car_List_Visible()
{
        //將車輛子視窗delete掉,以免access null point
        while(CallCenter_Main_Form->m_pSubFormList->Count>0)
        {
                delete (TCallCenter_Sub_Form*)CallCenter_Main_Form->m_pSubFormList->Items[0];
                CallCenter_Main_Form->m_pSubFormList->Delete(0);
        }
        //先將所有車輛Visible = true
        for(int i=0; i < CallCenter_Main_Form->m_pCarList->Count; i++)
        {
                CCar* pCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
                pCar->Visible = true;
                pCar->m_pImage_Car->Visible = true;
        }
}
//---------------------------------------------------------------------------
bool __fastcall TForm_ChooseCar::Get_Remarks_From_DB()
{
//讀取備註地點資料進入資料庫,成功時回傳1,失敗回傳0
//CancelWhenTheSame--> true時備註地點與之前數目一樣不做  false時都做
        Form_ViewInfo->ListView_Remark->Items->Clear();
        Set <int,0,500> CompanySet;        //篩選公司用集合
        CompanySet.Clear();
        TList* pList_new = new TList;
        TList* pList_tmp;
        TQuery* pQ_Remarks = new TQuery(DataModule1);
        pQ_Remarks->DatabaseName = "DB_DCH";
        pQ_Remarks->SQL->Add("SELECT * FROM tRemarks");
        AnsiString SQL_Line3 = "WHERE CoID IN ('Common'";
        for(int i=0; i<ListView_Selected->Items->Count; i++)
        {
                CCarInfo* pCarInfo = CustomerList->FindCarByCarID(ListView_Selected->Items->Item[i]->Caption);
                CGroup* pGroup = (CGroup*)pCarInfo->pParent;
                CCustomer* pCustomer = (CCustomer*)pGroup->pParent;
                if(!CompanySet.Contains(pCustomer->SerialNo))
                {
                        CompanySet << pCustomer->SerialNo;
                        SQL_Line3 = SQL_Line3 + ",'" + pCustomer->CoID + "'";
                }
        }
        pQ_Remarks->SQL->Add(SQL_Line3 + ")");
        try
        {
                pQ_Remarks->Active = false;
                pQ_Remarks->Active = true;
                for(pQ_Remarks->First();!pQ_Remarks->Eof;pQ_Remarks->Next())
                {
                        int m_X2 = pQ_Remarks->FieldValues["Lon"];
                        int m_Y2 = pQ_Remarks->FieldValues["Lat"];
                        AnsiString  m_szCoID = pQ_Remarks->FieldValues["CoID"];
                        AnsiString CompanyName = "*通用備註地點*";
                        if(m_szCoID != "Common")
                        {
                                CompanyName = DataModule1->Get_DB_Value("tCustInfo","FullName","CoID",m_szCoID);
                        }
                        AnsiString strName = pQ_Remarks->FieldValues["Name"];
                        AnsiString m_strPhone = pQ_Remarks->FieldValues["Phone"];
                        AnsiString m_strAddr = pQ_Remarks->FieldValues["Address"];
                        AnsiString m_strEmail = pQ_Remarks->FieldValues["Email"];
                        AnsiString m_strWebAddr = pQ_Remarks->FieldValues["WebAddress"];
                        AnsiString m_strImagePath = pQ_Remarks->FieldValues["ImagePath"];
                        AnsiString m_strRemarks = pQ_Remarks->FieldValues["Memo"];
                        int i_SerialNo = pQ_Remarks->FieldValues["SerialNo"];
                        CRemarkPlace *pRemark;
                        pRemark = new CRemarkPlace(CallCenter_Main_Form,m_X2,m_Y2,m_strImagePath.c_str());
                        strcpy(pRemark->m_szCoID,m_szCoID.c_str());
                        pRemark->m_RemarkType = pQ_Remarks->FieldValues["RemarkType"];
                        strcpy(pRemark->m_strName,strName.c_str());
                        strcpy(pRemark->m_strPhone,m_strPhone.c_str());
                        strcpy(pRemark->m_strAddr,m_strAddr.c_str());
                        strcpy(pRemark->m_strEmail,m_strEmail.c_str());
                        strcpy(pRemark->m_strWebAddr,m_strWebAddr.c_str());
                        strcpy(pRemark->m_strRemarks,m_strRemarks.c_str());
                        strcpy(pRemark->m_strCompanyName,CompanyName.c_str());
                        pRemark->i_SerialNo = i_SerialNo;
                        pRemark->SetHint();  //重設暗示文字
                        pList_new->Add(pRemark);    //Add New to TList
                        //Add New Data to ListView_Remarks
                        Form_ViewInfo->AddNewListView_Remarks(pRemark,Form_ViewInfo->ListView_Remark,pList_new->Count);
                }
                pQ_Remarks->Active = false;
                pList_tmp = CallCenter_Main_Form->m_pRemarksList;               //置換成新List
                CallCenter_Main_Form->m_pRemarksList = pList_new;
                if(pList_tmp)                                          //將舊List刪除
                {
                        CRemarkPlace* pRemarks;
                        for (int k = 0; k < pList_tmp->Count; k++)
                        {
                                pRemarks = (CRemarkPlace*) pList_tmp->Items[k];
                                delete pRemarks;
                        }
                        delete pList_tmp;
                }
        }
        catch(...)
        {
                return false;
        }
        return true;
}

void __fastcall TForm_ChooseCar::ComboBox_CompanyChange(TObject *Sender)
{
        //列出Group and Type
        CCustomer* pCustomer;
        ListView_Group->Items->Clear();
        ListView_Type->Items->Clear();
        ListView_NoSelect->Items->Clear();
        if(ComboBox_Company->ItemIndex > 0)      //單一公司顯示(Group and Type)
        {
                pCustomer = (CCustomer*)CustomerList->List->Items[ComboBox_Company->ItemIndex-1];
                Query_CarGroup->ParamByName("Csn")->AsInteger = pCustomer->SerialNo;
                Query_CarType->ParamByName("Csn")->AsInteger = pCustomer->SerialNo;
                Query_CarGroup->Open();
                Query_CarType->Open();
                for(int i=0; Query_CarGroup->Eof == false; i++)  //列出Group
                {
                        ListView_Group->Items->Add();
                        ListView_Group->Items->Item[i]->Caption = Query_CarGroup->FieldByName("Code")->AsString;
                        ListView_Group->Items->Item[i]->SubItems->Add(Query_CarGroup->FieldByName("Explanation")->AsString);
                        ListView_Group->Items->Item[i]->SubItems->Add(Query_CarGroup->FieldByName("SerialNo")->AsString);
                        Query_CarGroup->Next();
                }
                for(int i=0; Query_CarType->Eof == false; i++)
                {
                        ListView_Type->Items->Add();
                        ListView_Type->Items->Item[i]->Caption = Query_CarType->FieldByName("Code")->AsString;
                        ListView_Type->Items->Item[i]->SubItems->Add(Query_CarType->FieldByName("Explanation")->AsString);
                        ListView_Type->Items->Item[i]->SubItems->Add(Query_CarType->FieldByName("SerialNo")->AsString);
                        Query_CarType->Next();
                }
                Query_CarGroup->Close();
                Query_CarType->Close();
        }
        //列出該公司車輛
        UpdateNonSelect();        
}
//---------------------------------------------------------------------------


void __fastcall TForm_ChooseCar::ListView_NoSelectDblClick(TObject *Sender)
{
        SpeedButton_SingleGoClick(Sender);        
}
//---------------------------------------------------------------------------

void __fastcall TForm_ChooseCar::ListView_SelectedDblClick(TObject *Sender)
{
        SpeedButton_SingleBackClick(Sender);        
}
//---------------------------------------------------------------------------
CCodeSet::CCodeSet()
{
        CodeList = new TList();
}
//---------------------------------------------------------------------------
CCodeSet::~CCodeSet()
{
        for(int i=0; i < CodeList->Count; i++)
        {
                CCodeUnit* pCodeUnit = (CCodeUnit*)CodeList->Items[i];
                delete pCodeUnit;
        }
        delete CodeList;
}
//---------------------------------------------------------------------------
bool CCodeSet::Add(CCodeUnit* UnitStruct)
{
//新增特定sCodeUnit加入Set,成功return true,
//新增失敗或者重複的資料return false
        if(FindCodeUnit(UnitStruct->SerialNo) != NULL)
        {
                return false;
        }
        CodeList->Add(UnitStruct);
        return true;
}
//---------------------------------------------------------------------------
CCodeUnit* CCodeSet::FindCodeUnit(int UnitSN)
{
//檢查是否包含特定Unit,有return true,沒有return false
        for(int i=0; i < CodeList->Count; i++)
        {
                CCodeUnit* pCodeUnit = (CCodeUnit*)CodeList->Items[i];
                if(pCodeUnit->SerialNo == UnitSN)
                {
                        return pCodeUnit;
                }
        }
        return NULL;
}
//---------------------------------------------------------------------------
bool CCodeSet::Delete(int UnitSN)
{
//刪除特定Unit,input 為Unit的SerialNo
        for(int i=0; i < CodeList->Count; i++)
        {
                CCodeUnit* pCodeUnit = (CCodeUnit*)CodeList->Items[i];
                if(pCodeUnit->SerialNo == UnitSN)
                {
                        delete pCodeUnit;
                        CodeList->Delete(i);
                        return true;
                }
        }
        return false;
}
bool CCodeSet::Clear()
{
//清除所有Unit
        for(int i=0; i < CodeList->Count; i++)
        {
                CCodeUnit* pCodeUnit = (CCodeUnit*)CodeList->Items[i];
                delete pCodeUnit;
        }
        CodeList->Clear();
        return true;
}
