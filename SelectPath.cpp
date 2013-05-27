//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectPath.h"
#include "MainForm.h"
#include "CustRoute.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCSelectPath *CSelectPath;
//---------------------------------------------------------------------------
__fastcall TCSelectPath::TCSelectPath(TComponent* Owner)
        : TDataModule(Owner)
{
        Vertify = true;
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::DataModuleDestroy(TObject *Sender)
{
	//先將設此路線的車子的指定路線指標設為 NULL
	CCar *ptrCar;
    for(int i=0;i<CallCenter_Main_Form->m_pCarList->Count;i++)
    {
    	ptrCar = (CCar*)(CallCenter_Main_Form->m_pCarList->Items[i]);
        if(ptrCar->RouteNo==RouteNo)
        {
    		ptrCar->RouteNo = 0;
        }
    }//for

	//Free Memory
    for (int j = 0; j < m_pSelPointList->Count; j++)
    	delete ((sSelectPoint*) m_pSelPointList->Items[j]);
    delete m_pSelPointList;
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::DataModuleCreate(TObject *Sender)
{
    PathName = "";
    Radius = DEF_PATH_WIDTH;  		//指定路線的監控範圍(半徑)
    m_pSelPointList = new TList();//free Memory at ~CSelectPath()        
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::AddPoint(long mX2,long mY2)
{//加入經緯度座標點
        sSelectPoint* pPoint = new sSelectPoint;
        pPoint->mX2 = mX2;
        pPoint->mY2 = mY2;
        m_pSelPointList->Add(pPoint);
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::ClearPoint()
{
    for (int j = 0; j < m_pSelPointList->Count; j++)
    	delete ((sSelectPoint*) m_pSelPointList->Items[j]);

    m_pSelPointList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::AddToDB(TObject *Sender)
{
        m_pSelPointList->First();
        //決定出RouteNo
        Query1->Close();
        Query1->SQL->Clear();
        Query1->SQL->Add("SELECT RouteNo FROM tCustAutRoute ORDER BY RouteNo DESC");
        Query1->Open();
        RouteNo = Query1->FieldByName("RouteNo")->AsInteger;
        RouteNo++;
        sSelectPoint* pPoint;
        //將各點資料依序加入資料庫
        for(int i=0; i<m_pSelPointList->Count; i++)
        {
                pPoint = (sSelectPoint*)m_pSelPointList->Items[i];
                Query_Add->ParamByName("RouteNo")->AsInteger = RouteNo;
                Query_Add->ParamByName("PtSerialNo")->AsInteger = i;
                if(i==0)
                {
                        Query_Add->ParamByName("CoID")->AsString = CoID;
                        Query_Add->ParamByName("Note")->AsString = PathName;
                        Query_Add->ParamByName("StartPoint")->AsString = StartPoint;
                        Query_Add->ParamByName("EndPoint")->AsString = EndPoint;
                }
                else
                {
                   Query_Add->ParamByName("Note")->AsString = "";
                   Query_Add->ParamByName("StartPoint")->AsString = "";
                   Query_Add->ParamByName("EndPoint")->AsString = "";
                }
                Query_Add->ParamByName("Lon")->AsInteger = pPoint->mX2;
                Query_Add->ParamByName("Lat")->AsInteger = pPoint->mY2;
                Query_Add->ParamByName("Radius")->AsInteger = Radius;
                Query_Add->ExecSQL();
        }
        if(Form_CustRoute->Visible)   //假如必要則更新畫面
        {
                Form_CustRoute->Refresh(Sender);
        }
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::DeleteFromDB(TObject *Sender)
{
        Query_Delete->ParamByName("RouteNo")->AsInteger = RouteNo;
        Query_Delete->ExecSQL();
        //將使用該路徑的CarInfo設成沒有指定路徑
        TQuery* pQ_CarInfo = new TQuery(CSelectPath);      //delete in last line
        pQ_CarInfo->DatabaseName = "DB_DCH";
        pQ_CarInfo->SQL->Add("UPDATE tCarInfo");
        pQ_CarInfo->SQL->Add("SET RouteNo = 0");
        pQ_CarInfo->SQL->Add("WHERE RouteNo = :RouteNo");
        pQ_CarInfo->ParamByName("RouteNo")->AsInteger = RouteNo;
        try
        {
                pQ_CarInfo->ExecSQL();
        }
        catch(...)
        {
        }
        delete pQ_CarInfo;
        if(Form_CustRoute->Visible)   //假如必要則更新畫面
        {
                Form_CustRoute->Refresh(Sender);
        }
}
//---------------------------------------------------------------------------
void __fastcall TCSelectPath::QuestData(int RouteNo)
{
//由RouteNo至Database取得單一個Route information,並return TCSelectPath
        int Lon = 0;
        int Lat = 0;
        TQuery* pQ_Route = new TQuery(DataModule1);   //delete in last line
        pQ_Route->DatabaseName = "DB_DCH";
        pQ_Route->SQL->Add("SELECT * FROM tCustAutRoute");
        pQ_Route->SQL->Add("WHERE RouteNo = :RouteNo");
        pQ_Route->SQL->Add("ORDER BY PtSerialNo");
        pQ_Route->ParamByName("RouteNo")->AsInteger = RouteNo;
        try
        {
                pQ_Route->Active = false;
                pQ_Route->Active = true;
                RouteNo = RouteNo;
                CoID = pQ_Route->FieldByName("CoID")->AsString;
                PathName = pQ_Route->FieldByName("Note")->AsString;
                Radius = pQ_Route->FieldByName("Radius")->AsInteger;
                StartPoint = pQ_Route->FieldByName("StartPoint")->AsString;
                EndPoint = pQ_Route->FieldByName("EndPoint")->AsString;
                for(pQ_Route->First(); !pQ_Route->Eof; pQ_Route->Next())
                {
                        Lon = pQ_Route->FieldByName("Lon")->AsInteger;
                        Lat = pQ_Route->FieldByName("Lat")->AsInteger;
                        AddPoint(Lon,Lat);
                }
                pQ_Route->Active = false;
        }
        catch(EDBEngineError &E)
        {
                DataModule1->EstablishConnect(1);
        }
        catch(...)
        {
        }
        delete pQ_Route;
}

