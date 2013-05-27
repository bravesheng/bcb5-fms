//---------------------------------------------------------------------------

#ifndef GetTrackThreadH
#define GetTrackThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "common.h"

class TGetTrackThread : public TThread
{
private:
	void __fastcall GetLatestStatus();
        void __fastcall Determine_Car_Image_Color();    //決定車輛顏色
        void __fastcall DiagnoseConnect();
        void __fastcall OpenDBDetecotr();
        void __fastcall CarStatDetect();        //偵測車輛是否有違規情形
        bool __fastcall ShowReportEXE(char* szClassName,char* szWndName);
        bool __fastcall CloseReportEXE(char* szClassName,char* szWndName);
        void __fastcall Check_Acknowledgement();
        void __fastcall Check_If_Update_From_DB();         //檢查是否需要由資料庫更新目前資料
        void __fastcall Sync_Route_List();              //維護指定路線List資料,使之與Database同步
        void __fastcall ProcessSound();         //處理警告聲音發生時間
protected:
	void __fastcall Execute();
public:
        bool bCanTerminate;      //是否可以停止thread
        __fastcall TGetTrackThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
