//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RouteEditorForm.h"
#include "MainForm.h"
#include "CustRoute.h"
#include "DataMudule.h"
#include "ChooseCarForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_RouteEditor *Form_RouteEditor;
//---------------------------------------------------------------------------
__fastcall TForm_RouteEditor::TForm_RouteEditor(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_RouteEditor::BitBtnOkClick(TObject *Sender)
{//Update the database tCustAutRoute & the memory data
        if(ComboBox_Company->ItemIndex <= 0)
        {
                DataModule1->Query_Update_tCustAutRoute->ParamByName("sCoID")->Value = "Common";
                pPath->CoID = "Common";
        }
        else
        {
                CCustomer* pCustomer = (CCustomer*)Form_ChooseCar->CustomerList->List->Items[ComboBox_Company->ItemIndex-1];
                DataModule1->Query_Update_tCustAutRoute->ParamByName("sCoID")->Value = pCustomer->CoID;
                pPath->CoID  = pCustomer->CoID;
        }
        DataModule1->Query_Update_tCustAutRoute->ParamByName("sNote")->Value = Edit_RouteName->Text;
        DataModule1->Query_Update_tCustAutRoute->ParamByName("sStartPoint")->Value = Edit_StartName->Text;
        DataModule1->Query_Update_tCustAutRoute->ParamByName("sEndPoint")->Value = Edit_EndName->Text;
        DataModule1->Query_Update_tCustAutRoute->ParamByName("iRadius")->Value = Edit_Radius->Text;
        DataModule1->Query_Update_tCustAutRoute->ParamByName("iRouteNo")->Value = RouteNo;
        DataModule1->Query_Update_tCustAutRoute->ExecSQL();
        pPath->PathName = Edit_RouteName->Text;
        pPath->StartPoint = Edit_StartName->Text;
        pPath->EndPoint = Edit_EndName->Text;
        pPath->Radius = Edit_Radius->Text.ToInt();
        //refresh the Database DBGrid
        if(Form_CustRoute->Visible)
        {
                Form_CustRoute->Query1->Active = false;
                Form_CustRoute->Query1->Active = true;
        }
        ReDrawImage(CallCenter_Main_Form->Image_Map,CallCenter_Main_Form->m_dScale);
}
//---------------------------------------------------------------------------
void __fastcall TForm_RouteEditor::Get_Value_EditMode()
{
        //ComboBox_Company加入公司名稱
        CCustomer* pCustomer;
        ComboBox_Company->Items->Clear();
        ComboBox_Company->Items->Add("通用路線");
        ComboBox_Company->ItemIndex = 0;
        for(int i=0; i < Form_ChooseCar->CustomerList->List->Count; i++)
        {
                pCustomer = (CCustomer*)Form_ChooseCar->CustomerList->List->Items[i];
                ComboBox_Company->Items->Add(pCustomer->FullName);
        }
        for(int i=0; i < CallCenter_Main_Form->m_pSelPathGroupList->Count; i++)
        {
                pPath = (TCSelectPath*)CallCenter_Main_Form->m_pSelPathGroupList->Items[i];
                RouteNo = Form_CustRoute->Query1->FieldByName("RouteNo")->AsInteger;
                if(pPath->RouteNo == RouteNo)
                {
                        //由CoID找出CompanyName
                        for(int i=0; i < Form_ChooseCar->CustomerList->List->Count; i++)
                        {
                                pCustomer = (CCustomer*)Form_ChooseCar->CustomerList->List->Items[i];
                                if(pCustomer->CoID == pPath->CoID)
                                {
                                        ComboBox_Company->ItemIndex = i+1;
                                        break;
                                }
                        }
                        //找出其他屬性
                        Edit_RouteNo->Text = Edit_RouteNo->Text + IntToStr(pPath->RouteNo);
                        Edit_RouteName->Text = pPath->PathName;
                        Edit_StartName->Text = pPath->StartPoint;
                        Edit_EndName->Text = pPath->EndPoint;
                        Edit_Radius->Text = pPath->Radius;
                        BitBtnOk->Enabled = false;
                        return;
                }
          }
}

//---------------------------------------------------------------------------
void __fastcall TForm_RouteEditor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Hide();
	Action = caFree;
        Form_RouteEditor = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm_RouteEditor::BitBtnCancelClick(TObject *Sender)
{
     Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm_RouteEditor::Edit_RouteNameChange(TObject *Sender)
{
     BitBtnOk->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm_RouteEditor::Edit_StartNameChange(TObject *Sender)
{
     BitBtnOk->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_RouteEditor::Edit_EndNameChange(TObject *Sender)
{
     BitBtnOk->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_RouteEditor::Edit_RadiusChange(TObject *Sender)
{
     BitBtnOk->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_RouteEditor::ComboBox_CompanyChange(TObject *Sender)
{
     BitBtnOk->Enabled = true;        
}
//---------------------------------------------------------------------------

