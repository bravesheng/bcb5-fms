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
        int SerialNo;                   //�y����
        AnsiString Code;                //�N��(A-Z)
        AnsiString Explanation;         //����
};
class CCodeSet
{
public:
        CCodeSet();
        ~CCodeSet();
        TList* CodeList;
        bool Add(CCodeUnit* UnitStruct);        //�s�W�S�wUnit
        CCodeUnit* FindCodeUnit(int UnitSN);              //�ˬd�O�_�]�t�S�wUnit
        bool Delete(int UnitSN);                //�R���S�wUnit
        bool Clear();                           //�M���Ҧ�Unit
};
//---------------------------------------------------------------------------
class CCarInfo
{
public:
        CCarInfo(void* Parent_Point); //Create function
        void* pParent;                //���V�W�@�h
        int SerialNo;                   //�y����
        AnsiString CarID;               //����
        AnsiString LocatorNo;           //�������X
        int SMPP_TON;                   //SMPP_TON
        int SMPP_NPI;                   //SMPP_NPI
        AnsiString LocatorPwd;          //�����K�X
        int TypeCode;                   //��������
        int GroupCode;                  //�s�դ���
        int RouteNo;                    //���\���|�s��
        int Image;                      //�����v����
        int DefaultIO;                  //�����w�]�ʱ�IO
        TDateTime UpdateTime;           //tCarInfo���̫��s�ɶ�,�ΥH�P�_�O�_�n��Database���s���

};
class CGroup
{
public:
        CGroup(void* Parent_Point);//Crerate function
        ~CGroup();
        void* pParent;             //���V�W�@�h
        int SerialNo;                   //�s�էǸ�
        AnsiString GrID;                //�s��ID
        AnsiString FullName;            //�s�զW��
        TList* CarInfoList;             //���ݨ���List
        void __fastcall RenewList();
        CCarInfo* FindCarByCarID(AnsiString CarID);
};
class CCustomer
{
public:
        CCustomer(void* Parent_Point);
        ~CCustomer();
        void* pParent;         //���V�W�@�h
        int SerialNo;                   //�Ȥ�y����
        AnsiString CoID;                //�Ȥ�ID
        AnsiString FullName;            //�Ȥ�W��
        TList* GroupList;               //�l�s��List
        void __fastcall RenewList();    //��s�s��List
        CCarInfo* FindCarByCarID(AnsiString CarID);
        CCodeSet* GroupCodes;           //����GroupCode���X
        CCodeSet* TypeCodes;            //����TypeCode���X
        bool __fastcall RenewTypeGroupSet();         //��sTypeCude and GroupCode Set
};
class CCustomerList        //�Ȥ�List
{
public:
        CCustomerList();
        ~CCustomerList();
        TList* List;                    //�Ȥ�List
        void __fastcall RenewList();    //��s�Ȥ�List
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
        CCustomerList* CustomerList;               //�Ȥ�}�C
        void __fastcall Change_Car_List_Visible();        ////�N����������i��令�i����,����������令���i����
        bool __fastcall Get_Remarks_From_DB();         //�Ѹ�ƮwŪ���Ƶ��a�I���
};
//---------------------------------------------------------------------------
extern PACKAGE TForm_ChooseCar *Form_ChooseCar;
//---------------------------------------------------------------------------
#endif
