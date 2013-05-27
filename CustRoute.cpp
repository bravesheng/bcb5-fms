//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CustRoute.h"
#include "common.h"
#include "RouteEditorForm.h"
#include "ViewForm.h"
#include "ChooseCarForm.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_CustRoute *Form_CustRoute;
//---------------------------------------------------------------------------
__fastcall TForm_CustRoute::TForm_CustRoute(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_CustRoute::DBGrid1DblClick(TObject *Sender)
{
        long mX2 = Query1->FieldByName("Lon")->AsInteger;
        long mY2 = Query1->FieldByName("Lat")->AsInteger;
        DrawImage(CallCenter_Main_Form->Image_Map,mX2,mY2,CallCenter_Main_Form->m_dScale);
        CallCenter_Main_Form->UpdateAllObjPos();
        //Set Center Line
        DrawCenterLine(CallCenter_Main_Form->Image_Map);
}
//---------------------------------------------------------------------------
void __fastcall TForm_CustRoute::Refresh(TObject *Sender)
{
        Query1->Close();
        Query1->Open();
}
//---------------------------------------------------------------------------
void __fastcall TForm_CustRoute::Button_AddClick(TObject *Sender)
{
     if(Button_Add->Tag == 9)       //if the tag is never changed
     {
        Button_Add->Tag = CallCenter_Main_Form->m_PointerType;
     }
     CallCenter_Main_Form->Menu_MapOp_DragMapClick(Sender);
     CallCenter_Main_Form->Menu_MapOp_SetPathClick(this);
     CallCenter_Main_Form->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm_CustRoute::Button_DeleteClick(TObject *Sender)
{
        if(::MessageBox(Handle,"�A�T�w�n�R���o�����w�ʱ����u�� ?","���w�ʱ����u",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)
    	        return;
        for(int i=0; i < CallCenter_Main_Form->m_pSelPathGroupList->Count; i++)
        {
                TCSelectPath *pPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                int RouteNo = Query1->FieldByName("RouteNo")->AsInteger;
                if(pPath->RouteNo == RouteNo)
                {
                        pPath->DeleteFromDB(Sender);
                        delete pPath;
                        CallCenter_Main_Form->m_pSelPathGroupList->Remove(pPath);
                        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
                }
          }
}
//---------------------------------------------------------------------------

void __fastcall TForm_CustRoute::ComboBox1Exit(TObject *Sender)
{
        Query_CarList->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_CustRoute::ComboBox1Change(TObject *Sender)
{
        Query_CarList->Open();
        Query_CarList->First();
        
        for(int i=0; i < Query_CarList->RecordCount; i++)
        {
                if(Query_CarList->FieldByName("CarID")->AsString == ComboBox1->Text)
                {
                        AnsiString tmp = Query_CarList->FieldByName("RouteNo")->AsString;
                        if((tmp == "")||(tmp == "0"))
                        {
                                Edit1->Text = "�ثe�S�����w���|.";
                                Edit1->Tag = 0;
                        }
                        else
                        {
                                Edit1->Text = "�ʱ����|���w����"
                                             +tmp
                                             + "�����|.";
                        }
                }
                Query_CarList->Next();
        }
        Query_CarList->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_CustRoute::CancelBtnClick(TObject *Sender)
{
        CCar *ptrCar;
        TCSelectPath* ptrPath;

                AnsiString strShow = "�A�T�w�n�M������: \"" + ComboBox1->Text + "\"\n���ʱ����u�ܡH";
                if(::MessageBox(Handle,strShow.c_str(),"�M��",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)
                        return;
                //��X����CCar������
                for(int i=0; i < CallCenter_Main_Form->m_pCarList->Count; i++)
                {
                        ptrCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
                        AnsiString CarID =  ptrCar->GetCarID();
                        if(ComboBox1->Text == CarID)
                        {//�N���T���l�P���u�s��
                                ptrCar->RouteNo = 0;
                                ptrCar->UpdateHint();      //vincent guess:�[�J�ثe��ƨ�Ө��l��tooltip
                                Query_UpdateCarInfo->Close();
                                Edit1->Text = "�ثe�S�����w���|.";
                                Edit1->Tag = 0;
                                Query_UpdateCarInfo->ParamByName("iRouteNo")->AsInteger = Edit1->Tag;    //��s��Ʈw
                                Query_UpdateCarInfo->ParamByName("sCarID")->AsString = ComboBox1->Text;
                                Query_UpdateCarInfo->ExecSQL();
                                return;
                        }
                }
}
//---------------------------------------------------------------------------
void __fastcall TForm_CustRoute::BindBtnClick(TObject *Sender)
{
        CCar *ptrCar;
        if(Edit1->Tag == 0)         //���p�ϥΪ̥��N�ȳ]�n
        {
                ShowMessage("�Х���ܫ��w�ʱ����u�κʱ������I");
                return;
        }
        else
        {
                AnsiString strShow = "�A�T�w�n������: \"" + ComboBox1->Text +AnsiString("\"\n�]�w���w�ʱ����u: \"")
                        +"��"+AnsiString(Edit1->Tag)+"��""\"���u�� ?";
                if(::MessageBox(Handle,strShow.c_str(),"���w�ʱ����u",MB_OKCANCEL|MB_ICONQUESTION)!=IDOK)
                        return;
                //��X����CCar������
                for(int i=0; i < CallCenter_Main_Form->m_pCarList->Count; i++)
                {
                        ptrCar = (CCar*)CallCenter_Main_Form->m_pCarList->Items[i];
                        AnsiString CarID =  ptrCar->GetCarID();
                        if(ComboBox1->Text == CarID)
                        {//�N���T���l�P���u�s��
                                ptrCar->RouteNo = Edit1->Tag;   //���wRouteNo������
                                ptrCar->UpdateHint();      //vincent guess:�[�J�ثe��ƨ�Ө��l��tooltip
                                Query_UpdateCarInfo->Close();
                                Query_UpdateCarInfo->ParamByName("iRouteNo")->AsInteger = Edit1->Tag;    //��s��Ʈw
                                Query_UpdateCarInfo->ParamByName("sCarID")->AsString = ComboBox1->Text;
                                Query_UpdateCarInfo->ExecSQL();
                                return;
                        }
                }
        }
}
//---------------------------------------------------------------------------





void __fastcall TForm_CustRoute::DBGrid1CellClick(TColumn *Column)
{
        if(ComboBox1->Text != "")
        {
                Edit1->Tag = Query1->FieldByName("RouteNo")->AsInteger;
                Edit1->Text = "�ʱ����|���w����";
                Edit1->Text = Edit1->Text + Edit1->Tag;
                Edit1->Text = Edit1->Text + "�����|.";
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm_CustRoute::FormClose(TObject *Sender,
      TCloseAction &Action)
{  //���s�W���u��]�w�ƹ����Ъ��A�^���e���A
   if(Button_Add->Tag == 0)
   {
      CallCenter_Main_Form->Menu_MapOp_PointerClick(Sender);
   }
   else if(Button_Add->Tag == 1)
   {
      CallCenter_Main_Form->Menu_MapOp_DragMapClick(Sender);
   }
   Button_Add->Tag = 9;
}
//---------------------------------------------------------------------------


void __fastcall TForm_CustRoute::Button_UpdateClick(TObject *Sender)
{
        if(!Form_RouteEditor)
                Form_RouteEditor = new TForm_RouteEditor(this);
        Form_RouteEditor->Get_Value_EditMode();
        Form_RouteEditor->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TForm_CustRoute::ComboBox_CompanyChange(TObject *Sender)
{
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Add("SELECT * FROM tCustAutRoute");
        Query1->SQL->Add("WHERE (PtSerialNo = 0)");
        if(ComboBox_Company->ItemIndex != 0)
        {
                CCustomer* pCustomer = (CCustomer*)Form_ChooseCar->CustomerList->List->Items[ComboBox_Company->ItemIndex-1];
                Query1->SQL->Add("AND (CoID IN ('Common','" + pCustomer->CoID + "'))");
        }
        Query1->SQL->Add("ORDER BY  RouteNo, PtSerialNo");
        Query1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TForm_CustRoute::FormShow(TObject *Sender)
{
        CCustomer* pCustomer;
        ComboBox_Company->Items->Clear();
        ComboBox_Company->Items->Add("�������");
        for(int i=0; i < Form_ChooseCar->CustomerList->List->Count; i++)
        {
                pCustomer = (CCustomer*)Form_ChooseCar->CustomerList->List->Items[i];
                ComboBox_Company->Items->Add(pCustomer->FullName);
        }
        ComboBox_Company->ItemIndex = 0;
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Add("SELECT * FROM tCustAutRoute");
        Query1->SQL->Add("WHERE (PtSerialNo = 0)");
        Query1->SQL->Add("ORDER BY  RouteNo, PtSerialNo");
        Query1->Open();
}
//---------------------------------------------------------------------------
void __fastcall TForm_CustRoute::ComboBox1DropDown(TObject *Sender)
{
        ComboBox1->Clear();
        for(int i=0; i < Form_ViewInfo->ListView_Car->Items->Count; i++)
        {
                ComboBox1->Items->Add(Form_ViewInfo->ListView_Car->Items->Item[i]->Caption);
        }
}
//---------------------------------------------------------------------------
