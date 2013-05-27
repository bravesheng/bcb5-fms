//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HisTracer.h"
#include "DataMudule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCHisTracer *CHisTracer;
//---------------------------------------------------------------------------
__fastcall TCHisTracer::TCHisTracer(TComponent* Owner)
        : TDataModule(Owner)
{
        TIniFile* ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
        AnsiString Str = ini->ReadString("HIS_TRACER","Max_Trace_Point","2000");
        Max_Trace_Point = Str.ToInt();
}
//---------------------------------------------------------------------------
void TCHisTracer::Execute(void)    //Execite the trace and draw to image
{
   if(!QuestData())
      ShowMessage("資料過多無法處理，請縮小範圍！");
}
//---------------------------------------------------------------------------
int TCHisTracer::QuestData(void)       //to quest data from SQL
{
        AnsiString str;
        sCarStat Temp;

   //Query the data from the table tStatusHis in Database Server.
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Add("SELECT  *");
        Query1->SQL->Add("FROM  tStatusHis");
        Query1->SQL->Add("WHERE  LocatorNo = :MobiNum");
        Query1->SQL->Add("AND Cancel <> 1");          //Cancel==1的值為刪除的值
        Query1->SQL->Add("AND  SendDate BETWEEN :StartDate AND :EndDate");
        Query1->SQL->Add("AND NOT(  (SendDate = :StartDate)  AND  (SendTime < :StartTime)  )");
        Query1->SQL->Add("   AND NOT(  (SendDate = :EndDate)  AND  (SendTime > :EndTime)  ) ");
        Query1->SQL->Add("ORDER BY SendDate, SendTime");
        Query1->ParamByName("MobiNum")->AsString = LocatorNo;
        Query1->ParamByName("StartDate")->AsString = StartDate;
        Query1->ParamByName("EndDate")->AsString = EndDate;
        Query1->ParamByName("StartTime")->AsString = StartTime;
        Query1->ParamByName("EndTime")->AsString = EndTime;
        if(Query1->Prepared==false)
                Query1->Prepare();
        try
        {
                Query1->Open();
        }
        catch(...)
        {
                DataModule1->EstablishConnect(1);
        }
        VectorCarStat.clear();
        if(Query1->RecordCount > Max_Trace_Point)
                return(0);
        //Move the car information to VectorCarStatus.
        TQuery* pQuery = new TQuery(this);
        pQuery->DatabaseName = "DB_DCH";
        for(int i=0; !Query1->Eof; Query1->Next())
        {
                strcpy(Temp.m_szLocatorNo,Query1->FieldByName("LocatorNo")->AsString.c_str());
                strcpy(Temp.m_szSendDate,Query1->FieldByName("SendDate")->AsString.c_str());
                strcpy(Temp.m_szSendTime,Query1->FieldByName("SendTime")->AsString.c_str());
                int DistID = Query1->FieldByName("DistID")->AsInteger;
                Temp.m_iStatus = Query1->FieldByName("CarStatus")->AsVariant;
                Temp.m_iLon = Query1->FieldByName("Lon")->AsVariant;
                Temp.m_iLat = Query1->FieldByName("Lat")->AsVariant;
                Temp.m_sSpeed = Query1->FieldByName("Speed")->AsVariant;
                Temp.GPSLock = Query1->FieldByName("GPSLock")->AsBoolean;
                Temp.m_szPlaceName = Query1->FieldByName("PlaceName")->AsString;
                Temp.m_dMileage = Query1->FieldByName("Mileage")->AsVariant;
                //取得行政區中文名稱
                pQuery->SQL->Clear();
                pQuery->SQL->Add("SELECT Cname FROM tArea");
                pQuery->SQL->Add("WHERE AreaID = :DistID");
                pQuery->ParamByName("DistID")->AsInteger = DistID;
                pQuery->Active = false;
                pQuery->Active = true;
                Temp.DistName = pQuery->FieldByName("Cname")->AsString;
                pQuery->Active = false;
                VectorCarStat.push_back(Temp);     //加入
                i++;
        }
        delete pQuery;
        return(1);
}


