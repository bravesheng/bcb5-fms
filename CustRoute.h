//---------------------------------------------------------------------------

#ifndef CustRouteH
#define CustRouteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include "SelectPath.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
class TForm_CustRoute : public TForm
{
__published:	// IDE-managed Components
        TDBGrid *DBGrid1;
        TDataSource *DataSource1;
        TQuery *Query1;
        TStatusBar *StatusBar1;
        TPageControl *PageControl1;
        TTabSheet *Tab_Admin;
        TTabSheet *Tab_Assign;
        TToolBar *ToolBar1;
        TToolButton *Button_Add;
        TToolButton *Button_Delete;
        TToolButton *Button_Update;
        TBitBtn *BindBtn;
        TBitBtn *CancelBtn;
        TLabel *Label1;
        TQuery *Query_CarList;
        TEdit *Edit1;
        TComboBox *ComboBox1;
        TQuery *Query_UpdateCarInfo;
        TImageList *ImageList1;
        TToolButton *ToolButton1;
        TPanel *Panel1;
        TLabel *Label2;
        TComboBox *ComboBox_Company;
        void __fastcall DBGrid1DblClick(TObject *Sender);
        void __fastcall Button_AddClick(TObject *Sender);
        void __fastcall Button_DeleteClick(TObject *Sender);
        void __fastcall ComboBox1Exit(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall BindBtnClick(TObject *Sender);
        void __fastcall DBGrid1CellClick(TColumn *Column);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button_UpdateClick(TObject *Sender);
        void __fastcall ComboBox_CompanyChange(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ComboBox1DropDown(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm_CustRoute(TComponent* Owner);
        void __fastcall Refresh(TObject *Sender);  //Just refresh the Query to let the Database Grid is newest


};
//---------------------------------------------------------------------------
extern PACKAGE TForm_CustRoute *Form_CustRoute;
//---------------------------------------------------------------------------
#endif
