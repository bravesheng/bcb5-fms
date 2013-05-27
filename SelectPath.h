//---------------------------------------------------------------------------

#ifndef SelectPathH
#define SelectPathH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#define DEF_PATH_WIDTH 100      //預設的指定路線監控路線半徑
//---------------------------------------------------------------------------
struct sSelectPoint		//指定路線的點
{
   long mX2;    //指定路線的點 - X 座標(WGS)
   long mY2;    //指定路線的點 - Y 座標(WGS)
};
//指定路線
class TCSelectPath : public TDataModule
{
__published:	// IDE-managed Components
        TQuery *Query_Add;
        TQuery *Query1;
        TQuery *Query_Delete;
        void __fastcall DataModuleDestroy(TObject *Sender);
        void __fastcall DataModuleCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCSelectPath(TComponent* Owner);
        int RouteNo;            //此路線的識別好號碼,唯一不會重複
        AnsiString CoID;        //此路線所屬Company ID
        AnsiString PathName;          //路線名稱註記
        int Radius;  		//指定路線的監控範圍(監控半徑)
        TList* m_pSelPointList;         //for List of  sSelectPoint
        AnsiString StartPoint;
        AnsiString EndPoint;
        bool Vertify;   //標記是否驗證過
        void __fastcall AddPoint(long mX2,long mY2);	        //加入WGS座標點
        void __fastcall ClearPoint();
        void __fastcall AddToDB(TObject *Sender);               //將線路加入資料庫tCusAutRoute
        void __fastcall DeleteFromDB(TObject *Sender);          //將線路從資料庫刪除
        void __fastcall QuestData(int RouteNo);        //讀取tAutRoute單一Route的資料,並回傳TCSelectPath


};
//---------------------------------------------------------------------------
extern PACKAGE TCSelectPath *CSelectPath;
//---------------------------------------------------------------------------
#endif
