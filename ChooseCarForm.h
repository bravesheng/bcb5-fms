//---------------------------------------------------------------------------

#ifndef ChooseCarFormH
#define ChooseCarFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class CCodeUnit
{
public:
        int SerialNo;                   //流水號
        AnsiString Code;                //代號(A-Z)
        AnsiString Explanation;         //說明
};
class CCodeSet
{
public:
        CCodeSet();
        ~CCodeSet();
        TList* CodeList;
        bool Add(CCodeUnit* UnitStruct);        //新增特定Unit
        CCodeUnit* FindCodeUnit(int UnitSN);              //檢查是否包含特定Unit
        bool Delete(int UnitSN);                //刪除特定Unit
        bool Clear();                           //清除所有Unit
};
//---------------------------------------------------------------------------
class CCarInfo
{
public:
        CCarInfo(void* Parent_Point); //Create function
        void* pParent;                //指向上一層
        int SerialNo;                   //流水號
        AnsiString CarID;               //車號
        AnsiString LocatorNo;           //車機號碼
        int SMPP_TON;                   //SMPP_TON
        int SMPP_NPI;                   //SMPP_NPI
        AnsiString LocatorPwd;          //車機密碼
        int TypeCode;                   //型式分類
        int GroupCode;                  //群組分類
        int RouteNo;                    //允許路徑編號
        int Image;                      //車輛影像檔
        int DefaultIO;                  //車輛預設監控IO
        TDateTime UpdateTime;           //tCarInfo的最後更新時間,用以判斷是否要由Database取新資料

};
class CGroup
{
public:
        CGroup(void* Parent_Point);//Crerate function
        ~CGroup();
        void* pParent;             //指向上一層
        int SerialNo;                   //群組序號
        AnsiString GrID;                //群組ID
        AnsiString FullName;            //群組名稱
        TList* CarInfoList;             //所屬車輛List
        void __fastcall RenewList();
        CCarInfo* FindCarByCarID(AnsiString CarID);
};
class CCustomer
{
public:
        CCustomer(void* Parent_Point);
        ~CCustomer();
        void* pParent;         //指向上一層
        int SerialNo;                   //客戶流水號
        AnsiString CoID;                //客戶ID
        AnsiString FullName;            //客戶名稱
        TList* GroupList;               //子群組List
        void __fastcall RenewList();    //更新群組List
        CCarInfo* FindCarByCarID(AnsiString CarID);
        CCodeSet* GroupCodes;           //所屬GroupCode集合
        CCodeSet* TypeCodes;            //所屬TypeCode集合
        bool __fastcall RenewTypeGroupSet();         //更新TypeCude and GroupCode Set
};
class CCustomerList        //客戶List
{
public:
        CCustomerList();
        ~CCustomerList();
        TList* List;                    //客戶List
        void __fastcall RenewList();    //更新客戶List
        CCarInfo* FindCarByCarID(AnsiString CarID);
};

//---------------------------------------------------------------------------
class TForm_ChooseCar : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel2;
        TPanel *Panel1;
        TLabel *Label1;
        TComboBox *ComboBox_Company;
        TListView *ListView_Type;
        TListView *ListView_Group;
        TQuery *Query_CarType;
        TQuery *Query_CarGroup;
        TListView *ListView_Selected;
        TListView *ListView_NoSelect;
        TPanel *Panel3;
        TSpeedButton *SpeedButton_SingleGo;
        TSpeedButton *SpeedButton_AllGo;
        TSpeedButton *SpeedButton_SingleBack;
        TSpeedButton *SpeedButton_AllBack;
        TSpeedButton *SpeedButton_OK;
        TSplitter *Splitter1;
        TSplitter *Splitter2;
        void __fastcall ComboBox_CompanyDropDown(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ListView_GroupMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ListView_TypeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall SpeedButton_SingleGoClick(TObject *Sender);
        void __fastcall SpeedButton_AllGoClick(TObject *Sender);
        void __fastcall SpeedButton_SingleBackClick(TObject *Sender);
        void __fastcall SpeedButton_AllBackClick(TObject *Sender);
        void __fastcall SpeedButton_OKClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ComboBox_CompanyChange(TObject *Sender);
        void __fastcall ListView_NoSelectDblClick(TObject *Sender);
        void __fastcall ListView_SelectedDblClick(TObject *Sender);
private:	// User declarations
        AnsiString sViewAll;
public:		// User declarations
        __fastcall TForm_ChooseCar(TComponent* Owner);
        void __fastcall UpdateNonSelect();
        CCustomerList* CustomerList;               //客戶陣列
        void __fastcall Change_Car_List_Visible();        ////將選取之車輛進行改成可見的,未選取車輛改成不可見的
        bool __fastcall Get_Remarks_From_DB();         //由資料庫讀取備註地點資料
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ChooseCar *Form_ChooseCar;
//---------------------------------------------------------------------------
#endif
