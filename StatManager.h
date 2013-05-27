//---------------------------------------------------------------------------

#ifndef StatManagerH
#define StatManagerH
#define EVENTTYPE 0
#define DESCRIPT 1
#define HANDLER 2
#define SENDDATE 3
#define SENDTIME 4
#define CARSN 5
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm_ManageState : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *PopupMenu1;
        TMenuItem *N_Handle;
        TMenuItem *N_Position;
        TMenuItem *N_Reset;
        TImageList *ImageList1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet_RT;
        TListView *ListView1;
        TTabSheet *TabSheet_HIS;
        TDBGrid *DBGrid1;
        TDataSource *DataSource1;
        TQuery *Query_EventHis;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall N_HandleClick(TObject *Sender);
        void __fastcall N_PositionClick(TObject *Sender);
        void __fastcall N_ResetClick(TObject *Sender);
        void __fastcall PopupMenu1Popup(TObject *Sender);
        void __fastcall ListView1DblClick(TObject *Sender);
        void __fastcall PageControl1Change(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
    void __fastcall FormStartDock(TObject *Sender,
          TDragDockObject *&DragObject);
    void __fastcall FormEndDock(TObject *Sender, TObject *Target, int X,
          int Y);
private:	// User declarations

public:		// User declarations
        __fastcall TForm_ManageState(TComponent* Owner);
        void __fastcall ImportEvent(int CarSN, AnsiString CarID, int EventType, AnsiString Descript, AnsiString Handler, AnsiString SendDate, AnsiString SendTime);
        void __fastcall Delete_Event_From_DB(AnsiString CarID, int EventType);   //將以解決的Event自資料庫刪除
        int __fastcall FindEvent(AnsiString CarID, int EventType);      //找出特定的狀況
        void __fastcall Send_Cmd_Automatic(int EventType, AnsiString CarID);
        bool __fastcall ProcessSound();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ManageState *Form_ManageState;
//---------------------------------------------------------------------------
#endif
