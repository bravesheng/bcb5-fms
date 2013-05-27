//---------------------------------------------------------------------------

#ifndef HisTracerH
#define HisTracerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <vector.h>
//---------------------------------------------------------------------------


class TCHisTracer : public TDataModule
{
__published:	// IDE-managed Components
        TQuery *Query1;
private:	// User declarations
   int QuestData();         //to quest data from SQL
public:		// User declarations
        int Max_Trace_Point;
        __fastcall TCHisTracer(TComponent* Owner);
   struct sCarStat
   {
        AnsiString DistName;      //所屬行政區名稱
      char   m_szLocatorNo[21];
      char   m_szRecvDate[10];
      char   m_szRecvTime[10];
      char   m_szSendDate[10];
      char   m_szSendTime[10];
      int    m_iStatus;
      int    m_iLon;
      int    m_iLat;
      short  m_sSpeed;
      AnsiString   m_szPlaceName;
      int    m_iDistID;
      double m_dMileage;       //單位公里
      int    GPSLock;
   };
   vector<sCarStat> VectorCarStat;     //vector to store car status!!
   AnsiString LocatorNo;          //What car want to trace
   AnsiString StartDate;         //The start date to trace
   AnsiString StartTime;         //The start time to trace
   AnsiString EndDate;           //The end date to trace
   AnsiString EndTime;           //The end time to trace
   void Execute();             //Execite the trace and draw to image
};
//---------------------------------------------------------------------------
extern PACKAGE TCHisTracer *CHisTracer;
//---------------------------------------------------------------------------
#endif
