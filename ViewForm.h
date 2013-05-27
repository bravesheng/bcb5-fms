//---------------------------------------------------------------------------

#ifndef ViewFormH
#define ViewFormH
#define CAR_LOCATOR_NO 7
#define IO_STAT 3
#define CAR_REPORTRATE 6
#define CAR_SPEED 1
#define CAR_PLACE 0
#define CAR_LON 9
#define CAR_LAT 10
#define CAR_RECVTIME 11
#define CAR_SENDTIME 12
#define CAR_MILEAGE 13
#define REPORT_CHECK_RATE 14
#define GPSLOCK 15
#define CAR_TYPE 5
#define CAR_GROUP 4
#define CAR_IO_IN_WATCH 8
#define ROAD_NAME 2
#define RMK_NAME 0
#define RMK_CLASS 1
#define RMK_PHONE 2
#define RMK_ADDR 3
#define RMK_EMAIL 4
#define RMK_HTTP 5
#define RMK_MEMO 6
#define RMK_PATH 7
#define RMK_PTR 8
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <Buttons.hpp>
#include "RemarkPlace.h"
#include "ChooseCarForm.h"
//---------------------------------------------------------------------------
class TForm_ViewInfo : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet_Car;
        TTabSheet *TabSheet_Remark;
        TPanel *Panel1;
        TListView *ListView_Car;
        TListView *ListView_Remark;
        TSpeedButton *SpeedButton_ChooseCar;
        void __fastcall ListView_CarDblClick(TObject *Sender);
        void __fastcall ListView_RemarkColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall ListView_RemarkCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall ListView_RemarkDblClick(TObject *Sender);
        void __fastcall SpeedButton_ChooseCarClick(TObject *Sender);
        void __fastcall ListView_CarColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall ListView_CarCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall ListView_CarChange(TObject *Sender,
          TListItem *Item, TItemChange Change);
    void __fastcall FormStartDock(TObject *Sender,
          TDragDockObject *&DragObject);
    void __fastcall FormEndDock(TObject *Sender, TObject *Target, int X,
          int Y);
private:	// User declarations

public:		// User declaration
        AnsiString sViewAll;
        CCustomerList* CustomerList;               //«È¤á°}¦C
        __fastcall TForm_ViewInfo(TComponent* Owner);
        void __fastcall AddNewListView_Remarks(CRemarkPlace* pRemarks,TListView* pListView,int CountNum);
        TListItem* __fastcall GetCarItem(AnsiString CarID);
};


//---------------------------------------------------------------------------
extern PACKAGE TForm_ViewInfo *Form_ViewInfo;
//---------------------------------------------------------------------------
#endif
