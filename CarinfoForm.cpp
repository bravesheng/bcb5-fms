//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CarinfoForm.h"
#include "common.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_CarInfo *Form_CarInfo;
//---------------------------------------------------------------------------
__fastcall TForm_CarInfo::TForm_CarInfo(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm_CarInfo::FormShow(TObject *Sender)
{
        CCar* pCar = CallCenter_Main_Form->GetCarByCarID(ListView1->Items->Item[0]->SubItems->Strings[0]);
        AnsiString LocatorNo = pCar->GetLocatorNo();
        ListView1->Items->Item[1]->SubItems->Strings[0] = LocatorNo;
        TQuery* pQuery = new TQuery(Form_CarInfo);
        pQuery->DatabaseName = "DB_DCH";
        pQuery->SQL->Add("SELECT * FROM tParaHis");
        pQuery->SQL->Add("WHERE LocatorNo = :LocatorNo");
        pQuery->SQL->Add("ORDER BY ParaDate DESC, ParaTime DESC");
        pQuery->ParamByName("LocatorNo")->AsString = LocatorNo;
        try
        {
                pQuery->Active = true;
                if(pQuery->RecordCount != 0)
                {
                        ListView1->Items->Item[2]->SubItems->Strings[0] = pQuery->FieldByName("OdometerFactor")->AsString;
                        ListView1->Items->Item[3]->SubItems->Strings[0] = pQuery->FieldByName("CenterNo")->AsString;
                        ListView1->Items->Item[4]->SubItems->Strings[0] = pQuery->FieldByName("ReportRate")->AsString;
                        ListView1->Items->Item[5]->SubItems->Strings[0] = pQuery->FieldByName("ParaDate")->AsString;
                        ListView1->Items->Item[6]->SubItems->Strings[0] = pQuery->FieldByName("ParaTime")->AsString;
                }
                else
                {
                        ListView1->Items->Item[2]->SubItems->Strings[0] = "N/A";
                        ListView1->Items->Item[3]->SubItems->Strings[0] = "N/A";
                        ListView1->Items->Item[4]->SubItems->Strings[0] = "N/A";
                        ListView1->Items->Item[5]->SubItems->Strings[0] = "N/A";
                        ListView1->Items->Item[6]->SubItems->Strings[0] = "N/A";
                }
                pQuery->Active = false;
        }
        catch(...)
        {
        }
        delete pQuery;
}
//---------------------------------------------------------------------------

