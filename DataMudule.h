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
    bool Locked;        //�аO�O�_�w�gLock
    CAccessControl();
    ~CAccessControl();
    bool Lock();        //����Lock�ʧ@
    bool UnLock();      //����UnLock�ʧ@
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
        bool bEstablishConnect;         //�ΥH��ܬO�_���b�i��s�u���ʧ@
        CAccessControl bCanAccess;                //�ΥH��ܸ�Ʈw�O�i��,�w����Critical Section �\��
        CAccessControl bCanAccess2;               //�P�W�\��,�Ω�DataModule1�ۦ�ϥή�
        __fastcall TDataModule1(TComponent* Owner);
        TGetTrackThread* DB_Check_Thread;
        void __fastcall Update_DB_Value(AnsiString TableName,AnsiString FieldName,Variant FieldValue,AnsiString ConditionName,Variant ConditionValue);
        AnsiString __fastcall Get_DB_Value(AnsiString TableName,AnsiString FieldName,AnsiString ConditionName,AnsiString ConditionValue);
        void __fastcall Delete_DB_Value(AnsiString TableName,AnsiString ConditionName,AnsiString ConditionValue);
        void __fastcall QuestRoute();      //vincent:Ū��tAutRoute���
        void __fastcall  SendCmdToCar(sCmdMsg& Cmd, CCar* pCar);
        // Database Operate
        void ChkDatabase(TDatabase* pDB);               //�ˬd��Ʈw�s�u
        bool TryDB(TDatabase* pDB);              //���ճs�u��Ʈw
        __fastcall Update_Table_Remarks(CRemarkPlace* pRemark);         //�s�W�Ƶ��a�I�ܸ�Ʈw,���\�ɦ^��1,���Ѧ^��0
        void __fastcall EstablishConnect(int Type);          //�i���Ʈw����,�ós�u
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
#endif
