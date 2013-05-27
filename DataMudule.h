//---------------------------------------------------------------------------

#ifndef DataMuduleH
#define DataMuduleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "GetTrackThread.h"
#include "RemarkPlace.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class CAccessControl
{
public:
    bool Locked;        //標記是否已經Lock
    CAccessControl();
    ~CAccessControl();
    bool Lock();        //執行Lock動作
    bool UnLock();      //執行UnLock動作
};

class TDataModule1 : public TDataModule
{
__published:	// IDE-managed Components
        TDatabase *Database_DCH;
        TQuery *Query_Update_tCustAutRoute;
        TQuery *Query_Route;
        TTable *Table_Cmd;
        TQuery *Query_DB;
        TTable *Table_Remarks;
        TQuery *Query_Update;
        TQuery *Query_tCustInfo;
        TQuery *Query_tCustGroup;
        TQuery *Query_tCarInfo;
        TTimer *Timer_Check_If_Establish_Connect;
        TDatabase *Database_TEMP;
        void __fastcall Timer_Check_If_Establish_ConnectTimer(
          TObject *Sender);
private:	// User declarations

public:		// User declarations
        bool bEstablishConnect;         //用以表示是否正在進行連線的動作
        CAccessControl bCanAccess;                //用以表示資料庫是可用,已完成Critical Section 功能
        CAccessControl bCanAccess2;               //同上功能,用於DataModule1自行使用時
        __fastcall TDataModule1(TComponent* Owner);
        TGetTrackThread* DB_Check_Thread;
        void __fastcall Update_DB_Value(AnsiString TableName,AnsiString FieldName,Variant FieldValue,AnsiString ConditionName,Variant ConditionValue);
        AnsiString __fastcall Get_DB_Value(AnsiString TableName,AnsiString FieldName,AnsiString ConditionName,AnsiString ConditionValue);
        void __fastcall Delete_DB_Value(AnsiString TableName,AnsiString ConditionName,AnsiString ConditionValue);
        void __fastcall QuestRoute();      //vincent:讀取tAutRoute資料
        void __fastcall  SendCmdToCar(sCmdMsg& Cmd, CCar* pCar);
        // Database Operate
        void ChkDatabase(TDatabase* pDB);               //檢查資料庫連線
        bool TryDB(TDatabase* pDB);              //嘗試連線資料庫
        __fastcall Update_Table_Remarks(CRemarkPlace* pRemark);         //新增備註地點至資料庫,成功時回傳1,失敗回傳0
        void __fastcall EstablishConnect(int Type);          //進行資料庫偵測,並連線
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
#endif
